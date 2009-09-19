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
// inline Point32& Deskew( Point32& pt, int32_t skew1024);
// inline Point16& Deskew( Point16& pt, int32_t skew1024);
//
// NOTE: POSITIVE SKEW == CLOCKWISE
//////////////////////////////////////////////////////

#ifdef __cplusplus

#ifndef __SKEW1024_H
#define __SKEW1024_H

#ifndef __CTTYPES_H
   #include "cttypes.h"
#endif
inline Point32& Deskew( Point32& pt, int32_t skew1024)
{
   long phi_sq = (skew1024*skew1024);
   long dx = ( (skew1024*pt.y+0x200) >> 10 );
   long dy = ( (skew1024*pt.x+0x200) >> 10 );
   long ddx =( (phi_sq*pt.x+0x100000) >> 21 );
   long ddy =( (phi_sq*pt.y+0x100000) >> 21 );
   pt.x -= dx;
   pt.x -= ddx;
   pt.y += dy;
   pt.y -= ddy;
   return pt;
}

inline Point16& Deskew( Point16& pt, int32_t skew1024)
{
   long phi_sq = (skew1024*skew1024);
   long dx = ( (skew1024*(long)pt.y+0x200) >> 10 );
   long dy = ( (skew1024*(long)pt.x+0x200) >> 10 );
   long ddx =( (phi_sq*(long)pt.x+0x100000) >> 21 );
   long ddy =( (phi_sq*(long)pt.y+0x100000) >> 21 );
   pt.x -= (int16_t)dx;
   pt.x -= (int16_t)ddx;
   pt.y += (int16_t)dy;
   pt.y -= (int16_t)ddy;
   return pt;
}

inline Point32& DeskewRel( Point32& pt, int32_t skew1024, Point32& rel )
{
   pt.x -= rel.x;
   pt.y -= rel.y;
   Deskew( pt, skew1024 );
   pt.x += rel.x;
   pt.y += rel.y;
   return pt;
}

inline Point16& DeskewRel( Point16& pt, int32_t skew1024, Point16& rel )
{
   pt.x -= rel.x;
   pt.y -= rel.y;
   Deskew( pt, skew1024 );
   pt.x += rel.x;
   pt.y += rel.y;
   return pt;
}

inline Point32& GetCenter( const Rect32& rc, Point32& center)
{
   center.x = (rc.left+rc.right)>>1;
   center.y = (rc.top+rc.bottom)>>1;
   return center;
}

inline Point16& GetCenter( const Rect16& rc, Point16& center)
{
   center.x = (rc.left+rc.right)>>1;
   center.y = (rc.top+rc.bottom)>>1;
   return center;
}

inline Point32& Copy(Point32& dst, Point16& src)
{
   dst.x=src.x; dst.y=src.y;
   return dst;
}

inline Point16& Copy(Point16& dst, Point32& src)
{
   dst.x=(int16_t)src.x; dst.y=(int16_t)src.y;
   return dst;
}

inline Rect32& Copy(Rect32& dst, Rect16& src)
{
   dst.left = src.left; dst.right  = src.right;
   dst.top  = src.top;  dst.bottom = src.bottom;
   return dst;
}
inline Rect16& Copy(Rect16& dst, Rect32& src)
{
   dst.left = (int16_t)src.left; dst.right  = (int16_t)src.right;
   dst.top  = (int16_t)src.top;  dst.bottom = (int16_t)src.bottom;
   return dst;
}

//////////////////////////////////////////////////////////
// diagonal points => rect
inline Rect32& Pt2Rc(Rect32& dst, Point16& a, Point16& b )
{
   dst.left = a.x; dst.right  = b.x;
   dst.top  = a.y; dst.bottom = b.y;
   return dst;
}
inline Rect16& Pt2Rc(Rect16& dst, Point16& a, Point16& b )
{
   dst.left = a.x; dst.right  = b.x;
   dst.top  = a.y; dst.bottom = b.y;
   return dst;
}
inline Rect32& Pt2Rc(Rect32& dst, Point32& a, Point32& b )
{
   dst.left = a.x; dst.right  = b.x;
   dst.top  = a.y; dst.bottom = b.y;
   return dst;
}
inline Rect16& Pt2Rc(Rect16& dst, Point32& a, Point32& b )
{
   dst.left = (int16_t)a.x; dst.right  = (int16_t)b.x;
   dst.top  = (int16_t)a.y; dst.bottom = (int16_t)b.y;
   return dst;
}

////////////////////////////////////////////////////
// movements ( -=, += )
inline Rect32& operator -=(Rect32& rc, Point32& pt)
{
   rc.left  -= pt.x;   rc.right -= pt.x;
   rc.top   -= pt.y;   rc.bottom-= pt.y;
   return rc;
}

