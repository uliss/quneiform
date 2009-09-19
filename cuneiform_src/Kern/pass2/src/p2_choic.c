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

// добавлять всегда версии ФОНа
#define _ADD_FON_

#define _SPECTEST_

// учитывать язык предыдущего слова
#define _USE_PREV_LANG_

// проверять целиком куски (пример М - iii) а не три раза М-i
#define _TEST_FEW_MULTI_          // 29.01.2001
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "ligas.h"
#include "lang_def.h"
#include "cstr.h" // OLEG
#include "fon.h"
#include "p2libr.h"
#include "std.h"
#include "minmax.h"
//////////
#include "p2defs.h"

#include "rstr_p2.h"
/////////////////////
extern P2GLOBALS p2globals;
//////////

static int StayOldPalki(RecVersions *oldVer, RecVersions *newVer,
		CSTR_rast_attr *attr);
static Bool32 TestDobavki(uchar oldName, uchar newName);
/////////////////
// from erect2.c
static char const iInN[] = "\xa8\x88\xad\x8d"; // иИнН
static char const lLpP[] = "\xab\x8b\xaf\x8f"; // лЛпП
static Bool32 non_near_letters(RecVersions *v) {
	uchar v0, v1, p0, p1;
	if (v->lnAltCnt < 2)
		return TRUE;
	v0 = v->Alt[0].Code;
	v1 = v->Alt[1].Code;
	p0 = v->Alt[0].Prob;
	p1 = v->Alt[0].Prob;
	if (strchr(iInN, v0) && strchr(iInN, v1) && abs(p0 - p1) < 15)
		return FALSE;
	if (strchr(lLpP, v0) && strchr(lLpP, v1) && abs(p0 - p1) < 15)
		return FALSE;
	return TRUE;
}
//////////////////
Bool32 p2_leo_choise_fon_or_leo_absent(uchar p_fon, uchar p_leo) {
	if (p_fon < 200)
		return FALSE; // low LEO or heigh FON

	if (p_leo < 251 && p_fon > 254)
		return TRUE; // heigh FON & low LEO
	if (p_leo < 245 && p_fon > 250)
		return TRUE; // heigh FON & low LEO
	if (p_leo < 225 && p_fon > 246) // new - Nick
		return TRUE; // heigh FON &
	if (p_leo < 220 && p_fon > 240)
		return TRUE; // heigh FON & low LEO
	if (p_leo < 180 && p_fon > 230)
		return TRUE; // heigh FON & low LEO
	if (p_leo < 140 && p_fon > 220)
		return TRUE; // heigh FON & low LEO
	if (p_leo < 100)
		return TRUE; // heigh FON & low LEO

	return FALSE; // low FON
}
/////////////////////////
Bool32 p2_leo_choise_fon_or_leo(uchar p_fon, uchar p_leo) {
	if (p_fon > 180) {
		if (p_leo > 253)
			return FALSE; // heigh LEO & low FON
		if (p_leo > 240 && p_fon < 240)
			return FALSE; // heigh LEO & low FON
		if (p_leo > 220 && p_fon < 230)
			return FALSE; // heigh LEO & low FON
		if (p_leo > 200 && p_fon < 200)
			return FALSE; // heigh LEO & low FON
		if (p_leo > 180 && p_fon < 190)
			return FALSE;
		return TRUE; // low LEO or heigh FON
	}

	if (p_fon >= p_leo + 5)
		return TRUE;

	/*
	 if( p_fon > 140 )
	 {
	 if( p_fon > p_leo + 40)
	 return TRUE;
	 return FALSE;
	 }

	 if( p_fon > p_leo + 60)
	 return TRUE;
	 */
	return FALSE; // low FON
}
////////////////////////
static void leo_set_max_vers(RecVersions *v, int p) {
	int i, n, pold;
	if (p < 1)
		p = 1;
	n = v->lnAltCnt;
	pold = v->Alt[0].Prob;
	v->Alt[0].Prob = p;
	if (n > 1)
		for (i = 1; i < n; i++) {
			v->Alt[i].Prob = (v->Alt[i].Prob * p) / pold;
			if (v->Alt[i].Prob < 1)
				v->Alt[i].Prob = 1;
		}
	return;
}
/////////////////////
static uchar * leo_strchr_codes_ansi(uchar *c1, uchar c2) {
	uchar *c;
	for (c = c1; *c; c++)
		if (stdLeoCompareChar(stdAnsiToAscii(*c), c2))
			return c;
	return NULL;
}
////////////////////
static Bool32 leo_near_letters(RecVersions *fon, uchar leo_code) {
	// Nick 19.06.2001
	if (p2globals.language != LANG_RUSSIAN && p2globals.language != LANG_RUSENG)
		return FALSE;

	if (fon->lnAltCnt < 2)
		return FALSE;
	if (!(leo_strchr_codes_ansi("0оО", fon->Alt[0].Code)
			&& leo_strchr_codes_ansi("0оО", leo_code) && leo_strchr_codes_ansi(
			"0оО", fon->Alt[1].Code)) && !(leo_strchr_codes_ansi("3зЗ",
			fon->Alt[0].Code) && leo_strchr_codes_ansi("3зЗ", leo_code)
			&& leo_strchr_codes_ansi("3зЗ", fon->Alt[1].Code))
			&& !(leo_strchr_codes_ansi("пПлЛ", fon->Alt[0].Code)
					&& leo_strchr_codes_ansi("пПлЛ", leo_code)
					&& leo_strchr_codes_ansi("пПлЛ", fon->Alt[1].Code))
			&& !(leo_strchr_codes_ansi("иИнН", fon->Alt[0].Code)
					&& leo_strchr_codes_ansi("иИнН", leo_code)
					&& leo_strchr_codes_ansi("иИнН", fon->Alt[1].Code))
			&& !(leo_strchr_codes_ansi("б6", fon->Alt[0].Code)
					&& leo_strchr_codes_ansi("б6", leo_code)
					&& leo_strchr_codes_ansi("б6", fon->Alt[1].Code)))
		return FALSE;
	return (fon->Alt[0].Prob - fon->Alt[1].Prob < 5);
}
////////////////////////
static Bool32 leo_add_vers(RecVersions *dst, RecVersions *src) {
	int i, j;
	if (!src->lnAltCnt)
		return FALSE;
	for (i = dst->lnAltCnt, j = 0; i < REC_MAX_VERS && j < src->lnAltCnt; i++, j++)
		dst->Alt[i] = src->Alt[j];
	dst->lnAltCnt = i;
	return TRUE;
}
////////////////////
static int32_t cmp_prob(const void *a, const void *b) {
	return (int32_t) (((RecAlt *) b)->Prob) - (int32_t) (((RecAlt *) a)->Prob);
}
///////////////////////
int32_t p2_leo_sort_vers_prob(RecVersions *v) {
	int i, n0, n1;
	uchar c0, c1;
	RecAlt a0, a1;
	stdQsort(v->Alt, v->lnAltCnt, sizeof(RecAlt), cmp_prob);
	if (v->lnAltCnt > 1 && v->Alt[0].Prob == v->Alt[1].Prob) {
		c0 = v->Alt[0].Code;
		c1 = v->Alt[1].Code;
		a0 = v->Alt[0];
		a1 = v->Alt[1];

		for (n1 = n0 = i = 0; i < v->lnAltCnt; i++) {
			n0 += (v->Alt[i].Code == c0);
			n1 += (v->Alt[i].Code == c1);
		}
		if (n1 > n0) {
			//a0.Code=c1;
			//a1.Code=c0 ;
			v->Alt[0] = a1;
			v->Alt[1] = a0;

		}
	}
	return v->lnAltCnt;
}
////////////////////////
// убрать повторы из альтернатив
static void leo_compress(RecVersions *v) {
	uchar alph[256];
	RecVersions c;
	int i, j;

	memcpy(&c, v, sizeof(RecVersions));
	memset(alph, 0, 256);
	for (j = i = 0; i < v->lnAltCnt; i++) {
		if (!alph[c.Alt[i].Code]) {
			alph[c.Alt[i].Code] = 1;
			memcpy(&v->Alt[j++], &c.Alt[i], sizeof(RecAlt));
		}
	}

	v->lnAltCnt = j;

	return;
}
////////////////////////////
static Bool32 IsSpecialTwins(uchar name1, uchar name2) {
	if (p2globals.language == LANG_RUSSIAN && p2globals.langSer) {
		// serbian 'J','j' (0xA3,0xBC in code page 1251)
		if (name1 == 0xD6 && name2 == 0xD9 || name2 == 0xD6 && name1 == 0xD9)
			return TRUE;
	}

	return FALSE;
}
/////////////
//        ASCII НПнп
static char mixedLetters[] = "\x8d\x8f\xad\xaf";
static int MixedProbs(RecVersions *old, RecVersions *ver) {
	int i, j;
	int prob;
	uchar used[REC_MAX_VERS];
	int oldNum = MIN(REC_MAX_VERS, old->lnAltCnt);

	if (oldNum <= 0)
		return 0;

	if (stdLeoCompareChar(ver->Alt[0].Code, old->Alt[0].Code) || !strchr(
			mixedLetters, ver->Alt[0].Code) || !strchr(mixedLetters,
			old->Alt[0].Code))
		return 0;

	memset(used, 0, sizeof(used));
	for (i = 0; i < ver->lnAltCnt; i++) {
		prob = ver->Alt[i].Prob;
		for (j = 0; j < oldNum; j++) {
			if (used[j] == 0 && stdLeoCompareChar(ver->Alt[i].Code,
					old->Alt[j].Code)) {
				prob += old->Alt[j].Prob;
				prob >>= 1;
				ver->Alt[i].Prob = (uchar) prob;
				used[j] = 1;
				break;
			}
		}
	}

	for (i = ver->lnAltCnt, j = 0; j < oldNum && i < REC_MAX_VERS; j++, i++) {
		if (used[j] == 0)
			ver->Alt[i] = old->Alt[j];
	}
	ver->lnAltCnt = i;
	p2_leo_sort_vers_prob(ver);

	return 1;
}
///////////////////

