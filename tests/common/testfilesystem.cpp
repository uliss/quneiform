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

#include "testfilesystem.h"
#include "common/filesystem.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestFileSystem);

using namespace cf;

void TestFileSystem::testExtensions()
{
#define STR_EQ(expected, actual) CPPUNIT_ASSERT_EQUAL(std::string(expected), actual)

    STR_EQ("xml", fs::fileExtension("test.xml"));
    STR_EQ("long_extension", fs::fileExtension("test.long_extension"));
    STR_EQ("zip", fs::fileExtension("test.xml.zip"));
    STR_EQ("", fs::fileExtension("no-extension"));
    STR_EQ("zip", fs::fileExtension(".zip"));

    STR_EQ("test", fs::removeFileExtension("test.xml"));
    STR_EQ("test", fs::removeFileExtension("test.long_extension"));
    STR_EQ("test.xml", fs::removeFileExtension("test.xml.zip"));
    STR_EQ("no-extension", fs::removeFileExtension("no-extension"));
    STR_EQ("", fs::removeFileExtension(".zip"));

    STR_EQ("test.zip", fs::replaceFileExtension("test.xml", "zip"));
    STR_EQ("test.zip", fs::replaceFileExtension("test.long_extension", "zip"));
    STR_EQ("test.xml.rar", fs::replaceFileExtension("test.xml.zip", "rar"));
    STR_EQ("no-extension.zip", fs::replaceFileExtension("no-extension", "zip"));
    STR_EQ(".zip", fs::replaceFileExtension(".xml", "zip"));
}
