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

#include <iostream>
#include <boost/current_function.hpp>

#include "resource.h"
#include "ctiimage.h"
#include "cticontrol.h"

using namespace cf;

#define CIMAGE_DEBUG() std::cerr << BOOST_CURRENT_FUNCTION << "\n"

bool CIMAGE_WriteCallbackImage(const std::string& name, CIMAGEIMAGECALLBACK Cbk)
{
    return CImage::instance().writeImageCallbacks(name, Cbk);
}

bool CIMAGE_GetCallbackImage(const std::string& name, CIMAGEIMAGECALLBACK * pCbk)
{
    return CImage::instance().getImageCallbacks(name, pCbk);
}

bool CIMAGE_AddImage(const std::string& name, BitmapHandle handle)
{
    return CImage::instance().addImage(name, handle);
}

bool CIMAGE_AddImageCopy(const std::string& name, BitmapHandle handle)
{
    return CImage::instance().addImageCopy(name, handle);
}

bool CIMAGE_ReadDIB(const std::string& name, BitmapHandle * dest)
{
    BitmapHandle img = CImage::instance().image(name);
    if(!img)
        return false;

    assert(dest);
    *dest = img;
    return true;
}

bool CIMAGE_ReadDIBCopy(const std::string& name, BitmapHandle * dest)
{
    BitmapHandle copy = CImage::instance().imageCopy(name);

    if(!copy)
        return false;

    assert(dest);
    *dest = copy;
    return true;
}

bool CIMAGE_GetRawData(const std::string& name, CIMAGE_InfoDataInGet * in,
                      CIMAGE_InfoDataOutGet * out)
{
    return CImage::instance().getImageRawData(name, in, out);
}

bool CIMAGE_GetDIBData(const std::string& name, const Rect& r, cf::BitMask * bitMask, BitmapHandle * dest)
{
    return CImage::instance().getDIBFromImage(name, r, bitMask, dest);
}

bool CIMAGE_GetImageInfo(const std::string &name, BitmapInfoHeader * dest)
{
    assert(dest);

    BitmapHandle handle = CImage::instance().image(name);

    if(!handle)
        return false;

    *dest = (*handle);
    return true;
}

bool CIMAGE_RemoveImage(const std::string& name)
{
    return CImage::instance().removeImage(name);
}

bool CIMAGE_FreeCopiedDIB(BitmapHandle dib)
{
    return CImage::instance().free(dib);
}

void CIMAGE_Reset()
{
    CImage::instance().reset();
}

bool CIMAGE_AddReadCloseRect(const std::string& name, const cf::Rect& r)
{
    return CImage::instance().addRectToReadMask(name, r);
}

bool CIMAGE_RemoveReadCloseRect(const std::string& name, const Rect& r)
{
    return CImage::instance().removeRectFromReadMask(name, r);
}

bool CIMAGE_AddWriteCloseRect(const std::string& name, const cf::Rect& r)
{
    return CImage::instance().addRectToWriteMask(name, r);
}

bool CIMAGE_RemoveWriteCloseRect(const std::string& name, const cf::Rect& r)
{
    return CImage::instance().removeRectFromWriteMask(name, r);
}

// For GetCBImage
Bool16 CIMAGE_Callback_ImageOpen(CIMAGE_ImageInfo * lpImageInfo)
{
    return CImage::instance().CBImageOpen(lpImageInfo);
}

uint16_t CIMAGE_Callback_ImageRead(char * lpImage, uint16_t wMaxSize)
{
    return CImage::instance().CBImageRead(lpImage, wMaxSize);
}

Bool16 CIMAGE_Callback_ImageClose()
{
    return CImage::instance().CBImageClose();
}
