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
#include <algorithm>
#include "testoutputformat.h"
#include "common/outputformat.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestOutputFormat);

using namespace CIF;

void TestOutputFormat::testInit()
{
    OutputFormat fmt(PUMA_TOEDNATIVE);
    CPPUNIT_ASSERT_EQUAL(fmt.get(), PUMA_TOEDNATIVE);
    CPPUNIT_ASSERT(fmt.extension() == "ed");
    CPPUNIT_ASSERT(fmt.name() == "native");
    CPPUNIT_ASSERT(fmt.description() == "Cuneiform 2000 format");

    OutputFormatList lst = OutputFormat::formats();
    CPPUNIT_ASSERT(!lst.empty());
    CPPUNIT_ASSERT(std::find(lst.begin(), lst.end(), PUMA_TOEDNATIVE) != lst.end());
    CPPUNIT_ASSERT(std::find(lst.begin(), lst.end(), PUMA_TOHTML) != lst.end());
    CPPUNIT_ASSERT(std::find(lst.begin(), lst.end(), PUMA_TORTF) != lst.end());
    CPPUNIT_ASSERT(std::find(lst.begin(), lst.end(), PUMA_TOHOCR) != lst.end());
    CPPUNIT_ASSERT(std::find(lst.begin(), lst.end(), PUMA_TOTEXT) != lst.end());
    CPPUNIT_ASSERT(std::find(lst.begin(), lst.end(), PUMA_TOSMARTTEXT) != lst.end());
    CPPUNIT_ASSERT(std::find(lst.begin(), lst.end(), PUMA_DEBUG_TOTEXT) != lst.end());
}
