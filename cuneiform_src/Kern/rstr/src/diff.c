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
/*	        Differenciation of some letters	                     */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>	// 17.06.2002 E.P.

#include "struct.h"
#include "ligas.h"
#include "status.h"
#include "lang.h"
#include "func.h"
#include "ukr.h"
#include"linutil.h"

#include "compat_defs.h"

extern uchar fax1x2;
extern int16_t pitchsize;
extern int16_t current_fragment;
extern int16_t line_number;
extern uchar language;
extern uchar db_status;

static B_LINES bl;
static int16_t H;
static int16_t curr_frag;
static cell *clist[2];

int16_t dist_point_of_i_1,dist_point_of_i_2,
    dist_point_of_i_3,dist_point_of_i_b;

int16_t AnglesCurve(uchar *raster,int16_t D_X,int16_t hei);

static int16_t upper_dot_I(cell *c);
static int16_t upper_right_angle(cell *c);
static void test_rq(cell *);
static void test_c(cell *);
static int16_t long_line_c(cell *);
static void crit_oa(cell *);
static int16_t short_lines(cell *);
// static int16_t n_or_ri(cell *);
// static void n_or_u(cell *,version *);
// static uchar left_line(cell *);
//static int16_t ij_dot(cell *);
static int16_t excl_dot(cell *);
static int16_t inv_dot(cell *);
static int16_t not_rt(cell *);
static int16_t long_lines_rt(cell *);
static int16_t not_ff(cell *);
static int16_t long_lines_ff(cell *);
// static int16_t not_1(cell *);
// static int16_t not_no(cell *);
static int16_t upper_right_line(cell *);
uint32_t check_letter(cell *c, uchar let); // 18.06.2002 E.P.

void criteries()
 {
 cell *c;

 if (!line_number || current_fragment!=curr_frag)
  {
  dist_point_of_i_1=dist_point_of_i_2=dist_point_of_i_3=
  dist_point_of_i_b=0;
  curr_frag=current_fragment;
  }

 c=cell_f();
 while ((c=c->nextl)->nextl != NULL)
 {
  criteria(c);
 if(language == LANG_RUSSIAN )
  {
  r_criteria(c,NULL);
  if( c->nvers>0 && memchr("’вѓЈ",c->vers[0].let,4) &&
      !is_russian_baltic_conflict(c->vers[0].let)&& // 17.07.2001 E.P.
	  !is_russian_turkish_conflict(c->vers[0].let) 	// 21.05.2002 E.P.
	)
    stick_center_study(c,NULL,1);
  }
 }
 }

