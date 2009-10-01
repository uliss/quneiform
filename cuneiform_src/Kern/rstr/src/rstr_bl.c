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


#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "struct.h"
#include "cstr.h"
#include "func.h"

#include "linear.h"

#ifdef _BAL_VERSION_   // Nick 10.02.2001

#include "bal.h"

extern uchar fax1x2;
extern uchar language;
extern uchar multy_language;

extern uint16_t actual_resolution;
extern Bool   line_BL;
extern uchar  spec_camera;
extern uchar  no_linpen;
extern int16_t  prop_l_delta, prop_r_delta;
extern uchar  db_pass;

extern uchar *letters_pidx_table;  // 512
extern uchar decode_ASCII_to_[256][4];


int16_t     minrow,mincol, maxrow, maxcol;
int16_t     bs_got=0;     // in baton.c - int32_t - error!?

int16_t			  bbs0,bbs1, bbs2, bbs3, bbs4, bbsm;  // bbst,
int16_t           bsdust_upper, bsdust_lower, bsdust_ps;
int16_t           Nb1, Nb2, Nb3, Nb4, Nbt, Ps, Psf;
uchar          multi_bas;

int16_t           Ns1=0,Ns2=0;        // in Iot.c   only  !!!
//int16_t           pen_up;         // for otladka 't' in St_tools.c

uchar          let_to_dust=0;

char          all_caps=0;
char          all_diffs_made=0;
CSTR_line     lin_str=(CSTR_line)NULL;     // pass3.c only

char          dust_in = 0;

static BAL_bas_ln all_bases[32];

// Linpon.c
int16_t sMALL_SIZE=SMALL_SIZE;

int16_t obtain_diff(int16_t arg);

/////////////////////

int16_t get_bsm()
 { return (bbsm+minrow); }
/////////
int16_t def_locbas(cell *cl)
{
 BAL_bas_ln *bc, *bn, *bsp;
 int16_t i, x, y, dc, dn;
 uchar fl_defb;

 if ((multi_bas & 128) == 0)
   goto retps;  // multiple bases not set

 if (cl==NULL)
 {
   bc=all_bases;
   goto make;
 }
 x=cl->col;
 y=x+cl->w/2;
 for (i=0, bc=all_bases, bn=bc+1; i<32; i++, bc++,bn++)
 {
  if (y <= bc->endcol)  break;
  if (bn->endcol==32000) break;
  if (x >= bn->startcol) continue;
  if ((dc=x-bc->endcol)<=0) break;
  if ((dn=bn->startcol-y)<=0) { bc=bn; break; }
  if (dc<=dn) break;
  bc=bn;
  break;
 }
make:
 bsp=bc;
 Ps   = bsp->ps;
 bbs1 = bsp->b1;
 bbs2 = bsp->b2;
 bbs3 = bsp->b3;
 bbs4 = bsp->b4;
 Ps = bbs3-bbs2;    //?????????????? 940119 ????
 if (Ps <= 0)
 {
   Ps = 1;
   bbs2 = bbs3-1;
 }
 bbsm = (bbs2+bbs3)/2;
 fl_defb = bsp->fl_def;
 Nb1 = (fl_defb & c_db_b1)? bsp->n1 : -1;
 Nb2 = (fl_defb & c_db_b2)? bsp->n2 : -1;
 Nb3 = (fl_defb & c_db_b3)? bsp->n3 : -1;
 Nb4 = (fl_defb & c_db_b4)? bsp->n4 : -1;
 if (Nb1==0) Nb1=-1;
 if (Nb2==0) Nb2=-1;
 if (Nb3==0) Nb3=-1;
 if (Nb4==0) Nb4=-1;
 all_caps = fl_defb & c_fdef_cap;

retps:
 Psf = Ps;
 if (fax1x2) Psf += 3;  // AL 940329 was +2 TWICE ??!!

 return Ps;
}
//////////
void get_b_lines(cell *C, B_LINES *bas)
 {
 int16_t dif;

 if (C)
 {
   def_locbas(C);
   if (all_diffs_made)    // are all diffs already defined ?
   {
     dif = C->bdiff;
     if (C->difflg & (c_db_down + c_db_up))
	 {
//       if (C->difflg & c_db_up)   dif --;     // restore proper
//       else          dif ++;     // base diff without corrections
//   // Nick - 16.03.2001 - accord to  set c_db_up,down !
       if (C->difflg & c_db_up)   dif ++;     // restore proper
       else          dif --;     // base diff without corrections
	 }
   }
   else      // diff not defined yet
     dif = obtain_diff(C->col);
   dif += minrow;
 }
 else
 {
   def_locbas(NULL);
   dif=minrow;
 }

 if (Ps <= 0)
 {
   Ps = 1;
   bbs2 = bbs3-1;
 }
 if (bbs2 >= bbs3)
   bbs2 = bbs3-1;
 if (bbs1 >= bbs2)
   bbs1 = bbs2-1;
 if (bbs4 <= bbs3)
   bbs4=bbs3+1;
 bas->ps = bbs3-bbs2; //////////?????????? 940119 ???
 bas->b0=bbs0+minrow;
 bas->b1=bbs1+dif;
 bas->b2=bbs2+dif;
 bas->b3=bbs3+dif;
 bas->b4=bbs4+dif;
 bas->bm=bbsm+dif;
 bas->n1=Nb1;
 bas->n2=Nb2;
 bas->n3=Nb3;
 bas->n4=Nb4;
 }
