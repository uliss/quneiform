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

#define CONTOOLS_MODULE
#include "compat_defs.h"
#include "context.h"
#include "linutil.h"
#include "lang.h"


static int16_t LiliI                (cell *);
static int16_t bon_to_dig           (cell *);
static Bool apostr_ll           (cell *);
static void inc_or_dec_slash    (cell *wc, int16_t numv);
extern uchar langUkr;
extern uchar langSer;

int16_t check_numb_2_9 (uchar c)
//
//	This procedure checks symbol to be number.
//	Number means >=2 and <=9 since 0 and 1 could be confused.
//
 {

 if ((c >= '2') && (c <= '9')) return TRUE;
                   return FALSE;
 }

 int16_t check_numb_0_9(uchar c)
//
//	This procedure checks symbol to be number.
//
 {

 if ((c >= '0') && (c <= '9')) return 1;
 if (c == '$')                 return 1;
 if (c == '%')                 return 1;
			       return 0;
 }

 int16_t check_upper(uchar c)
//
//  This procedure checks symbol to be capital letter,
//	that means A-Z.
//
 {

 if (is_upper(c))
        return TRUE;

 if( language == LANG_RUSSIAN && langSer ){
    switch( c )
    {
       case SERB_HH:     return TRUE;
       case SERB_L :     return TRUE;
       case SERB_N :     return TRUE;
       case SERB_H :     return TRUE;
       case SERB_U :     return TRUE;
       case SERB_J :     return TRUE;
    }
 }
 if( language == LANG_CROATIAN ){
    switch( c )
		{
       case CROAT_D :     return TRUE;
       case CROAT_SR:     return TRUE;
       case CROAT_CA:     return TRUE;
       case CROAT_ZR:     return TRUE;
       case CROAT_CR:     return TRUE;
       case CROAT_d :     return FALSE;
       case CROAT_sr:     return FALSE;
       case CROAT_ca:     return FALSE;
       case CROAT_zr:     return FALSE;
       case CROAT_cr:     return FALSE;
		}
	 }
 if( language == LANG_POLISH ){
    switch( c )
    {
       case POLISH_SS  :     return TRUE;
       case POLISH_s   :     return FALSE;
       case POLISH_ZZR :     return TRUE;
       case POLISH_zr  :     return FALSE;
       case POLISH_LL  :     return TRUE;
       case POLISH_l   :     return FALSE;
       case POLISH_AA  :     return TRUE;
       case POLISH_a   :     return FALSE;
       case POLISH_ZZD :     return TRUE;
       case POLISH_zd  :     return FALSE;
       case POLISH_CC  :     return TRUE;
       case POLISH_c   :     return FALSE;
       case POLISH_EE  :     return TRUE;
       case POLISH_e   :     return FALSE;
       case POLISH_NN  :     return TRUE;
       case POLISH_n   :     return FALSE;
       case POLISH_OO  :     return TRUE;
       case POLISH_o   :     return FALSE;
    }
  }

 // 05.09.2000 E.P.
 if (language == LANG_CZECH)
	{
	if (
		c == AA_right_accent  ||
		c == CC_inv_roof	  ||
		c == DD_inv_roof	  ||
		c == EE_right_accent  ||
		c == EE_inv_roof	  ||
		c == II_right_accent  ||
		c == NN_inv_roof	  ||
		c == OO_right_accent  ||
		c == RR_inv_roof	  ||
		c == SS_inv_roof_latin	  ||
		c == TT_inv_roof	  ||
		c == UU_right_accent  ||
		c == UU_circle_accent ||
		c == YY_right_accent  ||
		c == ZZ_inv_roof_latin
	   )
	   return TRUE;

	if (
		c == a_right_accent	||
		c == c_inv_roof		||
		c == d_inv_roof		||
		c == e_right_accent	||
		c == e_inv_roof		||
		c == i_right_accent	||
		c == n_inv_roof		||
		c == o_right_accent	||
		c == r_inv_roof		||
		c == s_inv_roof_latin		||
		c == t_inv_roof		||
		c == u_right_accent	||
		c == u_circle_accent||
		c == y_right_accent	||
		c == z_inv_roof_latin
	   )
	   return FALSE;
	}

 // 05.09.2000 E.P.
 if (language == LANG_ROMAN)
	{
	if (
		c == AA_semicircle			||
		c == AA_roof_accent			||
		c == II_roof_accent			||
		c == SS_bottom_accent_latin ||
		c == TT_bottom_accent
	   )
	   return TRUE;

	if (
		c == a_semicircle			||
		c == a_roof_accent			||
		c == i_roof_accent			||
		c == s_bottom_accent_latin	||
		c == t_bottom_accent
	   )
	   return FALSE;
	}

 // 05.09.2000 E.P.
 if (language == LANG_HUNGAR)
	{
	if (
		c == AA_right_accent ||
		c == EE_right_accent ||
		c == II_right_accent ||
		c == OO_right_accent ||
		c == OO_2dot_accent  ||
		c == OO_double_right
	   )
	   return TRUE;

	if (
		c == a_right_accent ||
		c == e_right_accent ||
		c == i_right_accent ||
		c == o_right_accent ||
		c == o_2dot_accent  ||
		c == o_double_right
	   )
	   return FALSE;
	}

 if (language == LANG_SLOVENIAN)
	{
	if (
		c == CC_inv_roof	  ||
		c == SS_inv_roof_latin	  ||
		c == ZZ_inv_roof_latin
	   )
	   return TRUE;

	if (
		c == c_inv_roof		||
		c == s_inv_roof_latin		||
		c == z_inv_roof_latin
	   )
	   return FALSE;
	}

 if (language == LANG_LATVIAN)
	{
	if (
		c == AA_macron			||
		c == CC_inv_roof		||
		c == EE_macron			||
		c == GG_bottom_accent	||
		c == II_macron			||
		c == KK_bottom_accent	||
		c == LL_bottom_accent	||
		c == NN_bottom_accent	||
		c == SS_inv_roof_baltic	||
		c == UU_macron			||
		c == ZZ_inv_roof_baltic	||
		0
	   )
	   return TRUE;

	if (
		c == a_macron			||
		c == c_inv_roof			||
		c == e_macron			||
		c == LATVIAN_g			||
		c == i_macron			||
		c == k_bottom_accent	||
		c == l_bottom_accent	||
		c == n_bottom_accent	||
		c == s_inv_roof_baltic	||
		c == u_macron			||
		c == z_inv_roof_baltic	||
		0
	   )
	   return FALSE;
	}

 if (language == LANG_LITHUANIAN)
	{
	if (
		c == AA_bottom_accent	||
		c == CC_inv_roof		||
		c == EE_bottom_accent	||
		c == EE_dot_accent		||
		c == II_bottom_accent	||
		c == SS_inv_roof_baltic	||
		c == UU_bottom_accent	||
		c == UU_macron			||
		c == ZZ_inv_roof_baltic	||
		0
	   )
	   return TRUE;

	if (
		c == a_bottom_accent	||
		c == c_inv_roof			||
		c == e_bottom_accent	||
		c == e_dot_accent		||
		c == i_bottom_accent	||
		c == s_inv_roof_baltic	||
		c == u_bottom_accent	||
		c == u_macron			||
		c == z_inv_roof_baltic	||
		0
	   )
	   return FALSE;
	}

 if (language == LANG_ESTONIAN)
	{
	if (
		c == AA_2dot_accent		||
		c == OO_tild_accent		||
		c == OO_2dot_accent		||
		c == SS_inv_roof_baltic	||
		c == UU_2dot_accent		||
		c == ZZ_inv_roof_baltic	||
		0
	   )
	   return TRUE;

	if (
		c == a_2dot_accent		||
		c == o_tild_accent		||
		c == o_2dot_accent		||
		c == s_inv_roof_baltic	||
		c == u_2dot_accent		||
		c == z_inv_roof_baltic	||
		0
	   )
	   return FALSE;
	}

 if (language == LANG_TURKISH)
	{
	if (
		c == AA_roof_accent				||
		c == CC_bottom_accent			||
		c == GG_semicircle				||
		c == II_dot_accent				||
		c == II_roof_accent				||
		c == OO_2dot_accent				||
		c == SS_bottom_accent_turkish	||
		c == UU_2dot_accent				||
		c == UU_roof_accent				||
		0
		)
		return TRUE;

	if (
		c == a_roof_accent				||
		c == c_bottom_accent			||
		c == g_semicircle				||
		c == i_sans_accent				||
		c == i_roof_accent				||
		c == o_2dot_accent				||
		c == s_bottom_accent_turkish	||
		c == u_2dot_accent				||
		c == u_roof_accent				||
		0
	   )
	   return FALSE;
	}

 if ( language != LANG_ENGLISH && language != LANG_RUSSIAN &&
		! is_cen_language(language) &&		// 07.09.2000 E.P.
		! is_baltic_language(language) &&	// 09.07.2001 E.P.
		! is_turkish_language(language)		// 21.05.2002 E.P.
	 )
 {
    switch( c )
    {
       case AA_left_accent    :     return TRUE;
       case AA_right_accent   :     return TRUE;
       case AA_roof_accent    :     return TRUE;
       case AA_2dot_accent    :     return TRUE;
       case AA_circle_accent  :     return TRUE;
       case AE_cap_deaf_sound :     return TRUE;
       case CC_bottom_accent  :     return TRUE;
       case EE_left_accent    :     return TRUE;
       case EE_right_accent   :     return TRUE;
       case EE_roof_accent    :     return TRUE;
       case EE_2dot_accent    :     return TRUE;
       case II_left_accent    :     return TRUE;
       case II_right_accent   :     return TRUE;
       case II_roof_accent    :     return TRUE;
       case II_2dot_accent    :     return TRUE;
       case OO_left_accent    :     return TRUE;
       case OO_right_accent   :     return TRUE;
       case OO_roof_accent    :     return TRUE;
       case OO_2dot_accent    :     return TRUE;
       case UU_left_accent    :     return TRUE;
       case UU_right_accent   :     return TRUE;
       case UU_roof_accent    :     return TRUE;
       case UU_2dot_accent    :     return TRUE;
    }
 }

 if( language==LANG_RUSSIAN && langUkr ){
  switch(c)
    {
    case UKR_E  : return TRUE;
    case UKR_G  : return TRUE;
    case UKR_I  : return TRUE;
    case UKR_II : return TRUE;
    }
 }
        return FALSE;
 }

 int16_t check_lower(uchar c)
