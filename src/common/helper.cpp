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

size_t streamSize(std::stringstream& s) {
    return streamSize(static_cast<std::istream&>(s));
}

size_t streamSize(std::istream& is) {
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

size_t streamSize(std::ostream& os) {
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

std::string extractClassMethodNameFromSignature(const std::string& signature)
{
    size_t end = signature.find('(');

    if(end == std::string::npos)
        return std::string();

    size_t start = signature.rfind("::", end);
    if(start == std::string::npos)
        return std::string();

    start += 2; // '::' compensation

    return signature.substr(start, end - start);
}

std::string makeClassMethodSignature(const std::string& signature)
{
    size_t method_end = signature.find('(');

    if(method_end == std::string::npos)
        return std::string();

    size_t class_start = signature.rfind(' ', method_end);
    if(!class_start)
        return std::string();

    class_start++; // ' ' compensation

    std::string res = signature.substr(class_start, method_end - class_start);

    if(res.substr(0, 4) == "cf::")
        res = res.substr(4);

    return "[" +  res + "]";
}

}

