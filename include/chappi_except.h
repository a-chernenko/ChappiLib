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

#include <stdexcept>

namespace chappi {

template <typename ErrorType>
class runtime_error : public std::runtime_error {
    ErrorType _error;

public:
    runtime_error(ErrorType error, const char* message)
        : std::runtime_error(message)
        , _error { error }
    {
    }
    runtime_error(ErrorType error, std::string message)
        : std::runtime_error(message)
        , _error { error }
    {
    }
    ErrorType get_error() const noexcept { return _error; }
    virtual ~runtime_error() noexcept = default;
};

} // namespace chappi
