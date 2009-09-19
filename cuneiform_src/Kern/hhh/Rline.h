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

#ifndef __RLINE_H
#define __RLINE_H

#include "globus.h"

/*
#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"
*/
#ifdef __RLINE__
  #define RLINE_FUNC  FUN_EXPO
#else
  #define RLINE_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
        extern "C" {
#endif

#define __XTYPES_H
#define RLINE_BLOCK_TYPE CPAGE_GetInternalType("RLINE_BLOCK_TYPE")

RLINE_FUNC(Bool32) RLINE_Init(uint16_t wHeightCode,void * hStorage);
RLINE_FUNC(Bool32) RLINE_Done();
RLINE_FUNC(uint32_t) RLINE_GetReturnCode();
RLINE_FUNC(char *) RLINE_GetReturnString(uint32_t dwError);
RLINE_FUNC(Bool32) RLINE_GetExportData(uint32_t dwType, void * pData);
RLINE_FUNC(Bool32) RLINE_SetImportData(uint32_t dwType, void * pData);
/////////////////////////////////////////////////////////////

typedef enum
{
        RLINE_FNRLINE_SearchLines =1,
        RLINE_FNRLINE_DeleteLines,
        RLINE_Bool32_NOFILLGAP3,
        RLINE_Bool32_NOHBORDER,
        RLINE_Bool32_NOVBORDER,
		RLINE_FNRLINE_LinesPass1,
		RLINE_FNRLINE_LinesPass2,
		RLINE_FNRLINE_LinesPass3
} RLINE_EXPORT_ENTRIES;


#define DEC_FUN(a,b,c) typedef a (*FN##b)c; RLINE_FUNC(a) b c

DEC_FUN(Bool32,  RLINE_SearchLines,(void* lpInPage,void* hpCLINE));
DEC_FUN(Bool32,  RLINE_DeleteLines,(void* lpInPage, const char* lpOutDIB));
DEC_FUN(Bool32,  RLINE_LinesPass1, (Handle hCPage,Handle hCCOM,void* phCLINE,PBool32 pgneed_clean_line, Bool32 sdl, uchar lang));
DEC_FUN(Bool32,  RLINE_LinesPass2, (Handle hCCOM,void* phCLINE, Handle hCPAGE));
DEC_FUN(Bool32,  RLINE_LinesPass3, (Handle hCPage, Handle hCLINE, Handle hCCOM, uchar lang));
//DEC_FUN(Bool32,  RLINE_DeleteLines,(void* lpInName , void* lpDIB, void** lpOutName));

/*
DEC_FUN();
DEC_FUN();
DEC_FUN();
*/
#undef DEC_FUN


#ifdef __cplusplus
            }
#endif

#endif
