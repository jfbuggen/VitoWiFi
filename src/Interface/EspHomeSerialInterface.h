/*
Copyright (c) 2023 Bert Melis. All rights reserved.

This work is licensed under the terms of the MIT license.  
For a copy, see <https://opensource.org/licenses/MIT> or
the LICENSE file.
*/

#pragma once

#if defined(USE_ESP32)  // This is just a way to detect EspHome environment

#include <cassert>

#include <esphome.h>
#include "SerialInterface.h"

namespace VitoWiFiInternals {

class EspHomeSerialInterface : public SerialInterface {
 public:
  explicit EspHomeSerialInterface(UARTComponent* interface);
  bool begin();
  void end();
  std::size_t write(const uint8_t* data, uint8_t length) override;
  uint8_t read() override;
  size_t available() override;

 private:
  UARTComponent* _interface;
};

}  // end namespace VitoWiFiInternals

#endif
