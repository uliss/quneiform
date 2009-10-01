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
#include <string.h>
#include <stdio.h>

#include "status.h"
#include "cstr.h"
#include "linear.h"
#include "ligas.h"
#include "lang_def.h"	// 08.09.2000 E.P.
#include "minmax.h"

extern line_BL;
extern CSTR_line lin_str;

static void set_basint(void);
static void make_intpairs(void);

int16_t int_caps(bas_ln *bc);
int16_t int_smalls(bas_ln *bc);
int16_t int_nocap(bas_ln *bc, int16_t);
int16_t int_nosmall(bas_ln *bc);
int16_t cell_sticker(CSTR_rast bc);
int16_t int_sticker(bas_ln *bc);
int16_t int_similar(bas_ln *b1, bas_ln *b2);
int16_t int_sim_base2(bas_ln *b1, bas_ln *b2);
int16_t int_sim_base1(bas_ln *b1, bas_ln *b2);

int16_t stick_like(CSTR_rast r) {
	//version * v;
	uchar c;
	CSTR_rast_attr trr;
	CSTR_rast_attr *tr = &trr;
	UniVersions v;
	UniVersions *vers = &v;
	int i;

	CSTR_GetAttr(r, tr);
	if ((tr->flg & CSTR_f_let) == 0)
		return 0;
	CSTR_GetCollectionUni(r, vers);
	//for ( v=tr->vers; (c=v->let); v++)
	for (i = 0; i < vers->lnAltCnt; i++) {
		c = vers->Alt[i].Liga;
		if (!memchr("LTJ()<>[]trI1ijl!/", c, 18) && (c != liga_i) && !(language
				== LANG_TURKISH && // 30.05.2002 E.P.
				(c == i_sans_accent || c == II_dot_accent)))
			return 0;
	}
	return 1;
}

int16_t get_maxmax(uchar *begs) {
	int16_t i, m, m1, lb1;
	uchar *bp, *bc, *bn;
	m1 = 0;
	lb1 = -1;
	for (i = 1, bp = begs, bc = bp + 1, bn = bc + 1; i < (bbs3 + 32 - 5); i++, bp++, bc++, bn++) {
		if ((*bc < *bp) || (*bc < *bn))
			continue;
		m = *bp + 2* (* bc) + *bn;
		if (m1 < m)
		{	m1=m; lb1=i;}
	}
	return lb1;
}

int16_t can_serve(CSTR_rast b, int16_t bs, int16_t pass) {
	CSTR_rast_attr bb2;
	CSTR_rast_attr *b2 = &bb2;
	UniVersions vv;
	UniVersions *v0 = &vv;
	uchar chr;
	int i;

	CSTR_GetAttr(b, b2);
	if (b2->basflg & CSTR_bs_done)
		return 0;
	if (b2->basflg & CSTR_bs_bt)
		return 0; // 't' not to serve as initiators

	CSTR_GetCollectionUni(b, v0);

	if (v0->lnAltCnt <= 0)
		return 0;

	if (pass == 2) // admit cells at b1
		return 1;
	if (bs && ((b2->difflg & bs) == 0))
		return 0; // not defines base - skip
	if (pass)
		return 1; // any version OK at pass 1; avoid sticks at pass 0

	for (i = 0; i < v0->lnAltCnt; i++) {
		chr = v0->Alt[i].Liga;
		if (!memchr("LTJ()<>[]trI1l!/", chr, 16) && (chr != liga_i)
				&& !(language == LANG_TURKISH && // 30.05.2002 E.P.
						(chr == i_sans_accent || chr == II_dot_accent)))
			return 1; // "non-stick" available
	}
	return 0;
}

static int init_type;

CSTR_rast def_init_cell() {
	CSTR_rast b1, b2, b3;
	CSTR_rast_attr aab2;
	CSTR_rast_attr *ab2 = &aab2;

	int16_t up, dflg, nmax, nacc, pass;

	Start_cell = NULL;
	pass = 0;
	again: if (b2_solid) {
		int16_t w;
		int16_t dmin = 32000;
		b1 = NULL;
		b2 = cell_f(); // CSTR_GetFirstRaster(lin_str);
		while (b2) {
			b2 = CSTR_GetNextRaster(b2, f_letter);
			if (!b2)
				break;

			if (!can_serve(b2, 2, pass))
				continue;

			CSTR_GetAttr(b2, ab2);
			up = ab2->row - minrow - ab2->bdiff;
			if ((w = abs(up - bbs2)) < dmin) {
				dmin = w;
				Start_cell = b2;
			}
		}

		if (dmin < 3) {
			CSTR_GetAttr(Start_cell, ab2);
			init_type = ab2->difflg;
			return Start_cell;
		}
	}
	nmax = 0;
	b1 = NULL;
	b2 = cell_f();
	//while ( (b2=b2->nextl)->nextl != NULL)
	while (b2) {
		b2 = CSTR_GetNextRaster(b2, f_letter);
		if (!b2)
			break;

		if (!can_serve(b2, 3, pass))
			continue;

		CSTR_GetAttr(b2, ab2);
		up = ab2->row - minrow - ab2->bdiff;
		dflg = ab2->difflg & 3;
		nacc = 0;
		b3 = cell_f();
		while (b3) {
			b3 = CSTR_GetNextRaster(b3, f_letter);
			if (!b3)
				break;

			//  if (b3->difflg & dflg)
			CSTR_GetAttr(b3, ab2);
			if (ab2->basflg & dflg) // agrees to be same type ?
			{
				int16_t rw;
				rw = ab2->row - minrow - ab2->bdiff;
				if (abs(rw - up) < 2)
					nacc++;
			}
		}
		if (nacc > nmax) {
			nmax = nacc;
			Start_cell = b2;
		}
	}
	pass++;
	if ((pass == 1) && (Start_cell == NULL))
		goto again;
	if (Start_cell) {
		CSTR_GetAttr(Start_cell, ab2);
		init_type = ab2->difflg;
		return Start_cell;
	}

	if (multi_bas & 4)
	// killed cells - accept cells at b1
	{
		init_type = CSTR_db_b1;
		nmax = 0;
		b1 = NULL;
		b2 = cell_f();
		//while ((b2=b2->nextl)->nextl != NULL)
		while (b2) {
			b2 = CSTR_GetNextRaster(b2, f_letter);
			if (!b2)
				break;

			if (!can_serve(b2, 0, pass))
				continue;

			CSTR_GetAttr(b2, ab2);
			up = ab2->row - minrow - ab2->bdiff;
			if (abs(up - obs1) >= 3)
				continue;
			nacc = 0;
			b3 = cell_f();
			while (b3)
			// (b3=b3->nextl)->nextl != NULL)
			{
				b3 = CSTR_GetNextRaster(b3, f_letter);
				if (!b3)
					break;

				CSTR_GetAttr(b3, ab2);

				if (ab2->basflg & CSTR_bs_b1a) // agrees to be at B1 ?
				{
					int16_t rw;

					rw = ab2->row - minrow - ab2->bdiff;
					if (abs(rw - up) < 2)
						nacc++;
				}
			}
			if (nacc > nmax) {
				nmax = nacc;
				Start_cell = b2;
			}
		} // while

	} // if killed
	return Start_cell;
}

// void make_intpairs();
/*
 // если arg==0 - добавить мусор из строки lin_str
 // проверить/изменить метки мусор-нет
 void bring_dust(int16_t arg)
 {
 if (arg==0)
 {
 dust_ini(lin_str);
 glsnap('d',cell_f()->next,"dust initted");
 dust_in = 1;
 }
 all_cell_ledust();
 glsnap('d',cell_f()->next,"letters to dust");
 dust_to_let();
 glsnap('d',cell_f()->next,"dust to letters");
 discrim_by_dust();
 glsnap('d',cell_f()->next,"discrim by dust");
 }

 //////////////////
 */
void stand_bas() {
	int16_t i, j;
	int32_t w1, w2;
	// b3 assured
	Nb2 = Ns2;
	Nb1 = Ns1;
	Nb4 = Ns4;
	if (Ns1)
		bbs1 = (sbs1 + (Ns1 >> 1)) / Ns1;
	else
		Nb1 = -1;
	if (Ns4)
		bbs4 = (sbs4 + (Ns4 >> 1)) / Ns4;
	else
		Nb4 = -1;
	if (Ns2) {
		if (3* Ns2 > Ns3)
			bbs2 = (sbs2 + (Ns2 >> 1)) / Ns2;
		else {
			int16_t i;
			w1 = sbs3;
			w1 *= Ns2;
			w2 = sbs2;
			w2 *= Ns3;
			w1 -= w2;
			j = Ns2 * Ns3;
			i = (w1 + j / 2) / j;
			bbs2 = bbs3 - i;
		}
		if (Nb1 < 0)
			bbs1 = (3* bbs2 - bbs3) >> 1;
		if (Nb4 < 0)
			bbs4 = (3* bbs3 - bbs2) >> 1;
	}

	else // B2 absent
	{
		if (Nb1 > 0) {
			bbs2 = ((bbs1 << 1) + bbs3) / 3;
			if (Nb4 < 0)
				bbs4 = ((bbs3 << 2) - bbs1) / 3;
		} else // B1, B2 absent
		{
			if (Nb4 > 0) {
				i = bbs4 - bbs3;
				bbs2 = bbs3 - (i << 1);
				bbs1 = bbs2 - i;
			} else // B3 only present
			{
				bbs1 = sbsu / ncbs;
				bbs2 = ((bbs1 << 1) + bbs3) / 3;
				bbs4 = ((bbs3 << 2) - bbs1) / 3;
				{
					CSTR_rast B1;
					CSTR_rast_attr at;
					B1 = cell_f();
					//while  ((B1=B1->next)->next)
					while (B1 = CSTR_GetNext(B1)) {
						CSTR_GetAttr(B1, &at);
						at.bdiff = 0;
						CSTR_SetAttr(B1, &at);
					}
				}
				bs_got = 0;
				goto finpspsf;
			}
		}
	}
	// check mutually
	if (bbs2 < bbs1) {
		if (Nb2 > Nb1) // b2 more reliable
			bbs1 = (3* bbs2 - bbs3) >> 1;
		else
			bbs2 = ((bbs1 << 1) + bbs3) / 3;
	}
	if (bbs3 > bbs4) {
		if (Nb3 > Nb4) // b3 more reliable
			bbs4 = (3* bbs3 - bbs2) >> 1;
		else
			bbs3 = ((bbs4 << 1) + bbs2) / 3;
	}

	if ((8* (bbs2 -bbs1)) < (bbs3-bbs2)) // upper level doubtful
			{
				if ((Nb1 < 4) && ((Nb1+Nb1) <= Nb2))
				{	bbs1=(3*bbs2-bbs3)>>1; Nb1=-1; goto setb;}
				if ((Nb2 < 4) && ((Nb2+Nb2) <= Nb1))
				{	bbs2=(bbs3+(bbs1<<1))/3; Nb2=-1; goto setb;}
			}
			if (bbs2 > (bbs1+(bbs3-bbs1)/2))
			{
				bbs2 = (bbs3+2*bbs1)/3;
				Nb2=-1; goto setb;
			}
			if (bbs2 < (bbs1+(bbs3-bbs1)/8))
			{
				bbs2 = (bbs3+2*bbs1)/3;
				Nb2=-1; goto setb;
			}
			setb:
			bs_got=1;
			finpspsf:
			Ps=Psf=bbs3-bbs2;
			if (fax1x2) Psf +=2;
			if (Ps <= 0)
			{	Ps = Psf = 1;
				bbs2=bbs3-1;
				if (bbs1 >= bbs2)
				bbs1 = bbs2-1;
			}
			bbsm=(bbs2+bbs3)>>1;
		}

