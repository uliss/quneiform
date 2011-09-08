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

#ifndef CSSEXPORTER_H_
#define CSSEXPORTER_H_

#include <map>
#include <string>

#include "styleexporter.h"

namespace cf
{

class CssExporter: public StyleExporter
{
    public:
        CssExporter(CEDPagePtr page, const FormatOptions& opts);
        void addCssStyle(const std::string& name, const std::string& content);

        virtual size_t hash(const CEDChar& chr) const;
        virtual size_t hash(const CEDParagraph& par) const;
        virtual std::string makeCssStyle(const CEDChar& chr) const;
        virtual std::string makeCssStyle(const CEDParagraph& par) const;
        virtual std::string makeStyle(const CEDChar& chr);
        virtual std::string makeStyle(const CEDParagraph& par);
        virtual void writePageEnd(CEDPage& page);
    private:
        typedef std::map<std::string, std::string> StyleMap;
        typedef StyleMap::value_type StyleEntry;
        void writeStyleEntry(const StyleEntry& entry);
    private:
        StyleMap styles_;
};

}

#endif /* CSSEXPORTER_H_ */
