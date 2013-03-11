/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include <sstream>
#include "testconsolemessages.h"
#include "common/console_messages.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestConsoleMesssages);

using namespace cf;

void TestConsoleMesssages::testMsg()
{
    std::ostringstream buf;

    console::error(buf);
    CPPUNIT_ASSERT(buf.str().find("Error") != std::string::npos);

    console::info(buf);
    CPPUNIT_ASSERT(buf.str().find("Info") != std::string::npos);

    console::warning(buf);
    CPPUNIT_ASSERT(buf.str().find("Warning") != std::string::npos);

    console::ColorMessage msg("red message", console::RED);
    buf << msg;
    CPPUNIT_ASSERT(buf.str().find("red message") != std::string::npos);
}
