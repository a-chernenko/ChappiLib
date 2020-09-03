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

enum class tca6424_port { _0, _1, _2 };

struct tca6424_port_data {
  tca6424_port port{};
  uint8_t value{};
};

namespace detail {
struct tca6424_counter {
  chips_counter<tca6424_counter> data;
};
}  // namespace detail

template <typename ErrorType = int, ErrorType NoerrorValue = 0,
          typename DevAddrType = uint8_t, typename AddrType = uint8_t,
          typename ValueType = uint8_t>
class tca6424 final : public chip_base<ErrorType, NoerrorValue, DevAddrType,
                                       AddrType, ValueType> {
  static constexpr auto _chip_name = "TCA6424";
  detail::tca6424_counter _counter;

 public:
  CHIP_BASE_RESOLVE

  tca6424(bool log_enable)
      : tca6424{(log_enable) ? std::clog.rdbuf() : nullptr} {}
  tca6424(std::streambuf *buf_ptr = {}, reg_read_fn reg_read = {},
          reg_write_fn reg_write = {})
      : chip_base<error_type, NoerrorValue, dev_addr_type, addr_type,
                  value_type>{buf_ptr} {
#if defined(LOG_ENABLE) && defined(LOG_ENABLE_TCA6424)
    log_info(__func__);
#endif
  }
  ~tca6424() noexcept {
#if defined(LOG_ENABLE) && defined(LOG_ENABLE_TCA6424)
    log_info(__func__);
#endif
  }
  int get_num() const noexcept final { return _counter.data.get_num(); }
  int get_counts() const noexcept final { return _counter.data.get_counts(); }
  std::string get_name() const noexcept final {
    return get_name(_chip_name, get_num());
  }
  void configure_port(const tca6424_port_data &data) const {
#if defined(LOG_ENABLE) && defined(LOG_ENABLE_TCA6424)
    log_info(__func__);
#endif
    write(0x0c + static_cast<int>(data.port), data.value);
  }
  void configure_port(const tca6424_port_data &data, error_type &error) const
      noexcept {
    helpers::noexcept_set_function<tca6424, error_type, NoerrorValue,
                                   tca6424_port_data, &tca6424::configure_port>(
        this, data, error);
  }
  void set_port(const tca6424_port_data &data) const {
#if defined(LOG_ENABLE) && defined(LOG_ENABLE_TCA6424)
    log_info(__func__);
#endif
    write(0x04 + static_cast<int>(data.port), data.value);
  }
  void set_port(const tca6424_port_data &data, error_type &error) const
      noexcept {
    helpers::noexcept_set_function<tca6424, error_type, NoerrorValue,
                                   tca6424_port_data, &tca6424::set_port>(
        this, data, error);
  }
  void get_port(tca6424_port_data &data) const {
#if defined(LOG_ENABLE) && defined(LOG_ENABLE_TCA6424)
    log_info(__func__);
#endif
    read(0x04 + static_cast<int>(data.port), data.value);
  }
  value_type get_port(tca6424_port port) const {
    tca6424_port_data data{};
    data.port = port;
    get_port(data);
    return data.value;
  }
  value_type get_port(tca6424_port port, error_type &error) const noexcept {
    tca6424_port_data data{};
    data.port = port;
    helpers::noexcept_get_function<tca6424, error_type, NoerrorValue,
                                   tca6424_port_data, &tca6424::get_port>(
        this, data, error);
    return data.value;
  }
};

}  // namespace chappi
