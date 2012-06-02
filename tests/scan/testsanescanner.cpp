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
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestSaneScanner);

static void dumpImage(cf::ImagePtr im, const std::string& name) {
    cf::CTIControl::writeDIBtoBMP(name, (BitmapHandle) im->data());
}

#define ASSERT_STR_OPTION(name, value) {\
    std::string tmp;\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT_EQUAL(std::string(value), tmp);\
}

#define ASSERT_SET_STR_OPTION(name, value) {\
    std::string tmp;\
    CPPUNIT_ASSERT(s.setOption(name, std::string(value)));\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT_EQUAL(std::string(value), tmp);\
}

#define ASSERT_SET_STR_OPTION_FAIL(name, value) {\
    std::string tmp;\
    CPPUNIT_ASSERT(!s.setOption(name, std::string(value)));\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT(std::string(value) != tmp);\
}

#define ASSERT_BOOL_OPTION(name, value) {\
    bool tmp;\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT_EQUAL(bool(value), tmp);\
}

#define ASSERT_SET_BOOL_OPTION(name, value) {\
    bool tmp;\
    CPPUNIT_ASSERT(s.setOption(name, bool(value)));\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT_EQUAL(bool(value), tmp);\
}

#define ASSERT_FLOAT_OPTION(name, value) {\
    float tmp;\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT_EQUAL(float(value), tmp);\
}

#define ASSERT_SET_FLOAT_OPTION(name, value) {\
    float tmp;\
    CPPUNIT_ASSERT(s.setOption(name, float(value)));\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT_EQUAL(float(value), tmp);\
}

#define ASSERT_SET_FLOAT_OPTION_FAIL(name, value) {\
    float tmp;\
    CPPUNIT_ASSERT(!s.setOption(name, float(value)));\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT(float(value) != tmp);\
}

#define ASSERT_INT_OPTION(name, value) {\
    int tmp;\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT_EQUAL(int(value), tmp);\
}

#define ASSERT_SET_INT_OPTION(name, value) {\
    int tmp;\
    CPPUNIT_ASSERT(s.setOption(name, int(value)));\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT_EQUAL(int(value), tmp);\
}

#define ASSERT_SET_INT_OPTION_FAIL(name, value) {\
    int tmp;\
    CPPUNIT_ASSERT(!s.setOption(name, int(value)));\
    CPPUNIT_ASSERT(s.option(name, &tmp));\
    CPPUNIT_ASSERT(int(value) != tmp);\
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

    s.dumpOptions(std::cerr);
}

void TestSaneScanner::testSetOption()
{
    cf::SaneScanner s;
    CPPUNIT_ASSERT(s.open("test:0"));

    // bool
    CPPUNIT_ASSERT(s.hasOption("hand-scanner"));
    CPPUNIT_ASSERT(s.setOption("hand-scanner", true));
    CPPUNIT_ASSERT(s.setOption("hand-scanner", false));

    // int list
    CPPUNIT_ASSERT(s.hasOption("depth"));
    CPPUNIT_ASSERT(s.setOption("depth", 16));
    CPPUNIT_ASSERT(s.setOption("depth", 1));
    CPPUNIT_ASSERT(s.setOption("depth", 8));
    CPPUNIT_ASSERT(!s.setOption("depth", 1024));

    // float range
    float res = 0;
    CPPUNIT_ASSERT(s.hasOption("resolution"));
    CPPUNIT_ASSERT(s.option("resolution", &res));
    CPPUNIT_ASSERT_EQUAL(50.0f, res);
    CPPUNIT_ASSERT(s.setOption("resolution", 1.0f));
    CPPUNIT_ASSERT(s.option("resolution", &res));
    CPPUNIT_ASSERT_EQUAL(1.0f, res);

    // string list
    CPPUNIT_ASSERT(s.hasOption("mode"));
    CPPUNIT_ASSERT(!s.setOption("mode", std::string("Invalid")));
    CPPUNIT_ASSERT(s.setOption("mode", std::string("Color")));
    CPPUNIT_ASSERT(s.setOption("mode", std::string("Gray")));

    cf::Rect area(0, 0, 100, 150);
    CPPUNIT_ASSERT(s.setScanArea(area));
    CPPUNIT_ASSERT_EQUAL(area, s.scanArea());
}

void TestSaneScanner::testScanMode()
{
    cf::SaneScanner s;
    CPPUNIT_ASSERT(s.open("test:0"));

    ASSERT_SET_STR_OPTION("mode", "Gray");
    ASSERT_SET_STR_OPTION("mode", "Color");
    ASSERT_SET_STR_OPTION_FAIL("mode", "Invalid mode");
}

void TestSaneScanner::testScanDepth()
{
    cf::SaneScanner s;
    CPPUNIT_ASSERT(s.open("test:0"));

    ASSERT_INT_OPTION("depth", 8);

    ASSERT_SET_INT_OPTION("depth", 1);
    ASSERT_SET_INT_OPTION("depth", 8);
    ASSERT_SET_INT_OPTION("depth", 16);
    ASSERT_SET_INT_OPTION_FAIL("depth", 2);
}

void TestSaneScanner::testHandScanner()
{
    cf::SaneScanner s;
    CPPUNIT_ASSERT(s.open("test:0"));

    CPPUNIT_ASSERT_EQUAL(cf::Rect(0, 0, 80, 100), s.scanArea());
    ASSERT_BOOL_OPTION("hand-scanner", false);

    ASSERT_SET_BOOL_OPTION("hand-scanner", true);
    cf::ImagePtr im = s.start();
    CPPUNIT_ASSERT(im);
    CPPUNIT_ASSERT(im->height() != 0);
    CPPUNIT_ASSERT(im->width() != 0);

    dumpImage(im, "test_sane_hand_scanner.bmp");

    ASSERT_SET_BOOL_OPTION("hand-scanner", false);
}

void TestSaneScanner::testScanResolution()
{
    cf::SaneScanner s;
    CPPUNIT_ASSERT(s.open("test:0"));

    ASSERT_FLOAT_OPTION("resolution", 50);
    ASSERT_SET_FLOAT_OPTION("resolution", 1.1);
    ASSERT_SET_FLOAT_OPTION_FAIL("resolution", 0.5);

    for(int i = 1; i <= 1200; i++) {
        ASSERT_SET_FLOAT_OPTION("resolution", i);
    }

    ASSERT_SET_FLOAT_OPTION_FAIL("resolution", 1201);
    ASSERT_SET_FLOAT_OPTION_FAIL("resolution", -1);
}

void TestSaneScanner::testScanPicture()
{
    cf::SaneScanner s;
    CPPUNIT_ASSERT(s.open("test:0"));
    ASSERT_STR_OPTION("test-picture", "Solid black");

    ASSERT_SET_STR_OPTION("test-picture", "Solid white");

    cf::ImagePtr im = s.start();
    dumpImage(im, "test_sane_picture_white.bmp");
}
