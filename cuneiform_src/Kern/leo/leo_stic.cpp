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

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cctype>
#include <cassert>
#include <cmath>
#include <algorithm>

#define ERECT_ENABLE 0

#include "evn.h"
#include "leo.h"
#include "dif.h"
#include "inc_tab.h"
#include "std.h"
#include "alphaset.h"
#include "leo_func.h"

// extern data
extern unsigned char alphabet[];
extern uchar nIsPrint;
extern int32_t leo_typ_of_font;

// from module LEO.C
extern uchar leo_alpha_type, prn_roma_regim;
// для осмысленности assert
int32_t try_call_vec = 0;
int32_t leo_stick_nose_1;
int leo_incline = ERECT_ENABLE;
static int leo_av_inc = 0, leo_av_inc_n = 0, leo_av_inc1 = 0,
		leo_av_inc_n1 = 0, is_pool = 0, save_w, save_h;
static uint16_t save_pool[4096];

static int leo_current_slash_level(void);
static int leo_current_1_level(void);
static Bool32 leo_stick_thin_prop(RecRaster *r, int ang[], int num, int den);
static Bool32 leo_stick_thin(RecRaster *r, int inc, int num, int den);
static Bool32 leo_stick_make_tab(int inc, int h, int tab_angle[]);
static void leo_save_rl(uint16_t *lpool);
static int32_t leo_num_of_long_sticks(RecVector *vSticks, int Cnt, int h);
static Bool32 leo_wide_stick(RecVector *vSticks, int Cnt, int h, int w);
static Bool32 leo_test_inclinable(RecVersions *v);
static int leo_get_first_pixel(uchar *r, int d);
static int leo_get_last_pixel(uchar *r, int d);
static int leo_shift_raster(uchar *raster, int dx, int dy, int tab_angle[]);
static int leo_current_slash_level(void);
static int32_t leo_get_incline(RecObject* object);

Bool32 similar_i(RecRaster *rs) {
	int w = rs->lnPixWidth, wb, h = rs->lnPixHeight, i, le, ri, lem, rim;
	uchar *r = rs->Raster;

	wb = REC_GW_WORD8(rs->lnPixWidth);
	for (lem = rim = 256, i = 0; i < h; i++, r += wb) {
		le = DIFLeftDistance(r, (int16_t) wb);
		if (le == -1)
			break;
		ri = rs->lnPixWidth - wb * 8 + DIFRightDistance(r, (int16_t) wb);
		if (lem > le)
			lem = le;
		if (rim > ri)
			rim = ri;
	}

	return abs(i - w) <= std::max(w / 4, 2)
				|| (rim != 256 && lem != 256 && abs(i - w + rim + lem) < std::max(w / 4, 2))
				|| (i > 3 && i < h / 5); // similar to square
}

Bool32 leoRecogSimpleStick(RecObject* object) {
	Bool32 ret = FALSE;
	if (!nIsPrint) {
		ret
				= ((leo_alpha_type == ALPH_ROM ? 1 : object->recData.lwCompCnt
						== 1) && object->recData.recRaster.lnPixWidth * 4
						<= object->recData.recRaster.lnPixHeight);
	} else {
		if (object->recData.lwCompCnt == 1
				&& object->recData.recRaster.lnPixWidth * 3
						<= object->recData.recRaster.lnPixHeight)
			ret = TRUE;
		if (object->recData.lwCompCnt == 2
				&& object->recData.recRaster.lnPixWidth * 4
						<= object->recData.recRaster.lnPixHeight && similar_i(
				&object->recData.recRaster))
			ret = TRUE;
		if ((leo_typ_of_font & (LEO_FONT_MTR))
				&& object->recData.recRaster.lnPixWidth * 4
						<= object->recData.recRaster.lnPixHeight)
			ret = TRUE;
	}

	return ret;
}

Bool32 leo_stick_make_tab(int inc, int h, int tab_angle[]) {
	int i, ainc = inc > 0 ? inc : -inc;

	memset(tab_angle, 0, h * sizeof(int));
	if (((h - 1) * ainc) < 2048)
		inc = 0;
	if (inc == 0)
		return FALSE; // zero skew
	for (i = 0; i < h; i++)
		tab_angle[i] = ((h - 1 - i) * inc) / 2048;
	return TRUE; // normal skew
}
static int hist_wid[128], hist_le[128], hist_ri[128], hist_wi[128],
		hist_num[128];
static int32_t slash_level = 700;

int32_t leo_stick_horiz_hist(int h) {
	int s, i;
	for (s = i = 0; i < h; i++)
		s += (hist_num[i] > 1);
	return s;
}

