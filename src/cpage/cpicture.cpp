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

#include "minmax.h"
#include "resource.h"
#include "mymem.h"
#include "cpage.h"
#include "backup.h"
#include "polyblock.h"
#include "internal.h"
#include "picture.h"
#include "cpage_debug.h"

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

#define MAXDIFF 0 // максимальное расхождение в координатах при определении верт или гор.
//При значениях >0 работает неверно при заполнении массивов lpVer и lpHor при коротких линиях !!! Art

static int CompareLong(const void *arg1, const void *arg2)
{
    return (*(long*) arg1 - *(long*) arg2);
}

static int GetIndex(long * lpLong, long nLong, long n)
{
    int i = 0;

    for (i = 0; i < nLong; i++) {
        if (abs(n - lpLong[i]) <= MAXDIFF)
            break;
    }

    return i;
}

bool CPAGE_PictureGetMask(CBlockHandle hPicture, char * data, uint32_t * size)
{
    bool rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    assert(size);
    cf::cpage::Picture pict;

    if (CPAGE_GetBlockData(hPicture, TYPE_CPAGE_PICTURE, &pict, sizeof(pict)) == sizeof(pict)) {
        int i, j;
        int nVer, nHor, sz_x, sz_y;
        int nMaxVer = 0;
        long * lpVer = NULL;
        int nMaxHor = 0;
        long * lpHor = NULL;
        char * lpMatrix = NULL;

        // Подсчитаем число вертикальных разделителей
        for (i = 0; i < pict.cornerCount(); i++) {
            int ci = (i + 1) % pict.cornerCount();

            if (abs(pict.cornerAt(i).x() - pict.cornerAt(ci).x()) <= MAXDIFF)
                nMaxVer++;

            if (abs(pict.cornerAt(i).y() - pict.cornerAt(ci).y()) <= MAXDIFF)
                nMaxHor++;
        }

        // создадим массивы линий
        assert(nMaxVer > 1);
        assert(nMaxHor > 1);

        if (nMaxVer < 2 || nMaxHor < 2)
            return FALSE;

        lpVer = (long*) malloc(sizeof(long) * nMaxVer);
        lpHor = (long*) malloc(sizeof(long) * nMaxHor);
        lpMatrix = (char*) malloc(sizeof(char) * nMaxVer * (nMaxHor - 1));

        if (lpVer && lpHor && lpMatrix) {
            memset(lpMatrix, 0, sizeof(char) * nMaxVer * (nMaxHor - 1));

            for (nVer = nHor = 0, i = 0; i < pict.cornerCount(); i++) {
                int ci = (i + 1) % pict.cornerCount();

                if (PointXDistance(pict.cornerAt(i), pict.cornerAt(ci)) <= MAXDIFF)
                    lpVer[nVer++] = pict.cornerAt(i).x();

                if (PointYDistance(pict.cornerAt(i), pict.cornerAt(ci)) <= MAXDIFF)
                    lpHor[nHor++] = pict.cornerAt(i).y();
            }
        }

        else {
            SetReturnCode_cpage(IDS_ERR_NO_MEMORY);
            goto lOut;
        }

        // Упорядочим их
        qsort(lpVer, nMaxVer, sizeof(long), CompareLong);

        // Уберем повторяющиеся величины
        for (i = 1; i < nMaxVer; i++) {
            if (lpVer[i] == lpVer[i - 1]) {
                memcpy(lpVer + i - 1, lpVer + i, sizeof(lpVer[0]) * (nMaxVer - i));
                nMaxVer--;
                i--;
                continue;
            }
        }

        qsort(lpHor, nMaxHor, sizeof(long), CompareLong);

        for (i = 1; i < nMaxHor; i++) {
            if (lpHor[i] == lpHor[i - 1]) {
                memcpy(lpHor + i - 1, lpHor + i, sizeof(lpHor[0]) * (nMaxHor - i));
                nMaxHor--;
                i--;
                continue;
            }
        }

        // Создадим матрицу описания границ
        for (i = 0; i < pict.cornerCount(); i++) {
            int ci = (i + 1) % pict.cornerCount();
            int delta_x = PointXDistance(pict.cornerAt(i), pict.cornerAt(ci));
            int delta_y = PointYDelta(pict.cornerAt(i), pict.cornerAt(ci));

            if (delta_x <= MAXDIFF) {// вертикальная граница
                int sign = delta_y ? (delta_y / abs(delta_y)) : 1;
                int x = GetIndex(lpVer, nMaxVer, pict.cornerAt(i).x());
                int y1 = GetIndex(lpHor, nMaxHor, pict.cornerAt(i).y());
                int y2 = GetIndex(lpHor, nMaxHor, pict.cornerAt(ci).y());

                if (x < nMaxVer && y1 < nMaxHor && y2 < nMaxHor)
                    for (int y = MIN(y1, y2); y < MAX(y1, y2); y++)
                        *(lpMatrix + x + y * nMaxVer) = sign;
            }
        }

        // Создадим маску по матрице
        sz_x = (lpVer[nMaxVer - 1] - lpVer[0] + 7) / 8;
        sz_y = lpHor[nMaxHor - 1] - lpHor[0];
        assert(sz_x > 0 && sz_y > 0);
        *size = sz_x * sz_y;
        rc = TRUE;

        if (data) {
            int sign = 0;
            memset(data, 0, *size);

            for (int y = 0; y < (nMaxHor - 1); y++) {
                int sp = 0;

                for (int x = 0; x < nMaxVer; x++) {
                    int cs = *(lpMatrix + x + y * nMaxVer);

                    if (cs) {
                        if (!sign)
                            sign = cs;

                        if (cs == sign)
                            sp = x;

                        else { // Записываем маску
                            int beg_x = (lpVer[sp] - lpVer[0]) / 8;
                            int end_x = (lpVer[x] - lpVer[0] + 7) / 8;
                            int beg_y = lpHor[y] - lpHor[0];
                            int end_y = lpHor[y + 1] - lpHor[0];

                            for (i = beg_y; i < end_y; i++)
                                for (j = beg_x; j < end_x; j++) {
                                    *(data + i * sz_x + j) = (char) 0xFF;
                                }
                        }
                    }
                }
            }
        }

    lOut:

        if (lpHor)
            free(lpHor);

        if (lpVer)
            free(lpVer);

        if (lpMatrix)
            free(lpMatrix);
    }

    return rc;
}
