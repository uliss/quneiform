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

#include "minmax.h"

#define GRAF

// распознавать жирные цифры по ЛЕО
// Nick 27.06.2001
//#define _TEST_DIGITS_LEO_

// разрезание/склейка наложением кластеров
#define _FON_2_PALKI_
#define _FON_GLUE_BROKEN_

//#define _FON_GLUE_
//#define _LEO_LAST_

// использовать флаги для уменьшения перебора при
// разрезании-склеивании
// также должен стоять в p2_tools !!!
//#define _USE_DETOUCH_

#define _USE_LEO_
// распознавать английский по ЛЕО
#define _ENGLISH_LEO_
// использовать ЛЕО для всех языков
#define _ALL_LEO_

// проверять выходные слова по словарю. В случае
// словарности второго и не словарности первого прохода -
// брать второй целиком.
#define _TEST_OUT_SOLID_

// сохранить символы, кластеры в базах
// #define _SAVE_IN_CTB_
// параметры для нового распознавания
#define _USE_FON_

#define POROG_BROKEN 200
#define TRSGLUFINE  240
#define POROG_2PALKI 240

#ifdef _USE_FON_
#define TRSFI      220   // in check_word, estletter
#define TRSFINE        220  // in compose_word
#define TRSNOTBAD  180  // in compose_word
#else
#define TRSFI      180   // in check_word, estletter
#define TRSFINE        180  // in compose_word
#define TRSNOTBAD  120  // in compose_word
#endif

// 180 - TRSGOODCUT , TRSNOTBADCUT  in CG_MAIN
// PROB_OK 220 in p2_rec
//
#define POROG_PUNCT 220

#define POROG_SELF_SOLID  20  // for testSelf
#define POROG_SELF_COMMON 40  // for testSelf
#define MAX_WID_BROKEN    64

#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

#include "lang_def.h"
#include "cstr.h" // OLEG
#include "fon.h"
#include "std.h"
#include "alphabet.h"
#include "ligas.h"
#include "p2libr.h"

#include "p2.h"
#include "rstr_p2.h"

#ifdef _USE_LEO_
#define POROG_GOOD_LEO 220   // 180 - many errors
#define POROG_GOOD_SPEC 245  // some letters - bad leo
#define POROG_GOOD_VERY_SPEC 252
//#include "leo.h"
static RecObject ro;
static uchar specBadRus[] = "ХхГгаВвЮюТтЙйКк";
static uchar specBadEng[] = "mnVvWwaBrYQ";
static uchar specBadNon[] = "";
static uchar *specBadLeo = specBadRus;
static uchar specVeryBadRus[] = "Щщ";
static uchar specVeryBadEng[] = "";
static uchar *specVeryBadLeo = specBadNon;
#endif
//======== EXTERN FUNCTIONS
int p2_rotateRecRaster(RecRaster *rec, int ninc);
// p2_tools.c
int32_t p2_setOddEvenFlag(CSTR_rast first, CSTR_rast last);
//======== EXTERN VARIABLES
//extern char alphabet[256];  // rcm.c
//extern uchar fon_alphabet_language[2][256];
/*======= Export funcs =================*/
//int32_t   p2_proc (CSTR_line lineDraft,CSTR_line lineOne);
//int16_t     estletter       (cell * BC,s_glue * GL);
// snap.c
//extern Handle hSnapLEO;
//////// local variables    //////////
P2GLOBALS p2globals;
//static float fallTime=0.0f;
// из какой колонки строка
static int p2_lineColumn = 0;
// средний наклон текущего слова
static int p2_incline = 0;
static uchar p2_font; // OLEG
// показывать слова в снэпе
static Bool32 p2_ShowWords = FALSE;
// место остановки
static int p2_stopColumn = -1;
// вызывать ЛЕО при случае ?
static int p2_needLeo = 0;
static CSTR_line lineSnap = (CSTR_line) NULL;

// какой язык на прежней строке Nick 08.08.2001
static int prevLine = -11;
static int prevLang = -1;
/*======= Local funcs ==================*/
// установить язык
FON_FUNC(int32_t) FONRecogBroken(CSTR_rast firLeo,CSTR_rast lasLeo,
		CSTR_rast firNew,CSTR_rast lasNew,
		int lang, int porog, int nNaklon, int nRazmaz);

static void p2_SetLanguage(CSTR_rast first, CSTR_rast last, uchar lang);
static int BrokenRerecog(CSTR_rast first, CSTR_rast last, CSTR_line lineRaw,
		Bool32 single);
static int GlueRerecog(CSTR_rast first, CSTR_rast last, CSTR_line lineRaw,
		Bool32 boAll);
static int RerecogPalki(CSTR_rast first, CSTR_rast last, CSTR_line lineRaw);
static int RecogBrokenPalki(CSTR_rast firLeo, CSTR_rast lasLeo,
		CSTR_line lineRaw, Rect32* rect, int lang, int32_t prob);
static int p2_getIncline(CSTR_rast first, CSTR_rast last);
static uchar p2_getFont(CSTR_rast first, CSTR_rast last); // OLEG
static void FindAccordLine(CSTR_line lineRaw, CSTR_rast *firOld,
		CSTR_rast *lasOld, CSTR_rast first, CSTR_rast last, int32_t naklon);
// from first, < last
static int32_t CheckWord(CSTR_rast first, CSTR_rast last, CSTR_line lineOut,
		uchar *isSolid);
static void MixtureAnswer(CSTR_rast first, CSTR_rast last, CSTR_rast firstNew,
		FontInfo *fontinfo);
static int32_t RecogWord(CSTR_rast first, CSTR_rast last, CSTR_line lineFon,
		FontInfo *fontinfo, int nNaklon);
static int32_t composeWords(CSTR_rast fStart, CSTR_rast fEnd,
		CSTR_rast *lStart, CSTR_rast lEnd, FontInfo *fontinfo, Bool32 broken);
static CSTR_rast AddRastersLine(CSTR_rast fRast, CSTR_rast eRast,
		CSTR_rast rastOut, Bool emptyCollect, int nRotate, int nNaklon);
static Bool32 p2_needStop(CSTR_rast first, CSTR_rast last);
static int mark_strong_letters(CSTR_rast fRecog, CSTR_rast lRecog,
		CSTR_rast fRaw, CSTR_rast lRaw);
///////////////////////////

#ifdef _SAVE_IN_CTB_
#include "ctb.h"
static char nickName[12]="nickb00";
#endif

//////////////////////
static char outt[1024];
static CSTR_head *llRaw, *ll1, *ll2;
/////////////

// коды ошибок для p2_proc
#define ERR_GET_RAST -3   // получение растра и параметров
#define ERR_GET_LINE -4   // получение параметров строки
#define ERR_NEW_LINE -5   // создание строки
#define ERR_NEW_RAST -6   // создание растра

