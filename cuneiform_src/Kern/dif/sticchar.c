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

#include "stick.h"
#include <string.h>
#include <stdlib.h>

#include "minmax.h"

#define COPY_PEAK(lr,i,j)                \
	{                                \
	lr->mount[i]  = lr->mount[j];    \
	lr->m_pos[i]  = lr->m_pos[j];    \
	lr->mb_pos[i] = lr->mb_pos[j];   \
	lr->me_pos[i] = lr->me_pos[j];   \
	}

#define COPY_CONC(lr,i,j)                \
	{                                \
	lr->conc[i]   = lr->conc[j];     \
	lr->c_pos[i]  = lr->c_pos[j];    \
	lr->cb_pos[i] = lr->cb_pos[j];   \
	lr->ce_pos[i] = lr->ce_pos[j];   \
	}

#define SET_4_VARS(arr1,arr2,arr3,arr4,ind,val1,val2,val3,val4) \
	{                                                       \
	arr1[ind] = val1;                                       \
	arr2[ind] = val2;                                       \
	arr3[ind] = val3;                                       \
	arr4[ind] = val4;                                       \
	}

//////////////////////Functions prototypes///////////////////////////
int16_t set_stick_char (uchar left[], uchar right[], int16_t hooks[],
       int16_t dy, int16_t dx, int16_t opt, int16_t wide, int16_t corr_mode,
       int16_t skip_ul, int16_t skip_dl, int16_t skip_ur, int16_t skip_dr,
       int16_t inc_num,
		   STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
       int16_t *l_mode, int16_t *r_mode);
static int16_t fun_ge(uchar fun[],int16_t n, int16_t level);
static int16_t fun_le(uchar fun[],int16_t n, int16_t level);
static void calc_chars (STICK_CHARS *res, int16_t level, int16_t typ,
          uchar fun[], int16_t n, int16_t skip_u, int16_t skip_d);
static int16_t study_nose_1(uchar left[],int16_t n,int16_t level, int16_t width);
static void set_serifs(uchar left[],uchar right[],
		       STICK_CHARS *left_chars,STICK_CHARS *right_chars,
           int16_t skip_ul,int16_t skip_dl,int16_t skip_ur,int16_t skip_dr,
           int16_t l,int16_t r,int16_t dy,int16_t dx);
static void correct_serifs(uchar left[],uchar right[],
		       STICK_CHARS *left_chars,STICK_CHARS *right_chars,
           int16_t skip_ul,int16_t skip_dl,int16_t skip_ur,int16_t skip_dr,
           int16_t l,int16_t r,int16_t dy);
static int16_t correct_beam(STICK_CHARS *l, STICK_CHARS *r,int16_t lev,int16_t dist);
static int16_t correct_neck(STICK_CHARS *l, STICK_CHARS *r,int16_t lev,int16_t dist);
static void num_flag_conc (STICK_CHARS *res, int16_t width);
static void set_near ( uchar fun[], int16_t skip_u, int16_t skip_d,
       int16_t n_full, int16_t lev, int16_t lim[],
       int16_t left[], int16_t right[], int16_t l_pos[], int16_t r_pos[],
       int16_t lb_pos[], int16_t le_pos[], int16_t rb_pos[], int16_t re_pos[],
       int16_t *sum_l_near_int, int16_t *sum_r_near_int);
static int16_t set_flags (STICK_CHARS *res, int16_t right[], int16_t left[],
         int16_t r_pos[], int16_t l_pos[],
         int16_t mb_pos[], int16_t me_pos[],int16_t cb_pos[], int16_t ce_pos[],
         int16_t sum_r, int16_t sum_l, int16_t ear);
static int16_t compress4(int16_t v);
static void set_long(uchar fun[],int16_t skip_u,int16_t skip_d,int16_t n,
        int16_t lev,int16_t lim[], int16_t sl, int16_t sr,
        int16_t left[],int16_t right[],int16_t l_pos[],int16_t r_pos[],
              int16_t lb_pos[],int16_t le_pos[],int16_t rb_pos[],int16_t re_pos[],
        int16_t *ll,int16_t *rr);
static int16_t correct_two_zones(int16_t *kk,int16_t *mm,int16_t i,int16_t imax,int16_t extr_pos,
         int16_t lim[]);
static int16_t index_arr(int16_t v,int16_t limit[],int16_t n);
static void find_peak_new (int16_t *ind1, int16_t *ind2, uchar fun[], int16_t nn,
  int16_t level, int16_t sr, int16_t *extr_pos, uchar *extr_fun);
static int16_t typ_nose_1(uchar left[],int16_t n,int16_t lim,int16_t lev);
static Bool16 find_first_ne(uchar fun[],int16_t n,int16_t direct,int16_t level);
static void find_conc_new (int16_t *ind1, int16_t *ind2, uchar fun[], int16_t nn,
  int16_t level, int16_t sl, int16_t *extr_pos, uchar *extr_fun);
///////////////////////////////////////////////////////////////////////////

