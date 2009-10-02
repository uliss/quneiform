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

#ifndef H_cstrdef_h
#define H_cstrdef_h

#include "recdefs.h"
#include "ccomdefs.h"
#pragma pack (push,8)
#define DEBUG_MODE 1

#define     CSTR_VERSION_CODE  2

typedef Handle CSTR_line;

typedef struct tagCSTR_stick {
	char x, y, l, w, top, bot;
	int16_t incl;
} CSTR_stick;

typedef struct strucCSTR_attr {

	int32_t incline; // Incline*2048 = tg
	int32_t fragment;
	int16_t col, row;
	int16_t r_col, r_row;
	int16_t r_hei, r_wid;
	int16_t l_col, l_row;
	int32_t wid, hei;
	// 32
	uint32_t Flags;
#define CSTR_STR_No              0x00000000
#define CSTR_STR_DotMatrix_RCM   0x00000001
#define CSTR_STR_DotMatrix_LEO   0x00000002
#define CSTR_STR_SCALED          0x00000004
#define CSTR_STR_EMPTY           0x00000008
#define CSTR_STR_ReadyBL         0x00000010
#define CSTR_STR_Digital         0x00000020
#define CSTR_STR_PlusMinus       0x00000040
#define CSTR_STR_YesNo           0x00000080
#define CSTR_STR_DigitalFuzzy    0x00000100
#define CSTR_STR_ReadyResults    0x00000200
#define CSTR_STR_Fax100x200      0x00000400
#define CSTR_STR_Matrix          0x00000800
#define CSTR_STR_CapDrop         0x00001000
#define CSTR_STR_HandFragment    0x00002000
#define CSTR_STR_PointSuspension 0x00004000
#define CSTR_STR_NEGATIVE        0x00008000
#define CSTR_STR_UPDOWN          0x00010000
#define CSTR_STR_DOWNUP          0x00020000
#define CSTR_STR_END_POINTED     0x00040000//в конце строки имеются точки
#define CSTR_STR_BEGIN_POINTED   0x00080000//в начале строки имеются точки
	int16_t bs1, bs2, bs3, bs4;
	int16_t Nb1, Nb2, Nb3, Nb4;
	int16_t Nbt, bsm, Ps, Psf;
	uchar VersionName[16]; // Nick 20.02.2001 from 36 vto 16
	int16_t tab_row; // Nick 20.02.2001
	int16_t ref_father; // параметры формул
	int16_t ref_nSon; // Nick 22.05.2001
	int16_t ref_twin;
	uchar reserve[12]; // Nick 22.05.
	int16_t pageSkew2048;
	int16_t ResolutionY;
	// 100
	uchar language;
	uchar scale;
	int16_t l_hei, l_wid, erection;
	int16_t agregat;
	int16_t tab_column;
	// 112
	int32_t number;
	int32_t version;
	// 120
	uchar colors[4];
	// 124
	int16_t agregat_row;
	uchar tab_number;
	uchar cn_weight; // due to car numbers recognition
	// align to 128 bytes
} CSTR_attr;

