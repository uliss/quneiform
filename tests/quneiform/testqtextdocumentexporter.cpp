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
#include <QTextTable>
#include "common/formatoptions.h"
#include "gui/qtextdocumentexporter.h"
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

TestQTextDocumentExporter::TestQTextDocumentExporter(QObject * parent) :
    QObject(parent)
{
}

void TestQTextDocumentExporter::testConstruct() {
    QTextDocument doc;
    QTextDocumentExporter exp(NULL, cf::FormatOptions());
    exp.setDocument(&doc);
    QVERIFY(exp.document());
    QVERIFY(!exp.document()->isModified());
}

void TestQTextDocumentExporter::testWriteChar() {
    FormatOptions opts;
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDChar c1('t');
    exp.exportChar(c1);
    QVERIFY(!exp.document()->isEmpty());
    QVERIFY(exp.document()->isModified());
    QCOMPARE(exp.document()->toPlainText(), QString("t"));

    CEDChar c2('e');
    exp.exportChar(c2);
    QCOMPARE(exp.document()->toPlainText(), QString("te"));
}

void TestQTextDocumentExporter::testWriteCharBold() {
    FormatOptions opts;
    opts.useBold(false);
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QVERIFY(exp.cursor()->charFormat().fontWeight() != QFont::Bold);

    ch.setFontStyle(FONT_BOLD & FONT_UNDERLINE);
    exp.exportChar(ch);
    QVERIFY(exp.cursor()->charFormat().fontWeight() != QFont::Bold);

    exp.formatOptions().useBold(true);
    ch.setFontStyle(FONT_UNDERLINE);
    exp.exportChar(ch);
    QVERIFY(exp.cursor()->charFormat().fontWeight() != QFont::Bold);

    // bold
    ch.setFontStyle(FONT_BOLD);
    exp.exportChar(ch);
    QVERIFY(exp.cursor()->charFormat().fontWeight() == QFont::Bold);
}

void TestQTextDocumentExporter::testWriteCharItalic() {
    FormatOptions opts;
    opts.useItalic(false);
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QVERIFY(!exp.cursor()->charFormat().fontItalic());

    ch.setFontStyle(FONT_ITALIC & FONT_BOLD);
    exp.exportChar(ch);
    QVERIFY(!exp.cursor()->charFormat().fontItalic());

    exp.formatOptions().useItalic(true);
    ch.setFontStyle(FONT_UNDERLINE);
    exp.exportChar(ch);
    QVERIFY(!exp.cursor()->charFormat().fontItalic());

    // italic
    ch.setFontStyle(FONT_ITALIC);
    exp.exportChar(ch);
    QVERIFY(exp.cursor()->charFormat().fontItalic());
}

void TestQTextDocumentExporter::testWriteCharUnderlined() {
    FormatOptions opts;
    opts.useUnderlined(false);

    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QVERIFY(!exp.cursor()->charFormat().fontUnderline());

    ch.setFontStyle(FONT_UNDERLINE & FONT_BOLD);
    exp.exportChar(ch);
    QVERIFY(!exp.cursor()->charFormat().fontUnderline());

    exp.formatOptions().useUnderlined(true);
    ch.setFontStyle(FONT_BOLD);
    exp.exportChar(ch);
    QVERIFY(!exp.cursor()->charFormat().fontUnderline());

    // underlined
    ch.setFontStyle(FONT_UNDERLINE);
    exp.exportChar(ch);
    QVERIFY(exp.cursor()->charFormat().fontUnderline());
}

void TestQTextDocumentExporter::testWriteCharFontSize() {
    FormatOptions opts;
    opts.useFontSize(false);

    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QVERIFY(!exp.cursor()->charFormat().fontPointSize());

    ch.setFontHeight(100);
    exp.exportChar(ch);
    QCOMPARE(exp.cursor()->charFormat().fontPointSize(), 0.0);

    exp.formatOptions().useFontSize(true);
    ch.setFontHeight(0);
    exp.exportChar(ch);
    QCOMPARE(exp.cursor()->charFormat().fontPointSize(), 0.0);

    ch.setFontHeight(-1);
    exp.exportChar(ch);
    QCOMPARE(exp.cursor()->charFormat().fontPointSize(), 0.0);

    ch.setFontHeight(100);
    exp.exportChar(ch);
    QCOMPARE(exp.cursor()->charFormat().fontPointSize(), 100.0);
}

