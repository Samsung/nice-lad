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
 * @file        src/SecurityManager/SecurityManagerWrapper.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <security-manager.h>

#include "SecurityManagerWrapper.h"

namespace SecurityManager {

int SecurityManagerWrapper::security_manager_groups_get(char ***groups, size_t *groups_count) {
    return ::security_manager_groups_get(groups, groups_count);
}

void SecurityManagerWrapper::security_manager_groups_free(char **groups, size_t groups_count) {
    return ::security_manager_groups_free(groups, groups_count);
}

int SecurityManagerWrapper::SECURITY_MANAGER_SUCCESS_CONST(void) const {
    return ::SECURITY_MANAGER_SUCCESS;
}

} /* namespace SecurityManager */
