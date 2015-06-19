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
 * @file        tests/Audit/FakeAuditWrapper.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef TESTS_AUDIT_FAKEAUDITWRAPPER_H
#define TESTS_AUDIT_FAKEAUDITWRAPPER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Audit/BaseAuditWrapper.h>

class FakeAuditWrapper : public Audit::BaseAuditWrapper {
public:
    using BaseAuditWrapper::BaseAuditWrapper;

    MOCK_METHOD0(audit_open, int(void));
    MOCK_METHOD1(audit_close, void(int fd));

    MOCK_METHOD4(audit_add_rule_data, int(int fd, struct audit_rule_data *rule, int flags,
                                          int action));
    MOCK_METHOD4(audit_delete_rule_data, int(int fd, struct audit_rule_data *rule, int flags,
                                             int action));
    MOCK_METHOD2(audit_rule_syscallbyname_data, int(struct audit_rule_data *rule,
                                                    const char *scall));
    MOCK_METHOD3(audit_rule_fieldpair_data, int(struct audit_rule_data **rulep, const char *pair,
                                                int flags));
    MOCK_METHOD1(audit_rule_free_data, void(struct audit_rule_data *rule));

    MOCK_METHOD0(create_rule_data, struct audit_rule_data *(void));

    MOCK_CONST_METHOD0(MAX_AUDIT_MESSAGE_LENGTH_CONST, int(void));
    MOCK_CONST_METHOD0(AUDIT_FILTER_EXIT_CONST, int(void));
    MOCK_CONST_METHOD0(AUDIT_ALWAYS_CONST, int(void));
};

#endif /* TESTS_AUDIT_FAKEAUDITWRAPPER_H */
