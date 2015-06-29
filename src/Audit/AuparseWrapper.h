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
 * @file        src/Audit/AuparseWrapper.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_AUDIT_AUPARSEWRAPPER_H
#define SRC_AUDIT_AUPARSEWRAPPER_H

#include <functional>
#include <memory>

#include <auparse.h>

#include "BaseAuparseWrapper.h"

namespace Audit {

class AuparseWrapper: public BaseAuparseWrapper {
public:
    AuparseWrapper(ausource_t source, const void *b);
    virtual ~AuparseWrapper() = default;

    virtual int auparse_feed(const char *data, size_t data_len);
    virtual int auparse_flush_feed(void);
    virtual int auparse_feed_has_data(void);

    virtual void auparse_add_callback(EventHandler *eventHandler);

    virtual int auparse_first_record(void);
    virtual int auparse_next_record(void);

    virtual int auparse_get_type(void);
    virtual const char *auparse_get_type_name(void);

    virtual int auparse_first_field(void);
    virtual int auparse_next_field(void);

    virtual const char *auparse_get_field_name(void);
    virtual int auparse_get_field_type(void);
    virtual const char *auparse_interpret_field(void);

protected:
    typedef std::unique_ptr<auparse_state_t, std::function<void(auparse_state_t *)>>
            AuditParseStatePtr;

    AuditParseStatePtr m_auParseState = nullptr;
};

} /* namespace Audit */

#endif /* SRC_AUDIT_AUPARSEWRAPPER_H */
