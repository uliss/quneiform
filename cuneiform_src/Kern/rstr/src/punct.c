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
/*		Punctuation signs recognition                        */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "struct.h"
#include "func.h"
#define PUNCT_CORRECTION0 //Loginov
//#define SAVE_FEAT //Loginov
#include "ligas.h"
#include "lang.h"
#include "tuner.h"
#include "status.h"
#include "linutil.h"	// 31.05.2002 E.P.

#include"compat_defs.h"

extern uchar fax1x2;
extern int16_t pitchsize;
extern uchar db_status;
extern int16_t line_number;
extern cell *accadr1;
extern cell *accadr2;
extern uchar line_tabcell;

#define HPERMIN 4
#define KEGM    14
#define FEAT_SIZE 40

static puchar rstr;
static int16_t italic;
static B_LINES bl;
static cell *clist[4];
puchar sv_fantom=NULL;

static void order();
static void bad_to_dust();
static void glue_dust();
static int16_t exclam (cell *,cell *,B_LINES *);
static cell *let_gl(cell *,cell *,cell *,cell *,B_LINES *);
static void dots();
static void quockets();
static void dust_to_bad(cell *);
static void punctsign(cell **,cell **);
static void intval (cell *,cell *);
static void no_cut(cell *);
static int16_t dustpos(int16_t,cell *);
#ifdef PUNCT_CORRECTION
 //static int16_t exist_prev_next(cell *c,int16_t *dif1,int16_t *dif2);
//static int16_t get_dust_status(cell *c,uchar *dust_st);
//static int16_t dust_pos_new(cell *);
int16_t is_dash(cell *c);
int16_t is_dust_2(cell *c);
int16_t is_dust(cell *);
int16_t is_point_stop(cell *c);
#endif
static int16_t chkquot(int16_t,int16_t,cell *);
static int16_t chkstar(int16_t,int16_t,cell *);
static int16_t chkdotcom(int16_t,int16_t,cell *);
static int16_t chkdash(int16_t,int16_t,cell *);
static int16_t chkplus(int16_t,int16_t,cell *);
static int16_t chkslash(int16_t,int16_t,cell *);
static int16_t chkcircle(int16_t,int16_t,cell *);
static int16_t chkquock(int16_t,int16_t,cell *);
static int16_t chkquocks(int16_t,int16_t,cell *);
static int16_t contain(cell *new, cell *old);
cell *hide(cell *C);

static void czech_dt_glue_apostroph(); // 07.09.2000 E.P.


int16_t check_shevron(cell *c,int16_t flag_qual); //Alik

//
// kill temporary twin flags BAD+DUST for pass4
//
void clear_punct_twin_flags(void)
{
cell *c,*e;
for(c=cell_f()->next, e=cell_l(); c!=e; c=c->next )
    {
    if( c->flg==(c_f_bad|c_f_dust) )
        dust_to_bad(c);
    if( c->nvers && (c->flg&c_f_bad) )
        c->flg = c_f_let;
    }
return;
}

void punct()
 {
 cell *c1,*c2,*c;
 uchar l;
 int16_t x,d;

 if (language == LANG_RUSSIAN)  sv_fantom=NULL;
 snap_newpass('e');
 quockets();

 // ACC_ROOF_INF над d,t в Чешском похож на апостроф. 07.09.2000 E.P.
 if (language==LANG_CZECH)
	czech_dt_glue_apostroph();

 bad_to_dust();
 glue_dust();
 dots();
 order();

 italic=0;
 c1=cell_f();
 while ((c2=c1->next)!=NULL)
  {
       if ((c1->font|c1->font_new)&c_fp_it)
        italic=1;
       if ((c1->font|c1->font_new)&c_fp_str)
        italic=0;
       if( (c1->pos_inc&erect_rot) && c1->stick_inc>256 )  // OLEG
        italic=1;
       if (c1->nextl->prev==c1 ||
                //     (c2->flg & (c_f_space|c_f_punct)) ||
          (c1->flg & (c_f_solid|c_f_confirmed)) // Vald
          )
         {c1=c1->nextl; continue;}
       x=c1->col+c1->w;
       if (!italic && c1->nextl->col<x ||
           c1->nextl->col-x<=1 &&
           c1->cg_flag&c_cg_cutr &&
           c1->nextl->cg_flag&c_cg_cutl &&
           c1->next->cg_flag&c_cg_cutl &&
           c1->nextl->prev->cg_flag&c_cg_cutr)
        {
            if (db_status)
              snap_newcell(c1->next);
            if (snap_activity('e'))
            {
               snap_show_text("delete cutted dust");
               snap_monitor();
            }
             /*
             c1->next=c1->nextl;
             c1->nextl->prev=c1;
             */
             hide(c2);
             c1=c1->nextl;
             continue;
        }
        if(language != LANG_RUSSIAN )
        {
            if (memchr("fFrTP",(l=c1->vers[0].let),5))
              x-=(c1->w)/2;
            if (memchr("vVwWyY7",l,7))
              x-=(c1->w)/4;
            if (((c1->font|c1->font_new)&c_fp_it || italic) && memchr("dlEHIJMNSUZ1t6",l,14))
              x-=(c1->w)/4;
              // Interference i_left_accent ~ м
            if (
				// Конфликтный код 07.09.2000 E.P.
			    !is_baltic_language(language) && 	// 17.07.2001 E.P.
			    !is_turkish_language(language) && (	// 30.05.2002 E.P.
				!is_cen_language(language) && l==i_left_accent ||
				l==i_right_accent ) ||

				// Балтийские палки. 10.07.2001 E.P.
				is_baltic_language(language) &&
					(
					l==II_bottom_accent || l==i_bottom_accent ||
					l==l_bottom_accent
					) ||
				// Турецкие палки. 30.05.2002 E.P.
				is_turkish_palka(l)
				)
              x-=(c1->w)/4;
        }
        else
        {
            //    if (memchr("Ј’ђ",(l=c1->vers[0].let),5))  Paul  10-14-96
            if (memchr("ѓЈ’ђ",(l=c1->vers[0].let),4))
              x-=(c1->w)/2;
            if (memchr("г“7",l,7))
              x-=(c1->w)/4;
            //    if ((c1->font&c_fp_it || italic) && memchr("€ЌЏ16Ў",l,14))Paul  10-14-96
            if (((c1->font|c1->font_new)&c_fp_it || italic) &&
				!(memchr("Ђ„†Љ•–™љ›њ",l,10) &&
					!is_russian_turkish_conflict(l) // 21.05.2002 E.P.
				 )
				)
              x-=(c1->w)/4;
              // Interference i_left_accent ~ м
            if (
				// Конфликтный код 07.09.2000 E.P.
				!is_baltic_language(language) && 	// 17.07.2001 E.P.
				!is_turkish_language(language) && (	// 30.05.2002 E.P.
				!is_cen_language(language) && l==i_left_accent
				|| l==i_right_accent
				) ||

				// Балтийские палки. 10.07.2001 E.P.
				is_baltic_language(language) &&
					(
					l==II_bottom_accent || l==i_bottom_accent ||
					l==l_bottom_accent
					) ||
				// Турецкие палки. 30.05.2002 E.P.
				is_turkish_palka(l)
			   )
              x-=(c1->w)/4;
        }
        c1=c1->nextl;

		//AK add crashguard
		if ( (!AKCellInChain(c2) || !AKCellInChain(c1)) )
			break;

        while (c2!=c1)
        {
              if (c2->flg & c_f_punct)  { c2=c2->next;  continue; }
              get_b_lines(c2,&bl);
              c=c1->prevl;
              d=0;
              if (memchr("AL",l,2) && !((c->font|c->font_new)&c_fp_it) && c2->row+c2->h<c->row+c->h/2)
                  d=(c->w)/2;
              if (!(c2->flg&c_f_bad) &&
                 (x-d>c2->col+(c2->w)/2 ||
                 /*      accent_tab[l] && c->col+c->w>c2->col && c2->row<bl.bm ||*/
                  !((c->font|c->font_new)&c_fp_it) && c2->row+c2->h>=bl.b3-3 && c2->w<=4 &&
                  c->col+c->w>c2->col && strchr("adhijlmnquzAIHKLMNRZ12",l)!=NULL))
              {
              /*
              if( c2->main_acc )
                { // OLEG : save accent for pass2
                cell    *cc=c2->main_acc, *cn=cc->next;
                clist[0]=cc;
                clist[1]=c2;
                compose_cell(2,clist,cc);
                if( cc->accent_leader )
                    cc->accent_leader=0;
                c2=cn;
                }
              else
              */
                {
                no_cut(c2);
                c2=hide(c2);
                c2=c2->next;
                }
              }
              else
              {
                   if (db_status)
                    snap_newcell(c2);
                   if (snap_activity('e'))
                   {
                      snap_show_text("interval begin");
                      snap_monitor();
                   }
                   punctsign(&c1,&c2);  // AL 940319
                   intval(c1,c2);
                   if (db_status)
                      snap_newcell(c1->prev);
                   if (snap_activity('e'))
                   {
                      snap_show_text("interval end");
                      snap_monitor();
                   }
                   break;
              }
        }//end of while (c2!=c1)
  }//end of while ((c2=c1->next)!=NULL)
 for (c1=cell_f(),c2=c1->nextl,c=c1->next; c->next; c=c->next)
  if (c->flg&(c_f_let|c_f_bad) && !(c->flg&(c_f_dust|c_f_punct)))
   {c1=c; c2=c->nextl;}
  else
   if (
        (!(c1->flg&c_f_fict) &&
        (c1->flg&c_f_bad || c1->vers[0].let=='i' && c1->vers[0].prob==128) &&
        c->col+c->w/2<c1->col+c1->w ||
        !(c2->flg&c_f_fict) &&
        (c2->flg&c_f_bad || c2->vers[0].let=='i' && c2->vers[0].prob==128) &&
        c->col+c->w/2>c2->col)
#ifdef PUNCT_CORRECTION
        ||  ( language==LANG_RUSSIAN && is_dust_2(c) )
#endif
      )
    {
       if (db_status)
        snap_newcell(c);
       if (snap_activity('e'))
        {
        snap_show_text("delete dust");
        snap_monitor();
        }
       c=hide(c);
    }

 if (snap_activity('e'))
  {
  snap_show_text("punctuation end");
  snap_monitor();
  }

 }

 static void order()
 {
	cell *c1,*c2,c;
	//AK add c/g (case of c1 = cell_l()
	if ( cell_f()->next != cell_l() )
	{
	for (c1=(cell_f())->next; (c2=c1->next)->next!=NULL; c1=c2)
	if (c1->col==c2->col && c1->w>c2->w)
	{
   memcpy(&c,c1,sizeof(cell));
   memcpy(c1,c2,sizeof(cell));
   c1->next=c.next; c1->prev=c.prev;
   c.next=c2->next; c.prev=c2->prev;
   if (c1->flg&(c_f_dust|c_f_punct))
    {
    if (!(c.flg&(c_f_dust|c_f_punct)))
     (c.prevl)->nextl=(c.nextl)->prevl=c2;
    }
   else
    {
    if (c.flg&(c_f_dust|c_f_punct))
     (c1->prevl)->nextl=(c1->nextl)->prevl=c1;
    else
     {
     c1->nextl=c.nextl; c1->prevl=c.prevl;
     c.nextl=c2->nextl; c.prevl=c2->prevl;
     }
    }
   memcpy(c2,&c,sizeof(cell));
   c2=c2->prev;
   if (!(c2->prev->flg&c_f_fict))
    c2=c2->prev;
   } //for if
  } //if
 }

