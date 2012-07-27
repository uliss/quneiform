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

#include <cstdlib>
#include <cassert>

#include "rselstr_internal.h"

ROOT * root_file = 0;
int nRoots = 0;
size_t root_capacity = 0;

bool rootIsNull()
{
    return root_file == 0;
}

bool rootIsEmpty()
{
    return nRoots == 0;
}

ROOT * rootAt(int idx)
{
    return &root_file[idx];
}

ROOT * rootLast()
{
    if(rootCount() < 1)
        return 0;

    return rootAt(rootCount() - 1);
}

ROOT * rootFirst()
{
    return root_file;
}

int rootCount()
{
    return nRoots;
}

void rootFree()
{
    free(root_file);
    root_file = 0;
    nRoots = 0;
}

void rootAdd(const ROOT& r)
{
    assert(nRoots >= 0);

    if(nRoots >= root_capacity)
        rootReserve(10);

    nRoots++;
    root_file[nRoots - 1] = r;
}

ROOT * rootReserve(int number)
{
    root_capacity = number * sizeof(ROOT);

    root_file = (ROOT*) realloc(root_file, root_capacity);
    return root_file;

}
