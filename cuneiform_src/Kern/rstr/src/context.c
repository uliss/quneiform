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

#include "compat_defs.h"
#define     MAIN_CONTEXT_MODULE
#include "context.h"
#include "tuner.h"
#include "status.h"

static void contextNumberTable(void);
/****************/

void context() {

	snap_newpass('c');
	memset(use_word_flags, 0, sizeof(pCell) * MAX_NUM_WORDS);
	test_O_and_slash();
	context_proc_line();

	// Nick 2.11.2000
	{
		extern uchar line_tabcell;
		if (line_tabcell)
			contextNumberTable();
	}

#ifndef DISABLE_SPACE
	cont_space();
#endif

	if (snap_activity('c')) {
		snap_show_text("Context: line finished");
		snap_monitor();
	}
}

int16_t russian_context_process(cell * C);

void context_proc_line() {
	//
	//	This procedure changes alternatives using context.
	//
	cell * C;
	int16_t lang_ret_code;

	pDotComCell = NULL;
	after_word_flag = 0;
	EB = C = cell_f(); // setup pointer to first (dummy) cell in string
	while ((n_ltr = find_and_class_word(EB)) != 0) {
		C = WB;
		total_words++;
		snap_newcell(C);
		if (snap_activity('c')) {
			snap_show_text("Context: word begins");
			snap_monitor();
		}
		switch (language) {
		case LANG_ENGLISH:
			lang_ret_code = english_context_process(C);
			break;
		case LANG_SWEDISH:
			lang_ret_code = swedish_context_process(C);
			break;
		case LANG_SPANISH:
			lang_ret_code = spanish_context_process(C);
			break;
		case LANG_FRENCH:
		case LANG_ITALIAN:
			lang_ret_code = franch_context_process(C);
			break;
		case LANG_GERMAN:
			lang_ret_code = german_context_process(C);
			break;
		case LANG_RUSSIAN:
			if (C->language == LANG_RUSSIAN) // Oleg : pass4 can used English cells
				lang_ret_code = russian_context_process(C);
			break;
		default:
			lang_ret_code = english_context_process(C);
		}
		if (lang_ret_code == CONTINUE_ACTION)
			continue;
		// Bool internet_word(void)
		{ //
			cell * c;
			char wrd[80], *p;
			int16_t i, n;
			for (c = WB, p = wrd, n = sizeof(wrd) - 1, i = 0; i < n && c
					!= EB->next; c = c->next) {
				*p++ = c->nvers ? c->vers[0].let : '~';
				i++;
			}
			*p = 0;
			if (language == LANG_ENGLISH && !strcmp("http", wrd)) {
				if (c->nvers && !(c->flg & c_f_fict) && strchr(":",
						c->vers[0].let) && c->next->nvers && !(c->next->flg
						& c_f_fict) && strchr("/", c->next->vers[0].let)
						&& c->next->next->nvers && !(c->next->next->flg
						& c_f_fict) && strchr("/", c->next->next->vers[0].let)) {
					c->flg |= c_f_solid;
					c->next->flg |= c_f_solid;
					c->next->flg |= c_f_solid;
					for (c = WB, p = wrd; c != EB->next; c = c->next) {
						c->flg |= c_f_solid;
					}
					continue;
				}
			}
		}
		if (C->nextl->vers[0].let != bad_char) {
			if ((language == LANG_ENGLISH) && (Lbs_word()))
				continue;
			if ((language == LANG_SWEDISH) && (swed_lju_flag == TRUE))
				continue;
			if (!p2_active) // Oleg
				Livers(C);
		}
#ifdef UFA
		{
			int16_t num=0,num_dig=0;
			cell *c;
			for(num=num_dig=0,c=WB;c!=EB;c=c->next)
			{
				if( !(c->flg&c_f_let) ) continue;
				num++;
				if( memchr("0123456789Ћ®‡§Ў",c->vers[0].let,15) ) num_dig++;
			}
			if( num==1 || num_dig!=num )
			if (word_flag & COMPLEX)
			complex_word(); // mixed alphanumeric or lower_upper type
		}
#else
		//if( !p2_active )
		if (word_flag & COMPLEX)
			complex_word(); // mixed alphanumeric or lower_upper type
#endif
		C = WB;
		if ((pDotComCell != NULL) && ((word_flag & AFTERDOTCOM) && (word_flag
				& AFTERDOT)) && check_lower(C->vers[0].let) && (!fl_cap_first))
			vers_to_first_place(pDotComCell, 1);
		pDotComCell = NULL;

	} /* end_of_while */
}

