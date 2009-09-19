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

// Линейный критерий (в основном)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "struct.h"
#include "status.h"
#include "cstr.h"
#include "func.h"
#include "linear.h"
#include "ligas.h"
#include "lang.h"
#include "linutil.h"
#include "tuner.h"

#ifdef _BAL_VERSION_   // Nick 10.02.2001

extern uchar   pass2;
//extern Bool  FlagRestDust;
void   save_dust(void);
void   restore_dust(void);

extern uint16_t actual_resolution;
extern uchar let_captype[];
extern int16_t line_number;
extern uchar fax1x2;
extern uchar language;

/// static variables

static int16_t DPs, DPsf;
static char diff_curv = 0;   // always !

static int16_t change_vote = 0;  // only ++, never more used !

static int16_t ncut_cells=0;  // static
static int16_t ncut_vers=0;   // static
static int16_t ntot_cells=0;  // static

//////////////////

void cell_position(cell *c)
 {
 int16_t w,f, row;

 if (Nb2==-2)
 {
   c->cpos=ce_p_large;
   return;
 }

 c->cpos=ce_p_all;
 row = c->row - minrow - c->bdiff;
 f=0;
 if (Nb1!=-1)
  {
  if (row < (bbs1+bbs2)/2)
  {
    c->cpos=ce_p_large+ce_p_Q+ce_p_Ql+ce_p_ex;
  }
  else
   {c->cpos-=ce_p_large+ce_p_Q+ce_p_Ql+ce_p_ex; f=1;
   }
  }

 if (!f && Nb2!=-1)
  {
  if ((w=row-bbs2)>-2)
   { c->cpos&=ce_p_small+ce_p_low;
   }
  else
   if (w<-4) { c->cpos &= ce_p_all-ce_p_small-ce_p_low;
   }
  }
 if (Nb3!=-1)
  {
  f=0;
  if ((w=row+c->h-bbs3)>0)
   {
   if (w>=4) { c->cpos &= ce_p_all-ce_p_large-ce_p_small-ce_p_ex;
   }
   else
    {
    if (w>2) { c->cpos &= ce_p_all-ce_p_low-ce_p_Ql-ce_p_ex;
    }
    else { c->cpos &= ce_p_large+ce_p_small+ce_p_t;
    }
    f=1;
    }
   }
  else
   {
   if (w>=-2) { c->cpos &= ce_p_large+ce_p_small+ce_p_t;
   }
   else
    {
    if (w>=-4) { c->cpos &= ce_p_large+ce_p_small+ce_p_t+ce_p_ex;
    }
    else { c->cpos &= ce_p_ex;
    }
    }
   f=1;
   }
  if (!f && Nb4!=-1)
   {
   if (row+c->h-bbs4>-3) { c->cpos &= ce_p_low+ce_p_Ql+ce_p_bra;
   }
   else { c->cpos &= ce_p_all-ce_p_Ql-ce_p_low;
   }
   }
  if (!c->cpos) { c->cpos=ce_p_large;
  }
  }
}
///////////////
// установить позиции компонент (c->cpos)
 void cell_by_base()
 {
 cell *c;

 for (c=(cell_f())->nextl; c->nextl!=NULL; c=c->nextl)
  cell_position(c);
 }
///////////////////////////
 /*********************************/