//extern uchar pass_cut;
void criteria (cell *c)
 {
 uchar  let,*raster;
 version *vers;
 int16_t r,flacc,flon,inc; //,dl,dr;
 version    save[VERS_IN_CELL];
 int16_t    snvers;

// #define PROB_V_OK 170
if( c->nvers>0 )
    memcpy(save,c->vers,c->nvers);
snvers=c->nvers;
 if (!c->nvers)
  return;
 get_b_lines(c,&bl);
 H=(int16_t)get_size();
 if (c->nvers==1 && c->vers[0].let=='>')
  test_rq(c);
 if (c->nvers==1 && ((let=c->vers[0].let)=='c' || let=='C') ||
     (c->nvers==2 && (c->vers[0].let=='c' && c->vers[1].let=='C' ||
		      c->vers[0].let=='C' && c->vers[1].let=='c')))
  test_c(c);
 if (c->nvers>=2 && (c->vers[0].let=='a' && c->vers[1].let=='o' ||
		     c->vers[0].let=='o' && c->vers[1].let=='a'))
  crit_oa(c);
 for (vers=c->vers,flacc=flon=0; (let=vers->let)!=0; vers++)
  {
{ //Andrew
  if( (let=vers->let) == liga_CC || let == liga_CR ){
     raster = save_raster(c);
     inc = AnglesCurve(raster,(int16_t)((c->w+7)/8),c->h);
     inc = (4-inc)*40;
     if( 3*c->w > 4*c->h )
       inc += 150;
     if( 3*c->w < 2*c->h )
       inc += 150;

     if( c->env && c->env->nl<4 )
       inc += 100;

     if( vers->prob > inc )
       vers->prob -= inc;
     else
       vers->prob = 0;
  }
}


  if (((language==LANG_FRENCH || language==LANG_ITALIAN) &&
				    memchr("ACEIOUaceiou",let,12) || // Включая нижние акценты "c" 05.07.2002 E.P.
       language==LANG_SPANISH && memchr("AEINOUaeniou",let,12) ||
       language==LANG_GERMAN && memchr("AOUaou",let,6) ||
       language==LANG_RUSSIAN && memchr("Ґ…",let,2) ||
       language==LANG_CROATIAN && memchr("SZCszc",let,6) /*&& !pass_cut*/ ||
       language==LANG_POLISH   && memchr("AESZCNOaeszcno",let,14) || 	// Включая нижние акценты a,e 05.07.2002 E.P.
       language==LANG_PORTUGUESE && memchr("AEOUIaeoui",let,10) ||
       language==LANG_SWEDISH && memchr("AEOaeo",let,6)) && !flacc ||

	   // 05.09.2000 E.P.
	   language==LANG_CZECH	   &&
					memchr("AaCcDdEeIiNnOoRrSsTtUuYyZz",let,26) ||
       language==LANG_ROMAN    && memchr("AaIiSsTt",let,8) ||	// Включая нижние акценты s,t 05.07.2002 E.P.
       language==LANG_HUNGAR   && memchr("AaEeIiOoUu",let,10) ||
	   language==LANG_SLOVENIAN	&& memchr("CcSsZz",let,6) ||

	   // 09.07.2001 E.P.
	   language==LANG_LATVIAN    && memchr("AaCcEeGgIiKkLlNnSsUuZz",let,22) ||
	   language==LANG_LITHUANIAN && memchr("AaCcEeIiSsUuZz",let,14) ||
	   language==LANG_ESTONIAN   && memchr("AaOoSsUuZz",let,10) ||

	   // 21.05.2002 E.P.
	   language==LANG_TURKISH   &&
		(memchr("AaCcIiGgOoSsUu",let,14) || let == i_sans_accent
		) ||
	   0
	   )
   {
   flacc=1;
   if (accent(c))
    break;
   }

  // Урезание чужих букв из общих таблиц
  if(
	  language==LANG_POLISH && memchr("qQ",let,2) ||

	  // BULGAR 08.09.2000 E.P.
	  language==LANG_RUSSIAN && langBul &&
		(
		let==155 || let==235 || // Ыы 08.09.2000 E.P.
		let==157 || let==237 || // Ээ 08.09.2000 E.P.
		let== r_EE_2dot || let== r_e_2dot
		) ||

	  language==LANG_SLOVENIAN && isnot_slovenian(let) ||

	  // 09.07.2001 E.P.
	  language==LANG_LATVIAN    && isnot_latvian(let) ||
	  language==LANG_LITHUANIAN && isnot_lithuanian(let) ||
	  language==LANG_ESTONIAN   && isnot_estonian(let) ||

	   // 21.05.2002 E.P.
	   language==LANG_TURKISH   && isnot_turkish(let) ||
	  0
	)
    {
    vers->prob>>=1;
    if( vers->prob&1 )
      vers->prob--;
    if( !vers->prob )
      vers->prob=2;
    }

  if( c->flg&(c_f_let|c_f_bad) ) // Oleg : 02-28-96 04:55pm : letter only
  if ( ((let=vers->let)=='i' || let=='j' ||
	  (language==LANG_TURKISH && let==II_dot_accent) // 17.06.2002 E.P.
	 )
		&&
		(r=ij_dot(c))!=0
	 )
    {	if (r>0)
			break;
		vers--;
		continue;
	}

  // Nick 18.06.2002
  if ( language == LANG_TURKISH && ((let=vers->let)== i_sans_accent || let == II_dot_accent ) &&
		(r = upper_right_angle( c )) > 0
	 )
  {
		vers->prob = MAX(2,vers->prob - r);
		continue;
  }

  if ((let=vers->let)=='!' || let=='?')
   {if (excl_dot(c)>0) break; vers--; continue;}
 if(language != LANG_RUSSIAN) // refuse with  ,Ў
  if ((let=vers->let)==invers_exm || let==invers_qm)
   {if (inv_dot(c)>0) break; vers--; continue;}

  if ( (
	   language==LANG_FRENCH ||
	   language==LANG_TURKISH // 18.06.2002 E.P.
	   ) && (vers->let=='c' || vers->let=='C'))
   c_to_ctail(c,0);  // AL 940321
  if (vers->let==liga_rt && not_rt(c))
   {vers--; continue;}
  if (/*vers->let==liga_ff*/is_liga_ff(vers->let) && not_ff(c))
   {vers--; continue;}

  if (vers->let=='n' || vers->let=='o')
   flon=1;
  }
 if (flon && tenv(c) && upper_right_line(c))
  {promote(0,c,'n',-48); promote(0,c,'o',-16);}
 /*Andrew*/
 sort_vers(c);
 c->vers[c->nvers].let = 0; /* Set correct tail */
 c->vers[c->nvers].prob= 0;
 if( comp_versions(c->vers,save,c->nvers,snvers) )
     {
     c->recsource |= c_rs_deskr;
     c->history   |= c_rs_deskr;
     }
 }

static void test_rq(cell *c)
 {

 if (c->row>bl.bm && c->row+c->h>bl.b3)
  set_bad_cell(c);
 }

static void test_c(cell *c)
 {

 if (tenv(c) && !long_line_c(c))
  {
  set_bad_cell(c);
  if (db_status)
   snap_newcell(c);
  if (snap_activity('d'))
   {
   snap_show_text("c-->bad");
   snap_monitor();
   }
  }
 }

static int16_t long_line_c(cell *c)
 {
 lnhead *line;
 int16_t l;
 interval *i;

 for (line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
			(l=line->lth)>0; line=(lnhead *)((pchar)line+l))
  if (line->flg&l_fbeg && line->flg&l_fend)
   {
   if (line->h<c->h/2) return 0;
   i=(interval *)((pchar)line+sizeof(lnhead))+(line->h-4);
   if (i->e-i->l>c->w/2) return 0;
   return 1;
   }
 return 0;
 }


