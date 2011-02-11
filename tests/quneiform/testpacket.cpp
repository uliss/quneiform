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
#include <QSignalSpy>
#include <QFile>
#include <QFileInfo>
#include "testpacket.h"
#include "gui/packet.h"
#include "gui/page.h"

TestPacket::TestPacket(QObject *parent) :
    QObject(parent)
{
}

void TestPacket::testConstruct() {
    Packet packet;
    QCOMPARE(packet.fileName(), QString("untitled.qpf"));
    QCOMPARE(packet.language(), -1);
    QVERIFY(!packet.isChanged());
    QVERIFY(packet.isNew());
}

void TestPacket::testPageSignals() {
    Packet packet;
    packet.append(new Page("page 1"));
    packet.append(new Page("page 2"));

    QSignalSpy changed(&packet, SIGNAL(changed()));

    packet.pageAt(0)->setSelected(true);
    QCOMPARE(changed.count(), 1);
    QVERIFY(packet.isChanged());
    // not repeated
    packet.pageAt(0)->setSelected(true);
    QCOMPARE(changed.count(), 1);

    packet.pageAt(0)->setSelected(false);
    QCOMPARE(changed.count(), 2);

    packet.pageAt(1)->setNumber(2);
    QCOMPARE(changed.count(), 3);

    packet.pageAt(1)->setOcrText("test");
    QCOMPARE(changed.count(), 4);

    packet.pageAt(0)->setPageArea(QRect(10, 10, 30, 40));
    QCOMPARE(changed.count(), 5);

    // no change
    packet.pageAt(0)->setViewScroll(QPoint(10, 20));
    QCOMPARE(changed.count(), 5);

    Page::Rectangles r;
    packet.pageAt(1)->setBlocks(r, Page::CHAR);
    QCOMPARE(changed.count(), 6);

    RecognitionSettings s;
    s.setDotMatrix(true);

    packet.pageAt(0)->setRecognitionSettings(s);
    QCOMPARE(changed.count(), 7);
}

void TestPacket::testAppend() {
    Packet packet;
    QSignalSpy changed(&packet, SIGNAL(changed()));
    QSignalSpy page_added(&packet, SIGNAL(pageAdded(Page*)));
    QSignalSpy image_duplication(&packet, SIGNAL(imageDuplicated(QString)));

    packet.append(new Page(""));

    QCOMPARE(changed.count(), 1);
    QCOMPARE(page_added.count(), 1);
    QVERIFY(packet.isChanged());
    QCOMPARE(packet.pageCount(), 1);
    QVERIFY(packet.pageAt(0)->parent() == &packet);
    QCOMPARE(image_duplication.count(), 0);

    packet.append(NULL);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(page_added.count(), 1);
    QCOMPARE(packet.pageCount(), 1);
    QCOMPARE(image_duplication.count(), 0);

    // duplication
    packet.append(new Page(""));
    QCOMPARE(changed.count(), 1);
    QCOMPARE(page_added.count(), 1);
    QCOMPARE(packet.pageCount(), 1);
    QCOMPARE(image_duplication.count(), 1);

    // allow duplication
    packet.append(new Page(""), true);
    QCOMPARE(changed.count(), 2);
    QCOMPARE(page_added.count(), 2);
    QVERIFY(packet.isChanged());
    QCOMPARE(packet.pageCount(), 2);
    QVERIFY(packet.pageAt(1)->parent() == &packet);
    QCOMPARE(image_duplication.count(), 1);
}

void TestPacket::testClear() {
    Packet packet;
    QSignalSpy changed(&packet, SIGNAL(changed()));
    QSignalSpy page_removed(&packet, SIGNAL(pageRemoved(Page*)));

    packet.clear();
    QCOMPARE(changed.count(), 0);
    QCOMPARE(page_removed.count(), 0);

    packet.append(new Page("path 1")); // +1 changed
    packet.append(new Page("path 2")); // +1 changed

    packet.clear();
    QCOMPARE(packet.pageCount(), 0);
    QCOMPARE(changed.count(), 3);
    QCOMPARE(page_removed.count(), 2);
}

void TestPacket::testCountSelected() {
    Packet packet;
    QCOMPARE(packet.countSelected(), 0);

    packet.append(new Page(""));
    QCOMPARE(packet.countSelected(), 0);

    packet.pageAt(0)->setSelected(true);
    QCOMPARE(packet.countSelected(), 1);

    packet.append(new Page("path"));
    QCOMPARE(packet.countSelected(), 1);
    packet.pageAt(1)->setSelected(true);
    QCOMPARE(packet.countSelected(), 2);

    packet.pageAt(0)->setSelected(false);
    QCOMPARE(packet.countSelected(), 1);

    packet.pageAt(1)->setSelected(false);
    QCOMPARE(packet.countSelected(), 0);
}

