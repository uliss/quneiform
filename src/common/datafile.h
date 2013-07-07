/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#ifndef CF_DATAFILE_H
#define CF_DATAFILE_H

#include <string>

#include "globus.h"

namespace cf {

enum DatafileSearchFlag
{
    DATAFILE_MAIN_PATH = 1,
    DATAFILE_ENVIRONMENT = 1 << 1,
    DATAFILE_INSTALL_PATH = 1 << 2
};

class CLA_EXPO Datafile
{
    Datafile();
    Datafile(const Datafile&);
    void operator=(const Datafile&);
public:
    static std::string envPath();

    static std::string mainPath();
    static void setMainPath(const std::string& mainPath);

    static int searchMask();
    static void setSearchMask(int mask);

    static bool exists(const std::string& name);
    static std::string fullPath(const std::string& name);
};

} // namespace cf

#endif // CF_DATAFILE_H
