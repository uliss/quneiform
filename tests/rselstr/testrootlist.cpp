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

#include "testrootlist.h"
#include "rselstr/rootlist.h"
#include "roots_struct.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestRootList);

using namespace cf;

void TestRootList::testInit()
{
    RootList lst;
    CPPUNIT_ASSERT(lst.isNull());
    CPPUNIT_ASSERT(lst.isEmpty());
    CPPUNIT_ASSERT_EQUAL(size_t(0), lst.count());
    lst.free();
}

void TestRootList::testAdd()
{
    RootList l;
    CPPUNIT_ASSERT(l.add(ROOT()));
    CPPUNIT_ASSERT_EQUAL(size_t(1), l.count());
    CPPUNIT_ASSERT(!l.isNull());
    CPPUNIT_ASSERT(!l.isEmpty());

    CPPUNIT_ASSERT_EQUAL(size_t(10), l.capacity());
    for(int i = 0; i < 10; i++)
        CPPUNIT_ASSERT(l.add(ROOT()));

    CPPUNIT_ASSERT_EQUAL(size_t(11), l.count());
    CPPUNIT_ASSERT_EQUAL(size_t(110), l.capacity());

    l.free();
    CPPUNIT_ASSERT(l.isNull());
    CPPUNIT_ASSERT(l.isEmpty());
    CPPUNIT_ASSERT_EQUAL(size_t(0), l.count());
    CPPUNIT_ASSERT(l.add(ROOT()));
}
