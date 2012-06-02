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

#ifndef TESTSANESCANNER_H
#define TESTSANESCANNER_H

#include <cppunit/extensions/HelperMacros.h>

class TestSaneScanner : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestSaneScanner);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST(testStart);
    CPPUNIT_TEST(testSetOption);
    CPPUNIT_TEST(testScanMode);
    CPPUNIT_TEST(testScanDepth);
    CPPUNIT_TEST(testHandScanner);
    CPPUNIT_TEST(testScanResolution);
    CPPUNIT_TEST(testScanPicture);
    CPPUNIT_TEST(testScanReadLimit);
    CPPUNIT_TEST(testScanReadDelay);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testOpen();
    void testStart();
    void testSetOption();
    void testScanMode();
    void testScanDepth();
    void testHandScanner();
    void testScanResolution();
    void testScanPicture();
    void testScanReadLimit();
    void testScanReadDelay();
};

#endif // TESTSANESCANNER_H