#define POROG_NICE_FON 245    // was 250
#define POROG_NOTBAD_FON 220  // was 207
static int AddFonVersions(RecVersions *old, RecVersions *ver) {
	int oldProb = old->Alt[0].Prob;
	int j, i;
	int bI = FALSE; //. OLEG : 17-03-00 solid stick disabled adding '!'
	uchar language = p2globals.language;

	if (old->lnAltCnt && strchr("lI", old->Alt[0].Code))
		bI = TRUE;
	// добавим варианты от кластерного распознавания
	for (j = 0; j < ver->lnAltCnt; j++) {
		if (j > 0 && ver->Alt[j].Prob < ver->Alt[0].Prob - 10)
			break;

		// не добавляем от ЛЕО !
		if (ver->Alt[j].Method != REC_METHOD_FON)
			continue;

		for (i = 0; i < old->lnAltCnt; i++)
			if (old->Alt[i].Code == ver->Alt[j].Code)
				break;
		if (i == 0)
			continue;
		if ((ver->Alt[j].Code == '!' || ver->Alt[j].Code == liga_exm // 15.09.2000
		) && bI) //. OLEG : 17-03-00
			continue;

		if (i < old->lnAltCnt)
			old->Alt[i].Prob = MAX(old->Alt[i].Prob, MIN(oldProb - 10,
					ver->Alt[j].Prob));
		else if (old->lnAltCnt < REC_MAX_VERS) {
			old->Alt[old->lnAltCnt] = ver->Alt[j];
			old->Alt[old->lnAltCnt].Prob = MIN(oldProb - 10, ver->Alt[j].Prob);
			old->lnAltCnt++;
		}
	}

	p2_leo_sort_vers_prob(old);
	return old->lnAltCnt;
}
////////////////
// смешать два набора альтернатив и поместить в rast
// return 1 - за основу взят новый, 0 - старый
int32_t p2_leoMixture(CSTR_rast rast, RecVersions *old, RecVersions *ver,
		FontInfo *fontinfo) {
	Bool32 enable_let = TRUE;
	//int32_t    old_case ;
	uchar oldProb; //  = data[14]
	uchar oldName; //  = data[3]
	int i;
	CSTR_rast_attr attr;

	if (ver->lnAltCnt <= 0)
		return 0;

	// проверить одинаковые оценки - 5.02.2001
	//
	if (old->lnAltCnt > 0 && ver->lnAltCnt > 0 && old->Alt[0].Code
			!= ver->Alt[0].Code) {
		for (i = 1; i < ver->lnAltCnt; i++) {
			if (ver->Alt[i].Prob != ver->Alt[0].Prob)
				break;
			if (ver->Alt[i].Code == old->Alt[0].Code) {
				RecAlt tmp = ver->Alt[0];
				ver->Alt[0] = ver->Alt[i];
				ver->Alt[i] = tmp;
				break;
			}
		}
	}

	CSTR_GetAttr(rast, &attr);

	// надо изменить атрибут символа?
	if (attr.flg & CSTR_f_bad) {
		attr.flg &= ~CSTR_f_bad;
		attr.flg |= CSTR_f_let;
		CSTR_SetAttr(rast, &attr);
	}

	// оставить первопроходные палки
	if (StayOldPalki(old, ver, &attr)) {
		if (old->Alt[0].Code == ver->Alt[0].Code) {
			old->Alt[0].Prob = MAX(old->Alt[0].Prob, ver->Alt[0].Prob);

			for (i = 1; i < old->lnAltCnt; i++)
				if (old->Alt[i].Prob > 1)
					old->Alt[i].Prob--;

#ifdef  _ADD_FON_   // 22.02.00
			AddFonVersions(old, ver);
#endif

			p2_StoreVersions(rast, old);
			attr.RecogHistory
					|= (ver->Alt[0].Method == REC_METHOD_FON ? CSTR_hi_fon
							: CSTR_hi_leo);
			CSTR_SetAttr(rast, &attr);
		} else {
#ifdef  _ADD_FON_   // 22.02.00
			AddFonVersions(old, ver);
			p2_StoreVersions(rast, old);
			attr.RecogHistory
					|= (ver->Alt[0].Method == REC_METHOD_FON ? CSTR_hi_fon
							: CSTR_hi_leo);
			CSTR_SetAttr(rast, &attr);
#endif
		}

		return 0; // за основу - старое
	}

	if (old->lnAltCnt <= 0) {
		oldProb = 0;
		oldName = 0;
	} else {
		oldProb = old->Alt[0].Prob;
		oldName = old->Alt[0].Code;
	}

	if (TestDobavki(oldName, ver->Alt[0].Code)) {
		p2_StoreVersions(rast, old);
		return 0; // только старое
	}

	// смесь н - п
	if (MixedProbs(old, ver)) {
		p2_StoreVersions(rast, ver);
		if (ver->Alt[0].Code != old->Alt[0].Code)
			attr.recsource
					= (ver->Alt[0].Method == REC_METHOD_FON ? CSTR_rs_bitcmp
							: CSTR_rs_LEO);
		attr.RecogHistory
				|= (ver->Alt[0].Method == REC_METHOD_FON ? CSTR_hi_fon
						: CSTR_hi_leo);
		CSTR_SetAttr(rast, &attr);
		return 1;
	}

	// enable_correct_case =  ((data[27]&LEO_CONTROL_CASE)==0);
	if (!oldName || !fontinfo)
		enable_let = TRUE;
	else
		enable_let = (fontinfo->count[oldName] != 0 && FONCheckItself(oldName,
				old->Alt[0].Info, attr.col, attr.row));

	if (ver->Alt[0].Prob > POROG_NICE_FON && oldName && enable_let
			&& ver->Alt[0].Code != oldName && !stdLeoCompareChar(
			ver->Alt[0].Code, oldName) && !IsSpecialTwins(ver->Alt[0].Code,
			oldName)
			&& (ver->lnAltCnt == 1 || ver->Alt[0].Prob > ver->Alt[1].Prob
					|| !stdLeoCompareChar(ver->Alt[1].Code, oldName))) {
		if (attr.RecogHistory && !(attr.RecogHistory & (CSTR_hi_3x5
				| CSTR_hi_3x5s)))
			i = ver->lnAltCnt;
		else
			i = 1;

		for (; i < ver->lnAltCnt; i++) {
			if (ver->Alt[i].Code == oldName && ver->Alt[i].Prob
					>= POROG_NOTBAD_FON)
				break;
		}

		if (i >= ver->lnAltCnt) // берем только fon
		{
			p2_StoreVersions(rast, ver);

			if (ver->Alt[0].Method == REC_METHOD_FON) {
				attr.recsource = CSTR_rs_bitcmp;
				attr.RecogHistory = CSTR_hi_fon;
			} else if (ver->Alt[0].Method == REC_METHOD_LEO) {
				attr.recsource = CSTR_rs_LEO;
				attr.RecogHistory = CSTR_hi_leo;
			}
			CSTR_SetAttr(rast, &attr);
			return 1;
		}
	}

	if (p2_leo_choise_fon_or_leo(ver->Alt[0].Prob, oldProb) && !(ver->lnAltCnt
			> 1 && ver->Alt[0].Prob > 253 && oldProb > 253
			&& stdLeoCompareChar(ver->Alt[1].Code, oldName)) && (enable_let
			|| p2_leo_choise_fon_or_leo_absent(ver->Alt[0].Prob, oldProb))
			&& !leo_near_letters(ver, oldName) && !IsSpecialTwins(
			ver->Alt[0].Code, oldName))

	{ // storing FON collection
		if (ver->lnAltCnt > 1) { // expert LEO
			if (ver->Alt[0].Prob == ver->Alt[1].Prob && ver->Alt[1].Code
					== oldName) {
				ver->Alt[1].Code = ver->Alt[0].Code;
				ver->Alt[0].Code = oldName;
				for (i = 1; i < ver->lnAltCnt; i++)
					if (ver->Alt[i].Prob > 1)
						ver->Alt[i].Prob--;

			}
		}
		if (oldName == ver->Alt[0].Code) {
			//if( ver->Alt[0].Prob>240 ) ver->Alt[0].Prob=255;
			//else
			ver->Alt[0].Prob = MAX(oldProb, ver->Alt[0].Prob);
			i = 1;
			//    data[15] |= LEO_VALID_FONT;
		} else
			i = 0;
		for (; i < ver->lnAltCnt; i++)
			if (ver->Alt[i].Prob > 1)
				ver->Alt[i].Prob--;

		if (old->lnAltCnt && ver->lnAltCnt && old->Alt[0].Prob
				> ver-> Alt[0].Prob)
			leo_set_max_vers(old, ver->Alt[0].Prob - 1);

		leo_add_vers(ver, old);
		p2_leo_sort_vers_prob(ver);
		leo_compress(ver);

		p2_StoreVersions(rast, ver);

		if (ver->Alt[0].Method == REC_METHOD_FON) {
			attr.recsource = CSTR_rs_bitcmp;
			attr.RecogHistory |= CSTR_hi_fon;
		} else if (ver->Alt[0].Method == REC_METHOD_LEO)// LEO
		{
			attr.recsource = CSTR_rs_LEO;
			attr.RecogHistory |= CSTR_hi_leo;
		}

		CSTR_SetAttr(rast, &attr);
		return 1;
	}

	// старая коллекция

	if (old->lnAltCnt > 1 && old->Alt[0].Prob == old->Alt[1].Prob
			&& old->Alt[1].Code == ver->Alt[0].Code) {
		old->Alt[0].Code = old->Alt[1].Code;
		old->Alt[1].Code = oldName;
		for (i = 1; i < old->lnAltCnt; i++)
			if (old->Alt[i].Prob > 1)
				old->Alt[i].Prob--;
		p2_StoreVersions(rast, old);

		if (ver->Alt[0].Method == REC_METHOD_FON)
			attr.RecogHistory |= CSTR_hi_fon;
		else if (ver->Alt[0].Method == REC_METHOD_LEO)
			attr.RecogHistory |= CSTR_hi_leo;

		CSTR_SetAttr(rast, &attr);
		return 0;
	}

#ifdef _ADD_FON_

	if (oldProb > 100 && ver->Alt[0].Prob > 120 && (stdLeoCompareChar(
			ver->Alt[0].Code, oldName) || IsSpecialTwins(ver->Alt[0].Code,
			oldName))) {
		if (ver->lnAltCnt == 1 || ver->Alt[0].Prob - 5 > ver->Alt[1].Prob ||
		// next - actual for LEO
				ver->lnAltCnt > 2 && (p2_is_lowerASCII(ver->Alt[0].Code)
						&& ver->Alt[0].Code == p2_to_lowerASCII(
								ver->Alt[1].Code) || p2_is_upperASCII(
						ver->Alt[0].Code) && ver->Alt[0].Code
						== p2_to_upperASCII(ver->Alt[1].Code))
						&& ver->Alt[0].Prob - 5 > ver->Alt[2].Prob)

		{
			old->Alt[0].Prob = MAX(old->Alt[0].Prob, ver->Alt[0].Prob);
			// Nick 16.06.2001
			for (i = 1; i < old->lnAltCnt; i++)
				if (old->Alt[i].Prob > 1)
					old->Alt[i].Prob--;
		}
	}
	oldProb = old->Alt[0].Prob;

	// добавим варианты от кластерного распознавания
	AddFonVersions(old, ver);

	if (oldProb < 254) {
		for (i = 0; i < old->lnAltCnt; i++)
			if (old->Alt[i].Prob > 1)
				old->Alt[i].Prob--;
	}

#else
	if( oldProb > 10 &&
			!stdLeoCompareChar(ver->Alt[0].Code,oldName) &&
			!IsSpecialTwins(ver->Alt[0].Code,oldName)
	)
	{

		// добавим вариант от кластерного распознавания
		for(i=1;i<old->lnAltCnt;i++)
		if( old->Alt[i].Code == ver->Alt[0].Code) break;
		if( i< old->lnAltCnt )
		old->Alt[i].Prob=MAX(old->Alt[i].Prob,MIN(oldProb-10,ver->Alt[0].Prob));
		else if( old->lnAltCnt < REC_MAX_VERS)
		{
			old->Alt[old->lnAltCnt]=ver->Alt[0];
			old->Alt[old->lnAltCnt].Prob= MIN(oldProb-10,ver->Alt[0].Prob);
			old->lnAltCnt++;
		}
		p2_leo_sort_vers_prob( old );

		if( oldProb < 254 )
		{
			for(i=0;i<old->lnAltCnt;i++)
			if( old->Alt[i].Prob > 1 ) old->Alt[i].Prob--;
		}
	}
	else if(ver->lnAltCnt==1 || ver->Alt[0].Prob - 5 > ver->Alt[1].Prob )
	old->Alt[0].Prob=MAX(old->Alt[0].Prob,ver->Alt[0].Prob);
#endif

	p2_StoreVersions(rast, old);

	if (ver->Alt[0].Code == oldName) {
		if (ver->lnAltCnt > 0 && ver->Alt[0].Prob > 200
				&& non_near_letters(ver)) {
			if (ver->Alt[0].Method == REC_METHOD_FON)
				attr.RecogHistory |= CSTR_hi_fon;
			else if (ver->Alt[0].Method == REC_METHOD_LEO)
				attr.RecogHistory |= CSTR_hi_leo;
			CSTR_SetAttr(rast, &attr);
		}
	}

	return 0;
}
/////////////////
#define POROG_PALKA 6
#define POROG_1_PALKA 190  // was 180
static const char Palki[] = "![]()1Ifiltr";
static Bool32 IsInPalki(uchar cc) // 15.09.2000
{
	uchar language = p2globals.language;

	if (strchr(Palki, cc) || cc == liga_i || (language == LANG_TURKISH && // 30.05.2002 E.P.
			(cc == i_sans_accent || cc == II_dot_accent)))
		return TRUE;

	return FALSE;
}
//////////
static int StayOldPalki(RecVersions *oldVer, RecVersions *newVer,
		CSTR_rast_attr *attr) {
	int i;
	ClustInfo clustinfo;

	if (oldVer->lnAltCnt <= 0 || newVer->lnAltCnt <= 0)
		return 0;

	// обе - палки?
	if (!IsInPalki(oldVer->Alt[0].Code) || !IsInPalki(newVer->Alt[0].Code))
		return 0;

	if (oldVer->Alt[0].Code == newVer->Alt[0].Code)
		return 1;

	// сомнения на первом проходе ?
	if (oldVer->Alt[0].Prob < POROG_1_PALKA)
		return 0;

	for (i = 1;; i++) {
		clustinfo.let = oldVer->Alt[0].Code;
		i = FONGetClustInfo(&clustinfo, i);
		if (i <= 0)
			return 1; // нет в кластерах

		if (abs(attr->w - clustinfo.mw) <= 4 && abs(attr->h - clustinfo.mh)
				<= 4)
			break;
	}

	for (i = 1; i < newVer->lnAltCnt; i++) {
		if (oldVer->Alt[0].Code == newVer->Alt[i].Code && newVer->Alt[0].Prob
				- newVer->Alt[i].Prob < POROG_PALKA)
			return 1;
	}

	return 0;
}
////////////
static Bool32 SpecificPalka(uchar Code) {
	switch (p2globals.language) {
	case LANG_ENGLISH:
		return FALSE;
	case LANG_RUSSIAN:
		if (p2globals.langUkr && (Code == 0xAF || Code == 0xBF || Code == 0xB2
				|| Code == 0xB3))
			return TRUE;
		return FALSE;
	case LANG_GERMAN:
	case LANG_FRENCH:
	case LANG_SPANISH:
	case LANG_ITALIAN:
	case LANG_PORTUGUESE:
		if (Code >= 0xCC && Code < 0xD0 || Code >= 0xEC && Code < 0xF0)
			return TRUE;
		return FALSE;
	case LANG_POLISH:
		if (Code == POLISH_l) // as l
			return TRUE;
		return FALSE;
	case LANG_CZECH:
		if (Code == II_right_accent || Code == i_right_accent || Code
				== t_inv_roof)
			return TRUE;
		return FALSE;
	case LANG_HUNGAR:
		if (Code == II_right_accent || Code == i_right_accent)
			return TRUE;
		return FALSE;
	case LANG_ROMAN:
		if (Code == i_roof_accent || Code == II_roof_accent || Code
				== t_bottom_accent)
			return TRUE;
		return FALSE;
	case LANG_DUTCH:
	case LANG_DANISH:
	case LANG_CROATIAN:
	case LANG_SWEDISH:
		return FALSE;

		// Балтийские палки. 10.07.2001 E.P.
	case LANG_LATVIAN:
		if (Code == II_macron || Code == i_macron || Code == LL_bottom_accent
				|| Code == l_bottom_accent)
			return TRUE;
		return FALSE;

	case LANG_LITHUANIAN:
		if (Code == II_bottom_accent || Code == i_bottom_accent)
			return TRUE;
		return FALSE;

	case LANG_TURKISH: // Турецкие палки 31.05.2002 E.P.
		if (Code == II_roof_accent || Code == i_roof_accent || Code
				== II_dot_accent || Code == i_sans_accent)
			return TRUE;
		return FALSE;

	default:
		return FALSE;
	}
}
////////////////
Bool32 TestPalka(CSTR_rast leoStart, CSTR_rast leoEnd, CSTR_rast fonStart,
		CSTR_rast fonEnd, int porog) {
	RecVersions vrLeo;
	CSTR_rast_attr attr;
	int Prob = 255;
	int num1 = 0;
	int nDust = 0;

	for (; leoStart && leoStart != leoEnd; leoStart = CSTR_GetNext(leoStart)) {
		if (!CSTR_GetAttr(leoStart, &attr))
			return 0;
		if (attr.flg & (CSTR_f_dust | CSTR_f_punct)) {
			nDust++;
			continue;
		}

		if (!CSTR_GetCollection(leoStart, &vrLeo))
			return 0;

		if (vrLeo.lnAltCnt <= 0)
			return 0;
		if (!IsInPalki(vrLeo.Alt[0].Code) && !SpecificPalka(vrLeo.Alt[0].Code))
			return 0;

		Prob = MIN(Prob, vrLeo.Alt[0].Prob);
		num1++;
	}

	if (num1 <= 1 && nDust == 0)
		return 0;

	if (!CSTR_GetCollection(fonStart, &vrLeo))
		return 0;
	if (vrLeo.lnAltCnt <= 0)
		return 0;
	if (IsInPalki(vrLeo.Alt[0].Code))
		return 0;

	if (p2globals.language == LANG_ESTONIAN) {
		FontInfo fontinfo;
		FONFontInfo(&fontinfo);

		CSTR_GetAttr(fonStart, &attr);

		if (fontinfo.count[vrLeo.Alt[0].Code] > 0 && abs(
				fontinfo.height[vrLeo.Alt[0].Code] - attr.h) < attr.h / 8) {
			if (Prob > vrLeo.Alt[0].Prob)
				return 2;

			return 0;
		}
	}

	if (vrLeo.Alt[0].Prob > porog || (nDust > 0 && Prob < 220
			&& vrLeo.Alt[0].Prob > 200) || Prob < 200 && vrLeo.Alt[0].Prob
			> 180 || Prob < 180 && vrLeo.Alt[0].Prob > 140 || // 19.07.99
			Prob < 165 && vrLeo.Alt[0].Prob > 110)
		return 1;

	return 0;
}
//////////////////
Bool32 TestNewDust(CSTR_rast fonStart, CSTR_rast fonEnd, CSTR_rast leoStart,
		int porog) {
	RecVersions vrLeo, vrFon;
	CSTR_rast_attr attr;
	int Prob = 255;
	int num1 = 0, nDust = 0;

	for (; fonStart && fonStart != fonEnd; fonStart = CSTR_GetNext(fonStart)) {
		if (!CSTR_GetAttr(fonStart, &attr))
			return 0;
		if (attr.flg & CSTR_f_dust) {
			if (!(attr.flg_new & CSTR_fn_bl_cut)) //отрезанные по базам dust'ы не считаем - Paul 17-11-99
				nDust++;
			continue;
		}

		if (!CSTR_GetCollection(fonStart, &vrFon))
			return 0;
		if (vrFon.lnAltCnt <= 0)
			return 0;
		Prob = MIN(Prob, vrFon.Alt[0].Prob);
		num1++;
	}

	if (num1 <= 0)
		return 1; // fon - all dust
	if (num1 > 1 || nDust == 0)
		return 0;

	if (!CSTR_GetCollection(leoStart, &vrLeo))
		return 0;
	if (vrLeo.lnAltCnt <= 0)
		return 0;

	// есть мусор и буквы - близнецы
	// или не слишком большая разница оценок -
	// берем старый вариант

	if (stdLeoCompareChar(vrFon.Alt[0].Code, vrLeo.Alt[0].Code)
			|| IsSpecialTwins(vrFon.Alt[0].Code, vrLeo.Alt[0].Code) || Prob
			- porog < vrLeo.Alt[0].Prob)
		return 1;

	return 0;
}
//////////////////
// проверка - не склеена ли старая буква с
// прекрасной оценкой (254)
// Если все новые - 255 и они не палки и их несколько -
// берем их.
// add 8.12.99 - or if old(254) but onle EVN and new >= 248
//
Bool32 TestWideLetter(CSTR_rast fonStart, CSTR_rast fonEnd, CSTR_rast leoStart) {
	RecVersions vrLeo;
	CSTR_rast_attr attr;
	int Prob;
	int num1 = 0;
	Bool32 onlyEvn = FALSE;

	if (!CSTR_GetCollection(leoStart, &vrLeo))
		return TRUE;
	if (vrLeo.lnAltCnt <= 0)
		return TRUE;
	if (!CSTR_GetAttr(leoStart, &attr))
		return TRUE;

	if (attr.RecogHistory
			&& !(attr.RecogHistory & (CSTR_hi_3x5 | CSTR_hi_3x5s)))
		onlyEvn = TRUE;
	else
		onlyEvn = FALSE;

	Prob = vrLeo.Alt[0].Prob;

	for (; fonStart && fonStart != fonEnd; fonStart = CSTR_GetNext(fonStart)) {
		if (!CSTR_GetAttr(fonStart, &attr))
			return 0;
		if (!(attr.flg & CSTR_f_let))
			return 0;
		if (!CSTR_GetCollection(fonStart, &vrLeo))
			return 0;
		if (vrLeo.lnAltCnt <= 0)
			return 0;
		// простая буква ?
		if (IsInPalki(vrLeo.Alt[0].Code))
			return 0;
		// старый вариант лучше ?
		if (Prob > vrLeo.Alt[0].Prob && (!onlyEvn || vrLeo.Alt[0].Prob < 248))
			return 0;
		num1++;
	}

	if (num1 > 1)
		return TRUE;
	return FALSE;
}
//////////////////
// TOPKXCBMETYOPAHKXCBMeyuoparxcnyopxca
// ТОРКХСВМЕТУорАНкхсвмеуиорагхспуОРХСА
static const char sameEngRus[] = "1234567890ABCEHKMOPTXaceopxym";
//                                        АВСЕНКМОРТХасеорхут (т-курсив!)
static const char
		sameRusEng[] =
				"1234567890\x80\x82\x91\x85\x8d\x8a\x8C\x8E\x90\x92\x95\xa0\xe1\xa5\xae\xe0\xe5\xe3\xf5";

