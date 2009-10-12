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

#include "cttypes.h"
#include "point.h"

inline CIF::Point& Deskew(CIF::Point& pt, int32_t skew1024) {
	long phi_sq = (skew1024 * skew1024);
	long dx = ((skew1024 * pt.y() + 0x200) >> 10);
	long dy = ((skew1024 * pt.x() + 0x200) >> 10);
	long ddx = ((phi_sq * pt.x() + 0x100000) >> 21);
	long ddy = ((phi_sq * pt.y() + 0x100000) >> 21);
	pt.rx() -= dx;
	pt.rx() -= ddx;
	pt.ry() += dy;
	pt.ry() -= ddy;
	return pt;
}

inline CIF::Point16& Deskew(CIF::Point16& pt, int32_t skew1024) {
	long phi_sq = (skew1024 * skew1024);
	long dx = ((skew1024 * (long) pt.y() + 0x200) >> 10);
	long dy = ((skew1024 * (long) pt.x() + 0x200) >> 10);
	long ddx = ((phi_sq * (long) pt.x() + 0x100000) >> 21);
	long ddy = ((phi_sq * (long) pt.y() + 0x100000) >> 21);
	pt.rx() -= dx;
	pt.rx() -= ddx;
	pt.ry() += dy;
	pt.ry() -= ddy;
	return pt;
}

inline CIF::Point& DeskewRel(CIF::Point& pt, int32_t skew1024, CIF::Point& rel) {
	pt.rx() -= rel.x();
	pt.ry() -= rel.y();
	Deskew(pt, skew1024);
	pt.rx() += rel.x();
	pt.ry() += rel.y();
	return pt;
}

inline CIF::Point16& DeskewRel(CIF::Point16& pt, int32_t skew1024,
		CIF::Point16& rel) {
	pt.rx() -= rel.x();
	pt.ry() -= rel.y();
	Deskew(pt, skew1024);
	pt.rx() += rel.x();
	pt.ry() += rel.y();
	return pt;
}

inline CIF::Point& GetCenter(const Rect32& rc, CIF::Point& center) {
	center.rx() = (rc.left + rc.right) >> 1;
	center.ry() = (rc.top + rc.bottom) >> 1;
	return center;
}

inline CIF::Point16& GetCenter(const Rect16& rc, CIF::Point16& center) {
	center.rx() = (rc.left + rc.right) >> 1;
	center.ry() = (rc.top + rc.bottom) >> 1;
	return center;
}

inline Rect32& Copy(Rect32& dst, Rect16 const & src) {
	dst.left = src.left;
	dst.right = src.right;
	dst.top = src.top;
	dst.bottom = src.bottom;
	return dst;
}

inline Rect16& Copy(Rect16& dst, Rect32 const & src) {
	dst.left = (int16_t) src.left;
	dst.right = (int16_t) src.right;
	dst.top = (int16_t) src.top;
	dst.bottom = (int16_t) src.bottom;
	return dst;
}

// diagonal points => rect
inline Rect32& Pt2Rc(Rect32& dst, const CIF::Point16& a, const CIF::Point16 & b) {
	dst.left = a.x();
	dst.right = b.x();
	dst.top = a.y();
	dst.bottom = b.y();
	return dst;
}
inline Rect16& Pt2Rc(Rect16& dst, const CIF::Point16 & a, const CIF::Point16 & b) {
	dst.left = a.x();
	dst.right = b.x();
	dst.top = a.y();
	dst.bottom = b.y();
	return dst;
}
inline Rect32& Pt2Rc(Rect32& dst, CIF::Point const & a, CIF::Point const & b) {
	dst.left = a.x();
	dst.right = b.x();
	dst.top = a.y();
	dst.bottom = b.y();
	return dst;
}
inline Rect16& Pt2Rc(Rect16& dst, CIF::Point const & a, CIF::Point const & b) {
	dst.left = a.x();
	dst.right = b.x();
	dst.top = a.y();
	dst.bottom = b.y();
	return dst;
}

