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
#include "testcedline.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCedLine);
#define private public
#include <ced/cedline.h>
using namespace CIF;

void TestCedLine::testInit() {
    CEDLine ln;
    CPPUNIT_ASSERT(0 == ln.currentChar());
    CPPUNIT_ASSERT(0 == ln.numOfChars);
    CPPUNIT_ASSERT(0 == ln.internalNumber());
    CPPUNIT_ASSERT(0 == ln.parentNumber());
    CPPUNIT_ASSERT(0 == ln.chars);
    CPPUNIT_ASSERT(0 == ln.next);
    CPPUNIT_ASSERT(0 == ln.prev);
}
