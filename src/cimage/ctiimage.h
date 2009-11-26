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

#ifndef __CIMAGE_H
#define __CIMAGE_H

#include "globus.h"
#include "imageinfo.h"
#include "singleton.h"

#ifdef __CIMAGE__
#define CIMAGE_FUNC  FUN_EXPO__
#else
#define CIMAGE_FUNC  FUN_IMPO__
#endif

class CTIControl;
namespace CIF
{
typedef Singleton<CTIControl> CImage;
}

enum {
    CIMAGE_MAXNAME = 260
};

CIMAGE_FUNC void CIMAGE_Init();

typedef enum {
    CIMAGE_FN_WriteCallbackImage = 1,
    CIMAGE_FN_GetCallbackImage,
    CIMAGE_FN_WriteDIB,
    CIMAGE_FN_ReadDIB,
    CIMAGE_FN_GetData,
    CIMAGE_FN_GetDIBData,
    CIMAGE_FN_ReplaceData,
    CIMAGE_FN_GetImageInfo,
    CIMAGE_FN_DeleteImage,
    CIMAGE_FN_FreeCopedDIB,
    CIMAGE_FN_FreeBuffers,
    CIMAGE_FN_Reset,
    CIMAGE_FN_AddReadCloseRects,
    CIMAGE_FN_RemoveReadCloseRects,
    CIMAGE_FN_AddWriteCloseRects,
    CIMAGE_FN_RemoveWriteCloseRects,
    CIMAGE_FN_EnableMask
} CIMAGE_EXPORT_ENTRIES;
#define DEC_FUN(a,b,c) CIMAGE_FUNC a CIMAGE_##b c;
DEC_FUN(Bool32, WriteCallbackImage, (const char *, CIMAGEIMAGECALLBACK))
DEC_FUN(Bool32, GetCallbackImage, (const char * , CIMAGEIMAGECALLBACK*))
DEC_FUN(Bool32, WriteDIB, (const char * , Handle, uint32_t))
DEC_FUN(Bool32, ReadDIB, (const char * , Handle*, uint32_t))
DEC_FUN(Bool32, GetData, (const char * , CIMAGE_InfoDataInGet*, CIMAGE_InfoDataOutGet*))
DEC_FUN(Bool32, GetDIBData, (const char * , CIMAGE_InfoDataInGet*, void**))
DEC_FUN(Bool32, ReplaceData, (const char * , CIMAGE_InfoDataInReplace*))
DEC_FUN(Bool32, GetImageInfo, (const char * , BitmapInfoHeader*))
DEC_FUN(Bool32, DeleteImage, (const char * ))
DEC_FUN(Bool32, FreeCopedDIB, (Handle))
DEC_FUN(Bool32, FreeBuffers, (void))
DEC_FUN(void, Reset, (void))
DEC_FUN(Bool32, AddReadCloseRects, (const char *, uint32_t, CIMAGE_Rect*))
DEC_FUN(Bool32, RemoveReadCloseRects, (const char *, uint32_t, CIMAGE_Rect*))
DEC_FUN(Bool32, AddWriteCloseRects, (const char *, uint32_t, CIMAGE_Rect*))
DEC_FUN(Bool32, RemoveWriteCloseRects, (const char *, uint32_t, CIMAGE_Rect*))
DEC_FUN(Bool32, EnableMask, (const char*, const char*, Bool32))
#undef DEC_FUN
#define DEC_CB_FUN(a,b,c) typedef a (*FNCIMAGE##b)c; a CIMAGE_##b c;
DEC_CB_FUN(Bool16, Callback_ImageOpen, (CIMAGE_ImageInfo* lpImageInfo))
DEC_CB_FUN(uint16_t, Callback_ImageRead, (pchar lpImage, uint16_t wMaxSize))
DEC_CB_FUN(Bool16, Callback_ImageClose, (void))

#undef DEC_CB_FUN

#endif
