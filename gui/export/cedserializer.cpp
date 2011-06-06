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
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
#include "ced/blockelement.h"
#include "ced/element.h"
#include "ced/cedarchive.h"

CEDSerializer::CEDSerializer(cf::CEDPage * page) : page_(page)
{
}

QDataStream& operator<<(QDataStream& os, const CEDSerializer& ced) {
    if(ced.page_) {
        try {
            std::ostringstream buf;
            cf::CEDOutputArchive ar(buf);
            ar << boost::serialization::make_nvp("cedpage", ced.page_);
            std::string txt(buf.str());
            os << true;
            os << txt.c_str();
        }
        catch(std::exception& e) {
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
        cf::CEDInputArchive ar(buf);

        cf::CEDPage * cedpage = new cf::CEDPage;
        ar >> cedpage;

        delete ced.page_;
        ced.page_ = cedpage;

        delete[] str;
    }

    return is;
}