#define MAX_LEN_WORD  48
static int WrdLength(CSTR_rast first, CSTR_rast last) {
	CSTR_rast rst;
	CSTR_rast_attr attr;
	int nlet;

	for (rst = first, nlet = 0; rst && rst != last; rst = CSTR_GetNext(rst)) {
		CSTR_GetAttr(rst, &attr);
		//  буква ?
		if (attr.flg & CSTR_f_let)
			nlet++;
	}
	return nlet;
}
/////////////////
static Bool32 GoodSpell(CSTR_rast first, CSTR_rast last, int minSize) {
	RecVersions verOld;
	CSTR_rast rst;
	CSTR_rast_attr attr;
	uchar wrd[MAX_LEN_WORD];
	uchar lang;
	int nlet;
	uchar minProb = 255;

	for (rst = first, nlet = 0; rst && rst != last; rst = CSTR_GetNext(rst)) {
		if (!CSTR_GetAttr(rst, &attr) || !CSTR_GetCollection(rst, &verOld))
			return FALSE;

		// too small - don't use
		if (attr.h <= 2 && attr.w <= 2)
			continue;

		// не буква ? может быть в начале или конце
		if (!(attr.flg & (CSTR_f_let | CSTR_f_bad))) {
			CSTR_rast rr;
			CSTR_rast_attr at;
			if (nlet == 0)
				continue;

			rr = CSTR_GetNext(rst);
			for (; rr && rr != last; rr = CSTR_GetNext(rr)) {
				CSTR_GetAttr(rr, &at);
				if (at.flg & (CSTR_f_let | CSTR_f_bad))
					break;
			}
			if (!rr || rr == last)
				break;
			return FALSE;
		}

		if (verOld.lnAltCnt <= 0)
			return FALSE;

		lang = attr.language;
		wrd[nlet++] = verOld.Alt[0].Code;
		minProb = MIN(minProb, verOld.Alt[0].Prob);
		if (nlet >= MAX_LEN_WORD)
			return FALSE;
	}

	wrd[nlet] = 0;
	// слишком короткое?
	if (nlet < minSize)
		return FALSE;
	// граничной длины - смотрим оценки
	if (nlet == minSize && minProb < 205)
		return FALSE;

	if (RSTR_p2_spell(wrd, lang))
		return TRUE;
	return FALSE;
}
/////////////////
static CSTR_rast LastRaster(CSTR_rast fir, CSTR_rast las) {
	CSTR_rast curLeo;

	if (las) {
		curLeo = CSTR_GetPrev(las);
		if (curLeo)
			return curLeo;
	}

	for (; fir; fir = curLeo) {
		curLeo = CSTR_GetNext(fir);
		if (!curLeo || curLeo == las)
			break;
	}
	return fir;
}
//////////////////
// заменить [startLeo,nextLeo) на копию [startFon,nextFon)
// с возможным поворотом
// возврат - новый startLeo
static CSTR_rast ReplaceWord(CSTR_rast startLeo, CSTR_rast nextLeo,
		CSTR_rast startFon, CSTR_rast nextFon, int incline) {
	CSTR_rast curLeo;

	// заменим Leo на Fon
	// удалим старое -  кроме первого
	for (curLeo = CSTR_GetNext(startLeo); curLeo && curLeo != nextLeo;)
		curLeo = CSTR_DelRaster(curLeo);

	// добавим новое
	if (!AddRastersLine(startFon, nextFon, startLeo, FALSE, incline, 0))
		return NULL;

	// now delete old first
	curLeo = CSTR_DelRaster(startLeo);

	return curLeo;
}
/////////////////////
static void ReplaceAnswer(CSTR_rast first, CSTR_rast last, CSTR_rast firstNew) {
	CSTR_rast rst;
	UniVersions verNew;
	CSTR_rast_attr attr;

	for (rst = first; rst && rst != last; rst = CSTR_GetNext(rst)) {
		if (!CSTR_GetAttr(rst, &attr))
			return;
		// не буква ?
		if (!(attr.flg & (CSTR_f_bad | CSTR_f_let))) {
			firstNew = CSTR_GetNext(firstNew);
			continue;
		}

		if (!firstNew || !CSTR_GetCollectionUni(firstNew, &verNew))
			return;
		CSTR_StoreCollectionUni(rst, &verNew);
		attr.recsource
				= (verNew.Alt[0].Method == REC_METHOD_FON ? CSTR_rs_bitcmp
						: CSTR_rs_LEO);
		attr.RecogHistory
				= (verNew.Alt[0].Method == REC_METHOD_FON ? CSTR_hi_fon
						: CSTR_hi_leo);
		CSTR_SetAttr(rst, &attr);

		firstNew = CSTR_GetNext(firstNew);
	}

}
/////////////////////////////////
static Bool GoodWordProb(CSTR_rast first, CSTR_rast last, int porog) {
	CSTR_rast rst;
	UniVersions verNew;
	CSTR_rast_attr attr;

	for (rst = first; rst && rst != last; rst = CSTR_GetNext(rst)) {
		if (!CSTR_GetAttr(rst, &attr))
			return FALSE;
		if (attr.flg & CSTR_f_bad)
			return FALSE;
		// не буква ?
		if (!(attr.flg & CSTR_f_let))
			continue;
		if (!CSTR_GetCollectionUni(rst, &verNew))
			return FALSE;
		if (verNew.Alt[0].Prob < porog)
			return FALSE;
	}

	return TRUE;
}
/////////////////////////////////
static int32_t p2_ShowSnap(CSTR_rast first, CSTR_rast last) {
	CSTR_rast cur;

	if (!lineSnap) {
		RSTR_p2_snap_show_text("Error create CSTR_NewLine for Snap!");
		return 0;
	}

	for (cur = CSTR_GetNext(CSTR_GetFirstRaster(lineSnap)); cur;)
		cur = CSTR_DelRaster(cur);
	AddRastersLine(first, last, CSTR_GetFirstRaster(lineSnap), FALSE, 0, 0);

	RSTR_p2_snap_monitor_ori(&lineSnap, 1);
	for (cur = CSTR_GetNext(CSTR_GetFirstRaster(lineSnap)); cur;)
		cur = CSTR_DelRaster(cur);

	return 1;
}
///////////////
// put better/mixed variant in line first-last
// return (new) first
static CSTR_rast CompareRecogVersions(CSTR_rast first, CSTR_rast last,
		CSTR_rast firstNew, CSTR_rast lastNew, uchar isSolid, Bool32 useSpell,
		FontInfo *fontinfo, char *outt, Bool32 broken) {
	int ret = 0;

	if (RSTR_p2_snap_activity('m') || p2_ShowWords) {
		// повторим старое
		strcpy(outt + strlen(outt) - 3, "pass1");
		RSTR_p2_snap_show_text(outt);
		p2_TextProb(CSTR_GetRasterLine(firstNew), 0, 0, outt, 1000);
		strcat(outt, "-pass2");
		RSTR_p2_snap_show_text(outt);
		if (RSTR_p2_snap_activity('m') && p2_needStop(first, last)) {
			CSTR_line l[] = { CSTR_GetRasterLine(firstNew), CSTR_GetRasterLine(
					first) };
			RSTR_p2_snap_show_text(
					"Press <1> - pass2. Press <2>- pass1.<Enter>-compose");
			RSTR_p2_snap_monitor_ori(l, 2);
			RSTR_p2_snap_show_text("End compose word.");
		}
	}

	//  lastNew=CSTR_GetLastRaster(lineFon);
	//  firstNew=CSTR_GetNext(firstNew);

	// словарное слово не заменяем на несловарное !
	if (isSolid && useSpell && WrdLength(first, last) >= 3 && !GoodSpell(
			firstNew, lastNew, 2)) {
		// флажки для словарного слова
		p2_setOddEvenFlag(first, last);

		if (RSTR_p2_snap_activity('m') || p2_ShowWords)
			RSTR_p2_snap_show_text("pass2 - not solid");
		return first;
	}

	//#ifdef _TEST_OUT_SOLID_
	// было не словарное - стало словарное?
	if (!isSolid && GoodSpell(firstNew, lastNew, useSpell ? 5 : 8)) {
		first = ReplaceWord(first, last, firstNew, lastNew, -p2_incline);
		if (!first)
			return NULL;

		if (RSTR_p2_snap_activity('m') || p2_ShowWords)
			RSTR_p2_snap_show_text("pass2 - solid");

		// флажки для словарного слова
		//           first=CSTR_GetNext(savFirst?savFirst:CSTR_GetFirstRaster(savLine));
		p2_setOddEvenFlag(first, last);
		return first;
	}
	//#endif

	// смешаем результаты распознавани
	// и запишем в lineOne - что-то из старого можем удалить
	ret = composeWords(firstNew, lastNew, &first, last, fontinfo, broken);
	//	  *firOld = first;

	if (RSTR_p2_snap_activity('m')) {
		//first=CSTR_GetNext(savFirst?savFirst:CSTR_GetFirstRaster(savLine));
		p2_TextProb((CSTR_line) NULL, first, last, outt, 1000);
		strcat(outt, "-compose");
		RSTR_p2_snap_show_text(outt);
	}

	if (ret < 0)
		return NULL; // mark error

	return first;
}
////////////////
// test-rerecog word [*firOld,last)
// temporary  - put in lineFon
// use for cut/glue - lineRaw
// put results -> replace *firOld, (or stay old)
static int32_t p2_processWord(CSTR_line lineRaw, CSTR_line lineFon,
		CSTR_rast *firOld, CSTR_rast last, FontInfo *fontinfo, Bool32 useSpell) {
	int32_t ret;
	CSTR_rast firstNew = CSTR_GetFirstRaster(lineFon);
	CSTR_rast lastNew, rst;
	uchar isSolid = 0;
	//  CSTR_rast       savFirst;
	//  CSTR_line       savLine;
	Bool vSnap = FALSE;
	CSTR_rast first = *firOld;

	// clear lineFon
	for (rst = CSTR_GetNext(firstNew); rst;)
		rst = CSTR_DelRaster(rst);

	// результат распознавания - в lineFon
	if ((ret = CheckWord(first, last, lineFon, &isSolid)) < 0)
		return ret;

	//  savFirst=CSTR_GetPrev(first);
	//  savLine=CSTR_GetRasterLine(first);

	if ((vSnap = (RSTR_p2_snap_activity('m') && p2_needStop(first, last)))
			|| p2_ShowWords) {
		p2_TextProb((CSTR_line) NULL, first, last, outt, 1000);
		if (ret == 0)
			strcat(outt, " - no letters");
		else if (ret == 1)
			strcat(outt, " - solid");
		else if (ret == 2)
			strcat(outt, " - mixture");
		else if (ret == 3)
			strcat(outt, " - cut");
		else if (ret == 4)
			strcat(outt, " - cutGlue");

		if (isSolid && ret > 1)
			strcat(outt, "(solid)");

		RSTR_p2_snap_show_text(outt);
		if (ret == 2 || ret == 4) {
			p2_TextProb(lineFon, 0, 0, outt, 1000);
			strcat(outt, "-pass2");
			RSTR_p2_snap_show_text(outt);
		}

		if (vSnap)
			p2_ShowSnap(first, last);
	}

	if (ret == 1) // флажки для словарного слова
		p2_setOddEvenFlag(first, last);

	if (ret < 2)
		return ret;

	if (p2globals.enable_smart_cut) {
		// if bad accord - попробуем разрезать наложением кластеров
		if (ret == 3 || ret == 4) {
			int brRet = BrokenRerecog(CSTR_GetNext(firstNew),
					CSTR_GetLastRaster(lineFon), lineRaw, TRUE);
			int glRet = 0;

			// в p2globals установлен текущий язык
			p2_SetLanguage(CSTR_GetNext(firstNew), CSTR_GetLastRaster(lineFon),
					(uchar) p2globals.language);

			// nice broken rerecog ?
			if (brRet == 1 && (ret == 4 || GoodSpell(CSTR_GetNext(firstNew),
					CSTR_GetLastRaster(lineFon), 5)))
				brRet = 2;
			else
				// try rerecog once more
				glRet = GlueRerecog(CSTR_GetNext(firstNew), CSTR_GetLastRaster(
						lineFon), lineRaw, FALSE);

			// в p2globals установлен текущий язык
			p2_SetLanguage(CSTR_GetNext(firstNew), CSTR_GetLastRaster(lineFon),
					(uchar) p2globals.language);

			if (brRet == 2 || glRet > 0
			// ||	brRet > 0
			) {
				*firOld = CompareRecogVersions(first, last, CSTR_GetNext(
						firstNew), CSTR_GetLastRaster(lineFon), isSolid,
						useSpell, fontinfo, outt, TRUE);

				first = *firOld;

				if (!(*firOld))
					ret = -1;
				else
					//if ( brRet == 2 || glRet > 0 )
					ret = 4;
			} else if (ret == 4)
				ret = 2;
		}
	}

	// в p2globals установлен текущий язык
	p2_SetLanguage(CSTR_GetNext(firstNew), CSTR_GetLastRaster(lineFon),
			(uchar) p2globals.language);

	switch (ret) {
	// ret == 0 || ret == 1 - ничего не надо
	case 2:
		// словарное слово не заменяем на несловарное !
		if (isSolid && useSpell && WrdLength(first, last) >= 3 && !GoodSpell(
				CSTR_GetNext(firstNew), CSTR_GetLastRaster(lineFon), 2)) {
			// флажки для словарного слова
			p2_setOddEvenFlag(first, last);

			if (RSTR_p2_snap_activity('m') || p2_ShowWords)
				RSTR_p2_snap_show_text("pass2 - not solid");
			break;
		}

#ifdef _TEST_OUT_SOLID_
		// было не словарное - стало словарное?
		if (!isSolid && useSpell && GoodSpell(CSTR_GetNext(firstNew),
				CSTR_GetLastRaster(lineFon), 5))

		{
			ReplaceAnswer(first, last, CSTR_GetNext(firstNew));
			if (RSTR_p2_snap_activity('m') || p2_ShowWords)
				RSTR_p2_snap_show_text("pass2 - solid");

			// флажки для словарного слова
			p2_setOddEvenFlag(first, last);
			break;
		}
#endif
		// смешаем варианты для каждой буквы / выберем из двух
		MixtureAnswer(first, last, CSTR_GetNext(firstNew), fontinfo);
		break;
	case 3:
		// перераспознаем с перерезкой
		// удалим старое-новое
		for (lastNew = CSTR_GetNext(firstNew); lastNew; lastNew = CSTR_GetNext(
				firstNew))
			CSTR_DelRaster(lastNew);

		// определим наклон букв - по распознанной строке!
		p2_incline = p2_getIncline(first, last);
		p2_font = p2_getFont(first, last); // OLEG


		{
			CSTR_rast firOld = (CSTR_rast) NULL, lasOld;

			//B->r_row-(int16_t)((int32_t)nIncline*B->r_col/2048);
			// возьмем нужный кусок сырой строки
			FindAccordLine(lineRaw, &firOld, &lasOld, first, last,
					(int) p2globals.nIncline);

			// ничего не подобрали - берем из распознанной
			//           на первом проходе
			if (!firOld) {
				firOld = first;
				lasOld = last;
			}

#ifdef _USE_DETOUCH_
			mark_strong_letters(first,last,firOld,lasOld);
#endif

#ifdef _FON_GLUE_
			{
				FON_FUNC(int) FONRecogGlue(CSTR_rast firLeo,CSTR_rast lasLeo,
						CSTR_rast firOut,int lang, int nNaklon);
				int lang=p2globals.language;

				if( lang==LANG_ENGLISH && p2globals.multy_language )
				lang = LANG_RUSENG;
				// удалим старое-новое
				for(lastNew=CSTR_GetNext(firstNew);lastNew;lastNew=CSTR_GetNext(firstNew))
				CSTR_DelRaster(lastNew);
				ret=FONRecogGlue(firOld,lasOld,firstNew,lang,p2globals.nIncline);
			}
#else

#ifdef _LEO_LAST_
			if( p2_needLeo)
			{ // first - without LEO
				int saveLeo=p2_needLeo;
				p2_needLeo = 0;
				ret=RecogWord(firOld,lasOld,lineFon,fontinfo,p2globals.nIncline);
				p2_needLeo=saveLeo;
				// если ошибка - бряк
				if( ret < 0 )
				return ret;
			}

			// в p2globals установлен текущий язык
			p2_SetLanguage(CSTR_GetNext(firstNew),CSTR_GetLastRaster(lineFon),(uchar)p2globals.language);

			if( !p2_needLeo ||
					!GoodWordProb(CSTR_GetNext(firstNew),CSTR_GetLastRaster(lineFon),180) &&
					!GoodSpell(CSTR_GetNext(firstNew),CSTR_GetLastRaster(lineFon),useSpell?5:8)
			)

#endif  // LAST_LEO
			ret = RecogWord(firOld, lasOld, lineFon, fontinfo,
					p2globals.nIncline);

#endif  // _FON_GLUE_
		}

		// если ошибка - бряк
		if (ret < 0)
			return ret;

		// try clusters cut/glue
		if (p2globals.enable_smart_cut) {
			int brRet = BrokenRerecog(CSTR_GetNext(firstNew),
					CSTR_GetLastRaster(lineFon), lineRaw, FALSE);
			int glRet = GlueRerecog(CSTR_GetNext(firstNew), CSTR_GetLastRaster(
					lineFon), lineRaw, TRUE);

			//		   RerecogPalki(CSTR_GetNext(firstNew), CSTR_GetLastRaster(lineFon), lineRaw);
		}

		lastNew = CSTR_GetLastRaster(lineFon);

		// в p2globals установлен текущий язык
		p2_SetLanguage(CSTR_GetNext(firstNew), CSTR_GetLastRaster(lineFon),
				(uchar) p2globals.language);

		// сравним результаты распознавания, смешаем их или выберем лучший
		*firOld = CompareRecogVersions(first, last, CSTR_GetNext(firstNew),
				lastNew, isSolid, useSpell, fontinfo, outt, FALSE);

		first = *firOld;
		if (!(*firOld))
			ret = -1;

		break;
	default:
		break;
	}

	//  first=CSTR_GetNext(savFirst?savFirst:CSTR_GetFirstRaster(savLine));
	if (ret >= 0 && RSTR_p2_snap_activity('m') && p2_needStop(first, last))
		p2_ShowSnap(first, last);

	return ret;
}
////////////////////////
// язык подходит для ЛЕО ?
static Bool IsLeoLanguage(uchar lang) {
	if (lang == LANG_RUSSIAN && !p2globals.langBul && !p2globals.langSer)
		return TRUE;

#ifdef _ENGLISH_LEO_
	if (lang == LANG_ENGLISH)
		return TRUE;
#endif

	return FALSE;
}
////////////////////
// букву можно распознать ЛЕО ?
static Bool32 IsLeoStandardLetter(uchar let, uchar lang) {
	static char alpha_str[] =
			"ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ0123456789йцукенгшщзхъфывапролджэячсмитьбю#%+";
	static char latin_str[] =
			"QWERTYUIOPASDFGHJKLZXCVBNM0123456789qwertyuiopasdfghjklzxcvbnm#%+";

	if (lang == LANG_RUSSIAN) {
		if (!p2globals.langBul && !p2globals.langSer && !p2globals.langUkr)
			return TRUE;

		if (strchr(alpha_str, stdAsciiToAnsi(let)))
			return TRUE;

		return FALSE;
	}

#ifdef _ENGLISH_LEO_
	if (lang == LANG_ENGLISH)
		return TRUE;

	if (strchr(latin_str, let))
		return TRUE;

#endif

	return FALSE;
}
/////////////
// установить  язык по глобальной переменной language
static void SetRecogAlphabet(FontInfo *fontinfo) {
#ifdef _USE_LEO_
	if (RSTR_p2_NoStopSnapLEO()) {
#ifdef _ALL_LEO_
		if (p2_needLeo)
#else
		if( p2_needLeo && IsLeoLanguage((uchar)p2globals.language) )
#endif
		{
			static char alpha_str[] =
					"ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ0123456789йцукенгшщзхъфывапролджэячсмитьбю#%+";
			static char latin_str[] =
					"QWERTYUIOPASDFGHJKLZXCVBNM0123456789qwertyuiopasdfghjklzxcvbnm#%+";
			static char dig_str[] = "0123456789+";
			// remove from alpha_str  - "/^()"

			uchar *aa;
			int nFont = FONGetFontCount();

			if (p2globals.line_alphabet == ALPHA_DIGITAL_TRUE
					|| p2globals.line_alphabet == ALPHA_DIGITAL) {
				aa = (uchar *) (&dig_str[0]);
				specBadLeo = specBadNon;
				specVeryBadLeo = specBadNon;
			} else if (p2globals.language == LANG_RUSSIAN) {
				aa = (uchar *) (&alpha_str[0]);
				specBadLeo = specBadRus;
				specVeryBadLeo = specVeryBadRus;
			} else {
				aa = (uchar *) (&latin_str[0]);
				specBadLeo = specBadEng;
				specVeryBadLeo = specVeryBadEng;
			}

			// LEOSetupField
			ADDREC_SetupField(aa, nFont, fontinfo);
		}
	}
#endif

	// если слово из колонки - постараемся установить
	// алфавит и кластеры в соответствии с колонкой
	// иначе - заданный алфавит
	if (!p2_lineColumn || !FONSetAlphaColumn(p2_lineColumn,
			p2globals.line_alphabet == ALPHA_DIGITAL_TRUE)) {
		if (p2globals.line_alphabet == ALPHA_DIGITAL_TRUE
				|| p2globals.line_alphabet == ALPHA_DIGITAL) {
			char dig_fon[256];
			int i;
			memset(dig_fon, 0, 256);
			for (i = '0'; i <= '9'; i++)
				dig_fon[i] = 1;
			dig_fon['+'] = 1;
			FONSetAlphabet(dig_fon);
		} else {
			char alf[256];
			RSTR_p2_SetP2Alphabet(p2globals.language, alf);
			FONSetAlphabet(alf);
		}
	}
}
////////////
static void p2_SetLanguage(CSTR_rast first, CSTR_rast last, uchar lang)
// установим язык - Nick 8.01.2002
{
	CSTR_rast rast;
	CSTR_rast_attr attr;

	for (rast = first; rast && rast != last; rast = CSTR_GetNext(rast)) {
		CSTR_GetAttr(rast, &attr);
		attr.language = lang;
		CSTR_SetAttr(rast, &attr);
	}
}
//////////////////////
// перераспознать с учетом языка
static int32_t RerecogLang(CSTR_line lineRaw, CSTR_line lineFon,
		CSTR_rast *first, CSTR_rast last, uchar lang, FontInfo *fontinfo,
		Bool32 useSpell) {
	uchar sav_lang = p2globals.language;
	int32_t ret = 0;

	p2globals.language = lang;

	SetRecogAlphabet(fontinfo);

	// протестируем слово, возможно перераспознаем
	ret = p2_processWord(lineRaw, lineFon, first, last, fontinfo, useSpell);

	p2_SetLanguage(*first, last, lang);

	p2globals.language = sav_lang;
	return ret;
}
///////////////////////////
// проверять корректность расстановки пробелов
static Bool32 GoodSpace(CSTR_rast rast) {
	CSTR_rast cur;
	CSTR_rast_attr attrPrev, attrNext;

	cur = CSTR_GetNext(rast);
	if (!cur)
		return TRUE;
	if (!CSTR_GetAttr(cur, &attrNext))
		return TRUE;

	cur = CSTR_GetPrev(rast);
	if (!cur)
		return TRUE;
	if (!CSTR_GetAttr(cur, &attrPrev))
		return TRUE;

	if ((attrNext.flg & CSTR_f_space) || (attrPrev.flg & CSTR_f_space))
		return TRUE;

	if (attrNext.col > attrPrev.col + attrPrev.w + 1)
		return TRUE;

	if (!(attrPrev.cg_flag & CSTR_cg_cutr))
		return TRUE;

	if (!(attrNext.cg_flag & CSTR_cg_cutl))
		return TRUE;

	return FALSE; // сомнительный пробел
}
///////////////////
//   return >= 0  - OK
P2_FUNC(int32_t) p2_proc(CSTR_line lineRaw,CSTR_line lineOne,P2GLOBALS *p2glob)
{
	//answer ans[256],nans[256];

	CSTR_rast first=CSTR_GetFirstRaster(lineOne);
	CSTR_rast last;
	CSTR_rast firstNew; // in row lineFon
	CSTR_rast lastNew; // in row lineFon
	CSTR_rast_attr attr;
	CSTR_line lineFon;
	CSTR_attr lineAttr;
	int32_t ret=0;
	int32_t linVers=2;
	int32_t linNumber;
	FontInfo fontinfo;
	char alphaFON[256];
	RecVersions rv; // OLEG
	char word_limits[]="-"; // OLEG
	int32_t retSelect=0;
	//clock_t         cl1=clock();

	//return ret;

	//  RSTR_p2_GetP2Globals(&p2globals);
	memcpy(&p2globals,p2glob,sizeof(P2GLOBALS));

	linNumber = p2globals.line_number;

	p2_ShowWords=FALSE; // set TRUE in snap (if need)!
	memset(alphaFON,1,256);
	FONSetAlphabet(alphaFON);

	// font information
	FONFontInfo(&fontinfo);

	if(!CSTR_GetLineAttr ( lineOne, &lineAttr))
	{
		p2_stopColumn=-1;
		return ERR_GET_LINE;
	}
	if( lineAttr.Flags & CSTR_STR_SCALED)
	p2globals.line_scale=lineAttr.scale;
	else
	p2globals.line_scale=0;

	if( lineAttr.tab_number ) // OLEG
	p2globals.line_tabcell= lineAttr.tab_number;

	p2globals.line_alphabet = 0;
	p2_lineColumn=lineAttr.tab_column;

	if( lineAttr.Flags & CSTR_STR_Digital )
	p2globals.line_alphabet=ALPHA_DIGITAL_TRUE;
	else if( lineAttr.Flags & CSTR_STR_DigitalFuzzy )
	p2globals.line_alphabet=ALPHA_DIGITAL;

	// для разрезания - now before p2_proc
	//  RSTR_p2_setBasLines(lineRaw);

#ifdef _SAVE_IN_CTB_
	if(p2globals.line_number == 1)
	{
		static void p2_initCTB(char *nickName);
		p2_initCTB(nickName);
	}
#endif

	// создадим рабочую строку
	// с уникальным номером версии
	for(linVers=CSTR_LINVERS_PASS2;linVers<CSTR_LINVERS_PASS2+60;linVers+=10)
	{
		lineFon=CSTR_NewLine(linNumber,linVers,-1);
		if( lineFon) break;
		//      uint32_t   cstrErr=CSTR_GetReturnCode();
		//      cstrErr&=0xFFFF;
		//      if(cstrErr!=CSTR_ERR_DOUBLICATE) break;
	}

	if(!lineFon)
	{
		p2_stopColumn=-1;
		return ERR_NEW_LINE;
	}

	// for snap
	for(linVers=CSTR_LINVERS_PASS2SNAP;linVers<CSTR_LINVERS_PASS2SNAP+60;linVers+=10)
	{
		lineSnap=CSTR_NewLine(linNumber,linVers,-1);
		if( lineSnap) break;
	}

	// установим атрибуты - как у исходной
	CSTR_SetLineAttr ( lineFon, &lineAttr);
	if(lineSnap)
	CSTR_SetLineAttr ( lineSnap, &lineAttr);

	llRaw=(CSTR_head *)lineRaw;
	ll1 =(CSTR_head *)lineOne;
	ll2 =(CSTR_head *)lineFon;

	if( RSTR_p2_snap_activity('m') )
	RSTR_p2_snap_monitor_ori(&lineOne,1);

	firstNew=CSTR_GetFirstRaster(lineFon);

	if( p2globals.multy_language )
	{
		CSTR_rast dup=(CSTR_rast)NULL,dupend;
		//  CSTR_rast prevFir, prevDup;
		uchar lang;

		if( p2globals.line_number != prevLine + 1 )
		prevLang = -1;

		for(first=CSTR_GetNext(first);first;)
		{
			// найдем начало варианта - не пробел или имеющий
			// дубликат в английской строке
			if( !CSTR_GetAttr(first,&attr) )
			{	ret = ERR_GET_RAST;
				break;
			}

#ifdef GRAF
			dup=CSTR_GetNextDown(first);
#else
			dup=CSTR_GetDup(first);
#endif

			if( !dup && (attr.flg & CSTR_f_space ))
			{
				first=CSTR_GetNext(first);
				continue;
			}
			// OLEG
			CSTR_GetCollection(first,&rv);
			if( rv.lnAltCnt && strchr(word_limits,rv.Alt[0].Code) )
			{
				first=CSTR_GetNext(first);
				continue;
			}
			// OLEG

			lang=attr.language;

			if( dup)
#ifdef GRAF
			dupend=CSTR_GetPrevDown(first);
#else
			dupend=CSTR_GetDupEnd(first);
#endif
			else
			dupend=(CSTR_rast)NULL;

			// last - первый пробел ( или конец )
			for(last=CSTR_GetNext(first);!dupend && last; last=CSTR_GetNext(last))
			{
				if( dup ) // должна быть пара
				{
#ifdef GRAF
					if( (dupend=CSTR_GetPrevDown(last)) != (CSTR_rast)NULL )
#else
					if( (dupend=CSTR_GetDupEnd(last)) != (CSTR_rast)NULL )
#endif
					{
						last=CSTR_GetNext(last); // показывать после конца
						break;
					}
					continue;
				}

				if( !CSTR_GetAttr(last,&attr) )
				{	ret = ERR_GET_RAST;
					break;
				}
				if( (attr.flg & CSTR_f_space ) &&
						GoodSpace(last)
				)
				break;
				// OLEG
				CSTR_GetCollection(last,&rv);
				if( rv.lnAltCnt && strchr(word_limits,rv.Alt[0].Code) )
				break;
				// OLEG
				// если вдруг перед началом смешанного слова - нечто.
				// НЕ отбросим это нечто !?
#ifdef GRAF
				if( CSTR_GetNextDown(last) )
#else
				if( CSTR_GetDup(last) )
#endif
				break;
			}
			if( ret < 0) break;

			if( dup && !dupend )
			break;

			//  средний наклон букв
			p2_incline=0;

			retSelect = 1;

			if ( dup == NULL ) // один вариант языка
			{
				p2_needLeo = 1;
				ret = RerecogLang( lineRaw,lineFon,&first,last,lang,&fontinfo,TRUE);
				p2_needLeo = 0;
				if(ret < 0)
				break;
			}
			else // два языка
			{
				int p2_selectRusEng (CSTR_rast rusStart,CSTR_rast rusEnd,
						CSTR_rast engStart,CSTR_rast engEnd,
						FontInfo *fontinfo, int prevLang);
				Bool32 spel1,spel2;

				// пока не рискуем ...
				p2_needLeo = 0; // 1 - bad results

				if( RSTR_p2_snap_activity('m'))
				RSTR_p2_snap_show_text("Rerecog - multi Language.");

				//     prevFir=CSTR_GetPrevRaster(first,CSTR_f_all|CSTR_f_fict);
				//     prevDup=CSTR_GetPrevRaster(dup,CSTR_f_all|CSTR_f_fict);
				dupend =CSTR_GetNextRaster(dupend,CSTR_f_all|CSTR_f_fict);

				ret = RerecogLang( lineRaw,lineFon,&first,last,lang,&fontinfo,FALSE);
				if(ret < 0)
				break;
				// почистим lineFon
				for(lastNew=CSTR_GetNext(firstNew);lastNew;lastNew=CSTR_GetNext(firstNew))
				CSTR_DelRaster(lastNew);
				ret = RerecogLang( lineRaw,lineFon,&dup,dupend,(uchar)(lang==LANG_RUSSIAN?LANG_ENGLISH:LANG_RUSSIAN),&fontinfo,FALSE);
				if(ret < 0)
				break;

				p2_needLeo=0;

				//     first=CSTR_GetNext(prevFir);
				//rsadd_study_word(first,last,&rstat);
				//     dup=CSTR_GetNext(prevDup);
				//rsadd_study_word(dup,dupend,&estat);

				spel1 = GoodSpell(first,last,4);
				spel2 = GoodSpell(dup,dupend,4);

				if( !spel1 && spel2 ||
						spel1 == spel2 &&
						( ((retSelect=p2_selectRusEng (first,last,dup,dupend,&fontinfo,prevLang)) == 2) ||
								retSelect == 3 // по предыдущему
						)
				)
				{
					first = ReplaceWord(first,last,dup,dupend,0);
					if( !first )
					//!AddRastersLine(dup,dupend,prevFir,FALSE,0,0))
					{
						ret = -7;
						break;
					}
					// remove loop
					for(;dup && dup!=dupend;)
					dup=CSTR_DelRaster(dup);
				}
				else
				{
#ifdef GRAF
					// nothing need !!!???
#else
					CSTR_SetDup(first,dup);
					//dupend=CSTR_GetPrev(dupend);
					//if(last) first=CSTR_GetPrev(last);
					//else     first=CSTR_GetPrev(CSTR_GetLastRaster(lineOne));
					CSTR_SetDupEnd(LastRaster(first,last),LastRaster(dup,dupend));
#endif
				}

				// чтобы проверять русско-английский по F4 !
				FONSetAlphabet(alphaFON);

				if(RSTR_p2_snap_activity('m'))
				{
					//first=CSTR_GetNext(prevFir);
					p2_TextProb((CSTR_line)NULL,first,last,outt, 1000);
					strcat(outt,"- result multi");
					RSTR_p2_snap_show_text(outt);
					if( p2_needStop(first,last))
					p2_ShowSnap(first,last);
				}
			} // end two languages

			if( !first || !CSTR_GetAttr(first,&attr) )
			{	ret = ERR_GET_RAST;
				break;
			}

			// в конце строки ?
			if(!last)
			{
				prevLang = -1;
				break;
			}
			else
			{
				CSTR_GetCollection(last,&rv);
				// есть перенос ?
				if( rv.lnAltCnt && rv.Alt[0].Code == '-' &&
						(retSelect == 1 || retSelect == 2)
				)
				prevLang = attr.language;
				else
				prevLang = -1;
			}

			// если dup - last может указывать не на пробел,
			// а быть символом !
			first=last;
			dup=(CSTR_rast)NULL;

			// удалим старое
			for(lastNew=CSTR_GetNext(firstNew);lastNew;lastNew=CSTR_GetNext(firstNew) )
			CSTR_DelRaster(lastNew);

		} // loop through all words
	}

	else // один язык
	{
		p2_needLeo=1;
		SetRecogAlphabet(&fontinfo);
		p2_needLeo=0;

		for(first=CSTR_GetNext(first);first;first=CSTR_GetNext(first))
		{ // scan all string
			// search next word
			if( !CSTR_GetAttr(first,&attr) )
			{	ret = ERR_GET_RAST;
				break;
			}
			if( attr.flg & CSTR_f_space ) continue;

			// last - первый пробел ( или конец )
			for(last=CSTR_GetNext(first);last; last=CSTR_GetNext(last))
			{
				if( !CSTR_GetAttr(last,&attr) )
				{	ret = ERR_GET_RAST;
					break;
				}

				if( (attr.flg & CSTR_f_space ) &&
						GoodSpace(last)
				)
				break;
			}
			if( ret < 0) break;

			// из слова сделаем строку
			p2_incline=0;

			// протестируем слово, возможно перераспознаем
			p2_needLeo = 1;
			//  ret = RerecogLang( lineRaw,lineFon,first,last,language,&fontinfo,TRUE);
			ret = p2_processWord(lineRaw, lineFon,&first, last, &fontinfo,TRUE);
			p2_SetLanguage(first,last,(uchar)p2globals.language);

			p2_needLeo = 0;

			if(ret < 0)
			break;
			// в конце строки ?
			if(!last)
			break;
			first=last;

			//  firstNew=lastNew;   // если хотим все сцепить
			// удалим старое
			for(lastNew=CSTR_GetNext(firstNew);lastNew;lastNew=CSTR_GetNext(firstNew) )
			CSTR_DelRaster(lastNew);

		} // loop through all words
	}

	// запомним
	prevLine = p2globals.line_number;

	CSTR_DeleteLine(lineFon);
	if(lineSnap)CSTR_DeleteLine(lineSnap);
	lineSnap=(CSTR_line)NULL;

	p2_ShowWords=FALSE; // set TRUE in snap (if need)!
	p2_stopColumn=-1;

	// fallTime+=((float)(clock()-cl1))/CLOCKS_PER_SEC;
	memcpy(p2glob, &p2globals, sizeof(P2GLOBALS));
	return ret;
}
////////////////////////
static CSTR_rast FillAnswer(CSTR_line lineOut, CSTR_rast rst, RecVersions *rec,
		CSTR_rast prevRst) {
	CSTR_rast_attr attr;
	CSTR_rast rast;

	CSTR_GetAttr(rst, &attr);

	if (prevRst)
		rast = CSTR_InsertRaster(prevRst);
	else
		rast = CSTR_NewRaster(lineOut, attr.col, attr.row, attr.w);

	if (!rast)
		return NULL;

	if (!CSTR_CopyRaster(rast, rst))
		return NULL;

	// new versions!
	if (rec->lnAltCnt <= 0)
		attr.flg = CSTR_f_bad;
	else if (!(attr.flg & CSTR_f_punct))
		attr.flg = CSTR_f_let | (attr.flg & ~(CSTR_f_bad | CSTR_f_dust
				| CSTR_f_punct));

	if (!CSTR_SetAttr(rast, &attr) || !p2_StoreVersions(rast, rec))
		return NULL;
	return rast;
}
//////////
P2_FUNC(int32_t) p2_recog(RecRaster *recRast,RecVersions *vers,void *sinfo,int32_t testSelf)
{
	int nAlt=0;
	FonSpecInfo *specInfo=(FonSpecInfo *)sinfo;
	int32_t p2_testSelf(RecRaster *recRast,RecVersions *vers,FonSpecInfo *specInfo,int32_t testSelf);

	// поворот к исходному положению
	if( p2_incline != 0 )
	p2_rotateRecRaster(recRast, -p2_incline);

	if( testSelf )
	{
		nAlt = p2_testSelf(recRast,vers,specInfo,testSelf);
	}

	if( nAlt <= 0 )
	nAlt=FONRecogChar(recRast,vers,specInfo);

#ifdef _USE_LEO_
	{
		if( RSTR_p2_NoStopSnapLEO() )
		{
#ifdef _TEST_DIGITS_LEO_
			if( nAlt > 0 && vers->Alt[0].Code >= '0' &&
					vers->Alt[0].Code <= '9'
			)
			nAlt = 0;
#endif

#ifdef _ALL_LEO_
			if( p2_needLeo && ( nAlt <= 0 || vers->Alt[0].Prob < 180) &&
					specInfo && IsLeoStandardLetter((uchar)specInfo->nLet,(uchar)p2globals.language)
			)
#else
			if( p2_needLeo && IsLeoLanguage((uchar)p2globals.language) &&
					( nAlt <= 0 || vers->Alt[0].Prob < 180) )
#endif
			{
				RecVersions *res;
				int i, ii;
				int porog;
				uchar language = p2globals.language;

				memset(&ro,0,sizeof(RecObject));
				memcpy(&ro.recData.recRaster,recRast,sizeof(RecRaster));

				ro.recData.lwStatus=0;
				ADDREC_SetupPage(NULL); // LEO
				ADDREC_Recog(&ro); // LEO
				res=&ro.recResults;

				if( res->lnAltCnt <= 0 )
				return nAlt;

				// discrim some let
				if( (!specInfo || !specInfo->palkiLeo) &&
						p2globals.language != LANG_RUSSIAN &&
						(
								strchr("|!1li",res->Alt[0].Code) ||
								res->Alt[0].Code == liga_i ||
								(language == LANG_TURKISH && // 30.05.2002 E.P.
										(res->Alt[0].Code==i_sans_accent||
												res->Alt[0].Code==II_dot_accent
										)
								) ||
								res->Alt[0].Code == liga_exm
						)

				)
				return nAlt;

#ifdef _ALL_LEO_

				// не русский, не английский можем только подтвердить
				if( !IsLeoLanguage((uchar)p2globals.language ) &&
						stdAnsiToAscii(res->Alt[0].Code ) != (uchar)specInfo->nLet
				)
				return nAlt;

#endif

#ifndef _LEO_LAST_
				// надо либо распознавать и палки -
				// ( но в ЛЕО много ошибок на палки)
				// либо Ы распознавать только ФОНом
				if( p2globals.language == LANG_RUSSIAN &&
						strchr("Ьь",res->Alt[0].Code)
				)
				return nAlt;

				if( strchr(specVeryBadLeo,res->Alt[0].Code) )
				porog=POROG_GOOD_VERY_SPEC;
				else if( strchr(specBadLeo,res->Alt[0].Code) )
				porog=POROG_GOOD_SPEC;
				else
#endif
				porog=POROG_GOOD_LEO;

				if(
						res->Alt[0].Prob < porog ||
						nAlt > 0 && vers->Alt[0].Prob >= res->Alt[0].Prob ||
						res->Alt[0].Code=='|' ||
						res->lnAltCnt > 1 && res->Alt[1].Code=='|' &&
						res->Alt[0].Prob==res->Alt[1].Prob
				)
				return nAlt;

				for(ii=i=0;i<res->lnAltCnt;i++)
				{
					vers->Alt[ii]=res->Alt[i];
					vers->Alt[ii].Code=stdAnsiToAscii(res->Alt[i].Code);
					// owerwrite special methods leo
					vers->Alt[ii].Method=REC_METHOD_LEO;
					ii++;
					if( p2globals.language==LANG_RUSSIAN &&
							res->Alt[i].Code >= 194 &&
							!strchr("абеЕ",res->Alt[i].Code) ||
							p2globals.language != LANG_RUSSIAN &&
							strchr("CcOoPpSsVvWwXxZz",res->Alt[i].Code)
					)
					{
						vers->Alt[ii]=vers->Alt[ii-1];
						vers->Alt[ii].Code=p2_is_lowerASCII(vers->Alt[ii].Code)?
						p2_to_upperASCII(vers->Alt[ii].Code):p2_to_lowerASCII(vers->Alt[ii].Code);
						ii++;
					}
					if(ii >= REC_MAX_VERS-1)
					break;
				}
				vers->lnAltCnt = ii;

				//levcut(cell *C, int16_t arg);
				// нужно вызвать в строке cell-ов для помеченных как LEO
				return vers->lnAltCnt;
			}
		}
	}
#endif

#ifdef _SAVE_IN_CTB_
	CTB_AddRecRaster(nickName, recRast, (uchar)(vers->lnAltCnt <=0?'@':vers->Alt[0].Code));
#endif

	return nAlt;
}
////////////////////
P2_FUNC(int32_t) p2_rotate(RecRaster *recRast)
{
	if( p2_incline != 0 )
	p2_rotateRecRaster(recRast, -p2_incline);

	return p2_incline;
}
//////////////////
static int32_t TestSolidCluster(CSTR_rast first, CSTR_rast last) {
	RecVersions vers;
	CSTR_rast_attr attr;
	int32_t ret = 2;

	for (; first && first != last; first = CSTR_GetNext(first)) {
		if (!CSTR_GetAttr(first, &attr) || !CSTR_GetCollection(first, &vers))
			return 0;

		// не буква ?
		if (!(attr.flg & (CSTR_f_bad | CSTR_f_let)))
			continue;

		if (vers.lnAltCnt <= 0)
			return 0;

		if (!(attr.flg & CSTR_f_solid) && ((vers.Alt[0].Code >= 'A'
				&& vers.Alt[0].Code <= 'Z') || (vers.Alt[0].Code >= 'a'
				&& vers.Alt[0].Code <= 'z') || (vers.Alt[0].Code >= 128
				&& vers.Alt[0].Code < 176) || (vers.Alt[0].Code >= 224
				&& vers.Alt[0].Code < 240)))
			return 0;

		if (vers.Alt[0].Prob < 170)
			ret = 1;

		if (FONGetNumCluster(vers.Alt[0].Info) <= 0)
			ret = 1;
	}

	return ret; // все из хороших кластеров?
}
/////////////////
static const uchar kuskiBroken[] = "1!|[]()Il<>"; //\xba\xbc"; // r
/////////////////////
static Bool32 IsKusokBroken(uchar Code) {
	int32_t language = p2globals.language;

	if (strchr(kuskiBroken, Code) || Code == liga_i || // 19.06.2001
			Code == liga_exm || language == LANG_LITHUANIAN && (Code == 'i'
			|| Code == i_bottom_accent) ||
	// 30.05.2002 E.P.
			language == LANG_TURKISH && (Code == 'i' || Code == i_sans_accent
					|| Code == 'I' || Code == II_dot_accent || Code
					== II_roof_accent || Code == i_roof_accent))
		return TRUE;

	return FALSE;
}
///////////////////
// return 0 - no letters
//        1 - all solid
//        2 - some new
//        3 - need rerecog
//        4 - try FON cut/glue
//      < 0 - error
//