// from linpon.c
int16_t smart_diff(cell * c)
{
int16_t d1,d2,bm,row;
 row = c->row - minrow;
 d1 = row - bbs1;
 d2 = row - bbs2;
 bm = (bbs3 + bbs2)/2;
  #ifdef UFA // Valdemar 12-05-94 08:25pm
  return c->bdiff;
  #endif
  if(c->h >= Ps && (abs(d1) < 2 || abs(d2) < 2) && row+c->h >= bbs3)
  {
   c->bdiff=0; // don't touch letter
   c->difflg &= ~(c_db_down | c_db_up);
  }  // ж,й,¤

 if(c->h >= Ps + 2 ) // large letter
 {
  if(abs(d1) < 2 || abs(d2) < 2)
  {
   c->bdiff=0; // don't touch letter
   c->difflg &= ~(c_db_down | c_db_up);
   c->difflg |= c_db_forbid;
  }
  else
  { // letter d'not lay on base
   if( row < (bbs1+bbs2)/2 ) return 0;    // farewell !
   if( row < bbs2 )
        if( row+c->h < bbs3 ) return 0;   // farewell !
        else if( c->bdiff > 0 )
                {
                c->bdiff=0; // don't touch letter
                c->difflg &= ~(c_db_down | c_db_up);
                }
  }
 }

  if( c->h < Ps-1 && c->flg & (c_f_let+c_f_bad) ) // letter < Ps may be dust
   if(abs(d1) < 2 || abs(d2) < 2){
      c->bdiff=0; // don't touch letter
   c->difflg &= ~(c_db_down | c_db_up);
   c->difflg |= c_db_forbid;
   }
return c->bdiff ;
}
///////////////////////
void interdif(cell *P, cell *N, cell *B1)
{
 int16_t bsc, bsp, bsn, wdn, wdp, clp, cln, clc;
 bsp=P->h+P->row-minrow;
 bsn=N->h+N->row-minrow;
 clp=P->col; cln=N->col;
 wdp=P->bdiff;
 wdn=N->bdiff;
 bsc=B1->h+B1->row-minrow;
 clc=B1->col;
 if (B1->flg & (c_f_let|c_f_bad))
 // 93.01.26 to be done for letters only;
    B1->difflg &= ~(c_db_down | c_db_up);
 if (P->flg & c_f_fict)
 {
   wdp = 0;
   if (N->flg & c_f_fict) { B1->bdiff=0; wdn = 0; goto retcv; }
   B1->bdiff = (char)wdn  ;
   if (B1->flg & (c_f_let|c_f_bad))
   {
     switch (bsn-bsc)
     {
       case  1: B1->bdiff--; B1->difflg |= c_db_up; break;
       case -1: B1->bdiff++; B1->difflg |= c_db_down; break;
       default:   //jump letter
                 break;
     }
   }
   goto retcv;
 }
 if (N->flg & c_f_fict)
 {
   wdn = 0;
   B1->bdiff = (char)wdp;
   if (B1->flg & (c_f_let|c_f_bad))
   {
     switch (bsp-bsc)
     {
       case  1: B1->bdiff = wdp-1; B1->difflg |= c_db_up; break;
       case -1: B1->bdiff = wdp+1; B1->difflg |= c_db_down; break;
       default:   //jump letter
                 break;
     }
   }
   goto retcv;
 }
 /* both P, N -- usual cells; defined baseline */
 if ((B1->flg & (c_f_let|c_f_bad)) == 0) goto interpol;  // dust - from nearest
 if ((B1->flg & (c_f_let|c_f_bad)) == 0) goto interpol;
 switch (bsp-bsc)
 {
   case  1: B1->bdiff = wdp-1; B1->difflg |= c_db_up; goto retcv;
   case -1: B1->bdiff = wdp+1; B1->difflg |= c_db_down; goto retcv;
   case  0: B1->bdiff = (char)wdp  ; goto retcv;
 }
 switch (bsn-bsc)
 {
   case  1: B1->bdiff = wdn-1; B1->difflg |= c_db_up; goto retcv;
   case -1: B1->bdiff = wdn+1; B1->difflg |= c_db_down; goto retcv;
   case  0: B1->bdiff = (char)wdn  ; goto retcv;
 }



 interpol:
 if (wdp==wdn) {B1->bdiff=(char)wdn; goto retcv; }
 // set diff of whoever is closer to cell
 if ((clc-clp) > (cln-clc))
   B1->bdiff=(char)wdn;
 else
   B1->bdiff=(char)wdp;
 retcv:
// if (abs(B1->bdiff) < 2)
if(Ns1+Ns2 > 0 && B1->bdiff != 0) smart_diff( B1 ); // Valdemar
if (abs(B1->bdiff) < 2 && language != LANG_RUSSIAN) // Valdemar
  {
   if ((wdn==0) && (wdp==0) && (diff_curv==0))
   {
     B1->bdiff=0;
     B1->difflg &= ~(c_db_down | c_db_up);
   }
   // forget diff correction when all were small (-1 <=  <= +1)
   if ((abs(wdn) < 2) && (abs(wdp) < 2) && (diff_curv==0))
     B1->difflg &= ~(c_db_down | c_db_up);
 }
}
//////////////////
// уточнить-найти B1->bdiff  с использованием соседей
void inter_diff(cell *B1)
{
 cell *P, *N;
 N=P=B1;
 while (1)
 {
  N=N->next;
  if (N->flg & c_f_fict) break;
  if (!(N->flg & (c_f_let | c_f_bad))) continue;
  if ((N->difflg & 4) && (!(N->difflg & 64))) /* defines base */
    break;
 }
 while (1)
 {
  P=P->prev;
  if (P->flg & c_f_fict) break;
  if (!(P->flg & (c_f_let | c_f_bad))) continue;
  if ((P->difflg & 4) && (!(P->difflg & 64))) /* defines base */
    break;
 }
 interdif(P,N,B1);
}
//////////////////////////

static int16_t  top, bot, db, bs1, bs2, bs3, bs4;

//
// FAX 100*200 penalties
// large below b1     (prorated to (b2-b1)/8; /6 /4
// static unsigned char ptu1138[9]={0,0,12,24,54,96,140,180,240};
   static unsigned char ftu1138[9]={0,0,0,12,24,44,76,120,180};
// static unsigned char ptu1136[9]={0,12,36,128,240,240,240,240};
   static unsigned char ftu1136[9]={0,0,12,26,100,180,240,240};
// static unsigned char ptu1130[9]={0,12,48,240,240,240,240};
   static unsigned char ftu1130[9]={0,0,12,28,180,240,240,240};
// small above b2     (prorated to (b2-b1)/8
/*****************
// static unsigned char ptu2138[9]={0,12,80,180,240,240,240,240};
   static unsigned char ftu2138[9]={0,0,12,60,180,240,240,240};
// static unsigned char ptu2136[9]={0,12,128,240,240,240,240,240};
   static unsigned char ftu2136[9]={0,0,12,80,180,240,240,240};
// static unsigned char ptu2135[9]={0,12,48,240,240,240,240,240};
   static unsigned char ftu2135[9]={0,0,12,28,140,240,240,240};
****************/
// static unsigned char ptu2138[9]={0,12,50,180,240,240,240,240};
   static unsigned char ftu2138[9]={0,0,12,60,180,240,240,240};
