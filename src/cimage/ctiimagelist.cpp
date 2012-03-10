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
#include "common/cifconfig.h"
#include "common/debug.h"

#include <boost/current_function.hpp>

namespace cf
{

CTIImageList::CTIImageList()
{}

CTIImageList::~CTIImageList()
{
    for(HeaderMap::iterator it = headers_.begin(); it != headers_.end(); ++it)
        delete it->second;

    headers_.clear();
}

bool CTIImageList::addImage(const std::string& name, BitmapHandle hDIB, uint32_t wFlag)
{
    if(findImage(name))
        deleteImage(name);

    if (name.empty()) {
        Debug() << "CTIImageList::AddImage: invalid image name: " << name << "\n";
        return false;
    }

    if (hDIB == NULL) {
        Debug() << "CTIImageList::AddImage: invalid image handle: " << name << "\n";
        return false;
    }

    CTIImageHeader * NewImage = new CTIImageHeader(hDIB, wFlag);
    headers_[name] = NewImage;

    if (Config::instance().debug())
        Debug() << "CTIImageList::AddImage: image added: " << name << "\n";

    return true;
}

bool CTIImageList::getImage(const std::string &lpName, BitmapHandle * phDIB)
{
    CTIImageHeader * Image = findImage(lpName);

    if (Image == NULL) {
        Debug() << "CTIImageList::GetImage: image not found: " << lpName << "\n";
        return false;
    }

    *phDIB = Image->imageHandle();
    return true;
}

bool CTIImageList::deleteImage(const std::string &name)
{
    CTIImageHeader * header = findImage(name);

    if(!header)
        return false;

    delete header;
    headers_.erase(name);

    if (Config::instance().debug())
        Debug() << "CTIImageList::DeleteImage: image deleted: " << name << "\n";

    return true;
}

CTIImageHeader * CTIImageList::findImage(const std::string& name)
{
    HeaderMap::iterator it = headers_.find(name);

    if(it == headers_.end())
        return NULL;
    else
        return it->second;
}

Bool32 CTIImageList::SetImageWriteMask(const char *lpName, PCTIMask pWMask)
{
    Bool32 bRet;
    CTIImageHeader * Image = findImage(lpName);

    if (Image == NULL) {
        SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
        return FALSE;
    }

    bRet = Image->setWriteMask(pWMask);
    return TRUE;
}

Bool32 CTIImageList::SetImageReadMask(const char *lpName, PCTIMask pAMask)
{
    Bool32 bRet;
    CTIImageHeader * Image = findImage(lpName);

    if (Image == NULL) {
        SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
        return FALSE;
    }

    bRet = Image->setReadMask(pAMask);
    return TRUE;
}

Bool32 CTIImageList::GetImageWriteMask(const char *lpName, PPCTIMask ppWMask, PBool32 pEnMask)
{
    CTIImageHeader * Image = findImage(lpName);

    if (Image == NULL) {
        SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
        return FALSE;
    }

    *ppWMask = Image->writeMask();
    *pEnMask = Image->isWriteMaskEnabled();
    return TRUE;
}

Bool32 CTIImageList::GetImageReadMask(const char *lpName, PPCTIMask ppMask, PBool32 pEnMask)
{
    CTIImageHeader * Image = findImage(lpName);

    if (Image == NULL) {
        SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
        return FALSE;
    }

    *ppMask = Image->readMask();
    *pEnMask = Image->isReadMaskEnabled();
    return TRUE;
}

bool CTIImageList::findHandle(Handle hImage)
{
    if(!hImage)
        return false;

    for(HeaderMap::iterator it = headers_.begin(); it != headers_.end(); ++it) {
        if(it->second->imageHandle() == hImage)
            return true;
    }

    return false;
}

bool CTIImageList::disableReadMask(const std::string& imageName)
{
    return maskAction(imageName, &CTIImageHeader::disableReadMask);
}

bool CTIImageList::disableWriteMask(const std::string &imageName)
{
    return maskAction(imageName, &CTIImageHeader::disableWriteMask);
}

bool CTIImageList::enableReadMask(const std::string& imageName)
{
    return maskAction(imageName, &CTIImageHeader::enableReadMask);
}

bool CTIImageList::enableWriteMask(const std::string& imageName)
{
    return maskAction(imageName, &CTIImageHeader::enableWriteMask);
}

bool CTIImageList::maskAction(const std::string& imageName, CTIImageList::MemberPtr ptr)
{
    CTIImageHeader * image = findImage(imageName);

    if (!image) {
        cf::Debug() << BOOST_CURRENT_FUNCTION << " image not found: " << imageName << "\n";
        return false;
    }

    (image->*ptr)();

    return true;
}

}
