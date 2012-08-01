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

#include "abstractrecognitionserver.h"
#include "common/imageurl.h"
#include "common/stringbuffer.h"
#include "common/helper.h"
#include "common/percentcounter.h"
#include "common/recognitionstate.h"

namespace cf
{

#define EXCEPT_BUFFER() StringBuffer() << METHOD_SIGNATURE()

AbstractRecognitionServer::AbstractRecognitionServer() :
    counter_(NULL), state_(NULL), text_debug_(false) {}

AbstractRecognitionServer::~AbstractRecognitionServer()
{}

PercentCounter * AbstractRecognitionServer::counter()
{
    return counter_;
}

bool AbstractRecognitionServer::isTextDebug() const
{
    return text_debug_;
}

BinarizeOptions AbstractRecognitionServer::binarizeOptions() const
{
    return bin_options_;
}

void AbstractRecognitionServer::setBinarizeOptions(const BinarizeOptions& bopts)
{
    bin_options_ = bopts;
}

FormatOptions AbstractRecognitionServer::formatOptions() const
{
    return fmt_options_;
}

void AbstractRecognitionServer::setFormatOptions(const FormatOptions& fopts)
{
    fmt_options_ = fopts;
}

RecognizeOptions AbstractRecognitionServer::recognizeOptions() const
{
    return rec_options_;
}

void AbstractRecognitionServer::setRecognizeOptions(const RecognizeOptions& ropts)
{
    rec_options_ = ropts;
}

CEDPagePtr AbstractRecognitionServer::recognizeImagePrivate()
{
    if(!binarize())
        throw RecognitionException(EXCEPT_BUFFER() << "binarization error", BINARIZATION_ERROR);
    if(!analyze())
        throw RecognitionException(EXCEPT_BUFFER() << "layout error", LAYOUT_ERROR);
    if(!recognize())
        throw RecognitionException(EXCEPT_BUFFER() << "recognition buffer", RECOGNITION_ERROR);

    return format();
}

void AbstractRecognitionServer::stateReset()
{
    if(state_)
        state_->reset();
}

CEDPagePtr AbstractRecognitionServer::recognizeImage(const ImageURL& url,
                                                const BinarizeOptions& bopts,
                                                const RecognizeOptions& ropts,
                                                const FormatOptions& fopts)
{
    if(url.empty())
        throw RecognitionException(EXCEPT_BUFFER() << "empty image url", FILE_NOT_FOUND);

    if(!url.exists())
        throw RecognitionException(EXCEPT_BUFFER() << "image not exists", FILE_NOT_FOUND);

    setBinarizeOptions(bopts);
    setRecognizeOptions(ropts);
    setFormatOptions(fopts);

    if(!open(url))
        throw RecognitionException(EXCEPT_BUFFER() << "open error", IMAGE_OPEN_ERROR);

    return recognizeImagePrivate();
}

CEDPagePtr AbstractRecognitionServer::recognizeImage(ImagePtr image,
                                                const BinarizeOptions& bopts,
                                                const RecognizeOptions& ropts,
                                                const FormatOptions& fopts)
{
    if(!image)
        throw RecognitionException(EXCEPT_BUFFER() << "Null image pointer", FILE_NOT_FOUND);

    if(image->dataSize() == 0)
        throw RecognitionException(EXCEPT_BUFFER() << "empty image given", IMAGE_OPEN_ERROR);

    setBinarizeOptions(bopts);
    setRecognizeOptions(ropts);
    setFormatOptions(fopts);

    if(!open(image))
        throw RecognitionException(EXCEPT_BUFFER() << "open error", IMAGE_OPEN_ERROR);

    return recognizeImagePrivate();
}

void AbstractRecognitionServer::setCounter(PercentCounter * counter)
{
    counter_ = counter;
}

void AbstractRecognitionServer::setStateTracker(RecognitionState * state)
{
    state_ = state;
}

void AbstractRecognitionServer::counterAdd(int value)
{
    if(counter_)
        counter_->add(value);
}

void AbstractRecognitionServer::stateSet(int value)
{
    if(state_)
        state_->set(static_cast<RecognitionState::state_t>(value));
}

void AbstractRecognitionServer::counterReset()
{
    if(counter_)
        counter_->reset();
}

void AbstractRecognitionServer::setTextDebug(bool value)
{
    text_debug_ = value;
}

}

