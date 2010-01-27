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

#include <map>
#include "outputformat.h"

namespace CIF
{

struct OutputFormatEntry
{
        std::string name;
        std::string descr;
        std::string ext;
};

typedef std::map<puma_format_t, OutputFormatEntry> OutputFormatMap;

OutputFormatMap& formatInfo()
{
    static OutputFormatMap info;
    return info;
}

OutputFormatList& formatList()
{
    static OutputFormatList lst;
    return lst;
}

static inline void addFormat(puma_format_t format, const std::string& name,
        const std::string& description, const std::string& extension)
{
    OutputFormatEntry entry = { name, description, extension };
    formatInfo()[format] = entry;
    formatList().push_back(format);
}

namespace
{

bool initOutputFormatData()
{
    addFormat(PUMA_TOEDNATIVE, "native", "Cuneiform 2000 format", "ed");
    addFormat(PUMA_TOTEXT, "text", "plain text", "txt");
    addFormat(PUMA_TOSMARTTEXT, "smarttext", "plain text with TeX paragraphs", "txt");
    addFormat(PUMA_TORTF, "rtf", "RTF format", "rtf");
    addFormat(PUMA_TOHTML, "html", "HTML format", "html");
    addFormat(PUMA_TOHOCR, "hocr", "hOCR HTML format", "html");
    addFormat(PUMA_DEBUG_TOTEXT, "textdebug", "for debugging purposes", "txt");
    return true;
}

const bool init = initOutputFormatData();
}

OutputFormat::OutputFormat(puma_format_t format) :
    format_(format)
{
}

OutputFormat::~OutputFormat()
{
}

std::string OutputFormat::description(puma_format_t format)
{
    OutputFormatMap::iterator it = formatInfo().find(format);
    return (it != formatInfo().end()) ? it->second.descr : "";
}

std::string OutputFormat::description() const
{
    return description(format_);
}

std::string OutputFormat::extension(puma_format_t format)
{
    OutputFormatMap::iterator it = formatInfo().find(format);
    return (it != formatInfo().end()) ? it->second.ext : "";
}

std::string OutputFormat::extension() const
{
    return extension(format_);
}

OutputFormatList OutputFormat::formats()
{
    return formatList();
}

std::string OutputFormat::name(puma_format_t format)
{
    OutputFormatMap::iterator it = formatInfo().find(format);
    return (it != formatInfo().end()) ? it->second.name : "";
}

std::string OutputFormat::name() const
{
    return name(format_);
}


}
