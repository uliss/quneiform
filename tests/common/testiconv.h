/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef TESTICONV_H
#define TESTICONV_H

#include <cppunit/extensions/HelperMacros.h>

class TestIconv: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestIconv);
    CPPUNIT_TEST(testConvertChar);
    CPPUNIT_TEST(testConvertString);
    CPPUNIT_TEST_SUITE_END();
public:
    void testConvertChar();
    void testConvertString();
};

#endif // TESTICONV_H
