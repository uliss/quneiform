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

namespace cf
{

struct OutputFormatEntry
{
        std::string name;
        std::string descr;
        std::string ext;
};

typedef std::map<format_t, OutputFormatEntry> OutputFormatMap;

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

static inline void addFormat(format_t format, const std::string& name,
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
    addFormat(FORMAT_TEXT, "text", "plain text", "txt");
    addFormat(FORMAT_SMARTTEXT, "smarttext", "plain text with TeX paragraphs", "txt");
    addFormat(FORMAT_HTML, "html", "HTML format", "html");
    addFormat(FORMAT_HOCR, "hocr", "hOCR HTML format", "html");
    addFormat(FORMAT_DJVUTXT, "djvutxt", "DJVU text format", "txt");
    addFormat(FORMAT_DJVUXML, "djvuxml", "DJVU XML format", "xml");
    //addFormat(FORMAT_RTF, "rtf", "RTF format", "rtf");
    addFormat(FORMAT_SUMMARY, "summary", "prints recognition summary", "txt");
    addFormat(FORMAT_DEBUG, "textdebug", "for debugging purposes", "txt");
    addFormat(FORMAT_NATIVE_TXT, "native", "native cuneiform text format", "ced");
    addFormat(FORMAT_NATIVE_XML, "native-xml", "native cuneiform xml format", "xml");
    addFormat(FORMAT_ODF, "odf", "OpenDocument Format", "odt");
    addFormat(FORMAT_FB2, "fb2", "Fiction Book 2 format", "fb2");

    return true;
}

const bool init = initOutputFormatData();
}

OutputFormat::OutputFormat(format_t format) :
    format_(format)
{
}

OutputFormat OutputFormat::byName(const std::string& name)
{
    OutputFormatMap::iterator it = formatInfo().begin(), end = formatInfo().end();
    while (it != end) {
        if (it->second.name == name)
            return OutputFormat(it->first);
        ++it;
    }
    return OutputFormat(FORMAT_NONE);
}

std::string OutputFormat::description(format_t format)
{
    OutputFormatMap::iterator it = formatInfo().find(format);
    return (it != formatInfo().end()) ? it->second.descr : "";
}

std::string OutputFormat::description() const
{
    return description(format_);
}

std::string OutputFormat::extension(format_t format)
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

bool OutputFormat::isValid() const
{
    return (FORMAT_NONE < format_) ? true : false;
}

std::string OutputFormat::name(format_t format)
{
    OutputFormatMap::iterator it = formatInfo().find(format);
    return (it != formatInfo().end()) ? it->second.name : "";
}

std::string OutputFormat::name() const
{
    return name(format_);
}

}
