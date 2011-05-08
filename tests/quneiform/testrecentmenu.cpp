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
#include <QDebug>
#include <QSignalSpy>
#include "testrecentmenu.h"
#include "gui/recentmenu.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR
#endif

#define PATH1 QString(CF_IMAGE_DIR "/english.png")
#define PATH2 QString(CF_IMAGE_DIR "/english_rotated_90.png")
#define PATH3 QString(CF_IMAGE_DIR "/english_rotated_180.png")
#define PATH4 QString(CF_IMAGE_DIR "/english_rotated_270.png")
#define PATH5 QString(CF_IMAGE_DIR "/russian.png")

#define QF_COMPARE(menu, idx, str) QCOMPARE(menu.actions().at(idx)->text(), str);

TestRecentMenu::TestRecentMenu(QObject *parent) :
    QObject(parent)
{
}

void TestRecentMenu::testConstruct() {
    RecentMenu m(NULL, "Title", "testConstruct", 10);
    QCOMPARE(m.title(), QString("Title"));
    QVERIFY(m.isEmpty());
    QVERIFY(m.actions().isEmpty());

    RecentMenu m0(NULL, "Title", "testConstruct", 0);
    m0.add("test");
    QVERIFY(m0.isEmpty());
    m0.add("test");
    QVERIFY(m0.isEmpty());
}

void TestRecentMenu::testAdd() {
    RecentMenu m4(NULL, "Title", "testAdd", 4);
    m4.clear();
    QVERIFY(m4.isEmpty());

    m4.add("non-exist");
    QVERIFY(m4.isEmpty());

    m4.add(PATH1);
    QVERIFY(!m4.isEmpty());
    QCOMPARE(m4.actions().count(), 1);
    QF_COMPARE(m4, 0, PATH1);

    m4.add(PATH2);
    QCOMPARE(m4.actions().count(), 2);
    QF_COMPARE(m4, 0, PATH2);
    QF_COMPARE(m4, 1, PATH1);

    m4.add(PATH3);
    QCOMPARE(m4.actions().count(), 3);
    QF_COMPARE(m4, 0, PATH3);
    QF_COMPARE(m4, 1, PATH2);
    QF_COMPARE(m4, 2, PATH1);

    m4.add(PATH4);
    QCOMPARE(m4.actions().count(), 4);
    QF_COMPARE(m4, 0, PATH4);
    QF_COMPARE(m4, 1, PATH3);
    QF_COMPARE(m4, 2, PATH2);
    QF_COMPARE(m4, 3, PATH1);

    m4.add(PATH5);
    QCOMPARE(m4.actions().count(), 4);
    QF_COMPARE(m4, 0, PATH5);
    QF_COMPARE(m4, 1, PATH4);
    QF_COMPARE(m4, 2, PATH3);
    QF_COMPARE(m4, 3, PATH2);

    m4.add(PATH2);
    QCOMPARE(m4.actions().count(), 4);
    QF_COMPARE(m4, 0, PATH2);
    QF_COMPARE(m4, 1, PATH5);
    QF_COMPARE(m4, 2,  PATH4);
    QF_COMPARE(m4, 3, PATH3);

    m4.add(PATH4);
    QCOMPARE(m4.actions().count(), 4);
    QF_COMPARE(m4, 0, PATH4);
    QF_COMPARE(m4, 1, PATH2);
    QF_COMPARE(m4, 2, PATH5);
    QF_COMPARE(m4, 3, PATH3);

    m4.clear();
}

void TestRecentMenu::testPersistant() {
    {
        RecentMenu m1(NULL, "Title", "testConstruct1", 1);
        m1.add(PATH1);
    }

    {
        RecentMenu m1(NULL, "Title", "testConstruct1", 1);
        QVERIFY(!m1.isEmpty());
        QF_COMPARE(m1, 0, PATH1);
    }
}

void TestRecentMenu::testSelected() {
    RecentMenu m2(NULL, "Title", "testSelected", 2);
    m2.add(PATH1);
    m2.add(PATH2);
    QCOMPARE(m2.actions().count(), 2);

    QSignalSpy selected(&m2, SIGNAL(selected(QString)));

    // last action (path 2)
    m2.actions().at(0)->trigger();
    QCOMPARE(selected.count(), 1);
    QString path = selected.takeFirst().at(0).toString();
    QCOMPARE(path, PATH2);

    m2.actions().at(1)->trigger();
    QCOMPARE(selected.count(), 1);
    path = selected.takeFirst().at(0).toString();
    QCOMPARE(path, PATH1);
}

QTEST_MAIN(TestRecentMenu);
