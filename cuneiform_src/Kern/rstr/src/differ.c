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
/*		      Differenciation of letters                     */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"

#include "compat_defs.h"

extern int16_t pitchsize;
extern uchar bcr_layout;
extern uchar language;

static void dif_DO(cell *);
static void dif_FP(cell *);
static void dif_n_ri(cell *);
static void dif_BE(cell *);
static void dif_vy(cell *);
static void dif_Sdol(cell *);
static void dif_cct(cell *);
static void dif_add(cell *);
static Bool compadd(cell *);
static void dif_sae(cell *);
static void dif_uv(cell *);
static void dif_O0(cell *);
static void dif_inv_roof(cell *c,uchar noRoof,uchar roof); // Nick 05.9.00
static void dif_f_t_inv_roof(cell *c1); // Nick 12.09.00
static void dif_f_Ii_right_accent(cell *c1,uchar Ii); // Nick 13.09.00
static void dif_j_i_bottom_accent(cell *c1 );
#define SMALKEG 18

void differ()
 {
 cell *c;

 for (c=cell_f()->nextl; c->nextl!=NULL; c=c->nextl)
  {
  dif_DO(c);
  dif_FP(c);
  dif_n_ri(c);
  dif_cct(c);
  dif_BE(c);
  dif_vy(c);
  dif_Sdol(c);
  dif_add(c);
  if (c->h<=SMALKEG)
   dif_sae(c);
  if( (c->font|c->font_new)&c_fp_str  )
   dif_uv(c);
  if (!pitchsize)
   dif_O0(c);

  dif_inv_roof(c,'t',t_inv_roof); // Nick 05.09.00
  dif_inv_roof(c,'d',d_inv_roof); // Nick 05.09.00

  dif_f_t_inv_roof(c); // Nick 13.09.2000
  dif_f_Ii_right_accent(c,i_right_accent); // Nick 13.09.2000
  dif_f_Ii_right_accent(c,II_right_accent); // Nick 13.09.2000

  dif_j_i_bottom_accent( c ); // Nick 3.09.2001

  }
 }

#define PROBMIN 140
#define DPROB    80

static void dif_DO(cell *c)
 {
 int16_t prob_D,prob_O,i,nstick,d,s1,s2;
 uchar let;
 STICK *st;

 for (prob_O=prob_D=0,i=c->nvers-1; i>=0; i--)
  if ((let=c->vers[i].let)=='D')
   prob_D=c->vers[i].prob;
  else
   if (let=='O' || let=='0' || let=='o' || let=='Q' || let=='G')
    prob_O=c->vers[i].prob;
 d=MAX(prob_O,prob_D);
 if (!prob_O || !prob_D ||
     MIN(prob_O,prob_D)<PROBMIN && (d!=c->vers[0].prob ||
					   abs(prob_O-prob_D)>DPROB))
  return;
 prob_O=d-prob_O;
 prob_D=d-prob_D;

 nstick=sticks_in_letter(c,0,&st);

 if (nstick<=0 || nstick>2)
  return;
 d=c->h-st->l; s1=st->bot+st->top;
 if (d<=1 && s1<=3 || d<=MAX(3,c->h/8) && s1<=1)
  {
  if (nstick==1)
   goto rec_D;
  d=st->l-(st+1)->l;
  s2=s1+(st+1)->bot+(st+1)->top;
  if (7*d>=2*c->h || d>=4 && s1<2 && s2<0 || d>=3 && s2<-2)
   goto rec_D;
  }
 if (c->h>=25 && d<=2 && c->vers[0].let=='D')
  {
  if (nstick==1)
   goto rec_D;
  d=st->l-(st+1)->l;
  if (3*d>=c->h)
   goto rec_D;
  }
 if (nstick==1)
  return;
 d=st->l-(st+1)->l;
 s2=st->bot+st->top+(st+1)->top+(st+1)->bot;
 if (d>MAX(5,c->h/4) || d>MAX(4,c->h/6) && s2<-1 || s2<-3)
  return;

 for (i=0; i<c->nvers; i++)
  if ((let=c->vers[i].let)=='D')
   c->vers[i].prob=0;
  else
   if (let=='O' || let=='0' || let=='o' || let=='Q' || let=='G')
    c->vers[i].prob+=prob_O;
 sort_vers(c);
 return;

rec_D:
 for (i=0; i<c->nvers; i++)
  if ((let=c->vers[i].let)=='D')
   c->vers[i].prob+=prob_D;
  else
   if (let=='O' || let=='0' || let=='o' || let=='Q' || let=='G')
    c->vers[i].prob=0;
 sort_vers(c);
 return;
 }