//
//  This procedure checks symbol to be lower case letter,
//	that means a-z.
//
 {

 if (is_lower(c)) return TRUE;

 if( language == LANG_RUSSIAN && langSer ){
    switch( c )
    {
       case SERB_hh:     return TRUE;
       case SERB_l :     return TRUE;
       case SERB_n :     return TRUE;
       case SERB_h :     return TRUE;
       case SERB_u :     return TRUE;
       case SERB_j :     return TRUE;
    }
 }

 if( language == LANG_CROATIAN )
	{
    switch( c )
		{
		case CROAT_d :     return TRUE;
		case CROAT_sr:     return TRUE;
		case CROAT_ca:     return TRUE;
		case CROAT_zr:     return TRUE;
		case CROAT_cr:     return TRUE;
		case CROAT_D :     return FALSE;
		case CROAT_SR:     return FALSE;
		case CROAT_CA:     return FALSE;
		case CROAT_ZR:     return FALSE;
		case CROAT_CR:     return FALSE;
		}
	}	// 25.05.2001 E.P.

 if( language == LANG_POLISH ){
    switch( c )
    {
       case POLISH_SS  :     return FALSE;
       case POLISH_s   :     return TRUE;
       case POLISH_ZZR :     return FALSE;
       case POLISH_zr  :     return TRUE;
       case POLISH_LL  :     return FALSE;
       case POLISH_l   :     return TRUE;
       case POLISH_AA  :     return FALSE;
       case POLISH_a   :     return TRUE;
       case POLISH_ZZD :     return FALSE;
       case POLISH_zd  :     return TRUE;
       case POLISH_CC  :     return FALSE;
       case POLISH_c   :     return TRUE;
       case POLISH_EE  :     return FALSE;
       case POLISH_e   :     return TRUE;
       case POLISH_NN  :     return FALSE;
       case POLISH_n   :     return TRUE;
       case POLISH_OO  :     return FALSE;
       case POLISH_o   :     return TRUE;
    }
   }

 // 05.09.2000 E.P.
 if (language == LANG_CZECH)
	{
	if (
		c == AA_right_accent  ||
		c == CC_inv_roof	  ||
		c == DD_inv_roof	  ||
		c == EE_right_accent  ||
		c == EE_inv_roof	  ||
		c == II_right_accent  ||
		c == NN_inv_roof	  ||
		c == OO_right_accent  ||
		c == RR_inv_roof	  ||
		c == SS_inv_roof_latin	  ||
		c == TT_inv_roof	  ||
		c == UU_right_accent  ||
		c == UU_circle_accent ||
		c == YY_right_accent  ||
		c == ZZ_inv_roof_latin
	   )
	   return FALSE;

	if (
		c == a_right_accent	||
		c == c_inv_roof		||
		c == d_inv_roof		||
		c == e_right_accent	||
		c == e_inv_roof		||
		c == i_right_accent	||
		c == n_inv_roof		||
		c == o_right_accent	||
		c == r_inv_roof		||
		c == s_inv_roof_latin		||
		c == t_inv_roof		||
		c == u_right_accent	||
		c == u_circle_accent||
		c == y_right_accent	||
		c == z_inv_roof_latin
	   )
	   return TRUE;
	}

 // 05.09.2000 E.P.
 if (language == LANG_ROMAN)
	{
	if (
		c == AA_semicircle			||
		c == AA_roof_accent			||
		c == II_roof_accent			||
		c == SS_bottom_accent_latin ||
		c == TT_bottom_accent
	   )
	   return FALSE;

	if (
		c == a_semicircle			||
		c == a_roof_accent			||
		c == i_roof_accent			||
		c == s_bottom_accent_latin	||
		c == t_bottom_accent
	   )
	   return TRUE;
	}

 // 05.09.2000 E.P.
 if (language == LANG_HUNGAR)
	{
	if (
		c == AA_right_accent ||
		c == EE_right_accent ||
		c == II_right_accent ||
		c == OO_right_accent ||
		c == OO_2dot_accent  ||
		c == OO_double_right
	   )
	   return FALSE;

	if (
		c == a_right_accent ||
		c == e_right_accent ||
		c == i_right_accent ||
		c == o_right_accent ||
		c == o_2dot_accent  ||
		c == o_double_right
	   )
	   return TRUE;
	}

 if (language == LANG_SLOVENIAN)
	{
	if (
		c == CC_inv_roof	  ||
		c == SS_inv_roof_latin	  ||
		c == ZZ_inv_roof_latin
	   )
	   return FALSE;

	if (
		c == c_inv_roof		||
		c == s_inv_roof_latin		||
		c == z_inv_roof_latin
	   )
	   return TRUE;
	}

 if (language == LANG_LATVIAN)
	{
	if (
		c == a_macron			||
		c == c_inv_roof			||
		c == e_macron			||
		c == LATVIAN_g			||
		c == i_macron			||
		c == k_bottom_accent	||
		c == l_bottom_accent	||
		c == n_bottom_accent	||
		c == s_inv_roof_baltic	||
		c == u_macron			||
		c == z_inv_roof_baltic	||
		0
	   )
	   return TRUE;

	if (
		c == AA_macron			||
		c == CC_inv_roof		||
		c == EE_macron			||
		c == GG_bottom_accent	||
		c == II_macron			||
		c == KK_bottom_accent	||
		c == LL_bottom_accent	||
		c == NN_bottom_accent	||
		c == SS_inv_roof_baltic	||
		c == UU_macron			||
		c == ZZ_inv_roof_baltic	||
		0
	   )
	   return FALSE;
	}

 if (language == LANG_LITHUANIAN)
	{
	if (
		c == a_bottom_accent	||
		c == c_inv_roof			||
		c == e_bottom_accent	||
		c == e_dot_accent		||
		c == i_bottom_accent	||
		c == s_inv_roof_baltic	||
		c == u_bottom_accent	||
		c == u_macron			||
		c == z_inv_roof_baltic	||
		0
	   )
	   return TRUE;

	if (
		c == AA_bottom_accent	||
		c == CC_inv_roof		||
		c == EE_bottom_accent	||
		c == EE_dot_accent		||
		c == II_bottom_accent	||
		c == SS_inv_roof_baltic	||
		c == UU_bottom_accent	||
		c == UU_macron			||
		c == ZZ_inv_roof_baltic	||
		0
	   )
	   return FALSE;
	}

 if (language == LANG_ESTONIAN)
	{
	if (
		c == a_2dot_accent		||
		c == o_tild_accent		||
		c == o_2dot_accent		||
		c == s_inv_roof_baltic	||
		c == u_2dot_accent		||
		c == z_inv_roof_baltic	||
		0
	   )
	   return TRUE;

	if (
		c == AA_2dot_accent		||
		c == OO_tild_accent		||
		c == OO_2dot_accent		||
		c == SS_inv_roof_baltic	||
		c == UU_2dot_accent		||
		c == ZZ_inv_roof_baltic	||
		0
	   )
	   return FALSE;
	}

 if (language == LANG_TURKISH)
	{
	if (
		c == a_roof_accent				||
		c == c_bottom_accent			||
		c == g_semicircle				||
		c == i_sans_accent				||
		c == i_roof_accent				||
		c == o_2dot_accent				||
		c == s_bottom_accent_turkish	||
		c == u_2dot_accent				||
		c == u_roof_accent				||
		0
		)
		return TRUE;

	if (
		c == AA_roof_accent				||
		c == CC_bottom_accent			||
		c == GG_semicircle				||
		c == II_dot_accent				||
		c == II_roof_accent				||
		c == OO_2dot_accent				||
		c == SS_bottom_accent_turkish	||
		c == UU_2dot_accent				||
		c == UU_roof_accent				||
		0
	   )
	   return FALSE;
	}


 if ( ( c == liga_qm ) || ( c == liga_exm ) ||
      ( c == liga_inv_qm ) || ( c == liga_inv_exm ) )
    return FALSE;

// if ((c >= ligas_beg)&&(c <= ligas_end)) return TRUE;    // ligatures B1-BF
 if (is_liga(c)) return TRUE;  // 05.09.2000 E.P.

 if ( language != LANG_ENGLISH && language != LANG_RUSSIAN &&
		! is_cen_language(language) &&	// 07.09.2000 E.P.
		! is_baltic_language(language) &&	// 07.09.2001 E.P.
		! is_turkish_language(language)		// 20.05.2002 E.P.
	 )
 {
    switch( c )
    {
       case a_left_accent    :     return TRUE;
       case a_right_accent   :     return TRUE;
       case a_roof_accent    :     return TRUE;
       case a_2dot_accent    :     return TRUE;
       case a_circle_accent  :     return TRUE;
       case ae_deaf_sound    :     return TRUE;
       case c_bottom_accent  :     return TRUE;
       case e_left_accent    :     return TRUE;
       case e_right_accent   :     return TRUE;
       case e_roof_accent    :     return TRUE;
       case e_2dot_accent    :     return TRUE;
       case i_left_accent    :     return TRUE;
       case i_right_accent   :     return TRUE;
       case i_roof_accent    :     return TRUE;
       case i_2dot_accent    :     return TRUE;
       case o_left_accent    :     return TRUE;
       case o_right_accent   :     return TRUE;
       case o_roof_accent    :     return TRUE;
       case o_2dot_accent    :     return TRUE;
       case u_left_accent    :     return TRUE;
       case u_right_accent   :     return TRUE;
       case u_roof_accent    :     return TRUE;
       case u_2dot_accent    :     return TRUE;
       case ss_deaf_sound    :     return TRUE;
    }
 }

if( language==LANG_RUSSIAN && langUkr ){
  switch(c)
    {
    case UKR_e  : return TRUE;
    case UKR_g  : return TRUE;
    case UKR_i  : return TRUE;
    case UKR_ii : return TRUE;
    }
 }

 return FALSE;
 }

 int16_t check_foreign_language_ligature(uchar c)
