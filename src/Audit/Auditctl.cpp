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
 * @file        src/Audit/Auditctl.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <Audit/ErrorException.h>

#include "Auditctl.h"

namespace Audit {

Auditctl::Auditctl(BaseAuditWrapper &auditApi)  : m_auditApi(auditApi) {
    m_auditFd = m_auditApi.audit_open();
    if (m_auditFd == -1) {
        throw ErrorException("Could not open audit fd");
    }
}

Auditctl::~Auditctl() {
    m_auditApi.audit_close(m_auditFd);
}

void Auditctl::addSyscallRule(const SyscallRuleData &ruleData) {
    auto rd = ruleData.get();
    m_auditApi.audit_add_rule_data(m_auditFd, rd, m_auditApi.AUDIT_FILTER_EXIT_CONST(),
                                   m_auditApi.AUDIT_ALWAYS_CONST());
    m_auditApi.audit_rule_free_data(rd);
}

void Auditctl::deleteSyscallRule(const SyscallRuleData &ruleData) {
    auto rd = ruleData.get();
    m_auditApi.audit_delete_rule_data(m_auditFd, rd, m_auditApi.AUDIT_FILTER_EXIT_CONST(),
                                      m_auditApi.AUDIT_ALWAYS_CONST());
    m_auditApi.audit_rule_free_data(rd);
}

} /* namespace Audit */