static int32_t CheckWord(CSTR_rast first, CSTR_rast last, CSTR_line lineOut,
		uchar *isSolid) {
	CSTR_rast rst;
	CSTR_rast rast; // new raster
	RecRaster recRast;
	int32_t nsym, // всего символов
			nlet = 0; // букв-цифр
	RecVersions verOld, verNew;
	CSTR_rast_attr attr;
	FonSpecInfo specInfo = { 0 };
	int32_t recRes = 0;
	int32_t ret = 2;
	int32_t porog;
	int32_t sumHei, i;
	uchar language = p2globals.language;
	int32_t porogFine;

	// Nick 10.07.2002
	if (language == LANG_TURKISH)
		porogFine = TRSFI + 10;
	else
		porogFine = TRSFI;

	*isSolid = 0;
	sumHei = 0;
	rast = NULL;

	for (rst = first, nsym = 0; rst && rst != last; rst = CSTR_GetNext(rst), nsym++) {
		if (!CSTR_GetAttr(rst, &attr) || !CSTR_GetCollection(rst, &verOld))
			return ERR_GET_RAST;

		// не буква ?
		if (!(attr.flg & (CSTR_f_bad | CSTR_f_let))) {
			verNew = verOld;
			//memset(&verNew,0,sizeof(verNew));
			for (i = 0; i < verOld.lnAltCnt; i++)
				verNew.Alt[i].Prob = MIN(verNew.Alt[i].Prob, POROG_BROKEN);
			rast = FillAnswer(lineOut, rst, &verNew, rast);

			if (p2globals.enable_smart_cut) {
				if (ret == 2 && (CSTR_GetNext(rst) != last || attr.h * 2* nlet
						> sumHei))
					ret = 4;
			}
			continue;
		}
		nlet++;

		sumHei += attr.h;

		// верим словарной проверке
		if (attr.flg & CSTR_f_solid) {
			if (nlet == 1) {
				int j;
				j = TestSolidCluster(rst, last);
				if (j == 0)
					*isSolid = 0;
				else
					*isSolid = 1;

				if (j == 2)
					ret = 1;
			}
		}
		//      else ci->flg &= ~c_f_solid; // clear solid mark


		if (!CSTR_GetImage(rst, (uchar*) &recRast, CSTR_TYPE_IMAGE_RS))
			return ERR_GET_RAST;

		// fill specInfo
		memset(&specInfo, 0, sizeof(FonSpecInfo));
		specInfo.nFieldRow = p2globals.line_number;
		specInfo.col = attr.col;
		specInfo.row = attr.row;
		if (verOld.lnAltCnt > 0) {
			specInfo.nInCTB = verOld.Alt[0].Info;
			specInfo.nLet = verOld.Alt[0].Code;
		}

		if (ret == 1)
			porog = POROG_SELF_SOLID;
		else
			porog = POROG_SELF_COMMON;

		if (ret == 1)
			specInfo.onlySelf = 1;
		else
			specInfo.onlySelf = 2;

#ifdef _LEO_LAST_
		{
			int save_p2Leo=p2_needLeo;
			// сначала попробовать перерезать без ЛЕО
			if( !(*isSolid))
			p2_needLeo=0;
#endif

		if (verOld.lnAltCnt > 0 && (verOld.lnAltCnt == 1 || verOld.Alt[0].Prob
				- porog > verOld.Alt[1].Prob))
			recRes = p2_recog(&recRast, &verNew, &specInfo, attr.clink);
		else
			recRes = p2_recog(&recRast, &verNew, &specInfo, 0);

#ifdef _LEO_LAST_
		p2_needLeo = save_p2Leo;
	}
#endif

		// плохо распознали - на перерезание
		if (verNew.lnAltCnt <= 0 || (verNew.Alt[0].Prob < porogFine
				&& verNew.Alt[0].Code != verOld.Alt[0].Code)
				|| verNew.Alt[0].Prob < TRSNOTBAD) {
			if (ret != 1)
				ret = 3;
		}

#ifdef _FON_2_PALKI_
		if (ret == 2 && p2globals.enable_smart_cut && verNew.lnAltCnt > 0
				&& IsKusokBroken(verNew.Alt[0].Code))
			ret = 4;
#endif
		// FON recognized
		if (verNew.lnAltCnt > 0 && verNew.Alt[0].Method == REC_METHOD_FON) {
			attr.clink = verNew.Alt[0].Prob;
			if (verNew.Alt[0].Code == verOld.Alt[0].Code && verNew.Alt[0].Prob
					>= TRSFI)
				attr.flg |= CSTR_f_confirmed;

			attr.nClust = specInfo.nClust;
		} else // LEO or nothing
		{
			attr.clink = 0;
			attr.nClust = 0;
			if (p2globals.enable_smart_cut && ret == 2)
				ret = 4;
		}

		CSTR_SetAttr(rst, &attr);

		// запомним результат распознавания
		rast = FillAnswer(lineOut, rst, &verNew, rast);
	}

	if (nlet <= 0)
		return 0; // just dust or special word

	return ret;
}
///////////////////////
static void MixtureAnswer(CSTR_rast first, CSTR_rast last, CSTR_rast firstNew,
		FontInfo *fontinfo) {
	CSTR_rast rst;
	RecVersions verOld, verNew;
	CSTR_rast_attr attr;

	//
	//
	for (rst = first; rst != last; rst = CSTR_GetNext(rst), firstNew
			= CSTR_GetNext(firstNew)) {
		if (!CSTR_GetAttr(rst, &attr) || !CSTR_GetCollection(rst, &verOld))
			return;

		// не буква ?
		if (!(attr.flg & (CSTR_f_bad | CSTR_f_let)))
			continue;

		if (!firstNew || !CSTR_GetCollection(firstNew, &verNew))
			return;

		if (verNew.lnAltCnt > 0)
			p2_leoMixture(rst, &verOld, &verNew, fontinfo);

	}

}
/////////////////////////////////
static void AddToRect(Rect32 *rect, CSTR_rast_attr *attr, CSTR_rast first) {
	if (!first) {
		rect->bottom = attr->row + attr->h;
		rect->top = attr->row;
		rect->right = attr->col + attr->w;
		rect->left = attr->col;
	} else {
		rect->bottom = MAX(rect->bottom, attr->row + attr->h);
		rect->top = MIN(rect->top, attr->row);
		rect->right = MAX(rect->right, attr->col + attr->w);
		rect->left = MIN(rect->left, attr->col);
	}
}
///////////////
// найти кусок в одной строке , соответствующий другой
static int FindEqualLine(CSTR_line lineRaw, CSTR_rast *firOld,
		CSTR_rast *lasOld, Rect32 *rect, int32_t bLeft, int32_t bRight,
		int32_t porog, int32_t naklon) {
	CSTR_rast_attr attr;
	CSTR_rast first, firNew;
	Rect32 newrect;
	int32_t nFind;

	bLeft = MAX(bLeft, rect->left - porog);

	*firOld = (CSTR_rast) NULL;
	firNew = NULL;

	first = CSTR_GetFirstRaster(lineRaw);
	if (!first)
		return 0;

	nFind = 0;
	for (first = CSTR_GetNext(first); first; first = CSTR_GetNext(first)) {
		if (!CSTR_GetAttr(first, &attr))
			return 0;

		if (naklon) {
			attr.row = attr.r_row - (int16_t)((int32_t) naklon * attr.r_col / 2048);
			attr.col = attr.r_col + (int16_t)((int32_t) naklon * attr.r_row / 2048);
		}

		if (attr.col < bLeft)
			continue;

		//          if( attr.row < rect->top - porog )
		//				continue;
		//			if( attr.row + attr.h > rect->bottom + porog )
		//				continue;

		if (attr.col >= bRight)
			break;
		if (attr.col + attr.w > rect->right + porog)
			break;

		AddToRect(&newrect, &attr, firNew);
		if (!firNew)
			firNew = first;
		nFind++;
	}

	*lasOld = first;

	if (!firNew)
		return 0;

	if (abs(newrect.left - rect->left) > porog ||
	//abs( newrect.top    - rect->top ) > porog ||
			abs(newrect.right - rect->right) > porog //||
	//abs( newrect.bottom - rect->bottom ) > porog
	)
		return 0;

	*firOld = firNew;
	return nFind;
}
/////////////////////////
static int RerecogInRect(Rect32 *rect, CSTR_line lineRaw, CSTR_rast firstNew,
		CSTR_rast lastNew, int lang) {
	FON_FUNC(int32_t) FONRecog2Glue(CSTR_rast firLeo,CSTR_rast lasLeo,
			CSTR_rast firNew,CSTR_rast lasNew,
			int lang, int porog, int nNaklon, int countRazmaz);
	int nOld;
	CSTR_rast firOld, lasOld;
	int ret;

	if (rect->right - rect->left < rect->bottom - rect->top) {
		// Nick 10.07.2002 - better for all lang ??? TO TEST
		if (p2globals.language == LANG_TURKISH) {
			if ((rect->right - rect->left) * 5 < (rect->bottom - rect->top) * 4)
				return 0;
		} else
			return 0;
	}

	if ((rect->right - rect->left) > (rect->bottom - rect->top) * 3)
		return 0;

	nOld = FindEqualLine(lineRaw, &firOld, &lasOld, rect, rect->left - 4,
			rect->right, 4, p2globals.nIncline);
	if (nOld <= 0) {
		// Nick 10.07.2002   - better for all ??? TO TEST
		if (p2globals.language == LANG_TURKISH) {
			firOld = firstNew;
			lasOld = lastNew;
		} else
			return 0;
	}

#ifdef _FON_GLUE_BROKEN_
	ret = FONRecog2Glue(firOld, lasOld, firstNew, lastNew, lang, TRSGLUFINE,
			p2globals.nIncline, nOld < 2 ? 1 : nOld == 2 ? 4 : 10);
#else
	ret = FONRecog2Glue( firOld,lasOld,firstNew, lastNew, lang,
			TRSGLUFINE, p2globals.nIncline, 1);
#endif

	return ret;
}
////////////
// try rerecog bad neighbours
static int GlueRerecog(CSTR_rast first, CSTR_rast last, CSTR_line lineRaw,
		Bool32 boAll) {

	CSTR_rast rst;
	RecVersions verOld;
	CSTR_rast_attr attr;
	RecVersions ver1;
	CSTR_rast firstNew;
	Rect32 rect;
	int nLet;
	int lang = p2globals.language;

	int firClink = 0;
	CSTR_rast prev;
	int nReGlue = 0;
	uint16_t firConf = 0;
	uchar firCut = 0;

	if (lang == LANG_ENGLISH && p2globals.multy_language)
		lang = LANG_RUSENG;

	//
	for (rst = first, nLet = 0, firstNew = NULL; rst && rst != last; rst
			= CSTR_GetNext(rst)) {
		if (!CSTR_GetAttr(rst, &attr) || !CSTR_GetCollection(rst, &verOld))
			return 0;

		// nick 17.05.2003
		if (attr.flg & CSTR_f_space)
			continue;

		if (attr.clink >= TRSFINE
		// || firstNew && attr.col - rect.right > attr.w
		) {
			if (!firstNew)
				continue;

			// не буква ?
			if (nLet <= 0 && !(firCut & CSTR_cg_cutl)) {
				firstNew = NULL;
				continue;
			}

			// try recut on 2
			if (rst && CSTR_GetCollection(rst, &ver1) && ver1.lnAltCnt > 0
					&& ver1.Alt[0].Code == 230) {
				AddToRect(&rect, &attr, firstNew);
				rst = CSTR_GetNext(rst);
			}

			if (firstNew != first) {
				prev = CSTR_GetPrev(firstNew);

				if (prev && ((firCut & CSTR_cg_cutl) || CSTR_GetCollection(
						prev, &ver1) && ver1.lnAltCnt > 0 && ver1.Alt[0].Code
						== 236)) {
					CSTR_GetAttr(prev, &attr);
					AddToRect(&rect, &attr, firstNew);
				} else
					prev = NULL;
			} else
				prev = NULL;

			if (RerecogInRect(&rect, lineRaw, prev ? prev : firstNew, rst, lang)
					<= 0)

			{
				if (firClink < TRSNOTBAD && !boAll)
					// || !(firConf& CSTR_f_confirmed) )
					return 0;
				rst = firstNew;
			} else
				nReGlue++;

			nLet = 0;
			firstNew = NULL;

			if (!rst || rst == last)
				break;

			continue;
		}

		AddToRect(&rect, &attr, firstNew);
		if (!firstNew) {
			firClink = attr.clink;
			firConf = attr.flg; // & CSTR_f_confirmed;
			firCut = attr.cg_flag; // CSTR_cg_cutr
			firstNew = rst;
		}

		if (attr.flg & (CSTR_f_bad | CSTR_f_let))
			nLet++;
	}

	if (firstNew) // test tail
	{
		if (firstNew != first) {
			prev = CSTR_GetPrev(firstNew);
			if (prev && ((firCut & CSTR_cg_cutl) || CSTR_GetCollection(prev,
					&ver1) && ver1.lnAltCnt > 0 && ver1.Alt[0].Code == 236)) {
				CSTR_GetAttr(prev, &attr);
				AddToRect(&rect, &attr, firstNew);
			} else
				prev = NULL;

		} else
			prev = NULL;

		if (RerecogInRect(&rect, lineRaw, prev ? prev : firstNew, rst, lang)
				<= 0)

		{
			if (firClink < TRSNOTBAD && !boAll) // || !(firConf& CSTR_f_confirmed) )
				return 0;
		} else
			nReGlue++;
	}

	return nReGlue;
}
/////////////////////////////////

