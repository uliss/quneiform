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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evn/evn.h"
#include "struct.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"
#include "alik_rec.h"
#include "status.h"
#include "rcutp/rcutp.h"
#include "linear.h"

#include "compat_defs.h"
#include "minmax.h"

//  snap globals
extern uchar db_status; // snap presence byte
extern uchar db_trace_flag; // 2 - more detailed estimate (ALT-F7)
extern uchar db_pass;

extern uchar *ForRaster1;

static void begin(int16_t, int16_t);
static void allcuts();
static void interlines();
static void minlincut(char);
static void fixcut(int16_t, int16_t, int16_t, int16_t);
static int16_t scamin(int16_t, int16_t, int16_t);
static void regmin(char);
static void make_limits();
static void findbotbnd();
static int16_t bot_serif(char);
static int16_t walltest(int16_t, int16_t, int16_t, int16_t, int16_t);
static int16_t middle_puso(char);
static int16_t lower_gap(char);
static void lower_pen(char);
static void findtopbnd();
static int16_t top_serif(char);
static int16_t upper_gap(char);
static void define_cat(struct extrem_elem *, char);
static void make_func();
static int16_t make_extrem(pchar, struct extrem_elem *);
static int16_t make_absextr(char, struct extrem_elem *);
static void make_info();
static char bodyes[128], cut_place[128];
static char foots[128], heads[128];
static struct extrem_elem exbody[MAX_HOR / 2];
static struct extrem_elem exfoot[MAX_HOR / 2];
static struct extrem_elem exhead[MAX_HOR / 2];
static int16_t totalh;
//Lepik searches objezd:
static int16_t SearchObjezd(int16_t x, puchar c, int16_t width, int16_t y,
		int16_t dh, int16_t r0, int16_t height, int16_t y_beg);

static void begin(int16_t width, int16_t height)
//
//	This procedure sets up all variables.
//
{

	s_width = width;
	s_width_b = (s_width + 7) / 8;
	s_height = height;
	if (s_height % 8 == 0)
		t_left_shift = 0;
	else
		t_left_shift = 8 - s_height % 8;
	t_width = s_height;
	t_width_b = (t_width + 7) / 8;
	t_height = s_width;
}

static void allcuts()
//
// This procedure builds graphics and finds all reasonable cutting points.
//
{
	int16_t cline;
	char svbest, svbestx;
	struct extrem_elem *Extrc;
	int16_t pnt;

	lastmax = 0;
	NBmax = 0;
	FlM = 0;
	Extrc = &exbody[1];
	for (Nmb = 1; Nmb < Exbn; Nmb++, Extrc++) {
		if (Extrc->type > 0) // mins only are of interest
		{
			if (((Extrc->val << 1) + Extrc->val) > ExbM) /* MAX */
			{
				NBmax++;
				lastmax = Nmb;
				if (!FlM)
					FlM = Nmb;
			}
			continue;
		}
		pnt = (Extrc->end + Extrc->beg) >> 1;
		cline = linumsx[pnt];
		if (cline == 0)
			continue;
		cline--;
		if ((cline == path[0]) || (cline == path[path_lth - 1]))
			goto try_;
		if (less_vertices[cline].n_connect == 1)
			continue;
		try_: svbestx = bestx = (char) pnt;
		svbest = best = Extrc->val;
		// TRY TO AVOID ALL semantic - just take minbody
		if (svbest < 3) {
			regmin(4);
			continue;
		}

		if ((foots[bestx] + heads[bestx]) < t_width) {
			findbotbnd();
			if (walltest(0, 6, 7, 1, 4))
				if (bot_serif(1))
					continue;
			best = svbest;
			bestx = svbestx;
			if (lower_gap(2))
				continue;
			best = svbest;
			bestx = svbestx;
		} else {
			findtopbnd();
			if (walltest(0, 5, 7, 1, 4) && (foots[bestx] * 3 > t_width))
				if (top_serif(4))
					continue;
		}
		best = svbest;
		bestx = svbestx;
		if (middle_puso(6))
			continue;
	}

	lower_pen(3);

	Eahn = (char) make_absextr(0, exhead);
	Eafn = (char) make_absextr(1, exfoot);

	extrc = &exhead[2];
	extrp = &exhead[1];
	extrn = &exhead[3];
	for (Nmb = 2; Nmb < Eahn; Nmb++, extrc++, extrp++, extrn++) {
		if (extrc->type >= 0)
			continue;
		bestx = (extrc->end + extrc->beg) >> 1;
		if (bestx < Startp)
			continue;
		if (bestx > Endp)
			break;
		cline = linumsx[pnt];
		if (cline == 0)
			continue;
		cline--;
		if ((cline == path[0]) || (cline == path[path_lth - 1]))
			goto try2;
		if (less_vertices[cline].n_connect == 1)
			continue;
		try2: if ((foots[bestx] + heads[bestx]) < t_width)
			continue; // lower case
		if (upper_gap(5))
			continue;
	}

	extrc = &exfoot[2];
	extrp = &exfoot[1];
	extrn = &exfoot[3];
	for (Nmb = 2; Nmb < Eafn; Nmb++, extrc++, extrp++, extrn++) {
		if (extrc->type <= 0)
			continue;
		bestx = (extrc->end + extrc->beg) >> 1;
		if (bestx < Startp)
			continue;
		if (bestx > Endp)
			break;
		cline = linumsx[pnt];
		if (cline == 0)
			continue;
		cline--;
		if ((heads[bestx] + foots[bestx]) > t_width)
			continue; /* lower case */
		if (bodyes[bestx - 1] < bodyes[bestx]) {
			bestx--;
			regmin(7);
			continue;
		}
		if (bodyes[bestx + 1] < bodyes[bestx]) {
			bestx++;
			regmin(7);
			continue;
		}
		regmin(7);
	}
}

static void interlines() { // NOT USED YET      analyze points at lines
}

