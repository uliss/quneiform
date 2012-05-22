/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#ifndef IMAGEEXPORTER_H_
#define IMAGEEXPORTER_H_

#include <string>
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "common/exception.h"
#include "common/imageformats.h"

namespace cf
{

class ImageRawData;
class ImageExporter;
typedef boost::shared_ptr<ImageExporter> ImageExporterPtr;

class ImageExporter
{
    public:
        typedef ImageExporterPtr (*Creator)();
    public:
        ImageExporter();
        virtual ~ImageExporter();

        typedef RuntimeExceptionImpl<ImageExporter> Exception;
        typedef std::vector<image_format_t> FormatList;

        /**
         * Returns filename extension for export format
         * for ex. FORMAT_GIF => "gif"
         * In case of unknown format returns empty string
         */
        std::string extension() const;

        /**
         * Returns export format
         */
        image_format_t format() const;

        /**
         * Check if export format is supported
         */
        bool isSupportedFormat(image_format_t format);

        /**
         * Returns mime type of exported image
         */
        virtual std::string mime() const = 0;

        /**
         * Saves image from memory to filename
         * @param output filename
         */
        void save(const ImageRawData& image, const std::string& path);

        /**
         * Saves image from memory to stream
         */
        void save(const ImageRawData& image, std::ostream& os);

        /**
         * Sets export format
         * @note if format not supported do nothing
         * @see isSupportedFormat
         */
        void setFormat(image_format_t format);

        /**
         * Returns list of supported formats
         */
        virtual FormatList supportedFormats() const = 0;
    protected:
        virtual void saveToStream(const ImageRawData& image, std::ostream& os) = 0;
    public:
        /**
         * Returns string representation of given format
         */
        static std::string formatToString(image_format_t format);
    private:
        image_format_t format_;
};

}

#endif /* IMAGEEXPORTER_H_ */
