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
#include <QTextEdit>
#include <QTextFrame>
#include <QTextTable>
#include "common/formatoptions.h"
#include "gui/export/qtextdocumentexporter.h"
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "ced/cedpage.h"
#include "ced/cedpicture.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "export/rout_own.h"
#include "rdib/qtimageloader.h"

#include "testqtextdocumentexporter.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

using namespace cf;

CEDLine * makeLine(const std::string& str) {
    CEDLine * line = new CEDLine;

    for(size_t i = 0; i < str.size(); i++)
        line->addElement(new CEDChar(str[i]));

    return line;
}

CEDParagraph * makePar(const std::string& str) {
    CEDParagraph * par = new CEDParagraph;
    CEDLine * current_line = 0;
    for(size_t i = 0; i < str.size(); i++) {
        if(str[i] == '\n') {
            par->addLine(current_line);

            if(i == (str.size() - 1))
                current_line = NULL;
            else
                current_line = new CEDLine;
        }
        else {
            if(!current_line)
                current_line = new CEDLine;

            current_line->addElement(new CEDChar(str[i]));
        }
    }

    if(current_line)
        par->addLine(current_line);

    return par;
}

TestQTextDocumentExporter::TestQTextDocumentExporter(QObject * parent) :
    QObject(parent)
{
}

void TestQTextDocumentExporter::testConstruct() {
    QTextDocument doc;
    QTextDocumentExporter exp(NULL, cf::FormatOptions());
    exp.setDocument(&doc);
    QVERIFY(exp.document());
    QVERIFY(!doc.isModified());
}

void TestQTextDocumentExporter::testWriteChar() {
    FormatOptions opts;
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDChar c1('t');
    exp.exportChar(c1);
    QVERIFY(!doc.isEmpty());
    QVERIFY(doc.isModified());
    QCOMPARE(doc.toPlainText(), QString("t"));

    CEDChar c2('e');
    exp.exportChar(c2);
    QCOMPARE(doc.toPlainText(), QString("te"));
}

void TestQTextDocumentExporter::testWriteCharBold() {
    FormatOptions opts;
    opts.useBold(false);
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QVERIFY(exp.cursor().charFormat().fontWeight() != QFont::Bold);

    ch.setFontStyle(FONT_BOLD & FONT_UNDERLINE);
    exp.exportChar(ch);
    QVERIFY(exp.cursor().charFormat().fontWeight() != QFont::Bold);

    exp.formatOptions().useBold(true);
    ch.setFontStyle(FONT_UNDERLINE);
    exp.exportChar(ch);
    QVERIFY(exp.cursor().charFormat().fontWeight() != QFont::Bold);

    // bold
    ch.setFontStyle(FONT_BOLD);
    exp.exportChar(ch);
    QVERIFY(exp.cursor().charFormat().fontWeight() == QFont::Bold);
}

void TestQTextDocumentExporter::testWriteCharItalic() {
    FormatOptions opts;
    opts.useItalic(false);
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QVERIFY(!exp.cursor().charFormat().fontItalic());

    ch.setFontStyle(FONT_ITALIC & FONT_BOLD);
    exp.exportChar(ch);
    QVERIFY(!exp.cursor().charFormat().fontItalic());

    exp.formatOptions().useItalic(true);
    ch.setFontStyle(FONT_UNDERLINE);
    exp.exportChar(ch);
    QVERIFY(!exp.cursor().charFormat().fontItalic());

    // italic
    ch.setFontStyle(FONT_ITALIC);
    exp.exportChar(ch);
    QVERIFY(exp.cursor().charFormat().fontItalic());
}

void TestQTextDocumentExporter::testWriteCharUnderlined() {
    FormatOptions opts;
    opts.useUnderlined(false);

    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QVERIFY(!exp.cursor().charFormat().fontUnderline());

    ch.setFontStyle(FONT_UNDERLINE & FONT_BOLD);
    exp.exportChar(ch);
    QVERIFY(!exp.cursor().charFormat().fontUnderline());

    exp.formatOptions().useUnderlined(true);
    ch.setFontStyle(FONT_BOLD);
    exp.exportChar(ch);
    QVERIFY(!exp.cursor().charFormat().fontUnderline());

    // underlined
    ch.setFontStyle(FONT_UNDERLINE);
    exp.exportChar(ch);
    QVERIFY(exp.cursor().charFormat().fontUnderline());
}

