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
 * @file        tests/Lad/audit_event_handler.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Lad/AuditEventHandler.h>

/**
 * @brief   AuditEventHandler should NOT signal on unsupported event
 * @test    Scenario:
 * - feed random event to AuditEventHandler
 * - check if AuditEventHandler::logDenial was NOT called
 */
TEST(AuditEventHandler, unknown_event) {
    Lad::AuditEventHandler eventHandler;

    using Event = Audit::Parser::Event;
    using Record = Audit::Parser::Record;

    Event event = {
        Record{ { "type", "RANDOM_EVENT" } }
    };

    bool called = false;

    eventHandler.onLogDenial.connect([&called] (const std::string &) {
        called = true;
    });

    eventHandler.handleEvent(event);

    ASSERT_FALSE(called);
}

/**
 * @brief   AuditEventHandler should signal on AVC event
 * @test    Scenario:
 * - feed AVC event to AuditEventHandler
 * - check if AuditEventHandler::logDenial was signaled with a proper arg
 */
TEST(AuditEventHandler, avc) {
    Lad::AuditEventHandler eventHandler;

    using Event = Audit::Parser::Event;
    using Record = Audit::Parser::Record;

    Event event = {
        Record{ { "ino", "012345" }, { "type", "AVC" }, { "lsm", "SMACK" },
                { "fn", "smack_inode_permission" }, { "name", "s" }, { "dev", "\"ubda\"" },
                { "action", "denied" }, { "subject", "\"_\"" }, { "pid", "1234" },
                { "object", "\"obj_label\"" }, { "comm", "cat" }, { "requested", "r" } }
    };

    std::string actualLog;

    eventHandler.onLogDenial.connect([&actualLog] (const std::string &log) {
        actualLog = log;
    });

    eventHandler.handleEvent(event);

    ASSERT_EQ(actualLog, "ACCESS DENIED ON SMACK object=\"obj_label\" subject=\"_\" access=r");
}

/**
 * @brief   AuditEventHandler should print [unknown] if some field is missing
 * @test    Scenario:
 * - feed AVC event to AuditEventHandler
 * - check if AuditEventHandler::logDenial was signaled with a proper arg
 */
TEST(AuditEventHandler, avc_missing_fields) {
    Lad::AuditEventHandler eventHandler;

    using Event = Audit::Parser::Event;
    using Record = Audit::Parser::Record;

    Event event = {
        Record{ { "ino", "012345" }, { "type", "AVC" }, { "lsm", "SMACK" },
                { "fn", "smack_inode_permission" }, { "name", "s" }, { "dev", "\"ubda\"" },
                { "action", "denied" }, { "subject", "\"_\"" }, { "pid", "1234" },
                { "comm", "cat" }, { "requested", "r" } }
    };

    std::string actualLog;

    eventHandler.onLogDenial.connect([&actualLog] (const std::string &log) {
        actualLog = log;
    });

    eventHandler.handleEvent(event);

    ASSERT_EQ(actualLog, "ACCESS DENIED ON SMACK object=[unknown] subject=\"_\" access=r");
}

/**
 * @brief   AuditEventHandler should signal on SYSCALL event
 * @test    Scenario:
 * - feed SYSCALL event to AuditEventHandler
 * - check if AuditEventHandler::logDenial was signaled with a proper arg
 */
