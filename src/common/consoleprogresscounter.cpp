/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <iostream>

#include "consoleprogresscounter.h"

namespace cf {

ConsoleProgressCounter::ConsoleProgressCounter(int width)
    :current_(0),
    width_(width)
{
    setCallback(this, &ConsoleProgressCounter::set);
}

void ConsoleProgressCounter::clear() {
    os() << std::string(width_ + 2, '\b') << std::flush;
}

std::ostream& ConsoleProgressCounter::os() {
    return std::cerr;
}

void ConsoleProgressCounter::print() {
    clear();

    os() << '['
            << std::string(current_ - 1, '=')
            << '>'
            << std::string(width_ - current_, ' ')
            << ']'
            << std::flush;

    if(current_ == width_)
        os() << std::endl;
}

void ConsoleProgressCounter::reset() {
    current_ = 0;
    os() << std::endl;
}

void ConsoleProgressCounter::set(unsigned char value) {
    int v = static_cast<int>((value / 100.0) * width_);

    if (v > width_)
        return;

    current_ = v;

    print();
}

}