void cut_sunk_let() {
	CSTR_rast wc;
	uchar chr, tbe, flcut;
	//version *v0;
	int16_t nv, rowup, rowdn;
	//c_comp *env;
	CCOM_comp *env;
	uchar tshp;
	uchar fl1, fl2, fl2t, flt, t_type;
	uchar cap_shape, pos;
	uchar above_b1;
	UniVersions vers, *v0;
	CSTR_rast_attr attr;
	int i, j;

	wc = cell_f();
	wc = CSTR_GetNextRaster(wc, f_letter);

	while (wc) {
		CSTR_GetCollectionUni(wc, &vers);

		v0 = &vers;
		nv = (int16_t) vers.lnAltCnt;

		if (nv <= 0)
			goto forw; // bad cell

		flcut = 0;
		CSTR_GetAttr(wc, &attr);

		rowdn = attr.row + attr.h - minrow - attr.bdiff;
		rowup = rowdn - attr.h;
		fl1 = 0xff; // all lines defined
		fl2 = fl2t = 0;
		flt = 0;

		//env=wc->env;
		env = CSTR_GetComp(wc);

		attr.basflg = 0;
		CSTR_SetAttr(wc, &attr);

		t_type = 0;
		cap_shape = 1; // capital in shape
		above_b1 = 0;

		if (rowdn < bbsm) // keep anomalies out of sight
		{
			attr.flg = CSTR_f_dust;
			CSTR_SetAttr(wc, &attr); // wc = B1 !
			goto forw;
		}

		for (i = 0; i < vers.lnAltCnt; i++) {
			chr = vers.Alt[i].Liga;
			pos = let_linpos[chr];

			switch (pos & 0x0f) {
			case 1:
				if (abs(rowdn - bbs3) > 3)
					goto delvers;
				// all on b3
				break;
			case 3:
				if (rowdn - bbs3 < 3)
					goto delvers;
				// 'p'
				break;
			case 6:
				if (bbs3 - rowdn > 3)
					goto delvers;
				// 'f'
				break;
				// case 4: quest_excl = 1;
			}
			if (chr == 'n') {
				if ((bbs2 - rowup) > 2)
					goto delvers;
			}

			tbe = let_lindef3[chr];
			tshp = let_linshape[chr];
			cap_shape &= tshp;
			above_b1 |= tshp;
			if (chr == 't') {
				flt = CSTR_bs_bt;
				t_type |= 1;
			} // 't' obtained

			//    if (env && (env->large & ch_underlined) || (wc->flg_new&CSTR_fn_under))
			if (env && (env->large & CCOM_LR_UNDERLINED) || (attr.flg_new
					& CSTR_fn_under)) {
				if (tshp & 2) // sticky in shape
				{
					fl1 = 0;
					continue;
				} // no lines defined
			}

			fl2t |= tbe; // accumulate b1/b2
			fl2 |= tbe; // accumulate b1/b2
			if ((tbe & 1) && ((t_type & 4) == 0)) // bs1 defined, not by stick
				t_type |= 2; // large
			t_type &= 3; // retain accumulated 't' and 'large'

			fl1 &= tbe;
			continue;
			delvers: for (j = i; j < vers.lnAltCnt - 1; j++)
				vers.Alt[j] = vers.Alt[j + 1];
			memset(&vers.Alt[vers.lnAltCnt - 1], 0, sizeof(UniAlt));
			vers.lnAltCnt--;
			i--;
			flcut++;
		}

		if (fl1 == 0xff) // no bases defined
		{
			attr.basflg = above_b1 & CSTR_bs_b1up;
			if (fl1 & c_df_round)
				attr.basflg |= CSTR_bs_round;
			if (cap_shape)
				attr.basflg |= CSTR_bs_cap;
		} else {
			attr.basflg = flt; // agree to 't' type
			if (fl1 & c_df_round)
				attr.basflg |= CSTR_bs_round;
			if (cap_shape)
				attr.basflg |= CSTR_bs_cap;
			attr.basflg |= (above_b1 & CSTR_bs_b1up);
			if (fl2 & c_df_twins) // a non_cutten version had a twin
			{
				if ((fl2t & (CSTR_db_b1 | CSTR_db_b2)) != (CSTR_db_b1
						| CSTR_db_b2))
				//  b1 or b2  were absent in full list
				{
					fl1 &= ~(CSTR_db_b1 | CSTR_db_b2); // forget b1/b2 as defined
					attr.basflg |= (CSTR_bs_b1a | CSTR_bs_b2a); // agrees to be both b1. b2
				}
			}
			if (fl2 & (CSTR_db_b1 | c_df_b1a))
				attr.basflg |= CSTR_bs_b1a;
			if (fl2 & (CSTR_db_b2 | c_df_b2a))
				attr.basflg |= CSTR_bs_b2a;
			attr.difflg = attr.difflg | (fl1 & 15);
			if (flt & (vers.lnAltCnt == 1))
				attr.basflg |= CSTR_bs_t; // "t-only" version
		}

		CSTR_SetAttr(wc, &attr);

		if (flcut) {
			// все убрали ?
			if (vers.lnAltCnt <= 0)
				set_bad_cell(wc);
			else
				CSTR_StoreCollectionUni(wc, &vers);
		}

		forw:
		// wc=wc->nextl;
		wc = CSTR_GetNextRaster(wc, f_letter);
	}

}

int16_t multi_hist(int16_t p) {
	CSTR_rast BC;
	CSTR_rast_attr attr;

	char txt[32];
	stand_bas();
	txt[0] = 0;
	if (db_status)
		sprintf(txt, "mult: %d", p);
	complete_bas(txt);
	multi_bas |= 0xc0;
	bs_int_no = 0;
	/**********/
#ifdef DBG
	// printf ("line: %d  mr: %d linpos: %d\n",line_number,sum_ans,linpos_arg);
#endif
	/**********/
	set_basarr(&all_bases[0], -32000, 32000);

	BC = cell_f();
	BC = CSTR_GetNextRaster(BC, f_letter);
	while (BC) {
		CSTR_GetAttr(BC, &attr);
		attr.basflg &= ~CSTR_bs_done; // reset "taken to interval"
		attr.bas_acc = 0;
		CSTR_SetAttr(BC, &attr);
		BC = CSTR_GetNextRaster(BC, f_letter);
	}

	BC = cell_f();
	while ((BC = CSTR_GetNextRaster(BC, f_letter)) != NULL)
		make_difbas(BC, 0x40);

	glsnap('d', cell_f()->next, "to cut to b3");
	cut_sunk_let();
	glsnap('d', cell_f()->next, "cut to b3 made");
	while (def_init_cell()) {
		set_int();
		fl_fail = 0;
		int_sbs1 = int_bs1;
		int_sbs2 = int_bs2;
		int_sbst = int_bst;
		int_sfbd = int_fbd;
		int_sfbs = int_fbs;
		int_smin2 = int_min2;
		int_smax2 = int_max2;
		int_smin1 = int_min1;
		int_smax1 = int_max1;
		int_smid1 = int_mid1;
		int_smid2 = int_mid2;
		int_ssumb1 = int_sumb1;
		int_ssumb2 = int_sumb2;
		int_snb1 = int_nb1;
		int_snb2 = int_nb2;
		int_snf = int_nf;
		int_supper = int_upper;
		int_slower = int_lower;
		int_sps1 = int_ps1;
		int_sps2 = int_ps2;

		Fail_cell = NULL;
		int_nf = 0; // no failures yet
		glsnap('d', Start_cell, "interval source");
		End_cell = Start_cell;

		while (1) {
			int16_t i;
			End_cell = CSTR_GetNextRaster(End_cell, f_letter | CSTR_f_fict);
			// End_cell->nextl;
			if ((i = same_int(End_cell, 1)) == 0)
				break;
			if ((i & 2) == 0)
				extend_int();
		}
		End_cell = CSTR_GetPrevRaster(End_cell, f_letter | CSTR_f_fict);

		if (Fail_Ecell) {
			Fail_cell = End_cell;
			End_cell = Fail_Ecell;
			Fail_Ecell = Fail_cell;
			if (End_cell == Fail_Ecell)
				Fail_Ecell = NULL;
		}
		glsnap('d', End_cell, "interval end");
		Fail_cell = NULL;
		int_nf = 0;
		fl_fail = 0;

		while (1) {
			int16_t i;
			Start_cell = CSTR_GetPrevRaster(Start_cell, f_letter | CSTR_f_fict);
			if ((i = same_int(Start_cell, 2)) == 0)
				break;
			if ((i & 2) == 0)
				extend_int();
		}

		//   Start_cell=Start_cell->nextl;
		Start_cell = CSTR_GetNextRaster(Start_cell, f_letter | CSTR_f_fict);
		if (Fail_Scell) {
			Fail_cell = Start_cell;
			Start_cell = Fail_Scell;
			Fail_Scell = Fail_cell;
			if (Start_cell == Fail_Scell)
				Fail_Scell = NULL;
		}
		glsnap('d', Start_cell, "interval start");
		set_basint();
	}
	make_intpairs();
	return 1;
}

