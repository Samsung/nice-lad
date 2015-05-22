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
 * @file        src/Utils/Feed.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <algorithm>
#include <cerrno>
#include <ctime>
#include <string>
#include <sys/select.h>
#include <unistd.h>

#include <Log/log.h>

#include "Feed.h"

namespace Utils {

Feed::Feed(int inputFd, size_t readSize, int signalFd) : m_terminate(false), m_inputFd(inputFd),
                                                         m_readSize(readSize), m_signalFd(signalFd),
                                                         m_waitForever(true)
{
    m_maxFd = std::max(m_inputFd, m_signalFd) + 1;
}

void Feed::start() {
    m_terminate = false;
    m_waitForever = true;

    while (m_terminate == false) {
        const auto retval = waitForInput();

        if (retval == InputType::Timeout) {
            LOGD("Feed: timeout");
            onTimeout();
            m_waitForever = true;
            continue;
        } else if (retval == InputType::Error) {
            // TODO: Consider invoking a callback here
            LOGE("Error in select [errno=" + std::to_string(errno) + "]");
            continue;
        } else if (retval == InputType::Signal) {
            LOGD("Feed: signal");
            onSignal(m_signalFd);
            continue;
        } else if (retval == InputType::Input) {
            LOGD("Feed: data");
            char dataChunk[m_readSize];
            const auto ret = TEMP_FAILURE_RETRY(read(m_inputFd, dataChunk, m_readSize));
            if (ret > 0) {
                onData(dataChunk, ret);
                m_waitForever = false;
            } else if (ret == 0) {
                onEod();
                break;
            } else {
                // TODO: Consider invoking a callback here
                LOGE("Error reading audit input [errno=" + std::to_string(errno) + "]");
            }
        }
    }
}

void Feed::stop() {
    m_terminate = true;
}

Feed::InputType Feed::waitForInput() {
    fd_set readMask;
    FD_ZERO(&readMask);
    FD_SET(m_inputFd, &readMask);
    FD_SET(m_signalFd, &readMask);

    int selectRet = -1;
    if (m_waitForever) {
        selectRet = TEMP_FAILURE_RETRY(select(m_maxFd, &readMask, nullptr, nullptr, nullptr));
    } else {
        // TODO: Make timeout configurable
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        selectRet = TEMP_FAILURE_RETRY(select(m_maxFd, &readMask, nullptr, nullptr, &timeout));
    }

    if (selectRet > 0) {
        if (FD_ISSET(m_signalFd, &readMask)) {
            return InputType::Signal;
        } else if (FD_ISSET(m_inputFd, &readMask)) {
            return InputType::Input;
        }
    } else if (selectRet == 0) {
        return InputType::Timeout;
    }

    return InputType::Error;

}

} /* namespace Utils */
