/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include <sstream>

#include "testrecognizeoptions.h"
#include "common/recognizeoptions.h"
#include "../test_common.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestRecognizeOptions);

using namespace cf;

void TestRecognizeOptions::testInit()
{
    RecognizeOptions ropts;
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ENGLISH, ropts.language());
    CPPUNIT_ASSERT_EQUAL(RecognizeOptions::ANGLE_0, ropts.turnAngle());
    CPPUNIT_ASSERT_EQUAL(0, ropts.imageNumber());
}

void TestRecognizeOptions::testOutput()
{
    RecognizeOptions ropts;
    std::ostringstream buf;
    buf << ropts;
    CPPUNIT_ASSERT(!buf.str().empty());
}

void TestRecognizeOptions::testSerializeTxt()
{
#ifdef CF_SERIALIZE
    RecognizeOptions ropts_src;
    ropts_src.setLanguage(LANGUAGE_BELARUSIAN);

    const char * TXT = "serialize_recognize_options.txt";
    writeToTextArchive(TXT, ropts_src);

    RecognizeOptions ropts_dest;
    CPPUNIT_ASSERT(ropts_dest != ropts_src);
    readFromTextArchive(TXT, ropts_dest);

    CPPUNIT_ASSERT(ropts_src == ropts_dest);
#endif
}

void TestRecognizeOptions::testSerializeXml()
{
#ifdef CF_SERIALIZE
    RecognizeOptions ropts_src;
    ropts_src.setLanguage(LANGUAGE_BELARUSIAN);

    const char * XML = "serialize_recognize_options.xml";
    writeToXmlArchive(XML, "ropts", ropts_src);

    RecognizeOptions ropts_dest;
    CPPUNIT_ASSERT(ropts_dest != ropts_src);
    readFromXmlArchive(XML, "ropts", ropts_dest);

    CPPUNIT_ASSERT(ropts_src == ropts_dest);
#endif
}

