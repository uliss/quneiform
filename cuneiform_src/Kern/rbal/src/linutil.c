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
#include <stdio.h>
#include <string.h>

#include "tuner.h"
#include "cstr.h"

#include "linear.h"
#include "linutil.h"
#include "ligas.h"      // Pit 10-10-94 03:56pm
#include "minmax.h"

/*============= Func prototypes ==================*/

uchar to_lower(uchar c);
uchar to_upper(uchar c);
int16_t is_lower(uchar ch);
int16_t is_upper(uchar ch);
int16_t isletter(uchar ch);
uchar get_homot(uchar ch);
int16_t twin(uchar ch);
int16_t count_line_hi(void);
int16_t draft_cut_hyps(int16_t bs, int16_t fl);

#define MAX_HEIGHT 70  // such was...  change ?
extern uchar language;

extern uchar db_status; // snap presence byte
extern uchar db_trace_flag; // snap-detail presence byte
extern uchar db_pass; // snap-pass indicator
extern uchar fEdCode; // Change code letter in module LINUTIL.C

/*============= Local function portotypes ==============*/

/*static*/int16_t h_hist(void);

/*============= Source code ============*/
Bool is_liga_ff(uchar c) {
	// Код лиги определен через макру в ligas.h 05.09.2000 E.P.
	return (c == liga_ff);
}
Bool is_liga_ffl(uchar c) {
	// Код лиги определен через макру в ligas.h 05.09.2000 E.P.
	return (c == liga_ffl);
}

int16_t is_english(uchar ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (
	//	   ch>=ligas_beg && ch<=ligas_end &&
			is_liga(ch) && // 14.09.2000 E.P.

					ch != liga_exm && ch != liga_qm);
}

int16_t is_serbian_special(uchar ch) {
	return (ch == SERB_j || ch == SERB_J || ch == SERB_n || ch == SERB_N || ch
			== SERB_l || ch == SERB_L || ch == SERB_h || ch == SERB_H || ch
			== SERB_hh || ch == SERB_HH || ch == SERB_u || ch == SERB_U);

}

int16_t is_polish_special(uchar ch) {
	return (ch == POLISH_SS || ch == POLISH_s || ch == POLISH_ZZD || ch
			== POLISH_zd || ch == POLISH_ZZR || ch == POLISH_zr || ch
			== POLISH_LL || ch == POLISH_l || ch == POLISH_AA || ch == POLISH_a
			|| ch == POLISH_CC || ch == POLISH_a || ch == POLISH_NN || ch
			== POLISH_n || ch == POLISH_EE || ch == POLISH_e || ch == POLISH_OO
			|| ch == POLISH_o);
}

int16_t is_czech_special(uchar let) {
	return (let == AA_right_accent || let == a_right_accent || let
			== CC_inv_roof || let == c_inv_roof || let == DD_inv_roof || let
			== d_inv_roof || let == EE_right_accent || let == e_right_accent
			|| let == EE_inv_roof || let == e_inv_roof || let
			== II_right_accent || let == i_right_accent || let == NN_inv_roof
			|| let == n_inv_roof || let == OO_right_accent || let
			== o_right_accent || let == RR_inv_roof || let == r_inv_roof || let
			== SS_inv_roof || let == s_inv_roof || let == TT_inv_roof || let
			== t_inv_roof || let == UU_right_accent || let == u_right_accent
			|| let == UU_circle_accent || let == u_circle_accent || let
			== YY_right_accent || let == y_right_accent || let == ZZ_inv_roof
			|| let == z_inv_roof);
}

int16_t is_roman_special(uchar let) {
	return (let == AA_semicircle || let == a_semicircle || let
			== AA_roof_accent || let == a_roof_accent || let == II_roof_accent
			|| let == i_roof_accent || let == SS_bottom_accent_latin || let
			== s_bottom_accent_latin || let == TT_bottom_accent || let
			== t_bottom_accent);
}

int16_t is_hungar_special(uchar let) {
	return (let == AA_right_accent || let == a_right_accent || let
			== EE_right_accent || let == e_right_accent || let
			== II_right_accent || let == i_right_accent || let
			== OO_right_accent || let == o_right_accent || let
			== OO_2dot_accent || let == o_2dot_accent || let == OO_double_right
			|| let == o_double_right);
}