void set_int()
// set baselines in last (bp->) interval using compound cell histogram
{
	int16_t ch;
	CSTR_rast_attr attr;
	CSTR_GetAttr(Start_cell, &attr);

	//int_up = int_upper = int_ini = Start_cell->row - minrow - Start_cell->bdiff;
	int_up = int_upper = int_ini = attr.row - minrow - attr.bdiff;
	int_dn = int_lower = int_upper + (ch = attr.h);
	int_dup = int_ddn = 1;
	if ((ch > 24) && (attr.basflg & CSTR_bs_round)) {
		int_ddn = 2;
	}
	int_bs1 = int_bs2 = int_bst = -128;
	int_fid = init_type; // Start_cell -> difflg;
	int_fis = attr.basflg;
	int_fbd = int_fbs = 0; // no bases accepted yet
	int_ps1 = int_ps2 = 0;
	if (int_fid & CSTR_db_b1) // large initiator
	{
		int_sumb1 = int_mid1 = int_min1 = int_max1 = int_up;
		int_nb1 = 1;
	}
	if (int_fid & CSTR_db_b2) // small initiator
	{
		int_sumb2 = int_mid2 = int_min2 = int_max2 = int_up;
		int_nb2 = 1;
	}
	if ((int_fid & (CSTR_db_b3 | CSTR_db_b2)) == (CSTR_db_b3 | CSTR_db_b2))
		int_ps2 = attr.h;
	if ((int_fid & (CSTR_db_b3 | CSTR_db_b1)) == (CSTR_db_b3 | CSTR_db_b1))
		int_ps1 = attr.h;
	//   RUndef=LUndef=NULL;
	Fail_Scell = Fail_Ecell = NULL;
	//mindef_col = Start_cell->col;
	//maxdef_col = Start_cell->col+Start_cell->w;
}

static uchar fbd, fbs;
int16_t same_int(CSTR_rast St, uchar dir) {
	int16_t ch, rb;
	CSTR_rast_attr attr;

	CSTR_GetAttr(St, &attr);
	if (attr.flg & CSTR_f_fict) {
		rb = 0;
		goto decide;
	}
	fbd = attr.difflg;
	fbs = attr.basflg;
	cur_up = attr.row - minrow - attr.bdiff;
	cur_h = ch = attr.h;
	cur_ps1 = cur_ps2 = 0;
	if ((fbd & (CSTR_db_b3 | CSTR_db_b2)) == (CSTR_db_b3 | CSTR_db_b2))
		cur_ps2 = cur_h;
	if ((fbd & (CSTR_db_b3 | CSTR_db_b1)) == (CSTR_db_b3 | CSTR_db_b1))
		cur_ps1 = cur_h;
	cur_dn = cur_up + ch;
	cur_dup = cur_ddn = 1;
	if ((ch > 24) && (fbs & CSTR_bs_round))
		cur_dup = 2;
	wrk_dup = MAX(cur_dup, int_dup);
	wrk_ddn = MAX(cur_ddn, int_ddn);
	if ((attr.flg & CSTR_f_let) && !(attr.difflg & 0x40)) {
		if (int_fid & CSTR_db_b1) // large initiator
			rb = comp_Llet(dir, St);
		else
			rb = comp_Slet(dir, St);
	} else {
		if (int_fid & CSTR_db_b1) // large initiator
			rb = comp_Lbad(St);
		else
			rb = comp_Sbad(St);
	}
	decide: if ((rb > 0) && (fl_fail == 0)) {
		int_sbs1 = int_bs1;
		int_sbs2 = int_bs2;
		int_sbst = int_bst;
		int_sfbd = int_fbd;
		int_sfbs = int_fbs;
		int_smin2 = int_min2;
		int_smax2 = int_max2;
		int_smin1 = int_min1;
		int_smax1 = int_max1;
		int_smid1 = int_mid1;
		int_smid2 = int_mid2;
		int_ssumb1 = int_sumb1;
		int_ssumb2 = int_sumb2;
		int_snb1 = int_nb1;
		int_snb2 = int_nb2;
		int_snf = int_nf;
		int_supper = int_upper;
		int_slower = int_lower;
		int_sps1 = int_ps1;
		int_sps2 = int_ps2;
	} else
		fl_fail = 1;
	if (rb == 0) // failes
	{
		if (Fail_cell) // fail inside suspect
		{
			if (dir == 1)
				End_cell = Fail_cell;
			else
				Start_cell = Fail_cell;
		}
		goto retstop;
	}
	if (rb < 0) {
		if (int_nf == 0) // start of suspect
		{
			if (dir == 1) {
				Fail_cell = End_cell;
				if (Fail_Ecell == NULL)
					//  Fail_Ecell = Fail_cell->prevl;
					Fail_Ecell = CSTR_GetPrevRaster(Fail_cell, f_letter
							| CSTR_f_fict);
			} else {
				Fail_cell = Start_cell;
				if (Fail_Scell == NULL)
					//Fail_Scell = Fail_cell->nextl;
					Fail_Scell = CSTR_GetNextRaster(Fail_cell, f_letter
							| CSTR_f_fict);
			}
		}
		int_nf++;
		if (int_nf >= 3) // too long suspect -- back to 1st failure
		{
			if (dir == 1)
				End_cell = Fail_cell;
			else
				Start_cell = Fail_cell;
			retstop: int_bs1 = int_sbs1;
			int_bs2 = int_sbs2;
			int_bst = int_sbst;
			int_fbd = int_sfbd;
			int_fbs = int_sfbs;
			int_min2 = int_smin2;
			int_max2 = int_smax2;
			int_min1 = int_smin1;
			int_max1 = int_smax1;
			int_mid1 = int_smid1;
			int_mid2 = int_smid2;
			int_sumb1 = int_ssumb1;
			int_sumb2 = int_ssumb2;
			int_nb1 = int_snb1;
			int_nb2 = int_snb2;
			int_nf = int_snf;
			int_upper = int_supper;
			int_lower = int_slower;
			int_ps1 = int_sps1;
			int_ps2 = int_sps2;
			fl_fail = 0;
			return 0;
		}
		return 1; // allow failures not more than 2 in sequence
	}
	int_nf = 0;
	Fail_cell = NULL;
	return rb;
}

static void int_basfin(bas_ln *bc) {
	CSTR_rast wc;
	CSTR_rast_attr attr;
	UniVersions vers;
	int16_t row, rowd, sumb1, sumb2, nsum1, nsum2, sumb4, nsum4;
	uchar chr, fl4;
	int i;

	wc = bc->c1;
	sumb1 = sumb2 = sumb4 = nsum1 = nsum2 = nsum4 = 0;

	while (wc) // OLEG
	{
		CSTR_GetAttr(wc, &attr);
		row = attr.row - minrow - attr.bdiff;
		if (abs(row - bc->b1) < 2) {
			sumb1 += row;
			nsum1++;
			goto chk4;
		}
		if (abs(row - bc->b2) < 2) {
			sumb2 += row;
			nsum2++;
			goto chk4;
		}
		chk4: CSTR_GetCollectionUni(wc, &vers);
		fl4 = 0;
		for (i = 0; i < vers.lnAltCnt; i++) {
			chr = vers.Alt[i].Liga;
			if (memchr("ypgjq", chr, 5))
				fl4 = 1;
		}

		if ((((rowd = (row + attr.h)) - 3) > bbs3) && fl4) {
			sumb4 += rowd;
			nsum4++;
		}
		if (wc == bc->c2)
			break;
		wc = CSTR_GetNextRaster(wc, f_letter);
	}
	if (nsum4 > 0) {
		bc->fl_def |= CSTR_db_b4;
		bc->b4 = (2* sumb4 + nsum4) / (2* nsum4 );
		bc->n4 = nsum4;
	} else
		bc->fl_def &= (0xff - CSTR_db_b4);
	if (nsum2 > 0) {
		bc->fl_def |= CSTR_db_b2;
		bc->b2 = (2* sumb2 + nsum2) / (2* nsum2 );
		bc->n2 = nsum2;
	} else {
		bc->fl_def &= (0xff - CSTR_db_b2);
		bc->n2 = 0;
	}
	if (nsum1 > 0) {
		bc->fl_def |= CSTR_db_b1;
		bc->b1 = (2* sumb1 + nsum1) / (2* nsum1 );
		bc->n1 = nsum1;
	} else {
		bc->fl_def &= (0xff - CSTR_db_b1);
		bc->n1 = 0;
	}
}

