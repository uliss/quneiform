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

#include "testcommondata.h"
#include "cpage/commondata.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCommonData);

using namespace cf;
using namespace cf::cpage;

void TestCommonData::testInit()
{
    CommonData cd;
    CPPUNIT_ASSERT(!cd.flags());
    CPPUNIT_ASSERT(!cd.type());
    CPPUNIT_ASSERT(!cd.vertexCount());
    CPPUNIT_ASSERT(!cd.number());
}

void TestCommonData::testFlags()
{
    CommonData cd;
    cd.setFlag(0x1);
    cd.setFlag(0x2);
    cd.setFlag(0x4);
    CPPUNIT_ASSERT_EQUAL(0x7, (int) cd.flags());

    cd.setFlags(0x3);
    CPPUNIT_ASSERT_EQUAL(0x3, (int) cd.flags());
}
