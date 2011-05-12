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
#include <QDebug>
#include <QThread>
#include <QTextDocument>
#include "testpagerecognizer.h"
#include "gui/page.h"
#include "common/lang_def.h"

#define private public
#include "gui/pagerecognizer.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

TestPageRecognizer::TestPageRecognizer(QObject *parent) :
    QObject(parent)
{
}

void TestPageRecognizer::testConstruct() {
    PageRecognizer r;
}

void TestPageRecognizer::testRecognize() {
    PageRecognizer r;
    QSignalSpy failed(&r, SIGNAL(failed(QString)));
    QSignalSpy opened(&r, SIGNAL(opened()));
    QSignalSpy formatted(&r, SIGNAL(formatted()));
    QSignalSpy recognized(&r, SIGNAL(recognized()));

    // NULL page
    r.setPage(NULL);
    r.recognize();
    QCOMPARE(failed.count(), 1);
    QCOMPARE(opened.count(), 0);
    QCOMPARE(formatted.count(), 0);
    QCOMPARE(recognized.count(), 0);

    failed.clear();

    // non-exist page
    Page * none = new Page("none");
    r.setPage(none);
    r.recognize();
    QCOMPARE(failed.count(), 1);
    QCOMPARE(opened.count(), 0);
    QCOMPARE(formatted.count(), 0);
    QCOMPARE(recognized.count(), 0);
    QVERIFY(none->hasFlag(Page::RECOGNITION_FAILED));

    failed.clear();

    // invalid page image format
    Page * invalid = new Page(CF_IMAGE_DIR "/invalid.png");
    r.setPage(invalid);
    r.recognize();
    QCOMPARE(failed.count(), 1);
    QCOMPARE(opened.count(), 0);
    QCOMPARE(formatted.count(), 0);
    QCOMPARE(recognized.count(), 0);
    QVERIFY(invalid->hasFlag(Page::RECOGNITION_FAILED));

    failed.clear();

    // valid english
    Page * eng = new Page(CF_IMAGE_DIR "/english.png");
    r.setPage(eng);
    r.recognize();
    QCOMPARE(eng->document()->toPlainText().trimmed(), QString("ENGLISH"));
    QCOMPARE(failed.count(), 0);
    QCOMPARE(opened.count(), 1);
    QCOMPARE(formatted.count(), 1);
    QCOMPARE(recognized.count(), 1);
    QVERIFY(!eng->hasFlag(Page::RECOGNITION_FAILED));

    // valid russian
    Page * rus = new Page(CF_IMAGE_DIR "/russian.png");
    r.setPage(rus);
    r.recognize();
    QCOMPARE(rus->document()->toPlainText().trimmed(), QString("PYCCK VI Vl"));
    QVERIFY(rus->isRecognized());
    QVERIFY(!rus->hasFlag(Page::RECOGNITION_FAILED));

    rus->setLanguage(Language(::LANGUAGE_RUSSIAN));
    r.recognize();
    QCOMPARE(rus->document()->toPlainText().trimmed(), QString::fromUtf8("РУССКИЙ"));

    delete none;
    delete eng;
    delete rus;
}

void TestPageRecognizer::testRecognizeRotated() {
    PageRecognizer r;

    // 90
    Page eng90(CF_IMAGE_DIR "/english_rotated_90.png");
    eng90.rotate(-90);

    r.setPage(&eng90);
    r.recognize();

    QVERIFY(eng90.isRecognized());
    QCOMPARE(eng90.document()->toPlainText().trimmed(), QString("ENGLISH"));

    // 180
    Page eng180(CF_IMAGE_DIR "/english_rotated_180.png");
    eng180.rotate(-180);

    r.setPage(&eng180);
    r.recognize();

    QVERIFY(eng180.isRecognized());
    QCOMPARE(eng180.document()->toPlainText().trimmed(), QString("ENGLISH"));

    // 270
    Page eng270(CF_IMAGE_DIR "/english_rotated_270.png");
    eng270.rotate(-270);

    r.setPage(&eng270);
    r.recognize();

    QVERIFY(eng270.isRecognized());
    QCOMPARE(eng270.document()->toPlainText().trimmed(), QString("ENGLISH"));
}

