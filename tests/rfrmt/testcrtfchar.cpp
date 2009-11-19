/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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
#include "rfrmt/crtfchar.h"
#include "rfrmt/creatertf.h"
#include "common/tostring.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCRtfChar);

using namespace CIF;

void TestCRtfChar::testInit() {
    CRtfChar ch;
    LETTER lt;
    CIF::Rect layout(Point(), 100, 20);
    Rtf_CED_CreateChar(&layout, &lt, NULL);
    CPPUNIT_ASSERT_EQUAL(layout, Rect(Point(-1, -1), Point(-1, -1)));
    CPPUNIT_ASSERT_EQUAL(lt.alternative, (uchar)' ');
    CPPUNIT_ASSERT_EQUAL(lt.probability, (uchar)0);

    Rtf_CED_CreateChar(&layout, &lt, &ch);
    CPPUNIT_ASSERT_EQUAL(layout, Rect());
}
