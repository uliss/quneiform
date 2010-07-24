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

#ifndef TESTTEXTEXPORTER_H_
#define TESTTEXTEXPORTER_H_

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>

namespace CIF {
    class TextExporter;
}

class TestTextExporter: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestTextExporter);
    CPPUNIT_TEST(testExportLine);
    CPPUNIT_TEST(testExportPage);
    CPPUNIT_TEST(testExportParagraph);
    CPPUNIT_TEST(testExportPicture);
    CPPUNIT_TEST(testExportSection);
    CPPUNIT_TEST_SUITE_END();
public:
    void testExportLine();
    void testExportPage();
    void testExportParagraph();
    void testExportPicture();
    void testExportSection();
public:
    void setUp();
    void tearDown();
private:
    CIF::TextExporter * exp_;
    std::ostringstream buffer_;
};

#endif /* TESTTEXTEXPORTER_H_ */
