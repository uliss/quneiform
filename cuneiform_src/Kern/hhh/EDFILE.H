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

  #pragma message(__FILE__"(1)")  // BC doesn't understand it somewhy...

#ifndef __EDFILE__
#define __EDFILE__

#include "globus.h"
#pragma pack(push,1)

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
 Word8 code;
 Word8 pos;
 Word16 row;
 Word16 col;
 Word16 width;
 Word16 height;
 };

struct text_ref
 {                                     /* 0 - letter */
 Word8 code;                            /* 1 -        */
 Word8 type;                            /* 2 - word   */
 Word16 object;                           /* 3 - string */
 };

struct font_kegl                       /*  1 - serific     */
 {                                     /*  2 - gelvetic    */
 Word8 code;                            /*  4 - bold        */
 Word8 new_kegl;                        /*  8 - light       */
 Word16 new_font;                        /* 16 - italic      */
// Word8 reserv;                          /* 32 - straight    */
 };                                    /* 64 - underlined  */

struct kegl
 {
 Word8 code;
 Word8 new_kegl;
 };

struct EdTagLanguage //for russian-english recognize
 {
 Word8 code; // 0x0F
 Word8 language;
 };

struct shift
 {
 Word8 code;
 Word8 kegl;
 Word16 clearance;                         /* value of lift or descent */
 };                                     /* lift positive            */
					/* descent negative         */
struct retrieve_level
 {
 Word8 code;
 Word8 kegl;
 };

struct underline
 {                                     /* 0 - thin straight       */
 Word8 code;                            /* 1- half thick straight  */
 Word8 type;                            /* 2- thick straight       */
 };                                    /* 3- thin cursive         */
				       /* 4- half thick cursive   */
				       /* 5- thick cursive        */
				       /* 6- beg of underline     */
				       /* 7- end of underline     */
struct dens_print
 {
 Word8 code;
 Word8 dens_atr;                         /* attribute of print's    */
 };				        /*                 density */

struct tabul
 {
 Word8 code;
 Word8 numb_in_tab_tabul;                /* number of position in        */
 };                                     /*         table of tabulation  */

struct tabl_tabul
 {
 Word8 code;
 Word8 lth;                              /* scale of arow               */
 Word16 arow_pos[1] ;
 };

struct fragm_disk
 {
 Word8 code;
 Word8 fragm_numb;
 Word16 depth;
 };
                                      
struct step_back
 {
 Word8 code;
 Word8 Step_back;                        // value of backstep //AK 18.03.97 step -> Step
 };

struct line_beg
 {
 Word8 code;
 Word8 height;
 Word16 base_line;                         /* displacement for current         */
 };                                     /*     line to upper frame of fragm */

struct position
 {
 Word8 code;
 Word8 store;
 Word16 pos;                               /* position in line for      */
 };                                     /*     left frame of  fragm  */

struct table_conform_sizes
 {
 Word8 code;
 Word8 store;
 Int8 tab_val_A [9*2];                   /* table of sizes of letter A  */
 };              			 /*     for kegles from 4 to 12 */


struct group_words
 {
 Word8 code;
 Word8  gr_wd_type;                     	  /* 0 - beg of group             */
 };			                  /* 1 - cur. word of group       */
					  /* 2 - end                      */
					  /* 3 - partition between groups */

struct group_symbols
 {
 Word8 code;
 Word8 gr_sb_type;
 };

struct border
    {
    Word8 code;          /* 0x16 SS_BORDER             */
    Word8 type;          /* 1 - left                   */
#define b_vert 1	/* 2 - right                  */
#define b_hor  4	/* 4 - top                    */
			/* 8 - bottom                 */
    Word16  length;        /* Length of border in pixels */
    Word16  x,y;           /* Coordinates of line begin  */
    };

struct table_header
    {
    Word8    code;       /* 0x17 SS_TABLE_HEADER                   */
    Word8    cols;       /* Number of columns                      */
    Word16     lth;        /* Total length of record                 */
    Word16     nfrag[1];    /* List of fragments in header of table   */
    };

