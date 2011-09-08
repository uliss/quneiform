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

#ifndef ENVPATHS_H
#define ENVPATHS_H

#include <vector>
#include <string>

#include "globus.h"

namespace cf
{

typedef std::vector<std::string> PathList;

/**
  * Returns list of system paths from @b PATH environment variable
  */
FUN_EXPO__ PathList envPaths();

/**
  * Finds given file in system environment @b PATH
  * if found - found path saved into @b foundPath param
  * @param file - search subject
  * @param foundPath - where to store found path, if NULL - ignore
  * @return @b true if file found, otherwise @b false
  */
FUN_EXPO__ bool findInEnvPath(const std::string& file, std::string * foundPath = NULL);
FUN_EXPO__ bool findInEnvPath(const PathList& files, std::string * foundPath = NULL);

/**
  * Finds given file in given paths
  * @return @b true if file found, otherwise @b false
  */
FUN_EXPO__ bool findInPaths(const std::string& file, const PathList& envPaths, std::string * foundPath = NULL);
FUN_EXPO__ bool findInPaths(const PathList& files, const PathList& envPaths, std::string * foundPath = NULL);


}

#endif // ENVPATHS_H
