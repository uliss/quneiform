/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef TESTPROCESSRECOGNITIONSERVER_H
#define TESTPROCESSRECOGNITIONSERVER_H

#include <cppunit/extensions/HelperMacros.h>

class TestProcessRecognitionServer : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestProcessRecognitionServer);
    CPPUNIT_TEST(testRecognize);
    CPPUNIT_TEST(testRecognizeRotated);
    CPPUNIT_TEST(testRecognizeArea);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST_SUITE_END();
public:
    void testRecognize();
    void testRecognizeRotated();
    void testRecognizeArea();
    void testOpen();
};

#endif // TESTPROCESSRECOGNITIONSERVER_H
