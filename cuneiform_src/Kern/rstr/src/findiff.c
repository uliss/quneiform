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
/*	       Final differenciation of some letters	             */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"

#include "compat_defs.h"

extern uchar language;

static B_LINES bl;
static cell* clist[2];

static void final_vy(cell *);
static void final_eg(cell *);
static void final_ao_gpq(cell *);
static void final_OQ(cell *);
static void final_dotcom(cell *);
static int16_t final_slash_l(cell *);
static Bool is_slash(cell *);
static int16_t final_no_slash(cell *);
static int16_t final_back_slash(cell *);
static Bool is_back_slash(cell *);
static void final_bh(cell *);
static void final_ii_u(cell *);
static void final_AOU_2dot(cell *);
static void final_A_circle(cell *);
static void final_vv_w(cell *);
static void final_d_chck(cell *);
static void final_Il_1(cell *);
static void final_7_T(cell *);
static void delspace();

extern int16_t final_crit_russian(cell * c);

#define PROBD 160
#define PROBB 130
#define PROBL 100

void final_crit()
 {
 cell *c;
 uchar let,let1;
 int16_t cnt;

 for (cnt=0,c=cell_f()->next; c->next!=NULL; c=c->next)
  {
  if (c->flg&c_f_space) continue;
  let=c->vers[0].let;
  if(final_crit_russian(c)) cnt++;
  if (c->font&c_fp_undrln)
   {
   if (let=='v')
    final_vy(c);
   if (let=='e' || let=='u')
    final_eg(c);
   if (let=='o' || let=='a' && c->nvers>=2 && c->vers[1].let=='o')
    final_ao_gpq(c);
   if (let=='O')
    final_OQ(c);
   if (let=='.')
    final_dotcom(c);
   }
  if (let=='/')
   cnt+=final_slash_l(c);
  else
   {
   if ((c->font|c->font_new)&c_fp_it && memchr("Il1",let,3))
    cnt+=final_no_slash(c);
   if (c->flg&c_f_bad || c->vers[0].prob<PROBL)
    cnt+=final_back_slash(c);
   }
  if ((let=='c' || let=='C') && c->vers[1].let=='<')
   {
   c->nvers=1;
   c->vers[1].let=0;
   }
  if ((let=='b' || let=='h') && (c->font|c->font_new)&c_fp_it)
   final_bh(c);
  if ( (language==LANG_GERMAN   ||
	    language==LANG_SPANISH  ||
		language==LANG_FRENCH	||
		language==LANG_ESTONIAN	||	// 19.07.2001 E.P.
		language==LANG_TURKISH		// 30.05.2002 E.P.
	   ) &&
      ((c->font|c->font_new)&c_fp_it || let=='i' ||
		let==liga_i ||
		 language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let==i_sans_accent||let==II_dot_accent)
	  ) &&
      (let=='i' || let==liga_i ||
		 language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let==i_sans_accent||let==II_dot_accent) ||
	   let==liga_fi || let==liga_ffi
	  ) &&
      ( (let1=c->nextl->vers[0].let)=='i' ||
		let1==liga_i ||
		 language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let1==i_sans_accent||let1==II_dot_accent)
	  ) &&
       (c->cg_flag&c_cg_cutr && c->nextl->cg_flag&c_cg_cutl ||
	    ((c->font|c->font_new)&c_fp_str) && c->nextl->col-(c->col+c->w)<=1
	   )
	  )
   final_ii_u(c);
  if (language==LANG_GERMAN && memchr("0AOU",let,4) ||
      language==LANG_SWEDISH && memchr("0AO",let,3) ||
      language==LANG_SPANISH && let=='U' ||
	  language==LANG_ESTONIAN && memchr("0AOU",let,4)	// 19.07.2001 E.P.
	 )
   {
   get_b_lines(c,&bl);
   if (bl.b1>c->row)
    final_AOU_2dot(c);
   }
  if (language==LANG_SWEDISH && c->vers[0].let=='A')
   {
   get_b_lines(c,&bl);
   if (bl.b1>c->row+MAX(4,bl.ps/4))
    final_A_circle(c);
   }
  if (let=='-' && 3*c->h<c->w &&
      (c->prev->flg&(c_f_fict|c_f_space) && c->prev->vers[0].let!=0x1e ||
       memchr(".,:;",c->prev->vers[0].let,4)) &&
      (c->next->flg&(c_f_fict|c_f_space) && c->next->vers[0].let!=0x1e ||
       memchr(".,:;",c->next->vers[0].let,4)))
   c->vers[0].let='_';
  if (let=='F' && c->cg_flag&c_cg_cutr &&
      memchr(".,",c->next->vers[0].let,2) && c->next->cg_flag&c_cg_cutl &&
      c->col+c->w>c->next->col)
   {
   clist[0]=c;
   clist[1]=c->next;
   compose_cell(2,clist,c);
   c->vers[0].let='E';
   c->left=c->col;
   c->right=c->col+c->w;
   }
  if (memchr("Vv",let,2) && c->cg_flag&c_cg_cutr &&
      c->next->vers[0].let==let && c->next->cg_flag&c_cg_cutl)
   final_vv_w(c);
  if (let=='d' && c->vers[0].prob>=PROBD &&
      memchr("cistx",c->next->vers[0].let,5) &&
      c->next->vers[0].prob<=PROBB &&
      c->next->col-(c->col+c->w)<=1)
   final_d_chck(c);
  if (let==0x27 && c->cg_flag&c_cg_cutr &&
      memchr("Il",c->next->vers[0].let,2) && c->next->cg_flag&c_cg_cutl)
   {
     c=c->next;  // AL 940321
     final_Il_1(c);
     cnt++;
   }
  if (let=='7' && c->cg_flag&c_cg_cutr &&
      c->next->vers[0].let==0x27 && c->next->cg_flag&c_cg_cutl)
   {final_7_T(c); cnt++;}
  }
 if (cnt)
  {
  delspace();
  space_size((int16_t)get_size());
  space_cell();
  context_proc_line();
  cont_space();
  }
 }

