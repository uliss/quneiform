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

#include "context.h"
#include "linutil.h"
#include "func.h"
#include "tuner.h"
#include "status.h"
#include "linear.h"

int16_t final_crit_russian(cell * c);
int16_t russian_context_process(cell * c);

static int16_t help_spelling_checker(cell * c);

int16_t russian_context_process(cell * c) {
	int16_t return_code, nv;

	return_code = NO_ACTION;
	if (n_ltr == 1) /** single letter case: **/
	{
		nv = get_nvers(c, (uchar) '\x8e' /* О */);
		if (nv > 0 && memchr("0", c->vers[0].let, 1) && ((c->prev->flg
				& c_f_let) || (c->next->flg & c_f_let) || (c->next->nvers > 0
				&& memchr(".,", c->next->vers[0].let, 2)))) {
			vers_to_first_place(c, get_nvers(c, (uchar) '\x8e' /* О */)); // make it first
			sort_vers(c);
			return_code = CONTINUE_ACTION;
		}

		nv = get_nvers(c, (uchar) '3');
		//if (nv > 0 && memchr("зЗ", c->vers[0].let, 2)) {
		if (nv > 0 && memchr("\xA7\x87", c->vers[0].let, 2)) {
			vers_to_first_place(c, get_nvers(c, (uchar) '3')); // make it first
			sort_vers(c);
			return_code = CONTINUE_ACTION;
		}

		nv = get_nvers(c, (uchar) '\xa8' /* и */);
		if (((c->font | c->font_new) & c_fp_it) || (c->cg_flag & c_cg_comp))
			if (nv > 0 && get_nvers(c, (uchar) '\xef' /* я */) != 0) // Oleg : 12-07-1994 : '\x9f' /* Я */in first
			{
				if (c->vers[nv].prob > 120) {
					promote(0, c, (uchar) '\xa8' /* и */, 30);
					sort_vers(c);
					return_code = CONTINUE_ACTION;
				}
			//} else if (memchr("нп", c->vers[0].let, 2)) { //return return_code;
			} else if (memchr("\xAD\xAF", c->vers[0].let, 2)) { //return return_code;
				promote(0, c, (uchar) '\xa8' /* и */, 0); //  add vers
				vers_to_first_place(c, get_nvers(c, (uchar) '\xa8' /* и */)); // make it first
				sort_vers(c);
				return_code = CONTINUE_ACTION;
			}
	}

	else { // more then 1 letter in word
		cell * wc;
		int16_t i;
		int16_t num_dig;
		uchar let;
#ifdef INTERSEPTOR
		for( wc=c,num_dig=i=0; i < n_ltr;wc=wc->nextl,i++ ) // scan to word end
		//if( memchr("1234567890+-/ОбЗТВ.,",wc->vers[0].let,20) )
		if( memchr("1234567890+-/\x8E\xA1\x87\x92\x82.,",wc->vers[0].let,20) )
		num_dig++;
		if( num_dig==n_ltr )
		{ // russian word
			for( wc=c,num_dig=i=0; i < n_ltr;wc=wc->nextl,i++ ) // scan to word end
			//if( memchr("ОЗбВ",wc->vers[0].let,4) )
			if( memchr("\x8E\x87\xA1\x82",wc->vers[0].let,4) )
			{ // add russian versions
				switch( wc->vers[0].let )
				{
					case '\x82' /* В */: let=(uchar)'8'; break;
					case '\x8e' /* О */: let=(uchar)'0'; break;
					case '\x87' /* З */: let=(uchar)'3'; break;
					case '\xa1' /* б */: let=(uchar)'6'; break;
				}
				nv = get_nvers(wc, let);
				if( nv==-1 )
				{
					promote(0,wc,let,0); //  add vers
					vers_to_first_place ( wc,get_nvers(wc,let) ); // make it first
					sort_vers(wc);
				}
				else
				{
					vers_to_first_place ( wc,nv ); // make it first
					sort_vers(c);
				}
			}
		}
#endif
		for (wc = c, num_dig = i = 0; i < n_ltr; wc = wc->nextl, i++) // scan to word end
			//if (memchr("036ОЗб", wc->vers[0].let, 6))
			if (memchr("036\x8E\x87\xA1", wc->vers[0].let, 6))
				num_dig++;
		if (num_dig != n_ltr) { // russian word
			for (wc = c, num_dig = i = 0; i < n_ltr; wc = wc->nextl, i++) // scan to word end
				if (memchr("036", wc->vers[0].let, 3)) { // add russian versions
					switch (wc->vers[0].let) {
					case '0':
						let = (uchar) '\x8e' /* О */;
						break;
					case '3':
						let = (uchar) '\x87' /* З */;
						break;
					case '6':
						let = (uchar) '\xa1' /* б */;
						break;
					}
					nv = get_nvers(wc, let);
					if (nv == -1) {
						promote(0, wc, let, 0); //  add vers
						promote(0, wc, let, -48); // spell checker may correct this word
						sort_vers(wc);
					}
				}
		}
		if (n_ltr > 3)
			for (wc = c, i = 0; i < n_ltr; wc = wc->nextl, i++) // scan to word end
			{
				if (wc->nvers == 1 && wc->vers[0].let == (uchar) '\xe1' /* с */
						&& wc->vers[0].prob < 254 && !(wc->cg_flag & c_cg_cut)
						&& wc->recsource & c_rs_ev) {
					promote(0, wc, (uchar) '\xa5' /* е */, 0); //  add vers
					promote(0, wc, (uchar) '\xa5' /* е */, -48); // spell checker may correct this word
					sort_vers(wc);
					return_code = NO_ACTION; // Oleg : 12-07-1994 : go complex word
				}
				if (wc->vers[0].let == (uchar) '\xea' /* ъ */) {
					// Малый твердый знак в Болгарском основная буква.  08.09.2000 E.P.
					if (!langBul && i == n_ltr - 1 && wc->vers[0].prob > 150) // last letter in word
					{
						wc->vers[0].let = (uchar) '\xec' /* ь */;
						/*  else  wc->vers[0].prob=MAX(0,wc->vers[0].prob-20); // decrease prob
						 sort_vers(wc);*/
						return_code = NO_ACTION; // Oleg : 12-07-1994 : go complex word
					}
				}
			}
	}

	return return_code;
}