static void dif_FP(cell *c)
 {
 int16_t prob_F,prob_P,i,nstick,d;
 uchar let;
 STICK *st;

 for (prob_F=prob_P=0,i=c->nvers-1; i>=0; i--)
  if ((let=c->vers[i].let)=='F')
   prob_F=c->vers[i].prob;
  else
   if (let=='P')
    prob_P=c->vers[i].prob;
 d=MAX(prob_F,prob_P);
 if (!prob_F || !prob_P ||
     MIN(prob_F,prob_P)<PROBMIN && (d!=c->vers[0].prob ||
					      abs(prob_F-prob_P)>DPROB))
  return;
 prob_F=d-prob_F;
 prob_P=d-prob_P;

 nstick=sticks_in_letter(c,0,&st);

 if (nstick!=2)
  return;
 if ((st+1)->bot+(st+1)->top<=-2) goto rec_P;
 if ((st+1)->bot<0) return;

 for (i=0; i<c->nvers; i++)
  if ((let=c->vers[i].let)=='P')
   c->vers[i].prob=0;
  else
   if (let=='F')
    c->vers[i].prob+=prob_F;
 sort_vers(c);
 return;

rec_P:
 for (i=0; i<c->nvers; i++)
  if ((let=c->vers[i].let)=='P')
   c->vers[i].prob+=prob_P;
  else
   if (let=='F')
    c->vers[i].prob=0;
 sort_vers(c);
 return;
 }

static void dif_n_ri(cell *c)
 {
 cell *cc;
 puchar r;
 int16_t i,j1,j2;

 if (c->vers[0].let=='n' && !pitchsize && (cc=dot_ri(c))!=NULL)
  {
  if( ((c->font|c->font_new)&c_fp_str) ||
       c->save_stick_inc<300 && c->pos_inc==erect_rest )
   {
   r=save_raster(c);
   i=(c->h/2)*((c->w+7)/8);
   for (j1=0; !(r[i+j1/8]&(128>>(j1%8))); j1++) ;
   for ( ; j1<c->w && r[i+j1/8]&(128>>(j1%8)); j1++) ;
   for ( ; j1<c->w && !(r[i+j1/8]&(128>>(j1%8))); j1++) ;
   if (j1==c->w)
    return;
   for (j2=j1+1 ; j2<c->w && r[i+j2/8]&(128>>(j2%8)); j2++) ;
   if (abs(cc->col+cc->w/2-(c->col+(j1+j2)/2))>=2)
    return;
   }
  c->vers[0].let=liga_ri;
  c->vers[1].let=0;
  c->nvers=1;
  }
 }

#define SMAXE 1
#define SMINB 5

static void dif_BE(cell *c)
 {
 uchar let1,let2;
 puchar raster;
 int16_t l,s,i,j,j0;

 if (c->nvers!=2)
  return;
 let1=c->vers[0].let; let2=c->vers[1].let;
 if (!(let1=='B' && let2=='E' || let1=='E' && let2=='B'))
  return;
 raster=save_raster(c);
 l=(c->w+7)/8;
 for (s=i=0; i<c->h/4; i++,j0=j)
  {
  for (j=c->w-1; !(raster[i*l+j/8]&(128>>(j%8))); j--) ;
  if (!i) continue;
  if (j>j0) s++;
  if (j<j0) s--;
  }
 for (i=c->h-1; i>c->h-1-c->h/4; i--,j0=j)
  {
  for (j=c->w-1; !(raster[i*l+j/8]&(128>>(j%8))); j--) ;
  if (i==c->h-1) continue;
  if (j>j0) s++;
  if (j<j0) s--;
  }
 if (s>SMAXE && s<SMINB)
  return;
 c->nvers=1;
 c->vers[0].let=((s>=SMINB)?'B':'E');
 c->vers[1].let=0;
 }

