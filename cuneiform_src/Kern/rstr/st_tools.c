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
/*	ST_TOOLS.C	28.01.1994	old S_TOOLS.C			*/
/*----------------------------------------------------------------------*/
//////#define	MKPRINT_ENABLE
#define	mk_80_for_CUT	80
/*----------------------------------------------------------------------*/
/*

  ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟
  ╟╟                                                              ╟╟
  ╟╟     S_TOOLS.C                                                ╟╟
  ╟╟	 functions cell-manipulations and abris modification      ╟╟
  ╟╟     data of last modification :  03.11.92.                   ╟╟
  ╟╟                                                              ╟╟
  ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "func.h"
#include "ligas.h"
#include "lang.h"
#include "tuner.h"
#include "linutil.h"	// 31.05.2002 E.P.
#include "minmax.h"

static int16_t correct_braces(uchar fun[],int16_t n,int16_t lev,int16_t typ);
static int16_t num_zero_intervals(uchar fun[],int16_t n,int16_t lev);
static int16_t dest_to_comp( uchar raster[],int16_t hei, int16_t wid);
static int16_t off_shift_string( uchar string[],int16_t len );
static int16_t kill_stick(version *v);
static int16_t kill_version( uchar prob);

  extern uchar db_status    ;	// snap presence byte
  extern uchar db_trace_flag;    // snap-detail presence byte
  extern uchar db_pass      ;    // snap-pass indicator
  extern int16_t nIncline      ;
  extern int16_t pitchsize     ;
//////  extern int16_t pen_up, pen_dn;	// 10.01.1994	for OTLADKA PRINT ONLY;
//  extern int16_t pen_up;		// 17.01.1994	for OTLADKA 't'
  extern uchar line_tabcell;

  extern uchar enable_table_recog; // Oleg : 10-02-95 09:05pm : sheet version
  extern Bool test_alphabet_elem(uchar let);
/*......................................................................*/
uint16_t	mkm1=0x1111, mkm2=0x2222, mkm3=0x3333,	// MK Variables
	mkm4=0x4444, mkm5=0x5555;
#ifdef	MKPRINT_ENABLE
uint16_t	mkma=0xAAAA, mkmb=0xBBBB, mkmc=0xCCCC, mkmd=0xDDDD, mkme=0xEEEE;
uint16_t	d,d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10;	// MK OTLADKA
#endif
uint16_t	mkFlag_Dump = FALSE;
//////static	int16_t	mk_INT_short_snap_last_typ;	// 18.06.1993	SOPLI
uint16_t	N_eq_Neck;					// 28.07.1993
uint16_t	left_mode_EEM;	// NOTA BENE:  NEPORJADOK; see CHA, DEF, DIS;
int	inc_num_EEM;	// 15.11.1993;  formed in STIC_MAK.C;
uchar	inc_char_EEM;	// 16.11.1993 for SNAP only;  formed in STIC_MAK.C;
int	dis_LIMIT_EEM;	// 18.11.1993;
int	mk_dis_for_liga_exm;	// 06.01.1994
uchar	left_letter_EEM;	// 17.01.1994
/*......................................................................*/

#define bytlen(bit) (((bit)+7)>>3)

/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */
/* ╟╟╟           FUNCTIONS FOR CELL-MANIPULATIONS               ╟╟╟  */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */

/* add version : if '*' or  'Ч' (spec symbols) then kill primary versions */
void new_vers( cell *c, uchar vers, uchar prob)
{

int16_t n=(c->flg&c_f_dust)?0:c->nvers,spec_sym=( chkbullet(vers) || vers=='*' );
cell *tmp = c->prev;

if( enable_table_recog )
if( !test_alphabet_elem(vers) )  return;

if( !(c->flg&(c_f_let|c_f_bad)) )
	{       /* add to list of letter */
	while( !(tmp->flg&(c_f_let|c_f_bad|c_f_fict)) )
		tmp = tmp->prev;
	tmp->nextl->prevl = c;
	c->nextl          = tmp->nextl;
	tmp->nextl        = c;
	c->prevl          = tmp;
	}

c->flg  = c_f_let;

if( prob==254 && spec_sym )
	{
	c->vers[0].let = vers;
	c->vers[0].prob = prob;
	c->nvers=1;
	}
else
	{       /* no spec symbol or spec symbol with small prob */
	if( n<7 )
		{       /* nvers<7 */
		c->vers[n].let  = vers;
		c->vers[n].prob = prob;
		c->nvers++;
		}
	else
		{        /* nvers==7 */
		sort_vers(c);
		c->vers[6].let  = vers;
		c->vers[6].prob = prob;
		}
	sort_vers(c);
	}

return;
}

/* set letter (let) propability to (prob) if prob > old propability */
/* return FALSE if letter (let) not exist in cell *c                */
Bool set_prob(cell *c, char let, uchar prob)
{
int16_t i,n=c->nvers;
for(i=0;i<n;i++)
	if( c->vers[i].let==(uchar)let )
		{
		if( prob > c->vers[i].prob )
			c->vers[i].prob = prob;
		break;
		}
sort_vers(c);


// Nick 19.02.2001 - add 0, else may be ERROR!!!
// - some places tests only vers->let, not c->nvers !
 if( c->nvers >= VERS_IN_CELL )
	 c->nvers = VERS_IN_CELL-1;

 c->vers[c->nvers].let  = 0;
 c->vers[c->nvers].prob = 0;


return( i<n );
}

/* decrease propability for any version in cell */
void down_all_versions(cell *c, int16_t monus)
{
int16_t i,p;
for(i=0;i<c->nvers;i++)
  {
  p = c->vers[i].prob - monus;
  c->vers[i].prob = MAX(p,2);
  }
sort_vers(c);
return;
}

/* add version (let,prob) if she not exist, else correct propability */
void add_stick_vers(cell *c,char let,uchar prob)
{
if( !check_let(c,let) )
	new_vers(c,let,prob);	/* adding new version */
else
	set_prob(c,let,prob);	/* increasing old version */
return;
}

