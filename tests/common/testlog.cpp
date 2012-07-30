/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include "testlog.h"
#include "common/log.h"
#include "common/rect.h"
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestLog);

void TestLog::testLog()
{
    cfTrace() << "trace" << "output";
    cfDebug() << "debug";
    cfInfo() << "info";
    cfWarning() << "warning";
    cfError() << "error";
    cfFatal() << "fatal";

    cfTrace(cf::MODULE_CCOM) << cf::Rect();
    cfDebug(cf::MODULE_CIMAGE) << cf::Point();

    cf::Logger::setMessageHandler(&cf::fileMessageHandler);
    cfInfo() << "test log info message";
    cfInfo() << "test log info message";
    cf::Logger::setMessageHandler(0);
}

void TestLog::testLogConfig()
{
    using namespace cf;
    LoggerConfig conf;
    conf.enableLog(MODULES_ALL, MSG_INFO);
    CPPUNIT_ASSERT(conf.isEnabled(MODULES_ALL, MSG_INFO));
    CPPUNIT_ASSERT(conf.isEnabled(MODULE_RSTR, MSG_INFO));

    conf.disableLog(MODULES_ALL, MSG_DEBUG);
    CPPUNIT_ASSERT(!conf.isEnabled(MODULE_RSTR, MSG_DEBUG));

    cfInfo() << "only ERROR log enabled for RSTR";
    Logger::config().disableLog(MODULE_RSTR, MSG_TRACE);
    Logger::config().disableLog(MODULE_RSTR, MSG_DEBUG);
    Logger::config().disableLog(MODULE_RSTR, MSG_INFO);
    Logger::config().disableLog(MODULE_RSTR, MSG_WARNING);
    Logger::config().disableLog(MODULE_RSTR, MSG_FATAL);
    Logger::config().enableLog(MODULE_RSTR, MSG_ERROR);
    cfTrace(MODULE_RSTR) << "SHOULD NOT SEE THIS MESSAGE!";
    cfDebug(MODULE_RSTR) << "SHOULD NOT SEE THIS MESSAGE!";
    cfInfo(MODULE_RSTR)  << "SHOULD NOT SEE THIS MESSAGE!";
    cfWarning(MODULE_RSTR) << "SHOULD NOT SEE THIS MESSAGE!";
    cfFatal(MODULE_RSTR) << "SHOULD NOT SEE THIS MESSAGE!";
    cfError(MODULE_RSTR) << "Test error message. No other messages shoud be above!";

    Logger::config().disableColorize(MODULES_ALL);
    cfInfo() << "Colorize disabled";
    Logger::config().enableColorize(MODULES_ALL);
    cfInfo() << "Colorize enabled";
}
