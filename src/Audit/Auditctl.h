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
 * @file        src/Audit/Auditctl.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_AUDIT_AUDITCTL_H
#define SRC_AUDIT_AUDITCTL_H

#include <Audit/BaseAuditWrapper.h>
#include <Audit/SyscallRuleData.h>

namespace Audit {

class Auditctl {
public:
    Auditctl(BaseAuditWrapper &auditApi);
    ~Auditctl();

    void addSyscallRule(const SyscallRuleData &ruleData);
    void deleteSyscallRule(const SyscallRuleData &ruleData);

    BaseAuditWrapper &auditApi(void) {
        return m_auditApi;
    }

private:
    BaseAuditWrapper &m_auditApi;
    int m_auditFd;
};

} /* namespace Audit */

#endif /* SRC_AUDIT_AUDITCTL_H */