static uchar tab_1[]=
	{
	120,	// 0. no left upper flag and left mode > 0
	80,	// 1. too down begin of nose
	40,	// 2. too thick nose
	8,	// 3. narrow stick and small nose
	20,	// 4. -- // -- by height stick
	80,	// 5. narrow stick and long nose
	20,	// 6. heigh stick hav small nose
	20,	// 7. left meandr and small nose
	80,	// 8. too low nose
//////	12	// 9. MK: VERT FLAT NOSE (first 48, sec.12); PROBA 31.05.1993
	};

int16_t set_stick_char (uchar left[], uchar right[], int16_t hooks[],
       int16_t dy, int16_t dx, int16_t opt, int16_t wide, int16_t corr_mode,
       int16_t skip_ul, int16_t skip_dl, int16_t skip_ur, int16_t skip_dr,
//////       int16_t inc,
       int16_t inc_num,   // 10.12.1993
//////		   STICK_CHARS *left_chars, STICK_CHARS *right_chars,
//////		   STICK_SIGNUMS *signums,
		   STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
       int16_t *l_mode, int16_t *r_mode)
{
//////int16_t l,r;
int16_t lm, rm;
int16_t lll_dis;

memset(s, 0 , sizeof(STICK_SIGNUMS));
rm = opt + (wide<<1) - 2;	/* right mode */
if( rm%4 )
	{
	rm = ((rm>>2)<<2)+2 ;  /* step = 2 */
	if( fun_ge(right,dy,(int16_t)(rm-2))<fun_le(right,dy,(int16_t)(rm+2)) )
		rm += 2;      /* select column (rm-2),(rm+2) */
	else
		rm -= 2;
	}
/*....................................................................*/
calc_chars (r, rm, 0, right, dy, skip_ur, skip_dr);

lm = opt - (wide<<1) + 2;	/* left  mode */
if( lm%4 )
	{
	lm = ((lm>>2)<<2)+2 ;
//////	if( fun_ge(left,dy,l-2)<fun_le(left,dy,lm+2) )	// BEFORE 01.02.1993
//mkm3 =	fun_ge(left,dy,lm-2)<<8; //////////////////////////////////////
//mkm3 |=	fun_le(left,dy,lm+2);	/////////////////////////////////////////////
	if( fun_ge(left,dy,(int16_t)(lm-2)) <= fun_le(left,dy,(int16_t)(lm+2))+1 )	// 01.02.1993
		lm += 2;    /* select black column */
	else
		lm -= 2;
	}
if( lm>rm )
	lm=rm; /* left can be less or equ right */
if (lm<0)  lm = 0;	// 14.10.1993 for h14/1 CUT: "Tri" => "Yli" (see 'l')

calc_chars (l, lm, 1, left, dy, skip_ul, skip_dl);

//////signums->dis_nose_1 = study_nose_1(left,dy,lm,wide);
	/* study_nose_1 - discriminator 1 for flat long nose */
lll_dis = study_nose_1(left,dy,lm,wide);	// 01.06.1993
s->lll_nose_1 = lll_dis >> 8;		// 00xx or 01xx => 0 or 1
s->dis_nose_1 = lll_dis & 0xFF;

s->typ_nose_1 = (uchar)typ_nose_1(left,dy,(int16_t)((wide>4)?4:2),lm);
	/* typ_nose_1 - indicator normal (inc) nose 1 */

l->up_hook    = (hooks[0]>1);	/* hooks-signums ex. right down : */
l->down_hook  = (hooks[1]>1);	/*   °                            */
r->up_hook   = (hooks[2]>1);	/*   °  °                         */
r->down_hook = (hooks[3]>1);	/*   °°°°                         */

s->height = (uchar)dy;
s->width  = dx;
s->stick_width  =(uchar) wide;

/*****************************	BEFORE 10.12.1993:
//////s->inc    = inc;		// before 03.06.1993
s->inc    = inc>>8;		// 03.06.1993: inc: (0/1).(inc_v/16)
//////s->inc_v16 = inc & 0xFF;
s->inc_v16 = (char) (inc & 0xFF);	// 17.11.1993 for case inc_v16<0;
********************************/

s->inc = (inc_num!=0);
s->inc_num = (char)inc_num;	// 10.12.1993

/* serifs checking */
set_serifs (left, right, l, r,
	    skip_ul, skip_dl, skip_ur, skip_dr, lm, rm, dy, dx);
correct_serifs (left, right, l, r,
		skip_ul, skip_dl, skip_ur, skip_dr, lm, rm, dy);

if( !correct_beam(l,r,(int16_t)(dy>9?dy/10:1),1) )
	correct_beam (l, r,(int16_t)( dy>19 ? dy/10 : 2 ), 0);

correct_neck (l, r, 2, 0);

num_flag_conc (r, wide);	// NB: 06.01.1994 add l,r->max_flag;
num_flag_conc (l, wide);

//////*l_mode = (lm-corr_mode)>>2;
///*l_mode = left_mode_EEM = (lm-corr_mode)>>2;	// 05.08.1993	SOPLI I WOPLI
///*r_mode = (rm-corr_mode)>>2;
    *l_mode = (lm-corr_mode)>>2;	// 07.01.1994
    s->left_mode  = (uchar)*l_mode;
    *r_mode = (rm-corr_mode)>>2;
    s->right_mode =(uchar) *r_mode;
//////s->full_width = l->max_flag + wide + r->max_flag;	// 07.01.1994	###
s->full_width = (s->right_mode - s->left_mode + 1) +	// 07.01.1994
		l->max_flag + r->max_flag;

return(1);
}


