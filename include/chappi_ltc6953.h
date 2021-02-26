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

#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <thread>

#ifdef _MSC_VER
#if _MSVC_LANG < 201704
#error \
    "This file requires compiler and library support for the ISO C++ 2020 standard or later."
#endif
#endif

#include "chappi_base.h"
#include "chappi_register.h"

namespace chappi {

namespace ltc6953_constants {}  // namespace ltc6953_constants

namespace ltc6953_registers {

#pragma pack(push, 1)

using register_type = uint8_t;

template <typename ValueType>
constexpr register_type register_to_integer(ValueType value) noexcept {
  return static_cast<std::underlying_type_t<ValueType>>(value);
}

template <typename register_bits_type>
union register_basic {
  register_type reg{};
  register_bits_type bits;
};

struct register_common {
  const register_type D0 : 1;
  const register_type D1 : 1;
  const register_type D2 : 1;
  const register_type D3 : 1;
  const register_type D4 : 1;
  const register_type D5 : 1;
  const register_type D6 : 1;
  const register_type D7 : 1;
};

// TODO: add enums
// TODO: add registers

struct register_h00 {
  const register_type D0 : 1;
  const register_type D1 : 1;
  register_type VCOOK : 1;
  register_type nVCOOK : 1;
  const register_type D4 : 1;
  const register_type D5 : 1;
  const register_type D6 : 1;
  const register_type D7 : 1;
};

struct register_h02 {
  register_type POR : 1;
  register_type FILTV : 1;
  const register_type D2 : 1;
  const register_type D3 : 1;
  const register_type D4 : 1;
  register_type PDVCOPK : 1;
  const register_type D6 : 1;
  register_type PDALL : 1;
};

namespace detail {

struct registers_map {
  register_basic<register_h00> reg_h00{};
  register_basic<register_h02> reg_h02{};
  register_basic<register_common> reg_h06{};
  register_basic<register_common> reg_h07{};
  register_basic<register_common> reg_h08{};
  register_basic<register_common> reg_h09{};
  register_basic<register_common> reg_h0A{};
};

struct registers_range {
  int begin{};
  int end{};
};

}  // namespace detail

const int register_max_num{sizeof(detail::registers_map) /
                           sizeof(register_type)};
const detail::registers_range registers_range_common{0, 0x38};

namespace detail {

union {
  const registers_map regs{.reg_h00{.bits{.D0 = 1, .D5 = 1}},
                           .reg_h02{.bits{.D3 = 1}},
                           .reg_h06{.reg{}},
                           .reg_h07{.reg{}},
                           .reg_h08{.reg{}},
                           .reg_h09{.reg{}},
                           .reg_h0A{.reg{}}};
  const register_type array[register_max_num];
} const registers_map_defaults{};

}  // namespace detail

union registers_map {
  detail::registers_map regs{detail::registers_map_defaults.regs};
  register_type array[register_max_num];
};

using registers_update =
    ::chappi::registers_update<ltc6953_registers::register_max_num>;

#pragma pack(pop)

}  // namespace ltc6953_registers

namespace detail {
struct ltc6953_counter {
  chips_counter<ltc6953_counter> data;
};
}  // namespace detail

template <typename ErrorType = int, ErrorType NoerrorValue = 0,
          typename DevAddrType = uint8_t, typename AddrType = uint8_t,
          typename ValueType = uint8_t>
class ltc6953 final : public chip_base<ErrorType, NoerrorValue, DevAddrType,
                                       AddrType, ValueType> {
  static constexpr auto _chip_name = "LTC6953";
  detail::ltc6953_counter _counter;

  mutable ltc6953_registers::registers_map _registers_map{};
  mutable ltc6953_registers::registers_update _registers_update{};
  mutable bool _is_integer_mode{};

 public:
  CHIP_BASE_RESOLVE

  ltc6953(bool log_enable)
      : ltc6953{(log_enable) ? std::clog.rdbuf() : nullptr} {}
  ltc6953(std::streambuf *buf_ptr = {}, reg_read_fn reg_read = {},
          reg_write_fn reg_write = {}, dev_addr_type dev_addr = {})
      : chip_base<error_type, NoerrorValue, dev_addr_type, addr_type,
                  value_type>{buf_ptr, reg_read, reg_write, dev_addr} {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
  }
  ~ltc6953() noexcept {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
  }
  int get_num() const noexcept final { return _counter.data.get_num(); }
  int get_counts() const noexcept final { return _counter.data.get_counts(); }
  std::string get_name() const noexcept final {
    return get_name(_chip_name, get_num());
  }
  void update_changes() const {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
    using namespace ltc6953_registers;
    while (_registers_update.is_changed()) {
      const auto registers_num = _registers_update.get_changed();
      write(registers_num, _registers_map.array[registers_num]);
      _registers_update.clear_changed(registers_num);
    };
  }
  void reset() const {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
    using namespace ltc6953_registers;
    // TODO:
  }
  void reset(error_type &error) const noexcept {
    helpers::noexcept_void_function<ltc6953, error_type, NoerrorValue,
                                    &ltc6953::reset>(this, error);
  }

  void powerdown(bool enabled) const {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
    using namespace ltc6953_registers;
    _registers_map.regs.reg_h02.bits.PDALL = register_type(enabled);
    write(0x02, _registers_map.regs.reg_h02.reg);
  }
  void powerdown(bool enabled, error_type &error) const noexcept {
    helpers::noexcept_set_function<ltc6953, error_type, NoerrorValue,
                                   &ltc6953::powerdown>(this, enabled, error);
  }

  void is_vco_valid(bool &is_valid) const {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
    using namespace ltc6953_registers;
    read(0, _registers_map.regs.reg_h00.reg);
    is_valid = bool(_registers_map.regs.reg_h00.bits.VCOOK);
  }
  bool is_vco_valid() const {
    bool is_valid{};
    is_vco_valid(is_valid);
    return is_valid;
  }
  bool is_vco_valid(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc6953, error_type, NoerrorValue,
                                          bool, &ltc6953::is_vco_valid>(this,
                                                                        error);
  }

 private:
  template <typename Arg = int, typename... Args>
  void _update_registers(Arg register_num, Args... other_registers) const {
    write(register_num, _registers_map.array[register_num]);
    _update_registers(other_registers...);
  }
  void _update_registers() const {}
};

}  // namespace chappi
