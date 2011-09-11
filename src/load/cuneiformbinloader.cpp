/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include <boost/archive/binary_iarchive.hpp>
#include <boost/current_function.hpp>
#include <iostream>

#include "cuneiformbinloader.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
#include "ced/blockelement.h"
#include "ced/element.h"
#include "common/serialize.h"
#include "common/helper.h"

namespace  cf {

typedef boost::archive::binary_iarchive CEDBinInputArchive;

CuneiformBinLoader::CuneiformBinLoader()
{}

CEDPagePtr CuneiformBinLoader::load(std::istream& is) {
    if(is.fail()) {
        std::cerr << BOOST_CURRENT_FUNCTION << ": bad input stream" << std::endl;
        throw Exception("[CuneiformBinLoader] bad input stream given");
    }

    try {
        CEDBinInputArchive ar(is);
        CEDPage * p = NULL;
        ar >> p;
        return CEDPagePtr(p);
    }
    catch(std::exception& e) {
        std::cerr << BOOST_CURRENT_FUNCTION << " load error: " << e.what() << std::endl;
        throw Exception(toString("[CuneiformBinLoader] load error: ", e.what()));
    }
}

}