static int16_t fun_ge(uchar fun[],int16_t n, int16_t level)
{
int16_t i,s,f;
for(s=i=0;i<n;i++)
	{
	f=fun[i];
	s+= ( f>=level ) && ( f<level+4 ) ;
	}
return(s);
}

static int16_t fun_le(uchar fun[],int16_t n, int16_t level)
{
int16_t i,s,f;
for(s=i=0;i<n;i++)
	{
	f=fun[i];
	s+= ( f<=level ) && ( f>level-4 ) ;
	}
return(s);
}

static void calc_chars (STICK_CHARS *res, int16_t level, int16_t typ,
          uchar fun[], int16_t n, int16_t skip_u, int16_t skip_d)
{
#define SIZ 6
int16_t r, l, sr=2, sl=2, sum_r, sum_l;
int16_t left[5],  l_pos[5], lb_pos[5], le_pos[5];
int16_t right[5], r_pos[5], rb_pos[5], re_pos[5];
int16_t lim[SIZ];

if( level<0 )
	level = 0;

memset (res,    0,  sizeof (STICK_CHARS));
memset (left,   0,  sizeof (left));
memset (right,  0,  sizeof (right));
memset (r_pos,  0,  sizeof (r_pos));
memset (l_pos,  0,  sizeof (l_pos));
memset (lb_pos, 0,  sizeof (lb_pos));
memset (le_pos, 0,  sizeof (le_pos));
memset (rb_pos, 0,  sizeof (rb_pos));
memset (re_pos, 0,  sizeof (re_pos));

lim[0] = 0;  lim[1] = n/6;  lim[2] = n/3;
lim[3] = (n<<1)/3;  lim[4] = n-n/6;  lim[5] = n;

if( n<16 )
//////	{	lim[1]++; lim[2]++;	lim[3]--; lim[4]++;	}  // OLEG ###
	{	lim[1]++; lim[2]++;	lim[3]--; lim[4]--;	}  // MK

set_near (fun, skip_u, skip_d, n, level, lim,
		left, right, l_pos, r_pos,
		lb_pos, le_pos, rb_pos, re_pos, &sum_l, &sum_r);

set_long (fun, skip_u, skip_d, n, level, lim, sl, sr,
		left, right, l_pos, r_pos,
		lb_pos, le_pos, rb_pos, re_pos, &l, &r);

	/*		      і                */
	/*		      і                */
	/*     left[]         і     right[]    */
	/*	              і                */
	/*		      і                */
	/*	            level              */
	/*     right :  conc       mount       */
	/*     left  :  mount      conc        */

	if( typ )
			/* for left array */
		set_flags (res, left, right, l_pos, r_pos,
			  lb_pos, le_pos, rb_pos, re_pos, sum_l, sum_r, l);
	else
			/* for right array */
		set_flags (res, right, left, r_pos, l_pos,
			  rb_pos, re_pos, lb_pos, le_pos, sum_r, sum_l, r);

return;
}

