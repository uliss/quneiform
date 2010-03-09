/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef EXPORTER_H_
#define EXPORTER_H_

#include <iostream>
#include <string>
#include <stdexcept>

#include "imageexporter.h"
#include "rfrmt/formatoptions.h"

namespace CIF
{

class Exporter
{
    public:
        Exporter(const FormatOptions& opts);
        virtual ~Exporter();

        typedef std::runtime_error Exception;

        /**
         * Returns true if charset conversion needed
         * it happens when input encoding no equal output encoding
         */
        bool isCharsetConversion() const;

        /**
         * Exports document to file
         * By default, file is truncated
         */
        virtual void exportTo(const std::string& filename);

        /**
         * Exports document to output stream
         * @note not all exporter can do this kind of export
         * in full manner
         */
        virtual void exportTo(std::ostream& os);

        /**
         * Returns export options
         */
        const FormatOptions& formatOptions() const;
        FormatOptions& formatOptions();

        /**
         * Returns pointer to image exporter
         */
        ImageExporterPtr imageExporter();

        /**
         * Returns document encoding
         */
        std::string inputEncoding() const;

        /**
         * Returns destination encoding
         */
        std::string outputEncoding() const;

        /**
         * Returns export filename
         * @return empty string, when exporting to stream
         */
        std::string outputFilename() const;

        /**
         * Sets format options
         */
        void setFormatOptions(const FormatOptions& opts);

        /**
         * Sets image exporter
         */
        void setImageExporter(ImageExporterPtr exporter);

        /**
         * Sets source document encoding
         */
        void setInputEncoding(const std::string& enc);

        /**
         * Sets desired encoding
         */
        void setOutputEncoding(const std::string& enc);

        /**
         * Sets export filename
         */
        void setOutputFilename(const std::string& filename);
    private:
        virtual void doExport(std::ostream& os) = 0;
        void autoDetectOutputEncoding();
    protected:
        /**
         * Sets input/output encoding by language
         */
        void setEncodings();
    private:
        ImageExporterPtr image_exporter_;
        FormatOptions format_options_;
        std::string input_encoding_;
        std::string output_encoding_;
        std::string output_filename_;
};

}

#endif /* EXPORTER_H_ */
