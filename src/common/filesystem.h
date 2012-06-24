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

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

#include "globus.h"

namespace cf {
namespace fs {

/**
 * Returns filename portion of pathname
 * @see removeFileExt()
 */
FUN_EXPO__ std::string baseName(const std::string& path);

/**
 * Checks if file exists
 */
FUN_EXPO__ bool fileExists(const std::string& fname);

/**
 * Returns file extension -
 */
FUN_EXPO__ std::string fileExtension(const std::string& filename);

/**
 * Returns filename without extension
 * @see baseName()
 */
FUN_EXPO__ std::string removeFileExtension(const std::string& filename);

/**
 * Returns filename with new extension
 * @see removeFileExt()
 */
FUN_EXPO__ std::string replaceFileExtension(const std::string& filename, const std::string& new_ext);

}
}

#endif // FILESYSTEM_H