#define PROB_V_OK 170

static void dif_vy(cell *c)
 {

 if (c->vers[0].let=='v' && c->vers[0].prob>=PROB_V_OK)
  v_to_y(c,1);
 }

static void dif_cct(cell *c)
{
 if (
     ( language==LANG_FRENCH ||
	   language==LANG_TURKISH // 18.06.2002 E.P.
	 ) &&
     c->vers[0].prob>=PROB_V_OK &&
     (c->vers[0].let=='c' ||
      c->vers[0].let=='C' ||  // AL 940321
      c->vers[0].let==c_bottom_accent   ||
      c->vers[0].let==CC_bottom_accent)
    )
   c_to_ctail(c,1);  // AL 940321
}

static void dif_Sdol(cell *c)
 {
 puchar r;
 int16_t l,i,j,jmin,jmax,jm,max;
 int16_t hist[20];
 uchar b;

 if (c->vers[0].let!='S' || c->w-2*(c->w/3)>20)
  return;
 r=save_raster(c);
 l=(c->w+7)/8;
 jmin=c->w/3;
 jmax=c->w-1-c->w/3;
 for (j=jmin; j<=jmax; j++)
  {
  hist[j-jmin]=0;
  b=128>>(j%8);
  for (i=0; i<c->h; i++)
   if (r[i*l+j/8]&b)
    hist[j-jmin]++;
  }
 for (max=0,j=jmin; j<=jmax; j++)
  if (max<hist[j-jmin])
   {max=hist[j-jmin]; jm=j;}
 if (max<c->h &&
     (max==hist[0] || max==hist[jmax-jmin] ||
      max-(hist[0]+hist[jmax-jmin])/2<MAX(3, c->h/6)))
  return;
 for (i=1; i<c->nvers && c->vers[i].let!='$'; i++) ;
 if (i<c->nvers)
 c->vers[i].prob=c->vers[0].prob;
 else
  {
  c->nvers=MIN(c->nvers+1,VERS_IN_CELL-1);
  c->vers[c->nvers-1].let='$';
  c->vers[c->nvers-1].prob=c->vers[0].prob;
  c->vers[c->nvers].let=0;
  }
 sort_vers(c);
 }

#define PROBOK  210
#define DELPROB  70
#define PROBBAD 120

static void dif_add(cell *c)
 {
 version *v;
 puchar r;
 int16_t l,i,j,n1,n2;
 uchar b;

 if (bcr_layout)
  if (c->flg&c_f_bad || c->vers[0].prob<=120 ||
      memchr("CQ(",c->vers[0].let,3))
   if (c->nextl->col<c->col+c->w)
    if (compadd(c))
     return;
/* if (c->nvers<=1 || c->vers[0].prob>=PROBOK)
  return;*/
 for (v=c->vers; v->let!=0 && v->let!='@'; v++) ;
 if (v->let==0 || c->vers[0].prob-v->prob>=DELPROB)
  return;
 r=save_raster(c);
 l=(c->w+7)/8;
 i=(c->h/2)*l;
 for (n1=j=0; j<c->w; j++)
  if (r[i+j/8]&(128>>(j%8)) && (!j || !(r[i+(j-1)/8]&(128>>((j-1)%8)))))
   n1++;
 if (n1<3)
  {
  i=((2*c->h)/3)*l;
  for (n1=j=0; j<c->w; j++)
   if (r[i+j/8]&(128>>(j%8)) && (!j || !(r[i+(j-1)/8]&(128>>((j-1)%8)))))
    n1++;
  }
 j=c->w/2;
 b=128>>(j%8);
 j/=8;
 for (n2=i=0; i<c->h; i++)
  if (r[l*i+j]&b && (!i || !(r[l*(i-1)+j]&b)))
   n2++;
 if (n2<3)
  {
  j=c->w/3;
  b=128>>(j%8);
  j/=8;
  for (n2=i=0; i<c->h; i++)
   if (r[l*i+j]&b && (!i || !(r[l*(i-1)+j]&b)))
    n2++;
  }
 if (n1<3 || n1>5 || n2<3 || n2>5)
  {v->prob=0; goto ret;}
 if (!bcr_layout)
  return;
 v->let=c->vers[0].let;
 v->prob=c->vers[0].prob;
 c->vers[0].let='@';
ret:
 sort_vers(c);
 }

