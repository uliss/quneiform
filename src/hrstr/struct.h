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

#ifndef H_struct_h
#define H_struct_h

#include "tuner.h"
#include "cttypes.h"

#ifdef  HUGE_IMAGE
#define WORLD_MAX_HEIGHT            10000
#define WORLD_MAX_WIDTH             10000    //7000
#define WORLD_MAX_RESOLUTION    800
#else
#define WORLD_MAX_HEIGHT            4096  // ordinary image
#define WORLD_MAX_WIDTH             4096
#define WORLD_MAX_RESOLUTION    400
#endif

#define LPOOL_SIZE                  8000
#define RASTER_MAX_HEIGHT               63
#define RASTER_MAX_WIDTH                128
#define SMALL_SIZE                          8
// AK! Atention!!!!//////////////////////////////////////////////////////
//  имеет смысл при необходимости отловить глюки так как падает при попытке
//  принять dust за letter
#define err_pnlet(c)       ((c))->nextl=((c))->prevl=(cell*)(0xffff0000)//
#define err_pnnextlet(c)   ((c))->nextl=             (cell*)(0xffff0000)//
#define err_pnprevlet(c)   ((c))->prevl=             (cell*)(0xffff0000)//
#define err_pncell(c)      ((c))->next =((c))->prev =(cell*)(0xffff0000)//
#define err_pnnextcell(c)  ((c))->next =             (cell*)(0xffff0000)//
#define err_pnprevcell(c)  ((c))->prev =             (cell*)(0xffff0000)//
// AK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#define err_pnlet(c)  ((c))->nextl=((c))->prevl=(cell*)(0xffff0000);
///////////////////////////////////////////////////////////////////////AK
// AL 900318
//-------------------- conectivity component ---------------------

struct mn_struc {
	void *mnfirstbox; // address of the first box
	int16_t mncounter; // (was int16_t) number of living lines in the component
#define mnfree  mnfirstbox      // reference to next free main number
	int16_t mnupper; // upper bound of component
	int16_t mnlower; // lower bound of component
	int16_t mnboxcnt; // number of boxes in component
#define usual_box_count 20      // heuristic of number of lines in a letter
#define great_box_count 200     // heuristic for number of boxes in a picture
	uchar mnlines; // number of lines in the component
	uchar mnbegs; // number of free line begins
	uchar mnends; // number of free line ends
	uchar mnflag; // flag byte for main number
#define mnpicture 1             // component is a picture
	struct mn_struc *mnnext; // address of next dead component
};
typedef struct mn_struc MN;

//------------------- The box has a header ----------------------

struct box_struct {
	struct box_struct *boxnext; // chain address (zero if no next box)
	MN * boxmain; // component main number pointer
	uint16_t boxptr; // ptr to the empty place in the box
	int16_t boxleft; // left boundary for line envelope
	int16_t boxright; // right boundary for line envelope
	int16_t boxey; // row of line end+1 ( if line ends within
	//    box)
	int16_t boxel; // length of the last segment (if line ends
	//    within box)
	int16_t boxex; // coordinate of last segment end (if line
	//    ends within box)
	uchar boxflag; // byte for box attributes flags
	uchar boxwf; // working flag (for picture compress)
	uint16_t boxresw; // reserved word (for *4 arround)
};
typedef struct box_struct BOX;

// Values of boxflag:

#define BOXMARK         1       // flag for temporary box marking
#define BOXPICTURE      2       // the picture box - only header in the box is true
#define BOXFREEBEG      0x30    // line start type mask
#define BOXFREEEND      0xc0    // line end type mask
//      The 'boxfreebeg' and 'boxfreeend' bites has following meaning:
//      '11' - free begin/end of line
//      '01' - not free begin/end
//      '00' - no information about begin/end in this box
#define BOXBEG          0x10    // line start not free
#define BOXEND          0x40    // line end not free
// BOX size

#define BOXHLTH (sizeof(BOX))   // the box header length
#define BOXSIZE (BOXHLTH+32*4)  // the length of box
#define BOXBOUNDARY (BOXSIZE-6) // the boundary control
struct ldescr_struct {
	int16_t y; // y coord. of the first interval
	int16_t l; // first interval length
	int16_t x; // x coord. of the end of the first interval
};
typedef struct ldescr_struct LNSTRT;