static void crit_oa(cell *c)
 {
 int16_t r;

 if (tenv(c) && (r=short_lines(c))>0)
  {
  if (r==3)
   {
   if (c->vers[0].let=='o')
    {
    c->vers[0].let='a';
    c->vers[0].prob=c->vers[1].prob;
    c->nvers=1;
    c->vers[1].let=0;
    if (db_status)
     snap_newcell(c);
    if (snap_activity('d'))
     {
     snap_show_text("o-->a");
     snap_monitor();
     }
    }
   else
    {
    c->nvers=1;
    c->vers[1].let=0;
    if (db_status)
     snap_newcell(c);
    if (snap_activity('d'))
     {
     snap_show_text("delite o");
     snap_monitor();
     }
    }
   }
  else
   if (!(c->cg_flag&c_cg_cutr))
    {
    promote(0,c,'o',-32);
    if (db_status)
     snap_newcell(c);
    if (snap_activity('d'))
     {
     snap_show_text("penalty o-32");
     snap_monitor();
     }
    }
  }
 }

static int16_t short_lines(cell *c)
 {
 lnhead *line;
 interval *intval;
 int16_t l,row,col,h,w,a;
 uchar flg;

 for (a=0,line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
		       (l=line->lth)>0; line=(lnhead *)((pchar)line+l)    )
  if ((h=line->h)<=2)
   {
   flg=line->flg;
   row=line->row;
   intval=(interval *)((pchar)line+sizeof(lnhead));
   if (h==2 && flg&l_fend) {intval++; row++;}
   w=intval->l;
   col=intval->e-w;
   if (3*(c->w)/4<=col+w/2 && w>=c->h/8)
    {
    if (flg&l_fbeg && 4*row<c->h) a|=1;
    if (flg&l_fend && 4*(c->h-row)<c->h) a|=2;
    }
   }
 return a;
 }

/*
static int16_t n_or_ri(cell *c)
 {
 int16_t r;
 version *v1,*v2;

 if (!pitchsize && dot_ri(c))
  {
  c->vers[0].let=liga_ri;
  c->vers[1].let=0;
  if (c->vers[0].prob < 240)   // penalized ?
   c->vers[0].prob = 174;      // ensure cut process
  c->nvers=1;
  if (db_status)
   snap_newcell(c);
  if (snap_activity('d'))
   {
   snap_show_text("n->ri");
   snap_monitor();
   }
  return 1;
  }
 for (r=c->nvers=0,v1=v2=c->vers; v1->let; v1++)
  if (v1->let!=liga_ri)
   {
   v2->let=v1->let; v2->prob=v1->prob;
   v2++; (c->nvers)++;
   }
  else r=-1;
 v2->let=0;
 if (!c->nvers) set_bad_cell(c);
 return r;
 }
*/

cell *dot_ri(cell *c)
 {
 cell *cc;
 int16_t e;

 get_b_lines(c,&bl);
 for (e=c->col+c->w,cc=c->prevl->next; cc->col<=e; cc=cc->next)
  if (cc->flg&(c_f_dust+c_f_punct) &&
      (fax1x2 || cc->h>=2) && cc->w>=2 &&
      abs(cc->h-cc->w)<=H/6+((fax1x2)?2:0) &&
      (!fax1x2 || cc->h<=cc->w) &&
      cc->col+cc->w-(((c->font|c->font_new)&c_fp_it)?c->w/3:5)<=c->col+c->w &&
      (cc->col+cc->w/2<=c->col+c->w || (c->font|c->font_new)&c_fp_it ||
      c->save_stick_inc>300 && c->pos_inc==erect_rest) &&
      cc->row+4>=MIN(bl.b0,bl.b1) && cc->row+cc->h>=bl.b1 &&
      cc->row+cc->h-2<=bl.b2 &&
      cc->col+cc->w/2>=c->col+(2*c->w)/3)
    return cc;
 return NULL;
 }

/*
static void n_or_u(cell *c,version *vers)
 {
 uchar l1,l2;
 version *v1,*v2;
 uchar str[5];

 if (tenv(c) && (l1=left_line(c))!=0)
  {
  l2=(l1=='n')?'u':'n';
  vers->let=l1;
  for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
   if (v1->let!=l2)
    {
    v2->let=v1->let; v2->prob=v1->prob;
    v2++; (c->nvers)++;
    }
  v2->let=0;
  if (db_status)
   snap_newcell(c);
  if (snap_activity('d'))
   {
   sprintf(str,"%c-->%c",l2,l1);
   snap_show_text(str);
   snap_monitor();
   }
  }
 }

static uchar left_line(cell *c)
 {
 lnhead *line;
 int16_t l,H,h,b1,b2,e1,e2;
 interval *i1,*i2;

 for (H=c->h,line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
			  (l=line->lth)>0; line=(lnhead *)((pchar)line+l) )
  if (line->flg&l_fend && (h=line->h)>=3*H/4 && line->row+h+2>=H)
   {
   i1=(interval *)((pchar)line+sizeof(lnhead))+(h-2);
   if ((b1=(e1=i1->e)-i1->l)<(c->w)/2) break;
   }
 if (l)
  {
  i2=i1-(h/2-2);
  e2=i2->e; b2=e2-i2->l;
  if (e1>e2+2 && b1>b2) return 'u';
  if (abs(b1-b2)<=1 && abs(e1-e2)<=1 && abs(e1-b1-e2+b2)<=1 ||
      e1>e2+1 && b1<b2-1)
   return 'n';
  }
 return 0;
 }
*/

