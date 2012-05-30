/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include "iscanner.h"

namespace cf {

IScanner::IScanner()
{
}

IScanner::~IScanner()
{
}

const IScanner::ScanOptions& IScanner::options() const
{
    return opts_;
}

IScanner::ScanOptions& IScanner::options()
{
    return opts_;
}

void IScanner::clearOptions()
{
    opts_.clear();
}

std::ostream& IScanner::dumpOptions(std::ostream& os) const
{
    for(OptionIteratorConst it = opts_.begin(); it != opts_.end(); ++it) {
        os << (*it) << "\n";
    }

    return os;
}

}
