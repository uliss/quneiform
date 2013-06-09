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
#include <boost/shared_ptr.hpp>

#include "globus.h"

namespace cf {

class ScanOptionInfo;
class ScanOptionValue;

class CLA_EXPO ScanOption
{
public:
    ScanOption(const std::string& name);
    ~ScanOption();

    void enable() { setEnabled(true); }
    void disable() { setEnabled(false); }
    bool isEnabled() const;
    void setEnabled(bool value = true);

    std::string name() const;
    void setName(const std::string& name);

    ScanOptionInfo * info();
    const ScanOptionInfo * info() const;
    ScanOptionValue * value();
    const ScanOptionValue * value() const;

    bool setValue(bool v);
    bool setValue(int v);
    bool setValue(float v);
    bool setValue(const std::string& v);
private:
    mutable boost::shared_ptr<ScanOptionInfo> info_;
    mutable boost::shared_ptr<ScanOptionValue> value_;
    std::string name_;
    bool enabled_;
};

std::ostream& operator<<(std::ostream& os, const ScanOption& opt);

}

#endif // SCANOPTION_H
