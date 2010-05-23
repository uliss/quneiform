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
#include <stdexcept>
#include "testcrtfhorizontalcolumn.h"
#include "common/tostring.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCRtfHorizontalColumn);
#define private public
#include "rfrmt/crtfhorizontalcolumn.h"
#include "rfrmt/crtfverticalcolumn.h"
using namespace CIF;

typedef CRtfHorizontalColumn::IndexList IndexList;

void TestCRtfHorizontalColumn::testInit() {
    CRtfHorizontalColumn col;
    CPPUNIT_ASSERT_EQUAL(col.type(), CRtfHorizontalColumn::SINGLE_TERMINAL);
    CPPUNIT_ASSERT(col.columnCount() == 0);
    col.clearColumns();
}

int numSmall(const CRtfHorizontalColumn& col) {
    int res = 0;

    for (size_t i = 0; i < col.vcols_.size(); i++) {
        if (col.vcols_[i]->isSmall())
            res++;
    }

    return res;
}

void ADD_VCOL(CRtfHorizontalColumn& col, const Rect& r, fragment_t type = FT_TEXT) {
    CRtfVerticalColumn * vcol = new CRtfVerticalColumn;
    vcol->setRealRect(r);
    vcol->setType(type);
    col.addColumn(vcol);
}

void ADD_VCOL(CRtfHorizontalColumn& col, int width, int height, int xoffset) {
    ADD_VCOL(col, Rect(Point(xoffset, 0), width, height));
}

void TestCRtfHorizontalColumn::testMarkSmallColumns() {
    CRtfHorizontalColumn col;
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(0, numSmall(col));

    ADD_VCOL(col, 10, 10, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(0, numSmall(col));

    ADD_VCOL(col, 14, 11, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(0, numSmall(col));

    ADD_VCOL(col, 17, 9, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(0, numSmall(col));

    ADD_VCOL(col, 4, 4, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(1, numSmall(col));

    ADD_VCOL(col, 50, 80, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL((int) col.columnCount() - 1, numSmall(col));
}

void TestCRtfHorizontalColumn::testMaxVColumnHeight() {
    CRtfHorizontalColumn col;
    CPPUNIT_ASSERT_EQUAL(0, col.maxVColumnHeight());
    ADD_VCOL(col, 0, 1, 0);
    ADD_VCOL(col, 0, 5, 0);
    ADD_VCOL(col, 0, 2, 0);
    CPPUNIT_ASSERT_EQUAL(5, col.maxVColumnHeight());
    ADD_VCOL(col, 0, 111, 0);
    ADD_VCOL(col, 0, 0, 0);
    CPPUNIT_ASSERT_EQUAL(111, col.maxVColumnHeight());
}

void TestCRtfHorizontalColumn::testMaxVColumnWidth() {
    CRtfHorizontalColumn col;
    CPPUNIT_ASSERT_EQUAL(0, col.maxVColumnWidth());
    ADD_VCOL(col, 0, 0, 0);
    ADD_VCOL(col, 10, 0, 0);
    ADD_VCOL(col, 1, 0, 0);
    CPPUNIT_ASSERT_EQUAL(10, col.maxVColumnWidth());
    ADD_VCOL(col, 120, 0, 0);
    ADD_VCOL(col, 4, 0, 0);
    CPPUNIT_ASSERT_EQUAL(120, col.maxVColumnWidth());
}

void TestCRtfHorizontalColumn::testProcessSpaceByHist() {
    CRtfHorizontalColumn col;
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 0);
    Histogram hist(0);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 0);
    hist.push_back(1);
    // 1
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 0);

    // 0
    hist[0] = 0;
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 1);
    CPPUNIT_ASSERT(col.hist_spaces_.at(0) == 0);

    // 00
    hist.push_back(0);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 1);
    CPPUNIT_ASSERT(col.hist_spaces_.at(0) == 0);

    // 001
    hist.push_back(1);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 1);

    // 0011
    hist.push_back(1);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 1);

    // 00110
    hist.push_back(0);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 2);
    CPPUNIT_ASSERT(col.hist_spaces_.at(0) == 0);
    CPPUNIT_ASSERT(col.hist_spaces_.at(1) == 4);

    // 001101
    hist.push_back(1);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 2);

    // 0011010
    hist.push_back(0);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 3);
    CPPUNIT_ASSERT(col.hist_spaces_.at(0) == 0);
    CPPUNIT_ASSERT(col.hist_spaces_.at(1) == 4);
    CPPUNIT_ASSERT(col.hist_spaces_.at(2) == 6);
}

