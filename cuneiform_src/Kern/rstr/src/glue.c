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


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "struct.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"
#include "minmax.h"

static int16_t n1,n2;
static int16_t h1,h2, h10, h20;
static int16_t y1,y2;
struct int_s *int1;
struct int_s *int2;
static lnhead *lp1;
static lnhead *lp2;
static int pass;
static int16_t gi1l,gi1e,gh1;
static uchar can_glue;
static int16_t gi2l,gi2e,gh2;
static int16_t gs1l,gs1e,sh1;
static int16_t gs2l,gs2e,sh2;
static char fgl1,fgl2;
static int16_t rastur,rastlc;
static int16_t maxh, maxw;
static int16_t c1ur,c1lc;
static int16_t c2ur,c2lc;
static uchar raster[128*64/8];
static int16_t arg_dist;

static void comptorast(c_comp *);
static void glueline();
static void gluetorast();
static void inttorast(int16_t,int16_t,int16_t);
static int16_t glueable();

MN *glue(c_comp **cp, int16_t flag_dist)
//
//	This procedure glues a set of components. It takes each pair of
//	components and put first one into raster with additional intervals
//	that are "glue" to paste components. The maximum distance between
//	intervals to be "glueable" is 3 in vertical and horizontal directions.
//
//	ENTER: List of addresses of components.
//	EXIT : Result of component extraction from extended (by glue) joint
//	       raster.
//
//	Structure of component on entry:
//	   unsigned char size	 c_comp size in paragraphs
//	   int upper;		 upper bound (absolute)
//	   int left;		 left bound (absolute)
//	   int h;		 c_comp height
//	   int w;		 c_comp width
//	   char dummy[8];	 unneeded information
//	   int lines;		 segment of lines of this component
//	   int nl;		 number of lines
//	   unsigned char begs;	 number of free beginnings
//	   unsigned char ends	 number of free ends
//
//	A word before all lines is total length.
//	A word after all lines is 0.
//	Lines go just one after another.
//	Line begins with header and finishes with 0 byte.
//	Structure of lines on entry:
//	  Header:
//	   int lth;		 length of this line presentation
//	   int h;		 line height
//	   int row;		 ralative row of line beginning
//	  Intervals:
//	   unsigned char l;	 length of interval (in pixels)
//	   unsigned char e;	 end of interval + 1
//
 {
 int16_t i,j;
 c_comp *cp1;
 c_comp *cp2;
 int16_t Lc1,lc1,Lc2,lc2;

 Z=&string;
 i=0;
 rastur=rastlc=10000;
 arg_dist = flag_dist;
 maxh=maxw=0;
 while ((cp1=cp[i]) != NULL)           // calculation of (absolute)
  {			               //       upper and left bounds
  if (cp1->left < rastlc)
   rastlc=cp1->left;
  if (maxw < (j=cp1->left+cp1->w)) maxw=j;
  if (maxh < (j=cp1->upper+cp1->h)) maxh=j;
  if (cp1->upper < rastur)
   rastur=cp1->upper;
  i++;
  }

 maxw -= rastlc; maxh -= rastur;
 if ((maxw > RASTER_MAX_WIDTH) || (maxh > RASTER_MAX_HEIGHT))
   return NULL;
 pass=1;
 can_glue=0;
 again:
 memset (raster,0,sizeof(raster));

 i=0;
 gi1l=0;
 gi2l=0;
 while ((cp1=cp[i]) != NULL)
 {
   c1ur=cp1->upper;
   c1lc=cp1->left;
   comptorast(cp1);
   j=i+1;
   while ((cp2=cp[j]) != NULL)
   {
     c2ur=cp2->upper;
     c2lc=cp2->left;
     if (c2lc > c1lc+cp1->w+4) goto fin; // too far
     if (c2lc+cp2->w+4 < c1lc) goto fin; // too far
     if (c2ur > c1ur+cp1->h+3) goto fin; // too far
     if (c2ur+cp2->h+3 < c1ur) goto fin; // too far
     Lc1=cp1->nl;                    // number of lines in component
     lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
     for (lc1=0; lc1 < Lc1; lc1++)
     {
       Lc2=cp2->nl;                // number of lines in component
       lp2=(lnhead *) ( (char *)cp2 + cp2->lines + 2); // beginning of first line
       for (lc2=0; lc2 < Lc2; lc2++)
       {
          glueline();                    // glue, if possible
          lp2=(lnhead *) ((char *)lp2+lp2->lth);   // next line
       }
       lp1=(lnhead *) ((char *)lp1+lp1->lth);   // next line
     }
fin:
     j++;
   } // WHILE cp2
   i++;
 }   // WHILE cp1
/****  testcnt++; if ((testcnt & 7)==0) return NULL; ***/
 if ((flag_dist) && (pass==1))
 {
   if (can_glue)   // was glued
     return(c_locomp(raster,128/8,64,rastur,rastlc));
   else
     { pass=2; goto again; }
 }
  return(c_locomp(raster,128/8,64,rastur,rastlc));
 }