static int16_t study_nose_1(uchar left[],int16_t n,int16_t level, int16_t width)
{
int16_t  i = -1, ii, nn=n>>1, dis=0 ,minim, imin  ,s , lmin ;

if( level==0 )
	return(0);						// 0000 !!!

for(minim=left[(imin=0)],ii=1;ii<nn;ii++)
	if( minim>left[ii] )
		minim = left[(imin=ii)];

if( level==minim )
	return(tab_1[0]);  /* no left objects */			// 120

for (lmin=imin; lmin<nn && left[lmin]==minim; lmin++);

lmin -= imin; /* thickness of nose */

if( left[0]==level )
	{   /* part of line    */
	for( i=0;i<nn && left[i]>=level; i++);
	if( i>2 )           /* i - number of first nose-row */
		dis += tab_1[1];	// too down begin of nose	// 80
	if( i>0 && lmin>width && width>2 )
		dis += tab_1[2];	// too thick nose		// 40
	}

if( left[0]==left[1] )
	{   /* part of line    */
	if( left[0]>0 )
		{
    for( i=0;i<nn && abs(left[i]-left[i+1])<4 ; i++);
		if( i>2 )           /* i - number of first nose-row */
			dis += tab_1[1];  // too down begin of nose	// 80
		}
	else
		{    /* upper nose */
    for(i=0;i<nn && abs(left[i]-left[i+1])<4 ; i++);
		if( i>width )
			dis+=tab_1[2];	// too thick nose		// 40
		}
	}

if( level==4+((minim>>2)<<2) )
	{    /* width of nose = 1 */
	if( i<0 )i=0;
	if( width>4 || level>4 )
		for( ;i<nn && left[i]>=level; i++);
	else
		for(i=0;i<nn && left[i]>=4;i++);
	for(ii=i;i<n && left[i]==left[i+1];i++);
	if( i==ii )
		{ /* too small object ( height=1 ) */
		if( width>3 )
			dis += tab_1[3]*(width-3);			// *8
		else if( n>14 )
			dis += tab_1[4]; // big height of narrow stick	// 20
		}
	if( i+1-ii > ((n*3)>>3) )
		dis += tab_1[5];	// too long object ( width=1 )	// 80
	else if( i+1-ii == ((n*3)>>3) && n<14 )
		dis += tab_1[5];	// too long object ( width=1 )	// 80
	if( n > 20 )
		dis += tab_1[6];	// high stick hav small nose	// 20

    for(s=(left[0]==0),i=1;i<n-1;i++)       // 08.04.93 AL, MK
		if( left[i]!=0 && left[i+1]==0 )
			s++;
	if( s>1 )
		dis += tab_1[7]*(s-1);	// left meandr			//*20
	}

if( dis==0 )	{  /* second conditions for low nose */
	for(ii=0;ii<imin && left[ii]>=level;ii++);
	if( ii==imin && ii>(3*n)/20 )
		dis += tab_1[8];	// too low nose			// 80
	}
/*......................................................................*/
/************************************************************************
if (dis==0)		// 31.05.1993	PROBA for STD-4 unlv2009,2010
	if (lmin>=3)
//////		dis += tab_1[9];	// VERT FLAT NOSE		// 12
		dis += 2;	//   ************************************
		*********************************************************/

//////	if (lmin>=3)  dis |= 0x0100;	// 01.06.1993  for signums->lll_nose_1

	if (lmin>=3 && lmin>imin)		// 02.06.1993
		dis |= 0x0100;			// for signums->lll_nose_1

return( dis );
}

static void set_serifs(uchar left[],uchar right[],
		       STICK_CHARS *left_chars,STICK_CHARS *right_chars,
           int16_t skip_ul,int16_t skip_dl,int16_t skip_ur,int16_t skip_dr,
           int16_t l,int16_t r,int16_t dy,int16_t dx)
{
int16_t l1,l2,r1,r2,dy_u=dy/6,dy_d=dy-1-dy/6,d=(dx-1)<<2;

r1 = MIN(r+4,d); r2 = MIN(r+8,d); /* r1-one,r2-two step in right direct */
if( r1==r2 )
	r2 += 4; /* right mode = dx-1 */
/* upper right serif */
if( right_chars->mount[0] && right_chars->m_pos[0]<dy_u )
{	   /* exist right upper flag            */
	   /* his extremum belong 1/6 of height */
if( right[skip_ur]>=r2 ||
    right_chars->mount[0]>1 && right_chars->mb_pos[0]<3 )
	right_chars->up_serif=2; /* long flag   */
else if( right[skip_ur]>=r1 )
	right_chars->up_serif=1; /* short flag  */
}

/* downer right serif */
if( right_chars->mount[4] && right_chars->m_pos[4]>dy_d )
{
if( right[dy-1-skip_dr]>=r2 ||
    right_chars->mount[4]>1 && right_chars->me_pos[4]>dy-3 )
	right_chars->down_serif=2;
else if( right[dy-1-skip_dr]>=r1 )
	right_chars->down_serif=1;
}

l2 = MAX(l-8,0); l1 = MAX(l-4,0);  /* l1-one,l2-two step in left direct */

/* upper left serif */
if( left_chars->mount[0] && left_chars->m_pos[0]<dy_u )
{
if( left[skip_ul]<=l2 ||
    left_chars->mount[0]>1 && left_chars->mb_pos[0]<3 )
	left_chars->up_serif=2;
else if( left[skip_ul]<=l1 )
	left_chars->up_serif=1;
}

/* downer left serif */
if( left_chars->mount[4] && left_chars->m_pos[4]>dy_d )
{
if( left[dy-1-skip_dl]<=l2 ||
    left_chars->mount[4]>1 && left_chars->me_pos[4]>dy-3 )
	left_chars->down_serif=2;
else if( left[dy-1-skip_dl]<=l1 )
	left_chars->down_serif=1;
}
return;
}

