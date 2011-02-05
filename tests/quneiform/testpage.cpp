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
#include <QFile>
#include <QDataStream>
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
    p.setFlag(Page::SAVED);
    QSignalSpy spy(&p, SIGNAL(changed()));
    QSignalSpy spy2(&p, SIGNAL(recognized()));

    p.setOcrText("sample");

    QVERIFY(p.hasFlag(Page::RECOGNIZED));
    QVERIFY(!p.hasFlag(Page::SAVED));
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

void TestPage::testSetRecognitionSettings() {
    Page p("");
    RecognitionSettings s;
    s.setFax(true);
    p.setFlag(Page::SAVED);

    QSignalSpy spy(&p, SIGNAL(changed()));
    p.setRecognitionSettings(s);

    QVERIFY(!p.hasFlag(Page::SAVED));
    QVERIFY(s == p.recognitionSettings());
    QCOMPARE(spy.count(), 1);

    p.setRecognitionSettings(s);
    QCOMPARE(spy.count(), 1);

    s.setOneColumn(true);
    p.setRecognitionSettings(s);
    QCOMPARE(spy.count(), 2);


}

void TestPage::testSetRects() {
    Page p("");

    QVERIFY(p.rects(Page::CHAR).empty());
    QVERIFY(p.rects(Page::COLUMN).empty());
    QVERIFY(p.rects(Page::LINE).empty());
    QVERIFY(p.rects(Page::SECTION).empty());
    QVERIFY(p.rects(Page::PARAGRAPH).empty());
    QVERIFY(p.rects(Page::PICTURE).empty());

    QSignalSpy spy(&p, SIGNAL(changed()));

    Page::Rectangles rects;
    rects << QRect(0, 0, 10, 20);
    rects << QRect(1, 1, 2, 2);

    p.setRects(rects, Page::CHAR);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(p.rects(Page::CHAR).count(), 2);
    QCOMPARE(p.rects(Page::CHAR).at(0), QRect(0, 0, 10, 20));
    QCOMPARE(p.rects(Page::CHAR).at(1), QRect(1, 1, 2, 2));
    QVERIFY(p.rects(Page::COLUMN).empty());
    QVERIFY(p.rects(Page::LINE).empty());
    QVERIFY(p.rects(Page::SECTION).empty());
    QVERIFY(p.rects(Page::PARAGRAPH).empty());
    QVERIFY(p.rects(Page::PICTURE).empty());

    rects.removeLast();
    p.setRects(rects, Page::COLUMN);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(p.rects(Page::COLUMN).count(), 1);
    QCOMPARE(p.rects(Page::COLUMN).at(0), QRect(0, 0, 10, 20));
    QVERIFY(p.rects(Page::LINE).empty());
    QVERIFY(p.rects(Page::SECTION).empty());
    QVERIFY(p.rects(Page::PARAGRAPH).empty());
    QVERIFY(p.rects(Page::PICTURE).empty());

    rects.removeLast();
    p.setRects(rects, Page::LINE);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(p.rects(Page::LINE).count(), 0);
    QVERIFY(p.rects(Page::SECTION).empty());
    QVERIFY(p.rects(Page::PARAGRAPH).empty());
    QVERIFY(p.rects(Page::PICTURE).empty());

    rects << QRect(100, 100, 100, 100);
    p.setRects(rects, Page::SECTION);
    QCOMPARE(spy.count(), 4);
    QCOMPARE(p.rects(Page::SECTION).count(), 1);
    QCOMPARE(p.rects(Page::SECTION).at(0), QRect(100, 100, 100, 100));
    QVERIFY(p.rects(Page::PARAGRAPH).empty());
    QVERIFY(p.rects(Page::PICTURE).empty());

    rects << QRect(20, 20, 20, 20);
    p.setRects(rects, Page::PARAGRAPH);
    QCOMPARE(spy.count(), 5);
    QCOMPARE(p.rects(Page::PARAGRAPH).count(), 2);
    QCOMPARE(p.rects(Page::PARAGRAPH).at(0), QRect(100, 100, 100, 100));
    QCOMPARE(p.rects(Page::PARAGRAPH).at(1), QRect(20, 20, 20, 20));
    QVERIFY(p.rects(Page::PICTURE).empty());

    rects << QRect(30, 30, 30, 30);
    p.setRects(rects, Page::PICTURE);
    QCOMPARE(spy.count(), 6);
    QCOMPARE(p.rects(Page::PICTURE).count(), 3);
    QCOMPARE(p.rects(Page::PICTURE).at(0), QRect(100, 100, 100, 100));
    QCOMPARE(p.rects(Page::PICTURE).at(1), QRect(20, 20, 20, 20));
    QCOMPARE(p.rects(Page::PICTURE).at(2), QRect(30, 30, 30, 30));
}