//
//  This procedure checks symbol to be foreign language ligature.
//
 {

    switch( c )
    {
       case AA_left_accent    :     return TRUE;
       case AA_right_accent   :     return TRUE;
       case AA_roof_accent    :     return TRUE;
       case AA_2dot_accent    :     return TRUE;
       case AA_circle_accent  :     return TRUE;
       case AE_cap_deaf_sound :     return TRUE;
       case CC_bottom_accent  :     return TRUE;
       case EE_right_accent   :     return TRUE;
       case EE_roof_accent    :     return TRUE;
       case EE_2dot_accent    :     return TRUE;
       case II_left_accent    :     return TRUE;
       case II_right_accent   :     return TRUE;
       case II_roof_accent    :     return TRUE;
       case II_2dot_accent    :     return TRUE;
       case OO_left_accent    :     return TRUE;
       case OO_right_accent   :     return TRUE;
       case OO_roof_accent    :     return TRUE;
       case OO_2dot_accent    :     return TRUE;
       case UU_left_accent    :     return TRUE;
       case UU_right_accent   :     return TRUE;
       case UU_roof_accent    :     return TRUE;
       case UU_2dot_accent    :     return TRUE;
       case a_left_accent    :     return TRUE;
       case a_right_accent   :     return TRUE;
       case a_roof_accent    :     return TRUE;
       case a_2dot_accent    :     return TRUE;
       case a_circle_accent  :     return TRUE;
       case ae_deaf_sound    :     return TRUE;
       case c_bottom_accent  :     return TRUE;
       case e_left_accent    :     return TRUE;
       case e_right_accent   :     return TRUE;
       case e_roof_accent    :     return TRUE;
       case e_2dot_accent    :     return TRUE;
       case i_left_accent    :     return TRUE;
       case i_right_accent   :     return TRUE;
       case i_roof_accent    :     return TRUE;
       case i_2dot_accent    :     return TRUE;
       case o_left_accent    :     return TRUE;
       case o_right_accent   :     return TRUE;
       case o_roof_accent    :     return TRUE;
       case o_2dot_accent    :     return TRUE;
       case u_left_accent    :     return TRUE;
       case u_right_accent   :     return TRUE;
       case u_roof_accent    :     return TRUE;
       case u_2dot_accent    :     return TRUE;
       case ss_deaf_sound    :     return TRUE;
    }
                   return FALSE;
 }

