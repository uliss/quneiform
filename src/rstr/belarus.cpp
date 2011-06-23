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

/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/********************************************************
 *                                                      *
 * 		 Special programs for ‰	                 *
 *                                                      *
 ********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "func.h"
#include "struct.h"
#include "linear.h"
#include "linutil.h"
#include "ligas.h"
#include "func.h"
#include "minmax.h"
#include "belarus.h"

// Discrim © by base lines

static int rec_shortu(cell* c,cell * cap)
{
    cell *clist[8];
    uchar let;
    let = c->vers[0].let;
    if(cap->row > c->row)        return 0; // not a cap
    if(cap->w * 3 < c->w)          return 0; // not so wide as need
    if(cap->w < 4 || cap->h < 3) return 0; // just dot
    if(cap->col < c->col-2)      return 0; // left dust
    if(c->row > cap->row + cap->h &&
            (c->row - (cap->row + cap->h)) > c->h / 2 ) return 0; // dust lay so high

    if((let == (uchar)'\xE3') && (c->pos_inc & erect_rot))
        // 'u' with cap
        if(c->col + c->w/2 < cap->col ||
                c->col + c->w / 2 > cap->col + cap->w) return 0;  // not centered dust

    clist[0] = c;
    clist[1] = cap;
    if(!compose_cell(2, clist, c))
        return -1; //OLEG:new return style of composed
    let = is_lower(let) ? (uchar) u_bel : (uchar) U_bel;
    c->vers[0].let = let;
    c->vers[0].prob = MIN(254, c->vers[0].prob + 2);
    c->recsource = 0; // artifact
    c->dens = 255; // undef
    c->nvers = 1;
    c->vers[1].let = c->vers[1].prob = 0;
    return 1;
}

static int rec_shortu_halo(cell * c)
{
#define n_pieces        48
    cell *cap, *caplist[n_pieces];
    uchar let;
    int16_t i, cap_row, cap_col, cap_h, cap_w, cap_rt, cap_bt, ncaps;
    i = 0;
    cap = c;
    while((cap=cap->next) != NULL && cap != c->nextl && i < n_pieces - 1)
        if( cap->flg & c_f_dust) caplist[i++] = cap;

    cap = c;

    while((cap = cap->prev) != NULL && cap != c->prevl && i < n_pieces - 1)
        if(cap->flg & c_f_dust) caplist[i++] = cap;

    caplist[i] = NULL;
    ncaps = i;
    i = 0;

    while(cap = caplist[i]) { //  validate halo
        if(cap->row > c->row)
            goto delcap; // inproper place

        if(cap->h == 1 && cap->w == 1)
            goto delcap; // just dot

        i++;
        continue;
delcap:
        memmove(&caplist[i], &caplist[i + 1], (ncaps - i) * sizeof(cell*));
        ncaps--;
    }

    if(ncaps < 2)
        return 0;

    cap_row = cap_col = 32000;
    cap_rt = cap_bt = 0;
    i = 0;

    while(cap = caplist[i]) {
        i++;
        cap_row = MIN(cap_row, cap->row);
        cap_col = MIN(cap_col, cap->col);
        cap_rt = MAX(cap_rt, cap->col + cap->w);
        cap_bt = MAX(cap_bt, cap->row + cap->h);
    }
    cap_h = cap_bt-cap_row;
    cap_w = cap_rt-cap_col;

    let = c->vers[0].let;
    if(cap_row > c->row)        return 0; // not a cap
    if(cap_w * 3 < c->w)          return 0; // not so wide as need
    if(cap_w > c->w)            return 0; // too wide
    if(cap_w < 4 || cap_h < 3)  return 0; // just dot
    if(cap_col < c->col-2)      return 0; // left dust
    if(c->row > cap_row + cap_h &&
            (c->row - (cap_row + cap_h)) > c->h / 2 ) return 0; // dust lay so high

    if(let == (uchar)'\xE3' && (c->pos_inc & erect_rot))
        // 'u' with cap
        if(c->col + c->w / 2 < cap_col ||
                c->col + c->w / 2 > cap_col + cap_w) return 0;  // not centered dust


    memmove(&caplist[1], caplist, i * sizeof(cell*));
    caplist[0]=c;
    if(!compose_cell((int16_t)(i + 1), caplist, c) )
        return 0; //OLEG:new return style of composed
    let = is_lower(let) ? (uchar)'\xA9' : (uchar)'\x89';
    c->vers[0].let = let;
    c->vers[0].prob = MIN(254, c->vers[0].prob + 2);
    c->recsource = 0; // artifact
    c->dens = 255; // undef
    c->nvers=1;
    c->vers[1].let = c->vers[1].prob = 0;
    return 1;
}

// Go by string and recog 'ч'
void cf::proc_shortu()
{
    cell * c,*cap;
    uchar let;
    int16_t ndust;
    c = cell_f();
    while((c = c->nextl) != NULL ){
        if(!(c->flg & (c_f_let + c_f_bad)))
            continue;

        let = c->vers[0].let; ndust = 0;
        if(!memchr("\xE3\x93", let, 2))
            continue;

        cap = c;
        while((cap=cap->next) != NULL && cap != c->nextl) {
            if(cap->flg & c_f_dust) {
                ndust++;
                switch(rec_shortu(c,cap)){
                case 1:
                    goto next_let;
                case -1:
                    return;
                case 0:
                    break;
                }
            }
        }

        cap = c;

        while((cap = cap->prev) != NULL && cap != c->prevl) {
            if(cap->flg & c_f_dust) {
                ndust++;
                //if(rec_shortu(c,cap))goto next_let;
                switch(rec_shortu(c,cap)) {
                case 1:
                    goto next_let;
                case -1:
                    return;
                case 0:
                    break;
                }
            }
        }

        if(ndust > 1)
            rec_shortu_halo(c); // many dusts, try all together
        next_let: ;
    } // while by letters
}
