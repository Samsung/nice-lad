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
 * @file        src/Audit/Parser.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <Log/log.h>

#include "ErrorException.h"
#include "Parser.h"

namespace Audit {

    Parser::Parser(BaseAuparseWrapper &auparseApi) : m_auparseApi(auparseApi) {
        auparseApi.auparse_add_callback(this);
    }

    // TODO: Consider throwing instead of silently continuing on errors
    void Parser::handleEvent() {
        Event event;

        auto hasRecords = m_auparseApi.auparse_first_record();
        while (hasRecords == 1) {
            Record rec;

            auto hasFields = m_auparseApi.auparse_first_field();
            while (hasFields == 1) {
                const auto fieldName = m_auparseApi.auparse_get_field_name();
                const auto fieldValue = m_auparseApi.auparse_interpret_field();
                if (fieldName && fieldValue)
                    rec.emplace(fieldName, fieldValue);
                hasFields = m_auparseApi.auparse_next_field();
            }

            hasRecords = m_auparseApi.auparse_next_record();
            event.push_back(std::move(rec));
        }
        this->onEvent(event);
    }

    void Parser::feed(const char *data, std::size_t size) {
        const auto ret = m_auparseApi.auparse_feed(data, size);
        if (ret != 0)
            throw ErrorException("Could not feed parser -- auparse_feed() failed");
    }

    void Parser::flush() {
        const auto ret = m_auparseApi.auparse_flush_feed();
        if (ret != 0)
            throw ErrorException("Could not flush parser -- auparse_flush_feed() failed");
    }

} /* namespace Audit */
