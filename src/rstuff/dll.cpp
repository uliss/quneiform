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
/*  Файл       :  'dll.cpp'                                                   */
/*  Содержание :  Интерфейс библиотеки                                        */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/

#include "resource.h"
#include "rstuff.h"
#include "rsfunc.h"
#include "rsmemory.h"
#include "dpuma.h"
#include "smetric.h"

#include "rline.h"

#define __RSTUFF_CPP__
#include "rsglobaldata.h"
#undef __RSTUFF_CPP__
#include "dsnap.h"
#include "compat_defs.h"

//GLOBAL VARIABLES
struct tagRC16
{
    uint16_t gwLowRC;
    uint16_t gwHeightRC;
};
union RCode
{
    tagRC16 RC16;
    uint32_t gwRC;
};

static RCode RC = { 0, 0 };
static uint16_t wHighErrCode = 0;

#define RESULT 2

Bool32 gbRSLT = FALSE;

int32_t InitCount = 0;
Handle ObvKillLines;
Handle OKL;
Handle Zone;
Handle OrtMove;
Handle hNewLine;
Handle hDotLine;
Handle hAngles;
Handle hCalcMuchSkew;
Handle hTalkMuchSkew;
Handle MainDebug;
Handle InsideKill;
Handle hNotTestAlik;
Handle NotKillPointed;

Handle hMainTime;
Handle hPrep;
Handle hContBigComp;
Handle hSearchLine;
Handle hCalcIncline;
Handle hOrto;
Handle hVerOrNewLine;
Handle hKillLine;
Handle hPrep2;
Handle hKillLineAfter;
Handle hEnd;
Handle hDebugAutoTemplate;
Handle hAutoTemplate1;
Handle hAutoTemplateBC;
Handle hAutoTemplateBCShow;
Handle hAutoTemplateMar;
Handle hAutoTemplateWrite;
Handle hWndTurn = NULL;
Handle hDebugPreprocess;
Handle hDebugPrintResolution;

int KVO = 1;
int move;

extern uchar* Buffer;
extern uchar* WorkMem;

void RSTUFF_Init() {
    gLTInfo = new LinesTotalInfo;

    if (!SMetric_Init(0xFFFF, 0))
        throw RStuffException("SMetric_Init failed");

    if (!RLINE_Init(0, 0))
        throw RStuffException("RLINE_Init failed");
}

void RSTUFF_Done() {
    delete gLTInfo;
    gLTInfo = NULL;

    if (!SMetric_Done())
        return;

    if (!RLINE_Done())
        return;
}

Bool32 RSTUFF_Reset() {
    SetReturnCode_rstuff((uint16_t) 0);
    return TRUE;
}

uint32_t RSTUFF_GetReturnCode() {
    return RC.gwRC;
}

Bool32 RSTUFF_SetImportData(uint32_t dwType, void * pData) {
    Bool rc = FALSE;
    RC.gwRC = 0;

    switch (dwType) {
    case RSTUFF_FN_SetProgresspoints:
        rc = SetCBProgressPoints(pData);
        break;
    default:
        SetReturnCode_rstuff((uint16_t) IDS_RSTUFF_ERR_NOTIMPLEMENT);
        rc = FALSE;
    }

    return rc;
}

Bool32 RSTUFF_RSBinarise(void) {
    SetReturnCode_rstuff((uint16_t) 0);
    return TRUE;
}

Bool32 RSTUFF_RSNormalise(PRSPreProcessImage Image, void* vBuff, int Size, void* vWork,
        int SizeWork) {
    SetReturnCode_rstuff((uint16_t) 0);
    SetMainBuff(vBuff, Size);
    SetWorkBuff(vWork, SizeWork);
    Bool32 rc = Normalise(Image);
    ReSetMem();
    return rc;
}

Bool32 RSTUFF_RSNormVerify(PRSPreProcessImage Image) {
    SetReturnCode_rstuff((uint16_t) 0);
    return VerifyN(Image);
}

Bool32 RSTUFF_RSNormRemoveLines(PRSPreProcessImage Image) {
    SetReturnCode_rstuff((uint16_t) 0);
    return KillLines(Image);
}

Bool32 RSTUFF_RSLayout(PRSPreProcessImage Image) {
    SetReturnCode_rstuff((uint16_t) 0);
    return TRUE;
}

void SetReturnCode_rstuff(int rc) {
    RC.gwRC = rc;
}