// static unsigned char ptu2136[9]={0,12,80,140,240,240,240,240};
   static unsigned char ftu2136[9]={0,0,12,80,140,240,240,240};
// static unsigned char ptu2135[9]={0,12,48,240,240,240,240,240};
   static unsigned char ftu2135[9]={0,0,12,28,140,240,240,240};

// static unsigned char ptu12[8]={0,20,100,240,240,240,240,240};
   static unsigned char ftu12[8]={0,20,100,240,240,240,240,240};
// too small: prorated to Ps/16
// static unsigned char ptu22[8]={0,0,12,36,72,128,180,240};
   static unsigned char ftu22[8]={0,0,12,36,72,128,180,240};
// static unsigned char ptu3[8]={112,48,16,0,0,0,0,0};
//   static unsigned char ftu3[8]={48,16,0,0,0,0,0,0};
static unsigned char ftu3[6][4] =
// 't' penalty depends on j=(b2-b1) and i=(b2-top)
 {  0,0,0,0,
    0,0,0,0,
    4,0,0,0,
    12,0,0,0,
    24,8,0,0,
    48,16,0,0
 };

// static unsigned char ptu4[8]={112,80,20,0,0,0,0,0};
   static unsigned char ftu4[8]={80,20,0,0,0,0,0,0};
// static unsigned char ptd1[8]={0,0,20,40,80,100,140,240};
// 940329   static unsigned char ftd1[8]={0,0,0,20,40,80,100,140};
   static unsigned char ftd1[8]={0,0,0,20,40,60,80,140};
// static unsigned char ptd2[8]={0,0,0,0,20,40,60,80};
   static unsigned char ftd2[8]={0,0,0,0,20,40,60,80};
// static unsigned char ptd3[8]={240,100,20,0,0,0,0,0};
   static unsigned char ftd3[8]={240,100,20,0,0,0,0,0};
// static unsigned char ptd6[8]={0,0,20,80,120,140,240,240};
   static unsigned char ftd6[8]={0,0,0,20,60,100,120,240};

