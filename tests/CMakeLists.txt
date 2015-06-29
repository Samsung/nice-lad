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
# @file        tests/CMakeLists.txt
# @author      Aleksander Zdyb <a.zdyb@samsung.com>
#

FIND_PACKAGE(Boost 1.57 REQUIRED)

FIND_PACKAGE(Threads REQUIRED) # Required by gmock

PKG_CHECK_MODULES(gmock QUIET gmock)

IF(NOT gmock_FOUND)
    ADD_SUBDIRECTORY(${GMOCK_ROOT} gmock)
    INCLUDE_DIRECTORIES(
        ${GMOCK_ROOT}/include
        ${GMOCK_ROOT}/gtest/include
    )
ENDIF(NOT gmock_FOUND)

SET(LAD_SRC_DIR ../src)

INCLUDE_DIRECTORIES(
    ${audit_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${gmock_INCLUDE_DIRS}
    ${LAD_SRC_DIR}
)

SET(SOURCE_FILES
    main.cpp
    Audit/auditctl.cpp
    Audit/parser.cpp
    Audit/syscall_rule_data.cpp
    Lad/audit_event_handler.cpp
    SecurityManager/data_provider.cpp

    ${LAD_SRC_DIR}/Audit/Auditctl.cpp
    ${LAD_SRC_DIR}/Audit/Parser.cpp
    ${LAD_SRC_DIR}/Audit/SyscallRuleData.cpp
    ${LAD_SRC_DIR}/Lad/AuditEventHandler.cpp
    ${LAD_SRC_DIR}/Log/log.cpp
    ${LAD_SRC_DIR}/SecurityManager/DataProvider.cpp
)

ADD_EXECUTABLE(${TARGET_NICE_LAD_TESTS} ${SOURCE_FILES})

IF(gmock_FOUND)
    TARGET_LINK_LIBRARIES(${TARGET_NICE_LAD_TESTS}
        ${CMAKE_THREAD_LIBS_INIT}
        ${gmock_LDFLAGS}
        ${gmock_LIBRARIES}
    )
ELSE(gmock_FOUND)
    TARGET_LINK_LIBRARIES(${TARGET_NICE_LAD_TESTS}
        ${CMAKE_THREAD_LIBS_INIT}
        gmock gtest
    )
ENDIF(gmock_FOUND)

INSTALL(TARGETS ${TARGET_NICE_LAD_TESTS} DESTINATION ${BIN_DIR})
