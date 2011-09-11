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
#include <QTimer>
#include <QTextDocument>
#include "testpagerecognitionqueue.h"
#include "gui/page.h"
#include "gui/packet.h"
#include "gui/pagerecognizer.h"

#define private public
#include "gui/pagerecognitionqueue.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

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

void TestPageRecognitionQueue::testStart() {
    PageRecognitionQueue q;
    QSignalSpy started(&q, SIGNAL(started()));
    QSignalSpy finished(&q, SIGNAL(finished(int)));

    q.start();
    QCOMPARE(started.count(), 1);
    QCOMPARE(finished.count(), 1);
    QCOMPARE(finished.at(0).at(0).toInt(), 0);
    finished.clear();

    Page p("none");
    q.add(&p);
    QCOMPARE(q.pageCount(), 1);
    q.start();
    QVERIFY(p.hasFlag(Page::RECOGNITION_FAILED));
    QVERIFY(!p.hasFlag(Page::RECOGNIZED));
    QVERIFY(q.isEmpty());
    QVERIFY(q.isFailed("none"));

    Page p1(CF_IMAGE_DIR "/invalid.png");
    q.add(&p1);
    q.start();
    QVERIFY(p1.hasFlag(Page::RECOGNITION_FAILED));
    QVERIFY(!p1.hasFlag(Page::RECOGNIZED));
    QVERIFY(q.isFailed(CF_IMAGE_DIR "/invalid.png"));

    finished.clear();
    Page p2(CF_IMAGE_DIR "/english.png");
    q.add(&p2);
    q.start();
    QVERIFY(!p2.hasFlag(Page::RECOGNITION_FAILED));
    QVERIFY(p2.hasFlag(Page::RECOGNIZED));
    QCOMPARE(p2.document()->toPlainText().trimmed(), QString("ENGLISH"));
    QVERIFY(!q.isFailed(CF_IMAGE_DIR "/english.png"));
    QCOMPARE(finished.at(0).at(0).toInt(), 1);
}

void TestPageRecognitionQueue::testSetLanguage() {
    PageRecognitionQueue q;
    QSignalSpy finished(&q, SIGNAL(finished(int)));
    Page p1(CF_IMAGE_DIR "/english.png");
    Page p2(CF_IMAGE_DIR "/russian.png");
    q.add(&p1);
    q.add(&p2);

    q.start();
    QCOMPARE(p1.document()->toPlainText().trimmed(), QString("ENGLISH"));
    QCOMPARE(p2.document()->toPlainText().trimmed(), QString("PYCCK VI Vl"));
    QCOMPARE(q.pageErrorNum(), 0);
    QCOMPARE(finished.at(0).at(0).toInt(), 2);
}

void TestPageRecognitionQueue::testEmitStep() {
    PageRecognitionQueue q;
    Page p1(CF_IMAGE_DIR "/english.png");
    QSignalSpy percents(&q, SIGNAL(percentDone(int)));

    q.start();
    QCOMPARE(percents.count(), 0);
    QCOMPARE(q.pageErrorNum(), 0);

    q.add(&p1);
    q.start();
    QCOMPARE(percents.count(), 6);
    QCOMPARE(percents.at(0).at(0).toInt(), 1);
    QCOMPARE(percents.at(1).at(0).toInt(), 10);
    QCOMPARE(percents.at(2).at(0).toInt(), 26);
    QCOMPARE(percents.at(3).at(0).toInt(), 34);
    QCOMPARE(percents.at(4).at(0).toInt(), 90);
    QCOMPARE(percents.at(5).at(0).toInt(), 100);

    Page p2("none");
    Page p3(CF_IMAGE_DIR "/invalid.png");
    percents.clear();
    q.add(&p2);
    q.add(&p3);
    q.start();

    QCOMPARE(percents.count(), 6);
    QCOMPARE(q.pageErrorNum(), 2);
}

void TestPageRecognitionQueue::testIsFailed() {
    PageRecognitionQueue q;
    Page p("none");
    q.add(&p);

    QVERIFY(!q.isFailed("none"));
    q.start();

    QVERIFY(q.isFailed("none"));
    QVERIFY(!q.isFailed("none2"));
}

void TestPageRecognitionQueue::testAbort() {
//    PageRecognitionQueue q;
//    q.recognizer()->setStageSleep(PageRecognizer::OPEN, 50);
//    QSignalSpy finished(&q, SIGNAL(finished(int)));
//    Page p1(CF_IMAGE_DIR "/english.png");
//    Page p2(CF_IMAGE_DIR "/russian.png");

//    q.add(&p1);
//    q.add(&p2);
//    QCOMPARE(q.pageCount(), 2);

//    // abort second page
//    QTimer::singleShot(90, &q, SLOT(abort()));
//    q.start();

//    QCOMPARE(q.pageCount(), 0);
//    QVERIFY(q.isEmpty());
//    QCOMPARE(finished.count(), 1);
//    QCOMPARE(finished.at(0).at(0).toInt(), 1);

//    // no pages left after abort
//    finished.clear();
//    q.start();
//    QCOMPARE(finished.count(), 1);
//    QCOMPARE(finished.at(0).at(0).toInt(), 0);

//    // again after abort
//    finished.clear();
//    q.add(&p2);
//    QSignalSpy page_done(q.recognizer(), SIGNAL(done()));
//    QCOMPARE(q.pageCount(), 1);
//    q.start();
//    QCOMPARE(page_done.count(), 1);
//    QCOMPARE(finished.count(), 1);
//    QCOMPARE(finished.at(0).at(0).toInt(), 1);
}

void TestPageRecognitionQueue::testPercentDone() {
    PageRecognitionQueue q;
    QSignalSpy percents(&q, SIGNAL(percentDone(int)));
    Page p1(CF_IMAGE_DIR "/english.png");

    q.add(&p1);
    q.start();

    QCOMPARE(percents.count(), 6);
    QCOMPARE(percents.at(0).at(0).toInt(), 1);
    QCOMPARE(percents.at(1).at(0).toInt(), 10);
    QCOMPARE(percents.at(2).at(0).toInt(), 26);
    QCOMPARE(percents.at(3).at(0).toInt(), 34);
    QCOMPARE(percents.at(4).at(0).toInt(), 90);
    QCOMPARE(percents.at(5).at(0).toInt(), 100);

    percents.clear();

    Page p2(CF_IMAGE_DIR "/russian.png");
    q.add(&p1);
    q.add(&p2);
    q.start();

    QCOMPARE(percents.count(), 12);
    QCOMPARE(percents.at(0).at(0).toInt(), 0);
    QCOMPARE(percents.at(1).at(0).toInt(), 5);
    QCOMPARE(percents.at(2).at(0).toInt(), 13);
    QCOMPARE(percents.at(3).at(0).toInt(), 17);
    QCOMPARE(percents.at(4).at(0).toInt(), 45);
    QCOMPARE(percents.at(5).at(0).toInt(), 50);
    QCOMPARE(percents.at(6).at(0).toInt(), 50);
    QCOMPARE(percents.at(7).at(0).toInt(), 55);
    QCOMPARE(percents.at(8).at(0).toInt(), 63);
    QCOMPARE(percents.at(9).at(0).toInt(), 67);
    QCOMPARE(percents.at(10).at(0).toInt(), 95);
    QCOMPARE(percents.at(11).at(0).toInt(), 100);
}

QTEST_MAIN(TestPageRecognitionQueue);

