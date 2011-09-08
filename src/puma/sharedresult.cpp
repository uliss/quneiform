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
#include <boost/current_function.hpp>

#include "sharedresult.h"
#include "common/console_messages.h"
#include "common/debug.h"
#include "common/helper.h"
#include "export/cuneiformbinexporter.h"
#include "load/cuneiformbinloader.h"

#define CF_ERROR std::cerr << cf::console::error << BOOST_CURRENT_FUNCTION << ": "
#define CF_WARNING std::cerr << cf::console::warning << BOOST_CURRENT_FUNCTION << ": "
#define CF_INFO cf::Debug() << cf::console::info << BOOST_CURRENT_FUNCTION << ": "

namespace cf {

SharedResult::SharedResult() {
    clear();
}

void SharedResult::clear() {
    size_ = 0;
    std::fill(data_, data_ + MAXSIZE, '\0');
}

CEDPagePtr SharedResult::get() {
    CEDPagePtr res;

    if(size_ == 0) {
        CF_WARNING << "no result" << std::endl;
        return res;
    }

    try {
        std::stringstream buf;
        buf.write(data_, size_);
        CuneiformBinLoader l;
        res = l.load(buf);
        CF_INFO << size_ << " bytes read\n";
    }
    catch(std::exception& e) {
        CF_ERROR << e.what() << std::endl;
    }

    return res;
}

bool SharedResult::store(CEDPagePtr p) {
    std::stringstream buf;

    try {
        CuneiformBinExporter e(p, FormatOptions());
        e.exportTo(buf);
    }
    catch(std::exception& e) {
        CF_ERROR << "serialization error: " << e.what() << std::endl;
        return false;
    }

    size_ = streamSize(buf);

    if(size_ >= MAXSIZE) {
        CF_ERROR << "the page is too big (" << size_ << " bytes).\n"
                 << "It exceeds shared memory size: " << MAXSIZE << " bytes." << std::endl;
        clear();
        return false;
    }

    buf.readsome(data_, MAXSIZE);

    if(buf.fail()) {
        CF_ERROR << "buffer read failed." << std::endl;
        clear();
        return false;
    }

    CF_INFO << size_ << " bytes stored\n";

    return true;
}

}
