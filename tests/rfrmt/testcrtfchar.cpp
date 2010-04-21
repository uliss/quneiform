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
#include "testcrtfchar.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCRtfChar);

#include "rfrmt/crtfchar.h"
using namespace CIF;

void TestCRtfChar::testInit() {
    CRtfChar ch;
    CPPUNIT_ASSERT(ch.versionCount() == 0);
    CPPUNIT_ASSERT_THROW(ch.versionAt(0), std::out_of_range);
    CPPUNIT_ASSERT_THROW(ch.first(), std::out_of_range);
}
