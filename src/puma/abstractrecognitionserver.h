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

#ifndef ABSTRACTRECOGNITIONSERVER_H
#define ABSTRACTRECOGNITIONSERVER_H

#include <stdexcept>
#include <string>
#include <boost/noncopyable.hpp>

#include "globus.h"
#include "common/image.h"
#include "ced/cedpageptr.h"

namespace cf
{

class FormatOptions;
class RecognizeOptions;
class PercentCounter;
class RecognitionState;

class CLA_EXPO AbstractRecognitionServer : private boost::noncopyable
{
public:
    AbstractRecognitionServer() : counter_(NULL), state_(NULL), text_debug_(false) {}
    virtual ~AbstractRecognitionServer() {}
    PercentCounter * counter() { return counter_; }
    bool isTextDebug() const { return text_debug_; }

    /**
      * Recognizes image by path
      * @param imagePath - path to image file
      * @param ropts - recognition options
      * @param fopts - format options
      * @return pointer to CEDPage
      */
    virtual CEDPagePtr recognize(const std::string& imagePath,
                                 const RecognizeOptions& ropts,
                                 const FormatOptions& fopts) = 0;

    /**
      * Recognizes given image
      * @param image - source image
      * @param ropts - recognition options
      * @param fopts - format options
      * @return pointer to CEDPage
      */
    virtual CEDPagePtr recognize(ImagePtr image,
                                 const RecognizeOptions& ropts,
                                 const FormatOptions& fopts) = 0;

    void setCounter(PercentCounter * counter) { counter_ = counter; }
    void setStateTracker(RecognitionState * state) { state_ = state; }
    void setTextDebug(bool value) { text_debug_ = value; }
public:
    struct RecognitionException : public std::runtime_error {
        RecognitionException(const std::string& msg) : std::runtime_error(msg) {}
    };
protected:
    PercentCounter * counter_;
    RecognitionState * state_;
private:
    bool text_debug_;
};

}

#endif // ABSTRACTRECOGNITIONSERVER_H
