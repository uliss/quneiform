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

namespace cf
{
FUN_EXPO__ std::string escapeHtmlSpecialChars(const std::string& path);

template<class T>
std::string toString(const T& t) {
    std::ostringstream os;
    os << t;
    return os.str();
}

template<class T1, class T2>
std::string toString(const T1& t1, const T2& t2) {
    std::ostringstream buf;
    buf << t1 << ' ' << t2 << std::endl;
    return buf.str();
}

template<class T1, class T2, class T3>
std::string toString(const T1& t1, const T2& t2, const T3& t3) {
    std::ostringstream buf;
    buf << t1 << ' ' << t2 << ' ' << t3 << std::endl;
    return buf.str();
}

template<class T1, class T2, class T3, class T4>
std::string toString(const T1& t1, const T2& t2, const T3& t3, const T4& t4) {
    std::ostringstream buf;
    buf << t1 << ' ' << t2 << ' ' << t3 << ' ' << t4 << std::endl;
    return buf.str();
}

inline void toUpper(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

inline void toLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

inline void replaceAll(std::string& str, const std::string& what, const std::string& to) {
    std::string::size_type pos = 0;
    while ((pos = str.find(what, pos)) != std::string::npos) {
        str.replace(pos, what.size(), to);
        pos += to.size();
    }
}

FUN_EXPO__ size_t streamSize(std::istream& is);
FUN_EXPO__ size_t streamSize(std::ostream& is);
FUN_EXPO__ size_t streamSize(std::stringstream& s);
}

#endif /* HELPER_H_ */
