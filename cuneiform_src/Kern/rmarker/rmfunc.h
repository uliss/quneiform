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

#ifndef _RMFUNC_H_
#define _RMFUNC_H_

#include <stdlib.h>
#include <stdio.h>
#include "ctiimage.h"
#include "ccomdefs.h"
#include "cpage.h"
#include "mpumatime.h"

typedef int Bool;


# define  TYPE_BIG_COMP		CPAGE_GetInternalType("TYPE_BIG_COMP")
struct BIG_IMAGE
{
	CCOM_handle hCCOM;
	uchar ImageName[CPAGE_MAXNAME];
};


Bool32 ShortVerticalLinesProcess ( uint32_t Step, PRMPreProcessImage Image );
Bool32 PageMarkup(PRMPreProcessImage Image);
Bool32    SetCBProgressPoints(void *);
uint32_t GetReturnCode_rmarker(void);
Bool32 SearchPictures(PRMPreProcessImage,BIG_IMAGE );
Bool32 SearchNeg(PRMPreProcessImage,BIG_IMAGE,int);
Bool32 SearchFon(PRMPreProcessImage,BIG_IMAGE,int);
void GiveMainBuff (void **vvBuff, int *Size);
void GiveWorkBuff (char **ccBuff, int *Size);
void SetMainBuff(void *vBuff, int Size);
void SetWorkBuff(void *vBuff, int Size);
void ReSetMem(void);
void MySetNegative (void *vB, Handle hCPage);
//
#define DEC_CB_TYPE(a)  PF##a
#define DEC_CB_FUN(a,b,c) typedef a (*DEC_CB_TYPE(b))c; a b c;
DEC_CB_FUN(void,    ProgressStart,      (void))
DEC_CB_FUN(Bool32,  ProgressStepAutoLayout,  (uint32_t,  uint32_t))
DEC_CB_FUN(Bool32,  ProgressStepSearchTables, (uint32_t,  uint32_t))
DEC_CB_FUN(void,    ProgressFinish,     (void))
DEC_CB_FUN(Bool32,  InitPRGTIME,        (void))
DEC_CB_FUN(PRGTIME, StorePRGTIME,       (uint32_t, uint32_t))
DEC_CB_FUN(void,    RestorePRGTIME,     (PRGTIME))
DEC_CB_FUN(Bool32,  DonePRGTIME,        (void))
DEC_CB_FUN(Bool32,  DPumaSkipComponent, (void))
DEC_CB_FUN(Bool32,  DPumaSkipTurn,      (void))
DEC_CB_FUN(Bool32,  rexcProgressStep,   (uint32_t))
DEC_CB_FUN(void,    SetUpdate,          (uint32_t, uint32_t))
DEC_CB_FUN(char *,  GetModulePath,      (void))
#undef DEC_CB_FUN

#endif
