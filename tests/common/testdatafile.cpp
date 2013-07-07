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

#include <stdlib.h>

#include "testdatafile.h"
#include "common/datafile.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestDatafile);

using namespace cf;

void TestDatafile::testInit()
{
    CPPUNIT_ASSERT(Datafile::instance().path().empty());
    CPPUNIT_ASSERT_EQUAL(Datafile::instance().searchMask(),
                         DATAFILE_PATH | DATAFILE_ENVIRONMENT | DATAFILE_INSTALL_PATH);
    CPPUNIT_ASSERT(!Datafile::instance().datafileExists("abcde"));
}

void TestDatafile::testSetPath()
{
    CPPUNIT_ASSERT(Datafile::instance().path().empty());
    Datafile::instance().setPath("/some/path");
    CPPUNIT_ASSERT_EQUAL(std::string("/some/path"), Datafile::instance().path());
    Datafile::instance().setPath("");
}

void TestDatafile::testEnvPath()
{
    setenv("CF_DATADIR", "/some/path", TRUE);
    CPPUNIT_ASSERT_EQUAL(std::string("/some/path"), Datafile::instance().envPath());
    unsetenv("CF_DATADIR");
    CPPUNIT_ASSERT(Datafile::instance().envPath().empty());
}

void TestDatafile::testSearchPath()
{
#ifndef WIN32
    Datafile::instance().setPath("/bin");
    CPPUNIT_ASSERT(Datafile::instance().datafileExists("sh"));

    Datafile::instance().setPath("/bin/");
    CPPUNIT_ASSERT(Datafile::instance().datafileExists("sh"));

    int old_mask = Datafile::instance().searchMask();
    Datafile::instance().setSearchMask(0);
    CPPUNIT_ASSERT(!Datafile::instance().datafileExists("sh"));
    Datafile::instance().setSearchMask(old_mask);

    Datafile::instance().setPath("");
    CPPUNIT_ASSERT(!Datafile::instance().datafileExists("sh"));
    setenv("CF_DATADIR", "/bin", TRUE);
    CPPUNIT_ASSERT(Datafile::instance().datafileExists("sh"));
    Datafile::instance().setSearchMask(0);
    CPPUNIT_ASSERT(!Datafile::instance().datafileExists("sh"));
    Datafile::instance().setSearchMask(old_mask);
    unsetenv("CF_DATADIR");
#endif
}