static void minlincut(char arg)
//
//    This procedure builds graphics and finds all reasonable cutting points.
//
{
	int16_t nx, cline, min, max, svf, svn, svl, twh, tw2, tw3, is4, wdsfl,
			lastx, lastw, curw, nfix, flgr, fldef;
	struct info_elm *inf;

	//
	// 3 points are chosen:
	//   1. the thinnes place in a single pattern-connecting line
	//   2. same, but not necessarily separating
	//   3. the thinnest projection
	// such points are collected:
	//	 inside pattern ( 5 pixels far from each border )
	//	 OR between walls:
	//		not earlier than (projection >= 1/4 Cell-height) passes by
	//		not later than last (projection >= 1/2 cell-height) passes by

	wdsfl = svn = svl = 0;
	min = 127;
	max = 0;
	inf = &points[1];
	lastx = 0;
	lastw = 127;
	fldef = 1;
	nfix = svf = 0;
	flgr = 0;
	tw2 = t_width << 1;
	twh = t_width >> 2;
	tw3 = tw2 + t_width;

	for (nx = 1; nx < t_height; nx++, inf++) {
		curw = inf->sumb;
		is4 = curw << 2;
		if ((int16_t) min >= (int16_t) (inf->botf))
			min = inf->botf;
		if ((int16_t) max <= (int16_t) (inf->toph))
			max = inf->toph;
		if ((int16_t) is4 > (int16_t) t_width) {
			wdsfl++;
			nfix = 2;
		} // may look at thin line
		if (((int16_t) (max - min) > (int16_t) twh) && ((nx > (int16_t) 5)
				&& svn)) {
			fixcut(svf, svl, svn, arg);
			svn = 0;
			max = 0;
			min = 127;
		}
		if ((int16_t) lastw == (int16_t) curw)
			continue;
		if ((int16_t) lastw > (int16_t) curw) // became narrower
		{
			if (((int16_t) nx > (int16_t) (t_height - 4)) && ((int16_t) (nx
					- lastx) > (int16_t) 2))
			// close to pattern end
			{
				if ((lastw << 2) > tw3) // too thick
					return;
				fixcut(127, lastx, nx, arg);
				return;
			}
			flgr = 0;
			goto gr1;
		}
		// became wider
		if ((inf->topline == inf->botline) && fldef)
			if ((lastw < 5) && ((curw - lastw) > 1) && (nx > 5)) // end-of-defis
			{
				fldef = 0;
				bestx = nx - 1;
				regmin(8);
			}
		if (flgr)
			goto gr1; // still grows
		flgr = 1;
		nfix++;
		if ((nx < 5) && (!wdsfl)) // too early
			goto gr1;
		if ((lastw << 2) > tw3) // too thick
			goto gr1; // ????????????????????????????????????
		switch (arg) {
		case 0: // sum at single line
		case 1:
			break;
		case 2: // min at separ. line
			cline = linums[nx];
			if (cline == 0)
				break; // no ciutgraph's line here
			cline--;
			if ((inf->topline != inf->botline)
					|| (less_vertices[cline].n_connect < 2)) // single line connecting the pattern
				goto gr1;
			// no need to fix
			break;
		case 3: // sum at many lines
			if (inf->topline == inf->botline) // single line connects the pattern
				goto gr1;
			break;
		}
		// ready to register the point
		if (wdsfl) // wall passed
		{
			fixcut(nfix, lastx, nx, arg);
			wdsfl = 0;
			svn = 0;
			max = 0;
			min = 127;
		} else {
			svn = nx;
			svf = nfix;
			svl = lastx;
		}
		gr1: lastx = nx;
		lastw = curw;
	}
	if (flgr) // still grows
		return;
	if (nx - lastx < 4) // too close to right end
		return;
	fixcut(127, lastx, nx, arg);
}

static void fixcut(int16_t fl, int16_t x1, int16_t x2, int16_t arg) {
	int16_t ctf;

	switch (arg) {
	case 0:
	case 1:
	case 3:
		ctf = 8 + 32; /* single cut if disconnects */
		break;
	case 2:
		ctf = 8;
		break;
	}
	if (fl > 1)
		if (!scamin(x1, -1, ctf)) {
			bestx = (char) x1;
			regmin((char) ctf);
		}
	if (fl < 127)
		if (!scamin(x2, 1, ctf)) {
			bestx = (char) x1;
			regmin((char) ctf);
		}
	if ((fl > 1) && (fl < 127)) {
		bestx = (x1 + x2) >> 1;
		regmin((char) ctf);
	}
}

static int16_t scamin(int16_t a, int16_t d, int16_t r) {
	int16_t nx, y, m;

	nx = a;
	y = points[nx].sumb;
	do {
		m = nx;
		nx += d;
	} while ((nx > 4) && (nx < (t_height - 4)) && ((points[nx].sumb - y) <= 1));
	if (abs(m - a) > 2) {
		bestx = (char) m;
		regmin((char) r);
		return 1;
	}
	return 0;
}

static void regmin(char c)
// cut in x=bestx, method=c;
{
	int16_t v, line_num;
	char rept, full, w, wx, ww, dh, h, vr;
	struct info_elm *inf;
	struct own_cut *ad;

	wx = bestx;
	vr = bestc = c;
	full = c & 32;
	rept = c & 64;
	inf = &points[wx];
	best = bodyes[wx];

	if (full) {
		h = dh = (char) t_width;
		if (inf->lnum == 1) {
			if ((3* inf ->sumb) > (t_width << 1))
				return; // too thick
			dh = inf->sumb;
			h = inf->toph;
			full = 0;
			vr &= ~32;
		}
		goto mark;
	}

	if (rept) {
		h = bestxh;
		dh = bestxr;
		goto mark;
	}

	line_num = linumsx[wx];
	if (line_num == 0) {
		h = dh = (char) t_width;
	} else {
		struct short_line_header *l_ptr;
		struct segment *s_ptr;
		line_num--;
		l_ptr = &Ed_lines[line_num];
		s_ptr = (struct segment *) ((uchar *) t_line_ptr + l_ptr->segm_addr);
		s_ptr += wx - l_ptr->top;
		dh = s_ptr->lth;
		h = s_ptr->end;
	}
	/****************************************************************
	 // try to correct "best" bodycut using "real" bodyes
	 if (cut_body[wx] < dh)
	 ********************************************/
	{
		dh = bodyes[wx]; /*** cut_body[wx]; ***/
		h = cut_place[wx];
	}
	/*****************************************************************/
	if ((3* dh ) > (t_width << 1))
		return; //  too thick
	mark: ;
	for (v = 1, ad = odrans + 1; v < STK_H - 1; ad++, v++) {
		if (wx == 127)
			break;

		if (wx < (ad->x))
			goto ins;
		if (wx > (ad->x))
			continue;
		// same
		if ((vr & ad->var) & 32)
			break; // both full
		if ((vr & ad->var) & 64)
			break; // both diag
		if (!((vr | ad->var) & (32 + 64))) // both usual
		{
			if (ad->dh < dh)
				break; // old thinner
			if (ad->dh > dh)
				goto repl; // new thinner
			if (ad->h == h) // exactly same cut point
				break; // new full or not - ignore it
			goto ins;
			// same size, another cut
		}
		if (!(vr & 32))
			continue; // usual - after full
		goto ins; // usual and unusual
		repl: ww = 1; // replace flag
		goto proc;
		ins: ww = 0;
		proc: w = ad->x;
		ad->x = wx;
		wx = w;
		w = ad->dh;
		ad->dh = dh;
		dh = w;
		w = ad->h;
		ad->h = h;
		h = w;
		w = ad->var;
		ad->var = vr;
		vr = w;
		if (ww)
			break; // replace, not insert
	}
}

static void make_limits() {
	char nx, min, max, w;

	Exbn = (char) make_extrem(bodyes, exbody);
	ExbM = ExtreM;
	Exbm = Extrem;
	Exbx = Extrex;
	Exfn = (char) make_extrem(foots, exfoot);
	ExfM = ExtreM;
	Exfm = Extrem;
	Exhn = (char) make_extrem(heads, exhead);
	ExhM = ExtreM;
	Exhm = Extrem;
	define_cat(exbody, Exbn);
	define_cat(exfoot, Exfn);
	define_cat(exhead, Exhn);

	Startp = -1;
	Endp = -1;
	min = 127;
	max = 0;
	for (nx = 0; nx < totalh; nx++) {
		if (min > points[nx].botf)
			min = points[nx].botf;
		if (max < points[nx].toph)
			max = points[nx].toph;
		w = max - min;
		if (((w << 1) + w) > t_width) {
			Startp = nx;
			break;
		}
	}

	min = 127;
	max = 0;
	for (nx = totalh - 1; nx >= 0; nx--) {
		if (min > points[nx].botf)
			min = points[nx].botf;
		if (max < points[nx].toph)
			max = points[nx].toph;
		w = max - min;
		if (((w << 1) + w) > t_width) {
			Endp = nx;
			break;
		}
	}
}