void TestQTextDocumentExporter::testWriteCharFontSize() {
    FormatOptions opts;
    opts.useFontSize(false);

    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QVERIFY(!exp.cursor().charFormat().fontPointSize());

    ch.setFontHeight(100);
    exp.exportChar(ch);
    QCOMPARE(exp.cursor().charFormat().fontPointSize(), 0.0);

    exp.formatOptions().useFontSize(true);
    ch.setFontHeight(0);
    exp.exportChar(ch);
    QCOMPARE(exp.cursor().charFormat().fontPointSize(), 0.0);

    ch.setFontHeight(-1);
    exp.exportChar(ch);
    QCOMPARE(exp.cursor().charFormat().fontPointSize(), 0.0);

    ch.setFontHeight(100);
    exp.exportChar(ch);
    QCOMPARE(exp.cursor().charFormat().fontPointSize(), 100.0);
}

void TestQTextDocumentExporter::testWriteCharColors() {
    FormatOptions opts;
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QCOMPARE(exp.cursor().charFormat().foreground().color(), QColor(Qt::black));
    QCOMPARE(exp.cursor().charFormat().background().color(), QColor(0, 0, 0, 255));

    ch.setColor(Color(255, 100, 10));
    ch.setBackgroundColor(Color(100, 1, 2));
    exp.exportChar(ch);
    QCOMPARE(exp.cursor().charFormat().foreground().color(), QColor(255, 100, 10));
    QCOMPARE(exp.cursor().charFormat().background().color(), QColor(100, 1, 2));
}

void TestQTextDocumentExporter::testWriteCharEncoding() {
    FormatOptions opts;
    opts.setShowAlternatives(true);
    opts.setLanguage(LANGUAGE_RUSSIAN);
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);


    CEDChar ch1('a');
    exp.exportChar(ch1);
    QCOMPARE(doc.toPlainText(), QString("a"));
    QVERIFY(exp.cursor().charFormat().toolTip().isEmpty());

    ch1.addAlternative('b');
    exp.clear();
    exp.exportChar(ch1);
    QCOMPARE(doc.toPlainText(), QString("a"));
    QCOMPARE(exp.cursor().charFormat().toolTip(), QString("Alternatives:\n        'b' (254)"));

    CEDChar ch2('\xD4');
    ch2.addAlternative('\xF4');
    exp.clear();
    exp.exportChar(ch2);
    QCOMPARE(doc.toPlainText(), QString::fromUtf8("Ф"));
    QCOMPARE(exp.cursor().charFormat().toolTip(), QString::fromUtf8("Alternatives:\n        'ф' (254)"));
}

void TestQTextDocumentExporter::testWriteCharAlternatives() {
    FormatOptions opts;
    opts.setShowAlternatives(false);
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDChar ch1('a');
    exp.exportChar(ch1);
    QCOMPARE(doc.toPlainText(), QString("a"));
    QVERIFY(exp.cursor().charFormat().toolTip().isEmpty());
    QVERIFY(!exp.cursor().charFormat().hasProperty(QTextDocumentExporter::ALTERNATIVES));

    exp.formatOptions().setShowAlternatives(true);
    exp.clear();
    exp.exportChar(ch1);
    QCOMPARE(doc.toPlainText(), QString("a"));
    QVERIFY(exp.cursor().charFormat().toolTip().isEmpty());
    QVERIFY(!exp.cursor().charFormat().hasProperty(QTextDocumentExporter::ALTERNATIVES));

    ch1.addAlternative(Letter('b', 100));
    ch1.addAlternative(Letter('c', 50));

    exp.clear();
    exp.formatOptions().setShowAlternatives(false);
    exp.exportChar(ch1);
    QCOMPARE(doc.toPlainText(), QString("a"));
    QMap<QString, QVariant> alt_map;
    alt_map.insert("b", 100);
    alt_map.insert("c", 50);
    QVERIFY(!exp.cursor().charFormat().hasProperty(QTextDocumentExporter::ALTERNATIVES));

    exp.clear();
    exp.formatOptions().setShowAlternatives(true);
    exp.exportChar(ch1);
    QCOMPARE(doc.toPlainText(), QString("a"));
    QCOMPARE(exp.cursor().charFormat().toolTip(),
             QString("Alternatives:\n        'b' (100)\n        'c' (50)"));
    QCOMPARE(exp.cursor().charFormat().underlineColor(), QColor(Qt::red));
    QCOMPARE(exp.cursor().charFormat().underlineStyle(), QTextCharFormat::DashUnderline);
    QVERIFY(exp.cursor().charFormat().hasProperty(QTextDocumentExporter::ALTERNATIVES));
}

