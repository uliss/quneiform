/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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
#include <QtPlugin>
#include <QImageReader>
#include <QLabel>

#include "testpdfplugin.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR
#endif

void TestPDFPlugin::testSupportedFormats()
{
    QVERIFY(QImageReader::supportedImageFormats().contains("pdf"));
}

void TestPDFPlugin::testRead()
{
    QImageReader r(CF_IMAGE_DIR "/quneiform.pdf", "PDF");
    QVERIFY(r.canRead());

    QImage page1;
    QVERIFY(r.read(&page1));
    QCOMPARE(page1.size(), QSize(281, 81));

    page1.save("test_pdfplugin_page1.png", "PNG");
    r.jumpToNextImage();

    QImage page2;
    QVERIFY(r.read(&page2));
    QCOMPARE(page1.size(), QSize(281, 81));
    page2.save("test_pdfplugin_page2.png", "PNG");
}

Q_IMPORT_PLUGIN(pdf_imageplugin)

QTEST_MAIN(TestPDFPlugin)
