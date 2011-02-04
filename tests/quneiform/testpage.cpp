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

#include <QtTest>
#include "testpage.h"
#include "gui/page.h"

TestPage::TestPage(QObject *parent) :
    QObject(parent)
{
}

void TestPage::testConstruct() {
    Page p("");

    QCOMPARE(p.name(), QString(""));
    QCOMPARE(p.angle(), 0);
    QVERIFY(p.flags() == Page::NONE);
    QCOMPARE(p.imagePath(), QString(""));
    QCOMPARE(p.imageSize(), QSize());
    QVERIFY(p.isNull());
    QVERIFY(!p.isRecognized());
    QVERIFY(!p.isSaved());
    QVERIFY(!p.isSelected());
    QVERIFY(p.number() == 0);
    QCOMPARE(p.ocrText(), QString(""));
    QCOMPARE(p.pageArea(), QRect());
    QCOMPARE(p.transform(), QTransform());
    QCOMPARE(p.viewScroll(), QPoint());
}

QTEST_MAIN(TestPage)