int16_t comp_Slet(uchar dir, CSTR_rast St) // Small initiator
{
	int16_t bndh, bndup, bnddn, difi, dif1, dift, adifi, adif1, adift;
	uchar roundi, roundc;
	CSTR_rast_attr attr;
	UniVersions vers;

	CSTR_GetAttr(St, &attr);

	roundi = int_fis & CSTR_bs_round;
	roundc = fbs & CSTR_bs_round;
	adift = abs((dift = cur_up - int_bst));
	adif1 = abs((dif1 = cur_up - int_mid1));
	adifi = abs((difi = cur_up - int_mid2));
	bndh = 2;
	bndup = 1;
	bnddn = -1;

	// different roundness -- allow 1 more pixel to differ
	if ((roundc == 0) && (roundi != 0))
		bndup = 2;
	if ((roundi == 0) && (roundc != 0))
		bnddn = -2;

	if ((difi <= bndup) && (difi >= bnddn)) // approx same
	{
		/////   931105 if (fbs & (CSTR_bs_b2a|c_bs_cap))    // new agrees to be small
		if (fbs & CSTR_bs_b2a) // new agrees to be small
		{
			if (int_ps2 && (fbd & CSTR_db_b2) && (fbd & CSTR_db_b3))
			// initiator defines Ps, new is small
			{
				if (abs(int_ps2 - cur_h) > bndh)
					return -1;
			}
			if (cur_up < int_min2) {
				if (int_max2 - cur_up > 3)
					return -1;
				int_min2 = cur_up;
				goto accept_b2;
			}
			if (cur_up > int_max2) {
				if (cur_up - int_min2 > 3)
					return -1;
				int_max2 = cur_up;
			}
			accept_b2: if (((dir == 1) && (Fail_Ecell == NULL)) || ((dir == 2)
					&& (Fail_Scell == NULL))) {
				attr.bas_acc |= CSTR_ba_b2;
				int_sumb2 += cur_up;
				int_nb2++;
				int_mid2 = (int_sumb2 + int_nb2 / 2) / int_nb2;
				CSTR_SetAttr(St, &attr);
			}
			return 5; // agrees to be small
		}
		return -1;
	}

	CSTR_GetCollectionUni(St, &vers);
	if (difi > 0) // smaller small
	{
		if (vers.lnAltCnt > 0 && vers.Alt[0].Liga == 'i')
			return 5;
		return -1; //// -1;   ???  931020
	}
	// new is larger
	if (fbd & CSTR_db_b2) // new is  small (larger  than initiator)
		return -1;
	if ((fbs & (CSTR_bs_b1a | CSTR_bs_bt)) == 0) // new doesn't agree to be b1 or t
		return -1;
	if (fbs & CSTR_bs_b1up) // new can be above b1 - accept "as is"
		return 5;
	if (attr.bas_acc & CSTR_ba_b2) // already aceepted at b2
	{
		if (adifi * 5 < (bbs3 - int_mid2))
			return -1;
		if (adifi < 3)
			return -1;
	}
	if (int_bs1 == -128) // b1 not set yet ?
	{
		if ((fbs & CSTR_bs_bt) == 0) // new doesn't agree to be t
		{ // - new "b1" should not be too low
			if (adifi * 5 < (bbs3 - int_mid2))
				return -1;
			if (adifi < 3)
				return -1;
		}
		int_bs1 = int_sumb1 = int_mid1 = int_min1 = int_max1 = cur_up;
		int_nb1 = 1;
		int_fbd = fbs;
		return 5;
	}
	if (adif1 <= 1) // same as b1/bt  ///// ************ <= 2
	{
		int16_t tmp;
		tmp = int_fbd & fbs;
		if (tmp == 0)
			return -1;
		if (cur_up < int_min1) {
			if (int_max1 - cur_up > 3)
				return -1;
			int_min1 = cur_up;
			goto accept_b1;
		}
		if (cur_up > int_max1) {
			if (cur_up - int_min1 > 3)
				return -1;
			int_max1 = cur_up;
		}
		accept_b1: if (((dir == 1) && (Fail_Ecell == NULL)) || ((dir == 2)
				&& (Fail_Scell == NULL))) {
			int_sumb1 += cur_up;
			int_nb1++;
			int_bs1 = int_mid1 = (int_sumb1 + int_nb1 / 2) / int_nb1;
		}
		int_fbd &= fbs;
		return 5;
	}
	if (dif1 > 1) // new is smaller than b1
	{
		if ((fbs & CSTR_bs_bt) == 0) // doesn't agree to be 't'
			return -1;
		if (int_bst == -128) // 't'-level not defined yet
		{
			int_bst = cur_up;
			int_fbd = fbs; //////// ???????? 931022 c_bs_b1a | CSTR_bs_bt;
			return 5;
		}
		if (adift < 2)
			return 5;
		return -1;
	}
	// new is larger than registered b1
	if (fbs & CSTR_bs_b1up) // new can be above b1 - accept "as is"
		return 5;
	if (int_bst != -128) // both levels busy ?
		return -1;
	if ((int_fbd & CSTR_bs_bt) == 0) // old doesn't agree to be 't'
		return -1;
	// define both fron now on
	int_bst = int_bs1;
	int_bs1 = int_sumb1 = int_mid1 = int_min1 = int_max1 = cur_up;
	int_nb1 = 1;
	int_fbd = CSTR_bs_b1a;
	return 5;
}

int16_t comp_Llet(uchar dir, CSTR_rast St) // Large initiator
{
	int16_t bndh, bndup, bnddn, difi, dif2, dift, adifi, adif2, adift;
	uchar roundi, roundc;

	CSTR_rast_attr attr;
	UniVersions vers;

	CSTR_GetAttr(St, &attr);

	roundi = int_fis & CSTR_bs_round;
	roundc = fbs & CSTR_bs_round;
	adif2 = abs((dif2 = cur_up - int_mid2));
	adift = abs((dift = cur_up - int_bst));
	adifi = abs((difi = cur_up - int_mid1));
	bndh = 2;
	bndup = 1;
	bnddn = -1;
	// different roundness -- allow 1 more pixel to differ
	if ((roundc == 0) && (roundi != 0))
		bndup = 2;
	if ((roundi == 0) && (roundc != 0))
		bnddn = -2;

	if ((difi <= bndup) && (difi >= bnddn)) // approx same
	{
		if (fbs & CSTR_bs_b1a) // agrees to be large
		{
			if (int_ps1) // initiator defines Ps
			{
				if (abs(int_ps1 - cur_h) > bndh)
					return -1;
			}
			if (cur_up < int_min1) {
				if (int_max1 - cur_up > 3)
					return -1;
				int_min1 = cur_up;
				goto accept_b1;
			}
			if (cur_up > int_max1) {
				if (cur_up - int_min1 > 3)
					return -1;
				int_max1 = cur_up;
			}
			accept_b1: if (((dir == 1) && (Fail_Ecell == NULL)) || ((dir == 2)
					&& (Fail_Scell == NULL))) {
				attr.bas_acc |= CSTR_ba_b1;
				int_sumb1 += cur_up;
				int_nb1++;
				int_mid1 = (int_sumb1 + int_nb1 / 2) / int_nb1;
				CSTR_SetAttr(St, &attr);
			}
			return 5; // agrees to be small
		}
		return -1;
	}
	if (difi < 0) // larger large
	{
		if (fbs & CSTR_bs_b1up) // new can be above b1 - accept "as is"
			return 5;
		return -1;
	}
	// new is smaller
	if (fbd & CSTR_db_b1) // new type is 'large' (smaller than initiator)
		return -1;
	if ((fbs & (CSTR_bs_b2a | CSTR_bs_bt)) == 0) // new doesn't agree to be b2 or t
		return -1;
	if (attr.bas_acc & CSTR_ba_b1) // already aceepted at b1
	{
		if (adifi * 6 < (bbs3 - int_mid1))
			return -1;
		if (adifi < 3)
			return -1;
	}
	// where have I place it ??
	if (int_bs2 == -128) // b2 not set yet ?
	{
		if ((fbs & CSTR_bs_bt) == 0) // new doesn't agree to be 't'
		{ // new "b2" should not be too high
			if (adifi * 6 < (bbs3 - int_mid1))
				return -1;
			if (adifi < 3)
				return -1;
		}
		int_bs2 = int_sumb2 = int_mid2 = int_min2 = int_max2 = cur_up;
		int_nb2 = 1;
		int_fbd = fbs;
		return 5;
	}
	if (adif2 <= 1) // same as b2/bt   ////***** <= 2
	{
		int16_t tmp;
		tmp = int_fbd & fbs;
		if (tmp == 0)
			return -1;
		if (cur_up < int_min2) {
			if (int_max2 - cur_up > 3)
				return -1;
			int_min2 = cur_up;
			goto accept_b2;
		}
		if (cur_up > int_max2) {
			if (cur_up - int_min2 > 3)
				return -1;
			int_min2 = cur_up;
		}
		accept_b2: if (((dir == 1) && (Fail_Ecell == NULL)) || ((dir == 2)
				&& (Fail_Scell == NULL))) {
			int_sumb2 += cur_up;
			int_nb2++;
			int_bs2 = int_mid2 = (int_sumb2 + int_nb2 / 2) / int_nb2;
		}
		int_fbd &= fbs;
		return 5;
	}
	if (dif2 < -1) // new is larger than b2
	{
		if ((fbs & CSTR_bs_bt) == 0) // doesn't agree to be 't'
			return -1;
		if (int_bst == -128) // 't'-level not defined yet
		{
			int_bst = cur_up;
			int_fbd = CSTR_bs_b2a; // lower level -- b2-only
			return 5;
		}
		if (adift < 2)
			return 5; // t-level still here
		return -1;
	}
	// new is smaller than registered b2
	CSTR_GetCollectionUni(St, &vers);
	if (vers.lnAltCnt > 0 && vers.Alt[0].Liga == 'i')
		return 5;
	if (int_bst != -128) // both levels busy ?
		return -1;
	if ((int_fbd & CSTR_bs_bt) == 0) // old doesn't agree to be 't'
		return -1;
	// define both fron now on
	int_bst = int_bs2;
	int_bs2 = int_sumb2 = int_mid2 = int_min2 = int_max2 = cur_up;
	int_nb2 = 1;
	int_fbd = CSTR_bs_b2a;
	return 5;
}

int16_t comp_Lbad(CSTR_rast St) {
	CSTR_rast_attr attr;

	CSTR_GetAttr(St, &attr);

	if (abs(cur_up - int_mid1) < 2) {
		attr.bas_acc |= CSTR_ba_b1;
		CSTR_SetAttr(St, &attr);
		return 1; // same height
	}
	if (cur_up < int_mid1)
		return -1; // larger
	if (cur_dn < (bbs3 - 2))
		return 2; // too short (not till b3)
	// smaller - must be not a large letter
	// 931029      if (St->bas1 != -128)     // some base inside
	// ******          return -1;
	if (attr.bas_acc & CSTR_ba_b1)
		return -1;
	return 1;
}

