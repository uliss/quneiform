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

#include "status.h"
#include "cstr.h"
#include "linear.h"
#include "ligas.h"

#include "bal.h"
#include "minmax.h"

extern int16_t it_done;
static int16_t skew_corr_stat(CSTR_line ln, int16_t pool_src);
static int16_t rast_is_BOX_solid(CSTR_rast B1, int16_t scale);
static Bool snap_monitor_stat(void);
static Bool snap_monitor_ori_stat(CSTR_line *snap_line, int32_t num_lines);//IGOR
static Bool snap_is_marked_stat(CSTR_line ln);//IGOR
static Bool snap_baselines_stat(uchar a);//IGOR
static void snap_draw_line_stat(Handle wnd, Point16 *start, Point16 *end,
		int32_t skew, uint32_t rgb, int16_t pen, uint32_t key);//IGOR
static void snap_del_line_stat(Handle wnd, uint32_t key);//IGOR
static Bool snap_show_text_stat(uchar *txt);
static Bool snap_activity_stat(uchar a);

static uchar let_linempty[512] = { 0 };

int16_t (*RSTR_skew_corr)(CSTR_line ln, int16_t pool_src) = skew_corr_stat;
int16_t (*RSTR_rast_is_BOX_solid)(CSTR_rast B1, int16_t scale) =
		rast_is_BOX_solid;
Bool (*snap_monitor_rbal)(void) = snap_monitor_stat;
Bool (*snap_show_text_rbal)(uchar *txt) = snap_show_text_stat;
Bool (*snap_activity_rbal)(uchar a) = snap_activity_stat;
Bool (*snap_monitor_ori_rbal)(CSTR_line *snap_line, int32_t num_lines) =
		snap_monitor_ori_stat; //IGOR
Bool (*snap_is_marked_rbal)(CSTR_line ln) = snap_is_marked_stat;//IGOR
Bool (*snap_baselines_rbal)(uchar a) = snap_baselines_stat;//IGOR
void (*snap_draw_line_rbal)(Handle wnd, Point16 *start, Point16 *end,
		int32_t skew, uint32_t rgb, int16_t pen, uint32_t key) =
		snap_draw_line_stat;//IGOR
void (*snap_del_line_rbal)(Handle wnd, uint32_t key) = snap_del_line_stat;//IGOR

CSTR_line lin_str = (CSTR_line) NULL;

uchar language = 3;
int16_t line_number = 0;

uchar fax1x2 = 0;
uint16_t actual_resolution = 300;
uchar fEdCode = 0; // ASCII
Bool line_BL = FALSE;
uchar spec_camera = 0;
int32_t nIncline = 0;
uchar no_linpen = 0;
int16_t prop_l_delta = 0, prop_r_delta = 0;

uchar db_status = 0;
uchar db_pass = 0;

uchar *let_lindef3 = &let_linempty[0];
uchar *let_lindef = &let_linempty[0];
uchar *let_linpos = &let_linempty[0];
uchar *let_linshape = &let_linempty[0];
uchar *let_sans_acc = &let_linempty[0]; // [257]
uchar *letters_pidx_table = &let_linempty[0]; // 512
uchar decode_ASCII_to_[256][4] = { 0 };

///////////////////////////////
CSTR_rast cell_f() {
	if (!lin_str)
		return NULL;

	return CSTR_GetFirstRaster(lin_str);
}
///////////
CSTR_rast cell_l() {
	if (!lin_str)
		return NULL;

	return CSTR_GetLastRaster(lin_str);
}
///////////