int16_t ij_test(cell *c)
 {
 version *v1,*v2;
 cell cc;
 int16_t r,n;

 for (v1=c->vers; v1->let && v1->let!='i' && v1->let!='j'; v1++) ;
 if (!(v1->let))
  return 1;
 get_b_lines(c,&bl);
 H=bl.ps;
 if (language==LANG_FRENCH||language==LANG_CROATIAN ||
	 language==LANG_CZECH || language==LANG_HUNGAR  || // 05.09.2000 E.P.
	 language==LANG_LATVIAN	||	// 16.07.2001 E.P.
	 language==LANG_TURKISH // 31.05.2002 E.P.
	 )
  memcpy(&cc,c,sizeof(cell));
 if ((r=ij_dot(c))>=0)
  return r;
 if (language==LANG_FRENCH||language==LANG_CROATIAN ||
	 language==LANG_CZECH || language==LANG_HUNGAR  || // 05.09.2000 E.P.
	 language==LANG_LATVIAN	||	// 16.07.2001 E.P.
	 language==LANG_TURKISH // 31.05.2002 E.P.
	 )
  {
  memcpy(c,&cc,sizeof(cell));
  for (v1=c->vers; v1->let && v1->let!='i'; v1++) ;
  if (v1->let)
   r=type_acc(c,TRUE);
  for (n=0,v1=v2=c->vers; v1->let; v1++)
   if (v1->let!='j' && (r || v1->let!='i'))
    {
    v2->let=v1->let;
    v2->prob=v1->prob;
    v2++;
    n++;
    }
  v2->let=0;
  c->nvers=n;
  if (!n)
   set_bad_cell(c);
  if (r)
   {
   if (db_status)
    snap_newcell(c);
   if (snap_activity('d'))
    {
    snap_show_text("reset i");
    snap_monitor();
    }
   }
  }
 return 1;
 }

#define TRPROB 50
// OLEG : SERBIAN PASS4
/*static*/ int16_t ij_dot(cell *c)
 {
 version *v,*v1,*v2;
 int16_t d;
 cell *cc=0;	// 17.06.2002 E.P.

 cc=dot_ij(c);

/* очень плохо для i_sans_accent - tur001.tif и т.д. Nick 20.06.2002
	// Пока нет лиги для II_dot_accent с прилипшей точкой.
	// 18.06.2002 E.P.
	if( is_turkish_language(language) &&
		check_letter(c, II_dot_accent)
	  )
		return 0;
*/

   if( language == LANG_RUSSIAN && langUkr )
     if( c->w >= c->h - 2 )
       goto ret0;

  if( cc &&
         (
		 is_cen_language(language) || // 05.09.2000 E.P. Было POLISH, CROATIAN
		 is_baltic_language(language) ||// 16.07.2001 E.P.
		 is_turkish_language(language) // 30.05.2002 E.P.
		 )
    )
    {
    uchar saveN, saveV[VERS_IN_CELL*sizeof(version)], rc, let;

    saveN = (uchar)c->nvers;
    memcpy(saveV,c->vers,VERS_IN_CELL*sizeof(version));
    c->vers[0].let='i'; c->vers[0].prob=254;
    c->vers[1].let=c->vers[1].prob=0;
    stick_center_study(c,NULL,1);
    rc = c->vers[0].prob;
    c->nvers = saveN ;
    memcpy(c->vers,saveV,VERS_IN_CELL*sizeof(version));

    if( c->stick_inc>100 || rc<200 )
       goto ret0;

	let = c->vers[0].let;
    if( is_cen_language(language) &&
		memchr("SsZzCc",let,6)&&c->w*8>c->h*5
	  )
       goto ret0;

	// На всякий случай. 16.07.2001 E.P.
    if( is_baltic_language(language) &&
		memchr("Eeg",let,3)&&c->w*8>c->h*5
	  )
       goto ret0;

    }

 if (cc)
  {
  if (2*cc->row+cc->h>2*bl.b2)
   goto ret0;
  for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
   if (v1->let=='i' || v1->let=='j' ||
	   language==LANG_TURKISH && // 31.05.2002 E.P.
		v1->let==II_dot_accent
	   )
    {
    v2->let=v1->let;
    v2->prob=MIN(254,v1->prob+84);
    v2++; (c->nvers)++;
    }
  v2->let=0;
  if (db_status)
   snap_newcell(c);

  if (snap_activity('d'))
   {
   snap_show_text("confirm ij");
   snap_monitor();
   }

  d=c->row-(cc->row+cc->h);
  if (d<=1) dist_point_of_i_1++;
  if (d==2) dist_point_of_i_2++;
  if (d==3) dist_point_of_i_3++;
  if (d>=4) dist_point_of_i_b++;

  return 1;
  }
 cc=c->prevl;
 if (cc->row<=c->row-4 && cc->col+cc->w>=c->col+(c->w)/2 &&
     cc->col+cc->w/2<c->col+c->w/2)
  {
  if (!cc->nvers || cc->vers[0].prob<TRPROB)
   goto ret0;
  for (v=cc->vers; v->let; v++)
   if (!strchr("/l()J1It[]",v->let) && v->let!=liga_i &&
	   !(language==LANG_TURKISH && // 30.05.2002 E.P.
			(v->let==i_sans_accent||v->let==II_dot_accent)
		) &&
	   v->prob>=TRPROB
	  )
     goto ret0;
  }
 cc=c->nextl;
 if (cc->row<=c->row-4 && cc->col<=c->col+(c->w)/2 && cc->vers[0].let!='f')
  {
ret0:
  return 0;
  }

 // if no dot
 for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
 {
	if( language==LANG_TURKISH && // 31.05.2002 E.P.
		 v1->let==II_dot_accent &&
		 !upper_dot_I(	c )      // Nick 23.06.2002
		)
		continue;

	if ( v1->let!='i' && v1->let!='j' )
	{
		v2->let=v1->let;
		v2->prob=v1->prob;
		v2++;
		(c->nvers)++;
	}
 }

 if( v1 == v2 )  // remove nothing  // Nick 23.06.2002
	 return 0;

 v2->let=0;
 if (!c->nvers)
  set_bad_cell(c);
 if (db_status)
  snap_newcell(c);
 if (snap_activity('d'))
  {
  snap_show_text("delete ij");
  snap_monitor();
  }
 return -1;
 }