static void findbotbnd() {
	char nm, nx, v1, lr;

	// find two BIG Maxima; point-candidate between them
	extrc = &exhead[0];
	nm = 0;
	nmax1 = nmax2 = -1;
	for (; nm < Exhn; nm++, extrc++) {
		if ((extrc->beg > bestx) && (nmax1 == -1))
			return;
		if (extrc->type <= 0)
			continue;
		if ((extrc->val << 1) < ExhM)
			continue;
		if ((nmax1 == -1) || (extrc->end < bestx)) {
			lbnd = extrc->beg;
			lwpnt = extrc->end;
			nmax1 = nm;
			continue;
		}
		nmax2 = nm;
		rbnd = extrc->end + 1;
		rwpnt = extrc->beg;
		goto lrdo;
	}

	return;

	lrdo: ;
	// lbnd - 1-st BIG MAX to the left of the point
	//   move it to left untill body (the Max) is thick enough
	v1 = bodyes[lbnd];
	while (lbnd >= 0) {
		if ((bodyes[lbnd] << 2) < 3* v1 )
			break; // body > 3/4 etalon
		lbnd--;
	}
	lbnd++;
	if (lbnd < 3)
		lbnd = 0;

	v1 = bodyes[lwpnt];
	lr = MAX(0, lwpnt - 4);
	while (lwpnt >= lr) {
		if ((bodyes[lwpnt] << 2) < 3* v1 )
			break; // body > 3/4 etalon
		lwpnt--;
	}

	// rbnd - the same to the right of the pnt
	//    move it right untill thick enough

	v1 = bodyes[rbnd];
	while ((uint16_t) rbnd < totalh) {
		if ((bodyes[rbnd] << 2) < 3* v1 )
			break;
		rbnd++;
	}
	if (rbnd > (totalh - 3))
		rbnd = (char) totalh;

	v1 = bodyes[rwpnt];
	lr = MIN(rwpnt + 4, totalh);
	while (rwpnt <= lr) {
		if ((bodyes[rwpnt] << 2) < 3* v1 )
			break;
		rwpnt++;
	}

	// define minima area as "about the same value of body"
	// ???????? TOO BRIGHT BOUNDS
	nx = bestx + 1;
	nx = exbody[Nmb].end;
	while (abs(bodyes[nx] - best) < 2)
		nx++;
	Rbnd = nx;
	nx = bestx - 1;
	nx = exbody[Nmb].beg;
	while (abs(bodyes[nx] - best) < 2)
		nx--;
	Lbnd = nx + 1;
	return;
}

static int16_t bot_serif(char ret) {
	int16_t nx, x, x1, x2 /*, ty, cy*/;
	char *cf, maxf, minf;
	char *ch, maxh, minh;

	// are there two maxima around ?
	if ((nmax1 | nmax2) & 0x80)
		return 0;
	// both have to be "walls", not "bounds"
	// both bounds - arise ?
	if ((bodyes[Lbnd - 1] < bodyes[Lbnd]) || (bodyes[Rbnd - 1] > bodyes[Rbnd]))
		return 0;
	x1 = exbody[Nmb].beg;
	x2 = exbody[Nmb].end;
	// Thin place (1 or 2 body, like  *******
	/**********************************
	 x=-1;
	 for (nx=x1; nx<=x2; nx++)
	 {
	 ty=heads[nx];
	 if ((foots[nx-1]+1) >= ty) { x=nx-1; cy=ty; }
	 if ((foots[nx+1]+1) >= ty) { x=nx; cy=ty; }
	 }
	 if (x!=-1) { bestx=x; bestxh=cy; bestxr=2; regmin(ret+64); return 1;}
	 ***************************************/// NO HORIZ CUTs
	x = (Rbnd + Lbnd) >> 1;

	// assure almost plain body
	if (abs(foots[Rbnd - 1] - foots[Lbnd]) > 2)
		return 0;
	if (abs(heads[Rbnd - 1] - heads[Lbnd]) > 2)
		return 0;
	if (abs(foots[Rbnd - 1] - foots[x]) > 1)
		return 0;
	if (abs(heads[Rbnd - 1] - heads[x]) > 1)
		return 0;

	// serif-to-vertical-wall
	if (((foots[Rbnd - 1] - foots[Rbnd]) > 3) || ((foots[Lbnd]
			- foots[Lbnd - 1]) > 3)) {
		bestx = (char) x;
		regmin(ret);
		return 1;
	}

	// serif-to-vertical-wall
	if (((heads[Rbnd] - heads[Rbnd - 1]) > 3) && ((foots[Rbnd - 1]
			- foots[Rbnd]) < 2)) {
		bestx = (char) x;
		regmin(ret);
		return 1;
	}

	// test significant Max between
	if ((nmax2 - nmax1) > 2)
		return 0;

	// verify the foot to be plain enough
	cf = &foots[Lbnd];
	ch = &heads[Lbnd];
	maxh = 0;
	minh = 127;
	maxf = 0;
	minf = 127;
	for (nx = Lbnd; nx < Rbnd; cf++, ch++, nx++) {
		if (maxf < *cf)
			maxf = *cf;
		if (minf > *cf)
			minf = *cf;
		if (maxh < *ch)
			maxh = *ch;
		if (minh > *ch)
			minh = *ch;
	}

	if (((foots[Rbnd] - foots[bestx]) > 1)
			&& ((heads[Rbnd] - heads[bestx]) > 1)) {
		bsnx = bestx;
	}

	if (maxf - minf > 2)
		return 0;
	if (maxh - minh > 2)
		return 0;

	// all tests succeeded
	bestx = (Lbnd + Rbnd - 1) >> 1;
	best = bodyes[bestx];
	regmin(ret);
	return 1;
}

static int16_t walltest(int16_t flg, int16_t dn1, int16_t dn2, int16_t k1,
		int16_t k2) {
	int16_t nx, w, wt, n1, n2, maxh, minf;

	maxh = 0;
	minf = 256;
	wt = t_width * k1;
	n1 = MAX(0, bestx - dn1);
	n2 = MIN(bestx + dn2, t_height); // totalh ?
	for (nx = n1; nx < n2; nx++) {
		if (flg) // don't cut puso beside tall wall
		{
			w = (heads[nx] - foots[nx]) * k2;
			if (w > wt) // h-f > 2/3 width
				return 1;
		}
		if (heads[nx] > maxh)
			maxh = heads[nx];
		if (foots[nx] < minf)
			minf = foots[nx];
	}
	if (flg) // walls beside point of interest ?
		return 0;
	w = (maxh - minf) * k2;
	if (w > wt)
		return 1;
	return 0;
}

