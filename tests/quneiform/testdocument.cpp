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
#include <QFile>
#include <QFileInfo>
#include "testdocument.h"
#include "gui/document.h"
#include "gui/page.h"

TestDocument::TestDocument(QObject *parent) :
    QObject(parent)
{
}

void TestDocument::testConstruct() {
    Document doc;
    QCOMPARE(doc.fileName(), QString("untitled.qpf"));
    QCOMPARE(doc.language(), -1);
    QVERIFY(!doc.isChanged());
    QVERIFY(doc.isNew());
}

void TestDocument::testPageSignals() {
    Document doc;
    doc.append(new Page("page 1"));
    doc.append(new Page("page 2"));

    QSignalSpy changed(&doc, SIGNAL(changed()));

    doc.page(0)->setSelected(true);
    QCOMPARE(changed.count(), 1);
    QVERIFY(doc.isChanged());
    // not repeated
    doc.page(0)->setSelected(true);
    QCOMPARE(changed.count(), 1);

    doc.page(0)->setSelected(false);
    QCOMPARE(changed.count(), 2);

    doc.page(1)->setNumber(2);
    QCOMPARE(changed.count(), 3);

    doc.page(1)->setOcrText("test");
    QCOMPARE(changed.count(), 4);

    doc.page(0)->setPageArea(QRect(10, 10, 30, 40));
    QCOMPARE(changed.count(), 5);

    // no change
    doc.page(0)->setViewScroll(QPoint(10, 20));
    QCOMPARE(changed.count(), 5);

    Page::Rectangles r;
    doc.page(1)->setRects(r, Page::CHAR);
    QCOMPARE(changed.count(), 6);

    RecognitionSettings s;
    s.setDotMatrix(true);

    doc.page(0)->setRecognitionSettings(s);
    QCOMPARE(changed.count(), 7);
}

void TestDocument::testAppend() {
    Document doc;
    QSignalSpy changed(&doc, SIGNAL(changed()));
    QSignalSpy page_added(&doc, SIGNAL(pageAdded(Page*)));
    QSignalSpy image_duplication(&doc, SIGNAL(imageDuplicated(QString)));

    doc.append(new Page(""));

    QCOMPARE(changed.count(), 1);
    QCOMPARE(page_added.count(), 1);
    QVERIFY(doc.isChanged());
    QCOMPARE(doc.pageCount(), 1);
    QVERIFY(doc.page(0)->parent() == &doc);
    QCOMPARE(image_duplication.count(), 0);

    doc.append(NULL);
    QCOMPARE(changed.count(), 1);
    QCOMPARE(page_added.count(), 1);
    QCOMPARE(doc.pageCount(), 1);
    QCOMPARE(image_duplication.count(), 0);

    // duplication
    doc.append(new Page(""));
    QCOMPARE(changed.count(), 1);
    QCOMPARE(page_added.count(), 1);
    QCOMPARE(doc.pageCount(), 1);
    QCOMPARE(image_duplication.count(), 1);

    // allow duplication
    doc.append(new Page(""), true);
    QCOMPARE(changed.count(), 2);
    QCOMPARE(page_added.count(), 2);
    QVERIFY(doc.isChanged());
    QCOMPARE(doc.pageCount(), 2);
    QVERIFY(doc.page(1)->parent() == &doc);
    QCOMPARE(image_duplication.count(), 1);
}

void TestDocument::testClear() {
    Document doc;
    QSignalSpy changed(&doc, SIGNAL(changed()));
    QSignalSpy page_removed(&doc, SIGNAL(pageRemoved(Page*)));

    doc.clear();
    QCOMPARE(changed.count(), 0);
    QCOMPARE(page_removed.count(), 0);

    doc.append(new Page("path 1")); // +1 changed
    doc.append(new Page("path 2")); // +1 changed

    doc.clear();
    QCOMPARE(doc.pageCount(), 0);
    QCOMPARE(changed.count(), 3);
    QCOMPARE(page_removed.count(), 2);
}

void TestDocument::testCountSelected() {
    Document doc;
    QCOMPARE(doc.countSelected(), 0);

    doc.append(new Page(""));
    QCOMPARE(doc.countSelected(), 0);

    doc.page(0)->setSelected(true);
    QCOMPARE(doc.countSelected(), 1);

    doc.append(new Page("path"));
    QCOMPARE(doc.countSelected(), 1);
    doc.page(1)->setSelected(true);
    QCOMPARE(doc.countSelected(), 2);

    doc.page(0)->setSelected(false);
    QCOMPARE(doc.countSelected(), 1);

    doc.page(1)->setSelected(false);
    QCOMPARE(doc.countSelected(), 0);
}

void TestDocument::testHasPage() {
    Document doc;
    QVERIFY(!doc.hasPage(""));

    doc.append(new Page(""));
    QVERIFY(doc.hasPage(""));
    QVERIFY(!doc.hasPage("foo"));

    doc.append(new Page(""), true);
    QVERIFY(doc.hasPage(""));
    QVERIFY(!doc.hasPage("foo"));

    doc.append(new Page("foo"));
    QVERIFY(doc.hasPage(""));
    QVERIFY(doc.hasPage("foo"));
}

void TestDocument::testLanguage() {
    Document doc;
    QSignalSpy changed(&doc, SIGNAL(changed()));
    QCOMPARE(doc.language(), -1);

    doc.setLanguage(1);
    QCOMPARE(doc.language(), 1);
    QCOMPARE(changed.count(), 1);
}

void TestDocument::testOpen() {
    Document doc;
    QVERIFY(!doc.open("unknown file"));
}

void TestDocument::testPage() {
    Document doc;
    QCOMPARE(doc.page(0), (Page*)0);
    QCOMPARE(doc.page(1), (Page*)0);

    doc.append(new Page(""));
    QVERIFY(doc.page(0) != 0);
    QCOMPARE(doc.page(1), (Page*)0);
}

void TestDocument::testSave() {
    Document doc;
    QString fname("doc.test");
    doc.save(fname);

    QFileInfo fi(fname);
    QVERIFY(fi.exists());
    QVERIFY(fi.size() > 0);

    QFile f(fname);
    f.remove(fname);
}

QTEST_MAIN(TestDocument)
