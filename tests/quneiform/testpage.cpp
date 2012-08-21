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
#include <QFuture>
#include <QtConcurrentRun>
#include <QSignalMapper>

#include <stdio.h>

#include "testpage.h"
#include "ced/cedpage.h"
#define private public
#include "gui/page.h"
#include "gui/pagerecognizer.h"

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

void noMessageOutput(QtMsgType, const char *) {}

void TestPage::initTestCase()
{
    qInstallMsgHandler(noMessageOutput);
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

void TestPage::handlePageChange()
{
}

void TestPage::handlePageRotate()
{
}

void TestPage::handlePageThumbChange()
{
    Page * p = qobject_cast<Page*>(sender());
    if(p)
        p->thumb().size();
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

void TestPage::testSetAngle() {
    Page p("");
    QSignalSpy changed(&p, SIGNAL(changed()));
    QSignalSpy rotated(&p, SIGNAL(rotated(int)));

    QCOMPARE(p.angle(), 0);
    p.setAngle(100);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(rotated.count(), 1);
    QCOMPARE(rotated.at(0).at(0).toInt(), 100);
    QCOMPARE(p.angle(), 100);

    p.setAngle(359);
    QCOMPARE(changed.count(), 2);
    QCOMPARE(rotated.count(), 2);
    QCOMPARE(rotated.at(1).at(0).toInt(), 359);
    QCOMPARE(p.angle(), 359);

    p.setAngle(360);
    QCOMPARE(changed.count(), 3);
    QCOMPARE(rotated.count(), 3);
    QCOMPARE(rotated.at(2).at(0).toInt(), 0);
    QCOMPARE(p.angle(), 0);

    p.setAngle(361);
    QCOMPARE(changed.count(), 4);
    QCOMPARE(rotated.count(), 4);
    QCOMPARE(rotated.at(3).at(0).toInt(), 1);
    QCOMPARE(p.angle(), 1);

    p.setAngle(-90);
    QCOMPARE(changed.count(), 5);
    QCOMPARE(rotated.count(), 5);
    QCOMPARE(rotated.at(4).at(0).toInt(), 270);
    QCOMPARE(p.angle(), 270);

    p.setAngle(-360);
    QCOMPARE(p.angle(), 0);
}

void TestPage::testConstruct() {
    Page p("");

    QVERIFY(!p.language().isValid());
    QCOMPARE(p.name(), QString(""));
    QCOMPARE(p.angle(), 0);
    QVERIFY(p.flags() == Page::NONE);
    QCOMPARE(p.imagePath(), QString(""));
    QCOMPARE(p.imageSize(), QSize());
    QVERIFY(p.isNull());
    QVERIFY(!p.isRecognized());
    QVERIFY(!p.isExported());
    QCOMPARE(p.viewScale(), float(1.0));
    QVERIFY(p.viewScroll() != QPoint());
    QVERIFY(p.isFirstViewScroll());
    QVERIFY(p.blocks(Page::CHAR).empty());
    QVERIFY(p.blocks(Page::COLUMN).empty());
    QVERIFY(p.blocks(Page::LINE).empty());
    QVERIFY(p.blocks(Page::SECTION).empty());
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    p.setViewScroll(QPoint());
    QVERIFY(!p.isFirstViewScroll());

    Page p2("none");
    QCOMPARE(p2.name(), QString("none"));
    QCOMPARE(p2.imagePath(), QString("none"));
    QVERIFY(p2.isNull());

    Page p3(SAMPLE_IMG);
    QCOMPARE(p3.name(), QString("english.png"));
    QCOMPARE(p3.imagePath(), QString(SAMPLE_IMG));
    QVERIFY(!p3.isNull());
    QCOMPARE(p3.imageSize(), QSize(281, 81));
    QVERIFY(!p3.hasReadAreas());
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
    QVERIFY(p.flags() == (Page::RECOGNIZED | Page::EXPORTED));

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
    QSignalSpy view_scaled(&p, SIGNAL(viewScaled()));

    p.scaleView(-1);

    QCOMPARE(changed.count(), 0);
    QCOMPARE(view_scaled.count(), 1);
}

void TestPage::testSetCEDPage() {
    cf::CEDPagePtr cedp(new cf::CEDPage);
    Page p("");
    QSignalSpy changed(&p, SIGNAL(changed()));
    QSignalSpy recognized(&p, SIGNAL(recognized()));

    QVERIFY(p.cedPage() == NULL);

    p.setCEDPage(cedp);

    QCOMPARE(p.cedPage(), cedp);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(recognized.count(), 1);
    QVERIFY(p.isRecognized());
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

    Page::Rectangles rects;
    rects << QRect(0, 0, 10, 20);
    rects << QRect(1, 1, 2, 2);

    p.setBlocks(rects, Page::CHAR);
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
    QCOMPARE(p.blocks(Page::COLUMN).count(), 1);
    QCOMPARE(p.blocks(Page::COLUMN).at(0), QRect(0, 0, 10, 20));
    QVERIFY(p.blocks(Page::LINE).empty());
    QVERIFY(p.blocks(Page::SECTION).empty());
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    rects.removeLast();
    p.setBlocks(rects, Page::LINE);
    QCOMPARE(p.blocks(Page::LINE).count(), 0);
    QVERIFY(p.blocks(Page::SECTION).empty());
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    rects << QRect(100, 100, 100, 100);
    p.setBlocks(rects, Page::SECTION);
    QCOMPARE(p.blocks(Page::SECTION).count(), 1);
    QCOMPARE(p.blocks(Page::SECTION).at(0), QRect(100, 100, 100, 100));
    QVERIFY(p.blocks(Page::PARAGRAPH).empty());
    QVERIFY(p.blocks(Page::PICTURE).empty());

    rects << QRect(20, 20, 20, 20);
    p.setBlocks(rects, Page::PARAGRAPH);
    QCOMPARE(p.blocks(Page::PARAGRAPH).count(), 2);
    QCOMPARE(p.blocks(Page::PARAGRAPH).at(0), QRect(100, 100, 100, 100));
    QCOMPARE(p.blocks(Page::PARAGRAPH).at(1), QRect(20, 20, 20, 20));
    QVERIFY(p.blocks(Page::PICTURE).empty());

    rects << QRect(30, 30, 30, 30);
    p.setBlocks(rects, Page::PICTURE);
    QCOMPARE(p.blocks(Page::PICTURE).count(), 3);
    QCOMPARE(p.blocks(Page::PICTURE).at(0), QRect(100, 100, 100, 100));
    QCOMPARE(p.blocks(Page::PICTURE).at(1), QRect(20, 20, 20, 20));
    QCOMPARE(p.blocks(Page::PICTURE).at(2), QRect(30, 30, 30, 30));
}

void TestPage::testSetViewScroll() {
    Page p("");
    QVERIFY(p.viewScroll() != QPoint());
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
    QPoint pt(10, 20);
    QRect r(20, 30, 40, 50);
    QString fname("page.tmp");
    p.setViewScroll(pt);
    p.addReadArea(r);
    p.setAngle(90);
    p.setLanguage(Language(5));

    RecognitionSettings s;
    s.setFax(true);
    p.setRecognitionSettings(s);

    Page::Rectangles rects;
    rects << r << r << r;
    p.setBlocks(rects, Page::CHAR);
    p.setBlocks(rects, Page::PICTURE);

    p.setCEDPage(cf::CEDPagePtr(new cf::CEDPage));
    p.cedPage()->setImageName("CED Image");

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
        QCOMPARE(p.name(), p2.name());
        QCOMPARE(p.readAreas(), p2.readAreas());
        QCOMPARE(p.recognitionSettings(), p2.recognitionSettings());
        QCOMPARE(p.viewScale(), p2.viewScale());
        QCOMPARE(p.viewScroll(), p2.viewScroll());
        QCOMPARE(p.blocks(Page::CHAR).count(), p2.blocks(Page::CHAR).count());
        QCOMPARE(p.blocks(Page::PICTURE).count(), p2.blocks(Page::PICTURE).count());
        QCOMPARE(p.language(), p2.language());
        QCOMPARE(p.cedPage()->imageName(), std::string("CED Image"));
    }

    QFile f(fname);
    f.remove();
}

static void threadRotate(Page * p)
{
    p->rotate(qrand() % 360);
}

static void threadSetAngle(Page * p)
{
    p->setAngle(qrand() % 360);
}

static void threadSetThumb(Page * p)
{
    p->setThumb(QImage(CF_IMAGE_DIR "/german.png"));
}

void TestPage::testMultiThreadFuzzing()
{

    QSignalMapper ch_mapper;
    QSignalMapper rt_mapper;
    QSignalMapper th_mapper;


    QList<Page*> pages;
    for(int i = 0; i < 1000; i++) {
        pages.append(new Page(ImageURL(SAMPLE_IMG)));
        connect(pages.back(), SIGNAL(changed()), &ch_mapper, SLOT(map()));
        connect(pages.back(), SIGNAL(rotated(int)), &rt_mapper, SLOT(map()));
        connect(pages.back(), SIGNAL(thumbChanged()), &th_mapper, SLOT(map()));
        connect(pages.back(), SIGNAL(thumbChanged()), SLOT(handlePageThumbChange()));

        th_mapper.setMapping(pages.back(), i);
    }

    QSignalSpy thumb_spy(&th_mapper, SIGNAL(mapped(int)));

    QFuture<void> f_thumb = QtConcurrent::map(pages, threadSetThumb);
    QFuture<void> f_rotate = QtConcurrent::map(pages, threadRotate);
    QFuture<void> f_angle = QtConcurrent::map(pages, threadSetAngle);

    f_angle.waitForFinished();
    f_rotate.waitForFinished();
    f_thumb.waitForFinished();

    QApplication::processEvents();

    QCOMPARE(thumb_spy.count(), 1000);

    foreach(Page * p, pages) {
        delete p;
    }
}

void TestPage::testReadBoundingRect()
{
    Page page;
    page.setImageSize(QSize(280, 80));
    QCOMPARE(page.imageSize(), QSize(280, 80));

    QCOMPARE(page.readBoundingRect(), QRect(0, 0, 280, 80));
    page.addReadArea(QRect(10, 10, 40, 60));
    QCOMPARE(page.readBoundingRect(), QRect(10, 10, 40, 60));

    page.addReadArea(QRect(-10, -10, 40, 60));
    QCOMPARE(page.readBoundingRect(), QRect(0, 0, 50, 70));
}

void TestPage::testMapPointFromBackend()
{
    Page page;
    //   0 1 2 3 4 5
    // 0 X * * * * *
    // 1 *         *
    // 2 *         *
    // 3 *         *
    // 4 *         *
    // 5 *         *
    // 6 *         *
    // 7 *         *
    // 8 *         *
    // 9 * * * * * *
    QPoint ptX(0, 0);
    page.setImageSize(QSize(6, 10));
    QCOMPARE(page.mapFromBackend(ptX), QPoint(0, 0));

    page.setAngle(90);
    // page coord
    //   0 1 2 3 4 5 6 7 8 9
    // 0 X * * * * * * * * *
    // 1 *                 *
    // 2 *                 *
    // 3 *                 *
    // 4 *                 *
    // 5 * * * * * * * * * *

    // view coord
    //   0 1 2 3 4 5
    // 0 * * * * * *
    // 1 *         *
    // 2 *         *
    // 3 *         *
    // 4 *         *
    // 5 *         *
    // 6 *         *
    // 7 *         *
    // 8 *         *
    // 9 X * * * * *
    QCOMPARE(page.mapFromBackend(ptX), QPoint(0, 9));

    page.setAngle(180);
    // page coord
    //   0 1 2 3 4 5
    // 0 X * * * * *
    // 1 *         *
    // 2 *         *
    // 3 *         *
    // 4 *         *
    // 5 *         *
    // 6 *         *
    // 7 *         *
    // 8 *         *
    // 9 * * * * * *

    // view coord
    //   0 1 2 3 4 5
    // 0 * * * * * *
    // 1 *         *
    // 2 *         *
    // 3 *         *
    // 4 *         *
    // 5 *         *
    // 6 *         *
    // 7 *         *
    // 8 *         *
    // 9 * * * * * X
    QCOMPARE(page.mapFromBackend(ptX), QPoint(5, 9));

    page.setAngle(270);
    // page coord
    //   0 1 2 3 4 5 6 7 8 9
    // 0 X * * * * * * * * *
    // 1 *                 *
    // 2 *                 *
    // 3 *                 *
    // 4 *                 *
    // 5 * * * * * * * * * *

    // view coord
    //   0 1 2 3 4 5
    // 0 * * * * * X
    // 1 *         *
    // 2 *         *
    // 3 *         *
    // 4 *         *
    // 5 *         *
    // 6 *         *
    // 7 *         *
    // 8 *         *
    // 9 * * * * * *
    QCOMPARE(page.mapFromBackend(ptX), QPoint(5, 0));
}

void TestPage::testMapPointFromBackendWithReadArea()
{
    Page page;
    page.setImageSize(QSize(6, 10));
    page.addReadArea(QRect(1, 2, 5, 2));
    page.setAngle(0);
    // backend coord
    //   0 1 2 3 4
    // 0 X * * * *
    // 1 * . . . *
    // 2 * * * * *

    // view coord
    //   0 1 2 3 4 5
    // 0 . . . . . .
    // 1 . . . . . .
    // 2 . X * * * *
    // 3 . * . . . *
    // 4 . * * * * *
    // 5 . . . . . .
    // 6 . . . . . .
    // 7 . . . . . .
    // 8 . . . . . .
    // 9 . . . . . .
    QCOMPARE(page.mapFromBackend(QPoint(0, 0)), QPoint(1, 2)); // top left
    QCOMPARE(page.mapFromBackend(QPoint(0, 2)), QPoint(1, 4)); // top bottom
    QCOMPARE(page.mapFromBackend(QPoint(4, 2)), QPoint(5, 4)); // right bottom
    QCOMPARE(page.mapFromBackend(QPoint(4, 0)), QPoint(5, 2)); // top right
    QCOMPARE(page.mapFromBackend(QRect(0, 0, 5, 2)), QRect(1, 2, 5, 2));

    page.setAngle(90);
    //   0 1 2
    // 0 X * *
    // 1 * . *
    // 2 * . *
    // 3 * . *
    // 4 * * *

    //   0 1 2 3 4 5
    // 0 . . . . . .
    // 1 . . . . . .
    // 2 . * * * * *
    // 3 . * . . . *
    // 4 . X * * * *
    // 5 . . . . . .
    // 6 . . . . . .
    // 7 . . . . . .
    // 8 . . . . . .
    // 9 . . . . . .
}

void TestPage::testMapFromBackend()
{
    Page page;
    QRect rect(10, 20, 60, 70);
    page.setImageSize(QSize(100, 200));

    QCOMPARE(page.mapFromBackend(rect).size(), QSize(60, 70));
    QCOMPARE(page.mapFromBackend(QRect(0, 0, 100, 200)), QRect(0, 0, 100, 200));

    page.setAngle(90);
    QCOMPARE(page.mapFromBackend(rect).size(), QSize(70, 60));
    QCOMPARE(page.mapFromBackend(QRect(0, 0, 200, 100)), QRect(0, 0, 100, 200));

    page.setAngle(180);
    QCOMPARE(page.mapFromBackend(rect).size(), QSize(60, 70));
    QCOMPARE(page.mapFromBackend(QRect(0, 0, 100, 200)), QRect(0, 0, 100, 200));

    page.setAngle(270);
    QCOMPARE(page.mapFromBackend(rect).size(), QSize(70, 60));
    QCOMPARE(page.mapFromBackend(QRect(0, 0, 200, 100)), QRect(0, 0, 100, 200));


    // with read area
    page.addReadArea(QRect(1, 2, 30, 40));

    page.setAngle(0);
    QCOMPARE(page.mapFromBackend(QRect(0, 0, 30, 40)), QRect(1, 2, 30, 40));

    page.setAngle(90);
    QCOMPARE(page.mapFromBackend(QRect(0, 0, 40, 30)), QRect(1, 2, 30, 40));

    page.setAngle(180);
    QCOMPARE(page.mapFromBackend(QRect(0, 0, 30, 40)), QRect(1, 2, 30, 40));

    page.setAngle(270);
    QCOMPARE(page.mapFromBackend(QRect(0, 0, 40, 30)), QRect(1, 2, 30, 40));
}

void TestPage::testMapToBackend()
{
    Page page;
    QPoint ptX(0, 0);
    page.setImageSize(QSize(6, 10));

    // angle 0
    QCOMPARE(page.mapToBackend(QPoint(0, 0)), QPoint(0, 0));
    QCOMPARE(page.mapToBackend(QPoint(1, 2)), QPoint(1, 2));
    QCOMPARE(page.mapToBackend(QRect(0, 0, 6, 10)), QRect(0, 0, 6, 10));
    QCOMPARE(page.mapToBackend(QRect(1, 2, 3, 4)), QRect(1, 2, 3, 4));

    page.setAngle(90);
    QCOMPARE(page.mapToBackend(QPoint(0, 0)), QPoint(9, 0));
    QCOMPARE(page.mapToBackend(QPoint(0, 9)), QPoint(0, 0));
    QCOMPARE(page.mapToBackend(QRect(0, 0, 6, 10)), QRect(0, 0, 10, 6));
    QCOMPARE(page.mapToBackend(QRect(1, 2, 3, 4)), QRect(4, 1, 4, 3));

    page.setAngle(180);
    QCOMPARE(page.mapToBackend(QPoint(0, 0)), QPoint(5, 9));
    QCOMPARE(page.mapToBackend(QPoint(5, 9)), QPoint(0, 0));
    QCOMPARE(page.mapToBackend(QRect(0, 0, 6, 10)), QRect(0, 0, 6, 10));
    QCOMPARE(page.mapToBackend(QRect(1, 2, 3, 4)), QRect(2, 4, 3, 4));

    page.setAngle(270);
    QCOMPARE(page.mapToBackend(QPoint(0, 0)), QPoint(0, 5));
    QCOMPARE(page.mapToBackend(QPoint(5, 0)), QPoint(0, 0));
    QCOMPARE(page.mapToBackend(QRect(0, 0, 6, 10)), QRect(0, 0, 10, 6));
    QCOMPARE(page.mapToBackend(QRect(1, 2, 3, 4)), QRect(2, 2, 4, 3));

    page.setAngle(0);
    // with page area
    page.addReadArea(QRect(1, 2, 5, 8));

    QCOMPARE(page.mapToBackend(QPoint(0, 0)), QPoint(-1, -2));
    QCOMPARE(page.mapToBackend(QPoint(1, 2)), QPoint(0, 0));
    QCOMPARE(page.mapToBackend(QRect(1, 2, 5, 8)), QRect(0, 0, 5, 8));

    page.setAngle(90);
    QCOMPARE(page.mapToBackend(QPoint(0, 0)), QPoint(9, -1));
    QCOMPARE(page.mapToBackend(QPoint(1, 9)), QPoint(0, 0));
    QCOMPARE(page.mapToBackend(QRect(1, 2, 5, 8)), QRect(0, 0, 8, 5));

    page.setAngle(180);
    QCOMPARE(page.mapToBackend(QPoint(0, 0)), QPoint(5, 9));
    QCOMPARE(page.mapToBackend(QRect(1, 2, 5, 8)), QRect(0, 0, 5, 8));

    page.setAngle(270);
    QCOMPARE(page.mapToBackend(ptX), QPoint(-2, 5));
    QCOMPARE(page.mapToBackend(QRect(1, 2, 5, 8)), QRect(0, 0, 8, 5));
}

void TestPage::testRects()
{
    Page page(SAMPLE_IMG);
    PageRecognizer r;
    r.setPage(&page);
    r.setWorkerType(PageRecognizer::LOCAL);
    QVERIFY(r.recognize());

    QCOMPARE(page.blocksCount(Page::COLUMN), 1);
    QCOMPARE(page.blocksCount(Page::SECTION), 1);
    QCOMPARE(page.blocksCount(Page::PICTURE), 0);
    QCOMPARE(page.blocksCount(Page::LINE), 2);
    QCOMPARE(page.blocksCount(Page::PARAGRAPH), 2);
    QCOMPARE(page.blocksCount(Page::CHAR), 7);

    QList<QRect> chars = page.blocks(Page::CHAR);
    QCOMPARE(chars[0], QRect(14, 18, 29, 44));  // E
    QCOMPARE(chars[1], QRect(52, 18, 34, 44));  // N
    QCOMPARE(chars[2], QRect(95, 17, 39, 46));  // G
    QCOMPARE(chars[3], QRect(145, 17, 28, 45)); // L
    QCOMPARE(chars[4], QRect(179, 18, 6, 44));  // I
    QCOMPARE(chars[5], QRect(195, 17, 31, 46)); // S
    QCOMPARE(chars[6], QRect(235, 18, 34, 44)); // H
}

QTEST_MAIN(TestPage)