typedef struct strucCSTR_rast_attr {
	int16_t row; // ideal row of cell
	int16_t col; // ideal collumn of cell
	//// 4
	int16_t h; // height of cell
	int16_t w; // width of cell
	//// 8
	char bdiff; // local base line corrective displacement
	uchar difflg; // local correction flg
	// baseline defined by cell:
#define CSTR_db_b1      1
#define CSTR_db_b2      2
#define CSTR_db_b3      4
#define CSTR_db_b4      8
#define CSTR_db_down    16     // base corrected 1 pixel down
#define CSTR_db_up      32     // base corrected 1 pixel up
#define CSTR_db_forbid  64     // don't take to calculate bases (abnormal pos)
#define CSTR_db_solid   128    // BOX_solid letter not penalized
	uchar basflg;
#define CSTR_bs_b1a     1      // agrees to be at  b1
#define CSTR_bs_b2a     2      //                  b2
#define CSTR_bs_bt      4      //                  't' level
#define CSTR_bs_t       8      // 't' only  (letters)
#define CSTR_bs_b1up    16     // may be above b1
#define CSTR_bs_round   32
#define CSTR_bs_cap     64     // capital shape
#define CSTR_bs_done    128
	uchar accent_leader; // leader cell in farajev complex-cell
	//// 12
	int16_t r_row; // real row of cell
	int16_t r_col; // real collumn of cell
	//// 16
	char bas1;
	char bas2;
	char bas3;
	char bas4;
	//// 20
	uchar cg_flag; // cut-to-glue message
#define CSTR_cg_noglue     1   // don't glue to ... ( just cut )
#define CSTR_cg_noenv      2    // envelope address obsolete
#define tenv(c)         ((c)->env && !((c)->cg_flag&c_cg_noenv))
#define CSTR_cg_comp       4   // composed cell
#define tsimple(c)      (tenv(c) && !((c)->cg_flag&c_cg_comp))
	//#define c_cg_rqdot      4   // dot test needed (iIl1 in versions)
	// 93.08.19  the flag uprazdnen
#define CSTR_cg_cutdone     8   // verarbeitet by cut
#define CSTR_cg_cutr        16  // cut at right side
#define CSTR_cg_cutl        32  // cut at left side
#define CSTR_cg_cut         16+32  // cut somewhere
#define CSTR_cg_cutacc      8+16+32 // cutted accent
#define CSTR_cg_just        64  // just created
#define CSTR_cg_cutoff      128  // ignore "cutted" flags at glue attempt
	uchar cpos; // position of component
#define CSTR_p_small        1
#define CSTR_p_large        2
#define CSTR_p_low              4
#define CSTR_p_t                    8
#define CSTR_p_Q                    16
#define CSTR_p_Ql                   32
#define CSTR_p_bra              64
#define CSTR_p_ex                   128
#define CSTR_p_all              255     // all positions are possible
	uchar reasno; // proportional criteria messages
#define CSTR_rn_right       1   // right refuse by proportions
#define CSTR_rn_left        2   // left refuse by proportions
#define CSTR_rn_pi                  4       // this is a dot of some 'i'
	uchar keg; // kegel
	//// 24
	uchar font; // font properties
#define CSTR_fp_ser         1       // serific
#define CSTR_fp_gelv        2       // helvetic
#define CSTR_fp_bold        4       // bold
#define CSTR_fp_light       8       // light
#define CSTR_fp_it          16      // italic
#define CSTR_fp_str         32      // stright
#define CSTR_fp_undrln      64      // underlined
#define CSTR_fp_narrow      128     // user defined
	uchar dens; // BOX - calculated density
	uchar recsource; // who and how recognized
#define CSTR_rs_ev          1   // events brought versions
#define CSTR_rs_BOX         2   // BOX done
#define CSTR_rs_BOXs        4  // BOX > "29500"
#define CSTR_rs_bitcmp      8  // bit compare takes versions
#define CSTR_rs_LEO        64  // LEO
	uchar bas_acc; // cell's relations with bases
#define CSTR_ba_b1          1    // accepted  to b1
#define CSTR_ba_b2          2    // accepted  to b2
#define CSTR_ba_killed      4 // temporary: all versions killed by linear
#define CSTR_ba_penalty     8
#define CSTR_ba_chance      16 // one more chance for cut version
	//// 28
	int16_t left; // left  of main part (without accent)
	int16_t right; // right of main part (without accent)
	//// 32
	uchar shape;
#define CSTR_shp_cap        1
#define CSTR_shp_stick      2
#define CSTR_shp_stb1       4
#define CSTR_shp_small      8
#define CSTR_shp_dbh        32
#define CSTR_shp_b1up       16
	uchar rus_eng_word; // information for RUS/ENG manipulations
#define CSTR_ruseng_no      0  // russian
#define CSTR_ruseng_rus     0  // russian
#define CSTR_ruseng_re      1  // russian or english
#define CSTR_ruseng_eng     2  // english
#define CSTR_ruseng_alt     3  // many alternates
	uchar broken_II; // II configuration
	uchar language;
	//// 36
	uchar pos_inc;
#define CSTR_erect_no       0   // really envelop
#define CSTR_erect_rot      1   // rotate image
#define CSTR_erect_rest     2   // restore after rotating
#define CSTR_erect_old      4   // rotate prototype images
#define CSTR_erect_zero     8   // disable rotate and cursive study
#define CSTR_set_erection( c, inc ) if( (inc)!=NO_INCLINE&&tenv(c) ) \
             { (c)->stick_inc=inc; (c)->pos_inc=inc?CSTR_erect_rot:CSTR_erect_zero; }
	uchar cg_flag_fine; // type of cutting position
#define CSTR_cg_cut_tl     0x01    // left top    cutten
#define CSTR_cg_cut_ml     0x02    // left middle cutten
#define CSTR_cg_cut_bl     0x04    // left bottom cutten
#define CSTR_cg_cut_fl     0x08    // left full   cutten
#define CSTR_cg_cut_tr     0x10    // right top    cutten
#define CSTR_cg_cut_mr     0x20    // right middle cutten
#define CSTR_cg_cut_br     0x40    // right bottom cutten
#define CSTR_cg_cut_fr     0x80    // right full   cutten
#define CSTR_NO_BATONS 255
	uchar n_baton;
	uchar flg_spell;
#define CSTR_fa_spell_none      0x00    // no spell checking
#define CSTR_fa_spell_solid     0x01    // solid word
#define CSTR_fa_spell_correct   0x02    // new order in alternates
#define CSTR_fa_spell_restruct  0x04    // new number of letter
#define CSTR_fa_spell_changed   (CSTR_fa_spell_correct|CSTR_fa_spell_restruct)
#define CSTR_fa_spell_reject    0x08    // non understand word
#define CSTR_fa_spell_nocarrying 0x10   // save dash in carrying word
	//// 40
	int16_t stick_inc; // inc of stick
#define NO_INCLINE 10000 // not init incline
	int16_t save_stick_inc; // saved inc of stick
	//// 44
#define MAX_BATON 3
	CSTR_stick save_baton[MAX_BATON]; // 8*3 bytes
	//// 68
#define dup_default     ((cell*)0xffffffff)
	uint32_t RecogHistory;
#define CSTR_hi_none        0x00000000
#define CSTR_hi_evn         0x00000001
#define CSTR_hi_3x5         0x00000002
#define CSTR_hi_3x5s        0x00000004
#define CSTR_hi_desk        0x00000008
#define CSTR_hi_fon         0x00000010
#define CSTR_hi_leo         0x00000020
	//// 72
	uint32_t RecogPass;
	//// 76
	int16_t clink; // link to cluster
	uint16_t clu_attr; // cluster attributes
	//// 80
	int32_t ColorLtr, ColorBack; // colors of letters and background
	//// 88
	uint16_t flg; // flag of cell
#define CSTR_f_no           0   // not init
#define CSTR_f_let          1   // letter component
#define CSTR_f_bad          2   // badly recognized
#define CSTR_f_dust             4       // dust
#define CSTR_f_punct        8   // punctuation
#define CSTR_f_space        16  // space
#define CSTR_f_solid        32  // spelled solid text
#define CSTR_f_confirmed        64      // confirmed letter
#define CSTR_f_all              127     // non fictive element
#define CSTR_f_fict             128     // fictive element
#define CSTR_f_detouch      256 // fictive element ???
#define CSTR_f_spell        512 // speller solid cstr
	uchar kegl_font;
	uchar dlang_dup;
#define CSTR_fd_none    0
#define CSTR_fd_similar 1
#define CSTR_fd_equal   2
#define CSTR_fd_lowprob 4
#define CSTR_fd_alias   8
	//// 92
	uchar kegf;
	uchar font_spec;
#define CSTR_fs_none    0
#define CSTR_fs_courier 1
	int16_t nClust;
	//// 96
	uint32_t flg_new;
#define CSTR_fn_shave       0x00000001 // shaved cell
#define CSTR_fn_acccap      0x00000002 // aceent cap
#define CSTR_fn_under       0x00000004 // shaved cell
#define CSTR_fn_initvers    0x00000008 // init versions
#define CSTR_fn_allocvers   0x00000010 // allocation version struct
#define CSTR_fn_temp        0x00000020 // temporary
#define CSTR_fn_val2odd     0x00000040  // odd valid word on pass2
#define CSTR_fn_val2even    0x00000080 // even valid word on pass2
#define CSTR_fn_bl_cut      0x00000100 // cut by base line(s)
#define CSTR_fn_down        0x00000200 // pointer to start branch
#define CSTR_fn_downend     0x00000400 // pointer to stop  branch
#define CSTR_fn_hide        0x00000800 // pointer to stop  branch
#define CSTR_fn_stop        0x00001000 // stop cell marked
#define CSTR_fn_space       0x00002000 // need convert to space ' '
#define CSTR_fn_taken       0x00004000 // was taken from dust
	//// 100
	uchar version;
	uchar font_new;
	//// 102
	uchar bottom_accent; // need save in Rbal
	//// 103 !
	uchar reserve[25]; // for next special flags, aling to 128
} CSTR_rast_attr;

