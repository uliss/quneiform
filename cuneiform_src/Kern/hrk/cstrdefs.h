/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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
//#include "lang.h"
#pragma pack (push,8)
#define DEBUG_MODE 1

#define     CSTR_VERSION_CODE  2

typedef Handle CSTR_line;

typedef struct tagCSTR_stick
 {
 Int8  x,y,l,w,top,bot;
 Int16 incl;
 }CSTR_stick;


typedef struct strucCSTR_attr{

    Int32  incline              ;   // Incline*2048 = tg
    Int32  fragment             ;
    Int16  col, row             ;
    Int16  r_col, r_row     ;
    Int16  r_hei, r_wid     ;
    Int16  l_col, l_row     ;
    Int32  wid, hei             ;
// 32
    Word32 Flags            ;
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
#define CSTR_STR_END_POINTED     0x00040000//â êîíöå ñòðîêè èìåþòñÿ òî÷êè
#define CSTR_STR_BEGIN_POINTED   0x00080000//â íà÷àëå ñòðîêè èìåþòñÿ òî÷êè

    Int16  bs1, bs2, bs3, bs4;
    Int16  Nb1, Nb2, Nb3, Nb4;
    Int16  Nbt, bsm, Ps,  Psf;
    Word8  VersionName[16]  ;  // Nick 20.02.2001 from 36 vto 16
	Int16  tab_row;           // Nick 20.02.2001
	Int16  ref_father;  // ïàðàìåòðû ôîðìóë
	Int16  ref_nSon;     // Nick 22.05.2001
	Int16  ref_twin;
	Word8  reserve[12];       // Nick 22.05.
    Int16  pageSkew2048     ;
    Int16  ResolutionY      ;
// 100
    Word8  language;
    Word8  scale;
    Int16  l_hei, l_wid,erection;
    Int16  agregat;
    Int16  tab_column;
// 112
    Int32       number                      ;
    Int32       version                     ;
// 120
    Word8       colors[4]                   ;
// 124
    Int16  agregat_row;
    Word8  tab_number;
	Word8  cn_weight; // due to car numbers recognition
    // align to 128 bytes
    }   CSTR_attr           ;



