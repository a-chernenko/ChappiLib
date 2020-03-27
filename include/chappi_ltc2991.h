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

#include "chappi_base.h"

namespace chappi {

struct ltc2991_data {
  double Tint;
  double V1;
  double V2;
  double V3;
  double V4;
  double V5;
  double V6;
  double V7;
  double V8;
};

namespace detail {
struct ltc2991_counter {
  chips_counter<ltc2991_counter> data;
};
}  // namespace detail

template <typename ErrorType = int, ErrorType NoerrorValue = 0,
          typename DevAddrType = uint8_t, typename AddrType = uint8_t,
          typename ValueType = uint8_t>
class ltc2991 final : public chip_base<ErrorType, NoerrorValue, DevAddrType,
                                       AddrType, ValueType> {
  static constexpr auto _chip_name = "LTC2991";
  detail::ltc2991_counter _counter;

 public:
  CHIP_BASE_RESOLVE

  ltc2991(bool log_enable)
      : ltc2991{(log_enable) ? std::clog.rdbuf() : nullptr} {}
  ltc2991(std::streambuf *buf_ptr = {}, reg_read_fn reg_read = {},
          reg_write_fn reg_write = {})
      : chip_base<error_type, NoerrorValue, dev_addr_type, addr_type,
                  value_type>{buf_ptr} {
    log_created(get_name());
  }
  ~ltc2991() noexcept { log_destroyed(get_name()); }
  int get_num() const noexcept final { return _counter.data.get_num(); }
  int get_counts() const noexcept final { return _counter.data.get_counts(); }
  std::string get_name() const noexcept final {
    return get_name(_chip_name, get_num());
  }
  void enable_all_channels() const {
    value_type value{};
    read(0x01, value);
    value |= 0b11111000;
    write(0x01, value);
  }
  void enable_all_channels(error_type &error) const noexcept {
    helpers::noexcept_void_function<ltc2991, error_type, NoerrorValue,
                                    &ltc2991::enable_all_channels>(this, error);
  }
  void repeated_mode(bool enable) const {
    value_type value{};
    read(0x08, value);
    if (enable) {
      value |= 0b00010000;
    } else {
      value &= ~0b00010000;
    }
    write(0x08, value);
  }
  void repeated_mode(bool enable, error_type &error) const noexcept {
    helpers::noexcept_set_function<ltc2991, error_type, NoerrorValue, bool,
                                   &ltc2991::repeated_mode>(this, enable,
                                                            error);
  }
  void get_temperature(double &value) const {
    value_type lsb{}, msb{};
    read(0x1A, msb);
    read(0x1B, lsb);
    value = ((msb & 0b00011111) << 8 | lsb) / 16.0;
  }
  double get_temperature() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        double, &ltc2991::get_temperature>(
        this);
  }
  double get_temperature(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          double, &ltc2991::get_temperature>(
        this, error);
  }
  void get_voltage_1(double &value) const { _get_voltage(0x0A, value); }
  double get_voltage_1() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        double, &ltc2991::get_voltage_1>(this);
  }
  double get_voltage_1(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          double, &ltc2991::get_voltage_1>(
        this, error);
  }
  void get_voltage_2(double &value) const { _get_voltage(0x0C, value); }
  double get_voltage_2() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        double, &ltc2991::get_voltage_2>(this);
  }
  double get_voltage_2(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          double, &ltc2991::get_voltage_2>(
        this, error);
  }
  void get_voltage_3(double &value) const { _get_voltage(0x0E, value); }
  double get_voltage_3() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        double, &ltc2991::get_voltage_3>(this);
  }
  double get_voltage_3(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          double, &ltc2991::get_voltage_3>(
        this, error);
  }
  void get_voltage_4(double &value) const { _get_voltage(0x10, value); }
  double get_voltage_4() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        double, &ltc2991::get_voltage_4>(this);
  }
  double get_voltage_4(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          double, &ltc2991::get_voltage_4>(
        this, error);
  }
  void get_voltage_5(double &value) const { _get_voltage(0x12, value); }
  double get_voltage_5() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        double, &ltc2991::get_voltage_5>(this);
  }
  double get_voltage_5(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          double, &ltc2991::get_voltage_5>(
        this, error);
  }
  void get_voltage_6(double &value) const { _get_voltage(0x14, value); }
  double get_voltage_6() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        double, &ltc2991::get_voltage_6>(this);
  }
  double get_voltage_6(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          double, &ltc2991::get_voltage_6>(
        this, error);
  }
  void get_voltage_7(double &value) const { _get_voltage(0x16, value); }
  double get_voltage_7() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        double, &ltc2991::get_voltage_7>(this);
  }
  double get_voltage_7(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          double, &ltc2991::get_voltage_7>(
        this, error);
  }
  void get_voltage_8(double &value) const { _get_voltage(0x18, value); }
  double get_voltage_8() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        double, &ltc2991::get_voltage_8>(this);
  }
  double get_voltage_8(error_type &error) const noexcept {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          double, &ltc2991::get_voltage_8>(
        this, error);
  }
  void get_data(ltc2991_data &value) const {
    value.Tint = get_temperature();
    value.V1 = get_voltage_1();
    value.V2 = get_voltage_2();
    value.V3 = get_voltage_3();
    value.V4 = get_voltage_4();
    value.V5 = get_voltage_5();
    value.V6 = get_voltage_6();
    value.V7 = get_voltage_7();
    value.V8 = get_voltage_8();
  }
  ltc2991_data get_data() const {
    return helpers::retval_get_function<ltc2991, error_type, NoerrorValue,
                                        ltc2991_data, &ltc2991::get_data>(this);
  }
  ltc2991_data get_data(error_type &error) const {
    return helpers::noexcept_get_function<ltc2991, error_type, NoerrorValue,
                                          ltc2991_data, &ltc2991::get_data>(
        this, error);
  }

 private:
  void _get_voltage(addr_type addr_msb, double &value) const {
    value_type val_lsb{}, val_msb{};
    read(addr_msb, val_msb);
    read(addr_msb + 1, val_lsb);
    value = ((val_msb & 0b00111111) << 8 | val_lsb) * 0.000305180;
  }
};

}  // namespace chappi
