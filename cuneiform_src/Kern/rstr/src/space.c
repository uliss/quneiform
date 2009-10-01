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
/*		      Space size counting                            */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "func.h"
#include "status.h"
#include "ligas.h"
#include "lang.h"
#include "linutil.h"
//#include "edfile.h"
#include "minmax.h"

//#define NEW_PROBEL

extern int16_t pitchsize;
extern uchar fax1x2;
extern uchar line_tabcell; // OLEG
#define sp_min      897
#define sp_min1     725
#define sp_max      392
#define sp_max1     326

#define lw_min        2
#define lw_aver       8
#define lw_max       11
#define lw_max1      12

#define SMALLKEG     10

#define prob_min     70

#define NBSMIN        5

static uchar dist[300];

// Перенес в функцию space_ligas() в конце модуля 29.08.2000 E.P.
//static uchar  leftlig[]={"ffffrr./fij!?Ў "},
//	     rightlig[]={"ilfiti/.lij!?Ў "};

static int16_t sm,sp,spmin,spmax;
static uint16_t n;
static int32_t H;
static Bool italic;
static int16_t  gap;

/* static void order();*/
static int16_t delta(cell *,cell *,uchar,uchar);
static int16_t vers1(cell *);
static int16_t longtail(cell *);
static int16_t notflatf(cell *,int16_t);
static int16_t ser_g(cell *);
static void insert_space(cell *,cell *,int16_t);

static void space_ligas(uchar *let1, uchar *let2); // 29.08.2000 E.P.

