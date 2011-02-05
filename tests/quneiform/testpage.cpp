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
#include <QSignalSpy>
#include "testpage.h"
#include "gui/page.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

#define SAMPLE_IMG CF_IMAGE_DIR "/croatian.bmp"

TestPage::TestPage(QObject *parent) :
    QObject(parent)
{
}

void TestPage::testAngle() {
    Page p(SAMPLE_IMG);

    p.rotate(90);
    QCOMPARE(p.angle(), 90);
    p.rotate(90);
    QCOMPARE(p.angle(), 180);
    p.rotate(90);
    QCOMPARE(p.angle(), 270);
    p.rotate(90);
    QCOMPARE(p.angle(), 0);

    p.rotate(-90);
    QCOMPARE(p.angle(), 270);
    p.rotate(-90);
    QCOMPARE(p.angle(), 180);
    p.rotate(-90);
    QCOMPARE(p.angle(), 90);
    p.rotate(-90);
    QCOMPARE(p.angle(), 0);
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
    QVERIFY(p.rects(Page::CHAR).empty());
    QVERIFY(p.rects(Page::COLUMN).empty());
    QVERIFY(p.rects(Page::LINE).empty());
    QVERIFY(p.rects(Page::SECTION).empty());
    QVERIFY(p.rects(Page::PARAGRAPH).empty());
    QVERIFY(p.rects(Page::PICTURE).empty());

    Page p2("none");
    QCOMPARE(p2.name(), QString("none"));
    QCOMPARE(p2.imagePath(), QString("none"));
    QVERIFY(p2.isNull());

    Page p3(SAMPLE_IMG);
    QCOMPARE(p3.name(), QString("croatian.bmp"));
    QCOMPARE(p3.imagePath(), QString(SAMPLE_IMG));
    QVERIFY(!p3.isNull());
    QCOMPARE(p3.imageSize(), QSize(640, 1390));
    QCOMPARE(p3.pageArea(), QRect());
}

void TestPage::testFlags() {
    Page p("");

    QVERIFY(p.flags() == Page::NONE);
    QVERIFY(!p.hasFlag(Page::RECOGNIZED));
    QVERIFY(!p.hasFlag(Page::RECOGNITION_FAILED));
    QVERIFY(!p.hasFlag(Page::SAVED));
    QVERIFY(!p.hasFlag(Page::SAVING_FAILED));

    p.setFlags(Page::SAVED);
    QVERIFY(!p.hasFlag(Page::RECOGNIZED));
    QVERIFY(!p.hasFlag(Page::RECOGNITION_FAILED));
    QVERIFY(p.hasFlag(Page::SAVED));
    QVERIFY(!p.hasFlag(Page::SAVING_FAILED));

    p.setFlag(Page::RECOGNIZED);
    QVERIFY(p.flags() == Page::RECOGNIZED | Page::SAVED);

    p.unsetFlag(Page::SAVED);
    QVERIFY(p.flags() == Page::RECOGNIZED);
    p.unsetFlag(Page::RECOGNIZED);
    QVERIFY(p.flags() == Page::NONE);
}

void TestPage::testPageName() {
    Page p("");

    QCOMPARE(p.name(), QString(""));
    QCOMPARE(p.imagePath(), QString(""));

    Page p2("name");
    QCOMPARE(p2.name(), QString("name"));
    QCOMPARE(p2.imagePath(), QString("name"));

    Page p3("name.txt");
    QCOMPARE(p3.name(), QString("name.txt"));
    QCOMPARE(p3.imagePath(), QString("name.txt"));

    Page p4("./name.txt");
    QCOMPARE(p4.name(), QString("name.txt"));
    QCOMPARE(p4.imagePath(), QString("./name.txt"));
}

void TestPage::testResetScale() {
    Page p(SAMPLE_IMG);
    p.rotate(90);
    p.scale(2);

    QSignalSpy spy(&p, SIGNAL(changed()));
    QSignalSpy spy2(&p, SIGNAL(transformed()));

    p.resetScale();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy2.count(), 1);
    QCOMPARE(p.angle(), 90);
    QTransform t;
    t.rotate(p.angle());
    QCOMPARE(p.transform(), t);
}

void TestPage::testRotate() {
    Page p(SAMPLE_IMG);
    QSignalSpy spy(&p, SIGNAL(changed()));
    QSignalSpy spy2(&p, SIGNAL(rotated(int)));

    p.rotate(90);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy2.count(), 1);
    QCOMPARE(p.angle(), 90);

    QList<QVariant> arguments = spy2.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 90);
}

void TestPage::testScale() {
    Page p(SAMPLE_IMG);
    QSignalSpy spy(&p, SIGNAL(changed()));
    QSignalSpy spy2(&p, SIGNAL(transformed()));

    p.scale(-1);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy2.count(), 1);
}

void TestPage::testSetFlag() {
    Page p("");
    QSignalSpy spy(&p, SIGNAL(changed()));

    p.setFlag(Page::SAVED);

    QVERIFY(p.hasFlag(Page::SAVED));
    QCOMPARE(spy.count(), 1);
}

void TestPage::testSetFlags() {
    Page p("");
    QSignalSpy spy(&p, SIGNAL(changed()));

    p.setFlags(Page::SAVED | Page::RECOGNIZED);

    QVERIFY(p.hasFlag(Page::SAVED));
    QVERIFY(p.hasFlag(Page::RECOGNIZED));
    QCOMPARE(p.flags(), Page::SAVED | Page::RECOGNIZED);
    QCOMPARE(spy.count(), 1);
}

void TestPage::testSetNumber() {
    Page p("");
    QSignalSpy spy(&p, SIGNAL(changed()));

    p.setNumber(10);

    QCOMPARE(p.number(), (unsigned int)10);
    QCOMPARE(spy.count(), 1);

    // same number - signal not emmited
    p.setNumber(10);
    QCOMPARE(spy.count(), 1);

    p.setNumber(11);
    QCOMPARE(spy.count(), 2);
}

void TestPage::testSetOcrText() {
    Page p("");
    QSignalSpy spy(&p, SIGNAL(changed()));
    QSignalSpy spy2(&p, SIGNAL(recognized()));

    p.setOcrText("sample");

    QCOMPARE(p.ocrText(), QString("sample"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy2.count(), 1);
}

void TestPage::testSetPageArea() {
    Page p("");
    QRect r(10, 20, 400, 500);
    QCOMPARE(p.pageArea(), QRect());

    QSignalSpy spy(&p, SIGNAL(changed()));

    p.setPageArea(r);

    QCOMPARE(p.pageArea(), r);
    QCOMPARE(spy.count(), 1);

    p.setPageArea(r);
    QCOMPARE(spy.count(), 1);

    p.setPageArea(r.adjusted(10, 10, 10, 10));
    QCOMPARE(spy.count(), 2);
}

void TestPage::testSetRecognizeSettings() {
    Page p("");
    RecognitionSettings s;
    s.setFax(true);

    QSignalSpy spy(&p, SIGNAL(changed()));
    p.setRecognizeOptions(s);

    QVERIFY(s == p.recognitionSettings());
    QCOMPARE(spy.count(), 1);

    p.setRecognizeOptions(s);
    QCOMPARE(spy.count(), 1);

    s.setOneColumn(true);
    p.setRecognizeOptions(s);
    QCOMPARE(spy.count(), 2);
}

QTEST_MAIN(TestPage)
