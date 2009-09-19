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

/*********************************************************************/
/*                                                                   */
/*     module ABRIS.H -   title module for abris.c and abrtools.c    */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>

#include "struct.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"
#include "ligas.h"

#ifdef ABRIS_MAIN_MODULE
# define ABRIS_EXTERN
#else
# define ABRIS_EXTERN extern
#endif

/*######## Constants Defenitions : ############*/

#define let_monN 8
#define PNL_BINT_INDX  6
#define c_rn_bint_idx  8

#define LEFT_S    1
#define RIGHT_S   2
#define TOP_S     3
#define BOTTOM_S  4

#define PNL_FOR_CONCAVE_BC  24    // Penalty for "anti" round
//  abris at bottom corner
#define PNL_FOR_CONVEX_BC  24     // Penalty for round
//  abris at bottom corner
#define PNL_FOR_SERIF_BC  10      // Penalty for serifical
//  abris at bottom corner
#define ROUND_SIDE_PNL 80
#define FUNNEL_PNL 40
#define GAP_IN_SIDE_PNL 20
#define CONST_VALLY_PNL 20

#define BC_SERIF    1
#define BC_CONCAVE  2
#define BC_CONVEX   3
#define BC_STRAIGHT_ANGLE 4

#define SMOOTH_FUNC         0
#define NOT_FOUND_MON_DECR  1
#define FOUND_MON_DECR      2
#define NOT_FOUND_MON_INCR  1
#define FOUND_MON_INCR      2

#define NOT_FOUND_ONE_INT_ZONE  3
#define FOUND_ONE_INT_ZONE      2
#define FOUND_TWO_INT_ZONE      1
#define NOT_FOUND_TWO_INT_ZONE  0

#define BOT_MIN_LEFT_S 2
#define TOP_MIN_LEFT_S 3
#define BOT_MIN_RGHT_S 3
#define TOP_MIN_RGHT_S 3

#define LIGHT_COMP       1
#define SEMI_BOLD_COMP   2
#define BOLD_COMP        3

#define MEDIUM_LETTER_bdh      20
#define TALL_LETTER_bdh        38
#define DISTANCED_BASE_LINES   12

/****** Global Variables: *************/
extern uchar fax1x2;
extern char db_pass;
extern char fbg;
extern int16_t line_number;
/***** Common variables and functions **********/
ABRIS_EXTERN B_LINES bases;
ABRIS_EXTERN int16_t wint; // work integer variable
ABRIS_EXTERN uchar wbyte; // work uchar variable
ABRIS_EXTERN char let_mon[let_monN], mon_fl;
ABRIS_EXTERN int16_t abriflag, ncells, fullh, midh, midw, fullw, cprob;
ABRIS_EXTERN int16_t gl_mincol, gl_minrow, gl_maxcol, gl_maxrow;
ABRIS_EXTERN uchar lmax, lmin1, lmin2, lxmin11, lxmin12, lxmin21, lxmin22,
		lxmax1, lxmax2, ljmp1, ljmp2, ljmp, leftfl; // left abris done flag
ABRIS_EXTERN uchar rmax, rmin1, rmin2, rxmin11, rxmin12, rxmin21, rxmin22,
		rxmax1, rxmax2, rjmp1, rjmp2, rjmp, rightfl; // right abris done flag
ABRIS_EXTERN uchar sfl, swt, sws;
ABRIS_EXTERN uchar lnmid;
ABRIS_EXTERN uchar bmax, bmin1, bmin2, bxmin11, bxmin12, bxmin21, bxmin22,
		bxmax1, bxmax2, botfl; // bottom abris done flag
ABRIS_EXTERN uchar tmax, tmin1, tmin2, txmin11, txmin12, txmin21, txmin22,
		txmax1, txmax2, topfl; // top abris done flag
ABRIS_EXTERN cell *scl;
ABRIS_EXTERN uchar letter;
ABRIS_EXTERN int16_t total_num_black_int;
ABRIS_EXTERN int16_t total_lth_inter_white;
ABRIS_EXTERN int16_t total_lth_black;
ABRIS_EXTERN uchar bold_flag;
ABRIS_EXTERN uchar num_lines_of_comp;
ABRIS_EXTERN uchar l_abr[128], r_abr[128], // left and right abrises
		top[128], bot[128], // top and bottom abrises
		sumh[128], // sums of black intervals
		linh[128], wid[128], // numbers of intervals; c_comp. widths
		histo[128];
ABRIS_EXTERN uchar histo_max_value;

/********** functions from module ABRTOOLS.C are common use *****************/
uchar a_rb_corner(int16_t);
uchar rb_corner();
uchar round_rb_corner();
uchar new_prob(int16_t penalty);
uchar test_bottom_corner(puchar abris, int16_t *size);
int16_t Let_bint_index(char Let);
int16_t two_int_over_b2(cell *curc, int16_t *loc_2_int);
uchar r_tail(uchar);
int16_t gap_in_side(uchar from, uchar to, uchar code_func, int16_t tresh, uchar *flit);
void get_max_min(puchar func, uchar from, uchar to, puchar fmax, puchar fmin,
		puchar x_fmax, puchar x_fmin);
int16_t make_histo(uchar *array, uchar max_num);
void make_abris(s_glue *GL, cell *cl); //AK. 07.03.97
void make_wid();
void make_left_max();
void make_right_max();
void make_top_max();
void make_bot_max();
uchar constancy_vally_lth(puchar func, uchar from, uchar to, uchar tresh);
uchar monotonous_decrease(puchar func, uchar from, uchar to, puchar last_decr,
		uchar tresh);
uchar monotonous_increase(puchar func, uchar from, uchar to, puchar last_incr,
		uchar tresh);
uchar find_2int_zone(uchar from, uchar to, puchar beg, puchar end);
uchar find_1int_zone(uchar from, uchar to, puchar beg, puchar end);
int16_t top_end_bottom_weights_delta();
void construct_line_by_two_points(uchar x1, uchar y1, uchar x2, uchar y2);
int16_t get_x_coord(uchar y_coord);
uchar test_against_convex(puchar func, uchar from, uchar to);
void make_inter_white_histo();

/******* functions from module ABRIS.C ********************************/
#ifdef ABRIS_MAIN_MODULE
uchar s_filt(cell *);
uchar w_filt();
uchar x_filt();
uchar k_filt();
uchar ss_filt();
uchar q_filt();
uchar M_filt();
uchar R_filt();
uchar HH_filt();
uchar a_filt();
uchar D_filt();
uchar B_filt();
uchar A_filt();
uchar c_filt();
uchar o_filt(int16_t);
uchar d_filt();
uchar v_filt();
uchar left_angle_braket_filt();
uchar right_angle_braket_filt();
uchar Dig_6_filt();
uchar E_filt();
uchar CR_filt();
uchar yu_filt();
#endif
