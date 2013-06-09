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

#include <iostream>
#include <sstream>

#include "scanoptioninfo.h"

namespace cf {

ScanOptionInfo::ScanOptionInfo(Type t, Constraint c) :
    type_(t),
    index_(0),
    unit_(UNIT_NONE),
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

bool ScanOptionInfo::isBool() const
{
    return type_ == BOOL;
}

bool ScanOptionInfo::isInt() const
{
    return type_ == INT;
}

bool ScanOptionInfo::isFloat() const
{
    return type_ == FLOAT;
}

bool ScanOptionInfo::isString() const
{
    return type_ == STRING;
}

bool ScanOptionInfo::isConstraintList() const
{
    return constraint_ == LIST;
}

bool ScanOptionInfo::isConstraintRange() const
{
    return constraint_ == RANGE;
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

int ScanOptionInfo::index() const
{
    return index_;
}

void ScanOptionInfo::setIndex(int idx)
{
    index_ = idx;
}

ScanOptionInfo::Value ScanOptionInfo::rangeMinValue() const
{
    return range_min_;
}

ScanOptionInfo::Value ScanOptionInfo::rangeMaxValue() const
{
    return range_max_;
}

void ScanOptionInfo::setRangeMinValue(const Value &value)
{
    if(constraint_ != RANGE)
        return;

    range_min_ = value;
}

void ScanOptionInfo::setRangeMaxValue(const Value &value)
{
    if(constraint_ != RANGE)
        return;

    range_max_ = value;
}

ScanOptionInfo::ValueList ScanOptionInfo::allowedValues() const
{
    return list_;
}

void ScanOptionInfo::appendAllowedValue(const std::string &value)
{
    list_.push_back(value);
}

void ScanOptionInfo::appendAllowedValue(int value)
{
    list_.push_back(value);
}

void ScanOptionInfo::appendAllowedValue(float value)
{
    list_.push_back(value);
}

void ScanOptionInfo::clearAllowedValues()
{
    list_.clear();
}

void ScanOptionInfo::setAllowedValues(const ScanOptionInfo::ValueList& values)
{
    if(constraint_ != LIST)
        return;

    list_ = values;
}

ScanOptionInfo::Unit ScanOptionInfo::unit() const
{
    return unit_;
}

void ScanOptionInfo::setUnit(ScanOptionInfo::Unit u)
{
    unit_ = u;
}

void ScanOptionInfo::resetConstraints()
{
    range_max_ = boost::any();
    range_min_ = boost::any();
    list_.clear();
}

static inline std::string toString(cf::ScanOptionInfo::Type t)
{
    using namespace cf;
    switch(t) {
    case ScanOptionInfo::BOOL:
        return "bool";
    case ScanOptionInfo::INT:
        return "integer";
    case ScanOptionInfo::FLOAT:
        return "float";
    case ScanOptionInfo::STRING:
        return "string";
    default:
        return "";
    }
}

static inline std::string valueToString(const boost::any& v, cf::ScanOptionInfo::Type t)
{
    if(v.empty())
        return "";

    std::ostringstream buf;

    switch(t) {
    case cf::ScanOptionInfo::INT:
        buf << boost::any_cast<int>(v);
        break;
    case cf::ScanOptionInfo::FLOAT:
        buf << boost::any_cast<float>(v);
        break;
    case cf::ScanOptionInfo::STRING:
        buf << boost::any_cast<std::string>(v);
        break;
    case cf::ScanOptionInfo::BOOL:
        buf << boost::any_cast<bool>(v);
        break;
    default:
        break;
    }

    return buf.str();
}

static inline std::string rangeToString(const cf::ScanOptionInfo& info)
{
    std::ostringstream buf;
    buf << "[";
    buf << valueToString(info.rangeMinValue(), info.type());
    buf << "...";
    buf << valueToString(info.rangeMaxValue(), info.type());
    buf << "]";
    return buf.str();
}

static inline std::string listToString(const cf::ScanOptionInfo& info)
{
    std::ostringstream buf;
    buf << '[';
    cf::ScanOptionInfo::ValueList values = info.allowedValues();

    for(size_t i = 0; i < values.size(); i++) {
        if(i != 0)
            buf << '|';

        buf << valueToString(values.at(i), info.type());
    }

    buf << ']';
    return buf.str();
}

std::ostream& operator<<(std::ostream& os, const cf::ScanOptionInfo& info)
{
    os << "\t title:       " << info.title() << "\n";
    os << "\t description: " << info.description() << "\n";
    os << "\t type:        " << toString(info.type()) << "\n";
    switch(info.constraint()) {
    case cf::ScanOptionInfo::RANGE:
        os << "\t range:       " << rangeToString(info) << "\n";
        break;
    case cf::ScanOptionInfo::LIST:
        os << "\t list:        " << listToString(info) << "\n";
    default:
        break;
    }
    return os;
}

}
