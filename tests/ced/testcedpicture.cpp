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
#include <fstream>
#include "testcedpicture.h"
#include <ced/cedpicture.h>
#include <common/tostring.h>
#include <ced/cedarchive.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDPicture);
using namespace CIF;

void TestCEDPicture::testSerialize() {
#ifdef CF_SERIALIZE
    ImagePtr img_ptr(new Image(new uchar[100], 100, Image::AllocatorNew));
    img_ptr->setFileName("CED picture");
    CEDPicture p;
    p.setImage(img_ptr);


    // save data to archive
    {
        std::ofstream ofs("serialize_cedpicture.txt");
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << p;
    }

    {
        CEDPicture new_p;
        // create and open an archive for input
        std::ifstream ifs("serialize_cedpicture.txt");
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_p;
        CPPUNIT_ASSERT_EQUAL(p.image()->size(), new_p.image()->size());
    }
#endif
}
