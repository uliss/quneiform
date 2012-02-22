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

#ifndef _CTIMASKLINESEGMENT_H_
#define _CTIMASKLINESEGMENT_H_

#include "cttypes.h"

namespace cf
{

class CTIMaskLineSegment
{
public:
    enum {
        CTIMLSEGMINTERSECTIN                = 1,
        CTIMLSEGMINTERSECTLEFT              = 2,
        CTIMLSEGMINTERSECTRIGHT             = 3,
        CTIMLSEGMINTERSECTOVER              = 4,
        CTIMLSEGMINTERSECTEQUAL             = 5,
        CTIMLSEGMINTERSECTFULLLEFT          = 6,
        CTIMLSEGMINTERSECTFULLRIGHT         = 7
    };

    enum point_dir_t {
        POINT_RIGHT                 = 1,
        POINT_LEFT                  = -1,
        POINT_IN                    =  0
    };

public:
    CTIMaskLineSegment * next() {
        return next_;
    }

    void setNext(CTIMaskLineSegment * pNext) {
        next_ = pNext;
    }

    int32_t start() const {
        return start_;
    }

    int32_t end() const {
        return end_;
    }

    bool IsPointInSegment(int32_t X) const {
        return (X >= start_ && X <= end_);
    }

    Bool32 CutRightTo(CTIMaskLineSegment * pSegm);
    // обрезать с конца
    Bool32 CutLeftTo(CTIMaskLineSegment * pSegm);
    // обрезать с начала
    Bool32 AddWith(CTIMaskLineSegment * pSegm);
    // пересечение данного сегмента с аргументом
    Bool32 IntersectWith(CTIMaskLineSegment * pSegm);
    // положение данного сегмента относительно аргумента
    uint32_t IsIntersectWith(CTIMaskLineSegment * pSegm);
    // равенство сегментов
    bool isEqual(CTIMaskLineSegment * pSegm) const {
        return (end_ == pSegm->end() && start_ == pSegm->start());
    }
public:
    CTIMaskLineSegment();
    CTIMaskLineSegment(int32_t Start, int32_t End);
    CTIMaskLineSegment(CTIMaskLineSegment * pSegm);
    ~CTIMaskLineSegment();
private:
    point_dir_t pointDirection(uint32_t X);
private:
    CTIMaskLineSegment * next_;
    int32_t start_;
    int32_t end_;
};

typedef CTIMaskLineSegment * PCTIMaskLineSegment;

}

#endif // !defined(_CTIMASKLINESEGMENT_H_)
