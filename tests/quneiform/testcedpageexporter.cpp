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
#include <QTextDocument>
#include <QTextFrame>
#include "testcedpageexporter.h"
#include "gui/export/qtextdocumentexporter.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"

#define private public
#include "gui/export/cedpageexporter.h"

#define CFVERIFY(statement) \
do {\
    if (!QTest::qVerify((statement), #statement, "", __FILE__, __LINE__))\
        return NULL;\
} while (0)

#define COMPARE_CHAR(c1, c2) QCOMPARE(c1->get(), (uchar)c2);

cf::CEDParagraph * toPar(cf::CEDPage * p, uint section = 0, uint column = 0, uint par = 0) {
    CFVERIFY(section < p->sectionCount());
    cf::CEDSection * s = p->sectionAt(section);
    CFVERIFY(column < s->columnCount());
    cf::CEDColumn * col = s->columnAt(column);
    CFVERIFY(par < col->elementCount());
    return dynamic_cast<cf::CEDParagraph*>(col->elementAt(par));
}

inline void compare(cf::CEDLine * l, const char * str) {
    QCOMPARE(l->elementCount(), (size_t) strlen(str));
    for(uint i = 0; i < l->elementCount(); i++) {
        COMPARE_CHAR(l->charAt(i), str[i]);
    }
}

inline void compare(cf::CEDParagraph * p, const char * str) {
    QStringList str_lst = QString::fromUtf8(str).split('\n');
    QCOMPARE(p->lineCount(), (size_t) str_lst.size());
    for(uint i = 0; i < p->lineCount(); i++) {
        QByteArray data = str_lst.at(i).toAscii();
        compare(p->lineAt(i), data.constData());
    }
}

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
    cf::CEDParagraph * p1 = toPar(&page, 0, 0, 0);
    QVERIFY(p1);
    compare(p1, "test");

    cf::CEDParagraph * p2 = toPar(&page, 0, 0, 1);
    QVERIFY(p2);
    compare(p2, "paragraph");
}

QTEST_MAIN(TestCEDPageExporter)