int16_t comp_Sbad(CSTR_rast St) {
	CSTR_rast_attr attr;
	CSTR_GetAttr(St, &attr);

	if (abs(cur_up - int_mid2) < 2) // same height
	{
		if (attr.bas1 == -128)
			goto set_to_b2;
		if (attr.bas1 < (int_mid2 + 2)) {
			set_to_b2: attr.bas_acc |= CSTR_ba_b2;
			CSTR_SetAttr(St, &attr);
			return 1;
		}
		// IN FUTURE: CHECK AGAINST HISTOGRAMM; now lin eto be not too low
		if ((bbs3 - attr.bas1) < 8)
			goto set_to_b2;
		return -1; // base inside - stop
	}
	if (cur_up > int_mid2)
		return -1; // smaller than small -- stop
	if (attr.bas1 == -128) // no base inside ?
	{
		if (attr.bas_acc & CSTR_ba_b2)
			return -1; // used as b2 -- not a "b1"
		return 1;
	}
	if (abs(attr.bas1 - int_mid2) < 2) {
		attr.bas_acc |= CSTR_ba_b1;
		CSTR_SetAttr(St, &attr);
		return 1;
	}
	return -1;
}

void extend_int() {
	if (int_lower < cur_dn)
		int_lower = cur_dn;
	if (int_upper > cur_up)
		int_upper = cur_up;
}

static void insert_int(bas_ln *bp) {
	bas_ln *b1, *b2;
	for (b1 = &all_bases[31], b2 = b1 - 1; b2 >= bp; b1--, b2--)
		memmove(b1, b2, sizeof(bas_ln));
}

static void delete_int(bas_ln *bp) {
	bas_ln *b1, *b2;
	for (b1 = bp, b2 = b1 + 1; b2 < &(all_bases[31]); b1++, b2++) {
		memmove(b1, b2, sizeof(bas_ln));
		if (b1->endcol == 32000)
			break;
	}
}

void prt_interval(bas_ln *bp) {
	char ww[128];
	if (db_status) {
		CSTR_rast wc;
		int16_t cl1, cl2;

		wc = bp->c1;
		if (bp->startcol == -32000) {
			cl1 = cl2 = 0;
			wc = CSTR_GetPrevRaster(cell_l(), f_letter | CSTR_f_fict);
		} else {
			cl1 = bp->startf;
			cl2 = bp->endf;
		}

		sprintf(ww, "int16_t: %d (%d (%d %d) %d)  %d: %d %d %d %d (-%d)",
				bp->ncells, cl1, bp->startcol, bp->endcol, cl2, bp->fl_def,
				bp->b1, bp->b2, bp->b3, bp->b4, minrow);
		glsnap('d', wc, ww);
	}

}

static void set_basint()
// set baselines in last (bp->) interval using compound cell histogram
{
	bas_ln *bp, *bn;
	CSTR_rast bc, wc, wcn, wcm;
	int16_t dones, sticks, i, dif, maxdif, s1, e1, e1c, sp, sn, ep, en;
	int16_t n_again = 0;
	CSTR_rast oldStart_cell = NULL, oldEnd_cell = NULL;
	CSTR_rast_attr attr;
	CSTR_rast_attr attrn;

	bc = Start_cell;
	dones = 0;
	sticks = 0;
	while (1) {
		CSTR_GetAttr(bc, &attr);
		if ((attr.basflg & CSTR_bs_done) == 0) {
			dones++;
			if (cell_sticker(bc))
				sticks++;
		}
		attr.basflg |= CSTR_bs_done;
		CSTR_SetAttr(bc, &attr);
		if (bc == End_cell)
			break;
		bc = CSTR_GetNextRaster(bc, f_letter | CSTR_f_fict);
	}
	n_again = 0; // OLEG : KNOT for clear cycling
	oldStart_cell = oldEnd_cell = NULL;
	again:

	//Paul 12-04-2002
	if (oldStart_cell == Start_cell && oldEnd_cell == End_cell)
		return;
	again2:
	//

	// search proper place for new interval
	if ((spec_camera || line_BL) && n_again && oldStart_cell == Start_cell)
		return;
	if (n_again < 0) // OLEG : KNOT for clear cycling 11 apr 2002
		return;
	oldStart_cell = Start_cell;
	oldEnd_cell = End_cell;
	n_again++;

	CSTR_GetAttr(Start_cell, &attr);
	//s1  = Start_cell->col;
	s1 = attr.col;

	CSTR_GetAttr(End_cell, &attr);
	e1c = attr.col;
	e1 = e1c + attr.w;
	if (all_bases[0].endcol == 32000) // 1st interval to be inserted
	{
		bn = all_bases;
		goto insleft;
	}
	for (i = 0, bp = NULL, bn = all_bases; i < 30; bp = bn, bn++, i++) {
		if (bn->endcol == 32000) // insert at right side ?
			break;
		if ((s1 >= bn->startcol) && (e1 <= bn->endcol)) // new covered
			return;
		if ((s1 <= bn->startcol) && (e1 >= bn->endcol)) // old covered
		{
			delete_int(bn);
			goto again2;
		}
		if (e1 <= bn->endcol)
			break;
	}
	if (bp == NULL)
		goto chkr; // before 1st interval
	if (bn->endcol != 32000) // next exists   AL 940405
	{
		// AL 940405  check columns, not cells
		CSTR_GetAttr(bn->c1, &attr);
		CSTR_GetAttr(CSTR_GetNextRaster(bp->c2, f_letter | CSTR_f_fict), &attrn);
		//if (bn->c1->col <= bp->c2->nextl->col)    // bp, bn overlap --> new covered
		if (attr.col <= attrn.col)
			return;
	}

	if (bn->endcol != 32000) // our interval between bp,bn
	{
		if (dones <= 1)
			return; // single letter --> forget
		if (dones == 2) // not too many "new" cells involved
		{
			if (int_similar(bp, bn)) // our interval may be a dream
			{
				if (sticks) // 2 "new" cells; 1 or 2 sticks;
					return;
			}
		}
	}
	sp = bp->startcol;

	CSTR_GetAttr(bp->c2, &attr);
	ep = attr.col;

	if (s1 <= ep) // overlaps left side
	{
		wc = wcm = CSTR_GetPrevRaster(Start_cell, f_letter | CSTR_f_fict);
		maxdif = 0;
		while (1) {
			wcn = CSTR_GetNextRaster(wc, f_letter | CSTR_f_fict);
			CSTR_GetAttr(wc, &attr);
			CSTR_GetAttr(wcn, &attrn);
			dif = attrn.col - attr.col - attr.w;
			if (dif > maxdif) {
				maxdif = dif;
				wcm = wc;
			}
			if (attrn.col > ep)
				break;
			wc = wcn;
		}
		bp->c2 = wcm;
		CSTR_GetAttr(wcm, &attr);
		bp->endcol = attr.col + attr.w;
		Start_cell = CSTR_GetNextRaster(wcm, f_letter | CSTR_f_fict);
		goto again;
	}
	chkr: sn = bn->startcol;
	en = bn->endcol;
	if (en == 32000) // insert at right side ?
		goto insleft;
	if (e1c >= sn) // overlaps right side;
	{
		//wc = wcm = bn->c1->prevl;
		wc = wcm = CSTR_GetPrevRaster(bn->c1, f_letter | CSTR_f_fict);
		maxdif = 0;
		while (1) {
			//wcn = wc->nextl;
			wcn = CSTR_GetNextRaster(wc, f_letter | CSTR_f_fict);

			CSTR_GetAttr(wc, &attr);
			CSTR_GetAttr(wcn, &attrn);

			//dif = wcn->col - wc->col - wc->w;
			dif = attrn.col - attr.col - attr.w;

			if (dif > maxdif) {
				maxdif = dif;
				wcm = wc;
			}
			if (attrn.col > e1c)
				break;
			wc = wcn;
		}
		//wc = wcm->nextl;
		wc = CSTR_GetNextRaster(wcm, f_letter | CSTR_f_fict);
		End_cell = wcm;
		bn->c1 = wc;
		CSTR_GetAttr(wc, &attr);
		bn->startcol = attr.col;
		goto again;
	}
	insleft: insert_int(bn);
	CSTR_GetAttr(Start_cell, &attr);
	bn->startcol = attr.col;
	CSTR_GetAttr(End_cell, &attr);
	bn->endcol = attr.col + attr.w;
	bn->cf1 = bn->c1 = Start_cell;
	bn->cf2 = bn->c2 = End_cell;
	bn->fl_def = CSTR_db_b3;
	bn->b3 = bbs3;
	bn->n3 = 127;
	bn->n1 = bn->n2 = bn->n4 = 0; //940128
	bn->startf = bn->startcol;
	bn->endf = bn->endcol;
	if (Fail_Ecell) {
		bn->cf2 = Fail_Ecell;
		//bn->endf = bn->cf2->col+bn->cf2->w;
		CSTR_GetAttr(bn->cf2, &attr);
		bn->endf = attr.col + attr.w;
	}
	if (Fail_Scell) {
		bn->cf1 = Fail_Scell;
		//bn->startf = bn->cf1->col;
		CSTR_GetAttr(bn->cf1, &attr);
		bn->startf = attr.col;
	}

	if (int_fid & CSTR_db_b1) // large initiator
	{
		bn->b1 = int_smid1;
		if ((bn->n1 = int_snb1) > 0)
			bn->fl_def |= CSTR_db_b1;
		if (int_sbst != -128) // bt accepted -- int_b2 is actual b2
			goto mkb2;
		if ((int_sfbd & CSTR_bs_bt) || (int_sbs2 == -128)) // b2 doubtful
			bn->b2 = ((bn->b1 << 1) + bbs3) / 3;
		else {
			mkb2: if ((bn->n2 = int_snb2) > 0)
				bn->fl_def |= CSTR_db_b2;
			bn->b2 = int_sbs2;
		}
	}

	else //  small initiator
	{
		bn->b2 = int_smid2;
		if ((bn->n2 = int_snb2) > 0)
			bn->fl_def |= CSTR_db_b2;
		if (int_sbst != -128) // bt accepted -- int_b1 is actual b1
			goto mkb1;
		if ((int_sfbd & CSTR_bs_bt) || (int_sbs1 == -128)) // b1 doubtful
			bn->b1 = (3* bn ->b2 - bbs3) >> 1;
		else {
			mkb1: if ((bn->n1 = int_snb1) > 0)
				bn->fl_def |= CSTR_db_b1;
			bn->b1 = int_sbs1;
		}
	}

	bn->ps = bn->b3 - bn->b2;
	if (bn->ps <= 0) {
		bn->ps = 1;
		bn->b2 = bn->b3 - 1;
	}

	if (int_lower > bn->b3 + 2) {
		bn->fl_def |= CSTR_db_b4;
		bn->b4 = int_lower;
		bn->n4 = 127;
	} else
		bn->b4 = (3* bbs3 - bn->b2) >> 1;
	if (bn->b1 > (bn->b2 - 3))
		bn->b1 = bn->b2 - 2;
	if (bn->b4 < (bn->b3 + 3))
		bn->b4 = bn->b3 + 3;

	int_basfin(bn);
	if (bn->ps == 1)
		bn->b2 = bn->b3 - 1;
	prt_interval(bn);
}

