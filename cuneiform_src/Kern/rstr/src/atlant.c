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
/*     module CARIATIDS - upright legs configuration checkout        */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"
#include "ligas.h"

#include "compat_defs.h"
#include "minmax.h"

#define zone_err(i,j) { des|=i; mn_err=j; goto mn_error; }
// 930727               des=i;  (versions 'n' 'u' make different bits)
#define pen_roof_upright 100
#define pen_roof_itun 32
#define pen_roof_itm  64
extern uchar db_trace_flag;  // 2 - more detailed estimate (ALT-F7)
extern char db_status;
extern char db_pass;
static int fl_sort_vers;
static char leg_midw, leg_uneven, leg_maxpos, leg_minpos, leg_botw, pen_ne,
            unev1, unev2, unev3, unev12, unev23,
            botw1, botw2, botw3, botw12, botw23,
            sumw1, sumw2, sumw3, sumw12, sumw23;
static int16_t leg_sumw;
static char mn_err, des, mhh, shh, ehh, hhh;
static uchar at_sum[64];
static uchar at_sum[64];
static char at_end[3][64], at_lth[3][64];
static char at_dif0[64], at_dif1[64], at_dif2[64];
static char at_roof[128];

static uchar pens_leg[32] = {
100,
100,64,100,
100,64,100,
100,64,100,
100,64,
100,64,
64,64,64,
100,100,
36
};
static uchar pen_ne_12[] = {0,12,36,64,96,100,128,160 };
static char *legs_err[]={
 "ok",
 "1st bad", "1st uneven", "1st like \\",
 "2nd bad", "2nd uneven", "2nd like \\",
 "3rd bad", "3rd uneven", "3rd like \\",
 "1-2 bad", "1-2 uneven",
 "2-3 bad", "2-3 uneven",
 "1 ne 2","1 ne 3","2 ne 3",
 "1st far","last far",
 "1-2 # 2-3"
};

static char roof_incompl[]={"incomplete roof/floor"};
static char roof_hole[]={"gap not in place"};

void est_snap(char user,cell *C,pchar txt);
static uchar pen_roofm[8]={0,2,4,12,24,36,48,100};
static uchar pen_roofn[8]={0,2,4,12,24,36,48,100};
static uchar pen_iroofn[8]={0,0,0,0,12,36,48,100};
static uchar pen_floor[8]={0,0,0,0,12,36,48,100};
static uchar pen_ifloor[8]={0,2,4,12,24,36,48,100};
static uchar ital_un, cum_ital;
static int16_t analeg(pchar);
void del_vers(cell *c, uchar x, uchar pen);

