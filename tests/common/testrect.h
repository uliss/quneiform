/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef TESTRECT_H_
#define TESTRECT_H_

#include <cppunit/extensions/HelperMacros.h>

class TestRect: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestRect);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testSerialize);
    CPPUNIT_TEST(testSerializeXml);
    CPPUNIT_TEST(testUnite);
    CPPUNIT_TEST(testNormalize);
    CPPUNIT_TEST(testWidth);
    CPPUNIT_TEST(testHeight);
    CPPUNIT_TEST(testNull);
    CPPUNIT_TEST(testMoveXTo);
    CPPUNIT_TEST(testMoveYTo);
    CPPUNIT_TEST(testMoveTo);
    CPPUNIT_TEST(testIntersects);
    CPPUNIT_TEST(testIntersected);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testSerialize();
    void testSerializeXml();
    void testUnite();
    void testNormalize();
    void testHeight();
    void testWidth();
    void testNull();
    void testMoveXTo();
    void testMoveYTo();
    void testMoveTo();
    void testIntersects();
    void testIntersected();
};

#endif /* TESTPOINT_H_ */
