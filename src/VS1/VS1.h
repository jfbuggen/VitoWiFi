/*
Copyright (c) 2023 Bert Melis. All rights reserved.

This work is licensed under the terms of the MIT license.  
For a copy, see <https://opensource.org/licenses/MIT> or
the LICENSE file.
*/

#pragma once

#include <functional>

#include "Logging.h"
#include "../Constants.h"
#include "../Helpers.h"
#include "PacketVS1.h"
#include "../Datapoint/Datapoint.h"
#if defined(VITOWIFI_GENERIC)
#include "../Interface/SerialInterface.h"
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
#include "../Interface/HardwareSerialInterface.h"
#if defined(ARDUINO_ARCH_ESP8266)
#include "../Interface/SoftwareSerialInterface.h"
#endif
#elif defined(__linux__)
#include "../Interface/LinuxSerialInterface.h"
#else
#error "platform not supported"
#endif

namespace VitoWiFi {

class VS1 {
 public:
  typedef std::function<void(const uint8_t* data, uint8_t length, const Datapoint& request)> OnResponseCallback;
  typedef std::function<void(OptolinkResult error, const Datapoint& request)> OnErrorCallback;

  #if defined(VITOWIFI_GENERIC)
  explicit VS1(VitoWiFiInternals::SerialInterface *interface);
  #elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
  explicit VS1(HardwareSerial* interface);
  #if defined(ARDUINO_ARCH_ESP8266)
  explicit VS1(SoftwareSerial* interface);
  #endif
  #else
  explicit VS1(const char* interface);
  #endif
  ~VS1();
  VS1(const VS1&) = delete;
  VS1 & operator=(const VS1&) = delete;

  void onResponse(OnResponseCallback callback);
  void onError(OnErrorCallback callback);

  bool read(const Datapoint& datapoint);
  bool write(const Datapoint& datapoint, const VariantValue& value);
  bool write(const Datapoint& datapoint, const uint8_t* data, uint8_t length);

  bool begin();
  void loop();
  void end();

 private:
  enum class State {
    INIT,
    SYNC_ENQ,
    SYNC_RECV,
    SEND,
    RECEIVE,
    UNDEFINED
  } _state;
  uint32_t _currentMillis;
  uint32_t _lastMillis;
  uint32_t _requestTime;
  uint8_t _bytesTransferred;
  VitoWiFiInternals::SerialInterface* _interface;
  Datapoint _currentDatapoint;
  PacketVS1 _currentRequest;
  uint8_t* _responseBuffer;
  uint8_t _allocatedLength;
  OnResponseCallback _onResponseCallback;
  OnErrorCallback _onErrorCallback;

  inline void _setState(State state);

  void _init();
  void _syncEnq();
  void _syncRecv();
  void _send();
  void _receive();

  void _tryOnResponse();
  void _tryOnError(OptolinkResult result);

  bool _expandResponseBuffer(uint8_t newSize);
};

}  // end namespace VitoWiFi
