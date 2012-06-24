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

#include "testimageurl.h"
#include "../test_common.h"
#include "common/imageurl.h"
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestImageURL);

using namespace cf;

void TestImageURL::testInit()
{
    ImageURL u;
    CPPUNIT_ASSERT(u.empty());
    CPPUNIT_ASSERT(u.simple());
    CPPUNIT_ASSERT(u.imageNumber() == 0);

    CPPUNIT_ASSERT(!ImageURL("test").empty());
    CPPUNIT_ASSERT(ImageURL("test").simple());

    CPPUNIT_ASSERT(!ImageURL(__FILE__, 1).simple());
}

void TestImageURL::testExists()
{
    CPPUNIT_ASSERT(!ImageURL().exists());
    CPPUNIT_ASSERT(!ImageURL("not-exists").exists());
    CPPUNIT_ASSERT(ImageURL(__FILE__).exists());
}

void TestImageURL::testSerialize()
{
#ifdef CF_SERIALIZE
    const ImageURL url1("test", 2);
    const ImageURL url2;

    const char * TEXT_OUT = "serialize_imageurl.txt";

    // save data to archive
    {
        std::ofstream ofs(TEXT_OUT);
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << url1 << url2;
    }

    ImageURL new_url;
    CPPUNIT_ASSERT(url1 != new_url);
    {
        // create and open an archive for input
        std::ifstream ifs(TEXT_OUT);
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_url;

        CPPUNIT_ASSERT_EQUAL(url1, new_url);

        ia >> new_url;
        CPPUNIT_ASSERT_EQUAL(url2, new_url);
    }
#endif
}

void TestImageURL::testSerializeXml()
{
#ifdef CF_SERIALIZE
    const ImageURL url1("test", 2);
    const ImageURL url2;

    const char * FNAME = "serialize_imageurl.xml";

    // save data to archive
    {
        std::ofstream ofs(FNAME);
        CEDXmlOutputArchive oa(ofs);
        writeToXml(oa, "url", url1);
        writeToXml(oa, "url", url2);
    }

    ImageURL new_url;
    CPPUNIT_ASSERT(url1 != new_url);
    {
        // create and open an archive for input
        std::ifstream ifs(FNAME);
        CEDXmlInputArchive ia(ifs);
        readFromXml(ia, "url", new_url);
        CPPUNIT_ASSERT_EQUAL(url1, new_url);
        readFromXml(ia, "url", new_url);
        CPPUNIT_ASSERT_EQUAL(url2, new_url);
    }
#endif
}
