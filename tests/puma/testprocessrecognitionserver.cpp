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
#include "common/helper.h"
#include "common/log.h"
#include "common/recognitionstate.h"
#include "common/percentcounter.h"
#include "rdib/imageloaderfactory.h"
#include "export/textexporter.h"
#include "../assert_recognize.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestProcessRecognitionServer);

using namespace cf;

#ifndef TEST_IMG_PATH
#define TEST_IMG_PATH ""
#endif

#define URL(fname) ImageURL(TEST_IMG_PATH fname)
#define TRACE() cfInfo() << METHOD_SIGNATURE()

void TestProcessRecognitionServer::testRecognize()
{
    ProcessRecognitionServer server;
    ImagePtr img;
    BinarizeOptions bopts;
    RecognizeOptions ropts;
    FormatOptions fopts;
    fopts.writeBom(false);

    // null image
    CPPUNIT_ASSERT_THROW(server.recognizeImage(img, bopts, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    img.reset(new Image());

    // empty image
    CPPUNIT_ASSERT_THROW(server.recognizeImage(img, bopts, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    // non-existant image via filename
    img->setFileName("/non-exists.png");
    CPPUNIT_ASSERT_THROW(server.recognizeImage(img, bopts, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    // normal image via filename
    CEDPagePtr page = server.recognizeImage(URL("/english.png"), bopts, ropts, fopts);
    CPPUNIT_ASSERT(page.get());
    CPPUNIT_ASSERT(!page->empty());

    TextExporter exp(page, fopts);
    std::ostringstream buf;
    exp.exportTo(buf);
    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(buf.str()), std::string("ENGLISH"));
    buf.str("");

    // normal image via shared memory
    img = ImageLoaderFactory::instance().load(URL("/english.png"));

    page = server.recognizeImage(img, bopts, ropts, fopts);
    CPPUNIT_ASSERT(page.get());
    CPPUNIT_ASSERT(!page->empty());
    exp.exportTo(buf);
    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(buf.str()), std::string("ENGLISH"));
    buf.str("");
}

void TestProcessRecognitionServer::testRecognizeRotated()
{
    BinarizeOptions bopts;
    RecognizeOptions ropts;
    FormatOptions fopts;

    ropts.setTurnAngle(RecognizeOptions::ANGLE_270);
    ASSERT_PROCESS_RECOGNIZE_RESULT(TEST_IMG_PATH "/english_rotated_90.png", bopts, ropts, fopts, "ENGLISH");

    ropts.setTurnAngle(RecognizeOptions::ANGLE_180);
    ASSERT_PROCESS_RECOGNIZE_RESULT(TEST_IMG_PATH "/english_rotated_180.png", bopts, ropts, fopts, "ENGLISH");

    ropts.setTurnAngle(RecognizeOptions::ANGLE_90);
    ASSERT_PROCESS_RECOGNIZE_RESULT(TEST_IMG_PATH "/english_rotated_270.png", bopts, ropts, fopts, "ENGLISH");
}

void TestProcessRecognitionServer::testRecognizeArea()
{
    BinarizeOptions bopts;
    RecognizeOptions ropts;
    FormatOptions fopts;

    ASSERT_PROCESS_RECOGNIZE_RESULT(TEST_IMG_PATH "/english.png", bopts, ropts, fopts, "ENGLISH");

    ropts.addReadRect(Rect(0, 0, 90, 80));
    ASSERT_PROCESS_RECOGNIZE_RESULT(TEST_IMG_PATH "/english.png", bopts, ropts, fopts, "EN");

    ropts.clearReadRects();
    ropts.addReadRect(Rect(190, -5, 100, 100));
    ASSERT_PROCESS_RECOGNIZE_RESULT(TEST_IMG_PATH "/english.png", bopts, ropts, fopts, "SH");
}

void TestProcessRecognitionServer::testOpen()
{
    TRACE();

    PercentCounter counter;
    RecognitionState state;
    ProcessRecognitionServer s;
    s.setCounter(&counter);
    s.setStateTracker(&state);

    CPPUNIT_ASSERT(!s.open(ImageURL()));
    CPPUNIT_ASSERT(counter.get() == 0);
    CPPUNIT_ASSERT_EQUAL(RecognitionState::FAILED, state.get());

    CPPUNIT_ASSERT(!s.open(ImageURL("sdfds", 2)));
    CPPUNIT_ASSERT(counter.get() == 0);
    CPPUNIT_ASSERT_EQUAL(RecognitionState::FAILED, state.get());

    CPPUNIT_ASSERT(!s.open(ImagePtr()));
    CPPUNIT_ASSERT(counter.get() == 0);
    CPPUNIT_ASSERT_EQUAL(RecognitionState::FAILED, state.get());

    CPPUNIT_ASSERT(s.open(ImageURL(TEST_IMG_PATH "/english.png")));
    CPPUNIT_ASSERT_EQUAL(10, (int) counter.get());
//    CPPUNIT_ASSERT_EQUAL(RecognitionState::OPENED, state.get());
}
