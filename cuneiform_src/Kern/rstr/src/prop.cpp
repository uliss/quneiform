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
/*                           module PROP                             */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>

#include "struct.h"
#include "func.h"

#define P   'd'        /* snap identifier for module PROP.C */
#define MaxNumPass 14  /* maximal number of passes */

#define Mrg_Num_Nar    4      /* number of the marginal narrow letters */
#define Normal_Num_Nar 3      /* 'normal' number of the narrow letters */
#define Score_N_N_N    2      /* bonus for 'normal' num. of narrow letters */
#define Extra_Num_Nar  6      /* extra number of the narrow letters */
#define Score_E_N_N    2      /* bonus for 'extra' num. of narrow letters */

#define Normal_Num_ENar 2    /* 'normal' number of the extra nar.letters */
#define Score_N_N_EN    3    /* bonus for 'normal' num. of ext.nar.letters */
#define Extra_Num_ENar  5    /* extra number of the extra nar. letters */
#define Score_E_N_EN    3    /* bonus for 'extra' num. of ext.nar.letters */

#define Mrg_Num_Wid     4      /* number of the marginal wide letters */
#define Normal_Num_Wide 3      /* 'normal' number of the wide letters */
#define Score_N_N_W     2      /* bonus for 'normal' num. of wide letters */
#define Extra_Num_Wide  6      /* extra number of the wide letters */
#define Score_E_N_W     2      /* bonus for 'extra' num. of wide letters */

#define Normal_Num_EWid 2    /* 'normal' number of the extra wid.letters */
#define Score_N_N_EW    3    /* bonus for 'normal' num. of ext.wid.letters */
#define Extra_Num_EWid  5    /* extra number of the extra wid. letters */
#define Score_E_N_EW    3    /* bonus for 'extra' num. of ext.wid.letters */

cell *cell_f();         /* get first cell of the string */
/*  short_recog_cell(cell *);  */   /* recognition of the cell */
puchar proptab_ptr();          /* obtain ptr to proportional table */
extern uchar db_pidx_crit;     /* use or not use proportional crit. */
extern uchar *letters_pidx_table;
extern uchar line_scale;
extern uchar fax1x2;

typedef struct prop_struct
 {
  cell *BC;                 /* pointer to the currente cell */
  int16_t n_narrow,             /* the number of the narrow letters */
      n_wide,               /* the number of the wide letters */
      n_extnar,             /* the number of the extra narrow letters */
      n_extwid,             /* the number of the extra wide letters */
      n_mrgnar,             /* the number of the marginal narrow letters */
      n_mrgwid;             /* the number of the marginal wide letters */
  puchar   ptab;
  uchar    npass;             /* the pass number on  the string */
  char wstr[260];            /* the string to information snaping */
  uchar nlet,                 /* the letter number in  the string */
      f_nar,              /* the flag of the narrow letters */
      f_wide,             /* the flag of the wide letters */
      f_extn,             /* the flag of the extra narrow letters */
      f_extw,             /* the flag of the extra wide letters */
      f_mrgn,             /* the flag of the marginal narrow letters */
      f_mrgw;             /* the flag of the marginal wide letters */
  uchar fladd;             /* the flag of the changing of the DELTA */
 } prop_struct;

typedef struct prop_tab_el
 {  /* description of the element of the proportional table */
  uchar min;               /* minimal proportional index */
  uchar max;               /* maximal proportional index */
 } prop_tab_el;
/**** Prototypes of all functions *******/
int16_t des_re_rec();

#define TROUBLE_PROP      14
#define ONE_LET_LINE_TRSH 4

uchar prop_in_trouble;     /* if DELTA>14 then line was strange (for cutting)*/
static int16_t  call_flag;

/*---------- prop_l_delta and prop_r_delta are global variables ----------*/
 int16_t prop_l_delta=0, prop_r_delta=0;
/*------------------------------------------------------------------------*/

static prop_struct *glp;
void pre_re_rec(void);
void proc_string(void);
void cnt_delta(void);
void re_recog(void);
void accept_let(void);
void bad_letter(void);
void p_prt_cell(void);