static int16_t middle_puso(char ret) {
	int16_t nm;

	if (heads[bestx] + foots[bestx] < t_width) {
		extrc = &exfoot[1];
		extrp = &exfoot[0];
		extrn = &exfoot[2];
		for (nm = 2; nm < Exfn; nm++, extrc++, extrp++, extrn++) {
			if (extrc->type <= 0)
				continue;
			if (((extrc->beg - 1) > bestx) || ((extrc->end + 1) < bestx))
				continue;
			if ((extrc->end - extrc->beg) > 2)
				continue;
			if (((extrc->val - extrp->val) >= 2) && ((extrc->val - extrn->val)
					>= 2)) {
				regmin(ret);
				return 1;
			}
		}
		return 0;
	} else {
		extrc = &exhead[1];
		extrp = &exhead[0];
		extrn = &exhead[2];
		for (nm = 2; nm < Exhn; nm++, extrc++, extrp++, extrn++) {
			if (extrc->type >= 0)
				continue;
			if (((extrc->beg - 1) > bestx) || ((extrc->end + 1) < bestx))
				continue;
			if ((extrc->end - extrc->beg) > 2)
				continue;
			if (((extrp->val - extrc->val) >= 2) && ((extrn->val - extrc->val)
					>= 2)) {
				regmin(ret);
				return 1;
			}
		}
		return 0;
	}
}

static int16_t lower_gap(char ret) {
	char nm, x1, x2;
	int16_t line_num;

	extrp = &exfoot[0];
	extrc = &exfoot[1];
	extrn = &exfoot[2];
	for (nm = 1; nm < Exfn; nm++, extrc++, extrp++, extrn++) {
		if (extrc->type <= 0)
			continue;
		x2 = extrc->end;
		if (x2 < bestx)
			continue;
		if (x2 > Endp)
			return 0;
		if ((nm < 2) || ((nm + 2) > Exfn))
			continue;
		x1 = extrc->beg;
		if ((foots[x1] + heads[x1]) > t_width)
			return 0; // upper case
		line_num = linums[x1];
		if (line_num == 0)
			return 0;
		line_num--;
		if (points[x1].botline != line_num)
			return 0; // not lowest line
		if ((extrc->val - extrp->val) < 2)
			return 0;
		if ((extrc->val - extrn->val) < 2)
			return 0;
		// not a deep gap
		if ((x2 - x1) > 2)
			return 0;
		// too wide gap
		if ((extrn->beg - x2) < 2) // right edge krutoe
		{
			if (bodyes[x2] << 1 > ExbM)
				return 0;
			if (bodyes[x2 + 1] << 1 > ExbM)
				return 0;
			if (bodyes[x2 + 1] << 1 > ExbM)
				return 0;
		}
		bestx = x1;
		while (1) {
			if (bodyes[bestx] == bodyes[bestx + 1])
				bestx++;
			else
				break;
		}
		regmin(ret);
		return 1;
	}
	return 0;
}

static void lower_pen(char ret) {
	char dx1, dx2, dy1, dy2, a1, a2, v;

	extrc = &exhead[2];
	extrp = &exhead[1];
	extrn = &exhead[3];
	for (Nmb = 2; Nmb < Exhn - 2; Nmb++, extrc++, extrp++, extrn++) {
		if (extrc->type <= 0)
			continue; // min
		bestx = (extrc->beg + extrc->end) >> 1;
		if ((foots[bestx] + heads[bestx]) > t_width)
			continue; // lower case
		if ((heads[bestx] << 1) > t_width)
			continue;
		// MAX
		if (bestx < Startp)
			continue;
		if (bestx > Endp)
			break;
		v = linumsx[bestx];
		if (v == 0)
			continue;
		v--;
		if (less_vertices[v].n_connect == 1)
			continue;
		dx1 = extrc->beg - extrp->end;
		dx2 = extrn->beg - extrc->end;
		dy1 = heads[extrc->beg] - heads[extrp->beg];
		dy2 = heads[extrc->beg] - heads[extrn->beg];
		if (abs(dy1 - dy2) > 2)
			continue;
		if ((dx1 + dx1 + dx1) < dy1)
			a1 = 1;
		else
			a1 = 0;
		if ((dx2 + dx2 + dx2) < dy2)
			a2 = 1;
		else
			a2 = 0;
		if (a1 && a2)
			bestx = extrc->end;
		if (a1 && (!a2))
			bestx = extrc->beg;
		if ((!a1) && a2)
			bestx = extrc->end;
		if ((!a1) && (!a2)) // bestx in the middle
			if ((extrc->end - extrc->beg) < 3) {
				if (dy2 * dx1 < dy1 * dx2)
					bestx = extrc->beg;
				else
					bestx = extrc->end;
			}
		regmin(ret);
	}
}

static void findtopbnd() {
	char nm, nx, v1;

	// find two BIG Maxima; point-candidate between them
	extrc = &exfoot[0];
	nm = 0;
	nmax1 = nmax2 = -1;
	for (; nm < Exfn; nm++, extrc++) {
		if ((extrc->beg > bestx) && (nmax1 == -1))
			return;
		if (extrc->type <= 0)
			continue;
		if (((t_width - extrc->val) << 1) < (t_width - ExhM))
			continue;
		if (nmax1 == -1) {
			lbnd = extrc->beg;
			nmax1 = nm;
			continue;
		}
		if (extrc->end < bestx) {
			nmax1 = nm;
			lbnd = extrc->beg;
			continue;
		}
		nmax2 = nm;
		rbnd = extrc->end + 1;
		goto lrdo;
	}

	return;

	lrdo: ;
	// lbnd - 1-st BIG MAX to the left of the point
	//   move it to left untill body (the Max) is thick enough
	v1 = bodyes[lbnd];
	while (lbnd >= 0) {
		if ((bodyes[lbnd] << 2) < 3* v1 )
			break; // body > 3/4 etalon
		lbnd--;
	}

	lbnd++;
	if (lbnd < 3)
		lbnd = 0;

	// rbnd - the same to the right of the pnt
	//   move it right untill thick enough
	v1 = bodyes[rbnd];
	while ((uint16_t) rbnd < totalh) {
		if ((bodyes[rbnd] << 2) < 3* v1 )
			break;
		rbnd++;
	}
	if (rbnd > (totalh - 3))
		rbnd = (char) totalh;

	// define minima area as "about the same value of body"
	nx = bestx + 1;
	while (abs(bodyes[nx] - best) < 2)
		nx++;
	Rbnd = nx;
	nx = bestx - 1;
	while (abs(bodyes[nx] - best) < 2)
		nx--;
	Lbnd = nx + 1;
	return;
}