static Bool32 leo_stick_thin_prop(RecRaster *r, int ang[], int num, int den) {
	int h = r->lnPixHeight, i, le, ri;
	for (le = 32000, ri = -32000, i = 1; i < h - 1; i++) {
		if (hist_le[i] != -1 && le > hist_le[i] + ang[i])
			le = hist_le[i] + ang[i];
		if (hist_ri[i] != -1 && ri < hist_ri[i] + ang[i])
			ri = hist_ri[i] + ang[i];
	}
	return ((ri - le) * den < h * num);
}

// !!! Call after leo_recog_stick ONLY! Used global historgamms !!!
static Bool32 leo_stick_thin(RecRaster *r, int inc, int num, int den) {
	int ang[256];
	if (!leo_stick_make_tab(inc, r->lnPixHeight, ang))
		return (r->lnPixWidth * den < r->lnPixHeight * num);
	return leo_stick_thin_prop(r, ang, num, den);
}

static void leo_save_rl(uint16_t *lpool) {
	uint16_t *lp, *lop;
	int16_t len, hei, row, flg, il_ie;

	lop = save_pool;
	while (1) {
		lp = lpool;
		len = *lpool++;
		*lop++ = len;
		if (!len)
			break;
		hei = *lpool++;
		*lop++ = hei;
		row = *lpool++;
		*lop++ = row;
		flg = *lpool++; /* ALIGN TO 2 Bytes */
		*lop++ = flg;
		for (; hei; hei--) {
			il_ie = *lpool++; // both elem from the structure interval
			*lop++ = il_ie;
		}
		flg = *lpool++;
		*lop++ = flg;
		lpool = lp + len / 2;
	}
	return;
}

// !!!!!!!!!!!!!!!!! USING FIXED STRUCT OF LNHEAD !!!!!!!!!
// return : 0 - non stick
//          1 - similar to 1 or dark handprinted stick
//          2 - fine handprinted stick
//          3 - handprinted 1
//          4 - fine printed stick
//          5 - dark printed stick
int32_t leo_recog_stick(uint16_t *lpool, int w, int h) {
	int16_t len, hei, row, flg;
	uchar il, ie, *loc;
	uint16_t *lp;
	int i, n, ir, n_2, n_0, wa, wm, rm, wmm, rmm;
	int jmp, or_, ol, lim;

	memset(hist_wid, 0, sizeof(hist_wid[0]) * w);
	memset(hist_le, -1, sizeof(hist_le[0]) * h);
	memset(hist_ri, -1, sizeof(hist_ri[0]) * h);
	memset(hist_num, 0, sizeof(hist_num[0]) * h);

	h--;
	n_2 = 0;
	while (1) {
		lp = lpool;
		len = *lpool++;
		if (!len)
			break;
		hei = *lpool++;
		row = *lpool++;
		flg = *lpool++; /* ALIGN TO 2 Bytes */
		n = hei;
		for (i = 0; i < n; i++) {
			loc = (uchar *) lpool;
			il = *loc++;
			ie = *loc++;
			lpool++;
			ir = row + i;
			if (ir > 0 && ir < h) {
				hist_wid[il]++;
				hist_wi[ir] = il;
				if (hist_le[ir] > ie - il || hist_le[ir] == -1)
					hist_le[ir] = ie - il;
				if (hist_ri[ir] < ie || hist_ri[ir] == -1)
					hist_ri[ir] = ie;
				if (il > 0) {
					hist_num[ir]++;
					if (hist_num[ir] > 1)
						n_2++;
					if (!nIsPrint && leo_alpha_type != ALPH_ROM && n_2 > 2)
						return 0; // holes on image
				}
			}
		}
		lpool++;
		lpool = lp + len / 2;
	}

	if ((!nIsPrint && n_2 > 2) || n_2 > 4)
		return 0; // holes on image

	for (n_0 = 0, i = 1; i < h; i++) {
		if (hist_num[i] == 0)
			n_0++;
	}
	if (n_0 * 4 >= h)
		return 0; // big holes
	if (nIsPrint) {
		return (n_2 < 5 && n_0 < 1) ? 4 : 5; // print 1-line comp
	}
	// study jumps on contures
	lim = std::min(3, std::max((w + 7) / 8, 1));
	for (jmp = 0, ol = hist_le[1], or_ = hist_ri[1], i = 2; i <= h; i++) {
		if (hist_le[i] > ol + 1)
			jmp++;
		if (hist_ri[i] > or_ + 1)
			jmp++;
		if (!nIsPrint && hist_le[i] > ol + lim)
			return 3; // similar 1
		if (!nIsPrint && hist_ri[i] > or_ + lim)
			return 1;
		ol = hist_le[i];
		or_ = hist_ri[i];
	}
	if (jmp > 3)
		return 1;

	// study holes & calc widths
	for (wm = rm = -1, wmm = rmm = -1, wa = n_2 = i = 0; i <= h; i++) {
		if (rm < hist_wid[i])
			rm = hist_wid[(wm = i)];
		if (wmm < hist_ri[i] - hist_le[i])
			wmm = hist_ri[(rmm = i)] - hist_le[i];
		wa += hist_wi[i];
	}

	wa /= h - 1;
	if (abs(wa - wm) < 5) {
		if (wmm * 3 > wm * 4 && rmm > h / 4 && rmm < h * 3 / 4)
			return 1;
		else
			return 2;
	}
	return 1;
}

