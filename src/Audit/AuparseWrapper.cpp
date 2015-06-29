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
 * @file        src/Audit/AuparseWrapper.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <auparse.h>

#include "AuparseWrapper.h"
#include "ErrorException.h"

namespace Audit {

static void handleEvent(auparse_state_t *au, auparse_cb_event_t cb_event_type, void *user_data) {
    if (cb_event_type != AUPARSE_CB_EVENT_READY)
        return;

    auto eventHandler = static_cast<EventHandler *>(user_data);
    eventHandler->handleEvent();
}

AuparseWrapper::AuparseWrapper(ausource_t source, const void *b) {
    auto auParseState = ::auparse_init(source, b);

    if (auParseState == nullptr) {
        throw ErrorException("Could not initialize audit parser");
    }

    m_auParseState = decltype(m_auParseState)(auParseState, ::auparse_destroy);
}

int AuparseWrapper::auparse_feed(const char *data, size_t data_len) {
    return ::auparse_feed(m_auParseState.get(), data, data_len);
}

int AuparseWrapper::auparse_flush_feed(void) {
    return ::auparse_flush_feed(m_auParseState.get());
}

int AuparseWrapper::auparse_feed_has_data(void) {
    return ::auparse_feed_has_data(m_auParseState.get());
}

void AuparseWrapper::auparse_add_callback(EventHandler *eventHandler) {
    ::auparse_add_callback(m_auParseState.get(), handleEvent, eventHandler, nullptr);
}

int AuparseWrapper::auparse_first_record(void) {
    return ::auparse_first_record(m_auParseState.get());
}

int AuparseWrapper::auparse_next_record(void) {
    return ::auparse_next_record(m_auParseState.get());
}

int AuparseWrapper::auparse_get_type(void) {
    return ::auparse_get_type(m_auParseState.get());
}

const char *AuparseWrapper::auparse_get_type_name(void) {
    return ::auparse_get_type_name(m_auParseState.get());
}

int AuparseWrapper::auparse_first_field(void) {
    return ::auparse_first_field(m_auParseState.get());
}

int AuparseWrapper::auparse_next_field(void) {
    return ::auparse_next_field(m_auParseState.get());
}

const char *AuparseWrapper::auparse_get_field_name(void) {
    return ::auparse_get_field_name(m_auParseState.get());
}

int AuparseWrapper::auparse_get_field_type(void) {
    return ::auparse_get_field_type(m_auParseState.get());
}

const char *AuparseWrapper::auparse_interpret_field(void) {
    return ::auparse_interpret_field(m_auParseState.get());
}

} /* namespace Audit */