void space_size(int16_t h)
{
 int16_t nh,nh1,i,j,d,pl,sss,dd,sv;
 uint16_t s,ss,s1,n1,n2,x[128],y[128],hist[128];
 int32_t S;
 cell *c1,*c2,*c;
 uchar str[2100],let1,let2,fl;

 snap_newpass('f');
/* order();*/
 gap=0;
 H=(h+((fax1x2)?2:0))*256;
 sm=nh=3*h/2+((fax1x2)?5:2);

 if (pitchsize)
	nh=4*pitchsize/3+2;  // pitchsize - стандартная ширина + пробел между символами
						 // (стандартное расстояние между центрами)

 italic=FALSE;
 c1=cell_f();
 c2=c1->next;
 S=n=0;

 for (i=0; i < 128 && i<=nh; i++)      //AK! add crash guard
	hist[i]=0;

 while (c1=c2,c2=c1->next, c2 && c2->next!=NULL)
 {
	if ((c1->font|c1->font_new)&c_fp_it)
		italic=TRUE;

	if ((c1->font|c1->font_new)&c_fp_str)
		italic=FALSE;

	let1=c1->vers[0].let;

	if (c1->vers[0].prob<=prob_min)
		let1=bad_char;

	let2=c2->vers[0].let;

	if (c2->vers[0].prob<=prob_min)
		let2=bad_char;

// Generic for Central Europe ligas 29.08.2000 E.P.
  if ( language != LANG_RUSSIAN )
      space_ligas(&let1,&let2);

	if ( let1=='_' && !memchr(".,",let2,2) ||
         let2=='_' && !memchr(".,",let1,2) ||
         let1==liga_bull || let2==liga_bull)
		continue;

	if (
		let1==low_quotes  || // Макрос 31.05.2001 E.P.
 	    let2==',' ||
        (language==LANG_ENGLISH || language==LANG_GERMAN  ||
        language==LANG_RUSSIAN) && (let2==':' || let2==';')
		)
		continue;

	if (let1=='$' && let2>='0' && let2<='9')
		continue;

	if (let1=='.' && let2=='.')
		continue;

	if (language==LANG_FRENCH && (let1==left_quocket || let2==right_quocket))
		continue;

	i=c2->left-c1->right;

	if ( (c=c1->prev)->flg!=c_f_fict && c->right>c1->right)
		i=c2->left-c->right;

	d=0;

	if (c1->cg_flag&c_cg_cutr && c2->cg_flag&c_cg_cutl)
		i=0;
	else
	{
		if ( !pitchsize )
		{
			d=delta(c1,c2,let1,let2);
			i+=d/256;
		}
		else
		{
#ifdef NEW_PROBEL
			if (i>=H/sp_min &&
				i * 100 > pitchsize *35 ) // Nick 4.01.2002
#else
			if (i>=H/sp_min && i>=pitchsize/2 )
#endif
			{
				i=c2->left+c2->w/2-(c1->left+c1->w/2);

				if ( memchr("l1I",let1,3) && !((c1->font|c1->font_new)&c_fp_it) &&
				     c1->font&c_fp_gelv && 4*c1->w>c1->h)
					d-=256*c1->w/3;

				if ( memchr("l1I",let2,3) && !((c2->font|c2->font_new)&c_fp_it) &&
					 c2->font&c_fp_gelv && 4*c2->w>c2->h)
					d+=256*c2->w/3;

				if ( (let1=='i' && 5*c1->w>2*c1->h ||
						(let1==liga_i ||
						 language==LANG_TURKISH &&
							(let1==i_sans_accent||let1==II_dot_accent) // 30.05.2002 E.P.
						)&&
					 4*c1->w>c1->h) &&
					 !((c1->font|c1->font_new)&c_fp_it) && c1->font&c_fp_gelv)
					d-=256*c1->w/5;

				if ( (let2=='i' && 5*c2->w>2*c2->h ||
						(let2==liga_i ||
						 language==LANG_TURKISH &&
							(let2==i_sans_accent||let2==II_dot_accent) // 30.05.2002 E.P.
						)&&
					 4*c2->w>c2->h) &&
				     !((c2->font|c2->font_new)&c_fp_it) && c2->font&c_fp_gelv)
					d+=256*c2->w/5;

				if ( let1=='f' && !((c1->font|c1->font_new)&c_fp_it) && c1->font&c_fp_gelv &&
					 2*c1->w>c1->h)
					d+=256*c1->w/5;

				if ( let2=='f' && !((c2->font|c2->font_new)&c_fp_it) && c2->font&c_fp_gelv &&
				     2*c2->w>c2->h)
					d-=256*c2->w/5;
				i+=d/256;
			}
			else
				i=0;
		}
	}
	if (i<0)
		i=0;
	if (i>nh)
		i=nh;

	if ( n > 298 )                       //AK! crash gurd fo dist[300]
		n = 298;

	dist[n++]=(uchar)i;

	if (i<sm) sm=i;


	if (snap_activity('f'))
	{
		if (n==1)
			sprintf(str,"distances:\n");
		else
			if ((n%10)==1)
				sprintf(str+strlen(str),"\n");
		sprintf(str+strlen(str),"%3d(%2d)",i,d/256);
	}

	if (i<nh)
		S+=i;
	if ( i > 127 )
		i = 127;
	hist[i]++;
 }

 if (nh > 127 )                                //AK! add crash guard
	 nh = 127;

 if (sm>h/2)
	 sm=0;

 if (snap_activity('f'))
 {
	snap_newcell(cell_f()->next);
	if (n)
		snap_show_text(str);
	snap_monitor();
  }

 if (snap_activity('f'))
 {
	sprintf(str,"histogramme:\n");
	for (i=0; i<=nh; i++)                                   //AK! add crash guard
	{
		if (i && !(i&15))
			sprintf(str+strlen(str),"\n");
		sprintf(str+strlen(str),"%3u",hist[i]);
	}
	snap_show_text(str);
	snap_monitor();
  }

 if (pitchsize )
 {
#ifdef NEW_PROBEL
	 	// Nick 04.01.2002. pitchsize - стандартная ширина + пробел между символами !
	 sp = pitchsize+MAX(pitchsize/6,2)+1;
#else
	 sp=pitchsize+MAX(pitchsize/6,3)+1;
#endif


	spmin=sp-1;
	spmax=sp+1;

	for( i=spmin;i >= pitchsize;i--)
	{
		if( hist[i] <= 0 )
			break;
	}
	if( hist[i] <= 0 && hist[i+1] > 0 )
	{
		sp = MIN(sp,i+1);
		spmin = MIN(spmin, sp);
	}


	gap=pitchsize/3;
	if (snap_activity('f'))
	{
		sprintf(str,"pitch=%u,space=%u",pitchsize,sp);
		snap_show_text(str);
		snap_monitor();
	}
	return;
 }


 if (n-hist[nh]<=1)
 {
	 s=h;
	 d=0;
	 goto ret1;
 }

 for (i=0; i<=nh; i++)
 {
	 x[i]=i;
	 y[i]=hist[i];
 }

 for (n1=nh+1; n1>2; n1=n2)
	for (pl=n2=i=0; i < 127 && i<n1; i++)                //AK! add crash guard
	{
		if ( ( i==0 || y[i-1] < y[i]) &&
			 (i == n1-1 || y[i+1] < y[i]))
		{
			x[n2]=x[i];
			y[n2]=y[i];
			n2++;
		}

		if ((i==0 || y[i-1]<y[i]) && i<n1-1 && y[i+1]==y[i])
		{
			pl=1;
			j=i;
		}

		if (pl && (i==n1-1 || y[i+1]<y[i]))
		{
			x[n2]=(x[j]+x[i])/2;
			y[n2]=y[i];
			n2++;
		}

		if (i<n1-1 && y[i+1]!=y[i])
			pl=0;
	}
 if (n1==1 || x[1]==nh || x[1]-x[0] < H/sp_min)
	 s=0;
 else
	 s=(x[0]+x[1]+1)/2;

 for (gap=x[0]; gap < 128 && hist[gap] != 0; gap++);
	if (snap_activity('f'))
	{
		sprintf(str,"argmax1=%u",x[0]);
		if (n1==2)
			sprintf(str+strlen(str),", argmax2=%u",x[1]);
		if (s)
			sprintf(str+strlen(str),"\ns=%u",s);
		snap_show_text(str);
		snap_monitor();
	}

 dd= ( language==LANG_FRENCH || language==LANG_GERMAN ||
	   language==LANG_SWEDISH) ? 2 : 0;

 for (nh1=nh-((hist[nh] < NBSMIN) ? 1 : 0),s1=0; s1<2; s1++)
 {
	for (n2=i=0; i<=nh1 && hist[i]<=s1; n2+=hist[i++])
	{}

	if (i>nh1)
	{
		d=0;
		break;
	}

	for (j=nh1; j > 0 &&  hist[j]<=s1; j--)                     //AK! add crash guard
	{}

	if (snap_activity('f'))
		sprintf(str,"level=%u from %u to %u",s1,i,j);

	for (d=0;i > 0 && i < 128 && i<j; n2+=n1*s1)
    {

		for ( ; i > 0 && i < 128 && hist[i]>s1 && i<j; n2+=hist[i++])
		{}

		if (i==j)
			break;

		for (pl=i++,n1=1; hist[i]<=s1; i++,n1++)
		{}

		if (snap_activity('f'))
			sprintf(str+strlen(str),"\nlacune : beg=%u,length=%u",pl,n1);

		if ( d<n1 && n+1>=(n-n2+1)*lw_min &&
		     ((n-n2) && n+1<=(n-n2+1)*(lw_max+dd) ||
			 !(n-n2) && n+1<=lw_max1+dd) && pl-sm<=H/sp_max)
		{
			if (!s && pl==d+ss+1 && (n1==d+1 || d>=3))
				fl=1;
			else
				fl=0;

			d=n1;
			ss=pl;

			if (d>=4 || ss+d-1>H/sp_min && d>=3 || ss>2*H/sp_min && d>=2)
				break;
		}
	}
	if (snap_activity('f'))
	{
		if (d) sprintf(str+strlen(str),"\ns=%u",ss+d/2);
		snap_show_text(str);
		snap_monitor();
	}
	if (d)
		break;
 }
 if ( nh > 127 )
	 nh = 127;
 if ( nh < 0 )
	 nh = 0;

 if (n+1<=(hist[nh]+1)*lw_aver && d==1)
	 d=0;
 if (s)
 {
	if (!d)
	{
		d=1;
		goto cntrl;
	}
  	else
		if (abs(s-(ss+d/2))<=MIN(s,ss+d/2))
		{
			sv=s;
			s=(s+ss+((s>ss)?d/2:0)+1)/2;
			d=3;
			goto cntrl;
		}
 }

 if (d)
 {
	s=ss;
	if (!fl)
	{
		if (d>=3)
			s++;
		if (d>=4)
			s++;
		if (d>=6)
			s++;
	}

	d=2;
	goto cntrl;
 }
 else
	goto avgap;
////////////////////////////////////////////
cntrl:
 sss=(n>=lw_aver) ? sp_min : sp_min1;
 if (s-sm>=H/sss)
	goto ret;
////////////////////////////////////////////
avgap:
 if ( nh > 127 )
	 nh = 127;
 if ( nh < 0 )
	 nh = 0;

 ss=(S*256)/(n-hist[nh]);
 for (S=i=0; i < 128 && i<nh; i++)
 {
	 j=(i*256)-ss;
	 if (j<0)
		 j=-j;
	 S+=(long)j*hist[i];
 }
 s1=S/(n-hist[nh]);

 if (s1<384)
	s1+=256;

 if (s1>ss)
	s1=ss;

 if (snap_activity('f'))
 {
	sprintf(str,"gap : aver=%u,disp=%u",(ss+128)/256,(s1+128)/256);
	snap_show_text(str);
	snap_monitor();
 }

 for (s=(ss+s1+128)/256; s < 128 &&  s<nh; s++)
 {
	if (hist[s]==0)
	{
		d=4;
		break;
	}

	if (hist[s]<hist[s+1])
	{
		d=5;
		break;
	}

	if ((s>h/2) && (hist[s]==hist[s+1]))
	{
		d=6;
		break;
	}
 }

 for (n2=0,i=s;i < 128 && i<=nh; i++)
	 n2+=hist[i];

 for (i=nh-1; !hist[i]; i--)
 {}

 s1=((ss+128)/256+(s1+128)/256+i+1)/2+1;

 if ( n+1<(n2+1)*lw_min && s1>s ||
      (n2 && n+1>(n2+1)*(lw_max+dd) || !n2 && n+1>lw_max1+dd) && s1<s)
 {
	 s=s1;
	 d=7;
 }
/////////////////////////////////////////////
ret:
 sss=(n>=lw_aver)?sp_min:sp_min1;
 if (s-sm<H/sss)
 {
	s=sm+H/sss;
	for (i=s;i < 128 && i > 0 && i<nh && !hist[i]; i++)
	{}

	if (i==nh)
    {
		for (i=s; i > 0 && i < 128 && !hist[i-1]; i--)
		{}

		if (4*hist[i-1]<n)
			s=i;
	}
	d=8;
 }
 sss=(h>SMALLKEG) ? sp_max : sp_max1;

 if (s-sm>H/sss)
 {
	for (n1=0,i=s; i < 128 && i > 0 && i<=nh; i++)
		n1+=hist[i];
	for (n2=0,i=sm+H/sss; i < 128 && i > 0 && i<=nh; i++)
		n2+=hist[i];
	if ( n1 && n+1>(n1+1)*(lw_max+dd) || !n1 && n+1>(lw_max1+dd) ||
         n+1>=(n2+1)*lw_min)
    {
		s=sm+H/sss;
		d=9;
	}
 }
 d*=10;

 if (d>=80)
	 goto ret1;

 if ((d!=30 || sv>s) && !hist[s] && hist[s+1]<hist[s-1] && hist[s-2])
 {
	 s++;
	 d++;
 }
 else
	if ( s > 1 && s < 127 &&
	     (d!=30 || sv<s) && s<nh && !hist[s-1] && hist[s-2]<hist[s] &&
		 hist[s+1] )
	{
	 s--;
	 d+=2;
	}

//////////////////////////////////////////////
ret1:
 if ( s > 0 && s < 127 && nh > 0 && nh < 128 &&
	  hist[s] && hist[nh])
 {
	s++;
	if ( hist[s-1] && hist[s] && !hist[s+1])
		s++;
	d+=5;
 }

 if (snap_activity('f'))
 {
	sprintf(str,"space : size=%u,var=%u",s,d);
	snap_show_text(str);
	snap_monitor();
 }

 spmin=s-1;
 sp=s;
 spmax=s+1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
/*
static void order()
 {
 cell *c1,*c2,c;

 for (c1=(cell_f())->nextl; (c2=c1->nextl)->nextl!=NULL; c1=c2)
  if (c1->left==c2->left && c1->w>c2->w)
   {
   memcpy(&c,c1,sizeof(cell));
   memcpy(c1,c2,sizeof(cell));
   c1->next=c.next; c1->nextl=c.nextl; c1->prev=c.prev; c1->prevl=c.prevl;
   c.next=c2->next; c.nextl=c2->nextl; c.prev=c2->prev; c.prevl=c2->prevl;
   memcpy(c2,&c,sizeof(cell));
   }
 }
*/

#define d_dig          17        // before and after digit
#define d_1            13        // before and after 1
#define d_sl           10        // before and after slash
#define d_ast          10        // before and after asterisk
#define d_it_ast        6        // before asterisk in French italic
#define d_amp          10        // before and after &
#define d_g            26        // before and after g
#define d_Y             5        // before and after Y
#define d_star          6        // before and after *
#define d_add           5        // before and after @
#define d_point_b       4        // before .
#define d_def_b        15        // before -
#define d_sign_b        2        // before ! and ?
#define d_invsign_b     6        // before invers ! and ?
#define d_perc_b        5        // before %
#define d_signit_b      4        // before ? italic
#define d_lbr_b        12        // before (
#define d_rbr_b         8        // before ) and ]
#define d_J_b           4        // before J
#define d_Jcur_b        5        // before J italic
#define d_plus_b        4        // before +
#define d_v_b          17        // before v
#define d_w_b          15        // before w
#define d_y_b          50        // before y
#define d_j_b           4        // before j
#define d_jcur_b        5        // before j italic
#define d_T_b           6        // before T
#define d_f_b           7        // before f
#define d_tligaf_b     21        // before t and ligas fi,fl,...
#define d_fcur_b        3        // before f italic
#define d_bhikmnr_b    30        // before bhikmnr gelvetic not italic
#define d_lowit_b       6        // before lower italic
#define d_Ait_b        21        // before A italic
#define d_git_b         7        // before g italic
#define d_fcur_a        4        // after f italic
#define d_f_a           4        // after f
#define d_r_a          19        // after r
#define d_FP_a         15        // after F and P
#define d_rbr_a         9        // after )
#define d_lbr_a         8        // after ( and [
#define d_heigit_a     10        // after height italic
#define d_vwyit_a      10        // after v,w,y italic
#define d_tit_a        15        // after t italic
#define d_git_a        10        // after g italic
#define d_sign_a        6        // after ! and ?
#define d_invsign_a     2        // after invers ! and ?
#define d_dhimnq_a     30        // after dhimnq gelvetic not italic
#define d_a_A          16        // between small and large
#define d_pnt_A        13        // between punctuation and large
#define d_pn_a         30        // between point and small
#define d_S_1           6        // between $ and lI
#define d_pn_ast       15        // between point and asterisk
#define d_t_t          13        // between t and t
#define d_def_dig      17        // between defis and digit
#define d_f_bhl        10        // between f and b,h,l
#define d_ap_qt         3        // between ' and "
#define d_dig_slsh     20        // between digit and slash
#define d_g_g          15        // between g and g
#define d_cm_dig       15        // between , and digit
#define d_A_VW          8        // between A and V or W
#define d_l_ast        15        // between l and asterisk (french)
#define d_flat         10        // between flats gelvetic not italic
#define d_sign          8        // between ? and !
#define d_rus_lowered  10        // russian italic 'p' 'y'

static int16_t delta(cell *c1,cell *c2,uchar let1,uchar let2)
 {
 cell *c3;
 int16_t d,dd;
 B_LINES bl;
 uchar let0;

 get_b_lines(c1,&bl);
 d=0;
 if (let2=='.' && c2->next->vers[0].let!='.')
  d-=H/d_point_b;
 if (let2=='%')
  {
  if (language!=LANG_FRENCH)
   d-=H/d_perc_b;
  else
   d-=H/(2*d_perc_b);
  }
 if ((let2=='!' || let2=='?') && language!=LANG_FRENCH)
   d-=H/d_sign_b;
 if (let2=='?' && language==LANG_FRENCH)
   d+=H/(4*d_sign_b);
 if (let2=='?' && (c2->font|c2->font_new)&c_fp_it)
  d-=H/d_signit_b;
 if(language!=LANG_RUSSIAN)//05-31-94 12:08pm,Pit
 if ((let2==invers_exm || let2==invers_qm) &&
     !(let1=='"' || let1==left_quocket))
  d+=H/d_invsign_b;
 if (let2=='+' && !memchr(".,",let1,2))
  d-=H/d_plus_b;
 if (let2=='-')
  d-=H/d_def_b;
 if (let1=='f')
  {
  if (!(c1->cpos&ce_p_Ql))
   {
   if (c1->vers[0].let=='f' && (30*c1->w>=13*c1->h || (c1->font|c1->font_new)&c_fp_it))
    d+=(256*c1->w)/d_f_a;
   else
    d+=(128*c1->w)/d_f_a;
   }
  if (memchr("bhl",let2,3))
   d-=H/d_f_bhl;
  if (c1->cpos&ce_p_Ql)
   d+=(256*c1->w)/d_fcur_a;
  }
 if (let2=='f' && c2->cpos&ce_p_Ql)
  {
  if (longtail(c2))
   {
   if (c2->vers[0].let=='f' || c2->vers[0].let==liga_fl)
    {
    if (7*(c2->w)>4*(c2->h))
     d+=H/d_fcur_b;
    else
     {
     if (7*(c2->w)>3*(c2->h))
      d+=3*H/(4*d_fcur_b);
     else
      d+=H/(2*d_fcur_b);
     }
    }
   else
    d+=H/(2*d_fcur_b);
   }
  else
   d+=H/d_lowit_b;
  }
 if (let2=='J' && let1!='.' && let1!=',' &&
	 let1!=low_quotes &&	// Макрос 31.05.2001 E.P.
     let1!=left_quocket && let1!=right_quocket
	 )
  {
  d+=(256*c2->w)/d_J_b;
  if (c2->cpos&ce_p_Ql && (c2->font|c2->font_new)&c_fp_it)
   {
   if (5*(c2->w)>=2*(c2->h))
    d+=H/d_Jcur_b;
   else
    d+=H/(2*d_Jcur_b);
   }
  }
 if ((let1>='0' && let1<='9' && let1!='1' || let1=='O') && let2!='(')
  d-=H/d_dig;
 if ((let2>='0' && let2<='9' && let2!='1' || let2=='O') && let1!=')')
  {
  d-=H/d_dig;
  if (let1=='.' && ((c2->font|c2->font_new)&c_fp_it || italic))
   d-=H/d_dig;
  }
 if (let2=='-' && (let1>='0' && let1<='9' || let1=='O' ||
		   memchr("lIi",let1,3) && vers1(c1)) ||
     let1=='-' && (let2>='0' && let2<='9' || let2=='O' ||
		   memchr("lIi",let2,3) && vers1(c2)))
  d-=H/d_def_dig;
 if (let1=='1' || memchr("lIi",let1,3) && vers1(c1))
  {
  dd=4*(H-(((c1->font|c1->font_new)&c_fp_it)?180:256)*c1->w)/d_1;
  if ((c3=c1->prev)->flg&(c_f_fict|c_f_space) ||
      c1->left-c3->right>dd/256)
   d-=dd+H/d_dig;
  }
 if (let2=='1' || memchr("lIi",let2,3) && vers1(c2))
  {
  dd=4*(H-(((c2->font|c2->font_new)&c_fp_it)?180:256)*c2->w)/d_1;
  if ((c3=c2->next)->flg&(c_f_fict|c_f_space) ||
      c3->left-c2->right>dd/256)
   d-=dd+H/d_dig;
  }
 if (let1==',' && (let2>='0' && let2<='9' && let2!='1' || let2=='O'))
  d-=H/d_cm_dig;
 if (let1=='g' || let2=='g' ||
	 language==LANG_TURKISH && // 27.06.2002 E.P.
		(let1==g_semicircle || let2==g_semicircle)
	)
  d+=H/d_g;
 if (let1=='/')
  d-=H/d_sl;
 if (let2=='/')
  d-=H/d_sl;
 if (let1=='/' && let2>='0' && let2<='9' ||
     let2=='/' && let1>='0' && let1<='9')
  d-=H/d_dig_slsh;
 if (let2=='(')
  {
  if (let1!='.')
   d-=H/d_lbr_b;
  else
   d+=H/d_lbr_b;
  }
 if (let2==')' || let2==']')
  d-=H/d_rbr_b;
 if ((let2=='v' || let2=='V') && !((c2->font|c2->font_new)&c_fp_it))
  {
  if (let1!='.')
   d+=H/d_v_b;
  else
   d+=2*H/(3*d_v_b);
  }
 if ((let2=='w' || let2=='W') &&
     !((c2->font|c2->font_new)&c_fp_it) && !((c1->font|c1->font_new)&c_fp_it) && c1->row+c1->h>=bl.bm)
  d+=H/d_w_b;
 if (let2=='y' && !((c2->font|c2->font_new)&c_fp_it))
  d+=H/d_y_b;
 if (let2=='Y' && !((c2->font|c2->font_new)&c_fp_it))
  {
  if (c1->row>=bl.b2-3 && let1!='.')
   d+=(256*c2->w)/d_Y;
  else
   d+=(256*c2->w)/(3*d_Y);
  }
 if (let2=='T' && !((c2->font|c2->font_new)&c_fp_it) && c1->row>=bl.b2-3)
  d+=(256*c2->w)/d_T_b;
 if (let2=='j')
  {
  d+=(256*c2->w)/d_j_b;
  if ((c2->font|c2->font_new)&c_fp_it)
   d+=(256*c2->w)/d_jcur_b;
  }
 if ((let2=='f' || let2=='t') && !((c2->font|c2->font_new)&c_fp_it))
  {
  if (c2->vers[0].let=='f' && let1!='f' && c2->nextl->vers[0].let!='i' &&
      2*c2->w>c2->h && notflatf(c2,bl.bm))
   d+=(256*c2->w)/d_f_b;
  else
   d+=H/d_tligaf_b;
  }
 if (let1=='$' && memchr("lIi",let2,3) && vers1(c2))
  d-=H/d_S_1;
 if (is_lower(let1) && let1!='l' &&
     (is_upper(let2)&& let2!='I' ||
      let2>='0' && let2<='9' && let2!='1'))
  d+=H/d_a_A;
 if (memchr(".,:;!?",let1,6) && is_upper(let2) && c1->prev!=NULL &&
      is_lower(let0=(c1->prev)->vers[0].let))
  d+=H/d_pnt_A;
 if ((let1=='!' || let1=='?') &&
     !(let2=='"' || let2=='!' || let2=='?' || let2==right_quocket))
  d+=H/d_sign_a;
 if ((let1=='!' || let1=='?') && (let2=='!' || let2=='?'))
  d-=H/d_sign;
 if(language!=LANG_RUSSIAN)//05-31-94 12:09pm,Pit
 if (let1==invers_exm || let1==invers_qm)
  d-=H/d_invsign_a;
 if (c1->font&c_fp_gelv && !((c1->font|c1->font_new)&c_fp_it) &&
     memchr("dhimnquHMN",let1,10) ||
     c1->w<=4 && (c1->flg&c_f_let))
  d-=H/d_dhimnq_a;
 if (c2->font&c_fp_gelv && !((c2->font|c2->font_new)&c_fp_it) &&
     memchr("bhikrmnuFKLP",let2,12) ||
     c2->w<=3 && (c2->flg&c_f_let))
  {
  d-=H/d_bhikmnr_b;
  if (let1=='g' && !((c1->font|c1->font_new)&(c_fp_it|c_fp_ser)) && !italic)
   d-=H/d_dhimnq_a;
  }
 if ((c1->font|c2->font)&c_fp_gelv && !(c1->font&c2->font&c_fp_ser) &&
     !((c1->font|c2->font)&c_fp_it) &&
    memchr("dhijlmnquHIMNU1",let1,15) &&
    memchr("bhiklmnruDEFHIKLMNPRU",let2,21))
  d-=(64*(c1->w+c2->w)+H/2)/d_flat;
 if (let1==')')
  d+=H/d_rbr_a;
 if (let1=='(' || let1=='[')
  d-=H/d_lbr_a;
 if (let1=='r')
  {
  if ((c1->font|c1->font_new)&c_fp_it)
   d+=2*H/d_r_a;
  else
   d+=H/d_r_a;
  }
 if (let1=='F' || let1=='P')
  d+=H/d_FP_a;
 if (let1==0x27 ||
	 let1==low_quotes || // Макрос 31.05.2001 E.P.
	 let2==0x27 || let2=='"' ||
     language==LANG_GERMAN && (let1==right_quocket || let2==left_quocket))
  d-=H/d_ast;
 if ((let1==left_quocket || let2==right_quocket)&&language!=LANG_RUSSIAN)
  d-=2*H/(3*d_ast);
 if (language==LANG_FRENCH && (c1->font|c1->font_new)&c_fp_it && let2==0x27)
  d-=H/d_it_ast;
 if (let1=='.' && (let2==0x27 || let2=='"'))
  d-=H/d_pn_ast;
 if (let1=='.' && is_lower(let2))
  d-=H/d_pn_a;
 if (let1=='&' || let2=='&')
  d+=H/d_amp;
 if (let1=='t' && (c1->font|c1->font_new)&c_fp_it)
  d+=H/d_tit_a;
 if (let1=='t' && let2=='t')
  d+=H/d_t_t;
 if ((c1->font|c1->font_new)&c_fp_it && memchr("dlCFHIJMNSTVWY1578",let1,18) &&
     c2->row+c2->h>bl.bm)
  {
  if (let1=='V')
   d+=2*H/d_heigit_a;
  else
   d+=H/d_heigit_a;
  }
 if ((c1->font|c1->font_new)&c_fp_it && memchr("yvw",let1,3) && c2->row+c2->h>bl.bm)
  d+=H/d_vwyit_a;
 if ((c2->font|c2->font_new)&c_fp_it &&
	 (let2=='p' || let2=='y' && longtail(c2)))
  d+=H/d_lowit_b;
/* if (let1==0x27 && let2=='"' || let1=='"' && let2==0x27)
  d-=H/d_ap_qt;*/
 if ((c1->font|c1->font_new)&c_fp_it &&
		(let1=='g' ||
			language==LANG_TURKISH && let1==g_semicircle // 27.06.2002 E.P.
		) && ser_g(c1))
  d+=H/d_git_a;
 if (let1=='g' && let2=='g')
  d+=H/d_g_g;
 if ( (let2=='g' ||
		language==LANG_TURKISH && let2==g_semicircle // 27.06.2002 E.P.
	  )
	  && longtail(c2)
	)
  {
  if (!((c2->font|c2->font_new)&c_fp_str) && 9*(c2->w)>=8*(c2->h))
   d+=H/d_git_b;
  if ((c2->font|c2->font_new)&c_fp_it)
   {
   if (6*(c2->w)>=5*(c2->h))
    d+=H/d_git_b;
   else
    d+=H/(2*d_git_b);
   }
  }
 if (!(c1->font&c_fp_gelv) && c1->flg&c_f_let && c1->w<=3)
  d-=256;
 if (!(c2->font&c_fp_gelv) && c2->flg&c_f_let && c2->w<=3)
  d-=256;
 if (let1=='A' && memchr("VW9",let2,3))
  {
  if (!((c1->font|c2->font)&c_fp_it))
   d+=H/d_A_VW;
  if ((c1->font|c1->font_new)&(c2->font|c2->font_new)&c_fp_it)
   d-=H/d_A_VW;
  }
 if (let2=='A' && (c2->font|c2->font_new)&c_fp_it)
  d+=H/d_Ait_b;
 if (let1=='*')
  d-=(256*c1->w)/d_star;
 if (let2=='*')
  d-=(256*c2->w)/d_star;
 if (let1=='@' || let2=='@')
  d-=H/d_add;
 if (language==LANG_FRENCH && let1=='l' && let2==0x27)
  d-=H/d_l_ast;
 if( language==LANG_RUSSIAN && ((c2->font|c2->font_new) & c_fp_it) &&
     (let2==(uchar)'а' || let2==(uchar)'г'))
    d += H/d_rus_lowered;

 return d;
 }

static int16_t vers1(cell *c)
 {
 int16_t n,i;

 for (n=c->nvers,i=0; i<n; i++)
  if (c->vers[i].let=='1') return 1;
 return 0;
 }

static int16_t longtail(cell *c)
 {
 puchar r;
 int16_t l,dh,dw,i,j;

 r=save_raster(c);
 l=(c->w+7)/8;
 dh=c->h/3;
 dw=MAX(3,c->w/6);
 for (i=0; i<dh; i++)
  for (j=0; j<dw; j++)
   if (r[i*l+j/8]&(128>>(j%8)))
    return 0;
 return 1;
 }

static int16_t notflatf(cell *c,int16_t bm)
 {
 puchar r;
 int16_t l,i;

 r=save_raster(c);
 l=((c->w+7)/8)*(bm-c->row);
 for (i=0; i<c->w/4; i++)
  if (r[l+i/8]&(128>>(i%8)))
   return 0;
 return 1;
 }

static int16_t ser_g(cell *c)
 {
 puchar r;
 int16_t l,i,j,a1,a2,s;
 uchar b;

 r=save_raster(c);
 l=(c->w+7)/8;
 j=c->w/2;
 b=128>>(j%8);
 j=j/8;
 for (s=a1=i=0; i<c->h; i++)
  {
  a2=r[l*i+j]&b;
  if (!a1 && a2)
   s++;
  a1=a2;
  }
 if (s>=4)
  return 1;
 return 0;
 }

Bool enable_word(cell *c1,cell *c2,uchar *word)
 {
   if( !strcmp("http",word) &&
        c2->nvers && c2->vers[0].let==':' )
        {
        return FALSE;
        }
    if( !strcmp("mail",word) &&
        c2->nvers && c2->vers[0].let==':' )
        {
        return FALSE;
        }
    if( !strcmp("mail:",word) )
        {
        return FALSE;
        }
    if( !strcmp("e-mail",word) &&
        c2->nvers && c2->vers[0].let==':' )
        {
        return FALSE;
        }
    if( !strcmp("e-mail:",word) )
        {
        return FALSE;
        }
   if( !strcmp("htt",word) &&
        c2->nvers && c2->vers[0].let=='p' &&
        c2->next && c2->next->nvers && c2->next->vers[0].let==':' )
        {
        return FALSE;
        }
    if( !strcmp("ht",word)&&
        c2->nvers        && c2->vers[0].let=='t' &&
        c2->nextl->nvers && c2->nextl->vers[0].let=='p' &&
        c2->nextl->next  && c2->nextl->next->nvers && c2->next->vers[0].let==':' )
        {
        return FALSE;
        }
    if( !strcmp("h",word)        &&
        c2->nvers               && c2->vers[0].let=='t' &&
        c2->nextl->nvers        && c2->nextl->vers[0].let=='t' &&
        c2->nextl->nextl->nvers && c2->nextl->nextl->vers[0].let=='p' &&
        c2->nextl->nextl->next  && c2->nextl->nextl->next->nvers && c2->next->vers[0].let==':' )
        {
        return FALSE;
        }
return TRUE;
}

void space_cell()
 {
 int16_t i,n,d;
 cell *c1,*c2;
 uchar let1,let2,word[300],*wrd=&word[0];

 c1=cell_f();
 c2=c1->next;
 n=0;
 *wrd=0;
 while (c1=c2,c2=c1->next,c2 && c2->next!=NULL)
  {
  let1=c1->vers[0].let;
  if (c1->vers[0].prob<=prob_min)
   let1=bad_char;

  if( wrd < &word[299] ) // Nick 30.05.2001
    *wrd++ = to_lower(let1);

  let2=c2->vers[0].let;
  if (c2->vers[0].prob<=prob_min)
   let2=bad_char;

  /*if((c1->cg_flag & c_cg_cutr) && (c2->cg_flag & c_cg_cutl))
  { n++;    continue; } // Vald if were cut no spaces
    */

  if (!pitchsize )
   {
   if (let1==low_quotes || // Макрос 31.05.2001 E.P.
	   let2==',' ||
       (language==LANG_ENGLISH || language==LANG_GERMAN || language==LANG_RUSSIAN) &&
       (let2==':' || let2==';'))
    continue;
   if (let1=='$' && let2>='0' && let2<='9')
    continue;
   if (let1=='.' && let2=='.')
    continue;
   if (let1=='_' && !memchr(".,",let2,2) ||
       let2=='_' && !memchr(".,",let1,2) ||
       let1==liga_bull || let2==liga_bull)
    {
    insert_space(c1,c2,0);
    wrd=&word[0];
    *wrd=0;
    continue;
    }
   if (language==LANG_FRENCH && (let1==left_quocket || let2==right_quocket))
    {
    insert_space(c1,c2,0);
    *wrd=0;
    continue;
    }
   i=dist[n++];
   if( (c1->flg&(c_f_let|c_f_solid)) && let2==right_quocket ||
       (c2->flg&(c_f_let|c_f_solid)) && let1==left_quocket )
    {
    continue;
    }
   *wrd=0;
   if( !enable_word(c1,c2,word) )
    continue;

   if(i>=sp && i<spmax )
    {
    insert_space(c1,c2,1);
    wrd=&word[0];
    *wrd=0;
    }

   if (i>=spmax)
    {
    insert_space(c1,c2,0);
    wrd=&word[0];
    *wrd=0;
    }

   if (i>=spmin && i<sp)
   {
    if( i>2 || !line_tabcell ) // OLEG
        {
        insert_space(c1,c2,-1);
        wrd=&word[0];
        *wrd=0;
        }
    }
   }
  else
  {
#ifdef NEW_PROBEL
   if ((d=c2->left-c1->right)>=H/sp_min &&
		d * 100 > pitchsize * 35 ) // Nick 4.01.2002
#else
   if ((d=c2->left-c1->right)>=H/sp_min &&
	     d>=pitchsize/2)
#endif
   {
    i=c2->left+c2->w/2-(c1->left+c1->w/2);
    if(language==LANG_RUSSIAN)
	{
        int16_t distance;
        //if (memchr("л›ий™оћ¦†",let2,10) && c2->w>c2->h) i-=c2->w/3;
        if (isletter(let2) && let2>128 && c2->w>c2->h)
			i-=c2->w/3;
        distance=c2->col-c1->col-c1->w;
        if (14*distance<6*c1->w && 14*distance<6*c2->w)
			i-=c2->w/4;
    }

    if (memchr("l1I",let1,3) && !((c1->font|c1->font_new)&c_fp_it) &&
	c1->font&c_fp_gelv && 4*c1->w>c1->h)
     i-=c1->w/3;
    if (memchr("l1I",let2,3) && !((c2->font|c2->font_new)&c_fp_it) &&
	c2->font&c_fp_gelv && 4*c2->w>c2->h)
     i+=c2->w/3;
    if ((let1=='i' && 5*c1->w>2*c1->h ||
			(let1==liga_i ||
			 language==LANG_TURKISH &&
				(let1==i_sans_accent||let1==II_dot_accent) // 30.05.2002 E.P.
			)&& 4*c1->w>c1->h) &&
	!((c1->font|c1->font_new)&c_fp_it) && c1->font&c_fp_gelv)
     i-=c1->w/5;
    if ((let2=='i' && 5*c2->w>2*c2->h ||
			(let2==liga_i ||
			 language==LANG_TURKISH &&
				(let2==i_sans_accent||let2==II_dot_accent) // 30.05.2002 E.P.
			)&& 4*c2->w>c2->h) &&
	!((c2->font|c2->font_new)&c_fp_it) && c2->font&c_fp_gelv)
     i+=c2->w/5;
    if (let1=='f' && !((c1->font|c1->font_new)&c_fp_it) && c1->font&c_fp_gelv &&
	2*c1->w>c1->h)
     i+=c1->w/5;
    if (let2=='f' && !((c2->font|c2->font_new)&c_fp_it) && c2->font&c_fp_gelv &&
	2*c2->w>c2->h)
     i-=c2->w/5;
    if (let2=='[' || let2=='(')
     i-=pitchsize/8;

#ifdef NEW_PROBEL
	if(  let1 == ',' && let2 >= 'A' )
		i += pitchsize/8;  // Nick 4.01.2002
#endif
   }
   else
	   i=0;

   if (memchr(",:;",let2,3) || let1=='$' && let2>='0' && let2<='9' ||
       let1=='.' && let2=='.')
    i-=sp/2;
   if (let1=='_' || let2=='_' || let1==liga_bull || let2==liga_bull)
    i=MAX(i,sp);

   if (i>=sp && i<spmax)
    {
    insert_space(c1,c2,1);
    wrd=&word[0];
    *wrd=0;
    }
   else if (i>=spmin && i<sp)
    {
    insert_space(c1,c2,-1);
    wrd=&word[0];
    *wrd=0;
    }
   else if (i>=spmax)
    {
    n=(i+pitchsize/2)/pitchsize-1;
    if (!n) n=1;
    for ( ; n; n--,c1=c1->next)
        {
        insert_space(c1,c2,0);
        wrd=&word[0];
        *wrd=0;
        }
    }
   }
  }
 if (snap_activity('f'))
  {
  snap_newcell((cell_f())->next);
  snap_show_text("spacing end");
  snap_monitor();
  }
 }

void insert_space(cell *c1,cell *c2,int16_t mode)
 {
 cell *c;

if( p2_active  )
    {
    if( (c1->flg_new&CSTR_fn_val2odd) && (c2->flg_new&CSTR_fn_val2odd)||
        (c2->flg_new&CSTR_fn_val2even) && (c1->flg_new&CSTR_fn_val2even) )
        return;
    }

 c=new_cell();
 //memset(c,0,sizeof(cell)); // Valdemar 11-20-95 05:46pm
 c->row=c->col=c->r_row=c->r_col=c->w=c->h=0;
 c->flg=c_f_space;
 c->flg_new = 0;
 c1->next=c2->prev=c;
 c->prev=c1; c->next=c2; c->nvers=1;
 switch (mode)
  {
  case 0:
   c->vers[0].let=' ';
   c->vers[0].prob=254;
   break;
  case 1:
   c->vers[0].let=SS_POS_HALF_SPACE;
   c->vers[0].prob=150;
   break;
  case -1:
   c->vers[0].let=SS_NEG_HALF_SPACE;
   c->vers[0].prob=75;
  }
 c->vers[1].let=0;
 c->reasno=0;
 }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void cont_space()
 {
 cell *c1,*c2,*c3,*c4,*c5;
 uchar let1,let2,let3,let5;
 int16_t h,i,d,dd,d1,d2,cnt,ss;
 uchar str[80],word[300],*wrd=&word[0];
 *wrd=0;
 if (pitchsize)
	 return;
 if (snap_activity('f'))
 {
	  snap_newcell((cell_f())->next);
	  snap_show_text("post-context spacing begin");
	  snap_monitor();
 }

 h=(int16_t)get_size();
 ss=sm+H/((n>=lw_aver)?sp_min:sp_min1);

 if (sp<ss)
 {
	 spmin=ss-1;
	 sp=ss;
	 spmax=ss+1;
 }

for ( cnt=0,c1=(cell_f())->next;
      c1 != cell_l() && c1->next != cell_l();
      c1=c1->next)
{
	c2 = c1->next;
	if ( c2 == cell_l() )
		break;

	let1=c1->vers[0].let;
	let2=c2->vers[0].let;

	if ( c1->vers[0].prob <= prob_min )
		let1=bad_char;

    if( wrd < &word[299] ) // Nick 30.05.2001
		*wrd++=to_lower(let1);

	if (c2->vers[0].prob<=prob_min)
		let2=bad_char;

    space_ligas(&let1,0);       // 29.08.2000 E.P.

    *wrd=0;
    if( !enable_word(c1,c2,word) )
     continue;

    space_ligas(0,&let2);       // 29.08.2000 E.P.

	if ( memchr("lIitf/",let1,6) && !memchr(" ,:;",let2,4) ||
         memchr("lIitf/",let2,6) && let1!=' ')
    {
		if (let1==0x1f || let1==0x1e)
		{
			c1=del_cell(c1);
			let1=c1->vers[0].let;

			if (c1->vers[0].prob<=prob_min)
				let1=bad_char;

			space_ligas(&let1,0);       // 29.08.2000 E.P.

		}

		if (let2==0x1f || let2==0x1e)
		{
			c2=(del_cell(c2))->next;
			let2=c2->vers[0].let;

			if (c2->vers[0].prob<=prob_min)
				let2=bad_char;

			space_ligas(0,&let2);       // 29.08.2000 E.P.
		}

		i=c2->left-c1->right;

		if(c1->prev)                          //AK add for case of p == NULL
		{
			if ( !((c3=c1->prev)->flg&(c_f_fict|c_f_space)) &&
			     c3->right>c1->right)
				i=c2->left-c3->right;
		}

		d=0;

		if ( c1->cg_flag&c_cg_cutr && c2->cg_flag&c_cg_cutl )
			i=0;
		else
		{
			d=delta(c1,c2,let1,let2);

			if (memchr("lIi",let1,3) && vers1(c1))
			{
				dd=4*(H-(((c1->font|c1->font_new)&c_fp_it)?180:256)*c1->w)/d_1;

				if ( (c3=c1->prev)->flg&(c_f_fict|c_f_space) ||
				     c1->left-c3->right>dd/256 )
					d+=dd+H/d_dig;

				if (let2=='-')
					d+=H/d_def_dig;
			}

			if (memchr("lIi",let2,3) && vers1(c2))
			{
				dd=4*(H-(((c2->font|c2->font_new)&c_fp_it)?180:256)*c2->w)/d_1;
				if ( (c3=c2->next)->flg&(c_f_fict|c_f_space) ||
					 c3->left-c2->right>dd/256)
					d+=dd+H/d_dig;

				if (let1=='-')
					d+=H/d_def_dig;

				if (let1==',')
					d+=H/d_cm_dig;
			}
		}
		i+=d/256;

		if (snap_activity('f'))
		{
			snap_newcell(c1);
			sprintf(str,"dist=%d(%d), spsize=%d",i,d/256,sp);
			snap_show_text(str);
			snap_monitor();
		}

		if (i>=spmax)
            {
			insert_space(c1,c2,0);
            wrd=&word[0];
            *wrd=0;
            }

		if (i>=sp && i<spmax)
            {
			insert_space(c1,c2,1);
            wrd=&word[0];
            *wrd=0;
            }

		if (i>=spmin && i<sp)
            {
			insert_space(c1,c2,-1);
            wrd=&word[0];
            *wrd=0;
            }

		if (i>=spmin)
		{
			cnt++;
			c1=c1->next;
		}
	}
}

if( cell_f()->next && ((cell_f())->next)->next )
for  ( c2=((cell_f())->next)->next;
       c2 != cell_l() && c2->next != cell_l();
	   //c2->next!=NULL && (c2->next)->next!=NULL;                      //AK!
	   c2=c2->next)
	if (c2->vers[0].let=='"')
	{
		if ( (c1=c2->prev)->flg&c_f_punct ||
		     c1->flg&c_f_space && c1->vers[0].let!=0x1e  ||
		     c1->flg&c_f_let && c1->vers[0].let=='(')
			continue;

		if ( (c3=c2->next)->flg&c_f_punct ||
		     c3->flg&c_f_space && c3->vers[0].let!=0x1e ||
		     c3->flg&c_f_let &&
		     ((let3=c3->vers[0].let)==')' || let3=='?' ||
		     let3=='!' || let3==liga_exm || let3==liga_qm))
			continue;

		if ( !(c1->flg&c_f_space) )
			d1=c2->left-c1->right;
		else
			d1=c2->left-c1->prev->right;

		if ( !(c3->flg&c_f_space) )
			d2=c3->left-c2->right;
		else
			d2=(c3->next)->left-c2->right;

		if (snap_activity('f'))
		{
			snap_newcell(c2);
			sprintf(str,"distleft=%d, distright=%d",d1,d2);
			snap_show_text(str);
			snap_monitor();
		}

		if (3*MAX(d1,d2)<2*spmin)
			continue;

		if (d1>d2)
		{
			if (c1->flg&c_f_space)
				c1=del_cell(c1);
			insert_space(c1,c2,1);
            wrd=&word[0];
            *wrd=0;
			cnt++;
		}
		else
		{
			if (c3->flg&c_f_space)
				c3=del_cell(c3)->next;
			insert_space(c2,c3,1);
            wrd=&word[0];
            *wrd=0;
			cnt++;
		}
	}

for ( c2=(cell_f())->next; /*c2->next!=NULL*/c2 != cell_l(); c2=c2->next)
	if (c2->vers[0].let=='"')
	{
		if ((c1=c2->prev)->prev==NULL)
			d1=1000;
		else
		{
			if ((let1=c1->vers[0].let)!=' ' && let1!=0x1f /*&& let1!=0x1e*/)
				continue;
			else
			{
				c1=c1->prev;
				if (c1->vers[0].let==0x27)
					continue;
				d1=c2->left-c1->right;
			}
		}

		if ((c3=c2->next)->next==NULL)
			d2=1000;
		else
		{
			if ((let3=c3->vers[0].let)!=' ' && let3!=0x1f /*&& let3!=0x1e*/)
				continue;
			else
			{
				c3=c3->next;
				if (c3->vers[0].let==0x27)
					continue;
				d2=c3->left-c2->right;
			}
		}

		if (snap_activity('f'))
		{
			snap_newcell(c2);
			sprintf(str,"distleft=%d, distright=%d",d1,d2);
			snap_show_text(str);
			snap_monitor();
		}

		if (2*MIN(d1,d2)>3*sp)
			continue;

		if (d1<d2)
			del_cell(c2->prev);

		if (d1>d2)
			del_cell(c2->next);
	}

for ( c1=(cell_f())->next;
      c1 != cell_l();
      //(c2=c1->next)!=NULL && (c3=c2->next)!=NULL && (c4=c3->next)!=NULL;
	  c1=c1->next )
{
	c2 = c1->next;
	if ( c2 == cell_l() )
		break;
	c3 = c2->next;
	if ( c3 == cell_l() )
		break;
	c4 = c3->next;
	if ( c4 == cell_l() )
		break;

	if ( ((let1=c1->vers[0].let)==' ' ||
		 let1==0x1f || let1==0x1e) &&
         c2->vers[0].let=='.' && c3->vers[0].let=='.'
		 && c4->vers[0].let=='.' &&
         c2->left-c1->prev->right<=(4*(c3->left-c2->right))/3)
		c1=del_cell(c1);
}

for ( c1=(cell_f())->next;
      c1 != cell_l();
//      (c2=c1->next)!=NULL && (c3=c2->next)!=NULL && (c4=c3->next)!=NULL;
	  c1=c1->next)
{
	c2 = c1->next;
	if ( c2 == cell_l() )
		break;
	c3 = c2->next;
	if ( c3 == cell_l() )
		break;
	c4 = c3->next;
	if ( c4 == cell_l() )
		break;

	if ( ((let1=c4->vers[0].let)==' ' || let1==0x1f || let1==0x1e) &&
         c1->vers[0].let=='.' && c2->vers[0].let=='.' && c3->vers[0].let=='.')
	{
		if ( (c5=c1->prev)->flg&c_f_fict ||
             (let1=c5->vers[0].let)==' ' || let1==0x1f)
		{
			d=0;
            c5=c4->next;
			if ( (c5->font|c5->font_new)&c_fp_str )
			{
				if ((let5=c5->vers[0].let)=='T')
					d=c5->w/d_T_b;
				if (let5=='v' || let5=='V')
					d=H/d_v_b;
				if (let5=='w' || let5=='W')
					d=H/d_w_b;
			}
			if (c5->left+d-c3->right<=(3*(c3->left-c2->right))/4)
				del_cell(c4);
		}
	}
}

for ( c1=(cell_f())->next;
      c1 != cell_l();
//      (c2=c1->next)!=NULL && (c3=c2->next)!=NULL && (c4=c3->next)!=NULL;
	  c1=c1->next)
{
	c2 = c1->next;
	if ( c2 == cell_l() )
		break;
	c3 = c2->next;
	if ( c3 == cell_l() )
		break;
	c4 = c3->next;
	if ( c4 == cell_l() )
		break;

	if ( c1->vers[0].let=='.' && c2->vers[0].let=='.' && c3->vers[0].let=='.')
		if ( memchr("TVW",(let1=c4->vers[0].let),3) && (c4->font|c4->font_new)&c_fp_str )
		{
			if (let1=='T')
				d=c4->w/d_T_b;
			if (let1=='V')
				d=H/d_v_b;
			if (let1=='W')
				d=H/d_w_b;
			if (c4->left-c3->right+d>c3->left-c2->right)
			{
				insert_space(c3,c4,1);
                wrd=&word[0];
                *wrd=0;
				cnt++;
			}
		}
}

for ( c1=(cell_f())->next;
	  c1 != cell_l();
//      (c2=c1->next)!=NULL && (c3=c2->next)!=NULL && (c4=c3->next)!=NULL;
	  c1=c1->next)
{
	c2 = c1->next;
	if ( c2 == cell_l() )
		break;
	c3 = c2->next;
	if ( c3 == cell_l() )
		break;
	c4 = c3->next;
	if (c4 == cell_l() )
		break;

	if ( c1->vers[0].let==(uchar)'.' && c4->vers[0].let==(uchar)'.' &&
         ((let2=c2->vers[0].let)==0x1f || let2==0x1e ||
         let2==(uchar)' ' && c3->left-c1->right<h/2) &&
         c3->flg&(c_f_let+c_f_bad) )
		del_cell(c2);
}

for (c1=(cell_f())->next; c1 != cell_l() && c1->next != cell_l(); c1=c1->next)
{
	c2 = c1->next;
	if ( c2 == cell_l() )
		break;

	if ( c1->vers[0].let == (uchar)'-' &&
		 c2->flg&c_f_space &&
		 c2->vers[0].let != (uchar)' ')
	{
		del_cell(c2);
		continue;
	}

	if ( c1->flg&c_f_space &&
		 c1->vers[0].let != (uchar)' ' &&
		 c2->vers[0].let == (uchar)'-' )
		c1=del_cell(c1);
}

for (c1=(cell_f())->next; c1 != cell_l() && c1->next != cell_l(); c1=c1->next)
{
	c2 = c1->next;

	if ( c1->vers[0].let=='f' && !(c1->cg_flag&c_cg_cutr) &&
         c2->vers[0].let=='l')
	{
		insert_space(c1,c2,-1);
        wrd=&word[0];
        *wrd=0;
		cnt++;
	}
}

if (snap_activity('f'))
{
	snap_newcell((cell_f())->next);
	snap_show_text("post-context spacing end");
	snap_monitor();
}

if (cnt)
	context_proc_line();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void showgap(cell *CL)
{
	memcpy(dist,CL,1);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// call after space_size(get_size());
int16_t get_space_size(void)
{
	return sp;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void set_space_size(int16_t spn)
{
	char str[80];

	sp=spn;
	spmin=sp-1;
	spmax=sp+1;

	if (snap_activity('f'))
	{
		sprintf(str,"R&E restore russian space size =%d,spmin=%d,spmax=%d",
				sp,spmin,spmax);
		snap_show_text(str);
		snap_monitor();
	}
		return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
int16_t get_gap(void)
{
	return gap;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void space_valid_words(void)
{
cell *c=cell_f()->nextl;
for(;c!=cell_l();c=c->nextl)
    {
    if( c->next!=c->nextl && c->next==c->nextl->prev &&
        (c->next->flg&c_f_punct) &&
        (c->flg&c_f_let) && (c->nextl->flg&c_f_let) &&
         (c->flg&c_f_solid) && (c->nextl->flg&c_f_solid))
        { // let1->>punct-->>let2
        if( !strchr("(-",c->next->vers[0].let) &&
            !(c->next->vers[0].let=='.' &&
              c->nextl->next && c->nextl->next->nvers &&
              c->nextl->next->vers[0].let=='.' ) &&

			// Англ. "Let's" и т.п. 04.09.2001 E.P.
			!(language!=LANG_RUSSIAN &&
			  c->next->vers[0].let=='\'' &&
			  c->nextl->next && c->nextl->next->nvers &&
              c->nextl->next->vers[0].let==' ' ) &&

			  // Француз. "C'est" и т.п. 11.03.2002 Nick
			!(
			  ( language == LANG_FRENCH ||

				// В турецком встречается апостроф
				// в середине слова 18.06.2002 E.P.
			    language == LANG_TURKISH
				)
			  &&
			  c->next->vers[0].let=='\'' )

		  )
            insert_space(c->next,c->nextl,1);
        }
    if( !(c->nextl->flg&c_f_fict) &&
        c->next==c->nextl &&
        (c->flg&c_f_let) && (c->flg&c_f_solid) &&
        (c->nextl->flg&c_f_let) && c->nextl->nvers>0 &&
//        !strchr("(\xd7",c->vers[0].let) &&
//        strchr("(\xd7",c->nextl->vers[0].let) &&
        c->vers[0].let != left_quocket &&		// 04.09.2001 E.P.
        c->nextl->vers[0].let==left_quocket &&	// 04.09.2001 E.P.

        c->nextl->col-(c->col+c->w)>c->nextl->h)
        {
        insert_space(c,c->nextl,1);
        }
    }

for(c=cell_f()->nextl;c!=cell_l();c=c->nextl)
    {
    if( (c->flg&c_f_space)&&(c->flg_new&c_fn_space) )
        {
        c->vers[0].let  = ' ';
        c->vers[0].prob = 254;
        }
    }
return;
}

//
static void space_ligas(uchar *let1, uchar *let2) // 29.08.2000 E.P.
{
  uchar c1=(let1)?(*let1):0,
       c2=(let2)?(*let2):0;

static uchar  leftlig[]={"ffffrr./fij!?ба"},
            rightlig[]={"ilfiti/.lij!?ба"};

static uchar  leftlig_cen[]={"ff"},
            rightlig_cen[]={"fl"};

static uchar rightlig_baltic[]={"ilfiti/.lrj!?"};

static uchar rightlig_turkish[]={"ilfiti/.lrj!?"};

  // Different codes for some ligas in Central Europe languages.
  if (is_cen_language(language))
    {
	if (let1)
		{
		if ( c1>=ligas_beg_latin && c1 <=ligas_end_latin)
		   *let1=rightlig_cen[c1-ligas_beg_latin];

		else if (is_ligas_latin(c1) && is_ligas_lat(c1))
		   *let1=rightlig[c1-ligas_beg];

		else if (c1==POLISH_l)              // 24.20.97
		   *let1='f';

		else if (c1==POLISH_a || c1==POLISH_e)
		   *let1='g';

		// 08.09.2000 E.P.
		else if ( c1 == liga_i)
			*let1='i';

		else if ( c1 == liga_exm)
			*let1='!';

		else if (c1==t_bottom_accent)        // 12.09.2000 E.P.
		   *let1=c1;

		// Заменить букву с акцентом на букву без акцента 12.09.2000 E.P.
		else if ( ( (accent_tab[c1] & ~ACC_SUPERUP)
					|| is_cen_bottom_accent(c1) ) &&
				let_sans_acc[c1] && let_sans_acc[c1] != c1
				)
					*let1 = let_sans_acc[c1];
		}

	if(let2)
		{

		if ( c2>=ligas_beg_latin && c2 <=ligas_end_latin)
		   *let2=leftlig_cen[c2-ligas_beg_latin];

		else if (is_ligas_latin(c2) && is_ligas_lat(c2))
		   *let2= leftlig[c2-ligas_beg];

		else if (c2==POLISH_l)              // 24.20.97
		   *let2='f';

		else if (c2==POLISH_a || c2==POLISH_e)
		   *let2='g';

		// 11.09.2000 E.P.
		else if ( c2 == liga_i)
			*let2='i';

		else if ( c2 == liga_exm)
			*let2='!';

		else if (c2==t_bottom_accent)        // 12.09.2000 E.P.
		   *let2=c2;

		// Заменить букву с акцентом на букву без акцента 12.09.2000 E.P.
		else if ( ( (accent_tab[c2] & ~ACC_SUPERUP)
					|| is_cen_bottom_accent(c2) ) &&
				let_sans_acc[c2] && let_sans_acc[c2] != c2
				)
					*let2 = let_sans_acc[c2];
		}
    }	// Конец центральноевропейских языков

  // Балтийские языки 10.07.2001 E.P.
  else if (is_baltic_language(language))
    {
	if (let1)
		{
		if ( c1>=ligas_beg_baltic && c1 <=ligas_end_baltic)
		   *let1=rightlig_baltic[c1-ligas_beg_baltic];

		else if ( c1 == liga_i)
			*let1='i';

		else if ( c1 == liga_exm)
			*let1='!';

		// Заменить букву с акцентом на букву без акцента 12.09.2000 E.P.
		else if ( ( (accent_tab[c1] & ~ACC_SUPERUP)
					|| is_cen_bottom_accent(c1) ) &&
				let_sans_acc[c1] && let_sans_acc[c1] != c1
				)
					*let1 = let_sans_acc[c1];
		}

	if(let2)
		{
		if ( c2>=ligas_beg_baltic && c2 <=ligas_end_baltic)
		   *let2=rightlig_baltic[c2-ligas_beg_baltic];

		else if ( c2 == liga_i)
			*let2='i';

		else if ( c2 == liga_exm)
			*let2='!';

		// Заменить букву с акцентом на букву без акцента 12.09.2000 E.P.
		else if ( ( (accent_tab[c2] & ~ACC_SUPERUP)
					|| is_cen_bottom_accent(c2) ) &&
				let_sans_acc[c2] && let_sans_acc[c2] != c2
				)
					*let2 = let_sans_acc[c2];

		}
    }

  // Турецкий язык 30.05.2002 E.P.
  else if (is_turkish_language(language))
    {
	if (let1)
		{
		if ( c1>=ligas_beg_turkish && c1 <=ligas_end_turkish)
		   *let1=rightlig_turkish[c1-ligas_beg_turkish];

		else if ( c1 == i_sans_accent)
			*let1 = 'i';

		// Заменить букву с акцентом на букву без акцента 12.09.2000 E.P.
		else if ( ( (accent_tab[c1] & ~ACC_SUPERUP)
					|| is_turkish_bottom_accent(c1) ) &&
				let_sans_acc[c1] && let_sans_acc[c1] != c1
				)
					*let1 = let_sans_acc[c1];
		}

	if(let2)
		{
		if ( c2>=ligas_beg_turkish && c2 <=ligas_end_turkish)
		   *let2=rightlig_turkish[c2-ligas_beg_turkish];

		else if ( c2 == i_sans_accent)
			*let2 = 'i';

		// Заменить букву с акцентом на букву без акцента 12.09.2000 E.P.
		else if ( ( (accent_tab[c2] & ~ACC_SUPERUP)
					|| is_turkish_bottom_accent(c2) ) &&
				let_sans_acc[c2] && let_sans_acc[c2] != c2
				)
					*let2 = let_sans_acc[c2];

		}
    }

  else  // Rest of languages
    {
	if (let1 && c1>=ligas_beg && c1<=ligas_end)
       *let1=rightlig[c1-ligas_beg];

    if (let2 && c2>=ligas_beg && c2<=ligas_end)
       *let2= leftlig[c2-ligas_beg];
    }

}

///////////////////////end of file////////////////////////////////////////////////////////////////////////////////
