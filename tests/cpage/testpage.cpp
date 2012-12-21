/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include "testpage.h"
#include "cpage/page.h"
#include "cpage/block.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPage);

using namespace cf::cpage;

void TestPage::testInit()
{
    Page p;
    CPPUNIT_ASSERT(p.empty());
    CPPUNIT_ASSERT_EQUAL(size_t(0), p.blockCount());
}

void TestPage::appendBlock()
{
    Page p;
    CPPUNIT_ASSERT_EQUAL(size_t(0), p.blockCount());

    Block b;
    b.setFlags(10);
    p.appendBlock(b);
    CPPUNIT_ASSERT_EQUAL(size_t(1), p.blockCount());

    CPPUNIT_ASSERT(p.blockAt(0)->empty());
    CPPUNIT_ASSERT_EQUAL(p.blockAt(0)->flags(), b.flags());
}