static const uchar littlePuncts[] = ".,'";
static const uchar bigPuncts[] = ":;";
// try rerecog broken letters
static int BrokenRerecog(CSTR_rast first, CSTR_rast last, CSTR_line lineRaw,
		Bool32 single) {
	CSTR_rast rst;
	RecVersions verOld;
	CSTR_rast_attr attr;
	CSTR_rast firstNew;
	Rect32 rect;
	int nLet;
	int language = p2globals.language;
	CSTR_rast firOld, lasOld;
	int goodBrok = 0;
	int badBrok = 0;
	int porog = POROG_BROKEN; // TRSFINE
	int nBrok, nOld;
	Bool32 wasPunct;
	int initClink = 0;
	int leftBou;

	if (language == LANG_ENGLISH && p2globals.multy_language)
		language = LANG_RUSENG;
	//
	//
	for (rst = first, nLet = 0, firstNew = NULL; rst && rst != last;) {
		if (!CSTR_GetAttr(rst, &attr) || !CSTR_GetCollection(rst, &verOld))
			return 0;

		if (attr.clink >= TRSFINE) {
#ifdef _FON_2_PALKI_
			int32_t prob;

			if (IsKusokBroken(verOld.Alt[0].Code)) {
				prob = verOld.Alt[0].Prob;
				AddToRect(&rect, &attr, NULL);
				firstNew = rst;
				rst = CSTR_GetNext(rst);
				if (!rst || rst == last)
					break;

				if (!CSTR_GetAttr(rst, &attr) || !CSTR_GetCollection(rst,
						&verOld))
					return 0;

				if (IsKusokBroken(verOld.Alt[0].Code) || strchr(bigPuncts,
						verOld.Alt[0].Code)) {
					prob = MIN(prob, verOld.Alt[0].Prob);
					AddToRect(&rect, &attr, firstNew);
					rst = CSTR_GetNext(rst);
					if (RecogBrokenPalki(firstNew, rst, lineRaw, &rect,
							language, prob))
						goodBrok++;
					else
						rst = CSTR_GetNext(firstNew);
				}
			} else
#endif
				rst = CSTR_GetNext(rst);

			firstNew = NULL;
			continue;
		}

		if (attr.flg & (CSTR_f_punct | CSTR_f_dust))
			wasPunct = TRUE;
		else
			wasPunct = FALSE;

		firstNew = rst;
		AddToRect(&rect, &attr, NULL);
		initClink = attr.clink;

		rst = CSTR_GetNext(firstNew);
		nBrok = 1;
		for (; rst && rst != last; rst = CSTR_GetNext(rst)) {
			Rect32 tmpRect = rect;

			CSTR_GetAttr(rst, &attr);
			if (attr.clink >= TRSFINE && attr.col > rect.left + (rect.right
					- rect.left) / 3 && (p2globals.language != LANG_TURKISH
					|| attr.col > rect.left + (2* (rect .right-rect.left))/3
					)

					)
					break;

					AddToRect(&tmpRect, &attr, firstNew );
					if( attr.col > rect.right &&
							tmpRect.right - tmpRect.left > tmpRect.bottom-tmpRect.top
			  )
				break;

			nBrok++;
			rect = tmpRect;
		}

			if(nBrok == 1)
			{
				if( wasPunct == TRUE ) // Nick 09.07.2002 - for "->u,n
				porog = MAX(POROG_2PALKI,initClink);
				else
				porog = MAX(POROG_BROKEN,initClink);
			}
			else
			porog = POROG_BROKEN;

			{
				CSTR_rast leftRast=CSTR_GetPrev(firstNew);
				CSTR_rast_attr leftAttr;
				if( leftRast )
				{
					CSTR_GetAttr(leftRast, &leftAttr);
					leftBou = MIN(rect.left, leftAttr.col+leftAttr.w);
				}
				else
				leftBou = rect.left-4;
			}

			if( rect.right - rect.left < MAX_WID_BROKEN &&
					(rect.right - rect.left+1)*2 >= rect.bottom-rect.top &&
					((nOld=FindEqualLine(lineRaw,&firOld,&lasOld,&rect, leftBou,
											rst&&rst!=last?(rect.right+attr.col)/2:rect.right,
											4, p2globals.nIncline)) > 1 ||
							nOld == 1 && single &&
							(nBrok > 1 || verOld.lnAltCnt <= 0 || wasPunct && !strchr(littlePuncts,verOld.Alt[0].Code) )
					)
					&&
					FONRecogBroken( firOld,lasOld,firstNew, rst, language,
							nOld==1&&nBrok==1?porog+15:porog, p2globals.nIncline, 4) > 0
			)
			goodBrok++;
			else
			{
				int oldRight= rect.right;

				// test wider raster
				for(;rst && rst != last; rst=CSTR_GetNext(rst))
				{
					Rect32 tmpRect=rect;

					CSTR_GetAttr(rst,&attr);
					if( attr.clink >= TRSFINE )
					break;

					AddToRect(&tmpRect, &attr, firstNew );
					if( attr.col > rect.right &&
							(tmpRect.right - tmpRect.left)*2 > (tmpRect.bottom-tmpRect.top)*3
					)
					break;

					nBrok++;
					rect = tmpRect;
				}

				// not same raster & good
				if( rect.right > oldRight &&
						rect.right - rect.left < MAX_WID_BROKEN &&
						(rect.right - rect.left)*2 > rect.bottom-rect.top &&
						((nOld=FindEqualLine(lineRaw,&firOld,&lasOld,&rect,leftBou,
												rst&&rst!=last?(rect.right+attr.col)/2:rect.right, 4, p2globals.nIncline)) > 1 ||
								nOld == 1 && single &&
								(nBrok > 1 || verOld.lnAltCnt <= 0 || wasPunct && !strchr(littlePuncts,verOld.Alt[0].Code) )
						)
						&&
						FONRecogBroken( firOld,lasOld,firstNew, rst, language,
								nOld==1&&nBrok==1?porog+15:porog, p2globals.nIncline, 4) > 0
				)
				goodBrok++;
				else
				{
					// try next symbol
					if( rect.right - rect.left < MAX_WID_BROKEN )
					rst=CSTR_GetNext(firstNew);
					// else if many dust - skip

					if( !wasPunct || nBrok > 1 )
					{
						if( initClink < TRSNOTBAD )
						badBrok++;
					}
				}
			}
		}

		return goodBrok<=0?0:badBrok>0?1:2;
	}
	/////////////////////////////////

