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

#include "cpage.h"
#include "polyblock.h"
#include "internal.h"
#include "resource.h"
#include "picture.h"
#include "cpage_debug.h"

// Конверторы преобразования из TYPE_PICTURE в CPAGE_PICTURE
uint32_t TYPE_PICTURE_to_CPAGE_PICTURE(POLY_ * lpDataIn, uint32_t SizeIn, cf::cpage::Picture * LpDataOut, uint32_t SizeOut)
{
    if (LpDataOut == NULL)
        return sizeof(cf::cpage::Picture);

    if (sizeof(POLY_) != SizeIn ||
            sizeof(cf::cpage::Picture) != SizeOut ||
            lpDataIn == NULL) {
        SetReturnCode_cpage(IDS_ERR_DISCREP);
        return 0;
    }

    POLY_  desc = *lpDataIn;
    cf::cpage::Picture pict;

    for (uint32_t i = 0; i < desc.com.count; i++) {
        pict.appendCorner(desc.com.Vertex[i]);
    }

    *LpDataOut = pict;
    return sizeof(cf::cpage::Picture);
}

uint32_t CPAGE_PICTURE_to_TYPE_PICTURE(const cf::cpage::Picture& lpDataIn, uint32_t SizeIn, POLY_ * LpDataOut, uint32_t SizeOut)
{
    uint32_t rc = 0;

    if (LpDataOut == NULL)
        return sizeof(POLY_);

    if (sizeof(POLY_) != SizeOut || sizeof(cf::cpage::Picture) != SizeIn) {
        SetReturnCode_cpage(IDS_ERR_DISCREP);
        return 0;
    }

    POLY_  desc;
    cf::cpage::Picture pict = lpDataIn;
    desc.com.count = pict.cornerCount();

    for (size_t i = 0; i < pict.cornerCount(); i++) {
        desc.com.Vertex[i] = pict.cornerAt(i);
    }

    desc.com.type       = TYPE_PICTURE;
    desc.com.number = 0;
    *LpDataOut = desc;
    rc = sizeof(POLY_);
    return rc;
}


namespace cf
{
namespace cpage {

void Picture::appendCorner(const Point& pt)
{
    corners_[number_] = pt;
    number_++;
}

Point Picture::cornerAt(size_t pos) const
{
    assert(pos < CPAGE_MAXCORNER);

    if(pos >= number_) {
        CPAGE_DEBUG_FUNC << "invalid corner number:" << pos;
    }

    return corners_[pos];
}

size_t Picture::cornerCount() const
{
    return number_;
}

#define ROTATE_2048(p,a) {\
             p.ry() = (int32_t) (p.y() - (int32_t) p.x() * a / 2048);\
             p.rx() = (int32_t) (p.x() + (int32_t) p.y() * a / 2048);\
        }

void Picture::rotateCorner(size_t pos, int skew2048)
{
    assert(pos < CPAGE_MAXCORNER);

    ROTATE_2048(corners_[pos], skew2048);
}

}
}