void TestQTextDocumentExporter::testWriteCharColors() {
    FormatOptions opts;
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);
    CEDChar ch('s');
    exp.exportChar(ch);
    QCOMPARE(exp.cursor()->charFormat().foreground().color(), QColor(Qt::black));
    QCOMPARE(exp.cursor()->charFormat().background().color(), QColor(0, 0, 0, 255));

    ch.setColor(Color(255, 100, 10));
    ch.setBackgroundColor(Color(100, 1, 2));
    exp.exportChar(ch);
    QCOMPARE(exp.cursor()->charFormat().foreground().color(), QColor(255, 100, 10));
    QCOMPARE(exp.cursor()->charFormat().background().color(), QColor(100, 1, 2));
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
    QCOMPARE(exp.document()->toPlainText(), QString("a"));
    QVERIFY(exp.cursor()->charFormat().toolTip().isEmpty());

    ch1.addAlternative('b');
    exp.clear();
    exp.exportChar(ch1);
    QCOMPARE(exp.document()->toPlainText(), QString("a"));
    QCOMPARE(exp.cursor()->charFormat().toolTip(), QString("Alternatives:\n        'b' (254)"));

    CEDChar ch2('\xD4');
    ch2.addAlternative('\xF4');
    exp.clear();
    exp.exportChar(ch2);
    QCOMPARE(exp.document()->toPlainText(), QString::fromUtf8("Ф"));
    QCOMPARE(exp.cursor()->charFormat().toolTip(), QString::fromUtf8("Alternatives:\n        'ф' (254)"));
}

void TestQTextDocumentExporter::testWriteCharAlternatives() {
    FormatOptions opts;
    opts.setShowAlternatives(false);
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDChar ch1('a');
    exp.exportChar(ch1);
    QCOMPARE(exp.document()->toPlainText(), QString("a"));
    QVERIFY(exp.cursor()->charFormat().toolTip().isEmpty());

    exp.formatOptions().setShowAlternatives(true);
    exp.clear();
    exp.exportChar(ch1);
    QCOMPARE(exp.document()->toPlainText(), QString("a"));
    QVERIFY(exp.cursor()->charFormat().toolTip().isEmpty());

    ch1.addAlternative(Letter('b', 100));
    ch1.addAlternative(Letter('c', 50));

    exp.clear();
    exp.formatOptions().setShowAlternatives(false);
    exp.exportChar(ch1);
    QCOMPARE(exp.document()->toPlainText(), QString("a"));
    QMap<QString, QVariant> alt_map;
    alt_map.insert("b", 100);
    alt_map.insert("c", 50);
    QCOMPARE(exp.cursor()->charFormat().property(QTextDocumentExporter::ALTERNATIVES).toMap(), alt_map);

    exp.clear();
    exp.formatOptions().setShowAlternatives(true);
    exp.exportChar(ch1);
    QCOMPARE(exp.document()->toPlainText(), QString("a"));
    QCOMPARE(exp.cursor()->charFormat().toolTip(),
             QString("Alternatives:\n        'b' (100)\n        'c' (50)"));
    QCOMPARE(exp.cursor()->charFormat().underlineColor(), QColor(Qt::red));
    QCOMPARE(exp.cursor()->charFormat().underlineStyle(), QTextCharFormat::DashUnderline);
}

void TestQTextDocumentExporter::testWriteCharBBox() {
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDChar ch('t');

    exp.exportChar(ch);
    QCOMPARE(exp.cursor()->charFormat().property(QTextDocumentExporter::BBOX).toRect(), QRect());
    exp.clear();

    ch.setBoundingRect(cf::Rect(cf::Point(1, 2), 300, 400));
    exp.exportChar(ch);
    exp.cursor()->movePosition(QTextCursor::PreviousCharacter);
    QCOMPARE(exp.cursor()->charFormat().toCharFormat().propertyCount(), 1);
    QCOMPARE(exp.cursor()->charFormat().property(QTextDocumentExporter::BBOX).toRect(), QRect(1, 2, 300, 400));
}

