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

#include "common/imageurl.h"
#include "common/log.h"
#include "common/helper.h"
#include "common/recognitionstate.h"
#include "common/percentcounter.h"
#include "puma/localrecognitionserver.h"
#include "rdib/imageloaderfactory.h"
#include "ced/cedpage.h"
#include "export/textexporter.h"
#include "export/debugexporter.h"
#include "../assert_recognize.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestLocalRecognitionServer);

#ifndef TEST_IMG_PATH
#define TEST_IMG_PATH ""
#endif

using namespace cf;

#define URL(fname) ImageURL(TEST_IMG_PATH fname)

#define TRACE() cfInfo() << METHOD_SIGNATURE()

void TestLocalRecognitionServer::testRecognizeImage()
{
    TRACE();

    LocalRecognitionServer server;
    ImagePtr img;
    BinarizeOptions bopts;
    RecognizeOptions ropts;
    FormatOptions fopts;
    fopts.writeBom(false);

    // null image
    CPPUNIT_ASSERT_THROW(server.recognizeImage(img, bopts, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    img.reset(new Image());

    // invalid image
    CPPUNIT_ASSERT_THROW(server.recognizeImage(img, bopts, ropts, fopts),
                         AbstractRecognitionServer::RecognitionException);

    // normal image
    img = ImageLoaderFactory::instance().load(URL("/english.png"));

    CEDPagePtr p = server.recognizeImage(img, bopts, ropts, fopts);
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
    server.recognizeImage(img, bopts, ropts, fopts);
    dexp.exportTo(buf);
    CPPUNIT_ASSERT(!boost::algorithm::trim_copy(buf.str()).empty());
    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(buf.str()), std::string("ENGLISH"));
}

void TestLocalRecognitionServer::testRecognizeRotated()
{
    TRACE();

    BinarizeOptions bopts;
    RecognizeOptions ropts;
    FormatOptions fopts;

    ropts.setTurnAngle(RecognizeOptions::ANGLE_270);
    ASSERT_LOCAL_RECOGNIZE_RESULT(TEST_IMG_PATH "/english_rotated_90.png", bopts, ropts, fopts, "ENGLISH");

    ropts.setTurnAngle(RecognizeOptions::ANGLE_180);
    ASSERT_LOCAL_RECOGNIZE_RESULT(TEST_IMG_PATH "/english_rotated_180.png", bopts, ropts, fopts, "ENGLISH");

    ropts.setTurnAngle(RecognizeOptions::ANGLE_90);
    ASSERT_LOCAL_RECOGNIZE_RESULT(TEST_IMG_PATH "/english_rotated_270.png", bopts, ropts, fopts, "ENGLISH");
}

void TestLocalRecognitionServer::testRecognizeArea()
{
    TRACE();

    BinarizeOptions bopts;
    RecognizeOptions ropts;
    FormatOptions fopts;

    ASSERT_LOCAL_RECOGNIZE_RESULT(TEST_IMG_PATH "/english.png", bopts, ropts, fopts, "ENGLISH");

    ropts.addReadRect(Rect(0, 0, 90, 80));
    ASSERT_LOCAL_RECOGNIZE_RESULT(TEST_IMG_PATH "/english.png", bopts, ropts, fopts, "EN");

    ropts.clearReadRects();
    ropts.addReadRect(Rect(190, -5, 100, 100));
    ASSERT_LOCAL_RECOGNIZE_RESULT(TEST_IMG_PATH "/english.png", bopts, ropts, fopts, "SH");

}

void TestLocalRecognitionServer::testOpen()
{
    TRACE();

    PercentCounter counter;
    RecognitionState state;
    LocalRecognitionServer s;
    s.setCounter(&counter);
    s.setStateTracker(&state);

    CPPUNIT_ASSERT(!s.open(ImageURL("")));
    CPPUNIT_ASSERT(counter.get() == 0);
    CPPUNIT_ASSERT_EQUAL(RecognitionState::FAILED, state.get());

    CPPUNIT_ASSERT(!s.open(ImagePtr()));
    CPPUNIT_ASSERT(counter.get() == 0);
    CPPUNIT_ASSERT_EQUAL(RecognitionState::FAILED, state.get());

    CPPUNIT_ASSERT(s.open(ImageURL(TEST_IMG_PATH "/english.png")));
    CPPUNIT_ASSERT_EQUAL(10, (int) counter.get());
    CPPUNIT_ASSERT_EQUAL(RecognitionState::OPENED, state.get());
}

void TestLocalRecognitionServer::testBinarize()
{
    TRACE();

    PercentCounter counter;
    RecognitionState state;
    LocalRecognitionServer s;
    s.setCounter(&counter);
    s.setStateTracker(&state);

    CPPUNIT_ASSERT(!s.binarize());
    CPPUNIT_ASSERT(counter.get() == 0);
    CPPUNIT_ASSERT_EQUAL(RecognitionState::NONE, state.get());

    CPPUNIT_ASSERT(s.open(ImageURL(TEST_IMG_PATH "/english.png")));
    CPPUNIT_ASSERT(s.binarize());
    CPPUNIT_ASSERT_EQUAL(20, (int) counter.get());
    CPPUNIT_ASSERT_EQUAL(RecognitionState::BINARIZED, state.get());
}

void TestLocalRecognitionServer::testAnalyze()
{
    TRACE();

    PercentCounter counter;
    RecognitionState state;
    LocalRecognitionServer s;
    s.setCounter(&counter);
    s.setStateTracker(&state);

    CPPUNIT_ASSERT(!s.analyze());
    CPPUNIT_ASSERT(counter.get() == 0);
    CPPUNIT_ASSERT_EQUAL(RecognitionState::NONE, state.get());

    CPPUNIT_ASSERT(s.open(ImageURL(TEST_IMG_PATH "/english.png")));
    CPPUNIT_ASSERT(!s.analyze());
    CPPUNIT_ASSERT(s.binarize());
    CPPUNIT_ASSERT(s.analyze());
    CPPUNIT_ASSERT_EQUAL(30, (int) counter.get());
    CPPUNIT_ASSERT_EQUAL(RecognitionState::ANALYZED, state.get());
}

void TestLocalRecognitionServer::testRecognize()
{
    TRACE();

    PercentCounter counter;
    RecognitionState state;
    LocalRecognitionServer s;
    s.setCounter(&counter);
    s.setStateTracker(&state);

    CPPUNIT_ASSERT(!s.recognize());
    CPPUNIT_ASSERT(counter.get() == 0);
    CPPUNIT_ASSERT_EQUAL(RecognitionState::NONE, state.get());

    CPPUNIT_ASSERT(s.open(ImageURL(TEST_IMG_PATH "/english.png")));
    CPPUNIT_ASSERT(!s.recognize());
    CPPUNIT_ASSERT(s.binarize());
    CPPUNIT_ASSERT(s.analyze());
    CPPUNIT_ASSERT(s.recognize());
    CPPUNIT_ASSERT_EQUAL(90, (int) counter.get());
    CPPUNIT_ASSERT_EQUAL(RecognitionState::RECOGNIZED, state.get());
}

void TestLocalRecognitionServer::testFormat()
{
    TRACE();

    PercentCounter counter;
    RecognitionState state;
    LocalRecognitionServer s;
    s.setCounter(&counter);
    s.setStateTracker(&state);

    CPPUNIT_ASSERT(!s.format());
    CPPUNIT_ASSERT(counter.get() == 0);
    CPPUNIT_ASSERT_EQUAL(RecognitionState::NONE, state.get());

    CPPUNIT_ASSERT(s.open(ImageURL(TEST_IMG_PATH "/english.png")));
    CPPUNIT_ASSERT(!s.format());
    CPPUNIT_ASSERT(s.binarize());
    CPPUNIT_ASSERT(s.analyze());
    CPPUNIT_ASSERT(s.recognize());
    CPPUNIT_ASSERT(s.format());
    CPPUNIT_ASSERT_EQUAL(100, (int) counter.get());
    CPPUNIT_ASSERT_EQUAL(RecognitionState::FORMATTED, state.get());
}
