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

#include "testconfigparser.h"
#include "common/configparser.h"

#ifndef TEST_DATADIR
#define TEST_DATADIR ""
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(TestConfigParser);

void TestConfigParser::testInit()
{
    cf::ConfigParser p;
    CPPUNIT_ASSERT_THROW(p.load("unknown"), std::exception);

    p.load(TEST_DATADIR "/test.json");

    CPPUNIT_ASSERT(p.getBool("debug.color"));

    // check types
    CPPUNIT_ASSERT(p.getBool("bool"));
    CPPUNIT_ASSERT_EQUAL(p.getFloat("float"), 1.234f);
    CPPUNIT_ASSERT_EQUAL(p.getInt("int"), int(-100));
    CPPUNIT_ASSERT_EQUAL(p.getString("string"), std::string("test string"));
}