inline Rect16& operator -=(Rect16& rc, Point16& pt)
{
   rc.left  -= pt.x;   rc.right -= pt.x;
   rc.top   -= pt.y;   rc.bottom-= pt.y;
   return rc;
}

inline Rect32& operator +=(Rect32& rc, Point32& pt)
{
   rc.left  += pt.x;   rc.right += pt.x;
   rc.top   += pt.y;   rc.bottom+= pt.y;
   return rc;
}

inline Rect16& operator +=(Rect16& rc, Point16& pt)
{
   rc.left  += pt.x;   rc.right += pt.x;
   rc.top   += pt.y;   rc.bottom+= pt.y;
   return rc;
}

inline Point32& operator -=(Point32& pt, Point32& pt1)
{
   pt.x -= pt1.x;   pt.y -= pt1.y;
   return pt;
}

inline Point16& operator -=(Point16& pt, Point16& pt1)
{
   pt.x -= pt1.x;   pt.y -= pt1.y;
   return pt;
}

inline Point32& operator +=(Point32& pt, Point32& pt1)
{
   pt.x += pt1.x;   pt.y += pt1.y;
   return pt;
}

inline Point16& operator +=(Point16& pt, Point16& pt1)
{
   pt.x += pt1.x;   pt.y += pt1.y;
   return pt;
}


inline Rect32& DeskewCenter( Rect32& rc, int32_t skew1024)
{
   Point32 center; GetCenter(rc, center);
   Point32 old_center = center;
   Deskew( center, skew1024 );
   center -= old_center;   // diff
   rc += center;
   return rc;
}

inline Rect16& DeskewCenter( Rect16& rc, int32_t skew1024)
{
   Point16 center; GetCenter(rc, center);
   Point16 old_center = center;
   Deskew( center, skew1024 );
   center -= old_center;   // diff
   rc += center;
   return rc;
}

inline Rect16& Expand( Rect16& rc, int16_t delta )
{
   rc.left -= delta; rc.top -= delta;
   rc.right+= delta; rc.bottom+= delta;
   return rc;
}

inline Rect32& Expand( Rect32& rc, int32_t delta )
{
   rc.left -= delta; rc.top -= delta;
   rc.right+= delta; rc.bottom+= delta;
   return rc;
}

// make points: lt & rb (main diag) or rt & lb (other)
inline void GetDiag1( Rect32& rc, Point32& lt, Point32& rb )
{  lt.x = rc.left; lt.y = rc.top; rb.x = rc.right;rb.y = rc.bottom; }
inline void GetDiag1( Rect16& rc, Point16& lt, Point16& rb )
{  lt.x = rc.left; lt.y = rc.top; rb.x = rc.right;rb.y = rc.bottom; }
inline void GetDiag2( Rect32& rc, Point32& rt, Point32& lb )
{  rt.x = rc.right; rt.y = rc.top; lb.x = rc.left;lb.y = rc.bottom; }
inline void GetDiag2( Rect16& rc, Point16& rt, Point16& lb )
{  rt.x = rc.right; rt.y = rc.top; lb.x = rc.left;lb.y = rc.bottom; }

inline Rect32& GetBounding( Rect32& rc_src, Rect32& rc_dest, int32_t skew1024 )
   // dest rect can be the same as source
{
   Point32 lt, rb; GetDiag1(rc_src, lt, rb);
   Deskew(lt, skew1024); Deskew(rb, skew1024);
   Point32 rt, lb; GetDiag2(rc_src, rt, lb);
   Deskew(rt, skew1024); Deskew(lb, skew1024);
   if (skew1024 < 0)
   {
      rc_dest.left  = lt.x;
      rc_dest.right = rb.x;
      rc_dest.top   = rt.y;
      rc_dest.bottom= lb.y;
   }
   else
   {
      rc_dest.left  = lb.x;
      rc_dest.right = rt.x;
      rc_dest.top   = lt.y;
      rc_dest.bottom= rb.y;
   };
   return rc_dest;
}

inline Rect16& GetBounding(  Rect16& rc_src, Rect16& rc_dest, int32_t skew1024 )
   // dest rect can be the same as source
{
   Point16 lt, rb; GetDiag1(rc_src, lt, rb);
   Deskew(lt, skew1024); Deskew(rb, skew1024);
   Point16 rt, lb; GetDiag2(rc_src, rt, lb);
   Deskew(rt, skew1024); Deskew(lb, skew1024);
   if (skew1024 < 0)
   {
      rc_dest.left  = lt.x;
      rc_dest.right = rb.x;
      rc_dest.top   = rt.y;
      rc_dest.bottom= lb.y;
   }
   else
   {
      rc_dest.left  = lb.x;
      rc_dest.right = rt.x;
      rc_dest.top   = lt.y;
      rc_dest.bottom= rb.y;
   };
   return rc_dest;
}

