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

#include <stdlib.h>
#include <algorithm>
#include <cassert>

#include "cpage.h"
#include "backup.h"
#include "polyblock.h"
#include "picture.h"
#include "cpage_debug.h"
#include "common/bitmask.h"

using namespace cf;

// extern functions
CBlockHandle CPAGE_PictureGetFirst(CPageHandle hPage)
{
    return CPAGE_GetBlockFirst(hPage, TYPE_CPAGE_PICTURE);
}

CBlockHandle CPAGE_PictureGetNext(CPageHandle hPage, CBlockHandle hPicture)
{
    return CPAGE_GetBlockNext(hPage, hPicture, TYPE_CPAGE_PICTURE);
}

bool CPAGE_PictureGetPlace(CBlockHandle picture, int skew2048, Point * pos, Size * size)
{
    if(!pos || !size) {
        CPAGE_ERROR_FUNC << "NULL point output arguments";
        return false;
    }

    cf::cpage::Picture pict;

    if (CPAGE_GetBlockData(picture, TYPE_CPAGE_PICTURE, &pict, sizeof(pict)) != sizeof(pict)) {
        CPAGE_ERROR_FUNC << "invalid picture data";
        return false;
    }

    if(pict.cornerCount() < 1) {
        CPAGE_ERROR_FUNC << "empty picture data";
        return false;
    }

    pict.rotate(skew2048);
    Point left_top = pict.cornerAt(0);
    Point right_bottom = left_top;

    for (size_t i = 1; i < pict.cornerCount(); i++) {
        Point pt = pict.cornerAt(i);
        left_top.rx() = std::min(left_top.x(), pt.x());
        left_top.ry() = std::min(left_top.y(), pt.y());
        right_bottom.rx() = std::max(right_bottom.x(), pt.x());
        right_bottom.ry() = std::max(right_bottom.y(), pict.cornerAt(i).y());
    }

    *pos = left_top;
    size->setWidth(PointXDistance(right_bottom, left_top));
    size->setHeight(PointYDistance(right_bottom, left_top));
    return true;
}

static const int MAXDIFF = 0; // максимальное расхождение в координатах при определении верт или гор.
//При значениях >0 работает неверно при заполнении массивов lpVer и lpHor при коротких линиях !!! Art

static int GetIndex(long * lpLong, long nLong, long n)
{
    for (int i = 0; i < nLong; i++) {
        if (abs(n - lpLong[i]) <= MAXDIFF)
            return i;
    }

    return 0;
}

static void countDelims(int * hor, int * vert, const cpage::Picture& pict)
{
    for (size_t i = 0; i < pict.cornerCount(); i++) {
        const size_t ci = (i + 1) % pict.cornerCount();
        const Point p0 = pict.cornerAt(i);
        const Point p1 = pict.cornerAt(ci);

        if (PointXDistance(p0, p1) <= MAXDIFF)
            (*vert)++;

        if (PointYDistance(p0, p1) <= MAXDIFF)
            (*hor)++;
    }
}

static void fillLineArrays(long * ver, long * hor, const cpage::Picture& pict)
{
    int nVer = 0;
    int nHor = 0;
    for (size_t i = 0; i < pict.cornerCount(); i++) {
        const size_t ci = (i + 1) % pict.cornerCount();
        const Point p0 = pict.cornerAt(i);
        const Point p1 = pict.cornerAt(ci);

        if (PointXDistance(p0, p1) <= MAXDIFF)
            ver[nVer++] = p0.x();

        if (PointYDistance(p0, p1) <= MAXDIFF)
            hor[nHor++] = p1.y();
    }
}

