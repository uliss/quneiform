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

#include "ctimemory.h"
#include "ctiimagelist.h"
#include "ctiimageheader.h"

#include <algorithm>

using namespace std;

bool NameCompare(CTIImageHeader * image, const std::string& Name) {
    return image->ImageName() == Name;
}

CTIImageList::CTIImageList() {
}

CTIImageList::~CTIImageList() {
    remove_if(images_.begin(), images_.end(), bind2nd(ptr_fun(NameCompare), ""));
}

CTIImageList::Container::iterator CTIImageList::findImage(const std::string& Name) {
    return find_if(images_.begin(), images_.end(), bind2nd(ptr_fun(NameCompare), Name));
}

Bool32 CTIImageList::AddImage(const char *lpName, Handle hDIB, uint32_t wFlag) {
    DeleteImage(lpName);

    if (lpName == NULL && lpName[0] == 0x0) {
        SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGE_NAME);
        return FALSE;
    }

    if (hDIB == NULL) {
        SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGE_INFO);
        return FALSE;
    }

    CTIImageHeader * NewImage = new CTIImageHeader(lpName, hDIB, wFlag);
    images_.push_back(NewImage);

    return TRUE;
}

Bool32 CTIImageList::GetImage(const char *lpName, Handle* phDIB) {
    Container::iterator it = findImage(lpName);
    if (it == images_.end())
        return FALSE;

    *phDIB = (*it)->GetImageHandle();
    return TRUE;
}

Bool32 CTIImageList::DeleteImage(const char *lpName) {
    Container::iterator it = findImage(lpName);
    if (it == images_.end())
        return FALSE;
    delete *it;
    images_.erase(it);
    return TRUE;
}

Bool32 CTIImageList::SetImageWriteMask(const char *lpName, PCTIMask pWMask) {
    Container::iterator it = findImage(lpName);
    if (it == images_.end())
        return FALSE;
    (*it)->SetWriteMask(pWMask);
    return TRUE;
}

Bool32 CTIImageList::SetImageReadMask(const char *lpName, PCTIMask pAMask) {
    Container::iterator it = findImage(lpName);
    if (it == images_.end())
        return FALSE;
    (*it)->SetReadMask(pAMask);
    return TRUE;
}

Bool32 CTIImageList::GetImageWriteMask(const char *lpName, PPCTIMask ppWMask, PBool32 pEnMask) {
    Container::iterator it = findImage(lpName);
    if (it == images_.end())
        return FALSE;

    *ppWMask = (*it)->GetWriteMask();
    *pEnMask = (*it)->IsMaskEnabled("w");

    return TRUE;
}

Bool32 CTIImageList::GetImageReadMask(const char *lpName, PPCTIMask ppMask, PBool32 pEnMask) {
    Container::iterator it = findImage(lpName);
    if (it == images_.end())
        return FALSE;

    *ppMask = (*it)->GetReadMask();
    *pEnMask = (*it)->IsMaskEnabled("r");
    return TRUE;
}

Bool32 CTIImageList::EnableMask(const char *pName, const char* pType, Bool32 mEnabled) {
    Container::iterator it = findImage(pName);
    if (it == images_.end())
        return FALSE;

    return (*it)->EnableMask(pType, mEnabled);
}

Bool32 CTIImageList::FindHandle(Handle hImage) {
    if (!hImage)
        return FALSE;

    for (Container::iterator it = images_.begin(), end = images_.end(); it != end; ++it) {
        if ((*it)->GetImageHandle() == hImage)
            return TRUE;
    }

    return FALSE;
}
