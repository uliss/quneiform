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

#ifndef CONFIGOPTIONS_H
#define CONFIGOPTIONS_H

#include <string>

#include "configparser.h"

namespace cf {

class CLA_EXPO ConfigOptions
{
    ConfigOptions();
    ConfigOptions(const ConfigOptions&);
public:
    static bool getBool(const std::string& value);
    static bool getBool(const std::string& value, bool fallback);
    static int getInt(const std::string& value);
    static int getInt(const std::string& value, int fallback);
    static float getFloat(const std::string& value);
    static float getFloat(const std::string& value, float fallback);
    static std::string getString(const std::string& value);
    static std::string getString(const std::string&value, const std::string& fallback);

    static bool hasOption(const std::string& value);
private:
    static void checkOpened();
    static void open();
private:
    static ConfigParser parser_;
    static bool is_opened_;
};

}

#endif // CONFIGOPTIONS_H