int16_t is_lower(uchar ch) {

	if (language == LANG_RUSSIAN)
		switch (fEdCode) {
		case ED_ASCII: // for ASCII
			if ((ch >= (uchar) ' ' && ch <= (uchar) 'Ї') || (ch >= (uchar) 'а'
					&& ch <= (uchar) 'п') || memchr("рсхчшэА", ch, 7))
				return 1;
			break;
		case ED_WIN: // for Windows (ANSI)
			if (ch >= 0xE0 && ch <= 0xFF)
				return 1;
			break;
		case ED_MAC: // for Macintosh
			if (((ch >= 0xE0 && ch <= 0xFE) || ch == 0xDF))
				return 1;
			break;
		}
	if (ch >= 'a' && ch <= 'z')
		return 1;
	return 0;
}
int16_t is_upper(uchar ch) {
	if (language == LANG_RUSSIAN)
		switch (fEdCode) {
		case ED_ASCII:
		case ED_MAC: // for ASCII and Macintosh
			if (ch >= (uchar) 'Ђ' && ch <= (uchar) 'џ' || ch
					== (uchar) r_EE_2dot)
				return 1;
			break;
		case ED_WIN: // for Windows (ANSI)
			if (ch >= 0xC0 && ch <= 0xDF)
				return 1;
			break;
		}
	if (ch >= 'A' && ch <= 'Z')
		return 1;
	return 0;
}

Bool is_digit(uchar ch) {
	if (ch >= (uchar) '0' && ch <= (uchar) '9')
		return TRUE;
	else
		return FALSE;
}

int16_t isletter(uchar ch) {
	if (is_lower(ch) || is_upper(ch))
		return 1;
	else
		return 0;
}

uchar get_homot(uchar ch) {
	if (ch == '0')
		return ((uchar) '®');
	if (is_upper(ch))
		return to_lower(ch);
	if (is_lower(ch))
		return to_upper(ch);
	return ch;
}

static const uchar non_twin[] = " ЂЎЃҐ…";
static const uchar lat_twins[] = "cCoOpPsSvVwWxXzZ";

int16_t twin(uchar ch) {
	if (!isletter(ch))
		return 0;
	if (language == LANG_RUSSIAN)
		if (memchr(non_twin, ch, sizeof non_twin))
			return 0;
		else
			return 1;
	if (language != LANG_RUSSIAN && memchr(lat_twins, ch, sizeof lat_twins))
		return 1;
	return 0;
}

/* Function returns UPPER CASE variant of the letter.             */
uchar to_upper(uchar c) {
	if (c >= (uchar) 'a' && c <= (uchar) 'z')
		return c - (uchar) 'a' + (uchar) 'A';
	if (language == LANG_RUSSIAN)
		switch (fEdCode) {
		case ED_ASCII: // for ASCII
			if (c >= (uchar) ' ' && c <= (uchar) 'Ї')
				return c - (uchar) ' ' + (uchar) 'Ђ';
			if (c >= (uchar) 'а' && c <= (uchar) 'п')
				return c - (uchar) 'а' + (uchar) 'ђ';
			break;
		case ED_WIN: // for Windows (ANSI)
			if (c >= 0xE0 && c <= 0xFF)
				return c - 0xE0 + 0xC0;
			break;
		case ED_MAC: // for Macintosh
			if ((c >= 0xE0 && c <= 0xFE))
				return c - 0xE0 + 0x80;
			if (c == 0xDF)
				return 0x9F;
			break;
		}
	return c;
}

/* Function returns LOWER CASE variant of the letter.             */
uchar to_lower(uchar c) {
	if (c >= (uchar) 'A' && c <= (uchar) 'Z')
		return c - (uchar) 'A' + (uchar) 'a';
	if (language == LANG_RUSSIAN)
		switch (fEdCode) {
		case ED_ASCII: // for ASCII
			if (c >= (uchar) 'Ђ' && c <= (uchar) 'Џ')
				return c - (uchar) 'Ђ' + (uchar) ' ';
			if (c >= (uchar) 'ђ' && c <= (uchar) 'џ')
				return c - (uchar) 'ђ' + (uchar) 'а';
			break;
		case ED_WIN: // for Windows
			if (c >= 0xC0 && c <= 0xDF)
				return c - 0xC0 + 0xE0;
			break;
		case ED_MAC: // for Macintosh
			if (c >= 0x80 && c <= 0x9E)
				return c - 0x80 + 0xE0;
			if (c == 0x9F)
				return 0xDF;
			break;
		}
	return c;
}

static struct {
	int16_t ns1, ns2;
	uchar lcase, caps;
} lht[128];

#define v_bs1	1
#define v_bs2	2
#define v_bs3	4

