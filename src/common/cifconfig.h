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

#include "globus.h"

namespace cf
{

class CLA_EXPO Config
{
        Config();
    public:
        enum debug_level_t {
            DEBUG_NONE = 0,
            DEBUG_LOW = 2,
            DEBUG_MEDIUM = 4,
            DEBUG_HIGH = 8
        };

        static Config& instance();

        bool debugDump() const;
        int debugLevel() const;
        bool debugLow() const;
        bool debugMedium() const;
        bool debugHigh() const;
        bool debug() const;
        void setDebug(bool value);
        void setDebugDump(bool value);
        void setDebugLevel(int level);
    private:
        bool debug_;
        bool dump_;
        int debug_level_;
};

}

#endif /* CIFCONFIG_H_ */
