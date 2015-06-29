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
 * @file        src/Lad/AuditEventHandler.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

#include <Log/log.h>

#include "AuditEventHandler.h"

namespace Lad {

const std::string EVTYPE_SYSCALL = "SYSCALL";
const std::string EVTYPE_AVC = "AVC";
const std::string EVTYPE_CYNARA = "TRUSTED_APP";
const std::string EVTYPE_NETFILTER = "NETFILTER_PKT";

const std::string UNKNOWN_PROPERTY = "[unknown]";

typedef void(AuditEventHandler::* EventDispatcherFn)(const Audit::Parser::Event &);

std::unordered_map<std::string, EventDispatcherFn> eventDispatcher = {
    { EVTYPE_SYSCALL,   &AuditEventHandler::handleSyscall },
    { EVTYPE_AVC,       &AuditEventHandler::handleAVC },
    { EVTYPE_CYNARA,    &AuditEventHandler::handleCynara },
    { EVTYPE_NETFILTER, &AuditEventHandler::handleNetfilter }
};

class PropertyGetter {
public:
    PropertyGetter(const Audit::Parser::Record &rec) : m_rec(rec) {}

    std::string operator()(const std::string &prop) const {
        const auto &propIter = m_rec.find(prop);
        if (propIter != m_rec.end())
            return propIter->second;
        else
            return UNKNOWN_PROPERTY;
    }

private:
    const Audit::Parser::Record &m_rec;
};

void AuditEventHandler::handleEvent(const Audit::Parser::Event &event) {

    std::string eventType;
    for (const auto &rec : event) {
        const auto typeIter = rec.find("type");
        if (typeIter == rec.end())
            continue;
        const auto &recType = typeIter->second;
        const auto disp = eventDispatcher.find(recType);
        if (disp != eventDispatcher.end()) {
            auto fun = disp->second;
            (this->*fun)(event);
        }
        break;
    }
}

void AuditEventHandler::handleSyscall(const Audit::Parser::Event &event) {
    auto syscall = UNKNOWN_PROPERTY;
    auto exit = UNKNOWN_PROPERTY;
    auto filename = UNKNOWN_PROPERTY;
    auto objGid = UNKNOWN_PROPERTY;
    auto objLabel = UNKNOWN_PROPERTY;
    auto subjLabel = UNKNOWN_PROPERTY;

    for (const auto &rec : event) {
        const auto typeIter = rec.find("type");
        if (typeIter == rec.end()) {
            LOGW("Malformed record in SYSCALL event");
            continue;
        }

        PropertyGetter props(rec);
        const auto &recType = typeIter->second;
        if (recType == "SYSCALL") {
            syscall = props("syscall");
            subjLabel = props("subj");
            exit = props("exit");
        } else if (recType == "PATH") {
            auto nameType = props("nametype");
            if (nameType == "NORMAL") {
                filename = props("name");
                objGid = props("ogid");
                objLabel = props("obj");
            }
        }
    }

    std::stringstream ss;
    ss << "ACCESS DENIED ON SYSCALL syscall=" << syscall << " filename=" << filename
       << " exit=" << exit << " gid=" << objGid << " object=" << objLabel
       << " subject=" << subjLabel;

    onLogDenial(ss.str());
}

// TODO: Sometimes Smack event comes with AVC and PATH records
//       -- have to investigate need to check on PATH
void AuditEventHandler::handleAVC(const Audit::Parser::Event &event) {
    auto objLabel = UNKNOWN_PROPERTY;
    auto subjLabel = UNKNOWN_PROPERTY;
    auto accessReq = UNKNOWN_PROPERTY;

    for (const auto &rec : event) {
        const auto typeIter = rec.find("type");
        if (typeIter == rec.end()) {
            LOGW("Malformed record in AVC event");
            continue;
        }

        PropertyGetter props(rec);
        const auto &recType = typeIter->second;
        if (recType == "AVC") {
            if (props("lsm") == "SMACK" && props("action") == "denied") {
                subjLabel = props("subject");
                objLabel = props("object");
                accessReq = props("requested");
            } else {
                return;
            }
        }
    }

    std::stringstream ss;
    ss << "ACCESS DENIED ON SMACK object=" << objLabel << " subject=" << subjLabel
       << " access=" << accessReq;
    onLogDenial(ss.str());
}

void AuditEventHandler::handleCynara(const Audit::Parser::Event &event) {
    auto client = UNKNOWN_PROPERTY;
    auto user = UNKNOWN_PROPERTY;
    auto privilege = UNKNOWN_PROPERTY;

    for (const auto &rec : event) {
        const auto typeIter = rec.find("type");
        if (typeIter == rec.end()) {
            LOGW("Malformed record in TRUSTED_APP event");
            continue;
        }

        PropertyGetter props(rec);
        const auto &recType = typeIter->second;
        if (recType == "TRUSTED_APP") {
            // Interested only in denials
            if (props("app") != "Cynara" || props("policy_type") != "0") {
                continue;
            }

            client = props("client");
            user = props("user");
            privilege = props("privilege");
        }
    }

    std::stringstream ss;
    ss << "ACCESS DENIED ON CYNARA client=" << client << " user=" << user
       << " privilege=" << privilege;
    onLogDenial(ss.str());
}

void AuditEventHandler::handleNetfilter(const Audit::Parser::Event &event) {
    auto obj = UNKNOWN_PROPERTY;
    auto outif = UNKNOWN_PROPERTY;
    auto proto = UNKNOWN_PROPERTY;
    auto saddr = UNKNOWN_PROPERTY;
    auto sport = UNKNOWN_PROPERTY;
    auto daddr = UNKNOWN_PROPERTY;
    auto dport = UNKNOWN_PROPERTY;

    for (const auto &rec : event) {
        const auto typeIter = rec.find("type");
        if (typeIter == rec.end()) {
            LOGW("Malformed record in NETFILTER_PKT event");
            continue;
        }

        PropertyGetter props(rec);
        const auto &recType = typeIter->second;
        if (recType == "NETFILTER_PKT") {
            if (props("action") != "2") { // Interested only in denials
                continue;
            }

            obj = props("obj");
            outif = props("outif");
            proto = props("proto");
            saddr = props("saddr");
            sport = props("sport");
            daddr = props("daddr");
            dport = props("dport");
        }
    }

    std::stringstream ss;
    ss << "ACCESS DENIED ON NETFILTER"
       << " obj=" << obj << " outif=" << outif << " proto=" << proto
       << " saddr=" << saddr << " sport=" << sport
       << " daddr=" << daddr << " dport=" << dport;
    onLogDenial(ss.str());
}

} /* namespace Lad */