#define PROBMIN    40
#define PROBOK     120
#define PROBOK_ceo 190

static void bad_to_dust()
 {
 cell *c, *cc;
 int16_t r;

 for (c=cell_f()->nextl; c->next!=NULL; c=c->nextl)
  {
  //  Oleg
  //if (!tsimple(c))
  //  continue;
  get_b_lines(c,&bl);
  rstr=NULL;
  if(
      (c->flg&c_f_bad ||
       c->vers[0].prob<PROBOK ||                // Oleg
       memchr("ceo<>",c->vers[0].let,4) && c->vers[0].prob<PROBOK_ceo ||
       language==LANG_RUSSIAN && memchr("Єўбэкн",c->vers[0].let,6) &&
                                 c->vers[0].prob<PROBOK_ceo ||
       language==LANG_RUSSIAN && memchr("ўн",c->vers[0].let,2) &&
       c->stick_inc>300 && c->vers[0].prob<PROBOK_ceo+10     ||
       language==LANG_RUSSIAN && memchr("ЁҐ®",c->vers[0].let,3) &&
       c->vers[0].prob<PROBOK_ceo-20
     ) && c->h<=bl.ps && c->row+2>=bl.b2 && c->row+c->h-2<=bl.b3 &&
          (r=chkquocks(bl.ps,2,c))!=0 && check_shevron(c,1)>=3)
   {
   c->vers[0].let=(r<0)?left_quocket:right_quocket;
   c->vers[0].prob=254;
   c->vers[1].let=0;
   c->nvers=1;
   c->flg=c_f_let;
   }
  if ((c->flg&c_f_bad || c->vers[0].prob<PROBMIN) &&
      c->h<=bl.ps &&
      (c->row+((fax1x2)?2:0)+1>=bl.bm ||
       c->row+c->h-1-((fax1x2)?2:0)<=bl.bm ||
       c->row+1>=bl.b2 && c->row+c->h-1<=bl.b3 && c->h<bl.ps ||
       c->cpos==ce_p_ex && (c->h>c->w || c->h<=2*bl.ps/3)))
   {
   if (db_status)
    snap_newcell(c);
   if (snap_activity('e'))
    {
    snap_show_text("bad to dust");
    snap_monitor();
    }
   c->flg=(c_f_dust|c_f_bad);
   (cc = c->prevl)->nextl=c->nextl;
   c->nextl->prevl=c->prevl;
   err_pnlet(c);  // ?? AL 940319
   c = cc;
   }
  }
 }

#define DCDUST  2

static void glue_dust()
 {
 cell *c1,*c2,*c3,*c4;cell *cc1;
 int16_t h,s,m1,m2,m3,m4;

 for (c1=(cell_f())->next; c1 && (c2=c1->next)!=NULL; c1=c1->next)
  if (c1->flg&c2->flg&c_f_dust)
   {
   get_b_lines(c1,&bl);
   h=bl.ps;
   if (c1->w>h/2 || c2->w>h/2)
    continue;
   m1=c1->col+(c1->w)/2;
   m2=c2->col+(c2->w)/2;
   if (c1->row>=bl.b1-2 && c1->row+c1->h<=bl.b3+2 &&
       c2->row>=bl.b1-2 && c2->row+c2->h<=bl.b3+2 &&
       abs(m1-m2)<=DCDUST)
    {
    s=c1->h+c2->h;
    if ((c3=c2->next)!=NULL && c3->flg&c_f_dust && c3->w<=h/2)
     {
     m3=c3->col+(c3->w)/2;
     if (c3->row>=bl.b1-2 && c3->row+c3->h<=bl.b3+2 &&
	 abs(m1-m3)<=DCDUST && abs(m2-m3)<=DCDUST)
      {
      s+=c3->h;
      if ((c4=c3->next)!=NULL && c4->flg&c_f_dust && c4->w<=h/2)
       {
       m4=c4->col+(c4->w)/2;
       if (c4->row>=bl.b1-2 && c4->row+c4->h<=bl.b3+2 &&
	   abs(m1-m4)<=DCDUST && abs(m2-m4)<=DCDUST && abs(m3-m4)<=DCDUST)
	s+=c4->h;
       else
	c4=NULL;
       }
      else
       c4=NULL;
      }
     else
      c3=c4=NULL;
     }
    else
     c3=c4=NULL;
    if (c3==NULL &&
	(c1->h<=4*c1->w/3 && c2->h<=4*c2->w/3 || exclam(c1,c2,&bl)))
     continue;
    if ((s>h || (c3!=NULL || fax1x2) && s>=h-1) &&
	(c1->row<bl.b2 || c2->row<bl.b2 || c3!=NULL &&
		 (c3->row<bl.b2 || c4!=NULL && c4->row<bl.b2)) &&
	(c1->row+c1->h>bl.bm || c2->row+c2->h>bl.bm || c3!=NULL &&
		 (c3->row+c3->h>bl.bm || c4!=NULL && c4->row+c4->h>bl.bm)))
     {
     if (db_status)
      snap_newcell(c1);
     if (snap_activity('e'))
      {
      snap_show_text("glue of dusts: before");
      snap_monitor();
      }
     cc1=let_gl(c1,c2,c3,c4,&bl);
     if( cc1 )
         {
         c1=cc1;
         if (db_status) snap_newcell(c1);
         if (snap_activity('e'))
          {
          snap_show_text("glue of dusts: after");
          snap_monitor();
          }
         }
    else
        {
        return ; // OLEG : restruct line
        }
     }
    }
   }
 }

static int16_t exclam(cell *c1,cell *c2,B_LINES *bl)
 {
 cell *c,*cc;

 c=(c1->row<c2->row)?c1:c2;
 if (c->row+c->h<=bl->bm && 5*c->h>3*bl->ps && 2*c->w<=c->h &&
     dustpos(bl->ps,c)<=3)
  {
  for (cc=c; cc->flg&c_f_dust || !(cc->flg&(c_f_let|c_f_bad|c_f_fict));
							      cc=cc->prev) ;
  c->flg=c_f_let;
  c->nvers=1;
  c->vers[0].let='!'; c->vers[0].prob=254;
  c->vers[1].let=0;
  c->prevl=cc;
  c->nextl=cc->nextl;
  cc->nextl=cc->nextl->prevl=c;
  if (dot_excl(c)!=NULL)
   {
   if (db_status)
    snap_newcell(c1);
   if (snap_activity('e'))
    {
    snap_show_text("dust to exclam");
    snap_monitor();
    }
   return 1;
   }
  c->flg=c_f_dust;
  cc->nextl=c->nextl;
  cc->nextl->prevl=cc;
  err_pnlet(c);  // AL 940319
  }
 return 0;
 }

static cell *let_gl(cell *c1,cell *c2,cell *c3,cell *c4,B_LINES *bl)
 {
 cell *c, *cp;
 int16_t r,n;
#define MBI 1
#define MBE 2

 cp = c1->prev;
 c=new_cell();
 c->flg=c_f_let;
 clist[0]=c1;
 clist[1]=c2;
 if (c3!=NULL)
  {
  clist[2]=c3;
  if (c4!=NULL)
   {
   clist[3]=c4;
   n=4;
   }
  else
   n=3;
  }
 else
  n=2;
 // AL make all decidions BEFORE deletion of cells
 r=0;
 if (c1->row+c1->h-1<=bl->b2 && abs(c1->h-c1->w)<=MAX(1,bl->ps/8) ||
     c2->row+c2->h-1<=bl->b2 && abs(c2->h-c2->w)<=MAX(1,bl->ps/8) ||
     c3!=NULL &&
     (c3->row+c3->h-1<=bl->b2 && abs(c3->h-c3->w)<=MAX(1,bl->ps/8) ||
      c4!=NULL &&
      c4->row+c4->h-1<=bl->b2 && abs(c4->h-c4->w)<=MAX(1,bl->ps/8)))
  r|=MBI;
 if (c1->row>=bl->bm && abs(c1->h-c1->w)<=MAX(1,bl->ps/8) ||
     c2->row>=bl->bm && abs(c2->h-c2->w)<=MAX(1,bl->ps/8) ||
     c3!=NULL &&
     (c3->row>=bl->bm && abs(c3->h-c3->w)<=MAX(1,bl->ps/8) ||
      c4!=NULL && c4->row>=bl->bm && abs(c4->h-c4->w)<=MAX(1,bl->ps/8)))
  r|=MBE;
 // AL
 // AL OK (returns new "c1")
if( !compose_cell(n,clist,c) )
    {
    free_cell(c);
    return NULL;
    }
 c->reasno=0;
 c->cpos=0;
 c->font=0;
 c->bdiff=0;
 c->difflg=0;
 c->cg_flag=0;
 c->dens=255;
 c->keg=(uchar)bl->ps;
 c->nvers=0;
 // AL-JOE decicion-making was here; all c1-c4 were deleted
 if( (r&MBI) && !(language==LANG_RUSSIAN&&!langUkr&&!langSer))
  {
  c->vers[c->nvers].let='i';
  c->vers[c->nvers].prob=128;
  (c->nvers)++;
  }
 if (r&MBE)
  {
  c->vers[c->nvers].let='!';
  c->vers[c->nvers].prob=128;
  (c->nvers)++;
  }
 if( language!=LANG_RUSSIAN )
     {
     c->vers[c->nvers].let='l';
     c->vers[c->nvers].prob=(c->nvers)?100:128;
     (c->nvers)++;
     }
 c->vers[c->nvers].let=0;
 if( language==LANG_RUSSIAN && c->nvers==0 )
    {
    c->vers[0].let=bad_char;
    }

 insert_cell(c,cp);
 return c;
 }

