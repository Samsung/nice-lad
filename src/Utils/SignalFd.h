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
 * @file        src/Utils/SignalFd.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_UTILS_SIGNALFD_H
#define SRC_UTILS_SIGNALFD_H

#include <cstdint>
#include <set>

#include <Utils/WithMessageException.h>

namespace Utils {

namespace SignalFd {

class ErrorException : public WithMessageException {
public:
    ErrorException(const std::string &message) : WithMessageException("Signal Fd: " + message) {}
};

int createSignalFd(const std::set<int> &signals);
uint32_t readSignalNo(int fd);

}

} /* namespace Utils */

#endif /* SRC_UTILS_SIGNALFD_H */
