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

#include <set>
#include "common/color.h"
#include "genericexporter.h"

namespace CIF
{

class StyleExporter: public GenericExporter
{
    public:
        StyleExporter(CEDPage * page, const FormatOptions& opts);
        ~StyleExporter();

        virtual void addStyle(const CEDChar& chr, size_t hash);
        virtual void exportChar(CEDChar& chr);
        virtual size_t hash(const CEDChar& chr) const;
    private:
        std::set<size_t> hashes_;
};

}

#endif /* STYLEEXPORTER_H_ */
