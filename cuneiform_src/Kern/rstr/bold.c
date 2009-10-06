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


/********************************************************************/
/*								    */
/*			       Bolds program                        */
/*								    */
/********************************************************************/
#include <string.h>
#include "struct.h"
#include "cuthdr.h"
#include "func.h"

cell *SHAVE_CELL;
char *SHAVE_RASTER;
char *SHAVE_FLAGS;
int16_t SHAVE_HEIGHT;
int16_t SHAVE_WIDTH ;

Bool boldshave(cell *C,int16_t method)
//
//  This procedure shaves skin from image in cell *C.
//
 {
 MN *mn;
 cell *D;
 int16_t bd; char df;
 uchar sv[sizeof(D->nvers)+sizeof(D->vers)];
 uchar svf;
// cell c;

 Z=&string;
 // memcpy (&c,C,sizeof(c));
 memcpy (sv,&C->nvers,sizeof(C->nvers)+sizeof(C->vers));
 svf = (uchar)C->flg;
 SHAVE_CELL=C;
 SHAVE_RASTER=save_raster(C);
 SHAVE_FLAGS=t_raster();
 memset(SHAVE_FLAGS,0,1024);
 shaving(method);
 mn=c_locomp(SHAVE_RASTER,(int16_t)((C->w+7)/8),C->h,C->r_row,C->r_col);
 if (!mn) return 0;
 D=C->prev;
 bd=C->bdiff; df = C->difflg & 0xf0;
 del_cell(C);
 C=create_cell(mn,D,(char)bd, df);
 // memcpy (C,&c,sizeof(c));
 memcpy (&C->nvers,sv,sizeof(C->nvers)+sizeof(C->vers));
 C->flg = svf;
 adjust_links(C);
 return 1;
 }

static uchar mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
static void rotate_coords(int16_t *row,int16_t *col,int16_t direct,int16_t H)
{
int16_t t;

if( !direct )
  { // rotate coords
  t   = *row;
  *row = H-*col-1;
  *col = t;
  }
return;
}

static void clear_pixel(int16_t H,int16_t BW,int16_t row,int16_t col,int16_t direct)
//
//  This procedure clear one pixel in raster
//
{
rotate_coords(&row,&col,direct,H);

SHAVE_RASTER[ BW*row + (col>>3)] &= ~mask[ col&7 ];
return;
}

static void set_pixel(int16_t H,int16_t BW,int16_t row,int16_t col,int16_t direct)
//
//  This procedure set one pixel in raster
//
{
rotate_coords(&row,&col,direct,H);

SHAVE_RASTER[ BW*row + (col>>3)] |= mask[ col&7 ];
return;
}


static uchar read_pixel(int16_t H,int16_t BW,int16_t row,int16_t col,int16_t direct)
//
//  This procedure read one pixel in raster
//
{
rotate_coords(&row,&col,direct,H);

if( row<0 || row>=SHAVE_HEIGHT )  return 0;
if( col<0 || col>=SHAVE_WIDTH  )  return 0;
return( (SHAVE_RASTER[ BW*row + (col>>3)] & mask[ col&7 ])!=0);
}

static int16_t enable_shl(int16_t ch,int16_t cw,int16_t row,int16_t col)
{
return( !read_pixel(ch,cw,(int16_t)(row-1),col,1) && !read_pixel(ch,cw,(int16_t)(row+1),col,1) );
}

static int16_t enable_shr(int16_t ch,int16_t cw,int16_t row,int16_t col)
{
return( !read_pixel(ch,cw,(int16_t)(row-1),col,1) && !read_pixel(ch,cw,(int16_t)(row+1),col,1) );
}

static int16_t enable_deleting_pimple(int16_t ch,int16_t cw,int16_t row,int16_t col,int16_t len,int16_t d)
{
int16_t i;
uchar p;

for(p=0,i=0;i<len;i++)
      {
      p <<= 1;
      p += read_pixel(ch,cw,row,(int16_t)(col+i),d);
      }
return len==3 ? (p!=5) : ( p!=9 && p!=11 && p!=13 ) ;
}

static int16_t disable_deleting_pimple1(int16_t ch,int16_t cw,int16_t row,int16_t e,int16_t d)
{
if( row<0 || row>=SHAVE_HEIGHT )  return 0;
// 0xx0    0xx0  for len==1
// 00pe    0pe0
if( !read_pixel(ch,cw,row,(int16_t)(e-3),d) && !read_pixel(ch,cw,row,e,d) &&
      (read_pixel(ch,cw,row,(int16_t)(e-2),d)||read_pixel(ch,cw,row,(int16_t)(e-1),d))  )
  return 1;
if( !read_pixel(ch,cw,row,(int16_t)(e-2),d) && !read_pixel(ch,cw,row,(int16_t)(e+1),d) &&
      (read_pixel(ch,cw,row,(int16_t)(e-1),d)||read_pixel(ch,cw,row,e,d))   )
  return 1;

return 0;
}

