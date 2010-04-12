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
#include "testsize.h"
#include <common/size.h>
#include <ced/cedarchive.h>
using namespace CIF;
CPPUNIT_TEST_SUITE_REGISTRATION(TestSize);

void TestSize::testSerialize() {
#ifdef CF_SERIALIZE
    Size sz(-20, -30);

    std::ofstream ofs("serialize_size.txt");

    // save data to archive
    {
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << sz;
    }

    Size new_sz;
    CPPUNIT_ASSERT(sz != new_sz);
    {
        // create and open an archive for input
        std::ifstream ifs("serialize_size.txt");
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_sz;
    }

    CPPUNIT_ASSERT(sz == new_sz);
#endif
}