#define NLET    256
#define PRADDOK 31000

static Bool compadd(cell *c)
 {
 s_glue GL;
 servBOX *s;
 indBOX *h;
 elmBOX *elm;
 extern pchar tableBOX;
 cell *cc;
 int16_t i,n;
 uint16_t max,prob;

 for (i=0,cc=c->prevl->next;
	 !(cc->flg&(c_f_let|c_f_bad|c_f_fict)) || cc->col<c->col+c->w;
							    cc=cc->next)
  if (tenv(cc))
   {
   if (cc->col+cc->w<c->col)
    continue;
   if (i==MAX_CELLS_IN_LIST-1)
    break;
   GL.celist[i]=cc;
   GL.complist[i]=cc->env;
   i++;
   }
 n=i;
 GL.celist[i]=NULL;
 GL.complist[i]=NULL;
 if ((s=make_broken_raster(&GL,1))!=NULL)
  {
  h=(indBOX *)tableBOX+'@';
  for (max=0,i=h->numel; i; i=elm->list)
   {
   //elm=(elmBOX *)((tableBOX+NLET*sizeof(indBOX))+i);
   elm=(elmBOX *)(tableBOX+NLET*sizeof(indBOX))+i;
   prob=(*scalarf)(elm->vect,s->matrBOX.vect,s->matrBOX.isq_rt);
   if (max<prob)
    max=prob;
   }
  if (max>PRADDOK)
   {
   c->vers[0].let='@';
   c->vers[0].prob=200;
   c->vers[1].let=0;
   c->nvers=1;
   compose_cell(n,GL.celist,c);
   return TRUE;
   }
  }
 return FALSE;
 }

#define DELPRSAE 70

static void dif_sae(cell *c)
 {
 version *v;
 puchar r;
 int16_t l,i,i1,i2,imin,j,max,min;

 if (!memchr("ae",c->vers[0].let,2))
  return;
 for (v=c->vers; v->let && v->let!='s'; v++) ;
 if (!v->let || c->vers[0].prob-v->prob>DELPRSAE)
  return;
 r=save_raster(c);
 l=(c->w+7)/8;
 i1=c->h/4;
 i2=c->h-1-c->h/3;
 for (max=0,min=c->w,i=i1; i<=i2; i++)
  {
  for (j=c->w-1; !(r[i*l+j/8]&(128>>(j%8))); j--) ;
  if (j>max)
   max=j;
  if (j<min)
   {
   min=j;
   imin=i;
   }
  }
 if (max==min || imin>c->h/2)
  return;
 for (i=c->h-3; i<c->h; i++)
  {
  for (j=c->w-1; !(r[i*l+j/8]&(128>>(j%8))); j--) ;
  if (j>max)
   return;
  }
 v->let=c->vers[0].let;
 v->prob=c->vers[0].prob;
 c->vers[0].let='s';
 sort_vers(c);
 }

