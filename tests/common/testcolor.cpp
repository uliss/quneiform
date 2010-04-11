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

#include "testcolor.h"
#include <common/color.h>
#include <ced/cedarchive.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestColor);
using namespace CIF;

void TestColor::testSerialize() {
#ifdef CF_SERIALIZE
    Color c(1, 2, 3);

    std::ofstream ofs("serialize_color.txt");

        // save data to archive
        {
            CEDOutputArchive oa(ofs);
            // write class instance to archive
            oa << c;
        }

        Color new_c;
        CPPUNIT_ASSERT(c != new_c);
        {
            // create and open an archive for input
            std::ifstream ifs("serialize_color.txt");
            CEDInputArchive ia(ifs);
            // read class state from archive
            ia >> new_c;
        }

        CPPUNIT_ASSERT(c == new_c);
#endif
}