// Nick 26.01.2001
// нужна инициализация для всех строк, т.к. в count_line_hi()
// не для всех строк можем попадать! И тогда статистика в этих строках
// останется от прежнего файла (или нечто вообще неизвестное)
void BaseLineStatisticInit(void) {
	int i;

	for (i = 0; i < arrnum(lht); i++)
		memset(&lht[i], 0, sizeof(lht[0])); //init
}
/////////////////
int16_t count_line_hi(void) {
	int16_t d23[MAX_HEIGHT] = { 0 }, d13[MAX_HEIGHT] = { 0 }, i, max, index,
			nbcaps = 0, nbsmall = 0;
	CSTR_rast c;
	CSTR_rast_attr attr;
	uchar ch;
	UniVersions vers;

	if (line_number >= arrnum(lht))
		return 0;
	memset(&lht[line_number], 0, sizeof(lht[0])); //init
	/* check validate of base 2 and base 3                       */
	if (!(ncletrs > 6 && ncletrs * 2 > ncbs)) // Presentable line ?
		return 0;// NO

	c = cell_f();
	//while((c=c->nextl)->nextl != NULL)
	while ((c = CSTR_GetNextRaster(c, CSTR_f_let)) != NULL) {
		CSTR_GetAttr(c, &attr);

		if (attr.h < 0 || attr.h >= MAX_HEIGHT)
			continue;

		CSTR_GetCollectionUni(c, &vers);

		if (vers.lnAltCnt <= 0 || vers.Alt[0].Prob < 140)
			continue;

		ch = let_lindef[vers.Alt[0].Liga];
		if ((ch & (v_bs1 | v_bs3)) == (v_bs1 | v_bs3)) {
			d13[attr.h]++;
			nbcaps++;
		}
		if ((ch & (v_bs2 | v_bs3)) == (v_bs2 | v_bs3)) {
			d23[attr.h]++;
			nbsmall++;
		}
	}

	// test - what we got
	for (i = 0, max = 0, index = 0; i < sizeof(d13) / sizeof(d13[0]); i++) {
		if (max < d13[i]) {
			max = d13[i];
			index = i;
		}
	}

	if (max > 0)
		lht[line_number].caps = (uchar) index;

	for (i = 0, max = 0, index = 0; i < sizeof(d23) / sizeof(d23[0]); i++) {
		if (max < d23[i]) {
			max = d23[i];
			index = i;
		}
	}

	if (max > 0)
		lht[line_number].lcase = (uchar) index;

	lht[line_number].ns1 = nbcaps;
	lht[line_number].ns2 = nbsmall;

	return (lht[line_number].caps != 0 || lht[line_number].lcase != 0); /* if there some information */
}

/* Return  most usable height in line */
int16_t h_hist(void) {
	CSTR_rast c;
	CSTR_rast_attr attr;
	int16_t h[MAX_HEIGHT] = { 0 }, th[MAX_HEIGHT] = { 0 }, i, max, index,
			recognized = 0, total = 0;
	uchar ch;
	UniVersions vers;

	c = cell_f();
	// while((c=c->nextl)->nextl != NULL)
	while ((c = CSTR_GetNextRaster(c, f_letter)) != NULL) {
		CSTR_GetAttr(c, &attr);
		if (attr.h < 0 || attr.h >= MAX_HEIGHT)
			continue;

		total++;
		th[attr.h]++; // all over

		CSTR_GetCollectionUni(c, &vers);

		//	if(!(c->flg & CSTR_f_let))continue;
		if (!(attr.flg & CSTR_f_let) || vers.lnAltCnt <= 0)
			continue;

		ch = let_lindef[vers.Alt[0].Liga];
		if ((ch & v_bs3) && ((ch & v_bs1) || (ch & v_bs2))) {
			recognized++;
			h[attr.h]++;
		}
	}

	if (recognized > 3 || recognized * 2 > total) {
		for (i = 0, max = 0, index = 0; i < sizeof(h) / sizeof(h[0]); i++) {
			if (max < h[i]) {
				max = h[i];
				index = i;
			}
		}

		max *= 2;
		max += h[index - 1] * 2;
		max += h[index + 1] * 2;
		max += h[index + 2];
		max += h[index - 2];
		max /= 2;

		if (recognized > 4) { // letters have the same height
			if (max * 4 >= recognized * 3)
				return index;
		} else
			return index;
	}

	else if (recognized < 4 && total > 4) {
		for (i = 0, max = 0, index = 0; i < sizeof(th) / sizeof(th[0]); i++) {
			if (max < th[i]) {
				max = th[i];
				index = i;
			}
		}

		max *= 2;
		max += th[index - 1] * 2;
		max += th[index + 1] * 2;
		max += th[index + 2];
		max += th[index - 2];
		max /= 2;
		if (max * 2 > total)
			return index; // letters have the same height
	}

	for (i = 0, max = 0, index = 0; i < sizeof(th) / sizeof(th[0]); i++) {
		if (max < th[i]) {
			max = th[i];
			index = i;
		}
	}

	if (max + th[index + 1] + th[index - 1] == total)
		return index;

	return 0;
}
/*---------------------------------------
 * Return:	0 - can't set case	*
 *		1 - base line 1		*
 *		2 - base line 2		*
 *--------------------------------------*/
uchar page_stat, stable_b3;

