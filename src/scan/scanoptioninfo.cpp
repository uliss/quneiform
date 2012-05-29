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

#include "scanoptioninfo.h"

namespace cf {

ScanOptionInfo::ScanOptionInfo(Type t, Constraint c) :
    type_(t),
    constraint_(c)
{
}

bool ScanOptionInfo::hasConstraint() const
{
    return constraint_ != NO_CONSTRAINT;
}

ScanOptionInfo::Constraint ScanOptionInfo::constraint() const
{
    return constraint_;
}

std::string ScanOptionInfo::description() const
{
    return description_;
}

std::string ScanOptionInfo::title() const
{
    return title_;
}

ScanOptionInfo::Type ScanOptionInfo::type() const
{
    return type_;
}

void ScanOptionInfo::setConstraint(ScanOptionInfo::Constraint c)
{
    constraint_ = c;
    resetConstraints();
}

void ScanOptionInfo::setDescription(const std::string& descr)
{
    description_ = descr;
}

void ScanOptionInfo::setTitle(const std::string& title)
{
    title_ = title;
}

void ScanOptionInfo::setType(ScanOptionInfo::Type t)
{
    type_ = t;
    resetConstraints();
}

boost::any ScanOptionInfo::rangeMinValue() const
{
    return range_min_;
}

boost::any ScanOptionInfo::rangeMaxValue() const
{
    return range_max_;
}

void ScanOptionInfo::setRangeMinValue(const boost::any& value)
{
    if(constraint_ != RANGE)
        return;

    range_min_ = value;
}

void ScanOptionInfo::setRangeMaxValue(const boost::any& value)
{
    if(constraint_ != RANGE)
        return;

    range_max_ = value;
}

ScanOptionInfo::ValueList ScanOptionInfo::allowedValues() const
{
    return list_;
}

void ScanOptionInfo::appendAllowedValue(const boost::any& value)
{
    list_.push_back(value);
}

void ScanOptionInfo::setAllowedValues(const ScanOptionInfo::ValueList& values)
{
    if(constraint_ != LIST)
        return;

    list_ = values;
}

void ScanOptionInfo::resetConstraints()
{
    range_max_ = boost::any();
    range_min_ = boost::any();
    list_.clear();
}

}
