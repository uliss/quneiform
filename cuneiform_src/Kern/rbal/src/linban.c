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

#define  MULTI
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cstr.h"
#define  LIN_MAIN
#include "linear.h"
#include "ligas.h"

#include "linutil.h"
#include "tuner.h"

#include "wrgb.h" //IGOR
#include "statsearchbl.h" //IGOR
#include "minmax.h"

//IGOR
#define draw_puma_bl(n,l)	rs.y = re.y = minrow + bbs##n;	\
							snap_draw_line_rbal(NULL, &rs, &re, (int16_t)nIncline/2, rgb, -100, 1##l)
#define draw_stat_bl(n,l)	rs.y = re.y = minrow + m_attr.bs##n;\
							snap_draw_line_rbal(NULL, &rs, &re, (int16_t)nIncline/2, rgb, -100, 2##l)
#define draw_bl_marking(n,l)	rsm.y = rs.y - 1; rem.y = rs.y + 1;	\
								for(mc = 0; mc < n; mc++)	\
								{	\
									rsm.x = rs.x - 1 + mc*5; rem.x = rs.x + 1 + mc*5;	\
									snap_draw_line_rbal(NULL, &rsm, &rem, (int16_t)nIncline/2, rgb, -100, 1##l);\
								}
#define del_bl(m,l)			snap_del_line_rbal(NULL,  m##l);
//IGOR

#ifdef UFA
#undef MULTI
#endif

//IGOR
extern uchar language;
extern CSTR_line lin_str;
extern int32_t nIncline;
//IGOR

int16_t obtain_diff(int16_t arg);

void set_basarr(bas_ln * bs, int16_t x, int16_t l) {
	// Nick 13.03.2001
	memset(bs, 0, sizeof(bas_ln));

	bs->startcol = x;
	bs->endcol = l;
	// bs->mindef = minc;
	// bs->maxdef = maxc;
	bs->ps = Ps;
	bs->b1 = bbs1;
	bs->b2 = bbs2;
	bs->b3 = bbs3;
	bs->b4 = bbs4;
	Ps = bs->ps = bbs3 - bbs2; //940120
	if (Ps <= 0) {
		Ps = 1;
		bbs2 = bbs3 - 1;
	}
	bs->fl_def = 0;
	if (Nb1 > 0)
		bs->fl_def |= CSTR_db_b1;
	if (Nb2 > 0)
		bs->fl_def |= CSTR_db_b2;
	if (Nb3 > 0)
		bs->fl_def |= CSTR_db_b3;
	if (Nb4 > 0)
		bs->fl_def |= CSTR_db_b4;
	if (all_caps)
		bs->fl_def |= c_fdef_cap;

	// Nick 13.03.2001 - accord to def_locbas
	bs->n1 = Nb1;
	bs->n2 = Nb2;
	bs->n3 = Nb3;
	bs->n4 = Nb4;
}

int16_t def_locbas(CSTR_rast cl) {
	bas_ln *bc, *bn, *bsp;
	int16_t i, x, y, dc, dn;
	CSTR_rast_attr attr;
	uchar fl_defb;

	if ((multi_bas & 128) == 0)
		goto retps; // multiple bases not set
	if (cl == NULL) {
		bc = all_bases;
		goto make;
	}

	CSTR_GetAttr(cl, &attr);
	x = attr.col;
	y = x + attr.w / 2;

	for (i = 0, bc = all_bases, bn = bc + 1; i < 32; i++, bc++, bn++) {
		if (y <= bc->endcol)
			break;
		if (bn->endcol == 32000)
			break;
		if (x >= bn->startcol)
			continue;
		if ((dc = x - bc->endcol) <= 0)
			break;
		if ((dn = bn->startcol - y) <= 0) {
			bc = bn;
			break;
		}
		if (dc <= dn)
			break;
		bc = bn;
		break;
	}
	make: bsp = bc;
	Ps = bsp->ps;
	bbs1 = bsp->b1;
	bbs2 = bsp->b2;
	bbs3 = bsp->b3;
	bbs4 = bsp->b4;
	Ps = bbs3 - bbs2; //?????????????? 940119 ????
	if (Ps <= 0) {
		Ps = 1;
		bbs2 = bbs3 - 1;
	}
	bbsm = (bbs2 + bbs3) / 2;
	fl_defb = bsp->fl_def;
	Nb1 = (fl_defb & CSTR_db_b1) ? bsp->n1 : -1;
	Nb2 = (fl_defb & CSTR_db_b2) ? bsp->n2 : -1;
	Nb3 = (fl_defb & CSTR_db_b3) ? bsp->n3 : -1;
	Nb4 = (fl_defb & CSTR_db_b4) ? bsp->n4 : -1;
	if (Nb1 == 0)
		Nb1 = -1;
	if (Nb2 == 0)
		Nb2 = -1;
	if (Nb3 == 0)
		Nb3 = -1;
	if (Nb4 == 0)
		Nb4 = -1;
	all_caps = fl_defb & c_fdef_cap;
	retps: Psf = Ps;
	if (fax1x2)
		Psf += 3; // AL 940329 was +2 TWICE ??!!
	return Ps;
}

void get_b_lines(CSTR_rast C, B_LINES *bas) {
	int16_t dif;
	CSTR_rast_attr attr;

	if (C) {
		CSTR_GetAttr(C, &attr);
		def_locbas(C);
		if (all_diffs_made) // are all diffs already defined ?
		{
			dif = attr.bdiff;
			if (attr.difflg & (CSTR_db_down + CSTR_db_up)) {
				//       if (attr.difflg & CSTR_db_up)   dif --;     // restore proper
				//       else          dif ++;     // base diff without corrections
				// Nick - 16.03.2001 - accord to  set CSTR_db_up,down
				if (attr.difflg & CSTR_db_up)
					dif++; // restore proper
				else
					dif--; // base diff without corrections
			}
		} else
			// diff not defined yet
			dif = obtain_diff(attr.col);
		dif += minrow;
	} else {
		def_locbas(NULL);
		dif = minrow;
	}

	if (Ps <= 0) {
		Ps = 1;
		bbs2 = bbs3 - 1;
	}
	if (bbs2 >= bbs3)
		bbs2 = bbs3 - 1;
	if (bbs1 >= bbs2)
		bbs1 = bbs2 - 1;
	if (bbs4 <= bbs3)
		bbs4 = bbs3 + 1;
	bas->ps = bbs3 - bbs2; //////////?????????? 940119 ???
	bas->b0 = bbs0 + minrow;
	bas->b1 = bbs1 + dif;
	bas->b2 = bbs2 + dif;
	bas->b3 = bbs3 + dif;
	bas->b4 = bbs4 + dif;
	bas->bm = bbsm + dif;
	bas->n1 = Nb1;
	bas->n2 = Nb2;
	bas->n3 = Nb3;
	bas->n4 = Nb4;
}
///////////////
// arg == column
int16_t obtain_diff(int16_t arg) {
	int16_t wl, ce, w1m, da, d1n;
	CSTR_rast C;
	CSTR_rast_attr attr;

	w1m = d1n = 127;
	wl = 0;
	C = cell_f();
	C = CSTR_GetNextRaster(C, f_letter);
	//while ((C=C->nextl)->nextl)
	for (; C; C = CSTR_GetNextRaster(C, f_letter)) {
		CSTR_GetAttr(C, &attr);
		if (!(attr.difflg & 4)) // base was not defined here ?
			continue;
		wl = attr.bdiff;
		ce = attr.col;
		da = arg - ce;
		if (da == 0)
			return wl;
		if (da < 0) {
			if (w1m == 127)
				return wl;
			if ((-da) < d1n)
				return wl;
			else
				return w1m;
		}
		d1n = da;
		w1m = wl;
	}
	return wl;
}
////////////

void complete_bas(char *txt) {
	CSTR_rast B1;
	CSTR_rast_attr attr;

	//glsnap('d',cell_f()->nextl,"to complete ");

	bbs0 = min_crow;
	Ps = Psf = bbs3 - bbs2;
	if (Ps <= 0) {
		Ps = 1;
		bbs2 = bbs3 - 1;
	}
	if (fax1x2)
		Psf = Ps + 3; // AL 940329  was '+2' TWO TIMES

	if (!snap_baselines_rbal('a'))//IGOR
	{//IGOR
		Nb4 = Ns4;
		if (Ns4 > 0)
			bbs4 = (sbs4 + (Ns4 >> 1)) / Ns4;
		else {
			bbs4 = (3* bbs3 - bbs2) >> 1;
			Nb4 = -1;
		}
	}//IGOR
	bbsm = (bbs2 + bbs3) >> 1;
	if (bbs4 < (bbs3 + 3)) {
		bbs4 = (3* bbs3 - bbs2) >> 1;
		Nb4 = -1;
	}
	bs_got = 1;

	diffs_by_cells();
	B1 = cell_f();
	//while ((B1=B1->nextl)->nextl)
	for (B1 = CSTR_GetNextRaster(B1, f_letter); B1; B1 = CSTR_GetNextRaster(B1,
			f_letter)) {
		CSTR_GetAttr(B1, &attr);
		attr.difflg &= ~64;
		CSTR_SetAttr(B1, &attr);
	}

	if (db_status) {
		char ww[128];
		sprintf(ww, "%s, mr=%d min=%d Ps=%d Bs: %d %d %d %d, Ns: %d %d %d %d",
				txt, sum_ans, minrow, Ps, bbs1, bbs2, bbs3, bbs4, Ns1, Ns2,
				Ns3, Ns4);
		glsnap('d', cell_f()->next, ww);
	}
}

int16_t approve_by_hist() {
	int16_t s;
	uchar *p1, *p2, *p3;

	if (Ns1 < 3)
		return 0;
	if (Ns1 > 5)
		goto tst2;
	p1 = &(lin_begs[bbs1 + 31]);
	p2 = p1 + 1;
	p3 = p2 + 1;
	s = (*p1) + (*p2) + (*p3);
	if (s < sbs1)
		return 0;
	tst2: if (Ns2 < 3)
		return 0;
	if (Ns2 > 5)
		return 1;
	p1 = &(lin_begs[bbs2 + 31]);
	p2 = p1 + 1;
	p3 = p2 + 1;
	s = (*p1) + (*p2) + (*p3);
	if (s < sbs2)
		return 0;
	return 1;
}

int16_t mult_b1(uchar k1, uchar k2, uchar k3, uchar k4) {
	if (ncut_old * 4 >= ntot_old)
		return 0; // many dead cells
	if (k2 * sum_letlth < k1 * sum_cellth)
		return 0; // large common lth of deads
	if (nab1 != ncletrs)
		return 0; // all must agree to be bs1
	if (k4 * Ns2 > k3 * ntot_old)
		return 0; // too many vote for b2
	bbs1 = sbsu / ncbs;
	Nb1 = ntot_old - ncut_old;
	Nb2 = -1;
	bbs2 = (bbs3 + 2* bbs1 ) / 3;
	all_caps = 1;
	complete_bas("mult b1 made");
	set_basarr(&all_bases[0], -32000, 32000);
	return 1;
}

int16_t all_b1(uchar k1, uchar k2, uchar k3, uchar k4) {
	if (ncut_old * 4 >= ntot_old)
		return 0; // many dead cells
	if (k2 * sum_letlth < k1 * sum_cellth)
		return 0; // large common lth of deads
	if (nab1 != ncletrs)
		return 0; // all must agree to be bs1
	if (k4 * Ns2 > k3 * ntot_old)
		return 0; // too many vote for b2
	doubt_bas();
	bbs1 = sbsu / ncbs;
	Nb1 = ntot_old - ncut_old;
	Nb2 = -1;
	bbs2 = (bbs3 + 2* bbs1 ) / 3;
	all_caps = 1;
	complete_bas("all b1 made");
	set_basarr(&all_bases[0], -32000, 32000);
	return 1;
}
int16_t b1b2_byhist() {
	int16_t fl12, dl, i, iw, ett, ets, ibeg, ilim, m, ms1, ms2, m1, m2, lb1, lb2,
			b2x;
	uchar old_ok, *bp, *bc, *bn;
	CSTR_rast cb2;
	CSTR_rast_attr attr;

	m1 = m2 = 0;
	///////// 931230 old_ok = ((Ns1+Ns2 <= 5)  && (oldPs2 > 0) && ((oldmult & 0x80) == 0))? 1:0;
	old_ok = 0;
	// old_ok --> may use previous line's bases
	// on pass#1 (draft) Ps should be 2/3 (Ps1) for proper dust receive
	lb1 = lb2 = -1;
	ett = ets = 0; // no 't' level
	fl12 = 0;
	if (Nst) {
		int16_t i;
		// uchar *bpp, *bnn;
		ets = ett = sbst / Nst + 32;
		if ((ett < 0) || (ett > 181)) // 940415
			return 0;
		bc = &lin_begs[ett];
		bp = bc - 1;
		bn = bc + 1;
		// bpp = (bp=bc-1)-1; bnn=(bn=bc+1)+1;
		// iw = (i = (*bp) + (*bc) + (*bn)) + (*bpp) + (*bnn);
		iw = (i = (*bp) + (*bc) + (*bn));
		// if ((2*Nst < i) || (3*Nst < iw)) ets = 0;
		// ett was misplaced (to histogramm) because of diff
		if (2* Nst < i)
			ets = 0;
	}
	if (ett > (bbs3 - 5))
		ett = ets = 0;
	for (i = 1, bp = lin_begs, bc = bp + 1, bn = bc + 1; i < (bbs3 + 32 - 5); i++, bp++, bc++, bn++) {
		if ((*bc < *bp) || (*bc < *bn))
			continue;
		if (ets && ((i > (ets - 2)) && (i < (ets + 2))))
			continue;
		m = *bp + 2* (* bc) + *bn;
		if (m2 < m)
		{	m2=m; ms2=m-*bc; lb2=i;}
	}
	//if( m2 < 6 ) lb2=-1; // weak peak. Valdemar 09-21-94 07:15pm
				if (lb2 < 0) //  NO MAX obtained;
				return 0;
				ibeg = 1; ilim = bbs3+32-5; // search 2nd max in full area
				if (Ns2 > 0) // bs2  obtained
				{
					int16_t it;
					it = (sbs2+(Ns2>>1))/Ns2 + 32;
					if (lb2 > it-3) // lb2 --> bs2
					{
						fl12 = 2; // indicate main max in b2
						ilim = lb2-2;
						goto fmax2;
					}
				}
				if (Ns1 > 0)
				{
					int16_t it;
					it = (sbs1+(Ns1>>1))/Ns1 + 32;
					if (lb2 < it+3) // lb2 --> bs1
					{
						fl12 = 1; // indicate main max in b1
						ibeg = lb2+3;
					}
				}
				fmax2:
				// 2nd max: if before max1 - save 1st found; if after max1 - save last found
				for (i=ibeg, bc = &lin_begs[ibeg], bp=bc-1, bc=bp+1, bn=bc+1; i<ilim; i++,bp++,bc++,bn++)
 {
    if ((*bc < *bp) || (*bc < *bn)) continue;
    // bs1 should NOT be close to bs2
    if ( (i > (lb2-4)) && (i < (lb2+4)) )  continue;
    if (ets && ((i > (ets-2)) && (i < (ets+2)) ))  continue;
    m=*bp + 2*(*bc) + *bn;
    if ((m1 < m) || ((m!=0) && (m1 == m) && (i > lb2)))
      { m1=m; ms1=m-*bc; lb1=i; }
 }
 if (lb1 < 0)  // ONE MAX obtained
 {
   if (ets)    // skipped 't' level ?
   {
     if (lb2 < ets)     //  lb2 ... ets
     {
       if (Nst < (ms2-2))   // more ususal letters than 't'
         { lb1 = lb2; lb2 = ets;  goto twomax; }
       else
         goto lb2tob1;

     }
     else              //   ets ... lb2
     {
       if (Nst < (ms2-2))   // more ususal letters than 't'
         { lb1 = ets; goto twomax; }
       else
         goto lb2tob2;

     }
   }
   //if (Ns1 >= Ns2)   // recall voting
   if (Ns1 > Ns2)   // recall voting
   {
   lb2tob1:
     bbs1 = lb2-32; Nb1=127; Nb2=-1;
     if ( old_ok && (lin_pass > 1) && (abs(bbs1-oldPs1) < 2) )
       bbs2 = bbs3-oldPs2;
     else
       bbs2 = (bbs3+2*bbs1)/3;
     goto rtn;
   }
   if (Ns2 > Ns1)   // recall voting
   {
   lb2tob2:
     bbs2 = lb2-32; Nb2=127; Nb1=-1;
     if ( old_ok && (lin_pass > 1) && (abs(bbs2-oldPs2) < 2) )
       bbs1 = bbs3-oldPs1;
     else
       bbs1 = bbs2 - (bbs3-bbs2)/2;
     goto rtn;
   }
   /*****  after 931228: **  TRY to use old line's bases  *****************/
   //if(language == LANG_RUSSIAN)         goto lb2tob2; // Valdemar
   if ((old_ok == 0)  || (lin_pass==1)) goto lb2tob1;
 try_old:
   Ps = oldPs2;
   if (Ps<=0) Ps = 1;
   if ((oldn1 != 127) && (oldn1 > Ns1))
   { bbs1 = bbs3-oldPs1; Ns1 = oldn1; Nb1=127;}
   if ((oldn2 != 127) && (oldn2 > Ns2))
   { bbs2 = bbs3-oldPs2; Ns2 = oldn2; Nb2=127;}
   if ((oldn4 != 127) && (oldn4 > Ns4))
   { bbs4 = bbs3-oldPs4; Ns4 = oldn4; Nb4=127;}
   goto rtn;
 }
 //  lb2 - more powerful max; lb1 - less powerful
 dl = lb1-lb2;
   /*#ifdef UFA
   if(language == LANG_RUSSIAN)
   {
   int16_t b2=MAX(lb1,lb2)-32;
   if( b2+3 >= bbs3/2 && fl12 != 1)
    { // one peak is a dream
    if( lb2-32 < bbs3/4 )
     { lb2 = -1; goto lb2tob1; }// bbs1 is a dream. Vald. for UFA
    else
     { lb1 = -1; goto lb2tob2; }// bbs2 is a dream. Vald. for UFA
    }
   }
   #endif*/
 if (dl >= 0)   // make them sorted: lb1, lb2
 {
   int16_t i;
   i=lb2; lb2=lb1; lb1=i;
   i=m2;  m2=m1;   m1=i;
 }
 if (ett)  // in presence of 't' lb1, lb2 must be properly positioned
 {
   if (lb2 < ett)     // base 2  must be greater than 't'
     lb2 = ett;
   else
   {
     if (lb1 > ett)
     { int16_t b2;
       b2 = lb1;
       lb1 = ett;
       // where is b2: which max is greater and use proportions
       if (m2 > 2*m1) goto twomax;               // lb2 wichtig
       if (m1 > 2*m2) { lb2 = lb1; goto twomax; }  // (940125 was  = i ??) lb1 wichtig
       b2x = (bbs3+2*(lb1-32))/3 + 32;
       if (lb2 > b2x)                 // lb2 too low - take 1st max
         lb2 = b2;
     }
   }
 }
 twomax:
 dl = lb2-lb1;
 if (dl < 3)     // similar bases
 {
   if (Ns1 < 3)  // nobody wanted bs1
     goto lb2tob2;
   if (Ns2 < 3)  // nobody wanted bs2
     goto lb2tob1;
   if(old_ok) goto try_old;  // real wiederspruch !!
 }
 bbs1 = lb1-32; Nb1=127; bbs2 = lb2-32; Nb2=127;
 /*************************************************************/
 b2x = (bbs3+2*bbs1)/3;         // 940125  UNLV1009/36
 if (bbs2 > b2x)   // b2 doubtful
 { int16_t fl_ser, s_ser, ng_ser, top, dif;
  // if (m2 <= m1)
  // {
  //   bbs2 = b2x; Nb2 = -1;
  // }
   ng_ser=0;
   s_ser=0;
   fl_ser=0;

   cb2=cell_f();
   //while ((cb2=cb2->nextl)->nextl)
   for(cb2=CSTR_GetNextRaster(cb2,f_letter);cb2;
       cb2=CSTR_GetNextRaster(cb2,f_letter) )
   {
     CSTR_GetAttr(cb2,&attr);
     top=attr.row-minrow;
     dif = attr.bdiff;

     if (dif != 127) top -= dif;
     if (abs(top-bbs2) < 2)    // interval of potential 'b2' letters
     {
       if (fl_ser==0)
         ng_ser++;
       fl_ser=1; s_ser++;
     }
     else
       fl_ser=0;
   }
   if  (s_ser <= 2*ng_ser)    // intervals of 'small' seem to be 'short'
   {
     bbs2 = b2x; Nb2 = -1;  // make artifitial b2
   }
 }
 /*******************************************************************/
rtn:
 complete_bas("b1/b2 made");
 set_basarr(&all_bases[0],-32000,32000);
 return 1;
}

		////////////////
void add_hist(CSTR_rast a, int16_t flg, uchar *begs) {
	int16_t hcell, hbeg;
	//lnhead *Lp1, *Lp2;
	CCOM_lnhead *Lp1, *Lp2;
	//c_comp *cp1;
	CCOM_comp *cp1;
	int16_t Lc1, lc1, rowd;
	CSTR_rast_attr attr;

	CSTR_GetAttr(a, &attr);
	hcell = attr.row - minrow;
	if ((rowd = -attr.bdiff) == -127)
		rowd = 0;
	rowd = rowd + hcell + 32;
	//cp1 = a->env;
	cp1 = CSTR_GetComp(a);

	Lc1 = cp1->nl; // number of lines in component

	//Lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
	Lp1 = (CCOM_lnhead *) (cp1->linerep + 2);
	Lp2 = Lp1;
	for (lc1 = 0; lc1 < Lc1; lc1++, Lp1 = Lp2) {
		int16_t th;
		//Lp2=(lnhead *) ((char *)Lp1+Lp1->lth);   // next line
		Lp2 = (CCOM_lnhead *) ((char *) Lp1 + Lp1->lth); // next line
		if (!(Lp1->flg & l_fbeg))
			continue;
		hbeg = Lp1->row;
		th = MIN(hcell + attr.h, bbs3) - hcell; // part of cell above bs3
		//  part of cell above bs3
		if (5* hbeg >= 3* th )
			continue; // line must begin above 2/5 of
		// the part of cell height
		// which is above bs3
		begs[rowd + hbeg] += flg;
	} // for all lines
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//AK! crash at memset
void hist_bot(int16_t x1, int16_t x2) {
	//cell      *a;
	CSTR_rast a;
	CSTR_rast_attr attr;

	int16_t hcell, hend;
	//lnhead    *Lp1, *Lp2;
	CCOM_lnhead *Lp1, *Lp2;
	//c_comp    *cp1;
	CCOM_comp *cp1;
	int16_t Lc1, lc1, rowd;
	CCOM_interval * int1, *int2;
	int il;

	if (hist_done & 1)
		return;
	hist_done |= 1;
	//glsnap('d',cell_f()->nextl,"make bases by histogram");
	//AK add loop memset(lin_ends,0,sizeof(lin_ends));
	for (il = 0; il < sizeof(lin_ends); lin_ends[il++] = 0)
		;

	a = cell_f();
	//while ((a=a->nextl)->nextl != NULL)
	a = CSTR_GetNextRaster(a, f_letter);
	for (; a; a = CSTR_GetNextRaster(a, f_letter)) {
		cp1 = CSTR_GetComp(a);
		CSTR_GetAttr(a, &attr);

		//if (a->env == NULL)
		if (!cp1)
			continue;
		if (attr.col < x1)
			continue;
		if (attr.col >= x2)
			break;
		hcell = attr.row - minrow;
		if ((rowd = -attr.bdiff) == -127)
			rowd = 0;
		rowd = rowd + hcell + 32;
		//cp1 = a->env;
		Lc1 = cp1->nl; // number of lines in component
		//Lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2);           // beginning of first line
		Lp1 = (CCOM_lnhead *) (cp1->linerep + 2); // beginning of first line
		Lp2 = Lp1;
		for (lc1 = 0; lc1 < Lc1; lc1++, Lp1 = Lp2) {
			Lp2 = (CCOM_lnhead *) ((char *) Lp1 + Lp1->lth); // next line
			if (!(Lp1->flg & l_fend))
				continue; // free ends only
			hend = Lp1->row + Lp1->h;
			//if (3*hend < 2*a->h)
			if (3* hend < 2* attr .h)
				continue; //  ignore stalagmites
			if (Lp1->h < 4)
				continue; // hair Valdemar 10-14-94 01:37pm
			hend += rowd;
			if ((attr.flg & CSTR_f_let) && (attr.basflg & CSTR_bs_round)) // all versions - round
			{ // test letter bottom more carefully
				int1 = (CCOM_interval *) (Lp1 + 1); // ptr to first interval
				int1 = int1 + Lp1->h - 1; // last interval;
				int2 = int1 - 1; // last but one
				if (int1->l < 3) // hair
				{
					if ((Lp1->h == 1) || ((int2->l - int1->l) > 2))
						hend--;// round letter, round end of line or short line
				}
			}
			if (hend >= 0 && hend < sizeof(lin_ends))
				lin_ends[hend]++;
		} // for all lines
	} // while letters and bads
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void hist_top(int16_t x1, int16_t x2) {
	//cell *a;
	CSTR_rast a;
	CSTR_rast_attr attr;
	int16_t hcell, hbeg;
	//lnhead *Lp1, *Lp2;
	CCOM_lnhead *Lp1, *Lp2;
	//c_comp *cp1;
	CCOM_comp *cp1;
	int16_t Lc1, lc1, rowd;
	int il;

	if (hist_done & 2)
		return;
	hist_done |= 2;
	//AK add loop memset(lin_begs,0,sizeof(lin_begs));
	for (il = 0; il < sizeof(lin_begs); lin_begs[il++] = 0)
		;
	a = cell_f();
	//while ((a=a->nextl)->nextl != NULL)
	a = CSTR_GetNextRaster(a, f_letter);
	for (; a; a = CSTR_GetNextRaster(a, f_letter)) {
		cp1 = CSTR_GetComp(a);
		CSTR_GetAttr(a, &attr);
		//if (a->env == NULL) continue;
		if (!cp1)
			continue;
		if (attr.col < x1)
			continue;
		if (attr.col >= x2)
			break;
		hcell = attr.row - minrow;
		if ((rowd = -attr.bdiff) == -127)
			rowd = 0;
		rowd = rowd + hcell + 32;
		//cp1 = a->env;
		Lc1 = cp1->nl; // number of lines in component
		//Lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
		Lp1 = (CCOM_lnhead *) (cp1->linerep + 2); // beginning of first line
		Lp2 = Lp1;
		for (lc1 = 0; lc1 < Lc1; lc1++, Lp1 = Lp2) {
			int16_t th, ih;
			Lp2 = (CCOM_lnhead *) ((char *) Lp1 + Lp1->lth); // next line
			if (!(Lp1->flg & l_fbeg))
				continue; // take free begs only
			hbeg = Lp1->row;
			th = MIN(hcell + attr.h, bbs3) - hcell; // part of cell above bs3
			//  part of cell above bs3
			if (2* hbeg >= th)
				continue; // line must begin above 1/2 of
			// the part of cell height
			// which is above bs3
			if ((attr.flg & CSTR_f_let) && (hbeg > 2))
				continue;
			// for recognized letters use upper edge only
			if (2* Lp1 ->h < attr.h)
				continue; //  Valdemar 09-21-94 04:53pm
			if (((ih = rowd + hbeg) > 0) && (ih < 191))
				lin_begs[ih]++;
		} // for all lines
	} // while letters and bads
}

int16_t bas_by_hist(int16_t x1, int16_t x2, int16_t fl) {
	if (fl & 1) {
		hist_bot(x1, x2);
		histes();
		diffs_by_cells();
	}
	if (fl & 2)
		hist_top(x1, x2);
	if (!approve_by_hist()) {
		int16_t rb12;
		rb12 = b1b2_byhist();
		if (rb12)
			return 1;
		bbs1 = 0;
		bbs3 = maxrow - minrow;
		Ps = (bbs3 - bbs1) * 2 / 3;
		if (Ps <= 0) {
			Ps = 1;
			bbs1 = bbs3 - 2;
		}
		bbs2 = bbs3 - Ps;
		bbs4 = bbs2 + Ps;
		Nb1 = Nb2 = Nb3 = Nb4 = -1;
	}
	return 1;
}

void histb(int16_t x1, int16_t x2, int16_t flg, uchar *begs) {
	CSTR_rast a;
	CSTR_rast_attr attr;

	memset(begs, 0, sizeof(lin_begs));
	a = cell_f();
	a = CSTR_GetNextRaster(a, f_letter);
	// while ((a=a->nextl)->nextl != NULL)
	for (; a; a = CSTR_GetNextRaster(a, f_letter)) {
		//if (a->env == NULL) continue;
		if (!CSTR_GetComp(a))
			continue;

		CSTR_GetAttr(a, &attr);

		if (attr.col < x1)
			continue;
		if (attr.col >= x2)
			break;
		add_hist(a, flg, begs);
	} // while letters and bads
}

void histes() {
	int16_t i, m, m3, lb3;
	uchar *bp, *bc, *bn;
	// find: MAX(segs) in same place as max(ends)
	m3 = -1;
	if (Ns3) {
		int16_t ibeg, iend;
		ibeg = bbs3 + 30;
		iend = ibeg + 6;
		for (i = ibeg, bc = &lin_ends[ibeg], bp = bc - 1, bn = bc + 1, m3 = 0; i
				< iend; i++, bp++, bc++, bn++) {
			m = *bp + 2* (* bc) + *bn;
			if (m3 < m)
			{	m3=m; lb3=i;}
		}
	}
	for (i=1, bp=lin_ends, bc=bp+1, bn=bc+1; i<127; i++,bp++,bc++,bn++)
	{
		if ((*bc < *bp) || (*bc < *bn)) continue;
		m=*bp + 2*(*bc) + *bn;
		if (m3 < m)
		{	m3=m; lb3=i;}
	}
	Nb3=127;
	if (m3 >= 0)
	bbs3 = lb3-32;
	else
	bbs3 = maxrow-minrow;
}
void discrim_pos();

int16_t defbas(int16_t filter) {
	int16_t mb1, i, bbh, mult;
	//cell *B1;
	CSTR_rast B1;
	CSTR_rast_attr attr;
	uchar ret;

#ifdef MULTI
	mult = 1;
#else
	mult = 0;
#endif

	bs_got = 0;
	mb1 = -1;
	bbh = -1;
	multi_bas = 0;
	all_caps = 0;
	forbiset = 0;
	diffs_made = 0;
	ret = filter & f_retain;
	HIST_STATISTIC = 0;

	B1 = cell_f();
	//while((B1=B1->next)->next)
	for (B1 = CSTR_GetNext(B1); B1; B1 = CSTR_GetNext(B1)) {
		CSTR_GetAttr(B1, &attr);
		// B1->difflg &= ret;  // retain some flags
		attr.difflg &= ret; // retain some flags
		CSTR_SetAttr(B1, &attr);
	}

	b2_solid = 0;

	// просуммируем статистику
	sum_ans = dbsum(filter);
	if (fl_artifact > 1)
		return 1;

	if (mult && language != LANG_RUSSIAN && all_b1(7, 8, 1, 6)) // all letters are capital
	{
		diffs_by_cells();
		histofl = 2;
		return 1;
	}

	i = Ns1 + Ns2;
	// Valdemar 03-22-94 03:22pm
	if ( // for English
	(((Ns3 < 4) || ((i < 5) && (3* i < ncbs)) || sum_ans) && language
			!= LANG_RUSSIAN) || // for Russian
			(sum_ans && language == LANG_RUSSIAN && HIST_STATISTIC == 0))
	// unable to make averages
	{
		if (mult && (mb1 = mult_b1(3, 4, 1, 4))) // caps only
		{
			diffs_by_cells();
			histofl = 2;
			return 1;
		}
		if (!snap_baselines_rbal('a'))//IGOR
		{//IGOR
			if (!(bbh = bas_by_hist(-32000, 32000, 3))) // histogramms unable to make b1/b2
			{
				histofl = 3;
				complete_bas("NO hist-bs");
				set_basarr(&all_bases[0], -32000, 32000);
				return 1;
			}
		}//IGOR
	}

	diffs_by_cells();
	if (lin_pass != 2) // draft or final passes
	{
		complete_bas("draft/fin bs");
		// histofl remains as was  at LINPOS
		set_basarr(&all_bases[0], -32000, 32000);
		return 1;
	}

	histofl = 0;
	krit_loc = 100; // simulate base changes; invoke next iteration
	if (histiter) // 2nd and all next iterations
		sum_ans = doubt_bas();
	else {
		histiter = 1;
		sum_ans = 0;
	}
	discrim_pos();
	if (sum_ans) // something wrong
	{
		if ((Ns1 > 5) && (Ns2 < 3) && (mb1 < 0) && mult && (mb1 = mult_b1(3, 4,
				1, 4)))
		// caps only ?
		{
			diffs_by_cells();
			histofl = 2;
			return 1;
		}

		set_basarr(&all_bases[0], -32000, 32000);
		if (language == LANG_RUSSIAN)
			if (ncbs < 8 || (Ns1 + Ns2) > 2)
				goto stand_bas;
		// NO histo

		if ((Ns1 + Ns2) < 5) // few letters - find b2, b1 by hist
		{
#ifdef UFA  // Valdemar 12-17-94 04:35pm
			if( 1 ) return 1; // UFA has no long strings for histogram
#endif
			if (!snap_baselines_rbal('a'))//IGOR
			{//IGOR
				if ((bbh < 0 && (!HIST_STATISTIC && language == LANG_RUSSIAN))
						&& (!(bbh = bas_by_hist(-32000, 32000, 3))))
				// histogramms unable to make b1/b2
				{
					histofl = 2;
					complete_bas("NO hist-bs");
					set_basarr(&all_bases[0], -32000, 32000);
					return 1;
				}
			}//IGOR
			complete_bas("hist bs");
			set_basarr(&all_bases[0], -32000, 32000);
			histofl = 1;
			return 1;
		} else // many letters  --- actual multi
		{
			if ((Ns1 > 5) && (Ns2 < 3) && (mb1 < 0) && (mb1 = mult_b1(3, 4, 1,
					4)))
			// caps only ?
			{
				diffs_by_cells();
				histofl = 2;
				return 1;
			}

			if (mult && (sum_ans == 22)) // base 2 doubtful
			{
				histofl = 3;
				multi_bas = 2;
				// multi_hist(2);
				return 1;
			}
			/***************************************************    ?? OFF 940106
			 // 931229
			 if (mult && (all_doubts & 32))  // base 1 doubtful
			 {
			 histofl = 3;
			 multi_bas = 1;
			 // multi_hist(1);
			 return 1;
			 }
			 *******************************************************/
			if (!snap_baselines_rbal('a'))//IGOR
			{//IGOR
				if ((bbh < 0 && (!HIST_STATISTIC && language == LANG_RUSSIAN))
						&& (!(bbh = bas_by_hist(-32000, 32000, 3))))
				// histogramms unable to make b1/b2
				{
					histofl = 3;
					complete_bas("NO hist-bs");
					set_basarr(&all_bases[0], -32000, 32000);
					return 1;
				}
			}//IGOR

			complete_bas("hist bs");
			set_basarr(&all_bases[0], -32000, 32000);
			histofl = 1;
			return 1;

		} // many letters while wrong bases
	} // wrong bases
	stand_bas: complete_bas("stand bs");
	set_basarr(&all_bases[0], -32000, 32000);
	return 1;
}

///////////
void discrim_pos() {
	CSTR_rast B1;
	CSTR_rast_attr attr;

	B1 = cell_f();
	//while ((B1=B1->next)->next)
	for (B1 = CSTR_GetNext(B1); B1; CSTR_SetAttr(B1, &attr), B1 = CSTR_GetNext(
			B1)) {
		CSTR_GetAttr(B1, &attr);

		if (Ns1 > 3)
			if ((attr.row - minrow + 3) < bbs1) {
				attr.difflg |= 64;
				forbiset++;
				continue;
			}
		if ((attr.row + attr.h - minrow - 3) > bbs4) {
			attr.difflg |= 64;
			forbiset++;
			continue;
		}
		if ((attr.row - minrow) > bbsm) {
			attr.difflg |= 64;
			forbiset++;
			continue;
		}
		if ((attr.row + attr.h - minrow) < bbsm) {
			attr.difflg |= 64;
			forbiset++;
			continue;
		}
		if ((attr.h + 4) < Ps) {
			attr.difflg |= 64;
			forbiset++;
			continue;
		}
	}
}

int16_t smart_diff( CSTR_rast); // reject stupid diffs


void diffs_by_cells() {
	int16_t i, j, df, mdif, ndif, difet;
	CSTR_rast B1, P, N;
	CSTR_rast_attr attr;

	if (diffs_made)
		return;

	B1 = cell_f();
	if (fl_artifact) {
		for (B1 = CSTR_GetNext(B1); B1; B1 = CSTR_GetNext(B1))
		//while((B1=B1->next)->next)
		{
			CSTR_GetAttr(B1, &attr);
			attr.bdiff = 0;
			attr.difflg = 0;
			CSTR_SetAttr(B1, &attr);
		}
		return;
	}

	diffs_made = 1;
	// if( hist_diffs() ) return;
	mdif = ndif = 0;
	difet = (fax1x2) ? 4 : 2;

	//while ((B1=B1->next)->next)
	for (B1 = CSTR_GetNext(B1); B1; B1 = CSTR_GetNext(B1)) {
		CSTR_GetAttr(B1, &attr);
		if (attr.difflg & 64)
			continue; // rejected earlier
		if (attr.difflg & 4) // had defined main base ?
		{
			attr.bdiff = attr.row - minrow + attr.h - bbs3;
			CSTR_SetAttr(B1, &attr);
			df = attr.bdiff;
			df = smart_diff(B1);
			CSTR_GetAttr(B1, &attr);
			ndif++;
			mdif += abs(df);
			attr.difflg &= ~(CSTR_db_up | CSTR_db_down); // reset bdiff corr flags
		}
		CSTR_SetAttr(B1, &attr);
	}

	mdif = (ndif) ? mdif / ndif : 0;
	// how many spontaneous diffs
	i = j = 0;
	B1 = cell_f();
	//while ((B1=B1->nextl)->nextl)
	for (B1 = CSTR_GetNextRaster(B1, f_letter); B1; B1 = CSTR_GetNextRaster(B1,
			f_letter)) {
		CSTR_GetAttr(B1, &attr);
		if (attr.difflg & 64)
			continue; // rejected earlier
		if (attr.difflg & 4) // defines baseline ?
		{
			j++;
			if (abs(attr.bdiff) > (mdif + difet))
				i++;
		}
	}

	if (3* i <= j) // not too many stupid letters - may forbid stupid
	{
		B1 = cell_f();
		//while ((B1=B1->nextl)->nextl)
		for (B1 = CSTR_GetNextRaster(B1, f_letter); B1; B1
				= CSTR_GetNextRaster(B1, f_letter)) {
			CSTR_GetAttr(B1, &attr);
			if (attr.difflg & 64)
				continue; // rejected earlier
			if (attr.difflg & 4) //  defines baseline ?
			{
				if (abs(attr.bdiff) > (mdif + difet)) // difer - upon FAX
				{
					attr.difflg &= ~4; // reset local position
					attr.difflg |= 64; // forbid base setting
					forbiset++;
				}
			} // if defined b3
			CSTR_SetAttr(B1, &attr);
		} // while let-bad
	} // if few stupid

	/* finally set diffs for everybody  */
	P = N = cell_f();
	while (1) {
		//N=N->nextl;
		N = CSTR_GetNextRaster(N, f_letter | CSTR_f_fict);
		CSTR_GetAttr(N, &attr);

		//   if ((N->flg & c_f_fict) ||
		//       ((N->difflg & 4) && (!(N->difflg & 64))) /* defines base */
		//      )
		if ((attr.flg & CSTR_f_fict) || ((attr.difflg & 4) && (!(attr.difflg
				& 64))) /* defines base */
		) {
			B1 = P;
			while (1) {
				//B1=B1->next;
				B1 = CSTR_GetNext(B1);
				if (!B1 || B1 == N)
					break;
				interdif(P, N, B1);
			}
			P = N;
		}

		//if (N->flg & c_f_fict) break;
		if (attr.flg & CSTR_f_fict)
			break;
	} // while let_bad
}

void dbreset(int16_t filter) {
	CSTR_rast B1;
	CSTR_rast_attr attr;
	uchar ret;
	UniVersions vers;
	int i;

	ret = filter & 0xc0; // retain suppress & solid flags only
	B1 = cell_f();
	//while((B1=B1->next)->next)
	for (B1 = CSTR_GetNext(B1); B1; B1 = CSTR_GetNext(B1)) {
		CSTR_GetAttr(B1, &attr);
		//B1->difflg &= ret;  // retain some flags
		attr.difflg &= ret; // retain some flags
		CSTR_SetAttr(B1, &attr);
		if (filter & f_cut) {
			CSTR_GetCollectionUni(B1, &vers);
			//for (v0=B1->vers; v0->let != 0; v0++)
			//{
			//  if (v0->prob < 200)                 // was initially poor
			//    v0->prob |= 1;                    // mark as cutten
			//}
			for (i = 0; i < vers.lnAltCnt; i++) {
				if (vers.Alt[i].Prob < 200)
					vers.Alt[i].Prob |= 1;
			}
			CSTR_StoreCollectionUni(B1, &vers);
		}
	}
}
/***************************************************************
 **************************************************/
void hist_max(CSTR_rast a) {
	int16_t m1, m2, hcell, hbeg, lntyp, lnbeg;
	CCOM_interval * int1, *int2;
	CCOM_lnhead *Lp1, *Lp2;
	CCOM_comp *cp1;
	bas_acc_t *ba1, *pa1, *pa2;
	int16_t Lc1, lc1, rowd;
	CSTR_rast_attr attr;

	CSTR_GetAttr(a, &attr);
	memset(bas_acc, 0, sizeof(bas_acc));

	hcell = attr.row - minrow;
	if ((rowd = -attr.bdiff) == -127)
		rowd = 0;
	rowd = rowd + hcell;
	//cp1 = a->env;
	cp1 = CSTR_GetComp(a);
	Lc1 = cp1->nl; // number of lines in component
	//Lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
	Lp1 = (CCOM_lnhead *) (cp1->linerep + 2); // beginning of first line
	Lp2 = Lp1;
	for (lc1 = 0; lc1 < Lc1; lc1++, Lp1 = Lp2) {
		int16_t th;
		//Lp2=(lnhead *) ((char *)Lp1+Lp1->lth);   // next line
		Lp2 = (CCOM_lnhead *) ((char *) Lp1 + Lp1->lth); // next line
		if (!(Lp1->flg & l_fbeg))
			continue;
		hbeg = Lp1->row;
		if (hbeg == 0)
			continue; // ignore upper scan lines

		th = MIN(hcell + attr.h, bbs3) - hcell; // part of cell above bs3

		//  part of cell above bs3
		if (5* hbeg >= 3* th )
			continue; // line must begin above 2/5 of
		// the part of cell height
		// which is above bs3
		int1 = (CCOM_interval *) (Lp1 + 1); // ptr to first interval
		lntyp = 1; //  assume wide
		if ((Lp1->h == 1) && (int1->l < 3))
			lntyp = 3; // hair
		else {
			int2 = int1 + 1;
			if (int2->l <= (int1->l + 2))
				lntyp = 2; // stick
		}
		lnbeg = hbeg + rowd;
		for (ba1 = bas_acc; ba1 < bas_acc + 8; ba1++) {
			if (ba1->cnt[0] == 0) // same base not found
			{
				ba1->row = lnbeg;
				ba1->cnt[lntyp] = 1;
				ba1->cnt[0] = 1;
				break;
			}
			if (abs(lnbeg - ba1->row) < 2) // same base found
			{
				ba1->cnt[0]++;
				ba1->cnt[lntyp]++;
				break;
			}
		}
	} // for all lines
	for (ba1 = bas_acc, pa1 = NULL, m1 = 0; ba1 < bas_acc + 8; ba1++)
		if (m1 < ba1->cnt[0]) {
			m1 = ba1->cnt[0];
			pa1 = ba1;
		}
	if (pa1 == NULL) // no more maximums
		return;
	pa1->cnt[0] = 0; // this one will not be found as a max more
	for (ba1 = bas_acc, pa2 = NULL, m2 = 0; ba1 < bas_acc + 8; ba1++)
		if (m2 < ba1->cnt[0]) {
			m2 = ba1->cnt[0];
			pa2 = ba1;
		}

	if (pa2 == NULL) // no more maximums
	{
		// a->bas1 = (char)pa1->row;
		attr.bas1 = (char) pa1->row;
		CSTR_SetAttr(a, &attr);
		return;
	}

	if (pa1->row > pa2->row) // pa1 - upper of 2 max
	{
		ba1 = pa1;
		pa1 = pa2;
		pa2 = ba1;
	}

	attr.bas1 = (char) pa1->row;
	// 931025 if (pa1->cnt[1] > pa1->cnt[2])
	// 931025   a->basflg |= c_bs_m1r;
	attr.bas2 = (char) pa2->row;
	// 931025 if (pa2->cnt[1] > pa2->cnt[2])
	// 931025   a->basflg |= c_bs_m2r;
	CSTR_SetAttr(a, &attr);
}

void set_rus_difflg(CSTR_rast B1, int16_t filter); // linutil.c

void make_difbas(CSTR_rast B1, int16_t filter) {
	CSTR_rast_attr attr;

	CSTR_GetAttr(B1, &attr);

	attr.bas1 = -128;
	attr.bas2 = -128;
	if (attr.flg & CSTR_f_bad) {
		attr.basflg = 0;
		attr.difflg = 0;
		CSTR_SetAttr(B1, &attr);
		hist_max(B1);
		return;
	}

	attr.basflg = 0;
	CSTR_SetAttr(B1, &attr);

	if (language != LANG_RUSSIAN)
		set_difflg(B1, filter);
	else
		set_rus_difflg(B1, filter);
}

static uchar t_type;
// set basflg, difflg;
void set_difflg(CSTR_rast B1, int16_t filter) {
	//c_comp *env;
	CCOM_comp *env;
	uchar let, tbe, tshp;
	uchar fl1, fl2, fl2t, flt, fl_cut, fl_retain;
	//  version *v0;
	uchar cap_shape;
	UniVersions vers;
	int i;
	CSTR_rast_attr attr;

	CSTR_GetAttr(B1, &attr);

	fl_cut = filter & f_cut;
	fl_retain = filter & f_retain;
	fl1 = 0xff; // all lines defined
	fl2 = fl2t = 0;
	flt = 0;
	//env=B1->env;
	env = CSTR_GetComp(B1);

	t_type = 0;
	cap_shape = 1; // capital in shape

	CSTR_GetCollectionUni(B1, &vers);
	//for (v0=B1->vers; (let=v0->let) != 0; v0++)
	for (i = 0; i < vers.lnAltCnt; i++) {
		let = vers.Alt[i].Liga;

		tbe = let_lindef[let];
		tshp = let_linshape[let];
		cap_shape &= tshp;
		if (let == 't') {
			flt = CSTR_bs_bt;
			t_type |= 1;
		} // 't' obtained

		if (env && env->large & CCOM_LR_UNDERLINED) {
			if (tshp & 2) // sticky in shape
			{
				fl1 = 0;
				break;
			} // no lines defined
		}

		fl2t |= tbe; // accumulate b1/b2

		//if ( fl_cut && (v0->prob & 1))                  // cutten version
		if (fl_cut && (vers.Alt[i].Prob & 1)) // cutten version
			continue;

		fl2 |= tbe; // accumulate b1/b2
		if (dust_in == 0) {
			if (tshp & 4) // stick allows to define bs1 ( iI1l )
			{
				if (oNb2 > 3) // base 2 defined
				{
					if (attr.h > (oPs + 3)) {
						t_type |= 4;
						tbe |= 1;
					} // allow to def bs1
				} else
					tbe &= 0xf4;
				// retain only b3 line for sticks while dusts not present
			}
			////// 93.08.19  No more RQDOT
			/*****************
			 if ((let=='i') || (let=='j'))
			 B1->cg_flag |= c_cg_rqdot;   // dot needed for 'i' and 'j'
			 ******************/
		}
		if ((tbe & 1) && ((t_type & 4) == 0)) // bs1 defined, not by stick
			t_type |= 2; // large
		t_type &= 3; // retain accumulated 't' and 'large'

		fl1 &= tbe;
	} // FOR versions

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
				attr.basflg |= (CSTR_bs_b1a | CSTR_bs_b2a); // agrees to be both b1. b2
			}
		}
		if (fl2 & (CSTR_db_b1 | c_df_b1a))
			attr.basflg |= CSTR_bs_b1a;
		if (fl2 & (CSTR_db_b2 | c_df_b2a))
			attr.basflg |= CSTR_bs_b2a;
		attr.difflg = attr.difflg | (fl1 & 15);
		//if (flt & (B1->nvers==1))
		if (flt & (vers.lnAltCnt == 1))
			attr.basflg |= CSTR_bs_t; // "t-only" version
	}

	CSTR_SetAttr(B1, &attr);
}

uchar disable_twins; // only for russian
void all_cell_levcut( int16_t);
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// собрать статистику для поиска БЛ
int16_t dbsum(int16_t filter) {
	CSTR_rast B1;
	CSTR_rast_attr attr;

	int16_t rz, fl1, fls, wr, top, bot;
	int16_t draft_cut_12, draft_cut_3;
	int16_t hist_b3[RASTER_MAX_HEIGHT * 2];

	char ww[256];//IGOR
	int16_t jl = 0, i_method = 0;//IGOR
	CSTR_attr m_attr;//IGOR

	hi_ac *ha;
	draft_cut_12 = draft_cut_3 = 0;
	disable_twins = 0; // enable all
	page_stat = 0; // clear stat flag
	recalc: Ns1 = Ns2 = Ns3 = Ns4 = Nst = nctot = ncbs = ncletrs = nab1 = sbs1
			= sbs2 = sbs3 = sbs4 = sbst = sbsu = sbsd = 0;
	max_height = sum_maxrow = sum_cellth = sum_letlth = 0;
	old_minrow = min_crow;
	old_maxrow = max_crow;
	max_crow = -32000;
	min_crow = 32000;
	out_of_minmax = 0;
	memset(hi_beg, 0, sizeof(hi_beg));
	memset(hist_b3, 0, sizeof(hist_b3));

	B1 = cell_f();
	B1 = CSTR_GetNextRaster(B1, f_letter);
	//while ((B1=B1->nextl)->nextl)
	for (; B1; B1 = CSTR_GetNextRaster(B1, f_letter)) {
		uchar ww;

		CSTR_GetAttr(B1, &attr);

		if (max_height < (ww = (char) attr.h))
			max_height = ww;
		top = attr.row - minrow;
		bot = top + ww;
		if (old_minrow > top)
			out_of_minmax++;
		if (old_maxrow < bot)
			out_of_minmax++;
		if (min_crow > top)
			min_crow = top;
		if (max_crow < bot)
			max_crow = bot;
		nctot++;
		ww = (char) attr.w;
		sum_cellth += ww;
		sum_maxrow = sum_maxrow + bot;
		if (attr.difflg & 64)
			continue; // rejected earlier
		ncbs++;
		sbsu += top;
		sbsd = sbsd + bot;

		make_difbas(B1, filter); // use filtered letters only

		// атрибуты могли измениться!?
		CSTR_GetAttr(B1, &attr);

		if (attr.flg & (CSTR_f_let)) {
			int16_t i, d;

			ncletrs++;
			sum_letlth += ww;
			fl1 = attr.difflg;
			fls = attr.basflg;
			wr = attr.row - minrow;
			if ((d = attr.bdiff) != 127)
				wr -= d;
			i = wr + 32;
			// 940415
			if ((i < 0) || (i > 191))
				continue;
			ha = hi_beg + i;
			(*ha).all++;
			if (t_type == 1) // 't' versions only
			{
				Nst++;
				sbst += wr;
			}
			if ((fl1 & 3) != 3) {
				if (fl1 & 1) // upper defined
				{
					Ns1++;
					sbs1 += wr;
					(*ha).b1++;
					if (fls & CSTR_bs_cap)
						(*ha).bc++;
				}
				if (fl1 & 2) // small defined
				{
					Ns2++;
					sbs2 += wr;
					(*ha).b2++;
				}
			}
			if (fls & CSTR_bs_b1a) {
				nab1++;
				(*ha).a1++;
			}
			if (fls & CSTR_bs_b2a)
				(*ha).a2++;
			if (fl1 & 4) // base defined
			{
				Ns3++;
				sbs3 = sbs3 + wr + attr.h;
				if ((wr + attr.h) < RASTER_MAX_HEIGHT * 2 && (wr + attr.h) >= 0)
					hist_b3[wr + attr.h]++;
			}
			if (fl1 & 8) // lowest defined
			{
				Ns4++;
				sbs4 = sbs4 + wr + attr.h;
			}
		} // If  letter
	} // while letter or bad cell

	if (!draft_cut_3) {
		tell_for_b3(hist_b3);
		if (stable_b3) {
			draft_cut_3 = 1;
			if (draft_cut_hyps(3, 0))
				goto recalc;
		}
	}

	// собственно поиск БЛ
	//IGOR
	if (snap_baselines_rbal('c'))
		i_method = 3;
	else if (snap_baselines_rbal('b'))
		i_method = 2;
	else if (snap_baselines_rbal('a'))
		i_method = 1;

	if (i_method != 1) {
		rz = calc_base();
		if (db_status && snap_is_marked_rbal(lin_str))//IGOR
		{
			jl
					+= sprintf(
							ww + jl,
							"line %d, lin_pass %d, language %d\nBsl: %d %d %d %d (PUMA)\n",
							line_number, lin_pass, language, minrow + bbs1,
							minrow + bbs2, minrow + bbs3, minrow + bbs4);
		}
	}

	if (i_method != 0) {
		int16_t n1 = 0, n2 = 0, n3 = 0, n4 = 0;
		m_attr.row = minrow;
		m_attr.language = language; //IGOR

		StatSearchBL(lin_str, &m_attr, &n1, &n2, &n3, &n4);

		if (i_method == 1) {
			bbs1 = m_attr.bs1;
			bbs2 = m_attr.bs2;
			bbs3 = m_attr.bs3;
			bbs4 = m_attr.bs4;

			bbsm = (m_attr.bs3 + m_attr.bs2) / 2;

			Ps = bbs3 - bbs2;

			Nb1 = m_attr.Nb1;
			Nb2 = m_attr.Nb2;
			Nb3 = m_attr.Nb3;
			Nb4 = m_attr.Nb4;

			Ns1 = n1, Ns2 = n2, Ns3 = n3, Ns4 = n4;

			page_stat = 1;
			//			histofl   = 1;
			//			b2_solid  = 1;
			//			bs_got    = 1;

			rz = 1;
		}

		if (db_status && snap_is_marked_rbal(lin_str))//IGOR
		{
			if (jl > 0) {
				jl += sprintf(ww + jl, "Bsl: %d %d %d %d (STAT)\n", minrow
						+ m_attr.bs1, minrow + m_attr.bs2, minrow + m_attr.bs3,
						minrow + m_attr.bs4);
			} else {
				jl
						+= sprintf(
								ww,
								"line %d, lin_pass %d, language %d\nBsl: %d %d %d %d (STAT)\n",
								line_number, lin_pass, language, minrow
										+ m_attr.bs1, minrow + m_attr.bs2,
								minrow + m_attr.bs3, minrow + m_attr.bs4);
			}
		}
	}
	//IGOR
	// Valdemar 17.11.93  Russian short indefinite lines
	if (language == LANG_RUSSIAN) {
		if (Ns2 == 0 && Ns1 == 0 && Ns3 > 1 && !draft_cut_3) {
			draft_cut_3 = 1;
			disable_twins = 2; // pPyY
			if (draft_cut_hyps(3, 0))
				goto recalc;
			// if Some cuttings was made - recalc
		} // Valdemar 11.12.93
		if (rz >= 10 && (page_stat || HIST_STATISTIC) && !draft_cut_12) { // set b1/b2 by page statistics
			draft_cut_12 = 1;
			disable_twins = 1; // disable all twins
			if (draft_cut_hyps((int16_t)(rz / 10) /* == 2 ? 1 : 2*/, 0))
				goto recalc;
		}
		if ((draft_cut_12 || draft_cut_3) && lin_pass == 3)
			all_cell_levcut(1); // reject some hyps
	}

	Ps = bbs3 - bbs2;
	if (Ps <= 0) {
		Ps = 1;
		bbs2 = bbs3 - 1;
	}

	//IGOR
	if (db_status || (db_status && i_method == 3 && lin_pass == 3 && (bbs1
			!= m_attr.bs1 || bbs2 != m_attr.bs2 || bbs3 != m_attr.bs3 || bbs4
			!= m_attr.bs4))) {
		Point16 rs, re, rsm, rem;
		int16_t startx, endx, mc;
		uint32_t rgb;
		CSTR_attr line_attr;

		if (CSTR_GetLineAttr(lin_str, &line_attr)) {
			startx = line_attr.col;
			endx = startx + (short) line_attr.wid;
		} else {
			rs.x = mincol;
			re.x = mincol + maxcol;
		}

		if (language == 3) {
			if (i_method != 1)//PUMA
			{
				int16_t ic = 0;
				rgb = wRGB(64, 128, 128);
				rs.x = startx;
				re.x = startx + MIN(80, endx - startx);
				while (rs.x <= endx && re.x <= endx) {
					draw_puma_bl(1,3);
					if (ic == 0) {
						draw_bl_marking(1,3);
					}
					draw_puma_bl(2,3);
					if (ic == 0) {
						draw_bl_marking(2,3);
					}
					draw_puma_bl(3,3);
					if (ic == 0) {
						draw_bl_marking(3,3);
					}
					if (bbs4 != -1) {
						draw_puma_bl(4,3);
						if (ic == 0) {
							draw_bl_marking(4,3);
						}
					}
					rs.x = re.x + 5;
					re.x = rs.x + MIN(80, abs(endx - rs.x));
					ic = 1;
				}
			}

			if (i_method != 0)//STAT
			{
				rs.x = startx;
				re.x = endx;
				rgb = wRGB(128, 0, 128);
				draw_stat_bl(1,3);
				draw_bl_marking(1,3);
				draw_stat_bl(2,3);
				draw_bl_marking(2,3);
				draw_stat_bl(3,3);
				draw_bl_marking(3,3);
				if (m_attr.bs4 != -1) {
					draw_stat_bl(4,3);
					draw_bl_marking(4,3);
				}
			}
		} else {
			if (i_method != 1)//PUMA
			{
				int16_t ic = 0;
				rgb = wRGB(124, 190, 190);
				rs.x = startx;
				re.x = startx + MIN(80, endx - startx);
				while (rs.x <= endx && re.x <= endx) {
					draw_puma_bl(1,0);
					if (ic == 0) {
						draw_bl_marking(1,0);
					}
					draw_puma_bl(2,0);
					if (ic == 0) {
						draw_bl_marking(2,0);
					}
					draw_puma_bl(3,0);
					if (ic == 0) {
						draw_bl_marking(3,0);
					}
					if (bbs4 != -1) {
						draw_puma_bl(4,0);
						if (ic == 0) {
							draw_bl_marking(4,0);
						}
					}
					rs.x = re.x + 5;
					re.x = rs.x + MIN(80, abs(endx - rs.x));
					ic = 1;
				}
			}

			if (i_method != 0)//STAT
			{
				rs.x = startx;
				re.x = endx;
				rgb = wRGB(255, 0, 255);
				draw_stat_bl(1,0);
				draw_bl_marking(1,0);
				draw_stat_bl(2,0);
				draw_bl_marking(2,0);
				draw_stat_bl(3,0);
				draw_bl_marking(3,0);
				if (m_attr.bs4 != -1) {
					draw_stat_bl(4,0);
					draw_bl_marking(4,0);
				}
			}
		}

		if (db_status) {
			sprintf(
					ww + jl,
					"summ: r=%d min=%d nc=%d Ps=%d Bs: %d %d %d %d, Ns: %d %d %d %d",
					rz, minrow, ncbs, Ps, sbs1, sbs2, sbs3, sbs4, Ns1, Ns2,
					Ns3, Ns4);
			glsnap('d', cell_f()->next, ww);
		}

		if (db_status && snap_is_marked_rbal(lin_str)) {
			CSTR_line ln[] = { lin_str };
			char txt[] = "Press any key...\n";

			sprintf(ww + jl, "%s", txt);

			snap_show_text_rbal(ww);
			snap_monitor_ori_rbal(&ln[0], 1);
		}

		if (i_method != 3 || (db_status && snap_is_marked_rbal(lin_str))) {
			if (language == 3) {
				del_bl(1,3);
				del_bl(2,3);
			} else {
				del_bl(1,0);
				del_bl(2,0);
			}
		}
	}
	//IGOR
	return rz;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
// собственно поиск БЛ
int16_t calc_base() {
	int16_t j, iret;
	int32_t w1, w2;

	Nb2 = Ns2;
	Nb3 = Ns3;
	Nb1 = Ns1;
	Nb4 = Ns4;
	iret = 0;

	if ((lin_pass > 1) && (out_of_minmax > 2) && (3* max_height < 2*
			(max_crow -min_crow)))
			{
				CSTR_rast C;
				CSTR_rast_attr attr;
				int16_t top, middle;

				all_wrong:
				fl_artifact = 3;
				bbsm = (min_crow+max_crow)/2;
				bbs2 = bbsm - max_height/2;
				bbs3 = bbsm + max_height/2;
				bbs1 = bbsm - max_height;
				bbs4 = bbsm + max_height;
				Ps=Psf=bbs3-bbs2;
				if (Ps <= 0)
				{
					Ps = 1;
					bbs2=bbs3-1;
				}
				Nb1=Nb2=Nb3=Nb4=0;
				all_caps = 0;

				set_basarr(&all_bases[0],-32000,32000);
				{	char ww[128];
					sprintf(ww,"WRONG BASES, min=%d Ps=%d Bs: %d %d %d %d, Ns: %d %d %d %d",
                  minrow, Ps, bbs1, bbs2, bbs3, bbs4, Ns1, Ns2, Ns3, Ns4);
       glsnap ('d',cell_f()->next,ww);
   }

   C=cell_f();
   C=CSTR_GetNextRaster(C,f_letter);
   //while ((C=C->nextl)->nextl)
   for(;C;C=CSTR_GetNextRaster(C,f_letter))
   {
     CSTR_GetAttr(C,&attr);

     top = attr.row - minrow;
     middle = top + attr.h/2;
     attr.difflg = 0;
     attr.bdiff = middle - bbsm;

	 CSTR_SetAttr(C,&attr);
   }
   return 1;
 }

	if (Ns3 > 0)
	bbs3=(sbs3+(Ns3>>1))/Ns3;
	else
	{
		if (ncbs)
		{
			fl_artifact = 1;
			if(language!=LANG_RUSSIAN ) {
				bbs3=sbsd/ncbs;
				bbs1=sbsu/ncbs;
			}
			else
			{
				if(HIST_STATISTIC)
				{
					Nb3=128; Nb1=128;
				}
				else
				{
					bbs3=sbsd/ncbs; bbs1=sbsu/ncbs;
				}
				goto page_stat;
			}
		}
		else
		{
			fl_artifact = 2;
			bbs3 = max_crow;
			bbs1=0;
		}

		bbs2=((bbs1<<1)+bbs3)/3; bbs4=((bbs3<<2)-bbs1)/3;
		bbsm=(bbs2+bbs3)>>1;
		Ps=Psf=bbs3-bbs2;
		if (Ps <= 0)
		{
			Ps = 1;
			bbs2 = bbs3-1;
		}
		goto iret1;
	}

	if (Ns2 > 0)
	{
		if (3*Ns2 > Ns3)
		bbs2=(sbs2+(Ns2>>1))/Ns2;
		else
		{	int16_t i;
			w1=sbs3; w1*=Ns2;
			w2=sbs2; w2*=Ns3;
			w1-=w2;
			j=Ns2*Ns3;
			i = (w1+j/2)/j;
			bbs2=bbs3-i;
		}
		bbsm=(bbs2+bbs3)>>1;
		Ps = bbs3 - bbs2;
		if (Ps <= 0)
		{
			Ps = 1;
			bbs2 = bbs3-1;
		}
	}

	if (Ns1 > 0)
	bbs1=(sbs1+(Ns1>>1))/Ns1;
	else
	Nb1=-1;
	if (Ns4 > 0)
	bbs4=(sbs4+(Ns4>>1))/Ns4;
	else
	Nb4=-1;

	if( language == LANG_RUSSIAN && (stable_b3 || Ns3<5))
	{
		int16_t pSt;
		if( pSt=GetPsFromHeights(), pSt!=0)
		{
			if(pSt>0)
			{
				Ps=pSt;
				bbs2=bbs3-pSt; if(!Ns2) Nb2=128;
				if( !Ns1 || bbs2-bbs1 < pSt/5 ) {bbs1=bbs2 - Ps/3; Nb1=128;} // bad B1
				if( Ns1 == 0 && Ns2 == 0 ) return 20; // b2 made
			}
			else
			{
				pSt=-pSt;
				bbs1=bbs3-pSt; if(!Ns1) Nb1=128;
				if( !Ns2 || bbs2-bbs1 < pSt/7 ) {bbs2=bbs1+pSt/3; Nb2=128;} // bad B2
				if( Ns1 == 0 && Ns2 == 0 ) return 10; // b1 made
			}
			goto iret0;
		}
		if(Ns2>0 && Ns1>0 && abs(bbs1-bbs2) <= 3) goto page_stat;
	} // ajust weak definitions
	if (Ns2 > 0)
	{
		if (Ns1 <= 0)
		bbs1=(3*bbs2-bbs3)>>1;
		if (Ns4 <= 0)
		bbs4=(3*bbs3-bbs2)>>1;
		goto iret0;
	}
	// B2 absent
	if (Ns1 > 0)
	{
		bbs2=((bbs1<<1)+bbs3)/3;
		if (Ns4 <= 0)
		bbs4=((bbs3<<2)-bbs1)/3;
		goto iret0;
	}
	// B1, B2 absent
	page_stat:
	if(language == LANG_RUSSIAN)
	{ // try to get b1,b2 from page statistic
		int16_t tmp;
#ifdef UFA
		uf_bases uf_b;
		uf_b = uf_let_case();
		if( uf_b.bb1==0 && uf_b.bb2==0 )
		{	if((tmp=setup_let_case(0))) return tmp*10;}// recalc line
		else
		{
			bbs2=uf_b.bb2; bbs1=uf_b.bb1;
			Ns1=Ns2=Nb1=Nb2=99; // artifact
			if( Ns3 <= 0 ) {bbs3 = uf_b.bb3; Ns3=Nb3=99;}
			goto iret0;
		}
#else
		if((tmp=setup_let_case(0)))
		return tmp*10;
#endif
	}

	bbs1=0;
	bbs2=bbs3/3;
	if (Ns4 <= 0)
	{	bbs4 = bbs3; goto iret2;} // B3 only

	j=bbs4-bbs3;
	bbs2=bbs3-(j<<1);
	bbs1=bbs2-j;
	goto iret3; // B3, B4 only

	iret3: iret++;
	iret2: iret++;
	iret1: iret++;
	iret0:
	if ((bbs3 < -26) || (bbs3 > 159))
	goto all_wrong;
	if ((bbs2 < -30) || (bbs2 > 155))
	goto all_wrong;
	if ((bbs1 < -31) || (bbs1 > 155))
	goto all_wrong;
	return iret;
}

// какие есть сомнения по БЛ
// возврат - не мера оценки, а коды, например 21 - сомнения
// по 1-й и 2-й линиям
static int16_t doubt_bas() {
	hi_ac *hc, *hp, *hn, *hpp, *hnn, *hl1, *hl2;
	int16_t n1, n2, n3, n4, n5, s, s5, n;
	uchar b12_s;

	all_doubts = 0;
	b12_s = 0;
	if ((bbs3 < -26) || (bbs3 > 159))
		goto all_wrong;
	if ((bbs2 < -30) || (bbs2 > 155))
		goto all_wrong;
	if ((bbs1 < -31) || (bbs1 > 155))
		goto all_wrong;
	if (Nb3 <= 0) {
		all_wrong: all_doubts |= 1;
		return 30;
	}
	if ((Nb2 <= 0) && (Nb1 <= 0)) {
		all_doubts |= 2;
		if (Nb4 <= 0)
			return 124; // B3 only
		all_doubts |= 4;
		return 12; // B3, B4 only
	}

	if (bbs2 < bbs1) {
		all_doubts |= 8;
		return 21;
	}

	if (histiter) {
		if (Ns2 > 3) // all votes for b2 must be in b2 region
		{
			hc = &(hi_beg[bbs2 + 32]);
			hp = hc - 1;
			hn = hc + 1;
			s = (n2 = (*hp).b2) + (n3 = (*hc).b2) + (n4 = (*hn).b2);
			n = Ns2 - (Ns2 + 5) / 6; // allow 1/6 to be outside the 3 boxes
			// if (((i=n-s) > 0) && (i > 3))
			if (n > s) {
				hpp = hp - 1;
				hnn = hn + 1;
				s5 = s + (n1 = (*hpp).b2) + (n5 = (*hnn).b2);
				if ((n > s5) || // 5 boxes don't take all load
						(n1 + n5 >= s) // inside boxes less massive
				)
					b12_s |= 2; // b2 doubtful
			}
		}
		if (Ns1 > 3) // all votes for b1 must be in b1 region
		{
			hc = &(hi_beg[bbs1 + 32]);
			hp = hc - 1;
			hn = hc + 1;
			s = (n2 = (*hp).b1) + (n3 = (*hc).b1) + (n4 = (*hn).b1);
			n = Ns1 - (Ns1 + 5) / 6; // allow 1/6 to be outside the 3 boxes
			// if (((i=n-s) > 0) && (i > 3))
			if (n > s) {
				hpp = hp - 1;
				hnn = hn + 1;
				s5 = s + (n1 = (*hpp).b2) + (n5 = (*hnn).b2);
				if ((n > s5) || // 5 boxes don't take all load
						(n1 + n5 >= s) // inside boxes less massive
				)
					b12_s |= 1; // b1 doubtful
			}
		}

		// many letters above b1; many votes for b2 above b2
		hc = hi_beg + 30;
		hl1 = hi_beg + 30 + bbs1; // b1-2
		hl2 = hi_beg + 30 + bbs2; // b2-2
		for (s = 0; hc < hl1; hc++)
			s += (*hc).all;
		if (s > 3)
			b12_s |= 4;
		for (s = 0; hc < hl2; hc++)
			s += (*hc).b2;
		if (s > 3)
			b12_s |= 8;
	}
	if ((b12_s & 2) == 0)
		b2_solid = 1;
	else
		all_doubts |= 16;
	if (b12_s & 5)
		all_doubts |= 32;
	if (bbs3 > bbs4) {
		all_doubts |= 64;
		return 43;
	}

	if ((8* (bbs2 -bbs1)) < (bbs3-bbs2)) // upper level doubtful
			{	all_doubts |= 128; return 218;}

			Ps=Psf=bbs3-bbs2;
			if (Ps <= 0)
			{
				Ps = 1;
				bbs2 = bbs3-1;
			}
			if (fax1x2) Psf = Ps + 2;

			if (histiter)
			{	int16_t wd;
				if (b12_s & 1) return 11;
				if (b12_s & 2) return 22;
				if (b12_s & 4) return 19;
				if (b12_s & 8) return 29;
				wd = minold-minrow;

				krit_loc = abs(obs2-bbs2+wd) + abs(obs3-bbs3+wd);
				krit_hist += krit_loc;
				krit_loc += abs(obs1-bbs1+wd);
				krit_loc += abs(obs4-bbs4+wd);
				if (krit_hist > 5)
				return 99;
				// if (2*change_vote > cells_inln)
				//    return 8;
			}
			histiter++;
			if (krit_loc) // not stabilized
			{
				obs1=bbs1; obs2=bbs2; obs3=bbs3; obs4=bbs4; oPs=Ps;
				oNb1=Ns1; oNb2=Ns2; oNb3=Ns3; oNb4=Ns4;
				minold = minrow;
				if (histiter > 3) return 88;
			}
			bs_got=1;
			return 0;
		}
