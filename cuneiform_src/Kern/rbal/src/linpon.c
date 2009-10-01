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
#include <stdio.h>
#include <string.h>

#include "status.h"
#include "cstr.h"
#include "linear.h"
#include "ligas.h"
#include "linutil.h"
#include "tuner.h"

int16_t sMALL_SIZE=SMALL_SIZE;  //NB 3.4.95

EXTLIN CSTR_line lin_str;

static void cell_position(CSTR_rast c);
int16_t if_dust(CSTR_rast C);

void   save_dust(void);
void   restore_dust(void);

#define no__MULTI_PROT__
extern uint16_t actual_resolution;
extern uchar let_captype[];
extern int16_t line_number;
int16_t it_done;

#define noPEN_TAB
int16_t DPs, DPsf;
#define no__POS_PROT__

#ifdef __POS_PROT__
static FILE *ff;
extern int16_t line_number;
static int16_t os2;
void p_prot(int16_t x)
{
  if (os2 & 2)
  {
    ff=fopen("os2.dbg","a");
    fprintf(ff,"%d ",x);
    if (x >= 9000)
       fprintf(ff,"\n");
    fclose(ff);
    return;
  }
  if ((os2 & 1) && (x>=10000))
  {
    ff=fopen("os2.dbg","a");
    fprintf(ff,"%d \n",x);
    fclose(ff);
    return;
  }
}
#endif


void li_snap(char *t)
{
 int n1, n2, n3, n4;
 CSTR_rast C;
 char ww[128];

 C=cell_f();
 //C=C->nextl;
 C=CSTR_GetNextRaster(C,f_letter);
 n1=Ns1;
 if (Nb1==-1)
	 n1=-Ns1;
 n2=Ns2;
 if (Nb2==-1)
	 n2=-Ns2;
 n3=Ns3;
 if (Nb3==-1)
	 n3=-Ns3;
 n4=Ns4;
 if (Nb4==-1)
	 n4=-Ns4;
 sprintf(ww, "%s cv=%d, min=%d (%d) \nPs=%d Bs: %d %d %d %d  Nb: %d %d %d %d ",
              t, ncut_vers, minrow, bbs0, Ps,
              bbs1, bbs2, bbs3, bbs4, n1, n2, n3, n4);
 glsnap ('d',C,ww);
}

void cell_by_base()
{
 CSTR_rast c;

 c = CSTR_GetNextRaster(cell_f(), f_letter );
 for (; c != NULL; c = CSTR_GetNextRaster(c, f_letter ))
  cell_position(c);
}

// установить c->cpos
static void cell_position(CSTR_rast c)
 {
 int16_t w,f, row;
 CSTR_rast_attr attr;

#ifdef __POS_PROT__
 os2 &= 1;
 if (
      (
        ((c->row==1667) && (c->col==491) && (line_number==26) ) ||
        ((c->row==59) && (c->col==139) && (line_number==1) )
      ) &&
      (c->vers[0].let=='y')
    )
    os2 |= 2;
#endif

 CSTR_GetAttr(c,&attr);
 if (Nb2==-2)
 {
   attr.cpos=CSTR_p_large;
#ifdef __POS_PROT__
   p_prot(9002);
#endif

   CSTR_SetAttr(c,&attr);
   return;
 }

 attr.cpos=CSTR_p_all;
 row = attr.row - minrow - attr.bdiff;

 f=0;
 if (Nb1!=-1)
 {
  if (row < (bbs1+bbs2)/2)
  {
#ifdef __POS_PROT__
    p_prot(2);
#endif
    attr.cpos=CSTR_p_large+CSTR_p_Q+CSTR_p_Ql+CSTR_p_ex;
  }
  else
   {
	attr.cpos-=CSTR_p_large+CSTR_p_Q+CSTR_p_Ql+CSTR_p_ex;
	f=1;
#ifdef __POS_PROT__
    p_prot(3);
#endif
   }
 }

 if (!f && Nb2!=-1)
 {
  if ((w=row-bbs2)>-2)
  {
	  attr.cpos &= CSTR_p_small+CSTR_p_low;
#ifdef __POS_PROT__
	  p_prot(4);
#endif
  }
  else
   if (w<-4)
   {
	   attr.cpos &= CSTR_p_all-CSTR_p_small-CSTR_p_low;
#ifdef __POS_PROT__
		p_prot(5);
#endif
   }
 }

 if (Nb3!=-1)
  {
  f=0;
  if ((w=row+attr.h-bbs3)>0)
  {
   if (w>=4) { attr.cpos &= CSTR_p_all-CSTR_p_large-CSTR_p_small-CSTR_p_ex;
#ifdef __POS_PROT__
   p_prot(6);
#endif
  }
  else
  {
    if (w>2)
	{
		attr.cpos &= CSTR_p_all-CSTR_p_low-CSTR_p_Ql-CSTR_p_ex;
#ifdef __POS_PROT__
        p_prot(7);
#endif
    }
    else
	{
		attr.cpos &= CSTR_p_large+CSTR_p_small+CSTR_p_t;
#ifdef __POS_PROT__
        p_prot(8);
#endif
    }
    f=1;
    }
   }
  else
  {
   if (w>=-2)
   {
	   attr.cpos &= CSTR_p_large+CSTR_p_small+CSTR_p_t;
#ifdef __POS_PROT__
   p_prot(9);
#endif
   }
   else
   {
    if (w>=-4)
	{
		attr.cpos &= CSTR_p_large+CSTR_p_small+CSTR_p_t+CSTR_p_ex;
#ifdef __POS_PROT__
		p_prot(10);
#endif
    }
    else
	{
		attr.cpos &= CSTR_p_ex;
#ifdef __POS_PROT__
		p_prot(11);
#endif
    }
   }
   f=1;
  }

  if (!f && Nb4!=-1)
  {
#ifdef __POS_PROT__
   p_prot(121);
#endif
   if (row+attr.h-bbs4>-3)
   {
	   attr.cpos &= CSTR_p_low+CSTR_p_Ql+CSTR_p_bra;
#ifdef __POS_PROT__
		p_prot(122);
#endif
   }
   else
   {
	   attr.cpos &= CSTR_p_all-CSTR_p_Ql-CSTR_p_low;
#ifdef __POS_PROT__
		p_prot(13);
#endif
   }
  }
  if (!attr.cpos)
  {
	  attr.cpos=CSTR_p_large;
#ifdef __POS_PROT__
	  p_prot(14);
#endif
  }
 }

#ifdef __POS_PROT__
 p_prot(9000+attr.cpos);
#endif

 CSTR_SetAttr(c,&attr);
}
 /*********************************
 **********************************/
/************

 <upper_type> * 16 + <down_type>
 upper_types:
        1: large  -    l
        2:   small ("ps" size) at bs2
        3:   t [ ] { }
        4:   ( )
        5:   g, p -- bs2 or upper
        10 = 2+8: j, y -- at bs2, not a "ps" size
        0:   free

 down_types :	1: at base-3 lst
		2: sunk      dQ
		3: p         dqQ(
		4: !?
		5: []{}	     qQ(
		6: ()f       lower than bbs3 or at bbs3
		0: free
************/

char tblg[]={0x16,0x16,0x16,0x16,0x31,0x21,0x10,0x10,0x16,0x11,0x12,0x11,0x11,0, 0, 0  };
//   	       b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   bb   bc   bd   be bf c0
//             fi   fl   ff   ffi  rt   ri   %    %   ffl    i.   j.   !.   ?.

char tblgt[]={1,1,1,1,4,2,1,1,1,1,1,4,4,0,0,0};
char tbsl[]={"&$/%()+!#{}"};
char tbsn[]={0x11,0x16,0x16,0x11,0x46,0x46,0,0x14,0x04,0x35,0x35};
//              &    $    /    %    (    ) +    !    #    {    }
//