Bool check_let(cell *c, char let )
{
uchar i,l=(uchar)let,n=(uchar)c->nvers;
version *bv;

for(bv=c->vers,i=0;i<n;i++,bv++)
	if( bv->let==l )
		return(TRUE); /* let belong list of versions */
return(FALSE);
}
/*----------------------------------------------------------------------*/
int16_t del_sticks (cell *c, char let)  {			// 26.10.1993
int16_t	i, n=c->nvers;
int16_t	prob;
int16_t	dis_CUT=0;
int16_t	base_4_1=0;	// BBB
B_LINES bl;

	if (c->cg_flag & c_cg_cutl)  dis_CUT  = mk_80_for_CUT;	// CUT LEFT
	if (c->cg_flag & c_cg_cutr)  dis_CUT += mk_80_for_CUT;	// CUT RIGHT
	if (dis_CUT)  {
		get_b_lines (c, &bl);
		base_4_1 = bl.b4 - bl.b1;
		}

	for(i=0;i<n;i++)  {
	    if (dis_CUT &&		// 21.10.1993: This - BEFORE CONTINUE !
//////		(c->vers[i].let == '('  ||  c->vers[i].let == ')'  ||
		(c->vers[i].let == '('  &&  abs(c->h-base_4_1)>3   ||  //26.10.1993
					    c->vers[i].let == ')'  ||
		 c->vers[i].let == '['  ||  c->vers[i].let == ']'  ||
		 c->vers[i].let == '{'  ||  c->vers[i].let == '}'  ||
		 c->vers[i].let == '<'  ||  c->vers[i].let == '>'))  {
		    prob = (&c->vers[i])->prob;
		    c->vers[i].prob = (prob>dis_CUT) ? (prob - dis_CUT) : 2;
		    }

	    if (c->vers[i].let==let)  continue;	// protect version

      if( language==LANG_CROATIAN &&
		  (c->vers[i].let=='d'||c->vers[i].let==CROAT_d) )
        continue;

	    kill_stick(&(c->vers[i]));		// stick version (wkl. "[]")
	    }	// FOR I

	sort_vers(c);				// verions changed
	return( 1 );
}
/*----------------------------------------------------------------------*/
int16_t	del_sticks_F_60 (cell *c)  {			// 05.03.1993
int16_t	i, n=c->nvers;
int16_t	prob_F;
uchar let;

for(i=0;i<n;i++)
	{
	let = c->vers[i].let;

	if( let!='F')    // SPECIAL CASE
		{
		  if( !(language==LANG_CROATIAN &&(let=='d'||let==CROAT_d))
		 /*&& !( language == LANG_POLISH && c->vers[i].let==POLISH_l )*/
		    ) // Nick - restored 29.09.2000
				kill_stick(&(c->vers[i]));  // stick version
		}
	  else
		{
		prob_F = (&c->vers[i])->prob;
		prob_F -= 60;
		if (prob_F<=0) prob_F = 2;
		c->vers[i].prob = (uchar)prob_F;
		}
	}

	sort_vers(c);				// verions changed
	return( 1 );
}
/*----------------------------------------------------------------------*/
/* decrease letter-propability for stick codes  */
static int16_t kill_stick(version *v)
{
uchar let = v->let ;			// A.A.LEMAN from 28.06.1993
char stick_list[]="frtIJT1l!ij/[]{}LFY";	// 22.11.1993		//17/19
if( memchr(stick_list, let,17)!=NULL             ||
    ( language == LANG_CROATIAN && (let==CROAT_d||let=='d') ) ||
    ( language == LANG_POLISH && let==POLISH_l ) ||
    ( language != LANG_RUSSIAN &&
	 (let==liga_i  || let==liga_j  ||

		!is_baltic_language(language) && 	// 17.07.2001 E.P.
		!is_turkish_language(language) &&
		(
		let==i_right_accent ||     // 08.07.1993 FRENCH:
        let==i_roof_accent  ||
		let==II_right_accent||
        let==II_roof_accent ||

		// йНМТКХЙРМШЕ ЙНДШ // 07.09.2000 E.P.
		!is_cen_language(language) && (

		let==i_left_accent  ||
		let==i_2dot_accent  ||
		let==II_left_accent ||
		let==II_2dot_accent ) ) )
	  ) ||

	  // 05.09.2000 E.P.
      is_cen_language(language) &&
		  (
		  let=='d' || let== 't' ||
		  let == d_inv_roof ||
		  let == t_inv_roof ||
		  let == t_bottom_accent
		  ) ||

		is_baltic_palka(let) ||	// аЮКРХИЯЙХЕ ОЮКЙХ. 10.07.2001 E.P.
		is_turkish_palka(let)	// рСПЕЖЙХЕ ОЮКЙХ. 21.05.2002 E.P.

  )
		v->prob = (uchar)kill_version(v->prob);
return( v->prob );
}

