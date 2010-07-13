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

#ifndef STYLEEXPORTER_H_
#define STYLEEXPORTER_H_

#include <map>
#include <string>
#include "common/color.h"
#include "genericexporter.h"

namespace CIF
{

class StyleExporter: public GenericExporter
{
    public:
        StyleExporter(CEDPage * page, const FormatOptions& opts);
        ~StyleExporter();

        virtual void exportChar(CEDChar& chr);
        virtual void exportParagraph(CEDParagraph& par);
        virtual size_t hash(const CEDChar& chr) const;
        virtual size_t hash(const CEDParagraph& par) const;
        virtual std::string makeStyle(const CEDChar& chr);
        virtual std::string makeStyle(const CEDParagraph& par);
    private:
        void addStyle(const std::string& name, size_t hash);
    private:
        // hash => style name map
        typedef std::map<size_t, std::string> HashMap;
        HashMap hashes_;
};

}

#endif /* STYLEEXPORTER_H_ */
