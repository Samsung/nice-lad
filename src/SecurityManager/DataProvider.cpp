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
 * @file        src/SecurityManager/DataProvider.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <functional>
#include <memory>

#include <Log/log.h>

#include <SecurityManager/ErrorException.h>

#include "DataProvider.h"

namespace SecurityManager {

DataProvider::DataProvider(BaseSecurityManagerWrapper &smApi) : m_smApi(smApi) {}

Lad::DataProvider::GroupsList DataProvider::getResourceGroups(void) {
    Lad::DataProvider::GroupsList retGroups;

    char **groups = nullptr;
    size_t groupsLen = 0;

    auto getRet = m_smApi.security_manager_groups_get(&groups, &groupsLen);

    if (getRet != m_smApi.SECURITY_MANAGER_SUCCESS_CONST()) {
        throw ErrorException("Could not get groups from Security Manager");
    }

    auto groupsDeleter = std::bind(&BaseSecurityManagerWrapper::security_manager_groups_free,
                                   &m_smApi, std::placeholders::_1, groupsLen);
    std::unique_ptr<char *, std::function<void(char **)>> groupsPtr(groups, groupsDeleter);

    for (size_t i = 0; i < groupsLen; ++i) {
        retGroups.push_back(groups[i]);
    }

    return retGroups;
}

} /* namespace SecurityManager */
