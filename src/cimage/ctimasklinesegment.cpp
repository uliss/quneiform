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

// CTIMaskLineSegment.cpp: implementation of the CTIMaskLineSegment class.

#include <cassert>

#include "ctimasklinesegment.h"

CTIMaskLineSegment::CTIMaskLineSegment() :
    next_(NULL), start_(-1), end_(-1) {
}

CTIMaskLineSegment::CTIMaskLineSegment(int Start, int End) :
    next_(NULL), start_(-1), end_(-1) {
    if (Start >= 0 && End >= 0 && Start <= End) {
        start_ = Start;
        end_ = End;
    }
}

CTIMaskLineSegment::CTIMaskLineSegment(const CTIMaskLineSegment& Segm) :
    next_(Segm.next_), start_(Segm.start_), end_(Segm.end_) {
}

CTIMaskLineSegment::~CTIMaskLineSegment() {
}

CTIMaskLineSegment::Intersection CTIMaskLineSegment::isIntersectWith(const CTIMaskLineSegment& Segm) const {
    int S = Segm.start();
    int E = Segm.end();
    PointPosition iDS = pointPosition(S);
    PointPosition iDE = pointPosition(E);

    if (*this == Segm)
        return INTERSECT_EQUAL;

    if (iDS == POINT_LEFT && iDE == POINT_RIGHT)
        return INTERSECT_OVER;

    if (isPointInSegment(S)) {
        return isPointInSegment(E) ? INTERSECT_IN : INTERSECT_RIGHT;
    }
    else {
        if (isPointInSegment(E))
            return INTERSECT_LEFT;
        else if (iDS == POINT_LEFT && iDE == POINT_LEFT)
            return INTERSECT_FULL_LEFT;
        else
            return INTERSECT_FULL_RIGHT;
    }
}

bool CTIMaskLineSegment::intersectWith(const CTIMaskLineSegment& Segm) {
    switch (isIntersectWith(Segm)) {
    case INTERSECT_LEFT:
        end_ = Segm.end();
        return true;
        break;
    case INTERSECT_RIGHT:
        start_ = Segm.start();
        return true;
        break;
    case INTERSECT_IN:
        end_ = Segm.end();
        start_ = Segm.start();
        return true;
        break;
    default:
        return false;
    }
}

bool CTIMaskLineSegment::addWith(const CTIMaskLineSegment& Segm) {
    switch (isIntersectWith(Segm)) {
    case INTERSECT_LEFT:
        start_ = Segm.start();
        return true;
        break;
    case INTERSECT_RIGHT:
        end_ = Segm.end();
        return true;
        break;
    case INTERSECT_IN:
        return true;
        break;
    case INTERSECT_OVER:
        start_ = Segm.start();
        end_ = Segm.end();
        return true;
        break;
    default:
        return false;
    }
}

bool CTIMaskLineSegment::cutLeftTo(const CTIMaskLineSegment& Segm) {
    switch (isIntersectWith(Segm)) {
    case INTERSECT_RIGHT:
    case INTERSECT_IN:
        end_ = Segm.start();
        return true;
        break;
    default:
        return false;
    }
}

bool CTIMaskLineSegment::cutRightTo(const CTIMaskLineSegment& Segm) {
    switch (isIntersectWith(Segm)) {
    case INTERSECT_LEFT:
    case INTERSECT_IN:
        start_ = Segm.end();
        return true;
        break;
    default:
        return false;
    }
}

CTIMaskLineSegment::PointPosition CTIMaskLineSegment::pointPosition(unsigned int X) const {
    if (!isPointInSegment(X))
        return ((int) X < start()) ? POINT_LEFT : POINT_RIGHT;

    return POINT_IN;
}