static void correct_serifs(uchar left[],uchar right[],
		       STICK_CHARS *left_chars,STICK_CHARS *right_chars,
           int16_t skip_ul,int16_t skip_dl,int16_t skip_ur,int16_t skip_dr,
           int16_t l,int16_t r,int16_t dy)
{
int16_t dy_u=dy/6;

/* right upper serif */
if( left_chars->up_serif==2 && right_chars->up_serif==0 )
	{
	if( right_chars->mount[0]==0 && right_chars->mount[1]==0 &&
	    right[skip_ur]==r && find_first_ne(&right[skip_ur],dy_u,1,r) )
		right_chars->up_serif=1;
		/* set half-serif if not exist flags belong  */
		/* 1/3 of height and exist pit               */
	}

if( right_chars->up_serif==2 && left_chars->up_serif==0 )
	{
	if( left_chars->mount[0]==0 && left_chars->mount[1]==0 &&
	    left[skip_ul]==l && find_first_ne(&left[skip_ul],dy_u,1,l) )
		left_chars->up_serif=1;
	}

if( left_chars->down_serif==2 && right_chars->down_serif==0 )
	{
	if( right_chars->mount[4]==0 && right_chars->mount[3]==0 &&
	    right[dy-1-skip_dr]==r &&
	    find_first_ne(&right[dy-1-skip_dr],dy_u,-1,r) )
		right_chars->down_serif=1;
	}

if( right_chars->down_serif==2 && left_chars->down_serif==0 )
	{
	if( left_chars->mount[4]==0 && left_chars->mount[3]==0 &&
	    left[dy-1-skip_dl]==l &&
	    find_first_ne(&left[dy-1-skip_dl],dy_u,-1,l) )
		left_chars->down_serif=1;
	}

dy_u = dy/6+1;  /* KNOT !!! */
if( right_chars->up_serif &&
    !find_first_ne(&right[skip_ur],dy_u,1,right[skip_ur]) )
	right_chars->up_serif = 0;
if( left_chars->up_serif &&
    !find_first_ne(&left[skip_ul],dy_u,1,left[skip_ul]) )
	left_chars->up_serif = 0;
if( right_chars->down_serif &&
    !find_first_ne(&right[dy-1-skip_dr],dy_u,-1,right[dy-1-skip_dr]) )
	right_chars->down_serif = 0;
if( left_chars->down_serif &&
    !find_first_ne(&left[dy-1-skip_dl],dy_u,-1,left[dy-1-skip_dl]) )
	left_chars->down_serif = 0;

return;
}

static int16_t correct_beam(STICK_CHARS *l, STICK_CHARS *r,int16_t lev,int16_t dist)
{
int16_t i,lm,rm,lmn,rmn,t,dist1=MAX(dist,1);
for(i=0;i<2;i++) /* study zones 0,1,2 */
	{
	lm  = l->mount[i];	rm  = r->mount[i];
	lmn = l->mount[i+1];	rmn = r->mount[i+1];
  if( lm>dist && rm>dist && abs(l->m_pos[i]-r->m_pos[i])<=lev )
		continue;
  t =  abs(l->m_pos[i] - r->m_pos[i+1]) ;
	if( lm>dist && rmn>dist && lmn<lm && t<=lev ||
	    lm>=dist1 && rmn>=dist1 && lmn<=lm && t<=1 )
		{        /* left(i) <--> right(i+1) */
		if( lm< rmn )
			{  /* tranfser flag left(i) to left(i+1) */
			COPY_PEAK(l,i+1,i);
			l->m_pos[i]=l->mount[i] = 0;
			}
		else
			{ /* tranfser flag right(i+1) to right(i) */
			COPY_PEAK(r,i,i+1);
			r->m_pos[i+1]=r->mount[i+1] = 0;
			}
		break;
		}

  t =  abs(l->m_pos[i+1] - r->m_pos[i]) ;
	if( lmn>dist && rm>dist && rmn<rm && t<=lev ||
	    lmn>=dist1 && rm>=dist1 && rmn<=rm && t<=1 )
		{  /* left(i+1) <--> right(i) */
		if( rm<lmn )
			{ /* tranfser flag right(i) to right(i+1) */
			COPY_PEAK(r,i+1,i);
			r->m_pos[i]=r->mount[i] = 0;
			}
		else
			{ /* tranfser flag left(i+1) to left(i) */
			COPY_PEAK(l,i,i+1);
			l->m_pos[i+1]=l->mount[i+1] = 0;
			}
		break;
		}

	}

for(i=0;i<3;i++)
	if( (r->mount[i]>dist)&&(l->mount[i]>dist) )
		return(1);

return(0);
}