void TestPage::testSetSelected() {
    Page p("");
    QVERIFY(!p.isSelected());

    QSignalSpy spy(&p, SIGNAL(changed()));
    p.setSelected(true);

    QCOMPARE(spy.count(), 1);
    QVERIFY(p.isSelected());

    p.setSelected(true);
    QCOMPARE(spy.count(), 1);
    QVERIFY(p.isSelected());

    p.setSelected(false);
    QCOMPARE(spy.count(), 2);
    QVERIFY(!p.isSelected());

    p.setSelected(false);
    QCOMPARE(spy.count(), 2);
}

void TestPage::testSetTransform() {
    Page p("");
    QCOMPARE(p.transform(), QTransform());

    QTransform t;
    QSignalSpy spy(&p, SIGNAL(changed()));
    QSignalSpy spy2(&p, SIGNAL(transformed()));

    p.setTransform(t);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(spy2.count(), 0);

    t.rotate(90);
    p.setTransform(t);
    QCOMPARE(p.transform(), t);
    QCOMPARE(p.angle(), 90);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy2.count(), 1);
}

void TestPage::testSetViewScroll() {
    Page p("");
    QCOMPARE(p.viewScroll(), QPoint());
    QSignalSpy spy(&p, SIGNAL(changed()));

    p.setViewScroll(QPoint(10, 20));
    // no signals
    QCOMPARE(spy.count(), 0);
    QCOMPARE(p.viewScroll(), QPoint(10, 20));
}

void TestPage::testUnsetFlag() {
    Page p("");
    p.setFlags(Page::SAVED | Page::RECOGNIZED);

    QSignalSpy spy(&p, SIGNAL(changed()));
    p.unsetFlag(Page::SAVED);
    QVERIFY(!p.hasFlag(Page::SAVED));
    QCOMPARE(spy.count(), 1);
    p.unsetFlag(Page::RECOGNIZED);
    QCOMPARE(spy.count(), 2);
    QVERIFY(!p.hasFlag(Page::RECOGNIZED));

    p.unsetFlag(Page::SAVING_FAILED);
    QCOMPARE(spy.count(), 2);
}

void TestPage::testReadWrite() {
    Page p(SAMPLE_IMG);
    int n = 10;
    QPoint pt(10, 20);
    QString t("sample text");
    QRect r(20, 30, 40, 50);
    QTransform tr;
    tr.rotate(90);
    p.setNumber(n);
    p.setViewScroll(pt);
    p.setOcrText(t);
    p.setPageArea(r);
    p.setTransform(tr);
    p.setSelected(true);

    RecognitionSettings s;
    s.setFax(true);
    p.setRecognitionSettings(s);

    Page::Rectangles rects;
    rects << r << r << r;
    p.setRects(rects, Page::CHAR);
    p.setRects(rects, Page::PICTURE);

    {
        QFile file("test_page.tmp");
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << p;
    }

    {
        Page p2("");
        QFile file("test_page.tmp");
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in >> p2;

        QCOMPARE(p.angle(), p2.angle());
        QCOMPARE(p.flags(), p2.flags());
        QCOMPARE(p.imagePath(), p2.imagePath());
        QCOMPARE(p.imageSize(), p2.imageSize());
        QCOMPARE(p.isNull(), p2.isNull());
        QCOMPARE(p.isRecognized(), p2.isRecognized());
        QCOMPARE(p.isSaved(), p2.isSaved());
        QCOMPARE(p.isSelected(), p2.isSelected());
        QCOMPARE(p.name(), p2.name());
        QCOMPARE(p.number(), p2.number());
        QCOMPARE(p.ocrText(), p2.ocrText());
        QCOMPARE(p.pageArea(), p2.pageArea());
        QCOMPARE(p.recognitionSettings(), p2.recognitionSettings());
        QCOMPARE(p.transform(), p2.transform());
        QCOMPARE(p.viewScroll(), p2.viewScroll());
        QCOMPARE(p.rects(Page::CHAR).count(), p2.rects(Page::CHAR).count());
        QCOMPARE(p.rects(Page::PICTURE).count(), p2.rects(Page::PICTURE).count());
    }
}

QTEST_MAIN(TestPage)
