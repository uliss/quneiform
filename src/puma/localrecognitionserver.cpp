/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <iostream>

#include "ced/cedpage.h"
#include "rdib/imageloaderfactory.h"
#include "localrecognitionserver.h"
#include "puma.h"
#include "common/percentcounter.h"
#include "common/recognitionstate.h"
#include "common/imageurl.h"
#include "puma_debug.h"

namespace cf {

// counter values sum should be 100
enum {
    COUNTER_OPEN_VALUE = 10,
    COUNTER_BINARIZE_VALUE = 10,
    COUNTER_ANALYZE_VALUE = 10,
    COUNTER_RECOGNIZE_VALUE = 60,
    COUNTER_FORMAT_VALUE = 10
};

LocalRecognitionServer::LocalRecognitionServer() :
    local_state_(RecognitionState::NONE)
{
}

LocalRecognitionServer::~LocalRecognitionServer()
{
    if(isTextDebug())
        Puma::instance().close();
}

void LocalRecognitionServer::addImageBlock(const Rect& r)
{
    PUMA_TRACE_FUNC() << r;
    Puma::instance().addImageBlock(r);
}

void LocalRecognitionServer::addTableBlock(const Rect &r)
{
    PUMA_TRACE_FUNC() << r;
    Puma::instance().addTableBlock(r);
}

void LocalRecognitionServer::addTextBlock(const Rect& r)
{
    PUMA_TRACE_FUNC() << r;
    Puma::instance().addTextBlock(r);
}

bool LocalRecognitionServer::binarize()
{
    PUMA_TRACE_FUNC();

    if(!isOpened()) {
        PUMA_ERROR_FUNC() << "image not opened";
        return false;
    }

    Puma::instance().binarizeImage();
    setTotalState(RecognitionState::BINARIZED);
    counterAdd(COUNTER_BINARIZE_VALUE);
    return true;
}

bool LocalRecognitionServer::analyze()
{
    PUMA_TRACE_FUNC();

    if(!isBinarized()) {
        PUMA_ERROR_FUNC() << "image not binarized";
        return false;
    }

    Puma::instance().layout();
    setTotalState(RecognitionState::ANALYZED);
    counterAdd(COUNTER_ANALYZE_VALUE);
    return true;
}

bool LocalRecognitionServer::open(const ImageURL& url)
{
    if(url.empty()) {
        PUMA_ERROR_FUNC() << "empty url";
        setFailed();
        return false;
    }

    PUMA_TRACE_FUNC() << "url open:" << url;
    return open(ImageLoaderFactory::instance().load(url));
}

void LocalRecognitionServer::close()
{
    PUMA_TRACE_FUNC();

    // for normal formatting - we have to close puma library,
    // except textdebug
    if(!isTextDebug() && !recognizeOptions().debugCleanupDelayed())
        Puma::instance().close();
}

CEDPagePtr LocalRecognitionServer::format()
{
    PUMA_TRACE_FUNC();

    if(!isRecognized()) {
        PUMA_ERROR_FUNC() << "image not recognized";
        return CEDPagePtr();
    }

    if(!isTextDebug())
        Puma::instance().formatResult();

    counterAdd(COUNTER_FORMAT_VALUE);
    setTotalState(RecognitionState::FORMATTED);

    CEDPagePtr res =  Puma::instance().cedPage();
    if(res)
        res->setImageName(image_->fileName());

    close();

    return res;
}

bool LocalRecognitionServer::manualLayout()
{
    PUMA_TRACE_FUNC();

    if(!isBinarized()) {
        PUMA_ERROR_FUNC() << "image not binarized";
        return false;
    }

    Puma::instance().prepare();
    setTotalState(RecognitionState::ANALYZED);
    counterAdd(COUNTER_ANALYZE_VALUE);
    return true;
}

void LocalRecognitionServer::handleExceptionCommon(std::exception& e)
{
    PUMA_ERROR_FUNC() << e.what();

    if(!recognizeOptions().debugCleanupDelayed())
        Puma::instance().close();

    setFailed();
}

bool LocalRecognitionServer::open(ImagePtr image)
{
    PUMA_TRACE_FUNC();

    if(!image) {
        PUMA_ERROR_FUNC() << "null image pointer given";
        setFailed();
        return false;
    }

    stateReset();
    counterReset();
    setOptions();

    try {
        Puma::instance().open(image);
        image_ = image;

        counterAdd(COUNTER_OPEN_VALUE);
        setTotalState(RecognitionState::OPENED);
    }
    catch(PumaException& e) {
        PUMA_ERROR_FUNC() << e.what();
        setFailed();
        return false;
    }

    return true;
}

bool LocalRecognitionServer::recognize()
{
    PUMA_TRACE_FUNC();

    if(!isAnalyzed()) {
        PUMA_ERROR_FUNC() << "image not analyzed";
        return CEDPagePtr();
    }

    try {
        Puma::instance().recognize();

        counterAdd(COUNTER_RECOGNIZE_VALUE);
        setTotalState(RecognitionState::RECOGNIZED);
        return true;
    }
    catch(RecognitionException& e) {
        handleExceptionCommon(e);
        throw e;
    }
    catch(PumaException& e) {
        handleExceptionCommon(e);
        throw RecognitionException(e.what(), RECOGNITION_ERROR);
    }
    catch (std::exception& e) {
        handleExceptionCommon(e);
        throw RecognitionException(e.what(), UNKNOWN);
    }
}

LayoutBlockList LocalRecognitionServer::imageBlocks() const
{
    return Puma::instance().imageBlocks();
}

LayoutBlockList LocalRecognitionServer::tableBlocks() const
{
    return Puma::instance().tableBlocks();
}

LayoutBlockList LocalRecognitionServer::textBlocks() const
{
    return Puma::instance().textBlocks();
}

void LocalRecognitionServer::saveLayout(const std::string& filename)
{
    Puma::instance().saveLayoutToFile(filename);
}

void LocalRecognitionServer::setOptions()
{
    Puma::instance().setBinarizeOptions(binarizeOptions());
    Puma::instance().setRecognizeOptions(recognizeOptions());
    Puma::instance().setFormatOptions(formatOptions());

    PUMA_TRACE_FUNC() << "\n"
//                      << binarizeOptions() << "\n"
                      << recognizeOptions() << "\n"
                      << formatOptions();
}

void LocalRecognitionServer::setTotalState(int state)
{
    local_state_ = state;
    stateSet(state);
}

void LocalRecognitionServer::setFailed()
{
    setTotalState(RecognitionState::FAILED);
    counterReset();
}

bool LocalRecognitionServer::isOpened() const
{
    return local_state_ == RecognitionState::OPENED;
}

bool LocalRecognitionServer::isBinarized() const
{
    return local_state_ == RecognitionState::BINARIZED;
}

bool LocalRecognitionServer::isAnalyzed() const
{
    return local_state_ == RecognitionState::ANALYZED;
}

bool LocalRecognitionServer::isRecognized() const
{
    return local_state_ == RecognitionState::RECOGNIZED;
}

}
