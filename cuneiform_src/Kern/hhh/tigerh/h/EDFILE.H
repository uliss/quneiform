/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

пЮГПЕЬЮЕРЯЪ ОНБРНПМНЕ ПЮЯОПНЯРПЮМЕМХЕ Х ХЯОНКЭГНБЮМХЕ ЙЮЙ Б БХДЕ ХЯУНДМНЦН ЙНДЮ,
РЮЙ Х Б ДБНХВМНИ ТНПЛЕ, Я ХГЛЕМЕМХЪЛХ ХКХ АЕГ, ОПХ ЯНАКЧДЕМХХ ЯКЕДСЧЫХУ СЯКНБХИ:

      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ХЯУНДМНЦН ЙНДЮ ДНКФМШ НЯРЮБЮРЭЯЪ СЙЮГЮММНЕ
        БШЬЕ СБЕДНЛКЕМХЕ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х ОНЯКЕДСЧЫХИ
        НРЙЮГ НР ЦЮПЮМРХИ.
      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ДБНХВМНЦН ЙНДЮ Б ДНЙСЛЕМРЮЖХХ Х/ХКХ Б
        ДПСЦХУ ЛЮРЕПХЮКЮУ, ОНЯРЮБКЪЕЛШУ ОПХ ПЮЯОПНЯРПЮМЕМХХ, ДНКФМШ ЯНУПЮМЪРЭЯЪ
        СЙЮГЮММЮЪ БШЬЕ ХМТНПЛЮЖХЪ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х
        ОНЯКЕДСЧЫХИ НРЙЮГ НР ЦЮПЮМРХИ.
      * мХ МЮГБЮМХЕ Cognitive Technologies, МХ ХЛЕМЮ ЕЕ ЯНРПСДМХЙНБ МЕ ЛНЦСР
        АШРЭ ХЯОНКЭГНБЮМШ Б ЙЮВЕЯРБЕ ЯПЕДЯРБЮ ОНДДЕПФЙХ Х/ХКХ ОПНДБХФЕМХЪ
        ОПНДСЙРНБ, НЯМНБЮММШУ МЮ ЩРНЛ он, АЕГ ОПЕДБЮПХРЕКЭМНЦН ОХЯЭЛЕММНЦН
        ПЮГПЕЬЕМХЪ.

щрю опнцпюллю опеднярюбкемю бкюдекэжюлх юбрнпяйху опюб х/хкх дпсцхлх кхжюлх "йюй
нмю еярэ" аег йюйнцн-кхан бхдю цюпюмрхи, бшпюфеммшу ъбмн хкх ондпюгслебюелшу,
бйкчвюъ цюпюмрхх йнллепвеяйни жеммнярх х опхцндмнярх дкъ йнмйпермни жекх, мн ме
нцпюмхвхбюъяэ хлх. мх бкюдекеж юбрнпяйху опюб х мх ндмн дпсцне кхжн, йнрнпне
лнфер хглемърэ х/хкх онбрнпмн пюяопнярпюмърэ опнцпюллс, мх б йнел яксвюе ме
мея╗р нрберярбеммнярх, бйкчвюъ кчаше наыхе, яксвюимше, яоежхюкэмше хкх
онякеднбюбьхе сашрйх, ябъгюммше я хяонкэгнбюмхел хкх онмеяеммше бякедярбхе
мебнглнфмнярх хяонкэгнбюмхъ опнцпюллш (бйкчвюъ онрепх дюммшу, хкх дюммше,
ярюбьхе мецндмшлх, хкх сашрйх х/хкх онрепх днунднб, онмеяеммше хг-гю деиярбхи
рперэху кхж х/хкх нрйюгю опнцпюллш пюанрюрэ янблеярмн я дпсцхлх опнцпюллюлх,
мн ме нцпюмхвхбюъяэ щрхлх яксвюълх), мн ме нцпюмхвхбюъяэ хлх, дюфе еякх рюйни
бкюдекеж хкх дпсцне кхжн ашкх хгбеыемш н бнглнфмнярх рюйху сашрйнб х онрепэ.

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

/*
  шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш
  шш                                                              шш
  шш     Copyright (C) 1990 Cognitive Technology Corporation.     шш
  шш	 All rights reserved. This program is proprietary and     шш
  шш     a trade secret of Cognitive Technology Corporation.      шш
  шш                                                              шш
  шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш
*/

