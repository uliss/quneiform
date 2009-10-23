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

/**********  Заголовок  *******************************************************/
/*  Автор,                                                                    */
/*  комментарии                                                               */
/*  и далнейшая                                                               */
/*  правка     :  Алексей Коноплев                                            */
/*  Редакция   :  08.06.00                                                    */
/*  Файл       :  'Normalise.cpp'                                             */
/*  Содержание :  Нормализация сырья                                          */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/

#include <fstream>
#include <stdio.h>
#include <string.h>
#include "rstuff.h"
#include "rsfunc.h"
#include "rsglobaldata.h"
#include "puma/pumadef.h"
#include "lns32/lnsdefs.h"
#include "un_buff.h"
#include "line_vp_util.h"
#include "line_vp_2_am.h"
#include "markdataoper.h"
#include "un_err.h"
#include "specprj.h"

// модули
#include "ccom/ccom.h"
#include "cpage/cpage.h"
#include "criimage.h"
#include "cimage/ctiimage.h"
#include "dpuma.h"
#include "exc.h"
#include "rpic.h"
#include "rline.h"
#include "rverline.h"
#include "smetric.h"
#include "rreccom/rreccom.h"

using namespace CIF;

extern Handle ObvKillLines;
extern Handle hNewLine;
extern Handle NotKillPointed;
extern Handle hAngles;
extern Handle hCalcMuchSkew;
extern Handle hTalkMuchSkew;

extern Handle hPrep;
extern Handle hContBigComp;
extern Handle hSearchLine;
extern Handle hDotLine;
extern Handle hCalcIncline;
extern Handle hVerOrNewLine;
extern Handle hOrto;
extern Handle hKillLine;
extern Handle hPrep2;
extern Handle hKillLineAfter;
extern Handle hEnd;
extern Handle hDebugAutoTemplate;
extern Handle hWndTurn;
extern Handle hDebugPrintResolution;

Bool32 MyGetZher(void **vvZher, int32_t *nZher, int32_t MaxZher, Handle hCPage) {
    uint32_t err32, nTeor, nReal;
    Handle hBlockZher;
    Handle hBlockPrev;
    int i;
    nTeor = sizeof(void *);
    i = 0;
    while (1) {
        if (i == 0)
            hBlockZher = CPAGE_GetBlockFirst(hCPage, RVERLINE_ZHERTVY_LINIY);
        else
            hBlockZher = CPAGE_GetBlockNext(hCPage, hBlockPrev, RVERLINE_ZHERTVY_LINIY);
        err32 = CPAGE_GetReturnCode();
        if (err32 != 0) {
            //if (i==0)
            //	Error_CPage ("[GetBlockFirst]");
            //else
            //	Error_CPage ("[GetBlockNext]");
            return FALSE;
        }
        if (!hBlockZher)
            break;
        if (i >= MaxZher)
            return FALSE;
        nReal = CPAGE_GetBlockData(hCPage, hBlockZher, RVERLINE_ZHERTVY_LINIY,
                (void *) &(vvZher[i]), nTeor);
        err32 = CPAGE_GetReturnCode();
        if (!nReal || (err32 != 0)) {
            //Error_CPage ("[SetBlockData]");
            return FALSE;
        }
        hBlockPrev = hBlockZher;
        i++;
    }
    *nZher = i;
    return TRUE;
}

Bool32 remove_overlayed(CCOM_comp *sour, CCOM_handle haCCOM) {
    CCOM_comp *cur = CCOM_GetFirst(haCCOM, NULL), *curn;
    int32_t over = 0;

    do {
        curn = CCOM_GetNext(cur, NULL);
        if (cur->upper < sour->upper && sour->upper - cur->upper > 10 * sour ->h) { // upper zone
            cur = curn;
            continue;
        }
        if (comp_over(sour, cur)) {
            CCOM_Delete(haCCOM, cur);
            over++;
        }
        if (cur->upper - sour->upper > 10 * sour ->h)
            break; // downer zone
        cur = curn;
    }
    while (cur != NULL);

    return (over > 0);
}

Bool32 comp_over(CCOM_comp *sour, CCOM_comp *cur) {
    int32_t le, ri, up, dn, w, h;

    if (sour->left + sour->w <= cur->left || cur->left + cur->w <= sour->left)
        return FALSE;
    if (sour->upper + sour->h <= cur->upper || cur->upper + cur->h <= sour->upper)
        return FALSE;

    if (sour->left < cur->left)
        le = cur->left;
    else
        le = sour->left;

    if (sour->left + sour->w < cur->left + cur->w)
        ri = sour->left + sour->w;
    else
        ri = cur->left + cur->w;
    w = ri - le;
    if (sour->upper < cur->upper)
        up = cur->upper;
    else
        up = sour->upper;

    if (sour->upper + sour->h < cur->upper + cur->h)
        dn = cur->upper + cur->h;
    else
        dn = sour->upper + sour->h;
    h = dn - up;

    if (w > 0 && h > 0 && cur->w < 2 * sour ->w && cur->h < 2 * sour ->h && w * 2 > cur->w && h * 2
            > cur->h)
        return TRUE;
    return FALSE;
}

