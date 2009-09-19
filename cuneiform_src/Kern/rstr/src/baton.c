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
/*                    Finding sticks in letter                       */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "func.h"

#include "compat_defs.h"
#include "minmax.h"

#define VERTMAX  64*2
#define HORMAX  128
#define MASHINCL 2048
#define INCLMIN     0
#define INCLMAX  2048
#define DELINCL   200
#define INCL0    (-INCLMIN/DELINCL)
#define NINCL    (INCLMAX/DELINCL+INCL0+1)                           // 11
#define NINCLMAX 50
#define DELLEFT  ((VERTMAX/2*(-INCLMIN)+(MASHINCL/4))/(MASHINCL/2))  // 0
#define DELRIGHT ((VERTMAX/2*INCLMAX+(MASHINCL/4))/(MASHINCL/2))     // 17
#define HORSZMAX (2*HORMAX+1+DELLEFT+DELRIGHT)                       // 275
#define NVALMAX 1024
#define NSTICKMAX 30
#define NEXTRMAX 128

/* from O.S.  */
#define lr    HORMAX

extern int16_t bs_got;

static STICK cane[NSTICKMAX];
static int32_t hist[ /*** HORSZMAX*NINCL ***/  4000 ];    // 4000
//static uchar raster[VERTMAX*HORMAX/8];                  // 1024
static uchar raster[VERTMAX*HORMAX];                      //O.S.
static struct val {int32_t b,e,y,c;} val[NVALMAX];
static struct extr {uchar x,incl,h;} extr[NEXTRMAX];
static int32_t ps,H,W,/*lr,*/mdl,dl,dr,horsz,MD,nval,nextr,nstick;
struct pairs {uchar b,e;};

static int32_t line_to_hist(lnhead *);
static int32_t sticks_find();
static int32_t test_extr(int32_t,int32_t,int32_t);
static void ordextr();
static Bool is_stick(struct extr*);
static int32_t typend(STICK *,int32_t);
static int32_t bend(int32_t,struct pairs *);
static void ordsticks();

int32_t tab_incl[VERTMAX*NINCLMAX];
int32_t n_incl=NINCL;
int32_t d_incl=DELINCL;
int32_t b_incl=INCLMIN;

void set_tab_incl()
 {
 int32_t i,h;                                                        //20.01.97
 int32_t incl,d;

 for (i=0; i<n_incl; i++)
  {
  incl=(i-b_incl)*d_incl;
  for (h=0; h<VERTMAX; h++)
   {
   d=h*incl;
   d+=((d>0)?MASHINCL/4:-MASHINCL/4);
   tab_incl[h*n_incl+i]=d/(MASHINCL/2);
   }
  }
/*
 for (i=0; i<n_incl; i++)
  {
  printf("incl=%d\n",(i-b_incl)*d_incl);
  for (h=0; h<VERTMAX; h++)
   printf(" %d",tab_incl[h*n_incl+i]);
  scanf("%c",&h);
  }
 */
 }

int16_t sticks_in_letter(cell *c,int16_t mode,STICK **res)
// return value - number of sticks (if >= 0)
//                code of error    (if <  0)
//                  -1   base lines arn't yet counted
//                  -2   no envelop
//                  -3   too many intervals ( > 1024)
//                  -4   too many extremums ( > 128)
//                  -5   too many sticks    ( > 30)
 {
 c_comp *cmp;
 lnhead *line;
 int32_t l;                                                          //20.02.97
 int16_t    ret;

 if (!bs_got) return -1;
 if (c->flg&c_f_space || !tenv(c))
  return -2;
 if( c->n_baton>0 && c->n_baton<4 )
    {
    *res=c->save_baton;
    return c->n_baton;
    }
 ps=get_size();
 H=c->h; W=c->w; MD=mode; /* lr=(W+7)/8 */;
 mdl=H/2;
 dl=((int32_t)H/2*(-INCLMIN)+(MASHINCL/4))/(MASHINCL/2);
 dr=((int32_t)H/2*INCLMAX+(MASHINCL/4))/(MASHINCL/2);
 horsz=2*W+1+dl+dr;
 memset(hist,0,horsz*NINCL*sizeof(*hist));
 memset(raster,0,lr*H*sizeof(*raster));

 for (nval=0,cmp=c->env,line=(lnhead *)((pchar)cmp+cmp->lines+sizeof(int16_t));
                           (l=line->lth)>0; line=(lnhead *)((pchar)line+l))
  if (!line_to_hist(line)) return -3;

 *res=cane;
 ret=(int16_t)(sticks_find());
 if( ret>=0 && ret<4 )
    {
    c->n_baton = (uchar)ret;
    if( ret>0 )
        memcpy(c->save_baton,cane,ret*sizeof(STICK));
    }
 return ret;
 }

