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

#ifndef TESTCRTFHAR_H_
#define TESTCRTFHAR_H_

#include <cppunit/extensions/HelperMacros.h>

class TestCRtfChar: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCRtfChar);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
};

#endif /* TESTCRTFHAR_H_ */