void TestCRtfHorizontalColumn::testAllTextToFrames() {
    CRtfHorizontalColumn col;
    col.allTextToFrames();
    ADD_VCOL(col, 0, 0, 0);
    ADD_VCOL(col, 0, 0, 0);
    col.columnAt(0)->setType(FT_TEXT);
    col.columnAt(1)->setType(FT_PICTURE);
    CPPUNIT_ASSERT_EQUAL(FT_TEXT, col.columnAt(0)->type());
    CPPUNIT_ASSERT_EQUAL(FT_PICTURE, col.columnAt(1)->type());
    col.allTextToFrames();
    CPPUNIT_ASSERT_EQUAL(FT_FRAME, col.columnAt(0)->type());
    CPPUNIT_ASSERT_EQUAL(FT_PICTURE, col.columnAt(1)->type());
}

void TestCRtfHorizontalColumn::testFillSingleTerminalColumnIndex() {
    CRtfHorizontalColumn col;
    col.fillSingleTerminalColumnIndex();
    CPPUNIT_ASSERT(col.terminal_col_idx_.size() == 1);

    col.fillSingleTerminalColumnIndex();
    CPPUNIT_ASSERT(col.terminal_col_idx_.size() == 1);
    CPPUNIT_ASSERT(col.terminal_col_idx_.front()->size() == 1);
    CPPUNIT_ASSERT(col.terminal_col_idx_.front()->at(0) == 0);
}

Rect rectH(int h) {
    return Rect(Point(0, h), 0, 0);
}

void TestCRtfHorizontalColumn::testFillAllTerminalColumnIndex() {
    CRtfHorizontalColumn col;
    col.fillAllTerminalColumnIndex();
    CPPUNIT_ASSERT(col.terminal_col_idx_.size() == 1);
    CPPUNIT_ASSERT(col.terminal_col_idx_.front()->size() == 0);

    ADD_VCOL(col, rectH(10));
    ADD_VCOL(col, rectH(5));
    ADD_VCOL(col, rectH(-1));
    ADD_VCOL(col, rectH(8));

    col.fillAllTerminalColumnIndex();
    CPPUNIT_ASSERT(col.terminal_col_idx_.size() == 1);
    CPPUNIT_ASSERT(col.terminal_col_idx_.front()->size() == 4);
    CPPUNIT_ASSERT(col.terminal_col_idx_.front()->at(0) == 2);
    CPPUNIT_ASSERT(col.terminal_col_idx_.front()->at(1) == 1);
    CPPUNIT_ASSERT(col.terminal_col_idx_.front()->at(2) == 3);
    CPPUNIT_ASSERT(col.terminal_col_idx_.front()->at(3) == 0);
}

void checkSort(const CRtfHorizontalColumn& col, bool value) {
    for(size_t i = 0; i < col.columnCount(); i++) {
        CPPUNIT_ASSERT_EQUAL(value, col.columnAt(i)->isSorted());
    }
}

void TestCRtfHorizontalColumn::testSortColumns() {
    CRtfHorizontalColumn col;
    IndexList idx;
    col.sortColumns(&idx);
    CPPUNIT_ASSERT(idx.size() == 0);

    ADD_VCOL(col, rectH(100));
    ADD_VCOL(col, rectH(15));
    ADD_VCOL(col, rectH(-11));
    ADD_VCOL(col, rectH(0));

    checkSort(col, false);
    col.sortColumns(&idx);
    CPPUNIT_ASSERT(idx.size() == 4);
    checkSort(col, true);
    CPPUNIT_ASSERT_EQUAL(2, idx[0]);
    CPPUNIT_ASSERT_EQUAL(3, idx[1]);
    CPPUNIT_ASSERT_EQUAL(1, idx[2]);
    CPPUNIT_ASSERT_EQUAL(0, idx[3]);

    // all columns marked as sorted
    // so no new adding
    col.sortColumns(&idx);
    CPPUNIT_ASSERT(idx.size() == 4);
}