static int16_t top_serif(char ret) {
	int16_t nx;
	int16_t x, x1, x2, v1, v2, v3, v4;
	char cmin, cmax;

	// serif has not to be too thick
	if (best > 4)
		return 0; // ?????????? Etalon=4 IS RUBBISH !!!!!!
	// has to have Max right
	switch (Exbn - Nmb) {
	case 1:
		return 0;
	case 2:
		return 0;
	case 3:
		if ((exbody[Exbn - 2].val << 2) < ExbM)
			return 0;
	}
	// establish bounds for "body_almost_const_width" condition
	Lbnd = Rbnd = cmin = cmax = bestx;
	v1 = bodyes[bestx];
	for (nx = bestx; nx < totalh; nx++) {
		if ((bodyes[nx] - v1) > 1)
			break;
		Rbnd = (char) nx;
	}

	for (nx = bestx; nx >= 0; nx--) {
		v4 = bodyes[nx] - v1;
		if (!v4) {
			Lbnd = (char) nx;
			continue;
		}
		if (v4 < 0) {
			v1 = v4;
			continue;
		}
		if (v4 < 2) {
			Lbnd = (char) nx;
			continue;
		}
		break;
	}
	// not beside MAxbody
	// unless seems to be "dash_to_serif" type
	if (lastmax && ((Nmb - lastmax) < 3)) {
		if ((exbody[lastmax].val << 1) < ExbM)
			goto allow; // after small max
		x = exbody[lastmax].end;
		x2 = exbody[Nmb].end;
		x1 = exbody[Nmb].beg;
		if (NBmax > 1) // not "r_to_anything
		{
			bestx = (x1 + x2) >> 1;
			goto rt;
		}

		// allow beside HIGH wall
		if ((heads[x] - heads[x1]) > 3)
			goto allow;
		// inhibit "diamond" cut in the area
		if ((x + 4) > x2)
			return 0;
	}

	allow: v1 = bodyes[bestx];
	for (nx = bestx; nx < totalh; nx++) {
		if (((v2 = bodyes[nx]) - v1) > 1)
			break;
		cmax = (char) nx;
		if (v2 == 0)
			rbnd = (char) nx;
	}
	for (nx = bestx; nx >= 0; nx--) {
		if (((v2 = bodyes[nx]) - v1) > 1)
			break;
		cmin = (char) nx;
		if (v2 == 0)
			lbnd = (char) nx;
	}

	// 1. isn't this a circle
	// go right to find local max of head
	v1 = v2 = v3 = heads[bestx];
	for (nx = bestx + 1; nx < totalh; nx++) {
		if (nx > (Rbnd + 1))
			goto tryback;
		v2 = heads[nx];
		if (v2 < v1)
			break; // till end of rising bridge
		if ((v2 - v3) > 3)
			break; // 'r' and other upper bridges not
		//    to rise too much
		cmax = (char) nx;
		v1 = v2;

	}
	tryback: ;
	v1 = v2 = v3;
	for (nx = bestx; nx >= 0; nx--) {
		if (nx < Lbnd)
			goto ok;
		v2 = heads[nx];
		if (v2 < v3)
			break;
		if (cmin > nx)
			cmin = (char) nx;

	}
	ok: rt: v2 = bestx;
	regmin(ret);

	if ((cmax != v2) && (cmax < (t_width - 10))) {
		bestx = cmax;
		regmin(ret);
	}
	return 1;
}

static int16_t upper_gap(char ret) {
	char nx, y;
	int16_t line_num;

	// not beside MAxbody
	if (!FlM)
		return 0;
	if (Nmb < 3)
		return 0;
	for (nx = extrp->beg; nx < extrc->end; nx++) {
		y = bodyes[nx];
		if (((y << 1) + y) > ExbM)
			return 0;
	}
	if (extrp->beg <= exbody[FlM].end)
		return 0;
	if (extrc->end > Endp)
		return 0;
	line_num = linums[extrc->beg];
	if (line_num == 0)
		return 0;
	line_num--;
	if (points[extrc->beg].topline != line_num)
		return 0;
	if ((extrp->val - extrc->val) < 2)
		return 0;
	if ((extrn->val - extrc->val) < 2)
		return 0;
	if ((extrc->end - extrc->beg) > 4)
		return 0;
	if (((extrc->beg - extrp->end) < 3) && ((extrp->val - extrc->val) > 4))
		return 0;
	bestx = (extrc->beg + extrc->end) >> 1;
	regmin(ret);
	return 1;
}

static void define_cat(struct extrem_elem *fun, char N) {
	int16_t nm;

	extrp = &fun[0];
	extrc = &fun[1];
	extrn = &fun[2];
	for (nm = 1; nm < N; nm++, extrn++) {
		if ((abs(extrc->val - extrp->val) < 2) && (abs(extrc->val - extrn->val)
				< 2))
			extrc->cat = 0;
		else
			extrc->cat = 1;
		extrp = extrc;
		extrc = extrn;
	}
}

static void make_func()
//
// This procedure calculates all three functions:
//        body, upper board, lower board
//
{
	struct short_line_header *l_ptr;
	struct segment *s_ptr;
	int16_t n, x, y, z, bd, bn, cps, cpn, u, p;
	char *h1, *h2, *h3, *f1, *f2, *f3, *cb, *cp;

	totalh = 0;
	for (n = 0; n < t_height; n++) {
		heads[n] = (char) t_width;
		bodyes[n] = (char) t_width;
		foots[n] = 0;
		linums[n] = 0;
	}
	for (n = 0; n < path_lth; n++) {
		p = path[n];
		l_ptr = &Ed_lines[p];
		s_ptr = (struct segment *) ((uchar *) t_line_ptr + l_ptr->segm_addr);
		for (x = l_ptr->top; x < l_ptr->bot; x++, s_ptr++, totalh++) {
			heads[totalh] = s_ptr->end;
			bodyes[totalh] = s_ptr->lth;
			foots[totalh] = s_ptr->end - s_ptr->lth;
			linums[totalh] = p + 1;
			linumsx[x] = p + 1;
		}
	}
	for (n = 0, h1 = &heads[0], h2 = &heads[1], h3 = &heads[-1], f1 = &foots[0], f2
			= &foots[1], f3 = &foots[-1], cb = &bodyes[0], cp = &cut_place[0]; n
			< totalh; n++, h1++, h2++, h3++, f1++, f2++, f3++, cb++, cp++) {
		if (n == (totalh - 1)) {
			bd = 127;
			goto back;
		}
		if ((u = *h2) > (x = *h1)) {
			cps = x;
			y = *f2;
			if (y == x) {
				bd = 1;
				goto back;
			}
			z = *f1;
			if (y > z) {
				bd = x - y + 1;
				goto back;
			}
			// else (y <= z)
			bd = x - z;
			goto back;
		}

		if (u == x) // *h2 == *h1
		{
			cps = x;
			y = *f2;
			z = *f1;
			if (y > z) {
				bd = x - y + 1;
				goto back;
			}
			bd = x - z;
			goto back;
		}
		// else (*h2 < * h1)
		cps = u + 1;
		y = *f2;
		z = *f1;
		if (u == z) {
			bd = 1;
			goto back;
		}
		if (y > z) {
			bd = x - y + 1;
			goto back;
		}
		bd = x - z;
		goto back;
		back: if (n == 0) {
			*cp = (char) cps;
			*cb = (char) bd;
			continue;
		}
		if ((u = *h3) > (x = *h1)) {
			cpn = x;
			y = *f3;
			if (y == x) {
				bn = 1;
				goto compl_;
			}
			z = *f1;
			if (y > z) {
				bn = x - y + 1;
				goto compl_;
			}
			// else (y <= z)
			bn = x - z;
			goto compl_;
		}

		if (u == x) // *h3 == *h1
		{
			cpn = x;
			y = *f3;
			z = *f1;
			if (y > z) {
				bn = x - y + 1;
				goto compl_;
			}
			bn = x - z;
			goto compl_;
		}
		// else (*h3 < * h1)
		cpn = u + 1;
		y = *f3;
		z = *f1;
		if (u == z) {
			bn = 1;
			goto compl_;
		}
		if (y > z) {
			bn = x - y + 1;
			goto compl_;
		}
		bn = x - z;
		goto compl_;
		compl_: if (bn <= bd) {
			*cp = (char) cpn;
			*cb = (char) bn;
		} else {
			*cp = (char) cps;
			*cb = (char) bd;
		}
	}
}