void atlant(cell *AA, s_glue *GL, int16_t flag, int16_t flag_m)
//
//	This procedure finds all uprigth legs of a component.
//	ON ENTRY: cell in question
//	ON EXIT : component most preferable type (0, 1, n, m) -
//		  leg number;
//
 {
 cell *A, *WC;
 char *at, *rp, ri;
 int16_t  df0, df1, df2, wdf, awdf;
 char *lp0=NULL, *lp1=NULL, *lp2=NULL, *e0=NULL, *e1=NULL, *e2=NULL;
 char bw0, bw1, bw2, un0, un1, un2, sm0, sm1, sm2,
      l0, l1, l2, l3, l4, l5;
 char la0, la1, la2;
 char beg0[3], end0[3];
 char pen_roof, pen_leg, eleg, nleg, nlegm, fleg;
 char R01, R02, H0, wt1, wt2, hhmin, hhmax,
      hhi1, hhi2, hh1, hh2, hhw, hzone, hbase, thmdle,
      nok, uok, hw, h1, h2, hc, hclow, hchigh,
      hm, mr1, mr2, mrw1, mrw2, fl_short, fl_shln;
 int16_t nc, mcol, mrow, Mrow;
 char vers;
 lnhead *Lp1, *Lp2;
 char     n1, nw1, y1;
 char   *botp, bott[128],
	*rofp, roof[128],
     minroof, minrx, minr1, minr2, minr3, minr4,
     roofl, roofr, roofd, rooftl, rooftr,
     botl, bottl, botd;
 int16_t legd, rowd, i, j;
 uchar  sm_reason, flsort;
 int16_t Lc1,lc1;
 interval *int1;
 c_comp *cp1;
/** static int16_t testi(cell *,char); **/

 Z=&string;
 discrim_un( AA, GL, flag_m );
 mcol=32000; mrow = 32000; Mrow=-32000;
 memset(at_sum,0,64);
 nok = uok = 0;
 cum_ital = 0;
 sm_reason =  0;
 flsort=(uchar)flag;     /*********** "flag"  and "flsort" NOT USED   ************/
 fl_sort_vers=0;
 vers=0;
 nc=0;
 while (1)
 {
   A=GL->celist[nc++];
   if (!A) break;
   if (mcol > A->col) mcol=A->col;
   if (mrow > A->row) mrow=A->row;
   if (Mrow < (A->row+A->h)) Mrow=A->row+A->h;
 }

  hc=Mrow-mrow;
  hclow = hc / 3;
  hchigh = hc - hclow;  // (hclow -- hchigh) === middle of cell
  vers=0;
  nleg=nlegm=-1;
  hh1=0; hh2=0;
  for (ri=0, rp=at_roof; ri<64; ri++,rp++) *rp=0;
  nc=0;
  while (1)
  {
    WC= GL->celist[nc++];
    if (!WC) break;
    rowd=WC->row - mrow;
    cp1 = WC->env;
    Lc1=cp1->nl;	    // number of lines in component
    Lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
    Lp2=Lp1;
    for (lc1=0; lc1 < Lc1; lc1++)
    { int16_t hl;
      if ((hl=Lp1->h) <= 3)  goto nextl1;   // ignore short lines
      hm = Lp1->row + rowd;
      if ((Lp1->flg & l_fend) && (2*(hm+Lp1->h) <= WC->h)) goto nextl1;
      // ignore free end line ending in upper half of the cell
      rp=&at_roof[hm];
      for (hw=0; hw < Lp1->h; hw++,rp++) *rp+=1;
      int1=(interval *)(Lp1+1);       // ptr to current interval
      for (i=0, at=&at_sum[hm]; i < Lp1->h; i++, at++, int1++)
        *at += int1->l;
      if (hh2 < hw)    hh2 = hw;

    nextl1:
      Lp2=Lp1=(lnhead *) ((char *)Lp2+Lp2->lth);   // next line
    }                                            // for all lines
  }                                             // while all cells

  hh1=hh2=H0=hm=hh2>>1;
  eleg=at_roof[hm];
  if ((flag_m & 1) && (eleg != 3))
    { del_vers(AA,1,100); flag_m -=1; }

  if ((flag_m & 2) && (eleg != 2))
    { del_vers(AA,2,100); flag_m -=2; }

  if ((flag_m & 4) && (eleg != 2))
    { del_vers(AA,4,100); flag_m -=4; }

  if (flag_m == 0) goto retsort;
  i=at_sum[H0];
  wt1=(char)i;   // to be replaced by medium
  j=0;
 again_h1h2:
  hhi1=hh1; hhi2=hh2;
  fleg=0;
  hhw = hh2;
  while (1)
   {
    if (at_roof[hh2] != eleg)    { hh2--; break; }
    if (abs(at_sum[hh2]-i) <= 3) { hhw=hh2; hh2++; continue; }
    if (fleg) { hh2=hhw; break; }
    fleg=1;
    hh2++;
   }
  fleg=0;
  hhw = hh1;
  while (1)
  {
    if (at_roof[hh1] != eleg)   { hh1++; break; }
    if (abs(at_sum[hh1]-i) < 3) { hhw=hh1; hh1--; continue; }
    if (fleg) { hh1=hhw; break; }
    fleg=1;
    hh1--;
  }
  if (j)
    goto h1h2_done;
  // try to get medium value
  i=0;
  for (j=hh1; j<hh2; j++)
    i += at_sum[j];
  j=hh2-hh1;
  if (j==0) goto h1h2_done;
  i = (i+j-1)/j;
  wt2 = (char)i;
  j=1;
  goto again_h1h2;
 h1h2_done:
  h1=(hc * 80 ) >> 8;   // 80/256 ===  1/3  interval for legs width test
  h2=(hc * 176) >> 8;   // 176/256 === 2/3
  // find MIN(leg beg)  MAX(leg end) for legs crossing  (h1, h2)
  nc=0;
  hhmin = 0; hhmax = 127;
  while (1)
  { int16_t  hw1, hw2;
    WC= GL->celist[nc++];
    if (!WC) break;
    rowd=WC->row - mrow;
    cp1 = WC->env;
    Lc1=cp1->nl;	    // number of lines in component
    Lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
    Lp2=Lp1;
    for (lc1=0; lc1 < Lc1; lc1++)
     {
      if (Lp1->h > 3)       // ignore short lines
      {
        hw1 = Lp1->row + rowd;
        hw2 = hw1 + Lp1->h-1;
        // ignore free end line ending in upper half of the cell
        if (((Lp1->flg & l_fend) == 0) || (2*(hw2) > WC->h))
        {
          if ((hw1 <= h1) && (hw2 >= h2))  // interesting line (leg) obtained
          {
            if (hw1 > hhmin)  hhmin = (char)hw1;
            if (hw2 < hhmax)  hhmax = (char)hw2;
          }
        }
      }
      Lp2=Lp1=(lnhead *) ((char *)Lp2+Lp2->lth);   // next line
     }                                            // for all lines
   }                                             // while all cells

  if (hh1 < hhmin) hh1=hhmin;
  if (hh2 > hhmax) hh2=hhmax;
  if (db_status && (db_trace_flag & 2))
  { char ww[80];
    sprintf(ww,"leg zone: 1/3: (%d-%d); %d: (%d-%d) %d: (%d-%d) (%d-%d)",
                                h1,h2, wt1, hhi1, hhi2,
                                                    wt2, hh1,hh2, hhmin, hhmax);
    est_snap(db_pass,AA,ww);
  }
  thmdle=(hh1+hh2)>>1;
  R01=thmdle-1; if (R01 < hh1) R01=hh1;
  R02=thmdle+1; if (R02 > hh2) R02=hh2;
  hzone=hh2-hh1;
  hbase=hh1;
  thmdle=hzone/2;
  if ((hzone*3  < hc) && (hzone < 3))         // too small 3-leg zone
  {
   if (db_status && (db_trace_flag & 2))
     est_snap(db_pass,AA,"small leg zone");
    del_vers(AA,7,100);
    goto retsort;
  }
 fl_short=fl_shln=0;  // short lines out of interest, not obtained yet
get_lines:
 nc=0;
 while(1)
 {
   WC=GL->celist[nc++];
   if (!WC) break;
   legd = WC->col - mcol;
   rowd=WC->row - mrow;
   cp1=WC->env;
   Lc1=cp1->nl;			        // number of lines in component
   Lp1=(lnhead *)( (char *)cp1 + cp1->lines + 2); // beginning of first line
   Lp2=Lp1;
   for (lc1=0; lc1 < Lc1; lc1++)
   {
     n1=Lp1->h-1;
     h1=Lp1->row+rowd;
     h2=h1+n1;
     if ( (Lp1->flg  & (l_fbeg|l_fend)) == 0)
     {
       if (n1 < 4)
         fl_shln=1;  // short nonfree line obtained
       else
       {
        //// 93.01.26 long nonfree leg begins or ends outside test zone
        if ((h1 >= R02) || (h2 <= R01))
        {
          est_snap(db_pass,AA,"nonfree leg");
          del_vers(AA,7,100);
          goto retsort;
        }
      //// 93.01.26 ------- end of correction
       }
     }
     if ((h1 <= R01) && (h2 >= R02))  // interesting line (leg) obtained
     {
       if (fl_short) goto skip;  // short lines of interest ?
       nlegm++;
       if (nlegm > 2)
         { del_vers(AA,7,100); goto retsort; }
       if ((h1 > hh1) || (h2 < hh2))  /* not a full leg */
       {
         if (fl_short == 0)
           goto skip;
         goto try_short;
       }
       nleg++;
       int1=(interval *)(Lp1+1);      // ptr to current interval
       int1 += (hh1-h1);     // 1-st interval of the part crossing middle zone
       for (hw=0; hw < hzone; hw++, int1++)
       {
         if (hw==thmdle)
         {
           beg0[nleg]=int1->e-int1->l+legd;   // leg position
           end0[nleg]=int1->e+legd;
         }
         at_lth[nleg][hw]=int1->l;
         at_end[nleg][hw]=int1->e+legd;
       }
     }    // if interesting line
     else
       if (fl_short)
       {
         int16_t cleg;
     try_short:
         if ( (Lp1->flg  & (l_fbeg|l_fend)) != 0)
           goto skip;   // non_free begs and ends of interest
         if ((h1 >= hh2) || (h2 < hh1))
           goto skip;   // leg not in studied zone
         int1=(interval *)(Lp1+1);      // ptr to current interval
         for (hw=h1; hw <= h2; hw++, int1++)
         {
           if (hw < hh1) continue;
           if (hw > hh2) break;
           for (cleg = 0; cleg < nleg; cleg++)
           { // test if short leg amends any found "full" leg
             int16_t cbeg, cend, clth, ch, nbeg, nend, nlth;
             ch = hw-hh1;
             cend = at_end[cleg][ch];
             clth = at_lth[cleg][ch];
             cbeg = cend-clth;
             nlth = int1->l;
             nend = int1->e+legd;
             nbeg = nend-nlth;
             if (nbeg > cend) // short part at right side of full leg
             {
               if ((nbeg-cend) < 2)
               {
                 at_end[cleg][ch]=(char)nend;
                 at_lth[cleg][ch]=(char)(nend-cbeg);
                 goto next_short_int;
               }
             }
             if (nend < cbeg)
             {
               if ((cbeg-nend) < 2)
               {
                 at_lth[cleg][ch]=cend-nbeg;
                 goto next_short_int;
               }
             }
           } // FOR all large legs
         next_short_int:
           continue;
         }   // for intervals of hort line
       }     // IF fl_short
skip:
     Lp2=Lp1=(lnhead *)((char *)Lp2+Lp2->lth);  // next line
   }   // for all lines
 }     // while all cells

 if (fl_short ==0)
 { nleg++;
   if ((flag_m & 1) && (nleg != 3))
     { del_vers(AA,1,100); flag_m -=1; }

   if ((flag_m & 2) && (nleg != 2))
     { del_vers(AA,2,100); flag_m -=2; }

   if (flag_m & 4)
     { if ((nleg != 2) || ((nc==1) && (hh1*3 > AA->h)))
        { del_vers(AA,4,100); flag_m -=4; }
 //    { if ((nleg != 2) || (hh1*3 > AA->h))
 // 10/24/92 must require 1/3: broken 'a' often goes as 'u'
 // check for simlar incline needed
     }
   fl_short=1;   // short lines to be "glued" to previously got lines
                 // -- thus to recover small gaps in pattern
   if (fl_shln)  // short lines in cell ?
     goto get_lines;
 }

 if (!flag_m) goto retsort;    // nothing more to test

// now only one version to be tested --  'n' or 'm' (2 or 3 legs)
 shh=0;
 ehh=hhh=hzone;
 goto mn_again;
mn_error:
if (db_status && (db_trace_flag & 2))
{ char ww[80];
  sprintf(ww," %s %d %d ",legs_err[mn_err],shh+hbase, ehh+hbase);
  est_snap(db_pass,AA,ww);
}
 if (des & 4) goto del_all;
 if (des & 1) { shh++; hhh--; }
 if (des & 2) { ehh--; hhh--; }
mn_again:
 if (hhh < 4)
 {
   if (db_status && (db_trace_flag & 2))
     est_snap(db_pass,AA,"too small leg zone");
 del_all:
   if (mn_err >= 32) mn_err=0;
   pen_leg = pens_leg[mn_err];

   if (mn_err == 19)
   { if (pen_ne > 7) pen_ne = 7;
     pen_leg = pen_ne_12[pen_ne];
   }
   if (nleg==2)
   {
     if (nok==0) del_vers(AA,2,pen_leg);
     if (uok==0) del_vers(AA,4,pen_leg);
   }
   else         del_vers(AA,1,pen_leg);
   goto retsort;
 }
 des = 0;
 mn_err = 0;
 ital_un = 0;
 pen_leg = 100;
 pen_ne = 0;
 l1 = la0 = (char)analeg(at_lth[0]);
 un0 = leg_uneven;
 bw0 = leg_botw;
 sm0 = (char)leg_sumw;

 // before 10/25:  leg_botw   = leg_maxpos - leg_minpos;
 //                    uneven = max - min;
 // after  10/25:      botw   = last + (last-1) - first - (first+1)
 // if (leg_uneven <= 2)  return leg_midw;
 // if ((leg_botw > 0) && fl) return (-leg_midw);  // wider at bottom

 l3=la1 = (char)analeg(at_lth[1]);
 un1 = leg_uneven;
 bw1 = leg_botw;
 sm1 = (char)leg_sumw;

 if (nleg==2)                 // 2-leg versions
 {
   if (end0[0]>end0[1])
   {
     unev1 = un1; unev2 = un0;
     botw1 = bw1; botw2 = bw0;
     sumw1 = sm1; sumw2 = sm0;
     l3=la0;       l1=la1;
     e0=at_end[1];  e1=at_end[0];
     lp0=at_lth[1]; lp1=at_lth[0];
   }
   else
   {
     unev1 = un0; unev2 = un1;
     botw1 = bw0; botw2 = bw1;
     sumw1 = sm0; sumw2 = sm1;
     l3=la1; l1=la0;
     e0=at_end[0]; e1=at_end[1];
     lp0=at_lth[0]; lp1=at_lth[1];
   }
   if ( (e1[ehh-1] - lp1[ehh-1] - e1[shh] +lp1[shh] >= 2) &&
        (e1[ehh-1] - e1[shh]  >= 2)
      )
     zone_err(3,6)       // backslash-like right leg
   if ( (e0[ehh-1] - lp0[ehh-1] - e0[shh] +lp0[shh] >= 2) &&
        (e0[ehh-1] - e0[shh]  >= 2)
      )
     zone_err(3,3)       // backslash-like left leg
   for (i=0; i< hzone; i++)
     at_dif1[i]=e1[i]-e0[i]-lp1[i];
   l2=(char)analeg(at_dif1);
   unev12 = leg_uneven;
   botw12 = leg_botw;
   sumw12 = (char)leg_sumw;

   if (AA->w - e1[thmdle] > l2 + 2)        // 2nd leg far from right end
     zone_err(3,18)
   if (e0[thmdle]-lp0[thmdle] > l2+2)      // 1st far from left end
     zone_err(3,17)

   ital_un = 0;
   mhh = (shh+ehh)/2;
   { int16_t d1, d2, d3, d12, d23;
     d3 = e0[shh];
     d2 = e0[mhh];
     d1 = e0[ehh-1];
     d12 = d2-d1;
     d23 = d3-d2;
     if ((d12*d23 >= 0) && (d12+d23 > 1))   // right incline
       ital_un = 1;
     cum_ital |= ital_un;
   }
   des = 0;
   if (ital_un)    // italic legs
   {
     if ((flag_m & 2) && (nok==0))       // 'n'
     {
       if (db_status && (db_trace_flag & 2))
         est_snap(db_pass,AA,"italic n");
       if (botw1 > 2)        // 1st leg  wider at bottom
         { des=2; mn_err=1; goto u1_test; }
         //zone_err(2,1)
       if (botw12 < -2)       // interleg wider at top
         { des=1; mn_err=10; goto u1_test; }
         //zone_err(1,10)
       if (unev2 > 2)
         { des=3; mn_err=5; goto u1_test; }
         //zone_err(3,5)      // 2nd uneven
       if (unev1 > 3)
         { des=3; mn_err=2; goto u1_test; }
         //zone_err(3,2)      // 1st uneven
       if ((l3-l1 > 2) || (l1-l3 > 4))
         { des=3; mn_err=14; goto u1_test; }
         //zone_err(3,14)  // unequal width
     }
     if (flag_m & 2) nok = 1;
     u1_test:
     if ((flag_m & 4) && (uok == 0))  // 'u'
     {
       if (db_status && (db_trace_flag & 2))
          est_snap(db_pass,AA,"italic u");
       if (botw2 < -2)       // 2nd leg  wider at top
         zone_err(1,4)
       if (botw12 > 2)       // interleg wider at bottom
         zone_err(2,10)
       if (unev1 > 3)
         zone_err(3,2)      // 1st uneven
       if (unev2 > 2)
         zone_err(3,5)      // 2nd uneven
       if ((l3-l1 > 4) || (l1-l3 > 2))
         zone_err(3,14)     // unequal width
     }
     if (flag_m & 4) uok = 1;
     if (des) goto mn_error;
   }
   else    // NON_ITALIC legs
   {
     if (db_status && (db_trace_flag & 2))
        est_snap(db_pass,AA,"upright u,n");
     if (unev12 > 2)       // interleg uneven
       zone_err(3,11)
     if (unev1 > 2)
       zone_err(3,2)      // 1st uneven
     if (unev2 > 2)
       zone_err(3,5)      // 2nd uneven
     if ((l3-l1 > 2) || (l1-l3 > 2))
       zone_err(3,14)     // unequal width
   }
 }
 else  // 'm'
 {
  // nleg = 3  // 'm' version only

  l5=la2=(char)analeg(at_lth[2]);
  bw2 = leg_botw;
  un2 = leg_uneven;
  sm2 = (char)leg_sumw;

  // sort 3 legs in left-to-right order
  if ((end0[0]<end0[1]) && (end0[1]<end0[2]))
  {
    e0=at_end[0];  e1=at_end[1];  e2=at_end[2];
    lp0=at_lth[0]; lp1=at_lth[1]; lp2=at_lth[2];
    unev1 = un0;   unev2 = un1;   unev3 = un2;
    botw1 = bw0;   botw2 = bw1;   botw3 = bw2;
    sumw1 = sm0;   sumw2 = sm1;   sumw3 = sm2;
    goto wrk3;
  }

  if ((end0[0]<end0[2]) && (end0[2]<end0[1]))
  {
    l1=la0; l3=la2; l5=la1;
    e0=at_end[0];  e1=at_end[2];  e2=at_end[1];
    lp0=at_lth[0]; lp1=at_lth[2]; lp2=at_lth[1];
    unev1 = un0;   unev2 = un2;   unev3 = un1;
    botw1 = bw0;   botw2 = bw2;   botw3 = bw1;
    sumw1 = sm0;   sumw2 = sm2;   sumw3 = sm1;
    goto wrk3;
  }

  if ((end0[1]<end0[2]) && (end0[2]<end0[0]))
  {
    l1=la1; l3=la2; l5=la0;
    e0=at_end[1]; e1=at_end[2]; e2=at_end[0];
    lp0=at_lth[1]; lp1=at_lth[2];  lp2=at_lth[0];
    unev1 = un1;   unev2 = un2;   unev3 = un0;
    botw1 = bw1;   botw2 = bw2;   botw3 = bw0;
    sumw1 = sm1;   sumw2 = sm2;   sumw3 = sm0;
    goto wrk3;
  }

  if ((end0[1]<end0[0]) && (end0[0]<end0[2]))
  {
    l1=la1; l3=la0; l5=la2;
    e0=at_end[1]; e1=at_end[0]; e2=at_end[2];
    lp0=at_lth[1]; lp1=at_lth[0];   lp2=at_lth[2];
    unev1 = un1;   unev2 = un0;   unev3 = un2;
    botw1 = bw1;   botw2 = bw0;   botw3 = bw2;
    sumw1 = sm1;   sumw2 = sm0;   sumw3 = sm2;
    goto wrk3;
  }

  if ((end0[2]<end0[0]) && (end0[0]<end0[1]))
  {
    l1=la2; l3=la0; l5=la1;
    e0=at_end[2]; e1=at_end[0]; e2=at_end[1];
    lp0=at_lth[2]; lp1=at_lth[0];  lp2=at_lth[1];
    unev1 = un2;   unev2 = un0;   unev3 = un1;
    botw1 = bw2;   botw2 = bw0;   botw3 = bw1;
    sumw1 = sm2;   sumw2 = sm0;   sumw3 = sm1;
    goto wrk3;
  }

  if ((end0[2]<end0[1]) && (end0[1]<end0[0]))
  {
    l1=la2; l3=la1; l5=la0;
    e0=at_end[2]; e1=at_end[1]; e2=at_end[0];
    lp0=at_lth[2]; lp1=at_lth[1];  lp2=at_lth[0];
    unev1 = un2;   unev2 = un1;   unev3 = un0;
    botw1 = bw2;   botw2 = bw1;   botw3 = bw0;
    sumw1 = sm2;   sumw2 = sm1;   sumw3 = sm0;
    goto wrk3;
  }

wrk3:
  df0=df1=df2=0;
  if( !e0 && !e1 && !e2 && !lp0 && !lp1 && !lp2 )   // Oleg : can be nonzero
    goto retsort;
  for (i=shh; i< ehh; i++)
  {
    df0+=(at_dif0[i]=e0[i]-lp0[i]);
    df1+=(at_dif1[i]=e1[i]-e0[i]-lp1[i]);
    df2+=(at_dif2[i]=e2[i]-e1[i]-lp2[i]);
  }

  l2=(char)analeg(at_dif1);      // left interleg
  unev12 = leg_uneven;
  botw12 = leg_botw;
  sumw12 = (char)leg_sumw;

  l4=(char)analeg(at_dif2);      // right interleg
  unev23 = leg_uneven;
  botw23 = leg_botw;
  sumw23 = (char)leg_sumw;

  l0=(char)analeg(at_dif0);      // before left leg

  if ( (e2[ehh-1] - lp2[ehh-1] - e2[shh] +lp2[shh] >= 2) &&
       (e2[ehh-1] - e2[shh]  >= 2)
     )
     zone_err(3,9)       // backslash-like right leg
  if ( (e1[ehh-1] - lp1[ehh-1] - e1[shh] +lp1[shh] >= 2) &&
       (e1[ehh-1] - e1[shh]  >= 2)
     )
     zone_err(3,6)       // backslash-like middle
  if ( (e0[ehh-1] - lp0[ehh-1] - e0[shh] +lp0[shh] >= 2) &&
       (e0[ehh-1] - e0[shh]  >= 2)
     )
     zone_err(3,3)       // backslash-like left


   ital_un = 0;
   mhh = (shh+ehh)/2;
   { int16_t d1, d2, d3, d12, d23;
     //93.01.28  d3,d1 were miscalculated; all 'm' were upright
     d3 = e0[shh] - lp0[shh];
     d2 = e0[mhh] - lp0[mhh];
     d1 = e0[ehh-1] - lp0[ehh-1];
     d12 = d2-d1;
     d23 = d3-d2;
     if ((d12*d23 >= 0) && (d12+d23 > 1))   // right incline
       ital_un = 1;
     cum_ital |= ital_un;
   }
   if (ital_un)
   {
     if (db_status && (db_trace_flag & 2))
        est_snap(db_pass,AA,"italic m");
     if (botw1 > 2)         // 1st leg  wider at bottom
       zone_err(2,1)
     if (botw2 > 2)         // 2nd leg  wider at bottom
       zone_err(2,4)
     if (botw12 < -2)       // 1 interleg wider at top
       zone_err(1,10)
     if (botw23 < -2)       // 2 interleg wider at top
       zone_err(1,11)
     if (unev1 > 3)
       zone_err(3,2)      // 1st uneven
     if (unev2 > 3)
       zone_err(3,5)      // 2nd uneven
     if (unev3 > 2)
       zone_err(3,8)      // 3rd uneven
     if ((l3-l1 > 2) || (l1-l3 > 2))
       zone_err(3,14)     // unequal width
     if ((l5-l1 > 2) || (l1-l5 > 4))
       zone_err(3,15)     // unequal width
     if ((l5-l3 > 2) || (l3-l5 > 4))
       zone_err(3,16)     // unequal width
     //93.01.28
     wdf=df1-df2;
	 awdf = abs(wdf);
	 pen_ne = (char)awdf;
     if (awdf >=  2*hhh )          // > 2 pnts
       zone_err(3,19)
      /// 93.01.28
     if (l0 > l2+2)
        zone_err(3,17)                      // 1-st leg too far from left end
     if (AA->w - e2[thmdle] > l2 + 2)        // 3rd leg far from right end
        zone_err(3,18)
   }
   else   // NON_ITALIC
   {
     if (db_status && (db_trace_flag & 2))
        est_snap(db_pass,AA,"upright m");
     if (unev1 > 2)
       zone_err(3,2)      // 1st uneven
     if (unev2 > 2)
       zone_err(3,5)      // 2nd uneven
     if (unev3 > 2)
       zone_err(3,8)      // 3rd uneven
     if ((l3-l1 > 2) || (l1-l3 > 2))
       zone_err(3,14)     // unequal width
     if ((l5-l1 > 2) || (l1-l5 > 2))
       zone_err(3,15)     // unequal width
     if ((l5-l3 > 2) || (l3-l5 > 2))
       zone_err(3,16)     // unequal width

     wdf=df1-df2;
     awdf = abs(wdf);
     pen_ne = (char)awdf;
     if (awdf >=  2*hhh )          // > 2 pnts
       zone_err(3,19)
     else
       while ((2*awdf >=  3*hhh ) && (hhh > 5))   // > 1.5 pnts, high enough
       {

         //  exclude uppermost and lowermost lines
         wdf = wdf - at_dif1[shh] - at_dif1[ehh-1]
	       + at_dif2[shh] + at_dif2[ehh-1];
         awdf = abs(wdf);
		 pen_ne = (char)awdf;
         hhh-=2;
         if (awdf >= 2*hhh)
           zone_err(3,19)
         shh++; ehh--;
       }
      // remains not too different ( >1.5 , < 2 points )
     if (l0 > l2+2)
        zone_err(3,17)                      // 1-st leg too far from left end
     if (AA->w - e2[thmdle] > l2 + 2)        // 3rd leg far from right end
        zone_err(3,18)

   } // ELSE ITALIC
 }   // IF 'm'
// study horizontal paths - for native (not glued) patterns only
  memset(at_roof,0,sizeof(at_roof));
  memset(bott,127,sizeof(bott));
  memset(roof,127,sizeof(roof));
  nw1=0;
  while(1)
  {
    WC=GL->celist[nw1++];
    if (!WC) break;
    legd = WC->col-mcol;
    cp1=WC->env;
    Lc1=cp1->nl;                            // number of lines in component
    Lp1=(lnhead *) ((char *)cp1 + cp1->lines + 2); // beginning of first line
    Lp2=Lp1;
    for (lc1=0; lc1 < Lc1; lc1++)
     {
      n1=Lp1->h-1;
      h1=(char)Lp1->row;
      h2=h1+n1;
      y1=0;
      int1=(interval *)(Lp1+1);		  // ptr to current interval
      for (y1=0; y1 <= n1; y1++, h1++, int1++)
       {
	if (h1<R01)
      for (i=0, rp=&at_roof[int1->e+legd-1], rofp=&roof[int1->e+legd-1];
	       i<int1->l; i++,rp--,rofp--)
	   { *rp |=1;
	     if (*rofp > h1) *rofp=h1;
	   }
	if (h1>R02)
      for (i=0, rp=&at_roof[int1->e+legd-1], botp=&bott[int1->e+legd-1];
	       i<int1->l; i++,rp--,botp--)
	   { *rp |=2;
	     if (*botp > (AA->h - h1)) *botp=AA->h-h1;
	   }
       }   // all intervals of line
      Lp2=Lp1=(lnhead *)((char *)Lp2+Lp2->lth); // next line
     }     // all lines of component
   }       // while component
  if (flag_m & 1) goto horizm;

  if ((flag_m & 2) == 0)  goto horizu;   // 'u'

  for (i=e0[thmdle],rooftl=0,roofl=0;
       i<e1[thmdle]-lp1[thmdle]; rooftl++,i++)
   if (at_roof[i] & 1) roofl++;

  roofd = rooftl-roofl;
  if (roofd > 0)    // incomplete roof
  {
    if (nc > 2)      // glued - penalize by gap size
    { int16_t d,p;
      d=roofd;
      if (d > 7) d=7;
      if (ital_un)
        p=pen_roofn[d];
      else
        p=pen_iroofn[d];
      del_vers(AA,2,(uchar)p);
      if (db_status && (db_trace_flag & 2))
         est_snap(db_pass,AA,roof_incompl);
      goto retsort;
    }
    else
    {
      if (db_status && (db_trace_flag & 2))
         est_snap(db_pass,AA,roof_incompl);
      pen_roof = (ital_un) ?  pen_roof_itun : pen_roof_upright;
      goto del_n_roof;
    }
  }
  // complete or penalized roof ---> study roof's details
  minroof=0;
  minr2=minr3=e1[thmdle];
  minr4 = minr3 - lp1[thmdle];
  for (i=minr1=e0[thmdle]-lp0[thmdle]/2, rofp=&roof[i];
       i < minr3 - lp1[thmdle]/2; i++, rofp++)
    if (minroof < *rofp)
	{ minroof=*rofp; minrx=(char)i; }
  while (1)
    if ((mr1=roof[minr1++]) != 127) break;
  while (1)
    if ((mr2=roof[--minr2]) != 127) break;
  i=MIN(mr1,mr2);
  if (minroof <= (i+2)) goto tryu;		// not too deep gap
  if (minrx < minr4)                  // in left half of cell
  { int16_t gap_bnd_up, gap_bnd_mdl;
    gap_bnd_mdl = e0[thmdle]+3;
    if (minrx < gap_bnd_mdl) goto tryu;   // close to (left leg in its middle)
    gap_bnd_up = e0[1]+3;
    if (ital_un)
    { int16_t ital_inc;
      ital_inc = gap_bnd_up - gap_bnd_mdl;
      if (ital_inc > 0)
        gap_bnd_up += ital_inc;
    }
    if (minrx < gap_bnd_up) goto tryu;        // close to (left leg at its top)
    if (db_status && (db_trace_flag & 2))
      est_snap(db_pass,AA,roof_hole);
    pen_roof = 100;
    goto del_n_roof;
  }
  // in right half of cell:

  if (minrx >= minr2-2) goto tryu;  // arc above leg

  mrw1 = minroof;
  while (minrx < (minr3-2))        // study roof till (end of leg - 2)
  {
    mrw2 = roof[minrx++];
    if (mrw2 == 127)      goto tryu;         // monotoneous arc finishes
    if (mrw2 < mrw1)
    {
      pen_roof = (ital_un) ?  pen_roof_itun : pen_roof_upright;
      goto del_n_roof;
    }  // end of arc too early
    mrw1 = mrw2;
  }
  goto tryu;              // monotoneous increase of arc till end of leg
del_n_roof:
  del_vers(AA,2,pen_roof);
  flag_m -=2; if (flag_m==0) goto retsort;
tryu:
  if ((flag_m & 4) == 0) goto retsort;
horizu:
  for (i=e0[thmdle],bottl=0,botl=0;
       i<e1[thmdle]-lp1[thmdle]; bottl++,i++)
   if (at_roof[i] & 2) botl++;

  botd = bottl-botl;
  if (botd > 0)      // incomplete floor
  {
    if (nc > 2)      // glued - penalize by gap size
    { int16_t d,p;
      d=botd;
      if (d > 7) d=7;
      if (ital_un)
        p=pen_floor[d];
      else
        p=pen_ifloor[d];
      del_vers(AA,4,(uchar)p);
      if (db_status && (db_trace_flag & 2))
        est_snap(db_pass,AA,roof_incompl);
      goto retsort;
    }
    else
    {
      if (db_status && (db_trace_flag & 2))
        est_snap(db_pass,AA,roof_incompl);
      pen_roof = 100;
      goto del_u_roof;
    }
  }
  // complete or penalized floor ---> study floor details
  minroof=127;
  for (i=minr1=e0[thmdle]-lp0[thmdle]/2, botp=&bott[i];
       i < (minr2=e1[thmdle])-lp1[thmdle]/2; i++, botp++)
    if (minroof > *botp)
	{ minroof=*botp; minrx=(char)i; }
  while (1)
    if ((mr1=roof[minr1++]) != 127) break;
  while (1)
    if ((mr2=roof[--minr2]) != 127) break;
  i=MIN(mr1,mr2);
  if (minrx < (e0[thmdle]+3)) goto retsort;   // close to left leg
  if (minrx >= (e1[thmdle]-2)) goto retsort;  // arc under leg
  if (minroof <= (i+2)) goto retsort;		// not too deep gap
del_u_roof:
  if (db_status && (db_trace_flag & 2))
    est_snap(db_pass,AA,roof_hole);
  del_vers(AA,4,pen_roof);
  goto retsort;

horizm:
  for (i=e0[thmdle],rooftl=0,roofl=0;
       i<e2[thmdle]-lp2[thmdle]; rooftl++,i++)
    if (at_roof[i] & 1) roofl++;
  roofd = rooftl-roofl;
  if (nc>2)  // glued - more than 1/2 should be covered
   { int16_t d;
     if ((d=roofd) > roofl)
     {
       pen_roof = (cum_ital) ?  pen_roof_itm : pen_roof_upright;
       goto del_m_roof;
     }
     if (d > 7) d=7;
     del_vers(AA,1,pen_roofm[d]);
     if (db_status && (db_trace_flag & 2))
       est_snap(db_pass,AA,roof_incompl);
     goto retsort;
   }
  else       // nonglued - all must be covered
   { if (roofd > 0)    { pen_roof = 100; goto del_m_roof; } }
// test bottom part -- either both present (2 seriffs) or left only
  if (nc > 2)  goto retsort;  // glued cell

  for (i=e0[thmdle],roofl=0, rooftl=0;
       i<e1[thmdle]-lp1[thmdle]; rooftl++, i++)
    if (at_roof[i] & 2) roofl++;  // covered bottom in 1-st interleg

  for (i=e1[thmdle],roofr=0, rooftr=0;
       i<e2[thmdle]-lp2[thmdle]; rooftr++, i++)
    if (at_roof[i] & 2) roofr++;  // covered bottom in 2-nd interleg

  rooftl-=roofl; rooftr-=roofr;   // uncovered parts of interlegs
				  // interleg covered if rooftX==0
  if (rooftr != 0) goto m_arcs;      // right interleg not covered  - OK
  if (rooftl == 0) goto m_arcs;      // left also covered - OK
     // right covered, left not;
  if (2*rooftl <= l2) goto m_arcs;   // uncovered part of 1-st interleg
			      // small enough ( <1/2 of interleg width)
  //  test against ITALIC  (coverage may be via projection, not connection)
  // 93.08.19 italic flag already obtained AND penalty adjusted ?????
  /// if ((e0[0]-lp0[0]) <= (e0[hzone-1]-lp0[hzone-1]+1))
  ///   { pen_roof = pen_roof_ui; goto del_m_roof; }
  pen_roof = (cum_ital) ? pen_roof_itm : pen_roof_upright;
  goto del_m_roof;
 m_arcs:  // check large cavernae in complete roof
  //   complete or penalized roof ---> study roof's details
  //   between 1st and 3rd legs
  if (cum_ital) goto retsort; // no check roof holes for italic
  minroof=0;
  for (i=minr1=minrx=(e0[thmdle]+e1[thmdle]-lp1[thmdle])/2, rofp=&roof[i];
       i < (minr3=(e1[thmdle]+e2[thmdle]-lp2[thmdle])/2);
       i++, rofp++)
    if ((*rofp != 127)  && (minroof < *rofp))
      { minroof=*rofp; minrx=(char)i; }
  if (minroof <= 3) goto retsort;     // not too deep gap
  if (2*minrx > (2*e1[thmdle] - lp1[thmdle]-2)) goto retsort;
  pen_roof = 32;   // not at right side of 2nd leg
  if (minrx < (e1[thmdle] - lp1[thmdle]-1))
    pen_roof = pen_roof_upright;
  if (db_status && (db_trace_flag & 2))
    est_snap(db_pass,AA,roof_hole);
del_m_roof:
  del_vers(AA,1,pen_roof);

retsort:
  if (fl_sort_vers) sort_vers(AA);
 }

