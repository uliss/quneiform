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

#include <vector>
#include <stdexcept>
#include <string>
#include <boost/noncopyable.hpp>

#include "globus.h"
#include "common/image.h"
#include "common/binarizeoptions.h"
#include "common/formatoptions.h"
#include "common/recognizeoptions.h"
#include "ced/cedpageptr.h"
#include "layoutblock.h"

namespace cf
{

class PercentCounter;
class RecognitionState;
class ImageURL;

class CLA_EXPO AbstractRecognitionServer : private boost::noncopyable
{
public:
    AbstractRecognitionServer();
    virtual ~AbstractRecognitionServer();

    /**
     * Adds manual layout image block
     */
    virtual void addImageBlock(const Rect& r) = 0;

    /**
     * Adds manual layout table block
     */
    virtual void addTableBlock(const Rect& r) = 0;

    /**
     * Adds manual layout text block
     */
    virtual void addTextBlock(const Rect& r) = 0;

    PercentCounter * counter();
    void setCounter(PercentCounter * counter);

    /**
     * Returns true if using textdebug output format
     * it not require formatting tree im memory
     */
    bool isTextDebug() const;
    void setTextDebug(bool value);

    /**
     * Returns binarization options
     * @see setBinarizeOptions()
     */
    BinarizeOptions binarizeOptions() const;

    /**
     * Sets binarize options
     * @param bopt - binarization options
     * @see binarizeOptions()
     */
    void setBinarizeOptions(const BinarizeOptions& bopts);

    /**
     * Returns format options
     * @see setFormatOptions()
     */
    FormatOptions formatOptions() const;

    /**
     * Sets format options
     * @param fopts - formatting options
     * @see formatOptions()
     */
    void setFormatOptions(const FormatOptions& fopts);

    /**
     * Returns recognition options
     * @see setRecognizeOptions()
     */
    RecognizeOptions recognizeOptions() const;

    /**
     * Sets recognize options
     * @param ropts - recognition options
     * @see recognizeOptions()
     */
    void setRecognizeOptions(const RecognizeOptions& ropts);

    /**
     * Analyzes document layout
     * @return true on success
     */
    virtual bool analyze() = 0;

    /**
     * Binarizes image
     * @param bopts - binarization options
     * @return true on success
     */
    virtual bool binarize() = 0;

    /**
     * Formats recognition result
     * @return pointer to CEDPage
     */
    virtual CEDPagePtr format() = 0;

    /**
     * Makes document manual layout
     */
    virtual bool manualLayout() = 0;

    /**
     * Opens image
     * @param url - image url
     * @return true on success
     */
    virtual bool open(const ImageURL& url) = 0;

    /**
     * Opens image from memory
     * @param img - pointer to image
     * @return true onsuccess
     */
    virtual bool open(ImagePtr img) = 0;

    /**
     * Recognizes image
     * @returns true on success
     */
    virtual bool recognize() = 0;

    /**
      * Recognizes image by url
      * @param url - image url
      * @param bopts - binarization options
      * @param ropts - recognition options
      * @param fopts - format options
      * @return pointer to CEDPage
      */
    virtual CEDPagePtr recognizeImage(const ImageURL& url,
                                 const BinarizeOptions& bopts,
                                 const RecognizeOptions& ropts,
                                 const FormatOptions& fopts);

    /**
      * Recognizes given image
      * @param image - source image
      * @param bopts - binarization options
      * @param ropts - recognition options
      * @param fopts - format options
      * @return pointer to CEDPage
      */
    virtual CEDPagePtr recognizeImage(ImagePtr image,
                                 const BinarizeOptions& bopts,
                                 const RecognizeOptions& ropts,
                                 const FormatOptions& fopts);

    void setStateTracker(RecognitionState * state);

    /**
     * Returns list of iamge blocks after page layout
     * @see textBlocks()
     */
    virtual LayoutBlockList imageBlocks() const;

    /**
     * Returns array of table blocks after page layout
     * @see imageBlocks()
     */
    virtual LayoutBlockList tableBlocks() const;

    /**
     * Returns array of text blocks after page layout
     * @see imageBlocks()
     */
    virtual LayoutBlockList textBlocks() const;
public:
    enum reason_t {
        OK = 0,
        UNKNOWN = 1,
        FILE_NOT_FOUND,
        IMAGE_OPEN_ERROR,
        BINARIZATION_ERROR,
        LAYOUT_ERROR,
        RECOGNITION_ERROR,
        SHMEM_ERROR,
        WORKER_NOT_FOUND,
        WORKER_CRASH,
        WORKER_TIMEOUT
    };

    struct RecognitionException : public std::runtime_error {
        RecognitionException(const std::string& msg, reason_t r = UNKNOWN) :
            std::runtime_error(msg),
            reason_(r) {}
        reason_t reason() const { return reason_; }
    private:
        reason_t reason_;
    };
protected:
    void counterAdd(int value);
    void counterReset();
    void stateSet(int value);
    void stateReset();
private:
    CEDPagePtr recognizeImagePrivate();
private:
    BinarizeOptions bin_options_;
    FormatOptions fmt_options_;
    RecognizeOptions rec_options_;
    PercentCounter * counter_;
    RecognitionState * state_;
    bool text_debug_;
};

}

#endif // ABSTRACTRECOGNITIONSERVER_H
