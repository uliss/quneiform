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
#include <QTextTable>
#include <QTextFrame>
#include "testcedpageexporter.h"
#include "gui/export/qtextdocumentexporter.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"
#include "export/rout_own.h"

#define private public
#include "gui/export/cedpageexporter.h"

#define CFVERIFY(statement) \
do {\
    if (!QTest::qVerify((statement), #statement, "", __FILE__, __LINE__))\
        return NULL;\
} while (0)

#define COMPARE_CHAR(c1, c2) QCOMPARE(c1->get(), (uchar)c2);

QString toString(cf::CEDChar * chr, cf::Iconv& conv) {
    return QString::fromUtf8(conv.convert(chr->get()).c_str());
}

QString toString(cf::CEDLine * line) {
    QString res;
    cf::Iconv conv;
    conv.openToUtf8(LANGUAGE_RUSSIAN);
    for(size_t i = 0; i < line->elementCount(); i++) {
        res += toString(line->charAt(i), conv);
    }
    return res;
}

QString toString(cf::CEDParagraph * par) {
    QString res;

    for(size_t i = 0; i < par->lineCount(); i++)
        res += toString(par->lineAt(i));

    return res;
}

QString toString(cf::CEDColumn * col) {
    CFVERIFY(col);

    QString res;
    if(col->empty())
        return res;

    for(size_t i = 0; i < col->elementCount(); i++) {
        cf::CEDParagraph * p = dynamic_cast<cf::CEDParagraph*>(col->elementAt(i));
        if(!p)
            continue;
        res += toString(p);
    }
    return res;
}

void page_init(QTextFrame * page) {
    QTextFrameFormat format;
    format.setProperty(QTextDocumentExporter::BlockType, QTextDocumentExporter::PAGE);
    page->setFrameFormat(format);
}

void section_init(QTextFrame * section) {
    QTextFrameFormat format;
    format.setProperty(QTextDocumentExporter::BlockType, QTextDocumentExporter::SECTION);
    section->setFrameFormat(format);
}

void section_table_init(QTextTable * tbl) {
    QTextTableFormat fmt;
    fmt.setProperty(QTextDocumentExporter::BlockType, QTextDocumentExporter::SECTION);
    tbl->setFormat(fmt);
}

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

    QCOMPARE(page.marginBottom(), 0);
    QCOMPARE(page.marginLeft(), 0);
    QCOMPARE(page.marginRight(), 0);
    QCOMPARE(page.marginTop(), 0);
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

void TestCEDPageExporter::testExportString() {
    CEDPageExporter e;
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Bold);
    fmt.setFontUnderline(true);
    std::string str("test_string");
    cursor.insertText(str.c_str(), fmt);

    QTextFragment fragment = cursor.block().begin().fragment();

    cf::CEDLine l;
    e.exportString(fragment, &l);
    QCOMPARE(l.elementCount(), str.size());
    compare(&l, str.c_str());

    for(size_t i = 0; i < str.size(); i++) {
        QCOMPARE((int) l.charAt(i)->fontStyle(), FONT_BOLD | FONT_UNDERLINE);
    }
}

void TestCEDPageExporter::testExportColumn() {
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTable * tbl = cursor.insertTable(1, 2);

    cf::CEDPage page;
    CEDPageExporter e;
    e.setPage(&page);

    QVERIFY(page.empty());

    e.exportColumnTable(tbl);
}

void TestCEDPageExporter::testIsPage() {
    QVERIFY(!CEDPageExporter::isPage(NULL));
    QTextDocument doc;
    QVERIFY(!CEDPageExporter::isPage(doc.rootFrame()));

    page_init(doc.rootFrame());
    QVERIFY(CEDPageExporter::isPage(doc.rootFrame()));
}

void TestCEDPageExporter::testIsParagraph() {
    QTextDocument doc;
    QTextCursor cursor(&doc);

    cursor.insertBlock();
    QTextFrame::Iterator it = doc.rootFrame()->begin();
    QVERIFY(!it.currentFrame());
    QVERIFY(!CEDPageExporter::isParagraph(it.currentBlock()));

    QTextBlockFormat fmt;
    fmt.setProperty(QTextDocumentExporter::BlockType, QTextDocumentExporter::PARAGRAPH);
    cursor.insertBlock(fmt);
    QVERIFY(CEDPageExporter::isParagraph(cursor.block()));
}

void TestCEDPageExporter::testIsSection() {
    QVERIFY(!CEDPageExporter::isSection(NULL));
    QTextDocument doc;
    QTextCursor cursor(&doc);

    QTextFrame * section = cursor.insertFrame(QTextFrameFormat());
    QVERIFY(!CEDPageExporter::isSection(section));
    section_init(section);
    QVERIFY(CEDPageExporter::isSection(section));
}

void TestCEDPageExporter::testIsSectionTable() {
    // test NULL
    QVERIFY(!CEDPageExporter::isSectionTable(NULL));

    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTable * tbl = cursor.insertTable(2, 2);
    QVERIFY(!CEDPageExporter::isSectionTable(tbl));
    doc.clear();

    tbl = cursor.insertTable(1, 0);
    QVERIFY(!CEDPageExporter::isSectionTable(tbl));
    doc.clear();

    tbl =  cursor.insertTable(1, 1);
    QVERIFY(!CEDPageExporter::isSectionTable(tbl));

    section_table_init(tbl);
    QVERIFY(CEDPageExporter::isSectionTable(tbl));
}

void TestCEDPageExporter::testExportParagraph() {
    QTextDocument doc;
    QTextCursor cursor(&doc);

    cursor.insertBlock();
    cursor.insertText("test");

    cf::CEDColumn col;
    CEDPageExporter e;
    e.exportParagraph(cursor.block(), &col);

    QCOMPARE(col.elementCount(), (size_t) 1);
    QCOMPARE(toString(&col), QString("test"));
}

void TestCEDPageExporter::testUtf8() {
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QString str = QString::fromUtf8("utf-8 строка");
    cursor.insertText(str);

    try {
        cf::CEDColumn col;
        CEDPageExporter exp;
        exp.setLanguage(::Language(LANGUAGE_RUSSIAN));
        exp.exportParagraph(cursor.block(), &col);

        QCOMPARE(col.elementCount(), (size_t) 1);
        QCOMPARE(toString(&col), str);
        cf::CEDParagraph * p = dynamic_cast<cf::CEDParagraph*>(col.elementAt(0));
        QVERIFY(p);
        QCOMPARE(p->lineCount(), (size_t) 1);
        QCOMPARE(p->lineAt(0)->elementCount(), (size_t) str.length());
    }
    catch(std::exception& e) {
        qDebug() << e.what();
    }
}

#define CHECK_FONT_STYLE(e, fmt, style) {\
    cf::CEDChar * c = e.makeChar('a', fmt);\
    QCOMPARE(c->fontStyle(), (int) style);\
    delete c;\
}