static int16_t find_and_class_word(cell *first_C)
//
//  This procedure scans string and finds next word after cell *C,
//   classifies this word by follows types:
//                  uint16_t, UPPER, LOWER, NUMBER or COMPLEX.
//  This procedure forms also 4 counters:
//               num_lower, num_upper, num_dig, num_bad
//   and sets the type of word in word_flag;
//	ON ENTRY: C - address of cell before word.
//	ON_EXIT : WB - address of first cell in word.
//		  returns number of letters in word.
//
{
	char fl_beg;
	uchar class, wf;
	cell *wc, *C;

	wf = after_word_flag;
	delim_flag = NOT_EXPECT_DELIM;
	possible_delim_flag = 0;
	wc = first_C;

	FCW_Init: word_flag = wf;
	fl_cap_first = fl_beg = fl_beg_irish_name = 0;
	word_flag |= (UINT | NUMBER | LOWER | UPPER); // all types initially allowed
	irish_name_pos = 0;
	n_ltr = num_let = num_dig = num_lower = 0;
	num_upper = num_bad = roman_sym_num = 0;
	after_word_flag = 0;
	C = wc;

	do {
		C = C->next;
		if (C->next == NULL)
			break; // reached end of string
		if (n_ltr > MAX_SYMB)
			break;

		class = classify(C);

		switch (class) {
		case BAD_SYM:
			num_bad++;
		case ALPHANUM:
			if (fl_beg == 0) {
				fl_beg = 1;
				WB = C;
			}
			n_ltr++;
			if (fl_beg_irish_name == AFTER_C)
				irish_name_pos = (uchar) n_ltr; // store first letter position
			//  of the actual irish name
			break;
		case SPEC_SYM_SLASH:
		case SPEC_SYM:
			if (fl_beg)
				goto Check_Delim;
			else
				goto Before_Word;
		case DOT_SPACE:
			if (fl_beg)
				goto FCW_Break;
			else
				goto Before_Word;

		default:
			continue;
		}

		check_vers(C);
		continue;

		Check_Delim: if (find_delim(C->vers[0].let)) {
			if (possible_delim_flag & DELIM_REGULAR)
				goto FCW_Break;
			if (delim_flag == NOT_EXPECT_DELIM)
				continue;
			FCW_Break: set_after_word_flag(C->vers[0].let);
			break;
		}
		Before_Word: set_word_flag(C->vers[0].let);
		continue;
	} while (C->next); // end of DO-loop

	set_word_type();
	if (delim_flag) {
		if (real_delim()) {
			delim_flag = 0;
			goto FCW_Init;
		}
	}
	EB = C->prev;
	return (n_ltr);
}

