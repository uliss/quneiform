/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

пЮГПЕЬЮЕРЯЪ ОНБРНПМНЕ ПЮЯОПНЯРПЮМЕМХЕ Х ХЯОНКЭГНБЮМХЕ ЙЮЙ Б БХДЕ ХЯУНДМНЦН ЙНДЮ,
РЮЙ Х Б ДБНХВМНИ ТНПЛЕ, Я ХГЛЕМЕМХЪЛХ ХКХ АЕГ, ОПХ ЯНАКЧДЕМХХ ЯКЕДСЧЫХУ СЯКНБХИ:

      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ХЯУНДМНЦН ЙНДЮ ДНКФМШ НЯРЮБЮРЭЯЪ СЙЮГЮММНЕ
        БШЬЕ СБЕДНЛКЕМХЕ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х ОНЯКЕДСЧЫХИ
        НРЙЮГ НР ЦЮПЮМРХИ.
      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ДБНХВМНЦН ЙНДЮ Б ДНЙСЛЕМРЮЖХХ Х/ХКХ Б
        ДПСЦХУ ЛЮРЕПХЮКЮУ, ОНЯРЮБКЪЕЛШУ ОПХ ПЮЯОПНЯРПЮМЕМХХ, ДНКФМШ ЯНУПЮМЪРЭЯЪ
        СЙЮГЮММЮЪ БШЬЕ ХМТНПЛЮЖХЪ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х
        ОНЯКЕДСЧЫХИ НРЙЮГ НР ЦЮПЮМРХИ.
      * мХ МЮГБЮМХЕ Cognitive Technologies, МХ ХЛЕМЮ ЕЕ ЯНРПСДМХЙНБ МЕ ЛНЦСР
        АШРЭ ХЯОНКЭГНБЮМШ Б ЙЮВЕЯРБЕ ЯПЕДЯРБЮ ОНДДЕПФЙХ Х/ХКХ ОПНДБХФЕМХЪ
        ОПНДСЙРНБ, НЯМНБЮММШУ МЮ ЩРНЛ он, АЕГ ОПЕДБЮПХРЕКЭМНЦН ОХЯЭЛЕММНЦН
        ПЮГПЕЬЕМХЪ.

щрю опнцпюллю опеднярюбкемю бкюдекэжюлх юбрнпяйху опюб х/хкх дпсцхлх кхжюлх "йюй
нмю еярэ" аег йюйнцн-кхан бхдю цюпюмрхи, бшпюфеммшу ъбмн хкх ондпюгслебюелшу,
бйкчвюъ цюпюмрхх йнллепвеяйни жеммнярх х опхцндмнярх дкъ йнмйпермни жекх, мн ме
нцпюмхвхбюъяэ хлх. мх бкюдекеж юбрнпяйху опюб х мх ндмн дпсцне кхжн, йнрнпне
лнфер хглемърэ х/хкх онбрнпмн пюяопнярпюмърэ опнцпюллс, мх б йнел яксвюе ме
мея╗р нрберярбеммнярх, бйкчвюъ кчаше наыхе, яксвюимше, яоежхюкэмше хкх
онякеднбюбьхе сашрйх, ябъгюммше я хяонкэгнбюмхел хкх онмеяеммше бякедярбхе
мебнглнфмнярх хяонкэгнбюмхъ опнцпюллш (бйкчвюъ онрепх дюммшу, хкх дюммше,
ярюбьхе мецндмшлх, хкх сашрйх х/хкх онрепх днунднб, онмеяеммше хг-гю деиярбхи
рперэху кхж х/хкх нрйюгю опнцпюллш пюанрюрэ янблеярмн я дпсцхлх опнцпюллюлх,
мн ме нцпюмхвхбюъяэ щрхлх яксвюълх), мн ме нцпюмхвхбюъяэ хлх, дюфе еякх рюйни
бкюдекеж хкх дпсцне кхжн ашкх хгбеыемш н бнглнфмнярх рюйху сашрйнб х онрепэ.

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

/*----------------------------------------------------------------------*/
/*	STIC_CHA.C	28.01.1994	old CHA_STIC.C			*/
/*----------------------------------------------------------------------*/
//////#define	MKPRINT_ENABLE
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

/*  ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/*  ╟╟ CHA_STICK : CALCULATION CHARACTERISTICS OF STICK   05.11.92. ╟╟ */
/*  ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */



			// here was DEBUG_GRAPH
  #include <stdlib.h>
  #include <string.h>

  #include "func.h"
  #include "stick.h"
#include "minmax.h"

extern uchar fax1x2;	// MK NEW 06.01.1993
//////extern uchar left0[], right0[];	// MK NEW 19.01.1993
/*......................................................................*/
#ifdef	MKPRINT_ENABLE				// MK OTLADKA Variables
extern	uint16_t	mkm1, mkm2, mkm3, mkm4, mkm5;
extern	uint16_t	mkma, mkmb, mkmc, mkmd, mkme;
extern	uint16_t	d,d0,d1,d2,d3,d4,d5,d6,d7,d8,d9;
#endif
extern	uint16_t	N_eq_Neck;	// for sign_neck;  see ST_TOOLS.C;
extern	uint16_t	left_mode_EEM;	// NOTA BENE:  NEPORJADOK; see ST_TOOLS.C;
/*......................................................................*/

static int16_t fun_ge(uchar fun[],int16_t n, int16_t level);
static int16_t fun_le(uchar fun[],int16_t n, int16_t level);
static void num_flag_conc(STICK_CHARS *res, int16_t width);
static int16_t typ_nose_1(uchar left[],int16_t n,int16_t lim,int16_t lev);
static int16_t study_nose_1(uchar left[],int16_t n,int16_t level,int16_t width);
static int16_t correct_beam(STICK_CHARS *l, STICK_CHARS *r,int16_t lev,int16_t dist);
static int16_t correct_neck(STICK_CHARS *l, STICK_CHARS *r,int16_t lev,int16_t dist);
static void set_near( uchar fun[],int16_t skip_u, int16_t skip_d,
		    int16_t n,int16_t level,int16_t lim[],
		    int16_t left[],int16_t  right[],int16_t l_pos[],int16_t r_pos[],
                    int16_t lb_pos[],int16_t le_pos[],int16_t rb_pos[],int16_t re_pos[],
		    int16_t *l, int16_t *r);
