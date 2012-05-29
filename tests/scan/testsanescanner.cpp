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

#include "testsanescanner.h"
#include "scan/sanescanner.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestSaneScanner);

void TestSaneScanner::testInit()
{
    cf::SaneScanner s;
    cf::SaneScanner::DeviceList lst = s.listDevices();

    for(size_t i = 0; i < lst.size(); i++) {
        std::cerr << lst.at(i) << "\n";
    }

    CPPUNIT_ASSERT(!lst.empty());
}

void TestSaneScanner::testOpen()
{
    cf::SaneScanner s;
    cf::SaneScanner::DeviceList lst = s.listDevices();
    CPPUNIT_ASSERT(!lst.empty());

    CPPUNIT_ASSERT(s.open(lst[0]));

    CPPUNIT_ASSERT(!s.open("not exists"));
}

void TestSaneScanner::testStart()
{
    cf::SaneScanner s;
    cf::SaneScanner::DeviceList lst = s.listDevices();
    CPPUNIT_ASSERT(!lst.empty());
    CPPUNIT_ASSERT(s.open(lst[0]));

    cf::ImagePtr im = s.start();
}
