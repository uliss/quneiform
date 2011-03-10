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
#include <QApplication>
#include <QMessageBox>
#include "testpage.h"
#include "gui/page.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

#define SAMPLE_IMG CF_IMAGE_DIR "/english.png"


#define QVERIFY_THROW(expression, ExpectedExceptionType) \
    do { \
        bool caught_ = false; \
        try { expression; } \
        catch (ExpectedExceptionType const&) { caught_ = true; } \
        catch (...) {} \
        if (!QTest::qVerify(caught_, #expression ", " #ExpectedExceptionType, "", __FILE__, __LINE__))\
            return; \
    } while(0)


void CHECK_SMALL_FILE(const QString& fname, const QString& content) {
    Q_ASSERT(content.length() < 1024);

    QFile file(fname);
    if (file.open(QFile::ReadOnly)) {
        char buf[1024];
        qint64 lineLength = file.readLine(buf, sizeof(buf));
        if (lineLength != -1)
            QCOMPARE(content, QString(buf));
        else
            QFAIL("can't read from file");
    }
    else
        QFAIL("can't open file");
}

TestPage::TestPage(QObject *parent) :
    QObject(parent)
{
}

void TestPage::callTimerSlot(const char * member, int msec) {
    QTimer::singleShot(msec, this, member);
}

void TestPage::sendDialogAccept() {
    QWidget * dialog = QApplication::activeModalWidget();
    QMessageBox * box = qobject_cast<QMessageBox*>(dialog);
    if(box)
        box->accept();
}

void TestPage::sendDialogCancel() {
    QWidget * dialog = QApplication::activeModalWidget();
    QMessageBox * box = qobject_cast<QMessageBox*>(dialog);
    if(box)
        QTest::keyClick(box, Qt::Key_Escape);
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
    QVERIFY(!p.isExported());
    QVERIFY(!p.isSelected());
    QVERIFY(p.number() == 0);
    QCOMPARE(p.ocrText(), QString(""));
    QCOMPARE(p.pageArea(), QRect());
    QCOMPARE(p.transform(), QTransform());
    QCOMPARE(p.viewScroll(), QPoint());
    QVERIFY(p.blocks(Page::CHAR).empty());
    QVERIFY(p.blocks(Page::COLUMN).empty());
    QVERIFY(p.blocks(Page::LINE).empty());
    QVERIFY(p.blocks(Page::SECTION).empty());
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    Page p2("none");
    QCOMPARE(p2.name(), QString("none"));
    QCOMPARE(p2.imagePath(), QString("none"));
    QVERIFY(p2.isNull());

    Page p3(SAMPLE_IMG);
    QCOMPARE(p3.name(), QString("english.png"));
    QCOMPARE(p3.imagePath(), QString(SAMPLE_IMG));
    QVERIFY(!p3.isNull());
    QCOMPARE(p3.imageSize(), QSize(281, 81));
    QCOMPARE(p3.pageArea(), QRect());
}

void TestPage::testFlags() {
    Page p("");

    QVERIFY(p.flags() == Page::NONE);
    QVERIFY(!p.hasFlag(Page::RECOGNIZED));
    QVERIFY(!p.hasFlag(Page::RECOGNITION_FAILED));
    QVERIFY(!p.hasFlag(Page::EXPORTED));
    QVERIFY(!p.hasFlag(Page::EXPORT_FAILED));

    p.setFlags(Page::EXPORTED);
    QVERIFY(!p.hasFlag(Page::RECOGNIZED));
    QVERIFY(!p.hasFlag(Page::RECOGNITION_FAILED));
    QVERIFY(p.hasFlag(Page::EXPORTED));
    QVERIFY(!p.hasFlag(Page::EXPORT_FAILED));

    p.setFlag(Page::RECOGNIZED);
    QVERIFY(p.flags() == Page::RECOGNIZED | Page::EXPORTED);

    p.unsetFlag(Page::EXPORTED);
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

    QSignalSpy changed(&p, SIGNAL(changed()));
    QSignalSpy transformed(&p, SIGNAL(transformed()));

    p.resetScale();
    QCOMPARE(changed.count(), 1);
    QCOMPARE(transformed.count(), 1);
    QCOMPARE(p.angle(), 90);
    QTransform t;
    t.rotate(p.angle());
    QCOMPARE(p.transform(), t);
}

void TestPage::testResetTransform() {
    Page p(SAMPLE_IMG);
    p.rotate(90);
    p.scale(2);

    QSignalSpy changed(&p, SIGNAL(changed()));
    QSignalSpy transformed(&p, SIGNAL(transformed()));

    p.resetTransform();
    QCOMPARE(changed.count(), 1);
    QCOMPARE(transformed.count(), 1);
}

void TestPage::testRotate() {
    Page p(SAMPLE_IMG);
    QSignalSpy changed(&p, SIGNAL(changed()));
    QSignalSpy rotated(&p, SIGNAL(rotated(int)));

    p.rotate(90);

    QCOMPARE(changed.count(), 1);
    QCOMPARE(rotated.count(), 1);
    QCOMPARE(p.angle(), 90);

    QList<QVariant> arguments = rotated.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 90);
}

void TestPage::testScale() {
    Page p(SAMPLE_IMG);
    QSignalSpy changed(&p, SIGNAL(changed()));
    QSignalSpy transformed(&p, SIGNAL(transformed()));

    p.scale(-1);

    QCOMPARE(changed.count(), 1);
    QCOMPARE(transformed.count(), 1);
}

void TestPage::testSetFlag() {
    Page p("");
    QSignalSpy changed(&p, SIGNAL(changed()));

    p.setFlag(Page::EXPORTED);

    QVERIFY(p.hasFlag(Page::EXPORTED));
    QCOMPARE(changed.count(), 1);
}

void TestPage::testSetFlags() {
    Page p("");
    QSignalSpy changed(&p, SIGNAL(changed()));

    p.setFlags(Page::EXPORTED | Page::RECOGNIZED);

    QVERIFY(p.hasFlag(Page::EXPORTED));
    QVERIFY(p.hasFlag(Page::RECOGNIZED));
    QCOMPARE(p.flags(), Page::EXPORTED | Page::RECOGNIZED);
    QCOMPARE(changed.count(), 1);
}

void TestPage::testSetNumber() {
    Page p("");
    QSignalSpy changed(&p, SIGNAL(changed()));

    p.setNumber(10);

    QCOMPARE(p.number(), (unsigned int)10);
    QCOMPARE(changed.count(), 1);

    // same number - signal not emmited
    p.setNumber(10);
    QCOMPARE(changed.count(), 1);

    p.setNumber(11);
    QCOMPARE(changed.count(), 2);
}

void TestPage::testSetOcrText() {
    Page p("");
    p.setFlag(Page::EXPORTED);
    QSignalSpy changed(&p, SIGNAL(changed()));
    QSignalSpy recognized(&p, SIGNAL(recognized()));

    p.setOcrText("sample");

    QVERIFY(p.hasFlag(Page::RECOGNIZED));
    QVERIFY(!p.hasFlag(Page::EXPORTED));
    QCOMPARE(p.ocrText(), QString("sample"));
    QCOMPARE(changed.count(), 1);
    QCOMPARE(recognized.count(), 1);
}

void TestPage::testSetPageArea() {
    Page p("");
    QRect r(10, 20, 400, 500);
    QCOMPARE(p.pageArea(), QRect());

    QSignalSpy changed(&p, SIGNAL(changed()));

    p.setPageArea(r);

    QCOMPARE(p.pageArea(), r);
    QCOMPARE(changed.count(), 1);

    p.setPageArea(r);
    QCOMPARE(changed.count(), 1);

    p.setPageArea(r.adjusted(10, 10, 10, 10));
    QCOMPARE(changed.count(), 2);
}

void TestPage::testSetRecognitionSettings() {
    Page p("");
    RecognitionSettings s;
    s.setFax(true);
    p.setFlag(Page::EXPORTED);

    QSignalSpy changed(&p, SIGNAL(changed()));
    p.setRecognitionSettings(s);

    QVERIFY(!p.hasFlag(Page::EXPORTED));
    QVERIFY(s == p.recognitionSettings());
    QCOMPARE(changed.count(), 1);

    p.setRecognitionSettings(s);
    QCOMPARE(changed.count(), 1);

    s.setOneColumn(true);
    p.setRecognitionSettings(s);
    QCOMPARE(changed.count(), 2);


}

void TestPage::testSetBlocks() {
    Page p("");

    QVERIFY(p.blocks(Page::CHAR).empty());
    QVERIFY(p.blocks(Page::COLUMN).empty());
    QVERIFY(p.blocks(Page::LINE).empty());
    QVERIFY(p.blocks(Page::SECTION).empty());
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    QSignalSpy changed(&p, SIGNAL(changed()));

    Page::Rectangles rects;
    rects << QRect(0, 0, 10, 20);
    rects << QRect(1, 1, 2, 2);

    p.setBlocks(rects, Page::CHAR);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(p.blocks(Page::CHAR).count(), 2);
    QCOMPARE(p.blocks(Page::CHAR).at(0), QRect(0, 0, 10, 20));
    QCOMPARE(p.blocks(Page::CHAR).at(1), QRect(1, 1, 2, 2));
    QVERIFY(p.blocks(Page::COLUMN).empty());
    QVERIFY(p.blocks(Page::LINE).empty());
    QVERIFY(p.blocks(Page::SECTION).empty());
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    rects.removeLast();
    p.setBlocks(rects, Page::COLUMN);
    QCOMPARE(changed.count(), 2);
    QCOMPARE(p.blocks(Page::COLUMN).count(), 1);
    QCOMPARE(p.blocks(Page::COLUMN).at(0), QRect(0, 0, 10, 20));
    QVERIFY(p.blocks(Page::LINE).empty());
    QVERIFY(p.blocks(Page::SECTION).empty());
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    rects.removeLast();
    p.setBlocks(rects, Page::LINE);
    QCOMPARE(changed.count(), 3);
    QCOMPARE(p.blocks(Page::LINE).count(), 0);
    QVERIFY(p.blocks(Page::SECTION).empty());
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    rects << QRect(100, 100, 100, 100);
    p.setBlocks(rects, Page::SECTION);
    QCOMPARE(changed.count(), 4);
    QCOMPARE(p.blocks(Page::SECTION).count(), 1);
    QCOMPARE(p.blocks(Page::SECTION).at(0), QRect(100, 100, 100, 100));
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    rects << QRect(20, 20, 20, 20);
    p.setBlocks(rects, Page::PARAGRAPH);
    QCOMPARE(changed.count(), 5);
    QCOMPARE(p.blocks(Page::PARAGRAPH).count(), 2);
    QCOMPARE(p.blocks(Page::PARAGRAPH).at(0), QRect(100, 100, 100, 100));
    QCOMPARE(p.blocks(Page::PARAGRAPH).at(1), QRect(20, 20, 20, 20));
    QVERIFY(p.blocks(Page::PICTURE).empty());

    rects << QRect(30, 30, 30, 30);
    p.setBlocks(rects, Page::PICTURE);
    QCOMPARE(changed.count(), 6);
    QCOMPARE(p.blocks(Page::PICTURE).count(), 3);
    QCOMPARE(p.blocks(Page::PICTURE).at(0), QRect(100, 100, 100, 100));
    QCOMPARE(p.blocks(Page::PICTURE).at(1), QRect(20, 20, 20, 20));
    QCOMPARE(p.blocks(Page::PICTURE).at(2), QRect(30, 30, 30, 30));
}

void TestPage::testSetSelected() {
    Page p("");
    QVERIFY(!p.isSelected());

    QSignalSpy changed(&p, SIGNAL(changed()));
    p.setSelected(true);

    QCOMPARE(changed.count(), 1);
    QVERIFY(p.isSelected());

    p.setSelected(true);
    QCOMPARE(changed.count(), 1);
    QVERIFY(p.isSelected());

    p.setSelected(false);
    QCOMPARE(changed.count(), 2);
    QVERIFY(!p.isSelected());

    p.setSelected(false);
    QCOMPARE(changed.count(), 2);
}

void TestPage::testSetTransform() {
    Page p("");
    QCOMPARE(p.transform(), QTransform());

    QTransform t;
    QSignalSpy changed(&p, SIGNAL(changed()));
    QSignalSpy transformed(&p, SIGNAL(transformed()));

    p.setTransform(t);
    QCOMPARE(changed.count(), 0);
    QCOMPARE(transformed.count(), 0);

    t.rotate(90);
    p.setTransform(t);
    QCOMPARE(p.transform(), t);
    QCOMPARE(p.angle(), 90);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(transformed.count(), 1);
}

void TestPage::testSetViewScroll() {
    Page p("");
    QCOMPARE(p.viewScroll(), QPoint());
    QSignalSpy changed(&p, SIGNAL(changed()));

    p.setViewScroll(QPoint(10, 20));
    // no signals
    QCOMPARE(changed.count(), 0);
    QCOMPARE(p.viewScroll(), QPoint(10, 20));
}

void TestPage::testUnsetFlag() {
    Page p("");
    p.setFlags(Page::EXPORTED | Page::RECOGNIZED);

    QSignalSpy changed(&p, SIGNAL(changed()));
    p.unsetFlag(Page::EXPORTED);
    QVERIFY(!p.hasFlag(Page::EXPORTED));
    QCOMPARE(changed.count(), 1);
    p.unsetFlag(Page::RECOGNIZED);
    QCOMPARE(changed.count(), 2);
    QVERIFY(!p.hasFlag(Page::RECOGNIZED));

    p.unsetFlag(Page::EXPORT_FAILED);
    QCOMPARE(changed.count(), 2);
}

void TestPage::testReadWrite() {
    Page p(SAMPLE_IMG);
    int n = 10;
    QPoint pt(10, 20);
    QString t("sample text");
    QRect r(20, 30, 40, 50);
    QTransform tr;
    QString fname("page.tmp");
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
    p.setBlocks(rects, Page::CHAR);
    p.setBlocks(rects, Page::PICTURE);

    {
        QFile file(fname);
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << p;
    }

    {
        Page p2("");
        QFile file(fname);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in >> p2;

        QCOMPARE(p.angle(), p2.angle());
        QCOMPARE(p.flags(), p2.flags());
        QCOMPARE(p.imagePath(), p2.imagePath());
        QCOMPARE(p.imageSize(), p2.imageSize());
        QCOMPARE(p.isNull(), p2.isNull());
        QCOMPARE(p.isRecognized(), p2.isRecognized());
        QCOMPARE(p.isExported(), p2.isExported());
        QCOMPARE(p.isSelected(), p2.isSelected());
        QCOMPARE(p.name(), p2.name());
        QCOMPARE(p.number(), p2.number());
        QCOMPARE(p.ocrText(), p2.ocrText());
        QCOMPARE(p.pageArea(), p2.pageArea());
        QCOMPARE(p.recognitionSettings(), p2.recognitionSettings());
        QCOMPARE(p.transform(), p2.transform());
        QCOMPARE(p.viewScroll(), p2.viewScroll());
        QCOMPARE(p.blocks(Page::CHAR).count(), p2.blocks(Page::CHAR).count());
        QCOMPARE(p.blocks(Page::PICTURE).count(), p2.blocks(Page::PICTURE).count());
    }

    QFile f(fname);
    f.remove();
}

void TestPage::testExportTo() {
    Page p("");
    QSignalSpy changed(&p, SIGNAL(changed()));
    QSignalSpy exported(&p, SIGNAL(exported()));
    QString fname("test_page.txt");

    {
        QFile f(fname);
        if(f.exists())
            f.remove();
    }

    QVERIFY_THROW(p.exportTo(fname), Page::Exception);
    QCOMPARE(changed.count(), 0);
    QCOMPARE(exported.count(), 0);
    QVERIFY(!p.hasFlag(Page::EXPORTED));
    QVERIFY(!p.hasFlag(Page::EXPORT_FAILED));

    // save 1st time
    p.setOcrText("sample text"); // +1 changed() signal
    p.exportTo(fname);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(exported.count(), 1);
    CHECK_SMALL_FILE(fname, "sample text");
    QVERIFY(p.hasFlag(Page::EXPORTED));
    QVERIFY(!p.hasFlag(Page::EXPORT_FAILED));

    QFile f(fname);
    f.setPermissions(0);

    QVERIFY_THROW(p.exportTo(fname), Page::Exception);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(exported.count(), 1);
    QVERIFY(!p.hasFlag(Page::EXPORTED));
    QVERIFY(p.hasFlag(Page::EXPORT_FAILED));

    {
        QFile f(fname);
        if(f.exists())
            f.remove();
    }
}

QTEST_MAIN(TestPage)