static void dots()
 {
 cell *c,*cc,csv;
 uchar let;
 int16_t i,nv;
 version vers[VERS_IN_CELL];

 for (c=(cell_f())->next; c->next!=NULL; c=c->next)
  {
  c->left=c->col;
  c->right=c->col+c->w;
  if (c->flg&(c_f_dust|c_f_punct))
   continue;
  get_b_lines(c,&bl);
  if ((let=c->vers[0].let)==liga_ri && (cc=dot_ri(c))!=NULL ||
      (let=='i' || let=='j') && (cc=dot_ij(c))!=NULL ||
      (
	    ( language == LANG_POLISH &&
            ( let == ZZ_dot_accent || let == z_dot_accent ) ||
		  // 16.07.2001 E.P.
		  language == LANG_LITHUANIAN &&
            ( let == EE_dot_accent || let == e_dot_accent ) ||
		  // 30.05.2002 E.P.
		  language == LANG_TURKISH &&
            let == II_dot_accent
		) &&
            (cc=dot_ij(c))!=NULL ) ||
      (let==invers_exm || let==invers_qm) && (cc=dot_inv(c))!=NULL ||
      (let=='!' || let=='?') && (cc=dot_excl(c))!=NULL)
   {
   if (db_status)
    snap_newcell(cc);
   if (snap_activity('e'))
    {
    snap_show_text("delete dot");
    snap_monitor();
    }
   clist[0]=c;
   clist[1]=cc;
   // AL OK
   compose_cell(2,clist,c);
   if (let=='!' || let=='?')
    {c->left=c->col; c->right=c->col+c->w;}
   }

  for(i=0;i<c->nvers;i++)
    {
    vers[i]=c->vers[i];
    c->vers[i].let = let_sans_acc[c->vers[i].let];
    }
  if ((accent_tab[let] && accent_tab[let]!=ACC_DOT && type_acc(c,FALSE) ||
            c->bottom_accent && find_bottom_accent(c))   // 17.10.97 E.P.
      && accadr1!=NULL )
   {
   cc=accadr1;
   if (db_status)
    snap_newcell(cc);
   if (snap_activity('e'))
    {
    snap_show_text("delete accent");
    snap_monitor();
    }
   clist[0]=c;
   clist[1]=cc;
   // AL OK
   compose_cell(2,clist,c);
   if( c->accent_leader )
    c->accent_leader=0;
   }
   /*
  else if( accent_tab[let] && accent_tab[let]!=ACC_DOT &&
            (c->next->flg&(c_f_dust|c_f_punct)) &&
            (c->next->flg_new&c_fn_acccap) )
    { // OLEG : forming accent with cap for pass2
    cc=c->next;
    if (db_status)
        snap_newcell(cc);
    if (snap_activity('e'))
        {
        snap_show_text("delete accent satellit");
        snap_monitor();
        }
    clist[0]=c;
    clist[1]=cc;
    compose_cell(2,clist,c);
    if( c->accent_leader )
        c->accent_leader=0;
    }
    */
  for(i=0;i<c->nvers;i++)
    {
    c->vers[i]=vers[i];
    }
  if (accent_tab[let]& (ACC_2DOT
		|ACC_DOUBLE_RIGHT	// 31.08.2000 E.P.
		) && accadr2!=NULL) // ACC_2DOT 30.08.2000 E.P.
   {
   cc=accadr2;
   if (db_status)
    snap_newcell(cc);
   if (snap_activity('e'))
    {
    snap_show_text("delete second dot or double right accent");
    snap_monitor();
    }
   clist[0]=c;
   clist[1]=cc;
   // AL OK
   compose_cell(2,clist,c);
   if( c->accent_leader )
    c->accent_leader=0;
   }
  if ((let==bad_char || c->vers[0].prob<PROBMIN) && !(c->flg&c_f_dust))
   {
   if (c->cpos==ce_p_ex && c->h>c->w)
    {
    if (db_status)
     snap_newcell(c);
    if (snap_activity('e'))
     {
     snap_show_text("bad to dust");
     snap_monitor();
     }
    (c->prevl)->nextl=c->nextl;
    (c->nextl)->prevl=c->prevl;
    c->flg=c_f_dust|c_f_bad;
    err_pnlet(c);  // ALIF 940328
    }
   else
    {
    memcpy(&csv,c,sizeof(cell));
    short_recog_cell(c);
    levcut(c,1);
    for (nv=c->nvers,i=0; i<nv; i++)
     if (c->vers[i].let=='i') break;
    if (i<nv)
     {
     c->nvers=1;
     c->vers[0].let='i'; c->vers[0].prob=128;
     c->vers[1].let=0;
     if (db_status)
      snap_newcell(c);
     if (snap_activity('e'))
      {
      snap_show_text("bad to i");
      snap_monitor();
      }
     }
    else memcpy(c,&csv,sizeof(cell));
    }
   }
  if ((c->flg==c_f_bad || c->vers[0].prob<=PROBMIN) && !(c->flg&c_f_dust) &&
      c->cpos==ce_p_ex && c->h<=2*bl.ps/3)
   {
   if (db_status)
    snap_newcell(c);
   if (snap_activity('e'))
    {
    snap_show_text("bad to dust");
    snap_monitor();
    }
   (c->prevl)->nextl=c->nextl;
   (c->nextl)->prevl=c->prevl;
   c->flg=c_f_dust;
   err_pnlet(c);  // AL 940320
   }
  }
 }

static void quockets()
 {
 cell *c;
 uchar let;
 int16_t h;

 for (c=(cell_f())->nextl; c->next!=NULL; c=c->nextl)
  {
  get_b_lines(c,&bl);
  h=bl.ps;
  if (((let=c->vers[0].let)=='<' || let=='>') &&
      c->next->flg&(c_f_let|c_f_bad) && c->next->vers[0].let==let &&
       abs(c->h-c->next->h)<=(c->h+c->next->h)/4 &&
       MIN(c->h,c->next->h)<=h &&
       c->col+c->w-c->next->col<=(c->w+c->next->w)/3 &&
      abs(c->row+c->h/2-(c->next->row+c->next->h/2))<=(c->h+c->next->h)/6)
   {
   if (db_status)
    snap_newcell(c);
   if (snap_activity('e'))
    {
    snap_show_text("quocket: before");
    snap_monitor();
    }
   c->nvers=1;
   c->vers[0].let=(let=='<')?left_quocket:right_quocket;
   c->vers[0].prob=254;
   c->vers[1].let=0;
   clist[0]=c;
   clist[1]=c->next;
   // AL OK
   compose_cell(2,clist,c);
   c->flg=c_f_let;
   c->left=c->col;
   c->right=c->col+c->w;
   if (snap_activity('e'))
    {
    snap_show_text("quocket: after");
    snap_monitor();
    }
   }
  }
 }

