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
 * @file        src/Utils/Feed.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_UTILS_FEED_H
#define SRC_UTILS_FEED_H

#include <cstdint>

#include <boost/signals2.hpp>

namespace Utils {

class Feed {
public:
    Feed(int inputFd, size_t readSize, int signalFd);
    virtual ~Feed() = default;

    // TODO: Consider using std::function as member, if the application stays single-threaded
    boost::signals2::signal<void(const char *, std::size_t)> onData;
    boost::signals2::signal<void(void)> onEod;
    boost::signals2::signal<void(int)> onSignal;
    boost::signals2::signal<void(void)> onTimeout;

    void start();
    void stop();

private:
    enum class InputType : int8_t {
        Error = -1,
        Timeout = 0,
        Input,
        Signal = 127
    };

    InputType waitForInput();

    bool m_terminate;
    int m_inputFd;
    size_t m_readSize;
    int m_signalFd;
    bool m_waitForever;
    int m_maxFd;
};

} /* namespace Utils */

#endif /* SRC_UTILS_FEED_H */