// movements ( -=, += )
#define RC_MINUS_PT  {  rc.left  -= pt.x(); rc.right -= pt.x(); rc.top   -= pt.y(); rc.bottom-= pt.y(); return rc; }
#define RC_PLUS_PT   {  rc.left  += pt.x(); rc.right += pt.x(); rc.top   += pt.y(); rc.bottom+= pt.y(); return rc; }
#define RC_MINUS_PT_16 {  rc.left  -= pt.x(); rc.right -= pt.x(); rc.top   -= pt.y(); rc.bottom-= pt.y(); return rc; }
#define RC_PLUS_PT_16  {  rc.left  += pt.x(); rc.right += pt.x(); rc.top   += pt.y(); rc.bottom+= pt.y(); return rc; }
inline Rect32& operator -=(Rect32& rc, CIF::Point const & pt)
RC_MINUS_PT
inline Rect16& operator -=(Rect16& rc, CIF::Point16 const & pt)
RC_MINUS_PT_16
inline Rect32& operator -=(Rect32& rc, CIF::Point16 const & pt)
RC_MINUS_PT
inline Rect16& operator -=(Rect16& rc, CIF::Point const & pt)
RC_MINUS_PT_16
inline Rect32& operator +=(Rect32& rc, CIF::Point const & pt)
RC_PLUS_PT
inline Rect16& operator +=(Rect16& rc, CIF::Point16 const & pt)
RC_PLUS_PT_16
inline Rect32& operator +=(Rect32& rc, CIF::Point16 const & pt)
RC_PLUS_PT
inline Rect16& operator +=(Rect16& rc, CIF::Point const & pt)
RC_PLUS_PT_16

#define PT_MINUS_PT {  pt.rx() -= pt1.x();   pt.ry() -= pt1.y();  return pt;  }
#define PT_PLUS_PT  {  pt.rx() += pt1.x();   pt.ry() += pt1.y();  return pt;  }
inline CIF::Point& operator -=(CIF::Point& pt, CIF::Point16 const & pt1)
PT_MINUS_PT
inline CIF::Point16& operator -=(CIF::Point16& pt, CIF::Point const & pt1)
PT_MINUS_PT
inline CIF::Point& operator +=(CIF::Point& pt, CIF::Point16 const & pt1)
PT_PLUS_PT
inline CIF::Point16& operator +=(CIF::Point16& pt, CIF::Point const & pt1)
PT_PLUS_PT

// --- multiplication ---
#define RC_MUL_A  { rc.left*=a; rc.right*=a; rc.top*=a; rc.bottom*=a; return rc;}
inline Rect32& operator *=(Rect32& rc, const int a)
RC_MUL_A
inline Rect16& operator *=(Rect16& rc, const int a)
RC_MUL_A
inline Rect32& operator *=(Rect32& rc, const double a)
RC_MUL_A
inline Rect16& operator *=(Rect16& rc, const double a)
RC_MUL_A

#define PT_MUL_A  {  pt.rx()  *= a;   pt.ry() *= a;  return pt; }
inline CIF::Point& operator *=(CIF::Point& pt, const int a)
PT_MUL_A
inline CIF::Point16& operator *=(CIF::Point16& pt, const int a)
PT_MUL_A
inline CIF::Point& operator *=(CIF::Point& pt, double a)
PT_MUL_A
inline CIF::Point16& operator *=(CIF::Point16& pt, double a)
PT_MUL_A

// --- division ---
inline Rect32& operator /=(Rect32& rc, const int a) {
	rc.left /= a;
	rc.right /= a;
	rc.top /= a;
	rc.bottom /= a;
	return rc;
}
inline Rect16& operator /=(Rect16& rc, const int a) {
	rc.left /= a;
	rc.right /= a;
	rc.top /= a;
	rc.bottom /= a;
	return rc;
}
inline CIF::Point& operator /=(CIF::Point& pt, const int a) {
	pt.rx() /= a;
	pt.ry() /= a;
	return pt;
}
inline CIF::Point16& operator /=(CIF::Point16& pt, const int a) {
	pt.rx() /= a;
	pt.ry() /= a;
	return pt;
}

// equality
inline Bool operator ==(const Rect16& r1, const Rect16& r2) {
	return r1.left == r2.left && r1.top == r2.top && r1.right == r2.right
			&& r1.bottom == r2.bottom;
}

inline Bool operator ==(const Rect32& r1, const Rect32& r2) {
	return r1.left == r2.left && r1.top == r2.top && r1.right == r2.right
			&& r1.bottom == r2.bottom;
}

inline Rect32& DeskewCenter(Rect32& rc, int32_t skew1024) {
	CIF::Point center;
	GetCenter(rc, center);
	CIF::Point old_center = center;
	Deskew(center, skew1024);
	center -= old_center; // diff
	rc += center;
	return rc;
}