static void dif_uv(cell *c)
 {
 uchar l1,l2;
 puchar r;
 int16_t l,i,j1,j2,d1,d2;

 l1=c->vers[0].let;
 l2=c->vers[1].let;
 if ((l1=='v' && l2=='u' || l1=='V' && l2=='U') &&
     c->vers[0].prob-c->vers[1].prob<=DPROB)
  {
  r=save_raster(c);
  l=(c->w+7)/8;
  i=(c->h-MAX(2,(c->h)/6))*l;
  for (j1=0; !(r[i+j1/8]&(128>>(j1%8))); j1++) ;
  for (j2=c->w-1; !(r[i+j2/8]&(128>>(j2%8))); j2--) ;
  d1=j2-j1+1;
  i-=l;
  for (j1=0; !(r[i+j1/8]&(128>>(j1%8))); j1++) ;
  for (j2=c->w-1; !(r[i+j2/8]&(128>>(j2%8))); j2--) ;
  d2=j2-j1+1;
  if (3*(d1+d2)>=4*c->w)
   {
   c->vers[0].let=l2;
   c->vers[1].let=l1;
   }
  }
 }

#define DELTA_O0 10

static void dif_O0(cell *c)
 {
 version *v;
 uchar probO,prob0;

 for (probO=prob0=0,v=c->vers; v->let; v++)
  {
  if (v->let=='O')
   probO=v->prob;
  if (v->let=='0')
   prob0=v->prob;
  }
 if (!probO || !prob0 || (probO=prob0=MAX(probO,prob0))<=DELTA_O0)
  return;
 if (5*c->h<=6*c->w)
  {prob0-=DELTA_O0; goto exit;}
 if (2*c->h>=3*c->w)
  {probO-=DELTA_O0; goto exit;}
 return;
exit:
 for (v=c->vers; v->let; v++)
  {
  if (v->let=='O')
   v->prob=probO;
  if (v->let=='0')
   v->prob=prob0;
  if (v->let=='Q' && v->prob<=prob0 && v->prob>probO)
   v->prob=MAX(0,v->prob-DELTA_O0);
  }
 sort_vers(c);
 }
