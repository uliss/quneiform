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
#include "testhistogram.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestHistogram);
#include "common/histogram.h"

using namespace cf;

void TestHistogram::testInit() {
    Histogram h(0);
    CPPUNIT_ASSERT(h.empty());
    CPPUNIT_ASSERT(h.size() == 0);

    Histogram h2(10);
    CPPUNIT_ASSERT(!h2.empty());
    CPPUNIT_ASSERT(h2.size() == 10);
    for (size_t i = 0; i < 10; i++) {
        CPPUNIT_ASSERT(h2[i] == 0);
    }

    std::vector<float> v;

    h2.init(v.begin(), v.end());
    CPPUNIT_ASSERT(h2.empty());
    CPPUNIT_ASSERT(h2.size() == 0);

    Histogram h3(v.begin(), v.end());
    CPPUNIT_ASSERT(h3.empty());
    CPPUNIT_ASSERT(h3.size() == 0);

    v.push_back(1.0);
    v.push_back(2.0);

    Histogram h4(v.begin(), v.end());
    CPPUNIT_ASSERT(!h4.empty());
    CPPUNIT_ASSERT(h4.size() == v.size());
}

void TestHistogram::testClear() {
    int v[] = { 1, 2, 3, 4, 5 };
    Histogram h(v, v + sizeof(v) / sizeof(v[0]));
    CPPUNIT_ASSERT(!h.empty());
    CPPUNIT_ASSERT(h.size() == 5);

    for (size_t i = 0; i < 5; i++) {
        CPPUNIT_ASSERT(h[i] == v[i]);
    }

    h.clear();
    CPPUNIT_ASSERT(!h.empty());
    CPPUNIT_ASSERT(h.size() == 5);
    for (size_t i = 0; i < 5; i++) {
        CPPUNIT_ASSERT(h[i] == 0);
    }
}

void TestHistogram::testSpaceCount() {
    Histogram h(0);

    CPPUNIT_ASSERT(h.spaceCount() == 0);

    int h1[] = { 0, 0, 0, 0, 0 };
    h.init(h1, h1 + 5);

    CPPUNIT_ASSERT(h.spaceCount() == 1);

    int h2[] = { 1, 1, 1, 1, 1 };
    h.init(h2, h2 + 5);

    CPPUNIT_ASSERT(h.spaceCount() == 0);

    int h3[] = { 1, 0, 0, 1, 0 };
    h.init(h3, h3 + 5);

    CPPUNIT_ASSERT(h.spaceCount() == 2);

    int h4[] = { 0, 1, 0, 1, 0 };
    h.init(h4, h4 + 5);

    CPPUNIT_ASSERT(h.spaceCount() == 3);

    int h5[] = { 2, 1, 4, 1, 3 };
    h.init(h5, h5 + 5);

    CPPUNIT_ASSERT(h.spaceCount() == 0);
}

void TestHistogram::testFindU() {
    Histogram h(0);
    CPPUNIT_ASSERT(!h.findU());

    int h1[] = { 0, 0, 0, 0, 0 };
    h.init(h1, h1 + 5);
    CPPUNIT_ASSERT(!h.findU());

    int h2[] = { 1, 1, 1, 1, 1 };
    h.init(h2, h2 + 5);
    CPPUNIT_ASSERT(!h.findU());

    int h3[] = { 1, 0, 0, 1, 0 };
    h.init(h3, h3 + 5);
    CPPUNIT_ASSERT(h.findU());

    int h4[] = { 0, 1, 0, 1, 0 };
    h.init(h4, h4 + 5);
    CPPUNIT_ASSERT(h.findU());

    int h5[] = { 1, 2, 3, 4, 5 };
    h.init(h5, h5 + 5);
    CPPUNIT_ASSERT(!h.findU());

    int h6[] = { 5, 4, 3, 2, 1 };
    h.init(h6, h6 + 5);
    CPPUNIT_ASSERT(!h.findU());
}
