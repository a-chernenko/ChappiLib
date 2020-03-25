/*

MIT License

Copyright (c) 2019 Alexander Chernenko (achernenko@mail.ru)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <iostream>
#include "chappi.h"

using namespace chappi;

using error_type = int;
static const error_type no_error_v{error_type{0}};

using ltc2991_type = chappi::ltc2991<error_type, no_error_v>;
using ina219_type = chappi::ina219<error_type, no_error_v>;
using adn4600_type = chappi::adn4600<error_type, no_error_v>;
using hmc987_type = chappi::hmc987<error_type, no_error_v>;
using tca6424_type = chappi::tca6424<error_type, no_error_v>;
using ad5621_type = chappi::ad5621<error_type, no_error_v>;
using si57x_type = chappi::si57x<error_type, no_error_v>;

static ltc2991_type LTC2991{true};
static ina219_type INA219{true};
static adn4600_type ADN4600{true};
static hmc987_type HMC987{true};
static tca6424_type TCA6424{true};
static ad5621_type AD5621{true};
static si57x_type Si57x{true};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) try {
  ltc2991_type::reg_read_fn read_LTC2991 =
      []([[maybe_unused]] ltc2991_type::dev_addr_type dev_addr,
         [[maybe_unused]] ltc2991_type::addr_type addr,
         [[maybe_unused]] ltc2991_type::value_type &val) {
        // TODO: add device read
        return 0;
      };
  ltc2991_type::reg_write_fn write_LTC2991 =
      []([[maybe_unused]] ltc2991_type::dev_addr_type dev_addr,
         [[maybe_unused]] ltc2991_type::addr_type addr,
         [[maybe_unused]] ltc2991_type::value_type val) {
        // TODO: add device write
        return 0;
      };

  const int addr_LTC2991{0x49};
  LTC2991.setup_io(read_LTC2991, write_LTC2991, addr_LTC2991);
  LTC2991.repeated_mode(true);
  auto name = LTC2991.get_name();
  auto temperature = LTC2991.get_temperature();
  auto voltage_1 = LTC2991.get_voltage_1();
  auto voltage_2 = LTC2991.get_voltage_2();

  std::cout << name << '\n';
  std::cout << "temperature: " << temperature << '\n';
  std::cout << "voltage_1: " << voltage_1 << '\n';
  std::cout << "voltage_2: " << voltage_2 << '\n';

  return 0;
} catch (...) {
  std::cerr << "\nunexpected exception\n";
  return 0;
}
