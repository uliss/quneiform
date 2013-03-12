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

#include "testconfigoptions.h"
#include "common/configoptions.h"
#include "common/filesystem.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestConfigOptions);

using namespace cf;

namespace {

std::string CONFIG_PATH = fs::dirName(__FILE__) + "/test_config.json";

}

void TestConfigOptions::testBool()
{
    CPPUNIT_ASSERT_THROW(ConfigOptions::getBool("none"), std::runtime_error);
    CPPUNIT_ASSERT(!ConfigOptions::getBool("none", false));
    CPPUNIT_ASSERT(ConfigOptions::getBool("none", true));
    CPPUNIT_ASSERT(ConfigOptions::getBool("bool-true", false));
    CPPUNIT_ASSERT(ConfigOptions::getBool("bool-true"));
    CPPUNIT_ASSERT(!ConfigOptions::getBool("bool-false"));
}

void TestConfigOptions::testInt()
{
    CPPUNIT_ASSERT_THROW(ConfigOptions::getInt("none"), std::runtime_error);
    CPPUNIT_ASSERT_EQUAL(0, ConfigOptions::getInt("none", 0));
    CPPUNIT_ASSERT_EQUAL(100, ConfigOptions::getInt("none", 100));
    CPPUNIT_ASSERT_EQUAL(123, ConfigOptions::getInt("int-pos", 0));
    CPPUNIT_ASSERT_EQUAL(-154, ConfigOptions::getInt("int-neg", 100));
}

void TestConfigOptions::testFloat()
{
    CPPUNIT_ASSERT_THROW(ConfigOptions::getFloat("none"), std::runtime_error);
    CPPUNIT_ASSERT_EQUAL(float(0.0), ConfigOptions::getFloat("none", 0.0));
    CPPUNIT_ASSERT_EQUAL(float(100.0), ConfigOptions::getFloat("none", 100.0));
    CPPUNIT_ASSERT_EQUAL(float(0.124), ConfigOptions::getFloat("float-pos", 0.0));
    CPPUNIT_ASSERT_EQUAL(float(-123.345), ConfigOptions::getFloat("float-neg", 100.0));
}

void TestConfigOptions::testString()
{
    CPPUNIT_ASSERT_THROW(ConfigOptions::getString("none"), std::runtime_error);
    CPPUNIT_ASSERT_EQUAL(std::string(""), ConfigOptions::getString("none", ""));
    CPPUNIT_ASSERT_EQUAL(std::string("sample"), ConfigOptions::getString("none", "sample"));
    CPPUNIT_ASSERT_EQUAL(std::string(""), ConfigOptions::getString("string1", "sample"));
    CPPUNIT_ASSERT_EQUAL(std::string("str"), ConfigOptions::getString("string2", "sample"));
}

void TestConfigOptions::setUp()
{
    ConfigOptions::setConfigPath(CONFIG_PATH);
}

void TestConfigOptions::tearDown()
{
    ConfigOptions::setConfigPath("");
}
