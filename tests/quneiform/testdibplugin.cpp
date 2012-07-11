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

#include <QtTest>
#include <QDebug>
#include <QImageReader>

#include "testdibplugin.h"
#include "qtplugins/dibimageioplugin.h"
#include "cfcompat.h"
#include "common/bmp.h"

void TestDIBPlugin::testSupportedFormats()
{
    QVERIFY(QImageReader::supportedImageFormats().contains("dib"));
}

void TestDIBPlugin::testRead()
{
    QImage img;
    QByteArray data;
    data.fill('1', 1024);
    QVERIFY(!img.loadFromData(data, "dib"));

    QImage bmp(100, 120, QImage::Format_RGB32);
    bmp.fill(Qt::blue);

    QByteArray bmp_data;
    QBuffer buffer(&bmp_data);
    bmp.save(&buffer, "bmp");

    const char * dib_ptr = bmp_data.constData();
    QVERIFY(dib_ptr);

    dib_ptr += cf::BMP_FILE_HEADER_SIZE;

    QVERIFY(img.loadFromData((uchar*) dib_ptr, bmp_data.length() - sizeof(cf::BMP_FILE_HEADER_SIZE), "DIB"));

    img.save("test_dib_plugin.bmp", "bmp");
}

Q_IMPORT_PLUGIN(dib_imageplugin)

QTEST_MAIN(TestDIBPlugin)