char tblt[]={(char)0x11,(char)0x11,(char)0x11,(char)0x16,(char)0x10,(char)0x16,(char)0x11,(char)0x16,(char)0x11,(char)0x16,
//           0    1    2    3    4 ?? 5    6    7    8    9
//
	       00,  00,(char)0x01, 00,(char)0x01,(char)0x14,(char)0x11,
//	        :    ;    <   =    >    ?    @
//
  (char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x16,(char)0x11,(char)0x11,(char)0x11,(char)0x11,
//   A    B    C    D    E    F    G    H    I    J    K    L    M    N
//
  (char)0x11,(char)0x11,(char)0x16,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,(char)0x11,
//   O    P    Q    R    S    T    U    V    W    X    Y    Z
//
	     (char)0x35,(char)0x10,(char)0x35,00,00,00,
//	        [    \    ]  ^  _  '

  (char)0x21,(char)0x11,(char)0x21,(char)0x11,(char)0x21,(char)0x16,(char)0x52,(char)0x11,(char)0x21,(char)0xa2,(char)0x11,(char)0x11,(char)0x21,(char)0x21,
//   a    b    c    d    e    f    g    h    i    j    k    l    m    n
//
  (char)0x21,(char)0x53,(char)0x22,(char)0x21,(char)0x21,(char)0x31,(char)0x21,(char)0x21,(char)0x21,(char)0x21,(char)0xa2,(char)0x21};
//   o    p    q    r    s    t    u    v    w    x    y    z
//
//

// NON-FAX penalties

#ifdef PEN_TAB
static unsigned char ptu113[9][9] =
  {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  // dist (b2-b1)=0
       0,  0,  0,  0,  0,  0,  0,  0,  0,  // dist (b2-b1)=1
      64,  0,  0,  0,  0,  0,  0,  0,  0,  // dist (b2-b1)=2

     240, 54,  0,  0,
                       0,  0,  0,  0,  0,  // dist (b2-b1)=3
//           |
     240,110, 36,  0,  0,
                           0,  0,  0,  0,  // dist (b2-b1)=4
//             |
     240,130, 96, 36,  0,  0,
                               0,  0,  0,  // dist (b2-b1)=5
//               |
     240,140, 48, 36, 12,  0,  0,
                                   0,  0,  // dist (b2-b1)=6
//                 |
     240,170, 64, 48, 32, 12,  0,  0,
                                       0,  // dist (b2-b1)=7
//                   |
     240,180, 80, 54, 36, 24, 12,  0,  0   // dist (b2-b1)=8
//                     |
  };
// small above b2
static unsigned char ptu213[9][9] =
  {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  // dist (b2-b1)=0
       0,  0,240,240,240,240,240,240,240,  // dist (b2-b1)=1
       0,  0,128,240,240,240,240,240,240,  // dist (b2-b1)=2
       0,  0, 96,240,240,240,240,240,240,  // dist (b2-b1)=3
       0,  0, 64,170,240,240,240,240,240,  // dist (b2-b1)=4
       0,  0, 24,140,180,240,240,240,240,  // dist (b2-b1)=5
       0,  0, 18,128,180,180,240,240,240,  // dist (b2-b1)=6
       0,  0, 12, 96,140,180,240,240,240,  // dist (b2-b1)=7
       0,  0, 12, 54,128,140,180,240,240   // dist (b2-b1)=8
  };
static unsigned char ptust113[9][9] =
  {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  // dist (b2-b1)=0
       0,  0,  0,  0,  0,  0,  0,  0,  0,  // dist (b2-b1)=1
      64,  0,  0,  0,  0,  0,  0,  0,  0,  // dist (b2-b1)=2

     240, 54,  0,  0,
                       0,  0,  0,  0,  0,  // dist (b2-b1)=3

     240,140, 36,  0,  0,
                           0,  0,  0,  0,  // dist (b2-b1)=4

     240,170,132, 36,  0,  0,
                               0,  0,  0,  // dist (b2-b1)=5
     240,180,140,128, 12,  0,  0,
                                   0,  0,  // dist (b2-b1)=6
     240,180,150,132, 54, 12,  0,  0,
                                       0,  // dist (b2-b1)=7
     240,220,170,140,128, 24, 12,  0,  0   // dist (b2-b1)=8
  };
#else
// large below b1     if b2-b1 > 8 --> prorated to (b2-b1)/8
static unsigned char ptu1138[9]={0,0,12,24,54,96,140,180,240};
static unsigned char ptu1136[9]={0,12,36,128,240,240,240,240};
static unsigned char ptu1130[9]={0,12,48,240,240,240,240};
// small above b2     (prorated to (b2-b1)/8
static unsigned char ptu2138[9]={0,12,50,180,240,240,240,240};
//static unsigned char ptu2136[9]={0,12,80,140,240,240,240,240};
static unsigned char ptu2136[9]={0,12,70,120,240,240,240,240};
static unsigned char ptu2135[9]={0,12,48,240,240,240,240,240};
#endif


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

static int16_t  top, bot, db, bs1, bs2, bs3, bs4;

// возвращается новая оценка для chr. Старая - wp
// С - только для атрибутов!
int16_t cut_by_posu(CSTR_rast C, uchar chr, int16_t wp, uchar flg, uchar arg)
{
  int16_t  dps;
  uchar dflag;
  unsigned char pen_upr;
  int16_t  up, dn;
  int16_t  i, j, ib2, bsi, b1, b2, b3, b4;
  CSTR_rast_attr attr;

  CSTR_GetAttr(C, &attr);

  // Nick 31.01.2001
  // Если базовые линии - совершенно ненадежны, ничего по
  // ним не резать !
  if( (Nb1 == 128 || Nb1 <=0) && (Nb2 == 128 || Nb2 <=0) &&
	  (Nb3 == 128 || Nb3 <=0)
     )
	 return wp;

  if (flg==1)
  {
    top=attr.row-minrow;
	bot=top+attr.h;
    db=attr.bdiff;
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
    if ((chr=='<' || chr=='>') && (bot<=(b2+b3)/2 ||
		9*attr.h>10*(b3-b1)) ||
        chr=='@' && attr.h<b3-b2
		)
     goto cut;
    return wp;
  }
  dn=up & 15; up >>= 4;
  dflag = let_lindef[chr];
   pen_up=pen_dn=0;

   if (all_caps)
   {
     // letters with twins --> penalize to allow context to overrule
     if (is_lower(chr) && twin(chr))
	 {
		 pen_up=12;
		 goto decide;
	 }

     // p is less solid
     if (chr=='p')
	 {
		 pen_up = 24;
		 goto decide;
	 }

     // smalls are forbidden
     if (let_linshape[chr] & 8)
		 pen_up = 140;   // small - penalize deadly
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
    {
		if (cpsau(bot-b3+((attr.flg_new & CSTR_fn_under)?1:0)) < 3)
	     goto cut;
	}
    if (Nb4<=0)
		break;
    if ((i=cpsu(b4-bot)) < 0)
		break;
    if (i > 7)
		goto cut;
    pen_dn=ptd2[i];
	break;
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
   if (Nb3<=0)
	   break;
   if ((i=cpsu(bot-b3)) < 0)
	   goto cut;
   if (i > 7) break;
   pen_dn=ptd7[i]; break;
  } // switch DOWN

  if ((Nb1 <= 0) && (Nb2 <= 0))
	  goto decide;

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
    if ((i=cpsu(top-b2)+4) <= 0)
		break;  // Valdemar: what is bsi ?
    if (bsi > 0 && language != LANG_RUSSIAN)
		i=cpsu(top-b2-bsi)+4;
    if (i <= 0)
		break;
    if (i > 5)
		goto cut;
    pen_up=ptu12[i];
	break;

   case 2:
    if ((Nb1<=0) && (Nb2<=0)) goto cut;
    i = (bot-top)-(b3-b2);    // must be of "Ps" size
    if (abs(i) < 2) break;    // OK !!
    if (i < -1) goto case_sm;  // too small - penalize as is
    if (i > 2)  goto case_sm;
    // i==2 allow round
    if (dflag & c_df_round)
		break;

   case 10:  // at bs2
   case_sm:
    if ((Nb1<=0) && (Nb2<=0)) goto cut;
    if ((attr.difflg & CSTR_db_up)  && (attr.h < Ps))     // small, base corrected "up"
      { b1+=1; b2+=1; b3+=1;}
    if ((attr.difflg & CSTR_db_down)  && (attr.h > Ps))     // tall, base corrected "down"
      { b1-=1; b2-=1; b3-=1;}
    if (Nb1>0)
    { if ((i=top+top-b1-b2) < 0)
        { if (bsi >= 0)  goto cut;
          if (i+bsi+bsi < 0) goto cut;
        }
    }
    if (Nb2<=0)
    {
      if ((b3-(b3-b1)*2/5) <= top)
	  {
		  pen_up=120;
		  break;
	  }

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

// для факса
// возвращается новая оценка для chr. Старая - wp
// С - только для атрибутов!
int16_t cut_by_posf(CSTR_rast C, uchar chr, int16_t wp, uchar flg, uchar arg)
{
  int16_t  dps;
  uchar dflag;
  unsigned char pen_upr;
  int16_t  up, dn;
  int16_t  i, j, bsi, b1, b2, b3, b4;
  CSTR_rast_attr attr;

  CSTR_GetAttr(C,&attr);

  if (flg==1)
  {
    top=attr.row-minrow;
	bot=top+attr.h;
    db=attr.bdiff;
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
    if ((chr=='<' || chr=='>') && (bot<=(b2+b3)/2 || attr.h>=b3-b1))
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

  if ((Nb1 <= 0) && (Nb2 <= 0))
	  goto decide;

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
    if ((attr.difflg & CSTR_db_up)  && (attr.h < Ps))     // small, base corrected "up"
      { b1+=1; b2+=1; b3+=1;}
    if ((attr.difflg & CSTR_db_down)  && (attr.h > Ps))     // tall, base corrected "down"
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
    //93.01.27if (i && (dflag & CSTR_df_round)) i--;
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
    //// if ((pen_up >= 20) && (dflag & CSTR_df_round)) pen_up -= 20;
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

int16_t cut_by_pos(CSTR_rast C, uchar chr, int16_t wp, uchar flg, uchar arg)
{
  if (actual_resolution > 64 )
  {
    DPs  = (Ps  * 300 )/ actual_resolution;
    DPsf = (Psf * 300 )/ actual_resolution;
    if( DPs<1  ) DPs=1;
    if( DPsf<1 ) DPsf=1;
  }
  else
  {
	  DPs = Ps; DPsf = Psf;
  }

  if( no_linpen )
    return wp;
  else if (fax1x2)
    return cut_by_posf(C, chr, wp, flg, arg);
  else
    return cut_by_posu(C, chr, wp, flg, arg);
}

// проверить - изменить оценки версий по БЛ
int16_t levcut(CSTR_rast C, int16_t arg)
{
 int16_t ncut, nalive, wp, np;
 int16_t pold, pnew, flcv;
// version  *v0;
 uchar  chr, surviver;
 UniVersions vers;
 int i;
 CSTR_rast_attr attr;

 CSTR_GetAttr(C,&attr);

 top=attr.row-minrow;
 bot=top+attr.h;
 db=attr.bdiff;

 flcv = 0;
 nalive=ncut=0;

// v0=C->vers;
// if (v0->let == bad_char)
//	 return 0;  // bad cell
 CSTR_GetCollectionUni(C,&vers);
 if( vers.lnAltCnt <= 0 )
	 return 0;

 ntot_cells++;
 attr.bas_acc &= ~CSTR_ba_killed;
 CSTR_SetAttr(C,&attr);

 if( arg && (attr.flg & CSTR_f_solid))
	 return 0; // Valdemar 12-21-95 08:35pm

 def_locbas(C);
 bs1=bbs1+db; bs2=bbs2+db; bs3=bbs3+db; bs4=bbs4+db;

// for (; v0->let != 0;  v0++)
 for(i=0;i<vers.lnAltCnt;i++)
 {
   //chr=v0->let;
   //wp=v0->prob;
   chr = vers.Alt[i].Liga;
   wp  = vers.Alt[i].Prob;

   pold = wp & 1;
   wp &= 0xfe;

//   if (chr==bad_char)
//	   return 0;

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
       // v0->prob = np & 0xfe;
	   vers.Alt[i].Prob = np & 0xfe;
     else
       //v0->prob &= 0xfe;           // remove old mark
	   vers.Alt[i].Prob &= 0xfe;
   }
   else
   {
     if (pold==0)
		 ncut++;
     pnew = 1;
     if (arg)
	 {
      // v0->prob=v0->let=0;
	     vers.Alt[i].Prob = 0;
		 vers.Alt[i].Liga = 0;
	 }
     else
       //v0->prob |= 1;          // mark as cut
	   vers.Alt[i].Prob |= 1;    // mark as cut
   }

   if (pold != pnew)
     flcv=1;
 }  // FOR versions

 CSTR_StoreCollectionUni(C,&vers);

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
 {
// version *v0;
   uchar chr;

   CSTR_GetCollectionUni(C,&vers);
// for (v0=C->vers; (chr=v0->let) !=0; v0++)
   for(i=0;i<vers.lnAltCnt;i++)
   {
     chr = vers.Alt[i].Liga;
     if(! memchr("LTJ()<>[]trI1l!/",chr,16) && (chr!=liga_i) &&
		 !(language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(chr==i_sans_accent||chr==II_dot_accent)
		  ) &&
		 (chr!=liga_inv_exm)
	   )
       goto all_cut;
   }
   goto retncut;

 all_cut:
   attr.bas_acc |= CSTR_ba_killed;
   CSTR_SetAttr(C,&attr);
   ncut_cells++;
 }
retncut:
 return ncut;
}
///////////
void gen_reset()
{
 int16_t nc, sm;
 CSTR_rast C;
 CSTR_rast_attr attr;
 //version *v0;
 UniVersions vers;
 int i;
 int16_t wc, wr;

 all_diffs_made = 0;
 minrow=mincol=32000;
 maxrow=maxcol=0;
 nc = 0;
 sm = 0; // Piter add init sm=0

 C=cell_f();
 C=CSTR_GetNext(C);
//while ((C=C->next)->next)
 for(;C;C=CSTR_GetNext(C))
 {
   CSTR_GetAttr(C,&attr);

   if (attr.flg & CSTR_f_space)
	   continue;

   CSTR_GetCollectionUni(C,&vers);

   wc=attr.col;
   wr=attr.row;

   if (minrow > wr) minrow=wr;
   if (maxrow < (wr+attr.h))
	   maxrow=wr+attr.h;
   if (mincol > wc)
	   mincol=wc;
   if (maxcol < wc)
	   maxcol=wc;       // min & max s_t_a_r_t !!!  COLs

   attr.bas_acc &=~(CSTR_ba_chance);

   if (attr.flg & (CSTR_f_let|CSTR_f_bad))
   {
     //if (C->vers[0].let == bad_char) set_bad_cell(C);
     if( vers.lnAltCnt <= 0 )
	 {
		 CSTR_SetAttr(C,&attr);
		 set_bad_cell(C);
		 // изменились при set_bad_cell !!
		 CSTR_GetAttr(C,&attr);
	 }

     attr.basflg=attr.bas1=attr.bas2=attr.bas3=attr.bas4=0;
     attr.bdiff = 127;
     if (attr.flg & CSTR_f_let)
       cells_inln++;
     nc++;
     sm = sm + attr.row-minrow+attr.h;

//     for (v0=C->vers; v0->let !=0; v0++)
//       v0->prob  &= 0xfe;
	 CSTR_GetCollectionUni(C,&vers);
	 for(i=0; i < vers.lnAltCnt;i++)
		 vers.Alt[i].Prob &= 0xfe;

	 CSTR_StoreCollectionUni(C,&vers);
   }

   CSTR_SetAttr(C,&attr);
 }

 midcol=(maxcol+mincol)>>1;
 if (nc)
   bbs3 = sm/nc;
}
/*
//////////////
// черновое определение БЛ
//
// Для новой	 версии :
// Или 2 строки - наша новая вмеcто cell и ln - как была,
// оттуда брать dust
// Или специально пометить dust в ln
// Или пересмотреть defbas()
// Или ??

void basedraft(CSTR_line ln)
{
 char riter1, riter0;
 int16_t i;
 CSTR_rast C;

 //if (line_number == 0)
 if ( line_number <= 1 )
	 oldPs2 = -1;
 else
 {
   oldPs2=bbs3-bbs2;
   oldPs1=bbs3-bbs1;
   oldPs4=bbs3-bbs4;
   oldn1=Nb1; oldn2=Nb2; oldn4=Nb4;
   oldmult = multi_bas;
 }

 #ifdef __POS_PROT__
 os2 = 0;

 C=cell_f();
 while ((C=C->nextl)->nextl)
 {
  if (
       (
        ((C->row==1667) && (C->col==491) && (line_number==26) ) ||
        ((C->row==59) && (C->col==139) && (line_number==1) )
      ) &&
      (C->vers[0].let=='y')
    )
    os2 = 1;
 }
 #endif

 lin_str = ln;
 lin_pass = 1;
 dust_in = 0;
 riter0=2;
 it_done=0;

again_for_dust:
 riter0--;
 let_to_dust = 0;
 diff_curv = 0;
 change_vote=0;
 hist_done = 0;
 oNb1=oNb2=oNb3=oNb4=0;
 cells_inln = 0;
 gen_reset();
 trans_total = 0;
 fl_artifact = 0;
 ntot_old = ncut_old = nmult_old = ndead_old = nvers_old = 0;

 // сбор статистики, поиск БЛ
 defbas(0);

 bsdust_ps = Ps;
 sum_letlth = sum_cellth = 0;
 ncut_vers=ncut_cells=ntot_cells=0;
 i = bbs2 - (bbs3-bbs2)/2;
 bsdust_upper = MIN(i,bbs1) - 2 + minrow;
 if (language != LANG_ENGLISH)
   bsdust_upper -= (MAX(2,(bbs3-bbs2)/7));
 bsdust_lower = bbs4 + minrow;

 if (db_status)
 { char ww[128];
   sprintf(ww, "for dust: Ps=%d upper=%d lower=%d",
              bsdust_ps, bsdust_upper, bsdust_lower);
   glsnap ('d',cell_f()->next,ww);
 }

 riter1 = 0;
 while (riter1 < 3)
 {
   if ((forbiset) && (db_status))
     li_snap("forbid=");
   trans_total = forbiset;
   hist_done = 0;

//   add dust from lin_str (riter1==0),
//   check dust-letter
   bring_dust(riter1);


//   if (cell_f()->nextl->next == NULL)
   if( !CSTR_GetNextRaster(cell_f(),f_letter) )
   	   goto drfin;  // line emptied


// найти-уточнить наклон nIncline
   skew_corr(ln,1);  // skew_corr(ln,0); if CSTR_line

   C=cell_f();
//   while (C && C->next && (C=C->next)->next != NULL)            //AK add c/g 06.03.98
   for(C=CSTR_GetNext(C);C;C=CSTR_GetNext(C))
     ideal_rc(C);

   if (!trans_total)
	   break;
   if (riter0)
	   goto again_for_dust;

   if (fl_artifact > 1)
	   break;

   // уточнить БЛ
   defbas(0);
   riter1++;
 }

drfin:
 if (db_status)
	 li_snap("draft fin");
 bbs0 = min_crow;
}
*/
/****************************************************/
void approve_bases()
{
  int16_t appr;
  appr = 0;

  if (bbs1 >= (bbs2-1))
  {
    if (Ns2 > Ns1)
      bbs1=(3*bbs2-bbs3)>>1;
    else
      bbs2=(2*bbs1+bbs3)/3;
    appr = 1;
  }
  if (bbs4 < (bbs3+1))
    { bbs4=(3*bbs3-bbs2)>>1; appr = 1; }
  if (appr)
    set_basarr(&all_bases[0],-32000,32000);
}
/****************************************************/
// уточнить БЛ, обрезать версии, противоречащие БЛ
void linpos(CSTR_line ln)
{
 int16_t riter1;
 int32_t sumbox;
 int16_t sum_n, finCSTR_n, dead_cells=0,v_prom;

 lin_str = ln;

//  iterate dust_to_let AND let_to_dust

// if (cell_f()->nextl->next == NULL)
 if( !CSTR_GetNextRaster(cell_f(),f_letter) )
	 return;  // line emptied

 //////////  931020   if (fl_artifact > 1) return;
 finCSTR_n = 0;
 ///////////// ???  cut_sunk_let();    //////// ???????????? 940104
#ifdef __POS_PROT__
p_prot (10000+line_number);
#endif

all_again:
 hist_done = 0;


 lin_pass = 2;
 histiter = 0;     // forget previosly done iterations
 krit_hist = 0;

 gen_reset();

 it_done=riter1 = 0;
 while (riter1 < 10)
 {
//   if (cell_f()->nextl->next == NULL)
   if( !CSTR_GetNextRaster(cell_f(),f_letter) )
	 return;  // line emptied

   multi_bas = 0;
   ntot_old  = ntot_cells;
   ncut_old  = ncut_cells;
   nmult_old = ncut_mult;
   ndead_old = dead_cells;
   nvers_old = ncut_vers;
   ntot_cells=ncut_vers=ncut_cells=ncut_mult=killed_box=sum_n=dead_cells=0;

// найти БЛ
   defbas(f_cut);          // histogramms allowed now

   if (multi_bas)
	   break; // 1:b1 split; 2:b2 split
   if (histofl > 1)
	   break; // multiple base case
   if (page_stat)
	   break; // Valdemar: base lines defined by page statistic
   it_done++;
   all_cell_levcut(0);   // forbid versions

#ifdef __POS_PROT__
p_prot (11001);
#endif

   ncut_mult=killed_box=sum_n=v_prom=0;
   if (ncut_cells)
   {
     CSTR_rast wc;
     CSTR_rast_attr attr;

     sum_n=ncut_mult=dead_cells=0;
     sumbox=0;

//   for (wc=(cell_f())->nextl; wc->nextl!=NULL; wc=wc->nextl)
	 wc = CSTR_GetNextRaster(cell_f(),f_letter);
	 for(;wc;wc=CSTR_GetNextRaster(wc,f_letter))
     {
       CSTR_GetAttr(wc,&attr);
       if (attr.bas_acc & CSTR_ba_killed)
       {
         if ( (attr.basflg & (CSTR_bs_b1a | CSTR_bs_b2a)) == (CSTR_bs_b1a | CSTR_bs_b2a))
         {
		   ncut_mult++;
		   continue;
		 }
         // agrees to stay as cap and as small --- killed by unstable (splitted) lines
         if ( (attr.bas_acc & CSTR_ba_chance) || language != LANG_RUSSIAN )
         {
           CSTR_rast wd;
		   CSTR_rast_attr attr2;
		   int16_t weight;
		   UniVersions vers;
		   uchar chr;

//  no scaling h,w,... - we scaled on input to BAL !
           weight = RSTR_rast_is_BOX_solid(wc,0);     // solid BOX estimate -- killed
           // 940105
           //wd = wc->next;
		   wd = CSTR_GetNext(wc);
		   CSTR_GetAttr(wd,&attr2);
		   CSTR_GetCollectionUni(wc,&vers);
		   if( vers.lnAltCnt<=0)
			   chr = 0;
		   else
			   chr = vers.Alt[0].Liga;

           if (
                (attr2.flg & CSTR_f_dust)       &&
                (attr.col+attr.w > attr2.col)   &&
//                (wc->vers[0].let != 'i')    &&
//                (wc->vers[0].let != 'j')
                chr != 'i' &&
				chr != 'j'
              )
              weight -= 500;

           sumbox += weight;
           if (weight >= 31000) dead_cells++;
           sum_n++;
           //if ( cell_is_BOX_solid(wc) > 31000)     // solid BOX estimate -- killed
           //killed_box ++;
         }
        else
        {
         uint16_t iI,nv_o;
         int16_t pidx;
		 uchar let;
		 UniVersions vers;

         attr.bas_acc|=CSTR_ba_chance;  // try one more
		 CSTR_SetAttr(wc,&attr);

         CSTR_GetCollectionUni(wc,&vers);

         //nv_o=wc->nvers;
		 nv_o = (int16_t)vers.lnAltCnt;
         pidx=prop_index(attr.h,attr.w);

//         for(iI = 0; iI < wc->nvers; iI++)
		 for(iI = 0; iI < vers.lnAltCnt; iI++)
         {
		  // insert twin
          // may be - replace some version

          //let = wc->vers[iI].let;
          let = vers.Alt[iI].Liga;
          if(!(attr.flg & CSTR_f_let) || !isletter(let))
			  continue;
          if(!twin(let))
			  continue;
          let = is_lower(let) ? to_upper(let) : to_lower(let);
          if( !el_pidx_crit((uint16_t)let,pidx) )
			  continue;

		  // версия вставляется и переупорядочивается!
		  //
          promote(0,wc,let,0); // insvers

		  // возможно, иные значения !
		  CSTR_GetCollectionUni(wc,&vers);

          // wc->vers[wc->nvers].let=wc->vers[wc->nvers].prob=0;
         } // loop on vers

         if( vers.lnAltCnt > nv_o )
		 {
			 v_prom++;
             CSTR_StoreCollectionUni(wc,&vers);
		 }
        }  // else
       }
     }

     if (sum_n)
		 killed_box = sumbox/sum_n;

     if(v_prom > 0)
     {  // ins sum versions
      all_cell_levcut(0);
     }
   }
//   if (db_status) li_snap("cut:");
   if (db_status)
   {
     char txt[128];
     sprintf(txt,"cut=%d killed=%d/%d dead=%d",ncut_mult,killed_box,sum_n,dead_cells);
     li_snap(txt);
   }

   if (ncut_vers+v_prom == 0)
     break;

   obs1=bbs1; obs2=bbs2; obs3=bbs3; obs4=bbs4; oPs=Ps; obsm=bbsm;
   oNb1=Ns1;  oNb2=Ns2;  oNb3=Ns3;  oNb4=Ns4;
   minold = minrow;
   riter1++;
   if (fl_artifact) break;
 }

// many killed cells --- assume multibase case
///  931020  if ((4*ncut_cells > ntot_cells)  && (ntot_cells > 10) && (all_caps == 0))
#ifdef __POS_PROT__
p_prot (12000+multi_bas);
p_prot (12000+ncut_mult);
p_prot (12000+dead_cells);
p_prot (killed_box);
#endif

 if  (multi_bas)
   goto try_multi;
 if (all_caps)
   goto no_multi;

 if(language == LANG_RUSSIAN) // Valdemar
 {
  if( ntot_cells > 10 )
  {
   if( ncut_mult > 2 || (dead_cells > 2 && killed_box > 32000) )
    goto try_multi;
   else
    goto no_multi;
  }
 }

 if (ntot_cells > 10)
   if ((ncut_mult > 1)   || (killed_box > 32000) || (dead_cells > 1))
     goto try_multi;

 if (ntot_cells > 5)
   if ((killed_box > 32000) && (dead_cells > 1))
 {
 try_multi:
   if(language == LANG_RUSSIAN && !stable_b3)
	   goto no_multi;  // gleb
   if (multi_bas == 0)
	   multi_bas = 4;
   histofl = 3;
   obs1=bbs1; obs2=bbs2; obs3=bbs3; obs4=bbs4; oPs=Ps; obsm=bbsm;
   oNb1=Ns1;  oNb2=Ns2;  oNb3=Ns3;  oNb4=Ns4;
   minold = minrow;

#ifdef __POS_PROT__
p_prot(11002);
#endif

   multi_hist(multi_bas);
   if (bs_int_no > 5)
   // too many intervals ----> unable to make multi_base
   {
     bbs1=obs1; bbs2=obs2; bbs3=obs3; bbs4=obs4; Ps=oPs; bbsm=obsm;
     Ns1=oNb1;  Ns2=oNb2;  Ns3=oNb3;  Ns4=oNb4; minrow = minold;
     set_basarr(&all_bases[0],-32000,32000);
     multi_bas &= 0x3f;
   }

   if (bs_int_no==1)
   {
     bas_ln *bw;
     int16_t fl;
     fl = 0;
     bw = &all_bases[0];
     bbs1 = bw->b1; bbs2 = bw->b2; bbs3 = bw->b3; bbs4 = bw->b4;
     Ns1  = bw->n1; Ns2  = bw->n2; Ns3  = bw->n3; Ns4  = bw->n4;
     Ps   = bw->ps;
     if ((Ns4<=0) && (oNb4>0))  { Ns4=oNb4; bbs4=obs4-minrow+minold; }
     if (fl) Ps = bbs3-bbs2;
     bbsm = (bbs2 + bbs3)/2;
     set_basarr(&all_bases[0],-32000,32000);
     multi_bas &= 0x7f;
   }
 }

no_multi:
 /*****************************/
 #ifdef __MULTI_PROT__
 if (multi_bas & 7)
 {
   FILE *ff; int16_t mi;
   mi = multi_bas & 15;
   if (multi_bas & 64)  mi+=100;
   if (multi_bas & 128) mi+=200;
 ff = fopen ("multi.pro","a");
 fprintf(ff,"%s%2d %2d %3d --> %d\n ",group_name,page_number,line_number+1,mi,bs_int_no);
 fclose (ff);
 }
 #endif
 /*****************************/

 if (multi_bas == 0)
   approve_bases();

 all_cell_levcut(1);    // apply penalties

#ifdef __POS_PROT__
p_prot(11003);
#endif

 bbs0 = min_crow;
 if (fincells() && (finCSTR_n++ < 3))
 {
   if (db_status) glsnap('d',cell_f()->next,"linpos repeat");
   goto all_again;
 }
 if (db_status) li_snap("linpos ret:");
}

///////////////
static int16_t influate(CSTR_rast c, CSTR_rast b)
{
  uchar in_flag, cc, bc, dfc, dfb;
  int16_t wp, dp, dp2;
  //version  *v0;
  UniVersions vers;
  int i;

  in_flag = 0;

  CSTR_GetCollectionUni(c,&vers);
  if( vers.lnAltCnt > 0 )
	  cc = vers.Alt[0].Liga;
  else
	  cc = 0;

//  dfc = let_lindef[(cc = c->vers[0].let)];
  dfc = let_lindef[cc];

//  v0 = b->vers;
  CSTR_GetCollectionUni(b,&vers);

//  for (; v0->let != 0;  v0++)
  for(i=0;i<vers.lnAltCnt;i++)
  {
    //bc = v0->let;
    bc = vers.Alt[i].Liga;
    if (bc == 't')
		continue;   // don't touch  't'
    //wp = v0->prob;
	wp = vers.Alt[i].Prob;

    dfb = let_lindef[bc];

    cut_by_pos(b,bc,254,1,1);

    if ((dp=pen_up) <= 0)
      continue;   // not touched by LINEAR

    dp2 = dp/2;
    if (bbs2-bbs1 < 4)
      dp = dp2;    // close b1, b2 --> weak influence
    dp &= 0xfe;
    dp2 &= 0xfe;
    if (dfc & CSTR_db_b2)   // major small
    {
      if (dfb & (CSTR_db_b2 | c_df_b2a))  // minor small or agrees to be small
        goto add_dp;
      if (dfb & (CSTR_db_b1 | c_df_b1a))  // minor large or agrees to be large
        goto sub_dp2;
    }
    if (dfc & CSTR_db_b1)   // major large
    {
      if (all_caps)   // don't touch small by capital's influence
        if (is_lower(bc))
          continue;
      if (dfb & (CSTR_db_b1 | c_df_b1a))  // minor large or agrees to be large
        goto add_dp;
      if (dfb & (CSTR_db_b2 | c_df_b2a))  // minor small or agrees to be small
        goto sub_dp2;
    }
    continue;
  add_dp:
    in_flag = 1;
    wp += dp;
    if (wp > 254)
		wp = 254;
    //v0->prob = wp & 0xfe;
    vers.Alt[i].Prob = wp & 0xfe;
    continue;

  sub_dp2:
    in_flag = 1;
    wp -= dp2;
    if (wp < 0) wp = 0;
    // v0->prob = wp & 0xfe;
	vers.Alt[i].Prob = wp & 0xfe;
    continue;
  }  // FOR all versions

  if (in_flag)
  {
    CSTR_rast_attr attr;
	CSTR_GetAttr(b,&attr);
    //b->difflg |= CSTR_db_b4;   // indicate: influented
	attr.difflg |= CSTR_db_b4;   // indicate: influented
    CSTR_SetAttr(b,&attr);

	CSTR_StoreCollectionUni(b,&vers);
    sort_vers(b);
  }

  return in_flag;
}
/////////////////

 int16_t influ_cap(CSTR_rast b)
{
  uchar in_flag, bc;
  int16_t wp;
//  version  *v0;
  UniVersions vers;
  int i;

  in_flag = 0;

//  v0 = b->vers;
  CSTR_GetCollectionUni(b,&vers);
//  for (; v0->let != 0;  v0++)
  for (i=0; i<vers.lnAltCnt;  i++)
  {
    //bc = v0->let;
    bc = vers.Alt[i].Liga;
    //wp = v0->prob;
	wp = vers.Alt[i].Prob;

    // letters with twins --> penalized 12 to allow context to overrule
    if (is_lower(bc) && twin(bc))
	{
		wp += 128;
		goto make_dp;
	}

    // p is less solid
    if (bc=='p')
	{
		wp += 24; goto make_dp;
	}

    // letters with twins --> penalized 12 to allow context to overrule
    if (is_upper(bc) && twin(bc))
	{
		wp -= 100; goto make_dp;
	}

  make_dp:
    in_flag = 1;

    if (wp > 254) wp=254;
    if (wp < 0) wp = 0;

    //v0->prob = wp & 0xfe;
	vers.Alt[i].Prob = wp & 0xfe;
    continue;
  }  // FOR all versions

  if (in_flag)
  {
    CSTR_StoreCollectionUni(b,&vers);
    sort_vers(b);
  }

  return in_flag;
}
////////////////

static int16_t influ_b1b2(CSTR_rast b)
{
  uchar in_flag, bc;
  int16_t wp, top, db, b1, b2, b12, d12;
//  version  *v0;
  UniVersions vers;
  int i;
  CSTR_rast_attr attr;

  CSTR_GetAttr(b,&attr);

  top=attr.row-minrow;
  db=attr.bdiff;

  def_locbas(b);

  b1=bbs1+db; b2=bbs2+db;
  b12 = (b1+b2)/2;
  d12 = top - b12;

  in_flag = 0;

//  v0 = b->vers;
  CSTR_GetCollectionUni(b,&vers);
//  for (; v0->let != 0;  v0++)
  for (i=0; i<vers.lnAltCnt;  i++)
  {
    //bc = v0->let;
    bc = vers.Alt[i].Liga;
    //wp = v0->prob;
	wp = vers.Alt[i].Prob;
    if (d12 <= 0 )
		continue;   // like capital -- wasn't penalized
    if (is_upper(bc) && twin(bc))
	{
	  wp -= 48;
	  goto make_dp;
	}
    // letters with twins --> penalized 12 to allow context to overrule
    if (is_lower(bc) && twin(bc))
	{
		wp += 12;
		goto make_dp;
	}
    // p is less solid
    if (bc=='p')
	{
		wp += 24; goto make_dp;
	}

    if (3*attr.h > 2*Ps)
      wp += 140;    // small (not too small !!)
                    // still alive below b12 -- depenalize it
  make_dp:
    in_flag = 1;

    if (wp > 254) wp=254;
    else if (wp < 0) wp = 0;

    //v0->prob = wp & 0xfe;
	vers.Alt[i].Prob = wp & 0xfe;
    continue;
  }  // FOR all versions

  if (in_flag)
  {
    CSTR_StoreCollectionUni(b,&vers);
    sort_vers(b);
  }
  return in_flag;
}

static void mutual_influence()
{
 CSTR_rast C;
 CSTR_rast_attr attr;
 int16_t crow, brow, suminfl;
 suminfl = 0;

 C=cell_f();
 C=CSTR_GetNextRaster(C,f_letter);
// while ((C=C->nextl)->nextl)
 for(;C;C=CSTR_GetNextRaster(C,f_letter))
 {
  CSTR_GetAttr(C,&attr);
  attr.difflg &= ~CSTR_db_b4;  // use  b4 as "solid influented"
  CSTR_SetAttr(C,&attr);
 }

 C=cell_f();
 C=CSTR_GetNextRaster(C,f_letter);
// while ((C=C->nextl)->nextl)
 for(;C;C=CSTR_GetNextRaster(C,f_letter))
 {
   CSTR_GetAttr(C,&attr);
   if (!(attr.flg & CSTR_f_let)) continue;

   if (attr.difflg & CSTR_db_solid)
   { CSTR_rast B;
     CSTR_rast_attr attrb;

     crow = attr.row - attr.bdiff;
     B=C;
//     while ((B=B->nextl)->nextl)
	 B=CSTR_GetNextRaster(B,f_letter);
     for(;B;B=CSTR_GetNextRaster(B,f_letter))
     {
       CSTR_GetAttr(B,&attrb);
       if (!(attrb.flg & CSTR_f_let)) continue;
       if (attrb.difflg & CSTR_db_solid) break;
       brow = attrb.row - attrb.bdiff;
       if (abs(crow-brow) < 2)
         suminfl += influate(C,B);
       else
         break;
     }

	 // могли измениться флаги!
	 CSTR_GetAttr(C,&attr);
     B=C;
//     while ((B=B->prevl)->prevl)
	 B=CSTR_GetPrevRaster(B,f_letter);
	 for(;B;B=CSTR_GetPrevRaster(B,f_letter))
     {
	   CSTR_GetAttr(B,&attrb);
       if (!(attrb.flg & CSTR_f_let))
		   continue;

	   // вынести ?
       if ( attr.difflg & CSTR_db_b4)
		   continue; // already influented

       if (attrb.difflg & CSTR_db_solid)
		   break;
       brow = attrb.row - attrb.bdiff;
       if (abs(crow-brow) < 2)
         suminfl += influate(C,B);
       else
         break;
     }
   } // IF major solid
 }   // WHILE all cells

 if (suminfl)
   glsnap('d',cell_f()->next,"linear influence");
 if (all_caps)
 {
   suminfl = 0;

   if (all_doubts & 32)   // b1 splitted -- use influence
   { //   b1 splitted ---> retain penalties
     /***************************
     C=cell_f();
     while ((C=C->nextl)->nextl)
     {
       if (!(C->flg & CSTR_f_let)) continue;
       if (C->difflg & CSTR_db_b4) continue;  // influented
       suminfl += influ_cap(C);
     }
     *****************************/
   }
   else  // b1 solid -- use b1 versus b2
   {
     C=cell_f();
	 C=CSTR_GetNextRaster(C,f_letter);
//     while ((C=C->nextl)->nextl)
     for(;C;C=CSTR_GetNextRaster(C,f_letter))
     {
       CSTR_GetAttr(C,&attr);
       if (!(attr.flg & CSTR_f_let))
		   continue;
       suminfl += influ_b1b2(C);
     }
   }
   if (suminfl)
     glsnap('d',CSTR_GetNext(cell_f()),"all_caps influence");
 }
}
////////////////////
int16_t count_line_hi(void); // only for russian

// окончательная установка/проверка линий
void basefin(CSTR_line ln)
{
 CSTR_rast B1;
// version *v0;
 UniVersions vers;
 int i;

 lin_str = ln;

 it_done=0;
 if (multi_bas & 0x40)
	 return;
 // if (all_bases[0].endcol != 32000) return;  // array of bases
 if (fl_artifact > 1)
	 return;

// if (cell_f()->nextl->next == NULL)
 if( !CSTR_GetNextRaster(cell_f(),f_letter) )
	 return;  // line emptied

 if(language == LANG_RUSSIAN)
	 count_line_hi(); // collect some info about line
 mutual_influence();

 lin_pass = 3;
 obs1=bbs1; obs2=bbs2; obs3=bbs3; obs4=bbs4; oPs=Ps; obsm=bbsm;
 oNb1=Ns1;  oNb2=Ns2;  oNb3=Ns3;  oNb4=Ns4;

 // найти БЛ
 defbas(f_retain);

 if (histofl   // switched to histo ---> forget
    || Ps>72)  // Oleg : 10-27-95 07:24pm : exist this bad virtual strings
 {
   bbs1=obs1; bbs2=obs2; bbs3=obs3; bbs4=obs4; Ps=oPs; bbsm=obsm;
   Ns1=oNb1;  Ns2=oNb2;  Ns3=oNb3;  Ns4=oNb4;
  if (db_status)
  { char ww[128];
    sprintf(ww, "use LINPOS bases: Ps=%d Bs: %d %d %d %d",
                 Ps, bbs1, bbs2, bbs3, bbs4, Ns1, Ns2, Ns3, Ns4);
       glsnap ('d',cell_f()->next,ww);
  }
 }

// B1=cell_f();
 B1=CSTR_GetNextRaster(cell_f(),f_letter);
//  while ((B1=B1->nextl)->nextl)
 for(;B1;B1=CSTR_GetNextRaster(B1,f_letter))
 {
   CSTR_GetCollectionUni(B1,&vers);
//   for (v0=B1->vers; v0->let != 0; v0++)
   for(i=0;i<vers.lnAltCnt;i++)
//     v0->prob &= 0xfe;
       vers.Alt[i].Prob &= 0xfe;

   CSTR_StoreCollectionUni(B1,&vers);
 }

}
////////////////////

void interdif(CSTR_rast P, CSTR_rast N, CSTR_rast B1)
{
 int16_t bsc, bsp, bsn, wdn, wdp, clp, cln, clc;
 CSTR_rast_attr attrP,attrN,attrB;

 CSTR_GetAttr(P,&attrP);
 CSTR_GetAttr(N,&attrN);
 CSTR_GetAttr(B1,&attrB);

 bsp=attrP.h+attrP.row-minrow;
 bsn=attrN.h+attrN.row-minrow;

 clp=attrP.col;
 cln=attrN.col;

 wdp=attrP.bdiff;
 wdn=attrN.bdiff;

 bsc=attrB.h+attrB.row-minrow;
 clc=attrB.col;

 if (attrB.flg & (CSTR_f_let|CSTR_f_bad))
 {
 // 93.01.26 to be done for letters only;
    attrB.difflg &= ~(CSTR_db_down | CSTR_db_up);
	CSTR_SetAttr(B1,&attrB);
 }

 if (attrP.flg & CSTR_f_fict)
 {
   wdp = 0;
   if (attrN.flg & CSTR_f_fict)
   {
	   attrB.bdiff=0; wdn = 0;
	   goto retcv;
   }

   attrB.bdiff = (char)wdn  ;
   if (attrB.flg & (CSTR_f_let|CSTR_f_bad))
   {
     switch (bsn-bsc)
     {
       case  1: attrB.bdiff--;
		   attrB.difflg |= CSTR_db_up;
		   break;
       case -1: attrB.bdiff++;
		   attrB.difflg |= CSTR_db_down;
		   break;
       default:   //jump letter
                 break;
     }
   }
   goto retcv;
 }

 if (attrN.flg & CSTR_f_fict)
 {
   wdn = 0;
   attrB.bdiff = (char)wdp;
   if (attrB.flg & (CSTR_f_let|CSTR_f_bad))
   {
     switch (bsp-bsc)
     {
       case  1: attrB.bdiff = wdp-1;
		   attrB.difflg |= CSTR_db_up;
		   break;
       case -1: attrB.bdiff = wdp+1;
		   attrB.difflg |= CSTR_db_down;
		   break;
       default:   //jump letter
           break;
     }
   }
   goto retcv;
 }

 /* both P, N -- usual cells; defined baseline */
 if ((attrB.flg & (CSTR_f_let|CSTR_f_bad)) == 0)
	 goto interpol;  // dust - from nearest

 switch (bsp-bsc)
 {
   case  1: attrB.bdiff = wdp-1;
	   attrB.difflg |= CSTR_db_up;
	   goto retcv;
   case -1: attrB.bdiff = wdp+1;
	   attrB.difflg |= CSTR_db_down;
	   goto retcv;
   case  0: attrB.bdiff = (char)wdp  ;
	   goto retcv;
 }

 switch (bsn-bsc)
 {
   case  1: attrB.bdiff = wdn-1;
	   attrB.difflg |= CSTR_db_up;
	   goto retcv;
   case -1: attrB.bdiff = wdn+1;
	   attrB.difflg |= CSTR_db_down;
	   goto retcv;
   case  0: attrB.bdiff = (char)wdn  ;
	   goto retcv;
 }


interpol:
 if (wdp==wdn)
 {
	 attrB.bdiff=(char)wdn;
	 goto retcv;
 }

 // set diff of whoever is closer to cell
 if ((clc-clp) > (cln-clc))
   attrB.bdiff=(char)wdn;
 else
   attrB.bdiff=(char)wdp;

retcv:
  CSTR_SetAttr(B1,&attrB);

  if(Ns1+Ns2 > 0 && attrB.bdiff != 0)
  {
	  smart_diff( B1 ); // Valdemar
	  CSTR_GetAttr(B1,&attrB);
  }

  if (abs(attrB.bdiff) < 2 && language != LANG_RUSSIAN) // Valdemar
  {
   if ((wdn==0) && (wdp==0) && (diff_curv==0))
   {
     attrB.bdiff=0;
     attrB.difflg &= ~(CSTR_db_down | CSTR_db_up);
   }
   // forget diff correction when all were small (-1 <=  <= +1)
   if ((abs(wdn) < 2) && (abs(wdp) < 2) && (diff_curv==0))
     attrB.difflg &= ~(CSTR_db_down | CSTR_db_up);

   CSTR_SetAttr(B1,&attrB);
  }

}
//////////////////
void inter_diff(CSTR_rast B1)
{
 CSTR_rast P, N;
 CSTR_rast_attr attr;

 N=P=B1;
 while (1)
 {
//  N=N->next;
  N=CSTR_GetNext(N);
  if( !N )
	  break;
  CSTR_GetAttr(N,&attr);
  if (attr.flg & CSTR_f_fict)
	  break;
  if (!(attr.flg & (CSTR_f_let | CSTR_f_bad)))
	  continue;
  if ((attr.difflg & 4) && (!(attr.difflg & 64))) /* defines base */
    break;
 }

 while (1)
 {
//  P=P->prev;
  P=CSTR_GetPrev(P);
  CSTR_GetAttr(P,&attr);

  if (attr.flg & CSTR_f_fict)
	  break;
  if (!(attr.flg & (CSTR_f_let | CSTR_f_bad)))
	  continue;
  if ((attr.difflg & 4) && (!(attr.difflg & 64))) /* defines base */
    break;
 }

 interdif(P,N,B1);
}

////////////////

static int16_t fincells()
 {
 CSTR_rast B1;
 CSTR_rast_attr attr;
 int16_t top, done;
// version *v0;
 UniVersions vers;
 int i;

 done = 0;

 B1=cell_f();
 B1=CSTR_GetNextRaster(B1,f_letter);
// while((B1=B1->nextl)->nextl)
 for(;B1;B1=CSTR_GetNextRaster(B1,f_letter))
 {
   int16_t b2m, bm3, h;
//   CSTR_rast wc;

   def_locbas(B1);
   CSTR_GetAttr(B1,&attr);

   b2m = (bbs2+bbsm)/2;
   bm3 = (bbs3+bbsm)/2;

   if (  ((top=attr.row-minrow) > bm3) ||
         ((top + (h=attr.h)) < b2m)   // dust
         || (2*h <= Ps)         // small
      )
   {
     attr.flg = CSTR_f_dust;
	 CSTR_SetAttr(B1,&attr);
//	 wc = CSTR_GetPrevRaster(B1,f_letter);
//     (wc=B1->prevl)->nextl=B1->nextl;
//     B1->nextl->prevl=B1->prevl;
//     err_pnlet(B1); // AL 940319
//     B1=wc;
     done++;
     continue;
   }

   all_diffs_made = 1;          // diff defined now in all cells

   CSTR_GetCollectionUni(B1,&vers);

//   for (v0=B1->vers; v0->let != 0; v0++)
   for(i=0;i<vers.lnAltCnt;i++)
//     v0->prob &= 0xfe;
       vers.Alt[i].Prob &= 0xfe;

   if ( vers.lnAltCnt <= 0 ||
	    vers.Alt[0].Prob  < 220)
   {
     //B1->flg = CSTR_f_bad;
     attr.flg = CSTR_f_bad;
	 CSTR_SetAttr(B1,&attr);
   }

   if ( vers.lnAltCnt == 2 )
   { uchar c1, c2, p1, p2;
     c1=vers.Alt[0].Liga;
     c2=vers.Alt[1].Liga;
     if (
         (to_lower(c1)==to_lower(c2))  && twin(c1)
        )
     {
       p1=vers.Alt[0].Prob;
       p2=vers.Alt[1].Prob;
       if ((p1 > 248) && (p2 < 128))
       {  vers.lnAltCnt=1;
	      vers.Alt[1].Liga=vers.Alt[1].Prob=0;
		  CSTR_StoreCollectionUni(B1,&vers);
	   }
     }
   }
 }
 return done;
}
////////////////
// проверить все версии по БЛ
// arg !=0  - не трогать некоторые единичные версии
//
void all_cell_levcut(int16_t arg) // Valdemar makes func global
{
 CSTR_rast C;
 CSTR_rast_attr attr;
 UniVersions vers;

 C=cell_f();
 ncut_vers=ncut_cells=ntot_cells=0;

// while ((C=C->nextl)->nextl)
 for(C=CSTR_GetNextRaster(C,f_letter);C;C=CSTR_GetNextRaster(C,f_letter))
 {
  CSTR_GetAttr(C,&attr);
  attr.bas_acc &= ~CSTR_ba_killed;

  if (!(attr.flg & CSTR_f_let))
  {
	  CSTR_SetAttr(C,&attr);
	  continue;
  }

  CSTR_GetCollectionUni(C,&vers);

//  no scaling in RSTR_rast_is_BOX_solid for attr.h,w,...
//  - we scaled on input to BAL !
  if ( arg &&
       (vers.lnAltCnt==1) &&
       (memchr("4ERAFMbm",vers.Alt[0].Liga,8)) &&
       ( RSTR_rast_is_BOX_solid(C,0) > 31500)
	 )
  {
	  vers.Alt[0].Prob &= 0xfe;
	  attr.difflg |= CSTR_db_solid;
	  CSTR_StoreCollectionUni(C,&vers);
	  CSTR_SetAttr(C,&attr);
	  continue;
  }

  CSTR_SetAttr(C,&attr);
  ncut_vers+=levcut(C,arg);
 }
}
////////////
void discrim_by_dust()
{
  CSTR_rast c, d;
  CSTR_rast_attr attr,attrd;

  c=cell_f();
//  while ((c=c->nextl)->nextl)
  while( (c=CSTR_GetNextRaster(c,f_letter)) !=NULL )
  {
    CSTR_GetAttr(c,&attr);

    if (attr.difflg & 0x40)
		continue;
    if (attr.flg & CSTR_f_bad)
		continue;

    //d=c->prev;
    d = CSTR_GetPrev(c);

	if( d )
	{
	 CSTR_GetAttr(d,&attrd);

     if (attrd.flg & CSTR_f_dust)
     { if (attrd.col+attrd.w > attr.col)
        attr.difflg |= 0x40;
     }
	}

    //d=c->next;
	d = CSTR_GetNext(c);

	if( d )
	{
	 CSTR_GetAttr(d,&attrd);

     if (attrd.flg & CSTR_f_dust)
     { if (attr.col+attr.w > attrd.col)
        attr.difflg |= 0x40;
     }
	}

	CSTR_SetAttr(c,&attr);

  }
}
///////////////

void all_cell_ledust()
{
 CSTR_rast C;
 CSTR_rast_attr attr;
 int16_t df;

 C=cell_f();
// while ((C=C->nextl)->nextl)
 while( (C=CSTR_GetNextRaster(C,f_letter)) )
 {
//   CSTR_rast wc;

   df = if_dust(C);
   if ((df & 7) && ((df & 16) == 0))
   {
     trans_total++;
     let_to_dust = 1;

	 CSTR_GetAttr(C,&attr);
     attr.flg = CSTR_f_dust;
	 CSTR_SetAttr(C,&attr);
//     (wc=C->prevl)->nextl=C->nextl;
//     C->nextl->prevl=C->prevl;
//     err_pnlet(C);  // AL 940319
//     C=wc;
   }
 }
}
///////////////

int16_t if_dust(CSTR_rast C)
{
 int16_t rb, w1, wh, wps;
 CSTR_rast_attr attr;

 // Check position in the string - have to intersect with string middle
 // MUST request local BASELINES !!!!!!!!!!!!!!

 rb = 0;
 wps = def_locbas(C);
 CSTR_GetAttr(C,&attr);

 /////////  931230 if (5*C->h > 3*wps) rb |= 16;    // indicate LARGE
 w1=(attr.row - minrow)-bbsm-attr.bdiff;
 if (w1 >= 0)
 {
   rb |= 1;
   goto chks;
 }

 w1=w1+1+attr.h+((fax1x2)?2:0);
 if (w1 < 0)
  rb |= 2;
			       // Check height of cell:
chks:              // height(cell) + 4 >= main_base-small_base
 wh=attr.h+((fax1x2)?2:0);

 if ((wh <= sMALL_SIZE) || (5*wh < 3*wps))  //NB 3.4.94
  return (rb | 4);

 if (4*wh < 3*wps)
  return (rb | 8);

 return 0;
}
////////////////

int16_t get_bsm()
 { return (bbsm+minrow); }

/////////////
int32_t get_size()                                                   //16.01.97
 {
   if (multi_bas & 128)
      def_locbas(NULL);
   return (int32_t) Ps;                                              //16.01.97
 }
///////////

#define SKEW_SCALE 8
#define MAX_SKEW (1 << SKEW_SCALE)/10

//////////////
int16_t dust_to_let()
{
 int16_t n;
 CSTR_rast c;
 CSTR_rast_attr attr;
 int line_scale = 0;

 if( lin_str )
 {
	 CSTR_attr at;
	 CSTR_GetLineAttr(lin_str, &at);
	 line_scale = at.scale;
 }


// glsnap('d',cell_f()->next,"dust_to_let beg");

// for (n=0,c=cell_f()->next; c->next!=NULL; c=c->next)
 for (n=0,c=CSTR_GetNext(cell_f()); c && c!=cell_l(); c=CSTR_GetNext(c) )
 {

/*  Nick commented  - need tuning accord to OLEG
   if (c->env && (c->env)->large&ch_underlined)
   {
     cell *cp,*cn;
     cp=c->prev;
     cn=c->next;
     //n+=shaving_let(c); // OLEG : need tunning
     c=cp->next;
     if (c==cn)  //  nothing brought by shaving
      { c = cp; continue; }
   }
*/

   CSTR_GetAttr(c,&attr);
   if ( attr.flg & CSTR_f_dust)   // brought as dust
   {
	 int16_t wr;

     if ((((wr=if_dust(c)) & 7) == 0) || (wr & 16))
     // not posed as dust, not sized as small dust
     {
       // eliminate as dust; if "inside" -- bring as letter
       wr = attr.row - minrow + attr.h/2;
       if( (wr <= bbs1) || (wr >= bbs4) )
         continue;   // outside the line - not to be letter

       // AL 940411
       trans_total++;

//       (c->next)->prev=c->prev;
//  	 (cp=c->prev)->next=c->next;

       set_bad_cell(c);
       short_recog_cell(c,line_scale);
//       insert_cell(c,cp);

	   CSTR_GetAttr(c,&attr);
       if ( attr.flg & CSTR_f_let)
		   n++;
     }
   } // if dust
 }   // for cells

 // glsnap('d',cell_f()->next,"dust_to_let end");
 return n;
 }
////////////////

