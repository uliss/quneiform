/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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
#include "testhelper.h"
#include <common/helper.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestHelper);

using namespace cf;

void TestHelper::testBaseName() {
    CPPUNIT_ASSERT_EQUAL(std::string(""), baseName(""));
    CPPUNIT_ASSERT_EQUAL(std::string("filename.jpg"), baseName("/usr/lib/filename.jpg"));
    CPPUNIT_ASSERT_EQUAL(std::string("lib"), baseName("/usr/lib"));
    CPPUNIT_ASSERT_EQUAL(std::string("usr"), baseName("/usr/"));
    CPPUNIT_ASSERT_EQUAL(std::string("usr"), baseName("usr/"));
    CPPUNIT_ASSERT_EQUAL(std::string("usr"), baseName("usr"));
    CPPUNIT_ASSERT_EQUAL(std::string("/"), baseName("/"));
    CPPUNIT_ASSERT_EQUAL(std::string("."), baseName("."));
    CPPUNIT_ASSERT_EQUAL(std::string(".."), baseName(".."));
}

void TestHelper::testEscapeHtmlSpecialChars() {
    CPPUNIT_ASSERT_EQUAL(std::string(""), escapeHtmlSpecialChars(""));
    CPPUNIT_ASSERT_EQUAL(std::string("abcde"), escapeHtmlSpecialChars("abcde"));
    CPPUNIT_ASSERT_EQUAL(std::string("абвгд"), escapeHtmlSpecialChars("абвгд"));
    CPPUNIT_ASSERT_EQUAL(std::string("&lt;"), escapeHtmlSpecialChars("<"));
    CPPUNIT_ASSERT_EQUAL(std::string("&gt;"), escapeHtmlSpecialChars(">"));
    CPPUNIT_ASSERT_EQUAL(std::string("&quot;"), escapeHtmlSpecialChars("\""));
    CPPUNIT_ASSERT_EQUAL(std::string("&amp;"), escapeHtmlSpecialChars("&"));
    CPPUNIT_ASSERT_EQUAL(std::string("&apos;"), escapeHtmlSpecialChars("'"));
    CPPUNIT_ASSERT_EQUAL(std::string("&amp;&lt;abc&gt;"), escapeHtmlSpecialChars("&<abc>"));
}
