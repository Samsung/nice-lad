/*
 * Copyright (c) 2014-2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Lukasz Wojciechowski <l.wojciechow@partner.samsung.com>
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
 * @file        src/Log/log.h
 * @author      Adam Malinowski <a.malinowsk2@partner.samsung.com>
 * @author      Lukasz Wojciechowski <l.wojciechow@partner.samsung.com>
 * @version     1.0
 * @brief       This file defines logging utilities.
 */

#ifndef SRC_LOG_LOG_H
#define SRC_LOG_LOG_H

#ifndef LAD_NO_LOGS
#include <sstream>
#ifdef WITH_JOURNALD
#include <systemd/sd-journal.h>
#else // WITH_JOURNALD
#include <syslog.h>
#endif // WITH_JOURNALD
#endif // LAD_NO_LOGS

#define UNUSED __attribute__((unused))

extern int __log_level;

#ifndef LAD_NO_LOGS
namespace {
    template <typename ...Args>
    void UNUSED __LOG_FUN(int level, const std::stringstream &format, Args&&... args) {
#ifdef WITH_JOURNALD
        sd_journal_print(level, format.str().c_str(), std::forward<Args>(args)...);
#else // WITH_JOURNALD
        syslog(level, format.str().c_str(), std::forward<Args>(args)...);
#endif // WITH_JOURNALD
    }

    template <>
    void UNUSED __LOG_FUN(int level, const std::stringstream &format) {
#ifdef WITH_JOURNALD
        sd_journal_print(level, "%s", format.str().c_str());
#else // WITH_JOURNALD
        syslog(level, "%s", format.str().c_str());
#endif // WITH_JOURNALD
    }
} // namespace anonymous

    #define __LOG(LEVEL, FORMAT, ...) \
        do { \
            if (LEVEL <= __log_level) { \
                std::stringstream __LOG_FORMAT; \
                __LOG_FORMAT << FORMAT; \
                __LOG_FUN(LEVEL, __LOG_FORMAT, ##__VA_ARGS__); \
            } \
        } while (0)

#else // LAD_NO_LOGS
    #define __LOG(LEVEL, ...)
#endif

#define LOGM(...)  __LOG(LOG_EMERG, __VA_ARGS__)   /* system is unusable */
#define LOGA(...)  __LOG(LOG_ALERT, __VA_ARGS__)   /* action must be taken immediately */
#define LOGC(...)  __LOG(LOG_CRIT, __VA_ARGS__)    /* critical conditions */
#define LOGE(...)  __LOG(LOG_ERR, __VA_ARGS__)     /* error conditions */
#define LOGW(...)  __LOG(LOG_WARNING, __VA_ARGS__) /* warning conditions */
#define LOGN(...)  __LOG(LOG_NOTICE, __VA_ARGS__)  /* normal but significant condition */
#define LOGI(...)  __LOG(LOG_INFO, __VA_ARGS__)    /* informational */
#define LOGD(...)  __LOG(LOG_DEBUG, __VA_ARGS__)   /* debug-level messages */

void init_log(void);

#endif /* SRC_LOG_LOG_H */
