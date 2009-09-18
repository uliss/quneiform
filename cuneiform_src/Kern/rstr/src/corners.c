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



/*********************************************************************/
/*                                                                   */
/*     module CORNERS - studies corners of the component(s)          */
/*                                                                   */
/*********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nt_types.h"
#include "struct.h"
#include "cutstr.h"
#include "func.h"
#include "embbox.h"

#define  CONST_PLUS 10      // this constant is added abris values in order
                            // to avoid negative values caused by shifting
#define AVOID_PRYSCH 3

/************** all function prototypes ***********************************/
INT u_around_bl_corner ( s_glue * );
INT n_around_ur_corner ( s_glue * );
void comp_row_col( s_glue * );
static void comp_parms( BYTE );
// static INT bottom_right_corner (PBYTE, BYTE, BYTE, BYTE);
static INT bottom_left_corner (PBYTE, BYTE, BYTE, BYTE);
static INT upper_right_corner (PBYTE, BYTE, BYTE, BYTE);
//static INT upper_left_corner (PBYTE, BYTE, BYTE, BYTE);
static BOOL is_italic();
static void compare_corners_mass();
static void make_straight_abris (s_glue *, PBYTE, PBYTE);
static INT func_mode_val( PBYTE func, BYTE from, BYTE to);
static INT left_right_dist();
/*************************************************************************/

extern servBOX SBOX;
BYTE str_left[128];
BYTE str_right[128];

BYTE l_tab_shift[128];       // the tables of shifts ( avoiding italic
BYTE r_tab_shift[128];       //  and oblique )

#define ST_PEN_NUM 5
static INT pen_for_staires[ST_PEN_NUM+1] = { 0, 14, 36, 60, 72, 120 };
#define LRD_PEN_NUM 7
static INT pen_for_left_right_dist[LRD_PEN_NUM+1] =
                                     { 0, 0, 12, 46, 80, 120, 160, 220 };
static BYTE comp_h, un_code;
static BYTE num_st, beg_zone, end_zone;
static INT pen_u, pen_n;
static INT maxrow, maxcol, minrow, mincol;


#define Let_n   2
#define Let_u   4
void discrim_un( cell *GC, s_glue *GL, INT cod_let )
{
INT wi;

 memset ( l_tab_shift, 0, sizeof (l_tab_shift) );
 memset ( r_tab_shift, 0, sizeof (r_tab_shift) );

 pen_u = 0; pen_n = 0; un_code = (BYTE)cod_let;

 comp_row_col( GL );          // compute coordinates of the composed box
 // check italic incline of the component
 wi = letincl(GC);

 if ( ((GC->font|GC->font_new) & c_fp_it) && (GL->ncell == 1) )  return;

 make_straight_abris( GL, l_tab_shift, r_tab_shift );

 if (GL->ncell > 1)
  {
    if ( is_italic() )
     { compare_corners_mass();   goto ApplyPenalty;}
    else return;
  }
 if ( cod_let & Let_n )
  {
   pen_n = n_around_ur_corner(GL); // study right upper corner against arounding
   pen_n += left_right_dist();
  }
 if ( cod_let & Let_u )
   pen_u = u_around_bl_corner(GL); // study left bottom corner against arounding

ApplyPenalty:
 if ( pen_u )
  {
   cell_bonus_let(GC, 'u', (INT)(-(pen_u)) );
   sort_vers( GC );
  }
 if ( pen_n )
  {
   cell_bonus_let(GC, 'n', (INT)(-(pen_n)) );
   sort_vers( GC );
  }
}