static int32_t line_to_hist(lnhead *line)
 {
 int32_t x,/* xm, */y,ym,i,h;                                        //20.02.97
 interval *v;
 PLONG ph/* ,pr */;                                               //20.02.97
 PLONG pt;

 for (v=(interval *)((pchar)line+sizeof(lnhead)),
                           ym=(y=H-1-line->row)-line->h;
                                                   y>ym; y--,v++)
  {
  //for (pr=raster+lr*y,xm=v->e,x=xm-v->l; x<xm; x++)
  // *(pr+x/8)|=(128>>(x&7));
  memset(raster+lr*y+v->e-v->l,1,v->l);
  if (3*v->l>=2*ps) continue;
  switch(MD)
   {
   case -1: x=2*(v->e-v->l); break;
   case  0: x=2*v->e-v->l-1; break;
   case  1: x=2*v->e-2;
   }
  if (nval==NVALMAX)
    return FALSE;
  val[nval].y=y; val[nval].b=v->e-v->l;
  val[nval].e=v->e-1; val[nval].c=0; nval++;
  h=(y-mdl)*n_incl;
  if (h>0)
    {
    for (pt=tab_incl+abs(h),ph=hist+x+dl,i=0; i<NINCL; i++,pt++,ph+=horsz)
      (*(ph-*pt))++;
    }
  else
    {
    for (pt=tab_incl+abs(h),ph=hist+x+dl,i=0; i<NINCL; i++,pt++,ph+=horsz)
      (*(ph+*pt))++;
    }

  /*
  for (pt=tab_incl+abs(h),ph=hist,i=0; i<NINCL; i++,pt++,ph+=horsz)
   if (h>0)
    (*(ph+x+dl-*pt))++;
   else
    (*(ph+x+dl+*pt))++;
  */
  }
 return TRUE;
 }

static int32_t sticks_find()
 {
 int32_t i,x,hh,max;                                                 //20.02.97
 PLONG ph;                                                        //20.02.97

/* for (i=0; i<NINCL; i++)
  {
  printf("incl=%d\n",(i-INCL0)*DELINCL);
  for (x=0; x<horsz; x++) printf(" %u",hist[i*horsz+x]); scanf("%c",&x);
  }*/

 max=ps/5;
 for (ph=hist,nextr=i=0; i<NINCL; i++,ph++)
  {
  if ((hh=*ph+*(ph+1)/2)>=max && !test_extr(i,0,hh)) return -4;
  for (ph++,x=1; x<horsz-1; x++,ph++)
   if ((hh=*ph+(*(ph-1)+*(ph+1))/2)>=max && !test_extr(i,x,hh)) return -4;
  if ((hh=*ph+*(ph-1)/2)>=max && !test_extr(i,horsz-1,hh)) return -4;
  }
 ordextr();

/*
 for (i=0; i<nextr; i++)
  printf("x=%d,incl=%d,h=%d\n",
         extr[i].x-dl,(extr[i].incl-INCL0)*DELINCL,extr[i].h);
 scanf("%c",&x);
*/

 for (nstick=i=0; i<nextr; i++)
  if (!is_stick(&extr[i])) return -5;
 ordsticks();

/*
 for (i=0; i<nstick; i++)
  printf("x=%d,y=%d,l=%d,incl=%d,w=%d,top=%d,bot=%d\n",
         cane[i].x,cane[i].y,cane[i].l,cane[i].incl,cane[i].w,
                                                 cane[i].top,cane[i].bot);
 scanf("%c",&x);
*/

 return nstick;
 }