int16_t check_foreign_language_ligature_slavic(uchar c)
//
//  This procedure checks symbol to be foreign lahguage ligature.
//
 {

    switch( c )
    {
       case CROAT_D          :     return TRUE;
       case CROAT_d          :     return TRUE;
       case CROAT_SR         :     return TRUE;
       case CROAT_sr         :     return TRUE;
       //case CROAT_CA         :     return TRUE;
       //case CROAT_ca         :     return TRUE;
       case CROAT_ZR         :     return TRUE;
       case CROAT_zr         :     return TRUE;
       case CROAT_CR         :     return TRUE;
       case CROAT_cr         :     return TRUE;
       case POLISH_SS        :     return TRUE;
       case POLISH_s         :     return TRUE;
       case POLISH_ZZR       :     return TRUE;
       case POLISH_zr        :     return TRUE;
       case POLISH_LL        :     return TRUE;
       case POLISH_l         :     return TRUE;
       case POLISH_AA        :     return TRUE;
       case POLISH_a         :     return TRUE;
       case CC_right_accent  :     return TRUE;
       case c_right_accent   :     return TRUE;
       case POLISH_ZZD       :     return TRUE;
       case POLISH_zd        :     return TRUE;
       //case POLISH_CC        :     return TRUE;
       //case POLISH_c         :     return TRUE;
       case POLISH_EE        :     return TRUE;
       case POLISH_e         :     return TRUE;
       case POLISH_NN        :     return TRUE;
       case POLISH_n         :     return TRUE;
       case POLISH_OO        :     return TRUE;
       case POLISH_o         :     return TRUE;
    }
                   return FALSE;
 }

 int16_t check_alphanum(uchar c)
