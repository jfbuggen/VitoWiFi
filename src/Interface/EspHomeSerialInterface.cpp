/*
Copyright (c) 2023 Bert Melis. All rights reserved.

This work is licensed under the terms of the MIT license.  
For a copy, see <https://opensource.org/licenses/MIT> or
the LICENSE file.
*/

#if defined(ESPHOME_VARIANT)

#include "EspHomeSerialInterface.h"

namespace VitoWiFiInternals {
EspHomeSerialInterface::EspHomeSerialInterface(UARTComponent* interface)
: _interface(interface) {
  assert(interface);
}

bool EspHomeSerialInterface::begin() {
  _interface->set_baud_rate(4800);
  _interface->set_data_bits(8);
  _interface->set_partity(UART_CONFIG_PARITY_EVEN);
  _interface->set_stop_bits(2);
  _interface->setup();
  return (*_interface);
}

void EspHomeSerialInterface::end() {
  _interface->end();
}

std::size_t EspHomeSerialInterface::write(const uint8_t* data, uint8_t length) {
  return _interface->write(data, length);
}

uint8_t EspHomeSerialInterface::read() {
  uint8_t retVal = 0;
  _interface->read(&retVal, 1);
  return retVal;
}

size_t EspHomeSerialInterface::available() {
  return _interface->available();
}

}  // end namespace VitoWiFiInternals

#endif