void leo_set_sticks_group(RecVersions *v, int32_t ret,
		unsigned char alphabet[], int32_t simple_st, int32_t nose_1) {
	int inc;
	int level_1;

	v->lnAltCnt = 0;
	v->lnAltMax = REC_MAX_VERS;
	slash_level = leo_current_slash_level();
	level_1 = leo_current_1_level();
	inc = abs(leo_get_global_incline());
	if (ret > slash_level) {
		if (!nose_1) {
			if (alphabet['/']) {
				v->Alt[v->lnAltCnt].Code = '/';
				v->Alt[v->lnAltCnt].CodeExt = 0;
				v->Alt[v->lnAltCnt].Prob = 255;
				v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
				v->lnAltCnt++;
			}
		} else {
			if (!nIsPrint && alphabet['1']) {
				v->Alt[v->lnAltCnt].Code = '1';
				v->Alt[v->lnAltCnt].CodeExt = 0;
				v->Alt[v->lnAltCnt].Prob = 255;
				v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
				v->lnAltCnt++;
			}
			if (alphabet['/']) {
				v->Alt[v->lnAltCnt].Code = '/';
				v->Alt[v->lnAltCnt].CodeExt = 0;
				v->Alt[v->lnAltCnt].Prob = 255;
				v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
				v->lnAltCnt++;
			}
		}
		if (alphabet['I']) {
			v->Alt[v->lnAltCnt].Code = 'I';
			v->Alt[v->lnAltCnt].CodeExt = 0;
			v->Alt[v->lnAltCnt].Prob = 127;
			v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
			v->lnAltCnt++;
		}
		if (!nose_1) {
			if (!nIsPrint && alphabet['1']) {
				v->Alt[v->lnAltCnt].Code = '1';
				v->Alt[v->lnAltCnt].CodeExt = 0;
				v->Alt[v->lnAltCnt].Prob = 127;
				v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
				v->lnAltCnt++;
			}
		}
	}

	else if (ret < slash_level / 2 || (inc && abs(ret - inc) < 256)) {
		if (alphabet['I']) {
			v->Alt[v->lnAltCnt].Code = 'I';
			v->Alt[v->lnAltCnt].CodeExt = 0;
			v->Alt[v->lnAltCnt].Prob = 255;
			v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
			v->lnAltCnt++;
		}
		if (alphabet['1']) {
			v->Alt[v->lnAltCnt].Code = '1';
			v->Alt[v->lnAltCnt].CodeExt = 0;
			v->Alt[v->lnAltCnt].Prob = nIsPrint ? 127 : 255;
			v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
			v->lnAltCnt++;
		}
		if (alphabet['/']) {
			v->Alt[v->lnAltCnt].Code = '/';
			v->Alt[v->lnAltCnt].CodeExt = 0;
			v->Alt[v->lnAltCnt].Prob = 127;
			v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
			v->lnAltCnt++;
		}
		if (simple_st || !v->lnAltCnt) {
			v->Alt[v->lnAltCnt].Code = '|';
			v->Alt[v->lnAltCnt].CodeExt = 0;
			v->Alt[v->lnAltCnt].Prob = 127;
			v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
			v->lnAltCnt++;
		}
	}

	else if (ret > slash_level / 2 && ret < slash_level) {
		if (alphabet['I']) {
			v->Alt[v->lnAltCnt].Code = 'I';
			v->Alt[v->lnAltCnt].CodeExt = 0;
			v->Alt[v->lnAltCnt].Prob = 255;
			v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
			v->lnAltCnt++;
		}
		if (nose_1) {
			if (!nIsPrint && alphabet['1']) {
				v->Alt[v->lnAltCnt].Code = '1';
				v->Alt[v->lnAltCnt].CodeExt = 0;
				v->Alt[v->lnAltCnt].Prob = 255;
				v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
				v->lnAltCnt++;
			}

			if (alphabet['/']) {
				v->Alt[v->lnAltCnt].Code = '/';
				v->Alt[v->lnAltCnt].CodeExt = 0;
				v->Alt[v->lnAltCnt].Prob = 255;
				v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
				v->lnAltCnt++;
			}
		} else {
			if ((level_1 && level_1 < slash_level / 2)
				|| (!level_1 && ret	> slash_level * 3 / 4)) {
				if (alphabet['/']) {
					v->Alt[v->lnAltCnt].Code = '/';
					v->Alt[v->lnAltCnt].CodeExt = 0;
					v->Alt[v->lnAltCnt].Prob = 255;
					v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
					v->lnAltCnt++;
				}

				if (!nIsPrint && alphabet['1']) {
					v->Alt[v->lnAltCnt].Code = '1';
					v->Alt[v->lnAltCnt].CodeExt = 0;
					v->Alt[v->lnAltCnt].Prob = 255;
					v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
					v->lnAltCnt++;
				}
			} else {
				if (alphabet['1']) {
					if (!nIsPrint && alphabet['1']) {
						v->Alt[v->lnAltCnt].Code = '1';
						v->Alt[v->lnAltCnt].CodeExt = 0;
						v->Alt[v->lnAltCnt].Prob = 255;
						v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
						v->lnAltCnt++;
					}

					if (alphabet['/']) {
						v->Alt[v->lnAltCnt].Code = '/';
						v->Alt[v->lnAltCnt].CodeExt = 0;
						v->Alt[v->lnAltCnt].Prob = 255;
						v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
						v->lnAltCnt++;
					}
				}

			}

		}
		if (simple_st) {
			v->Alt[v->lnAltCnt].Code = '|';
			v->Alt[v->lnAltCnt].CodeExt = 0;
			v->Alt[v->lnAltCnt].Prob = 127;
			v->Alt[v->lnAltCnt].Method = REC_METHOD_FINAL;
			v->lnAltCnt++;
		}

	}
	return;
}

