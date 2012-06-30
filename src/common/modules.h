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

#ifndef MODULES_H
#define MODULES_H

#include <stdexcept>

namespace cf {

enum module_t {
    MODULE_RSTR,
    MODULE_MSK
};

class ModuleInitException : public std::runtime_error
{
public:
    ModuleInitException(const std::string& msg, module_t m, int code = 0) :
        std::runtime_error(msg),
        type_(m),
        err_code_(code)
    {}

    int code() const { return err_code_; }
    module_t module() const { return type_; }
private:
    module_t type_;
    int err_code_;
};


}

#endif // MODULES_H
