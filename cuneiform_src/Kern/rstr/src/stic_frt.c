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
/*	STIC_FRT.C	28.01.1994	from STIC_DIS.C			*/
/*----------------------------------------------------------------------*/
#define	mk_80_for_CUT	80
/*----------------------------------------------------------------------*/



  #include <stdlib.h>
  #include <string.h>

  #include "func.h"
  #include "stick.h"
  #include "ligas.h"
  #include "tuner.h"
#include "minmax.h"
extern uchar multy_language;

extern int16_t pitchsize ;

extern int16_t nIncline  ;
extern uchar fax1x2;	// MK NEW 05.01.1993
extern int  inc_num_EEM;	// in ST_TOOLS.C
extern int  dis_LIMIT_EEM;	// in ST_TOOLS.C;
/*......................................................................*/
#ifdef	MKPRINT_ENABLE				// MK OTLADKA Variables
extern uint16_t	mkm1, mkm2, mkm3, mkm4, mkm5;
extern uint16_t	mkma, mkmb, mkmc, mkmd;
///extern uint16_t	d,d0,d1,d2,d3,d4,d5,d6,d7,d8,d9;
#endif
/*......................................................................*/
extern	uint16_t	left_mode_EEM;	// NOTA BENE:  NEPORJADOK; see ST_TOOLS, CHA;
/*......................................................................*/
#define MIN_ADD_DIS  10		/* max discrim for adding '(',')','!'	*/
#define MAX_ADD_DIS  30		/* max discrim for adding '╨','1'	*/
/*----------------------------------------------------------------------*/
#include "stic-mac.h"
#include "stic-frt.h"
/*----------------------------------------------------------------------*/
int16_t	dis_f (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t	dis_r (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t	dis_t (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s, int16_t sign_f);

int16_t	find_beam (STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long);
int16_t	find_neck (STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long);
/*----------------------------------------------------------------------*/

/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟		functions for wide sticks			╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */

int16_t	dis_f (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t	i, dis=0, t, lm, rm, dy=s->height;
int16_t	lf=l->num_flags, lc=l->num_concs, rf=r->num_flags;
int16_t	wid=s->stick_width, inc=s->inc;
uchar	Flag_bad_overlay=0;	// MK

if ( s->l_f_symptom>1 && s->r_f_symptom>1 )  return(0);	/* good symptoms */

if ( lf==0 && lc==0 )
	dis += tab_f[11];	/* similar 'F' */			// 70
else	/* exist left concaves or flags     */
{
t=find_beam(l,r,2);		/* long beam */
if( t<0 )
	t=find_beam(l,r,1);	/* short beam */
if( t<0 )  {
	t=find_beam(l,r,0);
	if( t<0 )
		dis += tab_f[0];	/* no beam : true stick */	// 40
	else  {
	    lm = l->mount[t];  rm = r->mount[t];
	    if( t==0 && s->r_f_symptom==0 && s->l_f_symptom<2 ||
		t>0 )  {
		if( t==0 && lm>0 && rm>0 )
			dis += tab_f[0];	// beam belong 0-zone	// 40
		if( lm+rm>1 )  {	/* first flag>1 , second flag=0 */
			dis += t ? tab_f[1] : 0 ;			// 20,0
			if( t==0 && r->mb_pos[0]<2 &&
			    lf==0 && rf==2 && (r->mount[3] || r->mount[4]) )
				dis += tab_f[13]; /* similar '[' */	// 100
			if( rf==2 && lf==0 && r->mount[4] )
				dis += tab_f[10]; /* right half 't' */	// 60
			}
		else	{
			if (lm+rm==1 )	/* first flag=1, second flag=0 */
				dis += wid<4 ? tab_f[2]>>1 : tab_f[2];	// 10,20
			else		/* no flags */
				dis += (lm+rm)?tab_f[2]:tab_f[2]<<2;   //40,160
			}
		if( lm==0 && rm && r->mb_pos[0]<2 && r->mount[4]>0 &&
		    r->mount[3]==0 && r->mount[2]==0 && r->mount[1]==0 )
//////			dis += r->up_hook?tab_f[0]<<2:tab_f[0];       //160,40
			if (!fax1x2)		// OLD OLEG
			    dis += r->up_hook?tab_f[0]<<2:tab_f[0];   //160,40
			else			// NEW MK PROBA
			    dis += tab_f[0];	// 160 (4*40) ==> 40	// 40
			/* true stick + upper dust */
		}


	     }
	}
/*......................................................................*/
else	{	/* normal beam */
	lm = l->mount[t];  rm = r->mount[t];
	if( l->mb_pos[t]>r->me_pos[t]+1 ||
	    r->mb_pos[t]>l->me_pos[t]+1 )   /* bad overlay beam-flags */
		{ dis += tab_f[12];  Flag_bad_overlay++; }	// MK	// 80
	if( lm + rm==2 )
		{	/* first flag=1, second flag=1 */
		if( wid>4 )
			dis += tab_f[3];				// 8
				/* short beam and wide stick */
		dis+=((l->m_meandr>1)+(r->m_meandr>2))*
		      (inc?tab_f[4]:tab_f[5]);				// 2,4
		/* meandr and short beam . By inc this DIS decreased */
		}
	}
/*......................................................................*/
if( t>0 )     /* normal beam ; for zones 1,2,3,4 */
	{
	for( lm=rm=i=0;i<t;i++)  {	/* 0,...,t-1 zones */
		if( lm<l->mount[i] )  lm = l->mount[i];
		if( rm<r->mount[i] )  rm = r->mount[i];
		}

	if( lm==1 && l->mount[t]==1  )	/* left upper short flags */
		dis += tab_f[6];					// 10
	else if( lm>1 && lm>=l->mount[t] )  /* greater left top flags */
		dis += tab_f[7];					// 40

	if( rm==0 && l->conc[0]==0 )	/* no right upper flags */
		dis += tab_f[8];					// 40
	else if( rm==1 && wid<6 && rm*3<r->mount[t] )
		dis += tab_f[9];	/* short right upper flags */	// 4

	if ( r->down_hook && r->mount[4] &&	// OLD
	     rm > r->mount[0] )		// 27.07.1993 for good 'f'
		dis += tab_f[10];	/* similar 't' */		// 60

			// 20.01.1993: fax25/16 r.m[2]=r.m[4]=1; ?????
//////	if( l->mount[4]==0 & r->mount[4]>r->mount[t]-1 )	// OLD OLEG
	if( l->mount[4]==0 &&
	    r->mount[4] > MAX (r->mount[t], 2) - 1 )		// 20.01.1993
		dis += r->mount[0]<r->mount[t] ? tab_f[10]<<1 : tab_f[10];
		/* similar 't' : right downer long flag */	      //120,60

	if( s->base_2!=-1 )	/* base lines known */
		{
		int16_t h_beam = (l->m_pos[t]+r->m_pos[t])>>1;
		if (Flag_bad_overlay)	{		// MK 14/20.01.1992
//////		    h_beam = (lm>rm)  ?  l->m_pos[t]  :  r->m_pos[t]; ######
		    if (l->mount[t] > r->mount[t])	h_beam = l->m_pos[t];
		    else if (l->mount[t] < r->mount[t])	h_beam = r->m_pos[t];
		    else h_beam = MIN (l->m_pos[t], r->m_pos[t]);
		    }
		if ((l->down_serif + r->down_serif) < 4)  // DL==DR==2 ???
		    {					// OLD OLEG VARIANT:
		    if (s->base_2 > MAX(dy/6,3) &&
			h_beam > s->base_2+wid )
			    dis += tab_f[14];			// 100 => 60
		    }
		else					// NEW MK VARIANT:
		    if (s->base_2 > MAX(dy/6,3) &&
//////			h_beam > MAX((h_beam>s->base_2+wid),	// OLEG ######
			h_beam > MAX((s->base_2+wid),		// OLEG
				     (dy/2)))			// MISA
			    dis += tab_f[14];			// 100 => 60
		}
	}
else
	{	/* beam belong 0-zone */
	if( l->mount[0] && l->mb_pos[0]==0 &&
	    r->mount[0] && r->mb_pos[0]==0  )
		dis +=  tab_f[11];
	}
}

if( ( r->down_hook || r->mount[4]>2 ) && l->mount[0]>1 )
		dis += tab_f[10]; /* similar    'l' */

			// 22.01.1993	DISPROPORTION: GLISTA 1-3-1 zB
			// 'l' 32*5(1-3-1) stdj7/27(34)	"Parallel" (last 'l')
			// 'E' ...*12(1-10-1)
			// 'f' 26*5(1-3-1) stdj8/16(18)	......
	if ((!l->num_long_flags && !r->num_long_flags) &&
	    ( (dy>26 || s->width>=10) ||
	      (dy>16 && ((!l->num_flags) || (!r->num_flags)) ) ))
		dis += tab_f[15];				// 80 (BP)

//////	if ( l->up_serif == 2 )				// 17.02.1993 PROBA
//////		dis += 222;				// 222

/*......................................................................*/

if( s->l_f_symptom>0 && s->r_f_symptom>1 )
	dis = (dis * plus_f[1])>>4 ;
	/* decrease DIS if good right symptom and delicate left symptom */

{				// 11.03.1993 SOPLI about {f'}; NE NA MESTE !!!
int16_t	l12 = MAX (l->mount[1],l->mount[2]);
int16_t	r12 = MAX (r->mount[1],r->mount[2]);
	if ( r->mount[0] >		// fax23/14(16) "Cardiff's"
//////	     MAX (l12,l->mount[4]) + wid + MAX (r12,r->mount[4]) )	// a)
	     MAX (l12,l->mount[4]) + wid + MAX (r12,r->mount[4]) + 4)	// b)
		dis += 222;						// 222
}

					// 15.10.1993 similar 'F'
	if (wid>4  &&  l->conc[0]<2  &&	// k11/14 "OF"  21*12 wid=5
	    (r->mount[0]>4 ? 1 : 0) +	// 18.10.1993 fax4/9 "modified"
	    (r->mount[1]>4 ? 1 : 0) +
	    (r->mount[2]>4 ? 1 : 0) == 2  &&
	    l->mount[1]<3  && l->mount[2]<3)  // 20.10.1993 fax18/20(25) manuf
		dis += 40;						// 40

	if (s->cut_r  &&		// 21.10.1993 for similar 'D' (CUT)
	    r->mount[0]==r->mount[4]  &&
	    MAX(r->mount[1],r->mount[2])*3 < r->mount[0])
		dis += mk_80_for_CUT;					// 80

	if (s->cut_r  &&		// 18.11.1993 for similar 'E' (CUT)
	    r->mount[4]>5)		// j8/13(14); really "B/" (CUT)
		dis += mk_80_for_CUT;					// 80

return (dis);		// dis_f
}
/*----------------------------------------------------------------------*/
int16_t	dis_r (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  {
							// 27.01.1994
int16_t	nl=l->num_long_flags, nr=r->num_long_flags;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	dis=0, wid_2=wid>>1, dy=s->height;	// t
int16_t	lmu, rmu;	// 26.01.1994 (OLD lm, rm);
int16_t	lmd, rmd;	// 25.01.1994

//////printf (" r(%d,%d) ", r->mount[0], r->mount[4]);

lmu = MAX(l->mount[0],l->mount[1]);	// "UPPER ZONE: 0 or 1
rmu = MAX(r->mount[0],r->mount[1]);

lmd = l->mount [4];	rmd = r->mount [4];	// 25.01.1994, DOWN ZONE;

if( lmu>=1 && rmu>=1 )  {
//////	if( lm*3>rm*4 && lm>3 )			// OLD OLEG
//////		dis += (lm-rm)/2*tab_r[0];	// l.flag > r.flag	// *8

	if ( lmu>=3)  {				// 24.03.1993 MK PROBA
	    if (!inc)  {
		if ( lmu >= rmu - (lmu>=wid) )	// NO INC:  NEW
		    dis += tab_r[16];		// l.flag > r.flag	// 80
		}
	    else if ( lmu>3 && lmu>rmu )	// IF INC:  MODIFY
		dis += (lmu-rmu)*tab_r[0];	// l.flag > r.flag	// *20
	    }					// 25.10.1993 (OLD *8)

	if ( rmu==1 && r->mount[0]==r->mount[1] )
		dis += tab_r[11];					// 10

	if (lmu==2  &&  rmu==1)	 		// 25.10.1993
		dis += tab_r[17];	// too short upper flags	// 40

	if( rmu==1 && lmu==1 && l->m_meandr>1 &&
	    (r->m_meandr>1||r->mount[3]||r->mount[4]) )
		dis += tab_r[12];	/* too short upper flags */	// 40

//////	if( (lmu<3||!inc) && lmu>1 && lmu==rmu && nr==1 && nl==1 )
				// 17.01.1994	PROBA CORRECT for SMALL 'r':
	if ((!inc  ||  lmu<3 && (inc_num_EEM!=1 || dy>16))  &&
	    lmu>1 && lmu==rmu && nr==1 && nl==1)	// OLD OLEG CONDITIONS
		dis += tab_r[13];	/* simmetrial upper zone */	// 100
	}
else
	{
	if( rmu==1 )
		dis += r->m_meandr?tab_r[1]<<4 :tab_r[1] ;		//32,2
		/* too little right upper flags */
	if( rmu==0 )
		dis += tab_r[2] ; /* no right upper flags */		// 70
	}

	if(l->mount[2]>wid_2 )dis += tab_r[3];	// left center flag	// 10
//////	if( r->mount[2]>wid_2 )dis += tab_r[4];	// right center flag	// 10
						// 07.01.1994:
	if (r->mount[2]>wid_2)			// right center flag
//////		dis += (r->mount[2]<wid) ? 10 : 100;		// 10/100
//////		dis += (r->mount[2]<=wid) ? 10 : 100;		// 10/100
		dis += (r->mount[2]<=wid) ? 10 : 80;		// 10/80

dis += ((nr>2)+(nl>2))*tab_r[5];	// too many left or right flags	// *6

/******************************	21.01.1994	MK_PROBA_DELETE (2 Places)
//////if (r->down_serif && l->down_serif==0 ||
if ((r->down_serif || r->mount[4]>2) && l->down_serif==0 ||	// 07.01.1994
    l->down_serif && r->down_serif==0 )  {
	if( abs(r->mount[4]-l->mount[4])>wid_2+1 )
		dis += tab_r[6] ;	// too different down flags	// 4
//////	if( l->mount[4]==0 && r->mount[4]>1 )
	if (l->mount[4]==0 && r->mount[4] > (inc ? 2 : 1))  {	// 20.11.1993
static	uchar	OTL_similar_t=0;
		dis += tab_r[10];					// 20
//////		if (r->mount[4]>wid)		// 07.01.1994 similar 't'
		if (r->mount[4]>wid &&		// 07.01.1994 similar 't'
		    OTL_similar_t==1)		// 17.01.1994 FOR DEBUG;
			dis += 30;					// 30
		}
//////	DIS_CURVE(l,r,4,1,tab_r[7]);					// 10
	DIS_CURVE_MK(l,r,4,1,tab_r[7]);		// 27.07.1993		// 10
	}
*****************************************************************************/

if( r->mount[0]==0 && r->mount[1] >0 && r->mb_pos[1]>dy/6 )
	dis += tab_r[8]; /* for bad catting parts */			// 60

//////if (lm+rm<2 && wid>3)	// OLD OLEG
/////  || (wid>8 && rm<2) )	// and NEW MK 10.05.1993: first PROBA (||);
if (wid>3 && rmu<2)		// MK 11.05.1993
	dis += tab_r[9];	/* too small beam */			// 80

/******************************	21.01.1994	MK_PROBA_DELETE (2 Places)
t=r->mount[4];
//////if( t>rm && l->mount[4]<2 )		// before 24.03.1993
//////	dis += tab_r[10]*(t-rm);	// too long right downer flag	// *20
//////if( t>=rm && l->mount[4]<2 )		//   from 24.03.1993
if( t>=rmu && l->mount[4]<2 && l->mount[3]<2)	//   from 09.01.1994
	dis += tab_r[10]*(t-rmu+2);	// too long right downer flag	// *20
			// NOTA BENE: DOUBLE ABOVE  "too different down flags"
			//			and "similar 't'" (SEE!!!);
***************************************************************************/

//////if( abs( lmu-l->mount[4])<2 && abs(rmu-r->mount[4])<2 && lmu>1 && rmu>1 )
if (abs(lmu-lmd)<2 && abs(rmu-rmd)<2 && lmu>1 && rmu>1 &&	// MK EDITION
    (rmu+rmd) - (lmu+lmd) < 4)  // 27.01.1994 MK ADD CONDITION;
	dis += tab_r[14];	// similar 'I'				// 50
/*......................................................................*/
						// 27.01.1994
						// 26.01.1994
						// 25.01.1994	NEW CHARGES:
						// (see DIFFS 21.01.1994)

if (rmd>3)  {			// RIGTH DOWN	// 25/26.01.1994
	if (lmd<2)		// zB 1/3
		dis += 50;						// 50
//////	if (rmu<=rmd)		// 25/26.01.1994
	if (rmu+lmd<=rmd)	// 27.01.1994
		dis += 100;	// really 't'				// 100
	}

if (rmd==3)  {			// RIGTH DOWN	// 26.01.1994
	if (lmd==0 ||		// 0/3
	    lmd==1 && wid<4)	// 1/3, NOT BIG WID
		dis += 50;						// 50
	if (rmu<=rmd)		// really 't';
		dis += 100;						// 100
	}

if (rmd<3 && rmu<=rmd &&	// 26.01.1994 for GLUE 'u' (zB)
    lmu==0 && lmd==0)		// fax26/14 "solutions"
	dis += (rmd==1) ? 10:	// RMD=1	m.b. more ???		// 10
			  30;	// RMD=2	m.b. other ???		// 30

if (wid>=3)  {			// 26.01.1994 for GLUE 'm' (zB)
	if (rmu<2)		// m16/3 "comforting" (see both 'o', 'm')
		dis += 30;	// for def Laterne ???			// 30
	/*......*/
	}

if (dy>=16)  {			// 26.01.1994 for GLUE
	if (rmu<2)		// m16/3 "comforting" (see 'n')
		dis += 100;	// for def Laterne ???			// 100
	/*......*/
	}

//////if (lmd>=3 && rmd<=1)	// 26.01.1994	k19/5 "traditional" (4/2)
if (lmd>=3 && rmd==0)		// 27.01.1994	k19/5 "traditional" (4/2)
	dis += 60;		// as OLD for GLUE "r.";		// 60

if (s->t_TOP)			// t^
//////	dis += 50;		// 25.01.1994				// 50
	dis += 20;		// 25.01.1994				// 20

if (rmu>=wid*4  &&		// 'c' cutted or not glued
    wid<3)			// 27.01.1994 m25/16(22) "prepare" ('r' 3/12)
	dis += 50;							// 50

return (dis);		// dis_r
}
/*----------------------------------------------------------------------*/
/*	sign_f = 1 - 'f' belong list of versions */
int16_t	dis_t (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s, int16_t sign_f)
{							// 10.01.1994
int16_t i, dis=0, t, lm, rm, dy=s->height, dx=s->width, t2;
int16_t	nl=l->num_long_flags, nr=r->num_long_flags;
int16_t	sl=l->num_flags, sr=r->num_flags;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	tpos;	// 18.11.1993 (old - use the same {t}, how for zone)

t = find_beam (l, r, 1);
/*......................................................................*/
if( t<0 )  {						// BAD BEAM:
	t=find_beam(l,r,0);
//////	lm = l->mount[t];	// ###### but if t<0 - ?????? QWERTY ######
//////	rm = r->mount[t];	// 23.03.1993 make it more akkuratno !!!
	if( t<0 )  {
		dis += tab_t[0];  /* no flag long >=1  */		// 40
		lm = rm = 0;	// MK 23.03.1993 about SHURIK g20/51 s->r ERR
		}
	    else  { lm = l->mount[t];  rm = r->mount[t]; }	// 23.03.1993

	if( lm+rm>1 )  {	/* first flag>=2, second flag=0 */
//////		dis += lm ? lm*tab_t[1] : tab_t[1];		      // *20,20
		/************************************************************
		dis += lm ? lm*tab_t[1] :		// 18.10.1993:	// *20
			    ((s->cut_l || s->cut_r) ? tab_t[32] :	// 60
						tab_t[1]);		// 20
						*****************************/
					// 19.10.1993 after FARA-078:
		if (lm)	dis += lm*tab_t[1];			// *20
		else
		  if ((s->cut_l || s->cut_r) && rm<3 &&	// 19.10.1993
		      rm>r->mount[4])			// 20.10.1993
//////			dis += tab_t[32];	// 19.10.1993 	// 60
//////			dis += tab_t[32];	// 20.10.1993	// 44
				// 20.11.1993 fax8/19(21) "using" 'n' => "tl"
			dis += (wid>=5 ? 88 : 44);		// 88/44
		else	dis += tab_t[1];			// 20

		if( t==0 && r->mb_pos[0]<2 &&
//////		    !(s->base_2!=-1 && abs(r->m_pos[0]-s->base_2)<2 ) &&
		    !(s->base_2!=-1 &&			// 18.11.1993
		      abs(r->m_pos[0]-s->base_2) < (fax1x2 ? 3 : 2 )) &&
		    !(r->m_pos[0]>dy/6&&r->me_pos[0]>dy/3) )
			dis += tab_t[24];				//100
				//long right flag belong 0-zone
					  /* MANY KNOTs !!! */
		}
	else	{		/* first flag=1, second flag=0  */
		if( rm )
			dis += wid>2 ? tab_t[2] : tab_t[20] ;	     // 50,4
		else
			dis += tab_t[17];			      // 120
		if( wid<3 )  {
			if( t==2 && r->mount[3] )
				dis += tab_t[20];			// 4
			if( r->m_meandr>2 ||
			    r->m_meandr==2 && r->mount[4]==0 )
				dis += tab_t[25];			// 40
			}
		if( rm && lm==0 && t>0 && l->mount[0] )
			dis += tab_t[2];				// 50
		}
	}
/*......................................................................*/
else	{						// NORMAL BEAM:
	lm = l->mount[t];   rm = r->mount[t];

				// MK PROBA 03.02.1993 about 'r'=>'t'
	if ((t==0) && (wid>=4) && (lm<=1) && (rm>=4) &&
	    (r->mount[4]<5))	// 07.01.1994 EDITION
//////		dis += 122;	// MK PROBA 03.02.1993		// ###########
		dis += 22;	// MK PROBA 04.02.1993			// 22

if (!fax1x2)	{				// OTLADKA 20.01.1993
	if ( (l->me_pos[t] - l->mb_pos[t]) > dx &&	// OLD ###########
	     (r->me_pos[t] - r->mb_pos[t]) > dx )
		dis += tab_t[21]; /* bad overlay beam-flags */	      // 100
	}
if (fax1x2)	{				// OTLADKA 20.01.1993
	if ( ((l->mb_pos[t] - r->me_pos[t]) > 1) ||	// NEW PROBA
	     ((r->mb_pos[t] - l->me_pos[t]) > 1) )
		dis += tab_t[21]; /* bad overlay beam-flags */	      // 100
	}

/* N.B. in TC-interface base lines are not known */
  if( s->base_2!=-1 )  {  /* base lines known */
    t2=2;
if( multy_language ) t2=4;
	    if (s->base_2>MAX(s->height/6,3) &&
    (MAX(l->m_pos[t],r->m_pos[t]) < s->base_2-t2 ||
//////		 MIN(l->m_pos[t],r->m_pos[t]) > s->base_2+4))	// 18.10.1993
//////		 MAX(l->m_pos[t],r->m_pos[t]) > s->base_2+4))	// 18.10.1993
		 MAX(l->mb_pos[t],r->mb_pos[t]) > s->base_2+4))	// 19.10.1993
			dis += tab_t[18];			      // 100
	    else if (inc_num_EEM>=2  &&		// 18.11.1993 similar '7'
		     t==0  &&			// j10/29 "08/77" first '7'
		     l->mb_pos[0] <= 1 &&
		     l->me_pos[0] + 2 <= r->me_pos[0]  &&
		     l->me_pos[0] + 2 <= s->base_2)
			dis += tab_t[18];			      // 100
	    }

	if( lm+rm>3 )	/* flags : 1,2  2,1 2,2  ... */
		{	/* long flags */
		int16_t im,ma;
		if( lm*3>rm*4 && lm>3 )
			dis += tab_t[3];  /* left>right */		// 4

		for(i=0;i<3;i++)  {
			if( i==t )continue;
			if( l->mount[i]>=l->mount[t]) break;
			}

					// 26.08.1993	INCREASE for "ft"
		if (i==0  &&  nl==2  &&  nr==2  &&
		    t>0  &&  s->cut_l  &&
		    (!inc && l->mount[0]==lm || inc && l->mount[0]>lm)  &&
//////		    rm>wid  &&  r->mount[4]>=wid)	// first variant
		    rm>=3  &&  r->mount[4]>=3)		// second variant
			return (-444);	// NOTA BENE:  INCREASE !!!!!!!!!!
			/************************************************/

		if ( i<3 )  {  /* exist left top long flag  */
			if ( l->mount[t]>1 )
				dis += tab_t[4];			// 30
			else if( !inc )
				dis += tab_t[4]>>2;			// 7
			}
			/* exist big flag and beam */

		for (im=ma=-1,i=0;i<3;i++)  {
			if( i==t )continue;
			if( r->mount[i]>=r->mount[t]) break;
			if( r->mount[i]>ma)
				ma = r->mount[(im=i)];
			}

		if( i<3 )  {  /* exist long right upper flag */
			dis += ( r->me_pos[i]-r->mb_pos[i])*tab_t[14]; // *15
			}

		if( s->width>4 && im==0 && ma>=r->mount[t]-1 &&
		    !(ma==1&&inc) )
			dis += tab_t[23];				// 20
		}

	else if( lm + rm==2 )  	{
		for(i=0;i<3;i++)  {
			if( i==t )continue;
			if( l->mount[i]>1) break;
			if( r->mount[i]>2) break;
			}
		if( i<3 )
			dis += tab_t[4];				// 30
			/* exist big flag and beam */
		if( wid>4 )
			dis += tab_t[5]; // short beam and wide stick	// 2
		if( s->width>2 && !(l->mount[3]==1&&l->conc[4]) )
			dis+=((sl>1)+(sr>2))*
			     (inc ? tab_t[6] :				// *2
				    (s->width<4?tab_t[26]:tab_t[7])); //*20,40
		else if( sl>1 || sr>2  )
			dis += tab_t[20];				// 4
		/* meandr and narrow beam */
		if( inc && wid<5 &&
		    (l->me_pos[t]<=r->mb_pos[t]||r->me_pos[t]<=l->mb_pos[t]))
			dis += tab_t[22];				// 40
		}
	}	// IF ELSE: NORMAL BEEM;
/*......................................................................*/
	dis += ((nr>3)+(nl>1))*tab_t[8];			       // *24
	/* too many left or right flags */
	if( l->mount[4]>1 && !inc || l->mount[4]>2 && inc )
		dis += tab_t[10] ;  /* left downer flag */		// 40
if( t<2 )
	DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_t[11],tab_t[12]);	     //40,4

if( t>0 && t<3 && dx<<2 < dy*3 )
	{  /* KNOT : find big flags of beam - part bold 'H' */
	if( l->num_flags==1 && l->me_pos[t]-l->mb_pos[t]+1>dy/3 &&
	    s->cut_l   ||
	    r->num_flags==1 && r->me_pos[t]-r->mb_pos[t]+1>dy/3 &&
	    s->cut_r   )
		dis += tab_t[28];					// 40
	}

if( l->up_serif==2 && l->down_serif==2 &&
    r->up_serif==0 && r->down_serif==0 &&  t<2 )
	dis += tab_t[15];  /* similar    ],) */				// 60

//////t = MAX( l->m_pos[t], r->m_pos[t])+1;
tpos = MAX( l->m_pos[t], r->m_pos[t])+1;	// 18.11.1993
if( tpos>dy>>1 )
	dis += tab_t[13]*(tpos-(dy>>1)); /* too low beam */	     // *16
/* ----------------- FIND f-CONFIGURATIONS ------------------- */

//////if (tpos>(dy>>2) && r->mount[0]>2 && r->mount[1]>2 && l->conc[0]>0 )
if (t>0 &&		// 18.11.1993 PROBA for f10/15 "of"
    r->mount[0] > (wid>2 ? 2 : 1) &&
    l->conc[0]>0 )
	dis += tab_t[14];   /* head : similar to 'f' */			// 14
		// NB: include THIN STICK, but EXCLUDE SOME OTHER CASES;

/**********************************
if( (s->l_f_symptom>0 && s->l_f_symptom<3 &&
     s->r_f_symptom==1 ||
     s->l_f_symptom==3 && s->r_f_symptom==2 ||
     sl==1 && s->r_f_symptom && (!inc || s->l_f_symptom) )
    && sign_f && r->down_hook==0 )
	dis += tab_t[16];	// similar 'f' ! MANY KNOTs		// 70
    **********************************************/
if ( s->r_f_symptom==2 )	// similar 'f'		// 10.02.1993
	if (r->up_serif==2)
		dis += tab_t[16];					// 70
	else if ( r->up_serif==1 && s->l_f_symptom )
		dis += tab_t[30];					// 16
	else	dis += tab_t[31];					// 8

if( s->l_f_symptom==2 && s->r_f_symptom==2 && sign_f &&
    r->mount[4]==0 )
	dis += tab_t[19];    /* similar 'f' */				// 10

if (l->down_hook ||			// 04/07.06.1993
////l->mount[4]>1)			// 04.06.1993
    l->mount[4] >= 2 + inc + fax1x2)	// 07.06.1993
	dis += tab_t[16];		// similar 'f'			// 70
	// NB: I can use SERIF==2, SERIF==1, !INC in combinations with L.M.4

/* ----------------- FIND l-CONFIGURATIONS ------------------- */

if( l->m_meandr==1 && l->num_flags==1 && l->mount[4] &&
    r->m_meandr==1 && r->num_flags==1 && r->mount[0] )
	dis += tab_t[27];   /* head : similar to 'f' */			// 40

/*......................................................................*/
{				// MK: TOO int32_t HOOK: for "t." ==> 't'
///int16_t	n1, n2;			// NB: tab_t[29]=80 (fax12/32 "document.")
///	n1 = MAX (wid, (lm+rm));
//////	n2 = (n1<4) ? (n1+4) : (n1*2);	// 4 - MK VOLUNTAR.	before 9.3.1993
///	n2 = (n1<4) ? (n1+4) : (n1*2)-2; // 4 - MK VOLUNTAR.	from 09.03.1993
//////	if (r->mount[4]>n2)  dis += tab_t [29];	// before 26.01.1993	// 80
//////	if (r->mount[4]>=n2) dis += tab_t [29];	// from 26.01.1993	// 80
	if ((r->mount[4]>=lm+wid+rm) &&		// 10.03.1993
//////	    (r->mb_pos[4]+wid > dy))		// 10.03.1993 no big hook
	    (r->m_pos[4]+wid+3 > dy))		// 11.03.1993 no big hook
		dis += tab_t [29];				     //80=>100
			// fax12/32	"document." 1-5-4 / 11
			// fax41/72	"support."  2-5-4 / 12
			// fax35/21(24)	"format."   2-4-3 / 9
			// b18/27	"this"	    1-4-3 / 6
			// d19/35(36)	"with"	    2-3-2 / 7, BIG HOOK (-7)
}
/*......................................................................*/
	/********************************************************************
	if (s->neck==1)			// 18.10.1993 for d7/2
		dis += 10;						// 10
	if (s->neck>=2)			// 18.10.1993 for d7/2
		dis += 40;						// 40
		*************************************************************/

	if (s->neck>=2)			// 20.10.1993
		dis += 20;						// 20
/*......................................................................*/
						// 10.01.1994	BONUS:
	if (dis==0	&&	// NO DISCRIM;
	    s->t_TOP	&&	// t^
	    l->mount[0]	&&	// LMU EXIST;
	    l->conc[4]	&&	// LDC EXIST;
	    s->base_2mk>0  &&	// BASE_2 1 or 2;
	    s->base_2mk<=2)
		dis = -32;	// BONUS !!!!!!!!!!!!
/*......................................................................*/
return(dis);
}
/*----------------------------------------------------------------------*/
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟		auxiliary functions				╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/*----------------------------------------------------------------------*/
/* find beam (lim_long - width of beam-flag) in stick */
int16_t	find_beam (STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long)
{
int16_t i,num,maxim,m,rr,ll;

for(num=-1,maxim=m=i=0;i<3;i++)
	{
	rr=r->mount[i];
	ll=l->mount[i];
	m = ll + rr;
	if( rr>=lim_long && ll>=lim_long &&
	    maxim<=m ) /* two flags>=lim_long, find last maximum */
		{
		maxim = m;
		num = i;
		}
	}

return( num );
}
/*----------------------------------------------------------------------*/
int16_t	find_neck(STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long)
{
int16_t i,num,minim,m,rr,ll;

for(num=-1,minim=m=i=0;i<3;i++)
	{
	rr=r->conc[i];
	ll=l->conc[i];
	m = ll + rr;
	if( rr>=lim_long && ll>=lim_long &&
	    minim>=m ) /* two flags>lim_long, find last minimum */
		{
		minim = m;
		num = i;
		}
	}

return( num );
}
/*----------------------------------------------------------------------*/
	// END of STIC_FRT.C
/*----------------------------------------------------------------------*/
