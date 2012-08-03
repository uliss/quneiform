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

#ifndef MEMORYDATA_H
#define MEMORYDATA_H

#include <cstddef>
#include <stdexcept>

#include "common/binarizeoptions.h"
#include "common/formatoptions.h"
#include "common/recognizeoptions.h"
#include "common/image.h"
#include "common/imageurl.h"
#include "common/exception.h"
#include "ced/cedpageptr.h"

namespace cf {

struct MemoryDataPrivate;

class CLA_EXPO MemoryData
{
public:
    typedef RuntimeExceptionImpl<MemoryData> Exception;
public:
    MemoryData();
    MemoryData(void * memory, size_t size);

    /**
      * Fills memory with zero
      */
    void clear();

    /**
      * Return true if shared data is empty
      */
    bool empty() const;

    /**
      * Returns format options from shared data
      * @throw Exception on error
      * @see setFormatOptions()
      */
    FormatOptions formatOptions() const;

    /**
      * Checks if memory has image path
      */
    bool hasImagePath() const;

    /**
      * Returns pointer to image data
      */
    ImagePtr image() const;

    /**
      * Returns image url
      * @throw Exception on error
      * @see setImageURL()
      */
    ImageURL imageURL() const;

    /**
     * Returns true if point to NULL memory
     */
    bool isNull() const;

    /**
      * Returns pointer to data memory
      */
    void * memory();

    /**
     * Returns string message
     * @see setMessage()
     */
    std::string message() const;

    /**
      * Returns CED page pointer
      * @throw Exception on error
      */
    CEDPagePtr page() const;

    /**
      * Returns recognize options
      * @throw Exception on error
      */
    RecognizeOptions recognizeOptions() const;

    /**
     * Sets memory pointer and size to 0
     */
    void reset();

    /**
      * Sets format options for data
      * @see formatOptions()
      */
    void setFormatOptions(const FormatOptions& fopts);

    /**
      * Sets image data
      */
    void setImage(ImagePtr image);

    /**
     * Sets memory pointer
     */
    void setMemory(void * m);

    /**
     * Sets string message
     */
    void setMessage(const std::string& msg);

    /**
      * Sets image url
      * @see imageURL()
      * @throw Exception on error
      */
    void setImageURL(const ImageURL& path);

    /**
      * Sets page data
      */
    void setPage(CEDPagePtr page);

    /**
      * Sets recognize options
      * @see recognizeOptions()
      * @throw Exception on error
      */
    void setRecognizeOptions(const RecognizeOptions& ropts);

    /**
     * Sets memory size
     */
    void setSize(size_t sz);

    /**
      * Returns memory size
      */
    size_t size() const;
public:
    static size_t minBufferSize();
private:
    MemoryDataPrivate * data() const;
private:
    void * memory_;
    size_t size_;
};

}

#endif // MEMORYDATA_H
