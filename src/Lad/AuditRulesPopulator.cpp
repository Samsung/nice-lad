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
 * @file        src/Lad/AuditRulesPopulator.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include "AuditRulesPopulator.h"

namespace Lad {

AuditRulesPopulator::AuditRulesPopulator(Audit::Auditctl &auditctl, DataProvider &dataprovider)
    : m_auditctl(auditctl), m_dataprovider(dataprovider)
{
    auto groups = dataprovider.getResourceGroups();

    for (const auto &group : groups) {
        Audit::SyscallRuleData syscallRule(m_auditctl.auditApi(), "all");
        syscallRule.addPair("obj_gid", group);
        syscallRule.addPair("success", "0");
        syscallRule.addPair("key", "nice-lad");

        m_auditctl.addSyscallRule(syscallRule);
        m_syscallRules.push_back(std::move(syscallRule));
    }
}

AuditRulesPopulator::~AuditRulesPopulator() {
    for (auto &rule : m_syscallRules) {
        m_auditctl.deleteSyscallRule(rule);
    }
}

} /* namespace Lad */
