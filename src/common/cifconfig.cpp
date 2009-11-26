/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include "cifconfig.h"

namespace CIF
{

Config::Config() :
        debug_(true), dump_(false), debug_level_(DEBUG_HIGH)
{
}

bool Config::debug() const
{
    return debug_;
}

bool Config::debugDump() const
{
    return debug() && dump_;
}

int Config::debugLevel() const
{
    return debug_level_;
}

bool Config::debugLow() const
{
    return debug() && (debugLevel() >= DEBUG_LOW);
}

bool Config::debugMedium() const
{
    return debug() && (debugLevel() >= DEBUG_MEDIUM);
}

bool Config::debugHigh() const
{
    return debug() && (debugLevel() >= DEBUG_HIGH);
}

void Config::setDebug(bool value)
{
    debug_ = value;

    if (!debug_)
        debug_level_ = 0;
}

void Config::setDebugDump(bool value)
{
    dump_ = value;
}

void Config::setDebugLevel(int level)
{
    debug_level_ = level;
}

Config& Config::instance()
{
    static Config config;
    return config;
}
}