int16_t int_dream(bas_ln *bp, bas_ln *bc, bas_ln *bn) {
	int16_t sim;

	sim = int_similar(bp, bn);
	if (sim == 0)
		return 0;
	if (sim & 1) {
		if ((bc->fl_def & CSTR_db_b2) && ((bc->fl_def & CSTR_db_b1) == 0)
				&& ((abs(bc->b2 - bp->b1) <= 2) || (abs(bc->b2 - bn->b1) <= 2))) {
			if (int_nosmall(bc))
				return 1;
			return 0;
		}
	}
	if (sim & 2) {
		if ((bc->fl_def & CSTR_db_b1) && ((bc->fl_def & CSTR_db_b2) == 0)
				&& ((abs(bc->b1 - bp->b2) <= 2) || (abs(bc->b1 - bn->b2) <= 2))) {
			if (int_sticker(bc))
				return 1;
			if (int_nocap(bc, 0))
				return 1;
			return 0;
		}
		if ((bc->fl_def & CSTR_db_b2) && (((abs(bc->b2 - bp->b1) <= 2)
				&& (bp->fl_def & CSTR_db_b1)) || ((abs(bc->b2 - bn->b1) <= 2)
				&& (bn->fl_def & CSTR_db_b1)))) {
			if (int_nosmall(bc))
				return 1;
			return 0;
		}
	}
	return 0;
}

int16_t int_dream2(bas_ln *bp, bas_ln *bc, bas_ln *bn) {
	int16_t sim;

	sim = int_sim_base2(bp, bn);
	if ((sim & 2) == 0)
		return 0;
	if ((bc->fl_def & CSTR_db_b1) && ((bc->fl_def & CSTR_db_b2) == 0) && ((abs(
			bc->b1 - bp->b2) <= 2) || (abs(bc->b1 - bn->b2) <= 2))) {
		if (int_sticker(bc))
			return 1;
		if (int_nocap(bc, 0))
			return 1;
		return 0;
	}
	if ((bc->fl_def & CSTR_db_b2) && (((abs(bc->b2 - bp->b1) <= 2)
			&& (bp->fl_def & CSTR_db_b1)) || ((abs(bc->b2 - bn->b1) <= 2)
			&& (bn->fl_def & CSTR_db_b1)))) {
		if (int_nosmall(bc))
			return 1;
		return 0;
	}
	return 0;
}

int16_t cell_sticker(CSTR_rast wc) {
	uchar chr;
	UniVersions vers;
	int i;

	CSTR_GetCollectionUni(wc, &vers);

	if (vers.lnAltCnt <= 0)
		return 1; // bad as sticker

	for (i = 0; i < vers.lnAltCnt; i++) {
		chr = vers.Alt[i].Liga;
		if (!memchr("l1rtfI", chr, 6) && chr != liga_i && !(language
				== LANG_TURKISH && // 30.05.2002 E.P.
				(chr == i_sans_accent || chr == II_dot_accent)))
			return 0;
	}
	return 1;
}

int16_t int_sticker(bas_ln *bc) {
	CSTR_rast wc;
	wc = bc->c1;
	while (wc) {
		if (!cell_sticker(wc))
			return 0;
		if (wc == bc->c2)
			break;
		wc = CSTR_GetNextRaster(wc, f_letter | CSTR_f_fict);
	}
	return 1;
}

int16_t int_nocap(bas_ln *bc, int16_t flag) {
	int16_t clet;
	clet = int_caps(bc);
	if (clet == 0)
		return 1;
	if (clet > 2)
		return 0; // too many caps
	if (clet == 1) {
		if (flag == 0)
			return 1; // inside -  not a "single digit"
		return 0; // IN FUTURE: test against "DIGIT ONLY"
	}
	// IN FUTURE: for 2 cells test agains sticks
	return 0;
}

int16_t int_nosmall(bas_ln *bc) {
	int16_t slet;
	slet = int_smalls(bc);
	if (slet <= 1)
		return 1;
	// ELSE: if (slet >= 2)  return 0;   // too many smalls
	return 0;
}

int16_t int_caps(bas_ln *bc) {
	CSTR_rast wc;
	CSTR_rast_attr attr;
	UniVersions vers;
	int i;
	uchar chr;
	int16_t nv, dsmall, dcap, smallrq, smallal, capsal, capsrq, cells;

	smallal = smallrq = capsal = capsrq = cells = 0;
	wc = bc->c1;
	while (1) {
		cells++;
		CSTR_GetAttr(wc, &attr);
		CSTR_GetCollectionUni(wc, &vers);
		//v0=wc->vers;
		nv = (int16_t) vers.lnAltCnt; // wc->nvers;
		dsmall = dcap = 0;
		//if ((v0->let == bad_char) || (nv==0))
		if (vers.lnAltCnt <= 0)
			goto forw; // bad cell
		for (i = 0; i < vers.lnAltCnt; i++) {
			chr = vers.Alt[i].Liga;
			if (let_linshape[chr] & 8) // NO Cap
				dsmall++;
			if (let_linshape[chr] & 1) // NO small
				dcap++;
		}
		if (dsmall == nv)
			smallrq++;
		if (dcap == nv)
			capsrq++;
		if (attr.basflg & CSTR_bs_b1a)
			capsal++;
		if (attr.basflg & CSTR_bs_b2a)
			smallal++;
		forw: if (wc == bc->c2)
			break;
		//wc=wc->nextl;
		wc = CSTR_GetNextRaster(wc, f_letter | CSTR_f_fict);
	}

	if (smallrq > 2)
		return 0;
	if (capsrq < smallrq)
		return 0;
	return capsrq;
}

int16_t int_smalls(bas_ln *bc) {
	CSTR_rast wc;
	CSTR_rast_attr attr;
	UniVersions vers;

	uchar chr;
	int16_t nv, dsmall, dcap, smallrq, smallal, capsal, capsrq, cells;
	int i;

	smallal = smallrq = capsal = capsrq = cells = 0;
	wc = bc->c1;
	while (1) {
		cells++;

		CSTR_GetAttr(wc, &attr);
		CSTR_GetCollectionUni(wc, &vers);
		//v0=wc->vers;
		//nv = wc->nvers;
		nv = (int16_t) vers.lnAltCnt;
		dsmall = dcap = 0;
		//if ((v0->let == bad_char) || (nv==0))
		if (vers.lnAltCnt <= 0)
			goto forw; // bad cell
		for (i = 0; i < vers.lnAltCnt; i++) {
			chr = vers.Alt[i].Liga;
			if (let_linshape[chr] & 8) // NO Cap
				dsmall++;
			if (let_linshape[chr] & 1) // NO small
				dcap++;
		}
		if (dsmall == nv)
			smallrq++;
		if (dcap == nv)
			capsrq++;
		if (attr.basflg & CSTR_bs_b1a)
			capsal++;
		if (attr.basflg & CSTR_bs_b2a)
			smallal++;
		forw: if (wc == bc->c2)
			break;
		//wc=wc->nextl;
		wc = CSTR_GetNextRaster(wc, f_letter | CSTR_f_fict);
	}
	if (capsrq > 2)
		return 0;
	if (capsrq > smallrq)
		return 0;
	return smallrq;
}

int16_t int_sim_base2(bas_ln *b1, bas_ln *b2) {
	if ((b1->fl_def & CSTR_db_b2) == 0)
		return 0;
	if ((b2->fl_def & CSTR_db_b2) == 0)
		return 0;
	if (abs(b2->b2 - b1->b2) <= 1)
		return 2;
	return 0;
}
int16_t int_sim_base1(bas_ln *b1, bas_ln *b2) {
	if ((b1->fl_def & CSTR_db_b1) == 0)
		return 0;
	if ((b2->fl_def & CSTR_db_b1) == 0)
		return 0;
	if (abs(b2->b1 - b1->b1) <= 1) {
		if (b1->n2 < 2)
			return 1;
		if (b2->n2 < 2)
			return 2;
	}
	return 0;
}

int16_t int_similar(bas_ln *b1, bas_ln *b2) {
	// suspicios b1/b2
	int16_t sw;
	if ((b1->fl_def & CSTR_db_b2) && (b1->fl_def & CSTR_db_b1) && (5*
			(b1 ->b2-b1->b1) < b1->ps)
			)
			return 0;
			if (
					(b2->fl_def & CSTR_db_b2) &&
					(b2->fl_def & CSTR_db_b1) &&
					(5*(b2->b2-b2->b1) < b2->ps)
			)
			return 0;
			sw = 0;
			if (b1->fl_def & CSTR_db_b1) sw += 1;
			if (b1->fl_def & CSTR_db_b2) sw += 2;
			if (b2->fl_def & CSTR_db_b1) sw += 4;
			if (b2->fl_def & CSTR_db_b2) sw += 8;
			switch (sw)
			{
				case 0: //        -
				case 1: // b1     -
				case 2: //    b2  -
				case 3: // b1 b2  -
				case 4: //        -  b1
				case 6: //    b2  -  b1
				case 8: //        -      b2
				case 9: // b1     -      b2
				case 12: //        -  b1  b2
				return 0;

				case 5: // b1     -  b1
				case 7: // b1 b2  -  b1
				case 13: // b1     -  b1  b2
				if (abs( b2->b1 - b1->b1) <= 2)
				return 1;
				return 0;
				case 10: //    b2  -      b2
				case 11: // b1 b2  -      b2
				case 14: //    b2  -  b1  b2
				if (abs( b2->b2 - b1->b2) <= 2)
				return 2;
				return 0;
				case 15: // b1 b2  -  b1  b2
				if ((abs( b2->b2 - b1->b2) <= 2) &&
						(abs( b2->b1 - b1->b1) <= 2)
				)
				return 3;
				return 0;
			}
			return 0;
		}

