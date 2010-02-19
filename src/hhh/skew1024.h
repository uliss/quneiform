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
#include "common/point.h"

inline CIF::Point& GetCenter(const Rect32& rc, CIF::Point& center)
{
    center.rx() = (rc.left + rc.right) >> 1;
    center.ry() = (rc.top + rc.bottom) >> 1;
    return center;
}

inline CIF::Point16& GetCenter(const Rect16& rc, CIF::Point16& center)
{
    center.rx() = (rc.left + rc.right) >> 1;
    center.ry() = (rc.top + rc.bottom) >> 1;
    return center;
}

inline Rect32& Copy(Rect32& dst, Rect16& src)
{
    dst.left = src.left;
    dst.right = src.right;
    dst.top = src.top;
    dst.bottom = src.bottom;
    return dst;
}
inline Rect16& Copy(Rect16& dst, Rect32& src)
{
    dst.left = (int16_t) src.left;
    dst.right = (int16_t) src.right;
    dst.top = (int16_t) src.top;
    dst.bottom = (int16_t) src.bottom;
    return dst;
}

// diagonal points => rect
inline Rect32& Pt2Rc(Rect32& dst, CIF::Point16& a, CIF::Point16& b)
{
    dst.left = a.x();
    dst.right = b.x();
    dst.top = a.y();
    dst.bottom = b.y();
    return dst;
}
inline Rect16& Pt2Rc(Rect16& dst, CIF::Point16& a, CIF::Point16& b)
{
    dst.left = a.x();
    dst.right = b.x();
    dst.top = a.y();
    dst.bottom = b.y();
    return dst;
}
inline Rect32& Pt2Rc(Rect32& dst, CIF::Point& a, CIF::Point& b)
{
    dst.left = a.x();
    dst.right = b.x();
    dst.top = a.y();
    dst.bottom = b.y();
    return dst;
}
inline Rect16& Pt2Rc(Rect16& dst, CIF::Point& a, CIF::Point& b)
{
    dst.left = (int16_t) a.x();
    dst.right = (int16_t) b.x();
    dst.top = (int16_t) a.y();
    dst.bottom = (int16_t) b.y();
    return dst;
}

// movements ( -=, += )
inline Rect32& operator -=(Rect32 & rc, CIF::Point & pt)
{
    rc.left -= pt.x();
    rc.right -= pt.x();
    rc.top -= pt.y();
    rc.bottom -= pt.y();
    return rc;
}

inline Rect16& operator -=(Rect16 & rc, CIF::Point16 & pt)
{
    rc.left -= pt.x();
    rc.right -= pt.x();
    rc.top -= pt.y();
    rc.bottom -= pt.y();
    return rc;
}

inline Rect32& operator+=(Rect32 & rc, CIF::Point & pt)
{
    rc.left += pt.x();
    rc.right += pt.x();
    rc.top += pt.y();
    rc.bottom += pt.y();
    return rc;
}

inline Rect16& operator+=(Rect16 & rc, CIF::Point16 & pt)
{
    rc.left += pt.x();
    rc.right += pt.x();
    rc.top += pt.y();
    rc.bottom += pt.y();
    return rc;
}

inline Rect32& DeskewCenter(Rect32& rc, int32_t skew1024)
{
    CIF::Point center;
    GetCenter(rc, center);
    CIF::Point old_center = center;
    center.deskew(skew1024);
    center -= old_center; // diff
    rc += center;
    return rc;
}

inline Rect16& DeskewCenter(Rect16& rc, int32_t skew1024)
{
    CIF::Point16 center;
    GetCenter(rc, center);
    CIF::Point16 old_center = center;
    center.deskew(skew1024);
    center -= old_center; // diff
    rc += center;
    return rc;
}

inline Rect16& Expand(Rect16& rc, int16_t delta)
{
    rc.left -= delta;
    rc.top -= delta;
    rc.right += delta;
    rc.bottom += delta;
    return rc;
}

inline Rect32& Expand(Rect32& rc, int32_t delta)
{
    rc.left -= delta;
    rc.top -= delta;
    rc.right += delta;
    rc.bottom += delta;
    return rc;
}

// make points: lt & rb (main diag) or rt & lb (other)
inline void GetDiag1(Rect32& rc, CIF::Point& lt, CIF::Point& rb)
{
    lt.set(rc.left, rc.top);
    rb.set(rc.right, rc.bottom);
}
inline void GetDiag1(Rect16& rc, CIF::Point16& lt, CIF::Point16& rb)
{
    lt.set(rc.left, rc.top);
    rb.set(rc.right, rc.bottom);
}
inline void GetDiag2(Rect32& rc, CIF::Point& rt, CIF::Point& lb)
{
    rt.set(rc.right, rc.top);
    lb.set(rc.left, rc.bottom);
}
inline void GetDiag2(Rect16& rc, CIF::Point16& rt, CIF::Point16& lb)
{
    rt.set(rc.right, rc.top);
    lb.set(rc.left, rc.bottom);
}

inline Rect32& GetBounding(Rect32& rc_src, Rect32& rc_dest, int32_t skew1024)
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
        rc_dest.left = lt.x();
        rc_dest.right = rb.x();
        rc_dest.top = rt.y();
        rc_dest.bottom = lb.y();
    }

    else {
        rc_dest.left = lb.x();
        rc_dest.right = rt.x();
        rc_dest.top = lt.y();
        rc_dest.bottom = rb.y();
    }

    return rc_dest;
}