int16_t setup_let_case(int16_t ckH) {
	int16_t i, umax, lmax, lh, uprob, lprob, ret = 0;
	int16_t uc[MAX_HEIGHT] = { 0 }, lc[MAX_HEIGHT] = { 0 };

	// Nick 26.01.2000  - в таблицах могут быть очень большие номера строк!
	int lastLineNumber = MIN(sizeof(lht) / sizeof(lht[0]), line_number);
	int firLineNumber = 1; // у нас нет линии 0

	page_stat = 0;
	//if(line_number < 2) return 0;
	if (ckH)
		lh = ckH;
	else if ((lh = h_hist()) == 0)
		return 0;

	for (i = firLineNumber; i < line_number && i < sizeof(lht) / sizeof(lht[0]); i++) {
		uc[MIN(lht[i].caps, MAX_HEIGHT - 1)]++; /* Calc histogram */
		lc[MIN(lht[i].lcase, MAX_HEIGHT - 1)]++; // Nick 25.05.2001
	}

	umax = uc[lh] + uc[lh + 1] + uc[lh - 1] + uc[lh + 2] + uc[lh - 2];
	lmax = lc[lh] + lc[lh + 1] + lc[lh - 1];

	for (i = firLineNumber, lprob = 0; i < lastLineNumber; i++) // Nick 26.01.2001 - was line_number
		if (abs(lht[i].lcase - lh) < 3)
			lprob += lht[i].ns2;
	for (i = firLineNumber, uprob = 0; i < lastLineNumber; i++) // Nick 26.01.2001 - was line_number
	{
		if (abs(lht[i].caps - lh) < 3)
			uprob += lht[i].ns1;
	}

	if (lmax >= umax && lmax > 0) {
		ret = 2;
		if (!ckH)
			page_stat = 1;
	}
	if (umax > lmax && umax > 0) {
		ret = 1;
		if (!ckH)
			page_stat = 1;
	}
	if (db_status & snap_activity_rbal(db_pass) && ret != 0) {
		char buf[120];
		sprintf(buf, "Page statistic lh=%u  caps=%u,%u lcase=%u,%u ", lh, umax,
				uprob, lmax, lprob);
		snap_show_text_rbal(buf);
		snap_monitor_rbal();
	}
	return ret;
}

/********************************************************
 *                                                      *
 *      Func walk by string tries to discrim  some      *
 *      versions corresponding to base line bs          *
 *      Only for russian language.                      *
 ********************************************************/

int16_t draft_cut_hyps(int16_t bs, int16_t flag) {
	CSTR_rast c;
	CSTR_rast_attr attr;
	//version * v;
	UniVersions vers;
	int i;
	uchar ldef, let, gtwin = 0;
	int16_t dist, cutting_made = 0, diff;

	c = cell_f();

	if (flag) // add homotetia  versions
	{
		//  while ((c=c->nextl)->nextl)
		while ((c = CSTR_GetNextRaster(c, CSTR_f_let))) {
			CSTR_GetCollectionUni(c, &vers);
			let = vers.Alt[0].Liga;
			//   if(!(c->flg & CSTR_f_let) || !isletter(let))
			if (vers.lnAltCnt <= 0 || !isletter(let))
				continue;

			if (!twin(let))
				continue;
			let = is_lower(let) ? to_upper(let) : to_lower(let);
			promote(0, c, let, 0); // insvers

			//   c->vers[c->nvers].let=c->vers[c->nvers].prob=0;
		}
	}

	c = cell_f();
	if (bs == 3) // cut hyps by bb3
	{
		while ((c = CSTR_GetNextRaster(c, CSTR_f_let))) {
			//   if(!(c->flg & CSTR_f_let)) continue;
			CSTR_GetAttr(c, &attr);
			dist = 0;
			diff = 0;
			if (attr.bdiff != 127)
				diff = attr.bdiff;

			CSTR_GetCollectionUni(c, &vers);

			//   for (v=c->vers; (let=v->let) != 0; v++)
			for (i = 0; i < vers.lnAltCnt; i++) {
				dist = abs((attr.row + attr.h) - (minrow + bbs3 + diff)); // Is it far from bbs3
				let = vers.Alt[i].Liga;

				ldef = let_linpos[let] & 0x0f;
				if (memchr("Љџ†", let, 3))
					continue; // don't discrim
				if (memchr("ђа“г", let, 4))
					gtwin = 1;

				if (ldef == 2) // Is it sunk letter
				{
					if (dist < 3) { // Yes try to discrim
						//v->prob |= 1; // Exclude from statistic by base line detection
						vers.Alt[i].Prob |= 1;
						cutting_made = 1;
					}
				}
				if (ldef & 1 && ldef != 7) // Is it lay on bs3 don't touch ¤,ж,й
				{
					if (dist > 2) { // Yes try to discrim
						//v->prob |= 1;
						vers.Alt[i].Prob |= 1;
						cutting_made = 1;
					}
				}

			} // loop by hyps

			CSTR_StoreCollectionUni(c, &vers);

		} // loop by string
	} // end if bs == 3

	c = cell_f();
	if (bs == 2) {
		//  while ((c=c->nextl)->nextl)
		while ((c = CSTR_GetNextRaster(c, CSTR_f_let))) {
			// if(!(c->flg & CSTR_f_let)) continue;
			CSTR_GetAttr(c, &attr);
			CSTR_GetCollectionUni(c, &vers);

			//   for (v=c->vers; (let=v->let) != 0; v++)
			for (i = 0; i < vers.lnAltCnt; i++) {
				let = vers.Alt[i].Liga;

				ldef = let_linpos[let] >> 4;
				dist = (bbs3 - Ps) - (attr.row - minrow);

				if ((ldef & 2) == 2 && dist > 3) { // Is it letter lay on bb2
					//v->prob |= 1; // Exclude from statistic by base line detection
					vers.Alt[i].Prob |= 1; // Exclude from statistic by base line detection
					cutting_made = 1;
				}

				if (ldef & 1 && dist <= 3) // capital
				{
					//v->prob |= 1;
					vers.Alt[i].Prob |= 1; // Exclude from statistic by base line detection
					cutting_made = 1;
				}

			} // loop by hyps

			CSTR_StoreCollectionUni(c, &vers);

		} // loop by string
	}

	if (bs == 1) {
		while ((c = CSTR_GetNextRaster(c, CSTR_f_let))) {
			// if(!(c->flg & CSTR_f_let)) continue;
			CSTR_GetAttr(c, &attr);
			CSTR_GetCollectionUni(c, &vers);

			//   for (v=c->vers; (let=v->let) != 0; v++)
			for (i = 0; i < vers.lnAltCnt; i++) {
				let = vers.Alt[i].Liga;
				ldef = let_linpos[let] >> 4;
				dist = (bbs3 - Ps) - (attr.row - minrow);
				if (ldef & 1 && dist <= 3) // Is it lay near bb2
				{
					//v->prob |= 1; // Exclude from statistic by base line detection
					vers.Alt[i].Prob |= 1; // Exclude from statistic by base line detection
					cutting_made = 1;
				}
				if ((ldef & 2) == 2 && dist <= 3) // small
				{
					//v->prob |= 1;
					vers.Alt[i].Prob |= 1; // Exclude from statistic by base line detection
					cutting_made = 1;
				}

			} // loop by hyps

			CSTR_StoreCollectionUni(c, &vers);

		} // loop by string
	} // end if

	return (cutting_made || gtwin);
}
/////////////
extern uchar disable_twins;
extern int16_t it_done;

