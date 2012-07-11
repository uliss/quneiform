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

#include <fstream>

#include "testqtimageexporter.h"
#include "export/imageexporter.h"
#include "export/qtimageexporter.h"
#include "rdib/qtimageloader.h"
#include "common/imagerawdata.h"
#include "common/imageurl.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestQtImageExporter);

using namespace cf;

void TestQtImageExporter::testSave()
{
    std::fstream os;

    ImageExporterPtr exp(new QtImageExporter(FORMAT_PNG));
    CPPUNIT_ASSERT_EQUAL(exp->format(), FORMAT_PNG);

    ImageRawData img;
    // bad image data
    CPPUNIT_ASSERT_THROW(exp->save(img, os), ImageExporter::Exception);

#define URL(fname) ImageURL(EXPORTER_TEST_IMAGE_DIR + std::string(fname))

    QtImageLoader loader;
    ImagePtr image = loader.load(URL("test_in.bmp"));

    exp->setFormat(FORMAT_PNG);
    exp->save(*image, "export_qt.png");

    exp->setFormat(FORMAT_JPEG);
    exp->save(*image, "export_qt.jpg");
}

void TestQtImageExporter::testSupportedFormats()
{
    QtImageExporter exp;

    CPPUNIT_ASSERT(exp.isSupportedFormat(FORMAT_PNG));
    CPPUNIT_ASSERT(exp.isSupportedFormat(FORMAT_JPEG));
}