// return 1 - better first word  (russian usally)
//        2 - better second      (english)
int p2_selectRusEng(CSTR_rast rusStart, CSTR_rast rusEnd, CSTR_rast engStart,
		CSTR_rast engEnd, FontInfo *fontinfo, int prevLang) {
	CSTR_rast_attr attrRus, attrEng;
	RecVersions vrEng;
	RecVersions vrRus;
	int total = 0;
	int minrus = 256, mineng = 256;
	int betterRus = 0;
	int betterEng = 0;
	CSTR_rast tStart;
	int betterRealRus = 0;
	int betterRealEng = 0;
	int rusProb, engProb;
	int isMultiRus, isMultiEng;
	int isPalkaRus, isPalkaEng;
	int engLang = -1;

	//    if( nnew == 1 &&  TestPalka(engStart,engEnd,rusStart,rusEnd) )
	//         return 1;

	if (!engStart)
		return 1; // better russian

	// были ли возможности у второго прохода что-то изменить
	for (tStart = engStart; tStart && tStart != engEnd; tStart = CSTR_GetNext(
			tStart)) {
		if (!CSTR_GetAttr(tStart, &attrEng))
			return -3;
		if (!(attrEng.flg & CSTR_f_let))
			continue;
		if (!CSTR_GetCollection(tStart, &vrEng))
			return -3;
		if (vrEng.lnAltCnt <= 0)
			continue;

		engLang = attrEng.language;
		if (fontinfo->count[vrEng.Alt[0].Code] > 0)
			break;
	}
	if (!tStart || tStart == engEnd) {
		// учитывать язык предыдущего слова
#ifdef _USE_PREV_LANG_
		if (engLang >= 0 && engLang == prevLang)
			return 3;
#endif
		return 1; // оставить как было
	}

	for (; rusStart && rusStart != rusEnd;) {
		isPalkaRus = isPalkaEng = 0;
		rusProb = 0;

		if (!CSTR_GetAttr(rusStart, &attrRus))
			return -3;
		if (!(attrRus.flg & (CSTR_f_bad | CSTR_f_let))) {
			rusStart = CSTR_GetNext(rusStart);
			continue;
		}
		if (!CSTR_GetCollection(rusStart, &vrRus))
			return -3;
		total++;
		if (vrRus.lnAltCnt > 0) {
			rusProb = vrRus.Alt[0].Prob;
			if (IsInPalki(vrRus.Alt[0].Code))
				isPalkaRus++;
		}

		isMultiRus = 1;
		isMultiEng = 0;

#ifdef _TEST_FEW_MULTI_

		engProb = 255;
		for (; engStart && engStart != engEnd; engStart
				= CSTR_GetNext(engStart)) {
			if (!CSTR_GetAttr(engStart, &attrEng))
				return -3;
			if (!(attrEng.flg & (CSTR_f_bad | CSTR_f_let)))
				continue;
			if (!CSTR_GetCollection(engStart, &vrEng))
				return -3;

			isMultiEng++;
			if (vrEng.lnAltCnt <= 0)
				engProb = 0;
			else {
				engProb = MIN(engProb, vrEng.Alt[0].Prob);
				if (IsInPalki(vrEng.Alt[0].Code))
					isPalkaEng++;
			}

			if (attrRus.col + attrRus.w <= attrEng.col + attrEng.w)
				break;

			{ // test next
				CSTR_rast_attr tmpAttr;
				CSTR_rast tmpRast = CSTR_GetNext(engStart);

				if (!tmpRast || tmpRast == engEnd)
					break;
				if (!CSTR_GetAttr(tmpRast, &tmpAttr))
					return -3;
				if (attrRus.col + attrRus.w < tmpAttr.col + tmpAttr.w)
					break;
			}
		}

		if (isMultiEng == 0)
			engProb = 0;

		if (isMultiEng == 1 && attrRus.col + attrRus.w < attrEng.col
				+ attrEng.w) { // test next russian
			CSTR_rast_attr tmpAttr;
			CSTR_rast tmpRast = CSTR_GetNext(rusStart);

			if (tmpRast && tmpRast != engEnd && CSTR_GetAttr(tmpRast, &tmpAttr)
					&& (tmpAttr.flg & (CSTR_f_bad | CSTR_f_let)) && tmpAttr.col
					+ tmpAttr.w <= attrEng.col + attrEng.w) {
				isMultiRus++;
				attrRus = tmpAttr;
				rusStart = tmpRast;
				if (!CSTR_GetCollection(rusStart, &vrRus))
					return -3;
				if (vrRus.lnAltCnt > 0) {
					rusProb = MIN(rusProb, vrRus.Alt[0].Prob);
					if (IsInPalki(vrRus.Alt[0].Code))
						isPalkaRus++;
				} else
					rusProb = 0;
			}
		}
#else
		for(;engStart && engStart != engEnd; engStart=CSTR_GetNext (engStart) )
		{
			if( !CSTR_GetAttr (engStart, &attrEng) )
			return -3;
			if(!(attrEng.flg & (CSTR_f_bad|CSTR_f_let)))
			continue;
			if( !CSTR_GetCollection(engStart,&vrEng) )
			return -3;
			break;
		}

		if( engStart )
		engProb = vrEng.Alt[0].Prob;
		else
		engProb = 0;
#endif
		if (!engStart || engStart == engEnd) {
			betterRus++;
			break;
		}

		if (rusProb > 100) {
			if (engProb <= 0) {
				betterRus++;
				betterRealRus++;
			} else {
				Bool32 same;
				int porog;
				char *qq;

				if (attrEng.language == LANG_ENGLISH) {
					if (MAX(isMultiRus, isMultiEng) <= 1 && (qq = strchr(
							sameEngRus, vrEng.Alt[0].Code)) != NULL
							&& (char) vrRus.Alt[0].Code == sameRusEng[qq
									- sameEngRus]) {
						same = TRUE;
						porog = 10;
					} else {
						same = FALSE;
						porog = 1;
					}
				} else {
					if (MAX(isMultiRus, isMultiEng) <= 1 && (qq = strchr(
							sameRusEng, vrEng.Alt[0].Code)) != NULL
							&& (char) vrRus.Alt[0].Code == sameEngRus[qq
									- sameRusEng]) {
						same = TRUE;
						porog = 10;
					} else {
						same = FALSE;
						porog = 1;
					}
				}

				if (rusProb - porog > engProb) {
					betterRus++;
#ifdef _TEST_FEW_MULTI_
					if ((isPalkaRus <= 1) && (isMultiEng > 1) && isPalkaEng
							== isMultiEng)
						betterRus++;
#endif
					if (!same)
						betterRealRus++;
				} else if (engProb - porog > rusProb) {
					betterEng++;
#ifdef _TEST_FEW_MULTI_
					if ((isMultiEng <= 1) && (isMultiRus > 1) && isPalkaRus
							== isMultiRus)
						betterEng++;
#endif
					if (!same)
						betterRealEng++;
				}
#ifdef _SPECTEST_
				else if (!same && MAX(isMultiRus, isMultiEng) <= 1 && // 29.01.2001
						fontinfo->count[vrEng.Alt[0].Code] > 0
						&& fontinfo->count[vrRus.Alt[0].Code] > 0
						&& (vrEng.Alt[0].Method != REC_METHOD_FON
								|| vrRus.Alt[0].Method != REC_METHOD_FON)
						&& p2_rsadd_get_alias_class(vrEng.Alt[0].Code,
								attrEng.language) && p2_rsadd_get_alias_class(
						vrRus.Alt[0].Code, attrRus.language)) {
					int probEng = 0, probRus = 0, nAlt;
					RecRaster recRast;
					FonTestInfo tInfo[MAXCHECKALT];

					if (vrEng.Alt[0].Method == REC_METHOD_FON)
						probEng = vrEng.Alt[0].Prob;
					else {
						if (CSTR_GetImage(engStart, (uchar*) &recRast,
								CSTR_TYPE_IMAGE_RS)) {
							nAlt = FONTestChar(&recRast, vrEng.Alt[0].Code,
									tInfo, 0);
							if (nAlt > 0)
								probEng = tInfo[0].prob;
						}
					}

					if (vrRus.Alt[0].Method == REC_METHOD_FON)
						probRus = vrRus.Alt[0].Prob;
					else {
						if (CSTR_GetImage(rusStart, (uchar*) &recRast,
								CSTR_TYPE_IMAGE_RS)) {
							nAlt = FONTestChar(&recRast, vrRus.Alt[0].Code,
									tInfo, 0);
							if (nAlt > 0)
								probRus = tInfo[0].prob;
						}
					}

					if (probRus > probEng + 5) {
						betterRus++;
						betterRealRus++;
					} else if (probEng > probRus + 5) {
						betterEng++;
						betterRealEng++;
					}
				}
#endif
			}
		} else {
			if (engProb > 100) {
				betterEng++;
				betterRealEng++;
			}
		}

		if (vrRus.lnAltCnt <= 0)
			minrus = 0;
		else
			minrus = MIN(minrus, rusProb);
		if (vrEng.lnAltCnt <= 0)
			mineng = 0;
		else
			mineng = MIN(mineng, engProb);

		if (attrRus.col + attrRus.w <= attrEng.col + attrEng.w) {
			// to next russian raster
			rusStart = CSTR_GetNext(rusStart);
			if (!rusStart)
				break;
			if (!CSTR_GetAttr(rusStart, &attrRus))
				return -3;
			// надо сместить english ?
			if (attrRus.col + attrRus.w > attrEng.col + attrEng.w)
				engStart = CSTR_GetNext(engStart);
		} else {
			// to next english raster
			engStart = CSTR_GetNext(engStart);
			if (!engStart)
				break;
			if (!CSTR_GetAttr(engStart, &attrEng))
				return -3;
			// надо сместить russian ?
			if (attrRus.col + attrRus.w < attrEng.col + attrEng.w)
				rusStart = CSTR_GetNext(rusStart);
		}
	}

	if (total <= 0)
		return 0;

	// Nick - new
	if (betterRealRus > betterRealEng)
		return 1;
	if (betterRealEng > betterRealRus)
		return 2;

	if (betterRus > betterEng)
		return 1;
	if (betterEng > betterRus)
		return 2;

	if (minrus > mineng + 10)
		return 1;
	else if (minrus + 10 < mineng)
		return 2;

	// учитывать язык предыдущего слова
#ifdef _USE_PREV_LANG_
	if (engLang >= 0 && engLang == prevLang)
		return 3;
#endif
	return 0;
}
//////////////////////
static Bool32 TestDobavki(uchar oldName, uchar newName) {
	if (p2globals.language == LANG_FRENCH) {
		if (oldName == 0xf9 && newName == 'u')
			return TRUE;
	}

	return FALSE;
}
////////////////////////
