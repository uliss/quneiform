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

#include "testcell.h"
#include "hrstr/cell.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCell);

#define EQ(a, b) CPPUNIT_ASSERT_EQUAL(a, b)
#define VERS_EQ(cell, i, v) EQ(cell->vers[i].let, (uchar) v)
#define PROB_EQ(cell, i, p) EQ(cell->vers[i].prob, (uchar) p)

//using namespace cf;

void TestCell::testInit() {
    cell * c = new cell;
    CPPUNIT_ASSERT(!c->next);
    CPPUNIT_ASSERT(!c->nextl);
    CPPUNIT_ASSERT(!c->prev);
    CPPUNIT_ASSERT(!c->prevl);
    CPPUNIT_ASSERT(!c->w);
    CPPUNIT_ASSERT(!c->h);

    delete c;
}

void TestCell::testSortVersions() {
    cell * c = new cell;
    c->nvers = 4;
    c->vers[0].let = 'a';
    c->vers[0].prob = 10;
    c->vers[1].let = 'b';
    c->vers[1].prob = 20;
    c->vers[2].let = 'c';
    c->vers[2].prob = 30;
    c->vers[3].let = 'd';
    c->vers[3].prob = 40;

    c->sortVersions();
    VERS_EQ(c, 0, 'd');
    VERS_EQ(c, 1, 'c');
    VERS_EQ(c, 2, 'b');
    VERS_EQ(c, 3, 'a');

    PROB_EQ(c, 0, 40);
    PROB_EQ(c, 1, 30);
    PROB_EQ(c, 2, 20);
    PROB_EQ(c, 3, 10);

    c->vers[3].prob = 0;
    c->sortVersions();
    EQ((int) c->nvers, (int) 3);

    delete c;
}
