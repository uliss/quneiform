/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#ifndef CELL_H_
#define CELL_H_

#include "cttypes.h"
#include "comp.h"
#include "stick.h"

struct cell
{
        int16_t row; // ideal row of cell
        int16_t col; // ideal column of cell
        int16_t h; // height of cell
        int16_t w; // width of cell
        // 8
        c_comp *env; // envelope and line representation ptr
        cell *next; // next in all string
        cell *prev; // prev in all string
        cell *nextl; // next letter ( only for letters )
        cell *prevl; // prev letter ( only for letters )
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
        CIF::version vers[VERS_IN_CELL];
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
        cell * dup; // ref to copy of this word; defined only in spaces
        // 112
        STICK save_baton[MAX_BATON];
        // 24+112=136
        void * dupstart;
        void * dupend;
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
        CIF::version pr_vers; //version from I pass
        // 156
        uchar reserv[4];
};

#endif /* CELL_H_ */