static int16_t make_extrem(char func[], struct extrem_elem extr[]) {
	int16_t N, n;
	char prevx, prevf, prevd;
	char curf, curd;
	struct extrem_elem *el;

	ExtreM = 0;
	Extrem = 127;
	N = 0;
	el = extr;
	el->val = func[0];
	prevf = func[0];
	prevx = 0;
	prevd = 0;
	for (n = 1; n < totalh; n++) {
		curf = func[n];
		curd = curf - prevf;
		if (!curd)
			continue; // same value
		if (prevx == 0) {
			el->end = n - 1;
			el->beg = 0;
			el->type = -curd;
			el->val = func[0];
			goto jump;
		}
		if ((curd ^ prevd) & 0x80) // another sign of diff ?
		{
			N++;
			el++;
			el->type = prevd; // diff sign === extremum type
			el->beg = prevx;
			el->end = n - 1;
			el->val = prevf;
			if (el->type >= 0) {
				if (ExtreM < prevf) {
					ExtreM = prevf;
					ExtreX = prevx;
				}
			} else if (Extrem > prevf) {
				Extrem = prevf;
				Extrex = prevx;
			}
			goto jump;
		}
		jump: prevf = curf;
		prevx = (char) n;
		prevd = curd;
	}

	N++;
	el++;
	el->val = curf;
	el->type = 0; /*-prevd;*/// no extrem on right bound
	el->end = (char) totalh;
	el->beg = prevx;
	N++;
	return (N);
}

static int16_t make_absextr(char f, struct extrem_elem extr[]) {
	int16_t N, n;
	char prevx, prevf, prevd;
	char curf, curd;
	struct extrem_elem *el;

	ExtreM = 0;
	Extrem = 127;
	N = 0;
	el = extr;
	if (f)
		prevf = el->val = points[0].botf;
	else
		prevf = el->val = points[0].toph;
	prevx = 0;
	prevd = 0;
	for (n = 1; n < t_height /*totalh*/; n++) {
		if (f)
			curf = points[n].botf;
		else
			curf = points[n].toph;
		curd = curf - prevf;
		if (!curd)
			continue; // same value
		if (prevx == 0) {
			el->end = n - 1;
			el->beg = 0;
			el->type = -curd;
			if (f)
				el->val = points[0].botf;
			else
				el->val = points[0].toph;
			goto jump;
		}
		if ((curd ^ prevd) & 0x80) // another sign of diff ?
		{
			N++;
			el++;
			el->type = prevd; // diff sign === extremum type */
			el->beg = prevx;
			el->end = n - 1;
			el->val = prevf;
			if (el->type > 0) {
				if (ExtreM < prevf) {
					ExtreM = prevf;
					ExtreX = prevx;
				}
			} else {
				if (Extrem > prevf) {
					Extrem = prevf;
					Extrex = prevx;
				}
			}
			goto jump;
		}
		jump: prevf = curf;
		prevx = (char) n;
		prevd = curd;
	}

	N++;
	el++;
	el->val = curf;
	el->type = 0 /*-prevd*/; // no extrem on right bound
	el->end = (char) t_height /*totalh*/;
	el->beg = prevx;
	N++;
	return (N);
}

static void make_info() {
	int16_t nx, nl;
	struct short_line_header *l_ptr;
	struct segment *s_ptr;
	struct info_elm *inf;

	inf = &points[0];
	for (nx = 0; nx < t_height /*totalh*/; nx++, inf++) {
		inf->topline = inf->botline = -1;
		inf->toph = inf->topf = inf->sumb = inf->lnum = 0;
		inf->both = inf->botf = 127;
	}
	for (nl = 0; nl < max_line; nl++) {
		l_ptr = &Ed_lines[nl];
		s_ptr = (struct segment *) ((uchar *) t_line_ptr + l_ptr->segm_addr);

		for (nx = l_ptr->top; nx < l_ptr->bot; nx++, s_ptr++) {
			inf = &points[nx];
			inf->sumb += s_ptr->lth;
			inf->lnum++;
			if (s_ptr->end >= inf->toph) {
				inf->topf = s_ptr->end - s_ptr->lth;
				inf->toph = s_ptr->end;
				inf->topline = (char) nl;
			}
			if (s_ptr->end <= inf->botf) {
				inf->botf = s_ptr->end - s_ptr->lth;
				inf->both = s_ptr->end;
				inf->botline = (char) nl;
			}
		}
	}
}