///////////////////////////
int32_t get_size()                                                   //16.01.97
 { if (multi_bas & 128)
      def_locbas(NULL);
   return (int32_t) Ps;                                              //16.01.97
 }
///////////////////////
//
// from linutil.c
#include "ligas.h"      // Pit 10-10-94 03:56pm

/*============= Func prototypes ==================*/

uchar to_lower(uchar c);
uchar to_upper(uchar c);
int16_t  is_lower(uchar ch);
int16_t  is_upper(uchar ch);
int16_t  isletter(uchar ch);
uchar get_homot(uchar ch);
int16_t  draft_cut_hyps(int16_t bs,int16_t fl);

/*============= Source code ============*/
Bool is_liga_ff(uchar c)
{
// Код лиги определен через макру в ligas.h 05.09.2000 E.P.
  return (c==liga_ff);
}
Bool is_liga_ffl(uchar c)
{
// Код лиги определен через макру в ligas.h 05.09.2000 E.P.
  return (c==liga_ffl);
}

int16_t is_russian(uchar ch)
{
if( language==LANG_RUSSIAN || language==LANG_ENGLISH && multy_language )
switch(fEdCode){
  case ED_ASCII: // for ASCII
   if((ch >=(uchar)' ' && ch <=(uchar)'Ї') ||
      (ch >=(uchar)'а' && ch <=(uchar)'п') ||
      (ch >=(uchar)'Ђ' && ch <=(uchar)'џ') ||
       memchr("рсхчшэИА",ch,8)
     ) return 1;
  break;
  case ED_WIN: // for Windows (ANSI)
      if( (ch >=0xE0 && ch <=0xFF)||(ch >=(uchar)'Ђ' && ch <=(uchar)'џ'))
        return 1;
  break;
  case ED_MAC: // for Macintosh
      if(
       (ch >=0xE0 && ch <=0xFE) || ch == 0xDF || (ch >= 0xC0 && ch <= 0xDF)
        ) return 1;
  break;
        }
return 0;
}

