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

//////////////////////////////////////////////////////
// Rotation routines:
//
// inline Point& Deskew( Point& pt, int32_t skew1024);
// inline Point16& Deskew( Point16& pt, int32_t skew1024);
//
// NOTE: POSITIVE SKEW == CLOCKWISE
//////////////////////////////////////////////////////

#ifndef __SKEW1024_H
#define __SKEW1024_H

#include <math.h>

#include "cttypes.h"
#include "point.h"
#include "rect.h"

// movements ( -=, += )
inline CIF::Rect& operator -=(CIF::Rect& rc, CIF::Point& pt) {
    rc.rleft() -= pt.x();
    rc.rright() -= pt.x();
    rc.rtop() -= pt.y();
    rc.rbottom() -= pt.y();
    return rc;
}

inline CIF::Rect16& operator -=(CIF::Rect16& rc, CIF::Point16& pt) {
    rc.rleft() -= pt.x();
    rc.rright() -= pt.x();
    rc.rtop() -= pt.y();
    rc.rbottom() -= pt.y();
    return rc;
}

inline CIF::Rect& operator+=(CIF::Rect& rc, CIF::Point& pt) {
    rc.rleft() += pt.x();
    rc.rright() += pt.x();
    rc.rtop() += pt.y();
    rc.rbottom() += pt.y();
    return rc;
}

inline CIF::Rect16& operator+=(CIF::Rect16& rc, CIF::Point16& pt) {
    rc.rleft() += pt.x();
    rc.rright() += pt.x();
    rc.rtop() += pt.y();
    rc.rbottom() += pt.y();
    return rc;
}

inline CIF::Rect& DeskewCenter(CIF::Rect& rc, int32_t skew1024) {
    CIF::Point center = rc.center();
    CIF::Point old_center = center;
    center.deskew(skew1024);
    center -= old_center; // diff
    rc += center;
    return rc;
}

inline CIF::Rect16& DeskewCenter(CIF::Rect16& rc, int32_t skew1024) {
    CIF::Point16 center = rc.center();
    CIF::Point16 old_center = center;
    center.deskew(skew1024);
    center -= old_center; // diff
    rc += center;
    return rc;
}

inline CIF::Rect16& Expand(CIF::Rect16& rc, int16_t delta) {
    rc.rleft() -= delta;
    rc.rtop() -= delta;
    rc.rright() += delta;
    rc.rbottom() += delta;
    return rc;
}

inline CIF::Rect& Expand(CIF::Rect& rc, int32_t delta) {
    rc.rleft() -= delta;
    rc.rtop() -= delta;
    rc.rright() += delta;
    rc.rbottom() += delta;
    return rc;
}

// make points: lt & rb (main diag) or rt & lb (other)
inline void GetDiag1(CIF::Rect& rc, CIF::Point& lt, CIF::Point& rb) {
    lt.set(rc.left(), rc.top());
    rb.set(rc.right(), rc.bottom());
}
inline void GetDiag1(CIF::Rect16& rc, CIF::Point16& lt, CIF::Point16& rb) {
    lt.set(rc.left(), rc.top());
    rb.set(rc.right(), rc.bottom());
}
inline void GetDiag2(CIF::Rect& rc, CIF::Point& rt, CIF::Point& lb) {
    rt.set(rc.right(), rc.top());
    lb.set(rc.left(), rc.bottom());
}
inline void GetDiag2(CIF::Rect16& rc, CIF::Point16& rt, CIF::Point16& lb) {
    rt.set(rc.right(), rc.top());
    lb.set(rc.left(), rc.bottom());
}

inline CIF::Rect& GetBounding(CIF::Rect& rc_src, CIF::Rect& rc_dest, int32_t skew1024)
// dest rect can be the same as source
{
    CIF::Point lt, rb;
    GetDiag1(rc_src, lt, rb);
    lt.deskew(skew1024);
    rb.deskew(skew1024);
    CIF::Point rt, lb;
    GetDiag2(rc_src, rt, lb);
    rt.deskew(skew1024);
    lb.deskew(skew1024);
    if (skew1024 < 0) {
        rc_dest.rleft() = lt.x();
        rc_dest.rright() = rb.x();
        rc_dest.rtop() = rt.y();
        rc_dest.rbottom() = lb.y();
    }
    else {
        rc_dest.rleft() = lb.x();
        rc_dest.rright() = rt.x();
        rc_dest.rtop() = lt.y();
        rc_dest.rbottom() = rb.y();
    }
    return rc_dest;
}

inline CIF::Rect16& GetBounding(CIF::Rect16& rc_src, CIF::Rect16& rc_dest, int32_t skew1024)
// dest rect can be the same as source
{
    CIF::Point16 lt, rb;
    GetDiag1(rc_src, lt, rb);
    lt.deskew(skew1024);
    rb.deskew(skew1024);
    CIF::Point16 rt, lb;
    GetDiag2(rc_src, rt, lb);
    rt.deskew(skew1024);
    lb.deskew(skew1024);
    if (skew1024 < 0) {
        rc_dest.rleft() = lt.x();
        rc_dest.rright() = rb.x();
        rc_dest.rtop() = rt.y();
        rc_dest.rbottom() = lb.y();
    }
    else {
        rc_dest.rleft() = lb.x();
        rc_dest.rright() = rt.x();
        rc_dest.rtop() = lt.y();
        rc_dest.rbottom() = rb.y();
    }
    return rc_dest;
}

