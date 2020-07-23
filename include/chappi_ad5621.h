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

namespace detail {
struct ad5621_counter {
  chips_counter<ad5621_counter> data;
};
}  // namespace detail

template <typename ErrorType = int, ErrorType NoerrorValue = 0,
          typename DevAddrType = uint8_t, typename AddrType = uint8_t,
          typename ValueType = uint16_t>
class ad5621 final : public chip_base<ErrorType, NoerrorValue, DevAddrType,
                                      AddrType, ValueType> {
  static constexpr auto _chip_name = "AD5621";
  detail::ad5621_counter _counter;

 public:
  CHIP_BASE_RESOLVE

  ad5621(bool log_enable)
      : ad5621{(log_enable) ? std::clog.rdbuf() : nullptr} {}
  ad5621(std::streambuf *buf_ptr = {}, reg_read_fn reg_read = {},
         reg_write_fn reg_write = {})
      : chip_base<error_type, NoerrorValue, dev_addr_type, addr_type,
                  value_type>{buf_ptr} {
    log_created();
  }
  ~ad5621() noexcept { log_destroyed(); }
  int get_num() const noexcept final { return _counter.data.get_num(); }
  int get_counts() const noexcept final { return _counter.data.get_counts(); }
  std::string get_name() const noexcept final {
    return get_name(_chip_name, get_num());
  }
  void set_value(value_type value) const { write(0x00, value << 2); }
  void set_value(value_type value, error_type &error) const noexcept {
    helpers::noexcept_set_function<ad5621, error_type, NoerrorValue, value_type,
                                   &ad5621::set_value>(this, value, error);
  }
};

}  // namespace chappi