extern strucI workI;
cell *dot_iUkr(cell *c)
 {
 cell *cc,*csv;
 int16_t e,H,rw;

 workI.rd = NULL;
 workI.ld = NULL;
 csv=NULL;
 get_b_lines(c,&bl); H=bl.ps;
 for (e=c->col+2*c->w,cc=c->prevl->next; cc->col<=e; cc=cc->next)
  {
  if(     cc->flg&(c_f_dust+c_f_punct) &&  // dust
          2*cc->h<c->h &&                  // not too high
          cc->row+cc->h-2<=bl.b2 &&        // upper position
          (cc->row+4>=MIN(bl.b0,bl.b1) ||  // not too upper
           cc->row>=bl.b1-bl.ps/3)
/*        cc->row+cc->h<=c->row
*/
    )
   {
   if (c->w>4)                         // wide cell
    {
    if (cc->cg_flag&c_cg_just && cc->h>=2 && cc->w>=2 ||
	!(cc->cg_flag&c_cg_just) && cc->h+cc->w >=3)
     if(    8*cc->w>=c->w && cc->h>2 &&
/*
            (abs(cc->h-cc->w)<=H/5 ||
             cc->h<cc->w && cc->w-(cc->h+((fax1x2)?2:0))<=H/4 ||
             cc->h<cc->w && 3*(c->row-cc->row)>=c->h && abs(cc->w-c->w)<=1) &&
            cc->col+3>=c->col &&
            c->col+c->w/2-(cc->col+cc->w/2)<=MAX(2,c->w/4) &&
            cc->col+cc->w-6<=c->col+c->w &&
            (memchr("ij",c->prevl->vers[0].let,2)==NULL ||
                         c->prevl->col+c->prevl->w<cc->col) &&
*/
            (rw=abs(c->col+c->w/2-(cc->col+cc->w/2))) <= c->w &&
            (
              c->nextl->h < c->h/2 ||
              abs(c->col+c->w/2-(cc->col+cc->w/2)) <=
              abs(c->nextl->col+c->nextl->w/2-(cc->col+cc->w/2))
            ) &&
            (
              memchr("iI",c->prevl->vers[0].let,2)==NULL ||
              abs(c->col+c->w/2-(cc->col+cc->w/2)) <=
              abs(c->prevl->col+c->prevl->w/2-(cc->col+cc->w/2))
            )
       )
      {
      csv=cc;
      workI.ld = workI.rd;
      workI.lw = workI.rw;
      workI.lh = workI.rh;
      workI.rd = cc;
      workI.rw = (uchar)rw;
      workI.rh = c->row - (cc->row+cc->h/2);
      if (cc->col+cc->w/2>c->col+c->w/2)
       break;
      }
    }
   else                                //   c->w<=4    // narrow cell
    if( (rw=abs(c->col+c->w/2-(cc->col+cc->w/2))) <= 3*c->w/2 )
     {
      csv=cc;
      workI.ld = workI.rd;
      workI.lw = workI.rw;
      workI.lh = workI.rh;
      workI.rd = cc;
      workI.rw = (uchar)rw;
      workI.rh = c->row - (cc->row+cc->h/2);
     if (cc->col+cc->w/2>c->col+c->w/2)
      break;
     }
   }
  }
if( workI.rd && workI.ld == NULL ){
   rw = abs(c->col+c->w/2-(csv->col+csv->w/2));
   if( rw >= 2*csv->w )
     return  NULL;
}
 return csv;
 }


cell *dot_ij(cell *c)
 {
 cell *cc,*csv;
 int16_t e,d,H;
 uchar let = 0;

   if( language == LANG_RUSSIAN && langUkr )
     return  dot_iUkr(c);

 csv=NULL;
 get_b_lines(c,&bl); H=bl.ps;
 for (e=c->col+c->w,cc=c->prevl->next; cc->col<=e && cc->next; cc=cc->next)   //AK ad c/g  06.03.98
  {
  if (cc->flg&(c_f_dust+c_f_punct) &&  // dust
      2*cc->h<c->h &&                  // not too high
      cc->row+cc->h-2<=bl.b2 &&        // upper position
      (cc->row+4>=MIN(bl.b0,bl.b1) ||  // not too upper
       cc->row>=bl.b1-bl.ps/3) &&
      cc->row+cc->h<=c->row)
   {
   if (c->w>4)                         // wide cell
    {
    if (cc->cg_flag&c_cg_just && cc->h>=2 && cc->w>=2 ||
	!(cc->cg_flag&c_cg_just) && cc->h+cc->w >=3)
     if (8*cc->w>=c->w &&
	 (abs(cc->h-cc->w)<=H/5 ||
	  cc->h<cc->w && cc->w-(cc->h+((fax1x2)?2:0))<=H/4 ||
	  cc->h<cc->w && 3*(c->row-cc->row)>=c->h && abs(cc->w-c->w)<=1) &&
	 cc->col+3>=c->col &&
	 c->col+c->w/2-(cc->col+cc->w/2)<=MAX(2,c->w/4) &&
	 cc->col+cc->w-6<=c->col+c->w &&
     (memchr("ij",( let = c->prevl->vers[0].let),2)==NULL &&
		// Исправил || на "&& !" 08.09.2000 E.P.
      !( language == LANG_POLISH &&
			( let == ZZ_dot_accent || let == z_dot_accent ) ||

		 // 16.07.2001 E.P.
		 language == LANG_LITHUANIAN &&
			( let == EE_dot_accent || let == e_dot_accent ) ||

		 // 30.05.2002 E.P.
		 language == LANG_TURKISH &&
			let == II_dot_accent
	   ) ||
           c->prevl->col+c->prevl->w < cc->col) )
      {
      csv=cc;
      if (cc->col+cc->w/2 > c->col+c->w/2)
       break;
      }
    }
   else                                //   c->w<=4    // narrow cell
    if ((d=c->col-cc->col+(c->w-cc->w)/2)<=2 && d>=-3)
     {
     csv=cc;
     if (cc->col+cc->w/2>c->col+c->w/2)
      break;
     }
   }
  }
 return csv;
 }

