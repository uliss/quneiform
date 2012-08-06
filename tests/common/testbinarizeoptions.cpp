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

#include "testbinarizeoptions.h"
#include "common/binarizeoptions.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBinarizeOptions);

using namespace cf;

void TestBinarizeOptions::testInit()
{
    BinarizeOptions bopts;
    CPPUNIT_ASSERT_EQUAL(BINARIZATOR_DEFAULT, bopts.binarizator());
}

void TestBinarizeOptions::testBoolOptions()
{
    BinarizeOptions bopts;
    // not found
    CPPUNIT_ASSERT(!bopts.hasOption("bool"));
    CPPUNIT_ASSERT(!bopts.optionBool("bool", false));
    CPPUNIT_ASSERT(bopts.optionBool("bool", true));
    // exists
    bopts.setOption("bool", true);
    CPPUNIT_ASSERT(bopts.hasOption("bool"));
    CPPUNIT_ASSERT(bopts.optionBool("bool", false));
    CPPUNIT_ASSERT(bopts.optionBool("bool", true));
    bopts.setOption("bool", false);
    CPPUNIT_ASSERT(!bopts.optionBool("bool", false));
    CPPUNIT_ASSERT(!bopts.optionBool("bool", true));

    // other type
    bopts.setOption("bool", 1.0f);
    CPPUNIT_ASSERT_THROW(bopts.optionBool("bool", true), BinarizeOptions::Exception);

}

void TestBinarizeOptions::testFloatOptions()
{
    BinarizeOptions bopts;
    // not found
    CPPUNIT_ASSERT(!bopts.hasOption("float"));
    CPPUNIT_ASSERT_EQUAL(1.0f, bopts.optionFloat("float", 1.0f));
    // exists
    bopts.setOption("float", 2.0f);
    CPPUNIT_ASSERT(bopts.hasOption("float"));
    CPPUNIT_ASSERT_EQUAL(2.0f, bopts.optionFloat("float", -1.0f));

    // other type
    bopts.setOption("float", true);
    CPPUNIT_ASSERT_THROW(bopts.optionFloat("float", -1.0f), BinarizeOptions::Exception);
}

void TestBinarizeOptions::testIntOptions()
{
    BinarizeOptions bopts;
    // not found
    CPPUNIT_ASSERT(!bopts.hasOption("int"));
    CPPUNIT_ASSERT_EQUAL(-1, bopts.optionInt("int", -1));
    // exists
    bopts.setOption("int", 2);
    CPPUNIT_ASSERT(bopts.hasOption("int"));
    CPPUNIT_ASSERT_EQUAL(2, bopts.optionInt("int", -2));

    // other type
    bopts.setOption("int", 1.123f);
    CPPUNIT_ASSERT_THROW(bopts.optionInt("int", -2), BinarizeOptions::Exception);
}

void TestBinarizeOptions::testStringOptions()
{
    BinarizeOptions bopts;
    // not found
    CPPUNIT_ASSERT(!bopts.hasOption("str"));
    CPPUNIT_ASSERT_EQUAL(std::string("test"), bopts.optionString("str", "test"));
    // exists
    bopts.setOption("str", "value");
    CPPUNIT_ASSERT(bopts.hasOption("str"));
    CPPUNIT_ASSERT_EQUAL(std::string("value"), bopts.optionString("str", "other"));

    // other type
    bopts.setOption("str", 1.123f);
    CPPUNIT_ASSERT_THROW(bopts.optionString("str", ""), BinarizeOptions::Exception);
}