//
//	This procedure checks symbol to be letter or number.
//	that means 0-9, A-Z, a-z, $.
//
{
 if (isletter(c))              return 1;
 if ((c >= '0') && (c <= '9')) return 1;
 if (c == '$')                 return 1;
 if (c == '%')                 return 1;

  // Теперь макры лиг годятся для всех языков // 07.09.2000 E.P.
 if ( ( c == liga_qm ) || ( c == liga_exm ) ||
      ( c == liga_inv_qm )||
      ( c == liga_inv_exm )
	 )
    return FALSE;

 // Остальные лиги // 07.09.2000 E.P.
 if (is_liga(c))
	 return 1;

// if ((c >= ligas_beg)&&(c <= ligas_end)) return 1;    // ligatures B1-BF

 if ( language != LANG_ENGLISH &&
      !(language == LANG_RUSSIAN && langSer) &&
	  ! is_cen_language(language) &&		// 07.09.2000 E.P.
	  ! is_baltic_language(language) &&		// 09.07.2001 E.P.
	  ! is_turkish_language(language) &&	// 21.05.2002 E.P.
      check_foreign_language_ligature( c )
	)
    return 1;

  if ( language == LANG_CROATIAN &&
      (check_foreign_language_ligature_slavic( c )) )
    return 1;

  if ( language == LANG_POLISH && is_polish_special (c))  // 22.10.97
    return 1;

  // 05.09.2000 E.P.
  if ( language == LANG_CZECH && is_czech_special (c))
    return 1;

  // 05.09.2000 E.P.
  if ( language == LANG_ROMAN && is_roman_special (c))
    return 1;

  // 05.09.2000 E.P.
  if ( language == LANG_HUNGAR && is_hungar_special (c))
    return 1;

  if( language == LANG_RUSSIAN && langSer &&
      is_serbian_special( c ) )
    return 1;

  // 25.05.2001 E.P.
  if ( language == LANG_SLOVENIAN && is_slovenian_special (c))
    return 1;

  // 09.07.2001 E.P.
  if ( language == LANG_LATVIAN && is_latvian_special (c))
    return 1;

  if ( language == LANG_LITHUANIAN && is_lithuanian_special (c))
    return 1;

  if ( language == LANG_ESTONIAN && is_estonian_special (c))
    return 1;

  if ( language == LANG_TURKISH && is_turkish_special (c))
    return 1;

 return 0;
}

 int16_t check_lowerconson(uchar c)
//
//	This procedure checks symbol to be lowercase consonant,
//	that means b,c,d,f,g,h,j,k,l,m,n,p,q,r,s,t,v,w,x,y,z.
//
 {

 if ((c >= 'b') && (c <= 'd')) return 1;
 if ((c >= 'f') && (c <= 'h')) return 1;
 if ((c >= 'j') && (c <= 'n')) return 1;
 if ((c >= 'p') && (c <= 't')) return 1;
 if ((c >= 'v') && (c <= 'x')) return 1;
 if (c == 'z')                 return 1;
 if (c == c_bottom_accent)     return 1;
 if (c == ss_deaf_sound)       return 1;

 if ( language == LANG_POLISH ){
    if (c == POLISH_s    )     return 1;
    if (c == POLISH_zd   )     return 1;
    if (c == POLISH_zr   )     return 1;
    if (c == POLISH_l    )     return 1;
    if (c == POLISH_c    )     return 1;
    if (c == POLISH_n    )     return 1;
 }

 if ( language == LANG_CROATIAN ){
    if (c == CROAT_d    )     return 1;
 }

 // 05.09.2000 E.P.
 if (language == LANG_CZECH)
	{
	if (
		c == c_inv_roof		||
		c == d_inv_roof		||
		c == n_inv_roof		||
		c == r_inv_roof		||
		c == s_inv_roof_latin		||
		c == t_inv_roof		||
		c == z_inv_roof_latin
	   )
	   return 1;
	}

 // 05.09.2000 E.P.
 if (language == LANG_ROMAN)
	{
	if (
		c == s_bottom_accent_latin	||
		c == t_bottom_accent
	   )
	   return 1;
	}

 // 25.05.2001 E.P.
 if (language == LANG_SLOVENIAN)
	{
	if (
		c == c_inv_roof			||
		c == s_inv_roof_latin	||
		c == z_inv_roof_latin
	   )
	   return 1;
	}

 // 07.09.2001 E.P.
 if (language == LANG_LATVIAN)
	 {
	 if (
		 c == c_inv_roof		||
		 c == LATVIAN_g			||
		 c == k_bottom_accent	||
		 c == l_bottom_accent	||
		 c == n_bottom_accent	||
		 c == s_inv_roof_baltic	||
		 c == z_inv_roof_baltic	||
		 0
		 )
	   return 1;

	 }

 if (language == LANG_LITHUANIAN)
	 {
	 if (
		 c == c_inv_roof		  ||
		 c == s_inv_roof_baltic||
		 c == z_inv_roof_baltic||
		0
		)
	   return 1;


	 }

 if (language == LANG_ESTONIAN)
	 {
	 if (
		 c == s_inv_roof_baltic	||
		 c == z_inv_roof_baltic	||
		 0
		 )
	   return 1;
	 }

 if (language == LANG_TURKISH)
	 {
	 if (
		 c == c_bottom_accent	||
		 c == s_bottom_accent	||
		 c == g_semicircle		||
		 0
		 )
	   return 1;
	 }

 return 0;

}