////  Tools.c
void sort_vers(CSTR_rast c) {
	// version *v,*vs,*vm;
	UniAlt tmp;
	UniVersions vers;
	int i, j;

	CSTR_GetCollectionUni(c, &vers);
	if (vers.lnAltCnt <= 0) {
		set_bad_cell(c);
		return;
	}

	for (i = 0; i < vers.lnAltCnt; i++) {
		for (j = i + 1; j < vers.lnAltCnt; j++) {
			if (vers.Alt[j].Prob > vers.Alt[i].Prob)
				break;
		}

		if (j >= vers.lnAltCnt)
			continue;

		tmp = vers.Alt[i];
		vers.Alt[i] = vers.Alt[j];
		vers.Alt[j] = tmp;
		i--;
	}

	for (i = vers.lnAltCnt - 1; i >= 0; i--) {
		if (vers.Alt[i].Prob <= 0)
			vers.lnAltCnt--;
	}

	CSTR_StoreCollectionUni(c, &vers);

	if (vers.lnAltCnt <= 0)
		set_bad_cell(c);
}
/////////////
// Tools.c
void set_bad_cell(CSTR_rast c) {
	CSTR_rast_attr attr;
	UniVersions vers = { 0 };

	CSTR_GetAttr(c, &attr);

	// c->nvers=0;
	attr.recsource = 0;
	attr.RecogHistory = 0;
	// attr.bottom_accent = 0;  // need not in CSTR ?
	attr.accent_leader = 0;

	if (attr.flg & (CSTR_f_let | CSTR_f_bad))
		attr.flg = CSTR_f_bad;

	CSTR_SetAttr(c, &attr);

	vers.lnAltMax = REC_MAX_VERS;
	CSTR_StoreCollectionUni(c, &vers);
}
////////////////////////////////////////////
//////////
// dm2.c
void glsnap(char I, CSTR_rast C, char *txt) {

	if (!db_status)
		return;

	// snap_newcell(C);
	if (snap_activity_rbal(I)) {
		snap_show_text_rbal(txt);
		snap_monitor_rbal();
	}
}

// skew.c
void ideal_rc(CSTR_rast c) {
	CSTR_rast_attr attr;

	CSTR_GetAttr(c, &attr);

	attr.row = attr.r_row - (int16_t)((int32_t) nIncline * attr.r_col / 2048);
	attr.col = attr.r_col + (int16_t)((int32_t) nIncline * attr.r_row / 2048);

	CSTR_SetAttr(c, &attr);
}
///////////////////

///////////////////////////
// dm1.c
void promote(uchar sn, CSTR_rast cl, uchar let, int16_t delta) {
	uchar wl, wl_sacc, let_sacc;
	// version *vp1, *vp2;
	UniVersions vers;
	int i, j;
	uchar p1, pw;
	int16_t pwi, dlt, dlv, fld;

	CSTR_GetCollectionUni(cl, &vers);
	if (vers.lnAltCnt <= 0)
		return;

	let_sacc = let_sans_acc[let];

	dlt = delta;
	fld = 0;
	if (dlt > 512) {
		fld = 1;
		dlv = dlt = dlt - 512;
	} // absolute value (not a promotion)

	// for (vp2=vp1=cl->vers; (((wl=vp1->let) != 0) && ((p1=vp1->prob) != 0)); vp1++)
	for (i = j = 0; i < vers.lnAltCnt; i++) {
		wl = vers.Alt[i].Liga;
		p1 = vers.Alt[i].Prob;

		wl_sacc = let_sans_acc[wl];

		if (wl_sacc == let_sacc) {
			if (delta > 0) {
				if (i == j)
					dlt = 8;

				if (fld) // make known value (not less than present)
				{
					pw = vers.Alt[i].Prob; // vp1->prob;
					pwi = pw;
					if (pwi < dlv)
						pwi = dlv;
				} else // make the version to be the 1st
				{
					pw = vers.Alt[j].Prob; //vp2->prob;
					pwi = pw;
					pwi += dlt;
				}

				if (pwi > 254)
					pwi = 254;

				// give (max+delta) to promoted version
				//vp1->prob=(uchar)pwi;
				vers.Alt[i].Prob = (uchar) pwi;
				CSTR_StoreCollectionUni(cl, &vers);

				sort_vers(cl);

				if (sn)
					glsnap(sn, cl, "promoted ");
				return;
			} else {
				pwi = p1;
				pwi += delta;
				if (pwi <= 2)
					pwi = 2;
				//vp1->prob=(uchar)pwi;
				vers.Alt[i].Prob = (uchar) pwi;
				CSTR_StoreCollectionUni(cl, &vers);

				sort_vers(cl);

				if (sn)
					glsnap(sn, cl, "monused ");
				return;
			}
		} //  IF letter found
	} // FOR versions

	if (delta < 0)
		return;

	// add version
	if (vers.lnAltCnt >= REC_MAX_VERS)
		i = REC_MAX_VERS - 1;
	else
		i = vers.lnAltCnt;

	memset(&vers.Alt[i], 0, sizeof(UniAlt));
	//vp1->let=let;
	vers.Alt[i].Liga = let;
	strcpy(vers.Alt[i].Code, decode_ASCII_to_[let]);

	pw = vers.Alt[j].Prob;
	pwi = pw;

	if (fld) {
		if (pwi < dlv)
			pwi = dlv;
	} else
		pwi += dlt;

	if (pwi > 254)
		pwi = 254;

	// vp1->prob=(uchar)pwi;
	vers.Alt[i].Prob = (uchar) pwi;
	if (vers.lnAltCnt < REC_MAX_VERS)
		vers.lnAltCnt++;

	CSTR_StoreCollectionUni(cl, &vers);

	sort_vers(cl);

	if (sn)
		glsnap((char) (sn > 'a' ? sn : 'a'), cl, "insvers");
}

