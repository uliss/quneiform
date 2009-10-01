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

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                                                          //
//                   T I G E R   O C R   L I B R A R Y                      //
//                                                                          //
//                                                                          //
//           File       :  TGCALLBK.H                                       //
//           Contains   :  External TIGER OCR library definitions           //
//           Written by :  Alexander Talalai, Yuri Panchul                  //
//           Compiler   :  Microsoft C v6.0, WATCOM C/386 v8.5              //
//                                                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
# ifndef TGCALLBK_H_INCLUDED
# define TGCALLBK_H_INCLUDED
//////////////////////////////////////////////////////////////////////////////
# ifdef __cplusplus
 extern "C"
{
# endif
//////////////////////////////////////////////////////////////////////////////
#pragma pack(push,1)
//////////////////////////////////////////////////////////////////////////////
# ifndef TGCALL_H_INCLUDED
typedef short int INT16;
#include "dlltypes.h"
# endif
//////////////////////////////////////////////////////////////////////////////
//                   Callback functions from TIGER.DLL                      //
//////////////////////////////////////////////////////////////////////////////
//   Obtain image functions set                                             //
//////////////////////////////////////////////////////////////////////////////
struct Tiger_ImageInfo
{
    uint16_t  wImageHeight;           // Height of the image in lines
    uint16_t  wImageWidth;            // Width of the image in pixels
    uint16_t  wImageByteWidth;        // Size of the image line in bytes
    uint16_t  wImageDisplacement;     // Displacement of the first pixel of
                                  //   image line in pixels, usually 0
    uint16_t  wResolutionX;           // Resolution by X-axe
    uint16_t  wResolutionY;           // Resolution by Y-axe
    uchar  bFotoMetrics;           // White pixel
    uchar  bUnused;                // not used; for alignment purpose only

    uint16_t  wAddX;
    uint16_t  wAddY;
};
//////////////////////////////////////////////////////////////////////////////
typedef struct Tiger_ImageInfo	TIGER_IMAGEINFO;
typedef TIGER_IMAGEINFO TIGERFAR *   LPTIGER_IMAGEINFO;
//////////////////////////////////////////////////////////////////////////////
FNBOOL16               Tiger_Callback_ImageOpen (LPTIGER_IMAGEINFO lpImageInfo);
//////////////////////////////////////////////////////////////////////////////
FNBOOL16            Tiger_Callback_ImageOpen_xy (LPTIGER_IMAGEINFO lpImageInfo,
												 uint16_t x,
												 uint16_t y,
												 uint16_t w,
												 uint16_t h);
//////////////////////////////////////////////////////////////////////////////
FNDWORD          Tiger_Callback_ProcessPictures (char* buf,
												 DWORD lth);
//////////////////////////////////////////////////////////////////////////////
FNWORD                 Tiger_Callback_ImageRead (char* lpImage,
												 uint16_t wMaxSize);
//////////////////////////////////////////////////////////////////////////////
FNBOOL16              Tiger_Callback_ImageClose (void);
//////////////////////////////////////////////////////////////////////////////
//   Warrning !!!                                                           //
//   Tiger_Callback_ImageClose could be invoked even                        //
//   if Tiger_Callback_ImageOpen returned                                   //
//   bad status or was not opened at all                                    //
//////////////////////////////////////////////////////////////////////////////
//   Intermediate error message                                             //
//////////////////////////////////////////////////////////////////////////////
FNVOID               Tiger_Callback_ReportError (uint16_t wStatus,
												 char* lpMessage);
//////////////////////////////////////////////////////////////////////////////
//   Progress Monitor                                                       //
//                                                                          //
//   Each of progress monitor functions return TRUE if TIGER have to        //
//   continue work and FALSE to cancel process                              //
//////////////////////////////////////////////////////////////////////////////
FNBOOL16           Tiger_Callback_ProgressStart (void);
//////////////////////////////////////////////////////////////////////////////
FNBOOL16          Tiger_Callback_ProgressFinish (void);
//////////////////////////////////////////////////////////////////////////////
FNBOOL16         Tiger_Callback_ProgressSetPerc (uint16_t wPercent);
//////////////////////////////////////////////////////////////////////////////
FNBOOL16        Tiger_Callback_ProgressSetStage (uint16_t wStageNumber,
												 char* lpStageName,
												 uint16_t wVolume);
//////////////////////////////////////////////////////////////////////////////
FNBOOL              Tiger_Callback_Get_One_Word (char* Buffer );
//////////////////////////////////////////////////////////////////////////////
FNBOOL16                  Swp_ImageOpen         (LPTIGER_IMAGEINFO lpImageInfo);
//////////////////////////////////////////////////////////////////////////////
FNWORD                    Swp_ImageRead         (char* lpImage,
												 uint16_t wMaxSize);
//////////////////////////////////////////////////////////////////////////////
FNBOOL16                  Swp_ImageClose        (void);
//////////////////////////////////////////////////////////////////////////////
#ifndef _MSC_VER
FNBOOL16                  Tiger_FindLinesInTiff (char* name,
												 uint16_t x,
												 uint16_t y,
												 BOOL16 TIGERFUN(*)(LPTIGER_IMAGEINFO),
												 uint16_t   TIGERFUN(*)(char* *,uint16_t ),
												 BOOL16 TIGERFUN(*)());
//////////////////////////////////////////////////////////////////////////////
FNVOID                    Tiger_FindLinesClose  (void);
//////////////////////////////////////////////////////////////////////////////
#else
FNBOOL16                  Tiger_FindLinesInTiff (char* name,
												 uint16_t x,
												 uint16_t y,
												 BOOL16 (TIGERFUN(*))(LPTIGER_IMAGEINFO),
												 uint16_t   (TIGERFUN(*))(char* *,uint16_t ),
												 BOOL16 (TIGERFUN(*))());
//////////////////////////////////////////////////////////////////////////////
FNVOID                    Tiger_FindLinesClose  (void);
#endif
//////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
//////////////////////////////////////////////////////////////////////////////
# ifdef __cplusplus
}
# endif
//////////////////////////////////////////////////////////////////////////////
# endif
//////////////////////////////////////////////////////////////////////////////
// end of file