static int RecogBrokenPalki(CSTR_rast firLeo, CSTR_rast lasLeo,
		CSTR_line lineRaw, Rect32* rect, int lang, int32_t prob) {
	CSTR_rast firOld, lasOld;
	int rightBou;
	CSTR_rast_attr attr;

	if (rect->right - rect->left > rect->bottom - rect->top + 4)
		return 0;

	if (lasLeo) {
		CSTR_GetAttr(lasLeo, &attr);
		rightBou = MAX(rect->right, (rect->right + attr.col) / 2);
	} else
		rightBou = rect->right;

	if (FindEqualLine(lineRaw, &firOld, &lasOld, rect, rect->left - 2,
			rightBou, 2, p2globals.nIncline) < 1) {
		firOld = firLeo;
		lasOld = lasLeo;
	}

	prob = POROG_2PALKI + (prob > POROG_2PALKI ? (prob - POROG_2PALKI) / 2 : 0);

	if (FONRecogBroken(firOld, lasOld, firLeo, lasLeo, lang, prob,
			p2globals.nIncline, 4) > 0)
		return 1;

	return 0;
}
////////////
static int RerecogPalki(CSTR_rast first, CSTR_rast last, CSTR_line lineRaw) {
	CSTR_rast rst;
	RecVersions verOld;
	CSTR_rast_attr attr;
	CSTR_rast firstNew;
	Rect32 rect;
	int language = p2globals.language;
	int goodBrok;
	int32_t prob;

#ifndef _FON_2_PALKI_
	return 0;
#endif

	if (language == LANG_ENGLISH && p2globals.multy_language)
		language = LANG_RUSENG;

	for (rst = first, goodBrok = 0, firstNew = NULL; rst && rst != last;) {
		if (!CSTR_GetAttr(rst, &attr) || !CSTR_GetCollection(rst, &verOld))
			return 0;

		if (!IsKusokBroken(verOld.Alt[0].Code)) {
			rst = CSTR_GetNext(rst);
			continue;
		}

		prob = verOld.Alt[0].Prob;
		AddToRect(&rect, &attr, NULL);
		firstNew = rst;
		rst = CSTR_GetNext(rst);
		if (!rst || rst == last)
			break;

		if (!CSTR_GetAttr(rst, &attr) || !CSTR_GetCollection(rst, &verOld))
			return 0;

		if (!IsKusokBroken(verOld.Alt[0].Code)) {
			rst = CSTR_GetNext(rst);
			continue;
		}

		prob = MIN(prob, verOld.Alt[0].Prob);
		AddToRect(&rect, &attr, firstNew);
		rst = CSTR_GetNext(rst);
		if (RecogBrokenPalki(firstNew, rst, lineRaw, &rect, language, prob))
			goodBrok++;
		else
			rst = CSTR_GetNext(firstNew);
	}

	return goodBrok;
}
//////////////////
static Bool32 left(CSTR_rast curRast, int bound) {
	int x, w;
	CSTR_rast_attr attr;

	if (!CSTR_GetAttr(curRast, &attr))
		return FALSE;

	w = attr.w;
	if ((x = attr.col + w) <= bound)
		return TRUE;
	if (attr.col < bound && (x - bound) < w / 4)
		return TRUE;
	return FALSE;
}
//////////////////
static Bool32 right(CSTR_rast curRast, int bound, CSTR_rast nextRast) {
	CSTR_rast_attr attr, attrNext;

	if (!CSTR_GetAttr(curRast, &attr))
		return FALSE;

	if (attr.col >= bound)
		return TRUE;
	if (attr.col + attr.w <= bound)
		return FALSE;

	if (bound - attr.col < attr.w / 4)
		return TRUE;

	if (nextRast) {
		if (!CSTR_GetAttr(nextRast, &attrNext))
			return FALSE;

		if (attrNext.col >= attr.col + attr.w && bound - attr.col < attr.col
				+ attr.w - bound)
			return TRUE;

	}

	return FALSE;
}
/////////////////////////////
// найти кусок в одной строке , соответствующий другой
static void FindAccordLine(CSTR_line lineRaw, CSTR_rast *firOld,
		CSTR_rast *lasOld, CSTR_rast first, CSTR_rast last, int32_t naklon) {
	CSTR_rast_attr attr;
	int minX, maxX;

	*firOld = (CSTR_rast) NULL;

	if (!first)
		return;
	if (!CSTR_GetAttr(first, &attr))
		return;
	minX = attr.col;
	maxX = minX + attr.w;

	for (first = CSTR_GetNext(first); first && first != last; first
			= CSTR_GetNext(first)) {
		if (!CSTR_GetAttr(first, &attr))
			return;

		if (attr.flg & CSTR_f_space)
			continue;

		if (attr.col < minX)
			minX = attr.col;
		if (attr.col + attr.w > maxX)
			maxX = attr.col + attr.w;
	}

	first = CSTR_GetFirstRaster(lineRaw);
	if (!first)
		return;
	for (first = CSTR_GetNext(first); first; first = CSTR_GetNext(first)) {
		if (!CSTR_GetAttr(first, &attr)) {
			*firOld = NULL;
			return;
		}
		if (naklon) {
			attr.row = attr.r_row - (int16_t)((int32_t) naklon * attr.r_col / 2048);
			attr.col = attr.r_col + (int16_t)((int32_t) naklon * attr.r_row / 2048);
		}

		if (attr.col + attr.w - maxX >= maxX - attr.col && attr.col > minX)
			break;
		if (attr.col + attr.w - minX < minX - attr.col)
			continue;
		if ((*firOld) == (CSTR_rast) NULL)
			*firOld = first;
	}

	*lasOld = first;
}
/////////////////////////
//
// Поместить растры с fRast до eRast (не включая eRast)
//    в строку после rastOut
//
static CSTR_rast AddRastersLine(CSTR_rast fRast, CSTR_rast eRast,
		CSTR_rast rastOut, Bool emptyCollect, int nRotate, int nNaklon) {
	CSTR_rast_attr attr;
	CCOM_comp * comp;
	RecRaster rs;
	RecVersions vr;
	RecVersions emptyColl = { 0 };
	RecVersions *pVers;

	if (!rastOut)
		return (CSTR_rast) NULL;

	for (; fRast && fRast != eRast; fRast = CSTR_GetNextRaster(fRast,
			CSTR_f_all)) {
		if (CSTR_GetAttr(fRast, &attr) && CSTR_GetImage(fRast, (uchar *) (&rs),
				CSTR_TYPE_IMAGE_RS) && CSTR_GetCollection(fRast, &vr)
				&& !(rs.lnPixHeight && (comp = CSTR_GetComp(fRast)) == NULL)) // OLEG
		{
			// attr.language=p2globals.language;
			if (p2_font)
				attr.font = p2_font;
			if (emptyCollect) {
				if (attr.flg & CSTR_f_space)
					continue;
#ifdef _USE_DETOUCH_
				if (attr.flg & CSTR_f_detouch )
				attr.flg = CSTR_f_detouch|CSTR_f_bad;
				else
#endif
				attr.flg = CSTR_f_bad;
				pVers = &emptyColl;
			} else
				pVers = &vr;

			if (nRotate) {
				p2_rotateRecRaster(&rs, nRotate);
				attr.w = (uchar) rs.lnPixWidth << comp->scale;
				attr.save_stick_inc = -nRotate;
				attr.h = (uchar) rs.lnPixHeight << comp->scale; // OLEG
				// как будто так и было ...
				attr.stick_inc = NO_INCLINE; // 0 ?
				attr.pos_inc = CSTR_erect_no;
			}
			if (nNaklon) {
				attr.row = attr.r_row - (int16_t)((int32_t) nNaklon * attr.r_col
						/ 2048);
				attr.col = attr.r_col + (int16_t)((int32_t) nNaklon * attr.r_row
						/ 2048);
			}

			// нельзя NewRaster - вставится после пробела,
			// а надо до
			if (!(rastOut = CSTR_InsertRaster(rastOut)) || !CSTR_SetAttr(
					rastOut, &attr) || (rs.lnPixHeight && !CSTR_StoreRaster(
					rastOut, &rs)) || //OLEG
					!p2_StoreVersions(rastOut, pVers) || (rs.lnPixHeight
					&& !CSTR_StoreScale(rastOut, comp->scale)))// OLEG
				return (CSTR_rast) NULL;
		} else
			// error get
			return (CSTR_rast) NULL;
	}

	return rastOut;
}

