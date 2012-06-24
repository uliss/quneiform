/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#include <fstream>

#include "testmagickimageexporter.h"
#include "export/magickimageexporter.h"
#include "rdib/bmpimageloader.h"
#include "rdib/magickimageloader.h"
#include "common/imageurl.h"

using namespace cf;
CPPUNIT_TEST_SUITE_REGISTRATION(TestMagickImageExporter);

void TestMagickImageExporter::testSave() {
    std::fstream os;

    ImageExporterPtr exp(new MagickImageExporter);

    ImageRawData img;
    // bad image data
    CPPUNIT_ASSERT_THROW(exp->save(img, os), ImageExporter::Exception);

    // bad stream
    os << 1;
    uchar data[1000];
    img.set(data, 1000, ImageRawData::AllocatorNone);
    CPPUNIT_ASSERT_THROW(exp->save(img, os), ImageExporter::Exception);

    os.clear();
    data[0] = 'B';
    data[1] = 'M';
    CPPUNIT_ASSERT_THROW(exp->save(img, os), ImageExporter::Exception);

#define URL(fname) ImageURL(EXPORTER_TEST_IMAGE_DIR + std::string(fname))

    MagickImageLoader loader;
    ImagePtr image = loader.load(URL("test_in.bmp"));

    exp->save(*image, "export_magick.png");

    exp->setFormat(FORMAT_JPEG);
    exp->save(*image, "export_magick.jpg");

    exp->setFormat(FORMAT_GIF);
    exp->save(*image, "export_magick.gif");
}