static int16_t cut_by_posf(cell *C, uchar chr, int16_t wp, uchar flg, uchar arg)
{
  int16_t  dps;
  uchar dflag;
  unsigned char pen_upr;
  int16_t  up, dn;
  int16_t  i, j, bsi, b1, b2, b3, b4;

  int  pen_up,pen_dn;

  if (flg==1)
  {
    top=C->row-minrow;
	bot=top+C->h;
    db=C->bdiff;
    def_locbas(C);
    b1=bbs1+db; b2=bbs2+db; b3=bbs3+db; b4=bbs4+db;
  }
  else
  { b1=bs1; b2=bs2; b3=bs3; b4=bs4; }
  up = let_linpos[chr];
  if (up==0)
  {
    if ((chr=='<' || chr=='>') && (bot<=(b2+b3)/2 || C->h>=b3-b1))
     goto cut;
    return wp;
  }
  dn=up & 15; up >>= 4;
  dflag = let_lindef[chr];
   pen_up=pen_dn=0;
   if (all_caps)
   {
     // letters with twins --> penalize to allow context to overrule
     if (is_lower(chr) && twin(chr)) { pen_up=12; goto decide; }
     // p is less solid
     if (chr=='p') { pen_up = 24; goto decide; }
     // smalls are forbidden
     if (let_linshape[chr] & 8) pen_up = 140;   // small - penalize deadly
     goto decide;
   }
   pen_up=pen_dn=0;
   if (all_caps)
   {
     // letters with twins --> penalize to allow context to overrule
     if (is_lower(chr) && twin(chr)) { pen_up=12; goto decide; }
     // p is less solid
     if (chr=='p') { pen_up = 24; goto decide; }
     // smalls are forbidden
     if (up & 2) pen_up = 140;   // small - penalize deadly
     goto decide;
   }
   bsi=0;
   switch (dn)
   {
   case 0: break;
   case 1:
    if (Nb3<=0) break;
    bsi=i=bot-b3; j=cpsf(abs(i));
    if (j > 7) goto cut;
    pen_dn=ftd1[j];
    break;
   case 2:
    if (Nb3>0)
     { if (cpsf1(bot-b3) < 3) goto cut; }
    if (Nb4<=0) break;
    if ((i=cpsaf(b4-bot)) < 0) break;
    if (i > 7) goto cut;
    pen_dn=ftd2[i]; break;
   case 3:
   case 5:
    if (Nb3<=0) break;
    if ((i=cpsf(bot-b3)) < 0) goto cut;
    if (i > 7) break;
    if (memchr("[]{}",chr,4))
     i++;
    pen_dn=ftd3[i]; break;
   case 4:
    if (Nb3>0)
     { if (cpsf2(bot-b3) > 3) goto cut; }
    if (Nb4>0)
     { if (cpsaf(b4-bot) < 4) goto cut; }
    break;
   case 6:  /* at bbs3 or lower */
    if (Nb3 <= 0 ) break;
    i=bot-b3;
    if (i >= 0) break;
    j=cpsf(-i);
    if (j > 7) goto cut;
    pen_dn=ftd6[j];
    break;
   } // switch DOWN

  if ((Nb1 <= 0) && (Nb2 <= 0)) goto decide;
   switch (up)
   {
   case 0: break;
   case 1:       // large
    if (Nb1>0)   // b1 known
    {
      i=top-b1;
      if (i<=0) break;
      else
      {
        if (bsi > 0)
          { b1+=bsi; b2+=bsi; i-=bsi;
            if (i<=0) break;
          }
      }
      if (top>=b2) goto cut;
      j=b2-b1;
      if (j > 7)
      {
        i=i*8/j;     /* scaled in 1/8 bbs2--bbs1 */
        if (i>7) goto cut;
        pen_up=ftu1138[i];
        break;
      }
      if (j > 5)
      {
        i=i*6/j;     /* scaled in 1/8 bbs2--bbs1 */
        if (i>7) goto cut;
        pen_up=ftu1136[i];
        break;
      }
      if (i > 7) goto cut;
      pen_up=ftu1130[i];
      break;
    }
    if (Nb2<=0) break;
    if ((i=cpsf(top-b2)+4) <= 0) break;
    if (bsi > 0)  i=cpsf(top-b2-bsi)+4;
    if (i <= 0) break;
    if (i > 5) goto cut;
    pen_up=ftu12[i]; break;

   case 2:
    if ((Nb1<=0) && (Nb2<=0)) goto cut;
    i = (bot-top)-(b3-b2);    // must be of "Ps" size
    if (abs(i) < 2) break;    // OK !!
    if (i < -1) goto case_sm;  // too small - penalize as is
    if (i > 2)  goto case_sm;
    // i==2 allow round
    if (dflag & c_df_round) break;

   case 10:  // at bs2
   case_sm:
    if ((Nb1<=0) && (Nb2<=0)) goto cut;
    if ((C->difflg & c_db_up)  && (C->h < Ps))     // small, base corrected "up"
      { b1+=1; b2+=1; b3+=1;}
    if ((C->difflg & c_db_down)  && (C->h > Ps))     // tall, base corrected "down"
      { b1-=1; b2-=1; b3-=1;}
    if (Nb1>0)
    { if ((i=top+top+2-b1-b2) < 0)
        { if (bsi >= 0)  goto cut;
          if (i+bsi+bsi < 0) goto cut;
        }
    }
    if (Nb2<=0)
    {
      //if (bbsm <= top)  pen_up=120;
      if ((b3-(b3-b1)*2/5) <= top) pen_up=120;
      break;
    }
    if ((i=top-b2) > 0)      // too small
    {
      i = i*16/Ps;     // penalize prorated to Ps
      if (i>7) goto cut;
      pen_up=ftu22[i];
      break;
    }
    j=b2-b1;
    i = -i;
    //93.01.27if (i && (dflag & c_df_round)) i--;
    if (i < 2) break;
    // if (i==2)
    // { pen_up = 12; break; }
    if (j > 7)        // may use b1...b2 as a scale
    {
      i=i*8/j;     /* scaled in 1/8 bbs2--bbs1 */
      if (i>7) goto cut;
      pen_up=ftu2138[i];
      pen_upr=ftu2138[i-1];
      goto cor_round2;
    }
    if (j > 5)        // may use b1...b2 as a scale
    {
      i=i*6/j;     /* scaled in 1/8 bbs2--bbs1 */
      if (i>7) goto cut;
      pen_up=ftu2136[i];
      pen_upr=ftu2136[i-1];
      goto cor_round2;
    }
    if (i > 7) goto cut;
    pen_up=ftu2135[i];
      pen_upr=ftu2135[i-1];
    // 93.01.27
    cor_round2:
    if ((dflag & c_df_round)  && (i > 1))
    {
    ///  && (pen_up >= 20)) pen_up -= 20;
      pen_up = (pen_up + pen_upr) /2;
    }
    //// if ((pen_up >= 20) && (dflag & c_df_round)) pen_up -= 20;
    break;

    case 3:
     i=cpsf(b2-top);
     if (i < 0)
     {
       if (bsi <= 0) goto cut;
       i=cpsf(b2+bsi-top);
       if (i < 0) goto cut;
     }
    // if ((i < 8) && ((b2-b1) > 3))
    //   pen_up=ftu3[i];
    j = b2-b1;
    if (i <= 3)
    { int j1;
      j1 = (j <= 5)? j:5;
      pen_up=ftu3[j1][i];
    }
    //if ((Nb1>0) && (j > 4) && (top <= (b1+2)))
    if ((Nb1>0) && (j > 5) && (top <= (b1+2)))
      pen_up+=4;   /* prevent too tall 't'  */
    break;

    case 4:
     if (Nb1>0)
      { if ((i=top+top-b1-b2) < 0) break; }
     if (Nb2<=0) break;
     if ((i=cpsf(b2-top)) < 0)
       {
	if (bsi <= 0) goto cut;
    i=cpsf(b2+bsi-top);
	if (i < 0) goto cut;
       }
     if (i > 4) break;
     pen_up=ftu4[i]; break;

    case 5:   // 'g'
     if (Nb2<=0)
     {
       //if (bbsm <= top)  pen_up=120;
       if ((b3-(b3-b1)*2/5) <= top) pen_up=120;
       if (b1-top >= 1) pen_up=4;
       break;
     }
     i=b2-top;
     if ((i < 0) && (bsi > 0))  i+=bsi;
     if ((i=top-b2) > 0)      // too small
     {
       i = i*16/Ps;     // penalize prorated to Ps
       if (i>7) goto cut;
       pen_up=ftu22[i];
       break;
     }
   } // switch UP
decide:
   dps=pen_dn+pen_up;
   wp=wp-dps;

   // trscut  = 36 ---> penalty enough to cut at iterations
   if ((!arg)  && (dps > TRSCUT)) goto cut;

   // trsweak = 70 ---> result (obtained by penalty trspnlt > 120))
   if ((wp <= 0) || ((wp < TRSWEAK) && (dps > TRSPNLT))) goto cut;
   return wp;
cut:
   return 0;
}
///////////////////
// large below b1     if b2-b1 > 8 --> prorated to (b2-b1)/8
static unsigned char ptu1138[9]={0,0,12,24,54,96,140,180,240};
static unsigned char ptu1136[9]={0,12,36,128,240,240,240,240};
static unsigned char ptu1130[9]={0,12,48,240,240,240,240};
// small above b2     (prorated to (b2-b1)/8
static unsigned char ptu2138[9]={0,12,50,180,240,240,240,240};
//static unsigned char ptu2136[9]={0,12,80,140,240,240,240,240};
static unsigned char ptu2136[9]={0,12,70,120,240,240,240,240};
static unsigned char ptu2135[9]={0,12,48,240,240,240,240,240};


