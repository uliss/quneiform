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

/************************************************************************/
/* DIFFRB.C - базовые функции для дискриминаторов           */
/*             ( в Tiger-e были написаны под TASM)          */
/************************************************************************/
/* LeftDistance - расстояние до первого слева бита          */
/* RightDistance -расстояние до первого справа бита            */
/* SumIntervalBits  - посчитать сумму бит (начало и конец - биты )     */
/* SumBits - посчитать сумму бит в строке байт              */
/* VertSum - посчитать сумму бит в столбце                 */
/* MinMaxLeft - найти min и max расстояние на левом абрисе         */
/* MinMaxRight - найти min и max расстояние на правом абрисе      */
/* NumHorizInterval - число интервалов в строке           */
/* NumVertInterval - число интервалов в столбце          */
/* FOOT_A - вычислить вертикальную проекцию растра           */
/* FOOT3 - вычислить число ног ( ожидается 3-ногая буква)       */
/* FOOT - вычислить число ног                          */
/* Assym_let - оценить вертикальную асимметрию буквы              */
/* CenterVertInterval - середина интервала в столбце           */
/************************************************************************/
/* ПАРАМЕТРЫ : *********************************************************/
/*           RASTER - указатель на строку или на начало рабочено растра */
/*           Wx     - ширина Р.Р.                                       */
/*           NWIDTH - ширина оболочки в Р.Р.  или строки                */
/*           NHEIGHT- количество строк в Р.Р.                           */
/*           Сolumn - столбец                                           */
/************************************************************************/

#include <string.h>

#include "recdefs.h"
#include "leo/leodefs.h"
#include "diskrtab.h"       /* таблицы      */

namespace cf {
namespace dif {
uchar BUFFER[256]; /* вертикальная проекция  */
uchar LOCAL[50]; /* список центров ног     */
uchar LOCAL_W[50]; /* список ширин   ног     */
uchar beg2, end1; /* начало 2-ой, конец 1-ой ног  */

#define bytlen(bits)  (REC_GW_WORD8(bits))

int16_t dnri_hook;
int broken_ii = 0;
uchar broken_flag = 0;
}
}

using namespace cf;

