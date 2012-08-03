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
#include <QApplication>
#include "testimageview.h"
#include "gui/page.h"
#include "gui/selection.h"
#define private public
#include "gui/widgets/imageview.h"
#include "gui/selectionlist.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

static void wait_events() {
    QCoreApplication::processEvents();
    QCoreApplication::flush();
#ifndef Q_WS_MAC
    QTest::qWait(100);
#endif
}

static void mouseMove(QWidget * w, const QPoint& pos) {
    QTest::mouseMove(w, pos);
    wait_events();
}

TestImageView::TestImageView(QObject *parent) :
        QObject(parent)
{
}

void TestImageView::testConstruct() {
    ImageView iv;
    QVERIFY(iv.area_ == NULL);
    QVERIFY(iv.selections_ == NULL);
    QVERIFY(iv.page_ == NULL);
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
    QVERIFY(iv.area_ == NULL);
    QVERIFY(iv.selections_ == NULL);
}

#define NO_SCALE(page, view) \
QVERIFY(!view.transform().isScaling());\
QCOMPARE(p.viewScale(), (float)1.0)

#define SCALED(page, view) \
QVERIFY(view.transform().isScaling());\
QCOMPARE(page.viewScale(), (float) view.transform().m11())

void TestImageView::testFitPage() {
    ImageView iv;
    Page p(CF_IMAGE_DIR "/english.png");
    QCOMPARE(p.imageSize(), QSize(281, 81));

    // w - smaller, h - smaller (scale)
    iv.resize(200, 50);

    iv.showPage(&p);
    iv.fitPage();
    SCALED(p, iv);

    // w - smaller, h - bigger (scale)
    iv.resize(200, 100);
    iv.fitPage();
    SCALED(p, iv);

    // w - bigger, h - bigger (no scale)
    iv.resize(300, 100);
    iv.fitPage();
    NO_SCALE(p, iv);

    // w - bigger, h - smaller (scale)
    iv.resize(300, 50);
    iv.fitPage();
    SCALED(p, iv);
}

void TestImageView::testFitWidth() {
    ImageView iv;
    Page p(CF_IMAGE_DIR "/english.png");
    QCOMPARE(p.imageSize(), QSize(281, 81));

    // w - smaller, h - smaller (scale!)
    iv.resize(100, 70);
    iv.showPage(&p);

    iv.fitWidth();
    SCALED(p, iv);

    // w - bigger, h - smaller (no scale)
    iv.resize(300, 50);
    iv.fitWidth();
    NO_SCALE(p, iv);

    // w - bigger, h - bigger (no scale)
    iv.resize(300, 100);
    iv.fitWidth();
    NO_SCALE(p, iv);

    // w - smaller, h - bigger (scale!)
    iv.resize(200, 100);
    iv.fitWidth();
    SCALED(p, iv);
}

void TestImageView::testOriginalSize() {
    ImageView iv;
    Page p(CF_IMAGE_DIR "/english.png");
    QCOMPARE(p.imageSize(), QSize(281, 81));

    iv.resize(300, 200);
    iv.showPage(&p);

    iv.zoom(2);
    SCALED(p, iv);

    iv.originalSize();
    NO_SCALE(p, iv);
}

void TestImageView::testZoom() {
    ImageView iv;
    Page p(CF_IMAGE_DIR "/english.png");
    iv.showPage(&p);
    QCOMPARE(p.imageSize(), QSize(281, 81));
    QCOMPARE(iv.sceneRect(), QRectF(0, 0, 281, 81));

    iv.zoom(2);
    QCOMPARE(p.viewScale(), float(2.0));

    iv.zoom(2);
    QCOMPARE(p.viewScale(), float(4.0));

    iv.zoom(0.5);
    QCOMPARE(p.viewScale(), float(2.0));
}

void TestImageView::testShowPage() {
    ImageView iv;
    iv.resize(150, 50);
    QVERIFY(iv.sceneRect().isNull());
    iv.showPage(NULL);
    QVERIFY(iv.sceneRect().isNull());

    Page p(CF_IMAGE_DIR "/english.png");
    p.setViewScroll(QPoint(11, 12));
    p.addReadArea(QRect(5, 6, 10, 20));

    iv.showPage(&p);
    QVERIFY(!iv.sceneRect().isNull());
    QVERIFY(iv.selections_ != NULL);
    QCOMPARE(iv.selections_->selectionBoundingRect(), QRect(5, 6, 10, 20));

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
    iv.resize(300, 100);
    iv.show();
    Page p(CF_IMAGE_DIR "/english.png");
    // 281x81
    p.addReadArea(QRect(10, 20, 50, 60));
    iv.showPage(&p);
    wait_events();

#define CHECK_CURSOR(c) QCOMPARE(iv.selections_->selectionAt(0)->cursor().shape(), c);

    mouseMove(&iv, QPoint(20, 30));
    CHECK_CURSOR(Qt::SizeFDiagCursor);
    mouseMove(&iv, QPoint(68, 30));
    CHECK_CURSOR(Qt::SizeBDiagCursor);
    mouseMove(&iv, QPoint(68, 88));
    CHECK_CURSOR(Qt::SizeFDiagCursor);
    mouseMove(&iv, QPoint(20, 88));
    CHECK_CURSOR(Qt::SizeBDiagCursor);
    mouseMove(&iv, QPoint(40, 30));
    CHECK_CURSOR(Qt::SizeVerCursor);
    mouseMove(&iv, QPoint(68, 50));
    CHECK_CURSOR(Qt::SizeHorCursor);
    mouseMove(&iv, QPoint(40, 88));
    CHECK_CURSOR(Qt::SizeVerCursor);
    mouseMove(&iv, QPoint(20, 50));
    CHECK_CURSOR(Qt::SizeHorCursor);
}

void TestImageView::testMinMaxZoom() {
    ImageView iv;
    iv.setMinScale(0.25);
    iv.setMaxScale(4);

    Page p(CF_IMAGE_DIR "/english.png");
    iv.showPage(&p);

    QSignalSpy scaled(&iv, SIGNAL(scaled()));
    QSignalSpy too_big(&iv, SIGNAL(scaleIsTooBig()));
    QSignalSpy too_small(&iv, SIGNAL(scaleIsTooSmall()));

    iv.zoom(2);
    QCOMPARE(scaled.count(), 1);
    QVERIFY(too_big.isEmpty());
    QVERIFY(too_small.isEmpty());
    iv.zoom(2.1);
    QCOMPARE(scaled.count(), 2);
    QVERIFY(too_big.isEmpty());
    QVERIFY(too_small.isEmpty());
    iv.zoom(1.1);
    QCOMPARE(scaled.count(), 2);
    QCOMPARE(too_big.count(), 1);
    QVERIFY(too_small.isEmpty());

    iv.originalSize();
    scaled.clear();
    too_big.clear();

    iv.zoom(0.5);
    QCOMPARE(scaled.count(), 1);
    QVERIFY(too_big.isEmpty());
    QVERIFY(too_small.isEmpty());
    iv.zoom(0.4);
    QCOMPARE(scaled.count(), 2);
    QVERIFY(too_big.isEmpty());
    QVERIFY(too_small.isEmpty());
    iv.zoom(0.9);
    QCOMPARE(scaled.count(), 2);
    QVERIFY(too_big.isEmpty());
    QCOMPARE(too_small.count(), 1);
}

QTEST_MAIN(TestImageView);
