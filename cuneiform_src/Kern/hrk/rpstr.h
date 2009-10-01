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

////////////////////////////////////////////////////////////
// POST CORRECTION STRINGS FROM CONTAINER                 //
////////////////////////////////////////////////////////////
// O.Slavin                                               //
////////////////////////////////////////////////////////////
// Creation     : 26 Mar 1999                             //
////////////////////////////////////////////////////////////
// Modification : 26 Mar 1999                             //
////////////////////////////////////////////////////////////
// Copyright (C) Cognitive Technologies Ltd, 1999         //
// All rights reserved. This program is proprietary and   //
// a trade secret of Cognitive Technologies Limited.      //
////////////////////////////////////////////////////////////
#ifndef __RPSTR_H
#define __RPSTR_H

#include "globus.h"
#include "ccomdefs.h"
#include "cstrdefs.h"

#ifndef __RECDEFS_H
#include "recdefs.h"
#endif
#include "memfunc.h"

#ifdef __RPSTR__
#define RPSTR_FUNC  FUN_EXPO
#else
#define RPSTR_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
extern "C" {
#endif

RPSTR_FUNC(Bool32) RPSTR_Init( uint16_t wHeightCode , Handle hStorage);
RPSTR_FUNC(void) RPSTR_Done(void);
RPSTR_FUNC(uint32_t) RPSTR_GetReturnCode(void);
RPSTR_FUNC(char*) RPSTR_GetReturnString(uint32_t dwError);
RPSTR_FUNC(Bool32) RPSTR_SetImportData(uint32_t dwType, void * pData);
RPSTR_FUNC(Bool32) RPSTR_GetExportData(uint32_t dwType, void * pData);

enum RPSTRFunctionsExt {
	RPSTR_FNNEWPAGE,
	RPSTR_FNCORRECTSPELL,
	RPSTR_FNCORRECTINCLINE,
	RPSTR_FNCOLLECTCAPDROP,
	RPSTR_FNNORMVERTSTR,
	RPSTR_FNCORRECTLINESPELL
};
enum RPSTRFunctionsImp {
	RPSTR_FNIMP_ALLOC,
	RPSTR_FNIMP_REALLOC,
	RPSTR_FNIMP_FREE,
	RPSTR_FNIMP_OPEN,
	RPSTR_FNIMP_CLOSE,
	RPSTR_FNIMP_READ,
	RPSTR_FNIMP_WRITE,
	RPSTR_FNIMP_LANGUAGE,
	RPSTR_Bool32_Fax100
};

// 1    RPSTR_FNNEWPAGE          init new page
typedef Bool32 (*FNRPSTR_NewPage)(void);
RPSTR_FUNC(Bool32) RPSTR_NewPage(void);
// 2    RPSTR_FNCORRECTSPELL      spelling checker
typedef Bool32 (*FNRPSTR_CorrectSpell)(int32_t version);
RPSTR_FUNC(Bool32) RPSTR_CorrectSpell(int32_t version);
// 3    RPSTR_FNCORRECTINCLINE
typedef Bool32 (*FNRPSTR_CorrectIncline)(int32_t version);
RPSTR_FUNC(Bool32) RPSTR_CorrectIncline(int32_t version);
// 4    RPSTR_FNCOLLECTCAPDROP
typedef Bool32 (*FNRPSTR_CollectCapDrops)(int32_t version);
RPSTR_FUNC(Bool32) RPSTR_CollectCapDrops(int32_t version);
// 5    RPSTR_FNNORMVERTSTR
typedef void (*FNRPSTR_NormalizeVertStr)(void);
RPSTR_FUNC(void) RPSTR_NormalizeVertStr(void);
// 6 RPSTR_FNCORRECTLINESPELL      spelling checker for given line
typedef Bool32 (*FNRPSTR_CorrectLineSpell)(CSTR_line line, int32_t line_num,
		CSTR_rast* re, CSTR_rast* rb, int32_t* rf);
RPSTR_FUNC(Bool32) RPSTR_CorrectLineSpell(CSTR_line line, int32_t line_num, CSTR_rast* re, CSTR_rast* rb, int32_t* rf);

#ifdef __cplusplus
}
#endif

#endif
