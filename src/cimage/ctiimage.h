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

#include <string>

#include "globus.h"
#include "imageinfo.h"
#include "common/singleton.h"
#include "common/rect.h"

#ifdef __CIMAGE__
#define CIMAGE_FUNC  FUN_EXPO__
#else
#define CIMAGE_FUNC  FUN_IMPO__
#endif

namespace cf
{
class BitMask;
class CTIControl;
typedef Singleton<CTIControl, CreateUsingStatic> CImage;
}

CIMAGE_FUNC void CIMAGE_Init();
CIMAGE_FUNC Bool32 CIMAGE_WriteCallbackImage(const char *, CIMAGEIMAGECALLBACK);
CIMAGE_FUNC Bool32 CIMAGE_GetCallbackImage(const char *, CIMAGEIMAGECALLBACK*);
CIMAGE_FUNC bool CIMAGE_AddImage(const std::string& name, BitmapHandle handle);
CIMAGE_FUNC bool CIMAGE_AddImageCopy(const std::string& name, BitmapHandle handle);
CIMAGE_FUNC bool CIMAGE_RemoveImage(const std::string& name);
CIMAGE_FUNC bool CIMAGE_ReadDIB(const std::string& name, BitmapHandle * dest);
CIMAGE_FUNC bool CIMAGE_ReadDIBCopy(const std::string& name, BitmapHandle * dest);
CIMAGE_FUNC Bool32 CIMAGE_GetData(const char * , CIMAGE_InfoDataInGet*, CIMAGE_InfoDataOutGet*);
CIMAGE_FUNC bool CIMAGE_GetDIBData(const std::string& name, const cf::Rect& r, cf::BitMask * bitMask, BitmapHandle * dest);
CIMAGE_FUNC bool CIMAGE_GetImageInfo(const std::string& name, BitmapInfoHeader * dest);
CIMAGE_FUNC bool CIMAGE_FreeCopiedDIB(BitmapHandle dib);
CIMAGE_FUNC void CIMAGE_Reset();
CIMAGE_FUNC bool CIMAGE_AddReadCloseRect(const std::string& name, const cf::Rect& r);
CIMAGE_FUNC bool CIMAGE_RemoveReadCloseRect(const std::string& name, const cf::Rect& r);
CIMAGE_FUNC bool CIMAGE_AddWriteCloseRect(const std::string& name, const cf::Rect& r);
CIMAGE_FUNC bool CIMAGE_RemoveWriteCloseRect(const std::string& name, const cf::Rect& r);

Bool16 CIMAGE_Callback_ImageOpen(CIMAGE_ImageInfo* lpImageInfo);
uint16_t CIMAGE_Callback_ImageRead(pchar lpImage, uint16_t wMaxSize);
Bool16 CIMAGE_Callback_ImageClose();

#endif