void TestQTextDocumentExporter::testWriteCharBBox() {
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDChar ch('t');

    exp.exportChar(ch);
    QCOMPARE(exp.cursor().charFormat().property(QTextDocumentExporter::BBOX).toRect(), QRect());
    exp.clear();

    ch.setBoundingRect(cf::Rect(cf::Point(1, 2), 300, 400));
    exp.exportChar(ch);
    exp.cursor().movePosition(QTextCursor::PreviousCharacter);
    QCOMPARE(exp.cursor().charFormat().toCharFormat().propertyCount(), 1);
    QCOMPARE(exp.cursor().charFormat().property(QTextDocumentExporter::BBOX).toRect(),
             QRect(1, 2, 300, 400));
}

void TestQTextDocumentExporter::testWritePage() {
    CEDPage page;
    QTextDocumentExporter exp(&page, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    // test null margins
    exp.exportPage(page);
    QVERIFY(doc.toPlainText().trimmed().isEmpty());
    QTextFrameFormat page_format = doc.rootFrame()->format().toFrameFormat();
    QCOMPARE(page_format.topMargin(), 0.0);
    QCOMPARE(page_format.bottomMargin(), 0.0);
    QCOMPARE(page_format.leftMargin(), 0.0);
    QCOMPARE(page_format.rightMargin(), 0.0);
    QCOMPARE(page_format.intProperty(QTextDocumentExporter::BlockType), int(QTextDocumentExporter::PAGE));

    // test for margins
    page.setMargins(10, 20, 30, 40);
    exp.clear();
    exp.exportPage(page);
    QVERIFY(doc.toPlainText().trimmed().isEmpty());
    page_format = doc.rootFrame()->format().toFrameFormat();
    QCOMPARE(page_format.topMargin(), 10.0);
    QCOMPARE(page_format.rightMargin(), 20.0);
    QCOMPARE(page_format.bottomMargin(), 30.0);
    QCOMPARE(page_format.leftMargin(), 40.0);

    // test for several sections
    CEDSection * s = new CEDSection;
    page.addSection(s);
    page.addSection(new CEDSection);
    exp.clear();
    exp.exportPage(page);

    QVERIFY(doc.rootFrame()->childFrames().isEmpty());
    page.sectionAt(0)->addColumn(new CEDColumn);
    page.sectionAt(1)->addColumn(new CEDColumn);

    exp.clear();
    exp.exportPage(page);

    QCOMPARE(doc.rootFrame()->childFrames().count(), 2);
    QTextFrame * section_frame = qobject_cast<QTextFrame*>(doc.rootFrame()->childFrames().at(0));
    QVERIFY(section_frame);
    QCOMPARE(section_frame->format().intProperty(QTextDocumentExporter::BlockType),
             (int) QTextDocumentExporter::SECTION);
    section_frame = qobject_cast<QTextFrame*>(doc.rootFrame()->childFrames().at(1));
    QVERIFY(section_frame);
    QCOMPARE(section_frame->format().intProperty(QTextDocumentExporter::BlockType),
             (int) QTextDocumentExporter::SECTION);

    page.clear();
    page.addSection(new CEDSection);
    CEDColumn * col = new CEDColumn;
    col->addElement(makePar("test"));
    page.sectionAt(0)->addColumn(col);
    exp.clear();
    exp.exportPage(page);
    QCOMPARE(doc.toPlainText().trimmed(), QString("test"));
}

void TestQTextDocumentExporter::testWriteParagraph() {
    CEDParagraph par;
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);


    CEDParagraph * par2 = makePar("two line\nparagraph");
    exp.clear();
    exp.formatOptions().setPreserveLineBreaks(true);
    exp.exportParagraph(*par2);
    QCOMPARE(doc.toPlainText().trimmed(), QString("two line\nparagraph"));
    exp.clear();
    exp.formatOptions().setPreserveLineBreaks(false);
    exp.exportParagraph(*par2);
    QCOMPARE(doc.toPlainText(), QString("\ntwo line paragraph"));
    QCOMPARE(doc.toPlainText().trimmed(), QString("two line paragraph"));
    delete par2;

    CEDParagraph * par3 = makePar("two line hy-\nphen");
    exp.clear();
    exp.formatOptions().setPreserveLineBreaks(true);
    exp.exportParagraph(*par3);
    QCOMPARE(doc.toPlainText().trimmed(), QString("two line hy-\nphen"));
    exp.clear();

    exp.formatOptions().setPreserveLineBreaks(false);
    exp.exportParagraph(*par3);
    // soft hyphen
    QCOMPARE(doc.toPlainText().trimmed(), QString("two line hy\xADphen"));
    delete par3;
}