struct box_interval {
	int16_t l; // length of interval
	int16_t d; // displacement of the end of the current interval relative
}; //      to the previous
typedef struct box_interval BOXINT;

//-------------------- component in kit --------------------------

struct comp_struc {
	uint16_t size; // size of component in paragraphs >=3
	int16_t upper; // upper boundary of component
	int16_t left; // left boundary of component
	int16_t h; // height of component
	int16_t w; // width of component
	uchar rw; // raster width in bytes
	uchar type; // recognition type
#define ch_perfect    1     // perfect type defined
#define ch_letter     2     // letter type
#define ch_dust       4     // dust type
#define ch_punct      8     // punctuation
#define ch_stick     16     // vertical bar component
#define ch_great     32     // great component
#define ch_merge     64     // merged components
#define ch_notltr   128     // not letter or punctuation
	uchar cs; // recognition case (see bellow)
	uchar pidx; // proportional index (ref.)
	int16_t nvers; // number of alternative versions
	int16_t records; // recognition records offset
	int16_t lines; // ptr to line representation
	int16_t nl; // number of lines
	uchar begs; // number of free begins
	uchar ends; // number of free ends
	uchar reasno; // proportional criteria messages
	uchar large; // large type
#define ch_underlined   1       // component was underlined
#define ch_taken        2       // taken to line at dust_ini
	uchar scale; // scale of the component
	// union {
	// uchar stairs[4];                // bit_map for stairs_vector components
	struct comp_struc * next;
	//       };
};
typedef struct comp_struc c_comp;

//----------------------- cell ----------------------------------

#define VERS_IN_CELL 16 // Vald. 06-04-94
struct vers_struct // versions with probabilities
{
	uchar let; // character
	uchar prob; // probability
};
typedef struct vers_struct version;
//----------------------- stick -------------------------------

struct stick_struct {
	char x, y, l, w, top, bot;
	int16_t incl;
};
typedef struct stick_struct STICK;

