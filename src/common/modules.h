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
#include <string>
#include <cstddef>

#include "globus.h"

namespace cf {

typedef unsigned long long module_t;

static const module_t MODULE_NONE      = 0x0;
static const module_t MODULE_ALPHABETS = 0x1;
static const module_t MODULE_CCOM      = 0x2;
static const module_t MODULE_CED       = 0x4;
static const module_t MODULE_CIMAGE    = 0x8;
static const module_t MODULE_CLINE     = 0x10;
static const module_t MODULE_COMMON    = 0x20;
static const module_t MODULE_COMPAT    = 0x40;
static const module_t MODULE_CPAGE     = 0x80;
static const module_t MODULE_CPU       = 0x100;
static const module_t MODULE_CSTR      = 0x200;
static const module_t MODULE_CTB       = 0x400;
static const module_t MODULE_DIF       = 0x800;
static const module_t MODULE_EVN       = 0x1000;
static const module_t MODULE_EXC       = 0x2000;
static const module_t MODULE_EXPORT    = 0x4000;
static const module_t MODULE_FON       = 0x8000;
static const module_t MODULE_LEO       = 0x10000;
static const module_t MODULE_LNS       = 0x20000;
static const module_t MODULE_LOAD      = 0x40000;
static const module_t MODULE_LOC       = 0x80000;
static const module_t MODULE_MARKUP    = 0x100000;
static const module_t MODULE_MMX       = 0x200000;
static const module_t MODULE_MSK       = 0x400000;
static const module_t MODULE_PASS2     = 0x800000;
static const module_t MODULE_PUMA      = 0x1000000;
static const module_t MODULE_R35       = 0x2000000;
static const module_t MODULE_RBAL      = 0x4000000;
static const module_t MODULE_RBLOCK    = 0x8000000;
static const module_t MODULE_RCORKEGL  = 0x10000000;
static const module_t MODULE_RCUTP     = 0x20000000;
static const module_t MODULE_RDIB      = 0x40000000;
static const module_t MODULE_RFRMT     = 0x80000000;
static const module_t MODULE_RIMAGE    = 0x100000000LL;
static const module_t MODULE_RLINE     = 0x200000000LL;
static const module_t MODULE_RLING     = 0x400000000LL;
static const module_t MODULE_RNEG      = 0x800000000LL;
static const module_t MODULE_RPIC      = 0x1000000000LL;
static const module_t MODULE_RPSTR     = 0x2000000000LL;
static const module_t MODULE_RRECCOM   = 0x4000000000LL;
static const module_t MODULE_RSADD     = 0x8000000000LL;
static const module_t MODULE_RSELSTR   = 0x10000000000LL;
static const module_t MODULE_RSL       = 0x20000000000LL;
static const module_t MODULE_RSTR      = 0x40000000000LL;
static const module_t MODULE_RSTUFF    = 0x80000000000LL;
static const module_t MODULE_RVERLINE  = 0x100000000000LL;
static const module_t MODULE_SCAN      = 0x200000000000LL;
static const module_t MODULE_SHMEM     = 0x400000000000LL;
static const module_t MODULE_SMETRIC   = 0x800000000000LL;
static const module_t MODULE_STD       = 0x1000000000000LL;
static const module_t MODULE_USAGE     = 0x2000000000000LL;
static const module_t MODULES_ALL      = 0x4000000000000LL - 1;

FUN_EXPO__ std::string moduleToString(module_t m);

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