void TestPacket::testHasPage() {
    Packet packet;
    QVERIFY(!packet.hasPage(""));

    packet.append(new Page(""));
    QVERIFY(packet.hasPage(""));
    QVERIFY(!packet.hasPage("foo"));

    packet.append(new Page(""), true);
    QVERIFY(packet.hasPage(""));
    QVERIFY(!packet.hasPage("foo"));

    packet.append(new Page("foo"));
    QVERIFY(packet.hasPage(""));
    QVERIFY(packet.hasPage("foo"));
}

void TestPacket::testLanguage() {
    Packet packet;
    QSignalSpy changed(&packet, SIGNAL(changed()));
    QCOMPARE(packet.language(), -1);

    packet.setLanguage(1);
    QCOMPARE(packet.language(), 1);
    QCOMPARE(changed.count(), 1);
}

void TestPacket::testOpen() {
    Packet packet;
    QVERIFY(!packet.open("unknown file"));
    QString fname("packet.test");
    packet.append(new Page("path 1"));
    packet.save(fname);

    Packet packet2;
    QVERIFY(packet2.isNew());
    QVERIFY(!packet2.isChanged());

    packet2.open(fname);
    QCOMPARE(packet2.fileName(), QString("packet.test"));
    QVERIFY(!packet2.isNew());
    QVERIFY(!packet2.isChanged());
    QCOMPARE(packet2.pageCount(), 1);
    QCOMPARE(packet2.pageAt(0)->imagePath(), QString("path 1"));

    QFile f(fname);
    f.remove();
}

void TestPacket::testPage() {
    Packet packet;
    QCOMPARE(packet.pageAt(0), (Page*)0);
    QCOMPARE(packet.pageAt(1), (Page*)0);

    packet.append(new Page(""));
    QVERIFY(packet.pageAt(0) != 0);
    QCOMPARE(packet.pageAt(1), (Page*)0);
}

void TestPacket::testSave() {
    Packet packet;
    QVERIFY(packet.isNew());
    QString fname("packet.test");

    packet.append(new Page(""));
    packet.setLanguage(1);
    packet.save(fname);

    QFileInfo fi(fname);
    QVERIFY(fi.exists());
    QVERIFY(fi.size() > 0);
    QVERIFY(!packet.isNew());
    QVERIFY(!packet.isChanged());
    QCOMPARE(packet.fileName(), fname);

    QFile f(fname);
    f.remove(fname);
}

void TestPacket::testRemove() {
    Packet packet;
    Page * p = new Page("");
    QSignalSpy removed(&packet, SIGNAL(pageRemoved(Page*)));

    packet.append(p);
    QVERIFY(packet.isChanged());
    packet.save("packet.test");

    QVERIFY(!packet.isChanged());

    packet.append(new Page(""), true);
    QCOMPARE(packet.pageCount(), 2);

    QSignalSpy changed(&packet, SIGNAL(changed()));
    packet.remove(p);
    QCOMPARE(packet.pageCount(), 1);
    QCOMPARE(removed.count(), 1);
    QCOMPARE(changed.count(), 1);

    {
        Page * p = new Page("");
        packet.append(p, true);
        packet.append(p, true);
        packet.append(p, true);
        // delete duplicates
        changed.clear();
        packet.remove(p);
        QCOMPARE(removed.count(), 2);
        QCOMPARE(changed.count(), 1);
        QCOMPARE(packet.pageCount(), 1);
    }

    // do nothing
    packet.remove(NULL);
    QCOMPARE(removed.count(), 2);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(packet.pageCount(), 1);

    QFile f("packet.test");
    f.remove();
}

void TestPacket::testRemoveSelected() {
    Packet packet;
    Page * p = new Page("");
    QSignalSpy removed(&packet, SIGNAL(pageRemoved(Page*)));

    packet.append(p);
    QCOMPARE(packet.pageCount(), 1);

    QSignalSpy changed(&packet, SIGNAL(changed()));
    packet.removeSelected();
    QCOMPARE(packet.pageCount(), 1);
    QCOMPARE(removed.count(), 0);
    QCOMPARE(changed.count(), 0);

    p->setSelected(true);
    changed.clear();
    packet.removeSelected();

    QCOMPARE(packet.pageCount(), 0);
    QCOMPARE(removed.count(), 1);
    QCOMPARE(changed.count(), 1);
}

void TestPacket::testReadWrite() {
    QString fname("packet.test");
    Packet packet;

    {
        QFile file(fname);
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);

        packet.setLanguage(1);
        packet.append(new Page("test"));
        out << packet;
    }

    {
        Packet packet2;
        QFile file(fname);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);

        in >> packet2;

        QCOMPARE(packet.fileName(), packet2.fileName());
        QCOMPARE(packet.pageCount(), packet2.pageCount());
        QCOMPARE(packet.language(), packet2.language());
        QCOMPARE(packet2.pageAt(0)->imagePath(), QString("test"));
    }
}

QTEST_MAIN(TestPacket)