struct cell_struc {
	int16_t row; // ideal row of cell
	int16_t col; // ideal collumn of cell
	int16_t h; // height of cell
	int16_t w; // width of cell
	// 8
	c_comp *env; // envelope and line representation ptr
	struct cell_struc *next; // next in all string
	struct cell_struc *prev; // prev in all string
	struct cell_struc *nextl; // next letter ( only for letters )
	struct cell_struc *prevl; // prev letter ( only for letters )
	// 28
	char bdiff; // local base line corrective displacement
	uchar difflg; // local correction flg
	// baseline defined by cell:
#define c_db_b1         1
#define c_db_b2         2
#define c_db_b3         4
#define c_db_b4         8
#define c_db_down   16     // base corrected 1 pixel down
#define c_db_up     32     // base corrected 1 pixel up
#define c_db_forbid 64     // don't take to calculate bases (abnormal pos)
#define c_db_solid  128    // BOX_solid letter not penalized
	uchar basflg;
#define c_bs_b1a    1      // agrees to be at  b1
#define c_bs_b2a    2      //                  b2
#define c_bs_bt     4      //                  't' level
#define c_bs_t      8      // 't' only  (letters)
#define c_bs_b1up   16     // may be above b1
#define c_bs_round  32
#define c_bs_cap    64     // capital shape
#define c_bs_done   128
	char bas1;
	// 32
	char bas2;
	char bas3;
	char bas4;
	uchar cg_flag; // cut-to-glue message
#define c_cg_noglue     1   // don't glue to ... ( just cut )
#define c_cg_noenv      2       // envelope address obsolete
#define tenv(c)         ((c)->env && !((c)->cg_flag&c_cg_noenv))
#define c_cg_comp       4   // composed cell
#define tsimple(c)      (tenv(c) && !((c)->cg_flag&c_cg_comp))
	//#define c_cg_rqdot      4   // dot test needed (iIl1 in versions)
	// 93.08.19  the flag uprazdnen
#define c_cg_cutdone    8   // verarbeitet by cut
#define c_cg_cutr       16  // cut at right side
#define c_cg_cutl       32  // cut at left side
#define c_cg_cut        16+32  // cut somewhere
#define c_cg_cutacc     8+16+32 // cutted accent
#define c_cg_just       64  // just created
#define c_cg_cutoff    128  // ignore "cutted" flags at glue attempt
	int16_t r_row; // real row of cell
	int16_t r_col; // real collumn of cell
	int16_t nvers; // number of versions
	version vers[VERS_IN_CELL];
	uint16_t flg; // flag of cell
#define c_f_let         1       // letter component
#define c_f_bad     2   // badly recognized
#define c_f_dust        4       // dust
#define c_f_punct       8       // punctuation
#define c_f_space       16      // space
#define c_f_solid       32      // spelled letter
#define c_f_confirmed   64      // confirmed letter
#define c_f_fict        128     // fictive element
#define c_f_detouch     256     // fictive element
	uchar cpos; // position of component
#define ce_p_small      1
#define ce_p_large      2
#define ce_p_low        4
#define ce_p_t          8
#define ce_p_Q          16
#define ce_p_Ql         32
#define ce_p_bra        64
#define ce_p_ex         128
#define ce_p_all        255     // all positions are possible
	uchar reasno; // proportional criteria messages
#define c_rn_right      1       // right refuse by proportions
#define c_rn_left       2       // left refuse by proportions
#define c_rn_pi         4       // this is a dot of some 'i'
	uchar keg; // kegel
	uchar font; // font properties
#define c_fp_ser        1       // serific
#define c_fp_gelv       2       // helvetic
#define c_fp_bold       4       // bold
#define c_fp_light      8       // light
#define c_fp_it         16      // italic
#define c_fp_str        32      // stright
#define c_fp_undrln     64      // underlined
#define c_fp_narrow     128     // narrow str
	// 80
	uchar dens; // BOX - calculated density
	uchar recsource; // who and how recognized
#define c_rs_ev     1   //  events brought versions
#define c_rs_BOX    2   //  BOX done
#define c_rs_BOXs   4   //  BOX > "29500"
#define c_rs_bitcmp 8   //  bit compare takes versions
#define c_rs_deskr  16  //  diskriminator
#define c_rs_NCU    32  //  neural network
#define c_rs_LEO    64  //  LEO
	int16_t stick_inc; // inc of stick
#define NO_INCLINE 10000 // not init incline
	c_comp *complist; // if no envelope - list of envelopes
	// 88
	int16_t left; // left  of main part (without accent)
	int16_t right; // right of main part (without accent)
	uchar bas_acc; // cell's relations with bases
#define c_ba_b1  1    // accepted  to b1
#define c_ba_b2  2    // accepted  to b2
#define c_ba_killed 4 // temporary: all versions killed by linear
#define c_ba_chance 8 // one more chance for cut version
	uchar shape;
#define c_shp_cap   1
#define c_shp_stick 2
#define c_shp_stb1  4
#define c_shp_small 8
#define c_shp_dbh   32
#define c_shp_b1up  16
	uchar rus_eng_word; // information for RUS/ENG manipulations
#define ruseng_no      0  // russian
#define ruseng_rus     0  // russian
#define ruseng_re      1  // russian or english
#define ruseng_eng     2  // english
#define ruseng_alt     3  // many alternates
	uchar broken_II; // II configuration
	// 96
	uchar language;
	uchar pos_inc;
#define erect_no       0   // really envelop
#define erect_rot      1   // rotate image
#define erect_rest     2   // restore after rotating
#define erect_old      4   // rotate prototype images
#define erect_zero     8   // disable rotate and cursive study
#define set_erection( c, inc ) if( (inc)!=NO_INCLINE&&tenv(c) ) \
             { (c)->stick_inc=inc; (c)->pos_inc=inc?erect_rot:erect_zero; }
	uchar cg_flag_fine; // type of cutting position
#define c_cg_cut_tl     0x01    // left top    cutten
#define c_cg_cut_ml     0x02    // left middle cutten
#define c_cg_cut_bl     0x04    // left bottom cutten
#define c_cg_cut_fl     0x08    // left full   cutten
#define c_cg_cut_tr     0x10   // right top    cutten
#define c_cg_cut_mr     0x20   // right middle cutten
#define c_cg_cut_br     0x40   // right bottom cutten
#define c_cg_cut_fr     0x80   // right full   cutten
	uchar accent_leader; // leader cell in farajev complex-cell
	int16_t save_stick_inc; // save_inc of stick
	int16_t clink; // Link to the cluster
	// 104
	uint16_t clu_attr; // cluster attributes
	uchar bottom_accent; // was spare[1]; 16.10.97 E.P.
	uchar n_baton;
#define MAX_BATON 3
#define NO_BATONS 255
	struct cell_struc * dup; // ref to copy of this word; defined only in spaces
	// 112
	STICK save_baton[MAX_BATON];
	// 24+112=136
	int32_t dupstart, dupend;
#define dup_default     ((cell*)0xffffffff)
	// 144
	uchar dlang_dup;
	uchar font_new;
	uchar history;
	uchar solid_no;
	uint32_t flg_new;
#define c_fn_shave      0x00000001 // shaved cell
#define c_fn_acccap     0x00000002 // accent cap
#define c_fn_under      0x00000004 // underline
#define c_fn_initvers   0x00000008 // init versions
#define c_fn_allocvers  0x00000010 // allocation version struct
#define c_fn_temp       0x00000020 // temporary
#define c_fn_val2odd    0x00000040  // odd valid word on pass2
#define c_fn_val2even   0x00000080 // even valid word on pass2
#define c_fn_bl_cut     0x00000100 // cut by base line(s)
#define c_fn_down       0x00000200 // pointer to start branch
#define c_fn_downend    0x00000400 // pointer to stop  branch
#define c_fn_hide       0x00000800 // pointer to stop  branch
#define c_fn_stop       0x00001000 // stop cell marked
#define c_fn_space      0x00002000 // need convert to space ' '
#define c_fn_taken      0x00004000 // was taken from dust
	// 152
	int16_t nClust;
	version pr_vers; //version from I pass
	// 156
	uchar reserv[4];
};
typedef struct cell_struc cell;

