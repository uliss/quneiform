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
#include <iostream>

#include "ctimasklinesegment.h"

namespace cf
{

CTIMaskLineSegment::CTIMaskLineSegment()
    : next_(NULL),
      start_(-1),
      end_(-1)
{
}

CTIMaskLineSegment::~CTIMaskLineSegment()
{
}

CTIMaskLineSegment::CTIMaskLineSegment(int Start, int End)
    : next_(NULL),
      start_(-1),
      end_(-1)
{
    if ( Start >= 0 && End >= 0 && Start <= End) {
        start_ = Start;
        end_   = End;
    }
}

CTIMaskLineSegment::CTIMaskLineSegment(const CTIMaskLineSegment& segm)
    : next_(segm.next()),
      start_(segm.start()),
      end_(segm.end())
{
}

CTIMaskLineSegment::intersection_t CTIMaskLineSegment::isIntersectWith(const CTIMaskLineSegment& segm) const
{
    int S = segm.start();
    int E = segm.end();
    point_dir_t iDS = pointDirection(S);
    point_dir_t iDE = pointDirection(E);

    if (isEqual(segm))
        return INTERSECTION_EQUAL;
    else {
        if (iDS == POINT_LEFT && iDE == POINT_RIGHT)
            return INTERSECTION_OVER;
        else {
            if (isPointInSegment(S)) {
                if (isPointInSegment(E))
                    return INTERSECTION_IN;
                else
                   return INTERSECTION_RIGHT;
            }
            else {
                if (isPointInSegment(E))
                    return INTERSECTION_LEFT;
                else if (iDS == POINT_LEFT && iDE == POINT_LEFT)
                    return INTERSECTION_FULL_LEFT;
                else
                    return INTERSECTION_FULL_RIGHT;
            }
        }
    }
}

bool CTIMaskLineSegment::intersectWith(const CTIMaskLineSegment& segm)
{
    switch(isIntersectWith(segm)) {
    case INTERSECTION_LEFT :
        end_  = segm.end();
        return true;
    case INTERSECTION_RIGHT :
        start_ = segm.start();
        return true;
    case INTERSECTION_IN :
        end_   = segm.end();
        start_ = segm.start();
        return true;
    default:
        return false;
    }
}

bool CTIMaskLineSegment::addWith(const CTIMaskLineSegment& segm)
{
    switch (isIntersectWith(segm)) {
    case INTERSECTION_LEFT :
        start_ = segm.start();
        return true;
    case INTERSECTION_RIGHT :
        end_  = segm.end();
        return true;
    case INTERSECTION_IN :
        return true;
    case INTERSECTION_OVER:
        start_ = segm.start();
        end_  = segm.end();
        return true;
    default:
        return false;
    }
}

bool CTIMaskLineSegment::cutLeftTo(const CTIMaskLineSegment& segm)
{
    switch (isIntersectWith(segm)) {
    case INTERSECTION_RIGHT :
    case INTERSECTION_IN :
        end_  = segm.start();
        return true;
    default:
        return false;
    }
}

bool CTIMaskLineSegment::cutRightTo(const CTIMaskLineSegment& segm)
{
    switch (isIntersectWith(segm)) {
    case INTERSECTION_LEFT :
    case INTERSECTION_IN :
        start_ = segm.end();
        return true;
    default:
        return false;
    }
}

CTIMaskLineSegment::point_dir_t CTIMaskLineSegment::pointDirection(int X) const
{
    if(!isPointInSegment(X)) {
        if(X < start())
            return POINT_LEFT;
        else
            return POINT_RIGHT;
    }

    return POINT_IN;
}

std::ostream& operator<<(std::ostream& os, const CTIMaskLineSegment& s)
{
    os << "Segment[" <<  s.start() << ',' << s.end() << ']';
    return os;
}

}
