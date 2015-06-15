/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
/**
 * @file        src/Audit/WithMessageException.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_UTILS_WITHMESSAGEEXCEPTION_H
#define SRC_UTILS_WITHMESSAGEEXCEPTION_H

#include <exception>
#include <string>

namespace Utils {

class WithMessageException : std::exception {
public:
    WithMessageException(const std::string &message) : m_message(message) {}
    virtual ~WithMessageException() = default;

    virtual const char *what() const noexcept {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

} // namespace Utils


#endif // SRC_UTILS_WITHMESSAGEEXCEPTION_H
