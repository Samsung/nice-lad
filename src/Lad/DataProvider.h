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
 * @file        src/Lad/DataProvider.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_LAD_DATAPROVIDER_H
#define SRC_LAD_DATAPROVIDER_H

#include <list>
#include <string>

namespace Lad {

class DataProvider {
public:
    typedef std::list<std::string> GroupsList;

    virtual ~DataProvider() = default;

    virtual GroupsList getResourceGroups(void) = 0;
};

} /* namespace Lad */

#endif /* SRC_LAD_DATAPROVIDER_H */