typedef struct strucCSTR_rast_attr{
    Int16 row;  // ideal row of cell
    Int16 col;  // ideal collumn of cell
//// 4
    Int16 h;    // height of cell
    Int16 w;    // width of cell
//// 8
    Int8  bdiff;    // local base line corrective displacement
    Word8 difflg;   // local correction flg
// baseline defined by cell:
#define CSTR_db_b1      1
#define CSTR_db_b2      2
#define CSTR_db_b3      4
#define CSTR_db_b4      8
#define CSTR_db_down    16     // base corrected 1 pixel down
#define CSTR_db_up      32     // base corrected 1 pixel up
#define CSTR_db_forbid  64     // don't take to calculate bases (abnormal pos)
#define CSTR_db_solid   128    // BOX_solid letter not penalized
    Word8 basflg;
#define CSTR_bs_b1a     1      // agrees to be at  b1
#define CSTR_bs_b2a     2      //                  b2
#define CSTR_bs_bt      4      //                  't' level
#define CSTR_bs_t       8      // 't' only  (letters)
#define CSTR_bs_b1up    16     // may be above b1
#define CSTR_bs_round   32
#define CSTR_bs_cap     64     // capital shape
#define CSTR_bs_done    128
    Word8   accent_leader; // leader cell in farajev complex-cell
//// 12
    Int16 r_row;    // real row of cell
    Int16 r_col;        // real collumn of cell
//// 16
    Int8 bas1;
    Int8 bas2;
    Int8 bas3;
    Int8 bas4;
//// 20
    Word8 cg_flag;  // cut-to-glue message
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
 Word8 cpos;     // position of component
#define CSTR_p_small        1
#define CSTR_p_large        2
#define CSTR_p_low              4
#define CSTR_p_t                    8
#define CSTR_p_Q                    16
#define CSTR_p_Ql                   32
#define CSTR_p_bra              64
#define CSTR_p_ex                   128
#define CSTR_p_all              255     // all positions are possible
 Word8 reasno;    // proportional criteria messages
#define CSTR_rn_right       1   // right refuse by proportions
#define CSTR_rn_left        2   // left refuse by proportions
#define CSTR_rn_pi                  4       // this is a dot of some 'i'
 Word8 keg;      // kegel
//// 24
 Word8 font;     // font properties
#define CSTR_fp_ser         1       // serific
#define CSTR_fp_gelv        2       // helvetic
#define CSTR_fp_bold        4       // bold
#define CSTR_fp_light       8       // light
#define CSTR_fp_it          16      // italic
#define CSTR_fp_str         32      // stright
#define CSTR_fp_undrln      64      // underlined
#define CSTR_fp_narrow      128     // user defined
 Word8 dens;     // BOX - calculated density
 Word8 recsource;  // who and how recognized
#define CSTR_rs_ev          1   // events brought versions
#define CSTR_rs_BOX         2   // BOX done
#define CSTR_rs_BOXs        4  // BOX > "29500"
#define CSTR_rs_bitcmp      8  // bit compare takes versions
#define CSTR_rs_LEO        64  // LEO
 Word8 bas_acc;       // cell's relations with bases
#define CSTR_ba_b1          1    // accepted  to b1
#define CSTR_ba_b2          2    // accepted  to b2
#define CSTR_ba_killed      4 // temporary: all versions killed by linear
#define CSTR_ba_penalty     8
#define CSTR_ba_chance      16 // one more chance for cut version
//// 28
 Int16 left;            // left  of main part (without accent)
 Int16 right;           // right of main part (without accent)
//// 32
 Word8 shape;
#define CSTR_shp_cap        1
#define CSTR_shp_stick      2
#define CSTR_shp_stb1       4
#define CSTR_shp_small      8
#define CSTR_shp_dbh        32
#define CSTR_shp_b1up       16
 Word8 rus_eng_word; // information for RUS/ENG manipulations
#define CSTR_ruseng_no      0  // russian
#define CSTR_ruseng_rus     0  // russian
#define CSTR_ruseng_re      1  // russian or english
#define CSTR_ruseng_eng     2  // english
#define CSTR_ruseng_alt     3  // many alternates
 Word8 broken_II;    // II configuration
 Word8 language;
//// 36
 Word8 pos_inc;
#define CSTR_erect_no       0   // really envelop
#define CSTR_erect_rot      1   // rotate image
#define CSTR_erect_rest     2   // restore after rotating
#define CSTR_erect_old      4   // rotate prototype images
#define CSTR_erect_zero     8   // disable rotate and cursive study
#define CSTR_set_erection( c, inc ) if( (inc)!=NO_INCLINE&&tenv(c) ) \
             { (c)->stick_inc=inc; (c)->pos_inc=inc?CSTR_erect_rot:CSTR_erect_zero; }
 Word8  cg_flag_fine;        // type of cutting position
#define CSTR_cg_cut_tl     0x01    // left top    cutten
#define CSTR_cg_cut_ml     0x02    // left middle cutten
#define CSTR_cg_cut_bl     0x04    // left bottom cutten
#define CSTR_cg_cut_fl     0x08    // left full   cutten
#define CSTR_cg_cut_tr     0x10    // right top    cutten
#define CSTR_cg_cut_mr     0x20    // right middle cutten
#define CSTR_cg_cut_br     0x40    // right bottom cutten
#define CSTR_cg_cut_fr     0x80    // right full   cutten
#define CSTR_NO_BATONS 255
 Word8   n_baton;
 Word8   flg_spell;
#define CSTR_fa_spell_none      0x00    // no spell checking
#define CSTR_fa_spell_solid     0x01    // solid word
#define CSTR_fa_spell_correct   0x02    // new order in alternates
#define CSTR_fa_spell_restruct  0x04    // new number of letter
#define CSTR_fa_spell_changed   (CSTR_fa_spell_correct|CSTR_fa_spell_restruct)
#define CSTR_fa_spell_reject    0x08    // non understand word
#define CSTR_fa_spell_nocarrying 0x10   // save dash in carrying word
//// 40
 Int16  stick_inc;   // inc of stick
#define NO_INCLINE 10000 // not init incline
 Int16  save_stick_inc;   // saved inc of stick
//// 44
#define MAX_BATON 3
 CSTR_stick  save_baton[MAX_BATON]; // 8*3 bytes
//// 68
#define dup_default     ((cell*)0xffffffff)
        Word32  RecogHistory        ;
#define CSTR_hi_none        0x00000000
#define CSTR_hi_evn         0x00000001
#define CSTR_hi_3x5         0x00000002
#define CSTR_hi_3x5s        0x00000004
#define CSTR_hi_desk        0x00000008
#define CSTR_hi_fon         0x00000010
#define CSTR_hi_leo         0x00000020
//// 72
        Word32  RecogPass                   ;
//// 76
    Int16   clink               ; // link to cluster
    Word16  clu_attr            ; // cluster attributes
//// 80
    Int32   ColorLtr, ColorBack ; // colors of letters and background
//// 88
    Word16   flg;  // flag of cell
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
    Word8   kegl_font;
    Word8   dlang_dup;
#define CSTR_fd_none    0
#define CSTR_fd_similar 1
#define CSTR_fd_equal   2
#define CSTR_fd_lowprob 4
#define CSTR_fd_alias   8
//// 92
    Word8   kegf;
    Word8   font_spec;
#define CSTR_fs_none    0
#define CSTR_fs_courier 1
    Int16   nClust;
//// 96
    Word32 flg_new;
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
    Word8   version;
    Word8   font_new;
//// 102
	Word8   bottom_accent;    // need save in Rbal
//// 103 !
    Word8   reserve[25]             ; // for next special flags, aling to 128
    } CSTR_rast_attr            ;


