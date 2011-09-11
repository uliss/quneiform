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

#include "testpercentcounter.h"
#include "common/percentcounter.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPercentCounter);

using namespace cf;

void TestPercentCounter::callback(unsigned char perc) {
    perc_ = perc;
}

void TestPercentCounter::testConstruct() {
    PercentCounter pc;
    CPPUNIT_ASSERT(!pc.parent());
    CPPUNIT_ASSERT(pc.get() == 0);
    CPPUNIT_ASSERT(pc.contribution() == 100);
}

void TestPercentCounter::testAlone() {
    PercentCounter pc;
    pc.add(10);
    CPPUNIT_ASSERT_EQUAL((uchar)10, pc.get());
    pc.setContribution(50);
    CPPUNIT_ASSERT_EQUAL((uchar)50, pc.contribution());
    pc.add(20);
    CPPUNIT_ASSERT_EQUAL((uchar)30, pc.get());
    pc.setCallback(this, &TestPercentCounter::callback);
    pc.add(10);
    CPPUNIT_ASSERT_EQUAL(pc.get(), perc_);
    pc.add(20);
    CPPUNIT_ASSERT_EQUAL(pc.get(), perc_);
}

void TestPercentCounter::testParent() {
    PercentCounter parent;
    PercentCounter child(&parent);

    CPPUNIT_ASSERT_EQUAL(&parent, child.parent());

    child.setContribution(50);
    child.add(20);
    CPPUNIT_ASSERT_EQUAL((uchar)20, child.get());
    CPPUNIT_ASSERT_EQUAL((uchar)10, parent.get());
    parent.setCallback(this, &TestPercentCounter::callback);
    child.add(40);
    CPPUNIT_ASSERT_EQUAL((uchar)30, parent.get());
    CPPUNIT_ASSERT_EQUAL((uchar)30, perc_);
}

