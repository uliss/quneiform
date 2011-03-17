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
#include <QScrollBar>
#include "testimageview.h"
#include "gui/page.h"
#include "gui/selection.h"
#define private public
#include "gui/imageview.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

TestImageView::TestImageView(QObject *parent) :
    QObject(parent)
{
}

void TestImageView::testConstruct() {
    ImageView iv;
    QVERIFY(iv.layout_ == NULL);
    QVERIFY(iv.page_selection_ == NULL);
    QVERIFY(iv.page_ == NULL);
    QVERIFY(iv.page_shadow_ == NULL);
    QVERIFY(iv.scene());
    QVERIFY(iv.scene()->items().isEmpty());
    QVERIFY(!iv.isInteractive());
    QVERIFY(!iv.isEnabled());
}

void TestImageView::testClearScene() {
    ImageView iv;
    QVERIFY(iv.scene()->items().isEmpty());

    Page p(CF_IMAGE_DIR "/english.png");
    iv.showPage(&p);

    iv.clearScene();
    QVERIFY(iv.scene()->items().isEmpty());
    QVERIFY(iv.layout_ == NULL);
    QVERIFY(iv.page_selection_ == NULL);
}

void TestImageView::testFitPage() {
    ImageView iv;
    Page p(CF_IMAGE_DIR "/english.png");
    QCOMPARE(p.imageSize(), QSize(281, 81));

    // w - smaller, h - smaller (scale)
    iv.resize(200, 50);

    iv.showPage(&p);
    iv.fitPage();

    QVERIFY(iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - bigger (scale)
    iv.resize(200, 100);
    iv.fitPage();
    QVERIFY(iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - bigger (no scale)
    iv.resize(300, 100);
    iv.fitPage();
    QVERIFY(!iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - smaller (scale)
    iv.resize(300, 50);
    iv.fitPage();
    QVERIFY(iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    // rotation
    QTransform rot90;
    rot90.rotate(90);
    p.rotate(90);

    // w - smaller, h - smaller (scale + rotation)
    iv.resize(200, 50);
    iv.fitPage();

    QVERIFY(iv.transform().isRotating());
    QVERIFY(iv.transform() != rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - bigger (scale + rotation)
    iv.resize(200, 100);
    iv.fitPage();
    QVERIFY(iv.transform().isRotating());
    QVERIFY(iv.transform() != rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - bigger (no scale + rotation)
    iv.resize(300, 100);
    iv.fitPage();
    QVERIFY(iv.transform().isRotating());
    QCOMPARE(iv.transform(), rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - smaller (scale + rotation)
    iv.resize(300, 50);
    iv.fitPage();
    QVERIFY(iv.transform().isRotating());
    QVERIFY(iv.transform() != rot90);
    QCOMPARE(p.transform(), iv.transform());

    p.resetTransform();
    QTransform rot180;
    rot180.rotate(180);
    p.rotate(180);

    // w - smaller, h - smaller (scale)
    iv.resize(200, 50);
    iv.fitPage();
    QVERIFY(iv.transform() != rot180);
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - bigger (scale)
    iv.resize(200, 100);
    iv.fitPage();
    QVERIFY(iv.transform() != rot180);
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - bigger (no scale)
    iv.resize(300, 100);
    iv.fitPage();
    QCOMPARE(iv.transform(), rot180);
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - smaller (scale)
    iv.resize(300, 50);
    iv.fitPage();
    QVERIFY(iv.transform() != rot180);
    QCOMPARE(p.transform(), iv.transform());
}

void TestImageView::testFitWidth() {
    ImageView iv;
    Page p(CF_IMAGE_DIR "/english.png");
    QCOMPARE(p.imageSize(), QSize(281, 81));

    // w - smaller, h - smaller (scale!)
    iv.resize(100, 70);
    iv.showPage(&p);

    iv.fitWidth();
    QVERIFY(iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - smaller (no scale)
    iv.resize(300, 50);
    iv.fitWidth();
    QVERIFY(!iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - bigger (no scale)
    iv.resize(300, 100);
    iv.fitWidth();
    QVERIFY(!iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - bigger (scale!)
    iv.resize(200, 100);
    iv.fitWidth();
    QVERIFY(iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    QTransform rot90;
    rot90.rotate(90);
    p.rotate(90);

    // w - bigger, h - smaller (no scale + rotation)
    iv.resize(50, 300);
    iv.fitWidth();
    QVERIFY(iv.transform().isRotating());
    QCOMPARE(iv.transform(), rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - bigger (no scale + rotation)
    iv.resize(100, 300);
    iv.fitWidth();
    QVERIFY(iv.transform().isRotating());
    QCOMPARE(iv.transform(), rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - bigger (scale + rotation)
    iv.resize(100, 200);
    iv.fitWidth();
    QVERIFY(iv.transform().isRotating());
    QVERIFY(iv.transform() != rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - smaller (scale + rotation)
    iv.resize(70, 100);
    iv.fitWidth();
    QVERIFY(iv.transform().isRotating());
    QVERIFY(iv.transform() != rot90);
    QCOMPARE(p.transform(), iv.transform());

    p.resetTransform();
    QTransform rot180;
    rot180.rotate(180);
    p.rotate(180);

    // w - bigger, h - smaller (no scale)
    iv.resize(300, 50);
    iv.fitWidth();
    QCOMPARE(iv.transform(), rot180);
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - bigger (no scale)
    iv.resize(300, 100);
    iv.fitWidth();
    QCOMPARE(iv.transform(), rot180);
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - bigger (scale)
    iv.resize(200, 100);
    iv.fitWidth();
    QVERIFY(iv.transform() != rot180);
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - smaller (scale)
    iv.resize(100, 70);
    iv.fitWidth();
    QVERIFY(iv.transform() != rot180);
    QCOMPARE(p.transform(), iv.transform());
}

void TestImageView::testOriginalSize() {
    ImageView iv;
    Page p(CF_IMAGE_DIR "/english.png");
    QCOMPARE(p.imageSize(), QSize(281, 81));

    iv.resize(300, 200);
    iv.showPage(&p);

    iv.zoom(2);
    QVERIFY(iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    iv.originalSize();
    QVERIFY(!iv.transform().isScaling());
    QCOMPARE(p.transform(), iv.transform());

    QTransform rot90;
    rot90.rotate(90);

    // rotate and no scale
    p.rotate(90);
    QVERIFY(iv.transform().isRotating());
    QCOMPARE(p.transform(), rot90);

    // original
    iv.originalSize();
    QVERIFY(iv.transform().isRotating());
    QCOMPARE(p.transform(), rot90);

    // rotate and scale
    p.scale(2);
    QVERIFY(iv.transform().isRotating());
    QVERIFY(iv.transform() != rot90);

    // original
    iv.originalSize();
    QVERIFY(iv.transform().isRotating());
    QCOMPARE(p.transform(), rot90);

    QTransform rot180;
    rot180.rotate(180);
    QVERIFY(!rot180.isRotating());

    p.resetTransform();

    // rotate and no scale
    p.rotate(180);
    QCOMPARE(p.transform(), rot180);

    // original
    iv.originalSize();
    QCOMPARE(p.transform(), rot180);

    // rotate and scale
    p.scale(2);
    QVERIFY(iv.transform() != rot180);

    // original
    iv.originalSize();
    QCOMPARE(p.transform(), rot180);
}

void TestImageView::testZoom() {
    ImageView iv;
    Page p(CF_IMAGE_DIR "/english.png");
    iv.showPage(&p);
    QCOMPARE(p.imageSize(), QSize(281, 81));
    QCOMPARE(iv.sceneRect(), QRectF(0, 0, 281, 81));

    iv.zoom(2);
    QTransform t1;
    t1.scale(0.5, 0.5);
    QTransform t2;
    t2.scale(2, 2);

    QCOMPARE(p.transform(), t2);
    QCOMPARE(iv.transform(), t2);
    iv.zoom(2);
    QCOMPARE(p.transform(), t2 * t2);
    iv.zoom(0.5);
    QCOMPARE(p.transform(), t2);
    iv.zoom(0.5);
    QVERIFY(p.transform().isIdentity());
    iv.zoom(0.5);
    QCOMPARE(p.transform(), t1);
    iv.zoom(0.5);
    QCOMPARE(p.transform(), t1 * t1);
    iv.originalSize();

    // rotation
    iv.rotate(90);
    iv.zoom(2);
    QCOMPARE(iv.transform(), t2);
    iv.zoom(2);
    QCOMPARE(p.transform(), t2 * t2);
    iv.zoom(0.5);
    QCOMPARE(p.transform(), t2);
    iv.zoom(0.5);
    QVERIFY(p.transform().isIdentity());
    iv.zoom(0.5);
    QCOMPARE(p.transform(), t1);
    iv.zoom(0.5);
    QCOMPARE(p.transform(), t1 * t1);
}

void TestImageView::testShowPage() {
    ImageView iv;
    iv.resize(150, 50);
    QVERIFY(iv.sceneRect().isNull());
    iv.showPage(NULL);
    QVERIFY(iv.sceneRect().isNull());

    Page p(CF_IMAGE_DIR "/english.png");
    p.setViewScroll(QPoint(11, 12));
    p.setPageArea(QRect(5, 6, 10, 20));

    iv.showPage(&p);
    QVERIFY(!iv.sceneRect().isNull());
    QVERIFY(iv.page_selection_ != NULL);
    QCOMPARE(iv.page_selection_->normalRect(), QRect(5, 6, 10, 20));

    QCOMPARE(iv.verticalScrollBar()->value(), 12);
    QCOMPARE(iv.horizontalScrollBar()->value(), 11);

    // view scope
    {
        ImageView iv2;
        iv2.showPage(&p);
    }

    {
        ImageView iv3;
        iv3.resize(150, 50);
        QVERIFY(!iv3.isEnabled());
        QVERIFY(!iv3.isInteractive());
        QSignalSpy page_deleted(&iv3, SIGNAL(pageDeleted()));
        Page * p3 = new Page(CF_IMAGE_DIR "/english.png");
        p3->setViewScroll(QPoint(9, 10));
        iv3.showPage(p3);
        QVERIFY(iv3.isEnabled());
        QVERIFY(iv3.isInteractive());
        iv3.horizontalScrollBar()->setValue(12);
        iv3.verticalScrollBar()->setValue(13);
        QVERIFY(p3->viewScroll() != QPoint(12, 13));
        delete p3;
        QCOMPARE(page_deleted.count(), 1);
        QVERIFY(!iv3.isEnabled());
        QVERIFY(!iv3.isInteractive());
    }
}

void TestImageView::testSelection() {
    ImageView iv;
    iv.resize(281, 81);
    iv.show();
    Page p(CF_IMAGE_DIR "/english.png");
    iv.showPage(&p);

//    iv.selectPageArea();
//    iv.setFocus();
//    QTest::mouseMove(&iv, QPoint(10, 15));
//    QCoreApplication::processEvents();
//    QTest::mousePress(&iv, Qt::LeftButton, 0, QPoint(10, 15), 500);
//    QTest::mousePress(&iv, Qt::LeftButton, 0, QPoint(10, 15), 500);
//    QCoreApplication::processEvents();
//    QTest::mouseMove(&iv, QPoint(60, 65));
//    QCoreApplication::processEvents();
//    QTest::mouseRelease(&iv, Qt::LeftButton, 0, QPoint(60, 65), 500);
}

QTEST_MAIN(TestImageView);