void TestPageRecognizer::testRecognizeArea() {
    PageRecognizer r;
    QRect area(136, 5, 140, 70);

    // 0
    Page eng(CF_IMAGE_DIR "/english.png");
    eng.setPageArea(area);
    r.setPage(&eng);
    r.recognize();

    QVERIFY(eng.isRecognized());
    QCOMPARE(eng.document()->toPlainText().trimmed(), QString("LISH"));

    // 90
    Page eng90(CF_IMAGE_DIR "/english_rotated_90.png");
    eng90.rotate(-90);
    eng90.setPageArea(area);
    r.setPage(&eng90);
    r.recognize();

    QVERIFY(eng90.isRecognized());
    QCOMPARE(eng90.document()->toPlainText().trimmed(), QString("LISH"));

    // 180
    Page eng180(CF_IMAGE_DIR "/english_rotated_180.png");
    eng180.rotate(-180);
    eng180.setPageArea(area);
    r.setPage(&eng180);
    r.recognize();

    QVERIFY(eng180.isRecognized());
    QCOMPARE(eng180.document()->toPlainText().trimmed(), QString("LISH"));

    // 270
    Page eng270(CF_IMAGE_DIR "/english_rotated_270.png");
    eng270.rotate(-270);
    eng270.setPageArea(area);
    r.setPage(&eng270);
    r.recognize();

    QVERIFY(eng270.isRecognized());
    QCOMPARE(eng270.document()->toPlainText().trimmed(), QString("LISH"));
}

void TestPageRecognizer::testLoadImage() {
    PageRecognizer r;
    Page * eng = new Page(CF_IMAGE_DIR "/english.png");
    r.setPage(eng);
    QCOMPARE(eng->imageSize(), QSize(281, 81));

    QImage img = r.loadImage();
    QCOMPARE(img.size(), eng->imageSize());

    eng->setPageArea(QRect(1, 2, 30, 40));
    QCOMPARE(r.loadImage().size(), QSize(30, 40));

    eng->rotate(90);
    QCOMPARE(r.loadImage().size(), QSize(30, 40));

    eng->scaleView(12);
    QCOMPARE(r.loadImage().size(), QSize(30, 40));

    delete eng;
}

void TestPageRecognizer::testAbort() {
    PageRecognizer r;
    QSignalSpy aborted(&r, SIGNAL(aborted()));

    Page p(CF_IMAGE_DIR "/english.png");
    r.setPage(&p);
    QTimer::singleShot(0, &r, SLOT(abort()));
    QCoreApplication::processEvents();

    QCOMPARE(aborted.count(), 1);
    QVERIFY(!p.hasFlag(Page::RECOGNIZED));

    // abort only once
    bool ok = r.recognize();
    QVERIFY(!ok);

    // try again
    ok = r.recognize();
    QVERIFY(ok);
    QVERIFY(p.hasFlag(Page::RECOGNIZED));

    // again block
    QTimer::singleShot(10, &r, SLOT(abort()));
    r.setStageSleep(PageRecognizer::OPEN, 100);
    ok = r.recognize();
    QCOMPARE(aborted.count(), 2);
    QVERIFY(!ok);
}

void TestPageRecognizer::testPercents() {
    PageRecognizer r;
    QSignalSpy percents(&r, SIGNAL(percentsDone(int)));
    Page eng(CF_IMAGE_DIR "/english.png");
    r.setPage(&eng);
    r.recognize();

    QCOMPARE(percents.count(), 7);
    QCOMPARE(percents.at(0).at(0).toInt(), 10);
    QCOMPARE(percents.at(1).at(0).toInt(), 20);
    QCOMPARE(percents.at(2).at(0).toInt(), 30);
    QCOMPARE(percents.at(3).at(0).toInt(), 80);
    QCOMPARE(percents.at(4).at(0).toInt(), 90);
    QCOMPARE(percents.at(5).at(0).toInt(), 95);
    QCOMPARE(percents.at(6).at(0).toInt(), 100);
}

void TestPageRecognizer::testSlotConnections() {
    PageRecognizer r;
    Page eng(CF_IMAGE_DIR "/english.png");
    r.setPage(&eng);
    connect(this, SIGNAL(test()), &r, SLOT(recognize()));
    QSignalSpy done(&r, SIGNAL(done()));
    emit test();
    QCOMPARE(done.count(), 1);

    disconnect(this, SIGNAL(test()), &r, SLOT(recognize()));
    connect(this, SIGNAL(test()), &r, SLOT(abort()));
    QSignalSpy aborted(&r, SIGNAL(aborted()));
    emit test();
    QCOMPARE(done.count(), 1);
    QCOMPARE(aborted.count(), 1);
}

QTEST_MAIN(TestPageRecognizer);
