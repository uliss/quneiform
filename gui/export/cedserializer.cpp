/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <sstream>
#include <QDebug>

#include "cedserializer.h"
#include "export/cuneiformexporter.h"
#include "load/cuneiformtextloader.h"

CEDSerializer::CEDSerializer(cf::CEDPagePtr page) : page_(page)
{
}

cf::CEDPagePtr CEDSerializer::page() {
    return page_;
}

QDataStream& operator<<(QDataStream& os, const CEDSerializer& ced) {
    if(ced.page_) {
        try {
            std::ostringstream buf;
            cf::CuneiformExporter exp(ced.page_, cf::FormatOptions());
            exp.exportTo(buf);

            os << true;
            os << buf.str().c_str();
        }
        catch(std::exception& e) {
            os << false;
            std::cerr << e.what() << std::endl;
            std::cerr.flush();
        }
    }
    else {
        os << false;
    }

    return os;
}

QDataStream& operator>>(QDataStream& is, CEDSerializer& ced) {
    bool has_cedpage;

    is >> has_cedpage;

    if(has_cedpage) {
        char * str;
        is >> str;
        std::istringstream buf(str);

        cf::CuneiformTextLoader ld;
        ced.page_ = ld.load(buf);

        delete[] str;
    }

    return is;
}