bool CPAGE_PictureGetMask(CBlockHandle hPict, BitMask& mask)
{
    cpage::Picture pict;
    if (CPAGE_GetBlockData(hPict, TYPE_CPAGE_PICTURE, &pict, sizeof(pict)) != sizeof(pict)) {
        CPAGE_ERROR_FUNC << "invalid picture data";
        return false;
    }

    int nMaxVer = 0;
    int nMaxHor = 0;

    // Подсчитаем число вертикальных разделителей
    countDelims(&nMaxHor, &nMaxVer, pict);

    if (nMaxVer < 2 || nMaxHor < 2) {
        CPAGE_ERROR_FUNC << "invalid delimiter count:" << nMaxHor << nMaxVer;
        return false;
    }

    // создадим массивы линий
    long * lpVer = new long[nMaxVer];
    long * lpHor = new long[nMaxHor];

    fillLineArrays(lpVer, lpHor, pict);
    // sort vertical line delimiters
    std::sort(lpVer, lpVer + nMaxVer);
    long * last_ver = std::unique(lpVer, lpVer + nMaxVer);
    nMaxVer = std::distance(lpVer, last_ver);
    // sort horizontal line delimiters
    std::sort(lpHor, lpHor + nMaxHor);
    long * last_hor = std::unique(lpHor, lpHor + nMaxHor);
    nMaxHor = std::distance(lpHor, last_hor);

    const size_t matrix_size = nMaxVer * (nMaxHor - 1);
    char * lpMatrix = new char[matrix_size];
    std::fill(lpMatrix, lpMatrix + matrix_size, 0);

    // Создадим матрицу описания границ
    for (size_t i = 0; i < pict.cornerCount(); i++) {
        const size_t ci = (i + 1) % pict.cornerCount();
        Point pt_cur = pict.cornerAt(i);
        Point pt_next = pict.cornerAt(ci);
        const int delta_x = PointXDistance(pt_cur, pt_next);
        const int delta_y = PointYDelta(pt_cur, pt_next);

        // current and next corner at vertical position
        if (delta_x <= MAXDIFF) {// вертикальная граница
            // sign is: '-1' if edge goes up, '1' if goes down
            int sign = (delta_y < 0) ? -1 : 1;
            int x = GetIndex(lpVer, nMaxVer, pt_cur.x());
            int y1 = GetIndex(lpHor, nMaxHor, pt_cur.y());
            int y2 = GetIndex(lpHor, nMaxHor, pt_next.y());

            if (x < nMaxVer && y1 < nMaxHor && y2 < nMaxHor) {
                // fill with sign corner edge
                for (int y = std::min(y1, y2); y < std::max(y1, y2); y++) {
                    lpMatrix[x + y * nMaxVer] = sign;
                }
            }
        }
    }

    // Создадим маску по матрице
    int sz_x = lpVer[nMaxVer - 1] - lpVer[0];
    int sz_y = lpHor[nMaxHor - 1] - lpHor[0];

    if(sz_x <= 0 || sz_y <= 0) {
        CPAGE_ERROR_FUNC << "invalid mask size:" << sz_x << 'x' << sz_y;
        return false;
    }

    BitMask res(sz_x, sz_y);
    int sign = 0;

    for (int y = 0; y < (nMaxHor - 1); y++) {
        int sign_pos = 0;

        for (int x = 0; x < nMaxVer; x++) {
            int current_sign = lpMatrix[x + y * nMaxVer];

            // sign found
            if (current_sign) {
                // first time sign found on the mask raw
                if (!sign)
                    sign = current_sign;

                // update sign position, begin of range stored in @var 'sign_pos'
                if (current_sign == sign)
                    sign_pos = x;
                // other sign found, end of range stored in @var 'x'
                else { // Записываем маску
                    int beg_x = (lpVer[sign_pos] - lpVer[0]);
                    int end_x = (lpVer[x] - lpVer[0]) + 1; // for full border
                    int beg_y = lpHor[y] - lpHor[0]; // current row
                    int end_y = lpHor[y + 1] - lpHor[0] + 1; // next row + 1, for full border

                    for (int y = beg_y; y < end_y; y++) {
                        for (int x = beg_x; x < end_x; x++) {
                            res.set(x, y);
                        }
                    }
                }
            }
        }
    }

    delete[] lpHor;
    delete[] lpVer;
    delete[] lpMatrix;

    mask = res;

    return true;
}
