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
    iv.resize(50, 200);
    iv.fitPage();

    QVERIFY(iv.transform().isRotating());
    QVERIFY(iv.transform() != rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - bigger (scale + rotation)
    iv.resize(100, 200);
    iv.fitPage();
    QVERIFY(iv.transform().isRotating());
    QVERIFY(iv.transform() != rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - bigger (no scale + rotation)
    iv.resize(100, 300);
    iv.fitPage();
    QVERIFY(iv.transform().isRotating());
    QCOMPARE(iv.transform(), rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - bigger, h - smaller (scale + rotation)
    iv.resize(50, 300);
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
    iv.resize(50, 200);
    iv.fitWidth();
    QVERIFY(iv.transform().isRotating());
    QVERIFY(iv.transform() != rot90);
    QCOMPARE(p.transform(), iv.transform());

    // w - smaller, h - smaller (scale + rotation)
    iv.resize(50, 100);
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

void TestImageView::testHideFormatLayout() {
    ImageView iv;
    iv.hideFormatLayout();
    Page p("none");
    p.appendBlock(QRect(0, 0, 20, 30), Page::SECTION);
    iv.showPage(&p);
    iv.hideFormatLayout();
}

void TestImageView::testShowPage() {
    ImageView iv;
    iv.resize(100, 50);
    iv.show();
    Page p1("none");
    p1.scale(0.5);
    iv.showPage(&p1);
    QCOMPARE(iv.sceneRect(), QRectF());
    QCOMPARE(iv.transform(), QTransform(0.5, 0, 0, 0, 0.5, 0, 0, 0, 1));
    Page p2(CF_IMAGE_DIR "/english.png");
    p2.setPageArea(QRect(0, 0, 20, 30));
    iv.showPage(&p2);

    iv.horizontalScrollBar()->setValue(40);
    iv.verticalScrollBar()->setValue(50);
    QCOMPARE(iv.sceneRect(), QRectF(QPointF(), p2.imageSize()));
    QCOMPARE(iv.transform(), QTransform());
    QCOMPARE(iv.page_selection_->rect(), QRectF(0, 0, 20, 30));
    iv.showPage(&p1);
    QCOMPARE(p2.viewScroll(), QPoint(40, 50));

    QCOMPARE(iv.sceneRect(), QRectF());
    QCOMPARE(iv.transform(), QTransform(0.5, 0, 0, 0, 0.5, 0, 0, 0, 1));
    QCOMPARE(iv.horizontalScrollBar()->value(), 0);
    QCOMPARE(iv.verticalScrollBar()->value(), 0);
}

QTEST_MAIN(TestImageView);
