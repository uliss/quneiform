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

#include "testformatoptions.h"
#include "common/formatoptions.h"
#include "../test_common.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestFormatOptions);

using namespace cf;

void TestFormatOptions::testInit()
{
    FormatOptions fopts;
    CPPUNIT_ASSERT_EQUAL(std::string("Times New Roman"), fopts.serifName());
    CPPUNIT_ASSERT_EQUAL(std::string("Arial"), fopts.sansSerifName());
    CPPUNIT_ASSERT_EQUAL(std::string("Courier New"), fopts.monospaceName());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_RUS_ENG, fopts.language());
    CPPUNIT_ASSERT_EQUAL(L'~', fopts.unrecognizedChar());
    CPPUNIT_ASSERT_EQUAL(FORMAT_UNKNOWN, fopts.imageExportFormat());
    CPPUNIT_ASSERT(fopts.isBoldUsed());
    CPPUNIT_ASSERT(fopts.isItalicUsed());
    CPPUNIT_ASSERT(fopts.isUnderlinedUsed());
    CPPUNIT_ASSERT(fopts.isStylesUsed());
    CPPUNIT_ASSERT(fopts.isFontSizeUsed());
    CPPUNIT_ASSERT(!fopts.preserveLineBreaks());
    CPPUNIT_ASSERT(!fopts.showAlternatives());
    CPPUNIT_ASSERT(!fopts.isTestOutput());
    CPPUNIT_ASSERT_EQUAL(FormatOptions::FORMAT_ALL, fopts.formatMode());
}

void TestFormatOptions::testOutput()
{
    FormatOptions fopts;
    std::ostringstream buf;
    buf << fopts;
    CPPUNIT_ASSERT(!buf.str().empty());
}

void TestFormatOptions::testSerializeTxt()
{
#ifdef CF_SERIALIZE
    FormatOptions fopts_src;
    fopts_src.setSerifName("Georgia");
    fopts_src.setSansSerifName("Terminus");
    fopts_src.setMonospaceName("Comis Sans");

    const char * TXT = "serialize_format_options.txt";
    writeToTextArchive(TXT, fopts_src);

    FormatOptions fopts_dest;
    CPPUNIT_ASSERT(fopts_dest != fopts_src);
    readFromTextArchive(TXT, fopts_dest);

    CPPUNIT_ASSERT(fopts_src == fopts_dest);
#endif
}

void TestFormatOptions::testSerializeXml()
{
#ifdef CF_SERIALIZE
    FormatOptions fopts_src;
    fopts_src.setSerifName("Georgia");
    fopts_src.setSansSerifName("Terminus");
    fopts_src.setMonospaceName("Comis Sans");

    const char * XML = "serialize_format_options.xml";
    writeToXmlArchive(XML, "format_options", fopts_src);

    FormatOptions fopts_dest;
    CPPUNIT_ASSERT(fopts_dest != fopts_src);
    readFromXmlArchive(XML, "format_options", fopts_dest);

    CPPUNIT_ASSERT(fopts_src == fopts_dest);
#endif
}