static int16_t disable_deleting_pimple2(int16_t ch,int16_t cw,int16_t row, int16_t e, int16_t d)
{
if( row<0 || row>=SHAVE_HEIGHT )  return 0;
// 0xxx0   0xxx0  0xxx0   0xxx0 for len==2
// 0ppe0   00ppe  000ppe  ppe00
if( !read_pixel(ch,cw,row,(int16_t)(e-3),d) && !read_pixel(ch,cw,row,(int16_t)(e+1),d) &&
      (read_pixel(ch,cw,row,(int16_t)(e-2),d)||
       read_pixel(ch,cw,row,(int16_t)(e-1),d)||
       read_pixel(ch,cw,row,e,d))  )
  return 1;
if( !read_pixel(ch,cw,row,(int16_t)(e-4),d) && !read_pixel(ch,cw,row,e,d) &&
      (read_pixel(ch,cw,row,(int16_t)(e-3),d)||
       read_pixel(ch,cw,row,(int16_t)(e-2),d)||
       read_pixel(ch,cw,row,(int16_t)(e-1),d))   )
  return 1;

if( !read_pixel(ch,cw,row,(int16_t)(e-5),d) && !read_pixel(ch,cw,row,(int16_t)(e-1),d) &&
      (read_pixel(ch,cw,row,(int16_t)(e-4),d)||
       read_pixel(ch,cw,row,(int16_t)(e-3),d)||
       read_pixel(ch,cw,row,(int16_t)(e-2),d))   )
  return 1;
if( !read_pixel(ch,cw,row,(int16_t)(e-2),d) && !read_pixel(ch,cw,row,(int16_t)(e-2),d) &&
      (read_pixel(ch,cw,row,(int16_t)(e-1),d)||
       read_pixel(ch,cw,row,e,d)||
       read_pixel(ch,cw,row,(int16_t)(e-1),d))   )
  return 1;

return 0;
}

static void pimples_deleting_one_line(lnhead *line,int16_t cw,int16_t ch,int16_t direct)
//
//  This procedure deletes pimples from one line in work raster
//
{
int16_t      num_row, i, h;
interval *inter;

h=line->h;  i=0;  num_row = line->row;
inter=(interval *)((pchar)line+sizeof(lnhead));
for( ; h ; h--,inter++,i++,num_row++)
  if( (i==0 && (line->flg&l_fbeg))  || (h==1 && (line->flg&l_fend)) )
    {
    if( inter->l>2 )  continue;
    switch( inter->l )
      {
      case 1 :
      if( enable_deleting_pimple(ch,cw,(int16_t)(num_row+1),(int16_t)(inter->e-2),3,direct) &&
          enable_deleting_pimple(ch,cw,(int16_t)(num_row-1),(int16_t)(inter->e-2),3,direct) &&
          !disable_deleting_pimple1(ch,cw,(int16_t)(num_row+1),inter->e,direct)  &&
          !disable_deleting_pimple1(ch,cw,(int16_t)(num_row-1),inter->e,direct) )
          clear_pixel(ch,cw,num_row,(int16_t)(inter->e-1),direct);
        break;
      case 2 :
      if( enable_deleting_pimple(ch,cw,(int16_t)(num_row+1),(int16_t)(inter->e-3),4,direct) &&
          enable_deleting_pimple(ch,cw,(int16_t)(num_row-1),(int16_t)(inter->e-3),4,direct) &&
          !disable_deleting_pimple2(ch,cw,(int16_t)(num_row+1),inter->e,direct)  &&
          !disable_deleting_pimple2(ch,cw,(int16_t)(num_row-1),inter->e,direct) )
          {
          clear_pixel(ch,cw,num_row,(int16_t)(inter->e-1),direct);
          clear_pixel(ch,cw,num_row,(int16_t)(inter->e-2),direct);
          }
        break;
      }
    }

return ;
}


static void pimples_deleting(cell *c,int16_t direct)
//
//  This procedure deletes pimples from image in work raster
//
{
 lnhead   *line;
 int16_t      ll;
 int16_t      cw=direct?((c->w+7)/8):((c->h+7)/8),ch=c->w;

for (line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
    (ll=line->lth)>0; line=(lnhead *)((pchar)line+ll))
        pimples_deleting_one_line(line,cw,ch,direct);

return;
}