TEST(AuditEventHandler, syscall) {
    Lad::AuditEventHandler eventHandler;

    using Event = Audit::Parser::Event;
    using Record = Audit::Parser::Record;

    Event event = {
        Record{ { "key", "test" }, { "exe", "/sbin/audit-test" }, { "ses", "1" },
                { "items", "2" }, { "a3", "0x7fbf8989d0" }, { "a1", "O_WRONLY|O_CREAT|O_TRUNC" },
                { "comm", "audit-test" }, { "a0", "0x7fbf898f04" }, { "uid", "guest" },
                { "exit", "-13(Permission denied)" }, { "gid", "guest" }, { "syscall", "open" },
                { "success", "no" }, { "suid", "guest" }, { "subj", "_" }, { "arch", "x86_64" },
                { "ppid", "1234" }, { "fsuid", "guest" }, { "pid", "1235" },
                { "egid", "guest" }, { "sgid", "guest" }, { "a2", "0600" },
                { "auid", "root" }, { "euid", "guest" }, { "fsgid", "guest" },
                { "type", "SYSCALL" }, { "tty", "(none)" } },
        Record{ { "cwd", "/root/try" }, { "type", "CWD" } },
        Record{ { "nametype", "PARENT" }, { "type", "PATH" }, { "item", "0" },
                { "inode", "393236" }, { "name", "/tmp/" }, { "mode", "dir,sticky,777" },
                { "ogid", "root" }, { "rdev", "00:00" }, { "ouid", "root" }, { "dev", "62:00" },
                { "obj", "*" } },
        Record{ { "nametype", "NORMAL" }, { "type", "PATH" }, { "item", "1" },
                { "inode", "393380" }, { "name", "/tmp/test" }, { "mode", "file,600" },
                { "ogid", "unknown(1234)" }, { "rdev", "00:00" }, { "ouid", "root" },
                { "dev", "62:00" }, { "obj", "obj_label" } },
        Record{ { "proctitle", "audit-test -w /tmp/test" }, { "type", "PROCTITLE" } }
    };

    std::string actualLog;

    eventHandler.onLogDenial.connect([&actualLog] (const std::string &log) {
        actualLog = log;
    });

    eventHandler.handleEvent(event);

    ASSERT_EQ(actualLog, "ACCESS DENIED ON SYSCALL syscall=open filename=/tmp/test"
                         " exit=-13(Permission denied) gid=unknown(1234) object=obj_label"
                         " subject=_");
}

/**
 * @brief   AuditEventHandler should signal on Cynara event
 * @test    Scenario:
 * - feed Cynara event to AuditEventHandler
 * - check if AuditEventHandler::logDenial was signaled with a proper arg
 */
TEST(AuditEventHandler, cynara) {
    Lad::AuditEventHandler eventHandler;

    using Event = Audit::Parser::Event;
    using Record = Audit::Parser::Record;

    Event event = {
        Record{ { "type", "TRUSTED_APP" }, { "pid", "32532" }, { "uid", "cynara" },
                { "auid", "unset" }, { "ses", "unset" }, { "subj", "System" }, { "app", "Cynara" },
                { "client", "\"__test_client\"" }, { "user", "\"__test_user\"" },
                { "privilege", "\"http://tizen.org/privilege/account.read\"" },
                { "policy_type", "0" }, { "exe", "/usr/bin/cynara" }, { "hostname", "?" },
                { "addr", "?" }, { "terminal", "?" }, { "res", "failed" } }
    };

    std::string actualLog;

    eventHandler.onLogDenial.connect([&actualLog] (const std::string &log) {
        actualLog = log;
    });

    eventHandler.handleEvent(event);

    ASSERT_EQ(actualLog, "ACCESS DENIED ON CYNARA client=\"__test_client\" user=\"__test_user\""
                         " privilege=\"http://tizen.org/privilege/account.read\"");
}

/**
 * @brief   AuditEventHandler should signal on Netfilter event
 * @test    Scenario:
 * - feed Netfilter event to AuditEventHandler
 * - check if AuditEventHandler::logDenial was signaled with a proper arg
 */
TEST(AuditEventHandler, netfilter) {
    Lad::AuditEventHandler eventHandler;

    using Event = Audit::Parser::Event;
    using Record = Audit::Parser::Record;

    Event event = {
        Record{ { "type", "NETFILTER_PKT" }, { "action", "2" }, { "hook", "3" }, { "len", "60" },
                { "inif", "?" }, { "outif", "eth0" }, { "mark", "0x3" }, { "saddr", "10.0.2.16" },
                { "daddr", "198.145.20.7" }, { "ipid", "45575" }, { "proto", "tcp" },
                { "sport", "54460" }, { "dport", "443" }, { "obj", "User" } }
    };

    std::string actualLog;

    eventHandler.onLogDenial.connect([&actualLog] (const std::string &log) {
        actualLog = log;
    });

    eventHandler.handleEvent(event);

    ASSERT_EQ(actualLog, "ACCESS DENIED ON NETFILTER obj=User outif=eth0 proto=tcp saddr=10.0.2.16"
                         " sport=54460 daddr=198.145.20.7 dport=443");
}