void prop(int16_t call_num)

/***********************************************************************
	This procedure counts the proprtional coefficient.
	ON ENTRY: the address of the beginning cell of the
		  string of components.
	ON EXIT : prop_l_delta and prop_r_delta are proprtional coefficientes
				 (for low and upper bounds).
**************************************************************************/
{
prop_struct glstr;
  call_flag = call_num;
  if (!db_pidx_crit) return;
  glp=&glstr;
  glp->npass=0;              /* clean the pass number on the string */
  prop_in_trouble = 0;

/* set up delta = 0 avoiding extra exlarging of bounds after "wild" string */
  if ( prop_l_delta > TROUBLE_PROP ) prop_l_delta = 0;
  if ( prop_r_delta > TROUBLE_PROP ) prop_r_delta = 0;

/*** All SNAP-activity removed temporary *****************/

  snap_newpass(P);
  if (snap_activity(P))
  {
   sprintf(glp->wstr,"PROP-module begins - delta: l=%d, r=%d;",
			     prop_l_delta,prop_r_delta);
   snap_show_text(glp->wstr);
   snap_monitor();
  }
/*****************************************************/
Proc:
//  if (line_scale)        /* if the string contains scaled components, */
//    pre_re_rec();        /* than it is necessary re-recognize this string */
  proc_string();
  cnt_delta();               /* the counting of the delta to exlarge
                                the proportional bounds in the PROPTAB */

/*** All SNAP-activity removed temporary *****************/
  if (snap_activity(P))
  {
   sprintf(glp->wstr,
"string is processed - pass=%d, delta: l=%d, r=%d;\n narrow=%d,extnar=%d,bndnar=%d;\n wide=%d,extwide=%d,bndwide=%d;",
		     glp->npass,prop_l_delta,prop_r_delta,
		     glp->n_narrow,glp->n_extnar,glp->n_mrgnar,
		     glp->n_wide,glp->n_extwid,glp->n_mrgwid);
   snap_show_text(glp->wstr);
   snap_monitor();
  }
/*****************************************************/

  if (des_re_rec())   /* if it is nessessary to re-recognize */
   {
     glp->npass++;    /* increase the pass number on the string */
     if (glp->npass > MaxNumPass) goto Finish;
     re_recog();     /* re-recognition under new proportional bounds */
     goto Proc;
   } /* end-if */
  else
   {
    if ( (prop_l_delta) || (prop_r_delta) )
      re_recog();
   }
Finish:

/*** All SNAP-activity removed temporary *****************/
  if (snap_activity(P))
  {
   sprintf(glp->wstr,"PROP-module finished - delta: l=%d, r=%d;",
			     prop_l_delta,prop_r_delta);
   snap_show_text(glp->wstr);
   snap_monitor();
  }
/*****************************************************/
 if ( ( prop_r_delta > TROUBLE_PROP ) || ( prop_l_delta > TROUBLE_PROP )  )
   prop_in_trouble = 1;
 } /*------------------------------------------------------------------*/
/**************************************************************/
void re_recog(void)
{
     glp->BC=cell_f();
     while (1)
      {
       glp->BC=glp->BC->nextl;
       if (!glp->BC) break;
       if (!glp->BC->next) break;   /* dummy cell at end reached  */
       if ( ((glp->BC->reasno) & (c_rn_right|c_rn_left) ) ||
	    ((glp->BC->w <= 4) && (prop_l_delta > 9) )
	  )  // re-recognition under follow conditions:
	     // 1. cell has refused versons by prop. crit.
	     // 2. component has width <= 4 pixels and
	     //    typeface(font) of this string is very thin;
	 short_recog_cell(glp->BC);
	 /***
	 if (glp->BC->nvers)
	   { ij_dot(glp->BC);
	     if (glp->BC->nvers==0)
	       glp->BC->reasno=0;
	   }
	 ***/

      } /* end-while */
}