typedef struct strucCSTR_cell{
    CSTR_rast_attr   attr              ;
    struct strucCSTR_cell * next       ;
    struct strucCSTR_cell * prev       ;
    struct strucCSTR_cell * dup        ;
    Word8                 * recRaster  ;
    Int32                   lnPixWidth ;
    Int32                   lnPixHeight;
    CCOM_comp             * env        ;
    struct strucCSTR_cell * complist   ;
    UniVersions           * vers       ;
    CSTR_line               line_no    ;
    struct strucCSTR_cell * prev_down  ;
    CCOM_USER_BLOCK       * user_block ;
    struct strucCSTR_cell * dupend     ;
    struct strucCSTR_cell * prev_up    ;
    struct strucCSTR_cell * next_up    ;
    struct strucCSTR_cell * next_down  ;
} CSTR_cell;

typedef struct _strucCSTR_head{
CSTR_attr   attr                ;
CCOM_handle container           ;
Bool32      private_container   ;
Word32      Properties          ;
#define CSTR_LN_NO          0x00000000
#define CSTR_LN_FICT        0x00000001


Int32       number                      ;
Int32       version                     ;
CSTR_cell   first               ;
CSTR_cell   last                ;
struct _strucCSTR_head  *prev, *next,*next_fragm_line;
} CSTR_head ;


#define CSTR_TYPE_IMAGE_NO  0   // no type
#define CSTR_TYPE_IMAGE_LP  1   // linerepresentation
#define CSTR_TYPE_IMAGE_RS  2   // RecRaster
#define CSTR_TYPE_IMAGE_RS1 3   // B/W bitmap aligned to 1 byte

#ifdef DEBUG_MODE

typedef struct strucCSTR_cell * CSTR_rast;
#else

typedef Int32 CSTR_rast;
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

// ðàçíûå âåðñèè ëèíèé
#define CSTR_LINVERS_MAIN        0
#define CSTR_LINVERS_MAINOUT     1
#define CSTR_LINVERS_PASS2       2
#define CSTR_LINVERS_PASS2SNAP  12

#define CSTR_LINVERS_ENG         3
#define CSTR_LINVERS_ENGOUT      4
#define CSTR_LINVERS_SAVE        5
// äëÿ áàçîâûõ ëèíèé
#define CSTR_LINVERS_BAL         8

#pragma pack (pop)
#endif
