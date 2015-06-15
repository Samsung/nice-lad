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
 * @file        tests/main.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 * @brief       Unit-tests setup
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

int main(int argc, char** argv) {
    // Disables elapsed time by default.
    ::testing::GTEST_FLAG(print_time) = false;

    // This allows the user to override the flag on the command line.
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
