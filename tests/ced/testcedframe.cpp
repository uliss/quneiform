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
#include "testcedframe.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDFrame);
#include "ced/cedframe.h"
#include "ced/cedpicture.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"
#include "ced/cedarchive.h"

using namespace CIF;

void TestCEDFrame::testSerialize() {
#ifdef CF_SERIALIZE
    CEDFrame fr;
    fr.setBorderSpace(11);

    const char * FILENAME = "serialize_cedframe.txt";

    // save data to archive
    {
        std::ofstream ofs(FILENAME);
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << fr;
    }

    CEDFrame new_fr;
    {
        // create and open an archive for input
        std::ifstream ifs(FILENAME);
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_fr;
    }

#endif
}