#define DVY 10

static void final_vy(cell *c)
 {

 if (c->vers[0].prob<=DVY) return;
 get_b_lines(c,&bl);
 if (c->row+c->h-bl.b3>=2)
  {c->vers[0].let='y'; c->vers[1].let='v';}
 else c->vers[1].let='y';
 c->vers[1].prob=c->vers[0].prob-DVY;
 c->vers[2].let=0;
 c->nvers=2;
 }

#define DEG 30

static void final_eg(cell *c)
 {

 if (c->vers[0].prob<=DEG) return;
 if (c->nvers==VERS_IN_CELL-1) c->nvers--;
 c->vers[c->nvers].let='g';
 c->vers[c->nvers].prob=c->vers[0].prob-DEG;
 c->nvers++;
 c->vers[c->nvers].let=0;
 sort_vers(c);
 }

#define DAO 20

static void final_ao_gpq(cell *c)
 {
 uchar let;
 int16_t nvers,i;

 if (c->vers[0].prob<=DAO) return;
 get_b_lines(c,&bl);
 if (c->row+c->h-bl.b3>=2)
  {
  if ((let=c->vers[0].let)=='a')
   {
   c->vers[2].let='a'; c->vers[2].prob=c->vers[0].prob-DAO; nvers=3;
   if (c->nvers>=2 && c->vers[1].let=='o' && c->vers[1].prob>DAO)
    {c->vers[3].let='o'; c->vers[3].prob=c->vers[1].prob-DAO; nvers++;}
   c->vers[0].let='g'; c->vers[1].let='q';
   c->vers[1].prob=c->vers[0].prob;
   }
  else
   {
   nvers=3; i=1;
   if (c->nvers>=2 && c->vers[1].let=='o')
    {
    nvers=4;
    if (c->vers[1].prob>=c->vers[0].prob-DAO) i=2;
    else {c->vers[3].let=c->vers[1].let; c->vers[3].prob=c->vers[1].prob;}
    }
   c->vers[i].let='g'; c->vers[i+1].let='q';
   c->vers[i].prob=c->vers[i+1].prob=c->vers[0].prob-DAO;
   }
  }
 else
  {
  if (c->row+c->h-bl.b3>=2)
   {
   c->vers[3].let='o'; c->vers[3].prob=c->vers[0].prob-DAO; nvers=4;
   if (c->nvers>=2 && c->vers[1].let=='a' && c->vers[1].prob>DAO)
    {c->vers[4].let='a'; c->vers[4].prob=c->vers[1].prob-DAO; nvers++;}
   c->vers[0].let='g'; c->vers[1].let='p'; c->vers[2].let='q';
   c->vers[1].prob=c->vers[2].prob=c->vers[0].prob;
   }
  else
   {
   nvers=4; i=1;
   if (c->nvers>=2 && c->vers[1].let=='a')
    {
    nvers=5;
    if (c->vers[1].prob>=c->vers[0].prob-DAO) i=2;
    else {c->vers[4].let=c->vers[1].let; c->vers[4].prob=c->vers[1].prob;}
    }
   c->vers[i].let='g'; c->vers[i+1].let='p'; c->vers[i+2].let='q';
   c->vers[i].prob=c->vers[i+1].prob=c->vers[i+2].prob=c->vers[0].prob-DAO;
   }
  }
 c->vers[nvers].let=0; c->nvers=nvers;
 }

