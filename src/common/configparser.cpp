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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional.hpp>

#include "configparser.h"

namespace cf
{

struct ConfigParserPrivate
{
    boost::property_tree::ptree tree;
};

ConfigParser::ConfigParser() :
    impl_(new ConfigParserPrivate)
{}

ConfigParser::~ConfigParser()
{
    delete impl_;
}

bool ConfigParser::getBool(const std::string& path)
{
    if(!hasValue(path))
        throw Exception("key not found");

    return impl_->tree.get<bool>(path);
}

bool ConfigParser::getBool(const std::string &path, bool fallback)
{
    return impl_->tree.get(path, fallback);
}

float ConfigParser::getFloat(const std::string& path)
{
    if(!hasValue(path))
        throw Exception("key not found");

    return impl_->tree.get<float>(path);
}

float ConfigParser::getFloat(const std::string& path, float fallback)
{
    return impl_->tree.get<float>(path, fallback);
}

int ConfigParser::getInt(const std::string& path)
{
    if(!hasValue(path))
        throw Exception("key not found");

    return impl_->tree.get<int>(path);
}

int ConfigParser::getInt(const std::string& path, int fallback)
{
    return impl_->tree.get<int>(path, fallback);
}

std::string ConfigParser::getString(const std::string& path)
{
    if(!hasValue(path))
        throw Exception("key not found");

    return impl_->tree.get<std::string>(path);
}

std::string ConfigParser::getString(const std::string& path, const std::string& fallback)
{
    return impl_->tree.get<std::string>(path, fallback);
}

void ConfigParser::load(const std::string& fname)
{
    // Load the INI file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.
    boost::property_tree::read_json(fname, impl_->tree);
}

bool ConfigParser::hasValue(const std::string& path) const
{
    return impl_->tree.get_child_optional(path);
}

}