void set_rus_difflg(CSTR_rast B1, int16_t filter) {
	//CSTR_comp *env;
	CCOM_comp *env;
	CSTR_rast_attr attr;

	uchar let, tbe, tshp;
	uchar fl1, fl2, fl2t, flt, flnt, fl_cut, fl_retain;
	//  version *v0;
	UniVersions vers;
	int i;
	uchar cap_shape, solid, notwins;

	fl_cut = filter & f_cut;
	fl_retain = filter & f_retain;
	fl1 = 0xff; // all lines defined
	fl2 = fl2t = 0;
	solid = 0;
	notwins = disable_twins == 1 ? 1 : 0;
	flt = 0;
	flnt = 0; // 't' only

	//  env=B1->env;
	env = CSTR_GetComp(B1);

	cap_shape = 1; // capital in shape

	CSTR_GetAttr(B1, &attr);
	CSTR_GetCollectionUni(B1, &vers);

	//  for (v0=B1->vers; (let=v0->let) != 0; v0++)
	for (i = 0; i < vers.lnAltCnt; i++) {
		/*
		 if( v0->prob & 1){
		 v0->prob &= 0xfe; continue;  // clear cut flag
		 }
		 */
		let = vers.Alt[i].Liga;

		tbe = let_lindef[let];
		tshp = let_linshape[let];
		cap_shape &= tshp;

		// Nick - check ?
		solid += tbe < 32; // is it letter without twins
		flnt = 1;

		if (env->large & CCOM_LR_UNDERLINED) {
			if (tshp & 2) // sticky in shape
			{
				fl1 = 0;
				break;
			} // no lines defined
		}

		fl2t |= tbe; // accumulate b1/b2

		//if (v0->prob & 1)     // cutten version
		if (vers.Alt[i].Prob & 1) // cutten version
			continue;

		if (disable_twins == 2) {
			if (memchr("ђа“г", let, 4))
				notwins = 1;
		}

		fl2 |= tbe; // accumulate b1/b2

		if (tshp & 4) // stick allows to define bs1 ( iI1l )
		{
			if (oNb2 > 3) // base 2 defined
			{
				if (attr.h > (oPs + 3)) {
					tbe |= 1;
				} // allow to def bs1
			} else
				tbe &= 0xf4;

			// retain only b3 line for sticks while dusts not present
#ifdef UFA
			fl1=0xff; break; // stick can't define any bases
#endif
		}

		fl1 &= tbe;

		if (lin_pass == 3 &&
		//		v0 == B1->vers && B1->nvers > 1 &&
				i == 0 && vers.lnAltCnt > 1 &&
		//       (v0->prob - (v0+1)->prob) > 60 &&
				vers.Alt[i].Prob - vers.Alt[i + 1].Prob > 60 && !twin(let)
				&& (attr.recsource & CSTR_rs_BOX))
			break; //
	} // FOR versions

	if ((notwins || it_done) && fl1 != 0xff) {
		fl1 &= (uchar) ~c_df_twins;
		fl2 &= (uchar) ~c_df_twins;
	}

	attr.difflg &= fl_retain;

	if (fl1 != 0xff) // some bases defined
	{
		attr.basflg = flt; // agree to 't' type
		if (fl1 & c_df_round)
			attr.basflg |= CSTR_bs_round;
		if (cap_shape)
			attr.basflg |= CSTR_bs_cap;
		if (fl2 & c_df_twins) // a non_cutten version had a twin
		{
			if ((fl2t & (CSTR_db_b1 | CSTR_db_b2)) != (CSTR_db_b1 | CSTR_db_b2))
			//  b1 or b2  were absent in full list
			{
				fl1 &= ~(CSTR_db_b1 | CSTR_db_b2); // forget b1/b2 as defined
				if (attr.bas_acc & CSTR_ba_chance) // gleb
					attr.basflg |= (CSTR_bs_b1a | CSTR_bs_b2a); // agrees to be both b1. b2
			}
			if (disable_twins == 0 && fl1 & CSTR_db_b4)
				fl1 &= ~CSTR_db_b4;
		}
		if (fl2 & (CSTR_db_b1 | c_df_b1a))
			attr.basflg |= CSTR_bs_b1a;
		if (fl2 & (CSTR_db_b2 | c_df_b2a))
			attr.basflg |= CSTR_bs_b2a;
		attr.difflg = attr.difflg | (fl1 & 15);
		if (flnt == 0)
			attr.basflg |= CSTR_bs_t; // no "not a 't'" version
	}

	CSTR_SetAttr(B1, &attr);
}