#define DOQ 30

static void final_OQ(cell *c)
 {

 if (c->vers[0].prob<=DOQ) return;
 if (c->nvers==VERS_IN_CELL-1) c->nvers--;
 c->vers[c->nvers].let='Q';
 c->vers[c->nvers].prob=c->vers[0].prob-DOQ;
 c->nvers++;
 c->vers[c->nvers].let=0;
 sort_vers(c);
 }

static void final_dotcom(cell *c)
 {

 get_b_lines(c,&bl);
 if (c->row+c->h-bl.b3>=2)
  c->vers[0].let=',';
 }

static int16_t final_slash_l(cell *c)
 {
 int16_t i;
 uchar fnt;

 get_b_lines(c,&bl);
 if (c->row+c->h>=bl.b3+MIN(3,MAX(1,c->h/10)))
  return 0;
 if ((c->flg & (c_f_let|c_f_bad)) == 0)
   return 0;   // AL 940318
 if (c->flg & c_f_dust)
   return 0;   // AL 940318
 if ((fnt=c->prevl->font|c->nextl->font)&c_fp_ser && !(fnt&c_fp_gelv))
  return 0;
 c->nvers-=MAX(0,c->nvers-(VERS_IN_CELL-4));
 c->vers[c->nvers].let='l';
 c->vers[c->nvers+1].let='I';
 c->vers[c->nvers+2].let='1';
 c->vers[c->nvers].prob=c->vers[c->nvers+1].prob=c->vers[c->nvers+2].prob=
							    c->vers[0].prob;
 for (i=0; i<c->nvers; i++)
  if (memchr("lI1",c->vers[i].let,3))
   c->vers[i].prob=0;
 c->nvers+=3;
 c->vers[c->nvers].let=0;
 if (c->env!=NULL && !is_slash(c))
  c->vers[0].prob=MAX(2,c->vers[0].prob-50);
 sort_vers(c);
 return 1;
 }

#define NOMINCL     30
#define DENOMINCL   11

static Bool is_slash(cell *c)
 {
 puchar raster;
 int16_t i1,i2,j1,j2,l;
 int16_t    ginc=erect_get_local();
 if( c->stick_inc==NO_INCLINE || c->stick_inc==0 )
    stick_center_study(c,NULL,1);
if( nIncline<256 && c->stick_inc>700 && (c->flg&c_f_solid))
    return TRUE;
if( c->stick_inc!=NO_INCLINE && ginc<400 && ginc*2<c->stick_inc )
    return TRUE;
 raster=save_raster(c);
 l=(c->w+7)/8;
 i1=c->h/4;
 for (j1=0; !(raster[i1*l+j1/8]&(128>>(j1%8))); j1++) ;
 i2=3*c->h/4;
 for (j2=0; !(raster[i2*l+j2/8]&(128>>(j2%8))); j2++) ;
 if ((c->font|c->font_new)&c_fp_it && NOMINCL*(j1-j2)>=DENOMINCL*(i2-i1) ||
     (c->font|c->font_new)&c_fp_str && 2*NOMINCL*(j1-j2)>=DENOMINCL*(i2-i1))
  return TRUE;
 return FALSE;
 }

