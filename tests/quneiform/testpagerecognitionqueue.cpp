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
#include "testpagerecognitionqueue.h"
#include "gui/pagerecognitionqueue.h"
#include "gui/page.h"
#include "gui/packet.h"

TestPageRecognitionQueue::TestPageRecognitionQueue(QObject * parent) :
    QObject(parent)
{
}

void TestPageRecognitionQueue::testConstruct() {
    PageRecognitionQueue q;
    QVERIFY(q.isEmpty());
    QCOMPARE(q.pageCount(), 0);
}

void TestPageRecognitionQueue::testAddPacket() {
    PageRecognitionQueue q;
    Packet p;

    q.add(&p);
    QVERIFY(q.isEmpty());

    p.append(new Page("path"));
    q.add(&p);
    QCOMPARE(q.pageCount(), 1);

    // test for duplication
    q.add(&p);
    QCOMPARE(q.pageCount(), 1);

    p.append(new Page("path 2"));
    q.add(&p);
    QCOMPARE(q.pageCount(), 2);
}

void TestPageRecognitionQueue::testAddPage() {
    PageRecognitionQueue q;
    Page p("path");

    q.add(&p);
    QVERIFY(!q.isEmpty());
    QCOMPARE(q.pageCount(), 1);

    // test for duplication
    q.add(&p);
    QCOMPARE(q.pageCount(), 1);

    Page p2("path 2");
    q.add(&p2);
    QCOMPARE(q.pageCount(), 2);
}

QTEST_MAIN(TestPageRecognitionQueue);

