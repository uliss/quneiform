/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include <boost/algorithm/string/trim.hpp>

#include "testprocessrecognitionserver.h"
#include "ced/cedpage.h"
#include "puma/processrecognitionserver.h"
#include "common/recognizeoptions.h"
#include "common/formatoptions.h"
#include "rdib/imageloaderfactory.h"
#include "export/textexporter.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestProcessRecognitionServer);

using namespace cf;

#ifndef TEST_IMG_PATH
#define TEST_IMG_PATH ""
#endif

void TestProcessRecognitionServer::testRecognize()
{
    ProcessRecognitionServer server;
    ImagePtr img;
    RecognizeOptions ropts;
    FormatOptions fopts;
    fopts.writeBom(false);

    // null image
    CPPUNIT_ASSERT_THROW(server.recognize(img, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    img.reset(new Image());

    // empty image
    CPPUNIT_ASSERT_THROW(server.recognize(img, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    // non-existant image via filename
    img->setFileName("/non-exists.png");
    CPPUNIT_ASSERT_THROW(server.recognize(img, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    // normal image via filename
    img->setFileName(TEST_IMG_PATH "/english.png");

    CEDPagePtr page = server.recognize(img, ropts, fopts);
    CPPUNIT_ASSERT(page.get());
    CPPUNIT_ASSERT(!page->empty());

    TextExporter exp(page, fopts);
    std::ostringstream buf;
    exp.exportTo(buf);
    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(buf.str()), std::string("ENGLISH"));
    buf.str("");

    // normal image via shared memory
    img = ImageLoaderFactory::instance().load(TEST_IMG_PATH "/english.png");
    img->setFileName("");

    page = server.recognize(img, ropts, fopts);
    CPPUNIT_ASSERT(page.get());
    CPPUNIT_ASSERT(!page->empty());
    exp.exportTo(buf);
    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(buf.str()), std::string("ENGLISH"));
    buf.str("");
}