static int16_t kill_version( uchar prob)
{
prob>>=2;
if( prob&1 )
	prob--;
return( prob  );
}
///////////
static int dotIsGlue(cell *c)
{	// Nick 23.06.2002
#define MAX_UP 32
 lnhead *line;
 interval *intval;
 int16_t l,i,start;
 int maxH = MIN(MAX_UP,c->h/3);
 int size[MAX_UP];

	if( !c->env || maxH <= 1 )
		return 0;

	memset (size, 0, maxH*sizeof(int));

	for ( line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
		       (l=line->lth)>0; line=(lnhead *)((pchar)line+l)  )
	{
		start = line->row;

		intval=(interval *)((pchar)line+sizeof(lnhead));
		for ( i = 0; i < line->h ; i++, intval++)
		{
			if( start + i  >= maxH )
				break;

			size[start +i] += intval->l ;
		}
	}

	for(i=0, start = 0; i<maxH; i++)
	{
		if( size[i] <= 0 )
			return 0;

		if( size[i] > size[start] )
			start = i;
	}

	// Nick 09.07.2002
	if( start == 0 )
		return 0;

	return 1;
}
////////////////
/*----------------------------------------------------------------------*/
/* revaluation all propabilitys in list of version  */
int16_t	discrim_all_sticks (cell *c,
		STICK_CHARS *left_chars, STICK_CHARS *right_chars,
		STICK_SIGNUMS *signums)
{							// 27.01.1994
int16_t  i, n=c->nvers, num, tmp, c_f=check_let(c,'f');
int16_t  prob;
uchar let;
B_LINES bll;

	get_b_lines(c,&bll);

	for (num=i=0; i<n; i++)
	{
	  let = c->vers[i].let;
	  if( let=='|' && (c->pos_inc&erect_rot) && c->stick_inc>256 )
		  let='i';

	  // Nick 23.06.2002
	  if( is_turkish_language(language) && // 04.06.2002 E.P.
		  let == II_dot_accent &&
		  c->row <= bll.b1 &&
		  dotIsGlue(c)
		)
		continue;

	  tmp = discrim_stick(let, left_chars,right_chars, signums, c_f);

	  if( tmp )
		{
			num ++;
			prob = c->vers[i].prob - tmp;		// 10.01.1994:
			if (prob<2)   prob = 2;		// CENSURE for NORMAL;
			if (prob>254) prob = 254;	// CENSURE for BONUS (t^...);
			if (tmp==-444)			// INCREASE SPEC.CASE "ft"
				prob = 200;		// 27.01.1994 (for Igor)
			c->vers[i].prob = (uchar)prob;
		}
	}

	if( num>0 )
		sort_vers(c);
return(num);
}
/*----------------------------------------------------------------------*/
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */
/* ╟╟╟           FUNCTIONS FOR FILTRATE ABRIS-ARRAY             ╟╟╟  */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */
/* shave near pimples              */
void filtr_short(uchar fun[],int16_t n,int16_t lev)
{
uchar i;
lev++;
for(i=1;i<n-1;i++)
	if( fun[i-1]==fun[i+1] && abs(fun[i]-fun[i-1])<lev )
		fun[i] = fun[i+1];
return;
}

/* filtr 121 : fun[i] = fun[i-1]+2*fun[i]+fun[i+1]    */
void filtr121(uchar fun[],int16_t n)
{
#define LIM_OF_DIST 6
int16_t i,fprev=fun[0],fcurr,fnext;
for(i=1;i<n-1;i++)
	{
	fnext = fun[i+1];
	/* N.B. : filtrate near jumps only ! */
	if( abs(fnext+fprev-(fun[i]<<1))<LIM_OF_DIST*4 )
		{
		fcurr  = fprev+(fun[i]<<1)+fnext;
		fprev  = fun[i];
		fun[i] = fcurr>>2;
		}
	else
		{ /* disable correct current fun[i] : long flags */
		fcurr  = fprev+(fun[i]<<1)+fnext;
		fprev  = fun[i];
		}
	}
#undef LIM_OF_DIST
}

/* filtr_bullet : for fun[i-1]=fun[i+1]=0 set fun[i]=0 */
void filtr_bullet(uchar fun[],int16_t len)
{
int16_t i;
for( len--,i=3; i<len; i++) /* first and last elem skipped */
	if( fun[i]==0 && fun[i-2]==0 && fun[i-1]!=0 )
		{
		fun[i-1]=0;     /* filtrate */
		i++;            /* forward  */
		}
return;
}

/*  fultr_shave : fun[i-1]=fun[i+1] and fun[i]!=fun[i-1] set fun[i]=fun[i-1] */
void filtr_shave(uchar fun[],int16_t len)
{
int16_t i;
for( len--,i=3; i<len; i++) /* first and last elem skipped */
    if( fun[i]==fun[i-2] && abs( fun[i-1]-fun[i] )==1 )
		{
		fun[i-1]=fun[i];     /* filtrate */
		i++;            /* forward  */
		}
return;
}

int16_t find_minimum(uchar fun[],int16_t n,uchar *_imin)
{
uchar i, imin, minim, io, ff;

for(imin=0,minim=fun[0],i=1;i<n;)
	{
	io=i;
	ff=fun[i++];

	while( i<n && fun[i]==ff )i++;  /* fun[i] = const */

	if( minim>ff )
		{
		minim = ff;       /* imin - center of interval [io,i] : */
		imin=(i+io)>>1;   /*        fun[k]=const for io<=k<=i   */
		}

	}

*_imin = imin  ;

return(minim);
}


/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */
/* ╟╟╟           FUNCTIONS FOR ADDING NEW VERSIONS              ╟╟╟  */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */
/*----------------------------------------------------------------------*/
			// add '╨','1','!', liga_exm, invers_exm to cell:
void add_thick_stick_versions (cell *c, int16_t dx, int16_t wide, int16_t inc,
			   int16_t typ_snap,
			   STICK_CHARS *l, STICK_CHARS *r,
			   STICK_SIGNUMS *s)
{
int16_t	prob = conv_prob((int16_t)ADD_PROB(c));	// POSIT,EVEN  (prob.0 - 4  or  254)
//////int16_t  wid=s->stick_width;	// NB: wide - THE SAME ???
int16_t	prob_l = 0;	// 07.01.1994 for liga_exm !!!

//////	if (prob<140)  prob = 200;	// 17.01.1994, 200 - for der Laterne;
					// MANY ERRORS for CRASY LETTERS;

			// NB: VERSIONS ARE SORTED NOW !!!
if (c->nvers>=1 && c->vers[0].let=='l')  prob_l = c->vers[0].prob;  // 'l' #0;
else
if (c->nvers>=2 && c->vers[1].let=='l')  prob_l = c->vers[1].prob;  // 'l' #1;

//////mkm1 = full_wide;
//////mkm2 = (l->max_flag<<8) | r->max_flag;

if (s->neck==3)					// 16.12.1993  ADD ╨ FIRST:
//////	add_stick_vers (c, liga_i, cut_by_pos(c,liga_i,prob,1,1));
if(language != LANG_RUSSIAN)
	add_stick_vers (c, (char)liga_i,	// 17.01.1994, 140 for der Laterne:
			(uchar)cut_by_pos(c,liga_i,(char)(MAX(prob,140)),1,1));

if( !inc &&  wide<<1<=dx &&				// ADD ╨ SECOND:
    s->neck && typ_snap &&
    similar_0xBA (l, r, s) )
	/* no inc; exist neck; thin stick; similar '╨' */
//////	add_stick_vers (c,'╨',cut_by_pos(c,'╨',prob,1,1));
if(language != LANG_RUSSIAN)
	add_stick_vers (c, (char)liga_i, (uchar)cut_by_pos(c,liga_i,prob,1,1));

if( inc<2 &&         // ADD ╩ SECOND:
    s->neck && typ_snap &&
    similar_0xBB (l, r, s) )
if(language != LANG_RUSSIAN)
  add_stick_vers (c, (char)liga_j, (uchar)cut_by_pos(c,liga_j,prob,1,1));

if( !(c->cg_flag & c_cg_cutl ) && (dot_ij(c)==NULL) && typ_snap )  // ╨,1 ???
	{                   /* not left cut and not i-dot */
	if( similar_0xBA (l, r, s) )			// ADD ╨ THIRD:
	    if (c->nvers==0  ||  c->vers[0].let != '/')	// PROBA 25.06.1993
//////		add_stick_vers(c,'╨',cut_by_pos(c,'╨',prob,1,1));
if(language != LANG_RUSSIAN)
		add_stick_vers (c, (char)liga_i, (uchar)cut_by_pos(c,liga_i,prob,1,1));
/*......................................................................*/
	if( (check_let(c,'l')||check_let(c,'I')) &&		// ADD '1'
	    (!check_let(c,'1')) &&			// 08.07.1993 MK !!!
	    similar_1 (l, r, s)
	  )
		add_stick_vers (c,(char)'1',
			(uchar)cut_by_pos(c,'1',prob,1,1) );

  if( language == LANG_POLISH  &&
      (check_let(c,'t') || check_let(c,'1') ) &&   // ADD '1'
      (!check_let(c,(char)POLISH_l)) &&      // 08.07.1993 MK !!!
      similar_l_stroked (l, r, s)
	)
		add_stick_vers (c,(char)POLISH_l,
			(uchar)cut_by_pos(c,POLISH_l,prob,1,1) );

  }

/*......................................................................*/
if( !(c->cg_flag & c_cg_cut) && typ_snap )	// ADD '!'
	{                   /* not cut and exist !-dot */
	int16_t pr = ADD_PROB(c);
	if( language != LANG_RUSSIAN && pr<200 )
		pr=202; /* ????!!!!!! : for good glue */
	if( similar_excl (l, r, s) )
		{
		if( dot_excl(c)!=NULL )
			add_stick_vers(c,'!',(uchar)pr);
		else if( language == LANG_RUSSIAN &&  !line_tabcell  )
			{
			B_LINES bl;
			get_b_lines(c,&bl);
			if( c->nvers>0 && c->row<bl.bm )
				add_stick_vers(c,(char)liga_exm,(uchar)pr);
			}
		}
	else if( language == LANG_RUSSIAN &&  !line_tabcell && dot_excl(c)==NULL &&
		 similar_excl_with_dot (l, r, s) )
		{
		B_LINES bl;
		get_b_lines(c,&bl);
		if( c->nvers>0 && c->row<bl.bm )
			add_stick_vers(c,(char)liga_exm,(uchar)pr);
		}
	}

/*......................................................................*/
						// 07.01.1994  liga_exm !
				// gera21/11	"Republic!"	+
				//     21/18	"niens (!)"	+
				//     21/30	"gehangt!"	+
				// gerb1/11	"was!"		+
				//     1/18	"Oh!"		###
				// gerc13/33	"Wahrheit!"	+
				//     13/34	"auch Mut!"	### (l232 !232)
				// gerc17/4	"dem Wege!"	+
				//     17/5 "einzulassen!", "empfangen" ++
//////if (wide<=4  &&				// FIRST PROBA (06.01.1994)
//////if ((wide<=4 && full_wide<wide*2)  &&	// NEW PROBA (use max_flag's);
if ((s->right_mode - s->left_mode < 5)  &&	// 07.01.1994
    (l->conc[3] || r->conc[3])  &&		// (see DEF: "a_posteriory")
    (s->full_width <= wide+3)  &&
    (prob_l) && !line_tabcell)
    {			// SECOND VARIANT: 'l' #0 or #1;
    int16_t	tl, lbeg, lend, l_len,  tr, rbeg, rend, r_len,  prob_new;
    ////int16_t	mk_base = s->height - s->stick_width - 1;	// FIRST PROBA;
    int16_t	base_C = s->height - s->stick_width -2;		// for CONCAVE;
    int16_t	base_M = base_C + 1;				// for MOUNT;
	lbeg = lend = l_len = rbeg = rend = r_len = 0;  tl = tr = 3;
	if (l->conc[3])	{ lbeg = l->cb_pos[3];  lend = l->ce_pos[3]; }
		else if (l->conc[2])
			{ lbeg = l->cb_pos[2];  lend = l->ce_pos[2]; tl = 2; }
		else  lend = -64;	// zB sic, INDICATOR NO LEFT (LEN=-63);
	if (r->conc[3])	{ rbeg = r->cb_pos[3];  rend = r->ce_pos[3]; }
		else if (r->conc[2])
			{ rbeg = r->cb_pos[2];  rend = r->ce_pos[2]; tr = 2; }
		else  rend = -96;	// zB sic, LEN=-95 (DIFFERENT OF -63);
	l_len = lend - lbeg + 1;	r_len = rend - rbeg + 1;
    mkm3 = (lbeg<<8) | lend;
    mkm4 = (rbeg<<8) | rend;
    mkm5 = (l_len<<8) | r_len;
	if (abs (MAX(lend,rend)-base_C) < 2  &&		// COMMON CONDITION;
	    (abs (lend-rend) < 2  &&  (l_len>3 || r_len>3)  ||		// A;
    //////	     l->mount[0] && l->mount[4] && r->mount[0] && r->mount[4] || // B;
			// DELETE 07.01.1994 because IT IS DIRECT SERIF 'I';
	     l->mount[4] && r->mount[4] &&				// C;
	     abs (l->mb_pos[4]-base_M) < 2 &&
	     abs (r->mb_pos[4]-base_M) < 2  ||
	     l->conc[tl] + r->conc[tr] >=2 &&				// D;
	     (l->conc[tl]>=2 && abs(lend-base_C) < 2 ||			// D.1;
	      r->conc[tr]>=2 && abs(rend-base_C) < 2)))
            {		// D.2;
		    prob_new = prob_l;
		    if ((l->conc[tl] + r->conc[tr]) >= ((wide+1)>>1))
			    prob_new += 2;			// MORE;
		    if (s->neck)  prob_new -= 2;		// LESS; (ENTE);
		    if (prob_new>254)  prob_new = 254;	// CENSURE;
		    add_stick_vers (c, (char)liga_exm, (uchar)prob_new);
            //////		sort_vers (c);			// VERY VERY OLD (COPY OF...);
            //////		mk_dis_for_liga_exm = 2;	// 06.01.1994
		    mk_dis_for_liga_exm = (prob_l==254) ? 2 : 0;	// l 254->252;
		    }
	    }
					// NOTA BENE: without TEST of LINEAR;
/*......................................................................*/
if (language == LANG_SPANISH	&&		// 03.01.1994  INVERS EXM !
    s->base_2mk+2 < 0		&&
    s->base_3mk+2 < s->height	&&
    l->num_long_flags + r->num_long_flags == 0)  {
//////		add_stick_vers (c, invers_exm, pr);
		add_stick_vers (c, (char)invers_exm, (uchar)prob);	// ????????????
		}

return;
}
/*----------------------------------------------------------------------*/
/* add versions 't','f','(',')' for dx<=4 - small kegls */	// <=4 !!!
void add_thin_stick_versions (cell *c, uchar left[], uchar right[],
			   int16_t dy, int16_t dx, int16_t typ_inc,
			   STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t t = r->mount[0] + r->mount[1] + r->mount[2];
int16_t prob = conv_prob((int16_t)ADD_PROB(c));	// POSIT,EVEN  (prob.0 - 4  or  254)
B_LINES basL;

if( !typ_inc && dx<5 && language!=LANG_RUSSIAN )
  {
	//  Nick 10.09.2000 - for CZECH
	if (language==LANG_CZECH)
	{
      if( dy<22 && dy > dx*2 &&
          r->mount[0] && t > 1 &&
	      !r->conc[0] &&
	      l->conc[0]+l->conc[1] &&
	      !l->mount[0] &&
	      !l->mount[4] &&
          !check_let(c,(uchar)t_inv_roof)
        )
	   new_vers(c,t_inv_roof,(uchar)cut_by_pos(c,t_inv_roof,prob,1,1) );
	}

	//  Nick 10.09.2000 - for ROMAN
	if( language == LANG_ROMAN )
	{
      get_b_lines(c,&basL);
	  if( dy > dx*2 &&
		  !check_let(c,(uchar)t_bottom_accent) &&
          r->mount[0] + r->mount[1] &&
		  l->mount[0] + l->mount[1] &&
		  c->row < basL.b2 &&
          c->row + c->h > basL.b3 + (c->h/5 )
		 )
	    new_vers(c,t_bottom_accent,(uchar)cut_by_pos(c,t_bottom_accent,prob,1,1) );
	}

	/* adding 't','f' */
	if( dy<17 && t>0 && !check_let(c,'t') )
		new_vers(c,'t',(uchar)cut_by_pos(c,'t',prob,1,1) );

	if( dy<22 && t>1 && !check_let(c,'f') )
		new_vers(c,'f',(uchar)cut_by_pos(c,'f',prob,1,1) );
	}

if( dx<=4 && !typ_inc )  {		/* adding circle braces	*/
int16_t nzl = num_zero_intervals(left,dy,0);
int16_t nzr = num_zero_intervals(right,dy,(int16_t)((dx-1)<<2));
	if ( nzl==1  &&  nzr==2  &&  !check_let(c,'(')  &&
	    correct_braces(left,dy,0,1)  &&
	    correct_braces(right,dy,(int16_t)((dx-1)<<2),0)  &&
	    similar_lcb (l, r, s) )
		new_vers(c,'(',(uchar)cut_by_pos(c,'(',prob,1,1) );
	if ( nzl==2  &&  nzr==1  &&  !check_let(c,')')  &&
	    correct_braces(left,dy,0,0)  &&
	    correct_braces(right,dy,(int16_t)((dx-1)<<2),1)  &&
	    similar_rcb (l, r, s) )
		new_vers(c,')',(uchar)cut_by_pos(c,')',prob,1,1) );
	}
return;
}
/*----------------------------------------------------------------------*/
/* study limits of first and last columns for c_comp '(',')' */
static int16_t correct_braces(uchar fun[],int16_t n,int16_t lev,int16_t typ)
{
if( typ )
	{  /* right */
	if( fun[n/2]!=lev || fun[0]==lev || fun[n-1]==lev )
		return(0);
	}
else
	{ /* left */
	if( fun[0]!=lev&&fun[1]!=lev || fun[n-2]!=lev&&fun[n-1]!=lev )
		return(0);
	}
return(1);
}