int16_t int_covered(bas_ln *bc, bas_ln *bm, int16_t ends, int16_t flag) {
	int16_t sw;
	if (bc->ncells > bm->ncells)
		return 0;
	sw = 0;
	if (bc->fl_def & CSTR_db_b1)
		sw += 1;
	if (bc->fl_def & CSTR_db_b2)
		sw += 2;
	if (bm->fl_def & CSTR_db_b1)
		sw += 4;
	if (bm->fl_def & CSTR_db_b2)
		sw += 8;
	switch (sw) {
	//          WHOM (bc)   WHO (bm)
	case 0: //        -
	case 1: // b1     -
	case 2: //    b2  -
	case 3: // b1 b2  -
		return 0;

	case 4: //        -  b1
	case 8: //        -      b2
	case 12: //        -  b1  b2
		return 1;

	case 6: //    b2  -  b1
		if (abs(bc->b2 - bm->b1) > 2)
			return 0;
		if (flag || int_nosmall(bc))
			return 1;
		return 0;

	case 9: // b1     -      b2
		if (abs(bc->b1 - bm->b2) > 2)
			return 0;
		if (flag || int_nocap(bc, ends))
			return 1;
		return 0;

	case 5: // b1     -  b1
		if (abs(bc->b1 - bm->b1) <= 2)
			return 1;
		return 0;
	case 7: // b1 b2  -  b1
		if (abs(bc->b1 - bm->b1) <= 2)
			return 1;
		return 0;
	case 13: // b1     -  b1  b2
		if (abs(bc->b1 - bm->b1) <= 2)
			return 1;
		if (abs(bc->b1 - bm->b2) > 2)
			return 0;
		if (flag || int_nocap(bc, ends))
			return 1;
		return 0;
	case 10: //    b2  -      b2
	case 11: // b1 b2  -      b2
		if (abs(bc->b2 - bm->b2) > 2)
			return 0;
		return 1;
	case 14: //    b2  -  b1  b2
		if (abs(bc->b2 - bm->b2) <= 2)
			return 1;
		if (abs(bc->b2 - bm->b1) > 2)
			return 0;
		if (flag || int_nosmall(bc))
			return 1;
		return 0;
	case 15: // b1 b2  -  b1  b2
		if ((abs(bm->b2 - bc->b2) <= 2) && (abs(bm->b1 - bc->b1) <= 2))
			return 1;
		return 0;
	}
	return 0;
}

void merge_b4(bas_ln *b1, bas_ln *b2) //940128 see all calls too !
{
	if ((b1->fl_def & CSTR_db_b4) == 0)
		if (b2->fl_def & CSTR_db_b4) {
			b1->b4 = b2->b4;
			if ((b1->n4 = b2->n4) > 0)
				b1->fl_def |= CSTR_db_b4;
		}
}

void merge_bas1(bas_ln *b1, bas_ln *b2) {
	int sw;
	sw = 0;
	if (b1->fl_def & CSTR_db_b1)
		sw += 1;
	if (b2->fl_def & CSTR_db_b1)
		sw += 2;
	if ((b1->fl_def & CSTR_db_b4) == 0) {
		if (b2->fl_def & CSTR_db_b4) {
			b1->b4 = b2->b4;
			b1->n4 = b2->n4;
		}
	}
	switch (sw) {
	case 0:
	case 1:
		break;
	case 2:
		b1->b1 = b2->b2;
		b1->n2 = b2->n2;
		break;
	case 3:
		b1->b1 = (b1->b1 * b1->n1 + b2->b1 * b2->n1) / (b1->n1 + b2->n1);
		b1->n1 += b2->n1;
	}
	b1->fl_def = 0;
	if (b1->n1 > 0)
		b1->fl_def |= CSTR_db_b1;
	if (b1->n2 > 0)
		b1->fl_def |= CSTR_db_b2;
	if (b1->n3 > 0)
		b1->fl_def |= CSTR_db_b3;
	if (b1->n4 > 0)
		b1->fl_def |= CSTR_db_b4;
}

void merge_bases(bas_ln *b1, bas_ln *b2) {
	if ((b1->fl_def & CSTR_db_b1) == 0) {
		if (b2->fl_def & CSTR_db_b1)
			b1->b1 = b2->b1;
	}
	if ((b1->fl_def & CSTR_db_b2) == 0) {
		if (b2->fl_def & CSTR_db_b2)
			b1->b2 = b2->b2;
	}
	if ((b1->fl_def & CSTR_db_b4) == 0) {
		if (b2->fl_def & CSTR_db_b4) {
			b1->b4 = b2->b4;
			b1->n4 = b2->n4;
		}
	}
	b1->fl_def = 0;
	if (b1->n1 > 0)
		b1->fl_def |= CSTR_db_b1;
	if (b1->n2 > 0)
		b1->fl_def |= CSTR_db_b2;
	if (b1->n3 > 0)
		b1->fl_def |= CSTR_db_b3;
	if (b1->n4 > 0)
		b1->fl_def |= CSTR_db_b4;
}

