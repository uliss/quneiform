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

/*#include <io.h>*/
#include <string.h>
#include "cttypes.h"
#include "lang.h"
#include "ligas.h"
#include "evn.h"

#include "compat_defs.h"

static uchar alph_russian[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, // 5
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, // 7
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // d
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_digital[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // 3
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 5
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_ruseng[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, // 5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // 7
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // d
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_english[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, // 5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_ce[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, // 5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // 7
		1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar *alph_lang[LANG_TOTAL] = { alph_english, // LANG_ENGLISH    0
		alph_english, // LANG_GERMAN     1
		alph_english, // LANG_FRENCH     2
		alph_russian, // LANG_RUSSIAN    3
		alph_english, // LANG_SWEDISH    4
		alph_english, // LANG_SPANISH    5
		alph_english, // LANG_ITALIAN    6
		alph_ruseng, // LANG_RUSENG     7
		alph_russian, // LANG_UKRAINIAN  8
		alph_russian, // LANG_SERBIAN    9
		alph_ce, // LANG_CROATIAN   10
		alph_ce, // LANG_POLISH     11
		alph_english, // LANG_DANISH     12
		alph_english, // LANG_PORTUGUESE 13
		alph_english, // LANG_DUTCH      14
		alph_digital, // LANG_DIG        15
		alph_russian, // LANG_UZBEK		 16	// 01.09.2000 E.P.
		alph_russian, // LANG_KAZ	     17
		alph_ruseng, // LANG_KAZ_ENG    18
		alph_ce, // LANG_CZECH		 19
		alph_ce, // LANG_ROMAN		 20
		alph_ce, // LANG_HUNGAR	 21
		alph_russian, // LANG_BULGAR	 22
		alph_ce, // LANG_SLOVENIAN	 23
		alph_english, // LANG_LATVIAN	 24
		alph_english, // LANG_LITHUANIAN 25
		alph_english, // LANG_ESTONIAN	 26
		alph_english // LANG_TURKISH	 27
		};

Bool16 rec_set_alpha(uchar language, uchar *alphabet) {
	if (language >= LANG_TOTAL)
		return FALSE;
	if (alph_lang[language])
		memcpy(alphabet, alph_lang[language], 256);
	else
		return FALSE;
	if (language == LANG_RUSSIAN || language == LANG_RUSENG || language
			== LANG_BULGAR // 01.09.2000 E.P.
	) {
		alphabet[r_cu_d] = 1;
		alphabet[r_cu_g] = 1;
		alphabet[r_cu_m] = 1;
		alphabet[r_cu_u] = 1;
		alphabet[r_cu_z] = 1;
		alphabet[r_cu_a] = 1;

		alphabet[liga_exm] = 1;
		alphabet[liga_qm] = 1;
		alphabet[liga_uperc] = 1;
		alphabet[liga_lperc] = 1;
	}

	// В Болгарском нет трех русских букв. 01.09.2000 E.P.
	if (language == LANG_BULGAR) {
		alphabet[r_EE_2dot] = 0;
		alphabet[r_e_2dot] = 0;
		alphabet[(uchar) 'Ы'] = 0;
		alphabet[(uchar) 'ы'] = 0;
		alphabet[(uchar) 'Э'] = 0;
		alphabet[(uchar) 'э'] = 0;
	}

	if (language == LANG_UKRAINIAN) {
		alphabet[r_cu_d] = 1;
		alphabet[r_cu_g] = 1;
		alphabet[r_cu_m] = 1;
		alphabet[r_cu_u] = 1;
		alphabet[r_cu_z] = 1;
		alphabet[r_cu_a] = 1;
		alphabet['I'] = 1;
		alphabet['i'] = 1;
		alphabet[UKR_I] = 1;
		alphabet[UKR_i] = 1;
		alphabet[UKR_II] = 1;
		alphabet[UKR_ii] = 1;
		alphabet[UKR_E] = 1;
		alphabet[UKR_e] = 1;
		alphabet[UKR_G] = 1;
		alphabet[UKR_g] = 1;
		alphabet[liga_i] = 1;

		alphabet[liga_exm] = 1;
		alphabet[liga_qm] = 1;
		alphabet[liga_uperc] = 1;
		alphabet[liga_lperc] = 1;
	}
	if (language == LANG_SERBIAN) {
		alphabet[r_cu_d] = 1;
		alphabet[r_cu_g] = 1;
		alphabet[r_cu_m] = 1;
		alphabet[r_cu_u] = 1;
		alphabet[r_cu_z] = 1;
		alphabet[r_cu_a] = 1;
		alphabet[SERB_HH] = 1;
		alphabet[SERB_hh] = 1;
		alphabet[SERB_L] = 1;
		alphabet[SERB_l] = 1;
		alphabet[SERB_N] = 1;
		alphabet[SERB_n] = 1;
		alphabet[SERB_H] = 1;
		alphabet[SERB_h] = 1;
		alphabet[SERB_U] = 1;
		alphabet[SERB_u] = 1;
		alphabet[SERB_J] = 1;
		alphabet[SERB_j] = 1;
		alphabet['J'] = 1;
		alphabet['j'] = 1;
		alphabet[liga_j] = 1;

		alphabet[liga_exm] = 1;
		alphabet[liga_qm] = 1;
		alphabet[liga_uperc] = 1;
		alphabet[liga_lperc] = 1;
	}

	if (language == LANG_CROATIAN) {
		alphabet[CROAT_D] = 1;
		alphabet[CROAT_d] = 1;
		alphabet[CROAT_SR] = 1;
		alphabet[CROAT_sr] = 1;
		alphabet[CROAT_CA] = 1;
		alphabet[CROAT_ca] = 1;
		alphabet[CROAT_ZR] = 1;
		alphabet[CROAT_zr] = 1;
		alphabet[CROAT_CR] = 1;
		alphabet[CROAT_cr] = 1;
	}
	if (language == LANG_POLISH) {
		alphabet[POLISH_SS] = 1;
		alphabet[POLISH_s] = 1;
		alphabet[POLISH_ZZR] = 1;
		alphabet[POLISH_zr] = 1;
		alphabet[POLISH_ZZD] = 1;
		alphabet[POLISH_zd] = 1;
		alphabet[POLISH_CC] = 1;
		alphabet[POLISH_c] = 1;
		alphabet[POLISH_EE] = 1;
		alphabet[POLISH_e] = 1;
		alphabet[POLISH_NN] = 1;
		alphabet[POLISH_n] = 1;
		alphabet[POLISH_OO] = 1;
		alphabet[POLISH_o] = 1;
		alphabet[POLISH_LL] = 1;
		alphabet[POLISH_l] = 1;
	}

	if (language == LANG_GERMAN) {
		alphabet[AA_2dot_accent] = 1;
		alphabet[a_2dot_accent] = 1;
		alphabet[UU_2dot_accent] = 1;
		alphabet[u_2dot_accent] = 1;
		alphabet[OO_2dot_accent] = 1;
		alphabet[o_2dot_accent] = 1;
		alphabet[0xDF] = 1; // ??? small S-cet
	}

	if (language == LANG_FRENCH) {
		alphabet[AA_left_accent] = 1;
		alphabet[AA_roof_accent] = 1;
		alphabet[CC_bottom_accent] = 1;
		alphabet[EE_left_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[EE_roof_accent] = 1;
		alphabet[EE_2dot_accent] = 1;
		alphabet[II_roof_accent] = 1;
		alphabet[II_2dot_accent] = 1;
		alphabet[OO_roof_accent] = 1;
		alphabet[OE_cap_deaf_sound] = 1;
		alphabet[AE_cap_deaf_sound] = 1;
		alphabet[UU_left_accent] = 1;
		alphabet[UU_roof_accent] = 1;
		alphabet[UU_2dot_accent] = 1;

		alphabet[a_left_accent] = 1;
		alphabet[a_roof_accent] = 1;
		alphabet[c_bottom_accent] = 1;
		alphabet[e_left_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[e_roof_accent] = 1;
		alphabet[e_2dot_accent] = 1;
		alphabet[i_roof_accent] = 1;
		alphabet[i_2dot_accent] = 1;
		alphabet[o_roof_accent] = 1;
		alphabet[oe_deaf_sound] = 1;
		alphabet[ae_deaf_sound] = 1;
		alphabet[u_left_accent] = 1;
		alphabet[u_roof_accent] = 1;
		alphabet[u_2dot_accent] = 1;
	}

	if (language == LANG_SWEDISH) {
		alphabet[AA_2dot_accent] = 1;
		alphabet[OO_2dot_accent] = 1;
		alphabet[AA_circle_accent] = 1;

		alphabet[a_2dot_accent] = 1;
		alphabet[o_2dot_accent] = 1;
		alphabet[a_circle_accent] = 1;
	}
	if (language == LANG_SPANISH) {
		alphabet[AA_right_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[II_right_accent] = 1;
		alphabet[OO_right_accent] = 1;
		alphabet[UU_right_accent] = 1;
		alphabet[UU_2dot_accent] = 1;
		alphabet[NN_tild_accent] = 1;

		alphabet[a_right_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[i_right_accent] = 1;
		alphabet[o_right_accent] = 1;
		alphabet[u_right_accent] = 1;
		alphabet[u_2dot_accent] = 1;
		alphabet[n_tild_accent] = 1;
	}
	if (language == LANG_ITALIAN) {
		alphabet[AA_left_accent] = 1;
		alphabet[EE_left_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[II_left_accent] = 1;
		alphabet[II_right_accent] = 1;
		alphabet[II_roof_accent] = 1;
		alphabet[OO_left_accent] = 1;
		alphabet[OO_right_accent] = 1;
		alphabet[UU_left_accent] = 1;
		alphabet[UU_right_accent] = 1;

		alphabet[a_left_accent] = 1;
		alphabet[e_left_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[i_left_accent] = 1;
		alphabet[i_right_accent] = 1;
		alphabet[i_roof_accent] = 1;
		alphabet[o_left_accent] = 1;
		alphabet[o_right_accent] = 1;
		alphabet[u_left_accent] = 1;
		alphabet[u_right_accent] = 1;
	}
	if (language == LANG_DANISH) {
		alphabet[AE_cap_deaf_sound] = 1;
		alphabet[AA_circle_accent] = 1;
		alphabet[OO_crossed] = 1;

		alphabet[ae_deaf_sound] = 1;
		alphabet[a_circle_accent] = 1;
		alphabet[o_crossed] = 1;
	}
	if (language == LANG_PORTUGUESE) {
		alphabet[AA_left_accent] = 1;
		alphabet[AA_right_accent] = 1;
		alphabet[AA_tild_accent] = 1;
		alphabet[AA_roof_accent] = 1;
		alphabet[CC_bottom_accent] = 1;
		alphabet[EE_left_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[EE_2dot_accent] = 1;
		alphabet[EE_roof_accent] = 1;
		alphabet[OO_left_accent] = 1;
		alphabet[OO_right_accent] = 1;
		alphabet[OO_tild_accent] = 1;
		alphabet[OO_roof_accent] = 1;
		alphabet[II_right_accent] = 1;
		alphabet[II_2dot_accent] = 1;
		alphabet[UU_roof_accent] = 1;

		alphabet[a_left_accent] = 1;
		alphabet[a_right_accent] = 1;
		alphabet[a_tild_accent] = 1;
		alphabet[a_roof_accent] = 1;
		alphabet[c_bottom_accent] = 1;
		alphabet[e_left_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[e_2dot_accent] = 1;
		alphabet[e_roof_accent] = 1;
		alphabet[o_left_accent] = 1;
		alphabet[o_right_accent] = 1;
		alphabet[o_tild_accent] = 1;
		alphabet[o_roof_accent] = 1;
		alphabet[i_right_accent] = 1;
		alphabet[i_2dot_accent] = 1;
		alphabet[u_roof_accent] = 1;
	}
	if (language == LANG_DUTCH) {
		alphabet[AA_left_accent] = 1;
		alphabet[AA_2dot_accent] = 1;
		alphabet[AA_roof_accent] = 1;
		alphabet[CC_bottom_accent] = 1;
		alphabet[EE_left_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[EE_2dot_accent] = 1;
		alphabet[EE_roof_accent] = 1;
		alphabet[OO_2dot_accent] = 1;
		alphabet[NN_tild_accent] = 1;
		alphabet[II_roof_accent] = 1;
		alphabet[II_2dot_accent] = 1;
		alphabet[UU_roof_accent] = 1;
		alphabet[UU_2dot_accent] = 1;

		alphabet[a_left_accent] = 1;
		alphabet[a_2dot_accent] = 1;
		alphabet[a_roof_accent] = 1;
		alphabet[c_bottom_accent] = 1;
		alphabet[e_left_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[e_2dot_accent] = 1;
		alphabet[e_roof_accent] = 1;
		alphabet[o_2dot_accent] = 1;
		alphabet[n_tild_accent] = 1;
		alphabet[i_roof_accent] = 1;
		alphabet[i_2dot_accent] = 1;
		alphabet[u_roof_accent] = 1;
		alphabet[u_2dot_accent] = 1;
	}

	// В связи с новыми кодами лиг для cp1250 01.09.2000 E.P.
	if (is_cen_language(language)) {
		alphabet[liga_exm] = 1;
		alphabet[liga_inv_exm] = 1;
		alphabet[right_quocket] = 1;
		alphabet[liga_CC] = 1;
		alphabet[liga_CR] = 1;
	}

	if (language == LANG_CZECH) {
		alphabet[AA_right_accent] = alphabet[a_right_accent] = 1;
		alphabet[CC_inv_roof] = alphabet[c_inv_roof] = 1;
		alphabet[DD_inv_roof] = alphabet[d_inv_roof] = 1;
		alphabet[EE_right_accent] = alphabet[e_right_accent] = 1;
		alphabet[EE_inv_roof] = alphabet[e_inv_roof] = 1;
		alphabet[II_right_accent] = alphabet[i_right_accent] = 1;
		alphabet[NN_inv_roof] = alphabet[n_inv_roof] = 1;
		alphabet[OO_right_accent] = alphabet[o_right_accent] = 1;
		alphabet[RR_inv_roof] = alphabet[r_inv_roof] = 1;
		alphabet[SS_inv_roof_latin] = alphabet[s_inv_roof_latin] = 1;
		alphabet[TT_inv_roof] = alphabet[t_inv_roof] = 1;
		alphabet[UU_right_accent] = alphabet[u_right_accent] = 1;
		alphabet[UU_circle_accent] = alphabet[u_circle_accent] = 1;
		alphabet[YY_right_accent] = alphabet[y_right_accent] = 1;
		alphabet[ZZ_inv_roof_latin] = alphabet[z_inv_roof_latin] = 1;
	}

	if (language == LANG_ROMAN) {
		alphabet[AA_semicircle] = alphabet[a_semicircle] = 1;
		alphabet[AA_roof_accent] = alphabet[a_roof_accent] = 1;
		alphabet[II_roof_accent] = alphabet[i_roof_accent] = 1;
		alphabet[SS_bottom_accent] = alphabet[s_bottom_accent] = 1;
		alphabet[TT_bottom_accent] = alphabet[t_bottom_accent] = 1;
	}

	if (language == LANG_HUNGAR) {
		alphabet[AA_right_accent] = alphabet[a_right_accent] = 1;
		alphabet[EE_right_accent] = alphabet[e_right_accent] = 1;
		alphabet[II_right_accent] = alphabet[i_right_accent] = 1;
		alphabet[OO_right_accent] = alphabet[o_right_accent] = 1;
		alphabet[OO_2dot_accent] = alphabet[o_2dot_accent] = 1;
		alphabet[OO_double_right] = alphabet[o_double_right] = 1;
	}

	if (language == LANG_SLOVENIAN) {
		alphabet[CC_inv_roof] = alphabet[c_inv_roof] = 1;
		alphabet[SS_inv_roof_latin] = alphabet[s_inv_roof_latin] = 1;
		alphabet[ZZ_inv_roof_latin] = alphabet[z_inv_roof_latin] = 1;
	}

	// Прибалтийские лиги cp1257 09.07.2001 E.P.
	if (is_baltic_language(language)) {
		alphabet[low_quotes] = 1;
		alphabet[liga_exm] = 1;
		alphabet[liga_i] = 1;
		alphabet[right_quocket] = 1;
		alphabet[liga_TM] = 1;
		alphabet[liga_CC] = 1;
		alphabet[liga_CR] = 1;
		alphabet[liga_bull_baltic] = 1;
	}

	if (language == LANG_LATVIAN) {
		// LATVIAN 09.07.2001 E.P.
		alphabet[AA_macron] = 1;
		alphabet[a_macron] = 1;
		alphabet[CC_inv_roof] = 1;
		alphabet[c_inv_roof] = 1;
		alphabet[EE_macron] = 1;
		alphabet[e_macron] = 1;
		alphabet[GG_bottom_accent] = 1;
		alphabet[LATVIAN_g] = 1;
		alphabet[II_macron] = 1;
		alphabet[i_macron] = 1;
		alphabet[KK_bottom_accent] = 1;
		alphabet[k_bottom_accent] = 1;
		alphabet[LL_bottom_accent] = 1;
		alphabet[l_bottom_accent] = 1;
		alphabet[NN_bottom_accent] = 1;
		alphabet[n_bottom_accent] = 1;
		alphabet[SS_inv_roof_baltic] = 1;
		alphabet[s_inv_roof_baltic] = 1;
		alphabet[UU_macron] = 1;
		alphabet[u_macron] = 1;
		alphabet[ZZ_inv_roof_baltic] = 1;
		alphabet[z_inv_roof_baltic] = 1;

		alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
		alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
		alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;
		alphabet[(uchar) 'Y'] = alphabet[(uchar) 'y'] = 0;
	}

	if (language == LANG_LITHUANIAN) {
		// LITHUANIAN 09.07.2001 E.P.
		alphabet[AA_bottom_accent] = 1;
		alphabet[a_bottom_accent] = 1;
		alphabet[CC_inv_roof] = 1;
		alphabet[c_inv_roof] = 1;
		alphabet[EE_bottom_accent] = 1;
		alphabet[e_bottom_accent] = 1;
		alphabet[EE_dot_accent] = 1;
		alphabet[e_dot_accent] = 1;
		alphabet[II_bottom_accent] = 1;
		alphabet[i_bottom_accent] = 1;
		alphabet[SS_inv_roof_baltic] = 1;
		alphabet[s_inv_roof_baltic] = 1;
		alphabet[UU_bottom_accent] = 1;
		alphabet[u_bottom_accent] = 1;
		alphabet[UU_macron] = 1;
		alphabet[u_macron] = 1;
		alphabet[ZZ_inv_roof_baltic] = 1;
		alphabet[z_inv_roof_baltic] = 1;

		alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
		alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
		alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;
	}

	if (language == LANG_ESTONIAN) {
		// ESTONIAN 09.07.2001 E.P.
		alphabet[AA_2dot_accent] = 1;
		alphabet[a_2dot_accent] = 1;
		alphabet[OO_tild_accent] = 1;
		alphabet[o_tild_accent] = 1;
		alphabet[OO_2dot_accent] = 1;
		alphabet[o_2dot_accent] = 1;
		alphabet[SS_inv_roof_baltic] = 1;
		alphabet[s_inv_roof_baltic] = 1;
		alphabet[UU_2dot_accent] = 1;
		alphabet[u_2dot_accent] = 1;
		alphabet[ZZ_inv_roof_baltic] = 1;
		alphabet[z_inv_roof_baltic] = 1;

		alphabet[(uchar) 'C'] = alphabet[(uchar) 'c'] = 0;
		alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
		alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
		alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;
		alphabet[(uchar) 'Y'] = alphabet[(uchar) 'y'] = 0;

	}

	// Турецкий язык. 20.05.2002 E.P.
	if (language == LANG_TURKISH) {
		// Турецкие лиги, отличные от западноевропейских
		alphabet[liga_TM_turkish] = 1;
		alphabet[liga_CC_turkish] = 1;
		alphabet[liga_CR_turkish] = 1;
		alphabet[liga_bull_turkish] = 1;

		// Турецкие буквы, отличные от западноевропейских
		alphabet[GG_semicircle] = 1;
		alphabet[g_semicircle] = 1;
		alphabet[II_dot_accent] = 1;
		alphabet[i_sans_accent] = 1;
		alphabet[SS_bottom_accent_turkish] = 1;
		alphabet[s_bottom_accent_turkish] = 1;
		alphabet['Q'] = alphabet['q'] = 0;
		alphabet['W'] = alphabet['w'] = 0;
		alphabet['X'] = alphabet['x'] = 0;
	}

	return TRUE;
}

static const char *tabevn1[LANG_TOTAL] = { "rec1.dat", // LANG_ENGLISH		0
		"rec1.dat", // LANG_GERMAN		1
		"rec1.dat", // LANG_FRENCH		2
		"rec1rus.dat", // LANG_RUSSIAN		3
		"rec1.dat", // LANG_SWEDISH		4
		"rec1.dat", // LANG_SPANISH		5
		"rec1.dat", // LANG_ITALIAN		6
		"rec1r&e.dat", // LANG_RUSENG		7
		"rec1rus.dat", // LANG_UKRAINIAN	8
		"rec1rus.dat", // LANG_SERBIAN		9
		"rec1cen.dat", // LANG_CROATIAN	10
		"rec1cen.dat", // LANG_POLISH		11
		"rec1n.dat", // LANG_DANISH		12
		"rec1n.dat", // LANG_PORTUGUESE	13
		"rec1n.dat", // LANG_DUTCH		14
		"rec1.dat", // LANG_DIG			15
		"rec1uzb.dat", // LANG_UZBEK		16	// 01.09.2000 E.P.
		"rec1kaz.dat", // LANG_KAZ			17
		"rec1kaz.dat", // LANG_KAZ_ENG		18
		"rec1cen.dat", // LANG_CZECH		19
		"rec1cen.dat", // LANG_ROMAN		20
		"rec1cen.dat", // LANG_HUNGAR		21
		"rec1rus.dat", // LANG_BULGAR		22
		"rec1cen.dat", // LANG_SLOVENIAN	23
		"rec1blt.dat", // LANG_LATVIAN	    24
		"rec1blt.dat", // LANG_LITHUANIAN  25
		"rec1blt.dat", // LANG_ESTONIAN	26
		"rec1tur.dat" // LANG_TURKISH		27
		};

static const char *tabevn2[LANG_TOTAL] = { "rec2.dat", // LANG_ENGLISH		0
		"rec2.dat", // LANG_GERMAN		1
		"rec2.dat", // LANG_FRENCH		2
		"rec2rus.dat", // LANG_RUSSIAN		3
		"rec2.dat", // LANG_SWEDISH		4
		"rec2.dat", // LANG_SPANISH		5
		"rec2.dat", // LANG_ITALIAN		6
		"rec2r&e.dat", // LANG_RUSENG		7
		"rec2rus.dat", // LANG_UKRAINIAN	8
		"rec2rus.dat", // LANG_SERBIAN		9
		"rec2cen.dat", // LANG_CROATIAN	10
		"rec2cen.dat", // LANG_POLISH		11
		"rec2n.dat", // LANG_DANISH		12
		"rec2n.dat", // LANG_PORTUGUESE	13
		"rec2n.dat", // LANG_DUTCH		14
		"rec2.dat", // LANG_DIG			15
		"rec2uzb.dat", // LANG_UZBEK		16 // 01.09.2000 E.P.
		"rec2kaz.dat", // LANG_KAZ			17
		"rec2kaz.dat", // LANG_KAZ_ENG		18
		"rec2cen.dat", // LANG_CZECH		19
		"rec2cen.dat", // LANG_ROMAN		20
		"rec2cen.dat", // LANG_HUNGAR		21
		"rec2rus.dat", // LANG_BULGAR		22
		"rec2cen.dat", // LANG_SLOVENIAN	23
		"rec2blt.dat", // LANG_LATVIAN	    24
		"rec2blt.dat", // LANG_LITHUANIAN  25
		"rec2blt.dat", // LANG_ESTONIAN	26
		"rec2tur.dat" // LANG_TURKISH		27
		};

Bool16 rec_load_tables(uchar language) {
	if (language >= LANG_TOTAL)
		return FALSE;
	if (!EVNInitLanguage(tabevn1[language], tabevn2[language], language))
		return FALSE;
	return TRUE;
}

Bool16 rec_is_language(uchar language) {
	if (language < LANG_ENGLISH || language >= LANG_TOTAL)
		return FALSE;
	if (data_file_exists(tabevn1[language]) == -1)
		return FALSE;
	if (data_file_exists(tabevn2[language]) == -1)
		return FALSE;

	return TRUE;
}
