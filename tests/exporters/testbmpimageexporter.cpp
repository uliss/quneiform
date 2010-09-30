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

#include "testbmpimageexporter.h"
#include <fstream>
#include <common/imagerawdata.h>
#include <export/imageexporter.h>
#include <export/bmpimageexporter.h>
#include <rdib/bmpimageloader.h>
using namespace cf;
CPPUNIT_TEST_SUITE_REGISTRATION(TestBmpImageExporter);

void TestBmpImageExporter::testSave() {
    std::fstream os;

    ImageExporterPtr exp(new BmpImageExporter);

    // bad image data
    ImageRawData img;
    CPPUNIT_ASSERT_THROW(exp->save(img, os), ImageExporter::Exception);

    // bad stream
    char data[1000];
    img.set((uchar*) data, 1000, ImageRawData::AllocatorNone);
    CPPUNIT_ASSERT_THROW(exp->save(img, os), ImageExporter::Exception);

    BmpImageLoader loader;
    ImagePtr image = loader.load(EXPORTER_TEST_IMAGE_DIR + std::string("test_in.bmp"));

    exp->save(*image, "test_out.bmp");
}
