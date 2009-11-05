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

// CTIMaskLine.cpp: implementation of the CTIMaskLine class.
//

#include "ctimaskline.h"
#include "ctimemory.h"
#include "resource.h"

CTIMaskLine::CTIMaskLine() :
    length_(0), segments_(0), next_(NULL), line_(-1) {
}

CTIMaskLine::CTIMaskLine(uint32_t Lenght) :
    length_(Lenght), segments_(0), next_(NULL), line_(-1) {
}

CTIMaskLine::CTIMaskLine(uint32_t Lenght, CTIMaskLineSegment * pSegm) :
    length_(Lenght), segments_(0), next_(NULL), line_(-1) {
    AddSegment(pSegm);
}

CTIMaskLine::CTIMaskLine(uint32_t Lenght, uint32_t nLine, CTIMaskLineSegment * pSegm) :
    length_(Lenght), segments_(0), next_(NULL), line_(nLine) {
    AddSegment(pSegm);
}

CTIMaskLine::CTIMaskLine(uint32_t Lenght, uint32_t nLine, CTIMaskLineSegment * pSegm,
        CTIMaskLine * pcNextLine) :
    length_(Lenght), segments_(0), next_(pcNextLine), line_(nLine) {
    AddSegment(pSegm);
}

CTIMaskLine::~CTIMaskLine() {
    CTIMaskLineSegment * pS = first_.GetNext();
    CTIMaskLineSegment * pD;

    while (pS) {
        pD = pS;
        pS = pS->GetNext();
        delete pD;
    }
}

Bool32 CTIMaskLine::AddSegment(CTIMaskLineSegment * pSegm) {
    CTIMaskLineSegment * pS = &first_;
    CTIMaskLineSegment * pL;
    Bool32 bRet = FALSE;
    Bool32 Added = FALSE;
    Bool32 Check = FALSE;

    if (IsSegmentOnLine(*pSegm)) {
        pL = pS;

        while (pS && !Added) {
            switch (pS->IsIntersectWith(*pSegm)) {
            case CTIMaskLineSegment::CTIMLSEGMINTERSECTFULLLEFT:
                pS = NULL;
                break;
            case CTIMaskLineSegment::CTIMLSEGMINTERSECTFULLRIGHT:
                pL = pS;
                pS = pS->GetNext();
                break;
            case CTIMaskLineSegment::CTIMLSEGMINTERSECTIN:
            case CTIMaskLineSegment::CTIMLSEGMINTERSECTEQUAL:
                bRet = Added = TRUE;
                break;
            case CTIMaskLineSegment::CTIMLSEGMINTERSECTOVER:
                Check = TRUE;
            case CTIMaskLineSegment::CTIMLSEGMINTERSECTLEFT:
            case CTIMaskLineSegment::CTIMLSEGMINTERSECTRIGHT:
                bRet = pS->AddWith(*pSegm);
                Added = TRUE;
                break;
            default:
                return FALSE;
            }
        }

        if (!Added) {
            pS = pL->GetNext();
            pL->SetNext(new CTIMaskLineSegment(pSegm->GetStart(), pSegm->GetEnd()));
            (pL->GetNext())->SetNext(pS);
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

Bool32 CTIMaskLine::RemoveSegment(CTIMaskLineSegment * pSegm) {
    CTIMaskLineSegment * pPS = &first_;
    CTIMaskLineSegment * pS = pPS->GetNext();
    Bool32 Remed = FALSE;

    if (segments_) {
        if (IsSegmentOnLine(*pSegm)) {
            while (pS && !Remed) {
                switch (pS->IsIntersectWith(*pSegm)) {
                // pSegm равен pS
                case CTIMaskLineSegment::CTIMLSEGMINTERSECTEQUAL:
                    pPS->SetNext(pS->GetNext());
                    delete pS;
                    Remed = TRUE;
                    segments_--;
                    pS = pPS->GetNext();
                    break;
                    // pSegm перекывает pS
                case CTIMaskLineSegment::CTIMLSEGMINTERSECTOVER:
                    pPS->SetNext(pS->GetNext());
                    delete pS;
                    segments_--;
                    pS = pPS->GetNext();
                    break;
                    // pSegm перекывает правую часть pS
                case CTIMaskLineSegment::CTIMLSEGMINTERSECTRIGHT:
                    pS->CutLeftTo(*pSegm);
                    pPS = pPS->GetNext();
                    pS = pPS->GetNext();
                    break;
                    // pSegm перекывает левую часть pS
                case CTIMaskLineSegment::CTIMLSEGMINTERSECTLEFT:
                    pS->CutRightTo(*pSegm);
                    pPS = pPS->GetNext();
                    pS = pPS->GetNext();
                    break;
                    // pSegm лежит внутри pS
                case CTIMaskLineSegment::CTIMLSEGMINTERSECTIN:
                    pS->SetNext(new CTIMaskLineSegment(*pS));
                    pS->CutLeftTo(*pSegm);
                    pS = (pPS = pS)->GetNext();
                    pS->CutRightTo(*pSegm);
                    segments_++;
                    pPS = pPS->GetNext();
                    pS = pPS->GetNext();
                    break;
                case CTIMaskLineSegment::CTIMLSEGMINTERSECTFULLLEFT:
                    pS = NULL;
                    break;
                case CTIMaskLineSegment::CTIMLSEGMINTERSECTFULLRIGHT:
                    pPS = pS;
                    pS = pS->GetNext();
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

Bool32 CTIMaskLine::GetLeftIntersection(CTIMaskLineSegment * pcSegm) {
    CTIMaskLineSegment * pL = first_.GetNext();
    uint32_t wItype;
    Bool32 bInt = FALSE;

    while (pL) {
        wItype = pcSegm->IsIntersectWith(*pL);

        if (!(wItype == CTIMaskLineSegment::CTIMLSEGMINTERSECTFULLRIGHT || wItype
                == CTIMaskLineSegment::CTIMLSEGMINTERSECTFULLLEFT)) {
            pcSegm->IntersectWith(*pL);
            bInt = TRUE;
            break;
        }

        pL = pL->GetNext();
    }

    return bInt;
}

bool CTIMaskLine::CheckSegments() const {
    return true;
}

