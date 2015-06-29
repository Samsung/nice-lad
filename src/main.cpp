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
 * @file        src/main.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <csignal>
#include <cstdlib>
#include <memory>

#include <Audit/Auditctl.h>
#include <Audit/AuditWrapper.h>
#include <Audit/AuparseSourceFeedWrapper.h>
#include <Audit/ErrorException.h>
#include <Audit/Parser.h>
#include <Lad/AuditEventHandler.h>
#include <Lad/AuditRulesPopulator.h>
#include <Lad/Options.h>
#include <Log/log.h>
#include <Utils/Feed.h>
#include <Utils/SignalFd.h>
#include <Utils/WithMessageException.h>

int main(int argc, char **argv) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    init_log();

    LOGI("Starting nice-lad");

    try {
        Audit::AuditWrapper auditApi;
        Audit::AuparseSourceFeedWrapper auparseApi;
        Audit::Parser auParser(auparseApi);
        Audit::Auditctl auditctl(auditApi);
        auto &dataProvider = Lad::Options::dataProvider();
        auto &dataCollector = Lad::Options::dataCollector();
        Lad::AuditRulesPopulator rulesPopulator(auditctl, dataProvider);
        int sigFd = Utils::SignalFd::createSignalFd({ SIGHUP, SIGTERM });

        Utils::Feed feed(STDIN_FILENO, auditApi.MAX_AUDIT_MESSAGE_LENGTH_CONST(), sigFd);

        feed.onData.connect(std::bind(&Audit::Parser::feed, &auParser, _1, _2));

        feed.onTimeout.connect(std::bind(&Audit::Parser::flush, &auParser));

        feed.onEod.connect([&auParser] (void) {
            auParser.flush();
            LOGI("End of data. Terminating.");
        });

        feed.onSignal.connect([&feed] (int sigFd) {
            const auto sigNo = Utils::SignalFd::readSignalNo(sigFd);
            if (sigNo == SIGTERM) {
                LOGI("Got SIGTERM (Terminating)");
                feed.stop();
            } else if (sigNo == SIGHUP) {
                LOGI("Got SIGHUP (Reloading configuration)");
            } else {
                LOGW("Unexpected signal (" << sigNo << ")");
            }
        });

        Lad::AuditEventHandler eventHandler;
        auParser.onEvent.connect(std::bind(&Lad::AuditEventHandler::handleEvent, &eventHandler, _1));

        eventHandler.onLogDenial.connect(std::bind(&Lad::DataCollector::log, &dataCollector, _1));

        LOGD("nice-lad up and ready");

       feed.start();
    } catch (const std::exception &ex) {
        LOGC(ex.what() << " (Terminating)");
        return EXIT_FAILURE;
    } catch (...) {
        LOGC("Unknown error (Terminating)");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
