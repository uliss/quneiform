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

#include <boost/current_function.hpp>
#include <streambuf>

#include "testhelper.h"
#include "common/helper.h"
#include "common/filesystem.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestHelper);

using namespace cf;

void TestHelper::testBaseName() {
    CPPUNIT_ASSERT_EQUAL(std::string(""), fs::baseName(""));
    CPPUNIT_ASSERT_EQUAL(std::string("filename.jpg"), fs::baseName("/usr/lib/filename.jpg"));
    CPPUNIT_ASSERT_EQUAL(std::string("lib"), fs::baseName("/usr/lib"));
    CPPUNIT_ASSERT_EQUAL(std::string("usr"), fs::baseName("usr"));
    CPPUNIT_ASSERT_EQUAL(std::string("/"), fs::baseName("/"));
    CPPUNIT_ASSERT_EQUAL(std::string("."), fs::baseName("."));
    CPPUNIT_ASSERT_EQUAL(std::string(".."), fs::baseName(".."));
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

void TestHelper::testStreamSize() {
    std::ostringstream os;
    CPPUNIT_ASSERT_EQUAL(streamSize(os), (size_t) 0);
    os << "test";
    CPPUNIT_ASSERT_EQUAL(streamSize(os), (size_t) 4);
    os.setstate(std::ios_base::failbit);
    CPPUNIT_ASSERT_EQUAL(streamSize(os), (size_t) 0);

    std::istringstream iempty;
    CPPUNIT_ASSERT_EQUAL(streamSize(iempty), (size_t) 0);
    std::ostringstream is("hello");
    CPPUNIT_ASSERT_EQUAL(streamSize(is), (size_t) 5);
    is.setstate(std::ios_base::failbit);
    CPPUNIT_ASSERT_EQUAL(streamSize(is), (size_t) 0);

    std::stringstream s;
    CPPUNIT_ASSERT_EQUAL(streamSize(s), (size_t) 0);
    s << "stream";
    CPPUNIT_ASSERT_EQUAL(streamSize(s), (size_t) 6);
}

void TestHelper::testMethodName()
{
#define ASSERT_METHOD(signature, method) CPPUNIT_ASSERT_EQUAL(std::string(method), cf::extractClassMethodNameFromSignature(signature))

    ASSERT_METHOD("void foo()", "");
    ASSERT_METHOD("void Foo::bar()", "bar");
    ASSERT_METHOD("std::string cf::Foo::bar()", "bar");
    ASSERT_METHOD("void cf::Foo::bar(const std::string&)", "bar");
    ASSERT_METHOD("int *cf::Foo::bar(const std::string&)", "bar");
    ASSERT_METHOD("int& cf::Foo::bar(const std::string&)", "bar");
    ASSERT_METHOD("void cf::Foo::bar<class T>(const std::string&)", "bar<class T>");
    ASSERT_METHOD("::(", "");
    ASSERT_METHOD("::", "");
    ASSERT_METHOD(BOOST_CURRENT_FUNCTION, METHOD_NAME());

#undef ASSERT_METHOD
}

void TestHelper::testMethodSignature()
{
#define ASSERT_SIGNATURE(signature, method) CPPUNIT_ASSERT_EQUAL(std::string(method), cf::makeClassMethodSignature(signature))

    ASSERT_SIGNATURE("void foo()", "[foo]");
    ASSERT_SIGNATURE("void Foo::bar()", "[Foo::bar]");
    ASSERT_SIGNATURE("std::string cf::Foo::bar()", "[Foo::bar]");
    ASSERT_SIGNATURE("void cf::Foo::bar(const std::string&)", "[Foo::bar]");
    ASSERT_SIGNATURE("void CppUnit::Foo::bar(const std::string&)", "[CppUnit::Foo::bar]");
    ASSERT_SIGNATURE("int * CppUnit::Foo::bar(const std::string&)", "[CppUnit::Foo::bar]");
    ASSERT_SIGNATURE(" ::(", "");
    ASSERT_SIGNATURE(" ::", "");
    ASSERT_SIGNATURE(BOOST_CURRENT_FUNCTION, METHOD_SIGNATURE());

#undef ASSERT_METHOD
}

void TestHelper::testToLower()
{
    std::string str("ABCDEF");
    cf::toLower(str);
    CPPUNIT_ASSERT_EQUAL(std::string("abcdef"), str);
}

void TestHelper::testToUpper()
{
    std::string str("abcdef");
    cf::toUpper(str);
    CPPUNIT_ASSERT_EQUAL(std::string("ABCDEF"), str);
}

void TestHelper::testReplaceAll()
{
    std::string str("some test message");
    cf::replaceAll(str, "test", "example");
    CPPUNIT_ASSERT_EQUAL(std::string("some example message"), str);
    cf::replaceAll(str, "e", "$");
    CPPUNIT_ASSERT_EQUAL(std::string("som$ $xampl$ m$ssag$"), str);
    cf::replaceAll(str, "%", "6");
    CPPUNIT_ASSERT_EQUAL(std::string("som$ $xampl$ m$ssag$"), str);
    cf::replaceAll(str, "", "SPACE");
    CPPUNIT_ASSERT_EQUAL(std::string("som$ $xampl$ m$ssag$"), str);
    cf::replaceAll(str, "$", "");
    CPPUNIT_ASSERT_EQUAL(std::string("som xampl mssag"), str);
    cf::replaceAll(str, "m", "");
    CPPUNIT_ASSERT_EQUAL(std::string("so xapl ssag"), str);
}

void TestHelper::testFileExists() {
#ifdef _WIN32
//CPPUNIT_ASSERT(fs::fileExists("C:/Windows/System32/Drivers/hosts"));
#else
    CPPUNIT_ASSERT(fs::fileExists("/bin/sh"));
    CPPUNIT_ASSERT(!fs::fileExists("/not-exists"));
#endif
}
