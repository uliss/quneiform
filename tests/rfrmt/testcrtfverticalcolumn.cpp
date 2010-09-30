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
#include "testcrtfverticalcolumn.h"
CPPUNIT_TEST_SUITE_REGISTRATION( TestCRtfVerticalColumn);
#include "rfrmt/crtfverticalcolumn.h"

using namespace cf;

void TestCRtfVerticalColumn::testInit() {
    CRtfVerticalColumn vcol;
}

struct DrawVColumn
{
        int * i_;
        DrawVColumn(int * i) :
            i_(i) {
        }
        void operator()(const CRtfVerticalColumn *) {
            (*i_)++;
        }
};

void TestCRtfVerticalColumn::testSetDrawCallback() {
    RfrmtDrawVColumnFunction f;
    CRtfVerticalColumn::setDrawCallback(f);

    CRtfVerticalColumn vcol;
    vcol.drawLayout();

    int call_count = 0;
    f = DrawVColumn(&call_count);
    CRtfVerticalColumn::setDrawCallback(f);

    CPPUNIT_ASSERT_EQUAL(0, call_count);
    vcol.drawLayout();
    CPPUNIT_ASSERT_EQUAL(1, call_count);
    vcol.drawLayout();
    CPPUNIT_ASSERT_EQUAL(2, call_count);

    RfrmtDrawVColumnFunction null;
    CRtfVerticalColumn::setDrawCallback(null);
    vcol.drawLayout();
    CPPUNIT_ASSERT_EQUAL(2, call_count);
}
