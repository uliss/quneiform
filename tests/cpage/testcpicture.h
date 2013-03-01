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

#ifndef TESTCPICTURE_H
#define TESTCPICTURE_H

#include <cppunit/extensions/HelperMacros.h>

class TestCPicture : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCPicture);
    CPPUNIT_TEST(testGetPlace);
    CPPUNIT_TEST(testGetFirstNext);
    CPPUNIT_TEST(testGetMask);
    CPPUNIT_TEST_SUITE_END();
public:
    void tearDown();
    void testGetPlace();
    void testGetFirstNext();
    void testGetMask();
};


#endif // TESTCPICTURE_H
