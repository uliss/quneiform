/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#ifndef FORMATDEBUG_H_
#define FORMATDEBUG_H_

namespace CIF
{

class FormatDebug
{
    public:
        enum debug_t
        {
            COMMON = 1,
            INTERVAL = (1 << 1),
            GEOMETRY = (1 << 2),
            HISTOGRAM = (1 << 3),
            FRAGMENT = (1 << 4),
            RELEASE = (1 << 5)
        };

        static void addFlag(debug_t flag) {
            flags_ |= flag;
        }

        static bool hasFlag(debug_t flag) {
            return flags_ & flag;
        }

    private:
        FormatDebug();
        static int flags_;
};

void FMT_DBG(FormatDebug::debug_t level, const char * format, ...);
void FMT_DBG(const char * format, ...);

}

#endif /* FORMATDEBUG_H_ */