static void comptorast(c_comp *cp1)
{
 int16_t Lc1,lc1;

 Lc1=cp1->nl;			       // number of lines in component
 lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
 for (lc1=0; lc1 < Lc1; lc1++)
 {
   h1=lp1->row+c1ur-rastur;
   int1=(interval *)(lp1+1);       // ptr to current interval
   for (y1=0; y1 < lp1->h; y1++, int1++, h1++)
     inttorast(h1,(int16_t)(int1->e+c1lc-rastlc),int1->l);
   lp1=(lnhead *) ((char *)lp1+lp1->lth);   // next line
 }
}

static void glueline()
 {
 int16_t a,b,A,B,wa,wb,wA,wB;
 struct int_s *wint1;
 struct int_s *wint2;

 n1=lp1->h-1;
 h10=h1=lp1->row+c1ur-rastur;
 y1=0;
 fgl1=0;

 n2=lp2->h-1;
 h20=h2=lp2->row+c2ur-rastur;
 y2=0;
 fgl2=0;

 gs1l=gs2l=0;		               // nothing in latch
 int1=(struct int_s *)(lp1+1); 	       // ptr to first interval in line
 int2=(struct int_s *)(lp2+1); 	       // ptr to 2nd   interval in line
 compl:
 //if ((y1 == n1) && (y2 == n2))
 //  goto end;
 glubl:
 if (glueable())
 {
   // take to latches
   can_glue = 1;
   gs1e=gi1e; gs1l=gi1l; sh1=gh1; fgl1=1;
   gs2e=gi2e; gs2l=gi2l; sh2=gh2; fgl2=1;
   if (h1 == h2)
   {
     gluetorast();
     goto shiftboth;
   }
 }
 else
   gluetorast();   // glue from latches
 if ((y1 == n1) && (y2 == n2))
   goto end;
 // shift 1st or 2nd or both
 if (h1 < h2)
 {                 /* left line's interval is upper than right one's */
   if (y1 < n1)
   {
     gosh1:
     h1++; y1++; int1++;
     fgl1=0;
     goto compl;
   }
   if (fgl1)
     goto end;
   if (y2 < n2)
   {
     h2++; y2++; int2++;
     fgl2=0;
   }
   goto compl;
 }
 if (h2 < h1)                          // left line's interval is upper
 {                                //     than right one's
   if (y2 < n2)
   {
     gosh2:
     h2++; y2++; int2++;
     fgl2=0;
     goto compl;
   }
   if (fgl2)
     goto end;
   if (y1 < n1)
   {
     h1++; y1++; int1++;
     fgl1=0;
   }
   goto compl;
 }
				       // h1 == h2
shiftboth:
 if ((y1 == n1) && (y2 == n2))
   goto end;
 if ((y1 < n1) && (y2 < n2))           // may shift both
 {
   b=int1->e + c1lc - rastlc - 1;
   a=b - int1->l +1;
   B=int2->e + c2lc - rastlc - 1;
   A=B - int2->l +1;
   wint1 = int1+1;
   wint2 = int2+1;
   wb=wint1->e + c1lc - rastlc - 1;
   wa=wb - wint1->l +1;
   wB=wint2->e + c2lc - rastlc - 1;
   wA=wB - wint2->l +1;
   if (b < B)                           // 1-st then 2-nd
   {
     if (wb > b)                         // 1-st will be closer
       goto gosh1;
     if (wA < A)                         // 2-nd will be closer
       goto gosh2;
   }
   else
   {
     if (wB > B)                         // 2-st will be closer
       goto gosh2;
     if (wa < a)
       goto gosh1;
   }
 }
 if (y1 < n1)                          // left could be shifted
 {
   h1++; y1++; int1++;
   fgl1=0;
 }
 if (fgl1)
   goto end;
 if (y2 < n2)                          // right could be shifted
 {
   h2++; y2++; int2++;
   fgl2=0;
 }
 if (fgl2)
   goto end;
 goto glubl;
end:
 gluetorast();
}

static void gluetorast()
{

 if (gs1l)
 {
   inttorast(sh1,gs1e,gs1l);
   gs1l=0;
 }
 if (gs2l)
 {
   inttorast(sh2,gs2e,gs2l);
   gs2l=0;
 }
}

static void inttorast( int16_t h, int16_t end, int16_t lth)
 {
 int16_t j,je,me,ib,jb,mb;

 ib=end-lth;
 mb = 0xff >> (ib & 7);
 jb = (h << 4) + (ib>>3);
 me = 0xff << (8-(end & 7));
 je = (h << 4) + (end >> 3);
 if (jb == je)
  {
  raster[jb] |= (mb & me);
  return;
  }
 raster[jb] |= mb;
 raster[je] |= me;
 for (j=jb+1; j < je; j++)
  raster[j] = 0xff;
 }