namespace cf {
namespace dif {

/* LeftDistance - расстояние до первого слева бита          */
int16_t LeftDistance(uchar *RASTER, int16_t NWIDTH)
{
    int16_t i;

    for (i = 0; i < NWIDTH && (*RASTER) == 0; i++, RASTER++)
        ;

    if (i == NWIDTH)
        return (-1);

    return ((i << 3) + dif::start_pos[*RASTER]);
}

/* RightDistance -расстояние до первого справа бита            */
int16_t RightDistance(uchar *RASTER, int16_t NWIDTH)
{
    int16_t i;
    RASTER += NWIDTH - 1;

    for (i = 0; i < NWIDTH && (*RASTER) == 0; i++, RASTER--)
        ;

    if (i == NWIDTH)
        return (-1);

    return ((i << 3) + dif::last_pos[*RASTER]);
}

/* SumIntervalBits  - посчитать сумму бит (начало и конец - биты )     */
int16_t SumIntervalBits(uchar *RASTER, int16_t n1, int16_t n2)
{
    int16_t i, d, l;
    i = n1 >> 3;
    l = n2 >> 3;

    if (i == l) /* начало и конец в одном байте */
        return (3* dif::bit_cnt [dif::mask_l[n1 & 7] & RASTER[i] & dif::mask_r[n2 & 7]]);

    d = dif::bit_cnt[RASTER[i] & dif::mask_l[n1 & 7]];
    d += dif::bit_cnt[RASTER[l] & dif::mask_r[n2 & 7]];

    if (l == i + 1) /* все уместилось в 2 байта  */
        return (3* d );

    for (i++; i < l; i++)/* более 2-х байт               */
        d += dif::bit_cnt[RASTER[i]];

    return (3* d );
}

/* SumBits - посчитать сумму бит в строке байт              */
int16_t SumBits(uchar *RASTER, int16_t NWIDTH)
{
    int16_t i, s;

    for (i = s = 0; i < NWIDTH; i++, RASTER++)
        s += dif::bit_cnt[*RASTER];

    return (s);
}

/* VertSum - посчитать сумму бит в столбце                 */
int16_t VertSum(uchar *RASTER, int16_t Wx, int16_t NHEIGHT, int16_t Column)
{
    int16_t i, d;
    uchar mask = dif::mask_byte[Column & 7];
    RASTER += (Column >> 3);

    for (i = d = 0; i < NHEIGHT; i++, RASTER += Wx)
        d += (((*RASTER) & mask) != 0);

    return (d);
}

/* NumHorizInterval - число интервалов в строке           */
int16_t NumHorizInterval(uchar *RASTER, int16_t NWIDTH)
{
    int16_t i, d;
    uchar c, old;

    for (i = d = old = 0; i < NWIDTH; i++, RASTER++) {
        c = *RASTER;
        d += dif::piece_cnt[c];

        if ((c & 0x80) && old)
            d--; /* продолжение */

        old = ((c & 0x01) != 0); /* последний бит предыдущего байта */
    }

    return (d);
}

/* NumVertInterval - число интервалов в столбце          */
int16_t NumVertInterval(uchar *RASTER, int16_t Wx, int16_t NHEIGHT,
                        int16_t Column)
{
    int16_t i, d;
    uchar c, old, mask = dif::mask_byte[Column & 7];
    RASTER += (Column >> 3);

    for (d = 1, i = old = 0; i < NHEIGHT; i++, RASTER += Wx) {
        c = (((*RASTER) & mask) != 0);

        if (c ^ old)
            d++; /* скачок */

        old = c; /* предыдущий байт */
    }

    if (old)
        d++;

    d >>= 1;
    return (d);
}

/* FOOT_A - вычислить вертикальную проекцию растра           */
int16_t FOOT_A(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NLENGTH)
{
    int16_t i, j, k, d;
    uchar *p, c;
    d = bytlen(NWIDTH);
    memset(dif::BUFFER, 0, NWIDTH);

    for (i = 0; i < NLENGTH; i++, RASTER += Wx) {
        p = RASTER;

        for (k = j = 0; j < d; j++) {
            c = *p++;
#ifndef __MAC__
            *((uint32_t *) &dif::BUFFER[k]) += dif::tab_4bits_to_DWORD[c >> 4];
            *((uint32_t *) &dif::BUFFER[k + 4]) += dif::tab_4bits_to_DWORD[c & 15];
#else
            dif::BUFFER[k+7] += ((c & 0x01) != 0);
            dif::BUFFER[k+6] += ((c & 0x02) != 0);
            dif::BUFFER[k+5] += ((c & 0x04) != 0);
            dif::BUFFER[k+4] += ((c & 0x08) != 0);
            dif::BUFFER[k+3] += ((c & 0x10) != 0);
            dif::BUFFER[k+2] += ((c & 0x20) != 0);
            dif::BUFFER[k+1] += ((c & 0x40) != 0);
            dif::BUFFER[k ] += ((c & 0x80) != 0);
#endif
            k += 8;
        }
    } /* проекция */

    return (0);
}

/* FOOT - вычислить число ног                          */
int16_t FOOT(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NLENGTH,
             uchar HARD_FILTER)
{
    int16_t i, old, d, l, k;
    uchar c, curr, first, second;
    memset(dif::LOCAL, 0, 50);
    memset(dif::LOCAL_W, 0, 50);
    FOOT_A(RASTER, Wx, NWIDTH, NLENGTH); /* проекция */
    d = NLENGTH;
    d -= (d >> 3); /* 7/8 */

    for (old = 9999, i = 0; i < NWIDTH; i++)
        if (old > dif::BUFFER[i])
            old = dif::BUFFER[i];

    for (i = 0; i < NWIDTH; i++)
        dif::BUFFER[i] = (dif::BUFFER[i] >= d); /* бинаризация */

    for (old = d = i = 0; i < NWIDTH; i++) {
        c = dif::BUFFER[i];

        if (old == 0 && c == 1)
            d++;

        old = c;
    } /* d - число ног */

    if (HARD_FILTER == 0 || d <= 2 && HARD_FILTER != 2) { /* слабый фильтр */
        first = dif::BUFFER[0];
        second = dif::BUFFER[1];

        for (i = 2; i < NWIDTH; i++) {
            curr = dif::BUFFER[i];

            if (first == 1 && second == 0 && curr == 1)
                dif::BUFFER[i - 1] = 1;

            first = second;
            second = curr;
        }
    }

    else { /* сильный фильтр */
        first = dif::BUFFER[0];
        second = dif::BUFFER[1];

        for (i = 2; i < NWIDTH; i++) {
            curr = dif::BUFFER[i];

            if (first == 1 && second == 0 && curr == 1)
                dif::BUFFER[i - 1] = 1;

            if (first == 0 && second == 1 && curr == 0)
                dif::BUFFER[i - 1] = 0;

            first = second;
            second = curr;
        }
    }

    /* отфильтровали */

    for (old = k = l = i = 0; i < NWIDTH; i++) {
        c = dif::BUFFER[i];

        if (old ^ c) {
            if (c)
                l = i; /* черная  : начало ноги */

            else
                dif::LOCAL_W[k++] = i - l; /* белая : ширина ноги      */
        }

        old = c;
    }

    if (old)
        dif::LOCAL_W[k++] = i - l; /* последняя точка черная : ширина ноги */

    for (old = d = l = k = i = 0; i < NWIDTH; i++) {
        c = dif::BUFFER[i];

        if (old ^ c) {
            d++;

            if (d % 2)
                l = i; /* начало ноги     */

            else {
                dif::LOCAL[k++] = (l + i) >> 1; /* конец ноги   */
            }
        }

        old = c;
    }

    if (old) {
        dif::LOCAL[k++] = (l + i) >> 1; /* конец последней ноги */
        d++;
    }

    d >>= 1; /* d - число скачков */

    if (d != 2)
        return (d);

    /* 2 ноги */
    for (i = 1; i < NWIDTH; i++)
        if (dif::BUFFER[i] == 0 && dif::BUFFER[i - 1] == 1)
            break;

    dif::end1 = (uchar) i; /* конец 1-ой ноги */

    for (i = NWIDTH - 2; i >= 0; i--)
        if (dif::BUFFER[i] == 0 && dif::BUFFER[i + 1] == 1)
            break;

    dif::beg2 = (uchar) i; /* начало 2-ой ноги */
    return (2);
}

/* CenterVertInterval - середина интервала в столбце           */
int16_t CenterVertInterval(uchar *RASTER, int16_t Wx, int16_t NHEIGHT,
                           int16_t Column, int16_t *up, int16_t *down)
{
    uchar mask = dif::mask_byte[Column & 7], c, old;
    int16_t i, num, center, up_center;
    *up = *down = -1;
    RASTER += (Column >> 3);

    for (i = 0; i < NHEIGHT && ((*RASTER) & mask); i++, RASTER += Wx)
        ;

    if (i == NHEIGHT) {
        *up = NHEIGHT;
        *down = 0;
        return (0); /* весь столбец черный */
    }

    else
        up_center = i ? i : (NHEIGHT << 1); /* середина верхнего интервала */

    for (num = old = 0; i < NHEIGHT; i++, RASTER += Wx) {
        c = (((*RASTER) & mask) != 0);

        if (old ^ c)
            switch (num++) {
                case 0: /* начало */
                    *up = NHEIGHT - i;
                    center = i;
                    break;
                case 1: /* конец */
                    *down = NHEIGHT - i;
                    center += i;
                    break;
                default:
                    return (0); /* более 2 */
            }

        old = c;
    }

    switch (num) {
        case 0: /* верхний интервал */
            center = up_center;

            if (*up == -1)
                *up = NHEIGHT - 0;

            if (*down == -1)
                *down = NHEIGHT - up_center;

            break;
        case 1: /* нижний интервал */
            center += NHEIGHT;

            if (*up == -1)
                *up = NHEIGHT - up_center;

            if (*down == -1)
                *down = NHEIGHT - NHEIGHT;

            break;
    }

    return ((NHEIGHT << 1) - center); /* удвоенное расстояние от низа растра */
}

int16_t MinMaxLeft(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NHEIGHT,
                   int16_t *Pmin, int16_t *Pmax)
{
    int16_t maxr = 0, minr = 100, r, i;
    NWIDTH = bytlen(NWIDTH);

    for (i = 0; i < NHEIGHT; i++, RASTER += Wx) {
        r = LeftDistance(RASTER, NWIDTH);

        if (minr > r)
            minr = r;

        if (maxr < r)
            maxr = r;
    }

    *Pmin = minr;
    *Pmax = maxr;
    return (0);
}

/* MinMaxRight - найти min и max расстояние на правом абрисе      */
int16_t MinMaxRight(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NHEIGHT,
                    int16_t *Pmin, int16_t *Pmax)
{
    int16_t maxr = 0, minr = 100, r, i, end;
    r = bytlen(NWIDTH);
    end = (r - ((NWIDTH + 7) / 8)) * 8;
    NWIDTH = (uchar) r;

    for (i = 0; i < NHEIGHT; i++, RASTER += Wx) {
        r = RightDistance(RASTER, NWIDTH) - end;

        if (minr > r)
            minr = r;

        if (maxr < r)
            maxr = r;
    }

    *Pmin = minr + 1;
    *Pmax = maxr + 1;
    return (0);
}

/* FOOT3_2 - вычислить число ног ( ожидается 3-ногая буква)     */
/*           в верхней и нижней половинах                       */
int16_t FOOT3_2(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NLENGTH)
{
    int16_t i, old, du, dd;
    uchar c;
    FOOT_A(RASTER, Wx, NWIDTH, (uchar)(NLENGTH / 2)); /* проекция */

    for (i = 0; i < NWIDTH; i++)
        dif::BUFFER[i] = (dif::BUFFER[i] > 0); /* бинаризация */

    for (old = du = i = 0; i < NWIDTH; i++) {
        c = dif::BUFFER[i];

        if (old ^ c)
            du++;

        old = c;
    }

    if (old)
        du++;

    du >>= 1; /* du - число скачков вверху */
    FOOT_A(RASTER + (NLENGTH / 2) * Wx, Wx, NWIDTH, (uchar)(NLENGTH / 2)); /* проекция */

    for (i = 0; i < NWIDTH; i++)
        dif::BUFFER[i] = (dif::BUFFER[i] > 0); /* бинаризация */

    for (old = dd = i = 0; i < NWIDTH; i++) {
        c = dif::BUFFER[i];

        if (old ^ c)
            dd++;

        old = c;
    }

    if (old)
        dd++;

    dd >>= 1; /* du - число скачков вверху */
    return (dd == 3 && du != 3);
}

/* FOOT3 - вычислить число ног ( ожидается 3-ногая буква)     */
int16_t FOOT3(uchar *RASTER, int16_t Wx, uchar START, uchar NWIDTH,
              uchar NLENGTH, int16_t SHIFT)
{
    int16_t i, old, d;
    uchar c;
    FOOT_A(RASTER, Wx, NWIDTH, NLENGTH); /* проекция */
    d = NLENGTH;
    d = (d >> SHIFT); /* 1/4 */

    for (i = 0; i < NWIDTH; i++)
        dif::BUFFER[i] = (dif::BUFFER[i] >= d); /* бинаризация */

    for (old = d = 0, i = START; i < NWIDTH; i++) {
        c = dif::BUFFER[i];

        if (old ^ c)
            d++;

        old = c;
    }

    if (old)
        d++;

    d >>= 1; /* d - число скачков */
    return (d);
}

/* EndBlackInterval - номер последнего черного бита   */
/* в первой слева пачке черных бит      */
int16_t EndBlackInterval(uchar *RASTER, int16_t NWIDTH)
{
    int16_t i;

    for (i = 0; i < NWIDTH && (*RASTER) == 0; i++, RASTER++)
        ; /* skip white bytes    */

    if (i == NWIDTH)
        return (-1);

    if (i < NWIDTH - 1 && ((*RASTER) & 0x01) == 1 && dif::piece_cnt[*RASTER] == 1
            && ((*(RASTER + 1)) & 0x80) == 0x80) {
        RASTER++;
        i++;
    }

    return ((i << 3) + dif::tab_last_black_bit[*RASTER]);
}

}
}