static void punctsign(cell **ac1,cell **ac2)
 {
 int16_t h,d,dd,dp,fl,dc,p,lr,a;
 uchar let,str[80];
 cell *c;
 cell *c1;
 cell *c2;
 c1 = *ac1;
 c2 = *ac2;


 get_b_lines(c1,&bl);
 h=bl.ps;
 d=0;
 if (((let=c1->vers[0].let)=='!' || let=='?') && (c1->font|c1->font_new)&c_fp_it)
  d=-(h/10+2);
 if ((let=='J'|| let=='f') && c1->cpos&ce_p_Ql || let=='j')
  d=h/10+2;
 while (c2!=c1)
  {
  if( c2->flg==c_f_punct )
      {
      if (snap_activity('e'))
        {
        snap_show_text("skip old punct cell");
        snap_monitor();
        }
      c2 = c2->next;
      continue;
      }
  get_b_lines(c2,&bl);
  h=bl.ps;
  fl=0;
  dd=0;
  if (((c1->font|c1->font_new)&c_fp_it || c1->vers[0].let=='A') &&
      c2->row+c2->h<c1->row+c1->h/2)
   dd=h/10+1;
  if (c2->col+(c2->w)/2<=c1->col+1+d+dd &&
      c1->col-c2->col+(c1->w-c2->w)/2+dd>=h/6)
   {
   dp=dustpos(h,c2);

   if (db_status)
    snap_newcell(c2);
   if (snap_activity('e'))
    {
    sprintf(str,"punct recog, dustpos=%u",dp);
    snap_show_text(str);
    snap_monitor();
    }
   if (dp==5 && c2->row>bl.b3 && c2->row-bl.b3<=2 ||
      (5*c2->w>=c2->h || c2->w<=2 && !(c1->font&c_fp_ser)) &&
      ((p=c2->h+c2->w)>=HPERMIN && 5*p>=h-1 || h<=KEGM) &&
      (!dp || dp<5 && c2->col+(c2->w)/2<c1->col+d))
    {
    fl=1; dc=0; rstr=NULL;
    if (chkstar(h,dp,c2))
     c2->vers[0].let='*';
    else
     {
     if (chkquot(h,dp,c2))
      c2->vers[0].let=(dp<=1)?'"':
			low_quotes;	// Макрос 31.05.2001 E.P.
     else
      {
      if ((lr=chkquock(h,dp,c2))!=0)
       c2->vers[0].let=(lr<0)?'<':'>';
      else
       {
       if ((lr=chkquocks(h,dp,c2))!=0 && check_shevron(c2,1)>=3 )
	c2->vers[0].let=(lr<0)?left_quocket:right_quocket;
       else
	{
	if ((dc=chkdotcom(h,dp,c2))>0)
        {
           c2->vers[0].let=(dc&1)?((dp>2)?'.':':'):((dp>2)?',':0x27);
           if( line_tabcell && c2->vers[0].let==',' && 4*c2->h<=5*c2->w )
                c2->vers[0].let='.' ;
        }
	else
	 {
	 if (chkdash(h,dp,c2))
         {
             if (dp==1 || dp==2)
             {
                 if (5*c2->w<4*h || 3*c2->h>c2->w ||
                     15*c2->h>4*c2->w && !pitchsize)
                 {

                       c2->vers[0].let='-';

                 }
                 else
                  {
                     if ((3*c2->w>4*h || !pitchsize && 12*c2->w>11*h) &&
                         5*c2->h<c2->w || 9*c2->h<c2->w || 7*c2->w>12*h)
                      c2->vers[0].let='_';
                     else
                     {
                         if (
                               (
                                  (c=c2->prev)->prev==NULL ||
                                  (a=c2->col-c->col-c->w)>=5*h/8 ||
                                  !pitchsize && a>=h/2
                               ) &&
                               (
                                  (c=c2->next)->next==NULL ||
                                  (a=c->col-c2->col-c2->w)>=5*h/8 ||
                                  !pitchsize && a>=h/2 ||
                                  a<0 &&
                                  c2->col+c2->w>c->col+c->w &&
                                  (c=c->next)->next==NULL ||
                                  (a=c->col-c2->col-c2->w)>=5*h/8 ||
                                  !pitchsize && a>=h/2
                               )
                            )
                            c2->vers[0].let='_';
                         else
                            c2->vers[0].let='-';
                     }
                  }
             }
             else
                c2->vers[0].let='=';
         }
	 else
	  {
	  if (chkplus(h,dp,c2))
	   c2->vers[0].let='+';
	  else
	   {
	   if (chkslash(h,dp,c2))
	    c2->vers[0].let='/';
	   else
	    {
	    if (chkcircle(h,dp,c2))
	     c2->vers[0].let=(dp<=1)?'O':'o';
	    else
	     fl=0;
	    }
	   }
	  }
	 }
	}
       }
      }
     }
    }
   }
  if (fl && c2->prev->flg==c_f_let &&
      ( (let=c2->prev->vers[0].let)=='g' ||
	  	let=='r' ||
		language==LANG_TURKISH && let==g_semicircle // 27.06.2002 E.P.
	  ) &&
      c2->prev->cg_flag&c_cg_cutr && c2->cg_flag&c_cg_cutl &&
      abs(c2->row-c2->prev->row)<=2)
   fl=0;
  if (fl)
   {
   c2->flg^=(c_f_punct|c_f_dust);
   c2->vers[0].prob=254;
   if(0)
   if( c2->flg & (c_f_bad|c_f_let) )
        {
        cell *t,*n;
        if( c2->nextl==(cell *)0xffff0000 ||
            c2->prevl==(cell *)0xffff0000 )
            {
            for(t=c2->prev;t;t=t->prev)
                {
                if( t->flg&(c_f_let|c_f_bad|c_f_fict) )
                    break;
                }
            n=t->nextl;
            t->nextl=c2;
            c2->prevl=t;
            n->prevl=c2;
            c2->nextl=n;
            }
        }
   if (dc<3 && (!dc || dp<=2 || (c=c2->prev)->flg==c_f_fict ||
						   !(c->font&c_fp_undrln)))
    {
    c2->vers[1].let=0;
    c2->nvers=1;
    }
   else
    {
    if (dp && dp!=4)
     c2->vers[1].let=',';
    else
     {c2->vers[0].let=','; c2->vers[1].let='.';}
    c2->vers[1].prob=254;
    c2->vers[2].let=0;
    c2->nvers=2;
    }
   if (db_status) snap_newcell(c2);
   if (snap_activity('e'))
    {
    snap_show_text("punct recog OK");
    snap_monitor();
    }
   }
  else
   {
   if (c2->flg&c_f_bad)
   {
    dust_to_bad(c2);
        if (snap_activity('e'))
         {
             sprintf(str,"dust to bad");
             snap_show_text(str);
             snap_monitor();
         }
   }
   else
    {
    if( multy_language && c2->w>SMALL_SIZE &&
        c2->row>bl.b1 && c2->row+c2->h<bl.b4 )
        {
        dust_to_bad(c2);

				// Nick 1.06.2001
		if( c2->nvers <= 0 )
		  c2->flg = c_f_bad;

        c2->flg_new |= c_fn_temp;


        if (snap_activity('e'))
         {
             sprintf(str,"dust to bad");
             snap_show_text(str);
             snap_monitor();
         }
        }
    else
        {
        cell *cc2;
        cc2 = c2;

        no_cut(c2);
        c2=hide(c2);
            if (snap_activity('e'))
             {
                 sprintf(str,"kill cell");
                 snap_show_text(str);
                 snap_monitor();
             }
        if (*ac1==cc2)
          *ac1=c2->next;
        if (*ac2==cc2)
          *ac2=c2->next;
        }
    }
   }
  c2=c2->next;
  }
 }

#define BASEOK 15
/////////////////
/////////////////
static int16_t dustpos(int16_t h,cell *c)

/***********************************************************

	Counting the dust position in the string for cell
	Returns:
	0 - in the upper subline
	1 - on the small line
	2 - on the middle of line
	3 - on the main line
	4 - in the lower subline (touch the main line)
	5 - out of line
        6 - dust found
***********************************************************/

 {
 int16_t mid,a,d;

 get_b_lines(c,&bl);
 mid=c->row+c->h/2;
 if (mid==bl.bm)
  return 2;
 if (mid<bl.bm)
  {
//  if (bl.n1>0 && mid+h/7<bl.b1 || c->row+c->h<=bl.b1)
  if (mid<bl.b1)    //Paul  04-10-96
           return 5;
  if (mid<bl.b2 || !fax1x2 && mid==bl.b2)
           return 0;
  if (c->row-(h+6)/12-fax1x2<=bl.b2 && c->row+c->h-1<=bl.bm)
   return 1;
  return 2;
  }
 else
  {
  if (c->row-h/8>=bl.b3 || bl.n3>=BASEOK && c->row>bl.b3)
   return 5;
  if (c->row<bl.b3)
   {
   if ((a=c->row-bl.bm)<0 && c->row+c->h<=bl.b3 ||
	a<(d=bl.b3-(c->row+c->h))-1 && d>2)
    return 2;
   if ((c->h<=3 || mid<bl.b3) &&
       (c->row+c->h-bl.b3<=1 ||
	c->row+c->h-bl.b3<=MAX(7*c->h/24,2) &&
	c->row+c->h-bl.b3<=bl.b3-c->row-2))
    return 3;
   }
  return 4;
  }
 }
///////////////////
static int16_t chkdotcom(int16_t h,int16_t dp,cell *c)
 {
 int16_t x,y,dt,cm,hc,wc,wm,i,j,k,ls,s;
 uchar str[80];
 B_LINES bl;

 if (snap_activity('e'))
  {
  snap_show_text("check dotcom begin");
  snap_monitor();
  }
 get_b_lines(c,&bl);
 x=c->h-c->w;
 y=c->h+c->w;
 dt=cm=0;
 if (dp!=5 && !fax1x2 && c->h==1 && c->w>=3 ||
     dp==3 && 4*y<h && bl.b3-(c->row+c->h)>=MAX(2,c->h))
  goto ret;
 if (dp==5 ||
     5*y<=6*h &&
     2*c->w+((c->h==2 || dp==3)?1:0)>c->h+((fax1x2 && c->h>2)?2:0) &&
     (8*(x+((fax1x2)?3:0)+((c->cg_flag&c_cg_cutl)?2:0))>=-h ||
	 5*x>=-y || dp>=3 &&4*x>=-y || dp!=2 && c->h<=3 && c->w<=2*c->h) &&
     (x<=0 || ((!dp || dp==4)?8:5)*(x+((fax1x2)?1:0))<=h) &&
     (x<=0 || ((!dp || dp==4)?6:3)*x<=y) &&
     (dp!=2 || x+1+((fax1x2)?2:0)>=0 || c->w<=3) &&
     (c->h+((fax1x2)?2:0)>3 || dp!=4) &&
     (c->w<3 || c->h<2*c->w || c->row+c->h<=bl.b3) &&
     (!fax1x2 || dp!=4 || c->h<c->w))
  dt=1;
 if (dp!=5 && dp!=2 && (dp!=3 || c->row>=bl.bm-1) &&
     27*y>=8*h && 2*y<=3*h &&
     (x+((fax1x2)?3:0)>=((!dp || dp==4)?-1:0) &&
	      (c->h>=((fax1x2 || dp==4)?4:5) || dp<2) || x>0))
  cm=1;
 if (cm && dp<=1 && c->row>bl.b2)
  {cm=0; dt=1;}
 if (dt && cm)
  {
  if (rstr==NULL) rstr=save_raster(c);
  hc=c->h; wc=c->w;
  ls=(wc+7)>>3;
  if ((dp==4 || fax1x2 && dp==3 && -3*x<=2*c->h) && hc>=3)
   {
   for (j=0; !(rstr[j>>3]&(128>>(j%8))); j++) ;
   for (k=0; !(rstr[(hc-1)*ls+(k>>3)]&(128>>(k%8))); k++) ;
   for (s=0; !(rstr[(hc/2)*ls+(s>>3)]&(128>>(s%8))); s++) ;
   if (snap_activity('e'))
    {
    sprintf(str,"left abris: 0-%u,%u-%u,%u-%u",j,hc/2,s,hc-1,k);
    snap_show_text(str);
    snap_monitor();
    }
   if ((!j || !k) && s>=j && s>k)
    {dt=0; goto ret;}
   }
  for (wm=i=0; i<hc; i++)
   {
   for (j=0; !(rstr[i*ls+(j>>3)]&(128>>(j%8))); j++) ;
   for (k=wc-1; !(rstr[i*ls+(k>>3)]&(128>>(k%8))); k--) ;
   if (wm<(k-j+1)) wm=k-j+1;
   }
  if (snap_activity('e'))
   {
   sprintf(str,"max width = %u",wm);
   snap_show_text(str);
   snap_monitor();
   }
  if ((8*(hc-wm+((fax1x2)?1:0)+((!dp || dp==4)?1:0))>=h || 5*wm<=3*hc) &&
      wm<wc)
   dt=0;
  else
   {
   i=((2*hc+1)/3)*ls; if (hc<=4) i=(hc-1)*ls;
   for (j=0; !(rstr[i+(j>>3)]&(128>>(j%8))); j++) ;
   for (k=wc-1; !(rstr[i+(k>>3)]&(128>>(k%8))); k--) ;
   if (snap_activity('e'))
    {
    sprintf(str,"width on height %u = %u, left gap = %u",i/ls,k-j+1,j);
    snap_show_text(str);
    snap_monitor();
    }
   if (2*(k-j+1)<=wc || dp==4 && 7*(k-j+1)<=4*wc ||
       x+((fax1x2)?3:0)>1 && 4*(k-j+1)<=3*wc ||
       (dp==0 || dp==4) && 4*j>=wc && 4*(k-j+1)<=3*wc)
    {
    if (hc>4 || dp<2) dt=0;
    else
     {
     i=(hc-2)*ls;
     for (j=0; !(rstr[i+(j>>3)]&(128>>(j%8))); j++) ;
     for (k=wc-1; !(rstr[i+(k>>3)]&(128>>(k%8))); k--) ;
     if (snap_activity('e'))
      {
      sprintf(str,"width on height %u = %u",i/ls,k-j+1);
      snap_show_text(str);
      snap_monitor();
      }
     if (k-j+1<wc-1 || k-j+1==wc-1 && (hc>wc || dp==4))
      dt=0;
     }
    }
   else
    {
    if (dp>2)
     {
     if (dp==3 || x<=0) cm=0;
     }
    else
     {
     i=(hc/3)*ls; if (hc<=4) i=0;
     for (j=0; !(rstr[i+(j>>3)]&(128>>(j%8))); j++) ;
     for (k=wc-1; !(rstr[i+(k>>3)]&(128>>(k%8))); k--) ;
     if (snap_activity('e'))
      {
      sprintf(str,"width on height %u = %u, left gap = %u, right gap = %u",
	       i/ls,k-j+1,j,wc-k-1);
      snap_show_text(str);
      snap_monitor();
      }
     if (2*(k-j+1)<=wc+1 || x>1 && 4*(k-j+1)<3*wc ||
	 dp==0 && (4*j>=wc || 4*(wc-k-1)>=wc) && 4*(k-j+1)<3*wc)
      dt=0;
     else cm=0;
     }
    }
   }
  }
 if (dt && cm && (hc>=5 || fax1x2 && hc==4))
  {
  for (s=0,i=1; i<hc-1; i++,s+=k-j+1)
   {
   for (j=0; !(rstr[i*ls+(j>>3)]&(128>>(j%8))); j++) ;
   for (k=wc-1; !(rstr[i*ls+(k>>3)]&(128>>(k%8))); k--) ;
   }
  s=(s+(hc-2)/2)/(hc-2);
  if (snap_activity('e'))
   {
   sprintf(str,"average width = %u",s);
   snap_show_text(str);
   snap_monitor();
   }
  if ((6*(hc-s+((fax1x2 || !dp || dp==4)?1:0))<h && 5*s>3*hc) || s==wc)
   cm=0;
  }
ret:
 if (snap_activity('e'))
  {
  sprintf(str,"check dotcom : %s",
		       (dt)?((cm)?"both":"point"):((cm)?"comma":"none"));
  snap_show_text(str);
  snap_monitor();
  }
 return (dt+(cm<<1));
 }

