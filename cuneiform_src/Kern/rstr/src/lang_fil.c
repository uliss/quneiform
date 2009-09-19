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
/*	    Filtration of version corresponding	to language          */
/*                                                                   */
/*********************************************************************/
#include <string.h>

#include "struct.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"
#include "linutil.h"
#include "tuner.h"
#include "alphabet.h"

extern uchar language;
extern uchar langUkr;
extern uchar langBul;
extern uchar langSer;
extern c_comp wcomp;
extern uchar multy_language ;
extern uchar alphabet[];
extern uchar alpha_used_mode;
extern uchar line_tabcell;
//////////////////////////////////////////////////////////////////////////////////////
//
static uchar Only_symbols[257]={
/*     0 1 2 3 4 5 6 7 8 9 a b c d e f  */
/* 0*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* 1*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* 2*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* 3*/ 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
/* 4*/ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
/* 5*/ 1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
/* 6*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* 7*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* 8*/ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
/* 9*/ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
/* a*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* b*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* c*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* d*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* e*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* f*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
//////////////////////////////////////////////////////////////////////////////////////
//
extern  uchar    line_alphabet;
static Bool proplet_tg(uchar let)
{

	extern uchar NO_Punct;

	if ( language == LANG_DIG )
		return is_digit(let);

	if ( NO_Punct )
		return Only_symbols[let];
    if( line_alphabet==ALPHA_DIGITAL_TRUE )
        return is_digit(let);

	if( language==LANG_RUSSIAN )
	{
		if( langUkr )
		{
			if( memchr("љк›лќн",let,6) )
				return  FALSE;
		}
		else
		{
			if( let == UKR_I || let == UKR_i || let == UKR_II || //let == UKR_ii || SERB_n
				let == UKR_E || let == UKR_e || let == UKR_G || let == UKR_g || let == liga_i || memchr("iI",let,2) )
				return  FALSE;
		}

		if( langSer )
		{
			if( memchr("‰©™йљк›лњмќнћоџп",let,16) )
				return  FALSE;
		}
		else
		{
			if( let == SERB_HH || let == SERB_hh || let == SERB_H || let == SERB_h ||
				let == SERB_L || let == SERB_l || let == SERB_N || let == SERB_n ||
				let == SERB_U || let == SERB_u || let == SERB_J || let == SERB_j ||
				let == liga_j || memchr("jJ",let,2) )
				return  FALSE;
		}

		// В Болгарском нет трех русских букв. 05.09.2000 E.P.
		if (langBul &&
			( let == r_EE_2dot ||
			  let == r_e_2dot ||
			  let==155 || let==235 || // Ыы 08.09.2000 E.P.
			  let==157 || let==237  // Ээ 08.09.2000 E.P.
			) )
			return FALSE;

	}

	// for INTERSEPTOR need "S"
	if(language==LANG_RUSSIAN && multy_language && (is_english(let)||let=='$'||let==(uchar)'Я'))
		return FALSE;
	// Oleg : 20-10-1994 : kill english versions for Rus-Eng mode
	if(language==LANG_ENGLISH && (is_russian(let)||let=='|'))
		return FALSE;
	// Oleg : 20-10-1994 : kill russian versions for Rus-Eng mode

	// Затычка от ошибок в таблицах. 12.09.2000 E.P.
	if (!let &&
		 (is_cen_language(language) ||
		  is_baltic_language(language) ||	// 09.07.2001 E.P.
		  is_turkish_language(language)		// 21.05.2002 E.P.
		 )
	   )
		return FALSE;

	if( language==LANG_CROATIAN )
	{
		if( let<128 ||
			(is_ligas_latin(let)&&let!=POLISH_l&&let!=POLISH_a) ||
			let==CROAT_D  || let==CROAT_d  ||
			let==CROAT_SR || let==CROAT_sr ||
			let==CROAT_CA || let==CROAT_ca ||
			let==CROAT_ZR || let==CROAT_zr ||
			let==CROAT_CR || let==CROAT_cr )
			return TRUE;
		else
			return FALSE;
	}

	if( language==LANG_POLISH )
	{
		if( let < 128 || is_ligas_latin(let) ||
			is_polish_special( let )
		  )
			return TRUE;
		else
			return FALSE;
	}

	// 05.09.2000 E.P.
	if( language==LANG_CZECH )
	{
		if( let < 128 || is_ligas_latin(let) ||
			is_czech_special(let)
		  )
			return TRUE;
		else
			return FALSE;
	}

	// 05.09.2000 E.P.
	if( language==LANG_ROMAN )
	{
		if( let < 128 || is_ligas_latin(let) ||
			is_roman_special(let)
		  )
			return TRUE;
		else
			return FALSE;
	}

	// 05.09.2000 E.P.
	if( language==LANG_HUNGAR )
	{
		if( let < 128 || is_ligas_latin(let) ||
			is_hungar_special(let)
		  )
			return TRUE;
		else
			return FALSE;
	}

	// 25.05.2001 E.P.
	if( language==LANG_SLOVENIAN )
	{
		if( isnot_slovenian(let) )
			return FALSE;

		if( let < 128 || is_ligas_latin(let) ||
			is_slovenian_special(let)
		  )
			return TRUE;
		else
			return FALSE;
	}

	// 09.07.2001 E.P.
	if( language==LANG_LATVIAN)
	{
		if( isnot_latvian(let) )
			return FALSE;

		if( let < 128 || is_ligas_baltic(let) ||
			is_latvian_special(let)
		  )
			return TRUE;
		else
			return FALSE;
	}

	// 09.07.2001 E.P.
	if( language==LANG_LITHUANIAN)
	{
		if( isnot_lithuanian(let) )
			return FALSE;

		if( let < 128 || is_ligas_baltic(let) ||
			is_lithuanian_special(let)
		  )
			return TRUE;
		else
			return FALSE;
	}

	// 09.07.2001 E.P.
	if( language==LANG_ESTONIAN)
	{
		if( isnot_estonian(let) )
			return FALSE;

		if( let < 128 || is_ligas_baltic(let) ||
			is_estonian_special(let)
		  )
			return TRUE;
		else
			return FALSE;
	}

	// 20.05.2002 E.P.
	if( language==LANG_TURKISH)
	{
		if( isnot_turkish(let) )
			return FALSE;

		if( let < 128 || is_ligas_turkish(let) ||
			is_turkish_special(let)
		  )
			return TRUE;
		else
			return FALSE;
	}

	if (language==LANG_RUSSIAN)
		return TRUE;

	if (language!=LANG_FRENCH &&
		(let==CC_bottom_accent || let==c_bottom_accent) ||
		language!=LANG_GERMAN && let==ss_deaf_sound     ||
		language!=LANG_SPANISH && (let==invers_exm || let==invers_qm ||
		let==liga_inv_exm || let==liga_inv_qm))
		return FALSE;
    if( line_tabcell && let==liga_exm )
        return FALSE;

	return TRUE;
}

Bool proplet(uchar let)
{
Bool rc = proplet_tg(let) ;
if( !alpha_used_mode )
    return rc;
return (alphabet[let] && rc);
}