static int16_t glueable()
//
//	This procedure produces glue to paste two intervals *int1 and *int2.
//
 {
 int16_t a,b,A,B;	                       // [A,B] is lower than [a,b]
 int16_t lh;
 int16_t dh;

 gi1l=gi2l=0;
 dh=h1-h2;
 if (dh < 0)
  {
  b=int1->e + c1lc - rastlc - 1;
  a=b - int1->l +1;
  B=int2->e + c2lc - rastlc - 1;
  A=B - int2->l +1;
  lh=h2;
  dh=-dh;
  }
 else
  {
  B=int1->e + c1lc - rastlc - 1;
  A=B - int1->l + 1;
  b=int2->e + c2lc - rastlc - 1;
  a=b - int2->l +1;
  lh=h1;
  }

 switch(dh)                            // dh - distance (in rows)
  {			               //    between two intervals
  case 3:
   if (pass==2) goto ret03;
   gh1=lh-1;
   gh2=lh-2;
   gi1l=1;
   gi2l=1;
   if (b == A-1) { gi1e=A+1; gi2e=A;   return(1);}
   if (b == A)   { gi1e=A+1; gi2e=A+1; return(1);}
   if (a == B+1) { gi1e=B+1; gi2e=B+2; return(1);}
   if (a == B)   { gi1e=B+1; gi2e=B+1; return(1);}
   if ((b > A) && (a < B))
    {
    gi1e=gi2e=MIN(b,B)+1;
    gi1l=gi2l=gi1e-MAX(a,A);
    return(1);
    }
   ret03:
   gh1=0;
   gh2=0;
   return(0);
  case 2:
   if (pass==2) goto ret02;
   gh1=lh-1;
   gi1l=1;
   if (b == A-2) { gi1e=A;             return(1);}
   if (b == A-1) { gi1e=A+1; gi1l=2;   return(1);}
   if (b == A)   { gi1e=A+1;           return(1);}
   if (a == B+2) { gi1e=B+2;           return(1);}
   if (a == B+1) { gi1e=B+2; gi1l=2;   return(1);}
   if (a == B)   { gi1e=B+1;           return(1);}
   if ((b > A) && (a < B))
    {
    gi1e=MIN(b,B)+1;
    gi1l=gi1e-MAX(a,A);
    return(1);
    }
   ret02:
   gh1=0;
   gh2=0;
   return(0);
  case 1:
   gh1=lh-1;
   gh2=lh;
   gi1l=1;
   if ((pass==2) && ((h1==0) || (h2==0) || (h1==maxh) || (h2==maxh)))
   {
     if (arg_dist == 2)
     { int16_t wm;
       if ((n1 < (wm=maxh-2)) || (n2 < wm))  // not a full height fragments
         goto ret01;
     }
     if (b == A-5) { gi1e=A-2; gi2e=A;   gi1l=2; gi2l=2; return(1);}
     if (b == A-4) { gi1e=A-2; gi2e=A;   gi1l=1; gi2l=2; return(1);}
     if (a == B+4) { gi1e=B+4; gi2e=B+3; gi1l=1; gi2l=2; return(1);}
     if (a == B+5) { gi1e=B+5; gi2e=B+3; gi1l=2; gi2l=2; return(1);}
     goto ret01;
   }
   if (b == A-3) { gi1e=A-1; gi2e=A;   gi2l=1; return(1);}
   if (b == A-2) { gi1e=A;                     return(1);}
   if (a == B+2) { gi1e=B+2;                   return(1);}
   if (a == B+3) { gi1e=B+3; gi2e=B+2; gi2l=1; return(1);}
   ret01:
   gh1=0;
   gh2=0;
   return(0);
  case 0:
   gh1=lh;
   if ((pass==2) && ((h1==0) || (h1==maxh)))
   {
     if (arg_dist == 2)
     { int16_t wm;
       if ((n1 < (wm=maxh-2)) || (n2 < wm))  // not a full height fragments
         goto ret01;
     }
     if (b == A-5) { gi1e=A-2; gi2e=A;   gi1l=2; gi2l=2; return(1);}
     if (b == A-4) { gi1e=A-2; gi2e=A;   gi1l=1; gi2l=2; return(1);}
     if (a == B+4) { gi1e=B+4; gi2e=B+3; gi1l=1; gi2l=2; return(1);}
     if (a == B+5) { gi1e=B+5; gi2e=B+3; gi1l=2; gi2l=2; return(1);}
     goto ret00;
   }
   if (b == A-3) { gi1e=A;   gi1l=2;   return(1);}
   if (b == A-2) { gi1e=A;   gi1l=1;   return(1);}
   if (a == B+2) { gi1e=B+2; gi1l=1;   return(1);}
   if (a == B+3) { gi1e=B+3; gi1l=2;   return(1);}
   ret00:
   gh1=0;
   gh2=0;
   return(0);
  default :
   return(0);	                       // nonglueable
  }
 }