static uchar classify(cell *curc) {
	int16_t Let, nv;

	Let = curc->vers[0].let;
	if (curc->flg & c_f_bad)
		return BAD_SYM;
	if ((curc->flg & c_f_let) && check_alphanum((uchar) Let)) {
		test_roma_num_sym(curc);
		test_irish_name_cur_sym(Let);
		/* Oleg : 11-07-1994 : collection ('1','!') at the end of word */
		if (language == LANG_RUSSIAN && Let == '1' && curc->nvers == 2
				&& (curc->vers[1].let == '!' || curc->vers[1].let
						== (uchar) liga_exm)) {
			if (!(curc->prev->flg & c_f_fict) && !(curc->prev->nvers > 0
					&& memchr("0123456789%$-", curc->prev->vers[0].let, 13)))
				if (curc->vers[1].let == liga_exm)
					if ((curc->next->flg & c_f_fict) || ((curc->next->flg
							& c_f_space) && ((curc->next->vers[0].let
							== REGULAR_SPACE) || (curc->next->vers[0].let
							== POS_HALF_SPACE))) || ((curc->next->flg
							& c_f_punct) && ((curc->next->vers[0].let
							== right_quocket) || // '>>' for ger.l.
							(curc->next->vers[0].let == APOSTROPH)
							|| (curc->next->vers[0].let == '"')))
							|| ((curc->next->flg & c_f_let)
									&& curc->next->vers[0].let == right_quocket // '>>' for rus.
							)) {
						vers_to_first_place(curc, 1);
						return classify(curc);
					}
		} else
			return ALPHANUM;
	}

	if ((Let == '.') && (curc->next->flg & c_f_space)
			&& ((curc->next->vers[0].let == REGULAR_SPACE)
					|| (curc->next->vers[0].let == POS_HALF_SPACE))) {
		if ((curc->nvers > 1) && (curc->vers[1].let == ',')) {
			cell *c1, *c2;
			c1 = curc->prev;
			c2 = c1->prev;
			if (!((c1 != NULL) && (c2 != NULL) && (c1->flg & c_f_punct)
					&& (c2->flg & c_f_punct)))
				/** not dots ( ... ) case **/
				pDotComCell = curc;
		} else
			pDotComCell = NULL;
		return DOT_SPACE;
	}
	nv = get_nvers(curc, '/');
	if (nv >= 0) {
		if (curc->vers[0].prob - curc->vers[nv].prob <= REFUSE_BOUND)
			return SPEC_SYM_SLASH;
	}
	if (strchr("\x22\x27\x5c\x1f\x20?!.,-:;+", Let))
		/*****           "    '   \   +spc spc ********/
		return SPEC_SYM;

	if (Let == liga_exm) {
		if (curc->nvers == 1) // '!' is single version
			return SPEC_SYM;
		if (curc->next->flg & c_f_fict) // '!' is last symbol in line
			return SPEC_SYM;
		if ((curc->next->flg & c_f_space) && ((curc->next->vers[0].let
				== REGULAR_SPACE)
				|| (curc->next->vers[0].let == POS_HALF_SPACE)))
			return SPEC_SYM;
		if ((curc->next->flg & c_f_punct) && ((curc->next->vers[0].let == '.')
				|| (curc->next->vers[0].let == left_quocket) || // '<<' for ger.l.
				(curc->next->vers[0].let == right_quocket) || // '>>' for ger.l.
				(curc->next->vers[0].let == APOSTROPH)
				|| (curc->next->vers[0].let == '"')))
			return SPEC_SYM;
		if ((curc->next->flg & c_f_let) && (curc->next->vers[0].let == ')' || // Oleg & Vova
				curc->next->vers[0].let == left_quocket || // '<<' for rus.
				curc->next->vers[0].let == right_quocket // '>>' for rus.
				))
			return SPEC_SYM;
		if (language == LANG_RUSSIAN && // Oleg : 09-09-1994 :
				(curc->next->flg & c_f_let) && // word inhalt digital char
				curc->vers[1].let == '1')
			return SPEC_SYM;

		else {
			vers_to_first_place(curc, 1);
			return classify(curc);
		}

	}
	return 0;
}

static Bool find_delim(int16_t Let) {
	if ((strchr("?!:;+'\x22\x20\x1f\x2f\x5c", Let)) || (Let == liga_exm))
	/**********            "   spc +spc /  \  *********/
	{
		possible_delim_flag |= DELIM_REGULAR;
		return TRUE;
	}
	switch (Let) {
	case '.':
	case ',':
		if (possible_delim_flag & REAL_WORD_DELIM)
			return TRUE;
		possible_delim_flag |= DELIM_WORD_IS_POSSIBLE;
		if ((n_ltr) && (num_dig + num_bad < n_ltr))
			return TRUE;
		else
			return FALSE;
	case '-':
		if (possible_delim_flag & REAL_DIG_DELIM)
			return TRUE;
		possible_delim_flag |= DELIM_DIG_IS_POSSIBLE;
		if ((n_ltr) && (word_flag & NUMBER))
			return FALSE;
		else
			return TRUE;
	default:
		return FALSE;
	}
}

static void set_word_flag(int16_t Let) {
	if ((strchr("!?.", Let)) || (Let == liga_exm))
		word_flag |= AFTERDOT;
	if (pDotComCell != NULL)
		word_flag |= AFTERDOTCOM;
	if (Let == APOSTROPH)
		word_flag |= AFTERAPOSTROPHE;
}

