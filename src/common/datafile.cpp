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

#include "datafile.h"
#include "filesystem.h"
#include "config-paths.h"

namespace cf {

static const char * ENV_PATH = "CF_DATADIR";

DatafileImpl::DatafileImpl() :
    search_mask_(DATAFILE_PATH | DATAFILE_ENVIRONMENT | DATAFILE_INSTALL_PATH)
{
}

std::string DatafileImpl::envPath() const
{
    const char * env_path = getenv(ENV_PATH);
    if(env_path)
        return env_path;

    return std::string();
}

std::string DatafileImpl::path() const
{
    return path_;
}

void DatafileImpl::setPath(const std::string& path)
{
    path_ = path;
}

bool DatafileImpl::datafileExists(const std::string& name) const
{
    if(searchPlace(DATAFILE_PATH) && datafileExistsInPath(name))
        return true;

    if(searchPlace(DATAFILE_ENVIRONMENT) && datafileExistsInEnvPath(name))
        return true;

    if(searchPlace(DATAFILE_INSTALL_PATH) && datafileExistsInInstallPath(name))
        return true;

    return false;
}

std::string DatafileImpl::buildFullPath(const std::string& dir, const std::string& name) const
{
    std::string res = dir;
    size_t len = res.length();
    if(res[len - 1] != '/')
        res.append("/");

    return res.append(name);
}

bool DatafileImpl::datafileExistsInPath(const std::string& name) const
{
    std::string full_path = buildFullPath(path_, name);
    return fs::fileExists(full_path);
}

bool DatafileImpl::datafileExistsInEnvPath(const std::string& name) const
{
    std::string full_path = buildFullPath(envPath(), name);
    return fs::fileExists(full_path);
}

bool DatafileImpl::datafileExistsInInstallPath(const std::string& name) const
{
    std::string full_path = buildFullPath(INSTALL_DATADIR, name);
    return fs::fileExists(full_path);
}

int DatafileImpl::searchMask() const
{
    return search_mask_;
}

void DatafileImpl::setSearchMask(int mask)
{
    search_mask_ = mask;
}

} // namespace cf
