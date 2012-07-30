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
#include "ctiimageheader.h"
#include "ctimask.h"
#include "ctiimagelist.h"
#include "cimage_debug.h"

#define IMAGE_NOT_FOUND(name) \
    CIMAGE_ERROR_FUNC << "image not found:" << name;

namespace cf
{

CTIImageList::CTIImageList()
{}

CTIImageList::~CTIImageList()
{
    clear();
}

bool CTIImageList::addImage(const std::string& name, BitmapPtr handle, bool externalImage)
{
    if (name.empty()) {
        CIMAGE_ERROR_FUNC << "invalid image name:" << name;
        return false;
    }

    if (handle == NULL) {
        CIMAGE_ERROR_FUNC << "invalid image handle:" << name;
        return false;
    }

    if(image(name))
        removeImage(name);

    CTIImageHeader * new_image = new CTIImageHeader(handle, externalImage);
    headers_[name] = new_image;

    CIMAGE_DEBUG_FUNC << "image added:" << name;

    return true;
}

void CTIImageList::clear()
{
    CIMAGE_DEBUG_FUNC;

    for(HeaderMap::iterator it = headers_.begin(); it != headers_.end(); ++it)
        delete it->second;

    headers_.clear();
}

bool CTIImageList::imageHandle(const std::string& name, BitmapPtr * handle)
{
    CTIImageHeader * im = image(name);

    if (!im) {
        IMAGE_NOT_FOUND(name);
        return false;
    }

    if(handle)
        *handle = im->imageHandle();

    return true;
}

bool CTIImageList::hasImage(const std::string& name) const
{
    return headers_.find(name) != headers_.end();
}

bool CTIImageList::removeImage(const std::string &name)
{
    CTIImageHeader * header = image(name);

    if(!header)
        return false;

    delete header;
    headers_.erase(name);

    CIMAGE_DEBUG_FUNC << "image deleted:" << name;

    return true;
}

CTIImageHeader * CTIImageList::image(const std::string& name)
{
    HeaderMap::iterator it = headers_.find(name);

    if(it == headers_.end())
        return NULL;
    else
        return it->second;
}

std::list<std::string> CTIImageList::imageNames() const
{
    std::list<std::string> r;

    for(HeaderMap::const_iterator it = headers_.begin(); it != headers_.end(); ++it)
        r.push_back((*it).first);

    return r;
}

bool CTIImageList::setImageWriteMask(const std::string& name, CTIMask * mask)
{
    CTIImageHeader * im = image(name);

    if (!im) {
        IMAGE_NOT_FOUND(name);
        return false;
    }

    im->setWriteMask(mask);

    return true;
}

bool CTIImageList::setImageReadMask(const std::string& name, CTIMask * mask)
{
    CTIImageHeader * im = image(name);

    if (im == NULL) {
        IMAGE_NOT_FOUND(name);
        return false;
    }

    im->setReadMask(mask);

    return true;
}

bool CTIImageList::imageWriteMask(const std::string& name, CTIMask **ppWMask, bool * is_enabled)
{
    CTIImageHeader * im = image(name);

    if (im == NULL) {
        IMAGE_NOT_FOUND(name);
        return false;
    }

    *ppWMask = im->writeMask();

    if(is_enabled)
        *is_enabled = im->isWriteMaskEnabled();

    return true;
}

bool CTIImageList::imageReadMask(const std::string& name, CTIMask ** ppMask, bool * is_enabled)
{
    CTIImageHeader * im = image(name);

    if (im == NULL) {
        IMAGE_NOT_FOUND(name);
        return false;
    }

    *ppMask = im->readMask();

    if(is_enabled)
        *is_enabled = im->isReadMaskEnabled();

    return true;
}

bool CTIImageList::hasHandle(BitmapPtr handle)
{
    if(!handle)
        return false;

    for(HeaderMap::iterator it = headers_.begin(); it != headers_.end(); ++it) {
        if(it->second->imageHandle() == handle)
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
    CTIImageHeader * im = image(imageName);

    if (!im) {
        IMAGE_NOT_FOUND(imageName);
        return false;
    }

    if(ptr)
        (im->*ptr)();

    return true;
}

}
