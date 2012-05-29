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

#include "scanoption.h"
#include "scanoptioninfo.h"
#include "scanoptionvalue.h"

namespace cf {

ScanOption::ScanOption(const std::string& name) :
    info_(0),
    value_(0),
    name_(name)
{
}

ScanOption::~ScanOption()
{
    delete info_;
    delete value_;
}

std::string ScanOption::name() const
{
    return name_;
}

void ScanOption::setName(const std::string& name)
{
    name_ = name;
}

ScanOptionInfo * ScanOption::info()
{
    if(!info_)
        info_ = new ScanOptionInfo;

    return info_;
}

ScanOptionValue * ScanOption::value()
{
    if(!value_)
        value_ = new ScanOptionValue;

    return value_;
}

}
