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
#include "gui/cedpageexporter.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"

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
    cursor.insertFrame(fmt2);
    e.doExport(&doc, &page);

    QCOMPARE(page.sectionCount(), size_t(1));
    page.clear();
}

QTEST_MAIN(TestCEDPageExporter)