static void set_after_word_flag(int16_t Let) {
	if ((strchr("!?.", Let)) || (Let == liga_exm))
		after_word_flag |= AFTERDOT;
	if (pDotComCell != NULL)
		word_flag |= AFTERDOTCOM;
	if (Let == APOSTROPH)
		after_word_flag |= AFTERAPOSTROPHE;
}
/******************************************/
static void check_vers(cell *curc) //AK 04.07.97
{
	int16_t i = 0, Let = 0, fl = 0;

	fl = 0;
	for (i = 0; i < curc->nvers; i++) {
		if (fl == (UPPER | LOWER | NUMBER))
			break;
		if (curc->vers[i].prob < 100)
			break;
		Let = curc->vers[i].let;

		if (check_lower((uchar) Let)) {
			fl |= LOWER;
			continue;
		}
		if (check_upper((uchar) Let)) {
			fl |= UPPER;
			continue;
		}
		if (check_numb_0_9((uchar) Let)) {
			fl |= NUMBER;
			continue;
		}
	}
	if ((fl & UPPER) && (n_ltr == 1))
		fl_cap_first = 1;
	if (fl & LOWER)
		num_lower++;
	if (fl & UPPER)
		num_upper++;
	if (fl & NUMBER)
		num_dig++;
	if ((fl & (LOWER | UPPER)) && (fl & NUMBER == 0))
		num_let++;
	if ((curc->nvers == 1) && (check_numb_2_9((uchar) Let)))
		word_flag &= ~UINT;
	if ((fl & NUMBER) == 0)
		word_flag &= ~NUMBER;
	if (((fl & LOWER) == 0) && (n_ltr > 1))
		word_flag &= ~LOWER;
	if ((fl & UPPER) == 0)
		word_flag &= ~UPPER;
}
/******************************************/
static void set_word_type() {
	if ((num_dig > 0) && (num_lower + num_upper > 0))
		word_flag |= COMPLEX;
	if ((num_lower > 0) && (num_upper > 0))
		word_flag |= COMPLEX;
}

static Bool real_delim() {
	if (possible_delim_flag & DELIM_WORD_IS_POSSIBLE) {
		if ((n_ltr) && (num_dig + num_bad < n_ltr)) {
			if (num_let + num_bad < n_ltr) // mixed word
				return FALSE;
			possible_delim_flag |= REAL_WORD_DELIM;
			return TRUE;
		} else
			return FALSE;
	}
	if (possible_delim_flag & DELIM_DIG_IS_POSSIBLE) {
		if ((n_ltr) && (word_flag & NUMBER))
			return FALSE;
		else {
			possible_delim_flag |= REAL_DIG_DELIM;
			return TRUE;
		}
	}
	return TRUE;
}

static void Livers(cell *NC)
//
//	This procedure gives bonus or monus to the first letter
//	'i' or 'l' in accordance to second letter
//	(is it consonant or vowel).
//
{
	int16_t i, j, kc, kv, kt;
	cell * W;
	version *v, *w;

	i = 0;
	W = use_word_flags[0];
	while ((W != NULL) && (i < MAX_NUM_WORDS)) {
		if (W == NC)
			return;
		W = use_word_flags[i++];
	}
	use_word_flags[i] = NC;
	W = NC->nextl;
	kc = kv = kt = 0;
	for (j = 0, w = W->vers; j < W->nvers; j++, w++) {
		if ((w->prob < 140) || (W->vers[0].prob - w->prob > REFUSE_BOUND))
			break;
		kt++;
		if (check_lowerconson(w->let) || check_lowerconson(
				(uchar)(w->let + 0x20)))
			kc++;
		else
			kv++;
	}
	if (kt == 0)
		return;
	for (i = 0, v = NC->vers; i < NC->nvers; i++, v++) {
		if ((kc == kt) && (v->let == 'l'))
			v->prob = MAX(v->prob - MONUS, 10);
		if (kv == kt) {
			if ((v->let == 'i') || (v->let == liga_i) || language
					== LANG_TURKISH && // 30.05.2002 E.P.
					(v->let == i_sans_accent || v->let == II_dot_accent))
				v->prob = MAX(v->prob - MONUS, 10);
			if (v->let == 'I') {
				if ((kt == 1) && (check_upper(W->vers[0].let)) && (word_flag
						& UPPER))
					continue;
				else
					v->prob = MAX(v->prob - MONUS, 10);
			}
		}
	}
	sort_vers(NC);
}

#ifdef UNDEFINED
static void negspace()
//
//	This procedure substitutes negative half space before
//	capital letter for positive one.
//
{
	cell *C,*D,*E;
	version *v;
	char fl;
	int16_t i;

	C=cell_f();
	while ((C=C->nextl)->nextl)
	{
		fl=0;
		v=C->vers;
		for (i=0; i < C->nvers; i++,v++)
		if (check_upper(v->let))
		fl++;
		if (fl < C->nvers)
		continue; // not all version are capital letters
		if (C->prev->prev)
		{
			D=C->prev;
			if (D->flg & c_f_space)
			{
				v=D->vers;
				if (v->let == NEG_HALF_SPACE) // negative half space ?
				{
					E=D->prev;
					if ((E->flg & c_f_let) && (check_lower(E->vers[0].let)))
					{

						D->vers[0].let=POS_HALF_SPACE; // negative space -> positive
						D->vers[0].prob=150;
					}
				}
			}
		}
	}
}
#endif

