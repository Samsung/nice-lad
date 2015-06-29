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
 * @file        src/Lad/AuditEventHandler.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 */

#ifndef SRC_AUDITEVENTHANDLER_H
#define SRC_AUDITEVENTHANDLER_H

#include <string>

#include <boost/signals2.hpp>

#include <Audit/Parser.h>

namespace Lad {

class AuditEventHandler {
public:
	AuditEventHandler() = default;
	virtual ~AuditEventHandler() = default;

	// TODO: Consider using std::function as member, if the application stays single-threaded
	boost::signals2::signal<void(const std::string &)> onLogDenial;

	void handleEvent(const Audit::Parser::Event &event);

	void handleSyscall(const Audit::Parser::Event &event);
	void handleAVC(const Audit::Parser::Event &event);
	void handleCynara(const Audit::Parser::Event &event);
	void handleNetfilter(const Audit::Parser::Event &event);
};

} /* namespace Lad */

#endif /* SRC_AUDITEVENTHANDLER_H */
