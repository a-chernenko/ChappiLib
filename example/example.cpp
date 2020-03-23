/*

   Copyright 2018 Alexander Chernenko (achernenko@mail.ru)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

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
      : LTC2991{bool(false & logenable)},
        INA219{bool(false & logenable)},
        ADN4600{bool(false & logenable)},
        HMC987{bool(false & logenable)},
        TCA6424{bool(false & logenable)},
        AD5621{bool(false & logenable)},
        Si57x{bool(false & logenable)} {}
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
