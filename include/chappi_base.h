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

#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include "chappi_except.h"

#if __cplusplus < 201402L
#error \
    "This file requires compiler and library support for the ISO C++ 2014 standard or later."
#endif

namespace chappi {

class logstream {
  bool _enabled{true};
  std::ostream _log;

 public:
  logstream(bool log_enable)
      : logstream{(log_enable) ? std::clog.rdbuf() : nullptr} {}
  logstream(std::streambuf *buf_ptr = {}) : _log{buf_ptr} {}
  ~logstream() noexcept = default;
  template <typename Type>
  logstream &operator<<(Type &&rhs) {
    if (_enabled) {
      _log << std::forward<Type>(rhs);
    }
    return *this;
  }
  void set_enabled(bool enabled) noexcept { _enabled = enabled; }
  bool is_enabled() const noexcept { return _enabled; }
};

namespace helpers {

template <typename ClassType, typename GetType>
using base_get_functor = void (ClassType::*)(GetType &) const;

template <typename ClassType, typename ErrorType, ErrorType NoerrorValue,
          typename GetType, base_get_functor<ClassType, GetType> functor>
GetType noexcept_get_function(const ClassType *const _this,
                              ErrorType &error) noexcept {
  GetType retval;
  try {
    (_this->*functor)(retval);
    error = NoerrorValue;
  } catch (const runtime_error<ErrorType> &e) {
    error = e.get_error();
  }
  return retval;
}

template <typename ClassType, typename ErrorType, ErrorType NoerrorValue,
          typename GetType, base_get_functor<ClassType, GetType> functor>
void noexcept_get_function(const ClassType *const _this, GetType &retval,
                           ErrorType &error) noexcept {
  try {
    (_this->*functor)(retval);
    error = NoerrorValue;
  } catch (const runtime_error<ErrorType> &e) {
    error = e.get_error();
  }
}

template <typename ClassType, typename SetType>
using base_set_functor = void (ClassType::*)(SetType) const;

template <typename ClassType, typename ErrorType, ErrorType NoerrorValue,
          typename SetType, base_set_functor<ClassType, SetType> functor>
void noexcept_set_function(const ClassType *const _this, SetType setval,
                           ErrorType &error) noexcept {
  try {
    (_this->*functor)(setval);
    error = NoerrorValue;
  } catch (const runtime_error<ErrorType> &e) {
    error = e.get_error();
  }
}

template <typename ClassType>
using base_void_functor = void (ClassType::*)() const;

template <typename ClassType, typename ErrorType, ErrorType NoerrorValue,
          base_void_functor<ClassType> functor>
void noexcept_void_function(const ClassType *const _this,
                            ErrorType &error) noexcept {
  try {
    (_this->*functor)();
    error = NoerrorValue;
  } catch (const runtime_error<ErrorType> &e) {
    error = e.get_error();
  }
}

}  // namespace helpers

template <typename ClassType>
class chips_counter final {
#if __cplusplus < 201703L
  static int _counts;
#else
  inline static int _counts{-1};
#endif
  int _num{-1};

 public:
  using owner_type = ClassType;
  chips_counter() {
    ++_counts;
    _num = _counts;
  }
  ~chips_counter() { --_counts; }
  int get_counts() const { return _counts; }
  int get_num() const { return _num; }
};

#if __cplusplus < 201703L
template <typename ClassType>
int chips_counter<ClassType>::_counts{-1};
#endif

#define CHIP_BASE_RESOLVE                                                  \
  using typename chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType, \
                           ValueType>::error_type;                         \
  using typename chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType, \
                           ValueType>::dev_addr_type;                      \
  using typename chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType, \
                           ValueType>::addr_type;                          \
  using typename chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType, \
                           ValueType>::value_type;                         \
  using typename chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType, \
                           ValueType>::reg_read_fn;                        \
  using typename chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType, \
                           ValueType>::reg_write_fn;                       \
  using chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType,          \
                  ValueType>::no_error_value;                              \
  using chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType,          \
                  ValueType>::chip_base;                                   \
  using chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType,          \
                  ValueType>::get_name;                                    \
  using chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType,          \
                  ValueType>::read;                                        \
  using chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType,          \
                  ValueType>::write;                                       \
  using chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType,          \
                  ValueType>::log;                                         \
  using chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType,          \
                  ValueType>::log_info;                                    \
  using chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType,          \
                  ValueType>::log_set_enabled;                             \
  using chip_base<ErrorType, NoerrorValue, DevAddrType, AddrType,          \
                  ValueType>::log_is_enabled;

template <typename ErrorType, ErrorType NoerrorValue, typename DevAddrType,
          typename AddrType, typename ValueType>
class chip_base {
  template <typename return_type = ErrorType>
  using _reg_read_fn =
      std::function<return_type(DevAddrType, AddrType, ValueType &)>;
  template <typename return_type = ErrorType>
  using _reg_write_fn =
      std::function<return_type(DevAddrType, AddrType, ValueType)>;