static void make_straight_abris (s_glue *GL, PBYTE left_shift, PBYTE right_shift)
{
    INT             i, j, y;
    BYTE            beg, end;
    interval        *cur_int;
    lnhead          *Line;
    cell            *WC;

  // initialize both arraies
  memset (str_left,  255, sizeof (str_left));
  memset (str_right, 255, sizeof (str_right));
  j = 0;

  while ( (WC=GL->celist[j]) != NULL )
  {
    // set address of the first line
    Line = (lnhead *)(((PBYTE) (WC -> env)) + WC -> env -> lines + sizeof (INT));

    while ( Line -> lth )        // loop for all lines
    {
        y = maxrow - WC->row - (Line -> row) - 1; // uppermost y-coordinate
        cur_int = (interval *)(Line + 1);           // first interval

        for ( i = y; (cur_int -> l > 0 ); i--)     // loop for all intervals
        {
            beg = cur_int->e - cur_int->l + CONST_PLUS;
            beg = WC->col - mincol + ( beg - left_shift[i] );
            if ( beg < str_left[i] )
              str_left[i] = beg;

            end = maxcol - WC->col - ( cur_int->e + right_shift[i] );
            if ( end < str_right[i] )
              str_right[i] = end;

            cur_int++;
        }
        Line = (lnhead *)( (PBYTE)Line + Line->lth );
    }
   j++;
  }
}

/*********************
static INT upper_left_corner (PBYTE s_left, BYTE from, BYTE to, BYTE minst)
{
    INT  dif;
    INT  st, prev, cur;

 for ( st=0, cur=to-1, prev=to; ( cur>=from && st<minst ); prev--, cur-- )
  {
    if ( s_left[prev] - s_left[cur] > AVOID_PRYSCH )
     { st = 0; continue; }
    dif = s_left[cur] - s_left[prev];
    if ( dif > 0 )
     {
      if ( dif > 1 )
        { st = 0;  break; }
       else
        { st --; continue; }
      }
    if ( dif == 0 ) continue;
    st ++;
  }
 return st;
}
*******************/

static INT upper_right_corner (PBYTE s_right, BYTE from, BYTE to, BYTE minst)
{
    INT dif;
    INT st, prev, cur;

 for ( st=0, cur=to-1, prev=to; ( cur>=from && st<minst ); prev--, cur-- )
  {
    if ( s_right[prev] - s_right[cur] > AVOID_PRYSCH )
     { st = 0; continue; }
    dif = s_right[cur] - s_right[prev];
    if ( dif > 0 )
     { if ( dif > 1 )
        { st = 0;  break; }
       else
        { st --; continue; }
     }
    if ( dif == 0 ) continue;
    st ++;
  }
 return st;
}


static INT bottom_left_corner (PBYTE s_left, BYTE from, BYTE to, BYTE minst)
{
    INT  dif;
    INT  st, prev, cur;

 for ( st=0, cur=from+1, prev=from; ( cur <= to && st<minst ); prev++, cur++ )
  {
    if ( s_left[prev] - s_left[cur] > AVOID_PRYSCH )
     { st = 0; continue; }
    dif = s_left[cur] - s_left[prev];
    if ( dif > 0 )
     {
      if ( dif > 1 )
        { st = 0;  break; }
       else
        { st --; continue; }
      }
    if ( dif == 0 ) continue;
    st ++;
  }
 return st;
}


/*****************
static INT bottom_right_corner (PBYTE s_right, BYTE from, BYTE to, BYTE minst)
{
    INT  dif;
    INT  st, prev, cur;

 for ( st=0, cur=from+1, prev=from; ( cur <= to && st<minst ); prev++, cur++ )
  {
    if ( s_right[prev] - s_right[cur] > AVOID_PRYSCH )
     { st = 0; continue; }
    dif = s_right[cur] - s_right[prev];
    if ( dif > 0 )
     {
      if ( dif > 1 )
        { st = 0;  break; }
       else
        { st --; continue; }
      }
    if ( dif == 0 ) continue;
    st ++;
  }
 return st;
}
******************/

static void comp_parms( BYTE fullh )
{

  if ( (fullh < 40) && (fullh > 24) )
   { num_st = 4; beg_zone = 0; end_zone = (fullh)/3; }
  if ( (fullh <= 24) && (fullh >15) )
   { num_st = 3; beg_zone = 0; end_zone = (fullh)/3 + 1; }
  if ( fullh <= 15 )
   { num_st = 2; beg_zone = 0; end_zone = (fullh)/3; }
  if ( fullh >= 40 )
   { num_st = 5; beg_zone = 1; end_zone =(fullh)/3;  }
}

