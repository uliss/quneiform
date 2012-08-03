/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <iosfwd>
#include <string>
#include <stdexcept>

#include "globus.h"

namespace cf
{

class ConfigParserPrivate;

class CLA_EXPO ConfigParser
{
public:
    typedef std::runtime_error Exception;
public:
    ConfigParser();
    ~ConfigParser();

    bool getBool(const std::string& path);
    float getFloat(const std::string& path);
    int getInt(const std::string& path);
    std::string getString(const std::string& path);

    bool hasValue(const std::string& path) const;

    void load(const std::string& fname);
private:
    ConfigParserPrivate * impl_;
};

}

#endif // CONFIGPARSER_H