static int16_t chkdash(int16_t h,int16_t dp,cell *c)
 {
 int16_t r;
 uchar str[80];

 r=(dp<=3 && /*9*(c->h)<=4*h*/27*c->h<=13*h && c->w>=MIN(4,h/3) &&
    (5*(c->w)>=2*h || dp==2 && c->w>c->h))?1:0;

 if (snap_activity('e'))
  {
  sprintf(str,"check dash : %s",(r)?"yes":"no");
  snap_show_text(str);
  snap_monitor();
  }
 return r;
 }

static int16_t chkquot(int16_t h,int16_t dp,cell *c)
 {
 uint16_t ls,i,j,k,i1,j1,k1,min,l,s,r,lu,ru,ld,rd;
 uchar str[80];

 if (snap_activity('e'))
  {
  snap_show_text("check quotes begin");
  snap_monitor();
  }
 r=0;
 if ((dp<=1 || dp==4) && 3*(c->h)>=h && 4*(c->w)>=h && (c->w)<=h)
  {
  if (rstr==NULL) rstr=save_raster(c);
  ls=(c->w+7)>>3;
  for (min=c->h+1,i1=(c->w)/2-1; i1<=(c->w)/2+1; i1++)
   {
   s=i1/8; l=128>>(i1%8);

   for (j1=0; !(rstr[j1*ls+s]&l); j1++) ;

   for (k1=c->h-1; !(rstr[k1*ls+s]&l); k1--)                  // AK add c/g 06.03.98
	if ( k1 == 0 )
		break;

   if (k1-j1+1<min)
    {
    min=k1-j1+1;
    i=i1; j=j1; k=k1;
    }
   }
  if (snap_activity('e'))
   {
   sprintf(str,"height in the middle (%u) = %u",i,min);
   snap_show_text(str);
   snap_monitor();
   }
  if (min<c->h && 14*min<=11*((c->h)+((fax1x2)?2:0)))
   {
   j1=j; k1=k;
   if (j) j--;
   if (k<c->h-1) k++;
   lu=rstr[j*ls+((c->w)>>5)]&(128>>(((c->w)>>2)%8));
   ru=rstr[j*ls+((3*(c->w))>>5)]&(128>>(((3*(c->w))>>2)%8));
   ld=rstr[k*ls+((c->w)>>5)]&(128>>(((c->w)>>2)%8));
   rd=rstr[k*ls+((3*(c->w))>>5)]&(128>>(((3*(c->w))>>2)%8));
   if (snap_activity('e'))
    {
    sprintf(str,"korners : lt=%u,rt=%u,lb=%u,rb=%u",lu,ru,ld,rd);
    snap_show_text(str);
    snap_monitor();
    }
   if (j1 && lu && ru || c->h-1-k1 && ld && rd)
    r=1;
   }
  }
 if (snap_activity('e'))
  {
  sprintf(str,"check quotes : %s",(r)?((dp<=1)?"upper":"lower"):"no");
  snap_show_text(str);
  snap_monitor();
  }
 return(r);
 }

static int16_t chkstar(int16_t h,int16_t dp,cell *c)
 {
 int16_t ls,i,j,l,r,x,y,n,p;
 uchar str[80];

 if (snap_activity('e'))
  {
  snap_show_text("check star begin");
  snap_monitor();
  }
 r=0;
 x=c->h-c->w; y=c->h+c->w;
 if (dp>2 || x>h/5 || x<-h/7 || 3*y<2*h) goto ret;
 if (rstr==NULL) rstr=save_raster(c);
 ls=(c->w+7)>>3;
 i=(c->w)>>4; l=128>>(((c->w)>>1)%8);
 for (x=j=0; j<c->h; j++) if (rstr[j*ls+i]&l) x++;
 if (snap_activity('e'))
  {
  sprintf(str,"height in the middle = %u",x);
  snap_show_text(str);
  snap_monitor();
  }
 if (x<c->h-2) goto ret;
 j=((c->h)/2)*ls;
 for (n=p=x=i=0; i<c->w; i++)
  if (rstr[j+(i>>3)]&(128>>(i%8)))
   {
   x++;
   if (!p) {n++; p=1;}
   }
  else p=0;
 if (snap_activity('e'))
  {
  sprintf(str,"width in the middle = %u, intersection number = %u",x,n);
  snap_show_text(str);
  snap_monitor();
  }
 if (n!=1 || x>2*c->w/3) goto ret;
 for (n=p=j=0; j<c->h; j++)
  if (rstr[j*ls]&64)
   {if (!p) {n++; p=1;}}
  else p=0;
 if (n!=2)
  for (n=p=j=0; j<c->h; j++)
   if (rstr[j*ls]&64)
    {if (!p) {n++; p=1;}}
   else p=0;
 if (snap_activity('e'))
  {
  sprintf(str,"left  intersection number=%u",n);
  snap_show_text(str);
  snap_monitor();
  }
 if (n!=2) goto ret;
 i=(c->w-1)/8; l=128>>((c->w-1)%8);
 for (n=p=j=0; j<c->h; j++)
  if (rstr[j*ls+i]&l)
   {if (!p) {n++; p=1;}}
  else p=0;
 if (n!=2)
  {
  i=(c->w-2)/8; l=128>>((c->w-2)%8);
  for (n=p=j=0; j<c->h; j++)
   if (rstr[j*ls+i]&l)
    {if (!p) {n++; p=1;}}
   else p=0;
  }
 if (snap_activity('e'))
  {
  sprintf(str,"right intersection number=%u",n);
  snap_show_text(str);
  snap_monitor();
  }
 if (n==2) r=1;
ret:
 if (snap_activity('e'))
  {
  sprintf(str,"check star : %s",(r)?"yes":"no");
  snap_show_text(str);
  snap_monitor();
  }
 return(r);
 }

static int16_t chkplus(int16_t h,int16_t dp,cell *c)
 {
 int16_t ls,i,j,l,r,x,y,lu,ru,ld,rd;
 uchar str[80];

 if (snap_activity('e'))
  {
  snap_show_text("check plus begin");
  snap_monitor();
  }
 r=0;
 x=c->h-c->w; y=c->h+c->w;
 if (dp<=2 && x<=h/8 && x>=-h/8 && y>=h)
  {
  if (rstr==NULL) rstr=save_raster(c);
  ls=(c->w+7)>>3;
  i=(c->w)>>4; l=128>>(((c->w)>>1)%8);
  for (x=j=0; j<c->h; j++) if (rstr[j*ls+i]&l) x++;
  if (snap_activity('e'))
   {
   sprintf(str,"height in the middle = %u",x);
   snap_show_text(str);
   snap_monitor();
   }
  if (x>=c->h-2)
   {
   j=((c->h)/2)*ls;
   for (x=i=0; i<c->w; i++) if (rstr[j+(i>>3)]&(128>>(i%8))) x++;
   if (snap_activity('e'))
    {
    sprintf(str,"width in the middle = %u",x);
    snap_show_text(str);
    snap_monitor();
    }
   if (x>=c->h-2)
    {
    lu=rstr[((c->h)/4)*ls+(((c->w)/4)>>3)]&(128>>((c->w)/4)%8);
    ru=rstr[((c->h)/4)*ls+(((3*(c->w))/4)>>3)]&(128>>((3*(c->w))/4)%8);
    ld=rstr[((3*(c->h))/4)*ls+(((c->w)/4)>>3)]&(128>>((c->w)/4)%8);
    rd=rstr[((3*(c->h))/4)*ls+(((3*(c->w))/4)>>3)]&(128>>((3*(c->w))/4)%8);
    if (snap_activity('e'))
     {
     sprintf(str,"korners : lt=%u,rt=%u,lb=%u,rb=%u",lu,ru,ld,rd);
     snap_show_text(str);
     snap_monitor();
     }
    if (!(lu|ru|ld|rd))
     r=1;
    }
   }
  }
 if (snap_activity('e'))
  {
  sprintf(str,"check plus : %s",(r)?"yes":"no");
  snap_show_text(str);
  snap_monitor();
  }
 return(r);
 }