static int16_t excl_dot(cell *c)
 {
 version *v1,*v2;

 if (dot_excl(c))
  {
  for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
   if (v1->let=='!' || v1->let=='?')
    {
    v2->let=v1->let; v2->prob=v1->prob;
    v2++; (c->nvers)++;
    }
  v2->let=0;
  if (db_status)
   snap_newcell(c);
  if (snap_activity('d'))
   {
   snap_show_text("confirm !?");
   snap_monitor();
   }
  return 1;
  }
 for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
  if (v1->let!='!' && v1->let!='?')
   {
   v2->let=v1->let; v2->prob=v1->prob;
   v2++; (c->nvers)++;
   }
 v2->let=0;
 if (!c->nvers)
  set_bad_cell(c);
 if (db_status)
  snap_newcell(c);
 if (snap_activity('d'))
  {
  snap_show_text("delete !?");
  snap_monitor();
  }
 return -1;
 }

cell *dot_excl(cell *c)
 {
 cell * cc;
 int16_t e,H;


 for (e=c->col+c->w,cc=c->prevl->next; cc->col<=e&&cc!=cell_l(); cc=cc->next)
 {
  get_b_lines(cc,&bl); H=bl.ps;
  if (cc->flg&(c_f_dust+c_f_punct) && 2*cc->h<H &&
      ((fax1x2 || cc->h>=2) && cc->w>=2 || c->w<=3) &&
      cc->h-cc->w<=H/6 && cc->w-cc->h-((fax1x2)?2:1)<=H/6 &&
      (cc->col+3>=c->col || cc->col+5>=c->col && cc->w>3) &&
      cc->col+cc->w-3<=c->col+c->w &&
      cc->col<=c->col+c->w/2 &&
      cc->row<=bl.b3 && cc->row+cc->h+2>=bl.b3 &&
      cc->row>=c->row+c->h &&
      cc->row-(c->row+c->h)<=H/2)
   return cc;
 }
 return NULL;
 }

static int16_t inv_dot(cell *c)
 {
 version *v1,*v2;

 if (dot_inv(c))
  {
  for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
   if (v1->let==invers_exm || v1->let==invers_qm)
    {
    v2->let=v1->let; v2->prob=v1->prob;
    v2++; (c->nvers)++;
    }
  v2->let=0;
  if (db_status)
   snap_newcell(c);
  if (snap_activity('d'))
   {
   snap_show_text("confirm inv");
   snap_monitor();
   }
  return 1;
  }
 for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
  if (v1->let!=invers_exm && v1->let!=invers_qm)
   {
   v2->let=v1->let; v2->prob=v1->prob;
   v2++; (c->nvers)++;
   }
 v2->let=0;
 if (!c->nvers)
  set_bad_cell(c);
 if (db_status)
  snap_newcell(c);
 if (snap_activity('d'))
  {
  snap_show_text("delete inv");
  snap_monitor();
  }
 return -1;
 }

cell *dot_inv(cell *c)
 {
 cell * cc;
 int16_t e,H;

 get_b_lines(c,&bl); H=bl.ps;
 for (e=c->col+c->w,cc=c->prevl->next; cc->col<=e; cc=cc->next)
  if (cc->flg&(c_f_dust+c_f_punct))
   if (2*cc->h<H)
    if ((fax1x2 || cc->h>=2) && cc->w>=2 || c->w<=3)
     if (cc->h-cc->w<=H/6 && cc->w-cc->h-((fax1x2)?2:1)<=H/6)
      if (cc->col+3>=c->col && cc->col+cc->w-6<=c->col+c->w)
       if (cc->row+MAX(2,cc->h/3)>=bl.b2)
	if (cc->row+cc->h<=bl.bm)
	 if (cc->row+cc->h<c->row)
	  return cc;
 return NULL;
 }

