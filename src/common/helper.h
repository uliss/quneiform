/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "globus.h"

namespace CIF
{

FUN_EXPO__ std::string escapeHtmlSpecialChars(const std::string& path);

inline std::string getFileExt(const std::string& filename)
{
    size_t dot_position = filename.rfind('.');
    return (dot_position == std::string::npos) ? std::string() : filename.substr(dot_position + 1);
}

inline std::string replaceFileExt(const std::string& filename, const std::string& new_ext)
{
    return filename.substr(0, filename.rfind('.')) + new_ext;
}

inline std::string removeFileExt(const std::string& filename)
{
    return filename.substr(0, filename.rfind('.'));
}

template<class T>
std::string toString(const T& t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

inline void toUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

inline void toLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

inline void replaceAll(std::string& str, const std::string& what, const std::string& to) {
    std::string::size_type pos = 0;
    while((pos = str.find(what, pos)) != std::string::npos) {
        str.replace(pos, what.size(), to);
        pos += to.size();
    }
}
}

#endif /* HELPER_H_ */