// 22.03.97 17:48, V.P
inline Bool operator !(const CIF::Rect16 & r) {
    return (r.left() > r.right()) || (r.top() > r.bottom());
}
inline Bool operator !(const CIF::Rect & r) {
    return (r.left() > r.right()) || (r.top() > r.bottom());
}
inline Bool Not(const CIF::Rect16 & r) {
    return (r.left() > r.right()) || (r.top() > r.bottom());
}
inline Bool Not(const CIF::Rect & r) {
    return (r.left() > r.right()) || (r.top() > r.bottom());
}

#define SetMax( a, b ) { if ((a) < (b)) (a) = (b); }
#define SetMin( a, b ) { if ((a) > (b)) (a) = (b); }

inline CIF::Rect& operator |=(CIF::Rect& rc, const CIF::Rect& r) {
    SetMin( rc.rleft(), r.left() );
    SetMin( rc.rtop() , r.top() );
    SetMax( rc.rright(), r.right());
    SetMax( rc.rbottom(), r.bottom() );
    return rc;
}
inline CIF::Rect16& operator |=(CIF::Rect16& rc, const CIF::Rect16& r) {
    SetMin( rc.rleft(), r.left() );
    SetMin( rc.rtop() , r.top() );
    SetMax( rc.rright(), r.right());
    SetMax( rc.rbottom(), r.bottom() );
    return rc;
}

inline CIF::Rect& operator &=(CIF::Rect& rc, const CIF::Rect& r) {
    SetMax( rc.rleft(), r.left() );
    SetMax( rc.rtop(), r.top() );
    SetMin( rc.rright(), r.right());
    SetMin( rc.rbottom(), r.bottom() );
    return rc;
}
inline CIF::Rect16& operator &=(CIF::Rect16& rc, const CIF::Rect16& r) {
    SetMax( rc.rleft(), r.left() );
    SetMax( rc.rtop(), r.top() );
    SetMin( rc.rright(), r.right());
    SetMin( rc.rbottom(), r.bottom() );
    return rc;
}

inline CIF::Rect16& Set(CIF::Rect16& r, int left, int top, int right, int bottom) {
    r.rleft() = left;
    r.rtop() = top;
    r.rright() = right;
    r.rbottom() = bottom;
    return r;
}
inline CIF::Rect& Set(CIF::Rect& r, int left, int top, int right, int bottom) {
    r.rleft() = left;
    r.rtop() = top;
    r.rright() = right;
    r.rbottom() = bottom;
    return r;
}
inline CIF::Rect16& Check(CIF::Rect16& r) {
    if (r.left() > r.right()) {
        int x = r.left();
        r.rleft() = r.right();
        r.rright() = x;
    };
    if (r.top() > r.bottom()) {
        int x = r.top();
        r.rtop() = r.bottom();
        r.rbottom() = x;
    }
    return r;
}
inline CIF::Rect& Check(CIF::Rect& r) {
    if (r.left() > r.right()) {
        int32_t x = r.left();
        r.rleft() = r.right();
        r.rright() = x;
    };
    if (r.top() > r.bottom()) {
        int32_t x = r.top();
        r.rtop() = r.bottom();
        r.rbottom() = x;
    }
    return r;
}

inline int width(const CIF::Rect16& rc) {
    return rc.width() + 1;
}
inline int width(const CIF::Rect& rc) {
    return rc.width() + 1;
}

inline int height(const CIF::Rect16& rc) {
    return rc.height() + 1;
}
inline int height(const CIF::Rect& rc) {
    return rc.height() + 1;
}

inline Bool Overlap(const CIF::Rect16& r1, const CIF::Rect16& r2) {
    return !((r1.left() > r2.right()) || (r1.top() > r2.bottom()) || (r2.left() > r1.right())
            || (r2.top() > r1.bottom()));
}
inline Bool Overlap(const CIF::Rect& r1, const CIF::Rect& r2) {
    return !((r1.left() > r2.right()) || (r1.top() > r2.bottom()) || (r2.left() > r1.right())
            || (r2.top() > r1.bottom()));
}

inline Bool Inside(const CIF::Rect16& r, const CIF::Point16 & p) {
    return !((p.x() < r.left()) || (p.x() > r.right()) || (p.x() < r.top()) || (p.x() > r.bottom()));
}
inline Bool Inside(const CIF::Rect& r, const CIF::Point & p) {
    return !((p.x() < r.left()) || (p.x() > r.right()) || (p.x() < r.top()) || (p.x() > r.bottom()));
}

inline Bool Overlap(int a1, int a2, int b1, int b2) // two ranges intersection
{ // assumed that a1 <= a2; b1 <= b2
    return !((a2 < b1) || (b2 < a1));
}

template<class T, class U>
double PointDistance(const CIF::PointImpl<T>& p0, const CIF::PointImpl<U>& p1) {
    return ::sqrt((p0.x() - p1.x()) * (p0.x() - p1.x()) + (p0.y() - p1.y()) * (p0.y() - p1.y()));
}

#endif

