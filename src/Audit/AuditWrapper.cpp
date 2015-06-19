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
 * @file        src/Audit/AuditWrapper.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

 #include <cstdlib>

#include <libaudit.h>
extern int _audit_syscalladded; // God bless them

#include "AuditWrapper.h"

namespace Audit {

int AuditWrapper::audit_open(void) {
    return ::audit_open();
}

void AuditWrapper::audit_close(int fd) {
    ::audit_close(fd);
}

int AuditWrapper::audit_add_rule_data(int fd, struct audit_rule_data *rule, int flags, int action) {
    return ::audit_add_rule_data(fd, rule, flags, action);
}

int AuditWrapper::audit_delete_rule_data(int fd, struct audit_rule_data *rule, int flags,
                                         int action)
{
    return ::audit_delete_rule_data(fd, rule, flags, action);
}

int AuditWrapper::audit_rule_syscallbyname_data(struct audit_rule_data *rule, const char *scall) {
    auto ret = ::audit_rule_syscallbyname_data(rule, scall);
    if (ret == 0)
        _audit_syscalladded = 1;
    return ret;
}

int AuditWrapper::audit_rule_fieldpair_data(struct audit_rule_data **rulep, const char *pair,
                                            int flags) {
    return ::audit_rule_fieldpair_data(rulep, pair, flags);
}

void AuditWrapper::audit_rule_free_data(struct audit_rule_data *rule) {
    ::audit_rule_free_data(rule);
}

audit_rule_data *AuditWrapper::create_rule_data() {
    return static_cast<struct audit_rule_data *>(calloc(1, sizeof(struct audit_rule_data)));
}

int AuditWrapper::MAX_AUDIT_MESSAGE_LENGTH_CONST() const {
    return MAX_AUDIT_MESSAGE_LENGTH;
}

int AuditWrapper::AUDIT_FILTER_EXIT_CONST() const {
    return AUDIT_FILTER_EXIT;
}

int AuditWrapper::AUDIT_ALWAYS_CONST() const {
    return AUDIT_ALWAYS;
}

} /* namespace Audit */