///////////////////////////////////////////////////////////////
// 22.03.97 17:48, V.P
inline Bool operator !(const Rect16 & r)  { return (r.left > r.right)||(r.top > r.bottom); }
inline Bool operator !(const Rect32 & r)  { return (r.left > r.right)||(r.top > r.bottom); }
inline Bool Not(const Rect16 & r)  { return (r.left > r.right)||(r.top > r.bottom); }
inline Bool Not(const Rect32 & r)  { return (r.left > r.right)||(r.top > r.bottom); }

#define SetMax( a, b ) { if ((a) < (b)) (a) = (b); }
#define SetMin( a, b ) { if ((a) > (b)) (a) = (b); }

inline Rect32& operator |=(Rect32& rc, const Rect32& r)
{  SetMin( rc.left, r.left );   SetMin( rc.top , r.top  );
   SetMax( rc.right, r.right);  SetMax( rc.bottom, r.bottom );
   return rc;
}
inline Rect16& operator |=(Rect16& rc, const Rect16& r)
{  SetMin( rc.left, r.left );   SetMin( rc.top , r.top  );
   SetMax( rc.right, r.right);  SetMax( rc.bottom, r.bottom );
   return rc;
}

inline Rect32& operator &=(Rect32& rc, const Rect32& r)
{  SetMax( rc.left, r.left );   SetMax( rc.top , r.top  );
   SetMin( rc.right, r.right);  SetMin( rc.bottom, r.bottom );
   return rc;
}
inline Rect16& operator &=(Rect16& rc, const Rect16& r)
{  SetMax( rc.left, r.left );   SetMax( rc.top , r.top  );
   SetMin( rc.right, r.right);  SetMin( rc.bottom, r.bottom );
   return rc;
}

inline Rect16& Set( Rect16& r, int left, int top, int right, int bottom )
   {  r.left=left; r.top=top; r.right=right; r.bottom=bottom; return r; }
inline Rect32& Set( Rect32& r, int left, int top, int right, int bottom )
   {  r.left=left; r.top=top; r.right=right; r.bottom=bottom; return r; }
inline Rect16& Check( Rect16& r )
   {
      if (r.left>r.right) {int x=r.left; r.left=r.right; r.right=x; };
      if (r.top>r.bottom) {int x=r.top; r.top=r.bottom; r.bottom=x; }
      return r;
   }
inline Rect32& Check( Rect32& r )
   {
      if (r.left>r.right) {int32_t x=r.left; r.left=r.right; r.right=x; };
      if (r.top>r.bottom) {int32_t x=r.top; r.top=r.bottom; r.bottom=x; }
      return r;
   }

inline Point16& Set( Point16& p, int x, int y ){  p.x=x; p.y=y; return p; }
inline Point32& Set( Point32& p, int x, int y ){  p.x=x; p.y=y; return p; }

inline int width( const Rect16& rc ) { return rc.right-rc.left+1; }
inline int width( const Rect32& rc ) { return rc.right-rc.left+1; }

inline int height( const Rect16& rc ) { return rc.bottom-rc.top+1; }
inline int height( const Rect32& rc ) { return rc.bottom-rc.top+1; }

inline Bool Overlap( const Rect16& r1, const Rect16& r2 )
{ return !( (r1.left > r2.right)||(r1.top > r2.bottom) ||
            (r2.left > r1.right)||(r2.top > r1.bottom)
          );
}
inline Bool Overlap( const Rect32& r1, const Rect32& r2 )
{ return !( (r1.left > r2.right)||(r1.top > r2.bottom) ||
            (r2.left > r1.right)||(r2.top > r1.bottom)
          );
}

inline Bool Inside( const Rect16& r, const Point16 & p)
{ return !( (p.x < r.left)||(p.x > r.right )||
            (p.x < r.top) ||(p.x > r.bottom)
          );
}
inline Bool Inside( const Rect32& r, const Point32 & p)
{ return !( (p.x < r.left)||(p.x > r.right )||
            (p.x < r.top) ||(p.x > r.bottom)
          );
}

inline Bool Overlap( int a1, int a2, int b1, int b2 )  // two ranges intersection
{  // assumed that a1 <= a2; b1 <= b2
   return !( (a2 < b1) || (b2 < a1) );
}

#endif
#endif // __cplusplus
