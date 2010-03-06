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

#ifndef ODTEXPORTER_H_
#define ODTEXPORTER_H_

#include "genericexporter.h"
#include "zip.h"

namespace CIF
{

class OdfExporter: public CIF::GenericExporter
{
    public:
        OdfExporter(CEDPage * page, const FormatOptions& opts = FormatOptions());
        ~OdfExporter();

        void exportTo(const std::string& fname);
        void exportTo(std::ostream& os);
    private:
        void addOdfMeta();
        void addOdfMime();
        void odfClose();
        void odfOpen(const std::string& fname);
        void odfWrite(const std::string& fname, const std::string& data);
    private:
        zip * zip_;
};

}

#endif /* ODTEXPORTER_H_ */