static QRect toQRect(const cf::Rect& r) {
    return QRect(r.x(), r.y(), r.width(), r.height());
}

#define CHECK_BBOX(e, fmt, rect) {\
    fmt.setProperty(QTextDocumentExporter::BBOX, rect);\
    cf::CEDChar * c = e.makeChar('a', fmt);\
    QCOMPARE(toQRect(c->boundingRect()), rect);\
    delete c;\
}

void TestCEDPageExporter::testMakeChar() {
    CEDPageExporter e;
    QTextCharFormat fmt;

    cf::CEDChar * c = e.makeChar('a', fmt);
    QVERIFY(c);
    QCOMPARE(c->get(), (uchar) 'a');
    delete c;

    fmt.setFontUnderline(true);
    CHECK_FONT_STYLE(e, fmt, FONT_UNDERLINE);

    CharAlternatives alt;
    alt.add('t', 251);
    fmt.setProperty(QTextDocumentExporter::ALTERNATIVES, alt.toVariant());
    c = e.makeChar('a', fmt);
    QVERIFY(c->hasAlternatives());
    QCOMPARE(c->alternativeCount(), (size_t) 2);
    QCOMPARE(c->alternativeAt(1).getChar(), (uchar)'t');
    QCOMPARE(c->alternativeAt(1).probability(), (uchar) 251);
    delete c;

    QRect bbox(1, 2, 100, 200);
    CHECK_BBOX(e, fmt, bbox);
}