//**************************************************************************
// Nick 05.09.00
// diskriminator d - d'    t - t'  for czech lang
static void dif_inv_roof(cell *c, uchar letNoRoof, uchar letRoof) // Nick 05.9.00
{
 int16_t prob_t,prob_troof,i;
 uchar let;
 int16_t nIntersect,hei3;
 extern int16_t NumIntersect2(c_comp *cmp, int mHei);
 uchar goodLet,badLet;

 for (prob_t=prob_troof=0,i=c->nvers-1; i>=0; i--)
 {
  if ((let=c->vers[i].let)==letNoRoof)
   prob_t=c->vers[i].prob;
  else  if (let==letRoof)
    prob_troof=c->vers[i].prob;
 }

 if (!prob_t || !prob_troof ||
     MIN(prob_t,prob_troof)<PROBMIN &&
	  ( MAX(prob_t,prob_troof)!=c->vers[0].prob ||
		abs(prob_t-prob_troof)>DPROB
	  )
	)
  return;

 hei3 = c->h/3;
 if( hei3 <= 0 )
	 return;

 nIntersect=NumIntersect2(c->env, hei3);
 if( nIntersect < 0 )
	 return;

 nIntersect = (nIntersect*100) / hei3;

 if( nIntersect < 15 )
 {
	 goodLet = letNoRoof;
	 badLet  = letRoof;
 }
 else if( nIntersect > 30 )
 {
	 goodLet = letRoof;
	 badLet  = letNoRoof;
 }
 else
	 return;  // unknown

 for (i=0; i<c->nvers; i++)
 {
  if ((let=c->vers[i].let)==badLet)
   c->vers[i].prob=0;
  else  if (let==goodLet)
    c->vers[i].prob = MAX(prob_t,prob_troof);
 }

 sort_vers(c);
 return;
}
//**************************************************************************
// Nick 12.09.2000
// return < 0 - error
// else return standard width - now very simple
//
static int GetBounds(cell *c1,int *lBound,int *rBound,int *wid)
{
 lnhead   *line;
 int      len;
 interval *vint;
 c_comp   *cmp = c1->env;
 int      numRow;
 int      standWid;
 int      i,crow;

  if( !cmp )
     return -1;

  line=(lnhead *)((pchar)cmp+cmp->lines+sizeof(int16_t));
// test all lines - fill bounds
  for (numRow = standWid = 0; (len=line->lth)>0; line=(lnhead *)((pchar)line+len))
  {
  // current line
   for (vint=(interval *)((pchar)line+sizeof(lnhead)),
	   crow = line->row,i=0; i < line->h; i++, crow++, vint++ )
   {
	  if( crow < 0 || crow >= c1->h )
		  return -2;  // invalid c_comp

	  lBound[crow] = MIN(lBound[crow], vint->e - vint->l);
	  rBound[crow] = MAX(rBound[crow], vint->e);
	  wid[crow] += vint->l;
	  standWid  += vint->l;
	  numRow++;
   }
  }

  if( numRow <= 3 )
	  return -3;

  standWid /= numRow;

  return standWid;
}
//**************************************************************************
// Nick 12.09.00
// diskriminator f - t'  for czech lang
static void dif_f_t_inv_roof(cell *c1)
{
 int16_t      prob_f,prob_troof;
 int      i,crow;     // current row
 int      lBound[RASTER_MAX_HEIGHT+1];
 int      rBound[RASTER_MAX_HEIGHT+1];
 int      wid[RASTER_MAX_HEIGHT+1];
 int      standWid;   // standard width
 int      numRow;     // all intervals
 int      lastUp;
 int      wideStart,isStep;
 uchar     let;
 uchar    goodLet,badLet;

  if( language != LANG_CZECH )
 	 return;

  for (prob_f=prob_troof=0,i=c1->nvers-1; i>=0; i--)
  {
   if ((let=c1->vers[i].let)=='f')
    prob_f=c1->vers[i].prob;
   else  if (let==t_inv_roof)
    prob_troof=c1->vers[i].prob;
  }

  if( !prob_f || !prob_troof ||
      MIN(prob_f,prob_troof)<PROBMIN &&
	  ( MAX(prob_f,prob_troof)!=c1->vers[0].prob ||
		abs(prob_f-prob_troof)>DPROB
	  )
	)
  return;

  if( c1->h > RASTER_MAX_HEIGHT || c1->h < 5)
	 return;

  for(i=0; i < c1->h ; i++)
     lBound[i] = c1->w;

  memset(rBound,0,c1->h*sizeof(int));
  memset(wid,0,c1->h*sizeof(int));

  standWid = GetBounds( c1, lBound, rBound, wid);
  if( standWid <= 0 )
	  return;

  standWid = MAX(1,(standWid*3)>>2);

  lastUp = c1->h/4; // test !?

  for(isStep = wideStart = 0, crow = 0, numRow=0 ; crow < lastUp; crow++ )
  {
    if( !wid[crow] )
		continue;  // empty row

    if( wid[crow]  < rBound[crow] - lBound[crow] )
	{
		if( !wideStart )
		  numRow++;
	}
	else
		if( !wideStart && wid[crow] >= 2*standWid ) wideStart=crow+1;

    if( !isStep &&
		lBound[crow+1] > standWid &&
		wid[crow] == rBound[crow] - lBound[crow] &&
		lBound[crow] > lBound[crow+1]  + standWid
	  )
	  isStep = crow+1;
  }

  if( isStep  || numRow*2 > lastUp   )
  {
	 badLet   = 'f';
	 goodLet  = t_inv_roof;
  }
  else if( wideStart*2 < lastUp )
  {
	 badLet   = t_inv_roof;
	 goodLet  = 'f';
  }
  else
	 return;  // unknown

  for (i=0; i<c1->nvers; i++)
  {
   if ((let=c1->vers[i].let)==badLet)
    c1->vers[i].prob=0;
   else  if (let==goodLet)
    c1->vers[i].prob = MAX(prob_f,prob_troof);
  }

  sort_vers(c1);

  return;
}
//**************************************************************************
// Nick 13.09.00
// diskriminator f - (I,i) with right accent  for czech lang
static void dif_f_Ii_right_accent(cell *c1,uchar Ii)
{
 int16_t      prob_f,prob_troof;
 int      i,crow;     // current row
 int      lBound[RASTER_MAX_HEIGHT+1];
 int      rBound[RASTER_MAX_HEIGHT+1];
 int      wid[RASTER_MAX_HEIGHT+1];
 int      standWid;   // standard width
 int      numRow;     // all intervals
 int      lastUp;
 int      wideStart,numStep,wideEnd;
 uchar     let;
 uchar    goodLet,badLet;
 int      maxWidth;

  if( language != LANG_CZECH && language != LANG_HUNGAR)
 	 return;

  for (prob_f=prob_troof=0,i=c1->nvers-1; i>=0; i--)
  {
   if ((let=c1->vers[i].let)=='f')
    prob_f=c1->vers[i].prob;
   else  if (let==Ii)
    prob_troof=c1->vers[i].prob;
  }

  if( !prob_f || !prob_troof ||
      MIN(prob_f,prob_troof)<PROBMIN &&
	  ( MAX(prob_f,prob_troof)!=c1->vers[0].prob ||
		abs(prob_f-prob_troof)>DPROB
	  )
	)
  return;

  if( c1->h > RASTER_MAX_HEIGHT || c1->h < 5)
	 return;

  for(i=0; i < c1->h ; i++)
     lBound[i] = c1->w;

  memset(rBound,0,c1->h*sizeof(int));
  memset(wid,0,c1->h*sizeof(int));

  standWid = GetBounds( c1, lBound, rBound, wid);
  if( standWid <= 0 )
	  return;

  maxWidth = standWid;

  lastUp = c1->h - (c1->h/5);

  for( numStep = wideStart = wideEnd =0, crow = 0, numRow=0 ; crow < c1->h ; crow++ )
  {
    if( !wid[crow] )
		continue;  // empty row


    if( wid[crow]  < rBound[crow] - lBound[crow] )
	{
		if( crow*3 < c1->h )
		    numRow++;
	}
	else
	{
		maxWidth = MAX(maxWidth, wid[crow] );

		if(  wid[crow] >= 2*standWid )
		{
		 if( !wideStart && crow*3 < c1->h  )
			wideStart=crow+1;
		 else if ( !wideEnd && crow > lastUp  )
			wideEnd = crow+1;
		}
	}

	if( crow*3 < c1->h &&  lBound[crow] > lBound[crow+1] )
		numStep++;

  }

  // many double lines ? or line is in middle - no at end
  if( numRow*6 > c1->h  || wideStart && !wideEnd ||
     !is_baltic_language(language) && 	// 17.07.2001 E.P.
	  Ii == i_right_accent && wideStart )
  {
	 goodLet   = 'f';
	 badLet  = Ii;
  }
  else if( numStep*6 > c1->h || maxWidth*3 <= standWid*4 )
  {
	 goodLet   = Ii;
	 badLet  = 'f';
  }
  else
	 return;  // unknown

  for (i=0; i<c1->nvers; i++)
  {
   if ((let=c1->vers[i].let)==badLet)
    c1->vers[i].prob=0;
   else  if (let==goodLet)
    c1->vers[i].prob = MAX(prob_f,prob_troof);
  }

  sort_vers(c1);

  return;
}
//**************************************************************************
// Nick 3.09.01
// diskriminator j - i_bottom_accent  for lithuanian
static void dif_j_i_bottom_accent(cell *c1 )
{
 int16_t      prob_j, prob_i;

 int      i;
 int      lBound[RASTER_MAX_HEIGHT+1];
 int      rBound[RASTER_MAX_HEIGHT+1];
 int      wid[RASTER_MAX_HEIGHT+1];
 int      standWid;   // standard width
 int      lastUp,lastDn;
 uchar     let;
 uchar    goodLet,badLet;
 int32_t    isLeft, isRight;
 int32_t    leftMin, rightMax;
 int32_t    leftJump, rightJump;
 int32_t    leftPerekladina;

  if( language != LANG_LITHUANIAN )
 	 return;

  if( c1->nvers <= 0 )
	  return;

  if( c1->vers[0].let != 'j' &&
	  c1->vers[0].let != i_bottom_accent
	  )
	  return;

  if( c1->h > RASTER_MAX_HEIGHT || c1->h < 5)
	 return;

  for (prob_j=prob_i=0,i=c1->nvers-1; i>=0; i--)
  {
   if ((let=c1->vers[i].let)=='j')
    prob_j=c1->vers[i].prob;
   else  if (let==i_bottom_accent)
    prob_i=c1->vers[i].prob;
  }


  for(i=0; i < c1->h ; i++)
     lBound[i] = c1->w;

  memset(rBound,0,c1->h*sizeof(int));
  memset(wid,0,c1->h*sizeof(int));

  standWid = GetBounds( c1, lBound, rBound, wid);
  if( standWid <= 0 )
	  return;

  lastUp = MAX(3, c1->h/2);
  lastDn = c1->h - (c1->h/6);

  isLeft = 0;
  isRight = 0;

  leftMin = c1->w;
  rightMax=0;

  leftJump = rightJump = 0;
  leftPerekladina = 0;

  for(i=c1->h-1; i > lastUp;i--)
  {
	  if( !wid[i] )
		  continue;

	  rightJump = MAX(rightJump, rBound[i] - rBound[i-1]);
	  leftJump  = MAX(leftJump, lBound[i-1] - lBound[i]);

	  // горизонтальная выступающая палка
	  if( i < lastDn &&
		  wid[i] >= rBound[i]-lBound[i] && wid[i] <= standWid )
	  {
		  if( wid[i-1] >= 2*standWid &&
			  lBound[i-1] + (standWid>>1) < lBound[i] &&
			  wid[i-1] >= rBound[i-1] - lBound[i-1]
			   )
	        leftPerekladina = MAX(leftPerekladina, wid[i-1]);

		  if( wid[i-2] >= 2*standWid &&
			  lBound[i-2] + (standWid>>1) < lBound[i] &&
			  wid[i-2] >= rBound[i-2] - lBound[i-2]
			  )
	        leftPerekladina = MAX(leftPerekladina, wid[i-2]);
	  }


	  isRight = MAX(isRight, rightMax-rBound[i]);

	  if (rBound[i] > rightMax )
		  rightMax = rBound[i];

	  isLeft = MAX(isLeft, lBound[i]-leftMin);

	  if( lBound[i] < leftMin )
		  leftMin = lBound[i];
  }


  if( rightJump >= standWid ||
	  leftPerekladina >= 2*standWid && leftJump < standWid  ||
	  isRight >= standWid && isLeft < standWid
	)
  {
	 goodLet   = i_bottom_accent;
	 badLet  = 'j';
  }
  else if( leftJump > standWid && rightJump <= (standWid>>1)||
	       isLeft >= standWid && isRight < standWid
		  )
  {
	 goodLet   = 'j';
	 badLet  = i_bottom_accent;
  }
  else
	 return;  // unknown

  // need add version ?
  if( goodLet == 'j' && prob_j <= 0 ||
	  goodLet == i_bottom_accent && prob_i <= 0
	)
  {
   c1->nvers=MIN(c1->nvers+1,VERS_IN_CELL-1);
   c1->vers[c1->nvers-1].let=goodLet;
   c1->vers[c1->nvers-1].prob=MAX(prob_j,prob_i);
   c1->vers[c1->nvers].let=0;
   c1->vers[c1->nvers].prob=0;
  }

  for (i=0; i<c1->nvers; i++)
  {
   if ( c1->vers[i].let == goodLet)
    c1->vers[i].prob = MAX(prob_j,prob_i);

   else if ( c1->vers[i].let ==badLet )
    c1->vers[i].prob = MAX(2,c1->vers[i].prob - 20 );
  }

  sort_vers(c1);

  return;
}
//**************************************************************************
