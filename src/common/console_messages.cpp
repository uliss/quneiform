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
    default:
        return "\033[0m";
    }
#else
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

std::ostream& operator<<(std::ostream& os, const cf::console::ColorMessage& msg) {
    os << cf::console::color(msg.color) << msg.message << cf::console::color(cf::console::NORMAL);
    return os;
}