/* number of intervals of const in column (lev) */
static int16_t num_zero_intervals(uchar fun[],int16_t n,int16_t lev)
{
int16_t i,s;
for(i=1,s=(fun[0]==lev);i<n;i++)
	{
	if( fun[i]==lev && fun[i-1]!=lev )
		s++;
	}
return(  s  );
}

/* transformation prob to correct (>0,even) number */
int16_t conv_prob(int16_t prob)
{
//////if( prob<=0 )
if( prob<=1 )		// 06.01.1994 for CASE 1;
	return( 2 );
if( prob&1 )		// ODD to
	prob--;		// EVEN;
return( prob);
}


/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */
/* ╟╟╟                       SNAP-FUNCTIONS                     ╟╟╟  */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */


/* out text string */
void short_snap(char *s,int16_t typ_snap)
{
//////mk_INT_short_snap_last_typ = typ_snap;	// DELETE 24.06.1993
if ( db_status && ( typ_snap==0 ||
		    (db_trace_flag & 4) &&  snap_activity(db_pass)) )
	{
	snap_show_text(s);
	if( typ_snap )
		snap_monitor();
	}
return;
}
/*----------------------------------------------------------------------*/
/* out full information of stick */			/* 07.01.1994	*/
void full_snap (cell *c,
		s_glue *GLU,	// 14.12.1993 add this parametr;
		STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		int16_t tab_angle[],
		int16_t left_mode, int16_t right_mode, int16_t opt,
		int16_t wide,	// INDICAT "5!" IF NE (right_mode-left_mode+1);
		int16_t inc_v, int16_t typ_snap)
{
int16_t	dx=c->w, dy=c->h;
MN	*sh_mn;
cell	*sh_cell;
c_comp	*buf_cell;
char	buf[1024],*pbuf;
puchar	raster;
uchar	sh_raster[1024];
int16_t	i, d_x, off, buf_w, c_f;
uchar	cUL = (l->up_hook)   ? '~' : ' ',  dUL = l->up_serif;
uchar	cUR = (r->up_hook)   ? '~' : ' ',  dUR = r->up_serif;
uchar	cDL = (l->down_hook) ? '_' : ' ',  dDL = l->down_serif;
uchar	cDR = (r->down_hook) ? '_' : ' ',  dDR = r->down_serif;
int16_t	cut_0123;
static	char	*vars_cut_0123 [] =  {	"NO CUT",	// 0
					"CUT R",	// 1
					"CUT L",	// 2
					"CUT L,R" };	// 3

if (db_status && (typ_snap==0 ||
		 (db_trace_flag & 4) && snap_activity(db_pass)) )
	{
	c_f = check_let(c,'f');
	if (inc_v)  {			/* exist inc */
		for (i=0; i<dy; i++)
			tab_angle[i] = (tab_angle[i]-opt)>>2;
		raster = save_raster (c);
		off = off_shift_string (&raster[(dy-1)*bytlen(dx)],
					(int16_t)(bytlen(dx)) );
		d_x = shift_raster (raster, dy, dx, tab_angle,
      (int16_t)(MAX (tab_angle[0], tab_angle[dy-1])), sh_raster, 1);
		off -= off_shift_string (&sh_raster[(dy-1)*bytlen(d_x)],
					 (int16_t)(bytlen(d_x)) );
		off += dest_to_comp (sh_raster, dy, d_x);
		/* off - offset shift raster */
		right_mode -= off;
		left_mode  -= off;
		opt        -= off<<2;
		sh_mn = c_locomp (sh_raster, (int16_t)(bytlen(d_x)), dy, 0, 0);
		sh_cell = create_cell (sh_mn, c, c->bdiff, (char)(c->difflg & 0xf0));

		buf_cell = c->env;
		buf_w = c->w;
		del_cell (sh_cell);
		c->env = sh_cell->env;		/* show inc raster	*/
		c->w   = sh_cell->w;
		}

	pbuf = &buf[0];
/*......................................................................*/
/*********************************************	MK NEW FORM  08.01.1992
	 1         2         3         4         5         6         7         8
123456789o123456789o123456789o123456789o123456789o123456789o123456789o123456789o

   V A L U E S    ~HOOK~   P O S I T I O N S		NOTA BENE: WIN - ???
   LEFT  RIGHT    SERIFS     LEFT  RIGHT
   Fl Co Co Fl    SERIFS     Fl Co Co Fl
   Mo Co Co Mo    _HOOK_     Mo Co Co Mo
--------------------------------------------------------------------------------
0)╨ 0Ё 0╨ 0Ё 0╨V здддбддд© C╨ 0Ё 0╨ 0Ё 0╨ wid=4 (2Ж5) col=3.5 inc=0(8) {INC}
1)╨ 0Ё 0╨ 0Ё 0╨A Ё~0 Ё 0~Ё O╨ 0Ё 0╨ 0Ё 0╨ base_2=7  NO NECK  NO CUT
2)╨ 0Ё 0╨ 0Ё 0╨L цдддеддд╢ O╨ 0Ё 0╨ 0Ё 0╨ typ_1=1  typ_f=1,0  T40  Y80  {Ь}
3)╨ 0Ё 0╨ 0Ё 0╨U Ё_1 Ё 2_Ё R╨ 0Ё 0╨ 0Ё 0╨ dis_1=0  d_/=0  d_╨=0  d_()=0,0
4)╨ 0Ё 0╨ 0Ё 0╨E юдддаддды D╨ 0Ё 0╨ 0Ё 0╨ MK: 1111 2222 3333 4444 5555
n)1,5Ё 0Ё 0Ё4,2  SER.HOOK  m) 0Ё 0Ё 0Ё 0 (meandr)
Discrim: F-180 f-50 ╨-20 t-10 i-0 l-0
--------------------------------------------------------------------------------
123456789o123456789o123456789o123456789o123456789o123456789o123456789o123456789o
	 1         2         3         4         5         6         7         8
Comments:
Line-6: n) num_long_flags,num_flags num_concs
	m) m_meandr, c_meandr
--------------------------------------------------------------------------------
*************************************************************************/
/*......................................................................*/
for (i=0; i<5; i++)  {
  pbuf+= sprintf(pbuf,"%d)|%2d|%2d|%2d|%2d|",
		i, l->mount[i], l->conc[i], r->conc[i], r->mount[i]);
	switch (i)  {	// FIRST SWITCH
case 0: pbuf+= sprintf(pbuf,  "+---+---+");     break;
case 1: pbuf+= sprintf(pbuf,"|%c%d | %d%c|", cUL,dUL,dUR,cUR);  break;
case 2: pbuf+= sprintf(pbuf,  "+---+---+");     break;
case 3: pbuf+= sprintf(pbuf,"|%c%d | %d%c|", cDL,dDL,dDR,cDR);  break;
case 4: pbuf+= sprintf(pbuf,  "+---+---+");     break;
default:	break;	// BBB
		}	// END OF FIRST SWITCH

  pbuf+= sprintf(pbuf,"|%2d|%2d|%2d|%2d| ",
		l->m_pos[i], l->c_pos[i], r->c_pos[i], r->m_pos[i]);
	switch (i)  {	// SECOND SWITCH
					// wid=4 (2Ж5) col=3.5 inc=0(8) {INC}
case 0:	pbuf += sprintf (pbuf,		// 25.01.1994 BREAK ONE PRINT to TWO;
      "wid=%d%s%s (%d-%d) %d.%02d ",  // 07.01.1994
			wide,
			(wide!=right_mode-left_mode+1) ? "!" : "", //07.01.1994
			(dis_LIMIT_EEM) ? "*" : "",
			left_mode, right_mode,
			opt>>2, (opt&3)*25);	// zB 2.00, 2.25, 2.50, 2.75;

	pbuf += sprintf (pbuf,		// 25.01.1994 BREAK ONE PRINT to TWO;
			"inc[%d]=%d (%d) %c%s",
			inc_num_EEM,		// 15.11.1993
			inc_v,
			nIncline,
			inc_char_EEM,		// m.b.  L ( _ ) R  or / \;
			(GLU) ? " GLUE" : "");
		break;
				       // base_2=7,15  NO NECK  NO OVER  NO CUT
//////case 1:	pbuf += sprintf(pbuf, "base=%d,%d  ", s->base_2, s->base_3);
case 1:	pbuf += sprintf(pbuf, "base=");		// 03.01.1994:
	if (s->base_2mk!=0x80)  pbuf += sprintf(pbuf, "%d,", s->base_2mk);
			  else  pbuf += sprintf(pbuf, "*,");
	if (s->base_3mk!=0x80)  pbuf += sprintf(pbuf, "%d,", s->base_3mk);
			  else  pbuf += sprintf(pbuf, "*,");
	if (s->base_4mk!=0x80)  pbuf += sprintf(pbuf, "%d  ", s->base_4mk);
			  else  pbuf += sprintf(pbuf, "*  ");
	if (s->neck)  pbuf += sprintf(pbuf, "NECK=%d  ", s->neck);
		else  pbuf += sprintf(pbuf, "NO NECK  ");
//////	pbuf += sprintf(pbuf, "CUT=%d,%d  ", s->cut_l, s->cut_r);
	cut_0123 = (s->cut_l)  ?  2 : 0;
	if (s->cut_r)  cut_0123++;
	pbuf += sprintf(pbuf, "%s  ", vars_cut_0123 [cut_0123]);
	pbuf += sprintf(pbuf, "Psz=%d", pitchsize);
	break;
				       // typ_1=1  typ_f=1,0  T40  Y80  {Ь}
				       // typ_1=1  typ_f=1,0  T40  Y80  ijЬ0
				       // typ_1=1  typ_f=1,0  T1,2,3,4  ijЬ0
//case 2:   pbuf += sprintf(pbuf, "typ_1=%d  typ_f=%d,%d  T%d  Y%d  ijЬ%d",
//case 2:   pbuf += sprintf(pbuf, "typ_1=%d%c t_f=%d,%d T%d,%d,%d,%d:%d ijЬ%d",
case 2: pbuf += sprintf(pbuf, "typ_1=%d%c f%d,%d t%c T%d,%d,%d,%d:%d ij-%d",
			s->typ_nose_1, (s->lll_nose_1) ? '*' : ' ',
			s->l_f_symptom, s->r_f_symptom,
			(s->t_TOP) ? '^' : '~',		// 09.01.1994 t^ or t~;
//////			s->T_config, s->Y_config, s->dis_up_dot);
//////			s->T_config, s->T_2, s->T_3, s->T_skok,
			s->T_config, s->T_2, s->T_3, s->T_skok_L, s->T_skok_R,
			s->dis_up_dot);
	/**********************************	from 05.08.1993 to 26.08.1993
	if (s->up_dot_NAVIS)
		pbuf += sprintf(pbuf, "/");
	else if (s->up_dot_W)
	***********************************/
	if (s->up_dot_H)			// 26.08.1993
		pbuf += sprintf(pbuf, ",%d*%d,%d",
				s->up_dot_H, s->up_dot_W, s->up_dot_dCOL);
		break;
				       // dis_1=0  d_/=0  d_╨=0  d_()=0,0
case 3: pbuf += sprintf(pbuf, "dis_1=%d d_/=%d d_jj=%d",
			s->dis_nose_1, s->dis_slash, s->dis_0xBA);
	if (s->dis_0xBA)				// 28.07.1993
		pbuf += sprintf(pbuf, ".%d", N_eq_Neck);
	pbuf += sprintf(pbuf, " d_()=%d,%d  LM=%d",
//////			s->dis_left_brace, s->dis_right_brace, left_mode_EEM);
			s->dis_left_brace, s->dis_right_brace,
			(int16_t) left_mode_EEM);	// for ERROR: 65533 => -3;
		break;
				       // MK: 1111 2222 3333 4444 5555
case 4:	pbuf+= sprintf(pbuf, "MK: %04X %04X %04X %04X %04X",
				mkm1, mkm2, mkm3, mkm4, mkm5);
//////	pbuf+= sprintf(pbuf, "  %d,%d,%d", c->nl, c->begs, c->ends); ??? PROBA
{///////////////////////////////////////////////////////////////////
c_comp	*env;
/**	if (c->env==NULL)				// 18.02.1994
		pbuf+= sprintf(pbuf, "  ENV==NULL;");
	else if (c->cg_flag&c_cg_noenv)  **/
	if (!tenv(c))
		pbuf+= sprintf(pbuf, "  noenv;");
	else	{
		env = c->env;
		pbuf+= sprintf(pbuf, "  Nlines=%d", env->nl);
		}
}///////////////////////////////////////////////////////////////////
		break;

default:	break;	// BBB
		}	// END OF SECOND SWITCH
	pbuf+= sprintf(pbuf, "\n");
	}	// FOR I
/*......................................................................*/
								// LINE-5:
			//n)1,5Ё 0Ё 0Ё4,2  SER.HOOK  m) 0Ё 0Ё 0Ё 0 (meandr)
	pbuf += sprintf(pbuf,
//////		"n)%d,%dЁ%2dЁ%2dЁ%d,%d  SER.HOOK  m)%2dЁ%2dЁ%2dЁ%2d (meandr)",
//////		"n)%d,%d %2d %2d %d,%d  SER.HOOK  m)%2d %2d %2d %2d (meandr)",
//////		"  %d,%d %2d %2d %d,%d  SER.HOOK    %2d %2d %2d %2d (meandrs)",
		"  %d,%d %2d %2d %d,%d SER.HOOK %2d %2d %2d %2d (meandrs)",
		l->num_long_flags, l->num_flags, l->num_concs,
		r->num_concs, r->num_flags, r->num_long_flags,
		l->m_meandr, l->c_meandr, r->c_meandr, r->m_meandr);
		/************************	// 07.01.1994 OTLADKA:
		pbuf+=sprintf(pbuf,"  OTL: FULL=%d LMF=%d RMF=%d",
			s->full_width, l->max_flag, r->max_flag);
			*****************************************/
						// 10.01.1994 OTLADKA:
		cut_by_pos (c, 't', 254, 1, 1);	// LEMAN's FUNCTION
//		pbuf+=sprintf(pbuf," pen_up t=%d,", pen_up);
		cut_by_pos (c, 'r', 254, 1, 1);	// LEMAN's FUNCTION
//		pbuf+=sprintf(pbuf," r=%d,", pen_up);
	if (c!=NULL)  {		// 17.01.1994 FOR OTLADKA
cell	*cell_LEFT = c->prevl;
	    if (cell_LEFT!=NULL  &&  cell_LEFT->nvers!=0)
		pbuf+=sprintf(pbuf,"  LEFT %c(%c)",
				cell_LEFT->vers[0].let, left_letter_EEM);
	    }
/*......................................................................*/
	pbuf += sprintf(pbuf,"\nDiskrim       :");

  for(i=0;i<c->nvers;i++)
    {
    int16_t   dis ,  dis_abs;
    uchar  let  = c->vers[i].let;
    if( let=='|' && (c->pos_inc&erect_rot) && c->stick_inc>256 )  let='i';
    dis = discrim_stick(let,l,r,s,c_f);
    dis_abs = abs (dis);
		pbuf+=sprintf(pbuf," %c%c%d",		// 10.01.1994
			let_sans_acc [c->vers[i].let],
			(dis>=0) ? '-' : '+',		// NORMAL or BONUS;
			dis_abs);
		}
/*......................................................................*/
#ifdef	MKPRINT_ENABLE
	pbuf+=sprintf(pbuf,"     MK: %04X %04X %04X %04X %04X",
				     mkma,mkmb,mkmc,mkmd,mkme);

//////	pbuf+=sprintf(pbuf,"   MK: %d, %d, %d,%d, %d,%d, %d,%d, %d,%d, %d",
//////				    d, d0, d1,d2, d3,d4, d5,d6, d7,d8, d9);

//////	pbuf+=sprintf(pbuf," %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
//////			      d,d0,d1,d2,d3,d4,d5,d6,d7,d8,d9);
/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .	*/
				// MK SPEC PRINTS with AUTOINCREMENT d++
/***************************
	pbuf+=sprintf(pbuf,
		"  MK-%d-770%d:  %d  %d,%d  %d,%d  %d,%d  %d,%d  %d,%d",
		       d,  d&7,  d0, d1,d2, d3,d4, d5,d6, d7,d8, d9,d10);
	d++;	//////////// NOTA BENE: AUTOUNCREMENT d++ ////////////////
	*******************************************************************/
#endif
/*......................................................................*/

	snap_show_text (buf);        /* show stick-attribute */

	if ( typ_snap )
		snap_monitor ();

	if ( inc_v )  {
		if ( !typ_snap )  {
			snap_monitor ();
			snap_show_text ("Please, press ENTER");
			}
		c->env = buf_cell;	/* restore old c_comp	*/
		c->w   = buf_w;
		}
	}

return;
}
/*----------------------------------------------------------------------*/
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */
/* ╟╟╟           RASTER OPERATIONS FOR SNAP-DEBUG               ╟╟╟  */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟  */


/*----------------------------------------------------------------------*/
/* position of first bit in string */
static int16_t off_shift_string ( uchar string[], int16_t len )  {
uchar i,f,k;

for (i=0; i<len&&string[i]==0x00; i++);		/* skip  zero bytes */
f = string [i];
k = i<<3;
for (i=0x80; i; i>>=1,k++)	/* find first bit in nonzero byte */
	if ( f&i )
		break;
return (k);
}
/*----------------------------------------------------------------------*/
/* MINIMAL number of first bit in any row of raster  */
/* this (not optimal !) function used in SNAP only   */
static int16_t dest_to_comp (uchar raster[], int16_t hei, int16_t wid)  {
int16_t	i, minim, d, bwid=bytlen(wid);
uchar	*p ;
for (minim=wid,p=&raster[0],i=0; i<hei; i++,p+=bwid)  {
	d = off_shift_string ( p, bwid );	/* p - pointer to curret row */
	if ( d<minim )
		minim = d;
	}
return ( minim );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// shift any row of raster[dy][(dx+7)>>3] to tab_angle[i]
// max_shift - max offset of string
// result : raster *res, return : new width of raster
//  AK add stack crash protection : Oleg : up size of local buffer
int16_t shift_raster (uchar *raster, int16_t dy, int16_t dx, int16_t tab_angle[],
      int16_t max_shift, uchar *res, int16_t dir)
{
	int16_t i, ii, j, Dx, d, dd, c;
	uchar  *r, *rr, s1, s2;

	d  = bytlen (dx);
	Dx = dx+max_shift;
	dd = bytlen (Dx);

	if( dir==0 )
	{
		memcpy(res, raster, dd*dy);
		return (dx);
	}

	memset (res, 0, dd*dy);

	if( dir<0 )
		max_shift=0;

	for (r=raster,ii=i=0; i < dy ; i++,r+=d, ii+=dd)  //AK! add for stack crash protection
	    {
		c = max_shift-tab_angle[i];
        rr = res + ii + (c>>3) ;
		s1 = c&7;
		s2 = 8 - s1;
		rr[0]=(r[0]>>s1);      // first
		for( j=1; j<d; j++)
			rr[j] = (r[j-1]<<s2) | (r[j]>>s1);

        if( dd>d )
            rr[j]=r[j-1]<<s2;
	    }

return (Dx);
}
/*----------------------------------------------------------------------*/
