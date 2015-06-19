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
 * @file        src/Audit/AuditWrapper.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_AUDIT_AUDITWRAPPER_H
#define SRC_AUDIT_AUDITWRAPPER_H

#include "BaseAuditWrapper.h"

namespace Audit {

class AuditWrapper : public BaseAuditWrapper {
public:
    using BaseAuditWrapper::BaseAuditWrapper;
    virtual ~AuditWrapper() = default;

    virtual int audit_open(void);
    virtual void audit_close(int fd);

    virtual int audit_add_rule_data(int fd, struct audit_rule_data *rule, int flags, int action);
    virtual int audit_delete_rule_data(int fd, struct audit_rule_data *rule, int flags,
                                       int action);
    virtual int audit_rule_syscallbyname_data(struct audit_rule_data *rule, const char *scall);
    virtual int audit_rule_fieldpair_data(struct audit_rule_data **rulep, const char *pair,
                                          int flags);
    virtual void audit_rule_free_data(struct audit_rule_data *rule);

    virtual audit_rule_data *create_rule_data();

    virtual int MAX_AUDIT_MESSAGE_LENGTH_CONST() const;
    virtual int AUDIT_FILTER_EXIT_CONST() const;
    virtual int AUDIT_ALWAYS_CONST() const;
};

} /* namespace Audit */

#endif /* SRC_AUDIT_AUDITWRAPPER_H */
