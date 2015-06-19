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
 * @file        tests/Audit/syscall_rule_data.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Audit/SyscallRuleData.h>

#include "FakeAuditWrapper.h"

/**
 * @brief   SyscallRuleData should call audit_rule_syscallbyname_data()
 * @test    Scenario:
 * - call SyscallRuleData::get()
 * - check if audit_rule_syscallbyname_data() was called with proper args
 */
TEST(SyscallRuleData, no_values) {
    using ::testing::_;
    using ::testing::NiceMock;
    using ::testing::Return;
    using ::testing::StrEq;

    NiceMock<FakeAuditWrapper> auditApi;
    Audit::SyscallRuleData srd(auditApi);

    struct audit_rule_data *ruleData = reinterpret_cast<struct audit_rule_data *>(0x7E57);

    ON_CALL(auditApi, MAX_AUDIT_MESSAGE_LENGTH_CONST()).WillByDefault(Return(512));

    EXPECT_CALL(auditApi, create_rule_data()).WillOnce(Return(ruleData));
    EXPECT_CALL(auditApi, audit_rule_syscallbyname_data(ruleData, StrEq("all")));

    srd.get();
}

/**
 * @brief   SyscallRuleData should call audit_rule_fieldpair_data()
 * @test    Scenario:
 * - call SyscallRuleData::get()
 * - check if audit_rule_fieldpair_data() was called with proper args
 */
TEST(SyscallRuleData, some_values) {
    using ::testing::_;
    using ::testing::NiceMock;
    using ::testing::Pointee;
    using ::testing::Return;
    using ::testing::StrEq;

    const int AUDIT_FILTER_EXIT_CONST = 42;

    NiceMock<FakeAuditWrapper> auditApi;
    Audit::SyscallRuleData srd(auditApi);

    struct audit_rule_data *ruleData = reinterpret_cast<struct audit_rule_data *>(0x7E57);

    ON_CALL(auditApi, MAX_AUDIT_MESSAGE_LENGTH_CONST()).WillByDefault(Return(512));

    EXPECT_CALL(auditApi, create_rule_data()).WillOnce(Return(ruleData));
    EXPECT_CALL(auditApi, AUDIT_FILTER_EXIT_CONST()).Times(2)
        .WillRepeatedly(Return(AUDIT_FILTER_EXIT_CONST));
    EXPECT_CALL(auditApi, audit_rule_fieldpair_data(Pointee(ruleData),
                                                    StrEq("key1=value1"),
                                                    AUDIT_FILTER_EXIT_CONST))
        .WillOnce(Return(0));
    EXPECT_CALL(auditApi, audit_rule_fieldpair_data(Pointee(ruleData),
                                                    StrEq("key2=value2"),
                                                    AUDIT_FILTER_EXIT_CONST))
        .WillOnce(Return(0));
    EXPECT_CALL(auditApi, audit_rule_syscallbyname_data(ruleData, StrEq("all")));

    srd.addPair("key1", "value1");
    srd.addPair("key2", "value2");
    srd.get();
}