static Bool roma_num() {

	if (n_ltr != roman_sym_num)
		return FALSE;
	else {
		make_roma_num();
		return TRUE;
	}
}

void complex_word() {
	cell * C;
	int16_t i, j, est_dc;

	if (n_ltr > MAX_SYMB)
		return;
	if (roma_num())
		return;
#ifdef COMPLEX_PROT
	printf("%3d  ",line_number+1);
	for (C=WB; C->prev != EB; C=C->next)
	{
		if ((C->flg & c_f_let) == 0) continue;
		printf ("%c",C->vers[0].let);
	}
#endif
	snap_newcell(WB);
	if (snap_activity('c')) {
		snap_show_text("Complex word");
		snap_monitor();
	}
	setup_complex();
	C = WB;
	estimate_letter(C, 0);
	cdv += COMP_NVAR; // estimate first letter
	for (i = 1; C != EB;) {
		C = C->next;
		if ((C->flg & c_f_let) == 0)
			continue;
		estimate_letter(C, i);
		estimate_chain(i);
		i++;
		cdv += COMP_NVAR;
	}
	if (i == 1)
		return;
	j = best_estimation();
	best_l = j;
	num_v = i;
	last_dv = cdv;
	if ((i >= 3) && (language == LANG_ENGLISH)) {
		est_dc = find_dig_chr_var();
		if (est_dc > 0) {
			est_dc += BON_DIG_CHAR;
			if (est_dc < (last_dv + best_l)->est)
				goto NoNumeric;
			make_dig_chr_chain();
			goto CW_Ret;
		}
	}
	NoNumeric: for (; i > 0; i--, cdv -= COMP_NVAR) {
		j = make_best_choice(j);
	}

#ifdef COMPLEX_PROT
	printf(" ");
	for (C=WB; C->prev != EB; C=C->next)
	{
		if ((C->flg & c_f_let) == 0) continue;
		printf ("%c",C->vers[0].let);
	}
	printf("\n");
#endif
	CW_Ret: return;
}

static int16_t best_estimation() {
	int16_t i, mest, n;

	cdv -= COMP_NVAR;
	mest = cdv->est;
	n = 0;
	for (i = 1; i < COMP_NVAR; i++)
		if ((cdv + i)->est > mest) {
			mest = (cdv + i)->est;
			n = i;
		}
	return n;
}

static void estimate_chain(int16_t word_pos) {
	dv * prev, *curr;
	int16_t cn, pn;
	int16_t bn, mest, est;
	cell *c;
	uchar first, fr;

	for (cn = 0, curr = cdv; cn < COMP_NVAR; cn++, curr++) {
		mest = -32000;
		bn = 0;
		fr = first = L_EMPTY;
		if (curr->class == L_EMPTY)
			goto emp;
		for (pn = 0, prev = cdv - COMP_NVAR; pn < COMP_NVAR; pn++, prev++) {
			if (prev->class == L_EMPTY)
				continue;
			est = prev->est + curr->est;
			fr = prev->first;

			if ((curr->class != prev->class) && (curr->class != L_UNDEF)
					&& (prev->class != L_UNDEF)) {
				est -= JMP;
				if (fr != L_EMPTY) {
					c = (curr->C) -> prev;
					if ((c->flg & c_f_punct) == 0 || c->vers[0].let
							== APOSTROPH)
						est -= JMP;
					fr = L_EMPTY;
				}
			}
			if (((curr->class == L_SMALL) && (prev->class == L_NUM))
					|| ((prev->class == L_SMALL) && (curr->class == L_NUM)))
				est -= JMP_SN;
			if (((curr->class == L_SMALL) && (prev->class == L_NUM))
					&& (curr->C->nvers == 1) && (word_pos == 1))
				est -= ADD_JMP_SN;
			// testing against irish name  01-06-94 03:06pm  
			// note: irish_name_pos value is computed from 1;
			if (((word_pos == 1) || ((irish_name_pos) && (irish_name_pos
					== word_pos))) && (curr->class == L_SMALL) && (prev->class
					== L_BIG)) {
				est += JMP;
				fr = L_BIG;
			}
			if (((irish_name_pos) && (irish_name_pos - 1 == word_pos))
					&& (curr->class == L_BIG) && (prev->class == L_SMALL)) {
				est += JMP;
			}
			if (((irish_name_pos) && (irish_name_pos == word_pos))
					&& (curr->class == L_BIG) && (prev->class == L_BIG)) {
				est -= JMP;
			}
			// testing against irish name  01-06-94 03:06pm  
			if (est > mest) {
				mest = est;
				bn = pn;
				first = fr;
			}
		}
		emp: curr->back = bn;
		curr->est = mest;
		curr->first = first;
	}
}

