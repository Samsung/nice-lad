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
 * @file        src/Utils/SignalFd.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <cerrno>
#include <csignal>
#include <string>
#include <sys/signalfd.h>
#include <unistd.h>

#include "SignalFd.h"

namespace Utils {

namespace SignalFd {

namespace {

void blockAllSignals() {
    sigset_t sigset;
    if (sigfillset(&sigset) == -1) {
        throw ErrorException("Could not fill signal set");
    }

    if (sigprocmask(SIG_BLOCK, &sigset, nullptr) == -1) {
        throw ErrorException("Could not block signals");
    }
}

void fillSignalSet(sigset_t *sigset, const std::set<int> &signals) {
    if (sigemptyset(sigset) == -1) {
        throw ErrorException("Could not empty signal set");
    }

    for (int sig : signals) {
        if (sigaddset(sigset, sig) == -1) {
            throw ErrorException("Could not add signal to set");
        }
    }
}

} /* namespace */

int createSignalFd(const std::set<int> &signals) {
    sigset_t signalsToCatch;

    blockAllSignals();
    fillSignalSet(&signalsToCatch, signals);

    auto sigFd = signalfd(-1, &signalsToCatch, 0);
    if (sigFd == -1) {
        throw ErrorException("Could not create signal fd");
    }

    return sigFd;
}

uint32_t readSignalNo(int sigFd) {
    struct signalfd_siginfo fdsi;
    errno = 0;
    const auto ret = TEMP_FAILURE_RETRY(read(sigFd, &fdsi, sizeof(struct signalfd_siginfo)));
    if (ret != sizeof(struct signalfd_siginfo)) {
        throw ErrorException("Could not read signal [errno=" + std::to_string(errno) + "]");
    }
    return fdsi.ssi_signo;
}

} /* namespace SignalFd */

} /* namespace Utils */
