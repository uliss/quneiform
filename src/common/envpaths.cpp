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

#include <cstdlib>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/current_function.hpp>

#ifndef _WIN32
#include <sys/stat.h>
#endif

#include "envpaths.h"
#include "filesystem.h"
#include "common/debug.h"

namespace cf {

PathList envPaths()
{
    PathList res;
    const char * p = getenv("PATH");
    if(!p)
        return res;

    std::string path(p);
#ifdef _WIN32
    boost::algorithm::split(res, path, boost::algorithm::is_any_of(";"));
#else
    boost::algorithm::split(res, path, boost::algorithm::is_any_of(":"));
#endif
    return res;
}

static std::string pathConcat(const std::string& path1, const std::string& path2)
{
    if(path1.empty())
        return path2;

    if(path1.at(path1.size() - 1) == '/')
        return path1 + path2;
    else
        return path1 + std::string(1, '/') + path2;
}

static bool findExeInPath(const std::string& file, const std::string& path)
{
    std::string p = pathConcat(path, file);

    if(!fs::fileExists(p))
        return false;

#ifndef _WIN32
    struct stat st;
    if (lstat(p.c_str(), &st)) {
        Debug() << BOOST_CURRENT_FUNCTION << ": can't get stat info for " << p << "\n";
        return false;
    }

    if((st.st_mode & S_IFMT) != S_IFREG)
        return false;

#endif
    return true;
}

static bool findFilesInPath(const PathList& files,
                            const std::string& path,
                            std::string * foundPath)
{
    for(size_t i = 0; i < files.size(); i++) {
        if(!findExeInPath(files[i], path))
            continue;

        if(foundPath)
            *foundPath = pathConcat(path, files[i]);

        return true;
    }

    return false;
}

bool findInPaths(const PathList& files, const PathList& paths, std::string * foundPath)
{
    for(size_t i = 0; i < paths.size(); i++) {
        if(findFilesInPath(files, paths[i], foundPath))
            return true;
    }
    return false;
}

bool findInPaths(const std::string& file, const PathList& paths, std::string * foundPath)
{
    for(size_t i = 0; i < paths.size(); i++) {
        if(findExeInPath(file, paths[i])) {
            if(foundPath)
                *foundPath = pathConcat(paths[i], file);

            return true;
        }
    }

    return false;
}

bool findInEnvPath(const std::string& file, std::string * foundPath)
{
    return findInPaths(file, envPaths(), foundPath);
}

bool findInEnvPath(const PathList& files, std::string * foundPath)
{
    return findInPaths(files, envPaths(), foundPath);
}


}
