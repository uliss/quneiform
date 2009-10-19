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

#ifndef CIFCONFIG_H_
#define CIFCONFIG_H_

#include "singleton.h"

namespace CIF {

class ConfigImpl
{
public:
    ConfigImpl() :
        debug_(true), debug_level_(DEBUG_HIGH) {
    }

    enum debug_level_t
    {
        DEBUG_NONE = 0,
        DEBUG_LOW = 2,
        DEBUG_MEDIUM = 4,
        DEBUG_HIGH = 8
    };

    bool debugLow() const {
        return debug() && debugLevel() >= DEBUG_LOW;
    }

    bool debugMedium() const {
        return debug() && debugLevel() >= DEBUG_MEDIUM;
    }

    bool debugHigh() const {
        return debug() && debugLevel() >= DEBUG_HIGH;
    }

    bool debug() const {
        return debug_;
    }

    int debugLevel() const {
        return debug_level_;
    }

    void setDebug(bool value) {
        debug_ = value;
        if (!debug_)
            debug_level_ = 0;
    }

    void setDebugLevel(int level) {
        debug_level_ = level;
    }
private:
    bool debug_;
    int debug_level_;
};

typedef Singleton<ConfigImpl> Config;

}

#endif /* CIFCONFIG_H_ */