void test_irish_name_cur_sym( int16_t Let )
{
    switch ( fl_beg_irish_name )
    {
      case CAPITAL_M:
        if ( Let == 'c' )
          fl_beg_irish_name = LOWER_C;
        else
          if ( Let == 'a' )
            fl_beg_irish_name = LOWER_A;
          else
            fl_beg_irish_name = NO_IRISH_NAME;
        break;

      case LOWER_A  :
        if ( Let == 'c' )
          fl_beg_irish_name = LOWER_C;
        else
          fl_beg_irish_name = NO_IRISH_NAME;
        break;

      case LOWER_C  :
        if ( check_upper( (uchar)Let )  )
          fl_beg_irish_name = AFTER_C;
        else
          fl_beg_irish_name = NO_IRISH_NAME;
        break;

      case AFTER_C  :
      default       :
        if ( ( n_ltr == 0 ) && ( Let == 'M' ) )    // first letter
          fl_beg_irish_name = CAPITAL_M;
        else
          fl_beg_irish_name = NO_IRISH_NAME;
        break;
    }
}

 void test_O_and_slash()
{
cell *C;
int16_t nv;

 C = cell_f();
 while ((C=C->nextl)->nextl)
 {
  nv = get_nvers(C, '0');
  if ( nv >= 0 )
     //  Minimum decrease for 'O' to prefer '0' in the cases without
     //  obvious decision
   {
#ifndef UFA
  if( language!=LANG_RUSSIAN )
    cell_bonus(C, &(C->vers[nv]), 2);
#else
   if( language!=LANG_RUSSIAN )
    cell_bonus(C, &(C->vers[nv]), 2);
   else if( language==LANG_RUSSIAN )
   {
   if( get_nvers(C,'9')<0 && get_nvers(C,'6')<0 && get_nvers(C,'8')<0  )
    cell_bonus(C, &(C->vers[nv]), 2);
    // Oleg : 06-09-1994 : not exist '698' - bonus 0
   else
    { // Oleg : 06-09-1994 : exist any '698'-version
    nv=get_nvers(C,'Ћ');
    if( nv>=0 )
      cell_bonus(C, &(C->vers[nv]), -2);            // monus O
    }
   }
#endif
   sort_vers(C);
   }
  nv = get_nvers(C, '/');
  if ( nv == -1 ) continue;
  if( language == LANG_RUSSIAN && langSer  && nv &&
      C->nvers && C->vers[0].let==SERB_j )
      continue;
  inc_or_dec_slash(C, nv);
 }
}

static void inc_or_dec_slash(cell *wc, int16_t numv)
//
//  This procedure decreases probability '/' if its neighbour letters are
//   italic ( to prefer 'l' or 'I' ) and
//   increases probability '/' if its heighbour letters are straight
//   ( to avoid 'l' and 'I' appearence );
//
 {
 cell  *cprev, *cnext;
 Bool   no_Il = (get_nvers(wc, 'I')==-1) && (get_nvers(wc, 'l')==-1 && wc->vers[numv].let=='/');

  if ( wc->vers[0].prob - wc->vers[numv].prob > REFUSE_BOUND )
   return;
  cprev = wc->prev;
  cnext = wc->next;
  if ( !((cprev->flg & c_f_let) && (cnext->flg & c_f_let)) ) return;
  if ( ((cprev->font|cprev->font_new) & c_fp_it) && ((cnext->font|cnext->font_new) & c_fp_it) )
   { cell_bonus(wc, &(wc->vers[numv]), (int16_t)(no_Il?2:-BONUS_SLASH)); goto IDS_Sort; }
  if ( ((cprev->font|cprev->font_new) & c_fp_str) && ((cnext->font|cnext->font_new) & c_fp_str) )
    cell_bonus(wc, &(wc->vers[numv]), (int16_t)(no_Il?2:BONUS_SLASH));
IDS_Sort:
  sort_vers(wc);
 }

int16_t english_context_process( cell* C )
{
int16_t return_code, nv;

  return_code = NO_ACTION;

  if (n_ltr == 1 )
  //** single letter case: **
  {
    nv = get_nvers(C, 'O');

    if ( ( nv >= 0) && (C->next->flg&c_f_punct ) &&
         ( C->next->vers[0].let== APOSTROPH ) &&
         ( ( C->nextl ) && ( check_upper( C->nextl->vers[0].let ) ) ) )
    {  // case as: "O'Neel"
      vers_to_first_place( C, nv );
      return_code = CONTINUE_ACTION;
      goto ECP_Ret;
    }

    if ( ( nv >= 0) && (C->prev->flg&c_f_let ) &&
         ( C->prev->vers[0].let== '/' ) && ( C->prev->prev ) &&
         ( C->prev->prev->vers[0].let == 'I' ) )
    {  // case as: "I/O"
      vers_to_first_place( C, nv );
      return_code = CONTINUE_ACTION;
      goto ECP_Ret;
    }

    if ( !(C->next->flg&c_f_punct && C->next->vers[0].let=='.') )
    {
      if( language != LANG_LITHUANIAN || C->vers[0].let != i_bottom_accent ) // Nick 4.09.2001
          LiliI(C);
      return_code = CONTINUE_ACTION;
      goto ECP_Ret;
    }
  }

  if ((n_ltr == 2) && (word_flag & AFTERAPOSTROPHE))
  /***  'll case ***/
  {
    if (apostr_ll(C))
        return_code = CONTINUE_ACTION;
    else
        return_code = NO_ACTION;
  }
ECP_Ret:
    return  return_code;
}

