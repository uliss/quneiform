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

#ifndef __LNS_H
   #define __LNS_H

#ifndef __IMGACCS_H
   #include "imgaccs.h"
#endif

#ifndef __LNSDEFS_H
   #include "lnsdefs.h"
#endif

#ifndef __SNPDEFS_H
   #include "snpdefs.h"
#endif

   #ifdef __LNS__
      #define LNS_FUNC    FUN_EXPO
   #else
      #define LNS_FUNC    FUN_IMPO
   #endif
	#include "cline.h"
typedef struct tagLnsSetupStr
{
   int32_t nOptions;
#define LSS_NOVBORDER_FILTER     0x0001   // отключение чистки на левой-правой границе
#define LSS_NOHBORDER_FILTER     0x0002   // отключение чистки на верхней-нижней границе
#define LSS_NOFILLGAP3_FILTER    0x0004   // отключение размазывания на 3 пикселя (дотматрикс)
   char  buffer[60]; // резерв, заполняется 0
} LnsSetupStr;

   #ifdef __cplusplus
      extern "C" {
   #endif

   LNS_FUNC( void )            LnsPageStart( TImageAccess* img );

   LNS_FUNC( Bool16 )          LnsSetup( LnsSetupStr* ls ); // can call before LnsExtractLines

   LNS_FUNC( Bool16 )          LnsExtractLines( int32_t   min_h_len,
                                                int32_t   min_v_len,
                                                int32_t*  result_h_count,
                                                int32_t*  result_v_count
                                              );
/*   LNS_FUNC( Bool16 )          MyLnsExtractLines( int32_t   min_h_len,
                                                int32_t   min_v_len,
                                                int32_t*  result_h_count,
                                                int32_t*  result_v_count
                                              );*/
   //LNS_FUNC( Bool16 )			ExtractAllComponents( LinesTotalInfo          *lti );
   LNS_FUNC( Bool16 )			ExtractAllEvents(CLINE_handle hCLINE, LinesTotalInfo          *lti);
   LNS_FUNC( Bool16 )          LnsGetCount( int32_t   min_h_len,
                                            int32_t   min_v_len,
                                            int32_t*  result_h_count,
                                            int32_t*  result_v_count
                                          );

   //.....выгрузка фрагментов..............................
   LNS_FUNC( int ) LnsGetFragCount( Bool horisontal );
   LNS_FUNC( LnsFrag* ) LnsGetFragments( Bool horisontal );
   //......................................................

   LNS_FUNC( Bool16 )          LnsUpload( LinesTotalInfo* lti,
                                          int32_t   min_h_len,
                                          int32_t   min_v_len
                                        );

   LNS_FUNC( Rect16* )         LnsGetBlackSquares( int32_t* count );
      // 04 dec 1998 - black squares upload
   LNS_FUNC( Rect16* )         LnsGetCheckBoxes( int32_t* count );
      // 11 feb 1998 - checkboxes upload

   LNS_FUNC( TImageAccess* )   LnsGetSweepedImage( LinesTotalInfo* lti );
   LNS_FUNC( Err16 )           LnsGetError();
   LNS_FUNC( void )            LnsPageFinish();

   //////////////////////////////////////////////////////////////////////
   // set-get options from lns32.ini (located at the same place as .exe)
   LNS_FUNC( void )   LnsSetSection( char* szSection ); // 63 chars max
   LNS_FUNC( uint32_t ) LnsGetProfileInt(const char* szKey, int nDefault );
   LNS_FUNC( uint32_t ) LnsGetProfileString( char* szKey,
                            char* szResult, int nResult,
                            char* szDefault// = NULL
                          );
   LNS_FUNC( Bool32 ) LnsWriteProfileInt( char* szKey, int nValue );
   LNS_FUNC( Bool32 ) LnsWriteProfileString( char* szKey, char* szValue );
   //////////////////////////////////////////////////////////////////////
   LNS_FUNC( void   )      LnsRegisterSnpTree(
                     SnpTreeNode* parent,         // parent Snp Node, may be NULL
                     __SnpToolBox* p_snp_tools     // tools complect, may be NULL
                              );


   #ifdef __cplusplus
               }
   #endif

#endif



