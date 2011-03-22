/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <cstring>
#include "testiconv.h"
#include "common/iconv_local.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestIconv);

void TestIconv::testConvertChar()
{
    cf::Iconv iconv("cp1251", "UTF8");

    for(char i = 0; i < 125; i++) {
        CPPUNIT_ASSERT_EQUAL(std::string(1, i), iconv.convert(i));
    }

    // russian 'a'
    CPPUNIT_ASSERT_EQUAL(std::string("а"), iconv.convert("\xE0"));
    CPPUNIT_ASSERT_EQUAL(std::string("а"), iconv.convert('\xE0'));
}

void TestIconv::testConvertString() {
    cf::Iconv iconv("cp1251", "UTF8");
    CPPUNIT_ASSERT_EQUAL(std::string("ааа"), iconv.convert("\xE0\xE0\xE0"));

    // big string
    const int SZ = 10000;
    char BIG_STR[SZ + 1];
    memset(BIG_STR, 0xE0, SZ); // russian 'a'
    BIG_STR[SZ] = '\0';

    std::string str;
    str.reserve(SZ);
    for(int i = 0; i < SZ; i++)
        str.append("а");

    CPPUNIT_ASSERT_EQUAL(str, iconv.convert(BIG_STR));
}