static void estimate_letter(cell *C, int16_t pos) {
	version *v;
	int16_t i, p, pm;

	cdv->class = (cdv + 1)->class = (cdv + 2)->class = L_EMPTY;
	cdv->first = (cdv + 1)->first = (cdv + 2)->first = L_EMPTY;
	cdv->C = (cdv + 1)->C = (cdv + 2)->C = C;
	v = C->vers;
	pm = v->prob;
	i = 0;
	while (v->let) {
		p = v->prob;
		if (pm - p > REFUSE_BOUND)
			break;
		switch (class_of_letter(v->let)) {
		case L_BIG:
			if ((cdv + L_BIG)->class == L_EMPTY) {
				(cdv + L_BIG)->class = L_BIG;
				(cdv + L_BIG)->pos = (uchar) i;
				(cdv + L_BIG)->est = p - i;
			}
			break;
		case L_SMALL:
			if ((cdv + L_SMALL)->class == L_EMPTY) {
				(cdv + L_SMALL)->class = L_SMALL;
				(cdv + L_SMALL)->pos = (uchar) i;
				(cdv + L_SMALL)->est = p - i;
				if (pos != 0)
					(cdv + L_SMALL)->est += SMALL_INC;
				if ((pos == 0) && (word_flag & AFTERDOT))
					(cdv + L_SMALL)->est -= TITLE_JMP;
			}
			break;
		case L_NUM:
			if ((cdv + L_NUM)->class == L_EMPTY) {
				(cdv + L_NUM)->class = L_NUM;
				(cdv + L_NUM)->pos = (uchar) i;
				(cdv + L_NUM)->est = p + NUM_INC - i;
			}
			break;
		case L_UNDEF:
			if (v->let == liga_exm) // skip '!' with glued dot
				break;
			for (pm = 0; pm < 3; pm++)
				if ((cdv + pm)->class == L_EMPTY) {
					(cdv + pm)->class = L_UNDEF;
					(cdv + pm)->pos = (uchar) i;
					(cdv + pm)->est = p - i;
				}
			goto stloop;
		}
		v++;
		i++;
	}
	stloop: ;
}

static int16_t class_of_letter(char c) {
	if (check_upper(c))
		return L_BIG;
	if (check_lower(c))
		return L_SMALL;
	if (check_numb_0_9(c))
		return L_NUM;
	return L_UNDEF;
}

static void setup_complex() {

	beg_dv = (dv *) il1_pool_ptr();
	cdv = beg_dv;
}

static int16_t make_best_choice(int16_t num_var) {
	version *v, wi;
	int16_t n;

	v = (cdv + num_var)->C->vers;
	n = (cdv + num_var)->pos;
	v += n;
	if (n == 0)
		goto Ret;
	while (n-- > 0) // 19.07.2001 E.P.
	{
		memcpy(&wi, v, sizeof(*v));
		memcpy(v, v - 1, sizeof(*v));
		memcpy(v - 1, &wi, sizeof(*v));
		v--;
	}
	v->prob = (v + 1)->prob; // make first version equal to former first
	Ret: if ((cdv + num_var)->C->flg & c_f_solid)
		(cdv + num_var)->C->flg -= c_f_solid;
	return ((cdv + num_var)->back);
}

static int16_t find_dig_chr_var() {
	int16_t curest, i, wc;
	int16_t rp;

	maxest = 0;
	admvar = 0;
	// test variant "1st"

	rp = get_est_substr_in_word("11st");
	if (rp > 0)
		goto FD_Ret;

	rp = test_var("1st");
	if (rp > maxest) {
		maxest = rp;
		admvar = 1;
	}

	//FindVar2:
	rp = test_var("2nd");
	if (rp > maxest) {
		maxest = rp;
		admvar = 2;
	}

	//FindVar3:
	rp = test_var("3rd");
	if (rp > maxest) {
		maxest = rp;
		admvar = 3;
	}

	//FindVar4_7:
	curest = 0;
	rp = get_est_substr_in_word("th");
	if (rp < 0)
		goto FD_Ret;
	curest += rp;
	if (last_dig_dv == NULL)
		goto FD_Ret;
	if ((last_dig_dv + L_NUM)->class != L_NUM)
		goto FD_Ret;
	i = (last_dig_dv + L_NUM)->pos;
	wc = (last_dig_dv + L_NUM)->C->vers[i].let;
	if (strchr("0456789", wc) == NULL)
		goto FindVar4;
	rp = est_rest_dig(last_dig_dv);
	if (rp < 0)
		goto FindVar4;
	curest += rp;
	if (curest > maxest) {
		maxest = curest;
		admvar = 7;
	}
	FindVar4: rp = test_var("11th");
	if (rp > maxest) {
		maxest = rp;
		admvar = 4;
	}
	//FindVar5:
	rp = test_var("12th");
	if (rp > maxest) {
		maxest = rp;
		admvar = 5;
	}

	//FindVar6:
	rp = test_var("13th");
	if (rp > maxest) {
		maxest = rp;
		admvar = 6;
	}

	FD_Ret: return maxest;
}