static int16_t final_no_slash(cell *c)
 {
 int16_t i;
 uchar fnt;

 if (c->env==NULL)
  return 0;
 for (i=1; i<c->nvers; i++)
  if (c->vers[i].let=='/') break;
 if (i==c->nvers)
  return 0;
 if ((fnt=c->prevl->font|c->nextl->font)&c_fp_ser && !(fnt&c_fp_gelv) &&
     !(c->font&c_fp_ser))
  {
  c->vers[i].let=c->vers[0].let;
  c->vers[0].let='/';
  return 1;
  }
 if (is_slash(c))
  return 0;
 c->vers[i].prob=0;
 sort_vers(c);
 return 1;
 }

static int16_t final_back_slash(cell *c)
 {

 get_b_lines(c,&bl);
 if (c->row>bl.b1+2 || c->row+c->h<bl.b3 ||
     c->env==NULL || c->env->nl!=1 || !is_back_slash(c))
  return 0;
 c->nvers=1;
 c->vers[0].let='\\';
 c->vers[0].prob=254;
 c->vers[1].let=0;
 return 1;
 }

static Bool is_back_slash(cell *c)
 {
 puchar raster;
 int16_t l,i1,i2,i3,j1,j2,j3,k1,k2,k3;

 raster=save_raster(c);
 l=(c->w+7)/8;
 i1=MAX(2,c->h/8);
 for (j1=0; !(raster[i1*l+j1/8]&(128>>(j1%8))); j1++) ;
 if (j1>i1)
  return FALSE;
 i3=c->h-1-i1;
 for (k3=c->w-1; !(raster[i3*l+k3/8]&(128>>(k3%8))); k3--) ;
 if (c->w-1-k3>i1 || abs(c->w-k3-j1)>2)
  return FALSE;
 for (k1=j1+1; k1<c->w && raster[i1*l+k1/8]&(128>>(k1%8)); k1++) ;
 k1--;
 for (j3=k3-1; j3 && raster[i3*l+j3/8]&(128>>(j3%8)); j3--) ;
 j3++;
 i2=c->h/2;
 for (j2=0; !(raster[i2*l+j2/8]&(128>>(j2%8))); j2++) ;
 for (k2=j2+1; k2<c->w && raster[i2*l+k2/8]&(128>>(k2%8)); k2++) ;
 k2--;
 if (abs(k1-j1-(k3-j3))>2 || abs((j1+j3)/2-j2)>2 || abs((k1+k3)/2-k2)>2)
  return FALSE;
 if ((c->font|c->font_new)&c_fp_it && 4*NOMINCL*(j3-j1)>=DENOMINCL*(i3-i1) ||
     (c->font|c->font_new)&c_fp_str && 2*NOMINCL*(j3-j1)>=DENOMINCL*(i3-i1))
  return TRUE;
 return FALSE;
 }

static void final_bh(cell *c)
 {
 int16_t i;

 for (i=1; i<c->nvers; i++)
  if (memchr("bh",c->vers[i].let,2))
   {
   c->vers[i].prob=MAX(c->vers[i].prob,c->vers[0].prob-10);
   sort_vers(c);
   return;
   }
 if (c->nvers==VERS_IN_CELL-1)
  c->nvers--;
 c->vers[c->nvers].let=(c->vers[0].let=='b')?'h':'b';
 c->vers[c->nvers].prob=c->vers[0].prob-10;
 c->nvers++;
 c->vers[c->nvers].let=0;
 sort_vers(c);
 }