void TestQTextDocumentExporter::testWriteParagraph() {
    CEDParagraph par;
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    // default
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor()->blockFormat().alignment(), Qt::AlignLeft);

    // right
    exp.clear();
    par.setAlign(cf::ALIGN_RIGHT);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor()->blockFormat().alignment(), Qt::AlignRight);
    QCOMPARE(exp.document()->toPlainText(), QString("\n"));

    // left
    exp.clear();
    par.setAlign(cf::ALIGN_LEFT);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor()->blockFormat().alignment(), Qt::AlignLeft);

    // center
    exp.clear();
    par.setAlign(cf::ALIGN_CENTER);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor()->blockFormat().alignment(), Qt::AlignHCenter);

    // justify
    exp.clear();
    par.setAlign(cf::ALIGN_JUSTIFY);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor()->blockFormat().alignment(), Qt::AlignJustify);
}

void TestQTextDocumentExporter::testWriteParagraphIndent() {
    CEDParagraph par;
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    par.setIndent(10);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor()->blockFormat().textIndent(), 10.0);

    exp.clear();
    par.setIndent(-12);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor()->blockFormat().textIndent(), -12.0);

    exp.clear();
    par.setIndent(0);
    exp.exportParagraph(par);
    QCOMPARE(exp.cursor()->blockFormat().textIndent(), 0.0);
}

void TestQTextDocumentExporter::testWriteLine() {
    FormatOptions opts;
    opts.setPreserveLineBreaks(true);
    QTextDocumentExporter exp(NULL, opts);
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDLine line;
    exp.exportLine(line);
    QCOMPARE(exp.document()->toPlainText(), QString(""));

    line.addElement(new CEDChar('t'));
    exp.exportLine(line);
    QCOMPARE(exp.document()->toPlainText(), QString("t\n"));

    exp.clear();
    exp.formatOptions().setPreserveLineBreaks(false);
    exp.exportLine(line);
    QCOMPARE(exp.document()->toPlainText(), QString("t "));

    line.addElement(new CEDChar('-'));
    exp.clear();
    exp.exportLine(line);
    QCOMPARE(exp.document()->toPlainText(), QString("t\xAD"));
}

void TestQTextDocumentExporter::testWriteSection() {
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDSection sec;
    exp.exportSection(sec);
    QCOMPARE(exp.document()->toPlainText(), QString());

    sec.addColumn(new CEDColumn());
    exp.clear();
    exp.exportSection(sec);
    QCOMPARE(exp.document()->toPlainText(), QString());
    QVERIFY(!exp.cursor()->currentTable());

    sec.addColumn(new CEDColumn());
    exp.clear();
    exp.exportSection(sec);
    QVERIFY(exp.cursor()->currentTable());
    QCOMPARE(exp.cursor()->currentTable()->columns(), 2);
    QCOMPARE(exp.cursor()->currentTable()->rows(), 1);

    sec.addColumn(new CEDColumn());
    exp.clear();
    exp.exportSection(sec);
    QVERIFY(exp.cursor()->currentTable());
    QCOMPARE(exp.cursor()->currentTable()->columns(), 3);
    QCOMPARE(exp.cursor()->currentTable()->rows(), 1);
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

    QCOMPARE(exp.cursor()->currentTable()->columns(), 2);
    QCOMPARE(exp.cursor()->currentTable()->format().cellPadding(), 3.0);
    QCOMPARE(exp.cursor()->currentTable()->format().cellSpacing(), 0.0);
    QCOMPARE(exp.cursor()->currentTable()->format().borderStyle(), QTextFrameFormat::BorderStyle_Dotted);
    QCOMPARE(exp.cursor()->currentTable()->format().border(), 1.0);
    QCOMPARE(exp.cursor()->currentTable()->format().columnWidthConstraints().at(0),
             QTextLength(QTextLength::FixedLength, 100));
}

void TestQTextDocumentExporter::testWritePicture() {
    QTextDocumentExporter exp(NULL, FormatOptions());
    QTextDocument doc;
    exp.setDocument(&doc);

    CEDPicture pic;
    exp.exportPicture(pic);

    uchar buffer[100] = {0};
    ImagePtr img(new Image(buffer, 100, Image::AllocatorNone));

    pic.setImage(img);
    exp.exportPicture(pic);

//    QtImageLoader loader;
    QImage qimg(CF_IMAGE_DIR "/english.png");
    ImagePtr img2(new cf::Image(qimg.bits(), qimg.byteCount(), Image::AllocatorNone));
    img2->setSize(Size(qimg.width(), qimg.height()));
    pic.setImage(img2);

    exp.exportPicture(pic);
//    QCOMPARE(exp.cursor()->charFormat().toImageFormat().height(), 81.0);
}

QTEST_MAIN(TestQTextDocumentExporter);
