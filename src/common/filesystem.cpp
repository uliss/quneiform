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

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
#include <string>

#include "cfcompat.h"
#include "filesystem.h"

namespace cf {
    namespace fs  {
        bool fileExists(const std::string& fname)
        {
            return (_access(fname.c_str(), 0) == 0);
        }

        std::string fileExtension(const std::string& filename)
        {
            size_t dot_position = filename.rfind('.');
            return (dot_position == std::string::npos) ? std::string() : filename.substr(dot_position + 1);
        }

        std::string removeFileExtension(const std::string& filename)
        {
            return filename.substr(0, filename.rfind('.'));
        }

        std::string replaceFileExtension(const std::string& filename, const std::string& new_ext)
        {
            return removeFileExtension(filename) + std::string(1, '.') + new_ext;
        }

        std::string baseName(const std::string& path)
        {

#ifdef _WIN32
#define DIRSEP "/\\"
#else
#define DIRSEP "/"
#endif

#ifndef _WIN32
            if(path == "/")
                return "/";
#endif

            std::vector<std::string> res;
            boost::algorithm::split(res, path, boost::algorithm::is_any_of(DIRSEP));

            if(res.empty())
                return "";

            return res.back();
        }
    }
}
