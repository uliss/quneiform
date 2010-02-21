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

#include "rfrmt/formatoptions.h"

namespace CIF {

class Exporter
{
public:
    Exporter();
    Exporter(const FormatOptions& opts);
    virtual ~Exporter();

    typedef std::runtime_error Exception;

    virtual bool encodeNeeded() const;
    virtual void exportTo(const std::string& filename);
    void exportTo(std::ostream& os);
    const FormatOptions& formatOptions() const;
    FormatOptions& formatOptions();
    std::string inputEncoding() const;
    std::string outputEncoding() const;
    std::string outputFilename() const;
    void setFormatOptions(const FormatOptions& opts);
    void setInputEncoding(const std::string& enc);
    void setOutputEncoding(const std::string& enc);
    void setOutputFilename(const std::string& filename);
private:
    virtual void doExport(std::ostream& os) = 0;
    void autoDetectOutputEncoding();
    FormatOptions format_options_;
    std::string input_encoding_;
    std::string output_encoding_;
    std::string output_filename_;
};

}

#endif /* EXPORTER_H_ */
