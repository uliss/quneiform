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

/*----------------------------------------------------------------------*/
/*  STIC_DIS.C  18.01.1994                                              */
/*----------------------------------------------------------------------*/
#define	mk_80_for_CUT	80
/*----------------------------------------------------------------------*/



  #include <stdlib.h>
  #include <string.h>

			// here was DEBUG_GRAPH
  #include "func.h"
  #include "status.h"
  #include "stick.h"
  #include "ligas.h"
  #include "lang.h"
  #include "struct.h"
  #include "tuner.h"
  #include "linutil.h"	// 31.05.2002 E.P.
#include "minmax.h"

#ifdef UFA
extern uchar no_linear_crit;
#endif

extern uchar multy_language;

extern int16_t pitchsize ;

extern int16_t nIncline  ;
extern uchar fax1x2;	// MK NEW 05.01.1993
extern int  inc_num_EEM;	// in ST_TOOLS.C
extern int  dis_LIMIT_EEM;	// in ST_TOOLS.C;
extern int  mk_dis_for_liga_exm;	// 06.01.1994
extern uchar left_letter_EEM;	// 17.01.1994
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
#define MAX_ADD_DIS  30		/* max discrim for adding 'є','1'	*/
/*----------------------------------------------------------------------*/
#include "stic-mac.h"
#include "stic-tab.h"
/*----------------------------------------------------------------------*/
						// to STIC_BIG.C:
