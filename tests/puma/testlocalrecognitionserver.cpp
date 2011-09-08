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

#include <sstream>
#include <boost/algorithm/string/trim.hpp>

#include "testlocalrecognitionserver.h"

#include "common/recognizeoptions.h"
#include "common/formatoptions.h"
#include "puma/localrecognitionserver.h"
#include "rdib/imageloaderfactory.h"
#include "ced/cedpage.h"
#include "export/textexporter.h"
#include "export/debugexporter.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestLocalRecognitionServer);

#ifndef TEST_IMG_PATH
#define TEST_IMG_PATH ""
#endif

using namespace cf;

void TestLocalRecognitionServer::testRecognize()
{
    LocalRecognitionServer server;
    ImagePtr img;
    RecognizeOptions ropts;
    FormatOptions fopts;
    fopts.writeBom(false);

    // null image
    CPPUNIT_ASSERT_THROW(server.recognize(img, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    img.reset(new Image());

    // invalid image
    CPPUNIT_ASSERT_THROW(server.recognize(img, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    // normal image
    img = ImageLoaderFactory::instance().load(TEST_IMG_PATH "/english.png");

    CEDPagePtr p = server.recognize(img, ropts, fopts);
    CPPUNIT_ASSERT(p.get());
    CPPUNIT_ASSERT(!p->empty());

    TextExporter exp(p, fopts);
    std::ostringstream buf;
    exp.exportTo(buf);
    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(buf.str()), std::string("ENGLISH"));
    buf.str("");

    // debug off
    DebugExporter dexp(fopts);
    dexp.exportTo(buf);
    CPPUNIT_ASSERT(boost::algorithm::trim_copy(buf.str()).empty());
    buf.str("");

    // debug on
    server.setTextDebug(true);
    server.recognize(img, ropts, fopts);
    dexp.exportTo(buf);
    CPPUNIT_ASSERT(!boost::algorithm::trim_copy(buf.str()).empty());
    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(buf.str()), std::string("ENGLISH"));
}
