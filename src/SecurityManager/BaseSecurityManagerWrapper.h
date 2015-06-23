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
 * @file        src/SecurityManager/BaseSecurityManagerWrapper.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_SECURITYMANAGER_BASESECURITYMANAGERWRAPPER_H
#define SRC_SECURITYMANAGER_BASESECURITYMANAGERWRAPPER_H

namespace SecurityManager {

class BaseSecurityManagerWrapper {
public:
    virtual ~BaseSecurityManagerWrapper() = default;
    virtual int security_manager_groups_get(char ***groups, size_t *groups_count) = 0;
    virtual void security_manager_groups_free(char **groups, size_t groups_count) = 0;

    virtual int SECURITY_MANAGER_SUCCESS_CONST(void) const = 0;
};

} /* namespace SecurityManager */

#endif /* SRC_SECURITYMANAGER_BASESECURITYMANAGERWRAPPER_H */
