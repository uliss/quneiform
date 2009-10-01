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


static void comptorast (puchar raster, cell *C)
{
    memcpy (raster, save_raster (C), ((C -> w + 7)/8) * C -> h);
}

static void make_hor_vert (cell *cl)
{
    int16_t             i, y;
    interval        *c;
    struct statist  *h, *v;

    memset (hor,  0, sizeof (*hor)  * ((dx + 7) & 0xF8));
    memset (vert, 0, sizeof (*vert) * dy);

    c = (interval *)(((puchar) (cl -> env)) + cl -> env -> lines + sizeof (int16_t));

    while (((lnhead *) c) -> lth)
    {
        y = dy - 1 - (((lnhead *) c)) -> row;
        c = (interval *)((lnhead *) c + 1);
        v = vert + y;
        while (c -> l)
        {
            if (v->sum)
            {
                if (v -> top <= c -> e)             v -> top = c -> e;
                if (v -> bot >= c -> e - c -> l)    v -> bot = c -> e - c -> l;
            }
            else
            {
                v -> top = c -> e;
                v -> bot = c -> e - c -> l;
            }
            (v--) -> sum += c -> l;
            for (h = hor + c -> e - 1, i = c -> l; i > 0; i--, ((h--) -> sum)++)
            if (h -> sum)
            {
                if      (h -> bot >= y) h -> bot = (uchar)y;
                else if (h -> top <= y) h -> top = (uchar)y;
            }
            else
            {
                h -> top = h -> bot = (uchar)y;
            }
            c ++;
            y --;
        }
        c ++;
    }
}

static void make_hist ()
{
    int16_t i;

    memset (hist [0], 0, MAX_HOR_IL1 * N_HIST);
    for (i = 0; i < dy; i++)
    {
        hist [0] [i] = vert [i]. sum;
        hist [1] [i] = vert [i]. top - vert [i]. bot;
        hist [2] [i] = vert [i]. top - vert [i]. bot - vert [i]. sum;
        hist [3] [i] = dx            - vert [i]. bot;
        hist [4] [i] = vert [i]. top;
        hist [10] [i] = vert[i].bot;       // left abris  ( as in ABRIS.C )
        hist [11] [i] = dx - vert[i].top;  // right abris ( as in ABRIS.C )
    }

    for (i = 0; i < dx; i++)
    {
        hist [5] [i] = hor [i]. sum;
        hist [6] [i] = hor [i]. top - hor [i]. bot + 1;
        hist [7] [i] = hor [i]. top - hor [i]. bot + 1 - hor[i]. sum;
        hist [8] [i] = dy           - hor [i]. bot;
        hist [9] [i] = hor [i]. top + 1;
    }
}

static void make_func ()
{
    int16_t i, m;

    memset (funcs [0], 0, MAX_NEW_FUNC * MAX_HOR_IL1);
    for (i = 0; i < dy; i++)    funcs [1][i] = dx - hist [3][i];
    for (i = 0; i < dy; i++)    funcs [2][i] = dx - hist [4][i];
    for (i = 0; i < dy; i++)    funcs [3][i] = abs (hist [3][i] - hist [4][i]);
    for (i = 0; i < dy; i++)    funcs [4][i] =
                                        abs (hist [3][dy - 1 - i] - hist [4][i]);
    for (i = 0; i < dy; i++)    funcs [5][i] = hist [1][i];
//    for (i = 0; i < dx; i++)    funcs [6][i] = hist [5][i];
//    for (i = 0; i < dx; i++)    funcs [7][i] = hist [6][i];
    m = gmax (hist [1], 0, (int16_t)(dy - 1));
    for (i = 0; i < dy; i++)    funcs [8][i] = m - hist [1][i];
    nfunc = 8;
}

static int16_t line_width (puchar func, int16_t beg, int16_t end)
{
    int16_t i;
    int16_t max         = 0;
    int16_t gmax        = 0;
    int16_t lines_value = 0;
    int16_t count [MAX_HOR_IL1];

    memset (count, 0, sizeof (count));
    for (i = beg; i < end; i++)
    {
        if (func [i] > gmax)
        gmax = func [i];
        count [func [i]] ++;
    }
    for (i = 0; i <= gmax; i++)
    if (count [i] > max)
    {
        max = count [i];
        lines_value = i;
    }
    return lines_value;
}

static uint16_t integral (puchar func, int16_t from, int16_t to, int16_t threshold)
{
    uint16_t  s;
    uint32_t i;

    s = 0;
    for (i = from; i <= (uint32_t)to; i++)
    s += MAX (0, func [i] - threshold);
    return s;
}

static int16_t gmax (puchar func, int16_t from, int16_t to)
{
    uint16_t s;
    int16_t  i;

    s = 0;
    for (i = from; i <= to; i++)
    if (func[i] > s) { s=func[i]; gmax_y=i; }
    return s;
}

static int16_t gmin (puchar func, int16_t from, int16_t to)
{
    uint16_t s;
    int16_t  i;

    s = MAX_HOR_IL1;
    for (i = from; i <= to; i++)
    if (func[i] < s) { s=func[i]; gmin_y = i; }
    return s;
}

