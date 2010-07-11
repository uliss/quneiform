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

#ifndef SUMMARYEXPORTER_H_
#define SUMMARYEXPORTER_H_

#include "genericexporter.h"

namespace CIF
{

class SummaryExporter: public GenericExporter
{
    public:
        SummaryExporter(CEDPage * page, const FormatOptions& opts);
    private:
        void doExport(std::ostream& os);
        void printPageStat(std::ostream& os);
        void calcPageStat();
    private:
        size_t total_columns_;
        size_t total_pars_;
        size_t total_lines_;
        size_t total_chars_;
        size_t total_pictures_;
};

}

#endif /* SUMMARYEXPORTER_H_ */