void leo_set_simple_sticks_group(RecVersions *ver, unsigned char alphabet[],
		int32_t /*nose_1*/) {
	ver->lnAltCnt = 0;
	ver->lnAltMax = REC_MAX_VERS;
	if (alphabet['I']) {
		ver->Alt[ver->lnAltCnt].Code = 'I';
		ver->Alt[ver->lnAltCnt].CodeExt = 0;
		ver->Alt[ver->lnAltCnt].Prob = 255;
		ver->Alt[ver->lnAltCnt].Method = REC_METHOD_FINAL;
		ver->lnAltCnt++;
		if (alphabet['1']) {
			ver->Alt[ver->lnAltCnt].Code = '1';
			ver->Alt[ver->lnAltCnt].CodeExt = 0;
			ver->Alt[ver->lnAltCnt].Prob = 255;
			ver->Alt[ver->lnAltCnt].Method = REC_METHOD_FINAL;
			ver->lnAltCnt++;
		}

	} else {
		if (alphabet['1'] && !(leo_typ_of_font & LEO_FONT_MTR)) {
			ver->Alt[ver->lnAltCnt].Code = '1';
			ver->Alt[ver->lnAltCnt].CodeExt = 0;
			ver->Alt[ver->lnAltCnt].Prob = 255;
			ver->Alt[ver->lnAltCnt].Method = REC_METHOD_FINAL;
			ver->lnAltCnt++;
		}
	}

	if (alphabet['|'] || (nIsPrint && alphabet[stdAnsiToAscii((uchar) 'ы')])) {
		ver->Alt[ver->lnAltCnt].Code = '|';
		ver->Alt[ver->lnAltCnt].CodeExt = 0;
		ver->Alt[ver->lnAltCnt].Prob = (leo_typ_of_font & LEO_FONT_MTR) ? 255
				: 127;
		ver->Alt[ver->lnAltCnt].Method = REC_METHOD_FINAL;
		ver->lnAltCnt++;
	}

	return;
}

void leo_set_simple_sticks_print(RecVersions *ver,
		unsigned char /*alphabet*/[], uchar prob_stick) {
	ver->lnAltCnt = 0;
	ver->lnAltMax = REC_MAX_VERS;

	ver->Alt[ver->lnAltCnt].Code = '|';
	ver->Alt[ver->lnAltCnt].CodeExt = 0;
	ver->Alt[ver->lnAltCnt].Prob = prob_stick;
	ver->Alt[ver->lnAltCnt].Method = REC_METHOD_FINAL;
	ver->lnAltCnt++;

	ver->Alt[ver->lnAltCnt].Code = '1';
	ver->Alt[ver->lnAltCnt].CodeExt = 0;
	ver->Alt[ver->lnAltCnt].Prob = prob_stick / 2;
	ver->Alt[ver->lnAltCnt].Method = REC_METHOD_FINAL;
	ver->lnAltCnt++;

	return;
}

