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

#include <cstdlib>
#include <iostream>
#include <boost/current_function.hpp>

#include "configoptions.h"

namespace cf
{

static std::string getConfigPath()
{
#ifdef _WIN32
    return "./config.json";
#else
    std::string home = ::getenv("HOME");
    std::string res = home + "/.config/cuneiform/config.json";
    return res;
#endif
}

ConfigParser ConfigOptions::parser_;
bool ConfigOptions::is_opened_ = false;

bool ConfigOptions::hasOption(const std::string& value)
{
    checkOpened();
    return parser_.hasValue(value);
}

void ConfigOptions::checkOpened()
{
    if(!is_opened_)
        open();
}

void ConfigOptions::open()
{
    if(is_opened_)
        return;

    try {
        parser_.load(getConfigPath());
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    is_opened_ = true;
}

bool ConfigOptions::getBool(const std::string &value)
{
    checkOpened();
    return parser_.getBool(value);
}

bool ConfigOptions::getBool(const std::string& value, bool fallback)
{
    if(!hasOption(value))
        return fallback;

    return getBool(value);
}

int ConfigOptions::getInt(const std::string& value)
{
    checkOpened();
    return parser_.getInt(value);
}
int ConfigOptions::getInt(const std::string& value, int fallback)
{
    if(!hasOption(value))
        return fallback;

    return getInt(value);
}

float ConfigOptions::getFloat(const std::string& value)
{
    checkOpened();
    return parser_.getFloat(value);
}

float ConfigOptions::getFloat(const std::string &value, float fallback)
{
    if(!hasOption(value))
        return fallback;

    return getFloat(value);
}

std::string ConfigOptions::getString(const std::string& value)
{
    checkOpened();
    return parser_.getString(value);
}

std::string ConfigOptions::getString(const std::string& value, const std::string& fallback)
{
    if(!hasOption(value))
        return fallback;

    return getString(value);
}

}