static int16_t chkslash(int16_t h,int16_t dp,cell *c)
 {
 int16_t r,ls,i,j,s;
 uchar str[80];

 r=0;
 if ((dp==1 || dp==2) && c->h>=4 && c->h>=h/2-2 && c->w>=h-2)
  {
  if (rstr==NULL)
   rstr=save_raster(c);
  ls=(c->w+7)>>3;
  for (s=i=0; i<c->h; i++)
   {
   j=c->w-1-(c->w-1)*i/(c->h-1);
   if (rstr[ls*i+j/8]&(128>>(j%8))) s++;
   }
  if (s>=c->h-2 && !(rstr[ls]&(128>>1)) &&
      !(rstr[ls*(c->h-2)+(c->w-2)/8]&(128>>((c->w-2)%8))))
   r=1;
  }
 if (snap_activity('e'))
  {
  sprintf(str,"check slash : %s",(r)?"yes":"no");
  snap_show_text(str);
  snap_monitor();
  }
 return r;
 }

static int16_t chkcircle(int16_t h,int16_t dp,cell *c)
 {
 int16_t r,i;
 uchar flgsv,str[80];

 r=0;
 if (dp<4 && c->h>=h/2-1)
  {
  flgsv=(uchar)c->flg;
  short_recog_cell(c);
  c->flg=flgsv;
  for (i=0; i<c->nvers; i++)
   if (c->vers[i].let=='o')
    {r=1; break;}
  }
 if (snap_activity('e'))
  {
  sprintf(str,"check circle : %s",(r)?"yes":"no");
  snap_show_text(str);
  snap_monitor();
  }
 return r;
 }

static int16_t chkquock(int16_t h,int16_t dp,cell *c)
 {
 int16_t r,sl,sr,i,j,l;
 uchar str[80];

 r=0;
 if (dp==2 && 11*c->h>=4*h && c->h>=c->w)
  {
  if (rstr==NULL)
   rstr=save_raster(c);
  l=(c->w+7)>>3;
  for (sl=sr=i=0; i<c->h/3; i++)
   {
   for (j=0; j<c->w/2; j++)
    if (rstr[i*l+j/8]&(128>>(j%8)))
     sr++;
   for (j=(c->w+1)/2; j<c->w; j++)
    if (rstr[i*l+j/8]&(128>>(j%8)))
     sl++;
   }
  for (i=c->h/3; i<c->h-c->h/3; i++)
   {
   for (j=0; j<c->w/2; j++)
    if (rstr[i*l+j/8]&(128>>(j%8)))
     sl++;
   for (j=(c->w+1)/2; j<c->w; j++)
    if (rstr[i*l+j/8]&(128>>(j%8)))
     sr++;
   }
  for (i=c->h-c->h/3; i<c->h; i++)
   {
   for (j=0; j<c->w/2; j++)
    if (rstr[i*l+j/8]&(128>>(j%8)))
     sr++;
   for (j=(c->w+1)/2; j<c->w; j++)
    if (rstr[i*l+j/8]&(128>>(j%8)))
     sl++;
   }
  if (sr-sl>=sr/2 || 2*c->h>=3*c->w && sr-sl>=sr/3 ||
		     c->h>=2*c->w && sr-sl>=MAX(3,sr/4))
   r=1;
  else
   if (sl-sr>=sl/2 || 2*c->h>=3*c->w && sl-sr>=sl/3 ||
		      c->h>=2*c->w && sl-sr>=MAX(3,sr/4))
    r=-1;
  }
 if (snap_activity('e'))
  {
  sprintf(str,"check quocket : %s",(r)?((r>0)?"right":"left"):"no");
  snap_show_text(str);
  snap_monitor();
  }
 return r;
 }

int16_t chkquocks2(cell*c,puchar r,int16_t h,int16_t w,int16_t d);
static int16_t chkquocks(int16_t h,int16_t dp,cell *c)
 {
 int16_t i,di,i1,i2,j,j1,j2,l,d,r;
 uchar str[80];

 d=0;
 if (dp==2 && 11*c->h>=4*h && 3*c->w>=2*c->h &&
     c->w <=3*c->h ) // Pit 05-30-94 12:00pm
  {
  if (rstr==NULL)
   rstr=save_raster(c);
  l=(c->w+7)>>3;
  i=(c->h/2)*l;
  for (d=di=0; ; )
   {
   for (j1=0; !(rstr[i+di+j1/8]&(128>>(j1%8))); j1++) ;
   for (j2=c->w-1; !(rstr[i+di+j2/8]&(128>>(j2%8))); j2--) ;
   j2=c->w-1-j2;
   if (j1<=1 && j2>j1)
    {j=c->w-1-(5*c->w)/17; d=-1; break;}
   if (j2<=1 && j1>j2)
    {j=(5*c->w)/17; d=1; break;}
   if (!di)
    di=-l;
   else
    {
    if (di<0)
     di=l;
    else
     goto ret;
    }
   }
   if(chkquocks2(c,rstr,c->h,c->w,d)) goto ret;
{
extern uchar multy_language;
   // >> similar з , << similar Є,­
   if( multy_language && d!=0 && c->h>12 )
    {
    int16_t ii,jj,ll;
    if( d>0 && j1*8<c->w )
      {
      d=0;
      goto ret;
      }
    if( d>0 )
      {
      for (ll=0,ii=1; ii<c->h-1; ii++)
        {
        for (jj=c->w-1; !(rstr[l*ii+jj/8]&(128>>(jj%8))); jj--) ;
        jj = (c->w-1-jj);
        if( ll<jj ) ll=jj;
        }
      }
    if( d<0 )
      {
      for (ll=0,ii=1; ii<c->h-1; ii++)
        {
        for(jj=0;!(rstr[l*ii+jj/8]&(128>>(jj%8)));jj++);
        if( ll<jj ) ll=jj;
        }
      }
    if( ll<3 )
      {
      d=0;
      goto ret;
      }
    }
}
  for (r=0; r<=2; r++,j+=d)
   {
   for (i1=0; !(rstr[l*i1+j/8]&(128>>(j%8))); i1++) ;
   for (i2=c->h-1; !(rstr[l*i2+j/8]&(128>>(j%8))); i2--) ;
   i2=c->h-1-i2;
   if (i1 && i2 && i1+i2>=MAX(2,c->h/5))
    {
    for (j1=j; j1>=0 && !(rstr[l*(i1-1)+j1/8]&(128>>(j1%8))); j1--) ;
    for (j2=j; j2<c->w && !(rstr[l*(i1-1)+j2/8]&(128>>(j2%8))); j2++) ;
    if (j1<0 || j2==c->w)
     continue;
    for (j1=j; j1>=0 && !(rstr[l*(c->h-i2)+j1/8]&(128>>(j1%8))); j1--) ;
    for (j2=j; j2<c->w && !(rstr[l*(c->h-i2)+j2/8]&(128>>(j2%8))); j2++) ;
    if (j1<0 || j2==c->w)
     continue;
    break;
    }
   }
  if (r>2)
   {d=0; goto ret;}
  r=MAX(1,c->h/6);
  for (i=r,j1=(d<0)?0:c->w-1; !(rstr[l*i+j1/8]&(128>>(j1%8))); j1-=d) ;
  for (i=c->h-1-r,j2=(d<0)?0:c->w-1; !(rstr[l*i+j2/8]&(128>>(j2%8)));
								     j2-=d) ;
  if (d>0)
   {j1=c->w-1-j1; j2=c->w-1-j2;}
  if (j1<MIN(1,c->w/6) || j2<MIN(1,c->w/6))
   d=0;
  }
ret:
 if (db_status)
  snap_newcell(c);
 if (snap_activity('e'))
  {
  sprintf(str,"check quockets : %s",(d)?((d>0)?"right":"left"):"no");
  snap_show_text(str);
  snap_monitor();
  }
 return d;
 }

static void dust_to_bad(cell *c)
 {
 int16_t i;
 cell *c1;

 if (db_status)
  snap_newcell(c);
 short_recog_cell(c);
 levcut(c,1);
 for (i=0; i<c->nvers; i++)
  if (c->vers[i].let=='i') break;
 if (i<c->nvers)
  {
  if (snap_activity('e'))
   {
   snap_show_text("dust to i");
   snap_monitor();
   }
  c->nvers=1;
  c->vers[0].let='i';
  c->vers[0].prob=128;
  c->vers[1].let=0;
  }
 else
  {
  if (snap_activity('e'))
   {
   snap_show_text("dust to bad");
   snap_monitor();
   }
  set_bad_cell(c);
  }
 for (c1=c->prev;
	    !(c1->flg&(c_f_let|c_f_bad|c_f_fict)) || c1->flg&c_f_punct;
							      c1=c1->prev) ;
 c1->nextl->prevl=c;
 c->nextl=c1->nextl;
 c1->nextl=c;
 c->prevl=c1;
 }