//////extern	int16_t dis_F(STICK_CHARS *r,STICK_SIGNUMS *s);	// MK
extern  int16_t dis_RusG (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
extern	int16_t dis_F (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);//25.03.93
extern	int16_t dis_I (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
extern	int16_t dis_J (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
extern	int16_t dis_L (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
extern	int16_t dis_T (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
extern	int16_t dis_Y (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);

extern	int16_t dis_circle_brace(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s,
		 int16_t typ);
extern	int16_t dis_sign_less(STICK_SIGNUMS *s);	// MK&FARA 17.06.1993
extern	int16_t dis_sign_more(STICK_SIGNUMS *s);	// MK&FARA 17.06.1993
extern	int16_t dis_l_sq_brace(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
extern	int16_t dis_r_sq_brace(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
extern	int16_t dis_figa_left (STICK_SIGNUMS *s);
extern	int16_t dis_figa_right(STICK_SIGNUMS *s);
extern	int16_t dis_slash(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
extern	int16_t dis_vert(STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s,
		int16_t add_flag, char type);
/*----------------------------------------------------------------------*/
						// to STIC_FRT.C:
extern	int16_t dis_f (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
extern	int16_t dis_r (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
extern	int16_t dis_t (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s,
							int16_t sign_f);
extern	int16_t find_beam (STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long);
extern	int16_t find_neck (STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long);
/*----------------------------------------------------------------------*/
int16_t discrim_stick (uchar let, STICK_CHARS *l, STICK_CHARS *r,
      STICK_SIGNUMS *s, int16_t sign_f);

static int16_t dis_0xBA (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		 int16_t add_flag);
static int16_t dis_0xBB (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
     int16_t add_flag);
//////static int16_t dis_i (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
static int16_t dis_i (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		 int16_t npoints_1_or_2);
static int16_t dis_j (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
static int16_t dis_1 (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		 int16_t add_flag);
static int16_t dis_l (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
static int16_t dis_l_stroked (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s);
static int16_t dis_d_croatian (STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s);
static int16_t dis_d(STICK_CHARS *l,STICK_CHARS *r);
static int16_t dis_7(STICK_SIGNUMS *s);

int16_t similar_0xBA (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t similar_1    (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t similar_l_stroked(STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
/*----------------------------------------------------------------------*/
/* let - code discrim letter, width - optimal wide of stick, 		   */
/* inc = 1 or 0(no inc), (*l,*r) - left and right characteristics of stick */
int16_t discrim_stick (uchar let, STICK_CHARS *l, STICK_CHARS *r,
      STICK_SIGNUMS *s, int16_t sign_f )
{
int16_t dis=0;
#ifdef UFA
if( no_linear_crit )
  {
  left_mode_EEM=0;
  s->neck = 0;
  }
#endif

switch( let )
	{
	case 'f' : dis = dis_f(l,r,s) ;		break;
//////	case 'F' : dis = dis_F(r,s) ;		break;	// 07.01.1993	MK
	case 'F' : dis = dis_F(l,r,s) ;		break;	// 25.03.1993	MK
	case 'r' : dis = dis_r(l,r,s);		break;
	case 't' : dis = dis_t(l,r,s,sign_f);	break;
  case '|' : if(language!=LANG_RUSSIAN )  break;
             if( s->incline>256 )
              dis = dis_i(l,r,s, 1);
             else
              dis = dis_I(l,r,s);
             break;
	case 'I' : dis = dis_I(l,r,s);		break;
	case '1' : dis = dis_1(l,r,s,0);	break;
	case 'l' : dis = dis_l(l,r,s);		break;
	case ']' : dis = dis_r_sq_brace(l,r,s);	break;
	case '[' : dis = dis_l_sq_brace(l,r,s);	break;
	case '{' : dis = dis_figa_left (s);	break;
	case '}' : dis = dis_figa_right(s);	break;
	case '!' : dis = dis_vert(l,r,s,0,'!');	break;
	case 'i' : dis = dis_i(l,r,s, 1);	break;	// 07.12.1993: 1 point;
	case 'j' : dis = dis_j(l,r,s);		break;	// 22.11.1993	MK
	case '(' : dis = dis_circle_brace(l,r,s,1);	break;
	case ')' : dis = dis_circle_brace(l,r,s,0);	break;
	case '<' : dis = dis_sign_less(s);	break;	// MK&FARA 17.06.1993
	case '>' : dis = dis_sign_more(s);	break;
	case '/' : dis = dis_slash(l,r,s);	break;
	case 'J' : dis = dis_J(l,r,s);		break;
  case (uchar)'Ј' :
  case (uchar)'ѓ' :
		if( language==LANG_RUSSIAN )    dis = dis_RusG(l,r,s);
                                                break;

  case (uchar)'в' :

	if (is_baltic_language(language) || // Конфликтный код a_macron 17.07.2001 E.P.
		is_turkish_language(language)	// 21.05.2002 E.P.
		)
		{dis = 0; break;}

  case (uchar)'’' : if( language!=LANG_RUSSIAN )    break;

  case 'T' : dis = dis_T(l,r,s);      break;
  case 'Y' : dis = dis_Y(l,r,s);      break;
  case 'L' : dis = dis_L(l,r,s);      break;
  case '7' : dis = dis_7(s);          break;

  // Конфликтный код 08.09.2000 E.P.
  case liga_i_usual :       /* i + point */
	if (liga_i_usual == liga_i)
          dis = dis_0xBA(l,r,s,0);
	break;

  case liga_i_latin:       /* i + point */
	if (liga_i_latin == liga_i)
          dis = dis_0xBA(l,r,s,0);
	break;

  case liga_j :   /*  '»' */        /* 0xBB <-> j + point */
             dis = dis_0xBB(l,r,s,0); break;

  // 31.08.2000 E.P.
  case liga_exm_usual :   /*  '!' */        /* 0xBC <-> ! + point */
	if (liga_exm_usual==liga_exm)
		 dis = dis_vert(l,r,s,0,(char)liga_exm);
	break;

  case liga_exm_latin :   /*  '!' */        /* 0xBC <-> ! + point */
	if (liga_exm_latin==liga_exm)
		 dis = dis_vert(l,r,s,0,(char)liga_exm);
	break;

  case CROAT_d :
             if( language==LANG_CROATIAN )
              dis = dis_d_croatian(l,r,s);break;
  case 'd' :
             dis=0;
             if( language==LANG_CROATIAN )
              dis = dis_d(l,r);break;
  case POLISH_l :
               dis = dis_l_stroked(l,r,s);    break;
  default  : dis = 0;     break;
	}

  if( language == LANG_RUSSIAN && langUkr )
    if( let == 'i' ){
       if( dis > 0 )
         dis = MIN(dis,dis_i(l,r,s,2));
    }


if( language!=  LANG_RUSSIAN )			// FRENCH 07.12.1993:
{
// Конфликтные коды // 07.09.2000 E.P.
if (
	!is_baltic_language(language) && 	// 17.07.2001 E.P.
	!is_turkish_language(language) && (	// 30.05.2002 E.P.
	!is_cen_language(language) &&
		(let==i_left_accent  ||  let==i_2dot_accent)
	     ||
		let == i_right_accent  ||
		let == i_roof_accent
		) ||

		// Балтийские палки. 10.07.2001 E.P.
		is_baltic_language(language) &&
			(
			let==i_bottom_accent ||
			let==l_bottom_accent
			) ||
	is_turkish_language(language) && // 04.06.2002 E.P.
		let == i_sans_accent

	)
		dis = dis_i (l,r,s, 2);		// FRENCH	"2 points"

// Конфликтные коды // 07.09.2000 E.P.
if (
	!is_baltic_language(language) && 	// 17.07.2001 E.P.
	!is_turkish_language(language) && (	// 30.05.2002 E.P.
	!is_cen_language(language) &&
		(let == II_left_accent   || let == II_2dot_accent )
	    ||
        let == II_right_accent  ||
		let == II_roof_accent
	) ||

	// Балтийские палки. 10.07.2001 E.P.
	is_baltic_language(language) &&
		(
		let==II_bottom_accent
		) ||
	is_turkish_language(language) && // 04.06.2002 E.P.
		let == II_dot_accent
	)
		dis = dis_I (l,r,s);		// FRENCH
}
if( dis & 1 )
	dis--;
return( dis );	// NOTA BENE: m.b.-444 for dis_t for "ft"
}
/*----------------------------------------------------------------------*/
/* °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° */
/* °°°								°°° */
/* °°°		functions for narrow sticks			°°° */
/* °°°								°°° */
/* °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° */
/*----------------------------------------------------------------------*/
/*	letter є  (i+point)						*/
/*	sign_add = 1 - put 'є' to list of versions - delicate discrim	*/
static int16_t dis_0xBA (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		    int16_t sign_add)
{
int16_t	i, dis=0, t, lm, rm, dy=s->height, lt, rt, ltp;
int16_t	nr=r->num_long_flags, lc=l->num_concs, rc=r->num_concs;
int16_t	wid=s->stick_width, inc=s->inc;

for (lt=l->mount[(ltp=0)],i=1;i<3;i++)
	if ( lt<l->mount[i] )
		lt=l->mount[(ltp=i)];	/* up left max flag */

for (rt=0,i=0;i<3;i++)
	if ( rt<r->mount[i] )
		rt=r->mount[i];		/* up right max flag */

			// here was DEBUG_GRAPH
i=(pitchsize>0); /* sign pitch font */

if( lt>2 )  {	// (1) exist long flags
/*......................................................................*/
if( i )  {	// (2) pitch font
	t = ( l->mount[1]>l->mount[2] )?1:2;	/* max flag 1,2 zone	*/
	if( l->mount[0]>l->mount[t] && l->mb_pos[0]<dy/6 )
		t=0;				// flag in 0 zone
	lm = l->mount[t];			// t - num of max flag
	if( l->m_pos[t]>dy/2  )			// too low nose pitch є
		dis += tab_0xBA[0];					// 6

	lm = l->mount[0]; rm = r->mount[0];
	if( (l->mount[0]+r->mount[0]>lt || ltp==0 && l->m_pos[0]<=dy/6+1 )  )
		{   /* study 0-zone */
		if(  l->up_serif&&rm>1&&r->mount[1]==0  ||
		     lm>2&&rm>2&&lm + rm>wid ||
		     (l->mb_pos[0]<dy/6 || l->up_serif)&&t==0)
			dis += tab_0xBA[1];	// up serif		// 20
		if( l->mount[0]>=l->mount[4] && lt>2 )
			dis += tab_0xBA[1];	// long upper left flag	// 20
		}
	lm = l->mount[4]; rm=r->mount[4];
	t=MAX(wid/2,3);
	if( lm>t && rm<2 )	/* left long downer flag, right - small */
		dis += tab_0xBA[2];					// 10
	else
		if( lm>1 && rm>1 ) /* down serif */
			dis += (nr>(r->mount[0]>1?2:1))*tab_0xBA[3];	//32,16
			/* too many left or right flags */
//////	if( (r->mount[4]>3 || r->down_hook) && l->conc[4] ||	// OLD OLEG
//////	     r->mount[4]>2*wid-1 && l->mount[4]==0 )
	if( (r->mount[4]>3 || r->down_hook) && l->conc[4] && l->mount[4]<3 ||
	     r->mount[4]>2*wid-1 && l->mount[4]==0 )
		dis += tab_0xBA[20];   /* similar 't'  */		// 120
	}	// (2) IF PITCH
/*......................................................................*/
	else		// no pitch font
		dis += tab_0xBA[4];					// 30
/*......................................................................*/
	if( s->neck<2 )  {	// not exist good neck
		if( l->up_serif==2 &&
		    (ltp==0 || l->mount[0]>l->mount[ltp]-2) )
			dis += tab_0xBA[9];   // incorrect num of flags	// 30
		if( r->up_serif==2 && r->mount[0]>2 &&
		    (ltp==0 || r->mount[0]>l->mount[ltp]-2))
			dis += tab_0xBA[9];   // incorrect num of flags	// 30
		if ( l->mount[4]<2 && r->mount[4]<2 &&
		     l->mount[0]<2 && l->mount[1]<2     )
			dis += tab_0xBA[10];  /* true stick  */		// 40
		}

	DIS_CENTER_FLAG(r,2,wid,inc,tab_0xBA[6],tab_0xBA[7]);
	if ( l->down_serif && r->down_serif &&
	     MIN(l->mount[4],r->mount[4])<4 &&
	     s->neck<3)		// add 25.05.1993
		DIS_DIFFERENT_SERIFS(l,r,4,2,wid,tab_0xBA[13]);		// *26

	if( s->typ_nose_1==1 && s->dis_nose_1==0 &&
	    l->down_serif==2 && r->down_serif==2 && l->mount[0]>1 &&
	    l->mount[4]>1 && r->mount[4]>1 )
		dis += tab_0xBA[21];  /* similar '1' */			// 50
}	// (1) EXIST int32_t FLAGS
/*......................................................................*/
else	{	// (1) small flags
	if( rt>lt && ( lt>0 || lt==0 && rt>1) )
		dis += (rt-lt+1)*tab_0xBA[12];  /* right long flags */	// 30

	if( s->neck==0 )  {	// neck not exist
		if( lt==0 && rt==0 )	/* no flags */
			dis += tab_0xBA[10];				// 40
		if( lt==1 && rt==0 && l->m_meandr>2 ) /* no right flags */
			dis += tab_0xBA[10];		// and many	// 40
							// short left pimples
		if( l->mount[0]==0 && l->mount[1]==0 )
			dis += tab_0xBA[10];		// fine stick	// 40
		}	// NECK NOT EXIST

	if( s->neck<2 )  {	// bad neck
//////		if( l->up_serif==2 )			// OLD OLEG
		if( l->up_serif==2 || l->up_hook )	// 22.02.1993
			dis += tab_0xBA[9];  /* upper serif !  */	// 30
		if( r->up_serif==2 )
			dis += tab_0xBA[9];  /* downer serif ! */	// 30
		if ( l->m_meandr<1 && r->m_meandr<1 && lc<1 && rc<1 )
			dis += tab_0xBA[10]; /* true stick     */	// 40

//////		if( s->base_2>=dy/3 )		// OLD OLEG
//////		if ( s->base_2 >= (dy + fax1x2 + fax1x2)/3 + fax1x2 )
		if ( s->base_2 >= (dy + fax1x2 + fax1x2)/3 )	// 24.02.1993
			dis += tab_0xBA[10];	/* true stick     */	// 40
/***********************************
if (!fax1x2) {	if( s->base_2>=dy/3 )			// NO FAX
			dis += tab_0xBA[10]; }				// 40
//////	else {	if( s->base_2>(dy+2)/3 )			// MK FAX
//////	else {	if( s->base_2>=(dy+2)/3 )			// MK FAX
	else {	if( s->base_2 > (dy+2)/3 )		// 02.02.1993
			dis += tab_0xBA[10]; }				// 40
			****************************************************/
		}	// BAD NECK
/*......................................................................*/
//////	{		// (2) QQQQQQ	********* UNKNOWN BRACES *********
	/* for all є */		////// NOTA BENE: REALLY - NOT FOR ALL ### ///
				////// (INSIDE "SMALL FLAGS") !!!!!!!!!!!! ///

	if( l->conc[0] || l->conc[1] )	l->c_meandr--;
	if( r->conc[0] || r->conc[1] )	r->c_meandr--;

	DIS_CENTER_FLAG(r,1,wid,inc,tab_0xBA[6],tab_0xBA[7]);		//60,12
//////	if( s->neck<2 )		// OLD OLEG before 02.02.1993
//////	if ( (s->neck<2) && (s->base_2<dy/3) )		// SOPLI (see BETTER)
//////	if ( (s->neck<2) && (s->base_2<dy/3-1) )	// 22.02.1993
	if ( (s->neck<2) && (s->base_2<dy/3-fax1x2) )	// 23.02.1993
	    DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_0xBA[6],tab_0xBA[7]);	//60,12
	DIS_CENTER_FLAGS(l,r,3,wid,inc,tab_0xBA[6],tab_0xBA[7]);	//60,12

	DIS_CENTER_CONCS(l,r,2,inc,tab_0xBA[6],tab_0xBA[7]);		//60,12
	if( l->conc[0] || l->conc[1])
		l->c_meandr++;
	if( r->conc[0] || r->conc[1])
		r->c_meandr++;

	CALC_LENGTH_BEAM_FLAGS(l,r,wid,s,t);
	DIS_BEAM(l,r,3,t,tab_0xBA[8]) ;					// 20
	if( rt==1 && (s->neck==0 || (l->mount[2]+r->mount[2])>=dy))
//////				// EXPERIMENT 02.02.1993: ###
//////	if( s->neck < 2 )	// ##########################
		DIS_BEAM(l,r,2,1,tab_0xBA[8]) ;				// 20
	if( r->mount[4]>3 && l->mount[4]==0 )
		dis += tab_0xBA[19] ;					// 60
//////	}	// (2) QQQQQQ

	if( l->mount[0] && l->mount[4] && r->mount[4] &&
	    l->mount[0]>l->mount[4] )
		dis += tab_0xBA[16];					// 40
//////	if( l->down_serif && r->down_serif )
	if( l->down_serif && (r->down_serif || r->down_hook))	// 27.07.1993
		DIS_DIFFERENT_SERIFS(l,r,4,2,wid,tab_0xBA[13]);		// 26
	}	// (1) SMALL FLAGS
/*......................................................................*/
DIS_BRACES(l,r,tab_0xBA[5]);						// 40

if( inc && sign_add && s->neck<2 )
	dis += tab_0xBA[14];						// 20
if( s->dis_0xBA )   /* dis for bad neck */
	dis += s->dis_0xBA;						// +++
if( wid*2 > s->height )
	dis += tab_0xBA[17]; /* bad proportions */			// 80
if(  (l->mount[4]>3||l->down_hook) && r->conc[4]>1 ||
      l->mount[4]>2*wid && r->mount[4]==0 )
	dis += tab_0xBA[18]; /* similar  'J' */				// 40

if ( s->neck < 2  &&  r->down_serif  &&	// 16.02.1993  fax9/20(19) "tool"
     l->mount[0] > l->mount[1]  &&
     l->mount[0] > l->mount[2]  &&
     l->m_pos[0] < (s->base_2 - 2)  &&	// stdf12/11 "Given", /29 "option"
     (l->mb_pos[0] != l->me_pos[0] || l->mount[0] > 1) )	// stdm14/24(26) "Union"
		dis += tab_0xBA[22];					// 50

				// similar TOO int32_t 'L' (h3/32) "L. Steele"
if ((MIN(wid,6) + wid + r->mount[4]) > dy)			// 12.10.1993
	dis += 100;		// too int32_t right down flag		// 100

				// 16.01.1994	PROBA dis/=2 for NECK==3;
	if (s->neck==3)  dis = (dis >> 2) << 1;		// DIVIDE DIS / 2;

return(dis);
}
/*----------------------------------------------------------------------*/
static int16_t dis_i (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		  int16_t npoints_1_or_2)  {		// 17.01.1994
int16_t	dis=0;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t maxlen = MAX (MAX(l->mount[0],l->mount[4]), max(r->mount[0],l->mount[4]));

/*......................................................................*/
				// 05.08.1993 about DOT:
				// 26.08.1993 !up_dot_NAVIS => up_dot_H!=0
				// frad2/19  n'est => riest
				// f7/7 "been" 'n'=>"ri"
				// m15/10 "Philadelphia" first 'i' really INC
//////if (!s->up_dot_NAVIS  &&	// NO NAVISANIE 05.08.1993
//////if (s->up_dot_H  &&	// POINT EXIST	26.08.1993

if (npoints_1_or_2==1  &&	// 07.12.1993: NO DISCRIM in Case "n==2"
    s->up_dot_H  &&		// POINT EXIST	26.08.1993
    !inc  &&			// I can not work with inc TODAY
////s->up_dot_W  &&		// BBB: POINT EXIST (before 26.08.1993)
////s->up_dot_dCOL - left_mode_EEM > wid + 2)	// 05.08.1993 first PROBA
////(s->up_dot_dCOL - left_mode_EEM >= wid  ||	// 22.11.1993 second PROBA
				// 22.11.1993 next PROBA:
    (s->up_dot_dCOL - left_mode_EEM >= wid + (r->mount[0] ? 1 : 0) || //RIGHT;
     s->up_dot_dCOL + s->up_dot_W <= left_mode_EEM  ||		      // LEFT;
     s->cut_l &&		// 07.01.1994 for CUTTED 'n' and SMALL DUST;
     (r->conc[0] || r->conc[1]) &&	// CONCAVE EXIST (???);
     wid > (s->up_dot_W<<1)))		// SMALL DUST:  WID > 2*DOT_WID;
	return 666;		// for FARAJEV
/*......................................................................*/

DIS_CENTER_FLAGS(l,r,2,128,inc,tab_i[0],tab_i[1]);
//////DIS_BRACES(l,r,tab_i[2]);		/* braces [,] */	// OLD OLEG
DIS_BRACE(l,r,tab_i[2]); 		// MK: brace '[' in ANY CASE;	// 40
if (s->dis_up_dot)			// MK NEW 08/11.01.1992:
		{ DIS_BRACE(r,l,tab_i[3]); }	// brace '[' with {ш}	// 0 !
	else	{ DIS_BRACE(r,l,tab_i[2]); }	// brace '[' without {ш}// 40

if ( (maxlen + s->stick_width + maxlen)*2  >  s->height*3 )	// 08.07.1993
	dis += 222;				// PSEUDOPROPORTION	// 222

//////if (l->mount[0] > l->mount[4] + r->mount[4] + 2)		// 28.07.1993
if (l->mount[0] >	// see frae7/10,  stdm11/8:		// 29.07.1993
////MAX ((l->mount[4] + r->mount[4] + 2), (s->stick_width + 1)))
				// 17.01.1994 frac11/15 "Writer":
    MAX ((l->mount[4] + r->mount[4] + 1), (s->stick_width + 1)))
	dis += 166;		// "reverse r" for "ri" -> "ii"		// 166
			// NOTA BENE: "max" - for ZONE-3 R.DOWN, NO L.DOWN !!!

/*......................................................................*/
				// 17.01.1994  PROBA FOR CUTTED 'h':
if (s->up_dot_H==0  &&		// POINT NOT EXIST
    inc  &&			// and INC
    (left_letter_EEM=='l' || left_letter_EEM=='/'))
	dis += 40;		// for der Laterne;			// 40
/*......................................................................*/
return(dis);
}

// j with dot
static int16_t dis_0xBB (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		    int16_t sign_add)
{
int16_t dis=0;
if( !l->mount[4] && !l->mount[3])
  dis += 40;
if( !r->conc[4]  )
  dis += sign_add?10:20;
if( s->neck<2 && !sign_add )
  dis += 20;
if( s->base_3> s->height )
  dis += 200;
return dis;
}
/*----------------------------------------------------------------------*/
static int16_t dis_j (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  {
int16_t	dis=0;						// 07.12.1993
int16_t	wid=s->stick_width, inc=s->inc;
	l = l;	// FOR NO WARNING
/*......................................................................*/
				// 22.11.1993 about DOT:  COPY from dis_i:
if (s->up_dot_H  &&		// POINT EXIST	26.08.1993
    !inc  &&			// I can not work with inc TODAY
    (s->up_dot_dCOL - left_mode_EEM >= wid + (r->mount[0] ? 1 : 0) || // RGT.
     s->up_dot_dCOL + s->up_dot_W <= left_mode_EEM))		      // LEFT
	return 666;		// for FARAJEV
/*......................................................................*/
return(dis);
}
/*----------------------------------------------------------------------*/
/*	typ_add = 1 - put '1' to list of versions - delicate discrim */
static int16_t dis_1 (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s, int16_t typ_add)
{							// 17.01.1994
int16_t	dis=0, t, lmu, rmu, lmd, rmd, dy=s->height;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	tt = (dy>30) ? 2 : 1;	// 25.02.1993	// CAUTION: USED TWICE ######
//////int16_t	d_L, d_R;	// 04.06.1993

lmu = MAX(l->mount[0],l->mount[1]);  rmu = max(r->mount[0],r->mount[1]);
lmd = l->mount[4];  rmd = r->mount[4];

/*......................................................................*/
if( lmu+rmu+lmd+rmd != 0 )  {
if( lmu==1 && dy>20 )
	dis += tab_1[17];		/* too small nose */		// 20
t = wid>6 ? 1 : 0;
if( lmu<=t )				/* no left upper flag */
	dis += tab_1[0];						// 60
//#ifndef RUS_ENG_LANG
//if(  language != LANG_RUSSIAN || language == LANG_RUSSIAN && lmd<1 && rmd<1 )
//#else
//if( multy_language )
//#endif
if( language != LANG_RUSSIAN ||
    language == LANG_RUSSIAN && !multy_language && lmd<1 && rmd<1 ||
    language == LANG_RUSSIAN && multy_language )
{
if( rmu>1 && !inc || rmu>2 && inc)	/* exist right upper flag */
	{	/* different nonlinear (dis=f(rmu)) discrim */
	if( rmu<3 )
		dis += tab_1[1]*(rmu-1);				// *2
	else if( rmu<6 )
		dis += tab_1[1]*10*(rmu-1);				// *2
	else
		dis += tab_1[1]*20*(rmu-1);				// *2
	}
}
DIS_HALF_SERIF(l,r,4,0,tab_1[2]);	/* test downer serif */		// 8

if( l->down_serif && r->down_serif )
	DIS_DIFFERENT_SERIFS(l,r,4,4,wid,tab_1[4]);			// 6

if( inc && ( r->down_hook || r->mount[3]>1 ))
	dis += tab_1[14];		/* similar inc 'l' */		// 40
/*..................................................*/
if( typ_add==0 && !(s->typ_nose_1&&l->mount[0]>wid) )	/* not add */
{
int16_t ser = l->up_serif + r->up_serif + l->down_serif + r->down_serif ;

if(  language != LANG_RUSSIAN )
{
if ( lmu==1 && rmu==1 && lmd==1 && rmd==1 && ser>5 ||	// study angle:
/////lmu>1  && rmu>1  && lmd>1  && rmd>1  && ser>4 )	// flags+serifs-information
     lmu>tt && rmu>tt && lmd>tt && rmd>tt && ser>4 )	// 25.02.1993
		dis += tab_1[15];					// 70
}

DIS_BRACES_CIRCLE(l,r,wid,tab_1[16]);	// braces (,)			// 40

if ( (r->conc[0]>1 && r->conc[4]>1) ||	// OLD OLEG
     (r->conc[0]>wid) )			// 01.06.1993 for ')' after INC
	dis += tab_1[16];		// this also ')'-config		// 40
if( (r->conc[0] || r->conc[1]) && r->mount[4]>2 && l->mount[4]>2 )
	{	/* pitch є */
	int16_t tt=find_neck(l,r,1);
/******	if( tt<2 ) ******/
	if( tt>=0  &&  tt<2 )		// 31.05.1993 (WAS ERROR if FFFF)
		dis += tab_1[13];	// similar    є			// 20
	}
}
/*..................................................*/
if( l->mount[4]==0 && r->mount[4]==0 && r->mount[0]==0 &&
    s->typ_nose_1==0 )		/* no down serif */
		dis += tab_1[19];	// cutting nose			// 4

if ( (s->typ_nose_1==0)  &&		// 02.06.1993 PROBA:
     wid > 5  &&
     lmu > 3  &&
     lmd > 2  &&			// add 17:30 (for #4 ???)
     (s->lll_nose_1  ||  dy>24 && l->m_pos[0]<3) )
//////		dis += 88;	// PROBA-88				// 88
		dis += 20;	// PROBA-20				// 20

/******************************************************************
if (r->conc[0]>=MAX(wid,3))	// 09.06.1993  for CUT. 'h' to "ll"
	dis += 80;		// PROBA-80				// 80
	***************************************************************/

if (s->cut_r &&		// 09.06.1993 PROBA for CUT. "The" (stdj10);
    (r->mount[0] || r->m_meandr==0) &&
    !r->down_serif &&
////l->mount[0]>MAX(wid*2-3,5))	// first
    l->mount[0]>MAX(wid,5))	// second
//////	dis += 80;		// PROBA-80				// 80
//////	dis += 160;		// PROBA-160				// 160
	dis += (rmu+2)*80;	// PROBA-rmu"*80			// *80
}
/*.......................................................................*/
else	{ /* no serifs */
	if( l->mount[1]+l->mount[2]+l->mount[3]+
	    r->mount[1]+r->mount[2]+r->mount[3] < 2 )
		dis += tab_1[12]<<1;	// small dis  for solid stick	// 8
	else
		dis += tab_1[18];	// exist central flags		// 8
	if( s->neck>1 )
		dis += tab_1[13];					// 20
	else if( s->neck==1 )
		FOUR_CONC(l,r,tab_1[13]);				// *20?
	/*************************************	DELETE IT 20.10.1993
	if (!inc  &&  (s->cut_l || s->cut_r)) // 18.10.1993 CUT, NO INC&Serifs
//////	    { dis = 140; goto m_ex; }	// g14/6 "Greatest", first 't'	// 140!
	    { dis = 90;  goto m_ex; }	// g14/6 "Greatest", first 't'	// 90!
//////	    { dis = 76;  goto m_ex; }	// g14/6 "Greatest", first 't'	// 76!
	    *************************************************************/
	}
/*......................................................................*/
///if (l->mount[1]>2 && l->mb_pos[1]>5)	// 21.10.1993 for TOO DOWN NOSE
if (l->mount[1]>2  &&  			// 22.10.1993 for TOO DOWN NOSE
    l->mb_pos[1] >			// 22.10.1993 vers-080
////(s->cut_l ? 3 : 4))			// ok55/4(5) "22021"  (exotic Nose)
////(s->cut_l ? 3 : 5))			// j8/13(15) "105/18" (exotic Nose)
    (s->cut_l ? 3 : (inc ? 5 : 4)))	// fax25/12(17) "Windows": "nd" => "m1"
	dis += tab_1[6];					// 60
DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_1[6],tab_1[7]);		// 60,12
DIS_CENTER_CONCS(l,r,2,inc,tab_1[6],tab_1[7]);			// 60,12

#ifndef UFA
DIS_BRACES(l,r,tab_1[8]); /* braces [,] */    // 40
#endif

//////if( lmu==1 )	// OLD OLEG
//////if ( (lmu==1) && (dy<22) ) 	// MK 19.01.1922:  stdh25/15 (???)
//////if ( lmu==1 )		// OLD OLEG RESTORE OTLADKA 02.02.1992
if ( (lmu==1) && (dy<22) ) 	// 28.07.1993	SNOWA PROBA ##############
	{
	t = find_beam(l,r,1);
	if( t>=0 && t<3 && r->mount[t]>=l->mount[t] )
		dis += tab_1[10]*(r->mount[t]-r->mount[t]+1);	// *40
		/* similar 'f' */
	}

if( s->dis_nose_1 )
	dis += s->dis_nose_1;						// +++
	/* primary discrim */

if( wid>4 && l->mount[0]==1 && l->mount[1]==lmu )
	dis += r->conc[0]*tab_1[11];					// 20

if( l->up_serif>1 && lmu<=lmd && lmu>1 && rmu<2 && rmd>1 && lmd>1 &&
    typ_add )
	dis += tab_1[12];	// min DIS for l-config			// 4

if( s->neck>1 )
	dis += tab_1[13];	// similar 'є'				// 20

if( wid*2 > s->height )
	dis += tab_1[20];	// too wide stick : bad proportions	// 80
/*......................................................................*/
if (rmd<2 && (s->cut_l || s->cut_r))	// 21.10.1993 CUT, NO R.M.D
	dis += 46;			// 254 => 218			// 46
/*......................................................................*/
if (rmd>=2 && lmu>lmd+wid+rmd)		// 17.01.1994 too long nose
	dis += 60;			// for der Laterne		// 60
					// h15/6, i15/28 {'l} => {1}
/*......................................................................*/
//////m_ex:
if( is_digital_string() )
if( language==LANG_RUSSIAN )
  {
  if( dis>100 )     dis=40;
  else if( dis>60 ) dis=20;
  else if( dis>20 ) dis=4;
  else              dis=0;
  }
#ifdef  INTERSEPTOR
if( language==LANG_RUSSIAN )
  {
  if( dis>100 )     dis=40;
  else if( dis>60 ) dis=20;
  else if( dis>20 ) dis=4;
  else              dis=0;
  }
#endif
return(dis);
}
/*----------------------------------------------------------------------*/
static int16_t dis_l (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  {
							// 17.01.1994
int16_t	dis=0, t, lmu, rmu, lmd, rmd, num_l, num_z, dy=s->height;
int16_t	sl=l->num_flags, sr=r->num_flags;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	tt;		// MK WORK 28.01.1992
int16_t	d_L, d_R;	// 23.02.1993

	dis = dis_LIMIT_EEM;		// 18.11.1993 (0 or 100);
//////	dis += mk_dis_for_liga_exm;	// 06.01.1994 (0 or 2);	MOVE TO END;

tt = (dy>30) ? 2 : 1;	// used in some places... (see also BUMERANG)

lmu = l->mount[0];  rmu = r->mount[0];
lmd = l->mount[4];  rmd = r->mount[4];
/*......................................................................*/
if( lmu+rmu+lmd+rmd != 0 )  {				// WITH ANGLES:
t = wid>6 ? 1 : 0;
if ( rmu>t && lmu==0 )  {	/* no left upper flags ВДД */
	if( rmu==1 )  dis += r->conc[0] ? tab_l[0] : 0;		// 2,0
		else  dis += r->conc[0] ? tab_l[1] : tab_l[0];	// 8,2
	}

DIS_BRACES(l,r,tab_l[3]);	/* braces [,] */		// *40

//////if( r->down_hook==0 )			// OLD OLEG
if ( !r->up_hook && !r->down_hook )		// 25.02.1993	// for BUMERANG
{	/* not exist right down hook : find brace-configurations */

if (!s->dis_left_brace)			// MK 25.01.1993
	DIS_BRACES_CIRCLE(l,r,wid,tab_l[3]);	// braces (,)	// *40

//////if( l->mount[4]>2 && r->conc[4]>2 && r->conc[4]>wid && r->mount[4]==0  )
if( l->mount[4]>2 && r->conc[4]>2 && r->mount[4]==0 &&	// 30.06.1993 PROBA
	(r->conc[4]>wid ||
	 l->down_hook && r->conc[4]>8))	// frac3/1 "POUR" 'U'=>'ll' (last l)
		dis += (1+(r->conc[4]))*tab_l[3];		// *40
}

if ( lmu>1 && lmd==0 && rmu==0 && rmd>1  )		// conf : ї
		dis += inc ? 0 : tab_l[4]*(lmu+rmd-2);	//	  А   // *2

//////if( lmu==0 && lmd>1 && rmu>1 && rmd==0  )		// OLD OLEG
if( lmu==0 && lmd>tt && rmu>tt && rmd==0  )		// conf : Ъ   // BUMERANG
		dis += tab_l[5]*(rmu+lmd-2);		//	  Щ   // *40

if( rmu>0 && r->up_serif>1 )
	DIS_DIFFERENT_SERIFS(l,r,0,2,wid,tab_l[2]);		// *6

if( !inc && l->down_serif && r->down_serif )
	DIS_DIFFERENT_SERIFS(l,r,4,2,wid,tab_l[2]);		// *6

if( l->up_serif && r->up_serif && !inc )  {
	int16_t ser = l->up_serif + r->up_serif + l->down_serif + r->down_serif;

	DIS_CURVE(l,r,4,1,tab_l[8]);				// 10

	if ( (lmu+rmu)<<1>(lmd+rmd) )  {
//////	    if (lmu==1 && rmu==1 && lmd==1 && rmd==1 && ser>5 ||	//OLEG
//////		lmu>1  && rmu>1 && lmd>1 && rmd>1 && ser>4 )		//OLEG
	    if (wid<4 && lmu==1 && rmu==1 && lmd==1 && rmd==1 && ser>5 ||
		dy<=30 && lmu>1 && rmu>1 && lmd>1 && ser>4 ||
		dy>30  && lmu>2  && rmu>2 && lmd>2 )
			dis += tab_l[10];  /* up and down serifs  */	// 70
	    }
	}

if( inc )  {  /* inc may form false pimple : hard conditions for find serifs */
	if ( rmu>2 && lmu>1 && rmd>1 && lmd>2 )
		dis +=  tab_l[10];	// up and down serifs		// 70
	}
if ( inc  &&  (lmu + wid + wid + rmd) > dy )	// 08.07.1993
		dis += 222;		// stdk11/34 'z' => 'l'		//222

					// 11.01.1994  SPEC.TYPFEACE of liga_i:
if (lmu>=3  &&  l->m_pos[0]>=6  &&	// stdf7/7**,27,39,46 and
    abs (l->m_pos[0]-s->base_2)<=2)	// stdf12/12,25***,26
		dis += 20;		// for der Laterne;

/* ----------------------- STUDY ANGLES ---------------------- */
t = 1;
num_l = (lmu>t)+(rmu>t)+(lmd>t)+(rmd>t);	// number of long flags
num_z = (lmu==0)+(rmu==0)+(lmd==0)+(rmd==0);	// number of empty angles
//////num_z = (lmu<tt)+(rmu<tt)+(lmd<tt)+(rmd<tt);	// for BUMERANG

/******************************	BEFORE 31.05.1993
if( num_z==3 &&
    (num_l==1 && rmd==0 && r->mount[3]==0 ||
     (lmu>1&&dy>16 || lmu>0&&dy<17) &&
     s->dis_nose_1==0 && r->mount[3]==0 ||
     rmu>0 && s->r_f_symptom && s->l_f_symptom ))
	dis += tab_l[11];	// similar 'f','1'		// 48
	*****************************************/

/**************************************
if( (num_z==3) && (s->dis_nose_1==0) &&		// 31.05.1993
    (num_l==1 && rmd==0 && r->mount[3]==0 ||
     (lmu>1&&dy>16 || lmu>0&&dy<17) && r->mount[3]==0 ||	// MODIFIED
     rmu>0 && s->r_f_symptom && s->l_f_symptom ))
	dis += tab_l[11];	// similar 'f','1'		// 48
	*****************************************/

if ( (num_z==3) &&					// 01.06.1993
     (rmu>0 && s->r_f_symptom && s->l_f_symptom  ||	// Similar 'f'  ||
     (s->lll_nose_1==0  &&				// Similar '1'
      (num_l==1 && rmd==0 && r->mount[3]==0 ||
       (lmu>1 || lmu>0&&dy<17) && s->dis_nose_1==0 && r->mount[3]==0))))
		dis += tab_l[11];	// similar 'f','1'	// 48

if ( (num_l==1) &&				// 09.07.1993, 12.10.1993
/////(rmd>MAX(wid,3)) && (l->conc[4]*2<wid))	// fraa13/10	'L' (4:5)
     (rmd>MAX(wid,4)) && (l->conc[4]*2<wid))	// stdg19/8(11) 'l' (3:4)
		dis += tab_l[11];	// similar 'L'		// 48

if ( num_l==3 && lmu>0 && s->typ_nose_1==1 &&	// OLD OLEG   &&
/////pitchsize==0 &&					// 25.05.1993 &&
     ( (pitchsize==0) || (l->m_pos[0]>=MAX(wid,5)-1) ) &&	// 01.06.1993 &&
     lmu >= lmd)					// 31.05.1993
	dis += tab_l[11];	// similar '1'			// 48

//////if (num_l==3 && inc && s->cut_r && rmu<2)		// 02.06.1993	PROBA
////if (num_l==3 && s->inc_v16>16 && s->cut_r && rmu<2)	// 03.06.1993	(_v16);
if (num_l==3 && abs(s->inc_num)>1 && s->cut_r && rmu<2)	// 10.12.1993	EDIT;
//////	dis += 80;	// PROBA-80 for 'I' INC,CUT 		// 80
	dis += 40;	// PROBA-40 for 'I' INC,CUT 		// 40

			// MK 01.02.1993  for LENKA fax40/22(21):
if( num_l==3 && lmu>=4 && rmu>=4 && rmd<=4 )
	dis += tab_l[11];	// LENKA SPEC CASE: 'U'=>"Ll"	// 48

if( num_l==2 && num_z==2 && r->down_serif && l->down_serif )
	dis += tab_l[11];	/* two half-serifs only */	// 48

if ( ( lmd==0 || (lmd==1 && dy>30) || (rmd<<1>dy && lmd<3) ) &&
     (l->conc[4]<1 || l->ce_pos[4]==dy-1) )	// find L-configurations
	{
//////	if( rmd>wid-1 && wid>4 && (lmu<<1)<rmd )		// OLD OLEG
	if( rmd>wid-1 && wid>4 && (lmu<<1)<rmd && rmu<rmd)	// for BUMERANG
		dis += tab_l[13];				// 80
	else if( !inc )  {
		if ( rmd > (wid*3)>>1 )	dis += tab_l[13];	// 80
		else if ( rmd>=wid )	dis += tab_l[13]>>1;	// 40
		}
	}

/******************************************
if (r->conc[0]>=MAX(wid,3))	// 09.06.1993  for CUT. 'h' to "ll"
	dis += 80;		// PROBA-80			// 80
	***********************************/

/******************************************
if ( !(l->mount[0] && l->mount[4] && r->mount[4]) )  {
	if ( s->neck==2 &&
	     (lmu<2  ||  lmu==2 && l->m_meandr>1 && dy>20 ) )
		dis += tab_l[12];				// 20
	}	****************************/

if ( s->neck==2 &&
     ( !(l->mount[0] && l->mount[4] && r->mount[4]) )  &&
//// ( ((wid + 2) << 2)  >  dy)  &&		// "NO ZIRAFA"	// 22.02.1993
     ( lmu<2  ||  lmu==2 && l->m_meandr>1 && dy>20 ) )		// MK EDITION
		dis += tab_l[12];				// 20

if ( s->neck==3 )				// 18.02.1993	//m.b.NECK=3
//////		dis += tab_l[12];				// 20
		dis += 100;			// 15.12.1993	// 100

				// MK-SOPLI 26/28.01.1992 for 'I'=>'l'
//////if (dis<tab_l[11] && lmu>t && rmu>t && lmd>t)  dis = tab_l[10];	// 70;
//////if (dis<tab_l[10] && lmu>t && rmu>t && lmd>t)  dis = tab_l[10];	// 70;
////tt = (dy>30) ? (t + 1) : t;	// ok36/4 "Central"  37*16 [6-5-5] (3,2,6,5)
if (dis<tab_l[10] && lmu>tt && rmu>tt && lmd>tt)  dis = tab_l[10];	// 70;

if (s->cut_r &&		// 09.06.1993 PROBA for CUT. "The" (stdj10);
    (r->mount[0] || r->m_meandr==0) &&
    !r->down_serif &&
////l->mount[0]>MAX(wid*2-3,5))	// first
    l->mount[0]>MAX(wid,5))	// second
//////	dis += 80;		// PROBA 80			// 80
//////	dis += 160;		// PROBA 160			// 160
	dis += (rmu+2)*80;	// PROBA rmu"*80		// *80
}
/*......................................................................*/
else    {						// EMPTY ANGLES:
//////	if( s->neck>1 )					// OLD OLEG
	/****************************************************************
	if( ( s->neck==3 ) ||				// 22,23.02.1993:
//////	    ( s->neck==2 ) && ((wid+2)<<2)>dy )	// "NO ZIRAFA"	### ??? ###
//////	    ( s->neck==2  &&  wid<5 ) )		// 23.02.1993 16:00 "NO ZIRAFA"
	    ( s->neck==2  &&  dy<27 ) )		// 23.02.1993 17:30 "NO ZIRAFA"
		dis += tab_l[12];					// 20
		***********************************************************/
	if (s->neck==3)				// 15.12.1993 EDITION:
		dis += 100;						// 100
	else if ( s->neck==2  &&  dy<27 )	// "NO ZIRAFA"
		dis += tab_l[12];					// 20
	else if ( s->neck==1 )
		FOUR_CONC(l,r,tab_l[12]);				// 20
	/*************************************	DELETE IT 20.10.1993
	if (!inc  &&  (s->cut_l || s->cut_r)) // 18.10.1993 CUT, NO INC&Serifs
//////	    { dis = 140; goto m_ex; }	// g14/6 "Greatest", first 't'	// 140!
	    { dis = 90;	 goto m_ex; }	// g14/6 "Greatest", first 't'	// 90!
//////	    { dis = 76;  goto m_ex; }	// g14/6 "Greatest", first 't'	// 76!
	    ************************************/
	}
/*......................................................................*/
/* N.B. in group macros (CENTER_FLAG) used ->c_meandr.	*/
/*	If c_meandr==0 and exist near central flag,then	*/
/*	not discrim. For narrow sticks I wont check	*/
/*	near flags . -->> I increase signums c_meandr	*/
/*	for left and right chars of stick		*/
if( wid<3 )  { l->c_meandr++;  r->c_meandr++;  }	// MODIFY c_meandr !!!

//////DIS_CENTER_FLAGS(l,r,1,wid,inc,tab_l[6], tab_l[7]);	// OLD OLEG
//////DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_l[6], tab_l[7]);	// OLD OLEG

				// ABOUT ONE PIXEL		// 23.02.1993
	d_L = d_R = tab_l[7];	// DEFAULT			// =12
	if (s->neck<2)  {	// see OLD VARIANT in dis_I !!!!!!!!!!!!!!
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

				// add 25.05.1993 for stdf12/11 "Given"
	if (s->neck && l->m_pos[0]>MAX(5,s->base_2))	// ZONE-0 TOO DOWN !!!
		DIS_CENTER_FLAG (l,0,wid,inc,tab_l[6],d_L);	// 60, 12(0)

DIS_FLAGS_L_R(1,tab_l[6]);				// 60, 12(0)/12(0)
DIS_FLAGS_L_R(2,tab_l[6]);				// 60, 12(0)/12(0)
//////DIS_FLAGS_L_R(3,tab_l[6]);				// 60, 12(0)/12(0)

//////DIS_CENTER_CONCS(l,r,2,inc,tab_l[6],tab_l[7]);	// OLD OLEG
DIS_CENTER_CONC(l,2,inc,tab_l[6],d_L);		// 23.02.1993	// 60,12(0)
DIS_CENTER_CONC(r,2,inc,tab_l[6],d_R);		// 23.02.1993	// 60,12(0)

if (d_L || d_R)  {	// 02.02.1993	PROBA (ABOUT GLISTA)	//  OR ??????
CALC_LENGTH_BEAM_FLAGS (l,r,wid,s, t);
DIS_BEAM (l,r,1,t,wid==1 ? tab_l[9]<<1 : tab_l[9]);		// 140,70
DIS_BEAM (l,r,2,t,wid==1 ? tab_l[9]<<1 : tab_l[9]);	// 01.02.1993: ZONE-2
DIS_BEAM_MK (l,r,2, t, wid==1 ? tab_l[16]<<1 : tab_l[16]);  	// 28,14  01.02.1993: MK
if( !inc )
	DIS_BEAM(l,r,3,t,wid==1 ? tab_l[9]<<1 : tab_l[9]) ;	// 140,70
if (s->width<=4  &&  !inc  &&	// add 09.06.1993 about added thin '(' or ')'
    (l->mount[2] && r->conc[2] && 		// '(' LEFT
     (s->dis_left_brace==0) &&
     (l->m_meandr==1) && (r->c_meandr==1)  ||
     l->conc[2] && r->mount[2] && 		// ')' RIGHT
     (s->dis_right_brace==0) &&
     (l->c_meandr==1) && (r->m_meandr==1)))
	dis += tab_l[19];						// 6
}
else if (l->conc[0]  && r->conc[0]  &&	// 19.11.1993 GLISTA  similar 't'
	 l->mount[1] && r->mount[1] &&	// g14/32(33) "fastest" first 't'
	 wid < 5  &&			// g14/34(37) "Kit"
	 abs (l->mb_pos[1] - s->base_2) < 2  &&
	 abs (r->mb_pos[1] - s->base_2) < 2)
		dis += 20;					// 20

if( wid<3 )  { l->c_meandr--;  r->c_meandr--;  }	// RESTORE c_meandr !!!

if( wid*2 > s->height )
	dis += tab_l[14];	/* too wide stick : bad proportions */	// 80

if( l->mount[1]>0 && r->mount[1]>0 && r->mount[4]>0 && sl==1 && sr==2 )
	dis += tab_l[15];	/* similar 't' */			// 80

			// 09,10,11.02.1993	CASE NECK=1 wid>=6 (see dis_0xBA)
if ( (s->neck==1) && (wid>=6) &&
     (
//     ( l->conc[1] && l->mount[2] &&	// stda91/37 "Wylie"  'є'=>'l'
//	 l->ce_pos[1]==l->mb_pos[2]-1 )
//	 ||
       ( l->conc[0] && r->conc[0] &&	// fax 27/7 "Capability", first 'є'
//////	 ( l->cb_pos[0]==r->cb_pos[0] && l->cb_pos[0]!=0  ||
	 ( l->cb_pos[0]==r->cb_pos[0] && abs(l->cb_pos[0]-s->base_2)<4  ||
	   l->ce_pos[0]==r->ce_pos[0] && abs(l->ce_pos[0]-s->base_2)<4 ) &&
	   l->cb_pos[0]!=l->ce_pos[0] && r->cb_pos[0]!=r->ce_pos[0] )
	 ||
       ( l->conc[1] && r->conc[1] &&	// fax ....................... 'є'
	 ( l->cb_pos[1]==r->cb_pos[1] && abs(l->cb_pos[1]-s->base_2)<4  ||
	   l->ce_pos[1]==r->ce_pos[1] && abs(l->ce_pos[1]-s->base_2)<4 ) &&
	   l->cb_pos[1]!=l->ce_pos[1] && r->cb_pos[1]!=r->ce_pos[1] )
   ) )
		dis += tab_l[17];					// 6

			// 12.02.1993	thin LEG exist:
if ( l->conc[4] && r->conc[4] &&	// j1/16 "CURTIS": 'R'=>"II"
     (l->ce_pos[4] - l->cb_pos[4] + r->ce_pos[4] - r->cb_pos[4]) >= (dy>>1) )
		dis += tab_l[18];					// 20

			// 15.02.1993	CUT & NO MOUNT PROBA:
//////if ( s->cut_r && r->num_flags==0 )
//////		dis += 166;	//////////////////////////////////////////

/**********************	DELETE IT 22.09.1993 (see fraf9/14):
			// 11.03.1993	SOPLI about '2'=>'l'(inc); NE NA MESTE !!!
	if (l->mount[0]+r->mount[4] > wid*4 + 8)	// j2/17 "BOX 129"
		dis += 222;					// 222
		********************************/

			// 19.11.1993	l,I  similar 'E' ("THEATRE")
if (wid*2+1>=dy &&	// k11/31 "PERFORMERCE", last 'E' 21*11, wid=10
    r->conc[1] && r->conc[3] &&
    (r->conc[1] | r->conc[3]) > 1)	// all cases, exclude (1,1)
	dis += 100;						// 100
/*......................................................................*/
					// 17.01.1994 MOVE IT TO END:
	if (dis==0)  dis += mk_dis_for_liga_exm;	// (0 or 2);

	return(dis);
}

static int16_t dis_l_stroked (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  {
int16_t	dis=0, t, lmu, rmu, lmd, rmd, num_l, num_z, dy=s->height;
int16_t	sl=l->num_flags, sr=r->num_flags;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	tt;		// MK WORK 28.01.1992
int16_t	d_L, d_R;	// 23.02.1993

	dis = dis_LIMIT_EEM;		// 18.11.1993 (0 or 100);

tt = (dy>30) ? 2 : 1;	// used in some places... (see also BUMERANG)

lmu = l->mount[0];  rmu = r->mount[0];
lmd = l->mount[4];  rmd = r->mount[4];
/*......................................................................*/
if( lmu+rmu+lmd+rmd != 0 )  {				// WITH ANGLES:
t = wid>6 ? 1 : 0;
if ( rmu>t && lmu==0 )  {	/* no left upper flags ВДД */
	if( rmu==1 )  dis += r->conc[0] ? tab_l[0] : 0;		// 2,0
		else  dis += r->conc[0] ? tab_l[1] : tab_l[0];	// 8,2
	}

DIS_BRACES(l,r,tab_l[3]);	/* braces [,] */		// *40

if ( !r->up_hook && !r->down_hook )		// 25.02.1993	// for BUMERANG
{	/* not exist right down hook : find brace-configurations */

if (!s->dis_left_brace)			// MK 25.01.1993
	DIS_BRACES_CIRCLE(l,r,wid,tab_l[3]);	// braces (,)	// *40

if( l->mount[4]>2 && r->conc[4]>2 && r->mount[4]==0 &&	// 30.06.1993 PROBA
	(r->conc[4]>wid ||
	 l->down_hook && r->conc[4]>8))	// frac3/1 "POUR" 'U'=>'ll' (last l)
		dis += (1+(r->conc[4]))*tab_l[3];		// *40
}

if ( lmu>1 && lmd==0 && rmu==0 && rmd>1  )		// conf : ї
		dis += inc ? 0 : tab_l[4]*(lmu+rmd-2);	//	  А   // *2

if( lmu==0 && lmd>tt && rmu>tt && rmd==0  )		// conf : Ъ   // BUMERANG
		dis += tab_l[5]*(rmu+lmd-2);		//	  Щ   // *40

if( rmu>0 && r->up_serif>1 )
	DIS_DIFFERENT_SERIFS(l,r,0,2,wid,tab_l[2]);		// *6

if( !inc && l->down_serif && r->down_serif )
	DIS_DIFFERENT_SERIFS(l,r,4,2,wid,tab_l[2]);		// *6

if( l->up_serif && r->up_serif && !inc )  {
	int16_t ser = l->up_serif + r->up_serif + l->down_serif + r->down_serif;

	DIS_CURVE(l,r,4,1,tab_l[8]);				// 10

	if ( (lmu+rmu)<<1>(lmd+rmd) )  {
	    if (wid<4 && lmu==1 && rmu==1 && lmd==1 && rmd==1 && ser>5 ||
		dy<=30 && lmu>1 && rmu>1 && lmd>1 && ser>4 ||
		dy>30  && lmu>2  && rmu>2 && lmd>2 )
			dis += tab_l[10];  /* up and down serifs  */	// 70
	    }
	}

if( inc )  {  /* inc may form false pimple : hard conditions for find serifs */
	if ( rmu>2 && lmu>1 && rmd>1 && lmd>2 )
		dis +=  tab_l[10];	// up and down serifs		// 70
	}
if ( inc  &&  (lmu + wid + wid + rmd) > dy )	// 08.07.1993
		dis += 222;		// stdk11/34 'z' => 'l'		//222

					// 11.01.1994  SPEC.TYPFEACE of liga_i:
if (lmu>=3  &&  l->m_pos[0]>=6  &&	// stdf7/7**,27,39,46 and
    abs (l->m_pos[0]-s->base_2)<=2)	// stdf12/12,25***,26
		dis += 20;		// for der Laterne;

/* ----------------------- STUDY ANGLES ---------------------- */
t = 1;
num_l = (lmu>t)+(rmu>t)+(lmd>t)+(rmd>t);	// number of long flags
num_z = (lmu==0)+(rmu==0)+(lmd==0)+(rmd==0);	// number of empty angles

if ( (num_z==3) &&					// 01.06.1993
     (rmu>0 && s->r_f_symptom && s->l_f_symptom  ||	// Similar 'f'  ||
     (s->lll_nose_1==0  &&				// Similar '1'
      (num_l==1 && rmd==0 && r->mount[3]==0 ||
       (lmu>1 || lmu>0&&dy<17) && s->dis_nose_1==0 && r->mount[3]==0))))
		dis += tab_l[11];	// similar 'f','1'	// 48

if ( (num_l==1) &&				// 09.07.1993, 12.10.1993
     (rmd>MAX(wid,4)) && (l->conc[4]*2<wid))	// stdg19/8(11) 'l' (3:4)
		dis += tab_l[11];	// similar 'L'		// 48

if ( num_l==3 && lmu>0 && s->typ_nose_1==1 &&	// OLD OLEG   &&
     ( (pitchsize==0) || (l->m_pos[0]>=MAX(wid,5)-1) ) &&	// 01.06.1993 &&
     lmu >= lmd)					// 31.05.1993
	dis += tab_l[11];	// similar '1'			// 48

if (num_l==3 && abs(s->inc_num)>1 && s->cut_r && rmu<2)	// 10.12.1993	EDIT;
	dis += 40;	// PROBA-40 for 'I' INC,CUT 		// 40

if( num_l==3 && lmu>=4 && rmu>=4 && rmd<=4 )
	dis += tab_l[11];	// LENKA SPEC CASE: 'U'=>"Ll"	// 48

if( num_l==2 && num_z==2 && r->down_serif && l->down_serif )
	dis += tab_l[11];	/* two half-serifs only */	// 48

if ( ( lmd==0 || (lmd==1 && dy>30) || (rmd<<1>dy && lmd<3) ) &&
     (l->conc[4]<1 || l->ce_pos[4]==dy-1) )	// find L-configurations
	{
	if( rmd>wid-1 && wid>4 && (lmu<<1)<rmd && rmu<rmd)	// for BUMERANG
		dis += tab_l[13];				// 80
	else if( !inc )  {
		if ( rmd > (wid*3)>>1 )	dis += tab_l[13];	// 80
		else if ( rmd>=wid )	dis += tab_l[13]>>1;	// 40
		}
	}

if ( s->neck==2 &&
     ( !(l->mount[0] && l->mount[4] && r->mount[4]) )  &&
     ( lmu<2  ||  lmu==2 && l->m_meandr>1 && dy>20 ) )		// MK EDITION
		dis += tab_l[12];				// 20

if ( s->neck==3 )				// 18.02.1993	//m.b.NECK=3
		dis += 100;			// 15.12.1993	// 100

				// MK-SOPLI 26/28.01.1992 for 'I'=>'l'
if (dis<tab_l[10] && lmu>tt && rmu>tt && lmd>tt)  dis = tab_l[10];	// 70;

if (s->cut_r &&		// 09.06.1993 PROBA for CUT. "The" (stdj10);
    (r->mount[0] || r->m_meandr==0) &&
    !r->down_serif &&
    l->mount[0]>MAX(wid,5))	// second
	dis += (rmu+2)*80;	// PROBA rmu"*80		// *80
}
/*......................................................................*/
else    {						// EMPTY ANGLES:
	if (s->neck==3)				// 15.12.1993 EDITION:
		dis += 100;						// 100
	else if ( s->neck==2  &&  dy<27 )	// "NO ZIRAFA"
		dis += tab_l[12];					// 20
	else if ( s->neck==1 )
		FOUR_CONC(l,r,tab_l[12]);				// 20
	}
/*......................................................................*/
/* N.B. in group macros (CENTER_FLAG) used ->c_meandr.	*/
/*	If c_meandr==0 and exist near central flag,then	*/
/*	not discrim. For narrow sticks I wont check	*/
/*	near flags . -->> I increase signums c_meandr	*/
/*	for left and right chars of stick		*/
if( wid<3 )  { l->c_meandr++;  r->c_meandr++;  }	// MODIFY c_meandr !!!


				// ABOUT ONE PIXEL		// 23.02.1993
	d_L = d_R = tab_l[7];	// DEFAULT			// =12
	if (s->neck<2)  {	// see OLD VARIANT in dis_I !!!!!!!!!!!!!!
	    if ( !s->cut_l &&
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!l->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !l->num_flags)))  d_L = 0;
	    if ( !s->cut_r &&
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!r->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !r->num_flags)))  d_R = 0;
	    }

//DIS_CENTER_CONC(l,2,inc,tab_l[6],d_L);    // 23.02.1993 // 60,12(0)
//DIS_CENTER_CONC(r,2,inc,tab_l[6],d_R);    // 23.02.1993 // 60,12(0)

// special polish
if( l->mount[2]<3 || r->mount[2]<3 )
  {
  int16_t rm=r->mount[0],lm=l->mount[0];
  if( rm<r->mount[1] ) rm=r->mount[1];
  if( rm<r->mount[2] ) rm=r->mount[2];
  if( lm<l->mount[1] ) lm=l->mount[1];
  if( lm<l->mount[2] ) lm=l->mount[2];

  if( rm<2 && lm<2 )
    dis += 40;  // too small pits or absent mounts
  else if( l->mount[2]<2 && r->mount[2]==0 && r->mount[1]==0 )
    dis += 30;

  if( l->conc[2]>1 && r->conc[2]>1 && wid<7 )
    dis += 20*MAX(r->conc[2],l->conc[2]);

  if( l->mount[1]+l->mount[2]+r->mount[1]+r->mount[2]<3 )
    dis +=10;

  if( l->mount[3]==0 && l->mount[2]==0 )
    dis += 30;

  if( r->mount[4] )
    {
    rm = MAX(r->mount[1],r->mount[2]);
    if( r->mount[4]==1 )
      {
      if( rm<1 )
        dis+=80;
      }
    else
      {
      if( r->mount[4]<3 && rm<2 )
        dis+=80;
      if( r->mount[4]>2 && abs(r->mount[4] - rm)>2 )
        dis+=80;
      }
    }
  if( l->mount[4] )
    {
    if( l->mount[4]==1 )
      {
      if( l->mount[2]<1 )
        dis+=80;
      }
    else
      {
      lm = MAX(l->mount[1],l->mount[2]);
      if( l->mount[4]<3 && lm<2 )
        dis+=80;
      if( l->mount[4]>2 && abs(l->mount[4] - lm)>2 )
        dis+=80;
      }
    }
  }
if( wid<3 )  { l->c_meandr--;  r->c_meandr--;  }	// RESTORE c_meandr !!!

if( wid*2 > s->height )
	dis += tab_l[14];	/* too wide stick : bad proportions */	// 80

if( l->mount[1]>0 && r->mount[1]>0 && r->mount[4]>0 && sl==1 && sr==2 )
	dis += tab_l[15];	/* similar 't' */			// 80

			// 09,10,11.02.1993	CASE NECK=1 wid>=6 (see dis_0xBA)
if ( (s->neck==1) && (wid>=6) &&
     (
       ( l->conc[0] && r->conc[0] &&	// fax 27/7 "Capability", first 'є'
	 ( l->cb_pos[0]==r->cb_pos[0] && abs(l->cb_pos[0]-s->base_2)<4  ||
	   l->ce_pos[0]==r->ce_pos[0] && abs(l->ce_pos[0]-s->base_2)<4 ) &&
	   l->cb_pos[0]!=l->ce_pos[0] && r->cb_pos[0]!=r->ce_pos[0] )
	 ||
       ( l->conc[1] && r->conc[1] &&	// fax ....................... 'є'
	 ( l->cb_pos[1]==r->cb_pos[1] && abs(l->cb_pos[1]-s->base_2)<4  ||
	   l->ce_pos[1]==r->ce_pos[1] && abs(l->ce_pos[1]-s->base_2)<4 ) &&
	   l->cb_pos[1]!=l->ce_pos[1] && r->cb_pos[1]!=r->ce_pos[1] )
   ) )
		dis += tab_l[17];					// 6

			// 12.02.1993	thin LEG exist:
if ( l->conc[4] && r->conc[4] &&	// j1/16 "CURTIS": 'R'=>"II"
     (l->ce_pos[4] - l->cb_pos[4] + r->ce_pos[4] - r->cb_pos[4]) >= (dy>>1) )
		dis += tab_l[18];					// 20

			// 19.11.1993	l,I  similar 'E' ("THEATRE")
if (wid*2+1>=dy &&	// k11/31 "PERFORMERCE", last 'E' 21*11, wid=10
    r->conc[1] && r->conc[3] &&
    (r->conc[1] | r->conc[3]) > 1)	// all cases, exclude (1,1)
	dis += 100;						// 100
/*......................................................................*/
					// 17.01.1994 MOVE IT TO END:
	if (dis==0)  dis += mk_dis_for_liga_exm;	// (0 or 2);

	return(dis);
}

/*----------------------------------------------------------------------*/
/* °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° */
/* °°°								°°° */
/* °°°		functions for adding new versions		°°° */
/* °°°								°°° */
/* °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° */
/*----------------------------------------------------------------------*/
int16_t similar_0xBA (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
{
return(dis_0xBA(l,r,s,1)<MAX_ADD_DIS);
}

int16_t similar_0xBB (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
{
return(dis_0xBB(l,r,s,1)<MAX_ADD_DIS);
}
/*----------------------------------------------------------------------*/
int16_t similar_1 (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
{
return(dis_1(l,r,s,1)<MAX_ADD_DIS);
}

int16_t similar_l_stroked (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
{
return(dis_l_stroked(l,r,s)<MAX_ADD_DIS);
}
/*----------------------------------------------------------------------*/
/* similar to 'T' */
static int16_t dis_7 (STICK_SIGNUMS *s)
{
int16_t     dis=0;

if( language == LANG_RUSSIAN )
        {
	if( s->incline<16*8 )
                dis = 100;
//        if( r->mount[0]>4 )
//                dis += 100;
        }
return(dis);
}


int16_t dis_d_croatian (STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t     dis=0;

if( language == LANG_CROATIAN )
        {
        if( r->mount[2]>2 || r->mount[3]>2 || r->mount[4]>2 )
                dis += 100;
        if( r->mount[1] && r->m_pos[1]>s->base_2 )
                dis += 100;
        if( r->mount[0]==0 && l->mount[0]==0 ||
            r->mount[1]==0 && l->mount[1]==0 )
                dis += 20;
        }
return(dis);
}

int16_t dis_d (STICK_CHARS *l, STICK_CHARS *r)
{
int16_t     dis=0;

if( language == LANG_CROATIAN )
        {
        if( r->mount[0]>1 && l->mount[0]>1 )
                dis += 100;
        }
return(dis);
}
/*----------------------------------------------------------------------*/
	// End of STIC_DIS.C
