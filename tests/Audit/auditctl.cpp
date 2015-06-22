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
 * @file        tests/Audit/auditctl.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Audit/Auditctl.h>

#include "FakeAuditWrapper.h"
#include "FakeSyscallRuleData.h"

/**
 * @brief   Auditctl should correctly add new syscall rule
 * @test    Scenario:
 * - call Auditctl::addSyscallRule()
 * - check if audit_add_rule_data() was called with proper args
 * - check if const args of audit_add_rule_data() were obtained via API
 * - check if audit fd was opened before audit_add_rule_data()
 * - check if audit fd was closed after audit_add_rule_data()
 * - check if audit_rule_data was freed
 */
TEST(Auditctl, addSyscallRule) {
    using ::testing::InSequence;
    using ::testing::Return;
    using ::testing::Sequence;

    const int AUDIT_FILTER_EXIT_CONST = 1;
    const int AUDIT_ALWAYS_CONST = 42;
    const int AUDIT_FD_CONST = 3;
    struct audit_rule_data *ruleData = reinterpret_cast<struct audit_rule_data *>(0x7E57);

    FakeAuditWrapper auditApi;
    FakeSyscallRuleData srd(auditApi);

    Sequence s1, s2, s3, s4;

    EXPECT_CALL(auditApi, audit_open())
        .InSequence(s1).WillOnce(Return(AUDIT_FD_CONST));
    EXPECT_CALL(srd, get()).InSequence(s2).WillOnce(Return(ruleData));
    EXPECT_CALL(auditApi, AUDIT_FILTER_EXIT_CONST()).Times(1)
        .InSequence(s3).WillRepeatedly(Return(AUDIT_FILTER_EXIT_CONST));
    EXPECT_CALL(auditApi, AUDIT_ALWAYS_CONST()).Times(1)
        .InSequence(s4).WillRepeatedly(Return(AUDIT_ALWAYS_CONST));
    EXPECT_CALL(auditApi, audit_add_rule_data(AUDIT_FD_CONST, ruleData, AUDIT_FILTER_EXIT_CONST,
                                              AUDIT_ALWAYS_CONST))
            .InSequence(s1, s2, s3, s4).WillOnce(Return(0));
    EXPECT_CALL(auditApi, audit_rule_free_data(ruleData)).InSequence(s1, s2, s3, s4);
    EXPECT_CALL(auditApi, audit_close(AUDIT_FD_CONST)).InSequence(s1, s2, s3, s4);

    Audit::Auditctl *auditctl = new Audit::Auditctl(auditApi);
    auditctl->addSyscallRule(srd);
    delete auditctl;
}

/**
 * @brief   Auditctl should correctly delete syscall rule
 * @test    Scenario:
 * - call Auditctl::deleteSyscallRule()
 * - check if audit_delete_rule_data() was called with proper args
 * - check if const args of audit_add_rule_data() were obtained via API
 * - check if audit fd was opened before audit_add_rule_data()
 * - check if audit fd was closed after audit_add_rule_data()
 * - check if audit_rule_data was freed
 */
TEST(Auditctl, deleteSyscallRule) {
    using ::testing::InSequence;
    using ::testing::Return;
    using ::testing::Sequence;

    const int AUDIT_FILTER_EXIT_CONST = 1;
    const int AUDIT_ALWAYS_CONST = 42;
    const int AUDIT_FD_CONST = 3;
    struct audit_rule_data *ruleData = reinterpret_cast<struct audit_rule_data *>(0x7E57);

    FakeAuditWrapper auditApi;
    FakeSyscallRuleData srd(auditApi);

    Sequence s1, s2, s3, s4;

    EXPECT_CALL(auditApi, audit_open())
        .InSequence(s1).WillOnce(Return(AUDIT_FD_CONST));
    EXPECT_CALL(srd, get()).InSequence(s2).WillOnce(Return(ruleData));
    EXPECT_CALL(auditApi, AUDIT_FILTER_EXIT_CONST()).Times(1)
        .InSequence(s3).WillRepeatedly(Return(AUDIT_FILTER_EXIT_CONST));
    EXPECT_CALL(auditApi, AUDIT_ALWAYS_CONST()).Times(1)
        .InSequence(s4).WillRepeatedly(Return(AUDIT_ALWAYS_CONST));
    EXPECT_CALL(auditApi, audit_delete_rule_data(AUDIT_FD_CONST, ruleData, AUDIT_FILTER_EXIT_CONST,
                                                 AUDIT_ALWAYS_CONST))
            .InSequence(s1, s2, s3, s4).WillOnce(Return(0));
    EXPECT_CALL(auditApi, audit_rule_free_data(ruleData)).InSequence(s1, s2, s3, s4);
    EXPECT_CALL(auditApi, audit_close(AUDIT_FD_CONST)).InSequence(s1, s2, s3, s4);

    Audit::Auditctl *auditctl = new Audit::Auditctl(auditApi);
    auditctl->deleteSyscallRule(srd);
    delete auditctl;
}