static void final_ii_u(cell *c)
 {
 cell *c1;
 uchar lett = u_2dot_accent;

 c1=c->nextl;
 if (c->vers[0].let=='i' ||
	 language == LANG_TURKISH && ( c->vers[0].let==i_sans_accent || c->vers[0].let==II_dot_accent) // Nick 14.06.02
	)
  {
	if( language == LANG_TURKISH)
	{
		// don't glue
		if( c->vers[0].let==i_sans_accent && !(c1->cg_flag&c_cg_cutl) )
			return;

		if( c->vers[0].let==i_sans_accent && c1->vers[0].let==i_sans_accent) // Nick 14.06.02
			lett = 'u';  // if no points
	}

	clist[0]=c;
	clist[1]=c1;
	compose_cell(2,clist,c);
	c->vers[0].let = lett;       // u_2dot_accent;
	c->vers[0].prob=254;
	c->nvers=1;
	c->vers[1].let=0;
	c->right=c->col+c->w;
  }
 else
  {
  c->vers[0].let=(c->vers[0].let==liga_fi)?'f':liga_fi;
  c->vers[0].prob=254;
  c->nvers=1;
  c->vers[1].let=0;
  c1->vers[0].let=u_2dot_accent;
  c1->vers[0].prob=254;
  c1->nvers=1;
  c1->vers[1].let=0;
  c1->left-=5;
  }
 }

static void final_AOU_2dot(cell *c)
 {
 puchar r;
 int16_t l,i,i1,j,j1,j2,j3,j4;
 uchar let,b;

 r=save_raster(c);
 let=c->vers[0].let;
 l=(c->w+7)/8;
 i=MAX(1,(bl.b1-c->row)/2);
 for (j1=0; !(r[i*l+j1/8]&(128>>(j1%8))); j1++) ;
 for (j2=j1+1; j2<c->w && r[i*l+j2/8]&(128>>(j2%8)); j2++) ;
 if (j2==c->w || let!='A' && 2*(j2-1)>=c->w)
  return;
 for (j3=j2; j3<c->w && !(r[i*l+j3/8]&(128>>(j3%8))); j3++) ;
 if (j3==c->w || let!='A' && 2*j3<=c->w)
  return;
 for (j4=j3+1; j4<c->w && r[i*l+j4/8]&(128>>(j4%8)); j4++) ;
 if (abs(j2-j1-(j4-j3))>=MAX(3,bl.ps/6) ||
     j3-j2+1<(j2-j1+j4-j3-2)/4)
  return;
 j=(j2+j3)/2;
 b=128>>(j%8);
 j/=8;
 for (i1=i; i1>=0; i1--)
  if (r[i1*l+j]&b)
   return;
 if (memchr("0O",let,2))
  {
  for (i1=i; i1<c->h/2 && !(r[i1*l+j]&b); i1++) ;
  let=(i1==c->h/2)?'U':'O';
  }
 if (let!='A')
  {
  i=bl.bm-c->row;
  for (j2=0; !(r[i*l+j2/8]&(128>>(j2%8))); j2++) ;
  for (j3=c->w-1; !(r[i*l+j3/8]&(128>>(j3%8))); j3--) ;
  if (j3-j2+1-(j4-j1)<=MAX(2,bl.ps/8) || j4-j1+1<(j3-j2+1)/2)
   return;
  }
 c->vers[0].let=(let=='A')?AA_2dot_accent:
		((let=='O')?OO_2dot_accent:UU_2dot_accent);
 c->vers[0].prob=254;
 c->nvers=1;
 c->vers[1].let=0;
 }