static int16_t correct_neck(STICK_CHARS *l, STICK_CHARS *r,int16_t lev,int16_t dist)
{
int16_t i,lm,rm,lmn,rmn;
for(i=0;i<2;i++) /* study zones 0,1,2 */
	{
	lm  = l->conc[i];	rm  = r->conc[i];
	lmn = l->conc[i+1];	rmn = r->conc[i+1];
  if( lm>dist && rm>dist && abs(l->c_pos[i] - r->c_pos[i])<=lev)
		continue;
	if( lm>dist && rmn>dist && lmn<lm &&
      abs(l->c_pos[i] - r->c_pos[i+1])<=lev )
		{        /* left(i) <--> right(i+1) */
		if( lm< rmn )
			{  /* tranfser conc left(i) to left(i+1) */
			COPY_CONC(l,i+1,i);
			l->c_pos[i]=l->conc[i] = 0;
			}
		else
			{ /* tranfser conc right(i+1) to right(i) */
			COPY_CONC(r,i,i+1);
			r->c_pos[i+1]=r->conc[i+1] = 0;
			}
		break;
		}

	if( lmn>dist && rm>dist && rmn<rm &&
      abs(l->c_pos[i+1] - r->c_pos[i])<=lev )
		{  /* left(i+1) <--> right(i) */
		if( rm<lmn )
			{ /* tranfser conc right(i) to right(i+1) */
			COPY_CONC(r,i+1,i);
			r->c_pos[i]=r->conc[i] = 0;
			}
		else
			{ /* tranfser conc left(i+1) to left(i) */
                        COPY_CONC(l,i,i+1);
			l->c_pos[i+1]=l->conc[i+1] = 0;
			}
		break;
		}

	}

for(i=0;i<3;i++)
	if( (r->conc[i]>dist)&&(l->conc[i]>dist) )
		return(1);

return(0);
}

static void num_flag_conc (STICK_CHARS *res, int16_t width)
{
int16_t i, t, sc, sf, n;
int16_t max_flag=0;
for (sc=sf=n=i=0; i<5 ;i++)
	{
	if(res->mount[i]>0 )
		sf++;
	if(res->conc[i]>0 )
		sc++;
//////	if( n<res->mount[i] )
//////		n = res->mount[i];
	if( max_flag<res->mount[i] )		// EDITION 06.01.1994
		max_flag = res->mount[i];
	}
	/* nr - maximal longs */
//////t = n>width/2 ? width/2 :1;
t = (max_flag>width/2)  ?  width/2  :  1;	// EDITION 06.01.1994
if( !t ) t=1;
for(n=i=0; i<5 ;i++)
	if(res->mount[i]>t)
		n++;

res -> num_flags =(uchar) sf;          /* number of flags      */
res -> num_concs =(uchar) sc;          /* number of concaves   */
res -> num_long_flags = (uchar) n;      /* number of long flags */
res -> max_flag =(uchar) max_flag;	// 06.01.1994

return;
}

static void set_near ( uchar fun[], int16_t skip_u, int16_t skip_d,
//////       int16_t n, int16_t lev, int16_t lim[],
       int16_t n_full, int16_t lev, int16_t lim[],
       int16_t left[], int16_t right[], int16_t l_pos[], int16_t r_pos[],
       int16_t lb_pos[], int16_t le_pos[], int16_t rb_pos[], int16_t re_pos[],
       int16_t *sum_l_near_int, int16_t *sum_r_near_int)
{							// 28.01.1994
				// NOTA BAENE: 28.01.1994 LEMAN call to OLEG;
				// CHANGE  "n"  to  "n_full"  (BBB)
				// and USE NOW  "nn"  ONLY!!!
				// (TAK GOVORIL ZARATUSTRA)
int16_t i, k, b, e, ex,  nn = n_full - skip_d;
uchar  level=(uchar)lev, f; // NB: int16_t lev, uchar level;
int16_t nL, nR;   // 28.01.1994 (OLD l, r);

///for (l=r=0,i=skip_u;i<nn;i++) //this cykl find near mounts for any interval:
for (nL=nR=0,i=skip_u; i<nn; i++)  {			// 28.01.1994
	if ( (f=fun[i])>=level-4 && f<level &&		// FIRST IF;
//////	     (fun[i-1]>=level || i==skip_u) )		// OLEG
	     (i==skip_u || fun[i-1]>=level) )		// 27.01.1994
		{
//////    for (k=i++; i<n && abs(fun[i]-f)<4 && fun[i]<level; i++);
    for (k=i++; i<nn && abs(fun[i]-f)<4 && fun[i]<level; i++);
		i--;
//////		if ( i<n-1 && fun[i+1]>=level-4 || i==n-1 )	// OLEG
		if ( i==nn-1 || fun[i+1]>=level-4 )		// 28.01.1994
			nL++;
//////		if ( fun[i+1]>=level || i==nn-1 )  {	/* local conc */
		if ( i==nn-1 || fun[i+1]>=level )  {	// 28.01.1994
			b = index_arr (k,lim,SIZ);
			e = index_arr (i,lim,SIZ);
			ex = (k+i)>>1;
			if ( correct_two_zones(&b,&e,i,k,ex,lim) )
			    SET_4_VARS(left,l_pos,lb_pos,le_pos,b,4,ex,k,i);
			}
		}	// FIRST IF;

	else if(f<=level+4 && f>level &&		// SECOND IF;
//////		(fun[i-1]<=level || i==skip_u))		// OLEG
		(i==skip_u || fun[i-1]<=level))		// 27.01.1994
		{
//////    for (k=i++; i<n && abs(fun[i]-f)<4 && fun[i]>level; i++);
    for (k=i++; i<nn && abs(fun[i]-f)<4 && fun[i]>level; i++);
		i--;
//////		if ( i<n-1 && fun[i+1]<=level+4 || i==n-1)	// OLEG
		if ( i==nn-1 || fun[i+1]<=level+4)		// 28.01.1994
			nR++;
//////		if ( fun[i+1]<=level || i==nn-1 )  {	/* local pimple */
		if ( i==nn-1 || fun[i+1]<=level )  {	// 28.01.1994
			b = index_arr(k,lim,SIZ);
			e = index_arr(i,lim,SIZ);
			ex = (k+i)>>1;
			if( correct_two_zones(&b,&e,i,k,ex,lim) )
			    SET_4_VARS(right,r_pos,rb_pos,re_pos,b,4,ex,k,i);
			}
		}	// SECOND IF;
	}	// FOR I;
/*......................................................................*/
*sum_l_near_int = nL;
*sum_r_near_int = nR;

return;
}

