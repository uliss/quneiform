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

#include "testcedmerge.h"
#include "export/cedpagemerge.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDMerge);

using namespace cf;

void TestCEDMerge::testAddPage()
{
    CEDPageMerge e;
    CPPUNIT_ASSERT(e.empty());

    e.add(CEDPagePtr());
    CPPUNIT_ASSERT(e.empty());

    CEDPagePtr p(new CEDPage);
    p->setImageName("test");
    p->addSection(new CEDSection);
    p->addSection(new CEDSection);

    e.add(p);
    CPPUNIT_ASSERT(!e.empty());

    // add twice
    e.add(p);
}

void TestCEDMerge::testExport()
{
}