int16_t final_crit_russian(cell * c) {
	int16_t cnt = 0;

	cnt += help_spelling_checker(c);
	return cnt > 0;
}

//static uchar predessor[] = "оюесуэ";
static uchar predessor[] = "\xAE\xEE\xA5\xE1\xE3\xED";

int16_t help_spelling_checker(cell * c) {
	if (!(c->flg & (c_f_let | c_f_bad)))
		return 0;
	if (c->vers[0].let != (uchar) '\xa3' /* г */)
		return 0;

	if (memchr(predessor, c->prevl->vers[0].let, sizeof(predessor))
			&& (c->cg_flag & c_cg_cutl)) {
		if (c->vers[0].prob > 120) {
			cf::version * v0;
			for (v0 = c->vers; v0->let != 0; v0++)
				if (v0->let == (uchar) '\xe2' /* т */)
					return 0; // already exist - OK
			promote(0, c, (uchar) '\xe2' /* т */, 0); //  add vers
			promote(0, c, (uchar) '\xe2' /* т */, -48); // promotion
			c->vers[c->nvers].let = c->vers[c->nvers].prob = 0;
			return 1;
		}
	}
	return 0;
}

#define MAX_LEN_WORD  48

static Bool case_convert(cell *b, cell *e);
static uchar wave_up_abris(cell *b, cell *e);
static void make_convert(cell *b, cell *e, uchar r);