static void make_dig_chr_chain() {

	switch (admvar) {
	case 1:
		up_substr("1st");
		break;
	case 2:
		up_substr("2nd");
		break;
	case 3:
		up_substr("3rd");
		break;
	case 4:
		up_substr("11th");
		break;
	case 5:
		up_substr("12th");
		break;
	case 6:
		up_substr("13th");
		break;
	case 7:
		up_substr("th");
		break;
	default:
		return;
	}
	up_rest_str();
}

static void up_substr(pchar substr) {
	int16_t wlet, n, i, shift;
	dv *cv;
	cell *wc;

	n = strlen(substr) - 1;
	shift = best_l;

	for (cv = last_dv; n >= 0; n--, cv -= COMP_NVAR) {
		wc = (cv + shift) -> C;
		wlet = substr[n];
		i = get_nvers(wc, wlet);
		vers_to_first_place(wc, i);
		shift = (cv + shift) -> back;
	}
	if (cv < beg_dv)
		last_dig_dv = NULL;
	else
		last_dig_dv = cv;
	return;
}

static void up_rest_str() {
	cell *wc;
	dv *cv;
	int16_t wlet, i;

	if (last_dig_dv == NULL)
		return;
	for (cv = last_dig_dv; cv >= beg_dv; cv -= COMP_NVAR) {
		wc = (cv + L_NUM) ->C;
		i = (cv + L_NUM) ->pos;
		wlet = wc->vers[i].let;
		vers_to_first_place(wc, i);
	}
}

static int16_t get_est_substr_in_word(pchar str) {
	dv *cv;
	int16_t n, m, sump, lstr, shift;

	sump = 0;
	lstr = strlen(str);
	shift = best_l;

	if (num_v < lstr)
		goto NoSubstr;

	cv = last_dv;
	for (n = lstr - 1; n >= 0; n--) {
		for (m = 0; m < (cv + shift)->C->nvers; m++) {
			if ((cv + shift)->C->vers[m].let == str[n]) {
				sump += (cv + shift)->C->vers[m].prob;
				goto Cont;
			}
		}
		NoSubstr: sump = -1;
		goto Ret;
		Cont: shift = (cv + shift)->back;
		cv = cv - COMP_NVAR;
	}
	if (cv < beg_dv)
		last_dig_dv = NULL;
	else
		last_dig_dv = cv;
	Ret: return sump;
}

static int16_t est_rest_dig(dv *from) {
	dv *cv;
	int16_t i, ret_est;

	ret_est = 0;
	for (cv = from; cv >= beg_dv; cv -= COMP_NVAR) {
		if ((cv + L_NUM)->class == L_NUM) {
			i = (cv + L_NUM)->pos;
			ret_est += (cv + L_NUM)->C->vers[i].prob;
		} /*end-if*/
		else {
			ret_est = -1;
			goto ERD_Ret;
		}
	} /*end-for*/
	ERD_Ret: return ret_est;
}

static int16_t test_var(pchar str_var) {
	int16_t rc, curest;

	curest = 0;
	last_dig_dv = NULL;
	rc = get_est_substr_in_word(str_var);
	if (rc < 0) {
		curest = 0;
		goto EV_Ret;
	}
	curest += rc;
	if (last_dig_dv == NULL)
		goto EV_Ret;
	rc = est_rest_dig(last_dig_dv);
	if (rc < 0) {
		curest = 0;
		goto EV_Ret;
	}
	curest += rc;
	EV_Ret: return curest;
}

