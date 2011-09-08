/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include <iostream>
#include <boost/assign.hpp>

#include "testenvpaths.h"
#include "common/envpaths.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestEnvPaths);

using namespace cf;
using namespace boost::assign;

void TestEnvPaths::testCommon()
{
    PathList p = envPaths();

    CPPUNIT_ASSERT(!p.empty());

#ifndef _WIN32
    CPPUNIT_ASSERT(findInEnvPath("ls"));
    CPPUNIT_ASSERT(findInEnvPath("./ls"));
    CPPUNIT_ASSERT(findInEnvPath("../bin/ls"));
#else
    CPPUNIT_ASSERT(findInEnvPath("notepad.exe"));
#endif
}

void TestEnvPaths::testFindInEnvPath() {

}

void TestEnvPaths::testFindInPaths() {
#ifndef _WIN32
#ifdef __APPLE__
    PathList paths;
    CPPUNIT_ASSERT(!findInPaths("ls", paths));
    paths.push_back(".");
    CPPUNIT_ASSERT(!findInPaths("ls", paths));
    paths.push_back("/.fseventsd");
    CPPUNIT_ASSERT(!findInPaths("ls", paths));
    paths.push_back("/bin");
    CPPUNIT_ASSERT(findInPaths("ls", paths));

    std::string path;
    CPPUNIT_ASSERT(findInPaths("ls", paths, &path));
    CPPUNIT_ASSERT_EQUAL(path, std::string("/bin/ls"));

    paths.clear();
    PathList files;
    files += "notexists", "ls";

    CPPUNIT_ASSERT(!findInPaths(files, paths));
    paths.push_back("/.fseventsd");
    CPPUNIT_ASSERT(!findInPaths(files, paths));
    paths.push_back("/bin");
    CPPUNIT_ASSERT(findInPaths(files, paths));
    CPPUNIT_ASSERT(findInPaths(files, paths, &path));
    CPPUNIT_ASSERT_EQUAL(path, std::string("/bin/ls"));
    files.clear();
    files += "echo", "cat";
    CPPUNIT_ASSERT(findInPaths(files, paths, &path));
    CPPUNIT_ASSERT_EQUAL(path, std::string("/bin/echo"));
#endif
#else
    PathList paths;
    paths.push_back("/Windows/System32");
    CPPUNIT_ASSERT(findInPaths("notepad.exe", paths));
#endif
}