static unsigned char ptu12[8]={0,20,100,240,240,240,240,240};
// too small: prorated to Ps/16
static unsigned char ptu22[8]={0,0,12,36,72,128,180,240};
// 940123 TRY TO: static unsigned char ptu22[8]={0,0,36,80,128,180,220,240};
static unsigned char ptu22p[8]={240,180,120,96,64,32,0,0};
/////////  static unsigned char ptu3[8]={112,48,16,0,0,0,0,0};
static unsigned char ptu3[6][4] =
// 't' penalty depends on j=(b2-b1) and i=(b2-top)
 {  0,0,0,0,
    0,0,0,0,
    48,0,0,0,
    64,0,0,0,
    96,24,8,0,
    112,48,16,0
 };
static unsigned char ptu4[8]={112,80,20,0,0,0,0,0};
static unsigned char ptd1[8]={0,0,20,40,80,100,140,240};
static unsigned char ptd2[8]={0,0,0,0,20,40,60,80};
static unsigned char ptd3[8]={240,100,20,0,0,0,0,0};
static unsigned char ptd6[8]={0,0,20,80,120,140,240,240};
static unsigned char ptd7[8]={140,20,0,0,0,0,0,0}; //  ж й ¤



int16_t cut_by_posu(cell *C, uchar chr, int16_t wp, uchar flg, uchar arg)
{
  int16_t  dps;
  uchar dflag;
  unsigned char pen_upr;
  int16_t  up, dn;
  int16_t  i, j, ib2, bsi, b1, b2, b3, b4;

  int pen_up,pen_dn;

  // Nick 31.01.2001
  // Если базовые линии - совершенно ненадежны, ничего по
  // ним не резать !
  if( (Nb1 == 128 || Nb1 <=0) && (Nb2 == 128 || Nb2 <=0) &&
	  (Nb3 == 128 || Nb3 <=0)
     )
	 return wp;

  if (flg==1)
  {
    top=C->row-minrow; bot=top+C->h;
    db=C->bdiff;
    def_locbas(C);
    b1=bbs1+db; b2=bbs2+db; b3=bbs3+db; b4=bbs4+db;
  }
  else
  {
    b1=bs1; b2=bs2; b3=bs3; b4=bs4;
  }

  up = let_linpos[chr];
  if (up==0)
  {
    if ((chr=='<' || chr=='>') && (bot<=(b2+b3)/2 || 9*C->h>10*(b3-b1)) ||
        chr=='@' && C->h<b3-b2)
     goto cut;
    return wp;
  }
  dn=up & 15; up >>= 4;
  dflag = let_lindef[chr];
   pen_up=pen_dn=0;
   if (all_caps)
   {
     // letters with twins --> penalize to allow context to overrule
     if (is_lower(chr) && twin(chr)) { pen_up=12; goto decide; }
     // p is less solid
     if (chr=='p') { pen_up = 24; goto decide; }
     // smalls are forbidden
     if (let_linshape[chr] & 8) pen_up = 140;   // small - penalize deadly
    /*******************************************
     // 940125  not too small
      i = b3-top;   // compare to Ps
      if (i < 0) goto cut;
      i = i*8/Ps;
      if (i <= 7)
        pen_up=ptu22p[i];
    **********************************************/
     goto decide;
   }
   bsi=0;

   switch (dn)
   {
   case 0: break;
   case 1:
    if (Nb3<=0) break;
    bsi=i=bot-b3; j=cpsu(abs(i));
    if (j > 7) goto cut;
    pen_dn=ptd1[j];
    break;
   case 2:
    if (Nb3>0 )            // OLEG for under cells
     { if (cpsau(bot-b3+((C->flg_new&c_fn_under)?1:0)) < 3) goto cut; }
    if (Nb4<=0) break;
    if ((i=cpsu(b4-bot)) < 0) break;
    if (i > 7) goto cut;
    pen_dn=ptd2[i]; break;
   case 3:
   case 5:
    if (Nb3<=0) break;
    if ((i=cpsu(bot-b3)) < 0) goto cut;
    if (i > 7) break;
    if (memchr("[]{}",chr,4))
     i++;
    pen_dn=ptd3[i]; break;
   case 4:
    if (Nb3>0)
     { if (cpsau(bot-b3) > 3) goto cut; }
    if (Nb4>0)
     { if (cpsu(b4-bot) < 4) goto cut; }
    break;
   case 6:  /* at bbs3 or lower */
    if (Nb3 <= 0 ) break;
    i=bot-b3;
    if (i >= 0) break;
    j=cpsu(-i);
    if (j > 7) goto cut;
    pen_dn=ptd6[j];
    break;
   case 7: // Valdemar slightly lower bbs3 ж ¤ й
   if (Nb3<=0) break;
   if ((i=cpsu(bot-b3)) < 0) goto cut;
   if (i > 7) break;
   pen_dn=ptd7[i]; break;
   } // switch DOWN

  if ((Nb1 <= 0) && (Nb2 <= 0)) goto decide;

   switch (up)
   {
   case 0: break;
   case 1:       // large
    if (Nb1>0)   // b1 known
    {
      i=top-b1;
      if (i<=0) break;
      else
      {
        if (bsi > 0)
          { b1+=bsi; b2+=bsi; i-=bsi;
            if (i<=0) break;
          }
      }
      ib2 = b2-top;
      if (ib2 <= 0) goto cut;
      j=b2-b1;
#ifdef PEN_TAB
      j=b2-b1;
      if (j > 8)
      {
        i = ib2*8/j;
        if (i > 7) break;
        if  (MEMCHR("lILf",chr,4))
          pen_up = ptust113[8][i];
        else
          pen_up = ptu113[8][i];
        break;
      }
      if (ib2 > 8) break;
      pen_up = ptu113[j][ib2];
      break;
#else
      if (j > 7)
      {
        i=i*8/j;     // scaled in 1/8 bbs2--bbs1
        if (i>7) goto cut;
        pen_up=ptu1138[i];
        break;
      }
      if (j > 5)
      {
        i=i*6/j;     // scaled in 1/8 bbs2--bbs1
        if (i>7) goto cut;
        pen_up=ptu1136[i];
        break;
      }
      if (i > 7) goto cut;
      pen_up=ptu1130[i];
      break;
#endif
    } // NB1 known

    if (Nb2<=0) break;
    if ((i=cpsu(top-b2)+4) <= 0) break;  // Valdemar: what is bsi ?
    if (bsi > 0 && language != LANG_RUSSIAN)  i=cpsu(top-b2-bsi)+4;
    if (i <= 0) break;
    if (i > 5) goto cut;
    pen_up=ptu12[i]; break;

   case 2:
    if ((Nb1<=0) && (Nb2<=0)) goto cut;
    i = (bot-top)-(b3-b2);    // must be of "Ps" size
    if (abs(i) < 2) break;    // OK !!
    if (i < -1) goto case_sm;  // too small - penalize as is
    if (i > 2)  goto case_sm;
    // i==2 allow round
    if (dflag & c_df_round) break;

   case 10:  // at bs2
   case_sm:
    if ((Nb1<=0) && (Nb2<=0)) goto cut;
    if ((C->difflg & c_db_up)  && (C->h < Ps))     // small, base corrected "up"
      { b1+=1; b2+=1; b3+=1;}
    if ((C->difflg & c_db_down)  && (C->h > Ps))     // tall, base corrected "down"
      { b1-=1; b2-=1; b3-=1;}
    if (Nb1>0)
    { if ((i=top+top-b1-b2) < 0)
        { if (bsi >= 0)  goto cut;
          if (i+bsi+bsi < 0) goto cut;
        }
    }
    if (Nb2<=0)
    {
      if ((b3-(b3-b1)*2/5) <= top) { pen_up=120; break; }
      i = b3-top;   // compare to Ps
      if (i < 0) goto cut;
      i = i*8/Ps;
      if (i > 7) break;
      pen_up=ptu22p[i];
      break;
    }
    if ((i=top-b2) > 0)      // too small
    {
      i = i*16/Ps;     // penalize prorated to Ps
      if (i>7) goto cut;
      pen_up=ptu22[i];
      break;
    }
    j=b2-b1;
    i = -i;
    if ( dflag & c_df_round ) i--;
#ifdef PEN_TAB
    if (j > 8)
    {
      i = i*8/j;
      if (i > 7) goto cut;
      pen_up = ptu213[8][i];
      break;
    }
    if (i > 7) goto cut;
    pen_up = ptu213[j][i];
    break;
#else
    if (i < 3) break;
    if (j > 7)  // may use b1...b2 as a scale
    {
      i=i*8/j;     // scaled in 1/8 bbs2--bbs1
      if (i>7) goto cut;
      pen_up=ptu2138[i];
      pen_upr=ptu2138[i-1];
      goto cor_round2;
    }
    if (j > 5)        // may use b1...b2 as a scale
    {
      i=i*6/j;     // scaled in 1/8 bbs2--bbs1
      if (i>7) goto cut;
      pen_up=ptu2136[i];
      pen_upr=ptu2136[i-1];
      goto cor_round2;
    }
    if (i > 7) goto cut;
    pen_up=ptu2135[i];
      pen_upr=ptu2135[i-1];
    cor_round2:
    // 93.01.27
    if ((dflag & c_df_round)  && (i > 1))
    {
    ///  && (pen_up >= 20)) pen_up -= 20;
      pen_up = (pen_up + pen_upr) /2;
    }
#endif
    break;

    case 3:
    i=cpsu(b2-top);
    if (i < 0)
    {
      if (bsi <= 0) goto cut;
      i=cpsu(b2+bsi-top);
      if (i < 0) goto cut;
    }
    j = b2-b1;
    if (i <= 3)
    { int j1;
      j1 = (j <= 5)? j:5;
      pen_up=ptu3[j1][i];
    }
    if ((Nb1>0) && (j > 5) && (top <= (b1+2)))
    pen_up+=4;   /* prevent too tall 't'  */
    break;

    case 4:
     if (Nb1>0)
      { if ((i=top+top-b1-b2) < 0) break; }
     if (Nb2<=0) break;
     if ((i=cpsu(b2-top)) < 0)
       {
	if (bsi <= 0) goto cut;
    i=cpsu(b2+bsi-top);
	if (i < 0) goto cut;
       }
     if (i > 4) break;
     pen_up=ptu4[i]; break;

    case 5:   // 'g'
     if (Nb2<=0)
     {
       //if (bbsm <= top)  pen_up=120;
       if ((b3-(b3-b1)*2/5) <= top) pen_up=120;
       if (b1-top >= 1) pen_up=4;
       break;
     }
     i=b2-top;
     if ((i < 0) && (bsi > 0))  i+=bsi;
     if ((i=top-b2) > 0)      // too small
     {
       i = i*16/Ps;     // penalize prorated to Ps
       if (i>7) goto cut;
       pen_up=ptu22[i];
       break;
     }
   } // switch UP
decide:
   dps=pen_dn+pen_up;
   wp=wp-dps;

   // trscut  = 36 ---> penalty enough to cut at iterations
   if ((!arg)  && (dps > TRSCUT)) goto cut;

   // trsweak = 70 ---> result (obtained by penalty trspnlt > 120))
   if ((wp <= 0) || ((wp < TRSWEAK) && (dps > TRSPNLT))) goto cut;
   return wp;
cut:
   return 0;
}
//////////////////////////////
// оштрафовать - удалить версию chr по базовым линиям
int16_t cut_by_pos(cell *C, uchar chr, int16_t wp, uchar flg, uchar arg)
{
  if (actual_resolution > 64 )
  {
    DPs  = (Ps  * 300 )/ actual_resolution;
    DPsf = (Psf * 300 )/ actual_resolution;
    if( DPs<1  ) DPs=1;
    if( DPsf<1 ) DPsf=1;
  }
  else
  { DPs = Ps; DPsf = Psf; }
  if( no_linpen )
    return wp;
  else if (fax1x2)
    return cut_by_posf(C, chr, wp, flg, arg);
  else
    return cut_by_posu(C, chr, wp, flg, arg);
}
/////////////////
// оштрафовать - удалить версии C по базовым линиям
int16_t levcut(cell *C, int16_t arg)
{
 int16_t ncut, nalive, wp, np;
 int16_t pold, pnew, flcv;
 version  *v0;
 uchar  chr, surviver;

 top=C->row-minrow;
 bot=top+C->h;
 db=C->bdiff;
 flcv = 0;
 nalive=ncut=0;
 v0=C->vers;
 if (v0->let == bad_char) return 0;  // bad cell
 ntot_cells++;
 C->bas_acc &= ~c_ba_killed;
 if( arg && (C->flg & c_f_solid)) return 0; // Valdemar 12-21-95 08:35pm
 def_locbas(C);
 bs1=bbs1+db; bs2=bbs2+db; bs3=bbs3+db; bs4=bbs4+db;
 for (; v0->let != 0;  v0++)
 {
   chr=v0->let; wp=v0->prob; pold = wp & 1; wp &= 0xfe;
   if (chr==bad_char) return 0;
   np = cut_by_pos(C,chr,wp,0,(uchar)arg);
   if (np > 0)
   {
     pnew = 0;
     // ALIVE CRITERIUM WAS 220 before 931229
     if (np >= 200)
     {
       surviver = chr;
       nalive++;     // as if apply penalty
     }
     if (arg)                      // apply penalty ?
       v0->prob = np & 0xfe;
     else
       v0->prob &= 0xfe;           // remove old mark
   }
   else
   {
     if (pold==0) ncut++;
     pnew = 1;
     if (arg)
       v0->prob=v0->let=0;
     else
       v0->prob |= 1;                       // mark as cut
   }
   if (pold != pnew)
     flcv=1;
 }  // FOR versions

 if (flcv)
   change_vote++;
 if (arg)
   cell_position(C);
 if (arg)
   sort_vers(C);
 if ((nalive==0) ||
     ((nalive==1) && (surviver == invers_exm) && language != LANG_RUSSIAN)
    )
 // entirely killed cell
 { version *v0; uchar chr;
   for (v0=C->vers; (chr=v0->let) !=0; v0++)
   {
     if(! memchr("LTJ()<>[]trI1l!/",chr,16) &&
		 (chr!=liga_i) &&
		 !(language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(chr==i_sans_accent||chr==II_dot_accent)
		  ) &&
		 (chr!=liga_inv_exm)
	   )
       goto all_cut;
   }
   goto retncut;
 all_cut:
   C->bas_acc |= c_ba_killed;
   ncut_cells++;
 }
retncut:
 return ncut;
}
///////////////////////////
// по всей строке
// оштрафовать - удалить версии по базовым линиям
void all_cell_levcut(int16_t arg) // Valdemar makes func global
 {
 cell *C;
 C=cell_f();
 ncut_vers=ncut_cells=ntot_cells=0;
 while ((C=C->nextl)->nextl)
  {
  C->bas_acc &= ~c_ba_killed;
  if (!(C->flg & c_f_let)) continue;
  if ( arg &&
       (C->nvers==1) &&
       // 940114 (MEMCHR("4ERAFMdbm",C->vers[0].let,9)) &&
                 (memchr("4ERAFMbm",C->vers[0].let,8)) &&
       ( cell_is_BOX_solid(C) > 31500) )
     { C->vers[0].prob &= 0xfe; C->difflg |= c_db_solid; continue; }
  ncut_vers+=levcut(C,arg);
  }
 }