static int16_t LiliI(cell *NC)
//
//	This procedure gives bonus to 'I' in one letter word.
//
 {
 int16_t i;
 version *v;

 bon_to_dig(NC);                    // monus to non- "1"
 for (i=0,v=NC->vers; i < NC->nvers; i++,v++)
  {
  if (v->let == 'I')
   goto monus;
  }

 return 0;
monus:;
 for (i=0,v=NC->vers; i < NC->nvers; i++,v++)
  {
  if (v->let == 'I')
   continue;
  v->prob=MAX(v->prob-MONUS,10);   // didn't expect number
  }
 sort_vers(NC);
 return 1;
 }

static int16_t bon_to_dig(cell *NC)
//
//	This procedure gives bonus to digits in one letter word.
//
 {
 int16_t i;
 version *v;

 for (i=0,v=NC->vers; i < NC->nvers; i++,v++)
  if ((v->let >= '0')  && (v->let <= '9'))
   goto monus;
 return 0;
monus:;
 for (i=0,v=NC->vers; i < NC->nvers; i++,v++)
  if ((v->let < '0')  || (v->let > '9'))
   v->prob=MAX(v->prob-MONUS+MONUS1I,10);  // didn't expect other than 1
 sort_vers(NC);
 return 1;
 }

static Bool apostr_ll(cell *NC)
//
//	This procedure gives bonus to ll in two letters word
//	if there is after apostrophe. Example: he'll
//	ON ENTRY: NC - address of cell for first letter.
//
{
 int16_t i;
 version *v;

 for (i=0,v=NC->vers; i < NC->nvers; i++,v++)
  if (v->let == 'l')
   goto second;
 return FALSE;
second:;
 for (i=0,v=NC->nextl->vers; i < NC->nextl->nvers; i++,v++)
  if (v->let == 'l')
   goto monus;
 return FALSE;
monus:;
 for (i=0,v=NC->vers; i < NC->nvers; i++,v++)
  {
  if (v->let=='l')
   continue;
  v->prob=MAX(v->prob-MONUS,10);   // didn't expect other than l
  }
 sort_vers(NC);
 for (i=0,v=NC->nextl->vers; i < NC->nextl->nvers; i++,v++)
  {
  if (v->let == 'l')
   continue;
  v->prob=MAX(v->prob-MONUS,10);   // didn't expect other than l
  }
 sort_vers(NC->nextl);
 return TRUE;
}

int16_t franch_context_process( cell* C )
{
int16_t return_code;
int16_t i, nvI, nvl, nvs;

  return_code = NO_ACTION;

  if (n_ltr == 1 && (C->next->flg&c_f_punct) &&
                (C->next->vers[0].let== APOSTROPH ) )
  /**** shorted article case: *****/
  {
    for ( i = 0; i < C->nvers; i++ )
    {
        switch( C->vers[i].let )
        {
            case    'l':
            case    'd':
            case    'L':
            case    'D':
            {
                C->vers[i].prob =
                    MIN( MAX_PROB, C->vers[i].prob + FRANCH_ART_BONUS );
                return_code = CONTINUE_ACTION;
                break;
            }
            default    :
            {
                C->vers[i].prob =
                    MAX( MIN_PROB, C->vers[i].prob - FRANCH_ART_BONUS );
            }
        }
    }
    if ( return_code == CONTINUE_ACTION )
    {
        sort_vers( C );
        goto FCP_Ret;
    }
  }
  if ( ( (n_ltr == 2) || (n_ltr == 3) ) &&
       ( ( word_flag & AFTERDOTCOM ) ||
         ( word_flag & AFTERDOT ) ||
         ( C->prev->flg & c_f_fict ) ) )
      /*** "Il" or "Ils" case ***/
  {
    nvI = get_nvers( C, 'I' );
    if ( nvI < 0 )
        goto    FCP_Ret;
    if ( C->vers[ nvI ].prob < ADM_PROB )
        goto    FCP_Ret;
    nvl = get_nvers( C->nextl, 'l' );
    if ( nvl < 0 )
        goto    FCP_Ret;
    if ( C->nextl->vers[ nvl ].prob < ADM_PROB )
        goto    FCP_Ret;
    if ( n_ltr == 2 )
        goto FCP_Il_case;
    nvs = get_nvers( C->nextl->nextl, 's' );
    if ( nvs < 0 )
        goto    FCP_Ret;
    if ( C->nextl->nextl->vers[ nvs ].prob < ADM_PROB )
        goto    FCP_Ret;
    vers_to_first_place( C->nextl->nextl, nvs );

FCP_Il_case:
    vers_to_first_place( C, nvI );
    vers_to_first_place( C->nextl, nvl );
    return_code = CONTINUE_ACTION;
  }

FCP_Ret:
  return return_code;
}

