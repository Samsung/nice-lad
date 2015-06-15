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
 * @file        tests/Audit/parser.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Audit/Parser.h>
#include <Audit/ErrorException.h>

#include "FakeAuparseWrapper.h"

class ParserTest : public ::testing::Test {
public:
    void SetUp(void) {
        using ::testing::NotNull;

        EXPECT_CALL(m_auparseApi, auparse_add_callback(NotNull()));
    }

    void createEventExpectations(const Audit::Parser::Event &expectedEvent) {
        using ::testing::InSequence;
        using ::testing::Return;

        InSequence seq;
        if (expectedEvent.size() == 0)
            EXPECT_CALL(m_auparseApi, auparse_first_record()).WillOnce(Return(0));

        for (auto recIter = expectedEvent.begin(); recIter != expectedEvent.end(); ++recIter) {
            const auto &rec = *recIter;

            if (recIter == expectedEvent.begin())
                EXPECT_CALL(m_auparseApi, auparse_first_record()).WillOnce(Return(1));
            else
                EXPECT_CALL(m_auparseApi, auparse_next_record()).WillOnce(Return(1));

            for (auto fIter = rec.begin(); fIter != rec.end(); ++fIter) {
                const auto &field = *fIter;

                if (fIter == rec.begin())
                    EXPECT_CALL(m_auparseApi, auparse_first_field()).WillOnce(Return(1));
                else
                    EXPECT_CALL(m_auparseApi, auparse_next_field()).WillOnce(Return(1));

                EXPECT_CALL(m_auparseApi, auparse_get_field_name())
                    .WillOnce(Return(field.first.c_str()));
                EXPECT_CALL(m_auparseApi, auparse_interpret_field())
                    .WillOnce(Return(field.second.c_str()));
            }
            EXPECT_CALL(m_auparseApi, auparse_next_field()).WillOnce(Return(0));
        }

        // Only expect auparse_next_record(), if auparse_first_record() was >= 0
        if (expectedEvent.size() > 0)
            EXPECT_CALL(m_auparseApi, auparse_next_record()).WillOnce(Return(0));
    }

protected:
    FakeAuparseWrapper m_auparseApi;
};

/**
 * @brief   Parser should call auparse_flush_feed()
 * @test    Scenario:
 * - call Audit::Parser::flush()
 * - check if auparse_flush_feed() API was called
 */
TEST_F(ParserTest, flush) {
    EXPECT_CALL(m_auparseApi, auparse_flush_feed());

    Audit::Parser parser(m_auparseApi);
    parser.flush();
}

/**
 * @brief   Parser should call auparse_feed()
 * @test    Scenario:
 * - call Audit::Parser::feed() with some data
 * - check if auparse_feed() API was called with proper args
 */
TEST_F(ParserTest, feed) {
    using ::testing::Return;

    const char *data = "test data";
    const size_t dataLen = strlen(data);

    EXPECT_CALL(m_auparseApi, auparse_feed(data, dataLen)).WillOnce(Return(0));

    Audit::Parser parser(m_auparseApi);
    parser.feed(data, dataLen);
}

/**
 * @brief   Parser throw Audit::ErrorException on auparse_* functions failures
 * @test    Scenario:
 * - call Audit::Parser::feed()
 * - make auparse_feed() return error
 * - Make sure, parser throws Audit::ErrorException
 * - call Audit::Parser::flush()
 * - make auparse_flush_feed() return error
 * - Make sure, parser throws Audit::ErrorException
 */
TEST_F(ParserTest, auparse_x_errors) {
    using ::testing::Return;

    Audit::Parser parser(m_auparseApi);
    EXPECT_CALL(m_auparseApi, auparse_feed("test", 4)).WillOnce(Return(-1));
    EXPECT_CALL(m_auparseApi, auparse_flush_feed()).WillOnce(Return(-1));
    ASSERT_THROW(parser.feed("test", 4), Audit::ErrorException);
    ASSERT_THROW(parser.flush(), Audit::ErrorException);
}

/**
 * @brief   Parser should call auparse_first_record()
 * @test    Scenario:
 * - call Audit::Parser::handleEvent()
 * - check if auparse_first_record() API was called (return 0 for no records)
 * - Make sure no other API functions are called
 */
TEST_F(ParserTest, handle_no_records) {
    using ::testing::Return;

    EXPECT_CALL(m_auparseApi, auparse_first_record()).WillOnce(Return(0));

    Audit::Parser parser(m_auparseApi);
    parser.handleEvent();
}

/**
 * @brief   Parser should call auparse_first_record()
 * @test    Scenario:
 * - (1) call Audit::Parser::handleEvent()
 * - (2) check if auparse_first_record() API was called (return 0 for no records)
 * - Repeat (1) and (2)
 * - (3) Make sure no other API functions are called
 */
TEST_F(ParserTest, handle_no_records_twice) {
    using ::testing::Return;

    EXPECT_CALL(m_auparseApi, auparse_first_record())
        .Times(2).WillRepeatedly(Return(0));

    Audit::Parser parser(m_auparseApi);
    parser.handleEvent();
    parser.handleEvent();
}

/**
 * @brief   Parser should read event with single, empty record
 * @test    Scenario:
 * - call Audit::Parser::handleEvent()
 * - return single record with no fields
 * - Make sure, parser reports an event correctly
 */
TEST_F(ParserTest, handle_one_record_empty) {
    using ::testing::InSequence;
    using ::testing::Return;

    using Event = Audit::Parser::Event;
    using Record = Audit::Parser::Record;

    {
        InSequence seq;
        EXPECT_CALL(m_auparseApi, auparse_first_record()).WillOnce(Return(1));
        EXPECT_CALL(m_auparseApi, auparse_first_field()).WillOnce(Return(0));
        EXPECT_CALL(m_auparseApi, auparse_next_record()).WillOnce(Return(0));
    }

    Audit::Parser parser(m_auparseApi);

    Event actualEvent;
    Event expectedEvent = {{
        Record()
    }};

    parser.onEvent.connect([&actualEvent] (const Event &ev) {
       actualEvent = ev;
    });

    parser.handleEvent();

    ASSERT_EQ(expectedEvent, actualEvent);
}

/**
 * @brief   Parser should read event with single record
 * @test    Scenario:
 * - call Audit::Parser::handleEvent()
 * - return an event with two records with some fields (key=value)
 * - Make sure, parser reports the event correctly
 */
TEST_F(ParserTest, handle_one_record) {
    using Event = Audit::Parser::Event;
    using Record = Audit::Parser::Record;

    Event actualEvent;
    Event expectedEvent = {
        Record{ { "r1k1", "r1v1" }, { "r1k2", "r1v2" } },
        Record{ { "r2k1", "r2v1" }, { "r2k2", "r2v2" }, { "r2k3", "r2v3" } },
    };
    createEventExpectations(expectedEvent);

    Audit::Parser parser(m_auparseApi);
    parser.onEvent.connect([&actualEvent] (const Event &ev) {
       actualEvent = ev;
    });

    parser.handleEvent();
    ASSERT_EQ(expectedEvent, actualEvent);
}

// TODO: Add some tests with errors on event parsing
