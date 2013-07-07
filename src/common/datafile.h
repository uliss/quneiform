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
#include <boost/noncopyable.hpp>

#include "globus.h"
#include "singleton.h"

namespace cf {

enum DatafileSearchFlag
{
    DATAFILE_PATH = 1,
    DATAFILE_ENVIRONMENT = 1 << 1,
    DATAFILE_INSTALL_PATH = 1 << 2
};

class CLA_EXPO DatafileImpl : boost::noncopyable
{
public:
    DatafileImpl();

    std::string envPath() const;

    std::string path() const;
    void setPath(const std::string& path);

    bool searchPlace(DatafileSearchFlag flag) const { return search_mask_ & flag; }
    int searchMask() const;
    void setSearchMask(int mask);

    bool datafileExists(const std::string& name) const;
private:
    std::string buildFullPath(const std::string& dir, const std::string& name) const;
    bool datafileExistsInPath(const std::string& name) const;
    bool datafileExistsInEnvPath(const std::string& name) const;
    bool datafileExistsInInstallPath(const std::string& name) const;
private:
    std::string path_;
    int search_mask_;
};

typedef Singleton<DatafileImpl, CreateUsingStatic> Datafile;

} // namespace cf

#endif // CF_DATAFILE_H
