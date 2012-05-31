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

#include <sstream>

#include "scanoption.h"
#include "scanoptioninfo.h"
#include "scanoptionvalue.h"
#include "scan_debug.h"

namespace cf {

ScanOption::ScanOption(const std::string& name) :
    name_(name),
    enabled_(true)
{
}

ScanOption::~ScanOption()
{}

bool ScanOption::isEnabled() const
{
    return enabled_;
}

void ScanOption::setEnabled(bool value)
{
    enabled_ = value;
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
    return (ScanOptionInfo *) ((const ScanOption*)this)->info();
}

const ScanOptionInfo * ScanOption::info() const
{
    if(!info_)
        info_.reset(new ScanOptionInfo);

    return info_.get();
}

ScanOptionValue * ScanOption::value()
{
    return (ScanOptionValue *) ((const ScanOption*) this)->value();
}

const ScanOptionValue * ScanOption::value() const
{
    if(!value_)
        value_.reset(new ScanOptionValue);

    return value_.get();
}

bool ScanOption::setValue(bool v)
{
    if(!info()->isBool())
        return false;

    value()->set(v);
    return true;
}

template<class T>
static bool inList(const T& value, const ScanOptionInfo::ValueList& lst)
{
    for(size_t i = 0; i < lst.size(); i++) {
        boost::any t = lst[i];
        if(t.empty())
            continue;

        if(value == boost::any_cast<T>(t))
            return true;
    }

    return false;
}

template<class T>
static bool inRange(const T& value, const boost::any& min, const boost::any& max, T * pmin, T * pmax)
{
    if(!min.empty()) {
        T vmin = boost::any_cast<T>(min);
        if(pmin)
            *pmin = vmin;

        if(value < vmin)
            return false;
    }

    if(!max.empty()) {
        T vmax = boost::any_cast<T>(max);
        if(pmax)
            *pmax = vmax;

        if(vmax <= value)
            return false;
    }

    return true;
}

template<class T>
static bool setRangeValue(const T& v, ScanOptionInfo * info, ScanOptionValue * value)
{
    T min = 0, max = 0;

    if(inRange<T>(v, info->rangeMinValue(), info->rangeMaxValue(), &min, &max)) {
        value->set(v);
        return true;
    }
    else {
        SCANNER_WARNING << "given value (" << v << ") not in range ["
                        << min << "," << max << ")\n";
        return false;
    }
}

template<class T>
static bool setListValue(const T& v, ScanOptionInfo * info, ScanOptionValue * value)
{
    if(inList<T>(v, info->allowedValues())) {
        value->set(v);
        return true;
    }
    else {
        SCANNER_ERROR << "invalid value (" << v << ") not in list\n";
        return false;
    }
}

bool ScanOption::setValue(int v)
{
    if(!info()->isInt()) {
        SCANNER_ERROR << "not int value\n";
        return false;
    }

    if(!info()->hasConstraint()) {
        value()->set(v);
        return true;
    }

    if(info()->isConstraintRange())
        return setRangeValue(v, info(), value());

    if(info()->isConstraintList())
        return setListValue(v, info(), value());

    return false;
}

bool ScanOption::setValue(float v)
{
    if(!info()->isFloat()){
        SCANNER_ERROR << "not float value\n";
        return false;
    }

    if(!info()->hasConstraint()) {
        value()->set(v);
        return true;
    }

    if(info()->isConstraintRange())
        return setRangeValue(v, info(), value());

    if(info()->isConstraintList())
        return setListValue(v, info(), value());

    return false;
}

bool ScanOption::setValue(const std::string& v)
{
    if(!info()->isString()) {
        SCANNER_ERROR << "not string value\n";
        return false;
    }

    if(!info()->hasConstraint()) {
        value()->set(v);
        return true;
    }

    if(info()->isConstraintList())
        return setListValue(v, info(), value());

    return false;
}

}

static std::string toString(const cf::ScanOptionValue * v, cf::ScanOptionInfo::Unit unit)
{
    using namespace cf;

    std::ostringstream buf;

    if(v->isBool()) {
        if(v->getBool())
            buf << "true";
        else
            buf << "false";
    }

    if(v->isInt())
        buf << v->getInt();

    if(v->isFloat())
        buf << v->getFloat();

    if(v->isString())
        buf << '"' << v->getString() << '"';

    switch(unit) {
    case ScanOptionInfo::UNIT_BIT:
        buf << " bit";
        break;
    case ScanOptionInfo::UNIT_DPI:
        buf << " dpi";
        break;
    case ScanOptionInfo::UNIT_MM:
        buf << " mm";
        break;
    case ScanOptionInfo::UNIT_PERCENT:
        buf << "%";
        break;
    case ScanOptionInfo::UNIT_PIXEL:
        buf << "px";
        break;
    case ScanOptionInfo::UNIT_MICROSECOND:
        buf << "us";
        break;
    default:
        break;
    }

    return buf.str();
}

std::ostream& operator<<(std::ostream& os, const cf::ScanOption& opt)
{
    os << opt.name();

    if(opt.isEnabled())
        os << "***";

    os << "\n";
    os << (*opt.info());
    os << "\t value:       " << toString(opt.value(), opt.info()->unit()) << "\n";
    return os;
}