static Bool Lbs_word() {
	Bool ret_code;
	int16_t pos_l, pos_b, pos_s, pos_dot;
	cell *c_l, *c_b, *c_s, *c_dot;

	c_l = c_b = c_s = c_dot = NULL;
	ret_code = FALSE;
	c_l = WB;

	if (c_l > EB)
		goto LW_Ret;
	pos_l = get_nvers(c_l, 'l');
	if (pos_l < 0)
		goto LW_Ret;
	if (c_l->vers[pos_l].prob < ADM_PROB)
		goto LW_Ret;

	c_b = c_l->next;
	if (c_b > EB)
		goto LW_Ret;
	pos_b = get_nvers(c_b, 'b');
	if (pos_b < 0)
		goto LW_Ret;
	if (c_b->vers[pos_b].prob < ADM_PROB)
		goto LW_Ret;

	if (c_b->next->flg & c_f_punct) {
		c_dot = c_b->next;
		goto LW_Dot;
	}
	c_s = c_b->next;
	if (c_s > EB)
		goto LW_Ret;
	pos_s = get_nvers(c_s, 's');
	if (pos_s < 0)
		goto LW_Ret;
	if (c_s->vers[pos_s].prob < ADM_PROB)
		goto LW_Ret;

	c_dot = c_s->next;
	LW_Dot: if ((c_dot->flg & c_f_punct) == 0)
		goto LW_Ret;
	pos_dot = get_nvers(c_dot, '.');
	if (pos_dot < 0)
		goto LW_Ret;

	vers_to_first_place(c_l, pos_l);
	vers_to_first_place(c_b, pos_b);
	vers_to_first_place(c_dot, pos_dot);

	if (c_s)
		vers_to_first_place(c_s, pos_s);
	ret_code = TRUE;
	LW_Ret: return ret_code;
}

static void make_roma_num() {
	cell *wc;
	int16_t i, j;

	for (wc = WB, i = 1; ((i <= n_ltr) && (wc->prev != EB)); i++, wc
			= wc->nextl) {
		for (j = 0; j < wc->nvers; j++) {
			if (strchr("XVILC", wc->vers[j].let)) {
				vers_to_first_place(wc, j);
				break;
			}
		}
	}
}

/////////////
static void contextNumberTable(void) {
	cell *cc = cell_f()->next;
	cell *fir, *las;
	Bool16 isNumber = 0;
	int numDig, numAll;
	int isCommaDot = 0;
	int isDig;
	uchar letAsDig[] = "OoIli!SsBg\x8e\xae\x87\xa7\x9d\xed\x97\xe7\xa1\x82\xa2";
	//                            ОоЗзЭэЧчбВв
	uchar accordDig[] = "001111558900333344688";
	char *qq;
	int minSize, maxSize;

	while (cc && !(cc->flg & c_f_fict)) {
		fir = cc;
		isNumber = 1;
		numAll = 0;
		isCommaDot = 0;
		numDig = 0;
		isDig = 0;

		minSize = 1024;
		maxSize = 0;
		for (las = fir; las != NULL; las = las->next, numAll++) {
			if (las->flg & c_f_fict)
				break;

			if (las->flg & c_f_space)
				break;

			if (las->nvers <= 0) {
				isNumber = 0;
				continue;
			}

			if (!isNumber)
				continue;

			if (las->vers[0].let == '.' || las->vers[0].let == ',') {
				isCommaDot++;
				numDig++;
				isDig = 1;
			} else if (strchr("0123456789$%", las->vers[0].let)) {
				numDig++;
				if (las->vers[0].let != '$' && las->vers[0].let != '%') {
					isDig = 1;
					minSize = MIN(minSize, las->h);
					maxSize = MAX(maxSize, las->h);
				}
			} else if (las == fir && las->vers[0].let == '-')
				numDig++;
			else if (strchr(letAsDig, las->vers[0].let)) {
				if ((las->vers[0].let == '!' || las->vers[0].let == 'i')
						&& las->env && las->env->nl > 1)
					isNumber = 0;
				else {
					minSize = MIN(minSize, las->h);
					maxSize = MAX(maxSize, las->h);
				}
			} else
				isNumber = 0;

		}

		if (isCommaDot > 1)
			isNumber = 0;

		if ((maxSize - minSize) * 7 > maxSize)
			isNumber = 0;

		if (isNumber && isDig && numDig < numAll) {
			for (cc = fir; cc != NULL && cc != las; cc = cc->next) {
				if (cc->nvers <= 0)
					continue;
				qq = strchr(letAsDig, cc->vers[0].let);

				if (qq)
					cc->vers[0].let = accordDig[(uchar*) qq - letAsDig];
			}

			if (snap_activity('c')) {
				snap_show_text("Context: set number in table");
				snap_monitor();
			}
		}

		if (las && !(las->flg & c_f_fict))
			cc = las->next;
		else
			break;
	}

}
//////////////