///////////////
int16_t smart_diff(CSTR_rast c) {
	int16_t d1, d2, bm, row;
	CSTR_rast_attr attr;

	CSTR_GetAttr(c, &attr);

	row = attr.row - minrow;
	d1 = row - bbs1;
	d2 = row - bbs2;
	bm = (bbs3 + bbs2) / 2;

#ifdef UFA // Valdemar 12-05-94 08:25pm
	return attr.bdiff;
#endif

	if (attr.h >= Ps && (abs(d1) < 2 || abs(d2) < 2) && row + attr.h >= bbs3) {
		attr.bdiff = 0; // don't touch letter
		attr.difflg &= ~(CSTR_db_down | CSTR_db_up);

		CSTR_SetAttr(c, &attr);
	} // ж,й,¤

	if (attr.h >= Ps + 2) // large letter
	{
		if (abs(d1) < 2 || abs(d2) < 2) {
			attr.bdiff = 0; // don't touch letter
			attr.difflg &= ~(CSTR_db_down | CSTR_db_up);
			attr.difflg |= CSTR_db_forbid;
		} else { // letter d'not lay on base
			if (row < (bbs1 + bbs2) / 2)
				return 0; // farewell !
			if (row < bbs2) {
				if (row + attr.h < bbs3)
					return 0; // farewell !
				else if (attr.bdiff > 0) {
					attr.bdiff = 0; // don't touch letter
					attr.difflg &= ~(CSTR_db_down | CSTR_db_up);
				}
			}
		}

	}

	if (attr.h < Ps - 1 && (attr.flg & (CSTR_f_let | CSTR_f_bad)))
	// letter < Ps may be dust
	{
		if (abs(d1) < 2 || abs(d2) < 2) {
			attr.bdiff = 0; // don't touch letter
			attr.difflg &= ~(CSTR_db_down | CSTR_db_up);
			attr.difflg |= CSTR_db_forbid;
		}
	}

	CSTR_SetAttr(c, &attr);

	return attr.bdiff;
}

int16_t HIST_STATISTIC = 0;

extern int16_t hist_b3[];
extern int16_t hist_b3_f[];

