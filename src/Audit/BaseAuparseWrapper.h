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
 * @file        src/Audit/BaseAuparseWrapper.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_AUDIT_BASEAUPARSEWRAPPER_H
#define SRC_AUDIT_BASEAUPARSEWRAPPER_H

#include <cstddef>

#include "EventHandler.h"

namespace Audit {

class BaseAuparseWrapper {
public:
    virtual ~BaseAuparseWrapper() = default;

    virtual int auparse_feed(const char *data, size_t data_len) = 0;
    virtual int auparse_flush_feed(void) = 0;
    virtual int auparse_feed_has_data(void) = 0;

    virtual void auparse_add_callback(EventHandler *eventHandler) = 0;

    virtual int auparse_first_record(void) = 0;
    virtual int auparse_next_record(void) = 0;

    virtual int auparse_get_type(void) = 0;
    virtual const char *auparse_get_type_name(void) = 0;

    virtual int auparse_first_field(void) = 0;
    virtual int auparse_next_field(void) = 0;

    virtual const char *auparse_get_field_name(void) = 0;
    virtual int auparse_get_field_type(void) = 0;
    virtual const char *auparse_interpret_field(void) = 0;
};

} /* namespace Audit */

#endif /* SRC_AUDIT_BASEAUPARSEWRAPPER_H */
