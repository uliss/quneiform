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

namespace cf
{

std::string escapeHtmlSpecialChars(const std::string& path) {
    // uliss TODO!!!
    std::string ret;
    ret.reserve(path.size());
    for (std::string::const_iterator it = path.begin(), end = path.end(); it != end; ++it) {
        switch (*it) {
        case '"':
            ret.append("&quot;");
            break;
        case '\'':
            ret.append("&apos;");
            break;
        case '<':
            ret.append("&lt;");
            break;
        case '>':
            ret.append("&gt;");
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

#if defined _WIN32 || defined __WIN32__
inline bool IS_SLASH(char ch) {
    return ch == '\\' || ch == '/';
}
#else
inline bool IS_SLASH(char ch) {
    return ch == '/';
}
#endif

std::string baseName(const std::string& path) {
    bool last_slash = false;
    size_t slash_pos = std::string::npos;
    const size_t sz = path.size();
    for (size_t i = sz; i != 0; i--) {
        if (IS_SLASH(path[i - 1])) {
            if (i == sz) {
                last_slash = true;
            } else {
                slash_pos = i;
                break;
            }
        }
    }

    if (slash_pos == std::string::npos) {
        return (last_slash && sz > 1) ? path.substr(0, sz - 1) : path;
    }

    return last_slash ? path.substr(slash_pos, (sz - slash_pos) - 1) : path.substr(slash_pos);
}

std::streampos streamSize(std::istream& is) {
    std::streampos prev_pos = is.tellg();
    // -1 returned in case of error
    if (prev_pos < 0)
        return 0;
    is.seekg(0, std::ios::end);
    // exception thrown or failbit flag set
    if (is.fail())
        return 0;
    std::streampos ret = is.tellg();
    if (ret < 0)
        return 0;
    is.seekg(prev_pos);
    if (is.fail())
        return 0;
    return ret;
}

std::streampos streamSize(std::ostream& os) {
    std::streampos prev_pos = os.tellp();
    // -1 returned in case of error
    if (prev_pos < 0)
        return 0;
    os.seekp(0, std::ios::end);
    // exception thrown or failbit flag set
    if (os.fail())
        return 0;
    std::streampos ret = os.tellp();
    if (ret < 0)
        return 0;
    os.seekp(prev_pos);
    if (os.fail())
        return 0;
    return ret;
}

}