//////////////////////
// не выйти за слово ! поэтому не CSTR_GetNextRaster(CSTR_f_let|CSTR_f_bad)
static CSTR_rast GetFirstLetter(CSTR_rast leoStart, CSTR_rast leoEnd) {
	CSTR_rast_attr attr;

	for (; leoStart && leoStart != leoEnd; leoStart = CSTR_GetNext(leoStart)) {
		if (!CSTR_GetAttr(leoStart, &attr))
			return (CSTR_rast) NULL;

		if (attr.flg & (CSTR_f_bad | CSTR_f_let | CSTR_f_punct | CSTR_f_dust))
			return leoStart;
	}
	return (CSTR_rast) NULL;
}
//////////
static Bool32 SomnitelnyjRazrez(CSTR_rast_attr *attrFon, RecVersions *vrFon,
		RecVersions *vrLeo) {
	if (!(attrFon->cg_flag & CSTR_cg_cutl))
		return FALSE;

	if (vrFon->Alt[0].Code == 163 && vrLeo->Alt[0].Code == 0xe2
			&& (vrFon->Alt[0].Prob < 50 || (vrFon->Alt[0].Prob - 30
					< vrLeo->Alt[0].Prob)))
		return TRUE;

	return FALSE;
}
////////// Paul 17-11-99
static Bool32 SomnitelnyjBlRazrez(CSTR_rast_attr *attrFon, RecVersions *vrFon,
		RecVersions *vrLeo) {
	int language = p2globals.language; // 01.06.2001 E.P.

	uchar let = vrLeo->Alt[0].Code;
	return attrFon->flg_new & CSTR_fn_bl_cut &&

	// Конфликтные коды. 16.07.2001 E.P.
			!is_baltic_language(language) && (
	//           vrFon->Alt[0].Code==(uchar)'®' &&	// 0xae liga_CR
			vrFon->Alt[0].Code == liga_CR && // Макрос 01.06.2001 E.P.
					(let == (uchar) 'Ў' || // 0xa1
							let == (uchar) 'а' || // 0xe0
							let == r_cu_d || let == r_cu_g)
					&& vrLeo->Alt[0].Prob > 150 || vrFon->Alt[0].Code
					== (uchar) 'Ї' && // 0xaf
					let == (uchar) 'ж' && // 0xe6
					vrLeo->Alt[0].Prob > 220);
}
///////////////////
static Bool32 testUkrKryshki(uchar leoName, uchar fonName) {
	if (p2globals.language == LANG_RUSSIAN && p2globals.langUkr) {
		if (fonName == 0xa9 && (leoName == 0xda || leoName == 0xc2 || leoName
				== 0x69))
			return FALSE;
	}

	return TRUE;
}
//////////////////
// return 0 - leo
//        1 - fon
//       <0 - error
//
static int SelectLeoFon(CSTR_rast leoStart, CSTR_rast leoEnd,
		CSTR_rast fonStart, CSTR_rast fonEnd, int nold, int nnew,
		FontInfo *fontinfo, Bool32 broken) {
	Bool32 TestPalka(CSTR_rast leoStart, CSTR_rast leoEnd, CSTR_rast fonStart,
			CSTR_rast fonEnd, int porog);
	Bool32 TestNewDust(CSTR_rast fonStart, CSTR_rast fonEnd,
			CSTR_rast leoStart, int porog);
	Bool32 TestWideLetter(CSTR_rast fonStart, CSTR_rast fonEnd,
			CSTR_rast leoStart);

	CSTR_rast_attr attrFon, attrLeo;
	RecVersions vrLeo;
	RecVersions vrFon;
	CSTR_rast lastLeo = (CSTR_rast) NULL;
	CSTR_rast lastFon = (CSTR_rast) NULL;
	int total = 0, nsb = 0, // not so bad
			better = 0, strong = 0;
	int minLeo = 255, minFon = 255;
	Bool newFon;
	int numFONnotLEO;
	int palkiNew = 0;

	// склеили букву из палок?
	if (nnew == 1 && TestPalka(leoStart, leoEnd, fonStart, fonEnd, 240))
		return 1;

	// не порезали на палки?
	if (nold == 1 && (palkiNew = TestPalka(fonStart, fonEnd, leoStart, leoEnd,
			(p2globals.language == LANG_RUSSIAN || p2globals.language
					== LANG_ENGLISH ? 215 : 199))) == 1)
		return 0;
	// новое - с дустами ?
	if (nold == 1 && TestNewDust(fonStart, fonEnd, leoStart, 22))
		return 0;
	// старая - склеена из сложных букв?
	if (nold == 1 && TestWideLetter(fonStart, fonEnd, leoStart))
		return 1;

	numFONnotLEO = 0;
	for (lastLeo = lastFon = NULL;; fonStart = CSTR_GetNextRaster(fonStart,
			CSTR_f_all)) {
		// берем FON - последний (если nold > nnew )
		if (!fonStart || fonStart == fonEnd) {
			newFon = FALSE;
			if (!lastFon)
				break;
			else
				fonStart = lastFon;
		} else
			newFon = TRUE;

		if (!CSTR_GetAttr(fonStart, &attrFon))
			return -3;

		if (!(attrFon.flg & (CSTR_f_bad | CSTR_f_let | CSTR_f_punct)))
			continue;

		if (!CSTR_GetCollection(fonStart, &vrFon))
			return -3;

		lastFon = fonStart; // текущий хороший символ

		for (; leoStart && leoStart != leoEnd; leoStart
				= CSTR_GetNext(leoStart)) {
			if (!CSTR_GetAttr(leoStart, &attrLeo))
				return -3;

			if (attrLeo.flg & (CSTR_f_bad | CSTR_f_let | CSTR_f_punct))
				break;
		}

		// есть новое ?
		if (!leoStart || leoStart == leoEnd) {
			// есть у FON новое ?
			if (!newFon)
				break;
			leoStart = lastLeo;
		}

		total++;
		if (vrFon.lnAltCnt > 0) {
			if (vrFon.Alt[0].Method == REC_METHOD_FON)
				numFONnotLEO++;
			if (vrFon.Alt[0].Prob > TRSFINE)
				strong++;
			else if (vrFon.Alt[0].Prob > TRSNOTBAD)
				nsb++;
			minFon = MIN(minFon, vrFon.Alt[0].Prob);
		} else
			minFon = 0;

		if (leoStart && leoStart != leoEnd) {
			lastLeo = leoStart;

			if (!CSTR_GetCollection(leoStart, &vrLeo))
				return -3;

			// лучше ли новая версия ?

			if (vrLeo.lnAltCnt <= 0) {
				minLeo = 0;
				if (vrFon.lnAltCnt > 0)
					better++;
			} else if (vrFon.lnAltCnt > 0) {
				minLeo = MIN(minLeo, vrLeo.Alt[0].Prob);
				// пунктуация лучше плохой буквы
				if (attrFon.flg & (CSTR_f_punct)) {
					if (vrLeo.Alt[0].Prob < POROG_PUNCT)
						better++;
				} else {
					// пунктуация лучше плохой буквы
					if (attrLeo.flg & (CSTR_f_punct)) {
						if (vrFon.Alt[0].Prob > POROG_PUNCT)
							better++;
					} else {
						Bool32 p2_leo_choise_fon_or_leo_absent(uchar p_fon,
								uchar p_leo);
						Bool32 (*p2_choice)(uchar fon, uchar leo);

						p2_choice
								= (fontinfo->count[vrLeo.Alt[0].Code] > 0 ? p2_leo_choise_fon_or_leo
										: p2_leo_choise_fon_or_leo_absent);

						if (!SomnitelnyjRazrez(&attrFon, &vrFon, &vrLeo)
								&& vrFon.Alt[0].Prob - 1 > vrLeo.Alt[0].Prob
								&& p2_choice(vrFon.Alt[0].Prob,
										vrLeo.Alt[0].Prob) && (!(attrLeo.flg
								& CSTR_f_confirmed) || attrLeo.clink
								< vrFon.Alt[0].Prob - 3 || vrFon.Alt[0].Code
								== vrLeo.Alt[0].Code) && testUkrKryshki(
								vrLeo.Alt[0].Code, vrFon.Alt[0].Code)
								&& !SomnitelnyjBlRazrez(&attrFon, &vrFon,
										&vrLeo))
							better++;
					}
				}

			}

			// to next leo raster
			leoStart = CSTR_GetNextRaster(leoStart, CSTR_f_all);
		} else if (vrFon.lnAltCnt > 0)
			better++;
	}

	if (total <= 0)
		return 0;

	if (((strong > 0 || minFon > minLeo + 60) && total == strong + nsb
			&& ((better << 1) > total || (better << 1) == total && numFONnotLEO
					> 0 //&& !broken
			)) || better >= total || palkiNew == 2) {
		// проверка - может новая версия как старая, только еще dust
		// тогда применим смесь версий 1-1
		if (nold == 1 && total == 1 && lastFon && lastLeo) {
			if (!CSTR_GetAttr(lastFon, &attrFon) || !CSTR_GetCollection(
					lastFon, &vrFon))
				return -3;
			if (!CSTR_GetAttr(lastLeo, &attrLeo) || !CSTR_GetCollection(
					lastLeo, &vrLeo))
				return -3;
			if (attrFon.col == attrLeo.col && attrFon.w == attrLeo.w)
				// смешаем версии, поместим в lastFon
				p2_leoMixture(lastFon, &vrLeo, &vrFon, fontinfo);
		}

		return 1; // fon
	}

	return 0; // leo
}
//////////////////////
//  remake from p2_proc.c
static int16_t findWordBound(CSTR_rast *nextRast, CSTR_rast endRaster,
		int bound) {
	CSTR_rast_attr attr;
	int num = 0;
	CSTR_rast curRast = *nextRast;

	for (num = 0; curRast && curRast != endRaster; curRast
			= CSTR_GetNextRaster(curRast, CSTR_f_all)) {
		if (!CSTR_GetAttr(curRast, &attr))
			return -3;
		//  не рассматриваем
		if (attr.flg & CSTR_f_space)
			continue;

		// слева от границы ?
		if (attr.col + attr.w > bound)
			break;
		//    if( !left(curRast,bound) )
		num++;
	}
	*nextRast = curRast;

	return num;
}
//////////
//
// error codes : -1 - no memory
//               -3 - error GetAttributes,GetCollection
//               -4 - error Get/Set Raster
//
// сравнить варианты и поместить в (или заместить) [*oldStart,lEnd)
static int32_t composeWords(CSTR_rast fStart, CSTR_rast fEnd,
		CSTR_rast *oldLStart, CSTR_rast lEnd, FontInfo *fontinfo, Bool32 broken) {
	CSTR_rast curLeo, curFon;
	CSTR_rast nextLeo, nextFon;
	CSTR_rast startLeo, startFon;
	CSTR_rast_attr attrLeo, attrFon;
	int nold, nnew, i;
	//CSTR_line       lin_out= CSTR_GetRasterLine(lStart);
	int endLeoWord = 0;
	int fonRight, leoRight;
	CSTR_rast lStart = *oldLStart;

	if (!fStart || !lStart)
		return 0;

	// найдем границу слова
	curLeo = LastRaster(lStart, lEnd);
	if (curLeo) {
		CSTR_GetAttr(curLeo, &attrLeo);
		endLeoWord = attrLeo.col + attrLeo.w;
	}

	for (nextLeo = lStart, nextFon = fStart;;) {
		// to first letter or unrecognized
		curLeo = GetFirstLetter(nextLeo, lEnd);
		curFon = GetFirstLetter(nextFon, fEnd);

		// нет ничего нового ?
		if (!curFon)
			return 1;

		// не осталось старого
		if (!curLeo) {

			curLeo = LastRaster(*oldLStart, lEnd);
			//curLeo=CSTR_GetPrev(lEnd);
			if (!curLeo)
				return 0;

			// не ушли за границы слова?
			if (endLeoWord) {
				CSTR_GetAttr(curFon, &attrFon);
				if (attrFon.col >= endLeoWord - 1)
					return 1;
			}

			// добавим новое
			if (!AddRastersLine(curFon, fEnd, curLeo, FALSE, -p2_incline, 0))
				return -7;
			return 1;
		}

		if (!CSTR_GetAttr(curLeo, &attrLeo) || !CSTR_GetAttr(curFon, &attrFon))
			return -3;

		// один символ слева от другого ?
		if (attrLeo.col + attrLeo.w <= attrFon.col || attrLeo.col + attrLeo.w
				- attrFon.col < attrFon.col - attrLeo.col) {
			nextLeo = CSTR_GetNext(curLeo);
			if ((i = findWordBound(&nextLeo, lEnd, attrFon.col)) < 0)
				return i;
			// nextFon - stay old
			continue;
		}

		if (attrFon.col + attrFon.w <= attrLeo.col) {
			nextFon = CSTR_GetNext(curFon);
			if ((i = findWordBound(&nextFon, fEnd, attrLeo.col)) < 0)
				return i;

			// до начала слова - пропускаем
			if (curLeo == lStart)
				continue;

			// добавим новое
			curLeo = LastRaster(*oldLStart, nextLeo);
			if (!AddRastersLine(curFon, nextFon, curLeo, FALSE, -p2_incline, 0))
				return -7;
			// next Leo - остается прежним
			continue;
		}

		//  find multi-words - near same bounds
		startLeo = curLeo;
		startFon = curFon;
		nextLeo = GetFirstLetter(CSTR_GetNext(curLeo), lEnd);
		nextFon = GetFirstLetter(CSTR_GetNext(curFon), fEnd);

		leoRight = attrLeo.col + attrLeo.w;
		fonRight = attrFon.col + attrFon.w;
		for (nold = 1, nnew = 1;;) {
			CSTR_rast_attr attrNextLeo, attrNextFon;
			int fRight, lRight;

			if (nextLeo)
				CSTR_GetAttr(nextLeo, &attrNextLeo);
			if (nextFon)
				CSTR_GetAttr(nextFon, &attrNextFon);

			if ((!nextLeo || right(nextLeo, fonRight, nextFon)) && (!nextFon
					|| right(nextFon, leoRight, nextLeo)))
				break;

			if ((fonRight == leoRight) && nextLeo && nextFon && attrNextLeo.col
					== attrNextFon.col)
				break;

			if (nextLeo)
				lRight = MAX(leoRight, attrNextLeo.col + attrNextLeo.w);
			if (nextFon)
				fRight = MAX(fonRight, attrNextFon.col + attrNextFon.w);

			// кого добавить ?
			if ((nextLeo && nextFon && (lRight < fRight || lRight == fRight
					&& leoRight < fonRight)) || !nextFon
			//        ((!nextLeo  || !nextFon) && leoRight < fonRight )
			) {
				if (!nextLeo)
					break;
				nold++;
				curLeo = nextLeo;
				if (!CSTR_GetAttr(curLeo, &attrLeo))
					return -3;
				leoRight = MAX(leoRight, attrLeo.col + attrLeo.w);
				nextLeo = GetFirstLetter(CSTR_GetNext(curLeo), lEnd);
			} else if ((nextLeo && nextFon && (fRight < lRight || lRight
					== fRight && leoRight >= fonRight)) || !nextLeo) {
				if (!nextFon)
					break;
				nnew++;
				curFon = nextFon;
				if (!CSTR_GetAttr(curFon, &attrFon))
					return -3;
				fonRight = MAX(fonRight, attrFon.col + attrFon.w);
				nextFon = GetFirstLetter(CSTR_GetNext(curFon), fEnd);
			} else
				break;
		}

		//  if(firWord) // store spaces & dust before first Letter
		//  {
		//      firWord=FALSE;
		//        if( !AddRastersLine( CSTR_GetNextRaster(lStart, CSTR_f_all),startLeo, lin_out) )
		//          return -4;
		//  }

		if (!nextFon)
			nextFon = fEnd;
		if (!nextLeo)
			nextLeo = lEnd;
		//  как при обычном LEO - смесь
		if (nold == 1 && nnew == 1) {
			RecVersions vrLeo, vrFon;
			curFon = startFon;
			curLeo = startLeo;

			if (!CSTR_GetAttr(curFon, &attrFon) || (attrFon.flg & CSTR_f_dust))
				continue;
			if (!CSTR_GetAttr(curLeo, &attrLeo) || (attrLeo.flg & CSTR_f_dust)
			// old - was broken
					|| (p2globals.enable_smart_cut
							&& (attrFon.flg & CSTR_f_let) && (attrLeo.flg
							& CSTR_f_punct) && attrFon.clink >= POROG_PUNCT))
				i = 1;

			else {
				if (!CSTR_GetCollection(curLeo, &vrLeo) || !CSTR_GetCollection(
						curFon, &vrFon))
					continue;

				// смешаем версии, поместим в curLeo
				i = p2_leoMixture(curLeo, &vrLeo, &vrFon, fontinfo);

				if (i == 1) // картинку и версии взять из fon
				{
					CSTR_GetCollection(curLeo, &vrFon);
					p2_StoreVersions(curFon, &vrFon);

					// установить атрибуты распознавания
					CSTR_GetAttr(curLeo, &attrLeo);
					attrFon.recsource = attrLeo.recsource;
					attrFon.RecogHistory = attrLeo.RecogHistory;
					CSTR_SetAttr(curFon, &attrFon);
				}
			}
		}

		//    <->
		else
			// не смешиваем версии, а берем одну из двух
			i = SelectLeoFon(startLeo, nextLeo, startFon, nextFon, nold, nnew,
					fontinfo, broken);

		if (i < 0)
			return i;

		if (i == 0) // оставим как было
			continue;

		// заменим Leo на Fon
		curLeo = ReplaceWord(startLeo, nextLeo, startFon, nextFon, -p2_incline);

		if (!curLeo)
			return -7;

		// заменили первый растр ?
		if (startLeo == *oldLStart)
			*oldLStart = curLeo;

	} // end for

	/*
	 if(c1->flg & (c_f_confirmed|c_f_solid)) goto cont; // already confirmed
	 if(c2->nvers > 1 && c2->vers[0].prob - c2->vers[1].prob <= 20)
	 mixture(c2,c1);// special case probs are very close

	 if(c1->vers[0].prob > c2->vers[0].prob+30) goto cont; // nick

	 if( !(c1->cg_flag & c_cg_cut) && c2->cg_flag & c_cg_cut)
	 if( c2->cg_flag & c_cg_cutl )
	 if( (c2->prev->flg & (c_f_punct|c_f_dust)) &&
	 (c2->prev->cg_flag & c_cg_cutr)
	 ) goto cont;
	 else if( c2->cg_flag & c_cg_cutr )
	 if( (c2->next->flg & (c_f_punct|c_f_dust)) &&
	 (c2->next->cg_flag & c_cg_cutl)
	 ) goto cont;
	 */

	return 1;
}
///////////////////////////

