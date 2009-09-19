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

#ifndef __R35_H
   #define __R35_H

   #ifndef __GLOBUS_H
   #include "globus.h"
   #endif
   #include "r35defs.h"
#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"

   #ifdef __R35__
      #define R35_FUNC  FUN_EXPO
   #else
      #define R35_FUNC  FUN_IMPO
   #endif

   #ifdef __cplusplus
   extern "C" {
   #endif

   R35_FUNC(Bool32)  R35InitNDX( MemFunc* mem);
   R35_FUNC(Bool32)  R35InitHND( MemFunc* mem);
   R35_FUNC(Bool32)  R35InitPRN( MemFunc* mem);
   R35_FUNC(Bool32)  R35Init( MemFunc* mem);
      // Start Initialization; TRUE - OK
      // for mem==NULL need used internal memory functions from DLL
      // New call - after close library without R35Done

   R35_FUNC(Bool32)  R35SetAlphabet(
        char*     char_tbl_put_to ); // char table[0-255]
      // Set alphabet for recognition

   R35_FUNC(Bool32)   R35Binarize(RecRaster *rRaster,
							   uint16_t *CompImage,int32_t dx, int32_t dy);
   R35_FUNC(Bool32)   R35Binarize8(RecRaster *rRaster,
							   uchar *CompImage,int32_t dx, int32_t dy);

   R35_FUNC(void)  R35SetPlatform( int32_t cpu );
   R35_FUNC(Bool32)   R35Pack(
       RecRaster*  raster,          // raster description
       uint16_t*  Image3x5,
       int32_t TO_X, int32_t TO_Y);       // result

   R35_FUNC(Bool32)   R35PackProport(
       RecRaster*  raster,          // raster description
       uint16_t*  Image3x5,
       int32_t TO_X, int32_t TO_Y);       // result

   R35_FUNC(Bool32)   R35PackProportHor16(
        RecRaster   *raster,
        uint16_t      *Image3x5, int32_t    *to_x,  int32_t   *to_y);

   R35_FUNC(Bool32)  R35RecogCharIm3x5(
      uint16_t*  Im3x5,         // image 3x5
      RecVersions* R35res );

   R35_FUNC(Bool32)  R35RecogCharIm3x5_expert(
      uint16_t*  Im3x5,         // image 3x5
      RecVersions* R35res );


   R35_FUNC(Bool32)  R35RecogPrintCharIm3x5(
      uint16_t*  Im3x5,         // image 3x5
      RecVersions* R35res );

   R35_FUNC(Bool32)  R35RecogPrintCharIm3x5_expert(
      uint16_t*  Im3x5,         // image 3x5
      RecVersions* R35res, Bool32 r5x3 );

   R35_FUNC(Bool32)  R35RecogNdxIm3x5(
      uint16_t*  Im3x5,         // image 3x5
      RecVersions* R35res );

   R35_FUNC(Bool32)  R35RecogNdxIm3x5_expert(
      uint16_t*  Im3x5,         // image 3x5
      RecVersions* R35res );

   R35_FUNC(void)  R35Done(void);
      // Stop function. Call after R35Init()

   R35_FUNC(int16_t) R35GetErr(void);
      // Err16.h defined codes or one of the next
  R35_FUNC(Bool32)  R35Init_learn( int32_t num, uchar level, int32_t FontType );
  R35_FUNC(void)    R35Done_learn( int32_t FontType);
  R35_FUNC(Bool32)  R35RecogCharIm3x5_learn(
			uchar Code,uint16_t* Im3x5/*, Bool32 r5x3*/, int32_t FontType  );
  R35_FUNC(Bool32)  R35RecogCharIm3x5_learn_expert(
            uchar Code,uint16_t* Im3x5)  ;
  R35_FUNC(Bool32) R35Delete(uchar    let, int32_t num_del);
  R35_FUNC(Bool32) R35Add(uchar    let,uint16_t* Im3x5, int32_t FontType);
  R35_FUNC(void)   R35SetMTR(int fnt) ;

// error code
#define ER_R35_NO_ERROR  0
#define ER_R35_NO_RECOG  1
#define ER_R35_TOO_SMALL 2
#define ER_R35_OPEN	     3
#define ER_R35_READ	     4
#define ER_R35_MEMORY	 5
#define ER_R35_NO_LOAD	 6
#define ER_R35_NO_LEVEL	 7
#define ER_R35_ALPHA     8
#define ER_R35_WRITE     9

   #ifdef __cplusplus
            }
   #endif

#endif

