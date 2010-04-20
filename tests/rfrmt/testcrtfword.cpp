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
#include "testcrtfword.h"
#define private public
#include "rfrmt/crtfword.h"
#include <common/tostring.h>
using namespace CIF;
CPPUNIT_TEST_SUITE_REGISTRATION( TestCRtfWord);

void TestCRtfWord::testInit() {
    CIF::CRtfWord wd;
    CPPUNIT_ASSERT(wd.bRect() == Rect());
    CPPUNIT_ASSERT(wd.charCount() == 0);
    CPPUNIT_ASSERT_THROW(wd.charAt(0), std::out_of_range);
    CPPUNIT_ASSERT_THROW(wd.firstChar(), std::out_of_range);
    CPPUNIT_ASSERT_THROW(wd.lastChar(), std::out_of_range);
    CPPUNIT_ASSERT(wd.fontNumber() == 0);
    CPPUNIT_ASSERT(wd.idealFontSize() == 0);
    CPPUNIT_ASSERT(!wd.isSpelled());
    CPPUNIT_ASSERT(wd.realFontSize() == 0);
    CPPUNIT_ASSERT(wd.probability() == 0);
}
