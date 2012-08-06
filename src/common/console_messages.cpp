/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include "console_messages.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace cf {

namespace console {

static const char * color(color_t c) {
#ifndef _WIN32
    switch(c) {
    case NORMAL:
        return "\033[0m";
    case RED:
        return "\033[91m";
    case GREEN:
        return "\033[92m";
    case YELLOW:
        return "\033[93m";
    case BLUE:
        return "\033[94m";
    case PURPLE:
        return "\033[95m";
    case GRAY:
        return "\033[90m";
    case WHITE:
        return "\033[97m";
    default:
        return "\033[0m";
    }
#else
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);

    switch(c) {
    case NORMAL:
        SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    case RED:
        SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case GREEN:
        SetConsoleTextAttribute(hout, FOREGROUND_GREEN);
        break;
    case YELLOW:
        SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case BLUE:
        SetConsoleTextAttribute(hout, FOREGROUND_BLUE);
        break;
    case PURPLE:
        SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_BLUE);
        break;
    case GRAY:
        SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    case WHITE:
        SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        break;
    default:
        SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    }
    
    return "";
#endif
}

std::ostream& error(std::ostream& os) {
    os << message("[Error]", RED) << " ";
    return os;
}

std::ostream& warning(std::ostream& os) {
    os << message("[Warning]", BLUE) << " ";
    return os;
}

std::ostream& info(std::ostream& os) {
    os << message("[Info]", GREEN) << " ";
    return os;
}

}

}

std::ostream& operator<<(std::ostream& os, const cf::console::ColorMessage& msg) 
{
    // do not chain! this order required to WIN32 colors to work
    os << cf::console::color(msg.color); 
    os << msg.message;
    os << cf::console::color(cf::console::NORMAL);
    return os;
}
