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
    CPPUNIT_ASSERT(Datafile::mainPath().empty());
    CPPUNIT_ASSERT_EQUAL(Datafile::searchMask(),
                         DATAFILE_MAIN_PATH | DATAFILE_ENVIRONMENT | DATAFILE_INSTALL_PATH);
    CPPUNIT_ASSERT(!Datafile::exists("abcde"));
    CPPUNIT_ASSERT(Datafile::fullPath("abcde").empty());
}

void TestDatafile::testSetPath()
{
    CPPUNIT_ASSERT(Datafile::mainPath().empty());
    Datafile::setMainPath("/some/path");
    CPPUNIT_ASSERT_EQUAL(std::string("/some/path"), Datafile::mainPath());
    Datafile::setMainPath("");
}

void TestDatafile::testEnvPath()
{
#ifndef WIN32
    setenv("CF_DATADIR", "/some/path", TRUE);
    CPPUNIT_ASSERT_EQUAL(std::string("/some/path"), Datafile::envPath());
    unsetenv("CF_DATADIR");
    CPPUNIT_ASSERT(Datafile::envPath().empty());
#endif
}

void TestDatafile::testSearchPath()
{
#ifndef WIN32
    Datafile::setMainPath("/bin");
    CPPUNIT_ASSERT(Datafile::exists("sh"));
    CPPUNIT_ASSERT_EQUAL(std::string("/bin/sh"), Datafile::fullPath("sh"));

    Datafile::setMainPath("/bin/");
    CPPUNIT_ASSERT(Datafile::exists("sh"));
    CPPUNIT_ASSERT_EQUAL(std::string("/bin/sh"), Datafile::fullPath("sh"));

    int old_mask = Datafile::searchMask();
    Datafile::setSearchMask(0);
    CPPUNIT_ASSERT(!Datafile::exists("sh"));
    CPPUNIT_ASSERT(Datafile::fullPath("sh").empty());
    Datafile::setSearchMask(old_mask);

    Datafile::setMainPath("");
    CPPUNIT_ASSERT(!Datafile::exists("sh"));
    setenv("CF_DATADIR", "/bin", TRUE);
    CPPUNIT_ASSERT(Datafile::exists("sh"));
    CPPUNIT_ASSERT_EQUAL(std::string("/bin/sh"), Datafile::fullPath("sh"));
    Datafile::setSearchMask(0);
    CPPUNIT_ASSERT(!Datafile::exists("sh"));
    Datafile::setSearchMask(old_mask);
    unsetenv("CF_DATADIR");
#endif
}
