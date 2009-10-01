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

#ifndef __LEO_H
   #define __LEO_H

   #ifndef __GLOBUS_H
   #include "globus.h"
   #endif

#ifndef __LEODEFS_H
   #include "leodefs.h"
#endif

#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif

#ifndef __MEMFUNC_H
   #include "memfunc.h"
#endif

#ifndef __SNPDEFS_H
   #include "snpdefs.h"
#endif

   #ifdef __LEO__
      #define LEO_FUNC  FUN_EXPO
   #else
      #define LEO_FUNC  FUN_IMPO
   #endif

   #ifdef __cplusplus
   extern "C" {
   #endif

   LEO_FUNC(Bool32)  LEOInit( MemFunc* mem );
   LEO_FUNC(Bool32)  LEOInitPass2( void );
      // Start Initialization; TRUE - OK
      // for mem==NULL need used internal memory functions from DLL
      // New call - after close library without LEODone

   LEO_FUNC(Bool32)  LEOSetupPage(LeoPageSetup* ps);
   LEO_FUNC(Bool32)  LEOSetupField(LeoFieldSetup* fs);

   LEO_FUNC(Bool32)  LEORecogPrintChar(RecObject* obj  );
   LEO_FUNC(Bool32)  LEORecogChar(RecObject* obj  );
   LEO_FUNC(Bool32)  LEORecogInd(RecObject* obj  );

   LEO_FUNC(Bool32)  LEORecogCharDebug(RecObject* obj  );
//
   LEO_FUNC(Bool32)  LEORecogChar_expert(RecObject*  object);
   LEO_FUNC(Bool32)  LEORecogCharPRN_expert(RecObject*  object);
// container for Recrasters
   LEO_FUNC(Bool32)  LEORecogId_expert(int32_t id_page,RecVersions *exp);
   LEO_FUNC(Bool32)  LEORecogId_Char(int32_t id_page, RecVersions *resin, RecVersions *resout);
   LEO_FUNC(Bool32)  LEOValidId_Char(int32_t id_page, RecVersions *resin, RecVersions *resout);
   LEO_FUNC(Bool32)  LEORecogRestore_Char(RecVersions *resin, RecVersions *resout);
   LEO_FUNC(uchar)   LEOValidRestore_Char( RecVersions *resin,RecVersions *resout);
   LEO_FUNC(Bool32)  LEOSetValid(int16_t id_rast,uchar code, uchar valid,uchar control);
   LEO_FUNC(Bool32)  LEODelFinal(int16_t id_rast);
   LEO_FUNC(int16_t)   LEO_ContStore(RecRaster *r,uchar let, uchar nLns,Rect16 *rect,uchar IsPrint,
		uchar	Prob, uchar Valid, RecVersions *Res);
   LEO_FUNC(int32_t)   LEO_GetGlobalIncline(void);
   LEO_FUNC(Bool32)  LEOPushAlphabetType(uchar alpha_type, uchar isPrint);
   LEO_FUNC(Bool32)  LEO_ContRestoreObject( int16_t       idr, RecObject *object,
                                            uchar *alpha, uchar *isPrint);
//                                     id_page can be >0
// to be deleted after implementation of LEOSetupField(), 28.02.97 20:50, VP
   LEO_FUNC(Bool32) LEOSetAlphabet( char*     char_tbl_put_to   );
   LEO_FUNC(void)   LEOSetFont( int32_t typ_of_font);

   LEO_FUNC(void)   LEOCloseCont(void);
   LEO_FUNC(int32_t)  LEOOpenCont(void);
   LEO_FUNC(void)   LEODone(void);
   LEO_FUNC(void)   LEODonePass2(void);
   LEO_FUNC(Bool32) LEOFonRerecogCTB(char *CTBname);
   LEO_FUNC(Bool32) LEO_GetStoringMode(void);
   LEO_FUNC(void)   LEO_SetStoringMode(Bool32 Mode);

   LEO_FUNC(Bool32)  LEORecogHndMethod( RecObject*  object , int Method, int gra_type);
   LEO_FUNC(Bool32)  LEORecogPrnMethod( RecObject*  object , int Method, int gra_type);

    // Stop function. Call after LEOInit()
   LEO_FUNC(char *) LEOGetMetName(int32_t method, char *met_name);
   LEO_FUNC(int16_t)  LEOGetErr(void);
      // Err16.h defined codes or one of the next
         #define ER_LEO_NO_ERROR       0
         #define ER_LEO_NULL_OBJECT    1
         #define ER_LEO_SMALL_OBJECT   2
         #define ER_LEO_LARGE_OBJECT   3
         #define ER_LEO_NOINIT_R35     4
         #define ER_LEO_NOINIT_NET     5
         #define ER_LEO_NOINIT_TRE     6
         #define ER_LEO_NOINIT_IND     7
         #define ER_LEO_NOINIT_EVN     8
         #define ER_LEO_NOINIT_NCU     9
         #define ER_LEO_OPEN           10


   LEO_FUNC(int32_t)  LEOGetCPU(void);
   LEO_FUNC(void)   LEOSetPlatform(int32_t Cpu);
   LEO_FUNC(void)   LEOSortVersProb(RecVersions *v);

   LEO_FUNC( void   )      LEORegisterSnpTree(
            SnpTreeNode* parent,         // parent Snp Node, may be NULL
            __SnpToolBox* p_snp_tools     // tools complect, may be NULL
                                 );
   LEO_FUNC( void   )      LEOIndRegisterSnpTree(
            SnpTreeNode* parent,         // parent Snp Node, may be NULL
            __SnpToolBox* p_snp_tools     // tools complect, may be NULL
                                 );
      // Registartion SubTree for Debug  (SNP)


   LEO_FUNC(void)    LEOSnpFullRecog(RecRaster *rec);

// sizes container
   LEO_FUNC(void)   LEO_ClearStat(void);
   LEO_FUNC(int32_t)  LEO_AddStat(int32_t name,int32_t width,int32_t height,int32_t valid);
   LEO_FUNC(int32_t)  LEO_AddStatLetter(int32_t name,int32_t width,int32_t height,int32_t valid);
   LEO_FUNC(int32_t)  LEO_GetCommonStat(int32_t *sizes,int32_t *ocenka);
   LEO_FUNC(int32_t)  LEO_GetLetStat(int32_t name,int32_t *sizes,int32_t *ocenka);


   LEO_FUNC(int16_t) LEO_GetNoCutPoint(uchar *RASTER,  int16_t WB, uchar NWIDTH, uchar NLENGTH);
   LEO_FUNC(int32_t)   LEO_Str2FldNo(int32_t  str_no);
   // convert string number to field number. -1 if bound error
   LEO_FUNC(Bool32) LEO_StoreCollection(RecVersions *ver);
   // store ANSI(Windows) collection
   LEO_FUNC(void)  LEOFreeAlphabets(void);

   #ifdef __cplusplus
            }
   #endif

#endif

