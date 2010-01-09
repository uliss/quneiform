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

#include "common/interval.h"

static void comptorast(puchar raster, cell *C)
{
    memcpy(raster, save_raster(C), ((C -> w + 7) / 8) * C -> h);
}

static void make_hor_vert(cell *cl)
{
    int16_t i, y;
    interval *c;
    struct statist *h, *v;

    memset(hor, 0, sizeof(*hor) * ((dx + 7) & 0xF8));
    memset(vert, 0, sizeof(*vert) * dy);

    c = (interval *) (((puchar) (cl -> env)) + cl -> env -> lines + sizeof(int16_t));

    while (((lnhead *) c) -> lth) {
        y = dy - 1 - (((lnhead *) c)) -> row;
        c = (interval *) ((lnhead *) c + 1);
        v = vert + y;
        while (c -> l) {
            if (v->sum) {
                if (v -> top <= c -> e)
                    v -> top = c -> e;
                if (v -> bot >= c -> e - c -> l)
                    v -> bot = c -> e - c -> l;
            }
            else {
                v -> top = c -> e;
                v -> bot = c -> e - c -> l;
            }
            (v--) -> sum += c -> l;
            for (h = hor + c -> e - 1, i = c -> l; i > 0; i--, ((h--) -> sum)++)
                if (h -> sum) {
                    if (h -> bot >= y)
                        h -> bot = (uchar) y;
                    else if (h -> top <= y)
                        h -> top = (uchar) y;
                }
                else {
                    h -> top = h -> bot = (uchar) y;
                }
            c++;
            y--;
        }
        c++;
    }
}

static void make_hist()
{
    int16_t i;

    memset(hist[0], 0, MAX_HOR_IL1 * N_HIST);
    for (i = 0; i < dy; i++) {
        hist[0][i] = vert[i]. sum;
        hist[1][i] = vert[i]. top - vert[i]. bot;
        hist[2][i] = vert[i]. top - vert[i]. bot - vert[i]. sum;
        hist[3][i] = dx - vert[i]. bot;
        hist[4][i] = vert[i]. top;
        hist[10][i] = vert[i].bot; // left abris  ( as in ABRIS.C )
        hist[11][i] = dx - vert[i].top; // right abris ( as in ABRIS.C )
    }

    for (i = 0; i < dx; i++) {
        hist[5][i] = hor[i]. sum;
        hist[6][i] = hor[i]. top - hor[i]. bot + 1;
        hist[7][i] = hor[i]. top - hor[i]. bot + 1 - hor[i]. sum;
        hist[8][i] = dy - hor[i]. bot;
        hist[9][i] = hor[i]. top + 1;
    }
}

static void make_func()
{
    int16_t i, m;

    memset(funcs[0], 0, MAX_NEW_FUNC * MAX_HOR_IL1);
    for (i = 0; i < dy; i++)
        funcs[1][i] = dx - hist[3][i];
    for (i = 0; i < dy; i++)
        funcs[2][i] = dx - hist[4][i];
    for (i = 0; i < dy; i++)
        funcs[3][i] = abs(hist[3][i] - hist[4][i]);
    for (i = 0; i < dy; i++)
        funcs[4][i] = abs(hist[3][dy - 1 - i] - hist[4][i]);
    for (i = 0; i < dy; i++)
        funcs[5][i] = hist[1][i];
    //    for (i = 0; i < dx; i++)    funcs [6][i] = hist [5][i];
    //    for (i = 0; i < dx; i++)    funcs [7][i] = hist [6][i];
    m = gmax(hist[1], 0, (int16_t) (dy - 1));
    for (i = 0; i < dy; i++)
        funcs[8][i] = m - hist[1][i];
    nfunc = 8;
}

static int16_t line_width(puchar func, int16_t beg, int16_t end)
{
    int16_t i;
    int16_t max = 0;
    int16_t gmax = 0;
    int16_t lines_value = 0;
    int16_t count[MAX_HOR_IL1];

    memset(count, 0, sizeof(count));
    for (i = beg; i < end; i++) {
        if (func[i] > gmax)
            gmax = func[i];
        count[func[i]]++;
    }
    for (i = 0; i <= gmax; i++)
        if (count[i] > max) {
            max = count[i];
            lines_value = i;
        }
    return lines_value;
}

static uint16_t integral(puchar func, int16_t from, int16_t to, int16_t threshold)
{
    uint16_t s;
    uint32_t i;

    s = 0;
    for (i = from; i <= (uint32_t) to; i++)
        s += MAX(0, func[i] - threshold);
    return s;
}

static int16_t gmax(puchar func, int16_t from, int16_t to)
{
    uint16_t s;
    int16_t i;

    s = 0;
    for (i = from; i <= to; i++)
        if (func[i] > s) {
            s = func[i];
            gmax_y = i;
        }
    return s;
}

static int16_t gmin(puchar func, int16_t from, int16_t to)
{
    uint16_t s;
    int16_t i;

    s = MAX_HOR_IL1;
    for (i = from; i <= to; i++)
        if (func[i] < s) {
            s = func[i];
            gmin_y = i;
        }
    return s;
}

static int16_t vertsym(int16_t bound)
{
    if (gmax(funcs[3], (int16_t) (dy * 6 / 100), (int16_t) (dy * 94 / 100)) <= bound)
        return 1;
    return 0;
}

static int16_t centrsym(int16_t bound)
{
    if (gmax(funcs[4], (int16_t) (dy * 6 / 100), (int16_t) (dy * 94 / 100)) <= bound)
        return 1;
    return 0;
}

static Bool increase(puchar f, int16_t from, int16_t to)
{
    int16_t i;

    for (i = from; i < to; i++)
        if (f[i] > f[i + 1])
            return FALSE;
    if (f[from] == f[to])
        return FALSE;
    return TRUE;
}
