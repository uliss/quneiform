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

#include <QImageReader>
#include <QTest>
#include <QBuffer>
#include <QDebug>

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR
#endif

#include "testmultitiffplugin.h"

void TestMultiTIFFPlugin::testSupportedFormats()
{
    QVERIFY(QImageReader::supportedImageFormats().contains("mtiff"));
}

void TestMultiTIFFPlugin::testRead()
{
    QImage img;
    QByteArray data;
    data.fill('1', 1024);
    QVERIFY(!img.loadFromData(data, "mtiff"));

    QImage tiff(100, 120, QImage::Format_RGB32);
    tiff.fill(Qt::blue);

    QByteArray tiff_data;
    QBuffer buffer(&tiff_data);
    tiff.save(&buffer, "tiff");

    const char * tiff_ptr = tiff_data.constData();
    QVERIFY(tiff_ptr);

    QVERIFY(img.loadFromData((uchar*) tiff_ptr, tiff_data.length(), "MTIFF"));

    QImageReader r(CF_IMAGE_DIR "/multipage.tif", "MTIFF");
    QVERIFY(r.canRead());

    QImage im1;
    r.read(&im1);
    r.jumpToNextImage();
    QImage im2;
    r.read(&im2);
    im1.save("test_multipage_tiff_1.png", "PNG");
    im2.save("test_multipage_tiff_2.png", "PNG");
}

Q_IMPORT_PLUGIN(multitiff_imageplugin)

QTEST_MAIN(TestMultiTIFFPlugin)
