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

#include <stdarg.h>
#include <stdio.h>

#include "formatdebug.h"
#include "common/debug.h"

namespace CIF
{

int FormatDebug::flags_ = FormatDebug::COMMON;

#ifdef NDEBUG
void FMT_DBG(FormatDebug::debug_t, const char *, ...) {}
void FMT_DBG(const char *, ...) {}
#else

void FMT_DBG(const char * format, ...) {
    va_list list;
    va_start(list, format);
    FMT_DBG(FormatDebug::COMMON, format, list);
    va_end(list);
}

void FMT_DBG(FormatDebug::debug_t level, const char * format, ...) {
    if (format == NULL)
        return;
    if (FormatDebug::hasFlag(level))
        return;

    static char strbuf[4096]; // string to be put
    va_list list;
    va_start(list, format);
    vsnprintf(strbuf, sizeof(strbuf) - 1, format, list);
    va_end(list);
    Debug() << strbuf << "\n";
}
#endif

}
