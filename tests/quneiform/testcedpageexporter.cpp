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
#include <QTextDocument>
#include <QTextFrame>
#include "testcedpageexporter.h"
#include "gui/qtextdocumentexporter.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"

#define private public
#include "gui/cedpageexporter.h"

QString lineText(cf::CEDLine& line) {
    QString res;

    for(size_t i = 0; i < line.elementCount(); i++) {
        res.append(QChar(line.charAt(i)->get()));
    }

    return res;
}

QString parText(cf::CEDParagraph& par) {
    QString res;

    for(size_t i = 0; i < par.lineCount(); i++) {
        res += lineText(*par.lineAt(i)) + "\n";
    }

    return res;
}

QString colText(cf::CEDColumn& col) {
    QString res;

    for(size_t i = 0; i < col.elementCount(); i++) {
        cf::CEDParagraph * p = dynamic_cast<cf::CEDParagraph*>(col.elementAt(i));
        if(p)
            res += parText(*p);
    }

    return res;
}

TestCEDPageExporter::TestCEDPageExporter()
{
}

void TestCEDPageExporter::testConstruct() {
    CEDPageExporter e;
    QVERIFY(e.document() == 0);
    QVERIFY(e.page() == 0);
}

void TestCEDPageExporter::testDoExport() {
    QTextDocument doc;
    cf::CEDPage page;

    QTextFrameFormat format;
    format.setProperty(QTextDocumentExporter::BlockType, QTextDocumentExporter::PAGE);
    doc.rootFrame()->setFrameFormat(format);

    CEDPageExporter e;
    e.doExport(&doc, &page);
    QCOMPARE(e.document(), &doc);
    QCOMPARE(e.page(), &page);
}

void TestCEDPageExporter::testExportPage() {
    QTextDocument doc;
    cf::CEDPage page;
    CEDPageExporter e;
    e.doExport(&doc, &page);

    QCOMPARE(page.marginBottom(), 4);
    QCOMPARE(page.marginLeft(), 4);
    QCOMPARE(page.marginRight(), 4);
    QCOMPARE(page.marginTop(), 4);
    page.clear();

    QTextCursor cursor(&doc);
    QTextFrameFormat frame_format;
    frame_format.setTopMargin(10);
    frame_format.setLeftMargin(20);
    frame_format.setBottomMargin(30);
    frame_format.setRightMargin(40);
    frame_format.setProperty(QTextDocumentExporter::BlockType, QTextDocumentExporter::PAGE);
    cursor.currentFrame()->setFrameFormat(frame_format);

    e.doExport(&doc, &page);
    QCOMPARE(page.marginTop(), 10);
    QCOMPARE(page.marginLeft(), 20);
    QCOMPARE(page.marginBottom(), 30);
    QCOMPARE(page.marginRight(), 40);
    page.clear();

    cursor.insertBlock();
    cursor.insertText("test");
    QTextFrameFormat fmt2;
    fmt2.setProperty(QTextDocumentExporter::BlockType, QTextDocumentExporter::SECTION);
    cursor.insertFrame(fmt2);
    e.doExport(&doc, &page);

    QCOMPARE(page.sectionCount(), size_t(1));
    page.clear();
}

void TestCEDPageExporter::testExportSection() {
    QTextDocument doc;
    QTextCursor cursor(&doc);

    QTextFrameFormat format;
    format.setProperty(QTextDocumentExporter::BlockType, QTextDocumentExporter::SECTION);
    cursor.insertFrame(format);

    cursor = doc.rootFrame()->childFrames().at(0)->firstCursorPosition();
    cursor.insertText("test\nparagraph");

    cf::CEDPage page;
    CEDPageExporter e;
    e.setPage(&page);

    QVERIFY(page.empty());
    e.exportSection(doc.rootFrame()->childFrames().at(0));
    QCOMPARE(page.sectionCount(), (size_t) 1);
    QCOMPARE(page.sectionAt(0)->columnCount(), (size_t) 1);
    QCOMPARE(page.sectionAt(0)->columnAt(0)->elementCount(), (size_t) 2);
    cf::CEDParagraph * p1 = dynamic_cast<cf::CEDParagraph*>(page.sectionAt(0)->columnAt(0)->elementAt(0));
    QVERIFY(p1);
    QCOMPARE(p1->lineCount(), size_t(1));
    QCOMPARE(p1->lineAt(0)->elementCount(), size_t(4));
    QCOMPARE(p1->lineAt(0)->charAt(0)->get(), (uchar)'t');
    QCOMPARE(p1->lineAt(0)->charAt(1)->get(), (uchar)'e');
    QCOMPARE(p1->lineAt(0)->charAt(2)->get(), (uchar)'s');
    QCOMPARE(p1->lineAt(0)->charAt(3)->get(), (uchar)'t');

    cf::CEDParagraph * p2 = dynamic_cast<cf::CEDParagraph*>(page.sectionAt(0)->columnAt(0)->elementAt(1));
    QVERIFY(p2);
    QCOMPARE(p2->lineCount(), size_t(1));
    QCOMPARE(p2->lineAt(0)->elementCount(), size_t(9));
}

QTEST_MAIN(TestCEDPageExporter)
