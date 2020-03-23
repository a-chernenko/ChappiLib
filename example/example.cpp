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

template <typename ErrorType, ErrorType ErrorValue>
class chips_type final {
 public:
  using error_type = ErrorType;
  static const error_type no_error_v{ErrorValue};
  using ltc2991 = chappi::ltc2991<error_type, no_error_v>;
  using ina219 = chappi::ina219<error_type, no_error_v>;
  using adn4600 = chappi::adn4600<error_type, no_error_v>;
  using hmc987 = chappi::hmc987<error_type, no_error_v>;
  using tca6424 = chappi::tca6424<error_type, no_error_v>;
  using ad5621 = chappi::ad5621<error_type, no_error_v>;
  using si57x = chappi::si57x<error_type, no_error_v>;

  chips_type(bool logenable = false)
      : LTC2991{bool(logenable)},
        INA219{bool(logenable)},
        ADN4600{bool(logenable)},
        HMC987{bool(logenable)},
        TCA6424{bool(logenable)},
        AD5621{bool(logenable)},
        Si57x{bool(logenable)} {}
  ltc2991 LTC2991;
  ina219 INA219;
  adn4600 ADN4600;
  hmc987 HMC987;
  tca6424 TCA6424;
  ad5621 AD5621;
  si57x Si57x;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) try {
  using chips_type = chips_type<int, 0>;
  chips_type chips{true};
  chips_type::ltc2991::reg_read_fn read_LTC2991 =
      []([[maybe_unused]] chips_type::ltc2991::dev_addr_type dev_addr,
         [[maybe_unused]] chips_type::ltc2991::addr_type addr,
         [[maybe_unused]] chips_type::ltc2991::value_type &val) {
        // TODO: add device read
        return 0;
      };
  chips_type::ltc2991::reg_write_fn write_LTC2991 =
      []([[maybe_unused]] chips_type::ltc2991::dev_addr_type dev_addr,
         [[maybe_unused]] chips_type::ltc2991::addr_type addr,
         [[maybe_unused]] chips_type::ltc2991::value_type val) {
        // TODO: add device write
        return 0;
      };

  const int addr_LTC2991{0x49};
  chips.LTC2991.setup_io(read_LTC2991, write_LTC2991, addr_LTC2991);
  chips.LTC2991.repeated_mode(true);
  auto name = chips.LTC2991.get_name();
  auto temperature = chips.LTC2991.get_temperature();
  auto voltage_1 = chips.LTC2991.get_voltage_1();
  auto voltage_2 = chips.LTC2991.get_voltage_2();

  std::cout << name << '\n';
  std::cout << "temperature: " << temperature << '\n';
  std::cout << "voltage_1: " << voltage_1 << '\n';
  std::cout << "voltage_2: " << voltage_2 << '\n';

  return 0;
} catch (...) {
  std::cerr << "\nunexpected exception\n";
  return 0;
}