void TestQTextDocumentExporter::testWriteParagraphAlign() {
    CEDParagraph par;
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    // default
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor().blockFormat().alignment(), Qt::AlignLeft);

    // right
    exp.clear();
    par.setAlign(cf::ALIGN_RIGHT);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor().blockFormat().alignment(), Qt::AlignRight);
    QCOMPARE(doc.toPlainText(), QString("\n"));

    // left
    exp.clear();
    par.setAlign(cf::ALIGN_LEFT);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor().blockFormat().alignment(), Qt::AlignLeft);

    // center
    exp.clear();
    par.setAlign(cf::ALIGN_CENTER);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor().blockFormat().alignment(), Qt::AlignHCenter);

    // justify
    exp.clear();
    par.setAlign(cf::ALIGN_JUSTIFY);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor().blockFormat().alignment(), Qt::AlignJustify);
}

void TestQTextDocumentExporter::testWriteParagraphIndent() {
    CEDParagraph par;
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    par.setIndent(10);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor().blockFormat().textIndent(), 10.0);

    exp.clear();
    par.setIndent(-12);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor().blockFormat().textIndent(), -12.0);

    exp.clear();
    par.setIndent(0);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor().blockFormat().textIndent(), 0.0);
}

void TestQTextDocumentExporter::testWriteLine() {
    FormatOptions opts;
    opts.setPreserveLineBreaks(true);
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDLine line;
    exp.exportLine(line);
    QCOMPARE(doc.toPlainText(), QString(""));

    line.addElement(new CEDChar('t'));
    exp.exportLine(line);
    QCOMPARE(doc.toPlainText(), QString("t\n"));

    exp.clear();
    exp.formatOptions().setPreserveLineBreaks(false);
    exp.exportLine(line);
    QCOMPARE(doc.toPlainText(), QString("t "));

    line.addElement(new CEDChar('-'));
    exp.clear();
    exp.exportLine(line);
    QCOMPARE(doc.toPlainText(), QString("t\xAD"));

    CEDLine * l2 = makeLine("test line");
    exp.clear();
    exp.exportLine(*l2);
    QCOMPARE(doc.toPlainText().trimmed(), QString("test line"));
    delete l2;
}

void TestQTextDocumentExporter::testWriteSection() {
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDSection sec;
    sec.addColumn(new CEDColumn());

    exp.exportSection(sec);
    QVERIFY(doc.toPlainText().trimmed().isEmpty());
    QCOMPARE(doc.rootFrame()->childFrames().at(0)->format().intProperty(QTextDocumentExporter::BlockType),
             (int) QTextDocumentExporter::SECTION);

    sec.addColumn(new CEDColumn());
    exp.clear();
    exp.exportSection(sec);
    QTextTable * table = qobject_cast<QTextTable*>(doc.rootFrame()->childFrames().at(0));
    QVERIFY(table);
    QCOMPARE(table->columns(), (int) sec.columnCount());
    QCOMPARE(table->rows(), 1);
    QCOMPARE(table->format().intProperty(QTextDocumentExporter::BlockType),
             (int) QTextDocumentExporter::SECTION);

    sec.addColumn(new CEDColumn());
    exp.clear();
    exp.exportSection(sec);

    table = qobject_cast<QTextTable*>(doc.rootFrame()->childFrames().at(0));
    QCOMPARE(table->columns(), (int) sec.columnCount());

    sec.columnAt(0)->addElement(makePar("column 1"));
    sec.columnAt(1)->addElement(makePar("column 2"));
    exp.clear();
    exp.exportSection(sec);

    table = qobject_cast<QTextTable*>(doc.rootFrame()->childFrames().at(0));
    QCOMPARE(doc.toPlainText().trimmed(), QString("column 1\n\ncolumn 2"));

    QTextCursor cell_cursor = table->cellAt(0, 0).firstCursorPosition();
    QVERIFY(cell_cursor.movePosition(QTextCursor::NextBlock));
    QVERIFY(cell_cursor.atBlockStart());
    QCOMPARE(cell_cursor.block().text(), QString("column 1"));

    cell_cursor = table->cellAt(0, 1).firstCursorPosition();
    QVERIFY(cell_cursor.movePosition(QTextCursor::NextBlock));
    QVERIFY(cell_cursor.atBlockStart());
    QCOMPARE(cell_cursor.block().text(), QString("column 2"));

    // empty section
    CEDSection empty_section;
    exp.clear();
    exp.exportSection(empty_section);
    QCOMPARE(doc.toPlainText().trimmed(), QString(""));
    QVERIFY(doc.rootFrame()->childFrames().isEmpty());
}