Bool32 leo_small_object(RecObject *object, int wlim, int hlim) {
	if (nIsPrint && object->recData.recRaster.lnPixWidth * 6
			> object->recData.recRaster.lnPixHeight * 1)
		return FALSE;
	if (object->recData.recRaster.lnPixWidth < wlim
			&& object->recData.recRaster.lnPixWidth/*<wlim*/* 5
					< object->recData.recRaster.lnPixHeight/*<hlim*/* 1)
		return FALSE;
	return (object->recData.recRaster.lnPixWidth < wlim
			|| object->recData.recRaster.lnPixHeight < hlim);
}

Bool32 leo_wide_object(RecObject *object) {
	int w = object->recData.recRaster.lnPixWidth, h =
			object->recData.recRaster.lnPixHeight;
	if (h > w)
		return 100;
	return (h * 100 / w);
}

static int32_t leo_num_of_long_sticks(RecVector *vSticks, int Cnt, int h) {
	int i, n;
	for (n = i = 0; i < Cnt; i++)
		if (vSticks[i].len * 3 > h * 2)
			n++;
	return n;
}

static Bool32 leo_wide_stick(RecVector *vSticks, int Cnt, int h, int w) {
	int i, n, s;
	for (n = i = 0; i < Cnt; i++)
		if (vSticks[i].len * 3 > h * 2) {
			s = vSticks[i].len * vSticks[i].My / sqrt(vSticks[i].Mx
					* vSticks[i].Mx + vSticks[i].My * vSticks[i].My);
			if (s > w / 2) // Oleg : 06-08-1998 : wide, but not small
				return TRUE;
		}
	return FALSE;
}

Bool32 leo_big_object(RecObject *object, int wlim, int hlim) {
	return (object->recData.recRaster.lnPixWidth > wlim
			|| object->recData.recRaster.lnPixHeight > hlim);
}

void leo_diskrim_stick(RecVersions *ver) {
	RecVersions loc;

	if (is_pool && ver->lnAltCnt) {
		loc = *ver;
		if (DIFStick_expert((int16_t) save_w, (int16_t) save_h,
				(uchar*) save_pool, &loc)) {
			*ver = loc;
			leo_sort_vers_prob(ver);
		}
	}
	return;
}

void leo_swap(RecAlt *Alt0, RecAlt *Alt2) {
	RecAlt Alt;
	Alt = *Alt2;
	*Alt2 = *Alt0;
	*Alt0 = Alt;
	return;
}

void Filtrate3Str(uint32_t *po, uint32_t *pc, int len32) {
	int j;
	uint32_t *pp = pc - len32, *pn = pc + len32;

	for (j = 0; j < len32; j++)
		po[j] = pc[j] | (pp[j] & pn[j]);
}

void FiltrateRaster(RecRaster *rin, RecRaster *rout) {
	int i, n, wb;
	uchar *po, *pc;

	memcpy(rout, rin, sizeof(RecRaster));
	wb = REC_GW_WORD8(rout->lnPixWidth);
	n = rout->lnPixHeight - 1;

	for (pc = &rin->Raster[wb], po = &rout->Raster[wb], i = 1; i < n; i++, po
			+= wb, pc += wb) {
		Filtrate3Str((uint32_t*) po, (uint32_t*) pc, wb / 4);
	}

	return;
}

Bool32 leo_one_line(uint16_t*lpool) {
	int16_t len, num;
	uint16_t*lp;

	if (!lpool)
		return FALSE;
	num = 0;
	while (1) {
		lp = lpool;
		len = *lpool;
		if (!len)
			break;
		num++;
		lpool = lp + len / 2;
	}
	return (num == 1);
}

