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

#include "resource.h"
#include "ctimaskline.h"
#include "ctimemory.h"

namespace cf
{

CTIMaskLine::CTIMaskLine() :
        length_(0), segments_(0), line_(-1), next_(NULL)
{
}

CTIMaskLine::CTIMaskLine(uint Lenght) :
        length_(Lenght), segments_(0), line_(-1), next_(NULL)
{
}

CTIMaskLine::CTIMaskLine(uint Lenght, CTIMaskLineSegment *pSegm) :
        length_(Lenght),
        segments_(0), line_(-1), next_(NULL)
{
    addSegment(pSegm);
}

CTIMaskLine::CTIMaskLine(uint Lenght, uint nLine,
                         CTIMaskLineSegment * pSegm) :
        length_(Lenght),
        segments_(0), line_(nLine), next_(NULL)
{
    addSegment(pSegm);
}

CTIMaskLine::CTIMaskLine(uint Lenght, uint nLine,
                         CTIMaskLineSegment * pSegm, CTIMaskLine * pcNextLine) :
        length_(Lenght),
        segments_(0), line_(nLine), next_(pcNextLine)
{
    addSegment(pSegm);
}

CTIMaskLine::~CTIMaskLine()
{
    PCTIMaskLineSegment pS = first_.next();
    PCTIMaskLineSegment pD;

    while (pS) {
        pD = pS;
        pS = pS->next();
        delete pD;
    }
}

Bool32 CTIMaskLine::addSegment(CTIMaskLineSegment *pSegm)
{
    PCTIMaskLineSegment pS = &first_;
    PCTIMaskLineSegment pL;
    Bool32 bRet = FALSE;
    Bool32 Added = FALSE;
    Bool32 Check = FALSE;

    if (isSegmentOnLine(pSegm)) {
        pL = pS;

        while (pS && !Added) {
            switch (pS->isIntersectWith(pSegm)) {
                case CTIMaskLineSegment::INTERSECTION_FULL_LEFT:
                    pS = NULL;
                    break;
                case CTIMaskLineSegment::INTERSECTION_FULL_RIGHT:
                    pL = pS;
                    pS = pS->next();
                    break;
                case CTIMaskLineSegment::INTERSECTION_IN:
                case CTIMaskLineSegment::INTERSECTION_EQUAL:
                    bRet = Added = TRUE;
                    break;
                case CTIMaskLineSegment::INTERSECTION_OVER:
                    Check = TRUE;
                case CTIMaskLineSegment::INTERSECTION_LEFT:
                case CTIMaskLineSegment::INTERSECTION_RIGHT:
                    bRet = pS->addWith(pSegm);
                    Added = TRUE;
                    break;
                default:
                    return FALSE;
            }
        }

        if (!Added) {
            pS = pL->next();
            pL->setNext(new CTIMaskLineSegment(pSegm->start(),
                                               pSegm->end()));
            (pL->next())->setNext(pS);
            segments_++;
            bRet = TRUE;
        }

        if (Check)
            CheckSegments();
    }

    else
        SetReturnCode_cimage(IDS_CIMAGE_INVALID_MASK_LAGER_IMAGE);

    return bRet;
}

Bool32 CTIMaskLine::removeSegment(CTIMaskLineSegment *pSegm)
{
    PCTIMaskLineSegment pPS = &first_;
    PCTIMaskLineSegment pS = pPS->next();
    Bool32 Remed = FALSE;

    if (segments_) {
        if (isSegmentOnLine(pSegm)) {
            while (pS && !Remed) {
                switch (pS->isIntersectWith(pSegm)) {
                        // pSegm равен pS
                    case CTIMaskLineSegment::INTERSECTION_EQUAL:
                        pPS->setNext(pS->next());
                        delete pS;
                        Remed = TRUE;
                        segments_--;
                        pS = pPS->next();
                        break;
                        // pSegm перекывает pS
                    case CTIMaskLineSegment::INTERSECTION_OVER:
                        pPS->setNext(pS->next());
                        delete pS;
                        segments_--;
                        pS = pPS->next();
                        break;
                        // pSegm перекывает правую часть pS
                    case CTIMaskLineSegment::INTERSECTION_RIGHT:
                        pS->cutLeftTo(pSegm);
                        pPS = pPS->next();
                        pS = pPS->next();
                        break;
                        // pSegm перекывает левую часть pS
                    case CTIMaskLineSegment::INTERSECTION_LEFT:
                        pS->cutRightTo(pSegm);
                        pPS = pPS->next();
                        pS = pPS->next();
                        break;
                        // pSegm лежит внутри pS
                    case CTIMaskLineSegment::INTERSECTION_IN:
                        pS->setNext(new CTIMaskLineSegment(pS));
                        pS->cutLeftTo(pSegm);
                        pS = (pPS = pS)->next();
                        pS->cutRightTo(pSegm);
                        segments_++;
                        pPS = pPS->next();
                        pS = pPS->next();
                        break;
                    case CTIMaskLineSegment::INTERSECTION_FULL_LEFT:
                        pS = NULL;
                        break;
                    case CTIMaskLineSegment::INTERSECTION_FULL_RIGHT:
                        pPS = pS;
                        pS = pS->next();
                        break;
                }
            }
        }

        else
            SetReturnCode_cimage(IDS_CIMAGE_INVALID_MASK_LAGER_IMAGE);
    }

    else
        SetReturnCode_cimage(IDS_CIMAGE_NO_MASK_ON_THIS_LINE);

    return TRUE;
}

Bool32 CTIMaskLine::GetLeftIntersection(PCTIMaskLineSegment pcSegm)
{
    PCTIMaskLineSegment pL = first_.next();
    uint32_t wItype;
    Bool32 bInt = FALSE;

    while (pL) {
        //wItype = pL->IsIntersectWith( pcSegm );
        wItype = pcSegm->isIntersectWith(pL);

        if (!(wItype == CTIMaskLineSegment::INTERSECTION_FULL_RIGHT || wItype
                == CTIMaskLineSegment::INTERSECTION_FULL_LEFT)) {
            pcSegm->intersectWith(pL);
            bInt = TRUE;
            break;
        }

        pL = pL->next();
    }

    return bInt;
}

Bool32 CTIMaskLine::CheckSegments()
{
    return TRUE;
}

}

