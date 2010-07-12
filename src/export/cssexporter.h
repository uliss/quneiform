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

namespace CIF
{

class CssExporter: public StyleExporter
{
    public:
        CssExporter(CEDPage * page, const FormatOptions& opts);
        typedef std::map<std::string, std::string> StyleMap;
        typedef StyleMap::value_type StyleEntry;

        virtual void addStyle(const CEDChar& chr, size_t hash);
        virtual std::string makeCssStyle(const CEDChar& chr) const;
        virtual std::string makeCssStyleKey(size_t hash) const;
        virtual void writePageEnd(std::ostream& os, CEDPage& page);
        virtual void writeStyleEntry(std::ostream& os, const StyleEntry& entry);
    private:
        StyleMap styles_;
};

}

#endif /* CSSEXPORTER_H_ */
