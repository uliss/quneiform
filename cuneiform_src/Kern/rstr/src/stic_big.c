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
/*	STIC_BIG.C	18.01.1994	from DIS_STIC.C			*/
/*----------------------------------------------------------------------*/
//////#define	MKPRINT_ENABLE
#define	mk_80_for_CUT	80
/*----------------------------------------------------------------------*/



  #include <stdlib.h>
  #include <string.h>

			// here was DEBUG_GRAPH
  #include "func.h"
  #include "stick.h"
  #include "ligas.h"
  #include "lang.h"
  #include "tuner.h"
  #include "status.h"
#include "minmax.h"
extern uchar multy_language;

extern int16_t pitchsize ;
extern int16_t nIncline  ;
extern uchar fax1x2;	// MK NEW 05.01.1993
extern int  dis_LIMIT_EEM;	// in ST_TOOLS.C;
extern int  mk_dis_for_liga_exm;	// 06.01.1994
/*......................................................................*/
#ifdef	MKPRINT_ENABLE				// MK OTLADKA Variables
extern uint16_t	mkm1, mkm2, mkm3, mkm4, mkm5;
extern uint16_t	mkma, mkmb, mkmc, mkmd;
///extern uint16_t	d,d0,d1,d2,d3,d4,d5,d6,d7,d8,d9;
#endif
/*......................................................................*/
#define MIN_ADD_DIS  10		/* max discrim for adding '(',')','!'	*/
#define MAX_ADD_DIS  30		/* max discrim for adding '╨','1'	*/
/*----------------------------------------------------------------------*/
#include "stic-mac.h"
#include "stic-tb.h"
/*----------------------------------------------------------------------*/
///int16_t	dis_F (STICK_CHARS *r, STICK_SIGNUMS *s);	// MK
int16_t	dis_F (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s); // 25.03.1993
int16_t	dis_I (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t	dis_J (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t	dis_L (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t	dis_T (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t	dis_Y (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t dis_RusG (STICK_CHARS *l, STICK_CHARS *r,STICK_SIGNUMS *signums);
int16_t	dis_circle_brace(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s,
			int16_t typ);
int16_t	dis_l_sq_brace(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
int16_t	dis_r_sq_brace(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);

			// 30.09.1993 add DUMMY DISCRIM's:
int16_t	dis_figa_left (STICK_SIGNUMS *s);
int16_t	dis_figa_right(STICK_SIGNUMS *s);

int16_t	dis_slash(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
int16_t	dis_vert(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s, int16_t add_flag, char type);
/*......................................................................*/
static uchar config_f( STICK_CHARS *l,STICK_CHARS *r);
static uchar config_r( STICK_CHARS *l,STICK_CHARS *r);
static uchar config_t( STICK_CHARS *l,STICK_CHARS *r);
static uchar config_1( STICK_CHARS *l,STICK_CHARS *r);
/*......................................................................*/
/*		NOTA BENE: some functions without DECLARATION:
 *  int16_t similar_excl(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
 *  int16_t similar_lcb(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
 *  int16_t similar_rcb(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
 *  uchar similar_wide_frt1(STICK_CHARS *left_chars,STICK_CHARS *right_chars)
 */
/*----------------------------------------------------------------------*/
int16_t dis_F (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)	{
int16_t	dis=0;						// 12.10.1993
int16_t	porog = MAX (s->stick_width, 4);	// NB: 4 - MK VOLUNTARY
	// NB: porog used 2 times !!!
/*......................................................................*/
				// 03.01.1993	PROBA USE f_symptoms:
//mk5555 = (((uint16_t) s->l_f_symptom)<<8) + tab_F[s->l_f_symptom];
//	return (tab_F [s->l_f_symptom]); // 0,1,2,3 -> 0,50,100,150
/*......................................................................*/
				// 12.10.1993	SECOND PROBA USE f_symptoms:
	if ((s->l_f_symptom) &&
	    (l->mount[1]>=2 || l->mount[2]>=2))		// NB: Proisvolno:
		dis += tab_F [s->l_f_symptom];	// 0,1,2,3 -> 0,30,60,90
/*......................................................................*/
if (l->up_serif==0)  {		// USLOWIE 25.03.1993 about 'F'=>'P'
				// fax50/7,14	"FileNet"(#2),"France"
				// fax51/12	"FileNet"
int16_t	r0=r->mount[0], r1=r->mount[1], r2=r->mount[2], r01, r12;
	r01 = MAX (r0,r1);		// before 04.03.1993
	r12 = MAX (r1,r2);

	if (r0>=r1)  { r01 = r0;  r12 = MAX (r1,r2); }	// 04.03.1993 PROBA
	       else  { r01 = r1;  r12 = r2; }

	if ((r01==0) ||		// fax6/24 "financial" ("fi" -> "Fi")
//////	    (r12) &&				// 03.03.1993 PROBA
	    (r01-r12)>porog &&
	     r01>2*r12)		// stdd13/1 "Fine", stde13/20 "Firehouse"
		dis += tab_F[4];	// 80 for "fi"=>"Fi'	// 60 ??????
}
/*......................................................................*/
/*******************************************************  SUPPRESS 25.01.1993
				// 22.01.1993 fax42/???:
//////	if ( r->mount[4] > MAX (s->stickwidth, 4) )
	if ( r->mount[4] > porog )
		dis += tab_F[5];	// for 'E' => 'F'
		*****************************************/
/*......................................................................*/
{				// 01.02.1993 SECOND POPYTKA:
int16_t	porog_down;
	if ( r->mount[4] > porog )  {
	    porog_down = MAX (r->mount[0], porog);	// ??????
	    porog_down = MAX (r->mount[1], porog_down);
//////	    porog_down = MAX (r->mount[2], porog_down);		// before 9.3
	    porog_down = MAX (r->mount[2], porog_down) - 1;	// from 9.3.93
	    if ( r->mount[4] >= porog_down )	// STROGO fax35/30(33),/37(40)
		dis += tab_F[5];	// for 'E' => 'F'		// 100
	    }
}
/*......................................................................*/
	if ((r->mount[1]==0)  &&		// 12.10.1993 for 'T' => 'F'
	    (r->mount[2]==0)  &&		// (PROBA: ENTE !!!)
	    (r->me_pos[0]*3<s->height))		// ok56/1 "SELECTS" (12:37)
		dis += tab_F[5];	// similar 'T' (cut.L)		// 100
/*......................................................................*/
	if ((r->mount[1]*2<r->mount[0])  &&	// 18.10.1993 SHORT MIDDLE
	    (r->mount[2]*2<r->mount[0])  &&
	    (r->me_pos[0]*3<s->height))		// c20/9 "Full"
		dis += tab_F[6];	// SHORT MIDDLE (for GLUE)	// 40
/*......................................................................*/
						// 12.10.1993	PROPORTION:
	if (MAX(r->mount[0],
	    MAX(r->mount[1],r->mount[2]))*6 < s->height)
		dis += tab_F[5];	// TOO SHORT FLAGS		//100
/*......................................................................*/
	if (l->mount[0]>=3  &&			// 18.11.1993	j10/27 'P'
	    dis_LIMIT_EEM   &&		// m.b. without this ???
	    r->mount[0] <= l->mount[0]+1)
		dis += tab_F[5];					//100
/*......................................................................*/
	return (dis);
}
/*----------------------------------------------------------------------*/
int16_t dis_I (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s) {	// 17.01.1994

int16_t	dis=0, t, lmu, rmu, lmd, rmd, num_l, num_n, num_z,z;
int16_t	sl=l->num_flags, sr=r->num_flags;
int16_t	wid=s->stick_width, inc=s->inc, dy=s->height;
int16_t	max_u, max_d;
int16_t	d_L, d_R;	// 24.02.1993

	dis = dis_LIMIT_EEM;		// 18.11.1993 (0 or 100);
//////	dis += mk_dis_for_liga_exm;	// 06.01.1994 (0 or 2);	MOVE TO END;

lmu = l->mount[0];  rmu = r->mount[0];	max_u = MAX (lmu, rmu);
lmd = l->mount[4];  rmd = r->mount[4];	max_d = MAX (lmd, rmd);
/*......................................................................*/
if( lmu+rmu+lmd+rmd != 0 )  {
if ( max_u > (MAX(max_d,3)<<1) )		// MK PROBA 18.02.1993
	dis += 222;							// 222

DIS_HALF_SERIF (l,r,0,1,tab_I[0]);	/* test upper serif  */		// 20
DIS_HALF_SERIF (l,r,4,1,tab_I[0]);	/* test downer serif */		// 20

if( l->up_serif && r->up_serif )
	DIS_DIFFERENT_SERIFS(l,r,0,4,wid,tab_I[1]);			// *8
//////if( l->down_serif && r->down_serif )
if( l->down_serif && (r->down_serif || r->down_hook))	// 28.06.1993 about 'L'
	DIS_DIFFERENT_SERIFS(l,r,4,4,wid,tab_I[1]);			// *8

/* -------------------- STUDY ANGLES ----------------------- */

t = 3;				/* study long flags */
num_l = (lmu>t)+(rmu>t)+(lmd>t)+(rmd>t);
num_n = (lmu<1)+(rmu<1)+(lmd<1)+(rmd<1);
z = num_l>2 ? 2 : 1;
num_z = (lmu<z)+(rmu<z)+(lmd<z)+(rmd<z); /* num empty angles */
if( num_l==2 && num_n==2 )
	dis += tab_I[2];	/* two half-serifs only	*/		// 30
t = lmu+rmu+rmd+lmd;
if( num_z==2 && t>2 )		/* two empty half-serif */
	dis += t>4 ? tab_I[7]<<1:tab_I[7];				// 60

/******************************************************	BEFORE 26.10.1993
if( num_z==3 &&			// three empty half-serif
    (lmu>0 && s->dis_nose_1==0 ||	// exist 1-nose
     rmu>0 && s->r_f_symptom && s->l_f_symptom ))
//////	dis += tab_I[8];	// similar 'f','1'			// 70
	dis += 50;		// PROBA 22.10.1993 EVENING;		// 50
	*********************************************************************/

if (num_z==3)  {		// three empty half-serif  26.10.1993 Edition:
    if (rmu>0 && s->r_f_symptom && s->l_f_symptom)
	dis += tab_I[8];	// similar 'f'				// 70
    if (lmu>0 && s->dis_nose_1==0)	// exist 1-nose, similar '1':
	dis += (lmu==1) ? 50 :		// d13/5(6) "CRITIC"		// 50
	       (lmu==2) ? 60 :		// MK INTERPOLATION;		// 60
			  70;		// c3/21 "3.1x"			// 70
    }

if( num_z==3 && lmd>1 )		/* three empty half-serif */
	dis += tab_I[8];						// 70

if( num_z==1 && num_l==3  && rmu==1 )  /* UL,DL,DR flags */
	dis += tab_I[8];	/* similar 1,l */			// 70

t=1;				/* study near flags */
num_l = (lmu>t)+(rmu>t)+(lmd>t)+(rmd>t);
if( num_l==3 && lmu>0 && s->typ_nose_1==1 )
	dis += tab_I[8];	/* left flag similar 1-nose  */		// 70
if( num_z==1 && rmu<t && r->up_serif==0 )
	dis += tab_I[8];	/* similar 1 */				// 70
if( num_l==3 && rmu==1 && lmd+lmu+rmd>8 )
	dis += tab_I[8];	/* similar 1 */				// 70
if (s->neck==3)						// 16.12.1993
	dis += 100;							// 100
else if (s->neck==2 && num_l<3)
	dis += tab_I[10];	/* simular ╨ */				// 20
DIS_BRACES_CIRCLE(l,r,wid,tab_I[14]);	/* braces (,) */		// 40
}
/*......................................................................*/
else	{	/* all angles empty */
	if (s->neck==3)					// 16.12.1993
		dis += 100;						// 100
	else if (s->neck==2)
		dis += tab_I[10];	/* similar ╨ */			// 40
	else if( s->neck==1 )
		FOUR_CONC(l,r,tab_I[10]);				// 40
	/*************************************	DELETE IT 20.10.1993
	if (!inc  &&  (s->cut_l || s->cut_r)) // 18.10.1993 CUT, NO INC&Serifs
//////	    { dis = 140; goto m_ex; }	// g14/6 "Greatest", first 't'	// 140!
	    { dis = 90;	 goto m_ex; }	// g14/6 "Greatest", first 't'	// 90!
//////	    { dis = 76;  goto m_ex; }	// g14/6 "Greatest", first 't'	// 76!
	    *************************************************************/
	}
/*......................................................................*/
/* N.B.	in group macros (CENTER_FLAG) used ->c_meandr.	*/
/*	If c_meandr==0 and exist near central flag,then */
/*	not discrim. For narrow sticks I wont check	*/
/*	near flags . -->> I increase signums c_meandr	*/
/*	for left and right chars of stick		*/

if ( wid<3 )  { l->c_meandr++;  r->c_meandr++; }	// MODIFY c_meandr !!!

/*********************************************	OLD OLEG VARIANT
DIS_CENTER_FLAGS(l,r,1,wid,inc,tab_I[3], tab_I[4]);
DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_I[3], tab_I[4]);
DIS_CENTER_FLAGS(l,r,3,wid,inc,tab_I[3], tab_I[4]);
	********************************************************/

				// This Part - as in dis_l !!!
				// ABOUT ONE PIXEL		// 24.02.1993
	d_L = d_R = tab_I[4];	// DEFAULT			// =12
	/*********************************************	before 27.07.1993
	if (s->neck<2)  {					// as in dis_l
	    if ( (!l->num_long_flags) && !s->cut_l &&
		 ( (wid>=4 && dy>18) ||				// 23.02 17:30
		   (wid==3 && !l->num_flags) ) )	d_L = 0;
	    if ( (!r->num_long_flags) && !s->cut_r &&
		 ( (wid>=4 && dy>18) ||				// 23.02 17:30
		   (wid==3 && !r->num_flags) ) )	d_R = 0;
	    }
	    *************************************************************/
	if (s->neck<2)  {
//////	    if ( (!l->num_long_flags) && !s->cut_l &&	// OLD
	    if ( !s->cut_l &&
//////		 (dy>24)  ||				// 27.07.1993 PROBA
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!l->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !l->num_flags)))  d_L = 0;
//////	    if ( (!r->num_long_flags) && !s->cut_r &&	// OLD
	    if ( !s->cut_r &&
//////		 (dy>24)  ||				// 27.07.1993 PROBA
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!r->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !r->num_flags)))  d_R = 0;
	    }

//////DIS_CENTER_FLAGS(l,r,1,wid,inc,tab_I[3], ddd);		// 60,12(0)
DIS_CENTER_FLAG(l,1,wid,inc,tab_I[3], d_L);			// 60,12(0)
DIS_CENTER_FLAG(r,1,wid,inc,tab_I[3], d_R);			// 60,12(0)
//////DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_I[3], ddd);		// 60,12(0)
DIS_CENTER_FLAG(l,2,wid,inc,tab_I[3], d_L);			// 60,12(0)
DIS_CENTER_FLAG(r,2,wid,inc,tab_I[3], d_R);			// 60,12(0)
//////DIS_CENTER_FLAGS(l,r,3,wid,inc,tab_I[3], ddd);		// 60,12(0)
DIS_CENTER_FLAG(l,3,wid,inc,tab_I[3], d_L);			// 60,12(0)
DIS_CENTER_FLAG(r,3,wid,inc,tab_I[3], d_R);			// 60,12(0)

//////DIS_CENTER_CONCS(l,r,2,inc,tab_I[3], 0);	//////tab_I[4]);
DIS_CENTER_CONCS(l,r,2,inc,tab_I[3],tab_I[4]);	// RESTORE (SEPULENIE 28.01.93)
								// 60,12

if (d_L || d_R)  {	// 02.02.1993	PROBA (ABOUT GLISTA)	//  OR ??????
CALC_LENGTH_BEAM_FLAGS (l,r,wid,s, t);
DIS_BEAM (l,r,1,t,wid==1 ? tab_I[6]<<1 : tab_I[6]);		// 140,70
DIS_BEAM (l,r,2,t,wid==1 ? tab_I[6]<<1 : tab_I[6]);	// 18.02.1993  MK PROBA
DIS_BEAM_MK (l,r,2, t, wid==1 ? tab_I[16]<<1 : tab_I[16]);  	// 28,14  25.02.1993: MK
DIS_BEAM (l,r,3,t,wid==1 ? tab_I[6]<<1 : tab_I[6]);		// 140,70
if (s->width<=4  &&  !inc  &&	// add 09.06.1993 about added thin '(' or ')'
    (l->mount[2] && r->conc[2] && 		// '(' LEFT
     (s->dis_left_brace==0) &&
     (l->m_meandr==1) && (r->c_meandr==1)  ||
     l->conc[2] && r->mount[2] && 		// ')' RIGHT
     (s->dis_right_brace==0) &&
     (l->c_meandr==1) && (r->m_meandr==1)))
	dis += tab_I[17];					// 6
}
else if (l->conc[0]  && r->conc[0]  &&	// 19.11.1993 GLISTA  similar 't'
	 l->mount[1] && r->mount[1] &&	// g14/32(33) "fastest" first 't'
	 wid < 5  &&			// g14/34(37) "Kit"
	 abs (l->mb_pos[1] - s->base_2) < 2  &&
	 abs (r->mb_pos[1] - s->base_2) < 2)
		dis += 20;					// 20

if ( wid<3 )  { l->c_meandr--;  r->c_meandr--; }	// RESTORE c_meandr !!!

if( (r->mount[3] && r->mount[4]) && l->mount[4]==0 )
	dis += ( r->down_hook + l->up_hook )*tab_I[5];		// *60
	/* left up or right down hooks */

//////if( s->base_2<2 && s->base_2>-1 )	DELETE 28.01.1993  MK
//////	dis += tab_I[11] ;	/* short stick ??? */		// 70

if( wid*2 > s->height )
	dis += tab_I[12];					// 80

if( l->mount[1]>0 && r->mount[1]>0 && r->mount[4]>0 && sl==1 && sr==2 )
	dis += tab_I[13];	/* similar 't' */		// 80

			// 12.02.1993	thin LEG exist:
if ( l->conc[4] && r->conc[4] &&	// j1/16 "CURTIS": 'R'=>"II"
     (l->ce_pos[4] - l->cb_pos[4] + r->ce_pos[4] - r->cb_pos[4]) >= (dy>>1) )
	dis += tab_I[15];					// 20

			// 19.11.1993	I,l  similar 'E' ("THEATRE")
if (wid*2+1>=dy &&	// k11/31 "PERFORMERCE", last 'E' 21*11, wid=10
    r->conc[1] && r->conc[3] &&
    (r->conc[1] | r->conc[3]) > 1)	// all cases, exclude (1,1)
	dis += 100;						// 100
/*......................................................................*/
//////m_ex:
					// 17.01.1994 MOVE IT TO END:
	if (dis==0)  dis += mk_dis_for_liga_exm;	// (0 or 2);

	return(dis);
}
/*----------------------------------------------------------------------*/
int16_t dis_J (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  {	// 18.10.1993
int16_t	dis=0, t;
int16_t	wid=s->stick_width, inc=s->inc;

t = MAX(l->mount[3],l->mount[4]) ;
if ( t==0 )
	dis += tab_J[0];	// not exist left down arc		// 100
else if (t*2<wid && r->conc[4]*2<wid)		// 09.07.1993
	/********************************************************************
......|| inc && r->conc[4]*2<wid  ||		// 18.10.1993 (h5/38)  PROBA!
	 inc && r->conc[4]==0)	// NELEPO !!!	// 18.10.1993 (g5/8, h19/21)
	 ********************************************************************/
		dis += 88;		// MK: PROBA-88			// 88
else if	(inc && r->conc[4]*2<wid)		// 21.10.1993
		dis += (s->cut_l) ? 88 : 46;				//88/46
else if ( t==1 )  {		// too small left near arc
	if( l->mount[0]==1 && r->mount[0]==1 )
		dis += tab_J[1];	/* up serif */			// 20
	else
		dis += tab_J[2];	/* no serif */			// 80
	}
else if (t<wid && !l->up_serif && !r->up_serif &&	// 28.07.1993
//////	 (wid<7)  &&			// stde17/7  "Jazz" 4/7  // 28.07.1993
	 (wid<6)  &&			// stdk12/13 "N.J." 4/6	 // 05.08.1993
	 (wid<4 || (t+1)<wid))		// see stdk18:  'J' 3/4 and 'l' 2/3 !!!
		dis += tab_J[2];	// "no serif"			// 80

		// NB: IT WAS NELEPO {wid<6 && wid<4}; I must edit it !!!
								// MK 05.08.1993


DIS_DIFFERENT_SERIFS(l,r,0,4,wid,tab_J[3]);				// *6

DIS_CENTER_FLAGS(l,r,1,wid,inc,tab_J[4],tab_J[5]);		// *40/6
if( language==LANG_RUSSIAN && langSer )
  {
  int16_t dis0=dis;
  DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_J[4],tab_J[5]);    // *40/6
  if( dis>dis0 )
    dis = dis0+20;
  }
else
  {
  DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_J[4],tab_J[5]);    // *40/6
  }
DIS_CENTER_FLAG(r,3,wid,inc,tab_J[4],tab_J[5]);     // *40/6

//////if (r->down_serif)		// 30.06.1993: frab12/2 "travail" ('l')
//////		dis += 222;						// 222
if (r->down_serif && t<r->mount[4]*2)	// 28.07.1993: frae6/27 "Jean"
		dis += 122;						// 122

return(dis);
}
/*----------------------------------------------------------------------*/
int16_t dis_L (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  {
int16_t	dis=0;
int16_t	wid=s->stick_width, inc=s->inc, dy=s->height;

if ( l->up_serif==0 && r->up_serif==0 &&	// 09.07.1993	frab6/16(20)
     l->mount[0]    && r->mount[0] )		// "t." with beam in ZONE-0
	DIS_CENTER_FLAGS(l,r,0,wid,inc,tab_L[0],tab_L[1]);	    // *40/*6
//////	DIS_CENTER_FLAGS_1(l,r,0,wid,inc,tab_L[0],tab_L[1]);	??? // *40/*6

//////if(r->up_serif==0 && l->up_serif==0 )
//////	DIS_CENTER_FLAGS(l,r,1,wid,inc,tab_L[0],tab_L[1]);	// 26.01.1993:
	DIS_CENTER_FLAGS_1(l,r,1,wid,inc,tab_L[0],tab_L[1]);	    // *40/*6

DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_L[0],tab_L[1]);		    // *40/*6

//////if( r->up_serif && l->up_serif )	// before 28.05.1993
//////if( r->up_serif | l->up_serif )	// 28.05.1993
//////if (r->up_serif)			// 01.06.1993: for '[' !!!
if (r->up_serif && !l->up_hook)		// 18.11.1993: fax41/21(63) "APPLICATION"
	DIS_DIFFERENT_SERIFS(l,r,0,4,wid,tab_L[2]);		     // *70

//////if( MAX(r->mount[3], r->mount[4])<wid )		// OLD OLEG
//////if (MAX(r->mount[3], r->mount[4]) < MIN(wid,8))	// 09.07.1993
//////if (MAX(r->mount[3], r->mount[4]) < MIN(wid,5))	// 27.07.1993 VAR-1
//////if (MAX(r->mount[3], r->mount[4]) < MIN(wid,5) + l->mount[4]) //VAR-2
			// see	fax9/21(20) "ts",  frad9/1 'L',  stdh15/1 "ti",
			// 	g14/62(74) "Library", stdk18/8 "ARTICLES":
if (MAX (r->mount[3], r->mount[4]) - l->mount[4] <		// 28.07.1993
////MIN (wid,(s->cut_r ? 8 : 5) ) 				// VAR-1
////MIN (wid,((s->cut_r || s->neck>=2) ? 8 : 5)))		// VAR-2
////MIN (wid,5) + (s->cut_r ? 3 : 0) + s->neck)			// VAR-3
////MAX (wid,4) - 1 + (s->cut_r ? 2 : 0) + s->neck)		// VAR-4
    MAX (wid,4) - 1 + (s->cut_r ? 2 : 0) + s->neck +		// 12.10.1993
    (dy>20 ? 1 : 0) + (l->conc[4] ? 1 : 0)  &&
////r->mb_pos[4]*4>dy*3)	// 21.10.1993 PROBA for i5/5 "LATEST"; R.M.3-?
////r->mount[4]  &&  r->mb_pos[4]*4>dy*3)	// 21.10.1993, see g4/1 "1Soft"
////(!r->mount[4] || r->mb_pos[4]*4>dy*3))	// 21.10.1993, EVENING
    (!r->mount[4] || r->mb_pos[4]*4>dy*3 || dy<20))	// 22.10.1993
	dis += tab_L[3];	// too small right down flag		// 70
				// NB:	wid:  1  2  3  4  5  6  7  8  9
				//	max:  4  4  4  4  5  6  7  8  9
				//	 -1:  3  3  3  3  4  5  6  7  8
				// CUT.R ? :  5  5  5  5  6  7  8  9 10
				// NB: compare (R-L) with this !!!

			/******************************	DELETE 15.10.1993
			// e15/19 "LAN" ('A'=>'4')	31*29	7/19	(+)
			// h3/32  "L. Steele"		24*25	5/19	(+)
			// b8/1	  "PUBLISHERS"		31*30	6/22	###
			// fax19/25  "CALS"		18*19	4/12	###
			// j1/44  "TAJ MAHAL"		32*35	14/21	###
			// h10/11 "C R I S T A L"	28*28	8/19	###
if ((MIN(wid,6) + wid + r->mount[4]) > s->height)		// 12.10.1993
	dis += tab_L[7];	// too int32_t right down flag		// 100
	*********************************************************************/

					// 12.10.1993 j1/21 "McCULL", first 'L'
//////DIS_CENTER_FLAGS(l,r,2,128,inc,tab_L[4],tab_L[5]);	   // *60/*12
DIS_CENTER_FLAGS_2(l,r,2,wid,inc,tab_L[4],tab_L[5]);		   // *60/*12

if( r->mount[4]<=wid+1 && s->neck>=2 )		// m.b. NECK=3
	dis += tab_L[6];						// 40

if (r->conc[2] >= MAX(wid,4)-1)			// 02.06.1993	PROBA:
//////	dis += 68;	// PROBA-68					// 68
	dis += 80;	// PROBA-80					// 80

return(dis);
}
/*----------------------------------------------------------------------*/
int16_t dis_T (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t	dis=0, u, d;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	tl, tr;		// NEW: 20/23.01.1992
int16_t	porog;

				// MK 20/22.01.1993	// UP HOR ASSIMETR:
				// see fax27/13 {"This},  fax13/20: {"This}
	tl = l->mount[0];	// m.b. use MAX (l->mount[0], [1]) ???
	tr = r->mount[0];	// m.b. use MAX (l->mount[0], [1]) ???

			// EXTRAPOLATION ASSIMETRY BAD PROPORTION:
//////	if ( (2*MAX (tl, tr) + wid) >		// before 27.07.1993
//////	if ( (abs(tl-tr)*2 > wid + 1)  &&	// 27.07.1993: ASSIMETR !!!

  if ( (abs(tl-tr) > MAX(wid,5)-3)  &&  // 28.07.1993: ASSIMETR !!!
	     (2*MAX (tl, tr) + wid) >
	     (s->height + 2 +
	      (s->height>20) +
	      (s->height>27) +
//////	      (wid>10)*4 +	// stdj1/44(45) "TAJ MAHAL" 'T'=>'Y' DEL.27.07.1993
	      (fax1x2<<1)) )
		dis += tab_T[9];				// 100
  else if( s->height>25 && MAX(r->mount[0],r->mount[1])<3 && tl>6 )
    dis += tab_T[9];        // 04.08.1995: ASSIMETR !!! similar 1

	porog = MAX (((wid + 1) >> 1), 2);		// MK 25.01.1993
	if ( (s->height>=34) && (porog<4) ) porog = 4;	// MK 26.01.1993
//////	if ( (s->T_config==0) && (s->T_3==0) ) porog = 1;  // MK 01.02.1993
				// see ok24/1 "Teleform" T.0,2,0,0
	if ( s->T_2 > porog )
		dis += s->T_2 * tab_T[7];				// *20

//////	if ( !s->T_skok_L || !s->T_skok_R )
//////		dis += tab_T[10];			////// 80, 100, ......

	if (inc && !s->T_skok_R  &&		// 18.11.1993 similar '7'
	    l->me_pos[0] + 2 <= r->me_pos[0])	// j10/29 "08/77" (first '7')
		dis += tab_T[10];					// 60
/*......................................................................*/
//////DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_T[0],tab_T[1]);		//40,6
if (l->mount[2]<tl)			// 24.03.1993 for fax27/19(21) "The"
	DIS_CENTER_FLAG(l,2,wid,inc,tab_T[0],tab_T[1]);			//40,6
if (r->mount[2]<tr)
	DIS_CENTER_FLAG(r,2,wid,inc,tab_T[0],tab_T[1]);			//40,6

if( l->down_serif && r->down_serif==0 || l->down_serif==0 && r->down_serif  )
	DIS_CURVE(l,r,4,1,tab_T[2]);					// 10

//////DIS_DIFFERENT_SERIFS(l,r,0,4,wid,tab_T[3]);	//// UP OLD
DIS_DIFFERENT_SERIFS(l,r,4,4,wid,tab_T[3]);		// DOWN		// 6

u = MAX(l->mount[0],l->mount[1]);
d = MAX(l->mount[3],l->mount[4]);
if( language==LANG_RUSSIAN &&  s->cut_l && abs(u-d)<1 ||
    language==LANG_RUSSIAN && !s->cut_l && abs(u-d)<2 ||
    language!=LANG_RUSSIAN &&              abs(u-d)<2 )
  dis += tab_T[4];   /* similar 'I' */        // 60

u = MAX(r->mount[0],r->mount[1]);
d = MAX(r->mount[3],r->mount[4]);
if( language==LANG_RUSSIAN &&  s->cut_r && abs(u-d)<1 ||
    language==LANG_RUSSIAN && !s->cut_r && abs(u-d)<2 ||
    language!=LANG_RUSSIAN &&              abs(u-d)<2 )
	dis += tab_T[4];   /* similar 'I' */				// 60

			// NB: here were also OLEG for Y_config

return	(dis);
}
/*----------------------------------------------------------------------*/
int16_t dis_Y (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t	dis=0;
int16_t	wid = s->stick_width, inc = s->inc;

DIS_CENTER_FLAGS (l, r, 2, wid, inc,tab_Y[0], tab_Y[1]);
DIS_CENTER_FLAGS (l, r, 3, wid, inc,tab_Y[0], tab_Y[1]);

if( l->down_serif==0 && r->down_serif==0 )
	DIS_CENTER_FLAGS (l, r, 4, wid, inc, tab_Y[0], tab_Y[1]);

				// MK 21.01.1993
if (s->T_3)					// There are 3-intervals Lines
	dis += s->T_3 * tab_Y[4];					// *40
  else  {
	if (s->T_config)				// REPEAT of OLD ALGHORITM
		dis += tab_Y[2];					// 80
	if (s->T_2<2)		// 25.10.1993	// NO 2- or 3-intervals Lines
		dis += tab_Y[4];					// 40
	}

//////if (s->T_skok)	dis += tab_Y[5];
if ( s->T_skok_L | s->T_skok_R )	dis += tab_Y[5];	// 80

if( l->mount[0] && r->mount[0] && !inc )  {
	 int16_t lh,rh;
	 rh = r->me_pos[0]-r->mb_pos[0];
	 lh = l->me_pos[0]-l->mb_pos[0];
	 if( rh*3<lh || lh*3<rh )   /* T with cutting left or right ear */
		dis += tab_Y[3];
	 }

return(dis);
}
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* letters '(',')'  */
int16_t dis_circle_brace(STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s, int16_t typ)
{
int16_t dis=0;
int16_t inc = s->inc;
int16_t sl=l->num_flags,sr=r->num_flags,wid=s->stick_width;

if( l->up_serif && r->up_serif &&
    ( !inc || l->down_serif + r->down_serif>2 && !inc)  )
	dis += tab_circle_brace[0];		/* up serif	*/
if( l->down_serif && r->down_serif &&
    ( !inc || l->down_serif + r->down_serif>2 && !inc) )
	dis += tab_circle_brace[0];		/* down serif	*/

if( r->mount[0] && r->mount[4] )		/* '('		*/
	{
	if( r->up_serif==0 && r->down_serif==2 )
		dis += tab_circle_brace[1];	/* similar 't' */
	}

if( sl==1 && sr==0 && (l->mount[0] || l->mount[1]) )  /* ')' */
	dis += tab_circle_brace[2];		/* similar '1' */

if( typ ) /* left */
	{
	if( l->conc[0]==0 && l->conc[4]==0 && l->mount[2]==0 )
		dis += tab_circle_brace[3];
		/* no conc in top and bootom left angles     */
	if( sl==0 && sr==0 && r->conc[2]==0 )
		dis += tab_circle_brace[4];	/* solid stick */
	if( s->cut_r )
		dis  += tab_circle_brace[5];  /* ( : not cut right !  O=(+)*/
	if( s->cut_l )
		dis  += mk_80_for_CUT;		// PROBA 24.06.1993
	if( wid<7 && (r->mount[0]==0 && r->mount[1]>1 ||
		      r->mount[4]==0 && r->mount[3]>1)  )
		dis += tab_circle_brace[6];	/* bad bugles in 1,3 zones  */
	dis += s->dis_left_brace;		/* primary dis	*/
	}
else		/* right */
	{
	if( r->conc[0]==0 && r->conc[4]==0 && r->mount[2]==0 )
		dis += tab_circle_brace[3];
		/* no conc in top and bootom right angles */
	if( sl==0 && sr==0 && l->conc[2]==0 )
		dis += tab_circle_brace[4];	/* solid stick */
	if( s->cut_l )
		dis  += tab_circle_brace[5];	/* ) : not cut left !  O=|+)*/
	if( s->cut_r )
		dis  += mk_80_for_CUT;		// PROBA 24.06.1993
	if( wid<7 && (l->mount[0]==0 && l->mount[1]>1 ||
		      l->mount[4]==0 && l->mount[3]>1)  )
		dis += tab_circle_brace[6];	/* bad bugles in 1,3 zones  */
	dis += s->dis_right_brace;		/* primary dis */
	}

return(dis);
}
/*----------------------------------------------------------------------*/
/* letter '<'	*/				// MK-FARA 17.06.1993
int16_t dis_sign_less (STICK_SIGNUMS *s)			// 24.06.1993
{
int16_t dis=0;

	if( s->cut_r )				// 17.06.1993	MK-FARA
		dis  += mk_80_for_CUT;		// see tab_circle_brace[5];

	if( s->cut_l )				// 24.06.1993
		dis  += mk_80_for_CUT;

return(dis);
}
/*----------------------------------------------------------------------*/
/* letter '>'	*/				// MK-FARA 17.06.1993
int16_t dis_sign_more (STICK_SIGNUMS *s)			// 24.06.1993
{
int16_t dis=0;

	if( s->cut_l )				// 17.06.1993	MK-FARA
		dis  += mk_80_for_CUT;		// see tab_circle_brace[5];

	if( s->cut_r )
		dis  += mk_80_for_CUT;	// 24.06.1993

return(dis);
}
/*----------------------------------------------------------------------*/
/* letter [ */						// 22.11.1993
int16_t dis_l_sq_brace(STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t	dis=0, lmu, rmu, lmd, rmd, dy=s->height;
int16_t	wid=s->stick_width, inc=s->inc;

lmu = l->mount[0];  rmu = r->mount[0];
lmd = l->mount[4];  rmd = r->mount[4];

if (rmu==0 ||				// 29.09.1993
    rmu==1 && wid>4)  {
	dis += tab_sq_brace[0];		// no upper right flag	// 4 => 44
	if (r->mount[1]>=2)		// MK ADD;
		dis += tab_sq_brace[5];	// for 't' => '['	// +88 = 132
	}

else
  if (r->m_pos[0]>=2  &&	// 29/30.09.1993  for 't'=>'[' MK ADD:
      r->mb_pos[0]>0)  {	// 19.11.1993 for j2/14 '[' (0-2-4)
	dis += tab_sq_brace[0];		// FALSE U.R. FLAG	// 44
	if (r->m_pos[0]>=3)
		dis += tab_sq_brace[0];	// more FALSE;		// 44
	if (r->mb_pos[0]>=2)
		dis += tab_sq_brace[5];	// more FALSE		// 88
//////	if ((l->conc[5]) && (dy - l->cb_pos[5] >= 2))	#############
	if ((l->conc[4]) && (dy - l->cb_pos[4] >= 2))	// 19.11.1993
		dis += tab_sq_brace[5];	// more FALSE		// 88
	}

else if (rmd-rmu>=2  &&		// 29.09.1993 for stdc20/13 "bit"
//////	 ((l->conc[5]) && (dy - l->cb_pos[5] >= 2)))	#############
	 ((l->conc[4]) && (dy - l->cb_pos[4] >= 2)))	// 19.11.1993
		dis += tab_sq_brace[5];	// for 't' => '['	// 88

//////else	DELETE 29.09.1993
if( lmu>=rmu )
	dis += (lmu-rmu+1)*tab_sq_brace[1]; /* bad paar flags */	// *8

if ( rmd==0 )
	dis += tab_sq_brace[0];		// no downer right flag	// 4 => 44

//////else	DELETE 29.09.1993
if ( lmd>=rmd )
	dis += (lmd-rmd+1)*tab_sq_brace[1]; /* bad paar flags */	// 8

DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_sq_brace[2],tab_sq_brace[3]);	// 20,8

if( l->conc[0]>1 && l->ce_pos[0]>1 )
	dis += tab_sq_brace[4];		/* similar '(' */		// 26
if( l->conc[4]>1 && l->cb_pos[4]<dy-1 )
	dis += tab_sq_brace[4];		/* similar '(' */		// 26
if (wid==1 && l->conc[0] && l->conc[4])	// 22.11.1993 f11/21 "(03)"
	dis += tab_sq_brace[4];		// similar '('			// 26

if ( l->conc[3]>1 && r->conc[3]>1 )	// Oleg : 05.17.1994 pdem0/18
	dis += tab_sq_brace[4];		// similar '!'

	if( s->cut_l )	dis  += mk_80_for_CUT;		// 24.06.1993
	if( s->cut_r )	dis  += mk_80_for_CUT;

/********************************* 19.11.1993 DELETE IT (PROBA)
if ((rmu==1 && (rmd)) ||	// 29.09.1993 for stdh6/7 'l' ("celebrate")
    (rmd==1 && (rmu)))  {
	if (wid>=3)
		dis += tab_sq_brace[0];	// WIDE STICK with THIN		// 44
	if (r->num_flags>2)
		dis += tab_sq_brace[5];	// WIDE STICK with THIN		// 88
	}
	*******************************************************/

return(dis);
}
/*----------------------------------------------------------------------*/
/* letter ] */						// 29.09.1993	MK
int16_t dis_r_sq_brace(STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t dis=0,lmu,rmu,lmd,rmd,dy=s->height;
int16_t wid=s->stick_width,inc=s->inc;

lmu = l->mount[0];  rmu = r->mount[0];
lmd = l->mount[4];  rmd = r->mount[4];

if (lmu==0)  {
	dis += tab_sq_brace[0];		// no left upper flag	// 4 => 44
	if (lmd<=1)
		dis += tab_sq_brace[6];	// TERRIBLE			// 222
	}

//////else	DELETE 29.09.1993
if (rmu>=lmu)
	dis += (rmu-lmu+1)*tab_sq_brace[1]; /* bad paar flags */

if (lmd==0)  {
	dis += tab_sq_brace[0];		// no left downer flag	// 4 => 44
	if (lmu<=1)
		dis += tab_sq_brace[6];	// TERRIBLE			// 222
	}

//////else	DELETE 29.09.1993
if (rmd>=lmd)
	dis += (rmd-lmd+1)*tab_sq_brace[1]; /* bad paar flags */

DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_sq_brace[2],tab_sq_brace[3]);

if( r->conc[0]>1 && r->ce_pos[0]>1 )
	dis += tab_sq_brace[4];		/* similar ')' */
if( r->conc[4]>1 && r->cb_pos[4]<dy-1 )
	dis += tab_sq_brace[4];		/* similar ')' */

dis +=  s->dis_up_dot;			/* similar 'j' */

	if( s->cut_l )	dis  += mk_80_for_CUT;		// 24.06.1993
	if( s->cut_r )	dis  += mk_80_for_CUT;

if (wid>=4  &&			// 29.09.1993 for stdb13/35 'l' ("cornmeal")
    (lmu==1 && (lmd)) ||
    (lmd==1 && (lmu)))
	dis += tab_sq_brace[0];		// WIDE STICK with THIN		// 44

if (r->up_serif==2)
	dis += tab_sq_brace[0];						// 44

if (r->down_serif==2)
	dis += tab_sq_brace[0];						// 44

if (r->num_flags>=3)
	dis += tab_sq_brace[0];						// 44

if (wid<=2  && r->num_flags>=2 && l->num_flags>=3)
	dis += tab_sq_brace[0];						// 44

return(dis);
}
/*----------------------------------------------------------------------*/
int16_t	dis_figa_left (STICK_SIGNUMS *s)  {		// 30.09.1993	'{'
int16_t	dis=0;				// NOTA BENE: see ST_TOOLS.C:
					// by comments - it was some
					// work with {} about CUT,
					// but may be in some spec cases
					// (zB if NO STICK) ??????????????
	if (s->cut_l)	dis += 80;
	if (s->cut_r)	dis += 80;
	return (dis);
}
/*----------------------------------------------------------------------*/
int16_t	dis_figa_right (STICK_SIGNUMS *s)  {		// 30.09.1993	'}'
int16_t	dis=0;
	if (s->cut_l)	dis += 80;
	if (s->cut_r)	dis += 80;
	return (dis);
}
/*----------------------------------------------------------------------*/
int16_t dis_slash (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  {
int16_t	dis=0;						// 13.12.1993
////int16_t	wid = s->stick_width;	NOT USED NOW
int16_t	inc=s->inc;

dis += s->dis_slash;  /* primary discrim  */

DIS_CENTER_FLAG(l,0,128,inc,tab_slash[0],tab_slash[1]);		// 20, 0
DIS_CENTER_FLAG(r,0,128,inc,tab_slash[0]>>2,tab_slash[1]>>2); /* inc! */

DIS_CENTER_FLAGS(l,r,1,128,inc,tab_slash[0],tab_slash[1]);	// 20, 0
DIS_CENTER_FLAGS(l,r,2,128,inc,tab_slash[0],tab_slash[1]);	// 20, 0
DIS_CENTER_FLAGS(l,r,3,128,inc,tab_slash[0],tab_slash[1]);	// 20, 0

DIS_CENTER_FLAG(r,4,128,inc,tab_slash[0],tab_slash[1]);		// 20, 0
DIS_CENTER_FLAG(l,4,128,inc,tab_slash[0]>>2,tab_slash[1]>>2); /* inc! */

/******************************************************************
if ( s->cut_l | s->cut_r  )		// OLD OLEG;
	{
	DIS_BEAM(l,r,1,2,wid==1 ? tab_slash[2]<<1 : tab_slash[2]) ;	// 40
	DIS_BEAM(l,r,3,2,wid==1 ? tab_slash[2]<<1 : tab_slash[2]) ;	// 40
	}
	***********************************************************/
if ( s->cut_l | s->cut_r  )  {		// 13.12.1993 PROBA for j11/36 "/1"
	DIS_BEAM (l, r, 1, 2, tab_slash[2]) ;				// 40
	DIS_BEAM (l, r, 3, 2, tab_slash[2]) ;				// 40
	}
else	{		// 18.11.1993 for j8/18(20) "of Defense", first 'f'
	if (l->mount[2] && r->mount[2]>=2 &&
	    abs (r->m_pos[2] - s->base_2) < 3)
		dis += tab_slash[2];					// 40
	// NOTA BENE: zones 1,3 and other beam's lens - to future...
	}

return(dis);
}
/*----------------------------------------------------------------------*/
/* letter '!'								*/
/*      add_flag = 1 - put '!' to list of versions - delicate discrim	*/
int16_t dis_vert(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *signums,
		    int16_t add_flag, char typ)
{
int16_t dis=0,l_beam = add_flag ? 3 : 5;
int16_t inc = signums->inc;
int16_t rc=MAX(r->conc[3],r->conc[4]),lc=max(l->conc[3],l->conc[4]);
int16_t	s=signums->stick_width;
int16_t L_max=s>>1;	/* 1/2 stick width */
int16_t rmd,rmu,lmd,lmu;
int16_t r3,r4,l3,l4,ss,m;

if( language == LANG_RUSSIAN )
{
if( !add_flag )
	{
	DIS_BEAM(l,r,0,l_beam,tab_vert[0]) ;
	DIS_BEAM(l,r,1,l_beam,tab_vert[0]) ;
	if( typ!='!' )	/* ! with dot */
	{
	if( s<4 && (r->conc[3]||l->conc[3]))	m=1;
	else	m =  ( r->conc[3]&&l->conc[3]);
	ss = signums->height-1;
	r3=(r->conc[3]==0)||(r->conc[3]!=0&&r->ce_pos[3]==ss);
	r4=(r->conc[4]==0)||(r->conc[4]!=0&&r->ce_pos[4]==ss);
	l3=(l->conc[3]==0)||(l->conc[3]!=0&&l->ce_pos[3]==ss);
	l4=(l->conc[4]==0)||(l->conc[4]!=0&&l->ce_pos[4]==ss);
	if( (r3&&r4 || l3&&l4) && !m )
		{
		int16_t bon = ( r->conc[3]>1 || l->conc[3]>1 )?10:0;
		dis += tab_vert[6]-bon;   /* no neck */
		}
	}
	}
else
	{
	if( r->mount[0]<r->mount[1] || l->mount[0]<l->mount[1] )
		{
		DIS_BEAM(l,r,0,l_beam,tab_vert[0]) ;
		DIS_BEAM(l,r,1,l_beam,tab_vert[0]) ;
		}
	if(  (r->mount[0]>L_max || r->mount[1]>L_max) &&
		l->conc[1]>L_max )
		dis +=  tab_vert[6];

	}

if( !( (rc||lc) && (s-lc<2 || s-rc<2) ) )	/* not exist down neck !!! */
	DIS_BEAM(l,r,4,3,tab_vert[0]) ;         /* 10.12.1993		   */
if( signums->incline>40*16*8 )
        dis += tab_vert[4];             /* similar to '/' */
DIS_HALF_SERIF(l,r,0,1,tab_vert[5]); /* test upper serif  */
if( typ=='!'  )
DIS_HALF_SERIF(l,r,4,1,tab_vert[5]); /* test downer serif */
if( signums->width*2>signums->height )
	dis += tab_vert[4];
}
else
{
DIS_BEAM(l,r,0,l_beam,tab_vert[0]) ;
DIS_BEAM(l,r,1,l_beam,tab_vert[0]) ;
DIS_BEAM(l,r,4,3,tab_vert[0]) ;
}
DIS_CENTER_FLAGS(l,r,2,128,inc,tab_vert[1],tab_vert[2]);
if( r->mount[4]>1 && l->mount[4]==0 || l->mount[4]>1 && r->mount[4]==0)
	dis += tab_vert[3];   /* down flags */
if( typ=='!' && r->mount[0]>5 && l->mount[0]<2 || l->mount[0]>5 && r->mount[0]<2 )
	dis += tab_vert[3];   /* down flags */

lmu = MAX(l->mount[0],l->mount[1]);  rmu = max(r->mount[0],r->mount[1]);
lmd = l->mount[4];  rmd = r->mount[4];

if( lmu+rmu+lmd+rmd != 0 )
{                              // Oleg : 11-07-1994 : big carrot
if( rmd<2 && lmd<2 && rmu>3 && !(lmu>rmu-1&&rmu>=4))
	 dis += 40*(rmu-1);	/* exist right upper flag */

}


dis += signums->num_lines;

return(dis);
}

/*----------------------------------------------------------------------*/
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟		functions for wide sticks			╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/*----------------------------------------------------------------------*/
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟		auxiliary functions				╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/*----------------------------------------------------------------------*/
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟		functions for adding new versions		╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/*----------------------------------------------------------------------*/
int16_t similar_excl(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
{
return(dis_vert(l,r,s,1,'!')<MIN_ADD_DIS);
}
/*----------------------------------------------------------------------*/
int16_t similar_excl_with_dot(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
{
return(dis_vert(l,r,s,1,(char)liga_exm)<MIN_ADD_DIS+2 );
}
/*----------------------------------------------------------------------*/
/* similar left circle brance */
int16_t similar_lcb(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
{
return( dis_circle_brace(l,r,s,1)<MIN_ADD_DIS );
}
/*----------------------------------------------------------------------*/
/* similar right circle brance */
int16_t similar_rcb(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
{
return( dis_circle_brace(l,r,s,0)<MIN_ADD_DIS );
}
/*----------------------------------------------------------------------*/
uchar similar_wide_frt1(STICK_CHARS *left_chars,STICK_CHARS *right_chars)
{
uchar ret=0;
ret |= config_f(left_chars,right_chars);
ret |= config_r(left_chars,right_chars)<<2;
ret |= config_t(left_chars,right_chars)<<4;
ret |= config_1(left_chars,right_chars)<<6;
return(ret);
}
/*----------------------------------------------------------------------*/
/* rough propability of letter 'f' */
static uchar config_f( STICK_CHARS *l,STICK_CHARS *r)
{
uchar ret=3,lf=(l->mount[0]!=0)+(l->mount[1]!=0)+(l->mount[2]!=0),
	rf=(r->mount[0]!=0)+(r->mount[1]!=0)+(r->mount[2]!=0),
	br_l=(r->mount[0]&&r->mount[4]&&l->mount[2]);
if( rf<2 && r->m_meandr==2 && r->mount[4]==0 && r->mount[3]==0 )
	rf=2;
if( lf==1 && rf==2 || rf>1 && l->mount[4] ||
    lf<=1 && l->conc[0] && r->mount[0]  &&
	     !(l->conc[4]&&r->mount[4]) &&
	     !r->conc[2] && !br_l
    )
	ret=2;
else if( lf>1 || lf==1 && rf==1  )
	ret=1;
return(ret);
}
/*----------------------------------------------------------------------*/
/* rough propability of letter 'r' */
static uchar config_r( STICK_CHARS *l,STICK_CHARS *r)
{
uchar ret=3,lf=(l->mount[0]!=0)+(l->mount[1]!=0)+(l->mount[2]!=0),
	rf=(r->mount[0]!=0)+(r->mount[1]!=0);
if( lf==0 && rf==1 )
	ret=r->mount[0]?2:1;
else if( lf==1 && l->mount[0] && r->mount[0]>l->mount[0] )
	ret=1;
return(ret);
}
/*----------------------------------------------------------------------*/
/* rough propability of letter 't' */
static uchar config_t( STICK_CHARS *l,STICK_CHARS *r)
{
uchar ret=3,lf=(l->mount[0]!=0)+(l->mount[1]!=0)+(l->mount[2]!=0),
	rf=(r->mount[0]!=0)+(r->mount[1]!=0)+(r->mount[2]!=0);

if( lf==1 && l->m_meandr==1 && rf==1 )
	ret=2;
else if( lf==1 && rf==2 || rf==1 && lf==0 )
	ret=1;
return(ret);
}
/*----------------------------------------------------------------------*/
/* rough propability of letter '1' */
static uchar config_1( STICK_CHARS *l,STICK_CHARS *r)
{
uchar ret=3;

if( r->mount[0]==0 && r->mount[1]==0 && r->mount[2]==0 &&
    r->mount[3]==0 && r->mount[4]==0 && l->mount[2]==0 &&
    l->mount[3]==0 && l->mount[4]==0 )
    {
    if( l->mount[1]==0 && l->mount[0] )
	ret = 2;
    else if( l->mount[0]==0 && l->mount[1] )
	ret = 1;
    }
return(ret);
}
/*----------------------------------------------------------------------*/
int16_t dis_RusG (STICK_CHARS *l, STICK_CHARS *r,STICK_SIGNUMS *signums)
{
int16_t     dis=0, u, d, t;

u = MAX(l->mount[0],l->mount[1]);
d = MAX(r->mount[0],r->mount[1]);
t = abs(u-d);
if( t<2 )
	dis += 60;   /* similar 'T' : upper halfserifs */
else if( t<3 )
	dis += 30;

u = MAX(r->mount[0],r->mount[1]);
d = MAX(r->mount[3],r->mount[4]);
t = abs(u-d);
if( t<2 )
	dis += 60;   /* similar 'I' : right halfserifs */
if( u<signums->height/4 )
	dis += 60;	/* similar 'I'  or left part broken '▐' */

if( language==LANG_RUSSIAN && multy_language )
  {
  if( r->mount[1]==0 && r->mount[2]>2 )
    dis += 30;
  if( l->mount[1]==0 && l->mount[2]>2 )
    dis += 30;
  }

return	(dis);
}
/////////
// Nick 14.09.2000 - special for romanian II_roof_accent
int16_t dis_I_forRoman (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)
{
 int16_t dis=0;

	if( !l->mount[0] || !r->mount[0] )
		return 0;

	if( l->up_hook )
		dis += 20;
	if( r->up_hook )
		dis += 20;

	if( !l->mount[4] ||
		l->me_pos[0]-l->mb_pos[0] >= 2*(l->me_pos[4]-l->mb_pos[4])
      )
		dis+=40;

	if( !r->mount[4] ||
		r->me_pos[0]-r->mb_pos[0] >= 2*(r->me_pos[4]-r->mb_pos[4])
      )
		dis+=40;

    return dis;
}
////////////