void TestCEDPageExporter::testExportFontStyle() {
    CEDPageExporter e;
    QTextCharFormat fmt;
    fmt.setFontUnderline(true);
    CHECK_FONT_STYLE(e, fmt, FONT_UNDERLINE);
    fmt.setFontItalic(true);
    CHECK_FONT_STYLE(e, fmt, FONT_UNDERLINE | FONT_ITALIC);
    fmt.setFontWeight(QFont::DemiBold);
    CHECK_FONT_STYLE(e, fmt, FONT_UNDERLINE | FONT_ITALIC | FONT_BOLD);
    fmt.setFontWeight(QFont::Bold);
    CHECK_FONT_STYLE(e, fmt, FONT_UNDERLINE | FONT_ITALIC | FONT_BOLD);
    fmt.setFontWeight(QFont::Black);
    CHECK_FONT_STYLE(e, fmt, FONT_UNDERLINE | FONT_ITALIC | FONT_BOLD);
    fmt.setFontWeight(QFont::Normal);
    CHECK_FONT_STYLE(e, fmt, FONT_UNDERLINE | FONT_ITALIC);

    fmt.setFontItalic(false);
    fmt.setFontUnderline(false);
    fmt.setFontStrikeOut(true);
    CHECK_FONT_STYLE(e, fmt, FONT_STRIKE);
}

void TestCEDPageExporter::testExportChar() {
    CEDPageExporter e;
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Bold);
    fmt.setFontUnderline(true);
    cursor.insertText("t", fmt);

    QTextFragment fragment = cursor.block().begin().fragment();

    cf::CEDLine l;
    e.exportChar(fragment, &l);
    QVERIFY(!l.empty());
    QCOMPARE(l.charAt(0)->get(), (uchar)'t');
    QCOMPARE((int) l.charAt(0)->fontStyle(), FONT_BOLD | FONT_UNDERLINE);
}

void TestCEDPageExporter::testExportCharAlternatives() {
    CEDPageExporter e;
    e.setLanguage(Language(LANGUAGE_RUSSIAN));
    QTextCharFormat fmt;

    CharAlternatives alt;
    alt.add('t', 251);
    alt.add('e', 100);
    fmt.setProperty(QTextDocumentExporter::ALTERNATIVES, alt.toVariant());
    cf::CEDChar c('a');
    e.exportCharAlternatives(&c, fmt);
    QVERIFY(c.hasAlternatives());
    QCOMPARE(c.alternativeCount(), (size_t) 3);
    QCOMPARE(c.alternativeAt(1).getChar(), (uchar)'t');
    QCOMPARE(c.alternativeAt(1).probability(), (uchar) 251);
    QCOMPARE(c.alternativeAt(2).getChar(), (uchar)'e');
    QCOMPARE(c.alternativeAt(2).probability(), (uchar) 100);
}

void TestCEDPageExporter::testExportBBox() {
    CEDPageExporter e;
    QTextCharFormat fmt;
    cf::CEDChar c;
    QRect bbox(1, 2, 100, 200);
    fmt.setProperty(QTextDocumentExporter::BBOX, bbox);
    e.exportBBox(&c, fmt);

    QCOMPARE(c.boundingRect().width(), 100);
    QCOMPARE(c.boundingRect().height(), 200);
    QCOMPARE(c.boundingRect().x(), 1);
    QCOMPARE(c.boundingRect().y(), 2);
}

QTEST_MAIN(TestCEDPageExporter)