static int16_t set_flags(STICK_CHARS *res, int16_t right[],int16_t left[],
		     int16_t r_pos[],int16_t l_pos[],
		     int16_t lb_pos[],int16_t le_pos[],int16_t rb_pos[],int16_t re_pos[],
		     int16_t sum_r, int16_t sum_l,int16_t ear);
static void calc_chars(STICK_CHARS *res,int16_t level,int16_t typ,
		uchar fun[],int16_t n,int16_t skip_u,int16_t skip_d);
static int16_t correct_two_zones(int16_t *k,int16_t *m,int16_t i,int16_t im,int16_t ex,int16_t lim[]);
static int16_t index_arr(int16_t v,int16_t limit[],int16_t n);
static int16_t compress4(int16_t v);
static void set_long(uchar fun[],int16_t skip_u,int16_t skip_d,int16_t n,
	      int16_t level,int16_t lim[], int16_t sl, int16_t sr,
	      int16_t left[],int16_t right[],int16_t l_pos[],int16_t r_pos[],
              int16_t lb_pos[],int16_t le_pos[],int16_t rb_pos[],int16_t re_pos[],
	      int16_t *ll,int16_t *rr);
//static void find_peak(int16_t *ind1,int16_t *ind2,uchar fun[],int16_t nn,int16_t level,int16_t sr,
//////	 int16_t *extr_pos,uchar *extr_fun);
static void find_peak_new(int16_t *ind1,int16_t *ind2,uchar fun[],int16_t nn,	// MK
	int16_t level,int16_t sr,int16_t *extr_pos,uchar *extr_fun);
//static void find_conc(int16_t *ind1,int16_t *ind2,uchar fun[],int16_t nn,int16_t level,int16_t sr,
//////	 int16_t *extr_pos,uchar *extr_fun);
static void find_conc_new(int16_t *ind1,int16_t *ind2,uchar fun[],int16_t nn,	// MK
	int16_t level,int16_t sr,int16_t *extr_pos,uchar *extr_fun);
static Bool find_first_ne(uchar fun[],int16_t n,int16_t direct,int16_t level);
static void set_serifs(uchar left[],uchar right[],
		       STICK_CHARS *left_chars,STICK_CHARS *right_chars,
                       int16_t skip_ul,int16_t skip_dl,int16_t skip_ur,int16_t skip_dr,
		       int16_t l,int16_t r,int16_t dy,int16_t dx);
static void correct_serifs(uchar left[],uchar right[],
		       STICK_CHARS *left_chars,STICK_CHARS *right_chars,
		       int16_t skip_ul,int16_t skip_dl,int16_t skip_ur,int16_t skip_dr,
		       int16_t l,int16_t r,int16_t dy);
static void calc_of_skip(int16_t hist[],int16_t dy,int16_t opt,
			 uchar left[],uchar right[],int16_t lim_l,int16_t lim_r,
			 int16_t *s_ul,int16_t *s_dl,int16_t *s_ur,int16_t *s_dr);
static void correct_Y_peak(STICK_CHARS *l,int16_t dy);
static Bool sign_t_TOP (uchar left[], uchar right[]);

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
/*----------------------------------------------------------------------*/
/* in : left,right - abris arrays (by inc need correct (shift),     */
/*                   step of fun-scale = 4 ),                       */
/*      dy - c_comp hei and wid , opt - center-line stick,            */
/*      wide - optimal wide of stick,                               */
/*                                                                  */
/* out : left_chars,right_chars - characteristics of stick          */
/*                                                                  */
int16_t set_stick_char (uchar left[], uchar right[], int16_t hooks[],
		   int16_t dy, int16_t dx, int16_t opt, int16_t wide, int16_t corr_mode,
		   int16_t skip_ul, int16_t skip_dl, int16_t skip_ur, int16_t skip_dr,
//////		   int16_t inc,
		   int16_t inc_num,		// 10.12.1993
//////		   STICK_CHARS *left_chars, STICK_CHARS *right_chars,
//////		   STICK_SIGNUMS *signums,
		   STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		   int16_t *l_mode, int16_t *r_mode)
{
//////int16_t l,r;
int16_t	lm, rm;
int16_t	lll_dis;

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
l->down_hook  = (hooks[1]>1);	/*   ╟                            */
r->up_hook   = (hooks[2]>1);	/*   ╟  ╟                         */
r->down_hook = (hooks[3]>1);	/*   ╟╟╟╟                         */

s->height = dy;
s->width  = dx;
s->stick_width  = (uchar)wide;

/*****************************	BEFORE 10.12.1993:
//////s->inc    = inc;		// before 03.06.1993
s->inc    = inc>>8;		// 03.06.1993: inc: (0/1).(inc_v/16)
//////s->inc_v16 = inc & 0xFF;
s->inc_v16 = (char) (inc & 0xFF);	// 17.11.1993 for case inc_v16<0;
********************************/

s->inc = (inc_num!=0);
s->inc_num = (uchar)inc_num;	// 10.12.1993

/* serifs checking */
set_serifs (left, right, l, r,
	    skip_ul, skip_dl, skip_ur, skip_dr, lm, rm, dy, dx);
correct_serifs (left, right, l, r,
		skip_ul, skip_dl, skip_ur, skip_dr, lm, rm, dy);

if( !correct_beam(l,r,(int16_t)(dy>9?dy/10:1),1) )
	correct_beam (l, r, (int16_t)(dy>19 ? dy/10 : 2), 0);

correct_neck (l, r, 2, 0);

num_flag_conc (r, wide);	// NB: 06.01.1994 add l,r->max_flag;
num_flag_conc (l, wide);

//////*l_mode = (lm-corr_mode)>>2;
///*l_mode = left_mode_EEM = (lm-corr_mode)>>2;	// 05.08.1993	SOPLI I WOPLI
///*r_mode = (rm-corr_mode)>>2;
*l_mode = left_mode_EEM = (lm-corr_mode)>>2;	// 07.01.1994
s->left_mode  = (uchar)(left_mode_EEM);
*r_mode = (rm-corr_mode)>>2;
s->right_mode = (uchar)(*r_mode);
//////s->full_width = l->max_flag + wide + r->max_flag;	// 07.01.1994	###
s->full_width = (s->right_mode - s->left_mode + 1) +	// 07.01.1994
		l->max_flag + r->max_flag;

return(1);
}
/*----------------------------------------------------------------------*/
void correct_upper_peaks(STICK_CHARS *left_chars,
			 STICK_CHARS *right_chars,int16_t dy)
{

correct_Y_peak(left_chars,dy);
correct_Y_peak(right_chars,dy);

return;
}