int16_t is_english(uchar ch)
{
return (ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||
       (
//	   ch>=ligas_beg && ch<=ligas_end &&
	   is_liga(ch) &&  // 14.09.2000 E.P.

	   ch!=liga_exm && ch!=liga_qm )
	   ;
}

int16_t is_serbian_special(uchar ch)
{
return (ch == SERB_j  ||   ch == SERB_J  ||
        ch == SERB_n  ||   ch == SERB_N  ||
        ch == SERB_l  ||   ch == SERB_L  ||
        ch == SERB_h  ||   ch == SERB_H  ||
        ch == SERB_hh ||   ch == SERB_HH ||
        ch == SERB_u  ||   ch == SERB_U  );

}

int16_t is_polish_special(uchar ch)
{
return (ch == POLISH_SS  ||   ch == POLISH_s  ||
        ch == POLISH_ZZD ||   ch == POLISH_zd ||
        ch == POLISH_ZZR ||   ch == POLISH_zr ||
        ch == POLISH_LL  ||   ch == POLISH_l  ||
        ch == POLISH_AA  ||   ch == POLISH_a  ||
        ch == POLISH_CC  ||   ch == POLISH_a  ||
        ch == POLISH_NN  ||   ch == POLISH_n  ||
        ch == POLISH_EE  ||   ch == POLISH_e  ||
        ch == POLISH_OO  ||   ch == POLISH_o
    );
}

int16_t is_czech_special(uchar let)
{
return (
     let == AA_right_accent || let == a_right_accent	||
     let == CC_inv_roof		|| let == c_inv_roof		||
     let == DD_inv_roof		|| let == d_inv_roof		||
     let == EE_right_accent || let == e_right_accent	||
     let == EE_inv_roof		|| let == e_inv_roof		||
     let == II_right_accent || let == i_right_accent	||
     let == NN_inv_roof		|| let == n_inv_roof		||
     let == OO_right_accent || let == o_right_accent	||
     let == RR_inv_roof		|| let == r_inv_roof		||
     let == SS_inv_roof_latin	|| let == s_inv_roof_latin	||
     let == TT_inv_roof		|| let == t_inv_roof		||
     let == UU_right_accent || let == u_right_accent	||
     let == UU_circle_accent|| let == u_circle_accent	||
     let == YY_right_accent || let == y_right_accent	||
     let == ZZ_inv_roof_latin		|| let == z_inv_roof_latin
    );
}

int16_t is_roman_special(uchar let)
{
return (
     let == AA_semicircle			|| 	let == a_semicircle				||
     let == AA_roof_accent			|| 	let == a_roof_accent			||
     let == II_roof_accent			|| 	let == i_roof_accent			||
     let == SS_bottom_accent_latin	|| 	let == s_bottom_accent_latin	||
     let == TT_bottom_accent		|| 	let == t_bottom_accent
    );
}

int16_t is_hungar_special(uchar let)
{
return (
     let == AA_right_accent || 		let == a_right_accent ||
     let == EE_right_accent || 		let == e_right_accent ||
     let == II_right_accent || 		let == i_right_accent ||
     let == OO_right_accent || 		let == o_right_accent ||
     let == OO_2dot_accent  || 		let == o_2dot_accent  ||
     let == OO_double_right || 		let == o_double_right
    );
}
int16_t is_slovenian_special(uchar let)
{
return (
     let == CC_inv_roof			|| let == c_inv_roof		||
     let == SS_inv_roof_latin	|| let == s_inv_roof_latin	||
     let == ZZ_inv_roof_latin	|| let == z_inv_roof_latin
    );
}

int16_t isnot_slovenian(uchar let)
{
return (
     let == 'Q' || let == 'q' ||
     let == 'W' || let == 'w' ||
     let == 'X' || let == 'x' ||
     let == 'Y' || let == 'y'
    );
}

int16_t is_baltic_palka(uchar c)
{
// Определение балтийских палок. 30.05.2002 E.P.

	if (!is_baltic_language(language))
		return 0;

	return (
			c==II_macron		|| c==i_macron ||
			c==II_bottom_accent || c==i_bottom_accent ||
			c==LL_bottom_accent || c==l_bottom_accent ||
			0
			);
}

int16_t is_latvian_special(uchar let)
{
return (
     let == AA_macron			||let == a_macron			||
     let == CC_inv_roof			||let == c_inv_roof			||
     let == EE_macron			||let == e_macron			||
     let == GG_bottom_accent	||let == LATVIAN_g			||
     let == II_macron			||let == i_macron			||
     let == KK_bottom_accent	||let == k_bottom_accent	||
     let == LL_bottom_accent	||let == l_bottom_accent	||
     let == NN_bottom_accent	||let == n_bottom_accent	||
     let == SS_inv_roof_baltic	||let == s_inv_roof_baltic	||
     let == UU_macron			||let == u_macron			||
     let == ZZ_inv_roof_baltic	||let == z_inv_roof_baltic	||
	 0
    );
}

int16_t isnot_latvian(uchar let)
{
return (
     let == 'Q' || let == 'q' ||
     let == 'W' || let == 'w' ||
     let == 'X' || let == 'x' ||
     let == 'Y' || let == 'y'
    );
}

int16_t is_lithuanian_special(uchar let)
{
return (
     let == AA_bottom_accent	|| 	let == a_bottom_accent	||
     let == CC_inv_roof			|| 	let == c_inv_roof		||
     let == EE_bottom_accent	|| 	let == e_bottom_accent	||
     let == EE_dot_accent		|| 	let == e_dot_accent		||
     let == II_bottom_accent	|| 	let == i_bottom_accent	||
     let == SS_inv_roof_baltic	|| 	let == s_inv_roof_baltic||
     let == UU_bottom_accent	|| 	let == u_bottom_accent	||
     let == UU_macron			|| 	let == u_macron			||
     let == ZZ_inv_roof_baltic	|| 	let == z_inv_roof_baltic||
	 0
    );
}

int16_t isnot_lithuanian(uchar let)
{
return (
     let == 'Q' || let == 'q' ||
     let == 'W' || let == 'w' ||
     let == 'X' || let == 'x'
    );
}

int16_t is_estonian_special(uchar let)
{
return (
     let == AA_2dot_accent		|| 	let == a_2dot_accent		||
     let == OO_tild_accent		|| 	let == o_tild_accent		||
     let == OO_2dot_accent		|| 	let == o_2dot_accent		||
     let == SS_inv_roof_baltic	|| 	let == s_inv_roof_baltic	||
     let == UU_2dot_accent		|| 	let == u_2dot_accent		||
     let == ZZ_inv_roof_baltic	|| 	let == z_inv_roof_baltic	||
	 0
    );
}

int16_t isnot_estonian(uchar let)
{
return (
     let == 'C' || let == 'c' ||
     let == 'Q' || let == 'q' ||
     let == 'W' || let == 'w' ||
     let == 'X' || let == 'x' ||
     let == 'Y' || let == 'y'
    );
}

int16_t is_turkish_special(uchar let)
{
return (
     let == AA_roof_accent			|| 	let == a_roof_accent			||
     let == CC_bottom_accent		|| 	let == c_bottom_accent			||
     let == GG_semicircle			|| 	let == g_semicircle				||
     let == II_dot_accent			|| 	let == i_sans_accent			||
     let == II_roof_accent			|| 	let == i_roof_accent			||
     let == OO_2dot_accent			|| 	let == o_2dot_accent			||
     let == SS_bottom_accent_turkish|| 	let == s_bottom_accent_turkish	||
     let == UU_2dot_accent			|| 	let == u_2dot_accent			||
     let == UU_roof_accent			|| 	let == u_roof_accent			||
	 0
    );
}

int16_t is_turkish_bottom_accent(uchar c)
{
// Определение нижнего акцента. 20.05.2002 E.P.
	return (

	c == CC_bottom_accent || c == c_bottom_accent ||
	c == SS_bottom_accent || c == s_bottom_accent ||

	0);
}

int16_t is_turkish_palka(uchar c)
{
// Определение турецких палок. 21.05.2002 E.P.

	if (!is_turkish_language(language))
		return 0;

	return (
			c == II_roof_accent	|| c == i_roof_accent ||
			c == II_dot_accent	|| c == i_sans_accent ||
			0
			);
}

int16_t is_russian_turkish_conflict(uchar c)
{
/*
	Определение конфликта между русскими
	и турецкими буквами. 21.05.2002 E.P.

	Список конфликтных русских букв:

		- ХЩйо - выглядят как •, ™, ©, ®
		- украинские UKR_ii, UKR_G, UKR_g - выглядят как
			русские буквы В,З,Ц
		- тчю - выглядят как русские буквы в, з, о
		- r_cu_a, r_cu_d  - выглядят как русские буквы р,э

	Полный список конфликтных букв выглядит так:
			"•™©®ВЗЦвзорэ"
*/
	if (!is_turkish_language(language))
		return FALSE;

	if (
		c == liga_bull_turkish	|| // '•'
		c == liga_TM_turkish	|| // '™'
		c == liga_CC_turkish	|| // '©'
		c == liga_CR_turkish	|| // '®'
		c == AA_roof_accent		|| // 'В'
		c == a_roof_accent		|| // 'в'
		c == CC_bottom_accent	|| // 'З'
		c == c_bottom_accent	|| // 'з'
		c == OO_2dot_accent		|| // 'Ц'
		c == i_roof_accent		|| // 'о'
		c == r_cu_a				|| // 'р'
		c == r_cu_d				|| // 'э'
		0
	   )
		return TRUE;

	return FALSE;
}

int16_t isnot_turkish(uchar let)
{
extern  char    alphabet[256];

return (
	 !alphabet[let] || // Отсекаю французские акценты. 09.06.2002 E.P.
     let == 'Q' || let == 'q' ||
     let == 'W' || let == 'w' ||
     let == 'X' || let == 'x'
    );
}

///////////////////////////////////////
int16_t is_lower(uchar ch)
{

if(language==LANG_RUSSIAN)
        switch(fEdCode){
  case ED_ASCII: // for ASCII
   if((ch >=(uchar)' ' && ch <=(uchar)'Ї') ||
      (ch >=(uchar)'а' && ch <=(uchar)'п') ||
       memchr("рсхчшэА",ch,7)
     ) return 1;
  break;
  case ED_WIN: // for Windows (ANSI)
      if (ch >=0xE0 && ch <=0xFF)
        return 1;
  break;
  case ED_MAC: // for Macintosh
      if(((ch >=0xE0 && ch <=0xFE) || ch == 0xDF)
        ) return 1;
  break;
        }
 if(ch >= 'a' && ch <= 'z') return 1;
 return 0;
}
int16_t is_upper(uchar ch)
{
if(language==LANG_RUSSIAN)
    switch(fEdCode){
    case ED_ASCII:
    case ED_MAC: // for ASCII and Macintosh
      if(ch >=(uchar)'Ђ' && ch <=(uchar)'џ'||
         ch==(uchar)r_EE_2dot
        )  return 1;
    break;
    case ED_WIN: // for Windows (ANSI)
      if(ch >= 0xC0 && ch <= 0xDF)
        return 1;
    break;
    }
if(ch >= 'A' && ch <= 'Z') return 1;
 return 0;
}

Bool is_digit(uchar ch)
{
if(ch >= (uchar)'0' && ch <= (uchar)'9') return TRUE;
else return FALSE;
}

int16_t isletter(uchar ch)
{
if(is_lower(ch) || is_upper(ch)) return 1;
else return 0;
}

static const uchar non_twin[]=" ЂЎЃҐ…";
static const uchar lat_twins[]="cCoOpPsSvVwWxXzZ";

int16_t twin(uchar ch)
{
if(!isletter(ch)) return 0;
if( language==LANG_RUSSIAN )
 if(memchr(non_twin,ch,sizeof non_twin)) return 0;
 else                                    return 1;
if( language!=LANG_RUSSIAN && memchr(lat_twins,ch,sizeof lat_twins)) return 1;
return 0;
}

uchar get_homot(uchar ch )
{
 if(ch == '0') return ((uchar)'®');
 if(is_upper(ch)) return to_lower(ch);
 if(is_lower(ch)) return to_upper(ch);
 return ch;
}

/* Function returns UPPER CASE variant of the letter.             */
uchar to_upper( uchar c )
{
  if ( c >= (uchar)'a' && c <= (uchar)'z') return c - (uchar)'a' + (uchar)'A';
  if(language==LANG_RUSSIAN)
       switch(fEdCode){
  case ED_ASCII:  // for ASCII
    if ( c >= (uchar)' ' && c <= (uchar)'Ї') return c - (uchar)' ' + (uchar)'Ђ';
    if ( c >= (uchar)'а' && c <= (uchar)'п') return c - (uchar)'а' + (uchar)'ђ';
  break;
  case ED_WIN: // for Windows (ANSI)
    if ( c >= 0xE0 && c <= 0xFF ) return c - 0xE0 + 0xC0;
  break;
  case ED_MAC: // for Macintosh
    if ((c >= 0xE0 && c <= 0xFE) ) return c - 0xE0 + 0x80;
    if ( c == 0xDF )               return 0x9F;
  break;
  }
    return c;
}

 /* Function returns LOWER CASE variant of the letter.             */
uchar to_lower(uchar c)
{
  if ( c >= (uchar)'A' && c <= (uchar)'Z') return c - (uchar)'A'+ (uchar)'a' ;
  if(language==LANG_RUSSIAN)
        switch(fEdCode){
  case ED_ASCII: // for ASCII
    if ( c >= (uchar)'Ђ' && c <= (uchar)'Џ') return c - (uchar)'Ђ'+ (uchar)' ' ;
    if ( c >= (uchar)'ђ' && c <= (uchar)'џ') return c - (uchar)'ђ'+ (uchar)'а' ;
  break;
  case ED_WIN: // for Windows
    if ( c >= 0xC0 && c <= 0xDF) return c - 0xC0 + 0xE0 ;
  break;
  case ED_MAC: // for Macintosh
    if ( c >= 0x80 && c <= 0x9E) return c - 0x80 + 0xE0 ;
    if ( c == 0x9F )             return 0xDF;
  break;
        }
    return c;
}
/////////////////////
int16_t is_cen_bottom_accent(uchar c)
{
// Определение нижнего акцента 12.09.2000 E.P.
	return (

	c== AA_bottom_accent		|| c== a_bottom_accent	||
	c== CC_bottom_accent		|| c== c_bottom_accent	||
	c== EE_bottom_accent		|| c== e_bottom_accent	||
	c== SS_bottom_accent_latin || c== s_bottom_accent	||
	c== TT_bottom_accent		|| c== t_bottom_accent	||

	0);
}
/////////////////////
int16_t is_baltic_bottom_accent(uchar c)
{
// Определение нижнего акцента 10.07.2001 E.P.
	return (

	c == AA_bottom_accent_baltic || c == a_bottom_accent_baltic ||
	c == EE_bottom_accent_baltic || c == e_bottom_accent_baltic ||
	c == GG_bottom_accent		 ||
	c == II_bottom_accent		 || c == i_bottom_accent ||
	c == KK_bottom_accent		 || c == k_bottom_accent ||
	c == LL_bottom_accent		 || c == l_bottom_accent ||
	c == NN_bottom_accent		 || c == n_bottom_accent ||
	c == UU_bottom_accent		 || c == u_bottom_accent ||
	0);
}

int16_t is_russian_baltic_conflict(uchar c)
{
// Определение конфликта между русскими и балтийскими буквами. 17.07.2001 E.P.
	if (!is_baltic_language(language))
		return FALSE;

	if (strchr("абвджзилмноп",c))
		return TRUE;

	return FALSE;
}
/////////////////////////////////
// Linpon.c
int16_t is_defis(cell *C)
 {   int16_t bs2m, bsm3;
 int16_t i, j;
 if ((C->w < 3) || (C->h < 2) || (2*C->h > C->w) || (9 * (C->h) > 4 * Ps))
  return 0;
 i=C->row-minrow+C->bdiff; j=i+C->h;
 bs2m=(bbs2+bbsm)/2;
 bsm3=(bbsm+bbs3)/2;
 if ((i < bs2m) || (j > bsm3)) return 0;
 return 1;
 }
//////////////////////
// Linpon.c
int16_t if_dust(cell *C)
 { int16_t rb, w1, wh, wps;
 // Check position in the string - have to intersect with string middle
 // MUST request local BASELINES !!!!!!!!!!!!!!
 rb = 0;
 wps = def_locbas(C);
 /////////  931230 if (5*C->h > 3*wps) rb |= 16;    // indicate LARGE
 w1=(C->row - minrow)-bbsm-C->bdiff;
 if (w1 >= 0)
  { rb |= 1; goto chks; }
 w1=w1+1+C->h+((fax1x2)?2:0);
 if (w1 < 0)
  rb |= 2;
			       // Check height of cell:
chks:              // height(cell) + 4 >= main_base-small_base
 wh=C->h+((fax1x2)?2:0);
 if ((wh <= sMALL_SIZE) || (5*wh < 3*wps))  //NB 3.4.94
  return (rb | 4);
 if (4*wh < 3*wps)
  return (rb | 8);
 return 0;
 }
/////////////
//////////////////////
static void SetRstrGlobals(BAL_RSTR_GLOBALS  *rstrGlob)
{
int16_t rast_is_BOX_solid (CSTR_rast B1);

	rstrGlob->language = language;
	rstrGlob->line_number = line_number;
	rstrGlob->fax1x2 =fax1x2;
	rstrGlob->actual_resolution = actual_resolution;
    rstrGlob->fEdCode   = fEdCode ; // ASCII
    rstrGlob->line_BL   = line_BL;
    rstrGlob->spec_camera = spec_camera;
    rstrGlob->no_linpen   = no_linpen;

    rstrGlob->prop_l_delta  = prop_l_delta;
	rstrGlob->prop_r_delta  = prop_r_delta;

	rstrGlob->let_lindef3 = let_lindef3;
    rstrGlob->let_lindef = let_lindef;
    rstrGlob->let_linpos = let_linpos ;
    rstrGlob->let_linshape = let_linshape ;
    rstrGlob->let_sans_acc =  let_sans_acc;
    rstrGlob->letters_pidx_table =    letters_pidx_table ;
	rstrGlob->decode_ASCII_to_ = &decode_ASCII_to_[0][0];

	rstrGlob->skew_corr = skew_corr;
	rstrGlob->rast_is_BOX_solid = rast_is_BOX_solid;

	rstrGlob->db_status = db_status ;
	rstrGlob->db_pass   = db_pass ;
	rstrGlob->snap_activity = snap_activity;
	rstrGlob->snap_monitor  = snap_monitor;
	rstrGlob->snap_show_text= snap_show_text;
	rstrGlob->snap_monitor_ori= snap_monitor_ori;//IGOR
	rstrGlob->snap_is_marked= snap_is_marked;//IGOR
	rstrGlob->snap_baselines= snap_baselines;//IGOR
	rstrGlob->snap_draw_line= snap_draw_line;//IGOR
	rstrGlob->snap_del_line = snap_del_line;//IGOR
}
//////////////////////
static void GetBalGlobals(BAL_INOUT_GLOBALS  *balGlob)
{
	all_caps       = balGlob->all_caps;
    all_diffs_made = balGlob->all_diffs_made;
	bbs0 = balGlob->bbs0;
	bbs1 = balGlob->bbs1;
	bbs2 = balGlob->bbs2;
	bbs3 = balGlob->bbs3;
	bbs4 = balGlob->bbs4;
	bbsm = balGlob->bbsm;

	bs_got       = balGlob->bs_got      ;
	bsdust_lower = balGlob->bsdust_lower;
	bsdust_ps    = balGlob->bsdust_ps   ;
	bsdust_upper = balGlob->bsdust_upper;
	let_to_dust  = balGlob->let_to_dust ;
	maxcol = balGlob->maxcol ;
	mincol = balGlob->mincol ;
	maxrow = balGlob->maxrow ;
	minrow = balGlob->minrow ;
	multi_bas = balGlob->multi_bas;

	Nb1 = balGlob->Nb1 ;
	Nb2 = balGlob->Nb2 ;
	Nb3 = balGlob->Nb3 ;
	Nb4 = balGlob->Nb4 ;

	Ns1 = balGlob->Ns1 ;
	Ns2 = balGlob->Ns2 ;
	Ps  = balGlob->Ps  ;
	Psf = balGlob->Psf ;

	nIncline = (int16_t)balGlob->nIncline;
}
/////////////////////
static void SetBalGlobals(BAL_INOUT_GLOBALS  *balGlob)
{
	balGlob->all_caps       = all_caps;
    balGlob->all_diffs_made = all_diffs_made;
	balGlob->bbs0 = bbs0;
	balGlob->bbs1 = bbs1;
	balGlob->bbs2 = bbs2;
	balGlob->bbs3 = bbs3;
	balGlob->bbs4 = bbs4;
	balGlob->bbsm = bbsm;

	balGlob->bs_got       = bs_got;
	balGlob->bsdust_lower = bsdust_lower;
	balGlob->bsdust_ps    = bsdust_ps;
	balGlob->bsdust_upper = bsdust_upper;
	balGlob->let_to_dust  = let_to_dust;
	balGlob->maxcol = maxcol;
	balGlob->mincol = mincol;
	balGlob->maxrow = maxrow;
	balGlob->minrow = minrow;
	balGlob->multi_bas = multi_bas;

	balGlob->Nb1 = Nb1;
	balGlob->Nb2 = Nb2;
	balGlob->Nb3 = Nb3;
	balGlob->Nb4 = Nb4;

	balGlob->Ns1 = Ns1;
	balGlob->Ns2 = Ns2;
	balGlob->Ps  = Ps;
	balGlob->Psf = Psf;

	balGlob->nIncline = nIncline;
}
//////////////////////
static CSTR_head *llRaw=(CSTR_head *)NULL;
static uint16_t SetNoScaleLine(CSTR_line ln )
{
	CSTR_attr lattr;
	CSTR_rast rast;
	CSTR_rast_attr attr;
	CCOM_comp *cmp;
	uint16_t scale;

    llRaw = (CSTR_head *)ln;

	CSTR_GetLineAttr(ln,&lattr);
	if( !(lattr.Flags & CSTR_STR_SCALED) ||
		!lattr.scale
	  )
		return 0;

	for(rast=CSTR_GetNext(CSTR_GetFirstRaster(ln));rast;
	    rast = CSTR_GetNext(rast) )
	{
			CSTR_GetAttr(rast,&attr);

			attr.h>>=lattr.scale;
			attr.w>>=lattr.scale;
			attr.row>>=lattr.scale;
			attr.col>>=lattr.scale;
			attr.r_row>>=lattr.scale;
			attr.r_col>>=lattr.scale;

			CSTR_SetAttr(rast,&attr);

			cmp = CSTR_GetComp(rast);

			if( cmp )
			{
				cmp->w >>= lattr.scale;
				cmp->h >>= lattr.scale;
				cmp->upper >>= lattr.scale;
				cmp->left  >>= lattr.scale;
			}

	}

    scale = lattr.scale;
	lattr.Flags &= ~CSTR_STR_SCALED;

	lattr.scale = 0;
    CSTR_SetLineAttr(ln,&lattr);

	return scale;
}
///////////////
//////////////////////
static uint16_t RestoreScaleLine(CSTR_line ln , uint16_t scale)
{
	CSTR_attr lattr;
	CSTR_rast rast;
	CSTR_rast_attr attr;
	CCOM_comp *cmp;

	if( !scale )
		return scale;

	CSTR_GetLineAttr(ln,&lattr);
	lattr.Flags |= CSTR_STR_SCALED;
	lattr.scale  = (uchar)scale;
	CSTR_SetLineAttr(ln,&lattr);

	for(rast=CSTR_GetNext(CSTR_GetFirstRaster(ln));rast;
	    rast = CSTR_GetNext(rast) )
	{
			CSTR_GetAttr(rast,&attr);

			attr.h<<=lattr.scale;
			attr.w<<=lattr.scale;
			attr.row<<=lattr.scale;
			attr.col<<=lattr.scale;
			attr.r_row<<=lattr.scale;
			attr.r_col<<=lattr.scale;

			CSTR_SetAttr(rast,&attr);

			cmp = CSTR_GetComp(rast);

			if( cmp )
			{
				cmp->w <<= lattr.scale;
				cmp->h <<= lattr.scale;
				cmp->upper <<= lattr.scale;
				cmp->left  <<= lattr.scale;
			}
	}

	return scale;
}
///////////////
void clear_cells(void);
void Cells2CSTR(CSTR_line lin,CSTR_line lino,cell *cur,Bool32 enable_scaled);
// для CopyAttr2CSTR в Cells2CSTR() !!!
Bool SetFontReady(Bool fready);
int32_t p2_Cstr2Cell( CSTR_line lin,CSTR_rast first,CSTR_rast last,Bool32 needVers, uint32_t disable );
// предварительный поиск базовых линий
void basedraft(CSTR_line ln)
{
 BAL_RSTR_GLOBALS   rstrGlob;
 BAL_INOUT_GLOBALS  balGlob;
 CSTR_line line_bal=(CSTR_line)NULL;
 int  i;
 CSTR_rast rast;
 int  ret;
 uint16_t saveScale;
 Bool32 retBaseDraft;
 CSTR_attr lattr,lattr_bal;

   lin_str = ln;
   CSTR_GetLineAttr(ln,&lattr);

   for( i=0; i < 5;i++)
   {
    line_bal = CSTR_NewLine(line_number, CSTR_LINVERS_BAL + i*10, -1 );
    if( line_bal )
	  break;
   }

    if( !line_bal )
	 return;

	// переведем cell_f()-> в нашу линию, используя
	// атрибуты ln
	{
	  Bool old_ready = SetFontReady(TRUE);
      Cells2CSTR( ln, line_bal, NULL, 1);
	  SetFontReady(old_ready);
	}

    SetRstrGlobals(&rstrGlob);
	SetBalGlobals(&balGlob);

	// установим - как будто не было шкалирования -
	// как в версии с cell()
	saveScale = SetNoScaleLine( line_bal );

	// установим row,hei как у исходной - чтобы было совпадение с
	// старой версией для skew_corr()
	//
	CSTR_GetLineAttr(line_bal,&lattr_bal);
	lattr_bal.row=lattr.row;
	lattr_bal.hei=lattr.hei;
	CSTR_SetLineAttr(line_bal,&lattr_bal);

	dust_in = 0;
	BAL_basedraft(line_bal, 0, &rstrGlob, &balGlob, all_bases);

    //  вернем истину
	if( saveScale )
		 RestoreScaleLine( line_bal, saveScale );

	GetBalGlobals(&balGlob);

	// переведем в cell
    clear_cells();
    ret = p2_Cstr2Cell( line_bal,NULL,NULL,TRUE, CSTR_f_space );

    // добавим мусор
	dust_ini(ln); //bring_dust(0);
	dust_in = 1;

    // очистим нашу линию
	for(rast=CSTR_GetNext(CSTR_GetFirstRaster(line_bal));
	    rast; rast=CSTR_DelRaster(rast) );

    // переведем cell_f()-> в нашу линию, используя
	// атрибуты ln
    {
	  Bool old_ready = SetFontReady(TRUE);
      Cells2CSTR( ln, line_bal, NULL, 1);
	  SetFontReady(old_ready);
	}

	// установим - как будто не было шкалирования -
	// как в версии с cell()
	saveScale = SetNoScaleLine( line_bal );

	SetBalGlobals(&balGlob);

	// установим row,hei как у исходной - чтобы было совпадение с
	// старой версией для skew_corr()
	//
	CSTR_GetLineAttr(line_bal,&lattr_bal);
	lattr_bal.row=lattr.row;
	lattr_bal.hei=lattr.hei;
	CSTR_SetLineAttr(line_bal,&lattr_bal);

	retBaseDraft = BAL_basedraft(line_bal, 1, &rstrGlob, &balGlob, all_bases);
	GetBalGlobals(&balGlob);
    //  вернем истину
	if( saveScale )
		 RestoreScaleLine( line_bal, saveScale );

    // переведем в cell
    clear_cells();
    ret = p2_Cstr2Cell( line_bal,NULL,NULL,TRUE, CSTR_f_space );

	// есть сомнения (и есть дополнительные возможности)
    if( !retBaseDraft )
	{
	// добавим мусор
	 dust_ini(ln); //bring_dust(0);
	 dust_in = 1;

    // очистим нашу линию
	 for(rast=CSTR_GetNext(CSTR_GetFirstRaster(line_bal));
	    rast; rast=CSTR_DelRaster(rast) );

    // переведем cell_f()-> в нашу линию, используя
	// атрибуты ln
     {
	  Bool old_ready = SetFontReady(TRUE);
      Cells2CSTR( ln, line_bal, NULL, 1);
	  SetFontReady(old_ready);
	}

	// установим - как будто не было шкалирования -
	// как в версии с cell()
	 saveScale = SetNoScaleLine( line_bal );

	 // установим row,hei как у исходной - чтобы было совпадение с
	// старой версией для skew_corr()
	//
	 CSTR_GetLineAttr(line_bal,&lattr_bal);
	 lattr_bal.row=lattr.row;
	 lattr_bal.hei=lattr.hei;
	 CSTR_SetLineAttr(line_bal,&lattr_bal);

	 SetBalGlobals(&balGlob);
	 retBaseDraft = BAL_basedraft(line_bal, 2, &rstrGlob, &balGlob, all_bases);
	 GetBalGlobals(&balGlob);
    //  вернем истину
	 if( saveScale )
		 RestoreScaleLine( line_bal, saveScale );

    // переведем в cell
     clear_cells();
     ret = p2_Cstr2Cell( line_bal,NULL,NULL,TRUE, CSTR_f_space );
	}

	CSTR_DeleteLine(line_bal);
}
///////////////
// окончательно уточнить базовые линии
void basefin(CSTR_line ln)
{
 BAL_RSTR_GLOBALS   rstrGlob;
 BAL_INOUT_GLOBALS  balGlob;
 CSTR_line line_bal=(CSTR_line)NULL;
 int  i;
 int  ret;
 uint16_t saveScale;

   lin_str = ln;

   for( i=0; i < 5;i++)
   {
	   line_bal = CSTR_NewLine(line_number, CSTR_LINVERS_BAL + i*10, -1 );
    if( line_bal )
	  break;
   }

    if( !line_bal )
	 return;

	// переведем cell_f()-> в нашу линию, используя
	// атрибуты ln
    {
	  Bool old_ready = SetFontReady(TRUE);
      Cells2CSTR( ln, line_bal, NULL, 1);
	  SetFontReady(old_ready);
	}

    SetRstrGlobals(&rstrGlob);
	SetBalGlobals(&balGlob);

	// установим - как будто не было шкалирования -
	// как в версии с cell()
	saveScale = SetNoScaleLine( line_bal );

	BAL_basefin(line_bal, &rstrGlob, &balGlob, all_bases);
	GetBalGlobals(&balGlob);
	    //  вернем истину
	if( saveScale )
		 RestoreScaleLine( line_bal, saveScale );

	// переведем в cell
    clear_cells();
    ret = p2_Cstr2Cell( line_bal,NULL,NULL,TRUE, 0 );

	CSTR_DeleteLine(line_bal);
}
////////////////////
// уточнить базовые линии, проверить multi-base
// оштрафовать - удалить версии по базовым линиям
void linpos(CSTR_line ln)
{
 BAL_RSTR_GLOBALS   rstrGlob;
 BAL_INOUT_GLOBALS  balGlob;
 CSTR_line line_bal=(CSTR_line)NULL;
 int  i;
 int  ret;
 uint16_t saveScale;

   lin_str = ln;

   for( i=0; i < 5;i++)
   {
    line_bal = CSTR_NewLine(line_number, CSTR_LINVERS_BAL + i*10, -1 );
    if( line_bal )
	  break;
   }

    if( !line_bal )
	 return;

	// переведем cell_f()-> в нашу линию, используя
	// атрибуты ln
    {
	  Bool old_ready = SetFontReady(TRUE);
      Cells2CSTR( ln, line_bal, NULL, 1);
	  SetFontReady(old_ready);
	}

    SetRstrGlobals(&rstrGlob);
	SetBalGlobals(&balGlob);

	// установим - как будто не было шкалирования -
	// как в версии с cell()
	saveScale = SetNoScaleLine( line_bal );

	BAL_linpos(line_bal, &rstrGlob, &balGlob, all_bases);
	GetBalGlobals(&balGlob);
	    //  вернем истину
	if( saveScale )
		 RestoreScaleLine( line_bal, saveScale );

	// переведем в cell
    clear_cells();
    ret = p2_Cstr2Cell( line_bal,NULL,NULL,TRUE, 0 );

	CSTR_DeleteLine(line_bal);
}
////////////////
//////////////////
void BaseLineStatisticInit(void)
{
	BAL_StatInit();
}
/////////////////

#endif