//-------------------- line representation ----------------------

//      At the beginning of line representation - word of total length -
//      not use it, simply skip
//      At end of each line zero byte as mark of line end
//      After last line zero word

//      line header
struct ln_head {
	int16_t lth; // length of one line representation
	int16_t h; // height of line
	int16_t row; // relative row of line start
	uint16_t flg; // flags of free beg and free end
#define l_fbeg          0x20
#define l_fend          0x80
#define l_cbeg          0x02
#define l_cend          0x08
};
typedef struct ln_head lnhead;

//      one interval
struct int_s {
	uchar l; // length of interval
	uchar e; // end of interval coordinates
};
typedef struct int_s interval;

struct large_int_s {
	uint16_t l; // length of interval
	uint16_t e; // end of interval coordinates
};
typedef struct large_int_s large_interval;

//------------------- string of letters ------------------------

struct str_struc {
	int16_t row; // upper of line
	int16_t col; // left of line
	int16_t lower; // lower of line
	int16_t right; // right of line
	uint16_t dust; // end of letter ptrs
	uint16_t end; // end of dust ptrs
	uint16_t lth;
	uint16_t first;
	uint16_t last;
	uint16_t scale; // scale of the string
	int16_t fragment; // fragment of the string
	uchar language; // language of the string
	c_comp *ccomp[1]; // array of ptrs to components
};
typedef struct str_struc str;

//----------------------- base lines --------------------------

struct baseline_struct {
	int16_t b0, b1, b2, b3, b4, bm, ps; // four base lines, middle line, point size
	int16_t n1, n2, n3, n4;
};
typedef struct baseline_struct B_LINES;

//------------------------- rules -----------------------------
//AK:  without collision when snap.dll creating
#ifndef _SNAP_

#include "compat_defs.h"

struct rule_struct {
	POINT beg, end;
	uchar width;
	uchar type;
#define VERT_LN  0
#define HOR_LN   1
#define UNDRLN   2
#define FRM_LN   4
};
typedef struct rule_struct STRLN;

#endif  // _SNAP_
//-------------------------------------------------------------
#include "cutstr.h"
#include "embbox.h"
#endif