void v_to_y(cell * c, uchar compose)
 {
 cell * cc;
 int16_t e;

 for (e=c->col+c->w,cc=c->prevl->next; cc->col<=e; cc=cc->next)
  if (cc->flg&(c_f_dust+c_f_punct) &&
      cc->col+5>=c->col && cc->col+cc->w-1<=c->col+c->w &&
      cc->row>=bl.b3-1 && cc->row+2<bl.b4 && cc->row+cc->h-3<=bl.b4 &&
      cc->h+cc->w>4 && 6*cc->h>=c->h)
   {
   c->vers[0].let='y';
   c->vers[1].let='v';
   c->vers[1].prob=c->vers[0].prob;
   c->vers[2].let=0;
   c->nvers=2;
   if (compose)
   {
     clist[0]=c;
     clist[1]=cc;
     compose_cell(2,clist,c);
   }
   if (db_status)
    snap_newcell(c);
   if (snap_activity('d'))
    {
    snap_show_text("v->y");
    snap_monitor();
    }
   break;
   }
 }

void c_to_ctail(cell * c, uchar compose)
 {
 cell *cc;
 int16_t e;
 version *v;
 uchar str[10];

 for (e=c->col+c->w,cc=c->next; cc->col<=e; cc=cc->next)
  if (cc->flg&(c_f_dust|c_f_punct) &&
      // AL cc->row>=c->row+c->h && cc->h>cc->w-2 && cc->h>=c->h/4 &&
      cc->row>=c->row+c->h && cc->h>=cc->w-2 && cc->h>=c->h/4 &&
      abs(c->col+c->w/2-(cc->col+cc->w/2))<=c->w/4)
   {
   if (compose)
   {
     clist[0]=c;
     clist[1]=cc;
     compose_cell(2,clist,c);
   }
   for (v=c->vers; v->let; v++)
    {
    if (v->let=='c')
     v->let=c_bottom_accent;
    if (v->let=='C')
     v->let=CC_bottom_accent;
    }
   if (db_status)
    snap_newcell(c);
   if (snap_activity('d'))
    {
    sprintf(str,"c->%c",c_bottom_accent);
    snap_show_text(str);
    snap_monitor();
    }
   return;
   }
 }

static int16_t not_rt(cell *c)
 {
 version *v1,*v2;

 if (tenv(c) && long_lines_rt(c))
  {
  for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
   if (v1->let!=liga_rt)
    {
    v2->let=v1->let; v2->prob=v1->prob;
    v2++; (c->nvers)++;
    }
  v2->let=0;
  if (!c->nvers)
   set_bad_cell(c);
  if (db_status)
   snap_newcell(c);
  if (snap_activity('d'))
   {
   snap_show_text("delete rt");
   snap_monitor();
   }
  return 1;
  }
 return 0;
 }

static int16_t long_lines_rt(cell *c)
 {
 lnhead *line,*ln1,*ln2;
 int16_t l,m1,m2;
 interval *i1,*i2;

 for (m1=m2=1000,
	  ln1=line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
			     (l=line->lth)>0; line=(lnhead *)((pchar)line+l))
  if (line->flg&l_fbeg && line->h>=2)
   {
   if (line->row<m1)
    {m2=m1; ln2=ln1; m1=line->row; ln1=line;}
   else
    if (line->row<m2) {m2=line->row; ln2=line;}
   }
 if (m2 != 1000)
  {
  i1=(interval *)((pchar)ln1+sizeof(lnhead));
  i2=(interval *)((pchar)ln2+sizeof(lnhead));
  if (i1->e<i2->e || m2<=2) return 1;
  }
 return 0;
 }

static int16_t not_ff(cell *c)
 {
 version *v1,*v2;

 if (tenv(c) && long_lines_ff(c))
  {
  for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
   //if (v1->let!=liga_ff)
  if (!is_liga_ff(v1->let))
    {
    v2->let=v1->let; v2->prob=v1->prob;
    v2++; (c->nvers)++;
    }
  v2->let=0;
  if (!c->nvers)
   set_bad_cell(c);
  if (db_status)
   snap_newcell(c);
  if (snap_activity('d'))
   {
   snap_show_text("delete ff");
   snap_monitor();
   }
  return 1;
  }
 return 0;
 }

static int16_t long_lines_ff(cell *c)
 {
 lnhead *line,*ln1,*ln2;
 int16_t l,m1,m2;

 for (m1=m2=1000,
	  ln1=line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
			    (l=line->lth)>0; line=(lnhead *)((pchar)line+l))
  if (line->flg&l_fbeg && line->h>=2)
   {
   if (line->row<m1)
    {m2=m1; ln2=ln1; m1=line->row; ln1=line;}
   else
    if (line->row<m2) {m2=line->row; ln2=line;}
   }
 if (m2 != 1000)
  {
   if (m2 > 2 && m2 < (c->h >> 1)) return 1;
  }
 return 0;
 }

/*
static int16_t not_1(cell *c)
 {
 version *v1,*v2;

 if ((c->cg_flag&c_cg_cutl) && tenv(c) && ((c->env)->nl==1) &&
	    !(((c->env)->stairs[3])&0x20))
  {
  if (c->vers[0].let=='1')
   {
   for (v1=c->vers; v1->let; v1++)
    if ((v1->prob-=70)<=0)
     v1->prob=2;
   }
  else
   {
   for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
    if (v1->let!='1')
     {
     v2->let=v1->let; v2->prob=v1->prob;
     v2++; (c->nvers)++;
     }
   v2->let=0;
   if (!c->nvers)
    set_bad_cell(c);
   return 1;
   }
  }
 return 0;
 }
*/