int16_t german_context_process( cell* C )
{
int16_t return_code;
cell * wc;
uchar i, wp;
int16_t  nv_c, nv_h, nv_b;

    return_code = NO_ACTION;

    if ( ( n_ltr == 1 ) && ( C->vers[0].let == ss_deaf_sound ) )
    {
        C->vers[0].prob = MAX( MIN_PROB, C->vers[0].prob - GER_DOUBLE_S_MONUS );
        sort_vers( C );
        return_code = CONTINUE_ACTION;
        goto    GCP_Ret;
    }

    if ( n_ltr > 1 )
    {
        if ( C->vers[0].let == ss_deaf_sound )
        {
            C->vers[0].prob =
                MAX( MIN_PROB, C->vers[0].prob - GER_DOUBLE_S_MONUS );
            sort_vers( C );
        }
        for ( i = 0, wc = C; i < n_ltr - 1; wc = wc->nextl, i++ )
        {
            nv_c = get_nvers( wc, 'c' );
            if ( nv_c < 0 ) continue;
            if ( ( nv_c > 1 ) || ( wc->vers[nv_c].prob < ADM_PROB ) )
                continue;
            nv_h = get_nvers( wc->nextl, 'h' );
            if ( nv_h < 0 ) continue;
            nv_b = get_nvers( wc->nextl, 'b' );
            if ( nv_b < 0 ) continue;
            if ( ( nv_h > nv_b ) &&
                 ( wc->nextl->vers[nv_b].prob -
                    wc->nextl->vers[nv_h].prob < REFUSE_BOUND ) )
            {
               wp = wc->nextl->vers[nv_h].prob;
               wc->nextl->vers[nv_h].prob = wc->nextl->vers[nv_b].prob;
               wc->nextl->vers[nv_b].prob = wp;
               sort_vers( wc->nextl );
            }
        }
    }
GCP_Ret:
    return return_code;
}

int16_t swedish_context_process     ( cell* C )
{
int16_t return_code;

    swed_lju_flag = FALSE;
    return_code = NO_ACTION;
    if (n_ltr == 1 && !(C->next->flg&c_f_punct && C->next->vers[0].let=='.'))
    /** single letter case: **/
    {
      LiliI(C);
      return_code = CONTINUE_ACTION;
      goto SwCP_Ret;
    }
    if ( n_ltr > 2 )        // Process 'lju' at start of word
    {
      int16_t nv_l, nv_j, nv_u;

      nv_l = get_nvers( C, 'l');
      if ( nv_l < 0 ) goto SwCP_Ret;
      if ( C->vers[ nv_l ].prob < ADM_PROB ) goto SwCP_Ret;

      nv_j = get_nvers( C->nextl, 'j');
      if ( nv_j < 0 ) goto SwCP_Ret;
      if ( C->nextl->vers[ nv_j ].prob < ADM_PROB ) goto SwCP_Ret;

      nv_u = get_nvers( C->nextl->nextl, 'u');
      if ( nv_u < 0 ) goto SwCP_Ret;
      if ( C->nextl->nextl->vers[ nv_u ].prob < ADM_PROB ) goto SwCP_Ret;

      cell_bonus( C, &(C->vers[nv_l]), SWED_LJU_BONUS );
      sort_vers( C );
      cell_bonus( C->nextl, &(C->nextl->vers[nv_j]), SWED_LJU_BONUS );
      sort_vers( C->nextl );
      cell_bonus( C->nextl->nextl, &(C->nextl->nextl->vers[nv_u]), SWED_LJU_BONUS );
      sort_vers( C->nextl->nextl );
      swed_lju_flag = TRUE;
    }
SwCP_Ret:
    return return_code;
}

int16_t spanish_context_process     ( cell* C )
{
int16_t return_code;

    return_code = NO_ACTION;
    if ( n_ltr > 2 )
    {
      if ( apostr_ll( C ) )
        goto SpCP_Ret;
    }
    return_code = franch_context_process( C );
SpCP_Ret:
    return return_code;
}

int16_t get_nvers ( cell *A, int16_t Let )
{
int16_t n;
  for ( n = 0; n < A->nvers; n++)
   {
    if (  A->vers[n].let == Let )
      return n;
   }
  return -1;
}

void vers_to_first_place ( cell *A, int16_t nver)
{
version *v, wver;
 v = A->vers;
 v+=nver;
 if (nver == 0) return;

 while (nver--)
  {
  memcpy(&wver,v,sizeof(*v));
  memcpy(v,v-1,sizeof(*v));
  memcpy(v-1,&wver,sizeof(*v));
  v--;
  }
 v->prob=(v+1)->prob;        // make first version equal to former first

}

void test_roma_num_sym( cell *C )
//
// This procedure tests versions to be like roman numeric symbols:
//  X, V, I, L, C;  If symbol likes roman digit --> counter
//  roman_sym_num is incremented by 1;
//
{

    switch( C->vers[0].let )
    {
        case    'X':
            if ( ( C->nvers == 1 ) || (C->vers[1].let == 'x' ) )
                roman_sym_num ++;
            break;
        case    'x':
            if ( (C->nvers > 1) && (C->vers[1].let == 'X' ) )
               //  ( C->vers[0].prob - C->vers[1].prob  < REFUSE_BOUND ) )
                roman_sym_num ++;
            break;
        case    'V':
            if ( ( C->nvers == 1 ) || (C->vers[1].let == 'v' ) )
                roman_sym_num ++;
            break;
        case    'v':
            if ( (C->nvers > 1) && (C->vers[1].let == 'V' ) &&
                 ( C->vers[0].prob - C->vers[1].prob  < REFUSE_BOUND ) )
                roman_sym_num ++;
            break;
        case    'L':
            if ( ( C->nvers == 1 ) ||
                 ( C->vers[0].prob - C->vers[1].prob  > REFUSE_BOUND/2 ) )
                roman_sym_num ++;
            break;
        case    'I':
            if ( ( C->nvers == 1 ) || (C->vers[1].let == 'l') )
                roman_sym_num ++;
            break;
        case    'l':
            if ( ( C->nvers > 1 ) && (C->vers[1].let == 'I') &&
                 ( C->vers[0].prob - C->vers[1].prob  < REFUSE_BOUND ) )
                roman_sym_num ++;
            break;
        case    'C':
            if ( ( C->nvers == 1 ) || (C->vers[1].let == 'c' ) )
                roman_sym_num ++;
            break;
        case    'c':
            if ( (C->nvers > 1) && (C->vers[1].let == 'C' )  &&
                 ( C->vers[0].prob - C->vers[1].prob  < REFUSE_BOUND ) )
                roman_sym_num ++;
            break;
        default:
            break;
    }
}