void clear_right_bites(uchar *RASTER, int16_t NWIDTH, int16_t WBYTE,
                       int16_t NHEIGHT)
{
    int w = NWIDTH & 7, ww = (NWIDTH + 7) / 8, i;
    RASTER += ww;

    if (!w) {
        for (i = 0; i < NHEIGHT; i++, RASTER += WBYTE) {
            memset(RASTER, 0, WBYTE - ww);
        }
    }

    else {
        for (i = 0; i < NHEIGHT; i++, RASTER += WBYTE) {
            RASTER[-1] &= dif::mask_r[w];
            memset(RASTER, 0, WBYTE - ww);
        }
    }

    return;
}

int16_t FOOT_HEI(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NLENGTH)
{
    int16_t i, old, d, l, k;
    uchar c, curr, first, second;
    memset(dif::LOCAL, 0, 50);
    memset(dif::LOCAL_W, 0, 50);
    dif::FOOT_A(RASTER, Wx, NWIDTH, NLENGTH); /* проекция */
    d = NLENGTH;
    d -= 2; // full heigh

    for (old = 9999, i = 0; i < NWIDTH; i++)
        if (old > dif::BUFFER[i])
            old = dif::BUFFER[i];

    for (i = 0; i < NWIDTH; i++)
        dif::BUFFER[i] = (dif::BUFFER[i] >= d); /* бинаризация */

    for (old = d = i = 0; i < NWIDTH; i++) {
        c = dif::BUFFER[i];

        if (old == 0 && c == 1)
            d++;

        old = c;
    } /* d - число ног */

    first = dif::BUFFER[0];
    second = dif::BUFFER[1];

    for (i = 2; i < NWIDTH; i++) {
        curr = dif::BUFFER[i];

        if (first == 1 && second == 0 && curr == 1)
            dif::BUFFER[i - 1] = 1;

        first = second;
        second = curr;
    }

    /* отфильтровали */

    for (old = k = l = i = 0; i < NWIDTH; i++) {
        c = dif::BUFFER[i];

        if (old ^ c) {
            if (c)
                l = i; /* черная  : начало ноги */

            else
                dif::LOCAL_W[k++] = i - l; /* белая : ширина ноги      */
        }

        old = c;
    }

    if (old)
        dif::LOCAL_W[k++] = i - l; /* последняя точка черная : ширина ноги */

    for (old = d = l = k = i = 0; i < NWIDTH; i++) {
        c = dif::BUFFER[i];

        if (old ^ c) {
            d++;

            if (d % 2)
                l = i; /* начало ноги     */

            else {
                dif::LOCAL[k++] = (l + i) >> 1; /* конец ноги   */
            }
        }

        old = c;
    }

    if (old) {
        dif::LOCAL[k++] = (l + i) >> 1; /* конец последней ноги */
        d++;
    }

    d >>= 1; /* d - число скачков */
    return (d);
}