inline Rect16& DeskewCenter(Rect16& rc, int32_t skew1024) {
	CIF::Point16 center;
	GetCenter(rc, center);
	CIF::Point16 old_center = center;
	Deskew(center, skew1024);
	center -= old_center; // diff
	rc += center;
	return rc;
}

inline Rect32& DeskewCenterRel(Rect32& rc, int32_t skew1024, CIF::Point& pt) {
	CIF::Point center;
	GetCenter(rc, center);
	CIF::Point old_center = center;
	DeskewRel(center, skew1024, pt);
	center -= old_center; // diff
	rc += center;
	return rc;
}

inline Rect16& DeskewCenterRel(Rect16& rc, int32_t skew1024, CIF::Point16& pt) {
	CIF::Point16 center;
	GetCenter(rc, center);
	CIF::Point16 old_center = center;
	DeskewRel(center, skew1024, pt);
	center -= old_center; // diff
	rc += center;
	return rc;
}

inline Rect16& Expand(Rect16& rc, int16_t delta) {
	rc.left -= delta;
	rc.top -= delta;
	rc.right += delta;
	rc.bottom += delta;
	return rc;
}

inline Rect32& Expand(Rect32& rc, int32_t delta) {
	rc.left -= delta;
	rc.top -= delta;
	rc.right += delta;
	rc.bottom += delta;
	return rc;
}

// make points: lt & rb (main diag) or rt & lb (other)
inline void GetDiag1(Rect32& rc, CIF::Point& lt, CIF::Point& rb) {
	lt.rx() = rc.left;
	lt.ry() = rc.top;
	rb.rx() = rc.right;
	rb.ry() = rc.bottom;
}
inline void GetDiag1(Rect16& rc, CIF::Point16& lt, CIF::Point16& rb) {
	lt.rx() = rc.left;
	lt.ry() = rc.top;
	rb.rx() = rc.right;
	rb.ry() = rc.bottom;
}
inline void GetDiag2(Rect32& rc, CIF::Point& rt, CIF::Point& lb) {
	rt.rx() = rc.right;
	rt.ry() = rc.top;
	lb.rx() = rc.left;
	lb.ry() = rc.bottom;
}
inline void GetDiag2(Rect16& rc, CIF::Point16& rt, CIF::Point16& lb) {
	rt.rx() = rc.right;
	rt.ry() = rc.top;
	lb.rx() = rc.left;
	lb.ry() = rc.bottom;
}

inline Rect32& GetBounding(Rect32& rc_src, Rect32& rc_dest, int32_t skew1024)
// dest rect can be the same as source
{
	CIF::Point lt, rb;
	GetDiag1(rc_src, lt, rb);
	Deskew(lt, skew1024);
	Deskew(rb, skew1024);
	CIF::Point rt, lb;
	GetDiag2(rc_src, rt, lb);
	Deskew(rt, skew1024);
	Deskew(lb, skew1024);
	if (skew1024 < 0) {
		rc_dest.left = lt.x();
		rc_dest.right = rb.x();
		rc_dest.top = rt.y();
		rc_dest.bottom = lb.y();
	} else {
		rc_dest.left = lb.x();
		rc_dest.right = rt.x();
		rc_dest.top = lt.y();
		rc_dest.bottom = rb.y();
	};
	return rc_dest;
}

inline Rect16& GetBounding(Rect16& rc_src, Rect16& rc_dest, int32_t skew1024)
// dest rect can be the same as source
{
	CIF::Point16 lt, rb;
	GetDiag1(rc_src, lt, rb);
	Deskew(lt, skew1024);
	Deskew(rb, skew1024);
	CIF::Point16 rt, lb;
	GetDiag2(rc_src, rt, lb);
	Deskew(rt, skew1024);
	Deskew(lb, skew1024);
	if (skew1024 < 0) {
		rc_dest.left = lt.x();
		rc_dest.right = rb.x();
		rc_dest.top = rt.y();
		rc_dest.bottom = lb.y();
	} else {
		rc_dest.left = lb.x();
		rc_dest.right = rt.x();
		rc_dest.top = lt.y();
		rc_dest.bottom = rb.y();
	};
	return rc_dest;
}

