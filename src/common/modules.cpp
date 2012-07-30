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

#include <cassert>

#include "modules.h"

namespace cf
{

namespace
{

const char * module_names[] = {
    "NONE",
    "APLHABETS",
    "CCOM",
    "CED",
    "CIMAGE",
    "CLINE",
    "COMMON",
    "COMPAT",
    "CPAGE",
    "CPU",
    "CSTR",
    "CTB",
    "DIF",
    "EVN",
    "EXC",
    "EXPORT",
    "FON",
    "LEO",
    "LNS",
    "LOAD",
    "LOC",
    "MARKUP",
    "MMX",
    "MSK",
    "PASS2",
    "PUMA",
    "R35",
    "RBAL",
    "RBLOCK",
    "RCORKEGL",
    "RCUTP",
    "RDIB",
    "RFRMT",
    "RIMAGE",
    "RLINE",
    "RLING",
    "RNEG",
    "RPIC",
    "RPSTR",
    "RRECCOM",
    "RSADD",
    "RSELSTR",
    "RSHELLLINES",
    "RSTR",
    "RSTUFF",
    "RVERLINE",
    "RSCAN",
    "SHMEM",
    "SMETRIC",
    "STD",
    "USAGE",
    "ALL"
};

inline int module_names_size()
{
    return sizeof(module_names)/sizeof(const char*);
}

typedef unsigned long long ullong;

int bitPos(module_t m)
{
    for(uint i = 0; i <= 64; i++) {
        if(m & static_cast<ullong>(static_cast<ullong>(1L) << i))
            return (int) i;
    }

    return -1;
}

}

std::string moduleToString(module_t m)
{
    if(m == MODULES_ALL)
        return "ALL";

    int bit_pos = bitPos(m) + 1;
    if(bit_pos >= module_names_size())
        return std::string("INVALID");

    assert(bit_pos >= 0);

    return module_names[bit_pos];
}

}
