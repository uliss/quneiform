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

#define _RMARKER_CPP

#include <fstream>
#include "markpage.h"
#include "rneg/rneg.h"
#include "rmfunc.h"
#include "un_buff.h"
#include "line_vp_util.h"
#include "line_vp_2_am.h"
#include "markdataoper.h"
#include "amt_geom.h"
#include "un_err.h"
#include "puma_err.h"
#include "rneg/rneg.h"
#include "rselstr/rselstr.h"
#include "rline/rline.h"

namespace cf
{
RMarker::RMarker() :
    cpage_(NULL), ccom_(NULL), cline_(NULL), language_(LANGUAGE_RUS_ENG), pictures_(0), fax_(false),
            one_column_(false), kill_vsl_components_(false)
{
    RNEG_Init(0, NULL);
}

RMarker::~RMarker()
{
    RNEG_Done();
}

void RMarker::markupPage()
{

}
}

#define INCLINE_FACTOR  2048

#define IDEAL_XY(x, y)   \
         {\
             y = (int16_t) (y - (int32_t) x * nIncline / INCLINE_FACTOR);\
             x = (int16_t) (x + (int32_t) y * nIncline / INCLINE_FACTOR);\
         }

#define REAL_XY(x, y)   \
         {\
             x = (int16_t) (x - (int32_t) y * nIncline / INCLINE_FACTOR);\
             y = (int16_t) (y + (int32_t) x * nIncline / INCLINE_FACTOR);\
        }

#define TYPE_FON      CPAGE_GetInternalType("TYPE_FON")

extern Handle DebugFill;
extern Handle hVertCells;
extern Handle hNegaCells;
extern Handle hVertCellsAuto;
extern Handle hNegaCellsAuto;
extern Handle hDebugUpDown;
extern Handle hDebugPictures;
extern Handle hPrintFileVertCells;
extern Handle hDebugNeg;
extern Handle hDebugLinePass3;
extern Handle hDebugLinePass2;
extern Handle hDebugVerifLine;
extern Handle hNoGiveNeg;
extern Handle hNoSeePict;

extern Handle hPrep;
extern Handle hPicture;
extern Handle hNegative;
extern Handle hPrintCrossedPics;
extern Handle hVCutInZones;
extern Handle hSVLP;
extern Handle hBlocks;
extern Handle hLines3;
extern Handle hNegaTestCells;
extern Handle hVertTestCells;
extern Handle hFon;
extern Handle hEnd;

static uint32_t gwRC = 0;
Bool dpDebugUpDown;

Bool32 PageMarkup(PRMPreProcessImage Image)
{
    Bool32 rc = TRUE;
    gSVLBuffer.VLinefBufferA = NULL;
    gSVLBuffer.VLinefBufferB = NULL;
    gSVLBuffer.LineInfoA = (LinesTotalInfo*) calloc(1, sizeof(LinesTotalInfo));
    gSVLBuffer.LineInfoB = (LinesTotalInfo*) calloc(1, sizeof(LinesTotalInfo));

    rc = ShortVerticalLinesProcess(PUMA_SVL_FIRST_STEP, Image);

    BIG_IMAGE big_Image;
    //default Image:
    PAGEINFO info;
    GetPageInfo(Image->hCPAGE, &info);

    for (int i = 0; i < CPAGE_MAXNAME; i++)
        big_Image.ImageName[i] = info.szImageName[i];

    big_Image.hCCOM = NULL;
    Handle h = CPAGE_GetBlockFirst(Image->hCPAGE, TYPE_BIG_COMP);

    if (h) {
        CPAGE_GetBlockData(Image->hCPAGE, h, TYPE_BIG_COMP, &big_Image, sizeof(BIG_IMAGE));
        CPAGE_DeleteBlock(Image->hCPAGE, h);
    }

    //Поиск очевидных картинок
    if (rc)
        rc = SearchPictures(Image, big_Image);

    //Поиск негативов
    if (rc)
        rc = SearchNeg(Image, big_Image, info.Incline2048);

    //Третий проход по линиям
    if (LDPUMA_Skip(hDebugLinePass3) && LDPUMA_Skip(hDebugVerifLine)
            && LDPUMA_Skip(hDebugLinePass2)) {
        if (rc)
            RLINE_LinesPass3(Image->hCPAGE, Image->hCLINE, Image->hCCOM, (uchar) Image->gnLanguage);
    }

    ////снова подсчитываем короткие вертикальные линии и сравниваем с предыдущим результатом
    if (rc) {
        rc = ShortVerticalLinesProcess(PUMA_SVL_SECOND_STEP, Image);
    }

    ShortVerticalLinesProcess(PUMA_SVL_THRID_STEP, Image);
    free(gSVLBuffer.LineInfoA);
    free(gSVLBuffer.LineInfoB);

    // blocks
    if (!LDPUMA_Skip(Image->hDebugLayoutFromFile)) {
        Image->hCPAGE = CPAGE_RestorePage(TRUE, (pchar) (Image->szLayoutFileName));

        if (Image->hCPAGE == NULL) {
            SetReturnCode_rmarker(CPAGE_GetReturnCode());
            rc = FALSE;
        }

        else {
            CPAGE_SetCurrentPage(CPAGE_GetNumberPage(Image->hCPAGE));
            LDPUMA_Console("Layout восстановлен из файла '%s'\n", Image->szLayoutFileName);
        }
    }
    else if (rc) {
        if (LDPUMA_Skip(Image->hDebugCancelExtractBlocks)) {
            Bool32 bEnableSearchPicture;
            bEnableSearchPicture = Image->gnPictures;
            RBLOCK_SetImportData(RBLOCK_Bool32_SearchPicture, &bEnableSearchPicture);
            RBLOCK_SetImportData(RBLOCK_Bool32_OneColumn, &(Image->gbOneColumn));

            if (!RBLOCK_ExtractTextBlocks(Image->hCCOM, Image->hCPAGE, Image->hCLINE)) {
                SetReturnCode_rmarker(RBLOCK_GetReturnCode());
                rc = FALSE;
            }
        }

        else
            LDPUMA_Console("Пропущен этап автоматического Layout.\n");
    }

    CCOM_DeleteContainer(big_Image.hCCOM);
    return rc;
}

void SetReturnCode_rmarker(uint32_t rc)
{
    gwRC = rc;
}

uint32_t GetReturnCode_rmarker(void)
{
    return gwRC;
}

Bool32 SearchNeg(PRMPreProcessImage Image, BIG_IMAGE big_Image, int skew)
{
    if (!LDPUMA_Skip(hDebugNeg))
        return TRUE;

    RNEG_RecogNeg(big_Image.hCCOM, Image->hCPAGE, big_Image.ImageName, skew);
    return TRUE;
}

Bool32 SearchPictures(PRMPreProcessImage Image, BIG_IMAGE big_Image)
{
    Bool32 rc = TRUE;

    if (!LDPUMA_Skip(hDebugPictures))
        return TRUE;

    if (rc && LDPUMA_Skip(Image->hDebugCancelSearchPictures)) {
        if (Image->gnPictures) {
            if (!RPIC_SearchPictures(Image->hCCOM, big_Image.hCCOM, Image->hCPAGE)) {
                uint32_t RPicRetCode = RPIC_GetReturnCode();
                SetReturnCode_rmarker(RPicRetCode);
                rc = FALSE;
            }
        }
    }

    return rc;
}

