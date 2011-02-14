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

    m4.add("path 1");
    QVERIFY(!m4.isEmpty());
    QCOMPARE(m4.actions().count(), 1);
    QCOMPARE(m4.actions().at(0)->text(), QString("path 1"));

    m4.add("path 2");
    QCOMPARE(m4.actions().count(), 2);
    QCOMPARE(m4.actions().at(0)->text(), QString("path 2"));
    QCOMPARE(m4.actions().at(1)->text(), QString("path 1"));

    m4.add("path 3");
    QCOMPARE(m4.actions().count(), 3);
    QCOMPARE(m4.actions().at(0)->text(), QString("path 3"));
    QCOMPARE(m4.actions().at(1)->text(), QString("path 2"));
    QCOMPARE(m4.actions().at(2)->text(), QString("path 1"));

    m4.add("path 4");
    QCOMPARE(m4.actions().count(), 4);
    QCOMPARE(m4.actions().at(0)->text(), QString("path 4"));
    QCOMPARE(m4.actions().at(1)->text(), QString("path 3"));
    QCOMPARE(m4.actions().at(2)->text(), QString("path 2"));
    QCOMPARE(m4.actions().at(3)->text(), QString("path 1"));

    m4.add("path 5");
    QCOMPARE(m4.actions().count(), 4);
    QCOMPARE(m4.actions().at(0)->text(), QString("path 5"));
    QCOMPARE(m4.actions().at(1)->text(), QString("path 4"));
    QCOMPARE(m4.actions().at(2)->text(), QString("path 3"));
    QCOMPARE(m4.actions().at(3)->text(), QString("path 2"));

    m4.add("path 2");
    QCOMPARE(m4.actions().count(), 4);
    QCOMPARE(m4.actions().at(0)->text(), QString("path 2"));
    QCOMPARE(m4.actions().at(1)->text(), QString("path 5"));
    QCOMPARE(m4.actions().at(2)->text(), QString("path 4"));
    QCOMPARE(m4.actions().at(3)->text(), QString("path 3"));

    m4.add("path 4");
    QCOMPARE(m4.actions().count(), 4);
    QCOMPARE(m4.actions().at(0)->text(), QString("path 4"));
    QCOMPARE(m4.actions().at(1)->text(), QString("path 2"));
    QCOMPARE(m4.actions().at(2)->text(), QString("path 5"));
    QCOMPARE(m4.actions().at(3)->text(), QString("path 3"));

    m4.clear();
}

void TestRecentMenu::testPersistant() {
    {
        RecentMenu m1(NULL, "Title", "testConstruct1", 1);
        m1.add("path");
    }

    {
        RecentMenu m1(NULL, "Title", "testConstruct1", 1);
        QVERIFY(!m1.isEmpty());
        QCOMPARE(m1.actions().first()->text(), QString("path"));
    }
}

void TestRecentMenu::testSelected() {
    RecentMenu m2(NULL, "Title", "testSelected", 2);
    m2.add("path 1");
    m2.add("path 2");
    QCOMPARE(m2.actions().count(), 2);

    QSignalSpy selected(&m2, SIGNAL(selected(QString)));

    // last action (path 2)
    m2.actions().at(0)->trigger();
    QCOMPARE(selected.count(), 1);
    QString path = selected.takeFirst().at(0).toString();
    QCOMPARE(path, QString("path 2"));

    m2.actions().at(1)->trigger();
    QCOMPARE(selected.count(), 1);
    path = selected.takeFirst().at(0).toString();
    QCOMPARE(path, QString("path 1"));
}

QTEST_MAIN(TestRecentMenu);
