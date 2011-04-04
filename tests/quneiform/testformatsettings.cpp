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
#include "testformatsettings.h"
#include "gui/formatsettings.h"

TestFormatSettings::TestFormatSettings(QObject * parent) :
    QObject(parent)
{
}

void TestFormatSettings::testConstruct() {
    FormatSettings s;
    QVERIFY(s.isBoldUsed());
    QVERIFY(s.isUnderlinedUsed());
    QVERIFY(s.isItalicUsed());
    QVERIFY(!s.isFontSizeUsed());
    QVERIFY(s.preserveLineBreaks());
}

QTEST_MAIN(TestFormatSettings);

