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

#include "helper.h"

namespace CIF
{

std::string escapeHtmlSpecialChars(const std::string& path)
{
    // uliss TODO!!!
    std::string ret;
    ret.reserve(path.size());
    for(std::string::const_iterator it = path.begin(), end = path.end(); it != end; ++it) {
        switch(*it) {
        case '"':
            ret.append("&quot;");
            break;
        case '\'':
            ret.append("&apos;");
            break;
        case '<':
            ret.append("&lt;");
            break;
        case '&':
            ret.append("&amp;");
            break;
        default:
            ret.append(1, *it);
        }
    }
    return ret;
}

}