Bool32 leo_is_stick(RecObject* object) {
	RecVersions ver;
	int32_t ret, rret, yx, w, h, n;
	uint16_t * lpool;
	Bool32 rstick;
	RecVersions r;
	RecRaster *rr, rthis;

	rstick = 0;

	rr = &object->recData.recRaster;
	if (alphabet[(uchar) '/'] && !nIsPrint && object->recData.lwCompCnt < 3) {
		rr = &rthis;
		FiltrateRaster(&object->recData.recRaster, rr);
	}
	w = rr->lnPixWidth;
	h = rr->lnPixHeight;
	n = object->recData.lwCompCnt;
	leo_stick_nose_1 = 0;
	lpool = (uint16_t*) EVNMakeLine(rr, object->recData.lwCompCnt > 1 ? 0 : 1);
	is_pool = 0;
	if (lpool) {
		leo_save_rl(lpool);
		is_pool = 1;
		save_w = w;
		save_h = h;
	}
	if (nIsPrint && w * 4 >= h * 3) {
		if (w <= h && alphabet[(uchar) '/'] && nIsPrint && leo_one_line(lpool)) {
			r.lnAltCnt = 0;
			r.lnAltMax = REC_MAX_VERS;

			r.Alt[r.lnAltCnt].Code = '/';
			r.Alt[r.lnAltCnt].Prob = 255;
			r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
			r.Alt[r.lnAltCnt].CodeExt = 0;
			r.lnAltCnt++;
			if (DIFStick_expert((int16_t) w, (int16_t) h, (uchar*) lpool, &r)) {
				if (r.Alt[0].Prob > 240) {
					memcpy(&object->recResults, &r, sizeof(RecVersions));
					return TRUE;
				}
			}
		} else
			return FALSE;
	}
	if (((n == 1 && (w * 4 <= h * 3 || !nIsPrint)) || (n == 2 && w * 2 <= h))
			&& lpool) {
		if (nIsPrint && w * 3 <= h) {
			r.lnAltCnt = 0;
			r.lnAltMax = REC_MAX_VERS;

			if (alphabet['i'] && n == 2 && similar_i(rr)) {
				r.Alt[r.lnAltCnt].Code = 'i';
				r.Alt[r.lnAltCnt].Prob = 255;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.lnAltCnt++;
				memcpy(&object->recResults, &r, sizeof(RecVersions));
				return TRUE;
			}

			if (leo_alpha_type != ALPH_DIG) {
				r.Alt[r.lnAltCnt].Code = '|';
				r.Alt[r.lnAltCnt].Prob = 0;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.lnAltCnt++;
			}

			if (alphabet['l']) {
				r.Alt[r.lnAltCnt].Code = 'l';
				r.Alt[r.lnAltCnt].Prob = 0;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.lnAltCnt++;
			}
			if (alphabet['1']) {
				r.Alt[r.lnAltCnt].Code = '1';
				r.Alt[r.lnAltCnt].Prob = 0;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.lnAltCnt++;
			}

			if (alphabet['t']) {
				r.Alt[r.lnAltCnt].Code = 't';
				r.Alt[r.lnAltCnt].Prob = 0;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.lnAltCnt++;
			}

			if (alphabet['(']) {
				r.Alt[r.lnAltCnt].Code = '(';
				r.Alt[r.lnAltCnt].Prob = 0;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.lnAltCnt++;
			}

			if (alphabet[')']) {
				r.Alt[r.lnAltCnt].Code = ')';
				r.Alt[r.lnAltCnt].Prob = 0;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.lnAltCnt++;
			}

			if (DIFStick_expert((int16_t) w, (int16_t) h, (uchar*) lpool, &r)) {

				leo_sort_vers_prob(&r);

				if (r.Alt[0].Prob > 240) {
					memcpy(&object->recResults, &r, sizeof(RecVersions));
					return TRUE;
				}
			} else
				return FALSE;
		}
		rstick = leo_recog_stick(lpool, w, h);
		if (rstick == 3 && w * 2 > h * 1)
			return FALSE;
		if (rstick == 5)
			return FALSE;
		if (rstick == 4) {
			r.lnAltCnt = 0;
			r.lnAltMax = REC_MAX_VERS;

			if (leo_alpha_type != ALPH_DIG) {
				r.Alt[r.lnAltCnt].Code = '|';
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.Alt[r.lnAltCnt].Prob = 0;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.lnAltCnt++;
			}
			if (alphabet[(uchar) '1']) {
				r.Alt[r.lnAltCnt].Code = '1';
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.Alt[r.lnAltCnt].Prob = 0;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.lnAltCnt++;
			}
			if (alphabet[(uchar) '/']) {
				r.Alt[r.lnAltCnt].Code = '/';
				r.Alt[r.lnAltCnt].CodeExt = 0;
				r.Alt[r.lnAltCnt].Prob = 0;
				r.Alt[r.lnAltCnt].Method = REC_METHOD_FINAL;
				r.lnAltCnt++;
			}

			if (DIFStick_expert((int16_t) w, (int16_t) h, (uchar*) lpool, &r)) {
				if (prn_roma_regim) {
					int c = leo_exist_code(&r, '1');
					if (c != -1)
						r.Alt[c].Code = 'I';
				}
				leo_sort_vers_prob(&r);

				if ((w * 2 <= h && r.Alt[0].Prob > 200)
						|| (w < h && r.Alt[0].Prob > 220)) {

					memcpy(&object->recResults, &r, sizeof(RecVersions));
					return TRUE;
				} else
					return FALSE;
			} else
				return FALSE;
		}
		if (rstick) {
			if (rstick == 3) {
				rstick = 1;
				leo_stick_nose_1 = 1;
			}
			if (!(object->recData.lwStatus & REC_STATUS_STIC)) {
				assert(try_call_vec);
			}
			ret = object->recData.vSticks[0].incline;
			yx = 0;
			if (object->recData.vSticks[0].My > object->recData.vSticks[0].Mx) {
				ret = -ret;
				yx = 1;
			}
			if (object->recData.lwSticksCnt > 1) {
				int num = leo_num_of_long_sticks(object->recData.vSticks,
						object->recData.lwSticksCnt, h);
				if (num > 1 || (num < 1 && object->recData.lwSticksCnt > 2))
					return FALSE;
				if (leo_wide_stick(object->recData.vSticks,
						object->recData.lwSticksCnt, h, w))
					return FALSE;
			}
			rret = (ret < 0) ? -ret : ret;
			if ((rstick == 2 && object->recData.lwSticksCnt == 1 && ret < 160)
					|| (rstick == 1
							&& object->recData.lwSticksCnt == 1
							&& leo_stick_nose_1 && ret < -256)
					) {
				if ((!yx && abs(rret - (w * 2048 / h)) < 256)
						|| (yx 	&& abs(rret - (h * 2048 / w)) < 256)) {
					leo_set_sticks_group(&ver, rret, alphabet, 1,
							leo_stick_nose_1);
					if (ver.lnAltCnt == 0)
						return FALSE;
					memcpy(&object->recResults, &ver, sizeof(RecVersions));
					return TRUE;
				}

			}
			if (rstick && ret < 350) {
				if (leo_stick_thin(rr, ret, 1, 4)) {
					leo_set_sticks_group(&ver, rret, alphabet, 0,
							leo_stick_nose_1);
					if (ver.lnAltCnt == 0)
						return FALSE;
					memcpy(&object->recResults, &ver, sizeof(RecVersions));
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

static Bool32 leo_test_inclinable(RecVersions *v) {
	uchar inc_let[] = "1ЙЦЕНГШЩЫВПРЧИТЬБЮ";
	if (v->lnAltCnt < 1)
		return FALSE;
	return (memchr(inc_let, v->Alt[0].Code, sizeof(inc_let)) != NULL);
}

void leo_setup_inc(RecObject* object) {
	if (!leo_test_inclinable(&object->recResults))
		return;

	return;
}

static int leo_get_first_pixel(uchar *r, int d) {
	int i;

	for (i = 0; i < d && (*r) == 0; i++, r++)
		;

	if (i == d)
		return (d << 3);

	return ((i << 3) + start_pos[*r]);
}

static int leo_get_last_pixel(uchar *r, int d) {
	int i;

	r += d - 1;

	for (i = 0; i < d && (*r) == 0; i++, r--)
		;

	if (i == d)
		return (d << 3);

	return ((i << 3) + last_pos[*r]);
}
/* --------------------------------------------------------*/
/* shift any row of raster[dy][(dx+7)>>3] to tab_angle[i]  */
/*      max_shift - max offset of string                   */
/* result : raster *res, return : new width of raster      */
static int leo_shift_raster(uchar *raster, int dx, int dy, int tab_angle[]) {
	int i = 0, ii = 0, j = 0, Dx = 0, d = 0, dd = 0, c = 0, db = 0;
	uchar *r, *rr, s1, s2, s3;
	int max_shift, left;
	int dir = tab_angle[0];
	uchar res[REC_MAX_RASTER_SIZE];

	d = REC_GW_WORD8(dx);
	db = (dx + 7) / 8;

	for (r = raster, max_shift = 32000, left = i = 0; i < dy; i++, r += d) {
		c = leo_get_first_pixel(r, d) + tab_angle[i];
		if (max_shift > c)
			max_shift = c;
	}

	for (r = raster, left = i = 0; i < dy; i++, r += d) {
		c = d * 8 - leo_get_last_pixel(r, d) - (max_shift - tab_angle[i]);
		if (left < c)
			left = c;
	}

	Dx = left;
	dd = REC_GW_WORD8(Dx);

	if (dir == 0) {
		memcpy(res, raster, dd * dy);
		return (dx);
	}
	memset(res, 0, dd * dy);
	for (r = raster, ii = i = 0; i < dy; i++, r += d, ii += dd) {
		c = max_shift - tab_angle[i];
		if (c > 0) {
			rr = res + ii;
			s1 = c & 7;
			s2 = 8 - s1;
			s3 = (c >> 3);

			for (j = 0; j < db - 1; j++)
				rr[j] = (r[s3 + j + 1] >> s2) | (r[s3 + j] << s1);
			rr[j] |= (r[s3 + j] << s1); // last
		} else if (c == 0) {
			memcpy(res + ii, r, db);
		} else if (c < 0) {
			c = -c;
			rr = res + ii;
			s1 = c & 7;
			s2 = 8 - s1;
			s3 = (c >> 3);
			rr[s3 + 0] = r[0] >> s1;
			for (j = 1; j < db; j++)
				rr[s3 + j] = (r[j - 1] << s2) | (r[j] >> s1);
			rr[s3 + j] |= (r[j - 1] << s2);
		}
	}
	memcpy(raster, res, dd * dy);
	return (Dx);
}

void leo_erect(RecObject* object, int inc) {
	int ang[256];
	if (inc > 0)
		return;
	if (!leo_stick_make_tab(inc, object->recData.recRaster.lnPixHeight, ang))
		return;
	object->recData.recRaster.lnPixWidth = leo_shift_raster(
			object->recData.recRaster.Raster,
			object->recData.recRaster.lnPixWidth,
			object->recData.recRaster.lnPixHeight, ang);
	object->recData.lwCompCnt = EVNGetNumComp(&object->recData.recRaster);
	return;
}

void leo_clear_stat(void) {
	leo_av_inc = leo_av_inc_n = 0;
	leo_av_inc1 = leo_av_inc_n1 = 0;
	return;
}

int leo_get_global_incline(void) {
	int inc;
	if (leo_av_inc_n < 5)
		return 0;
	inc = leo_av_inc / leo_av_inc_n;
	if (abs(inc) < 100)
		return 0;
	return inc;
}

static int leo_current_slash_level(void) {
	int inc;
	if (leo_av_inc_n < 5) {
		return leo_alpha_type == ALPH_DIG ? 1800 : 700;
	}
	inc = leo_av_inc / leo_av_inc_n;
	if (inc < -1000)
		return leo_alpha_type == ALPH_DIG ? 1800 : 1300;
	if (inc < -700)
		return leo_alpha_type == ALPH_DIG ? 1800 : 1100;
	if (inc < -500)
		return leo_alpha_type == ALPH_DIG ? 1800 : 900;
	if (inc < -400)
		return leo_alpha_type == ALPH_DIG ? 1800 : 800;
	if (inc > -10)
		return leo_alpha_type == ALPH_DIG ? 1800 : 500;
	return leo_alpha_type == ALPH_DIG ? 1800 : 700;
}

static int leo_current_1_level(void) {
	if (leo_av_inc_n1 < 2)
		return 0;
	return abs(leo_av_inc1 / leo_av_inc_n1);
}

static int32_t leo_get_incline(RecObject* object) {
	RecVector *v;
	int i, s, n, k, kz, h;

	if (!object->recData.lwSticksCnt || !leo_test_inclinable(
			&object->recResults))
		return 0;
	h = object->recData.recRaster.lnPixHeight;
	v = object->recData.vSticks;
	n = object->recData.lwSticksCnt;
	for (s = k = kz = i = 0; i < n; i++, v++) {
		if (v->Mx > v->My && v->incline < 0 && abs(v->incline) > 300) {
			s += v->incline;
			k++;
		}
		if (v->Mx > v->My && abs(v->incline) <= 300 && (v->len * 2) >= h * 1) {
			kz++;
		}
		if (v->Mx > v->My && abs(v->incline) <= 450 && (v->len * 3) >= h * 2) {
			kz++;
		}
	}
	if (k && !kz)
		s /= k;
	else
		s = 0;
	return s;
}

void leo_add_inc(RecObject* object) {
	uchar inc_let[] = "1ЙЦЕНГШЩЫВПРЧИТЬБЮ";
	RecVersions *v;
	int inc;

	v = &object->recResults;
	if (v->lnAltCnt < 1)
		return;
	if (memchr(inc_let, v->Alt[0].Code, sizeof(inc_let)) == NULL)
		return;
	if (object->recData.lwStatus & REC_STATUS_STIC) {
		inc = leo_get_incline(object);
		leo_av_inc += inc;
		leo_av_inc_n++;
		if (v->Alt[0].Code == '1') {
			leo_av_inc1 += inc;
			leo_av_inc_n1++;
		}
	}
	return;
}
