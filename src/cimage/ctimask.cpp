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

#include <cstddef>

#include "resource.h"
#include "ctimask.h"
#include "ctimemory.h"

namespace cf
{

CTIMask::CTIMask() :
    line_(0),
    width_(0),
    height_(0),
    mwSegments(0)
{
}

CTIMask::CTIMask(int width, int Height) :
    line_(0),
    width_(width),
    height_(Height),
    mwSegments(0)
{
}

CTIMask::~CTIMask()
{
    PCTIMaskLine pPL = line_.GetNext();
    PCTIMaskLine pL = line_.GetNext();

    while (pL) {
        pL = pL->GetNext();
        delete pPL;
        pPL = pL;
    }
}

bool CTIMask::addRectangle(const Rect& r)
{
    uint32_t wXb;
    uint32_t wXe;
    uint32_t wYb;
    uint32_t wYe;
    PCTIMaskLine pPL = &line_;
    PCTIMaskLine pL;
    uint32_t wLine;

    if (!isRectOnMask(r))
        return false;

    wXb = r.x();
    wYb = r.y();
    wXe = wXb + r.width();
    wYe = wYb + r.height();
    CTIMaskLineSegment Segm(wXb, wXe);

    if (!SetPtrToPrevLine(wYb, &pPL)) {
        SetReturnCode_cimage(IDS_CIMAGE_UNABLE_ADD_MASK);
        return FALSE;
    }

    pL = pPL->GetNext();

    for (wLine = wYb; wLine < wYe; wLine++) {
        if (!pL)
            pPL->SetNext(pL = new CTIMaskLine(width_, wLine, &Segm));

        else {
            if (pL->lineNumber() == wLine) {
                // кладем новый сегмент в линию
                if (!pL->addSegment(&Segm)) {
                    SetReturnCode_cimage(IDS_CIMAGE_UNABLE_ADD_MASK);
                    return false;
                }
            }

            else {
                // двставляем новую линию
                pPL->SetNext(pL
                             = new CTIMaskLine(width_, wLine, &Segm, pL));
            }
        }

        if (pL)
            pL = ((pPL = pL)->GetNext());
    }

    mwSegments++;
    return true;
}

bool CTIMask::removeRectangle(const Rect& r)
{
    uint32_t wXb;
    uint32_t wXe;
    uint32_t wYb;
    uint32_t wYe;
    PCTIMaskLine pPL = &line_;
    PCTIMaskLine pL;
    uint32_t wLine;

    if (!isRectOnMask(r))
        return FALSE;

    wXb = r.x();
    wYb = r.y();
    wXe = wXb + r.width();
    wYe = wYb + r.height();
    CTIMaskLineSegment Segm(wXb, wXe);

    if (!SetPtrToPrevLine(wYb, &pPL)) {
        SetReturnCode_cimage(IDS_CIMAGE_UNABLE_REMOVE_MASK);
        return false;
    }

    pL = pPL->GetNext();

    for (wLine = wYb; wLine < wYe; wLine++) {
        if (!pL) {
            SetReturnCode_cimage(IDS_CIMAGE_UNABLE_REMOVE_MASK);
            return false;
        }

        else if (!pL->removeSegment(&Segm)) {
            SetReturnCode_cimage(IDS_CIMAGE_UNABLE_REMOVE_MASK);
            return false;
        }

        if (pL->segmentsNumber() == 0) {
            pPL->SetNext(pL->GetNext());
            delete pL;
            pL = pPL->GetNext();
        }

        if (pL)
            pL = ((pPL = pL)->GetNext());
    }

    mwSegments--;
    return true;
}

bool CTIMask::isRectOnMask(const Rect& r) const
{
    return r.left() < width_ ||
            r.right() < width_ ||
            r.top() < height_ ||
            r.bottom() < height_;
}

Bool32 CTIMask::SetPtrToPrevLine(uint32_t wLine, PPCTIMaskLine ppLine)
{
    if (!ppLine)
        return FALSE;

    while ((*ppLine)->GetNext()) {
        if (((*ppLine)->GetNext())->lineNumber() >= wLine)
            break;

        (*ppLine) = ((*ppLine)->GetNext());
    }

    return TRUE;
}

Bool32 CTIMask::GetLine(int wLine, PPCTIMaskLine ppcLine)
{
    PCTIMaskLine pL = line_.GetNext();
    Bool32 bLinePresent = FALSE;
    int32_t iLine;
    *ppcLine = NULL;

    if (wLine > height_) {
        return FALSE;
    }

    while (pL) {
        iLine = pL->lineNumber();

        if (iLine < (int32_t) wLine) {
            pL = pL->GetNext();
            continue;
        }

        if (iLine == (int32_t) wLine) {
            *ppcLine = pL;
            bLinePresent = TRUE;
            break;
        }

        if (iLine > (int32_t) wLine)
            break;
    }

    return bLinePresent;
}

}