static void final_A_circle(cell *c)
 {
 puchar r;
 int16_t l,i,im,j,j1,j2;

 r=save_raster(c);
 l=(c->w+7)/8;
 i=(bl.b1-c->row)/2;
 for (j1=0; !(r[i*l+j1/8]&(128>>(j1%8))); j1++) ;
 for (j2=c->w-1; !(r[i*l+j2/8]&(128>>(j2%8))); j2--) ;
 if (abs(bl.b1-c->row-(j2-j1+1))>bl.ps/8)
  return;
 j=(j1+j2)/2;
 if (r[i*l+j/8]&(128>>(j%8)))
  return;
 for (im=bl.b1-c->row+2; i<im && !(r[i*l+j/8]&(128>>(j%8))); i++) ;
 if (i==im)
  return;
 c->nvers=1;
 c->vers[0].let=AA_circle_accent;
 c->vers[0].prob=254;
 c->vers[1].let=0;
 }

static void final_vv_w(cell *c)
 {
 puchar r;
 int16_t l,i,j,n1,n2;
 uchar b;

 r=save_raster(c);
 l=(c->w+7)/8;
 j=c->w-1;
 b=128>>(j%8);
 j/=8;
 for (i=0; !(r[i*l+j]&b); i++) ;
 for (n1=0; i<c->h && r[i*l+j]&b; i++,n1++) ;
 if (n1<c->h/5)
  return;
 r=save_raster(c->next);
 l=(c->next->w+7)/8;
 for (i=0; !(r[i*l]&0x80); i++) ;
 for (n2=0; i<c->next->h && r[i*l]&0x80; i++,n2++) ;
 if (n2<c->next->h/5 || n1+n2<(c->h+c->next->h)/4)
  return;
 c->vers[0].let=(c->vers[0].let=='v')?'w':'W';
 clist[0]=c;
 clist[1]=c->next;
 compose_cell(2,clist,c);
 c->right=c->col+c->w;
 }

static void final_d_chck(cell *c)
 {

 c->vers[0].let='c';
 c->vers[1].let=0;
 c->nvers=1;
 if (memchr("itx",c->next->vers[0].let,3))
  {
  c->next->vers[0].let='h';
  c->next->vers[1].let='k';
  }
 else
  {
  c->next->vers[0].let='k';
  c->next->vers[1].let='h';
  }
 c->next->vers[0].prob=c->next->vers[1].prob=c->vers[0].prob;
 c->next->vers[2].let=0;
 c->next->nvers=2;
 }

#define NLET     256
#define PROBCMP1 31000

static void final_Il_1(cell *c)
 {
 s_glue GL;
 servBOX *s;
 indBOX *h;
 elmBOX *elm;
 extern pchar tableBOX;
 uint16_t max,prob;
 int16_t i;

 GL.celist[0]=c;
 GL.complist[0]=c->env;
 GL.celist[1]=c->prev;
 GL.complist[1]=c->prev->env;
 GL.celist[2]=NULL;
 GL.complist[2]=NULL;
 if ((s=make_broken_raster(&GL,1))!=NULL)
  {
  h=(indBOX *)tableBOX+'1';
  for (max=0,i=h->numel; i; i=elm->list)
   {
   //elm=(elmBOX *)((tableBOX+NLET*sizeof(indBOX))+i);
   elm=(elmBOX *)(tableBOX+NLET*sizeof(indBOX))+i;
   prob=(*scalarf)(elm->vect,s->matrBOX.vect,s->matrBOX.isq_rt);
   if (max<prob)
    max=prob;
   }
  if (max>=PROBCMP1)
   {
   c->vers[0].let='1';
   c->vers[1].let='I';
   c->vers[2].let='l';
   c->vers[1].prob=c->vers[2].prob=c->vers[0].prob;
   c->nvers=3;
   c->vers[3].let=0;
   // c->flg=c_f_let;
   compose_cell(2,GL.celist,c);
   c->left=c->col;
   c->right=c->col+c->w;
   }
  }
 }

static void final_7_T(cell *c)
 {

 clist[0]=c;
 clist[1]=c->next;
 c->vers[0].let='T';
 c->vers[1].let=0;
 c->nvers=1;
 compose_cell(2,clist,c);
 c->right=c->col+c->w;
 }

static void delspace()
 {
 cell *c;

 for (c=cell_f(); c!=NULL; c=c->next)
  if (c->flg==c_f_space)
   {
   c=c->prev;
   del_cell(c->next);
   }
 }
