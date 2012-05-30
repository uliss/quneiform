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

#include "scanoptionvalue.h"

namespace cf {

ScanOptionValue::ScanOptionValue()
{
}

ScanOptionValue::ScanOptionValue(bool value)
{
    value_ = value;
}

ScanOptionValue::ScanOptionValue(int value)
{
    value_ = value;
}

ScanOptionValue::ScanOptionValue(float value)
{
    value_ = value;
}

ScanOptionValue::ScanOptionValue(const std::string& value)
{
    value_ = value;
}

bool ScanOptionValue::getBool() const
{
    if(!isBool())
        throw TypeError("not bool option");

    return boost::any_cast<bool>(value_);
}

int ScanOptionValue::getInt() const
{
    if(!isInt())
        throw TypeError("not int option");

    return boost::any_cast<int>(value_);
}

float ScanOptionValue::getFloat() const
{
    if(!isFloat())
        throw TypeError("not float option");

    return boost::any_cast<float>(value_);
}

std::string ScanOptionValue::getString() const
{
    if(!isString())
        throw TypeError("not string option");

    return boost::any_cast<std::string>(value_);
}

bool ScanOptionValue::isBool() const
{
    return value_.type() == typeid(bool);
}

bool ScanOptionValue::isInt() const
{
    return value_.type() == typeid(int);
}

bool ScanOptionValue::isFloat() const
{
    return value_.type() == typeid(float);
}

bool ScanOptionValue::isString() const
{
    return boost::any_cast<std::string>(&value_);
}

void ScanOptionValue::reset()
{
    value_ = boost::any();
}

void ScanOptionValue::set(bool value)
{
    value_ = value;
}

void ScanOptionValue::set(int value)
{
    value_ = value;
}

void ScanOptionValue::set(float value)
{
    value_ = value;
}

void ScanOptionValue::set(const std::string& value)
{
    value_ = value;
}

}
