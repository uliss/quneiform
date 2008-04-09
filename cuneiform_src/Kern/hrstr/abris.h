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
#include "nt_types.h"
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
extern BYTE fax1x2;
extern CHAR db_pass;
extern CHAR fbg;
extern INT line_number;

/***** Common variables and functions **********/

ABRIS_EXTERN B_LINES bases;
ABRIS_EXTERN INT wint;   // work integer variable
ABRIS_EXTERN BYTE wbyte;   // work BYTE variable
ABRIS_EXTERN CHAR let_mon[let_monN], mon_fl;
ABRIS_EXTERN INT abriflag, ncells, fullh, midh, midw, fullw, cprob;
ABRIS_EXTERN INT gl_mincol, gl_minrow, gl_maxcol, gl_maxrow;
ABRIS_EXTERN BYTE lmax, lmin1, lmin2, lxmin11, lxmin12, lxmin21, lxmin22,
	    lxmax1, lxmax2, ljmp1, ljmp2, ljmp,
	    leftfl;  // left abris done flag
ABRIS_EXTERN BYTE rmax, rmin1, rmin2, rxmin11, rxmin12, rxmin21, rxmin22,
	    rxmax1, rxmax2, rjmp1, rjmp2, rjmp,
	    rightfl;  // right abris done flag
ABRIS_EXTERN BYTE sfl, swt, sws;
ABRIS_EXTERN BYTE lnmid;
ABRIS_EXTERN BYTE bmax, bmin1, bmin2, bxmin11, bxmin12, bxmin21, bxmin22,
	    bxmax1, bxmax2,
	    botfl;    // bottom abris done flag
ABRIS_EXTERN BYTE tmax, tmin1, tmin2, txmin11, txmin12, txmin21, txmin22,
	    txmax1, txmax2,
	    topfl;    // top abris done flag
ABRIS_EXTERN cell *scl;
ABRIS_EXTERN BYTE letter;
ABRIS_EXTERN INT total_num_black_int;
ABRIS_EXTERN INT total_lth_inter_white;
ABRIS_EXTERN INT total_lth_black;
ABRIS_EXTERN BYTE bold_flag;
ABRIS_EXTERN BYTE num_lines_of_comp;
ABRIS_EXTERN BYTE l_abr[128], r_abr[128],    // left and right abrises
	    top[128], bot[128],       // top and bottom abrises
	    sumh[128],                // sums of black intervals
        linh[128], wid[128],      // numbers of intervals; c_comp. widths
        histo[128];
ABRIS_EXTERN BYTE histo_max_value;

/********** functions from module ABRTOOLS.C are common use *****************/
BYTE    a_rb_corner(INT);
BYTE    rb_corner();
BYTE    round_rb_corner();
BYTE    new_prob( INT penalty );
BYTE    test_bottom_corner( PBYTE abris, INT *size );
INT     Let_bint_index( CHAR Let );
INT     two_int_over_b2 ( cell *curc, INT *loc_2_int);
BYTE    r_tail ( BYTE );
INT     gap_in_side(BYTE from, BYTE to, BYTE code_func, INT tresh, BYTE *flit);
void    get_max_min ( PBYTE func, BYTE from, BYTE to,
              PBYTE fmax, PBYTE fmin, PBYTE x_fmax, PBYTE x_fmin );
INT     make_histo ( BYTE *array, BYTE max_num);
void    make_abris(s_glue *GL, cell *cl);		   //AK. 07.03.97
void    make_wid();
void    make_left_max();
void    make_right_max();
void    make_top_max();
void    make_bot_max();
BYTE    constancy_vally_lth (PBYTE func, BYTE from, BYTE to, BYTE tresh);
BYTE    monotonous_decrease
            (PBYTE func, BYTE from, BYTE to, PBYTE last_decr, BYTE tresh);
BYTE    monotonous_increase
            (PBYTE func, BYTE from, BYTE to, PBYTE last_incr, BYTE tresh);
BYTE    find_2int_zone( BYTE from, BYTE to, PBYTE beg, PBYTE end );
BYTE    find_1int_zone( BYTE from, BYTE to, PBYTE beg, PBYTE end );
INT     top_end_bottom_weights_delta();
void    construct_line_by_two_points( BYTE x1, BYTE y1, BYTE x2, BYTE y2 );
INT     get_x_coord( BYTE y_coord );
BYTE    test_against_convex( PBYTE func, BYTE from, BYTE to );
void    make_inter_white_histo();

/******* functions from module ABRIS.C ********************************/
#ifdef ABRIS_MAIN_MODULE
BYTE s_filt(cell *);
BYTE w_filt();
BYTE x_filt();
BYTE k_filt();
BYTE ss_filt();
BYTE q_filt();
BYTE M_filt();
BYTE R_filt();
BYTE HH_filt();
BYTE a_filt();
BYTE D_filt();
BYTE B_filt();
BYTE A_filt();
BYTE c_filt();
BYTE o_filt(INT);
BYTE d_filt();
BYTE v_filt();
BYTE left_angle_braket_filt();
BYTE right_angle_braket_filt();
BYTE Dig_6_filt();
BYTE E_filt();
BYTE CR_filt();
BYTE yu_filt();
#endif
