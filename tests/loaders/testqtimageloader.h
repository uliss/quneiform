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

#ifndef TESTQTIMAGELOADER_H_
#define TESTQTIMAGELOADER_H_

#include <cppunit/extensions/HelperMacros.h>

class TestQtImageLoader: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestQtImageLoader);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testLoad);
    CPPUNIT_TEST(testLoadRecognize);
    CPPUNIT_TEST(testLoadParams);
    CPPUNIT_TEST(testMultiPage);
    CPPUNIT_TEST(testSupportedFormats);
    CPPUNIT_TEST(testLoadStream);
    CPPUNIT_TEST(testImageTypes);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testLoad();
    void testLoadRecognize();
    void testLoadParams();
    void testMultiPage();
    void testSupportedFormats();
    void testLoadStream();
    void testImageTypes();
};

#endif /* TESTQTIMAGELOADER_H_ */