static int32_t test_extr(int32_t i,int32_t x,int32_t hh)
 {
 PLONG ph,ph1;                                                    //20.02.97
 int32_t hh1;                                                        //20.02.97

 ph=hist+i*horsz+x;
 if (x>1 &&
       ((hh1=*(ph-1)+(*(ph-2)+*ph)/2)>hh || hh1==hh && *(ph-1)>=*ph) ||
     x<horsz-2 &&
       ((hh1=*(ph+1)+(*ph+*(ph+2))/2)>hh || hh1==hh && *(ph+1)>*ph))
  return TRUE;

 if (i)
  {
  ph1=ph-horsz;
  hh1=*ph1+(((x)?*(ph1-1):0)+((x<horsz-1)?*(ph1+1):0))/2;
  if (hh1>hh || hh1==hh && i>INCL0 ||
      x>1 && ((hh1=*(ph1-1)+(*(ph1-2)+*ph1)/2)>hh || hh1==hh && i>INCL0) ||
      x<horsz-2 && ((hh1=*(ph1+1)+(*ph1+*(ph1+2))/2)>hh ||
                                                   hh1==hh && i>INCL0))
   return TRUE;
  }

 if (i<NINCL-1)
  {
  ph1=ph+horsz;
  hh1=*ph1+(((x)?*(ph1-1):0)+((x<horsz-1)?*(ph1+1):0))/2;
  if (hh1>hh || hh1==hh && i<INCL0 ||
      x>1 && ((hh1=*(ph1-1)+(*(ph1-2)+*ph1)/2)>hh || hh1==hh && i<INCL0) ||
      x<horsz-2 && ((hh1=*(ph1+1)+(*ph1+*(ph1+2))/2)>hh ||
                                                   hh1==hh && i<INCL0))
   return TRUE;
  }

 if (nextr==NEXTRMAX) return FALSE;
 extr[nextr].x=(uchar)x; extr[nextr].incl=(uchar)i; extr[nextr].h=(uchar)hh; nextr++;

/* printf("x=%d,incl=%d,hist=%d\n",x-dl,(i-INCL)*DELINCL,hh);*/

 return TRUE;
 }

static void ordextr()
 {
 int32_t i1,i2;                                                      //20.02.97
 int32_t w;                                                          //20.02.97

 for (i1=1; i1<nextr; i1++)
  for (i2=i1; i2<nextr; i2++)
   if (extr[i1-1].h<extr[i2].h)
    {
    w=extr[i1-1].x; extr[i1-1].x=extr[i2].x; extr[i2].x=(uchar)w;
    w=extr[i1-1].incl; extr[i1-1].incl=extr[i2].incl; extr[i2].incl=(uchar)w;
    w=extr[i1-1].h; extr[i1-1].h=extr[i2].h; extr[i2].h=(uchar)w;
    }
 }

#define THINST  4
#define LLACUN  3

static Bool is_stick(struct extr *ex)
 {
 int32_t incl,incl1,i,n,x0,x,x1,y,y1,ymi,yma,ym,w,w1,m,f,h;          //20.02.97
 int32_t line[VERTMAX];                                              //20.02.97
 int32_t hw[HORMAX+1];                                               //20.02.97
 int32_t aval[VERTMAX];                                              //20.02.97

 memset(line,0,H*sizeof(*line));
 memset(hw,0,(W+1)*sizeof(*hw));
 incl=(ex->incl-INCL0)*DELINCL;
 x0=ex->x-dl;
 for (n=i=0; i<nval; i++)
  if (!(val[i].c))
   {
   y=val[i].y;
   h=(y-mdl)*n_incl;
   if (h>0)
    x=x0+*(tab_incl+h+ex->incl);
   else
    x=x0-*(tab_incl-h+ex->incl);
   if (abs((int32_t)(val[i].b+val[i].e)-x)<=2)                       //20.02.97
    {
    if( y>=0 ) // OLEG : cam crashing
        line[y]=1;
    hw[val[i].e-val[i].b+1]++;
    aval[n]=i;
        n++;
    }
   }
 if (4*n<ps) return TRUE;

 for (m=i=0; i<=W; i++)
  {
  if (hw[i]>m) {m=hw[i]; w=w1=i;}
  else
   if (hw[i]==m) w1=i;
  }
 w=(w+w1)/2;
 if (5*(hw[w]+(hw[w-1]+hw[w+1])/2)<ps) return TRUE;

 for (y=0; y<H; y++)
  if (!line[y])
   {
   h=(y-mdl)*n_incl;
   if (h>0)
    x=x0+*(tab_incl+h+ex->incl);
   else
    x=x0-*(tab_incl-h+ex->incl);
   x+=(x>0)?1:-1;
   x/=2;
   if (x>=0 && x<W  )
      line[y]=raster[lr*y+x];
   }

 for (ymi=0; !line[ymi]; ymi++) ;
 for (m=0,f=1,y=ymi; y<H; y++,m++)
  {
  if (f==line[y]) continue;
  if (f) {yma=y-1; m=1; f=0; continue;}
  if (w>THINST || m>=LLACUN) break;
  f=1;
  }
 if (f) yma=H-1;
 if (13*(yma-ymi+1)>=8*ps) goto ok;

 for (yma=H-1; !line[yma]; yma--) ;
 for (m=0,f=1,y=yma; y>=0; y--,m++)
  {
  if (f==line[y]) continue;
  if (f) {ymi=y+1; m=1; f=0; continue;}
  if (w>THINST || m>=LLACUN) break;
  f=1;
  }
 if (f) ymi=0;
 if (13*(yma-ymi+1)<8*ps) return TRUE;

ok:
 h=(ymi-mdl)*n_incl;
 if (h>0)
  x=x0+*(tab_incl+h+ex->incl);
 else
  x=x0-*(tab_incl-h+ex->incl);

 for (i=0; i<nstick; i++)
  {
  y1=cane[i].y;
  incl1=(cane[i].incl-b_incl)/d_incl;
  x1=cane[i].x;
  w1=cane[i].w;
  for (n=0,ym=MIN(yma,y1+cane[i].l-1),y=MAX(ymi,y1); y<=ym; y++)
   if (abs(x-2*x1+*(tab_incl+(y-ymi)*n_incl+ex->incl)-
                  *(tab_incl+(y-y1)*n_incl+incl1))<=w+w1)
    n++;
  if (5*n>2*(yma-ymi+1)) return TRUE;
  }

 if (nstick==NSTICKMAX) return FALSE;

 cane[nstick].x=(x+((x>0)?1:-1))/2;
 cane[nstick].y=(uchar)ymi;
 cane[nstick].l=yma-ymi+1;
 cane[nstick].incl=(int16_t)incl;
 cane[nstick].w=(uchar)w;
 cane[nstick].bot=(uchar)typend(&cane[nstick],0);
 cane[nstick].top=(uchar)typend(&cane[nstick],1);

 nstick++;

 for (i=0; i<n; i++)
    {
         if (aval[i] < 0 || aval[i] >= NVALMAX )
            continue;
         val[aval[i]].c=1;
    }
 return TRUE;
 }