typedef struct strucCSTR_cell {
	CSTR_rast_attr attr;
	struct strucCSTR_cell * next;
	struct strucCSTR_cell * prev;
	struct strucCSTR_cell * dup;
	uchar * recRaster;
	int32_t lnPixWidth;
	int32_t lnPixHeight;
	CCOM_comp * env;
	struct strucCSTR_cell * complist;
	UniVersions * vers;
	CSTR_line line_no;
	struct strucCSTR_cell * prev_down;
	CCOM_USER_BLOCK * user_block;
	struct strucCSTR_cell * dupend;
	struct strucCSTR_cell * prev_up;
	struct strucCSTR_cell * next_up;
	struct strucCSTR_cell * next_down;
} CSTR_cell;

typedef struct _strucCSTR_head {
	CSTR_attr attr;
	CCOM_handle container;
	Bool32 private_container;
	uint32_t Properties;
#define CSTR_LN_NO          0x00000000
#define CSTR_LN_FICT        0x00000001

	int32_t number;
	int32_t version;
	CSTR_cell first;
	CSTR_cell last;
	struct _strucCSTR_head *prev, *next, *next_fragm_line;
} CSTR_head;

#define CSTR_TYPE_IMAGE_NO  0   // no type
#define CSTR_TYPE_IMAGE_LP  1   // linerepresentation
#define CSTR_TYPE_IMAGE_RS  2   // RecRaster
#define CSTR_TYPE_IMAGE_RS1 3   // B/W bitmap aligned to 1 byte
#ifdef DEBUG_MODE