static int16_t set_flags (STICK_CHARS *res, int16_t right[], int16_t left[],
         int16_t r_pos[], int16_t l_pos[],
         int16_t mb_pos[], int16_t me_pos[],int16_t cb_pos[], int16_t ce_pos[],
         int16_t sum_r, int16_t sum_l, int16_t ear)
{
int16_t i;

for(i=0;i<5;i++)
	{
	res->mount[i] = (uchar)compress4(right[i]);
	res->m_pos[i] = (uchar)r_pos[i];
	res->mb_pos[i] = (uchar)mb_pos[i];
	res->me_pos[i] = (uchar)me_pos[i];

	res->conc[i]  =(uchar) compress4(left[i]);
	res->c_pos[i] =(uchar) l_pos[i];
	res->cb_pos[i] = (uchar)cb_pos[i];
	res->ce_pos[i] =(uchar) ce_pos[i];
	}
res->m_meandr = (uchar)sum_r;
res->c_meandr = (uchar)sum_l;
res->main_ear = (uchar)ear  ;

return(1);
}

static int16_t compress4(int16_t v)
{
if ( v==0 )			// 0  1  2  3  4  5  6  7  8  9 10 11  v
	return(0);		// 0  1  1  1  1  1  2  2  2  2  3  3  OLEG
//////if ( v<4 )		// 0  1  1  1  1  2  2  2  2  2  3  3  MISA
if ( v<5 )			// NB: THE SAME ???
	return(1);
if ( v==5 )			// 08.07.1993 MK PROBA
	return(2);

if( (v&3)>1 )
	v = (v>>2)+1;
else
	v >>= 2;
return( v );
}

static void set_long(uchar fun[],int16_t skip_u,int16_t skip_d,int16_t n,
        int16_t lev,int16_t lim[], int16_t sl, int16_t sr,
        int16_t left[],int16_t right[],int16_t l_pos[],int16_t r_pos[],
              int16_t lb_pos[],int16_t le_pos[],int16_t rb_pos[],int16_t re_pos[],
        int16_t *ll,int16_t *rr)
{
int16_t i,k,m,extr,imax,l,r,nn=n-skip_d;
 uchar level=(uchar)lev,f;
for(r=l=0,i=skip_u;i<nn;i++)
	{
	f=fun[i];
	if( f<=level+4 && f>=level-4 ) continue;
	if( f>level+4 )
		find_peak_new (&i,&imax,fun,n,level,sr,&extr,&f);  // MK NEW
/* right lane ________________________ f = MAX( fun[i] ) ______________  */
	else	/* f<level+4 */
		find_conc_new (&i,&imax,fun,n,level,sl,&extr,&f);  // MK NEW
/* left lane ___________________________ f = MAX(fun[i]) _____________ */

	k = index_arr(imax,lim,SIZ);    /* imax -  begin interval */
	m = index_arr(i,lim,SIZ);       /* i    -  end interval   */

	if( correct_two_zones(&k,&m,i,imax,extr,lim) )
		{
/* good mount : attemp set flag _____________________________________   */
		if( f>level+4 )
			{           /* right lane : */
			SET_4_VARS(right,r_pos,rb_pos,re_pos,k,
				   f-level,extr,imax,i);
			}
		else                /* f<level-4    */
			{           /* left lane :  */
			SET_4_VARS(left,l_pos,lb_pos,le_pos,k,
				   level-f,extr,imax,i);
			}
		}
	else
		{
		if( f>level+4 )
			{
			if( k==0 && m==2 )
				r = 1;
      else if ( k==0 && abs(lim[2]-i)<3 )
				r = 2;
			}
		else                /* f<level-4 */
			{
			if( k==2 && m==4 )
				l = 1;
      else if ( m==4 && abs(i-lim[3])<3 )
				l = 2;
			}

		}

	}/* end of main-for-if */
*ll=l; *rr=r;
return;
}