/*
static int16_t not_no(cell *c)
 {
 version *v1,*v2;

 if (tenv(c) && upper_right_line(c))
  {
  for (c->nvers=0,v1=v2=c->vers; v1->let; v1++)
   if (v1->let!='n' && v1->let!='o')
    {
    v2->let=v1->let; v2->prob=v1->prob;
    v2++; (c->nvers)++;
    }
  v2->let=0;
  if (!c->nvers) set_bad_cell(c);
  return 1;
  }
 return 0;
 }
*/

static int16_t upper_right_line(cell *c)
 {
 lnhead *line;
 interval *intval;
 int16_t l,e,max,min,i;

 for (e=0,line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
		       (l=line->lth)>0; line=(lnhead *)((pchar)line+l)    )
  if (line->h>=(c->h)/2)
   {
   intval=(interval *)((pchar)line+sizeof(lnhead));
   if (intval->e>e) e=intval->e;
   }
 for (line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
		       (l=line->lth)>0; line=(lnhead *)((pchar)line+l)    )
  if (line->h<=(c->h)/4 && line->flg&l_fbeg && line->row<=c->h/4)
   {
   intval=(interval *)((pchar)line+sizeof(lnhead));
   for (max=0,min=c->w,i=line->h; i; i--,intval++)
    {
    if (intval->e>max) max=intval->e;
    if (intval->e-intval->l<min) min=intval->e-intval->l;
    }
   if (max>e && min>=e-1)
    return 1;
   }
 return 0;
 }

uint32_t check_letter(cell *c, uchar let)
{
// Проверяет наличие версии. 18.06.2002 E.P.
version *p = c->vers;

while ( p < c->vers + c->nvers)
	{
	if( p->let == let )
		return 1;

	p++;
	}

return 0;
}
/////////////
// Nick 18.06.2002
#define MAX_UPR 32
static int16_t upper_right_angle(cell *c)
 {
 lnhead *line;
 interval *intval;
 int16_t l,i;
 int maxH = MIN(MAX_UPR,c->h/3);
 int nDuga = 0;
 int nBigLine = 0;
 int rBound[MAX_UPR];
 int lBound[MAX_UPR];
 int start;

	if( !c->env || maxH <= 1 )
		return 0;

	nDuga = 0;
	memset (rBound, 0, maxH*sizeof(int));
	for(i=0;i< maxH ;i++)
		lBound[i] = c->w;

	for ( line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
		       (l=line->lth)>0; line=(lnhead *)((pchar)line+l)  )
	{
		if( line->h * 4 >= c->h * 3 )
			nBigLine++;

		start = line->row;

		intval=(interval *)((pchar)line+sizeof(lnhead));
		for ( i = 0; i < line->h ; i++, intval++)
		{
			if( start +i  >= maxH )
				break;

			rBound[start +i] = MAX(rBound[start +i], intval->e );
			lBound[start +i] = MIN(lBound[start+i], intval->e-intval->l );
		}
	}

	if( nBigLine > 1 )
		return 0;

	if( rBound[0]*2 < c->w )
			nDuga++;

	for(i=1; i < maxH; i++ )
	{
			if ( ( rBound[i] > rBound[i-1] ||
				   ( rBound[i] == rBound[i-1] &&  rBound[i] * 2 < c->w )
				 )
				&& 	lBound[i] >= lBound[i-1]
               )
				nDuga++;
			else if( rBound[i] < rBound[i-1] )
			{
				nDuga--;
				break;
			}
	}

	if( nDuga <= 0 )
		return 0;

	if( nDuga * 3 >= maxH )
		return 100;

	return (300*nDuga)/MAX(1,maxH);
//	return 0;
}
/////////////
/////////////
// Nick 23.06.2002
static int16_t upper_dot_I(cell *c)
 {
 lnhead *line;
 interval *intval;
 int16_t l,i;
 int maxH = MIN(MAX_UPR,c->h/3);
 int nBigLine = 0;
 int rBound[MAX_UPR];
 int lBound[MAX_UPR];
 int start;

	if( !c->env || maxH <= 1 )
		return 0;

	memset (rBound, 0, maxH*sizeof(int));
	for(i=0;i< maxH ;i++)
		lBound[i] = c->w;

	for ( line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
		       (l=line->lth)>0; line=(lnhead *)((pchar)line+l)  )
	{
		if( line->h * 4 >= c->h * 3 )
			nBigLine++;

		start = line->row;

		intval=(interval *)((pchar)line+sizeof(lnhead));
		for ( i = 0; i < line->h ; i++, intval++)
		{
			if( start + i  >= maxH )
				break;

			rBound[start +i] = MAX(rBound[start+i], intval->e );
			lBound[start +i] = MIN(lBound[start+i], intval->e-intval->l );
		}
	}

	if( nBigLine > 1 )
		return 0;

	start = 0;   // wide part of upper dot
	for(i=1; i < maxH; i++ )
	{
			if( rBound[i] < rBound[i-1] || lBound[i] > lBound[i-1] )
			{
				start = i;
				break;
			}
	}

	if( start == 0 )
		return 0;

	for (i=start + 1; i < maxH ; i++ )
	{
		if( rBound[i] > rBound[i-1] || lBound[i] < lBound[i-1] )
			break;
	}

	if( i >= maxH )
		return 0;

	// too wide ?
	if( ( rBound[i-1] - lBound[i-1] ) * 2 > c->w )
		return 0;

	if( rBound[i-1] - lBound[i-1] >= rBound[start-1] - lBound[start-1] )
		return 0;

	return 1;
}
/////////////