static uchar p2_getFont(CSTR_rast first, CSTR_rast last) // OLEG
{
	CSTR_rast_attr attr;
	uchar font;
	if (!CSTR_GetAttr(first, &attr))
		return 0;
	font = attr.font;
	for (; first && first != last; first = CSTR_GetNext(first)) {
		if (!CSTR_GetAttr(first, &attr))
			continue;
		if (font != attr.font)
			return 0;
	}
	return font;
}
///////////////////////
static int p2_getIncline(CSTR_rast first, CSTR_rast last) {
	int num = 0;
	int summa = 0;
	CSTR_rast_attr attr;
	int maxY = 0;

	for (; first && first != last; first = CSTR_GetNext(first)) {
		if (!CSTR_GetAttr(first, &attr))
			continue;
		// только для букв и нераспознанных ?
		if (!(attr.flg & (CSTR_f_let | CSTR_f_bad)))
			continue;

		num++;
		if (attr.h > maxY)
			maxY = attr.h;

		//if(attr.stick_inc != NO_INCLINE)
		//       summa+=attr.stick_inc;

		// Oleg ubivaet stick_inc ( erection_delete )
		if (attr.save_stick_inc != NO_INCLINE)
			summa += attr.save_stick_inc;
	}

	if (num <= 0)
		return 0;

	if ((maxY * summa) / num <= 2048)
		return 0;

	return summa / num;
}
///////////////
// распознать компоненту в линейном представлении
// возврат - номер лучшего кластера
P2_FUNC(int32_t) p2_RecogCompLp(int16_t sizeLp,uchar *lp,int16_t w,int16_t h,
		int16_t col,int16_t row,
		RecVersions *vers)
{
	int32_t i;
	RecRaster recRast;
	FonSpecInfo specinfo;

	vers->lnAltCnt = 0;
	if( !lp )
	return 0;

	//  if(!p2_Line2Raster( comp, &recRast) )
	if( !p2_Comp2Raster(sizeLp,lp,w,h,&recRast))
	return 0;

	// поворот к исходному положению - теперь в p2_recog()
	//  if( p2_incline != 0 )
	//        p2_rotateRecRaster(&recRast, -p2_incline);

	memset(&specinfo,0,sizeof(specinfo));
	specinfo.col=col;
	specinfo.row=row;

	specinfo.palkiLeo = 1;
	i=p2_recog(&recRast,vers,&specinfo,0);

	return specinfo.nClust;
}
////////////////////////
static int32_t RecogWord(CSTR_rast first, CSTR_rast last, CSTR_line lineFon,
		FontInfo *fontinfo, int nNaklon) {
	int ret = 0;

	// есть наклон ?
	if (p2_incline || nNaklon) {
		CSTR_rast fir, las;

		// удалим старое содержимое lineFon
		fir = CSTR_GetFirstRaster(lineFon);
		for (las = CSTR_GetNext(fir); las; las = CSTR_GetNext(fir))
			CSTR_DelRaster(las);

		// поместим повернутые в строку lineFon
		if (AddRastersLine(first, last, CSTR_GetFirstRaster(lineFon), FALSE,
				p2_incline, nNaklon) == NULL)
			return -3;

		// будем работать с новой строкой
		first = CSTR_GetNext(CSTR_GetFirstRaster(lineFon));
		last = CSTR_GetLastRaster(lineFon);
	}

	// передадим параметры в RSTR
	ret = RSTR_p2_RecogCutGlu(first, last, lineFon, &p2globals);

	// проверим акценты
	{
		int32_t p2_testAccents(CSTR_rast first, CSTR_rast last);
		p2_testAccents(CSTR_GetNext(CSTR_GetFirstRaster(lineFon)), NULL);
	}

	// параметры из RSTR  - вдруг что-то изменилось?
	//  RSTR_p2_GetP2Globals(&p2globals);

	return ret;
}
/////////////
void p2_init(void) {
	return;
}
//////////
P2_FUNC(void) p2_SetShowWords(Bool32 val)
{
	p2_ShowWords=val;
}
/////////
P2_FUNC(Bool32) p2_GetShowWords(void)
{
	return p2_ShowWords;
}
/////////
#ifdef _SAVE_IN_CTB_
static void p2_initCTB(char *nickName)
{
	if(nickName[6]=='9')
	nickName[6]='A';
	else if(nickName[6]=='Z')
	{
		if(nickName[5]=='9')
		nickName[5]='A';
		else
		nickName[5]++;

		nickName[6]='0';
	}
	else
	nickName[6]++;

	{ // save bases - i(nit) ctb
		//              f(ont) ctb
		char loc_name[256]="ct666666";
		char gre_name[256]="page6666";
		nickName[7]='i';
		nickName[8]=0;
		CTB_copy(nickName,loc_name);
		nickName[7]='f';
		CTB_copy(nickName,gre_name);
		nickName[7]=0;
	}

	CTB_create(nickName,NULL);
}
///////////////
#endif

