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

}

static std::string toString(const cf::ScanOptionValue * v)
{
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

    return buf.str();
}

std::ostream& operator<<(std::ostream& os, const cf::ScanOption& opt)
{
    os << opt.name();

    if(opt.isEnabled())
        os << "***";

    os << "\n";
    os << (*opt.info());
    os << "\t value:       " << toString(opt.value()) << "\n";
    return os;
}
