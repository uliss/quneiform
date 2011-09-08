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

#ifndef CONSOLEPROGRESSCOUNTER_H
#define CONSOLEPROGRESSCOUNTER_H

#include <iosfwd>

#include "globus.h"
#include "percentcounter.h"

namespace cf {

class CLA_EXPO ConsoleProgressCounter : public PercentCounter
{
public:
    ConsoleProgressCounter(int width = 78);
    void reset();
private:
    void clear();
    std::ostream& os();
    void print();
    void set(unsigned char value);
private:
    int current_;
    int width_;
};

}

#endif // CONSOLEPROGRESSCOUNTER_H