static void make_intpairs() {
	bas_ln *bc, *bp, *bn, *bmax;
	CSTR_rast sw, s1, sn;
	CSTR_rast_attr attr, attr2;
	int16_t maxi, maxn, Scol, Ecol, i, simfl, pairfl, closfl;

	//s1 = cell_f()->nextl;
	s1 = CSTR_GetNextRaster(cell_f(), f_letter | CSTR_f_fict);
	//sn = cell_l()->prevl;
	sn = CSTR_GetPrevRaster(cell_l(), f_letter | CSTR_f_fict);
	//Scol = cell_f()->next->col;
	CSTR_GetAttr(CSTR_GetNext(cell_f()), &attr);
	Scol = attr.col;
	//Ecol = cell_l()->prev->col + cell_l()->prev->w;
	CSTR_GetAttr(CSTR_GetPrev(cell_l()), &attr);
	Ecol = attr.col + attr.w;

	for (i = 0, bc = all_bases; i < 30; bc++, i++)
		bc->fl_ext = 0;
	again: for (i = 0, bc = all_bases, bs_int_no = 0, maxn = 0; i < 30; bc++, i++) {
		if (bc->endcol == 32000)
			break;
		bc->ncells = 0;
		sw = s1;
		CSTR_GetAttr(sw, &attr);
		while (attr.col < bc->startcol) {
			//sw=sw->nextl;
			sw = CSTR_GetNextRaster(sw, f_letter | CSTR_f_fict);
			CSTR_GetAttr(sw, &attr);
		}

		if (attr.flg & CSTR_f_fict) // AL 940405
			break;

		bs_int_no++;
		bc->c1 = sw;
		if ((bc->fl_ext & 2) == 0) {
			bc -> startcol = attr.col; // sw->col;
			if (bc->startf > bc->startcol)
				bc->startf = bc->startcol;
		}
		//AK! Crashed line
		//while (sw->nextl)
		while (CSTR_GetNextRaster(sw, f_letter | CSTR_f_fict)) {
			bc->ncells++;
			//sw=sw->nextl;
			sw = CSTR_GetNextRaster(sw, f_letter | CSTR_f_fict);
			CSTR_GetAttr(sw, &attr);
			if (attr.col >= bc->endcol)
				break;
		}

		//bc->c2 = sw->prevl;
		bc->c2 = CSTR_GetPrevRaster(sw, f_letter | CSTR_f_fict);
		if ((bc->fl_ext & 1) == 0) {
			CSTR_GetAttr(bc->c2, &attr);
			bc->endcol = attr.col + attr.w;
			if (bc->endf < bc->endcol)
				bc->endf = bc->endcol;
		}
		if (bc->ncells > maxn) {
			bmax = bc;
			maxn = bc->ncells;
			maxi = i;
		}
	}
	if (bs_int_no <= 1)
		goto all_done;

	// try to eat everybody by max
	bc = bmax;
	again_m0: if (bs_int_no <= 1)
		goto all_done;
	if (maxi > 0) {
		int16_t intsim;
		bp = bc - 1;
		if (((intsim = int_similar(bc, bp)) != 0) || // not to eat close intrvl "blindly" but when b2==b1 only
				(((bc->startcol - bp->endcol) < 4) && int_covered(bp, bc, 0, 1))) {
			bc->startcol = bp->startcol;
			bc->c1 = bp->c1;
			bc->ncells += bp->ncells;
			bc->startf = MIN(bc->startf, bp->startf);
			if (intsim)
				merge_bases(bc, bp);
			else
				merge_b4(bc, bp);
			delete_int(bp);
			maxi--;
			bmax--;
			bc--;
			bs_int_no--;
			goto again_m0;
		}
	}
	again_mm: if (bs_int_no <= 1)
		goto all_done;
	if (maxi < (bs_int_no - 1)) {
		int16_t intsim;
		bn = bc + 1;
		if (((intsim = int_similar(bc, bn)) != 0) || // not to eat close intvl blindly
				(((bn->startcol - bc->endcol) < 4) && int_covered(bn, bc, 0, 1))) {
			bc->endcol = bn->endcol;
			bc->c2 = bn->c2;
			bc->ncells += bn->ncells;
			bc->endf = MAX(bc->endf, bn->endf);
			bs_int_no--;
			if (intsim)
				merge_bases(bc, bn);
			else
				merge_b4(bc, bn);
			delete_int(bn);
			goto again_mm;
		}
	}

	again_tripples: for (i = 0, bc = &all_bases[1], bn = bc + 1, bp = bc - 1; i
			< bs_int_no - 2; bp++, bc++, bn++, i++) {
		if (int_dream(bp, bc, bn)) // similar bn, bp; bc fits
		{
			bp->endcol = bn->endcol;
			bp->ncells += bn->ncells;
			bp->ncells += bc->ncells;
			bp->endf = MAX(bp->endf, bn->endf);
			bp->endf = MAX(bp->endf, bc->endf);
			bp->startf = MIN(bp->startf, bn->startf);
			bp->startf = MIN(bp->startf, bc->startf);
			bs_int_no -= 2;
			bp->c2 = bn->c2; // AL 940318
			merge_bases(bp, bn);
			delete_int(bc);
			delete_int(bc); // 'bn' came to 'bc' position
			goto again_tripples;
		}
		if (int_dream2(bp, bc, bn)) // similar bn, bp by BS2; bc fits
		{
			bp->endcol = bn->endcol;
			bp->ncells += bn->ncells;
			bp->ncells += bc->ncells;
			bp->endf = MAX(bp->endf, bn->endf);
			bp->endf = MAX(bp->endf, bc->endf);
			bp->startf = MIN(bp->startf, bn->startf);
			bp->startf = MIN(bp->startf, bc->startf);
			bp->c2 = bn->c2; // AL 940318
			bs_int_no -= 2;
			merge_bas1(bp, bn);
			delete_int(bc);
			delete_int(bc); // 'bn' came to 'bc' position
			goto again_tripples;
		}
		if (int_covered(bc, bp, 0, 0)) {
			bp->endcol = bc->endcol;
			bp->ncells += bc->ncells;
			bp->endf = MAX(bp->endf, bc->endf);
			bp->c2 = bc->c2; // AL 940318
			bs_int_no--;
			merge_b4(bp, bc);
			delete_int(bc); // 'bn' comes to 'bc' position
			goto again_tripples;
		}
		if (int_covered(bc, bn, 0, 0)) {
			bn->startcol = bc->startcol;
			bn->ncells += bc->ncells;
			bp->startf = MIN(bp->startf, bc->startf);
			bn->c1 = bc->c1; // AL 940318
			bs_int_no--;
			merge_b4(bn, bc);
			delete_int(bc); // 'bn' comes to 'bc' position
			goto again_tripples;
		}
	}

	again_sims: pairfl = 0;
	if (bs_int_no <= 1)
		goto all_done;
	bc = all_bases;
	bn = bc + 1;
	if (int_similar(bc, bn)) {
		bc->c2 = bn->c2;
		bc->endcol = bn->endcol;
		bc->ncells += bn->ncells;
		bc->endf = MAX(bc->endf, bn->endf);
		merge_bases(bc, bn);
		delete_int(bn);
		bs_int_no--;
		goto again_sims;
	}

	closfl = ((bn->startcol - bc->endcol) < 4) ? 1 : 0;
	if (int_covered(bc, bn, 1, closfl)) {
		bn->startcol = bc->startcol;
		bn->ncells += bc->ncells;
		bn->c1 = bc->c1;
		bn->startf = MIN(bc->startf, bn->startf);
		merge_b4(bn, bc);
		delete_int(bc);
		bs_int_no--;
		goto again_sims;
	}

	CSTR_GetAttr(s1, &attr);
	if (attr.col < bc->startcol) // dead part exists at start of line
	{
		bc->startcol = attr.col;
		bc->fl_ext |= 2; // AL 940405
		bc->c1 = s1; // same, later
	}

	again_sime: if (bs_int_no <= 1)
		goto all_done;
	bc = &all_bases[bs_int_no - 1];
	bp = bc - 1;
	if (int_similar(bc, bp)) {
		bc->ncells += bp->ncells;
		bc->startcol = bp->startcol;
		bc->c1 = bp->c1;
		bc->startf = MIN(bc->startf, bp->startf);
		merge_bases(bc, bp);
		delete_int(bp);
		bs_int_no--;
		goto again_sime;
	}
	closfl = ((bc->startcol - bp->endcol) < 4) ? 1 : 0;
	if (int_covered(bc, bp, 2, closfl)) {
		bp->endcol = bc->endcol;
		bp->ncells += bc->ncells;
		bp->c2 = bc->c2;
		bp->endf = MAX(bc->endf, bp->endf);
		merge_b4(bp, bc);
		delete_int(bc);
		bs_int_no--;
		goto again_sime;
	}

	CSTR_GetAttr(sn, &attr);
	if (attr.col + attr.w > bc->endcol) // dead part exists at end of line
	{
		bc->endcol = attr.col + attr.w;
		bc->fl_ext |= 1; // AL 940405
		bc->c2 = sn; // same, later
	}
	again_dead: if (bs_int_no <= 1)
		goto all_done;
	for (i = 0, bc = all_bases, bn = bc + 1; i < bs_int_no - 1; bc++, bn++, i++) {
		// AL 940405  check columns, not cells
		// old if was:  if (bn->c1 != bc->c2->nextl)  // dead zone exists
		CSTR_GetAttr(bn->c1, &attr);
		CSTR_GetAttr(CSTR_GetNextRaster(bc->c2, f_letter | CSTR_f_fict), &attr2);
		//if (bn->c1->col > bc->c2->nextl->col)
		if (attr.col > attr2.col)
		////  try to analyze dead zone's boxes
		{
			if (int_similar(bc, bn)) {
				bc->endcol = bn->endcol;
				bc->endf = MAX(bc->endf, bn->endf);
				bc->c2 = bn->c2;
				bc->ncells += bn->ncells;
				bs_int_no--;
				merge_bases(bc, bn);
				delete_int(bn);
				goto again_dead;
			}
			if (bc->endf > bc->endcol) // current extendable to right
			{
				bc->endf = bc->endcol = MIN(bc->endf, bn->startcol);
				bc ->fl_ext |= 1;
				goto again;
			}
			if (bn->startf < bn->startcol) // next extendable to left
			{
				bn->startf = bn->startcol = MAX(bn->startf, bc->endcol);
				bn ->fl_ext |= 2;
				goto again;
			}
			bc->endf = bc->endcol = bn->startcol;
			goto again;
		}
	}

	goto pairs_loop;
	again_pairs: pairfl = 1;
	pairs_loop: if (bs_int_no <= 1)
		goto all_done;
	for (i = 0, bc = all_bases, bn = bc + 1; i < bs_int_no - 1; bc++, bn++, i++) {
		if (int_similar(bc, bn)) {
			bc->endcol = bn->endcol;
			bc->endf = MAX(bc->endf, bn->endf);
			bc->c2 = bn->c2;
			bc->ncells += bn->ncells;
			merge_bases(bc, bn);
			delete_int(bn);
			bs_int_no--;
			goto again_pairs;
		}
		if (int_sim_base2(bc, bn)) {
			bc->endcol = bn->endcol;
			bc->endf = MAX(bc->endf, bn->endf);
			bc->c2 = bn->c2;
			bc->ncells += bn->ncells;
			merge_bas1(bc, bn);
			delete_int(bn);
			bs_int_no--;
			goto again_pairs;
		}
		if ((simfl = int_sim_base1(bc, bn)) != 0) {
			bc->endcol = bn->endcol;
			bc->endf = MAX(bc->endf, bn->endf);
			bc->c2 = bn->c2;
			bc->ncells += bn->ncells;
			if ((simfl == 1) && (bn->n2 >= 2)) {
				bc->b2 = bn->b2;
				if ((bc->n2 = bn->n2) > 0)
					bc->fl_def |= CSTR_db_b2;
			}
			merge_b4(bc, bn);
			delete_int(bn);
			bs_int_no--;
			goto again_pairs;
		}
	}
	if (pairfl)
		goto again_sims;
	if (bs_int_no <= 1)
		goto all_done;
	bc = all_bases;
	bn = bc + 1;
	if ((bc->ncells == 1) && (bn->ncells > 2)) {
		if (int_smalls(bc) // small single - take it
				|| (bn->startcol - bc->endcol < 4) // cap - if close
		) {
			bn->startcol = bc->startcol;
			bn->ncells += 1;
			bn->c1 = bc->c1;
			bn->startf = MIN(bc->startf, bn->startf);
			merge_b4(bn, bc);
			delete_int(bc);
			bs_int_no--;
		}
	}
	if (bs_int_no <= 1)
		goto all_done;
	bc = &all_bases[bs_int_no - 1];
	bp = bc - 1;
	if ((bc->ncells == 1) && (bp->ncells > 2)) {
		if (int_smalls(bc) // small single - take it
				|| (bc->startcol - bp->endcol < 4) // cap - if close
		) {
			bp->endcol = bc->endcol;
			bp->ncells += bc->ncells;
			bp->c2 = bc->c2;
			bp->endf = MAX(bc->endf, bp->endf);
			merge_b4(bp, bc);
			delete_int(bc);
			bs_int_no--;
		}
	}
	again_trisingl: for (i = 0, bc = &all_bases[1], bn = bc + 1, bp = bc - 1; i
			< bs_int_no - 2; bp++, bc++, bn++, i++) {
		if (bc->ncells == 1) {
			if ((bn->startcol - bc->endcol) > (bc->startcol - bp->endcol)) {
				bp->endcol = bc->endcol;
				bp->ncells += bc->ncells;
				bp->c2 = bc->c2;
				bp->endf = MAX(bc->endf, bp->endf);
				merge_b4(bp, bc);
				delete_int(bc);
				bs_int_no--;
				goto again_trisingl;
			} else {
				bn->startcol = bc->startcol;
				bn->ncells += 1;
				bn->c1 = bc->c1;
				bn->startf = MIN(bc->startf, bn->startf);
				merge_b4(bn, bc);
				delete_int(bc);
				bs_int_no--;
			}
		}
	}

	all_done: if (bs_int_no == 0)
		bs_int_no = 1;
	else
		all_bases[0].startcol = Scol;
	for (i = 0, bc = all_bases, bs_int_no = 0; i < 30; bc++, i++) {
		prt_interval(bc);
		if (bc->endcol == 32000)
			break;
		bs_int_no++;
	}
	all_bases[bs_int_no].endcol = Ecol;
}

