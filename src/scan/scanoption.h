/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef SCANOPTION_H
#define SCANOPTION_H

#include <string>

namespace cf {

class ScanOptionInfo;
class ScanOptionValue;

class ScanOption
{
public:
    ScanOption(const std::string& name);
    ~ScanOption();

    std::string name() const;
    void setName(const std::string& name);

    ScanOptionInfo * info();
    ScanOptionValue * value();
private:
    ScanOptionInfo * info_;
    ScanOptionValue * value_;
    std::string name_;
};

}

#endif // SCANOPTION_H
