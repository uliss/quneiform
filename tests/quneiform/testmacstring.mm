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

#include <QTest>
#include <CoreFoundation/CFString.h>

#include "testmacstring.h"
#include "gui/macosx/macstring.h"

#define COMPARE_QSTR(s) \
    QCOMPARE(MacString::toQString(CFStringCreateWithCString(NULL, s, kCFStringEncodingUTF8)), QString::fromUtf8(s));

#define COMPARE_CF(s1, s2) \
    QCOMPARE( \
        (int) CFStringCompare( \
            CFStringCreateWithCString(NULL, s1, kCFStringEncodingUTF8), \
            MacString::toCFStringRef(s2), \
            0), \
        (int) kCFCompareEqualTo);

#define COMPARE_CF_STD(s) COMPARE_CF(s, std::string(s));
#define COMPARE_CF_QSTR(s) COMPARE_CF(s, QString::fromUtf8(s));
#define COMPARE_CF_CHR(s) COMPARE_CF(s, s);

void TestMacString::testConstruct()
{
    COMPARE_QSTR("ascii string");
    COMPARE_QSTR("utf-8 строка");

    COMPARE_CF_STD("ascii string");
    COMPARE_CF_STD("utf-8 строка");

    COMPARE_CF_QSTR("ascii string");
    COMPARE_CF_QSTR("utf-8 строка");

    COMPARE_CF_CHR("ascii string");
    COMPARE_CF_CHR("utf-8 строка");
}

QTEST_APPLESS_MAIN(TestMacString)

