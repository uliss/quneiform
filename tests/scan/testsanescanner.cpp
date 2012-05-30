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
#include "cimage/cticontrol.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestSaneScanner);

static void dumpImage(cf::ImagePtr im, const std::string& name) {
    cf::CTIControl::writeDIBtoBMP(name, (BitmapHandle) im->data());
}

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

    CPPUNIT_ASSERT(lst[0] == "test:0");
    CPPUNIT_ASSERT(s.open(lst[0]));

    CPPUNIT_ASSERT(s.hasOption("hand-scanner"));
    CPPUNIT_ASSERT(s.setOption("hand-scanner", true));

    CPPUNIT_ASSERT(s.hasOption("depth"));
    CPPUNIT_ASSERT(s.setOption("depth", 16));
    CPPUNIT_ASSERT(s.setOption("depth", 1));
    CPPUNIT_ASSERT(s.setOption("depth", 8));
    CPPUNIT_ASSERT(!s.setOption("depth", 1024));


    CPPUNIT_ASSERT(!s.open("not exists"));
}

void TestSaneScanner::testStart()
{
    cf::SaneScanner s;
    cf::SaneScanner::DeviceList lst = s.listDevices();
    CPPUNIT_ASSERT(!lst.empty());

    // not opened
    cf::ImagePtr im = s.start();
    CPPUNIT_ASSERT(!im.get());

    CPPUNIT_ASSERT(s.open(lst[0]));
    im = s.start();

    CPPUNIT_ASSERT(im.get());
    CPPUNIT_ASSERT(im->height() != 0);
    CPPUNIT_ASSERT(im->width() != 0);

    dumpImage(im, "test_sane_scan.bmp");

//    s.dumpOptions(std::cerr);
}