#define SS_BITMAP_REF 0x00
#define SS_TEXT_REF 0x01
#define SS_REMARK SS_TEXT_REF
#define SS_FONT_KEGL 0x02
#define SS_KEGL 0x03
#define SS_SHIFT 0x04
#define SS_RETR_LEVEL 0x05
#define SS_UNDERLINE 0x06
#define SS_DENS_PRINT 0x07
#define SS_TABUL 0x08
#define SS_TABL_TABUL 0x09
#define SS_SHEET_DESCR 0x0a
#define SS_FRAGMENT 0x0b
#define SS_STEP_BACK 0x0c
#define SS_LINE_BEG 0x0d
#define SS_POSITION 0x0e
#define SS_LANGUAGE 0x0f
#define SS_TABL_CONFORM_SIZES 0x10
#define SS_GROUP_WORDS 0x11
#define SS_GROUP_SYMBOLS 0x12
#define SS_PARAGRAPH	 0x15
#define SS_BORDER        0x16
#define SS_TABLE_HEADER  0x17
#define SS_LIST_OF_FRAGMENTS 0x18
//  SS_EXTENTION	0x1C  Specian code for Extention ED format see EDEXT.H
#define SS_AKSANT 0x1d
#define SS_NEG_HALF_SPACE 0x1e
#define SS_POS_HALF_SPACE 0x1f

struct bit_map_ref
 {
 uchar code;
 uchar pos;
 int16_t row;
 int16_t col;
 int16_t width;
 int16_t height;
 };

struct text_ref
 {                                     /* 0 - letter */
 uchar code;                            /* 1 -        */
 uchar type;                            /* 2 - word   */
 int16_t object;                           /* 3 - string */
 };

struct font_kegl                       /*  1 - serific     */
 {                                     /*  2 - gelvetic    */
 uchar code;                            /*  4 - bold        */
 uchar new_kegl;                        /*  8 - light       */
 uchar new_font;                        /* 16 - italic      */
 uchar reserv;                          /* 32 - straight    */
 };                                    /* 64 - underlined  */

struct kegl
 {
 uchar code;
 uchar new_kegl;
 };

struct EdTagLanguage //for russian-english recognize
 {
 uchar code; // 0x0F
 uchar language;
 };

struct shift
 {
 uchar code;
 uchar kegl;
 int16_t clearance;                         /* value of lift or descent */
 };                                     /* lift positive            */
					/* descent negative         */
struct retrieve_level
 {
 uchar code;
 uchar kegl;
 };

struct underline
 {                                     /* 0 - thin straight       */
 uchar code;                            /* 1- half thick straight  */
 uchar type;                            /* 2- thick straight       */
 };                                    /* 3- thin cursive         */
				       /* 4- half thick cursive   */
				       /* 5- thick cursive        */
				       /* 6- beg of underline     */
				       /* 7- end of underline     */
struct dens_print
 {
 uchar code;
 uchar dens_atr;                         /* attribute of print's    */
 };				        /*                 density */

struct tabul
 {
 uchar code;
 uchar numb_in_tab_tabul;                /* number of position in        */
 };                                     /*         table of tabulation  */

struct tabl_tabul
 {
 uchar code;
 uchar lth;                              /* scale of arow               */
 int16_t arow_pos[1] ;
 };

struct fragm_disk
 {
 uchar code;
 uchar fragm_numb;
 int16_t depth;
 };

struct step_back
 {
 uchar code;
 uchar Step_back;                        // value of backstep //AK 18.03.97 step -> Step
 };

struct line_beg
 {
 uchar code;
 uchar height;
 int16_t base_line;                         /* displacement for current         */
 };                                     /*     line to upper frame of fragm */

struct position
 {
 uchar code;
 uchar store;
 int16_t pos;                               /* position in line for      */
 };                                     /*     left frame of  fragm  */

struct table_conform_sizes
 {
 uchar code;
 uchar store;
 char tab_val_A [9*2];                   /* table of sizes of letter A  */
 };              			 /*     for kegles from 4 to 12 */