void TestQTextDocumentExporter::testWriteSectionMargins() {
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDSection sec;
    sec.addColumn(new CEDColumn);
    exp.exportSection(sec);
    QVERIFY(doc.toPlainText().trimmed().isEmpty());
    QCOMPARE(doc.rootFrame()->childFrames().count(), 1);
    QTextFrameFormat section_format = doc.rootFrame()->childFrames().at(0)->format().toFrameFormat();
    QCOMPARE(section_format.topMargin(), 0.0);
    QCOMPARE(section_format.bottomMargin(), 0.0);
    QCOMPARE(section_format.leftMargin(), 0.0);
    QCOMPARE(section_format.rightMargin(), 0.0);

//    exp.clear();
//    sec.setMargins(10, 20, 30, 40);
//    exp.exportSection(sec);
//    QVERIFY(doc.toPlainText().trimmed().isEmpty());
//    section_format = doc.rootFrame()->childFrames().at(0)->format().toFrameFormat();
//    QCOMPARE(section_format.topMargin(), 10.0);
//    QCOMPARE(section_format.rightMargin(), 20.0);
//    QCOMPARE(section_format.bottomMargin(), 30.0);
//    QCOMPARE(section_format.leftMargin(), 40.0);
}

void TestQTextDocumentExporter::testWriteColumn() {
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDSection sec;
    sec.addColumn(new CEDColumn());
    sec.addColumn(new CEDColumn());
    sec.columnAt(0)->setWidth(100);
    exp.exportSection(sec);

    QTextTable * table = qobject_cast<QTextTable*>(doc.rootFrame()->childFrames().at(0));
    QCOMPARE(table->columns(), 2);
    QCOMPARE(table->format().cellPadding(), 3.0);
    QCOMPARE(table->format().cellSpacing(), 0.0);
    QCOMPARE(table->format().borderStyle(), QTextFrameFormat::BorderStyle_Dotted);
    QCOMPARE(table->format().border(), 1.0);
    QCOMPARE(table->format().columnWidthConstraints().at(0),
             QTextLength(QTextLength::FixedLength, 100));
}

void TestQTextDocumentExporter::testWritePicture() {
//    QTextDocumentExporter exp(NULL, FormatOptions());
//    QTextDocument doc;
//    exp.setDocument(&doc);

//    CEDPicture pic;
//    exp.exportPicture(pic);

//    uchar buffer[100] = {0};
//    ImagePtr img(new Image(buffer, 100, Image::AllocatorNone));

//    pic.setImage(img);
//    exp.exportPicture(pic);

////    QtImageLoader loader;
//    QImage qimg(CF_IMAGE_DIR "/english.png");
//    ImagePtr img2(new cf::Image(qimg.bits(), qimg.byteCount(), Image::AllocatorNone));
//    img2->setSize(Size(qimg.width(), qimg.height()));
//    pic.setImage(img2);

//    exp.exportPicture(pic);
//    QCOMPARE(exp.cursor().charFormat().toImageFormat().height(), 81.0);
}

void TestQTextDocumentExporter::testComplex() {
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDPage page;
    page.addSection(new CEDSection);
    CEDColumn * col = new CEDColumn;
    col->addElement(makePar("two\nlines"));
    page.sectionAt(0)->addColumn(col);
    exp.exportPage(page);

    QCOMPARE(doc.toPlainText().trimmed(), QString("two lines"));
}

QTEST_MAIN(TestQTextDocumentExporter);
