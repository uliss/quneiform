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

#ifndef TESTQTIMAGEEXPORTER_H
#define TESTQTIMAGEEXPORTER_H

#include <cppunit/extensions/HelperMacros.h>

#ifndef EXPORTER_TEST_IMAGE_DIR
#define EXPORTER_TEST_IMAGE_DIR "./"
#endif

class TestQtImageExporter: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestQtImageExporter);
    CPPUNIT_TEST(testSave);
    CPPUNIT_TEST(testSupportedFormats);
    CPPUNIT_TEST_SUITE_END();
public:
    void testSave();
    void testSupportedFormats();
};

#endif // TESTQTIMAGEEXPORTER_H