static int32_t typend(STICK *st,int32_t mode)
// returned value    -2 - left bend
//                   -1 - about left bend
//                    0 - straitgh stick
//                    1 - about right bend
//                    2 - right bend
 {
 struct pairs val[VERTMAX/3];
 int32_t l,ll,y,yb,ye,dy,x,x0,xm,dx,i,incl,h;                        //20.02.97
 int32_t r;                                                          //20.02.97

 incl=(st->incl-b_incl)/d_incl;
 l=st->l/3; if (l<7) l=7; if (l>st->l/2) l=st->l/2;
 dx=(st->w+1)/2+1; if (dx<3) dx=3;
 if (mode)
  {
  ye=st->y+st->l; yb=ye-l; dy=1;
  if (ye<H) {ye++; l++;}
  if (ye<H) {ye++; l++;}
  if (ye<H) {ye++; l++;}
  }
 else
  {
  ye=st->y-1; yb=ye+l; dy=-1;
  if (ye>=0) {ye--; l++;}
  if (ye>=0) {ye--; l++;}
  if (ye>=0) {ye--; l++;}
  }
 for (ll=0,y=yb; y!=ye; y+=dy)
  {
  h=(y-st->y)*n_incl;
  if (h>0)
   x0=st->x+*(tab_incl+h+incl)/2;
  else
   x0=st->x-*(tab_incl-h+incl)/2;
  for (xm=MIN(W-1,x0+dx),x=MAX(0,x0-dx);
       x<=xm && /* !(raster[y*lr+x/8]&(128>>(x&7))) */!raster[y*lr+x]; x++) ;
  if (x>xm) continue;
  val[ll].b=(uchar)(x-(x0-dx));
  for (x=xm; /* !(raster[y*lr+x/8]&(128>>(x&7))) */!raster[y*lr+x]; x--) ;
  val[ll].e=(uchar)(x-(x0-dx));
/*  printf("b=%d,e=%d\n",val[ll].b,val[ll].e);*/
  ll++;
  }
/* scanf("%c",&x);*/

 r=bend(ll,val);
 for (i=0; i<ll; i++) val[i].b=2*dx-val[i].e;
 r-=bend(ll,val);
 return r;
 }

static int32_t bend(int32_t l,struct pairs *val)
 {
 int32_t r,s,i,x;                                                    //20.02.97

 for (r=s=0,x=val[0].b,i=1; i<l; i++)
  if (val[i].b>x)
   {
   r++;
   s+=val[i].b-x;
   x=val[i].b;
   }
 if (!r || r==1 && s==1) return 0;
 if (r==1 || r==2 && s==2) return 1;
 return 2;
 }

static void ordsticks()
 {
 STICK s;
 int32_t i,j;                                                        //20.02.97

 for (i=1; i<nstick; i++)
  for (j=i; j<nstick; j++)
   if (cane[i-1].x>cane[j].x)
    {
    memcpy(&s,&cane[i-1],sizeof(STICK));
    memcpy(&cane[i-1],&cane[j],sizeof(STICK));
    memcpy(&cane[j],&s,sizeof(STICK));
    }
 }
