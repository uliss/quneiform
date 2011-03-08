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
#include "testselection.h"
#include "gui/selection.h"

TestSelection::TestSelection() : scene_(QRectF(0, 0, 100, 200)) {

}

void TestSelection::testConstruct() {
    Selection s1;
    Selection s2(QRectF(0, 1, 20, 30));

    QVERIFY(s1.rect().isNull());
    QCOMPARE(s2.rect(), QRectF(0, 1, 20, 30));
    QCOMPARE(s1.pos(), QPointF());
    QCOMPARE(s2.pos(), QPointF());

    QVERIFY(s1.flags() & QGraphicsItem::ItemIsFocusable);
    QVERIFY(s1.flags() & QGraphicsItem::ItemIsSelectable);
    QVERIFY(s1.flags() & QGraphicsItem::ItemIsMovable);
}

void TestSelection::testMoveBy() {
    Selection * s = new Selection;
    scene_.addItem(s);
    QVERIFY(s->pos().isNull());
    QCOMPARE(s->rect().topLeft(), QPointF());

    s->moveBy(QPointF(10, 20));
    QCOMPARE(s->rect().topLeft(), QPointF(10, 20));
    QVERIFY(s->pos().isNull());

    // left
    const QRectF rect(0, 0, 50, 50);
    s->setRect(rect);
    s->moveBy(QPointF(-1, 0));
    QCOMPARE(s->rect(), rect);
    QVERIFY(s->pos().isNull());
    s->moveBy(QPointF(0, -1));
    QCOMPARE(s->rect(), rect);
    QVERIFY(s->pos().isNull());
    s->moveBy(QPointF(-1, -1));
    QCOMPARE(s->rect(), rect);
    QVERIFY(s->pos().isNull());
    s->moveBy(QPointF(1, 1));
    QCOMPARE(s->rect(), QRectF(1, 1, 50, 50));
    QVERIFY(s->pos().isNull());
    s->setRect(rect);
    s->moveBy(QPointF(-1, 1));
    QCOMPARE(s->rect(), QRectF(0, 1, 50, 50));
    QVERIFY(s->pos().isNull());
    s->setRect(rect);
    s->moveBy(QPointF(1, -1));
    QCOMPARE(s->rect(), QRectF(1, 0, 50, 50));
    QVERIFY(s->pos().isNull());
    s->moveBy(QPointF(-1, 1));
    QCOMPARE(s->rect(), QRectF(0, 1, 50, 50));
    QVERIFY(s->pos().isNull());
    s->setRect(rect.translated(10, 10));
    s->moveBy(QPointF(-1, 1));
    QCOMPARE(s->rect(), QRectF(9, 11, 50, 50));

    //
    s->setRect(rect);
    // too large
    s->moveBy(QPointF(1000, 2000));
    QCOMPARE(s->rect(), QRectF(
            scene_.width() - rect.width(),
            scene_.height() - rect.height(),
            rect.width(),
            rect.height()));

    s->setRect(rect);
    s->moveBy(QPointF(0, 1000));
    QCOMPARE(s->rect(), QRectF(
            0,
            scene_.height() - rect.height(),
            rect.width(),
            rect.height()
            ));

    s->setRect(rect);
    s->moveBy(QPointF(1000, 0));
    QCOMPARE(s->rect(), QRectF(
            scene_.width() - rect.width(),
            0,
            rect.width(),
            rect.height()));

    scene_.clear();
}

void TestSelection::testNormalRect() {
    Selection * s = new Selection;
    scene_.addItem(s);

    QCOMPARE(s->rect(), QRectF(s->normalRect()));
    s->setPos(1, 2);
    s->setRect(5, 6, 10, 30);
    QCOMPARE(s->normalRect(), QRect(6, 8, 10, 30));

    scene_.clear();
}

QTEST_MAIN(TestSelection);
