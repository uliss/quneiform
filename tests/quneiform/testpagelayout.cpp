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
#include <QSettings>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "testpagelayout.h"
#include "gui/pagelayout.h"
#include "gui/page.h"

void setShowBBoxes(const QString& name, bool value) {
    QSettings settings;
    QString key = QString("debug/format/show") + name + "BBox";
    settings.setValue(key, value);
}

int numRect(const QGraphicsScene& s) {
    int num = 0;
    foreach(QGraphicsItem * item, s.items()) {
        if(dynamic_cast<QGraphicsRectItem*>(item))
            num++;
    }

    return num;
}

int numRect(QGraphicsItemGroup * group) {
    return group->childItems().count();
}

bool noRect(const QGraphicsScene& s) {
    return numRect(s) == 0;
}

bool isEmpty(QGraphicsItemGroup * group) {
    return group->childItems().isEmpty();
}

bool hasRect(const QGraphicsScene& s, const QRect& r) {
    foreach(QGraphicsItem * item, s.items()) {
        QGraphicsRectItem * rect = dynamic_cast<QGraphicsRectItem*>(item);
        if(rect && rect->rect() == r)
            return true;
    }
    return false;
}

bool checkRect(QGraphicsItemGroup * group, int i, const QRect& r) {
    return group->childItems().at(i)->boundingRect() == QRectF(r);
}

bool checkColor(QGraphicsItemGroup * group, const QColor& c) {
    if(isEmpty(group))
        return false;

    QGraphicsRectItem * rect = dynamic_cast<QGraphicsRectItem*>(group->childItems().at(0));

    if(!rect)
        return false;

    return rect->pen().color() == c;
}

TestPageLayout::TestPageLayout(QObject * parent) :
    QObject(parent)
{
}

void TestPageLayout::testConstruct() {
    PageLayout l;

    QVERIFY(l.charBlocks()->childItems().isEmpty());
    QVERIFY(l.columnBlocks()->childItems().isEmpty());
    QVERIFY(l.lineBlocks()->childItems().isEmpty());
    QVERIFY(l.paragraphBlocks()->childItems().isEmpty());
    QVERIFY(l.pictureBlocks()->childItems().isEmpty());
    QVERIFY(l.sectionBlocks()->childItems().isEmpty());
}

void TestPageLayout::testPopulateChars() {
    QGraphicsScene s;
    PageLayout l;
    Page p("");
    s.addItem(&l);

    // Characters
    setShowBBoxes("Characters", true);
    l.populate(p);
    QVERIFY(isEmpty(l.charBlocks()));
    QVERIFY(noRect(s));

    QRect rect(10, 10, 20, 30);
    p.appendBlock(rect, Page::CHAR);
    l.populate(p);
    QCOMPARE(numRect(l.charBlocks()), 1);
    QVERIFY(checkRect(l.charBlocks(), 0, rect));
    QVERIFY(hasRect(s, rect));
    QVERIFY(checkColor(l.charBlocks(), Qt::cyan));

    l.clear();
    QVERIFY(noRect(s));

    setShowBBoxes("Characters", false);
    l.populate(p);
    QVERIFY(isEmpty(l.charBlocks()));
    QVERIFY(noRect(s));
}

void TestPageLayout::testPopulateColumns() {
    QGraphicsScene s;
    PageLayout l;
    s.addItem(&l);
    Page p("");

    // Columns
    setShowBBoxes("Columns", true);
    l.populate(p);
    QVERIFY(isEmpty(l.columnBlocks()));
    QVERIFY(noRect(s));

    QRect rect(1, 2, 3, 4);
    p.appendBlock(rect, Page::COLUMN);
    p.appendBlock(rect, Page::COLUMN);
    l.populate(p);
    QVERIFY(!isEmpty(l.columnBlocks()));
    QVERIFY(checkRect(l.columnBlocks(), 0, rect));
    QVERIFY(checkRect(l.columnBlocks(), 1, rect));
    QCOMPARE(numRect(s), 2);
    QVERIFY(hasRect(s, rect));

    l.clear();
    QVERIFY(noRect(s));

    setShowBBoxes("Columns", false);
    l.populate(p);
    QVERIFY(isEmpty(l.columnBlocks()));
    QVERIFY(noRect(s));
}

void TestPageLayout::testPopulateLines() {
    QGraphicsScene s;
    PageLayout l;
    s.addItem(&l);
    Page p("");

    // Lines
    setShowBBoxes("Lines", true);
    l.populate(p);
    QVERIFY(isEmpty(l.lineBlocks()));
    QVERIFY(noRect(s));

    QRect rect(1, 2, 3, 4);
    p.appendBlock(rect, Page::LINE);
    p.appendBlock(rect, Page::LINE);
    l.populate(p);
    QVERIFY(!isEmpty(l.lineBlocks()));
    QVERIFY(checkRect(l.lineBlocks(), 0, rect));
    QVERIFY(checkRect(l.lineBlocks(), 1, rect));

    l.clear();
    QVERIFY(isEmpty(l.lineBlocks()));
    QVERIFY(noRect(s));

    setShowBBoxes("Lines", false);
    l.populate(p);
    QVERIFY(isEmpty(l.lineBlocks()));
    QVERIFY(noRect(s));
}