int16_t cut_points(int16_t width, int16_t height, uchar *r, struct cut_elm *ans) {
	char n, a, i;
	struct cut_elm *a1;
	struct own_cut oct[STK_H], *oc1;
	Z = &string;
	odrans = oct;

	for (n = 1; n < STK_H; n++) {
		oct[n].var = 0;
		oct[n].x = 127;
	}

	begin(width, height); // beginning of work
	s_raster = r;
	c_rastror(s_raster, t_raster(), s_width, s_height);
	// turn s_raster for 90 degrees clockwise
	t_comp_ptr = c_locomp(t_raster(), t_width_b, t_height, 0,
			(int16_t) (-t_left_shift));
	// extraction components from t_raster
	if (t_comp_ptr == NULL)
		return -1;
	t_line_ptr = c_boxln(t_comp_ptr);
	// box presentation of a component ->
	//   line presentation of the component
	if ((i = (char) make_graph()) <= 0) // make graph of shortened lines for
		//   turned raster
		return i;
	if (i == 1) {
		Z = &string;
		path_lth = 1;
		path[0] = 0;
		totalh = 0; // simulate graph
		make_func();
		make_info();
		make_limits();
		minlincut(0);
		goto ret;
		// only one line in component
	}
	Z = &string;
	find_path(); // find maximal path from left side
	//    to right side
	Z = &string;
	excl_connect(); // check connectivity components of graph after
	//    deletion of each vertex of the path
	Z = &string;
	memset(prot_dum, 32, prot_size);
	make_func();
	make_info();
	make_limits();
	allcuts();
	interlines();
	minlincut(1);
	minlincut(2);
	minlincut(3);

	ret: ;
	for (a1 = &ans[1], oc1 = &oct[1], n = 0, a = 0; n < STK_H - 1; n++) {
		a1->x = oc1->x;
		a1->h = oc1->h;
		a1->dh = oc1->dh;
		a1->var = oc1->var;
		if (a1->x == 127)
			break;
		a++;
		a1++;
		oc1++;
	}
	return a;
}
/////////////////////
int16_t Alik_cut_points(int16_t width, int16_t height, uchar *r,
		struct cut_elm *ans, int16_t row) {
	char n, a, i;
	struct cut_elm *a1;
	struct own_cut oct[STK_H + 1];
	struct own_cut *oc1;

	memset(&oct[STK_H], 0xFF, sizeof(struct own_cut));

	Z = &string;
	odrans = oct;
	for (n = 1; n < STK_H; n++) {
		oct[n].var = 0;
		oct[n].x = 127;
	}
	begin(width, height); // beginning of work

	memcpy(ForRaster1, r, s_width_b * s_height);

	s_raster = r;
	c_rastror(s_raster, t_raster(), s_width, s_height); // turn s_raster for 90 degrees clockwise
	t_comp_ptr = c_locomp(t_raster(), t_width_b, t_height, 0,
			(int16_t) (-t_left_shift)); // extraction components from t_raster
	if (t_comp_ptr == NULL)
		return -1;
	t_line_ptr = c_boxln(t_comp_ptr); // box presentation of a component ->  line presentation of the component
	if ((i = (char) make_graph()) <= 0 && width < 20)
		return i; // make graph of shortened lines for  turned raster

	//Alik_define_cut_points((pchar)ForRaster1,&oct[1],width,height,row);
	RCUTP_SetBL_for_CutPoints(minrow, bbs1, bbs2, bbs3, bbs4, Nb1, Nb2, Nb3,
			language);
	RCUTP_CutPoints((uchar*) ForRaster1, &oct[1], width, height, row);

	for (a1 = &ans[1], oc1 = &oct[1], n = 0, a = 0; n < STK_H - 1; n++) {
		a1->x = oc1->x - 1;
		a1->h = oc1->h;
		a1->dh = oc1->dh;
		a1->var = oc1->var;
		if (a1->x == 126) {
			a1->x = 127;
			break;
		}
		a++;
		a1++;
		oc1++;
	}
	return a;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AK changed 26.03.98
MN *cut_rast(puchar r, int16_t width, int16_t height, int16_t upper,
		int16_t left, struct cut_elm *cuts, char p, char flg, char *svp,
		cut_pos *cpos) {
	uchar *c, *s;
	uchar b, bt, bs;
	char x, h, dh, fl;
	int16_t totc, dx, y, r0, tr1, tr2, r1, r2, f1;
	//  flg - request type :
	//  0 -  glue back
	//  1 -  make cut
	//  2 -  make cut & create components
	//  3 -  create components after all cuts made

	if (flg == 3)
		return (c_locomp(r, (int16_t) ((width + 7) >> 3), height, upper, left));

	if (flg)
		memset(svp, 0, 16);

	// if (flg != 0)                       // to store pattern for full cuts
	//   for (i=0; i<16; i++) svp[i]=0;
	cpos->flag = 0;
	totc = 0;

	x = cuts[p].x;
	h = cuts[p].h;
	dh = cuts[p].dh;
	r0 = height - h;

	if ((cuts[p].var & 32) && (flg == 0)) // full restore
	{
		dx = (width + 7) >> 3;
		c = r + x / 8 + dx * r0; // H-end
		bt = 0x80 >> (x % 8);
		bs = 0x80;
		s = (uchar*) svp;

		for (y = 0; y < dh; y++, c += dx) {
			if ((char*) s < svp || (char*) s > (svp + 32)) //AK! add crash debug
			{
				continue;
			}

			if (*s & bs)
				*c |= bt;

			bs = bs >> 1;

			if (bs == 0) {
				bs = 0x80;
				s++;
			}
		}
		return 0;
	}

	fl = 0;
	f1 = 0;

	if (cuts[p].var & 64) {
		fl = cuts[p].dh;
		f1 |= 4;
		dh = 1;
	} // horizontal, sophisticated cut

	r1 = r2 = -1;
	bs = 0x80;
	s = (uchar*) svp;
	tr1 = tr2 = -1;
	////////
	agacut: ;
	dx = (width + 7) >> 3;
	c = r + x / 8 + dx * r0; // H-end
	bt = 0x80 >> (x % 8);
	b = bt ^ 0xff;
	if (flg) // cut
	{
		for (y = 0; y < dh; y++, r0++, c += dx) {
			if ((char*) s < svp || (char*) s > (svp + 32)) //AK! add
			{
				continue;
			}

			if ((uchar)(*c & bt) != 0) // interval starts or continues
			{
				// Lepik:
				if (!cuts_point_methode)
					if ((fl == 0) && (!(f1 & 1))) // beginning of vertical interval
					{
						int16_t ret;
						ret = SearchObjezd(x, c, width, y, dh, r0, height,
								(int16_t) (height - h));
						if (ret) {
							ret -= 1;
							y += ret;
							r0 += ret;
							c += (dx * ret);
							while (ret-- >= 0) {
								*s |= bs;
								bs = (uchar)(bs >> 1);
								if (bs == 0) {
									bs = 0x80;
									s++;
								}
							}

							continue;
						}
					}
				//
				totc++;

				if (tr1 < 0)
					tr1 = r0;

				if (f1 & 2)
					f1 |= 4; // start after end - sophisticated cut
				f1 |= 1; // interval start
				r2 = r0;
				*s |= bs;
				if (r1 < 0)
					r1 = r0;
			}
			//  Commented by Le, 12-02-94 11:58am.
			//   else            // interval ended or not started yet
			//     if (r1>=0)    // not a contigious interval
			//        f1 |= 2;   // interval end
			//
			// Instead:
			else if (f1 & 1) // if was.
			{
				f1 &= 0xfe;
				f1 |= 2;
			};
			//
			bs = (uchar)(bs >> 1);

			if (bs == 0) {
				bs = 0x80;
				s++;
			}

			*c &= b;
		}
	} else // glue
	{
		for (y = 0; y < dh; y++, c += dx) {
			if ((char*) s < svp || (char*) s > (svp + 32)) //AK! add
			{
				continue;
			}

			if (*s & bs)
				*c |= bt;

			bs = (uchar)(bs >> 1);

			if (bs == 0) {
				bs = 0x80;
				s++;
			}
		}
	}
	x++;
	fl--;

	if (flg && (r1 >= 0)) // return cut position;
	{
		cpos->row1 = tr1 + upper;

		/**/
		cpos->row2 = r2 + upper;
		//
		// Le: to cover possible gap:
		if (f1 & 2)
			f1 |= 1;
		//
		cpos->flag |= f1; // store cut type
		cpos->cuth = totc; // no of pixels cut -- to prevent "near a stick cuts"
		if (f1 & 4) {
			if (3* totc > (r2 - tr1))
				cpos->flag |= 8; // thick
		}
	}

	if (fl > 0) {
		r0 = height - h;
		r1 = r2 = -1;
		f1 &= 0xfe;
		goto agacut;
	}

	if (flg == 2)
		return (c_locomp(r, (int16_t) ((width + 7) >> 3), height, upper, left));

	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define obLook  4
//                       0    1    2    3    4    5    6    7
static uchar bits[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
static uchar lstr[8] = { 0x80, 0xC0, 0xE0, 0xF0, 0x78, 0x3C, 0x1E, 0x0F };
static uchar rstr[8] = { 0xF0, 0x78, 0x3C, 0x1E, 0x0F, 0x07, 0x03, 0x01 };
static uchar aux[8] = { 0x07, 0x03, 0x01, 0x00, 0x00, 0x80, 0xC0, 0xE0 };

int16_t SearchObjezd(int16_t x, // column number where cutting is going.
		puchar c, // pointer to curr byte in raster
		int16_t width, // width of raster (pixs)
		int16_t y, // current row (in relative coords)
		int16_t dh, // no of rows to investigate(rel coords)
		int16_t r0, // first row (in rast coords)
		int16_t height, // height of raster
		int16_t beg_y) // first row for cut
{
	int16_t i;
	puchar cc;
	int16_t dx; //  width of raster in bytes.
	int16_t bitNo; // Curr Bit No in byte
	uchar CurB, aCurB, Mask, aMask, Accu, aAccu;
	int16_t strategy; // 0 - left and right strips in one byte,
	// 1 - right split by byte,
	// 2 - left   split by byte.


	if (x + 1 < obLook || width - x < obLook) // not investigate margins.
		return 0;
	bitNo = x % 8;
	dx = (width + 7) >> 3;

	// the following block searches the last row of interval.
	{
		uchar b;
		b = 0x80 >> bitNo;
		for (i = y + 1, cc = c + dx; i < dh; i++, cc += dx)
			if (!(*cc & b))
				break;
		dh = i;
	}
	// strategy determination
	if ((x + 1 - obLook) >> 3 != x >> 3)
		strategy = 2;
	else if ((x + obLook - 1) >> 3 != x >> 3)
		strategy = 1;
	else
		strategy = 0;
	// strategies implementation

	switch (strategy) {
	case 0:
		// left strip investigation
		Mask = lstr[bitNo];
		Accu = 0;
		for (i = y, cc = c; i < dh; i++, cc += dx) {
			CurB = *cc;
			if ((CurB &= Mask) == Mask)
				break;
			if ((Accu |= CurB) == Mask)
				break;
		}
		;
		if (i == dh) //   upper/lower testings
		{
			i = bitNo;
			while (--i > 0) {
				if (!(Accu & bits[i])) {
					Accu |= bits[i];
					while (--i >= 0)
						Accu &= (~bits[i]);
				}
			}
			if (((r0 == 0) || ((*(c - dx) & Accu) == 0)) && ((beg_y + dh
					== height) || ((*cc & Accu) == 0))) {
				if (db_status && snap_activity(db_pass) && (db_trace_flag
						& 0x02)) {
					char w[120];
					sprintf(w, "interval %d(%d,%d) will not be cut: l found",
							x, r0, dh + beg_y);
					snap_show_text(w);
					snap_monitor();
				}
				return dh - y;
			}
		}
		// right strip investigation
		Mask = rstr[bitNo];
		Accu = 0;
		for (i = y, cc = c; i < dh; i++, cc += dx) {
			CurB = *cc;
			if ((CurB &= Mask) == Mask)
				break;
			if ((Accu |= CurB) == Mask)
				break;
		}
		;
		if (i == dh) //   upper/lower testings
		{
			i = bitNo;
			while (++i < 8) {
				if (!(Accu & bits[i])) {
					Accu |= bits[i];
					while (++i < 8)
						Accu &= (~bits[i]);
				}
			}
			if (((r0 == 0) || ((*(c - dx) & Accu) == 0)) && ((beg_y + dh
					== height) || ((*cc & Accu) == 0))) {
				if (db_status && snap_activity(db_pass) && (db_trace_flag
						& 0x02)) {
					char w[120];
					sprintf(w, "interval %d(%d,%d) will not be cut: r found",
							x, r0, dh + beg_y);
					snap_show_text(w);
					snap_monitor();
				}
				return dh - y;
			}
		}
		return 0;
	case 1:
		// left strip investigation
		Mask = lstr[bitNo];
		Accu = 0;
		for (i = y, cc = c; i < dh; i++, cc += dx) {
			CurB = *cc;
			if ((CurB &= Mask) == Mask)
				break;
			if ((Accu |= CurB) == Mask)
				break;
		}
		;
		if (i == dh) // upper/lower testings
		{
			i = bitNo;
			while (--i > 0) {
				if (!(Accu & bits[i])) {
					Accu |= bits[i];
					while (--i >= 0)
						Accu &= (~bits[i]);
				}
			}
			if (((r0 == 0) || ((*(c - dx) & Accu) == 0)) && ((beg_y + dh
					== height) || ((*cc & Accu) == 0))) {
				if (db_status && snap_activity(db_pass) && (db_trace_flag
						& 0x02)) {
					char w[120];
					sprintf(w, "interval %d(%d,%d) will not be cut:l found", x,
							r0, dh + beg_y);
					snap_show_text(w);
					snap_monitor();
				}
				return dh - y;
			}
		}
		// right strip investigation
		Mask = rstr[bitNo];
		aMask = aux[bitNo];
		Accu = 0;
		aAccu = 0;
		for (i = y, cc = c; i < dh; i++, cc += dx) {
			CurB = *cc & Mask;
			aCurB = *(cc + 1) & aMask;
			if ((CurB == Mask) && (aCurB == aMask))
				break;
			Accu |= CurB;
			aAccu |= aCurB;
			if ((Accu == Mask) && (aAccu == aMask))
				break;
		}
		;
		if (i == dh) // upper/lower testings
		{
			if (((r0 == 0) || (((*(c - dx) & Accu) == 0) && ((*(c - dx + 1)
					& aAccu) == 0))) && ((beg_y + dh == height) || (((*cc
					& Accu) == 0) && ((*(cc + 1) & aAccu) == 0)))) {
				if (db_status && snap_activity(db_pass) && (db_trace_flag
						& 0x02)) {
					char w[120];
					sprintf(w, "interval %d(%d,%d) will not be cut:r(c) found",
							x, r0, dh + beg_y);
					snap_show_text(w);
					snap_monitor();
				}
				return dh - y;
			}
		}

		return 0;
	case 2:
		// right strip investigation
		Mask = rstr[bitNo];
		Accu = 0;
		for (i = y, cc = c; i < dh; i++, cc += dx) {
			CurB = *cc;
			if ((CurB &= Mask) == Mask)
				break;
			if ((Accu |= CurB) == Mask)
				break;
		}
		;
		if (i == dh) // upper/lower testings
		{
			i = bitNo;
			while (++i < 8) {
				if (!(Accu & bits[i])) {
					Accu |= bits[i];
					while (++i < 8)
						Accu &= (~bits[i]);
				}
			}
			if (((r0 == 0) || ((*(c - dx) & Accu) == 0)) && ((beg_y + dh
					== height) || ((*cc & Accu) == 0))) {
				if (db_status && snap_activity(db_pass) && (db_trace_flag
						& 0x02)) {
					char w[120];
					sprintf(w, "interval %d(%d,%d) will not be cut: r found",
							x, r0, dh + beg_y);
					snap_show_text(w);
					snap_monitor();
				}
				return dh - y;
			}
		}
		// left strip investigation
		Mask = lstr[bitNo];
		aMask = aux[bitNo];
		Accu = 0;
		aAccu = 0;
		for (i = y, cc = c; i < dh; i++, cc += dx) {
			CurB = *cc & Mask;
			aCurB = *(cc - 1) & aMask;
			if ((CurB == Mask) && (aCurB == aMask))
				break;
			Accu |= CurB;
			aAccu |= aCurB;
			if ((Accu == Mask) && (aAccu == aMask))
				break;
		}
		;
		if (i == dh) // upper/lower testings
		{
			if (((r0 == 0) || (((*(c - dx) & Accu) == 0) && ((*(c - dx - 1)
					& aAccu) == 0))) && ((beg_y + dh == height) || (((*cc
					& Accu) == 0) && ((*(cc - 1) & aAccu) == 0)))) {
				if (db_status && snap_activity(db_pass) && (db_trace_flag
						& 0x02)) {
					char w[120];
					sprintf(w,
							"Interval %d(%d,%d) will not be cut: l(c) found",
							x, r0, dh + beg_y);
					snap_show_text(w);
					snap_monitor();
				}
				return dh - y;
			}
		}
		return 0;
	}
	return 0;
}
;