int16_t GetPsFromHeights(void) {
	CSTR_rast c;
	CSTR_rast_attr attr;
	UniVersions vers;

	int16_t BPs, cDefs, i, max1, max2, ind1, ind2, peak2, h_size, b_top, n_sunk,
			b1_or_b2_rong;
	int16_t Hh[RASTER_MAX_HEIGHT * 2] = { 0 };

	HIST_STATISTIC = 0;

	cDefs = BPs = peak2 = n_sunk = 0;
	h_size = RASTER_MAX_HEIGHT * 2;

	if (Ns1 && Ns2)
		b1_or_b2_rong = (abs(bbs1 - bbs2) > 3) ? FALSE : TRUE;
	else
		b1_or_b2_rong = FALSE;

	c = cell_f();
	//  while( (c=c->nextl)->nextl )
	while (c = CSTR_GetNextRaster(c, f_letter)) {
		int16_t df, sn;

		CSTR_GetAttr(c, &attr);
		df = attr.bdiff;
		if (df == 127)
			df = 0;
		b_top = attr.row - minrow;
		sn = (b_top + attr.h) - (df + bbs3);
		if (abs(sn) > ((attr.h + 2) / 5 - 2)) // far from b3
		{
			if (sn > attr.h >> 2)
				n_sunk++; // sunk letter
			continue;
		}

		CSTR_GetCollectionUni(c, &vers);
		if (vers.lnAltCnt > 0 && BracketIn(&vers))
			continue; // bracket present in verses,
		// skip letter
		Hh[attr.h]++;
		cDefs++; // accumulate to histogramm
	}

	// find first peak
	for (i = max1 = ind1 = 0; i < h_size; i++) {
		if (max1 < Hh[i]) {
			max1 = Hh[i];
			ind1 = i;
		}
	}

	max1 *= 2;
	if (ind1 > 0)
		max1 += Hh[ind1 - 1] * 2;
	if (ind1 < h_size - 1)
		max1 += Hh[ind1 + 1] * 2;
	if (ind1 < h_size - 2)
		max1 += Hh[ind1 + 2];
	if (ind1 > 1)
		max1 += Hh[ind1 - 2];
	max1 /= 2;

	if (max1)
		ind1 = flood_peak(Hh, ind1);

	if (cDefs >> 1 > max1) {
		multi_bas = 4;
		return 0;
	} // flooded

	// second peak
	for (i = max2 = ind2 = 0; i < h_size; i++) {
		if (i >= ind1 - 4 && i <= ind1 + 4)
			continue;
		if (max2 < Hh[i]) {
			max2 = Hh[i];
			ind2 = i;
		}
	}

	max2 *= 2;

	if (ind2 > 0)
		max2 += Hh[ind2 - 1] * 2;

	if (ind2 < h_size - 1)
		max2 += Hh[ind2 + 1] * 2;

	if (ind2 < h_size - 2)
		max2 += Hh[ind2 + 2];

	if (ind2 > 1)
		max2 += Hh[ind2 - 2];

	max2 /= 2;

	if (max2)
		ind2 = flood_peak(Hh, ind2);

	if (max2 > 0 && (abs(ind1 - ind2) >= MIN(ind1, ind2) / 3))
		peak2 = 1;

	if (max2 > 3 && peak2) {
		BPs = MIN(ind1, ind2);
		bbs1 = bbs3 - MAX(ind1, ind2);
		goto ret;
	} // two peaks


	if (n_sunk > 2 && !b1_or_b2_rong) {
		int16_t TPs, ds;
		TPs = def_upper_side(); // check tops
		ds = (peak2 && ind2 < ind1) ? ind2 : ind1;
		if (TPs > 0 && abs(TPs - ds) < 3) {
			BPs = ds;
			goto ret;
		}
	}

	if (b1_or_b2_rong == FALSE) {
		if (Ns2 > 0 && Ns2 >= Ns1) {
			int16_t BS2 = bbs3 - bbs2;
			if (abs(ind1 - BS2) < 3) {
				BPs = ind1;
				goto ret;
			} else if (peak2 && abs(ind2 - BS2) < 3) {
				BPs = ind2;
				goto ret;
			}
		}
		if (Ns1 > 0) {
			int16_t BS1 = bbs3 - bbs1;
			if (abs(ind1 - BS1) < 3) {
				BPs = -ind1;
				goto ret;
			} else if (peak2 && abs(ind2 - BS1) < 3) {
				BPs = -ind2;
				goto ret;
			}
		}
	} // if b1 or b2 not rong

	if (peak2 && ind1 < ind2) {
		BPs = ind1;
		goto ret;
	}
	if (!peak2 && n_sunk > 0) {
		BPs = ind1;
		goto ret;
	}

	if ((i = setup_let_case(ind1))) {
		if (i == 1)
			BPs = -ind1;
		if (i == 2)
			BPs = ind1;
		goto ret;
	}

	if (peak2 && ind2 < ind1) {
		BPs = -ind1;
		goto ret;
	}

	// default :
	//           H > 30 : caps letters
	//           else   : small
	HIST_STATISTIC = 2;
	return (ind1 > 30) ? (-ind1) : ind1;

	ret: HIST_STATISTIC = 1;
	if (db_status & snap_activity_rbal(db_pass)) {
		char buf[120];
		sprintf(buf,
				"Histogramms: min=%d b3=%d peak1= %d|%d peak2= %d|%d sunk=%d",
				minrow, bbs3, ind1, max1, ind2, max2, n_sunk);
		snap_show_text_rbal(buf);
		snap_monitor_rbal();
	}
	return BPs;
}

////////////////

static uchar BracketsList[] = "<>()[]";

uchar BracketIn(UniVersions *v) {
	int i;

	for (i = 0; i < v->lnAltCnt; i++) {
		if (memchr(BracketsList, v->Alt[i].Liga, sizeof(BracketsList) - 1))
			return 1;
	}

	return 0;
}

////////////

