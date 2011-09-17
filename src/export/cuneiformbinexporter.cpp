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

#include <sstream>
#include <boost/current_function.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "cuneiformbinexporter.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "ced/cedframe.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
#include "ced/blockelement.h"
#include "ced/element.h"
#include "ced/cedtable.h"
#include "common/serialize.h"
#include "common/helper.h"

namespace cf {

typedef boost::archive::binary_oarchive CEDBinOutputArchive;

CuneiformBinExporter::CuneiformBinExporter(CEDPagePtr page, const FormatOptions& opts) :
    Exporter(page, opts)
{
}

void CuneiformBinExporter::doExport(std::ostream& os) {
    if(!page()) {
        std::cerr << BOOST_CURRENT_FUNCTION << ": empty page" << std::endl;
        throw Exception("[CuneiformBinExporter] empty page given");
    }

    if(formatOptions().isTestOutput())
        page()->setImageName("");

    try {
        CEDBinOutputArchive ar(os);
        CEDPage * p = page().get();
        ar << p;
    }
    catch(std::exception& e) {
        std::cerr << BOOST_CURRENT_FUNCTION << " export error: " << e.what() << std::endl;
        throw Exception(toString("[CuneiformBinExporter] export error: ", e.what()));
    }
}

}
