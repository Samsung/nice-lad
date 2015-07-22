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
# @file        CMakeLists.txt
# @author      Aleksander Zdyb <a.zdyb@samsung.com>
# @author      Oskar Świtalski <o.switalski@samsung.com>
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
PROJECT(nice-lad)

OPTION(WITH_TESTS "Build tests" ON)

############################# compiler flags ##################################

SET(CMAKE_CXX_FLAGS_PROFILING  "-O0 -g -pg")
SET(CMAKE_CXX_FLAGS_DEBUG      "-O0 -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE    "-O2 -g")
SET(CMAKE_CXX_FLAGS_CCOV       "-O2 -g --coverage")

# Check for C++11 support and enable proper compilation flags
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
IF(COMPILER_SUPPORTS_CXX11)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSE()
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    IF(COMPILER_SUPPORTS_CXX0X)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    ELSE()
        MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    ENDIF()
ENDIF()

IF (CMAKE_BUILD_TYPE MATCHES "DEBUG")
    ADD_DEFINITIONS("-DBUILD_TYPE_DEBUG")
ENDIF (CMAKE_BUILD_TYPE MATCHES "DEBUG")

SET(BIN_DIR
    "${CMAKE_INSTALL_PREFIX}/bin"
    CACHE PATH
    "User executables directory"
)

SET(SBIN_DIR
    "${CMAKE_INSTALL_PREFIX}/sbin"
    CACHE PATH
    "System admin executables directory"
)

SET(SYS_CONFIG_DIR
    "${CMAKE_INSTALL_PREFIX}/etc"
    CACHE PATH
    "Read-only single-machine data directory"
)

INCLUDE(FindPkgConfig)

SET(TARGET_NICE_LAD "nice-lad")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(conf)

IF(WITH_TESTS)
    SET(TARGET_NICE_LAD_TESTS "nice-lad-tests")
    ADD_SUBDIRECTORY(tests)
ENDIF(WITH_TESTS)