static int16_t correct_two_zones(int16_t *kk,int16_t *mm,int16_t i,int16_t imax,int16_t extr_pos,
         int16_t lim[])
{
int16_t k=*kk, m=*mm;

if( k==m )
	return(1);

if( m==4 && k>1 && (extr_pos>lim[4]||i>lim[5]-3)/*i>lim[5]-((lim[5]-lim[3])>>1)*/  )
	k=4;
else  if( k==3 || extr_pos>=lim[3])       /* correct down objects */
	m=3;

if( k==0 && m<3 && (extr_pos<=lim[1]||imax<3)/*imax<((lim[2]-lim[0])>>1)*/ )
	m=0;
else  if( m==1 || extr_pos<=lim[2] )       /* correct up objects */
	k=1;

if( m==k+1 && k>0 && k<3 )
	{   /* two intervals : long object */
	imax >>=1; imax <<=1;
	i >>=1;    i <<=1;
	if( (lim[m]-imax)>=(i-lim[m]) )
		m=k;
	else
		k=m;
	}

if( k==1 && m==3 )
	k=m=2;       /* central object */

*kk = k;
*mm = m;
return( k==m );
}

static int16_t index_arr(int16_t v,int16_t limit[],int16_t n)
{
int16_t i;
for(i=1; i<n && v>=limit[i]; i++);
return( i-1 );
}

static void find_peak_new (int16_t *ind1, int16_t *ind2, uchar fun[], int16_t nn,
  int16_t level, int16_t sr, int16_t *extr_pos, uchar *extr_fun) // MK NEW
{							// 14.01.1993
int16_t i=*ind1, extr;
uchar  f=*extr_fun;
///int16_t  w_of_max = right0 [i] - left0 [i];  // MK 19.01.1993
///uchar f_i, w_i;

	extr=*ind2=i++ ;

	//MK: NO END && NEXT==TEK   && NEXT.NEXT==TEK  GO TO NEXT
	if (i<(nn-1) && (fun[i]==f) && (fun[i+1]==f))  extr = i++ ;

		/* increase fun */
	for(; i<nn && fun[i]>level+4 && fun[i]>=fun[i-1]-sr; i++)
		if ( f<fun[i] )  f=fun[(extr=i)];

/******		{					// PROBA 19.01.1993
		if ( f > (f_i=fun[i]) )  continue;
		w_i = right0 [i] - left0 [i];
		if ( (f < f_i) || (w_of_max < w_i) )
			{ f = fun [(extr=i)];  w_of_max = w_i; }
		}******/

		/* decrease fun */
	for(; i<nn && fun[i]>level+4 && fun[i]<=fun[i-1]+sr; i++)
		if ( f<fun[i] )  f=fun[(extr=i)];

/******		{					// PROBA 19.01.1993
		if ( f > (f_i=fun[i]) )  continue;
		w_i = right0 [i] - left0 [i];
		if ( (f < f_i) || (w_of_max < w_i) )
			{ f = fun [(extr=i)];  w_of_max = w_i; }
		}******/

	*ind1 = i-1;
	*extr_pos = extr ;
	*extr_fun = f;
//	return;
}

static int16_t typ_nose_1(uchar left[],int16_t n,int16_t lim,int16_t lev)
{                                         /* normal nose (inc+jumps) : */
int16_t  i ,jumps, ret   ;                        /*        °°             */
for(jumps=i=0;i<n && left[i]>=left[i+1]; i++) /*       °°°             */
	if( left[i]!=left[i+1] )              /*     °°°°°             */
		jumps ++;                     /*    °°°°°°             */
ret = ( i>=lim && jumps>3 );                  /*        °°             */

if( ret )
	{  /* inc nose */
	for(jumps=0; i<n && left[i]<lev; i++)  /* +  °°°°°°            */
		if( left[i]!=left[i+1] )       /*      °°°°            */
			jumps ++;              /*        °°            */
	if( jumps>2 && i*5<n )
		ret = 0;
	}
return( ret );
}

static Bool16 find_first_ne(uchar fun[],int16_t n,int16_t direct,int16_t level)
{
int16_t i,ind;
uchar lev=(uchar)level;
for(ind=i=0;i<n;i++,ind+=direct)
	if( fun[ind]!=lev )
		return(TRUE);
return(FALSE);
}

static void find_conc_new (int16_t *ind1, int16_t *ind2, uchar fun[], int16_t nn,
  int16_t level, int16_t sl, int16_t *extr_pos, uchar *extr_fun) // MK NEW
{							// 14.01.1992
int16_t i=*ind1, extr;
uchar  f=*extr_fun;
	extr=*ind2=i++;

	//MK: NO END && NEXT==TEK   && NEXT.NEXT==TEK  GO TO NEXT
	if (i<(nn-1) && (fun[i]==f) && (fun[i+1]==f))  extr = i++ ;

		/* decrease fun */
	for(;i<nn && fun[i]<level-4 && fun[i]<=fun[i-1]+sl; i++)
		if( f>fun[i] )  f=fun[(extr=i)];

		/* increase fun */
	for(;i<nn && fun[i]<level-4 && fun[i]>=fun[i-1]-sl; i++)
		if( f>fun[i] )  f=fun[(extr=i)];

	*ind1 = i-1;
	*extr_pos = extr ;
	*extr_fun = f;
//	return;
}