 public:
  using error_type = ErrorType;
  using value_type = ValueType;
  using addr_type = AddrType;
  using dev_addr_type = DevAddrType;
  using reg_read_fn = _reg_read_fn<error_type>;
  using reg_write_fn = _reg_write_fn<error_type>;

 protected:
  mutable logstream log;
  const error_type no_error_value;
  chip_base(bool log_enable) : log{log_enable}, no_error_value{NoerrorValue} {}
  chip_base(std::streambuf *buf_ptr = {}, const reg_read_fn &reg_read = {},
            const reg_write_fn &reg_write = {}, addr_type dev_addr = {})
      : log{buf_ptr},
        no_error_value{NoerrorValue},
        _dev_addr{dev_addr},
        _reg_read{reg_read},
        _reg_write{reg_write} {}
  virtual ~chip_base() noexcept = default;
  std::string get_name(const std::string &chip_name, int count) const {
    std::string name{};
    name += chip_name;
    name += '-';
    name += std::to_string(get_num());
    return name;
  }
#if defined(CHAPPI_LOG_ENABLE)
  void log_info(const std::string &message) const noexcept {
    log << '[' << get_name() << "] " << message << '\n';
  }
#else
  void log_info(const std::string &message) const noexcept {}
#endif

 public:
  virtual std::string get_name() const noexcept = 0;
  virtual int get_counts() const noexcept = 0;
  virtual int get_num() const noexcept = 0;
  void log_set_enabled(bool enabled) noexcept { log.set_enabled(enabled); }
  bool log_is_enabled() const noexcept { return log.is_enabled(); }
  void write(addr_type addr, value_type value) const {
    static const char error_msg[]{"chip reg write error"};
    error_type error = _reg_write(_dev_addr, addr, value);
#if defined(CHAPPI_LOG_ENABLE)
    log << '[' << get_name() << ']' << " <W> DEV:" << +_dev_addr
        << " | REG:" << +addr << " | VAL:" << +value << '\n';
#endif
    if (error != no_error_value)
      throw runtime_error<error_type>(error, error_msg);
  }
  void read(addr_type addr, value_type &value) const {
    static const char error_msg[]{"chip reg read error"};
    error_type error = _reg_read(_dev_addr, addr, value);
#if defined(CHAPPI_LOG_ENABLE)
    log << '[' << get_name() << ']' << " <R> DEV:" << +_dev_addr
        << " | REG:" << +addr << " | VAL:" << +value << '\n';
#endif
    if (error != no_error_value)
      throw runtime_error<error_type>(error, error_msg);
  }
  void write(addr_type addr, value_type value, error_type &error) const
      noexcept {
    error = _reg_write(_dev_addr, addr, value);
  }
  void read(addr_type addr, value_type &value, error_type &error) const
      noexcept {
    error = _reg_read(_dev_addr, addr, value);
  }
  void setup_io(const reg_read_fn &reg_read, const reg_write_fn &reg_write,
                dev_addr_type dev_addr = {}) noexcept {
    _reg_read = reg_read;
    _reg_write = reg_write;
    _dev_addr = dev_addr;
  }
  void set_dev_addr(dev_addr_type dev_addr) noexcept { _dev_addr = dev_addr; }
  dev_addr_type get_dev_addr() const noexcept { return _dev_addr; }

 private:
  dev_addr_type _dev_addr{};
  reg_read_fn _reg_read{};
  reg_write_fn _reg_write{};
};

}  // namespace chappi
