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
#include "common/formatoptions.h"
#include "gui/qtextdocumentexporter.h"
#include "ced/cedchar.h"
#include "ced/cedpage.h"
#include "export/rout_own.h"

#include "testqtextdocumentexporter.h"

using namespace cf;

TestQTextDocumentExporter::TestQTextDocumentExporter(QObject * parent) :
    QObject(parent)
{
}

void TestQTextDocumentExporter::testConstruct() {
    QTextDocumentExporter exp(NULL, cf::FormatOptions());
    QVERIFY(exp.document().isEmpty());
    QVERIFY(!exp.document().isModified());
}

void TestQTextDocumentExporter::testWriteChar() {
    FormatOptions opts;
    QTextDocumentExporter exp(NULL, opts);

    CEDChar c1('t');
    exp.exportChar(c1);
    QVERIFY(!exp.document().isEmpty());
    QVERIFY(exp.document().isModified());
    QCOMPARE(exp.document().toPlainText(), QString("t"));

    CEDChar c2('e');
    exp.exportChar(c2);
    QCOMPARE(exp.document().toPlainText(), QString("te"));
}

void TestQTextDocumentExporter::testWriteCharBold() {
    FormatOptions opts;
    opts.useBold(false);
    QTextDocumentExporter exp(NULL, opts);
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
    opts.useUndelined(false);

    QTextDocumentExporter exp(NULL, opts);
    CEDChar ch('s');
    exp.exportChar(ch);
    QVERIFY(!exp.cursor().charFormat().fontUnderline());

    ch.setFontStyle(FONT_UNDERLINE & FONT_BOLD);
    exp.exportChar(ch);
    QVERIFY(!exp.cursor().charFormat().fontUnderline());

    exp.formatOptions().useUndelined(true);
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

    CEDChar ch1('a');
    exp.exportChar(ch1);
    QCOMPARE(exp.document().toPlainText(), QString("a"));
    QVERIFY(exp.cursor().charFormat().toolTip().isEmpty());

    ch1.addAlternative('b');
    exp.clear();
    exp.exportChar(ch1);
    QCOMPARE(exp.document().toPlainText(), QString("a"));
    QCOMPARE(exp.cursor().charFormat().toolTip(), QString("Alternatives:\n        'b' (254)"));

    CEDChar ch2('\xD4');
    ch2.addAlternative('\xF4');
    exp.clear();
    exp.exportChar(ch2);
    QCOMPARE(exp.document().toPlainText(), QString::fromUtf8("Ф"));
    QCOMPARE(exp.cursor().charFormat().toolTip(), QString::fromUtf8("Alternatives:\n        'ф' (254)"));
}

void TestQTextDocumentExporter::testWriteCharAlternatives() {
    FormatOptions opts;
    opts.setShowAlternatives(false);
    QTextDocumentExporter exp(NULL, opts);

    CEDChar ch1('a');
    exp.exportChar(ch1);
    QCOMPARE(exp.document().toPlainText(), QString("a"));
    QVERIFY(exp.cursor().charFormat().toolTip().isEmpty());

    exp.formatOptions().setShowAlternatives(true);
    exp.clear();
    exp.exportChar(ch1);
    QCOMPARE(exp.document().toPlainText(), QString("a"));
    QVERIFY(exp.cursor().charFormat().toolTip().isEmpty());

    ch1.addAlternative(Letter('b', 100));
    ch1.addAlternative(Letter('c', 50));

    exp.clear();
    exp.formatOptions().setShowAlternatives(false);
    exp.exportChar(ch1);
    QCOMPARE(exp.document().toPlainText(), QString("a"));
    QMap<QString, QVariant> alt_map;
    alt_map.insert("b", 100);
    alt_map.insert("c", 50);
    QCOMPARE(exp.cursor().charFormat().property(QTextDocumentExporter::ALTERNATIVES).toMap(), alt_map);

    exp.clear();
    exp.formatOptions().setShowAlternatives(true);
    exp.exportChar(ch1);
    QCOMPARE(exp.document().toPlainText(), QString("a"));
    QCOMPARE(exp.cursor().charFormat().toolTip(),
             QString("Alternatives:\n        'b' (100)\n        'c' (50)"));
}

QTEST_MAIN(TestQTextDocumentExporter);