// inequality
#define RC_NOTEQ_RC  { return       r1.left != r2.left   || r1.top != r2.top       \
                                 || r1.right != r2.right || r1.bottom!= r2.bottom; }

inline Bool operator !=(const Rect16& r1, const Rect16& r2)
RC_NOTEQ_RC
inline Bool operator !=(const Rect16& r1, const Rect32& r2)
RC_NOTEQ_RC
inline Bool operator !=(const Rect32& r1, const Rect16& r2)
RC_NOTEQ_RC
inline Bool operator !=(const Rect32& r1, const Rect32& r2)
RC_NOTEQ_RC

#define PT_NOTEQ_PT  { return    pt1.x() != pt2.x() || pt1.y() != pt2.y(); }

inline Bool operator !=(const CIF::Point16& pt1, const CIF::Point& pt2)
PT_NOTEQ_PT
inline Bool operator !=(const CIF::Point& pt1, const CIF::Point16& pt2)
PT_NOTEQ_PT

// 22.03.97 17:48, V.P
inline Bool operator !(const Rect16 & r) {
	return (r.left > r.right) || (r.top > r.bottom);
}
inline Bool operator !(const Rect32 & r) {
	return (r.left > r.right) || (r.top > r.bottom);
}
inline Bool Not(const Rect16 & r) {
	return (r.left > r.right) || (r.top > r.bottom);
}
inline Bool Not(const Rect32 & r) {
	return (r.left > r.right) || (r.top > r.bottom);
}

#define SetMax( a, b ) { if ((a) < (b)) (a) = (b); }
#define SetMin( a, b ) { if ((a) > (b)) (a) = (b); }

inline Rect32& operator |=(Rect32& rc, const Rect32& r) {
	SetMin( rc.left, r.left );
	SetMin( rc.top , r.top );
	SetMax( rc.right, r.right);
	SetMax( rc.bottom, r.bottom );
	return rc;
}
inline Rect16& operator |=(Rect16& rc, const Rect16& r) {
	SetMin( rc.left, r.left );
	SetMin( rc.top , r.top );
	SetMax( rc.right, r.right);
	SetMax( rc.bottom, r.bottom );
	return rc;
}

inline Rect32& operator &=(Rect32& rc, const Rect32& r) {
	SetMax( rc.left, r.left );
	SetMax( rc.top , r.top );
	SetMin( rc.right, r.right);
	SetMin( rc.bottom, r.bottom );
	return rc;
}
inline Rect16& operator &=(Rect16& rc, const Rect16& r) {
	SetMax( rc.left, r.left );
	SetMax( rc.top , r.top );
	SetMin( rc.right, r.right);
	SetMin( rc.bottom, r.bottom );
	return rc;
}

inline Rect16& Set(Rect16& r, int left, int top, int right, int bottom) {
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	return r;
}
inline Rect32& Set(Rect32& r, int left, int top, int right, int bottom) {
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	return r;
}
inline Rect16& Check(Rect16& r) {
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
inline Rect32& Check(Rect32& r) {
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

inline int width(const Rect16& rc) {
	return rc.right - rc.left + 1;
}
inline int width(const Rect32& rc) {
	return rc.right - rc.left + 1;
}

inline int height(const Rect16& rc) {
	return rc.bottom - rc.top + 1;
}
inline int height(const Rect32& rc) {
	return rc.bottom - rc.top + 1;
}

inline Bool Overlap(const Rect16& r1, const Rect16& r2) {
	return !((r1.left > r2.right) || (r1.top > r2.bottom) || (r2.left
			> r1.right) || (r2.top > r1.bottom));
}
inline Bool Overlap(const Rect32& r1, const Rect32& r2) {
	return !((r1.left > r2.right) || (r1.top > r2.bottom) || (r2.left
			> r1.right) || (r2.top > r1.bottom));
}

inline Bool Inside(const Rect16& r, const CIF::Point16 & p) {
	return !((p.x() < r.left) || (p.x() > r.right) || (p.y() < r.top) || (p.y()
			> r.bottom));
}
inline Bool Inside(const Rect32& r, const CIF::Point & p) {
	return !((p.x() < r.left) || (p.x() > r.right) || (p.y() < r.top) || (p.y()
			> r.bottom));
}

inline Bool Overlap(int a1, int a2, int b1, int b2) // two ranges intersection
{ // assumed that a1 <= a2; b1 <= b2
	return !((a2 < b1) || (b2 < a1));
}

#endif
