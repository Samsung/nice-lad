# Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#
# @file        src/CMakeLists.txt
# @author      Aleksander Zdyb <a.zdyb@samsung.com>
#

OPTION(WITH_SECURITY_MANAGER "Use Security Manager to obtain resource groups" OFF)
OPTION(WITH_JOURNALD "Use journald to log denials. Syslog will be used otherwise" OFF)

IF(WITH_JOURNALD)
    PKG_CHECK_MODULES(journald REQUIRED libsystemd-journal)
ELSE(WITH_JOURNALD) # journald may still be possibly used for informational logging
    PKG_CHECK_MODULES(journald QUIET libsystemd-journal)
ENDIF(WITH_JOURNALD)

IF(journald_FOUND)
    ADD_DEFINITIONS("-DWITH_JOURNALD")
ENDIF(journald_FOUND)

IF(WITH_SECURITY_MANAGER)
    PKG_CHECK_MODULES(security_manager REQUIRED security-manager)
    ADD_DEFINITIONS("-DWITH_SECURITY_MANAGER")
ENDIF(WITH_SECURITY_MANAGER)

FIND_PACKAGE(Boost 1.57 REQUIRED)

PKG_CHECK_MODULES(audit
    REQUIRED
    audit>=2.4.2
    auparse>=2.4.2
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${audit_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${journald_INCLUDE_DIRS}
    ${security_manager_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
    ${audit_LIBRARY_DIRS}
    ${journald_LIBRARY_DIRS}
    ${security_manager_LIBRARY_DIRS}
)

SET(SOURCE_FILES
    main.cpp
    Audit/Auditctl.cpp
    Audit/AuditWrapper.cpp
    Audit/AuparseSourceFeedWrapper.cpp
    Audit/AuparseWrapper.cpp
    Audit/Parser.cpp
    Audit/SyscallRuleData.cpp
    Lad/AuditEventHandler.cpp
    Lad/AuditRulesPopulator.cpp
    Lad/Options.cpp
    Log/log.cpp
    Utils/Feed.cpp
    Utils/SignalFd.cpp
)

IF(WITH_JOURNALD)
SET(SOURCE_FILES
    ${SOURCE_FILES}
    Systemd/DataCollector.cpp
)
ELSE(WITH_JOURNALD)
SET(SOURCE_FILES
    ${SOURCE_FILES}
    Lad/SyslogDataCollector.cpp
)
ENDIF(WITH_JOURNALD)

IF(WITH_SECURITY_MANAGER)
SET(SOURCE_FILES
    ${SOURCE_FILES}
    SecurityManager/DataProvider.cpp
    SecurityManager/SecurityManagerWrapper.cpp
)
ELSE(WITH_SECURITY_MANAGER)
SET(SOURCE_FILES
    ${SOURCE_FILES}
    Lad/DummyDataProvider.cpp
)
ENDIF(WITH_SECURITY_MANAGER)

ADD_EXECUTABLE(${TARGET_NICE_LAD} ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(
    ${TARGET_NICE_LAD}
    ${audit_LIBRARIES}
    ${Boost_LIBRARIES}
    ${journald_LIBRARIES}
    ${security_manager_LIBRARIES}
    )

INSTALL(TARGETS ${TARGET_NICE_LAD} DESTINATION ${SBIN_DIR})