struct list_of_fragments
    {
    Word8    code;       /* 0x18 SS_LIST_OF_FRAGMENTS    */
    Word8    cols;       /* Number of columns            */
    Word16     lth;        /* Total length of record       */
    Word16     nfrag[1];   /* List of fragments in table   */
    };

struct aksant
 {
 Word8 code1;
 Word8 code;
 };

struct neg_half_space
 {
 Word8 code;
 Word8 authenticity_degree;
 };

struct pos_half_space
 {
 Word8 code;
 Word8 authenticity_degree;
 };

struct sheet_disk_descr
 {
 Word8 code;
 Int8 quant_fragm;
 Word16 sheet_numb;
 Word16 descr_lth;
 Word8 byte_flag;
 Word16 resolution;
 Word16  incline;
 Word16 version; 
 Int8 tabl[11];
 };

struct fragm_disk_descr
 {
 Word8 code;
 Word16 row;                             /* coordinates of left upper  */
 Word16 col;                             /* angle of fragment's frame  */
 Word16 height;                          /* height of fragment         */
 Word16 w_width;                         /* Q.w_width of fragment      */
 Int8 type;
#define FD_TYPE_TEXT 0
#define FD_TYPE_PICT 1
#define FD_TYPE_TABLE 2
#define FD_TYPE_EMPTY 3

 Word8 kegl;                           /* kegl for following fragm    */
 Word8 font;                            /* font ~~~~~~~~~~~~~~~~~~    */
 Word8 language;//reserv;              /* language for fragment       03-29-94 02:21pm,Pit*/
 Word8 type_underl;                    /* type specifications of font    */
 };				      /*     for additional information */
				      /*     look at underline          */


typedef struct text_ref	EDREM;

struct letter
{
    Word8    bType;          //  ASCII code.  >= ' '.
    Word8    bAttrib;          
};

typedef struct letterEx
{
    Word8    alternative;          //  ASCII code.  >= ' '.
    Word8    probability;          
}LETTER;

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

//#define	SSR_SECT_BORD_TOP		34
//#define	SSR_SECT_BORD_BOTTOM	35
//#define	SSR_SECT_BORD_LEFT		36
//#define	SSR_SECT_BORD_RIGHT		37

#define	SSR_COL_WIDTH			38
#define	SSR_COL_SPACING			39

#define SSR_FRAME_X				40
#define SSR_FRAME_Y				41
#define SSR_FRAME_W				42
#define SSR_FRAME_H				43

//Fragment types
#define TP_ONE_LINE      0x0001
#define TP_LEFT_ALLIGN   0x0002
#define TP_RIGHT_ALLIGN  0x0004
#define TP_CENTER        0x0008
#define TP_POS_INDENT    0x0010
#define TP_NEG_INDENT    0x0020
#define TP_BULLET        0x0040
#define TP_POESY         0x0080
#define TP_LN_SPACE      0x0100
#define TP_NOT_RECOG     0x0200
#define TP_BORDER        0x0400
#define TP_BRACKET       0x8000
#define TP_MCOL_BEG      (TP_BRACKET|0x1000)
#define TP_NEW_COL       (TP_BRACKET|0x2000)
#define TP_MCOL_END      (TP_BRACKET|0x3000)
#define TP_TAB_BEG       (TP_BRACKET|0x4000)
#define TP_NEW_ROW       (TP_BRACKET|0x5000)
#define TP_FRAME_BEG      (TP_BRACKET|0x6000)
#define TP_FRAME_END      (TP_BRACKET|0x7000)
#define TP_NEW_TAB_BEG	 (TP_BRACKET|0x4100)
#define TP_NEW_ROW_BEG   (TP_BRACKET|0x5100)
#define TP_NEW_CELL_BEG  (TP_BRACKET|0x2100)
#define TP_NEW_TAB_END   (TP_BRACKET|0x3100)
#define TP_DEL           (TP_NOT_RECOG|TP_BORDER)
#define TP_FICT_FR_FLAGS (TP_BORDER|TP_BRACKET)


#pragma pack(pop)

#include "edext.h"
#endif