static void correct_Y_peak(STICK_CHARS *c,int16_t dy)
{
int16_t i;
dy /=3;
if( !c->mount[0] )
	{
	for(i=1;i<3;i++)
		if( c->mount[i] && c->mb_pos[i]<dy && c->m_pos[i]<dy )
			{  /* transfer long upper flag */
			COPY_PEAK(c,0,i);
			c->mount[i]  = 0;
			c->m_pos[i]  = 0;
			c->mb_pos[i] = 0;
			c->me_pos[i] = 0;
			break;
			}
	}
return;
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


/* set good half-serifs                                              */
/* in : left[0:dy-1],right[0:dy-1] - abris-arrays ;                  */
/*      left_chars,right_chars - stick chars;                        */
/*      skip_ul(dl,ur,dr) - numbers of upper left ( downer left,     */
/*                          upper right,downer right) skipped rows ; */
/*      l,r - left and right modes; dy - height of stick             */
static void set_serifs(uchar left[],uchar right[],
		       STICK_CHARS *left_chars,STICK_CHARS *right_chars,
		       int16_t skip_ul,int16_t skip_dl,int16_t skip_ur,int16_t skip_dr,
		       int16_t l,int16_t r,int16_t dy,int16_t dx)
{
int16_t l1,l2,r1,r2,dy_u=dy/6,dy_d=dy-1-dy/6,d=(dx-1)<<2;

r1 = MIN(r+4,d); r2 = min(r+8,d); /* r1-one,r2-two step in right direct */
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

l2 = MAX(l-8,0); l1 = max(l-4,0);  /* l1-one,l2-two step in left direct */

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


/* find hidden serifs                                                */
/* in : left[0:dy-1],right[0:dy-1] - abris-arrays ;                  */
/*      left_chars,right_chars - stick chars;                        */
/*      skip_ul(dl,ur,dr) - numbers of upper left ( downer left,     */
/*                          upper right,downer right) skipped rows ; */
/*      l,r - left and right modes; dy - height of stick             */

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

/* exist value in array (fun) no equivalence (level)                      */
/* n - number of elems in array, direct=1 <--> fun[0],fun[1],...,fun[n-1] */
/* direct=-1 <--> fun[0],fun[-1],...,fun[1-n]                             */
static Bool find_first_ne(uchar fun[],int16_t n,int16_t direct,int16_t level)
{
int16_t i,ind; uchar lev=(uchar)level;
for(ind=i=0;i<n;i++,ind+=direct)
	if( fun[ind]!=lev )
		return(TRUE);
return(FALSE);
}
/*----------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------*/
/* study and calculate prob 1 for his nose (left abris) */
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
/*----------------------------------------------------------------------*/
/* study typ of nose for letter 1 */
static int16_t typ_nose_1(uchar left[],int16_t n,int16_t lim,int16_t lev)
{                                         /* normal nose (inc+jumps) : */
int16_t  i ,jumps, ret   ;                        /*        ╟╟             */
for(jumps=i=0;i<n && left[i]>=left[i+1]; i++) /*       ╟╟╟             */
	if( left[i]!=left[i+1] )              /*     ╟╟╟╟╟             */
		jumps ++;                     /*    ╟╟╟╟╟╟             */
ret = ( i>=lim && jumps>3 );                  /*        ╟╟             */

if( ret )
	{  /* inc nose */
	for(jumps=0; i<n && left[i]<lev; i++)  /* +  ╟╟╟╟╟╟            */
		if( left[i]!=left[i+1] )       /*      ╟╟╟╟            */
			jumps ++;              /*        ╟╟            */
	if( jumps>2 && i*5<n )
		ret = 0;
	}
return( ret );
}
/*----------------------------------------------------------------------*/
/* study upper zone  of right abris                                */
/* in  : fun[0:n-1] - begin of abris ( step of fun-scale=4),       */
/*       dx - wide of c_comp , m - diapason for study : fun[i]>dx-m  */
/*                                                                 */
/* out : r->f_symptom  : 0(not sign),1(bad f-abris),2(good)        */
int16_t calc_right_f_symptom(uchar fun[],int16_t n,int16_t m)
{
int i,l=MIN(m,4),lev,s,n2=n>>1,n3=n/3;
if( l==0 ) l=1;
for(lev=0;lev<l;lev++)
	{
	if( lev<2 )  /* hight propability only in 2 lasts columns */
		{
		for(s=(fun[0]<=lev)+(fun[n-1]<=lev),i=1;i<n2-1;i++)
			s += ((fun[i]<=lev)^(fun[i+1]<=lev));
                        /* s - num of overlav fun-graphic and lev-line */
		if( s >= 4 )
			{       /* exist 2 right pimples */
			for(i=0;i<n2&&fun[i]>lev;i++);
			if( i<2 )   /* not long distance of up limit */
				return(2);
			}
		}
	for(s=i=0;i<n3;i++)
		s += (fun[i]==lev);  /* exist fill in last column ? */
	if( s==n3 )
		{     /* exist upper right protuberane, height==n3 */
		for(;i<n;i++)
			s += (fun[i]==lev);
		if( s<=n2 )  /* if s>n2 then right limit is plane */
			return(1);
		}
	}
return(0);
}

/* study upper zone  of right abris                                */
/* in  : fun[0:n-1] - begin of abris ( step of fun-scale=4),       */
/*       nx - wide of c_comp                                         */
/*                                                                 */
/* out : l->f_symptom : 0(not f left abris),1(bad f),2(good f)     */
int16_t calc_left_f_symptom(uchar fun[],int16_t n,int16_t nx,Bool c_ft)
{
int16_t i,s,ss,f,ret_code,nn=n>>1;
ss=nn*(nx>>1);   /* place of study-zone */
for(s=i=0;i<nn; i++)
	{
	f = fun[i];
	if( f==0 || fun[i]<fun[i+1] )
		break;
	s += f;
	}
f=0;

if( i<nn )
	{
	if( i>MAX(n/6-1,2) )
		{
		for(f=0;i<n&&fun[i]==fun[i+1];i++,f++); i--;
		/* thickness of left peak */
		f = ( f>n/6 );  /* f=1 - too big left peak */
		ss = (fun[0]*i)>>1;
		}
	else    f=1;
	}
if( c_ft && nx<5 && n>16 && n<25 && s<<2>ss*6 )
	return(0);    /* knot : small inc, small kegl 't' */

ret_code=0;
if( i==nn && ss*3>s ) /* i - first overlay fun-graphic and 0-level-line */
	{
	for(i=0;i<n && fun[i]; i++); i--;
	if( (i<<2)>n )
		return(0); /* first colunm is zero */
	}
if( s>=ss )
	ret_code=2;        /* big white place */
else if( s*4>=ss*3 || i>0 && s*2>=ss )
	ret_code=1;        /* middle percent of white-place */
if( ret_code==2 )
	{
	int16_t dn=n;
	for(i=n-1;i>=0&&fun[i]==0;i--); i++;
	dn -= i;
	if( f==0 && dn<=n/6 && dn>1 )      /* exist left downer pimple */
		ret_code=3;
	if( n<21 )
		{
		int16_t s;
		for(s=i=0;i<3;i++)
			if( fun[i]!=fun[i+1] )
				s++;
		if( s<1 )
			ret_code=0;
		}
	}

return(ret_code);
}

/* correct befor calculated right and left flags              */
/* if left(i) and right(i+1)  or left(i+1) and right(i) flags */
/* near, then transfer appropriated flag                      */
/*                                                            */
/* flag long > dist , distance of flags<= lev                 */
/* correct characteristics l , r                              */
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

/* analoge fun correct_beam */
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
/*----------------------------------------------------------------------*/
/*  calculate flags and concates for left(typ=1) or right(typ=0) abris	*/
/*  in : abris[0:n-1]-abris array(step of fun-scale = 4),		*/
/*       skip_u,skip_d - upper or downer skip line			*/
/*									*/
/*  out : res->mount[0:4], res->conc[0:4], res->m_meandr ( number of	*/
/*        near mount(pimples)), res->c_meandr(number of near conc)	*/
static void calc_chars (STICK_CHARS *res, int16_t level, int16_t typ,
		      uchar fun[], int16_t n, int16_t skip_u, int16_t skip_d)
{
#define SIZ 6
int16_t	r, l, sr=2, sl=2, sum_r, sum_l;
int16_t	left[5],  l_pos[5], lb_pos[5], le_pos[5];
int16_t	right[5], r_pos[5], rb_pos[5], re_pos[5];
int16_t	lim[SIZ];

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

	/*		      Ё                */
	/*		      Ё                */
	/*     left[]         Ё     right[]    */
	/*	              Ё                */
	/*		      Ё                */
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
/*----------------------------------------------------------------------*/
#ifdef	OLEG_find_peak_OLD
/* in  : *ind1 - first position, fun[0:nn-1] - abris, level - treshold  */
/*      sl - diapason of flat, *extr_fun - first value( fun[*ind1]  )   */
/* out : [*ind2,*ind1] - overlay of peak,                                */
/*       *extr_fun = fun[*extr_pos] - maximum                           */
static void find_peak(int16_t *ind1,int16_t *ind2,uchar fun[],int16_t nn,int16_t level,int16_t sr,
	 int16_t *extr_pos,uchar *extr_fun)
{
int16_t i=*ind1,extr; uchar f=*extr_fun;
extr=*ind2=i++ ;

/* increase fun */
for(;i<nn && fun[i]>level+4 && fun[i]>=fun[i-1]-sr; i++)
	if( f<fun[i] )
		f=fun[(extr=i)];

/* decrease fun */
for(;i<nn && fun[i]>level+4 && fun[i]<=fun[i-1]+sr; i++)
	if( f<fun[i] )
		f=fun[(extr=i)];

*ind1 = i-1;
*extr_pos = extr ;
*extr_fun = f;
return;
}
#endif
/*----------------------------------------------------------------------*/
static void find_peak_new (int16_t *ind1, int16_t *ind2, uchar fun[], int16_t nn,
	int16_t level, int16_t sr, int16_t *extr_pos, uchar *extr_fun)	// MK NEW
{							// 14.01.1993
int16_t	i=*ind1, extr;
uchar	f=*extr_fun;
///int16_t	w_of_max = right0 [i] - left0 [i];	// MK 19.01.1993
///uchar	f_i, w_i;

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
/*----------------------------------------------------------------------*/
static void find_conc_new (int16_t *ind1, int16_t *ind2, uchar fun[], int16_t nn,
	int16_t level, int16_t sl, int16_t *extr_pos, uchar *extr_fun)	// MK NEW
{							// 14.01.1992
int16_t	i=*ind1, extr;
uchar	f=*extr_fun;
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
/*----------------------------------------------------------------------*/
#ifdef	OLEG_find_conc_OLD
/* in  : *ind1 - first position, fun[0:nn-1] - abris, level - treshold  */
/*      sl - diapason of flat, *extr_fun - first value( fun[*ind1]  )   */
/* out : [*ind2,*ind1] - overlay of conc,                               */
/*       *extr_fun = fun[*extr_pos] - minimum                           */
static void find_conc(int16_t *ind1,int16_t *ind2,uchar fun[],int16_t nn,int16_t level,int16_t sl,
	 int16_t *extr_pos,uchar *extr_fun)
{
int16_t i=*ind1,extr; uchar f=*extr_fun;
extr=*ind2=i++;

/* decrease fun */
for(;i<nn && fun[i]<level-4 && fun[i]<=fun[i-1]+sl; i++)
	if( f>fun[i] )
		f=fun[(extr=i)];

/* increase fun */
for(;i<nn && fun[i]<level-4 && fun[i]>=fun[i-1]-sl; i++)
	if( f>fun[i] )
		f=fun[(extr=i)];

*ind1 = i-1;
*extr_pos = extr ;
*extr_fun = f;
return;
}
#endif
/*----------------------------------------------------------------------*/
/* calculate and set  long flags and concates				*/
/* in : fun[0:n-1] - abris array(step of fun-scale = 4 )		*/
/*	level - treshold(study domain abs(fun[i]-level)>4-one step)	*/
/*	lim[0:5] - limits of zones 0,1,2,3,4				*/
/*									*/
/* out:	left[0:4],right[0:4] - left and right long flags		*/
/*		(abs(flag_value-level)					*/
/*	l_pos[0:4],r_pos[0:4] - centers left and right flags		*/
/*	*ll,*rr( sum of long uncorrect flags and concaves		*/
/*----------------------------------------------------------------------*/
static void set_long(uchar fun[],int16_t skip_u,int16_t skip_d,int16_t n,
	      int16_t lev,int16_t lim[], int16_t sl, int16_t sr,
	      int16_t left[],int16_t right[],int16_t l_pos[],int16_t r_pos[],
              int16_t lb_pos[],int16_t le_pos[],int16_t rb_pos[],int16_t re_pos[],
	      int16_t *ll,int16_t *rr)
{
int16_t i,k,m,extr,imax,l,r,nn=n-skip_d; uchar level=(uchar)lev,f;
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
/*----------------------------------------------------------------------*/
/* calculate and set near flags and concaves				*/
/* in :	fun[0:n-1] - abris array(step of fun-scale = 4 )		*/
/*	level - treshold(study domain abs(fun[i]-level)<4-one step)	*/
/*	lim[0:5] - limits of zones 0,1,2,3,4				*/
/*									*/
/* out:	left[0:4],right[0:4] - left and right near flags		*/
/*		(abs(flag_value-level)					*/
/*	l_pos[0:4],r_pos[0:4] - centers left and right flags		*/
/*	*sum_l_near_int,*sum_r_near_int( sum of right near intervals	*/
/*----------------------------------------------------------------------*/
static void set_near ( uchar fun[], int16_t skip_u, int16_t skip_d,
//////		   int16_t n, int16_t lev, int16_t lim[],
		   int16_t n_full, int16_t lev, int16_t lim[],
		   int16_t left[], int16_t right[], int16_t l_pos[], int16_t r_pos[],
		   int16_t lb_pos[], int16_t le_pos[], int16_t rb_pos[], int16_t re_pos[],
		   int16_t *sum_l_near_int, int16_t *sum_r_near_int)
{							// 28.01.1994
				// NOTA BAENE: 28.01.1994 LEMAN call to OLEG;
				// CHANGE  "n"  to  "n_full"  (BBB)
				// and USE NOW  "nn"  ONLY!!!
				// (TAK GOVORIL ZARATUSTRA)
int16_t	i, k, b, e, ex,  nn = n_full - skip_d;
uchar	level=(uchar)lev, f;	// NB: int16_t lev, uchar level;
int16_t	nL, nR;		// 28.01.1994 (OLD l, r);

///for (l=r=0,i=skip_u;i<nn;i++) //this cykl find near mounts for any interval:
for (nL=nR=0,i=skip_u; i<nn; i++)  {			// 28.01.1994
	if ( (f=fun[i])>=level-4 && f<level &&		// FIRST IF;
//////	     (fun[i-1]>=level || i==skip_u) )		// OLEG
	     (i==skip_u || fun[i-1]>=level) )		// 27.01.1994
		{
//////		for (k=i++; i<n && abs(fun[i]-f)<4 && fun[i]<level; i++);
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
//////		for (k=i++; i<n && abs(fun[i]-f)<4 && fun[i]>level; i++);
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
/*----------------------------------------------------------------------*/
/* in : left[0:4],right[0:4] - left and right flags			*/
/*	l_pos[0:4],r_pos[0:4] - centers left and right flags		*/
/*	sum_l,sum_r( sum of left and right near intervals		*/
/*									*/
/* out:	res->mount[0:4], res->conc[0:4], res->m_meandr ( number of	*/
/*	near mount(pimples)), res->c_meandr(number of near conc)	*/

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

	res->conc[i]  = (uchar)compress4(left[i]);
	res->c_pos[i] = (uchar)l_pos[i];
	res->cb_pos[i] = (uchar)cb_pos[i];
	res->ce_pos[i] = (uchar)ce_pos[i];
	}
res->m_meandr = (uchar)sum_r;
res->c_meandr = (uchar)sum_l;
res->main_ear = (uchar)ear  ;

return(1);
}

/*  for v = 4*v1+v2                                                  */
/*  return rez : if v1>0 then                                        */
/*			 if v2<2 theh rez=v/4 else(v2>1) rez=v/4+1   */
/*		  else rez=1                                         */
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


/*  if extrem-interval (imax,i) belong many zones *kk,...,*mm    */
/*  select only one zone                                         */
/*  return : 1 find good zone, 0 - no exist zone                 */

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


/*   return i : lim[i] < v <= lim[i+1]. lim - graphic of increase func   */
static int16_t index_arr(int16_t v,int16_t limit[],int16_t n)
{
int16_t i;
for(i=1; i<n && v>=limit[i]; i++);
return( i-1 );
}

			// here was DEBUG_GRAPH
/*----------------------------------------------------------------------*/
/* set auxiliary signums to struct *signums */
void set_stick_aux_signums (cell *c, uchar left[], uchar right[],
		int16_t dy, int16_t wide_up,
		STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		int16_t dis_left_brace, int16_t dis_right_brace, int16_t dis_slash,
		int16_t l_f_sym, int16_t r_f_sym,
//////		int16_t T_config, int16_t Y_config,
		int16_t T_config, uint16_t T_2_3, uint16_t T_skok,
		B_LINES *bl,int16_t num_lines, int16_t normal_incline)
{
int16_t	wid_up_here=wide_up;	// it was "wid";  BBB: change name 05.08.1993
cell	*cdot;			// 05.08.1993

if( c->row<bl->b2 )  {		/* normal position */
	s->base_2 = s->base_2mk = bl->b2 - c->row;	// 03.01.1994:
	s->base_3 = s->base_3mk = bl->b3 - c->row;
		    s->base_4mk = bl->b4 - c->row;
	if( (s->neck=(uchar)sign_neck(left,right,dy,s->base_2,wid_up_here))>0 )
		s->dis_0xBA = 0;   /* exist neck */
	else  if( l->down_serif==0 && r->down_serif==0 ||
		  l->mount[4]<4 && r->mount[4]<4   )  {
		s->dis_0xBA = 80;
		if (N_eq_Neck >= 2  &&	// 28.07.1993 PROBA for frad6/14
		    s->stick_width > 2  &&		// 29.07.1993
		    (l->mount[1]  &&
		     !r->mount[1]  &&			// 29.07.1993
		     abs(l->mb_pos[1] - s->base_2) < 2  &&
		     abs(l->me_pos[1] - l->mb_pos[1] + 2 - s->stick_width)<2)
		     ||
		    (l->mount[2]  &&
		     !r->mount[2]  &&			// 29.07.1993
		     abs(l->mb_pos[2] - s->base_2) < 2  &&
		     abs(l->me_pos[2] - l->mb_pos[2] + 2 - s->stick_width)<2))
			s->dis_0xBA = 10;	// POPYTKA==!PYTKA ?????????
		}
		/* neck not exist, stick hav'nt down serifs */
	}
else	{
//////	s->base_2 = -1; /* high base line 2 */
	s->base_2 = s->base_3 = -1;	/* high base line 2 */
		// NB: bl->n2 = 0,1 - BASE 2 not exist, 127 - surrealno;
	s->base_2mk = (bl->n2>0) ? (bl->b2 - c->row) : 0x80;
	s->base_3mk = (bl->n3>0) ? (bl->b3 - c->row) : 0x80;
	s->base_4mk = (bl->n4>0) ? (bl->b4 - c->row) : 0x80;
	}					// NB: 0x80 - MK: NOT EXIST;
/*......................................................................*/
	s->t_TOP = (uchar)sign_t_TOP (left, right);	// 09.01.1994
/*......................................................................*/
//////s->dis_up_dot      = (dot_ij(c)!=NULL ? 80 : 0);    /* set */
							// 05.08.1993
//////	s->up_dot_NAVIS = ( c->cg_flag & c_cg_rqdot );	// 26.08.1993 DELETE
	cdot = dot_ij(c);
	if (cdot==NULL)
	    s->dis_up_dot = s->up_dot_H = s->up_dot_W = s->up_dot_dCOL = 0;
	else  {
	    s->dis_up_dot = 80;		// set
	    s->up_dot_H = (uchar)cdot->h;
	    s->up_dot_W = (uchar)cdot->w;
	    s->up_dot_dCOL = cdot->col - c->col;	// Displacement
	    }
/*......................................................................*/
s->dis_slash       = (uchar)dis_slash;                     /* aux */
s->dis_left_brace  = (uchar)dis_left_brace;                /* sig */
s->dis_right_brace = (uchar)dis_right_brace;
s->l_f_symptom     = (uchar)l_f_sym;
s->r_f_symptom	   = (uchar)r_f_sym;
s->T_config	   = (uchar)T_config;
//////s->Y_config        = Y_config;
s->T_2		   = T_2_3 >> 8;
s->T_3		   = T_2_3 & 0xFF;
//////s->T_skok		 = T_skok;
s->T_skok_L	   = T_skok >> 8;		// 17.02.1993
s->T_skok_R	   = T_skok & 0xFF;	// 17.02.1993
s->cut_l	   = ( c->cg_flag & c_cg_cutl );
s->cut_r	   = ( c->cg_flag & c_cg_cutr );

s->num_lines = (uchar)num_lines; /* OLEG:03.03.94. */
s->incline = normal_incline;
/////mkm2 = (l->mb_pos[1]<<8) | l->me_pos[1];  // L.1
/////mkm3 = (l->mb_pos[2]<<8) | l->me_pos[2];  // L.2
/////mkm4 = (r->mb_pos[3]<<8) | r->me_pos[3];  // R.3
/////mkm5 = (r->mb_pos[4]<<8) | r->me_pos[4];  // R.4

return;
}
/*----------------------------------------------------------------------*/
/* study neck area : row=base-1,...,base+2; base-second base line    */
/*                   left[0:n-1],right[0:n-1] - left and right abris */
/*                   mode - width of stick                           */
/* return          : 0 - no neck                                     */
/*		     1 - delicate neck ( enable adding to versions,  */
/*			 disable discrimination 'l','I','1' prob  )  */
/*		     2 - good neck ( enable adding, enable discrim)  */
/*----------------------------------------------------------------------*/
Bool sign_neck (uchar left[], uchar right[], int16_t n, int16_t base, int16_t mode)
{							// 18.02.1993
///int16_t i,s,l,ml,sig_wide=mode>10?1:0,f,e=3;   /* sig_wide = signum of thick */
///int16_t i,l,ml,sig_wide=mode>10?1:0,f;	/* sig_wide = signum of thick */
int16_t	i, l, ml, f;
int16_t	n_less, n_eq;
int16_t	i1, i1m, i2, di;
////int16_t	sig_wide=mode>10?1:0;	// sig_wide = signum of thick	OLEG EDITION;
////int16_t	sig_wide = (mode>10);	// sig_wide = signum of thick	MK EDITION;
int16_t	sig_wide = FALSE;	// 09.01.1994 PROBA DELETE IT;

//////mkma = mode<<8;
//mkm1 = mode<<8;
//////if( sig_wide )	mode >>= 1;		/* thick stick */
if( sig_wide )	mode = (mode>>1) - 1;		/* thick stick */
	  else  if (mode>=6)  mode -= 2;	// CASE 6-10  from 08.02.1993
	  else	mode--;				// NORMAL CASE
//////if( base<3 )  { if( n>12 ) return(0); }	/* too high base */
						// MK 29.01.1993 14:00
if( base<2 )  { if( n>12 ) return(0); };	/* too high base */

/*......................................................................*/
if (!fax1x2)  {					// OLD OLEG: NO FAX
	n /= 3;			// (h/3): limit of neck begin  */
//////	base -= base<5?1:2;	// NB: OLEG used mod.base &zB [base+i]
	i1 = (base<5) ? (base-1) : (base-2);	// (-1,0,+1) or (-2,-1,0)
	if (base<=2)  i1++;	/////////////////////////////////////////////
	i1m = i1 - 1;				// before THREE LINES
	i2 = i1 + 3;		// NOT INCLUDING:  after  THREE LINES
	di = 1;
	}
/*......................................................................*/
else	{					// NEW MK: if FAX
	n = (n+2) / 3;		// (1+h+1)/3: +FAX+ limit of neck begin */
//////	i1 = (base<5) ? (base-2) : (base-4);	// (-2,0,+2) or (-4,-2,0)
	i1 = (base<7) ? (base-2) : (base-4);	// (-2,0,+2) or (-4,-2,0)
	if (base<=2)  i1 += 2;	// before 16.02.1993
//////	i1m = i1 - 2;		// see fax10/12 "application": 't'(-136)=>'k'
//////	i1m = i1 - 1;				// before THREE LINES
	i1m = (i1>2) ? (i1 - 1) : i1;		// before OR NOT of THREE LINES
	i2 = i1 + 6;		// NOT INCLUDING:  after  THREE LINES
//////	i1m = (i1>2) ? (i1 - 1) : i2;		// before THREE LINES or DOUBLE
	di = 2;
	}
/*......................................................................*/
//////mode --;		// see BELOW from 08.02.1993
for(f=-1,ml=mode,n_less=n_eq=0,i=i1;i<i2;i+=di)  {
	l = (right[i]-left[i])>>2;	/* wide c_comp at level (base+i)	*/
	if( sig_wide )	l >>= 1;		/* for wide c_comp	*/
	if( l<mode )	n_less++;		/* num of thin rows	*/
	if( l==mode )	n_eq++;			/* num of rows==mode	*/
	if( l<MAX(1,mode-1) && f==-1 )	f = i;  /* f = first row neck	*/
	if( ml>l )	ml = l;			/* minimum wide of neck	*/
	}
//////mkma |= mode;			// mode.mode'
//////mkmb = f<<8;			//    f.f'
//////mkmc = (n_less<<8) | n_eq;	// less.eq
//////mkmd = ml;			//   00.ml
//////mkme = (i1m<<8) | i2;
//mkm1 |= mode;			// mode.mode'
//mkm2 = f<<8;			//    f.f'
//mkm3 = (n_less<<8) | n_eq;	// less.eq
//mkm4 = ml;			//   00.ml
//mkm5 = (i1m<<8) | i2;
/*......................................................................*/
	N_eq_Neck = n_eq;
/*......................................................................*/
if( mode==1 && ml==0 )				// OLD: NOT CHANGED #####
	{
	if ( f )  /* find begin neck */
		for(; f>1 && right[f]==left[f]; f--); f++;
	return ( f>=n  ||  f-1 > (mode+1)<<1  ?  1  :  2);
	}
/*......................................................................*/
if ( ml < (mode - 2) )	return (3);	// 18.02.1993	NECK=3 !!!!!!!!!!
/*......................................................................*/
					// 16.02.1993 MK: similar 't'
if ( fax1x2 && base<=5 && ml>=(mode-1) &&
     (right[0] - left[0]) < (right[2] - left[2]) &&
     (right[2] - left[2]) < (right[4] - left[4]) )
	return (0);
/*......................................................................*/
if( n_less==0 ) {  // not find neck belong diapason base,base+3 (base correct)

	if( ((right[i2] - left[i2])  >> 2) < mode-1 )	return(1);
	if( ((right[i1m]- left[i1m]) >> 2) < mode-1 )	return(1);

					// 12.02.1993  int32_t NECK:
	if ( n_eq==2 &&			// fax36/9(38) "images"
	     ((right[i1m] - left[i1m]) >> 2) > (mode + 2) &&
	     ((right[i2]  - left[i2])  >> 2) > (mode + 2) )
		return (1);

	return (0);	// MK EDITION
}
/*......................................................................*/
//////if( s>0 )  {	/* exist neck */		// NECK EXIST: (sic!)
	if ( ml == mode-1 ) return (1);	/* width neck = width stick - 1	*/
	else	{			/* width neck > width stick - 1 */
					// MK PROBAS 04.02.1993
//////		if (fax1x2)  f = ( (f-1) & 0xFE ) + 1;	// FAX: 5->5, 6->5;
		if (fax1x2)  f = (f-1) & 0xFE;		// FAX: 5->4, 6->4;
//////mkmb |= (f&0xFF);
//mkm2 |= (f&0xFF);
		if( f>=n-1 ) return (1);  /* low neck  */
			else return (2);  /* high neck */
		}
//////return( s );
}
/*----------------------------------------------------------------------*/
static	Bool  sign_t_TOP (uchar left[], uchar right[])  {	// 26.01.1994
int16_t	d0 = right[0]-left[0];				// see sign_neck ABOVE;
int16_t	d1 = right[1]-left[1];				// NB: MASTAB = 4 !!!
int16_t	d2 = right[2]-left[2];				// 0, 4, 8, 12 are
							// 1, 2, 3,  4 really;
	if (d0<=4  &&			// 1 or 2;
	    d1<=12  &&  d1>d0  &&	// 2, 3, or 4, but more then d0;
	    d2>=d1+8  &&		// SKOK >=2 BETWEEN 1 AND 2;
	    right[2]-right[1]>=left[1]-left[2])  // 26.01.1994: dR>=dL;
		return (TRUE);		// t^

	return (FALSE);			// t~
}
/*----------------------------------------------------------------------*/
/* for 'f',they hav 2 united flags : big hook in 0-zone */
/*         and pimple in 1-zone                         */
void correct_f_symptoms(int16_t hist_int[],int16_t dy,
			       int16_t *l_f_sym,int16_t *r_f_sym)
{
int16_t i,s;
for(i=1;i<dy && hist_int[i]==1;i++);     /* 1 interval  in row */
for(s=0;i<dy && hist_int[i]==2;i++,s++); /* 2 intervals in row */
if( i<dy>>1 && s>3 )
	*r_f_sym = *l_f_sym=2;
return;
}

void calc_skipped_lines(int16_t hist_int[],uchar left[],uchar right[],
			int16_t dy,int16_t dx,int16_t wide,int16_t opt,Bool c_r,
			int16_t *ul,int16_t *ur,int16_t *dl,int16_t *dr)
{
int16_t skip_ul,skip_ur,skip_dl,skip_dr;
if( wide>2 || dx<<3>dy*3 )
calc_of_skip(hist_int,dy,opt,left,right,
	     (int16_t)((opt-(wide<<1)+2)>4),(int16_t)((opt+(wide<<1)-2)<(dx<<2)-4),
	     &skip_ul,&skip_dl,&skip_ur,&skip_dr);
else
	skip_ul=skip_dl=skip_ur=skip_dr=0;
if( c_r)
        {
	if( skip_ul && hist_int[skip_ul-1]>1 )
		skip_ul = 0;
	if( skip_ur && hist_int[skip_ur-1]>1 )
		skip_ur = 0;
	}

*ul = skip_ul;
*ur = skip_ur;
*dl = skip_dl;
*dr = skip_dr;
return;
}


static void calc_of_skip(int16_t hist[],int16_t dy,int16_t opt,
			 uchar left[],uchar right[],
			 int16_t left_mode,int16_t right_mode,
			 int16_t *s_ul,int16_t *s_dl,int16_t *s_ur,int16_t *s_dr)
{
int16_t su,sd;

if( left_mode )
	{
	for( su=0;su<3;su++)
		if( hist[su]==1 && right[su]-left[su]>=4 &&
		    left[su]<opt )
			break;
	for( sd=0;sd<3;sd++)
		if( hist[dy-1-sd]==1 && right[dy-1-sd]-left[dy-1-sd]>=4 &&
		    left[dy-1-sd]<opt )
			break;
	}
else
	{
	for( su=0;su<3;su++)
		if( hist[su]==1 && right[su]-left[su]>=4 )
			break;
	for( sd=0;sd<3;sd++)
		if( hist[dy-1-sd]==1 && right[dy-1-sd]-left[dy-1-sd]>=4 )
			break;
	}

*s_ul = su;
*s_dl = sd;

if( right_mode )
	{
	for( su=0;su<3;su++)
		if( hist[su]==1 && right[su]-left[su]>=4 &&
		    right[su]>opt )
			break;
	for( sd=0;sd<3;sd++)
		if( hist[dy-1-sd]==1 && right[dy-1-sd]-left[dy-1-sd]>=4 &&
		    right[dy-1-sd]>opt )
			break;
	}
else
	{
	for( su=0;su<3;su++)
		if( hist[su]==1 && right[su]-left[su]>=4 )
			break;
	for( sd=0;sd<3;sd++)
		if( hist[dy-1-sd]==1 && right[dy-1-sd]-left[dy-1-sd]>=4 )
			break;
	}

*s_ur = su;
*s_dr = sd;
return;
}
/*----------------------------------------------------------------------*/
static void num_flag_conc (STICK_CHARS *res, int16_t width)
{
int16_t	i, t, sc, sf, n;
int16_t	max_flag=0;
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

res -> num_flags = (uchar)sf;          /* number of flags      */
res -> num_concs = (uchar)sc;          /* number of concaves   */
res -> num_long_flags = (uchar)n;      /* number of long flags */
res -> max_flag = (uchar)max_flag;	// 06.01.1994

return;
}
/*----------------------------------------------------------------------*/
