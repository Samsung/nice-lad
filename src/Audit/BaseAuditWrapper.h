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
 * @file        src/Audit/BaseAuditWrapper.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_AUDIT_BASEAUDITWRAPPER_H
#define SRC_AUDIT_BASEAUDITWRAPPER_H

struct audit_rule_data;

namespace Audit {

class BaseAuditWrapper {
public:
    virtual ~BaseAuditWrapper() = default;

    virtual int audit_open(void) = 0;
    virtual void audit_close(int fd) = 0;

    virtual int audit_add_rule_data(int fd, struct audit_rule_data *rule, int flags,
                                    int action) = 0;
    virtual int audit_delete_rule_data(int fd, struct audit_rule_data *rule, int flags,
                                       int action) = 0;
    virtual int audit_rule_syscallbyname_data(struct audit_rule_data *rule, const char *scall) = 0;
    virtual int audit_rule_fieldpair_data(struct audit_rule_data **rulep, const char *pair,
                                          int flags) = 0;
    virtual void audit_rule_free_data(struct audit_rule_data *rule) = 0;

    virtual struct audit_rule_data *create_rule_data() = 0;

    virtual int MAX_AUDIT_MESSAGE_LENGTH_CONST() const = 0;
    virtual int AUDIT_FILTER_EXIT_CONST() const = 0;
    virtual int AUDIT_ALWAYS_CONST() const = 0;
};

} /* namespace Audit */

#endif /* SRC_AUDIT_BASEAUDITWRAPPER_H */
