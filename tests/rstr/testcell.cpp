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
