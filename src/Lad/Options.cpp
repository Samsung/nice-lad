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
 * @file        src/Lad/Options.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#if defined(WITH_SECURITY_MANAGER)
#include <SecurityManager/DataProvider.h>
#include <SecurityManager/SecurityManagerWrapper.h>
#else
#include <Lad/DummyDataProvider.h>
#endif

#if defined(WITH_JOURNALD)
#include <Systemd/DataCollector.h>
#else
#include <Lad/SyslogDataCollector.h>
#endif

#include "Options.h"

namespace Lad {

namespace Options {

namespace {

#if defined(WITH_JOURNALD)
    Systemd::DataCollector s_dataCollector;
#else
    Lad::SyslogDataCollector s_dataCollector;
#endif

#if defined(WITH_SECURITY_MANAGER)
    SecurityManager::SecurityManagerWrapper s_smApi;
    SecurityManager::DataProvider s_dataProvider(s_smApi);
#else
    Lad::DummyDataProvider s_dataProvider;
#endif

} /* namespace */


DataCollector &dataCollector(void) {
    return s_dataCollector;
}

DataProvider &dataProvider(void) {
    return s_dataProvider;
}

} /* namespace Options */

} /* namespace Lad */
