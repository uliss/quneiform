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
#include "testcedframe.h"
#include "../test_common.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDFrame);
#include "ced/cedframe.h"
#include "ced/cedpicture.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"

using namespace cf;

void TestCEDFrame::testSerialize() {
#ifdef CF_SERIALIZE
    const char * FILENAME = "serialize_cedframe.txt";
    CEDFrame fr;
    fr.setBorderSpace(11);
    writeToTextArchive(FILENAME, fr);

    CEDFrame new_fr;
    readFromTextArchive(FILENAME, fr);
#endif
}

void TestCEDFrame::testSerializeXml() {
#ifdef CF_SERIALIZE
    const char * FILENAME = "serialize_cedframe.xml";
    CEDFrame fr;
    fr.setBorderSpace(11);
    writeToXmlArchive(FILENAME, "cedframe", fr);

    CEDFrame new_fr;
    readFromXmlArchive(FILENAME, "cedframe", fr);
#endif
}