static void intval(cell *c,cell *cb)
 {
 cell *c1,*c2,*c3,/* *sav1,*sav2, */ *cbb;
 uchar l1,l2,l3;
 int16_t fe,fm;
 int16_t h,d,max,s,hh,ww;

 cbb=cb->prev;
 for (c1=cbb->next; c1!=c; c1=c1->next)
  {
  if (!(c1->flg&c_f_punct)
      || c1->cg_flag&c_cg_comp)  //Paul 07-10-96
   continue;
  get_b_lines(c1,&bl);
  h=bl.ps;
  l1=c1->vers[0].let;
  fe=fm=0;
  c2=c1->next;
  if (db_status) snap_newcell(c1);
  if (snap_activity('e'))
   {
   snap_show_text("pairing");
   snap_monitor();
   }
  if (c2==c)
   fe=1;
  else
   {
   fm=1;
   l2=c2->vers[0].let;
   if ((l1==':' || l1=='-' || l1=='_') &&
       (l2==':' || l2=='-' || l2=='_') &&
       c2->col-(c1->col+c1->w)<=MAX(3,h/5) &&
       abs(c1->row-c2->row)<=1 &&
       abs(c1->row+c1->h-(c2->row+c2->h))<=1 &&
       //!line_tabcell &&
       (d=dustpos(h,c1))>=0 && d<=3 &&
       MIN(c1->row,c2->row)>bl.b1)
    {
    hh=MAX(c2->row+c2->h,c1->row+c1->h)-MIN(c2->row,c1->row);
    ww=MAX(c2->col+c2->w,c1->col+c1->w)-MIN(c2->col,c1->col);
    if (ww<h || 9*hh>2*ww || pitchsize && ww<=3*pitchsize/2)
     c2->vers[0].let='-';
    else
     {
     if (6*ww>7*h || 9*hh<ww)
      c2->vers[0].let='_';
     else
      {
      if (((c3=c1->prev)->prev==NULL || c1->col-c3->col-c3->w>=h/2) &&
	  ((c3=c2->next)->next==NULL || c3->col-c2->col-c2->w>=h/2))
       c2->vers[0].let='_';
      else
       c2->vers[0].let='-';
      }
     }
    }
   else
    {
    if ((l1==0x27 || l1==':') && (l2==0x27 || l2==':') &&
	MAX(c1->h,c2->h)>=h/5 &&
	(d=c2->col-c1->col-c1->w)<=h/2 &&
	c1->row+c1->h>c2->row && c2->row+c2->h>c1->row &&
	((c3=c2->next)==c || (l3=c3->vers[0].let)!=0x27 && l3!=':' ||
	 d<=c3->col-c2->col-c2->w))
     {
     if (abs(c1->h-c2->h)<=MAX(2,MAX(c1->h,c2->h)/3))
      c2->vers[0].let='"';
     else
      {
/*      if (c1->h>c2->h)
       {
       sav1=c2->next; sav2=c2->prev;
       memcpy(c2,c1,sizeof(cell));
       c2->next=sav1; c2->prev=sav2;
       }   */
      c2->vers[0].let=0x27;
      }
     }
    else
     {
     if (l1==',' && l2==',' && abs(c1->h-c2->h)<=(c1->h+c2->h)/4 &&
	 abs(c1->row+c1->h/2-(c2->row+c2->h/2))<=h/4 &&
	 c2->col-(c1->col+c1->w)<=h/2 && !line_tabcell)
      c2->vers[0].let=low_quotes;	// Макрос 31.05.2001 E.P.
     else
      {
      if ((l1=='/' && (l2==0x27 || l2==':' || l2=='O') ||
	  (l1==0x27 || l1==':' || l1=='O') && l2=='/') &&
	   (c3=c2->next)!=c && ((l3=c3->vers[0].let)=='o' || l3=='.'))
       {c2->vers[0].let='%'; fm=2;}
      else
       {
       if (l1=='-' && l2=='-' &&
	  abs(c1->col+c1->w/2-(c2->col+c2->w/2))<(c1->w+c2->w)/8)
	c2->vers[0].let='=';
       else
	{
	if ((l1=='<' || l1=='>') && l2==l1)
	 c2->vers[0].let=(l1=='<')?left_quocket:right_quocket;
	else
	 {
	 if (c1->col+MAX(3,c1->w)+((italic && c1->row>c2->row)?3:0)<c2->col
	     || MIN(c1->row,c2->row)<
			     cbb->row-((fax1x2)?3:2)-((pitchsize)?1:0) &&
		MAX(c1->row,c2->row)>=bl.bm
	      /*|| (d=c1->row-c2->row)<=4 && d>=-4*/)
	  {fe=1; fm=0;}
	 else
	  {
	  if (((l1==':' || l1=='-' || l1==0x27 && 7*(c1->h-c1->w)<=h &&
		  (abs(c1->h+c1->w-(c2->h+c2->w))<=3 || dustpos(h,c1)) ||
						   fax1x2 && l1=='=') &&
	       (l2=='.' && c2->row<bl.b3 ||
		  (l2==':' || fax1x2 && l2=='=') && dustpos(h,c2)>=2 ||
		  l2==',' && abs(c1->h+c1->w-(c2->h+c2->w))<=3 &&
						  c2->row+c2->h<=bl.b3) ||
	       (l2==':' || l2=='-' || l2==0x27 && 7*(c2->h-c2->w)<=h &&
		  (abs(c1->h+c1->w-(c2->h+c2->w))<=3 || dustpos(h,c2)) ||
						   fax1x2 && l2=='=') &&
	       (l1=='.' && c1->row<bl.b3 ||
		  (l1==':' || fax1x2 && l1=='=') && dustpos(h,c1)>=2 ||
		  l1==',' && abs(c1->h+c1->w-(c2->h+c2->w))<=3 &&
						  c1->row+c1->h<=bl.b3)) &&
	      (abs(c1->h-c2->h)<=2 ||
				   2*c1->h<=3*c2->h && 2*c2->h<=3*c1->h) &&
	      (abs(c1->w-c2->w)<=2 ||
				   2*c1->w<=3*c2->w && 2*c2->w<=3*c1->w) &&
	      (c1->row>=c2->row || c2->col+c2->w/2-(c1->col+c1->w/2)<=
						  MAX(2,MIN(c1->w,c2->w)/2)))
	   {
	   c2->vers[0].let=':';
	   if (c1->nvers==2 || c2->nvers==2 || l1==',' || l2==',')
	    {
	    if (c1->nvers==2 && c1->vers[0].let=='.' ||
		c2->nvers==2 && c2->vers[0].let==',' || l1==',' || l2==',')
	     c2->vers[1].let=';';
	    else
	     {c2->vers[0].let=';'; c2->vers[1].let=':';}
	    c2->vers[1].prob=254;
	    c2->vers[2].let=0;
	    c2->nvers=2;
	    }
	   }
	  else
	   {
	   if (((l1==':' || l1=='-' && 2*c1->h>=c1->w ||
		 l1==0x27 && 7*(c1->h-c1->w)<=h ||
		 fax1x2 && l1=='=') && c1->row<=bl.bm &&
		(l2==',' || l2=='.' && dustpos(h,c2)==3 && c2->h>c1->h) ||
		(l2==':' || l2=='-' ||
		 l2==0x27 && 7*(c2->h-c2->w)<=h ||
		 fax1x2 && l2=='=') && c2->row<=bl.bm &&
		(l1==',' || l1=='.' && dustpos(h,c1)==3 && c1->h>c2->h)))
	    {
	    if (abs(c1->w-c2->w)<=3 ||
		c1->row<c2->row && 5*c1->w>=2*c2->w ||
		c2->row<c1->row && 5*c2->w>=2*c1->w)
	     c2->vers[0].let=';';
	    else
	     {fe=1; fm=0;}
	    }
	   else
	    {
	    if ((l1=='-' || l1=='_' || l1=='=') &&
		(l2=='-' || l2=='_' || l2=='='))
	     {
	     if (MAX(c1->row,c2->row)-MIN(c1->row+c1->h,c2->row+c2->h)<=h/2)
	      c2->vers[0].let='=';
	     else
	      c2->vers[0].let=':';
	     }
	    else
	     {
	     if (l1=='.' && l2=='.')
	      c2->vers[0].let=',';
	     else
	      {
	      if ((l1==':' || l1==0x27) && (l2==':' || l2==0x27))
	       c2->vers[0].let=0x27;
	      else
	       {fe=1; fm=0;}
	      }
	     }
	    }
	   }
	  }
	 }
	}
       }
      }
     }
    }
   }

  if (fe && (memchr(".:=><Oo",l1,6) || l1==0x27 || l1=='/'))
   {
   if (l1==':' && c1->h+c1->w>h/3)
    {
    d=dustpos(h,c1);
    if (!pitchsize && d==2 && c1->w<=c1->h+1 &&
    MAX(c1->w,c1->h)>bl.ps/3 && //Oleg
	((c3=c1->prev)->prev==NULL || c1->col-c3->col-c3->w>=h/2) &&
	((c3=c1->next)->next==NULL || c3->col-c1->col-c1->w>=h/2))
     {
     c1->vers[0].let=liga_bull;
     c1->vers[0].prob=150;
     }
    else
     {
     if ((d==2 || d==1 && c1->row>bl.b2) &&
	 bl.b3-(c1->row+c1->h)>=3 && (bl.n2>0 || c1->row<=bl.bm) &&
	 c1->w+((c1->next->flg&c_f_fict)?1:0)>c1->h+((fax1x2)?2:0))
      {
      c1->vers[0].let=c1->h>c1->w+1?'-':'.'; // Oleg
      c1->vers[0].prob=150;
      }
     else
      {
      if (d>2 ||
	  d==2 && c1->row+c1->h/2>bl.bm && !(c1->cg_flag&c_cg_cut) &&
	  (c1->row>bl.bm+1 || c1->row+c1->h+MAX(2,bl.ps/6)>=bl.b3) ||
	  d<2 && c1->row<bl.b2 && c1->h+((fax1x2)?2:0)>=c1->w &&
							   c1->h>=bl.ps/4)
       {
       c1->vers[0].let=(d<2)?0x27:'.';
       c1->vers[0].prob=150;
       }
      else
       {
       if (c1->flg&c_f_bad)
	dust_to_bad(c1);
       else
	{
	no_cut(c1);
    c1=hide(c1);
	}
       }
      }
     }
    }
   else
    {
    if (l1=='=' && dustpos(h,c1)==3 && c1->row+c1->h/2<=bl.b3-bl.ps/3)
     {
     if (c1->w<h || 9*c1->h>2*c1->w || pitchsize && c1->w<=3*pitchsize/2)
      c1->vers[0].let='-';
     else
      {
      if (11*c1->w>12*h || 9*c1->h<c1->w)
       c1->vers[0].let='_';
      else
       {
       if (((c3=c1->prev)->prev==NULL || c1->col-c3->col-c3->w>=h/2) &&
	   ((c3=c1->next)->next==NULL || c3->col-c1->col-c1->w>=h/2))
	c1->vers[0].let='_';
       else
	c1->vers[0].let='-';
       }
      }
     c1->vers[0].prob=150;
     }
    else
     {
     if ((l1=='<' || l1=='>') &&
	 ((c3=c1->prev)->vers[0].let==l1 &&
	     c1->col-(c3->col+c3->w)<=(c1->w+c3->w)/4 ||
	  (c3=c1->next)->vers[0].let==l1 &&
	     c3->col-(c1->col+c1->w)<=(c1->w+c3->w)/4))
      {
      cell *cp;  // AL 940320
      c3->vers[0].let=(l1=='<')?left_quocket:right_quocket;
      c3->vers[0].prob=254;
      c3->vers[1].let=0;
      c3->nvers=1;
      if (c3->flg&c_f_bad)
       c3->flg=c_f_let;
      cp = c1->prev;  // AL 940320
      clist[0]=c1;
      clist[1]=c3;
      compose_cell(2,clist,c3);
      if( c3->nextl==(cell *)0xffff0000 && c3->nextl==(cell *)0xffff0000 )
        c3->flg=c_f_punct;
      c3->left=c3->col;
      c3->right=c3->col+c3->w;
      //c1=c1->prev;
      c1=cp; // AL 940320
      }
     else
      {
      if ((l1!=0x27 || c1->row>=bl.b2) && (l1!='.' || c1->row>bl.b3))
       {
       if (c1->flg&c_f_bad)
	dust_to_bad(c1);
       else
	{
	no_cut(c1);
    c1=hide(c1);
	}
       }
      }
     }
    }
   }

  if (fm)
  {
       clist[0]=c1;
       clist[1]=c2;
       if( fm==2 ) // Oleg & Pit : 13-07-1999 : Check non init pointer
        clist[2]=c3;
       no_cut(c1);
       if (fm==2)
        no_cut(c3);
       // AL OK
       compose_cell((int16_t)(fm+1),clist,c2);
       c2->left=c2->col;
       c2->right=c2->col+c2->w;
       c1=c2;
       if (db_status)
           snap_newcell(c1);
       if (snap_activity('e'))
       {
           snap_show_text("pairing OK");
           snap_monitor();
       }
  }
  }

 for (c1=cbb->next; c1!=c; c1=c1->next)
  {
      get_b_lines(c1,&bl);
      h=bl.ps;
      if (((l1=c1->vers[0].let)==':' || l1==';') &&
          (c2=c1->next)!=c && c2->vers[0].let==0x27 &&
          c2->col-c1->col-c1->w<=h/2)
       {
          c2->vers[0].let='"';
          for (d=0; d<c1->nvers; d++)
          {
              if (c1->vers[d].let==':') c1->vers[d].let='.';
              if (c1->vers[d].let==';') c1->vers[d].let=',';
          }
          c1->env=c2->env=NULL;
       }
  }

 for (c1=cbb->next; c1!=c; c1=c1->next)
  if (((l1=c1->vers[0].let)=='.' || l1==',') &&
      (c2=c1->next)!=c &&
      ((l2=c2->vers[0].let)=='.' || l2==',') &&
      (c3=c2->next)!=c &&
      ((l3=c3->vers[0].let)=='.' || l3==',') &&
      c1->prev->vers[0].let!='.' &&
      !(l1==',' && c3->next->vers[0].let=='.'))
   {
   if ((max=MAX(c1->h,MAX(c2->h,c3->h)))-MIN(c1->h,MIN(c2->h,c3->h))<=
					      (s=(c1->h+c2->h+c3->h)/6))
    {
         c1->vers[0].let='.';
         c2->vers[0].let='.';
         c3->vers[0].let='.';
    }
   else
    {    cell *cc1;   // AL 940320
         cc1=c1->prev;
         if (c1->h<max-s)
         hide(c1);
         if (c2->h<max-s)
         hide(c2);
         if (c3->h<max-s)
         hide(c3);
         c1 = cc1; // AL
    }
   }

 for (c1=cbb->next; c1!=c; c1=c1->next)
  {
  get_b_lines(c1,&bl);
  h=bl.ps;
  if (c1->vers[0].let==0x27 &&
      (c2=c1->next)!=c && c2->vers[0].let==0x27 &&
      c2->col-c1->col-c1->w<=h/2 &&
      c1->row+c1->h>c2->row && c2->row+c2->h>c1->row)
   {
   c1->vers[0].let='"';
   clist[0]=c1;
   clist[1]=c2;
   // AL OK
   compose_cell(2,clist,c1);
   c1->left=c1->col;
   c1->right=c1->col+c1->w;;
   }
  }

 for (c1=cbb->next; c1!=c; c1=c1->next)
  {
  get_b_lines(c1,&bl);
  h=bl.ps;
  if (c1->vers[0].let==0x27 &&
      (c2=c1->next)!=c && ((l2=c2->vers[0].let)=='.' || l2==',') &&
      (c3=c2->next)!=c && c3->vers[0].let==0x27 &&
      c3->col-c1->col-c1->w<=h/2 &&
      c1->row+c1->h>c3->row && c3->row+c3->h>c1->row)
   {
   clist[0]=c1;
   clist[1]=c3;
   if (c2->col+c2->w/2>(c1->col+c3->col+c3->w)/2)
    {
    c1->vers[0].let='"';
    // AL OK
    compose_cell(2,clist,c1);
    c1->left=c1->col;
    c1->right=c1->col+c1->w;
    }
   else
    {
    cell *cp;   // AL 940320
    cp=c1->prev;
    c3->vers[0].let='"';
    compose_cell(2,clist,c3);
    c3->left=c3->col;
    c3->right=c3->col+c3->w;
    //c1=c1->prev->next;
    c1=cp->next;
    }
   }
  }

 for (c1=cbb->next; c1!=c; c1=c1->next)
  if (c1->flg==(c_f_punct|c_f_bad))
   c1->flg=c_f_punct;
 }

