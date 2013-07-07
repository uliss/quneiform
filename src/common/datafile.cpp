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
#include "common_debug.h"
#include "singleton.h"

namespace cf {

static const char * ENV_PATH = "CF_DATADIR";

class DatafileImpl
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
    std::string datafileFullPath(const std::string& name) const;
private:
    std::string buildFullPath(const std::string& dir, const std::string& name) const;
    bool datafileExistsInPath(const std::string& name) const;
    bool datafileExistsInEnvPath(const std::string& name) const;
    bool datafileExistsInInstallPath(const std::string& name) const;
private:
    std::string path_;
    int search_mask_;
};

typedef Singleton<DatafileImpl, CreateUsingStatic> DF;

DatafileImpl::DatafileImpl()
    : search_mask_(DATAFILE_MAIN_PATH | DATAFILE_ENVIRONMENT | DATAFILE_INSTALL_PATH) {}

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
    if(searchPlace(DATAFILE_MAIN_PATH) && datafileExistsInPath(name))
        return true;

    if(searchPlace(DATAFILE_ENVIRONMENT) && datafileExistsInEnvPath(name))
        return true;

    if(searchPlace(DATAFILE_INSTALL_PATH) && datafileExistsInInstallPath(name))
        return true;

    return false;
}

std::string DatafileImpl::datafileFullPath(const std::string& name) const
{
    if(searchPlace(DATAFILE_MAIN_PATH) && datafileExistsInPath(name))
        return buildFullPath(path_, name);

    if(searchPlace(DATAFILE_ENVIRONMENT) && datafileExistsInEnvPath(name))
        return buildFullPath(envPath(), name);

    if(searchPlace(DATAFILE_INSTALL_PATH) && datafileExistsInInstallPath(name))
        return buildFullPath(INSTALL_DATADIR, name);

    return std::string();
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
    bool rc = fs::fileExists(full_path);
    if(!rc)
        COMMON_DEBUG_FUNC << "datafile not exists:" << full_path;
    return rc;
}

bool DatafileImpl::datafileExistsInEnvPath(const std::string& name) const
{
    std::string full_path = buildFullPath(envPath(), name);
    bool rc = fs::fileExists(full_path);
    if(!rc)
        COMMON_DEBUG_FUNC << "datafile not exists:" << full_path;
    return rc;
}

bool DatafileImpl::datafileExistsInInstallPath(const std::string& name) const
{
    std::string full_path = buildFullPath(INSTALL_DATADIR, name);
    bool rc = fs::fileExists(full_path);
    if(!rc)
        COMMON_DEBUG_FUNC << "datafile not exists:" << full_path;
    return rc;
}

int DatafileImpl::searchMask() const
{
    return search_mask_;
}

void DatafileImpl::setSearchMask(int mask)
{
    search_mask_ = mask;
}

std::string Datafile::envPath()
{
    return DF::instance().envPath();
}

std::string Datafile::mainPath()
{
    return DF::instance().path();
}

void Datafile::setMainPath(const std::string& path)
{
    DF::instance().setPath(path);
}

int Datafile::searchMask()
{
    return DF::instance().searchMask();
}

void Datafile::setSearchMask(int mask)
{
    DF::instance().setSearchMask(mask);
}

bool Datafile::exists(const std::string& name)
{
    return DF::instance().datafileExists(name);
}

std::string Datafile::fullPath(const std::string& name)
{
    return DF::instance().datafileFullPath(name);
}

} // namespace cf