/*
static int16_t pure_hill (puchar func, int16_t from, int16_t to)
{
 while (from != to)
  {
   if (func[from] <= func[from+1]) from++;
   else break;
  }
 while (from != to)
  {
   if (func[from] >= func[from+1]) from++;
   else return FALSE;
  }
 return TRUE;
}

static int16_t flthll_1 ()
//
//	This procedure checks absence of hill on bottom one third
//	of letter and presence of it on top one third.
//
{
    int16_t beg, end;
    int16_t i1, i2;
    int16_t lw1;
    int16_t b1, b2;

    beg = 0;
    end = dy - 1;
    b1  = (beg + end) * 33 / 100;
    b2  = (beg + end) * 67 / 100;
    lw1 = line_width (hist [1], beg, end);
    i1  = integral (hist [1], beg, b1, lw1);
    i2  = integral (hist [1], b2, end, lw1);
    if (!((i1 < (b1 - beg) * 6/10) && (i2 > (end - b2) * 12/10)))   return 0;
                                                                    return 1;
}

static int16_t flat_1()
//
//	This procedure checks absence of hills on top and bottom
//	one thirds of letter.
//
{
    int16_t beg, end;
    int16_t i1, i2;
    int16_t lw1;
    int16_t b1, b2;

    beg = 0;
    end = dy - 1;
    b1  = (beg + end) * 33 / 100;
    b2  = (beg + end) * 67 / 100;
    lw1 = line_width (hist [1], beg, end);
    i1  = integral (hist [1], beg, b1, lw1);
    i2  = integral (hist [1], b2, end, lw1);
    if ((i1 < (b1 - beg) * 6/10) && (i2 < (end - b2) * 6/10))       return 1;
    if (gmax (hist [1], beg, end) - gmin (hist [1], beg, end) == 1) return 1;
                                                                    return 0;
}

static int16_t botserif (int16_t width, int16_t area)
{
    int16_t beg, end;
    int16_t i1, lw1;

    lw1 = line_width (hist [1], 0, dy * 5/10);
    beg = 0;
    end = dy * width / 100;
    i1  = integral (hist [1], beg, end, lw1);
    if (i1 >= (end - beg + 1) * area / 100)     return 1;
    beg = 1;                                    // exclude 1 pixel size foot
    i1  = integral (hist [1], beg, end, lw1);
    if (i1 >= (end - beg + 1) * area / 100)     return 1;
    beg = 0;
    if (gmax (hist [1], beg, end) - lw1 >= 4)   return 1;   // 4 is something
                                                return 0;
}

static int16_t topserif (int16_t width,int16_t area)
{
    int16_t beg, end;
    int16_t i1, lw1;

    lw1 = line_width (hist [1], dy * 5/10, dy - 1);
    beg = dy * (100 - width) / 100;
    end = dy - 1;
    i1  = integral (hist [1], beg, end, lw1);
    if (i1 >= (end - beg + 1) * area / 100)         return 1;
                                                    return 0;
}

static int16_t toprser ()
{
    int16_t i, i1;

    extremum (hist [4], 4, 0, dy - 1,
                line_width (hist [4], 0, dy - 1), YES, YES);
    i1 = 0;
    for (i = 0; i < n_peak [4]; i++)
    if ((peak_vect [4][i] & RIGHT_PEAK) && !(peak_vect [4][i] & MIDDLE_PEAK))
    i1 ++;
    if (i1)                                     return 1;
                                                return 0;
}

static int16_t toplser()
{
    int16_t i, i1;

    extremum (hist [3], 3, 0, dy - 1,
                line_width (hist [3], 0, dy - 1), YES, YES);
    i1 = 0;
    for (i = 0; i < n_peak[3]; i++)
    if ((peak_vect [3][i] & RIGHT_PEAK) && !(peak_vect [3][i] & MIDDLE_PEAK))
    i1 ++;
    if (i1)                                     return 1;
                                                return 0;
}
*/

static int16_t vertsym (int16_t bound)
{
    if (gmax (funcs [3], (int16_t)(dy * 6 / 100), (int16_t)(dy * 94 / 100)) <= bound) return 1;
                                                                return 0;
}

static int16_t centrsym (int16_t bound)
{
    if (gmax (funcs [4], (int16_t)(dy * 6 / 100), (int16_t)(dy * 94 / 100)) <= bound) return 1;
                                                                return 0;
}

/*
static int16_t toplad (int16_t jump, int16_t percent)
//
//	This procedure checks left brim on top of letter.
//
{
    int16_t i, m, s;
    int16_t beg, end;

    beg = dy * 67 / 100;
    end = dy - 1;
    m   = gmax (hist [1], beg, end);
    for (i = end; i >= beg; i--)
    {
        if ((hist [1][i] == m) || (hist [1][i] > hist [1][i-1]))    break;
    }
    if (i >= end - dy * jump / 100)             return 0;
    s = integral (hist [1], i, end, 0);
    if (s <= (end - i + 1) * m * percent / 100) return 1;
                                                return 0;
}

static int16_t botlad(int16_t jump,int16_t percent)
//
//	This procedure checks left brim on bottom of letter.
//
{
    int16_t i, m, s;
    int16_t beg, end;

    beg = 0;
    end = dy * 33 / 100;
    m   = gmax (hist [1], beg, end);
    for (i = beg; i <= end; i++)
    if ((hist [1][i] == m) || (hist [1][i] > hist [1][i + 1]))  break;
    if (i <= dy * jump / 100)                   return 0;
    s = integral (hist [1], beg, i, 0);
    if (s < (i+1) * m * percent / 100)          return 1;
                                                return 0;
}
*/

static Bool increase (puchar f, int16_t from, int16_t to)
{
    int16_t i;

    for (i = from; i < to; i++)
        if (f [i] > f [i + 1])                  return FALSE;
    if (f [from] == f [to])                     return FALSE;
                                                return TRUE;
}

/*
static Bool decrease (puchar f, int16_t from, int16_t to)
//
//	This procedure checks function to be decreasing.
//
{
    int16_t i;

    for (i = from; i < to; i++)
        if (f [i] < f [i + 1])                  return FALSE;
    if (f [from] == f [to])                     return FALSE;
                                                return TRUE;
}
*/