inline Rect16& GetBounding(Rect16& rc_src, Rect16& rc_dest, int32_t skew1024)
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
        rc_dest.left = lt.x();
        rc_dest.right = rb.x();
        rc_dest.top = rt.y();
        rc_dest.bottom = lb.y();
    }

    else {
        rc_dest.left = lb.x();
        rc_dest.right = rt.x();
        rc_dest.top = lt.y();
        rc_dest.bottom = rb.y();
    }

    return rc_dest;
}

// 22.03.97 17:48, V.P
inline Bool operator !(const Rect16 & r)
{
    return (r.left > r.right) || (r.top > r.bottom);
}
inline Bool operator !(const Rect32 & r)
{
    return (r.left > r.right) || (r.top > r.bottom);
}
inline Bool Not(const Rect16 & r)
{
    return (r.left > r.right) || (r.top > r.bottom);
}
inline Bool Not(const Rect32 & r)
{
    return (r.left > r.right) || (r.top > r.bottom);
}

#define SetMax( a, b ) { if ((a) < (b)) (a) = (b); }
#define SetMin( a, b ) { if ((a) > (b)) (a) = (b); }

inline Rect32& operator |=(Rect32 & rc, const Rect32 & r)
{
    SetMin( rc.left, r.left );
    SetMin( rc.top , r.top );
    SetMax( rc.right, r.right);
    SetMax( rc.bottom, r.bottom );
    return rc;
}
inline Rect16& operator |=(Rect16 & rc, const Rect16 & r)
{
    SetMin( rc.left, r.left );
    SetMin( rc.top , r.top );
    SetMax( rc.right, r.right);
    SetMax( rc.bottom, r.bottom );
    return rc;
}

inline Rect32& operator &=(Rect32 & rc, const Rect32 & r)
{
    SetMax( rc.left, r.left );
    SetMax( rc.top , r.top );
    SetMin( rc.right, r.right);
    SetMin( rc.bottom, r.bottom );
    return rc;
}
inline Rect16& operator &=(Rect16 & rc, const Rect16 & r)
{
    SetMax( rc.left, r.left );
    SetMax( rc.top , r.top );
    SetMin( rc.right, r.right);
    SetMin( rc.bottom, r.bottom );
    return rc;
}

inline Rect16& Set(Rect16& r, int left, int top, int right, int bottom)
{
    r.left = left;
    r.top = top;
    r.right = right;
    r.bottom = bottom;
    return r;
}
inline Rect32& Set(Rect32& r, int left, int top, int right, int bottom)
{
    r.left = left;
    r.top = top;
    r.right = right;
    r.bottom = bottom;
    return r;
}
inline Rect16& Check(Rect16& r)
{
    if (r.left > r.right) {
        int x = r.left;
        r.left = r.right;
        r.right = x;
    };

    if (r.top > r.bottom) {
        int x = r.top;
        r.top = r.bottom;
        r.bottom = x;
    }

    return r;
}
inline Rect32& Check(Rect32& r)
{
    if (r.left > r.right) {
        int32_t x = r.left;
        r.left = r.right;
        r.right = x;
    };

    if (r.top > r.bottom) {
        int32_t x = r.top;
        r.top = r.bottom;
        r.bottom = x;
    }

    return r;
}

inline int width(const Rect16& rc)
{
    return rc.right - rc.left + 1;
}
inline int width(const Rect32& rc)
{
    return rc.right - rc.left + 1;
}

inline int height(const Rect16& rc)
{
    return rc.bottom - rc.top + 1;
}
inline int height(const Rect32& rc)
{
    return rc.bottom - rc.top + 1;
}

inline Bool Overlap(const Rect16& r1, const Rect16& r2)
{
    return !((r1.left > r2.right) || (r1.top > r2.bottom) || (r2.left
                                                              > r1.right) || (r2.top > r1.bottom));
}
inline Bool Overlap(const Rect32& r1, const Rect32& r2)
{
    return !((r1.left > r2.right) || (r1.top > r2.bottom) || (r2.left
                                                              > r1.right) || (r2.top > r1.bottom));
}

inline Bool Inside(const Rect16& r, const CIF::Point16 & p)
{
    return !((p.x() < r.left) || (p.x() > r.right) || (p.x() < r.top) || (p.x()
                                                                          > r.bottom));
}
inline Bool Inside(const Rect32& r, const CIF::Point & p)
{
    return !((p.x() < r.left) || (p.x() > r.right) || (p.x() < r.top) || (p.x()
                                                                          > r.bottom));
}

inline Bool Overlap(int a1, int a2, int b1, int b2) // two ranges intersection
{ // assumed that a1 <= a2; b1 <= b2
    return !((a2 < b1) || (b2 < a1));
}

template<class T, class U>
double PointDistance(const CIF::PointImpl<T>& p0, const CIF::PointImpl<U>& p1)
{
    return ::sqrt(static_cast<float>((p0.x() - p1.x()) * (p0.x() - p1.x())
                  + (p0.y() - p1.y()) * (p0.y() - p1.y())));
}

#endif

