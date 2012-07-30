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
}