static int16_t analeg (char *l)
{
 char *sp, *ep, *cp, *sp1, *ep1, min, max, w;
 min=127; max=0;
 leg_sumw=0;
 sp=&l[shh];
 ep=&l[ehh-1];
 sp1 = sp+1;
 ep1 = ep-1;
 leg_botw = *ep + *ep1 - *sp - *sp1;
 for (cp=sp; cp <= ep; cp++)
 {
  w=*cp;
  leg_sumw += w;
  if (w > max)
    { max = w; leg_maxpos=cp-sp; }
  if (w < min)
    { min = w; leg_minpos=cp-sp; }
 }

 leg_uneven=max-min;
 w = ehh-shh;
 if (w <= 0) { leg_uneven = leg_botw = 100; return 100; }
 leg_midw = (leg_sumw+w-1)/w;
 if (w < 3)  // zone of 3 --- don't study even/uneven
 {
   leg_uneven = leg_botw = 0;
   return leg_midw;
 }
 // before 10/25:  leg_botw = leg_maxpos - leg_minpos;
 // if (leg_uneven <= 2)  return leg_midw;
 // if ((leg_botw > 0) && fl) return (-leg_midw);  // wider at bottom
 return leg_midw;

 }

void del_vers(cell *c, uchar x, uchar pen)
{
 version *dv1;
 uchar cx, cx_acc, fld, fla;
 int16_t p;
 fld=0;
 for (dv1=c->vers; (cx_acc=dv1->let)!=0; dv1++)
 {
   cx = let_sans_acc[cx_acc];
   if ((x & 1) && (cx=='m'))     { fla = 1; goto delv; }
   if ((x & 2) && (cx=='n'))     { fla = 1; goto delv; }
   if ((x & 2) && (cx==liga_ri)) { fla = 1; goto delv; }
   if ((x & 4) && (cx=='u'))     { fla = 1; goto delv; }
   continue;
 delv:
   if (fld & fla) { dv1->prob=0; continue;}
   p=dv1->prob-pen;
   if (p<0) p=2;
   dv1->prob = (uchar)p;
   fld |= fla;
   fl_sort_vers=1;
 }
}