////////////////////////////
// skew.c
static int16_t skew_corr_stat(CSTR_line ln, int16_t pool_src) {
	return (int16_t) nIncline;
}
////////////
/////////////////////
// dms.c
// сделать B1->env   (и для CSTR_rast -> comp )
static int16_t rast_is_BOX_solid(CSTR_rast B1, int16_t scale) {
	return 0;
}
//////////

static Bool32 CopyRasterToLine(CSTR_rast c, CSTR_line trg) {
	CSTR_rast_attr attr;
	RecRaster rs;
	UniVersions vr;
	CCOM_comp * comp;
	CSTR_rast cnew;

	if (!c || !trg)
		return FALSE;

	if (CSTR_GetAttr(c, &attr) && CSTR_GetImage(c, (uchar *) &rs,
			CSTR_TYPE_IMAGE_RS) && CSTR_GetCollectionUni(c, &vr) && (comp
			= CSTR_GetComp(c)) != NULL) {
		if (!(cnew = CSTR_NewRaster(trg, attr.col, attr.row, attr.w)))
			return FALSE;
		if (!CSTR_SetAttr(cnew, &attr))
			return FALSE;
		if (!CSTR_StoreRaster(cnew, &rs))
			return FALSE;
		if (!CSTR_StoreCollectionUni(cnew, &vr))
			return FALSE;
		if (!CSTR_StoreScale(cnew, comp->scale))
			return FALSE;

		return TRUE;
	}

	return FALSE;
}
////////////////////////////
// pass3.c
#define PROPMAX   25
void dust_ini(CSTR_line lin) {
	CSTR_rast_attr cur;
	CSTR_attr attr;
	CSTR_rast curr, last;
	CCOM_comp * cmp;
	int16_t cmpscale;

	CSTR_GetLineAttr(lin, &attr);

	// dust cycle
	curr = CSTR_GetFirstRaster(lin);
	last = CSTR_GetLastRaster(lin);
	cmpscale = 0* ( (attr.Flags & CSTR_STR_SCALED )!=0);

	for (curr=CSTR_GetNextRaster (curr, CSTR_f_all); curr&&curr!=last; curr=CSTR_GetNextRaster (curr, CSTR_f_all))
 {
  CSTR_GetAttr(curr,&cur);
  cmp=CSTR_GetComp(curr);

  if( !(cur.flg&CSTR_f_dust) )
        continue;

  if( cmp->large & CCOM_LR_TAKEN )
          continue;
  if( (cmp->type&CCOM_CH_GREAT) ||cmp->scale>2-1*0 )
      continue;
  if( (cur.w>>cmpscale) > RASTER_MAX_WIDTH ||  //???
      (cur.h>>cmpscale) > RASTER_MAX_HEIGHT )
      continue;
  if( cur.h>PROPMAX*cur.w)
          continue;

  CopyRasterToLine(curr, lin_str);
 }

/*  for FUTURE...

  c1=cell_f();
  while ((c1=c1->nextl)->nextl != NULL)
   ij_test(c1);  // do delete/confirm i/j
*/

 return;
}
	///////////////////////

Bool snap_monitor_stat(void) {
	return FALSE;
}
///////
Bool snap_monitor_ori_stat(CSTR_line *snap_line, int32_t num_lines)//IGOR
{
	return FALSE;
}
///////
Bool snap_is_marked_stat(CSTR_line ln)//IGOR
{
	return FALSE;
}
///////
Bool snap_baselines_stat(uchar a)//IGOR
{
	return FALSE;
}
///////
void snap_draw_line_stat(Handle wnd, Point16 *start, Point16 *end,
		int32_t skew, uint32_t rgb, int16_t pen, uint32_t key)//IGOR
{
	return;
}
///////
void snap_del_line_stat(Handle wnd, uint32_t key)//IGOR
{
	return;
}
///////
Bool snap_show_text_stat(uchar *txt) {
	return FALSE;
}
///////
Bool snap_activity_stat(uchar a) {
	return FALSE;
}
///////
/*
 Bool snap_newcell (CSTR_rast c)
 {
 return FALSE;
 }
 */