static void jumps_deleting_one_line(lnhead *line,int16_t cw,int16_t ch)
//
//  This procedure deletes pimples from one line in work raster
//
{
int16_t      num_row, i, h;
interval *inter;
int16_t      pb,pe, b,e, nb,ne;

h=line->h;
if( h<3 ) return;
i=0;  num_row = line->row;
inter=(interval *)((pchar)line+sizeof(lnhead));

inter++; num_row++;
i++, h-=2; // skip first, last intervals on the line
for( ; h ; h--,inter++,i++,num_row++)
  {
  pe = inter[-1].e; pb = pe - inter[-1].l;
  e  = inter->e;     b = e  - inter->l;
  if( e<pb-1 )

    {  // clear defect of component
    b += pb-1-e;
    inter->e = b + inter->l;
    }
  if( b>pe )
    {  // clear defect of component
    b = pe;
    inter->e = b + inter->l;
    }
  ne = inter[1].e;  nb = ne - inter[1].l;

  if( pe==ne && e==ne+1 && b>pb && b>nb ) // right pimples, left hole
  if( enable_shl(ch,cw,num_row,e) )
    {
    clear_pixel(ch,cw,num_row,(int16_t)(e-1),1);
    set_pixel  (ch,cw,num_row,(int16_t)(b-1),1);
    }
  if( pb==nb && b==nb-1 && e<pe && e<ne ) // right pimples, left hole
  if( enable_shr(ch,cw,num_row,(int16_t)(b-1)) )
    {
    clear_pixel(ch,cw,num_row,b,1);
    set_pixel  (ch,cw,num_row,e,1);
    }
  if( pe==ne && pb==nb )
    {                      // equ intervals
    if( b==nb && e==ne-1 ) // right hole
      set_pixel  (ch,cw,num_row,e,1);
    if( e==ne && b==nb+1 ) // left hole
      set_pixel  (ch,cw,num_row,(int16_t)(b-1),1);
    }
  if( nb==pb && b==nb+1 && ( e-b==ne-nb || e-b==pe-pb ) )
    set_pixel  (ch,cw,num_row,(int16_t)(b-1),1);  // right hole
  if( ne==pe && e==ne-1 && ( e-b==ne-nb || e-b==pe-pb ) )
    set_pixel  (ch,cw,num_row,e,1);// left hole
  }

return ;
}

static void jumps_deleting(cell *c)
//
//  This procedure deletes pimples from image in work raster
//
{
 lnhead   *line;
 int16_t      ll;
 int16_t      cw=((c->w+7)/8),ch=c->w;

for (line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
    (ll=line->lth)>0; line=(lnhead *)((pchar)line+ll))
        jumps_deleting_one_line(line,cw,ch);

return;
}

Bool pimpleshave(cell *C, int16_t shave, int16_t incline)
//
//  This procedure deletes pimples from image in cell *C.
//
 {
 MN   *mn;
 cell *CC,*WW;
 int16_t   bd;
 char df;
 uchar  svf;

 Z=&string;
 svf = (uchar)C->flg;
 // BEFORE SHAVE

 SHAVE_RASTER=save_raster(C);
 SHAVE_HEIGHT=C->h;
 SHAVE_WIDTH =C->w;
 if( incline )
  jumps_deleting(C);
bd=C->bdiff; df = C->difflg & 0xf0;
if( shave )
 {
 pimples_deleting(C,1); // modify SHAVE_RASTER
 mn=c_locomp(SHAVE_RASTER,(int16_t)((C->w+7)/8),C->h,C->r_row,C->r_col);
 if (!mn) return 0;

 WW=create_cell(mn,C,(char)bd, df);
 // FIRST SHAVING

 SHAVE_RASTER=save_raster(WW);
 SHAVE_HEIGHT=WW->h;
 SHAVE_WIDTH =WW->w;
 c_rastror(SHAVE_RASTER,t_raster(),WW->w,WW->h);
			      // turn s_raster for 90 degrees clockwise

 if (WW->h%8 == 0)
  t_left_shift=0;
 else
  t_left_shift=8-WW->h%8;
 t_height=WW->w;
 t_width_b = (WW->h+7)/8;
 mn=c_locomp(t_raster(),t_width_b,t_height,0,(int16_t)(-t_left_shift));
			      // extraction components from t_raster
 if (!mn) {del_cell(WW);return 0;}
 CC=create_cell(mn,WW,(char)bd, df);
 pimples_deleting(CC,0); // modify SHAVE_RASTER
 del_cell(CC);
 mn=c_locomp(SHAVE_RASTER,(int16_t)((WW->w+7)/8),WW->h,WW->r_row,WW->r_col);
 del_cell(WW);
 if (!mn) return 0;
 WW=create_cell(mn,C,(char)bd, df);
 if( C->flg!=c_f_dust && WW->w<MAX_DUST_WIDTH && WW->h<MAX_DUST_HEIGHT )
  return 0;
 // SECOND SHAVING
 }
 else
  { // without SHAVING : correct rotate defectes only
  mn=c_locomp(SHAVE_RASTER,(int16_t)((C->w+7)/8),C->h,C->r_row,C->r_col);
  if (!mn)
    return 0;
  WW=create_cell(mn,C,(char)bd, df);
  if( C->flg!=c_f_dust && WW->w<MAX_DUST_WIDTH && WW->h<MAX_DUST_HEIGHT )
    return 0;
  }
 C->h       = WW->h    ;
 C->w       = WW->w    ;
 C->row     = WW->row  ;
 C->r_row   = WW->r_row;
 C->r_col   = WW->r_col;
 C->col     = WW->col  ;
 C->env     = WW->env  ;
 del_cell(WW);
 // AFTER SHAVE

 return 1;
 }




