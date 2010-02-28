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

#ifndef OUTPUTFORMAT_H_
#define OUTPUTFORMAT_H_

#include <string>
#include <list>
#include "globus.h"

namespace CIF
{

enum format_t
{
    FORMAT_NONE = -1,
    FORMAT_EDNATIVE = 0,
    FORMAT_TEXT = 0x02,
    FORMAT_SMARTTEXT = 0x04,
    FORMAT_RTF = 0x08,
    FORMAT_TABLETXT = 0x0100, // Table text
    FORMAT_TABLECSV = 0x0200, // Table CSV (comma separated)
    FORMAT_TABLEDBF = 0x0400, // Table DBF
    FORMAT_TABLEWKS = 0x1000, // Table WKS (Lotus)
    FORMAT_HTML = 0x2000,
    FORMAT_HOCR = 0x4000,
    FORMAT_DEBUG = 0x8000,
    FORMAT_XHTML = 0x10000
};

typedef std::list<format_t> OutputFormatList;

class CLA_EXPO OutputFormat
{
    public:
        OutputFormat(format_t format);

        std::string description() const;
        std::string extension() const;
        format_t get() const;
        bool isValid() const;
        std::string name() const;
    public:
        static OutputFormat byName(const std::string& name);
        static std::string description(format_t format);
        static std::string extension(format_t format);
        static OutputFormatList formats();
        static std::string name(format_t format);
    private:
        format_t format_;
};

inline format_t OutputFormat::get() const
{
    return format_;
}

}

#endif /* OUTPUTFORMAT_H_ */
