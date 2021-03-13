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

struct adn4600_xpt_data {
  uint16_t input{};
  uint16_t output{};
};

namespace detail {
struct adn4600_counter : chips_counter<adn4600_counter> {};
}  // namespace detail

template <typename ErrorType = int, ErrorType NoerrorValue = 0,
          typename DevAddrType = uint8_t, typename AddrType = uint8_t,
          typename ValueType = uint16_t>
class adn4600 final : public chip_base<ErrorType, NoerrorValue, DevAddrType,
                                       AddrType, ValueType> {
  static constexpr auto _chip_name = "ADN4600";
  detail::adn4600_counter _counter;

 public:
  CHIP_BASE_RESOLVE

  adn4600(bool log_enable)
      : adn4600{(log_enable) ? std::clog.rdbuf() : nullptr} {}
  adn4600(std::streambuf *buf_ptr = {}, reg_read_fn reg_read = {},
          reg_write_fn reg_write = {}, dev_addr_type dev_addr = {})
      : chip_base<error_type, NoerrorValue, dev_addr_type, addr_type,
                  value_type>{buf_ptr, reg_read, reg_write, dev_addr} {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
  }
  ~adn4600() noexcept {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
  }
  int get_num() const noexcept final { return _counter.get_num(); }
  int get_counts() const noexcept final { return _counter.get_counts(); }
  std::string get_name() const noexcept final {
    return get_name(_chip_name, get_num());
  }
  void reset() const {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
    write(0x00, 0x01);
  }
  void reset(error_type &error) const noexcept {
    helpers::noexcept_void_function<adn4600, error_type, NoerrorValue,
                                    &adn4600::reset>(this, error);
  }
  void xpt_config(const adn4600_xpt_data &data) const {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
    const auto value =
        value_type(((data.input << 4) & 0x70) | (data.output & 0x07));
    write(0x40, value);
  }
  void xpt_config(const adn4600_xpt_data &data,
                  error_type &error) const noexcept {
    helpers::noexcept_set_function<adn4600, error_type, NoerrorValue,
                                   adn4600_xpt_data, &adn4600::xpt_config>(
        this, data, error);
  }
  void xpt_update() const {
#if defined(CHAPPI_LOG_ENABLE)
    log_info(__func__);
#endif
    write(0x41, 0x01);
  }
  void xpt_update(error_type &error) const noexcept {
    helpers::noexcept_void_function<adn4600, error_type, NoerrorValue,
                                    &adn4600::xpt_update>(this, error);
  }
};

}  // namespace chappi
