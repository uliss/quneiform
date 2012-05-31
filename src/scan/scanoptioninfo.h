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

#ifndef SCANOPTIONINFO_H
#define SCANOPTIONINFO_H

#include <string>
#include <iosfwd>
#include <boost/any.hpp>
#include <vector>

namespace cf {

class ScanOptionInfo
{
public:
    enum Type {
        UNKNOWN,
        BOOL,
        INT,
        FLOAT,
        STRING
    };

    enum Constraint {
        NO_CONSTRAINT,
        RANGE,
        LIST
    };

    enum Unit {
        UNIT_NONE = 0,
        UNIT_PIXEL,
        UNIT_BIT,
        UNIT_MM,
        UNIT_DPI,
        UNIT_PERCENT,
        UNIT_MICROSECOND
    };

    typedef std::vector<boost::any> ValueList;
public:
    ScanOptionInfo(Type t = UNKNOWN, Constraint c = NO_CONSTRAINT);

    bool hasConstraint() const;

    Constraint constraint() const;
    std::string description() const;
    std::string title() const;
    Type type() const;

    bool isBool() const;
    bool isInt() const;
    bool isFloat() const;
    bool isString() const;
    bool isConstraintList() const;
    bool isConstraintRange() const;

    void setConstraint(Constraint c);
    void setDescription(const std::string& descr);
    void setTitle(const std::string& title);
    void setType(Type t);

    boost::any rangeMinValue() const;
    boost::any rangeMaxValue() const;

    void setRangeMinValue(const boost::any& value);
    void setRangeMaxValue(const boost::any& value);

    ValueList allowedValues() const;
    void appendAllowedValue(const std::string& value);
    void appendAllowedValue(int value);
    void appendAllowedValue(float value);
    void clearAllowedValues();
    void setAllowedValues(const ValueList& values);

    Unit unit() const;
    void setUnit(Unit u);
private:
    void resetConstraints();
private:
    std::string title_;
    std::string description_;
    Type type_;
    Unit unit_;
    Constraint constraint_;
    boost::any range_min_;
    boost::any range_max_;
    ValueList list_;
};

}

std::ostream& operator<<(std::ostream& os, const cf::ScanOptionInfo& info);

#endif // SCANOPTIONINFO_H