static void no_cut(cell *c)
 {

 if (c->cg_flag&c_cg_cutl)
  (c->prev)->cg_flag-=(c->prev)->cg_flag&c_cg_cutr;
 if (c->cg_flag&c_cg_cutr)
  (c->next)->cg_flag-=(c->next)->cg_flag&c_cg_cutl;
 }

void clean_punct_temp()
{
cell *C=cell_f()->next;
for (; C->next; C=C->next)
    {
    if( (C->flg_new&c_fn_temp) &&
        (C->flg & (c_f_punct|c_f_let)) &&
         C->nvers && C->vers[0].prob>100 )
        C->flg_new -= c_fn_temp;
    }

return;
}

void clean_punct()
{
  cell *L,*R,*C=cell_f()->next;
  char str[80];
  int16_t gap=get_gap();

  for (; C->next; C=C->next)
  {
    if (!(C->flg & c_f_punct))
        continue;
    if (C->vers[0].let=='\'' && language == LANG_ENGLISH)
        continue;
    for (L=C; L->prev && !(L->flg & (c_f_let+c_f_bad)); L=L->prev);
    if (!(L->flg & (c_f_let+c_f_bad)))
        continue;
    R=L->nextl;
    if (!(R->flg & (c_f_let+c_f_bad)))
        continue;
    if (R->col - (L->col+L->w) <= gap )
    {
      if (snap_activity('f'))
        {
        sprintf(str,"too closed - delete");
        snap_show_text(str);
        snap_newcell(C);
        snap_monitor();
        }
      C=hide(C);
    }
  }
}

void clean_line()
{
  cell *B,*C=cell_f()->next;

  while (C->next)
  {
    if (C->flg&c_f_dust)
      C=del_cell(C);
    else
    if (C->flg&c_f_punct && strchr(".,-",C->vers[0].let))
    {
      B=C;
      while (B->next && !(B->flg & (c_f_let | c_f_bad)))  B=B->next;
      if (B->flg & (c_f_let | c_f_bad) && contain(B,C))  C=del_cell(C);
    }
    C=C->next;
  }
}

static int16_t contain(cell *new, cell *old)
//
//	This procedure check whether new cell contains old one.
//
 {

 if (new->r_col >= old->r_col)
  return 0;
 if (new->r_col+new->w <= old->r_col+old->w)
  return 0;
 if (new->r_row >= old->r_row)
  return 0;
 if (new->r_row+new->h <= old->r_row+old->h)
  return 0;
 return 1;
 }
/////////////////////////////////////////////////////////////////////////////////////////////
// AK change at 19.03.98
cell *hide(cell *C)
{
	cell *B=C;

	if (language == LANG_RUSSIAN)
	{
		C=C->prev;

		if (B->flg & c_f_punct)
			B->flg ^= c_f_dust | c_f_punct;

		B->complist=(c_comp *)sv_fantom;
		sv_fantom=del_save_cell(B);
	}
	else
		C=del_cell(C);

	return C;
}
///////////////////////////////////////////////////////////////////////////////////////////
void hide_dusts(uchar mode)
{
	cell *C;

	for (C=cell_f()->next; C && C != cell_l()/*C->next*/; C=C->next)
		if (C->flg&c_f_dust && !(mode && C->cg_flag&c_cg_cut))
		{
			C=hide(C);
		}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void recover_dusts()
{
  cell *B=cell_l();
return;
  while (sv_fantom)
  {
    B=rest_cell(sv_fantom,B);
    sv_fantom=(puchar)(B->complist);
  }
}
void clear_twin_flags(void)
{
cell *c,*e;

for(c=cell_f()->next, e=cell_l(); c!=e; c=c->next )
    {
    if( c->flg!=c_f_bad && (c->flg&c_f_bad)!=0 &&
        c->flg!=(c_f_bad|c_f_solid) && c->flg&(c_f_bad|c_f_solid)!=0 )
        dust_to_bad(c);
    }
return;
}
//*******************************************************************
void czech_dt_glue_apostroph()
{
// ACC_ROOF_INF над d,t в Чешском похож на апостроф. 07.09.2000 E.P.

 cell *c,*cc;
 uchar let;
 int16_t gap=get_gap();
 int16_t d;

for (c=(cell_f())->next; (cc=c->next)!=NULL; c=c->next)
	{
	c->left=c->col;
	c->right=c->col+c->w;

	if (c->flg&(c_f_dust|c_f_punct))
		continue;

	get_b_lines(c,&bl);
	let=c->vers[0].let;

	if (let != 'd' && let != 't')
		continue;

	// Апостроф после d, t
	if (cc->vers[0].let != '\'')
		continue;

	// Достаточно ли близко
	d= cc->col - c->right;
	if (d >= gap)
		continue;

	// Склеить
	clist[0]=c;
	clist[1]=cc;
	compose_cell(2,clist,c);

	// Заменить версию
	switch(let)
		{
		case 'd': c->vers[0].let = d_inv_roof; break;
		case 't': c->vers[0].let = t_inv_roof; break;
		}

	if (db_status)
		snap_newcell(c);

	if (snap_activity('e'))
		{
		snap_show_text("czech_dt_glue_apostroph");
		snap_monitor();
		}
	}

}
//*******************************************************************
