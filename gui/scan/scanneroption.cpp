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

#include "scanneroption.h"

ScannerOption::ScannerOption() :
    type_(UNKNOWN),
    valid_(false)
{
}

QList<QVariant> ScannerOption::allowedValues() const
{
    return allowed_values_;
}

QString ScannerOption::name() const
{
    return name_;
}

void ScannerOption::setName(const QString& name)
{
    name_ = name;
}

QVariant ScannerOption::value() const
{
    return value_;
}

void ScannerOption::setValue(const QVariant& value)
{
    value_ = value;
}

bool ScannerOption::toBool() const
{
    return value_.toBool();
}

int ScannerOption::toInt() const
{
    return value_.toInt();
}

float ScannerOption::toFloat() const
{
    return value_.toFloat();
}

QString ScannerOption::toString() const
{
    return value_.toString();
}

ScannerOption::Type ScannerOption::type() const
{
    return type_;
}

void ScannerOption::setType(ScannerOption::Type t)
{
    type_ = t;
}

ScannerOption::Constraint ScannerOption::constraint() const
{
    return constraint_;
}

void ScannerOption::setConstraint(ScannerOption::Constraint c)
{
    constraint_ = c;

    if(c == CONSTRAINT_NONE)
        clearAllowedValues();
}

void ScannerOption::setAllowedValues(const QList<QVariant>& v)
{
    allowed_values_ = v;
}

void ScannerOption::clearRange()
{
    range_.clear();
}

const ScannerOptionRange& ScannerOption::range() const
{
    return range_;
}

void ScannerOption::setRange(const ScannerOptionRange& r)
{
    range_ = r;
}

bool ScannerOption::isBool() const
{
    return type_ == BOOL;
}

bool ScannerOption::isInt() const
{
    return type_ == INT;
}

bool ScannerOption::isFloat() const
{
    return type_ == FLOAT;
}

bool ScannerOption::isString() const
{
    return type_ == STRING;
}

bool ScannerOption::isValid() const
{
    return valid_;
}

void ScannerOption::setValid(bool value)
{
    valid_ = value;
}

void ScannerOption::addAllowedValue(const QVariant &value)
{
    allowed_values_.append(value);
}

void ScannerOption::clearAllowedValues()
{
    allowed_values_.clear();
}

bool ScannerOption::hadAllowedValues() const
{
    return !allowed_values_.empty();
}

