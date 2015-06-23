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
 * @file        tests/SecurityManager/data_provider.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <memory>
#include <new>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <SecurityManager/DataProvider.h>
#include <SecurityManager/ErrorException.h>

#include "FakeSecurityManagerWrapper.h"

/**
 * @brief   SecurityManager::DataProvider should throw if groups cannot be obtained
 * @test    Scenario:
 * - call SecurityManager::DataProvider::getResourceGroups()
 * - return non-success from security_manager_groups_get
 * - check if SecurityManager::ErrorException was thrown
 */
TEST(SecurityManager, getResourceGroups_error) {
    using ::testing::NotNull;
    using ::testing::Return;
    using ::testing::NiceMock;

    NiceMock<FakeSecurityManagerWrapper> smApi;

    const int SECURITY_MANAGER_SUCCESS_CONST = 42;

    ON_CALL(smApi, SECURITY_MANAGER_SUCCESS_CONST())
        .WillByDefault(Return(SECURITY_MANAGER_SUCCESS_CONST));

    EXPECT_CALL(smApi, security_manager_groups_get(NotNull(), NotNull()))
        .WillOnce(Return(!SECURITY_MANAGER_SUCCESS_CONST));

    SecurityManager::DataProvider dp(smApi);

    EXPECT_THROW(dp.getResourceGroups(), SecurityManager::ErrorException);
}

/**
 * @brief   SecurityManager::DataProvider should retrieve resource groups
 * @test    Scenario:
 * - call SecurityManager::DataProvider::getResourceGroups()
 * - check if security_manager_groups_get() was called with proper args
 * - check if correct groups are returned by security_manager_groups_get()
 * - check if groups were freed with security_manager_groups_free()
 */
TEST(SecurityManager, getResourceGroups) {
    using ::testing::DoAll;
    using ::testing::NotNull;
    using ::testing::InvokeWithoutArgs;
    using ::testing::Return;
    using ::testing::SetArgPointee;
    using ::testing::ElementsAreArray;
    using ::testing::NiceMock;

    std::vector<std::string> expectedGroups = {
        "group1", "group2"
    };

    auto groupsDeleter = [] (char **groups) {
        if (groups == nullptr)
            return;
        for (size_t i = 0; groups[i] != nullptr; ++i)
            free(groups[i]);
        free(groups);
    };

    std::unique_ptr<char *, decltype(groupsDeleter)> groups(
        static_cast<char **>(calloc(expectedGroups.size(), sizeof(char *))),
        groupsDeleter);

    if (groups == nullptr)
        throw std::bad_alloc();

    auto freeGroups = [&groups] () {
        groups.reset();
    };

    size_t i = 0;
    for (const auto &g : expectedGroups) {
        groups.get()[i] = strdup(g.c_str());
        if (groups.get()[i] == nullptr)
            throw std::bad_alloc();
        ++i;
    }

    NiceMock<FakeSecurityManagerWrapper> smApi;

    const int SECURITY_MANAGER_SUCCESS_CONST = 42;

    ON_CALL(smApi, SECURITY_MANAGER_SUCCESS_CONST())
        .WillByDefault(Return(SECURITY_MANAGER_SUCCESS_CONST));

    EXPECT_CALL(smApi, security_manager_groups_get(NotNull(), NotNull()))
        .WillOnce(DoAll(SetArgPointee<0>(groups.get()),
                        SetArgPointee<1>(expectedGroups.size()),
                        Return(SECURITY_MANAGER_SUCCESS_CONST)));
    EXPECT_CALL(smApi, security_manager_groups_free(groups.get(), expectedGroups.size()))
        .WillOnce(InvokeWithoutArgs(freeGroups));

    SecurityManager::DataProvider dp(smApi);

    EXPECT_THAT(dp.getResourceGroups(), ElementsAreArray(expectedGroups));
}

