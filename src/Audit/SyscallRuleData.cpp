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
 * @file        src/Audit/SyscallRuleData.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <new>

#include <Audit/ErrorException.h>

#include "SyscallRuleData.h"

namespace Audit {

SyscallRuleData::SyscallRuleData(BaseAuditWrapper &auditApi, const std::string &syscall)
    : m_auditApi(auditApi), m_syscall(syscall) {}

void SyscallRuleData::addPair(const std::string &key, const std::string &value) {
    m_pairs.insert({ key, value });
}

struct audit_rule_data *SyscallRuleData::get() const {
    auto ruleData = m_auditApi.create_rule_data();

    m_auditApi.audit_rule_syscallbyname_data(ruleData, m_syscall.c_str());

    const auto pairLen = m_auditApi.MAX_AUDIT_MESSAGE_LENGTH_CONST();
    char pair[pairLen];

    for (const auto &kv : m_pairs) {
        auto ret = snprintf(pair, pairLen, "%s=%s", kv.first.c_str(), kv.second.c_str());
        if (ret < 0 || ret >= pairLen)
            throw ErrorException("Could not fill rule data");
        m_auditApi.audit_rule_fieldpair_data(&ruleData, pair, m_auditApi.AUDIT_FILTER_EXIT_CONST());
    }

    return ruleData;
}

} /* namespace Audit */