///////////////////////////////////
////////////////
// Linban.c
// вернуть С->bdiff для С c C->difflg & 4 и
// C->col ближайшим к arg
// from linban.c
int16_t obtain_diff(int16_t arg)
 {
 int16_t wl, ce, w1m, da, d1n;
 cell *C;

 w1m=d1n=127;
 wl=0;
 C=cell_f();
 while ((C=C->nextl)->nextl)
  {
  if (!(C->difflg & 4))                // base was not defined here ?
   continue;
  wl=C->bdiff;
  ce=C->col;
  da=arg-ce;
  if (da==0)
   return wl;
  if (da < 0)
   {
   if (w1m==127)
    return wl;
   if ((-da) < d1n)
    return wl;
   else
    return w1m;
   }
  d1n=da;
  w1m=wl;
  }
 return wl;
 }
///////////////////////
// from linpon - all for bring_dust()
static void discrim_by_dust()
{
  cell *c, *d;
  c=cell_f();
  while ((c=c->nextl)->nextl)
  {
    if (c->difflg & 0x40) continue;
    if (c->flg & c_f_bad) continue;
    d=c->prev;
    if (d->flg & c_f_dust)
    { if (d->col+d->w > c->col)
        c->difflg |= 0x40;
    }
    d=c->next;
    if (d->flg & c_f_dust)
    { if (c->col+c->w > d->col)
        c->difflg |= 0x40;
    }
  }
}
////////////////
static void all_cell_ledust()
{
 cell *C; int16_t df;
 C=cell_f();
 while ((C=C->nextl)->nextl)
 {
   cell *wc;
   //  if (if_dust(C) & 7)    931229: not large:
   df = if_dust(C);
   if ((df & 7) && ((df & 16) == 0))
   {
//     trans_total++;
     let_to_dust = 1;
     C->flg = c_f_dust;
////// 93.08.19  No more RQDOT
        // C->cg_flag &= ~c_cg_rqdot;
     (wc=C->prevl)->nextl=C->nextl;
     C->nextl->prevl=C->prevl;
     err_pnlet(C);  // AL 940319
     C=wc;
   }
 }
}
////////////////////////
static int16_t dust_to_let()
 {
 int16_t n;
 cell *c,*cn, *cp;
 // glsnap('d',cell_f()->next,"dust_to_let beg");
 for (n=0,c=(cell_f())->next; c->next!=NULL; c=c->next)
 {
   if (c->env && (c->env)->large&ch_underlined)
   {
     cp=c->prev;
     cn=c->next;
     //n+=shaving_let(c); // OLEG : need tunning
     c=cp->next;
     if (c==cn)  //  nothing brought by shaving
      { c = cp; continue; }
   }
   if (c->flg & c_f_dust)   // brought as dust
   { int16_t wr;
     //   if (((wr=if_dust(c)) & 7) == 0)  931229 -- or LARGE
     if ((((wr=if_dust(c)) & 7) == 0) || (wr & 16))
     // not posed as dust, not sized as small dust
     {
       //if (wr & 8)    // not sized as large - retain as dust
       //  continue;   // LARGE ??? -- maybe, use some other method ??
       // eliminate as dust; if "inside" -- bring as letter
       if (((wr=c->row - minrow + c->h/2) <= bbs1) || (wr >= bbs4))
         continue;   // outside the line - not to be letter
       // AL 940411
//       trans_total++;
       (c->next)->prev=c->prev; (cp=c->prev)->next=c->next;
       set_bad_cell(c);
       c->flg = c_f_bad;
       short_recog_cell(c);
       insert_cell(c,cp);
       if (c->flg&c_f_let) n++;
     }
   } // if dust
 }   // for cells
 // glsnap('d',cell_f()->next,"dust_to_let end");
 return n;
 }

/////////////////////////////
// добавить мелочь из lin_str (if arg==0)
// проверить буквы -> мусор и наоборот
// linbam.c
void bring_dust(int16_t arg)
{
 if (arg==0)
 {
   dust_ini(lin_str);
   glsnap('d',cell_f()->next,"dust initted");
   dust_in = 1;
 }
 all_cell_ledust();
 glsnap('d',cell_f()->next,"letters to dust");
 dust_to_let();
 glsnap('d',cell_f()->next,"dust to letters");
 discrim_by_dust();
 glsnap('d',cell_f()->next,"discrim by dust");
}
////////////////////////
#endif
