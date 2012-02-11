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
#include <QByteArray>

#include "cedserializer.h"

#ifdef USE_SERIALIZE
#include "export/cuneiformexporter.h"
#include "load/cuneiformtextloader.h"
#endif

CEDSerializer::CEDSerializer(cf::CEDPagePtr page) : page_(page)
{
}

cf::CEDPagePtr CEDSerializer::page() {
    return page_;
}

QDataStream& operator<<(QDataStream& os, const CEDSerializer& ced) {
#ifdef USE_SERIALIZE
    if(ced.page_) {
        try {
            std::ostringstream buf;
            cf::CuneiformExporter exp(ced.page_, cf::FormatOptions());
            exp.exportTo(buf);

            os << true;
            QByteArray array(buf.str().c_str());
            os << array;
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
#endif

    return os;
}

QDataStream& operator>>(QDataStream& is, CEDSerializer& ced) {
#ifdef USE_SERIALIZE
    bool has_cedpage;

    is >> has_cedpage;

    if(has_cedpage) {
        QByteArray array;
        is >> array;
        std::istringstream buf(array.constData());

        cf::CuneiformTextLoader ld;
        cf::CEDPagePtr page = ld.load(buf);
        ced.page_ = page;
    }
#endif

    return is;
}