int16_t def_upper_side(void) {
	CSTR_rast c;
	CSTR_rast_attr attr;
	int16_t i, max, max_sn, ind, ind_sn;
	uint16_t h_size, n_def, n_sn;

	int16_t h_top[RASTER_MAX_HEIGHT * 2] = { 0 }, h_sunk[RASTER_MAX_HEIGHT * 2] = {
			0 };

	h_size = RASTER_MAX_HEIGHT * 2;

	c = cell_f();
	n_def = n_sn = 0;
	//  while( (c=c->nextl)->nextl )
	while ((c = CSTR_GetNextRaster(c, CSTR_f_let))) {
		int16_t df, sn, b_top;

		//    if(!(c->flg & CSTR_f_let) ) continue;

		CSTR_GetAttr(c, &attr);

		df = attr.bdiff;
		if (df == 127)
			df = 0; // + diff
		b_top = attr.row - minrow;
		sn = (b_top + attr.h) - (df + bbs3);

		if (b_top - df >= RASTER_MAX_HEIGHT * 2)
			continue; // Oleg : for trash lines

		if (abs(sn) > ((attr.h + 2) / 5 - 2)) // far from b3
		{
			if (sn > attr.h >> 2) {
				h_sunk[b_top - df]++;
				n_sn++;
			}// sunk letter
			continue;
		}
		h_top[b_top - df]++;
		n_def++;
	}

	//  find peak tops
	for (i = max = ind = 0; i < h_size; i++) {
		if (max < h_top[i]) {
			max = h_top[i];
			ind = i;
		}
	}

	max *= 2;
	if (ind > 0)
		max += h_top[ind - 1] * 2;
	if (ind < h_size - 1)
		max += h_top[ind + 1] * 2;
	if (ind < h_size - 2)
		max += h_top[ind + 2];
	if (ind > 1)
		max += h_top[ind - 2];
	max /= 2;
	if (max)
		ind = flood_peak(h_top, ind);
	if (n_def >> 1 > max)
		return FALSE; // flooded tops

	// find peak tops of sunked
	for (i = max_sn = ind_sn = 0; i < h_size; i++) {
		if (max_sn < h_sunk[i]) {
			max_sn = h_sunk[i];
			ind_sn = i;
		}
	}

	max_sn *= 2;
	if (ind_sn > 0)
		max_sn += h_sunk[ind_sn - 1] * 2;
	if (ind_sn < h_size - 1)
		max_sn += h_sunk[ind_sn + 1] * 2;
	if (ind_sn < h_size - 2)
		max_sn += h_sunk[ind_sn + 2];
	if (ind_sn > 1)
		max_sn += h_sunk[ind_sn - 2];
	max_sn /= 2;

	if (max_sn)
		ind_sn = flood_peak(h_sunk, ind_sn);

	if (!max_sn || n_sn >> 1 > max_sn)
		return FALSE; // flooded sunk tops

	return (abs(ind - ind_sn) <= 3) ? bbs3 - ind : FALSE;
}
//////////////

void tell_for_b3(int16_t hist_array[]) {
	int16_t i;
	uint32_t NDisp, ON3;

	stable_b3 = FALSE;

	if (language != LANG_RUSSIAN)
		return;
	if (Ns3 < 5)
		return;

	NDisp = 0;
	bbs3 = (sbs3 + (Ns3 >> 1)) / Ns3;

	for (i = 0; i < RASTER_MAX_HEIGHT * 2; i++)
		NDisp += hist_array[i] * ((i - bbs3) * (i - bbs3));

	ON3 = (Ns3 - 1) * (Ns3 - 1);
	if (25* NDisp < ON3)
		stable_b3 = TRUE;
	return;
}

///////////////

int16_t flood_peak(int16_t *Hh, int16_t ind) {
	uint16_t Num, Den, t, t1;

	if (ind > 1)
		t = Hh[ind - 2] * (ind - 2);
	else
		t = 0;

	if (ind > 0)
		t1 = Hh[ind - 1] * (ind - 1);
	else
		t1 = 0;

	Num = Hh[ind] * ind + t1 + Hh[ind + 1] * (ind + 1) + t + Hh[ind + 2] * (ind
			+ 2);
	if (ind > 1)
		t = Hh[ind - 2];
	else
		t = 0;

	if (ind > 0)
		t1 = Hh[ind - 1];
	else
		t1 = 0;

	Den = Hh[ind] + t1 + Hh[ind + 1] + t + Hh[ind + 2];

	return ((2* Num + Den) / 2 / Den);
}

/////////
int16_t is_cen_bottom_accent(uchar c) {
	// Определение нижнего акцента 12.09.2000 E.P.
	return (

	c == AA_bottom_accent || c == a_bottom_accent || c == CC_bottom_accent || c
			== c_bottom_accent || c == EE_bottom_accent || c == e_bottom_accent
			|| c == SS_bottom_accent_latin || c == s_bottom_accent_latin || c
			== TT_bottom_accent || c == t_bottom_accent ||

	0);
}
////////////
