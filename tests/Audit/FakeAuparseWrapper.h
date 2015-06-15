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
 * @file        tests/Audit/FakeAuparseWrapper.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef TESTS_AUDIT_FAKEAUPARSEWRAPPER_H
#define TESTS_AUDIT_FAKEAUPARSEWRAPPER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Audit/BaseAuparseWrapper.h>

class FakeAuparseWrapper : public Audit::BaseAuparseWrapper {
public:
    using BaseAuparseWrapper::BaseAuparseWrapper;

    MOCK_METHOD2(auparse_feed, int(const char *data, size_t data_len));
    MOCK_METHOD0(auparse_flush_feed, int(void));
    MOCK_METHOD0(auparse_feed_has_data, int(void));
    MOCK_METHOD1(auparse_add_callback, void(Audit::EventHandler *eventHandler));

    MOCK_METHOD0(auparse_first_record, int(void));
    MOCK_METHOD0(auparse_next_record, int(void));

    MOCK_METHOD0(auparse_get_type, int(void));
    MOCK_METHOD0(auparse_get_type_name, const char *(void));

    MOCK_METHOD0(auparse_first_field, int(void));
    MOCK_METHOD0(auparse_next_field, int(void));

    MOCK_METHOD0(auparse_get_field_name, const char *(void));
    MOCK_METHOD0(auparse_get_field_type, int(void));
    MOCK_METHOD0(auparse_interpret_field, const char *(void));
};

#endif /* TESTS_AUDIT_FAKEAUPARSEWRAPPER_H */
