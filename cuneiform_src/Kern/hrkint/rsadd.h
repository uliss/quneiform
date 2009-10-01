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
// ANALIZER OF THE CELL OF TABLES                         //
////////////////////////////////////////////////////////////
// O.Slavin                                               //
////////////////////////////////////////////////////////////
// Creation     : 24 Seb 1999                             //
////////////////////////////////////////////////////////////

#ifndef __RAGR_H
#define __RAGR_H

#include "globus.h"
#include "recdefs.h"
#include "memfunc.h"

#ifdef __RSADD__
#define RSADD_FUNC  FUN_EXPO
#else
#define RSADD_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum RSADDFunctionsExt {
	RSADD_TAKE,
	RSADD_GETALIASES,
	RSADD_GETALIASCLASS,
	RSADD_KILL_DUSTS,
	RSADD_SETRSTR
};

RSADD_FUNC(Bool32) RSADD_Init( uint16_t wHeightCode, Handle hStorage );
RSADD_FUNC(void) RSADD_Done(void);
RSADD_FUNC(uint32_t) RSADD_GetReturnCode(void);
RSADD_FUNC(char*) RSADD_GetReturnString(uint32_t dwError);
RSADD_FUNC(Bool32) RSADD_SetImportData(uint32_t dwType, void * pData);
RSADD_FUNC(Bool32) RSADD_GetExportData(uint32_t dwType, void * pData);

// 1	RSADD_TAKE			главная функция слияния двух строк
typedef Bool32 (*FNRSADD_take)(CSTR_line lrus, CSTR_line leng);
RSADD_FUNC(Bool32) RSADD_take(CSTR_line lrus,CSTR_line leng);
// 2	RSADD_GETALIASES	список родственных символов
typedef uchar* (*FNRSADD_get_aliases)(uchar letter, uchar language,
		uchar alias_class);
RSADD_FUNC(uchar*) RSADD_get_aliases( uchar letter, uchar language, uchar alias_class);
// 3	RSADD_GETALIASCLASS	класс родственных символов
#define RSADD_as_none       0
#define RSADD_as_equal      1
#define RSADD_as_alias      2
#define RSADD_as_similar    4
typedef uchar (*FNRSADD_get_alias_class)(uchar letter, uchar language);
RSADD_FUNC(uchar) RSADD_get_alias_class(uchar letter,uchar language);
// 4 RSADD_KILL_DUSTS		уничтожить лишние дусты
typedef void (*FNRSADD_kill_dusts)(CSTR_line lrus);
RSADD_FUNC(void ) RSADD_kill_dusts(CSTR_line lrus);
// 5 RSADD_SETRSTR			приём информации и функций из б-ки RSTR
typedef void (*FNRSADD_SetRSTR)(uchar status, uchar camera, Handle monitor_ori,
		Handle activity, Handle show_text);
RSADD_FUNC(void) RSADD_SetRSTR( uchar status,uchar camera,
		Handle monitor_ori,
		Handle activity,
		Handle show_text);
#ifdef __cplusplus
}
#endif

#endif