P2_FUNC(void) p2_SetStopBound(int stopCol)
{
	p2_stopColumn=stopCol;
}
/////////////////
static Bool32 p2_needStop(CSTR_rast first, CSTR_rast last) {
	CSTR_rast_attr attr;

	if (p2_stopColumn < 0)
		return TRUE;
	if (!first)
		return FALSE;
	if (!CSTR_GetAttr(first, &attr))
		return FALSE;
	if (p2_stopColumn < attr.col)
		return FALSE;
	if (!last)
		last = CSTR_GetLastRaster(CSTR_GetRasterLine(first));
	if (!last)
		return FALSE;
	last = CSTR_GetPrev(last);
	if (!last)
		return FALSE;
	if (!CSTR_GetAttr(last, &attr))
		return FALSE;
	if (p2_stopColumn > attr.col)
		return FALSE;
	p2_stopColumn = -1;
	return TRUE;
}
////////////////
P2_FUNC(Bool32) p2_stopPlace(void)
{
	if( p2_stopColumn < 0)
	return FALSE;
	return TRUE;
}
//////////////////
#ifdef _USE_DETOUCH_
// проставить метки в сырой строке для уменьшения
// работы при разрезании
static int mark_strong_letters(CSTR_rast fRecog,CSTR_rast lRecog,CSTR_rast fRaw,CSTR_rast lRaw)
{
	int marked,le,ri;
	CSTR_rast_attr attr;

	for(marked=0,le=-1; fRecog && fRecog!=lRecog;fRecog=CSTR_GetNext(fRecog))
	{
		CSTR_GetAttr(fRecog,&attr);

		if( attr.flg & CSTR_f_confirmed )
		{
			if(le < 0) le = attr.r_col;
			ri = attr.r_col+attr.w;
			continue;
		}

		if(le < 0)
		continue;

		for(; fRaw && fRaw != lRaw; fRaw=CSTR_GetNext(fRaw))
		{
			if( !right(fRaw,le,NULL) )
			continue;
			if( !left(fRaw,ri) )
			break;

			CSTR_GetAttr(fRaw,&attr);
			attr.flg |= c_f_detouch;
			CSTR_SetAttr(fRaw,&attr);
			marked++;
		}
		le=-1;
	}

	// test tail
	if( le > 0 )
	{
		for(; fRaw && fRaw != lRaw; fRaw=CSTR_GetNext(fRaw))
		{
			if( !right(fRaw,le,NULL) )
			continue;
			if( !left(fRaw,ri) )
			break;

			CSTR_GetAttr(fRaw,&attr);
			attr.flg |= c_f_detouch;
			CSTR_SetAttr(fRaw,&attr);
			marked++;
		}
	}

	return marked;
}
#endif
//////////////////////