void pre_re_rec(void)
{
     glp->BC=cell_f();
     while (1)
      {
       glp->BC=glp->BC->nextl;
       if (!glp->BC) break;
       if (!glp->BC->next) break;   /* dummy cell at end reached  */
       short_recog_cell(glp->BC);
       /****
       if (glp->BC->nvers)
	 { ij_dot(glp->BC);
	   if (glp->BC->nvers==0)
	     glp->BC->reasno=0;
	 }
       ****/
      } /* end-while */
}

void proc_string(void)
{
  glp->n_narrow=0;     glp->n_wide=0;
  glp->n_extnar=0;     glp->n_extwid=0;
  glp->n_mrgnar=0;     glp->n_mrgwid=0;

  glp->f_nar=glp->f_extn=glp->f_mrgn='N';
  glp->f_wide=glp->f_extw=glp->f_mrgw='N';
  glp->nlet=0;

  glp->BC=cell_f();
  while (1)
   {
    glp->BC=glp->BC->nextl;
    if (!glp->BC) break;
    if (!glp->BC->next) break;   /* dummy cell at end reached  */
    glp->nlet++;
    if (glp->BC->flg == c_f_let)     /* well recognized letter */
      accept_let();                /* add to n_narrow or n_wide */
    if (glp->BC->flg == c_f_bad)     /* bad recognized letter */
      bad_letter();
    p_prt_cell();
   } /* end-while */
}
/*****************************************************************************/
void accept_let(void)
{
uchar clet;           /* the recognized letter (first version) */
int16_t  wi,             /* proportional index of the letter */
     lb,             /* left bound of the prop. index under delta */
     rb,             /* right bound of the prop. index under delta */
     mlb,             /* left bound of the 'normal' prop. index */
     mrb,             /* right bound of the 'normal' prop. index */
     a,b,ad,bd;

 wi = prop_index (glp->BC->h, glp->BC->w);
 wi*=7;       /* the proprtional index of the letter */
 clet=glp->BC->vers[0].let;   /* first version */
 glp->ptab=letters_pidx_table;   /* get the address of the beginning of PROPTAB */
 glp->ptab=glp->ptab+(clet*sizeof(prop_tab_el));
			  /* the offset in the table = (letter value * 2) */
 a=*(glp->ptab);       ad=a-prop_l_delta;
 b=*(glp->ptab+1);     bd=b+prop_r_delta;
 lb=(ad*6) + b;    /* [6*(a-delta) + b] */
 rb=(bd*6) +a;     /* [6*(b+delta) + a] */
 mlb=(a*6) + b;    /* [6*a + b] */
 mrb=(b*6) + a;    /* [6*b + a] */
 if (wi < lb)   { glp->n_narrow++; glp->f_nar='Y';}
 if (wi > rb)   { glp->n_wide++; glp->f_wide='Y';}
 if (wi < mlb)  { glp->n_mrgnar++; glp->f_mrgn='Y';}
 if (wi > mrb)  { glp->n_mrgwid++; glp->f_mrgw='Y';}
}