struct group_words
 {
 uchar code;
 uchar  gr_wd_type;                     	  /* 0 - beg of group             */
 };			                  /* 1 - cur. word of group       */
					  /* 2 - end                      */
					  /* 3 - partition between groups */

struct group_symbols
 {
 uchar code;
 uchar gr_sb_type;
 };

struct border
    {
    uchar code;          /* 0x16 SS_BORDER             */
    uchar type;          /* 1 - left                   */
#define b_vert 1	/* 2 - right                  */
#define b_hor  4	/* 4 - top                    */
			/* 8 - bottom                 */
    int16_t  length;        /* Length of border in pixels */
    int16_t  x,y;           /* Coordinates of line begin  */
    };

struct table_header
    {
    uchar    code;       /* 0x17 SS_TABLE_HEADER                   */
    uchar    cols;       /* Number of columns                      */
    int16_t     lth;        /* Total length of record                 */
    int16_t     nfrag[1];    /* List of fragments in header of table   */
    };

struct list_of_fragments
    {
    uchar    code;       /* 0x18 SS_LIST_OF_FRAGMENTS    */
    uchar    cols;       /* Number of columns            */
    int16_t     lth;        /* Total length of record       */
    int16_t     nfrag[1];   /* List of fragments in table   */
    };

struct aksant
 {
 uchar code1;
 uchar code;
 };

struct neg_half_space
 {
 uchar code;
 uchar authenticity_degree;
 };

struct pos_half_space
 {
 uchar code;
 uchar authenticity_degree;
 };

struct sheet_disk_descr
 {
 uchar code;
 char quant_fragm;
 int16_t sheet_numb;
 int16_t descr_lth;
 uchar byte_flag;
 uint16_t resolution;
 int16_t  incline;
 char tabl[13];
 };

struct fragm_disk_descr
 {
 uchar code;
 int16_t row;                             /* coordinates of left upper  */
 int16_t col;                             /* angle of fragment's frame  */
 int16_t height;                          /* height of fragment         */
 int16_t w_width;                         /* Q.w_width of fragment      */
 char type;
#define FD_TYPE_TEXT 0
#define FD_TYPE_PICT 1
#define FD_TYPE_TABLE 2
#define FD_TYPE_EMPTY 3

 uchar kegl;                           /* kegl for following fragm    */
 uchar font;                            /* font ~~~~~~~~~~~~~~~~~~    */
 uchar language;//reserv;              /* language for fragment       03-29-94 02:21pm,Pit*/
 uchar type_underl;                    /* type specifications of font    */
 };				      /*     for additional information */
				      /*     look at underline          */


typedef struct text_ref	EDREM;

#define SSR_HUGE		0    /* Internal remark about huge letter */

/* Seria within broken line: first - BROKEN_X, last - BROKEN_W  */
#define SSR_BROKEN_BASELINE	1
#define SSR_BROKEN_HEIGHT	2
#define SSR_BROKEN_X		3
#define SSR_BROKEN_W		4

/* Seria about line: first - LINE_FN, other - optional  */
#define SSR_LINE_FN		5
#define SSR_LINE_PARAGRAPH	6
#define SSR_LINE_TYPE		7
#define SSR_LINE_INDENT		8
#define SSR_LINE_NPIECES        9
#define SSR_LINE_FICT		16
#define SSR_LINE_BULIND		23
#define SSR_LINE_X		24
#define SSR_LINE_W		25
#define SSR_LINE_BASELINE	26
#define SSR_LINE_HEIGHT		27

/* Seria about fragment: first - FRAG_TYPE      */
#define SSR_FRAG_TYPE		10
#define SSR_FRAG_BASE		11
#define SSR_FRAG_X		12
#define SSR_FRAG_W		13
#define SSR_FRAG_N		14
#define SSR_FRAG_SN		17
#define SSR_FRAG_REL		18
#define SSR_FRAG_Y   		19
#define SSR_FRAG_H		20
#define SSR_FRAG_PNUM		21
#define SSR_FRAG_COLXW		28

#define SSR_FRAG_END		15	/* end of fragments list  */

/* Misceleneaus  */

#define SSR_LINE_NUMBER		22	/* Internal line number  */
#define SSR_WORD_BOLD           32      /* density of word */
#define SSR_SHEET_TYPE          33      /* type of sheet */

#include "edext.h"