void TestPageLayout::testPopulateParagraphs() {
    QGraphicsScene s;
    PageLayout l;
    s.addItem(&l);
    Page p("");

    // Paragraphs
    setShowBBoxes("Paragraphs", true);
    l.populate(p);
    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.paragraphBlocks()));

    QRect rect(1, 2, 3, 4);
    p.appendBlock(rect, Page::PARAGRAPH);
    p.appendBlock(rect, Page::PARAGRAPH);
    l.populate(p);
    QVERIFY(!isEmpty(l.paragraphBlocks()));
    QVERIFY(checkRect(l.paragraphBlocks(), 0, rect));
    QVERIFY(checkRect(l.paragraphBlocks(), 1, rect));

    l.clear();
    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.paragraphBlocks()));

    setShowBBoxes("Paragraphs", false);
    l.populate(p);
    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.paragraphBlocks()));
}

void TestPageLayout::testPopulatePictures() {
    QGraphicsScene s;
    PageLayout l;
    s.addItem(&l);
    Page p("");

    // Pictures
    setShowBBoxes("Pictures", true);
    l.populate(p);
    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.pictureBlocks()));

    QRect rect(1, 2, 3, 4);
    p.appendBlock(rect, Page::PICTURE);
    p.appendBlock(rect, Page::PICTURE);
    l.populate(p);
    QVERIFY(!isEmpty(l.pictureBlocks()));
    QVERIFY(checkRect(l.pictureBlocks(), 0, rect));
    QVERIFY(checkRect(l.pictureBlocks(), 1, rect));

    l.clear();
    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.pictureBlocks()));

    setShowBBoxes("Pictures", false);
    l.populate(p);
    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.pictureBlocks()));
}

void TestPageLayout::testPopulateSections() {
    QGraphicsScene s;
    PageLayout l;
    s.addItem(&l);
    Page p("");

    // Sections
    setShowBBoxes("Sections", true);
    l.populate(p);
    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.sectionBlocks()));

    QRect rect(1, 2, 3, 4);
    p.appendBlock(rect, Page::SECTION);
    p.appendBlock(rect, Page::SECTION);
    l.populate(p);
    QVERIFY(!isEmpty(l.sectionBlocks()));
    QVERIFY(checkRect(l.sectionBlocks(), 0, rect));
    QVERIFY(checkRect(l.sectionBlocks(), 1, rect));

    l.clear();
    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.sectionBlocks()));

    setShowBBoxes("Sections", false);
    l.populate(p);
    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.sectionBlocks()));
}

void TestPageLayout::testClear() {
    QGraphicsScene s;
    PageLayout l;
    s.addItem(&l);

    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.charBlocks()));
    QVERIFY(isEmpty(l.columnBlocks()));
    QVERIFY(isEmpty(l.lineBlocks()));
    QVERIFY(isEmpty(l.paragraphBlocks()));
    QVERIFY(isEmpty(l.pictureBlocks()));
    QVERIFY(isEmpty(l.sectionBlocks()));

    Page p("");
    QRect r(1, 2, 3, 4);
    p.appendBlock(r, Page::CHAR);
    p.appendBlock(r, Page::COLUMN);
    p.appendBlock(r, Page::LINE);
    p.appendBlock(r, Page::PARAGRAPH);
    p.appendBlock(r, Page::PICTURE);
    p.appendBlock(r, Page::SECTION);

    setShowBBoxes("Characters", true);
    setShowBBoxes("Columns", true);
    setShowBBoxes("Lines", true);
    setShowBBoxes("Paragraphs", true);
    setShowBBoxes("Pictures", true);
    setShowBBoxes("Sections", true);

    l.populate(p);

    QVERIFY(!isEmpty(l.charBlocks()));
    QVERIFY(!isEmpty(l.columnBlocks()));
    QVERIFY(!isEmpty(l.lineBlocks()));
    QVERIFY(!isEmpty(l.paragraphBlocks()));
    QVERIFY(!isEmpty(l.pictureBlocks()));
    QVERIFY(!isEmpty(l.sectionBlocks()));

    l.clear();

    QVERIFY(noRect(s));
    QVERIFY(isEmpty(l.charBlocks()));
    QVERIFY(isEmpty(l.columnBlocks()));
    QVERIFY(isEmpty(l.lineBlocks()));
    QVERIFY(isEmpty(l.paragraphBlocks()));
    QVERIFY(isEmpty(l.pictureBlocks()));
    QVERIFY(isEmpty(l.sectionBlocks()));

    QVERIFY(l.charBlocks());
    QVERIFY(l.columnBlocks());
    QVERIFY(l.lineBlocks());
    QVERIFY(l.paragraphBlocks());
    QVERIFY(l.pictureBlocks());
    QVERIFY(l.sectionBlocks());
}

QTEST_MAIN(TestPageLayout);