void comp_row_col( s_glue *GL )
{
cell * curc;
INT j;

  maxrow = maxcol = -32000;
  minrow = mincol = 32767;
  j = 0;

  while ( (curc = GL->celist[j]) != NULL )
  {
   if (mincol > curc->col) mincol=curc->col;
   if (minrow > curc->row) minrow=curc->row;
   if (maxrow < (curc->row + curc->h)) maxrow=curc->row+curc->h;
   if (maxcol < (curc->col+curc->w)) maxcol=curc->col+curc->w;
   j++;
  }
 GL->height = maxrow - minrow;
 comp_h = (BYTE)GL->height;
 GL->width = maxcol - mincol;
}

INT n_around_ur_corner ( s_glue *GL )
{
 BYTE from, to, np;
 INT ret_pen;

  ret_pen = 0;
  comp_parms ( (BYTE)GL->height );
  from = GL->height - end_zone + 1;
  to = GL->height - 1 - beg_zone;

  ret_pen = upper_right_corner( str_right, from, to, num_st );
  np = num_st - ret_pen;
  if ( np > ST_PEN_NUM ) np = ST_PEN_NUM;
  ret_pen = pen_for_staires[np];
  return ret_pen;
}

INT u_around_bl_corner ( s_glue *GL )
{
 BYTE from, to, np;
 INT ret_pen;

  comp_parms ( (BYTE)GL->height );
  from = beg_zone;
  to = end_zone;

  ret_pen = bottom_left_corner( str_left, from, to, num_st );
  np = num_st - ret_pen;
  if ( np > ST_PEN_NUM ) np = ST_PEN_NUM;
  ret_pen = pen_for_staires[np];

  return ret_pen;

}

static BOOL is_italic()
{
BYTE h14, i;
INT lmax, xmax;

 h14 =(comp_h>>2);
 for ( xmax=h14, lmax=0, i=h14; i < comp_h - 2; i++ )
  {
   if ( str_left[i+1] - str_left[i] > 3 )   // big gap to right is forbidden
      return FALSE;
   if ( lmax < str_left[i] )
    { lmax = str_left[i]; xmax = i; }
  }

 if ( str_left[xmax] - str_left[h14] > 2 )
  return TRUE;
 if ( str_left[xmax] - str_left[h14] == 0 )
  return FALSE;
 for ( i=h14; i <= xmax; i++ )
  {
   if ( str_left[i] > str_left[i+1] )
     return FALSE;
  }
 return TRUE;
}

static void compare_corners_mass()
{
INT  d;

 d = mBOX[2] - mBOX[12];
 if ( d > 0 )
  {
   if ( un_code & Let_u )
    pen_u += ( (d+1)>>1 ) & 0xfe;
  }
 else
  {
   if ( un_code & Let_n )
    pen_n += ( (abs(d)+1)>>1 ) & 0xfe;
  }
}

static INT left_right_dist()
{
INT lmd, rmd, h14;
BYTE j;

   h14 = (comp_h >> 2);
   lmd = func_mode_val( str_left, (BYTE)h14, (BYTE)(comp_h - h14));
   lmd -= CONST_PLUS;
   rmd = func_mode_val( str_right, (BYTE)h14, (BYTE)(comp_h - h14));
   j = abs(lmd - rmd);
   if ( j > LRD_PEN_NUM ) j = LRD_PEN_NUM;
   return pen_for_left_right_dist[j];
}

static INT func_mode_val( PBYTE func, BYTE from, BYTE to)
{
INT ans;
INT i, j, maxv;
BYTE counts[128];

  memset ( counts, 0, sizeof( counts ) );

  for ( maxv=0, i=from; i < to; i++ )
   {
    j = func[i];
    counts[j] ++;
    if ( maxv < j ) maxv = j;
   }
  for ( ans=0, j=0, i=0; i <= maxv; i++ )
   {
     if ( j < counts[i] )
       { j = counts[i]; ans = i; }
   }
  return ans;
}
