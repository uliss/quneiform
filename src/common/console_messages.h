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

#ifndef CONSOLE_MESSAGES_H
#define CONSOLE_MESSAGES_H

#include <string>
#include <iostream>

#include "globus.h"

namespace cf {

namespace console {

enum color_t {
    NORMAL = 0,
    RED = 91,
    GREEN = 92,
    BLUE = 94,
    YELLOW = 93
};

struct ColorMessage {
    ColorMessage(const std::string& msg, color_t c) : message(msg), color(c) {}
    std::string message;
    color_t color;
};

inline ColorMessage message(const std::string& msg, color_t c) {
    return ColorMessage(msg, c);
}

FUN_EXPO__ std::ostream& error(std::ostream& os);
FUN_EXPO__ std::ostream& warning(std::ostream& os);
FUN_EXPO__ std::ostream& info(std::ostream& os);

}

}

FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const cf::console::ColorMessage& msg);

#endif // CONSOLE_MESSAGES_H
