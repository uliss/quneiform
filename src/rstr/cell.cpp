/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <cstddef>
#include <cassert>
#include <algorithm>
#include "cell.h"
#include "ligas.h" // for bad_char
#include "func.h"

cell::cell() :
        row(0),
        col(0),
        h(0),
        w(0),
        env(NULL),
        next(NULL),
        prev(NULL),
        nextl(NULL),
        prevl(NULL),
        bdiff(0),
        difflg(0),
        basflg(0),
        bas1(0),
        bas2(0),
        bas3(0),
        bas4(0),
        cg_flag(0),
        r_row(0),
        r_col(0),
        nvers(0),
        flg(0),
        cpos(0),
        reasno(0),
        keg(0),
        font(0),
        dens(0),
        recsource(0),
        stick_inc(0),
        complist(NULL),
        left(0),
        right(0),
        bas_acc(0),
        shape(0),
        rus_eng_word(0),
        broken_II(0),
        language(0),
        pos_inc(erect_no),
        cg_flag_fine(0),
        accent_leader(0),
        save_stick_inc(0),
        clink(-1),
        clu_attr(0),
        bottom_accent(0),
        n_baton(NO_BATONS),
        dup(dup_default),
        dupstart(NULL),
        dupend(NULL),
        dlang_dup(0),
        font_new(0),
        history(0),
        solid_no(0),
        flg_new(0),
        nClust(0)
{
    pr_vers.let = bad_char;
    pr_vers.prob = 0;
}

void cell::removeVersion(int letter) {
    if (nvers < 1)
        return;

    for(int i = 0; i < nvers; i++) {
        if (vers[i].let == letter) {
            for (i++; i < nvers; i++)
                vers[i - 1] = vers[i];

            vers[i].let = vers[i].prob = 0;
            nvers--;
            return;
        }
    }
}

void cell::setErection(int inc) {
    if(inc != NO_INCLINE && hasEnv()) {
        stick_inc = inc;
        pos_inc = inc ? erect_rot : erect_zero;
    }
}

static bool versionCompare(cf::version v1, cf::version v2) {
    return v1.prob > v2.prob;
}

static bool isValidVersion(cf::version v) {
    return v.prob > 0;
}

void cell::sortVersions() {
    assert(nvers <= VERS_IN_CELL);

    if(nvers <= 0) {
        set_bad_cell(this);
        return;
    }

    std::sort(vers, vers + nvers, versionCompare);
    nvers = std::count_if(vers, vers + nvers, isValidVersion);

    if(!nvers)
        set_bad_cell(this);
}