//static uchar capital_rus[] = "ЙЦКНГШЩЗХЪФЫВПОЛДЖЭЯЧСМИТЬЮ";
static uchar capital_rus[] = "\x89\x96\x8A\x8D\x83\x98\x99\x87\x95\x9A\x94\x9B\x82\x8F\x8E\x8B\x84\x86\x9D\x9F\x97\x91\x8C\x88\x92\x9C\x9E";
//static uchar small_rus[] = "йцкнгшщзхъфывполджэячсмитью";
static uchar small_rus[] = "\xA9\xE6\xAA\xAD\xA3\xE8\xE9\xA7\xE5\xEA\xE4\xEB\xA2\xAF\xAE\xAB\xA4\xA6\xED\xEF\xE7\xE1\xAC\xA8\xE2\xEC\xEE";
//
// TRY CONVERT CASE FOR ANY CELL
//
void correct_case(void) {
	cell *c, *e;
	uchar buf[MAX_LEN_WORD + 40], word_len;
	uchar wrd[MAX_LEN_WORD + 40];
	Bool r;

	if (db_status && snap_activity('c')) {
		snap_show_text("CASE : Begin case context");
		snap_monitor();
	}

	c = cell_f();
	while (1) {
		if ((c = next_word(c)) == NULL)
			break;
		if (db_status && snap_activity('c'))
			strcpy((char*) buf, "CASE : Word : ");
		e = end_word(c, wrd, &word_len, (uchar*) "");
		if (db_status && snap_activity('c'))
			strcpy((char*) &buf[13], (char*) wrd);

		r = case_convert(c, e);

		if (r && db_status && snap_activity('c')) {
			snap_newcell(c);
			snap_show_text((char*) buf);
			snap_monitor();
		}
		if (e == NULL)
			break;
		c = e;
	}

	if (db_status && snap_activity('c')) {
		snap_show_text("CASE : End case context");
		snap_monitor();
	}

	return;
}

void make_convert(cell *b, cell *e, uchar r) {
	cell *c;
	puchar p;

	if (r == 1) { // to lower
		for (c = b->next; c != e; c = c->next) {
			if ((c->flg & c_f_let) && c->nvers > 0)
				if ((p = (uchar*) memchr(capital_rus, c->vers[0].let,
						sizeof(capital_rus))) != NULL)
					c->vers[0].let = small_rus[p - capital_rus];
		}
	} else { // to upper
		if (b->nvers > 0 && (b->flg & c_f_let) && is_lower(b->vers[0].let)
				&& !memchr(small_rus, b->vers[0].let, sizeof(small_rus)))
			return;
		for (c = (is_lower(b->vers[0].let)) ? b : b->next; c != e; c = c->next) {
			if ((c->flg & c_f_let) && c->nvers > 0)
				if ((p = (uchar*) memchr(small_rus, c->vers[0].let,
						sizeof(small_rus))) != NULL)
					c->vers[0].let = capital_rus[p - small_rus];
		}
	}
}

uchar wave_up_abris(cell *b, cell *e) {
	cell *c;
	int16_t up, dn, n, up1, up2;

	if (Nb1 == -1 || Nb2 == -1 || Nb2 == 128 || Nb1 == 128)
		return 0;
	for (up1 = up2 = n = up = dn = 0, c = b; c != e; c = c->next, n++) {
		if ((c->flg & c_f_let) && c->nvers > 0) {
			if (memchr(small_rus, c->vers[0].let, sizeof(small_rus)))
				dn++;
			if (memchr(capital_rus, c->vers[0].let, sizeof(capital_rus)))
				up++;
			if (abs(minrow + c->bdiff + bbs1 - c->row) < 3)
				up1++;
			if (abs(minrow + c->bdiff + bbs2 - c->row) < 3)
				up2++;
		}
	}
	if (n < 6 || up == 0 || dn == 0 || up1 && up2)
		return 0;

	return up2 ? 1 : 2;
}

Bool case_convert(cell *b, cell *e) {
	uchar r;

	if (b->next == e || (b->next->flg & c_f_fict) && b->next->next == e)
		return FALSE; // one or two letters in word

	if ((r = wave_up_abris(b->next, e)) == 0)
		return FALSE; // non convertable word

	make_convert(b, e, r);

	return TRUE;
}