void bad_letter(void)
{
       /****** if the reason of the refusing of the letter
	       is the proportional criteria case.
	       add to n_extnar or n_extwid  ***********/

       if (((glp->BC->reasno)&c_rn_right) == c_rn_right)
	{
	 glp->n_extwid++;
	 glp->f_extw='Y';
	}
       if (((glp->BC->reasno)&c_rn_left) == c_rn_left)
	{
	 glp->n_extnar++;
	 glp->f_extn='Y';
	}

}
/*******************************************************/
void cnt_delta(void)
{
int16_t wi;
/***** delta is increased if recognition was under delta  *****/

 glp->fladd = 0;    /* to clear the changing of DELTA flag */

 wi=glp->n_narrow;
 if (glp->n_narrow > Normal_Num_Nar)  {glp->fladd=1; prop_l_delta++;}
 if ( (prop_l_delta == 0) || (glp->npass != 0) )
 {
  if (glp->n_extnar > 1)    {glp->fladd=1; prop_l_delta++;}
  else              wi=wi+((glp->n_extnar)<<1);  /* wi=narrow+ 2*extnar */
 } /* end-if */
 if ( (wi<<3) > glp->nlet ) {glp->fladd=1; prop_l_delta++;}

 wi=glp->n_wide;
 if (glp->n_wide > Normal_Num_Wide)   {glp->fladd=1; prop_r_delta++;}
 if ( (prop_r_delta == 0) || (glp->npass != 0) )
 {
  if (glp->n_extwid > 1)               {glp->fladd=1; prop_r_delta++;}
  else              wi=wi+((glp->n_extwid)<<1);  /* wi=wide+ 2*extwid */
 } /* end-if */
 if ( (wi<<3) > glp->nlet ) {glp->fladd=1; prop_r_delta++;}

 if (glp->npass) goto Ret;
 wi=((glp->n_extnar)<<1) + glp->n_mrgnar;
 if ( (wi < Mrg_Num_Nar) && (8*wi < glp->nlet) ) prop_l_delta=0;
 wi=((glp->n_extwid)<<1) + glp->n_mrgwid;
 if ( (wi < Mrg_Num_Wid) && (8*wi < glp->nlet) ) prop_r_delta=0;

Ret:;
}

void prop_init(void)
{
 prop_l_delta=prop_r_delta=0;
}

void p_prt_cell(void)
{
/*** All SNAP-activity removed temporary *****************
 snap_newcell(glp->BC);
 if (snap_activity(P))
  {
   SPRINTF(glp->wstr,
"cell-info - pass=%d, nl=%d, delta: l=%d, r=%d;\n nar=%c,extn=%c,bndn=%c;\n wide=%c,extw=%c,bndw=%c;",
		     glp->npass,glp->nlet,prop_l_delta,prop_r_delta,
		     glp->f_nar,glp->f_extn,glp->f_mrgn,
		     glp->f_wide,glp->f_extw,glp->f_mrgw);
    snap_show_text(glp->wstr);
    snap_monitor();
   }
**************************************************************************/

 glp->f_nar=glp->f_extn=glp->f_mrgn='N';
 glp->f_wide=glp->f_extw=glp->f_mrgw='N';
}

int16_t des_re_rec()
{
int16_t cc;
  cc=0;

 if ( ( glp->nlet == 1 ) &&
      ( (prop_l_delta > ONE_LET_LINE_TRSH) || (prop_r_delta > ONE_LET_LINE_TRSH) )
    )  goto DRR_Ret;
 if ((glp->n_extnar > 1) )  cc++;
 if ((glp->n_extwid > 1) )  cc++;
 if ( glp->npass )
  {
   if (    (glp->n_extnar == 1)
       && ( ((1+glp->n_narrow)<<3) > glp->nlet)    ) cc++;
   if ((glp->n_extnar == 0)&&( ((glp->n_narrow)<<2) > glp->nlet)) cc++;

   if (    (glp->n_extwid == 1)
	&&( ((1+glp->n_wide)<<3) > glp->nlet)   ) cc++;
   if ((glp->n_extwid == 0)&&( ((glp->n_wide)<<2) > glp->nlet)) cc++;
  }
 else
  {
   if (    (glp->n_extnar == 1)
       && ( ((1+glp->n_mrgnar)<<3) > glp->nlet)    ) cc++;
   if ((glp->n_extnar == 0)&&( ((glp->n_mrgnar)<<2) > glp->nlet)) cc++;

   if (    (glp->n_extwid == 1)
	&&( ((1+glp->n_mrgwid)<<3) > glp->nlet)   ) cc++;
   if ((glp->n_extwid == 0)&&( ((glp->n_mrgwid)<<2) > glp->nlet)) cc++;

  }
DRR_Ret:
  return(cc);
}

/*=================== Proportional criterim itself ===================*/

/*--------------------- Count proprtional index -------------------*/

int16_t prop_index(int16_t h, int16_t w)
{
 if (fax1x2)	h+=2;
 if (w <= h)   	return ((uint16_t)w << 6) / h;
 else		return 128 - ((uint16_t)h << 6) / w;
}

