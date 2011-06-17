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

#include "cuneiformexporter.h"
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

namespace cf {

CuneiformExporter::CuneiformExporter(CEDPage * p, const FormatOptions& opts) :
    Exporter(opts),
    page_(p)
{
}

void CuneiformExporter::doExport(std::ostream& os) {
    if(!page_) {
        std::cerr << "empty page given";
        std::cerr.flush();
        return;
    }

    // uliss: hack for testing
    if(!formatOptions().writeMetaGenerator())
        page_->setImageName("");

    try {
        cf::CEDOutputArchive ar(os);
        ar << boost::serialization::make_nvp("cedpage", page_);
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr.flush();
    }
}

CEDPage * CuneiformExporter::page() {
    return page_;
}

void CuneiformExporter::setPage(CEDPage * p) {
    page_ = p;
}

}