/////////////////
// old - если arg==0 - добавить мусор из строки lin_str
// проверить/изменить метки мусор-нет
static void test_dust(void) // int16_t arg)
{
	/*
	 if (arg==0)
	 {
	 dust_ini(lin_str);
	 glsnap('d',cell_f()->next,"dust initted");
	 dust_in = 1;
	 }
	 */

	all_cell_ledust();
	glsnap('d', cell_f()->next, "letters to dust");
	dust_to_let();
	glsnap('d', cell_f()->next, "dust to letters");
	discrim_by_dust();
	glsnap('d', cell_f()->next, "discrim by dust");
}

//////////////
// черновое определение БЛ
//
// Для новой	 версии :
// сначала вход - ln,0
// выход
// Добавление дуста снаружи
// Вход ln, 1
// Вход ln, 2
//
static int basedraft(CSTR_line ln, uchar isDust) {
	// char riter1, riter0;
	int riter1;
	int16_t i;
	CSTR_rast C;

	lin_str = ln;

	if (!isDust) // начальный проход
	{
		//if (line_number == 0)
		if (line_number <= 1)
			oldPs2 = -1;
		else {
			oldPs2 = bbs3 - bbs2;
			oldPs1 = bbs3 - bbs1;
			oldPs4 = bbs3 - bbs4;
			oldn1 = Nb1;
			oldn2 = Nb2;
			oldn4 = Nb4;
			oldmult = multi_bas;
		}

		it_done = 0;
	}

	lin_pass = 1;

	dust_in = isDust ? 1 : 0;

	//  riter0=2;

	if (isDust > 1)
		goto mainCircle;

	if (isDust) // второй проход - уже с дустом
	{
		if ((forbiset) && (db_status))
			li_snap("forbid=");
		trans_total = forbiset;
		hist_done = 0;

		//  old version - add dust from lin_str (riter1==0),
		//   now - only check dust-letter
		test_dust();

		//   if (cell_f()->nextl->next == NULL)
		if (!CSTR_GetNextRaster(cell_f(), f_letter))
			goto drfin; // line emptied

		// найти-уточнить наклон nIncline ; второй параметр 1 - если по cell!
		nIncline = RSTR_skew_corr(ln, 2);

		C = cell_f();
		//   while (C && C->next && (C=C->next)->next != NULL)            //AK add c/g 06.03.98
		for (C = CSTR_GetNext(C); C; C = CSTR_GetNext(C))
			ideal_rc(C);

		if (!trans_total)
			goto drfin;
		//	   break;
		//   if (riter0)
		//	   goto again_for_dust;
	}

	//again_for_dust:

	//  riter0--;
	let_to_dust = 0;
	diff_curv = 0;
	change_vote = 0;
	hist_done = 0;
	oNb1 = oNb2 = oNb3 = oNb4 = 0;
	cells_inln = 0;

	// все обнулить
	gen_reset();

	trans_total = 0;
	fl_artifact = 0;
	ntot_old = ncut_old = nmult_old = ndead_old = nvers_old = 0;

	// сбор статистики, поиск БЛ
	defbas(0);

	bsdust_ps = Ps;
	sum_letlth = sum_cellth = 0;
	ncut_vers = ncut_cells = ntot_cells = 0;

	i = bbs2 - (bbs3 - bbs2) / 2;
	bsdust_upper = MIN(i, bbs1) - 2 + minrow;

	if (language != LANG_ENGLISH)
		bsdust_upper -= (MAX(2, (bbs3 - bbs2) / 7));
	bsdust_lower = bbs4 + minrow;

	if (db_status) {
		char ww[128];
		sprintf(ww, "for dust: Ps=%d upper=%d lower=%d", bsdust_ps,
				bsdust_upper, bsdust_lower);
		glsnap('d', cell_f()->next, ww);
	}

	if (!isDust) {
		if ((forbiset) && (db_status))
			li_snap("forbid=");
		trans_total = forbiset;
		hist_done = 0;
	}

	return 0; // вернемся за дустом -
	// какие-то БЛ уже есть, можно точнее
	// определить точки i, j  и т.п.


	mainCircle: for (riter1 = 0; riter1 < 3; riter1++) {
		if ((forbiset) && (db_status))
			li_snap("forbid=");
		trans_total = forbiset;
		hist_done = 0;

		//  old version - add dust from lin_str (riter1==0),
		//   now - only check dust-letter
		test_dust();

		//   if (cell_f()->nextl->next == NULL)
		if (!CSTR_GetNextRaster(cell_f(), f_letter))
			goto drfin; // line emptied


		// найти-уточнить наклон nIncline
		nIncline = RSTR_skew_corr(ln, 2);

		C = cell_f();
		//   while (C && C->next && (C=C->next)->next != NULL)            //AK add c/g 06.03.98
		for (C = CSTR_GetNext(C); C; C = CSTR_GetNext(C))
			ideal_rc(C);

		if (!trans_total)
			break;
		//   if (riter0)
		//	   goto again_for_dust;

		if (fl_artifact > 1)
			break;

		// уточнить БЛ
		defbas(0);
	}

	drfin: if (db_status)
		li_snap("draft fin");
	bbs0 = min_crow;

	return 1;
}
//////////////////////
static void GetRstrGlobals(BAL_RSTR_GLOBALS *rstrGlob) {
	language = (uchar) rstrGlob->language;
	line_number = (int16_t) rstrGlob->line_number;
	fax1x2 = (uchar) rstrGlob->fax1x2;
	actual_resolution = (uint16_t) rstrGlob->actual_resolution;
	fEdCode = (uchar) rstrGlob->fEdCode; // ASCII
	line_BL = (int16_t) rstrGlob->line_BL;
	spec_camera = (uchar) rstrGlob->spec_camera;
	no_linpen = (uchar) rstrGlob->no_linpen;

	prop_l_delta = rstrGlob->prop_l_delta;
	prop_r_delta = rstrGlob->prop_r_delta;

	let_lindef3 = rstrGlob->let_lindef3;
	let_lindef = rstrGlob->let_lindef;
	let_linpos = rstrGlob->let_linpos;
	let_linshape = rstrGlob->let_linshape;
	let_sans_acc = rstrGlob->let_sans_acc; // [257]
	letters_pidx_table = rstrGlob->letters_pidx_table;
	// 512

	memcpy(&decode_ASCII_to_[0][0], rstrGlob->decode_ASCII_to_, 256* 4 );
	// [256][4]

	RSTR_skew_corr = rstrGlob->skew_corr;
	RSTR_rast_is_BOX_solid = rstrGlob->rast_is_BOX_solid;

	db_status = rstrGlob->db_status;
	db_pass = rstrGlob->db_pass;
	snap_monitor_rbal = rstrGlob->snap_monitor;
	snap_activity_rbal = rstrGlob->snap_activity;
	snap_show_text_rbal= rstrGlob->snap_show_text;
	snap_monitor_ori_rbal= rstrGlob->snap_monitor_ori;//IGOR
	snap_is_marked_rbal= rstrGlob->snap_is_marked; //IGOR
	snap_baselines_rbal= rstrGlob->snap_baselines; //IGOR
	snap_draw_line_rbal= rstrGlob->snap_draw_line; //IGOR
	snap_del_line_rbal = rstrGlob->snap_del_line; //IGOR
}
//////////////////////
static void GetBalGlobals(BAL_INOUT_GLOBALS *balGlob) {
	all_caps = balGlob->all_caps;
	all_diffs_made = balGlob->all_diffs_made;
	bbs0 = balGlob->bbs0;
	bbs1 = balGlob->bbs1;
	bbs2 = balGlob->bbs2;
	bbs3 = balGlob->bbs3;
	bbs4 = balGlob->bbs4;
	bbsm = balGlob->bbsm;

	bs_got = balGlob->bs_got;
	bsdust_lower = balGlob->bsdust_lower;
	bsdust_ps = balGlob->bsdust_ps;
	bsdust_upper = balGlob->bsdust_upper;
	let_to_dust = balGlob->let_to_dust;
	maxcol = balGlob->maxcol;
	mincol = balGlob->mincol;
	maxrow = balGlob->maxrow;
	minrow = balGlob->minrow;
	multi_bas = balGlob->multi_bas;

	Nb1 = balGlob->Nb1;
	Nb2 = balGlob->Nb2;
	Nb3 = balGlob->Nb3;
	Nb4 = balGlob->Nb4;

	Ns1 = balGlob->Ns1;
	Ns2 = balGlob->Ns2;
	Ps = balGlob->Ps;
	Psf = balGlob->Psf;

	nIncline = balGlob->nIncline;
}
/////////////////////
static void SetBalGlobals(BAL_INOUT_GLOBALS *balGlob) {
	balGlob->all_caps = all_caps;
	balGlob->all_diffs_made = all_diffs_made;
	balGlob->bbs0 = bbs0;
	balGlob->bbs1 = bbs1;
	balGlob->bbs2 = bbs2;
	balGlob->bbs3 = bbs3;
	balGlob->bbs4 = bbs4;
	balGlob->bbsm = bbsm;

	balGlob->bs_got = bs_got;
	balGlob->bsdust_lower = bsdust_lower;
	balGlob->bsdust_ps = bsdust_ps;
	balGlob->bsdust_upper = bsdust_upper;
	balGlob->let_to_dust = let_to_dust;
	balGlob->maxcol = maxcol;
	balGlob->mincol = mincol;
	balGlob->maxrow = maxrow;
	balGlob->minrow = minrow;
	balGlob->multi_bas = multi_bas;

	balGlob->Nb1 = Nb1;
	balGlob->Nb2 = Nb2;
	balGlob->Nb3 = Nb3;
	balGlob->Nb4 = Nb4;

	balGlob->Ns1 = Ns1;
	balGlob->Ns2 = Ns2;
	balGlob->Ps = Ps;
	balGlob->Psf = Psf;

	balGlob->nIncline = nIncline;
}
//////////////////////
// now - 32 bases
static void SetBalBases(BAL_bas_ln *bal_bases) {
	int i;

	for (i = 0; i < 32; i++) {
		bal_bases[i].b1 = all_bases[i].b1;
		bal_bases[i].b2 = all_bases[i].b2;
		bal_bases[i].b3 = all_bases[i].b3;
		bal_bases[i].b4 = all_bases[i].b4;

		bal_bases[i].n1 = all_bases[i].n1;
		bal_bases[i].n2 = all_bases[i].n2;
		bal_bases[i].n3 = all_bases[i].n3;
		bal_bases[i].n4 = all_bases[i].n4;

		bal_bases[i].fl_def = all_bases[i].fl_def;

		bal_bases[i].startcol = all_bases[i].startcol;
		bal_bases[i].endcol = all_bases[i].endcol;
	}
}
/////////////////////
BAL_FUNC(Bool32) BAL_basedraft(CSTR_line ln, uchar isDust,
		BAL_RSTR_GLOBALS *rstrGlob,
		BAL_INOUT_GLOBALS *balGlob,
		BAL_bas_ln *bal_bases)
{
	int ret;

	lin_str = ln;

	GetRstrGlobals(rstrGlob);
	GetBalGlobals(balGlob);

	ret = basedraft(ln, isDust);

	SetBalGlobals(balGlob);

	SetBalBases(bal_bases); // don't need for draft ?

	return ret?TRUE:FALSE;
}
///////////////
BAL_FUNC(Bool32) BAL_linpos(CSTR_line ln,
		BAL_RSTR_GLOBALS *rstrGlob,
		BAL_INOUT_GLOBALS *balGlob,
		BAL_bas_ln *bal_bases)
{
	lin_str = ln;

	GetRstrGlobals(rstrGlob);
	GetBalGlobals(balGlob);

	linpos(ln);

	SetBalGlobals(balGlob);

	SetBalBases(bal_bases);

	return TRUE;
}
///////////////
BAL_FUNC(Bool32) BAL_basefin(CSTR_line ln,
		BAL_RSTR_GLOBALS *rstrGlob,
		BAL_INOUT_GLOBALS *balGlob,
		BAL_bas_ln *bal_bases )
{
	lin_str = ln;

	GetRstrGlobals(rstrGlob);
	GetBalGlobals(balGlob);

	basefin( ln );

	SetBalGlobals(balGlob);

	SetBalBases(bal_bases);

	return TRUE;
}
///////////////
BAL_FUNC(Bool32) BAL_StatInit(void)
{
	void BaseLineStatisticInit(void);

	BaseLineStatisticInit();

	return TRUE;
};
/////////////////////////