typedef struct strucCSTR_cell * CSTR_rast;
#else

typedef int32_t CSTR_rast;
#endif

#define CSTR_ANSI_CHARSET            0
#define CSTR_DEFAULT_CHARSET         1
#define CSTR_SYMBOL_CHARSET          2
#define CSTR_SHIFTJIS_CHARSET        128
#define CSTR_HANGEUL_CHARSET         129
#define CSTR_HANGUL_CHARSET          129
#define CSTR_GB2312_CHARSET          134
#define CSTR_CHINESEBIG5_CHARSET     136
#define CSTR_OEM_CHARSET             255
#define CSTR_JOHAB_CHARSET           130
#define CSTR_HEBREW_CHARSET          177
#define CSTR_ARABIC_CHARSET          178
#define CSTR_GREEK_CHARSET           161
#define CSTR_TURKISH_CHARSET         162
#define CSTR_VIETNAMESE_CHARSET      163
#define CSTR_THAI_CHARSET            222
#define CSTR_EASTEUROPE_CHARSET      238
#define CSTR_RUSSIAN_CHARSET         204

#define MAC_CHARSET             77
#define BALTIC_CHARSET          186
#define TURKISH_CHARSET         162	// 31.05.2002 E.P.
#define UZBEK_CHARSET           254
#define KAZAH_CHARSET           253

// разные версии линий
#define CSTR_LINVERS_MAIN        0
#define CSTR_LINVERS_MAINOUT     1
#define CSTR_LINVERS_PASS2       2
#define CSTR_LINVERS_PASS2SNAP  12

#define CSTR_LINVERS_ENG         3
#define CSTR_LINVERS_ENGOUT      4
#define CSTR_LINVERS_SAVE        5
// для базовых линий
#define CSTR_LINVERS_BAL         8

#pragma pack (pop)
#endif
