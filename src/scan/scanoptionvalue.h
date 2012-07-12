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

#ifndef SCANOPTIONVALUE_H
#define SCANOPTIONVALUE_H

#include <stdexcept>
#include <string>
#include <boost/any.hpp>

#include "globus.h"

namespace cf {

class CLA_EXPO ScanOptionValue
{
public:
    typedef std::runtime_error TypeError;
public:
    ScanOptionValue();
    ScanOptionValue(bool value);
    ScanOptionValue(int value);
    ScanOptionValue(float value);
    ScanOptionValue(const std::string& value);

    bool getBool() const;
    int getInt() const;
    float getFloat() const;
    std::string getString() const;

    bool isBool() const;
    bool isInt() const;
    bool isFloat() const;
    bool isString() const;

    void reset();

    void set(bool value);
    void set(int value);
    void set(float value);
    void set(const std::string& value);
private:
    boost::any value_;
};

}

#endif // SCANOPTIONVALUE_H
