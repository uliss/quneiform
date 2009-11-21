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

#ifndef __EDP__
#define __EDP__

#define RTF_DEBUG 0             // 0-off, 1-print, 2-additional lines to ED
#include "cttypes.h"
#include "edfile.h"
#include "msgerr.h"
#include <stdlib.h>

//*****************************************************************************
#define NATURAL uint
#define GOOD_PROB	70

/* Scan codes for keyboard */
#define BACK_SPACE 0x0e
#define ENTER      0x1c
#define ESCAPE     0x01
#define UP_ARROW   0x48
#define LEFT_ARROW 0x4b
#define RIGHT_ARROW 0x4d
#define DOWN_ARROW 0x50
#define INS	   0x52
#define F1	   0x3b
#define F2	   0x3c
#define F3	   0x3d
#define F4	   0x3e
#define F5	   0x3f
#define ALT_X	   0x2d

typedef struct sheet_disk_descr SDD; // Header of ED-file
typedef struct fragm_disk_descr FDD; // Fragment descriptor in header of file
typedef struct fragm_disk FD; // Begin-of-fragment record
typedef struct line_beg LB; // Begin_of_line record
typedef struct bit_map_ref BMR; // Bitmap reference record

//*****************************************************************************
// Part descriptor
typedef struct d_dp {
	uchar type;
#define VERT     0x01    // vertical splitting
#define HOR      0x02    // horisontal splitting
#define NONSP    0x04    // nonsplittable part
#define TERM     0x08    // terminal part
#define NEW      0x20    // not yet splitted
#define NONREST  0x40    // not yet restructed
	int16_t x, w, y, h; // coordinates and sizes
	struct d_dp *parent; // encompassing part
	struct d_dp *next; // next and previsious parts
	struct d_dp *prev; //   of the same level
	struct d_dp *first; // first and last subparts
	struct d_dp *last; //  (if not terminal)
	int16_t npart; // number of subparts
	struct d_df *fr1; // first fragment
	struct d_df *fr2; // next of last fragment
	int16_t nfrag; // number of fragments;
} DP;

//*****************************************************************************
// Fragment descriptor in memory
typedef struct d_df {
	struct d_df* next; // Next fragment in the chain
	struct d_df* prev; // Prev fragment in the chain
	int16_t x, w; // Horizontal position of alignement borders in TIF
	int16_t y, h; // Vertical position
	FDD* fdd; // FDD address
	struct d_dl* line1; // First line address
	struct d_dl* line2; // Last  line address
	struct d_dp* part; // Terminal part adress
	int16_t num; // Number of fragment givet automatically
	int16_t user_num; // Number of fragment given by user
	int16_t lplain; // Width of fragment in ED text mode
	uint16_t type;
#define ONE_LINE      0x0001
#define LEFT_ALLIGN   0x0002
#define RIGHT_ALLIGN  0x0004
#define CENTER        0x0008
#define POS_INDENT    0x0010
#define NEG_INDENT    0x0020
#define BULLET        0x0040
#define POESY         0x0080
#define LN_SPACE      0x0100
#define NOT_RECOG     0x0200
#define BORDER        0x0400
#define BRACKET       0x8000
#define MCOL_BEG      (BRACKET|0x1000)
#define NEW_COL       (BRACKET|0x2000)
#define MCOL_END      (BRACKET|0x3000)
#define TAB_BEG       (BRACKET|0x4000)
#define NEW_ROW       (BRACKET|0x5000)
#define DEL           (NOT_RECOG|BORDER)
#define FICT_FR_FLAGS (BORDER|BRACKET)

	int16_t base; // Regular margin from 'x'
	int16_t xmin; // Calculated minimum x-coord for fragment
	struct d_df *rel_fr; // for centered fragment - centered parent
	// TAB_BEG - list of cells sizes (x,w)
	int16_t pnum; // if TAB_BEG | MCOL_BEG - number of cells/columns
} DF;

#define fict_fr(a) (a->type & (FICT_FR_FLAGS))

// Line descriptor
typedef struct d_dl {
	struct d_dl*next; // Adress of the next line in the column
	struct d_dl*prev; // Adress of the prev line in the column
	int16_t baseline; // 3d baseline position (ideal)
	int16_t upper; // Upper boundary of the line
	uchar height; // Height beetween 2nd and 3rd baselines
	uchar pitch; // Pitch - number of plain characters per inch or 0.
	int16_t x, w; // Horizontal position
	uchar paragraph;
#define ORD_LN   0
#define NEW_PAR  0x1        // new paragraph
#define BUL_PAR  0x2        // paragraph with bullet
	int16_t indent; // size of indent (only if paragraph)
	int16_t bulind; // size of bullet indent (only if bullet)
	uint16_t type; // 0 for deleted line
#define DL_HUGE_XW   0x1      // XW changed due to huge letter
#define DL_HUGE      0x2      // Huge letter
#define DL_ASSEMBLY  0x4      // Line assembled from pieces
#define DL_SINGLE    0x8      // Single-letter line
	uchar* edbeg; // Address of the ED-text
	int16_t edlth; // It's length
	int16_t npieces; // Number of original pieces within line - 1
	int16_t lplain; // Number of print characters including spaces
} DL;

// RTF segment header
// typedef struct d_seg
//    {
//    void*   next;           // Next segment
//    void*   begtext;        // Begin of used space in the segment
//    void*   endtext;        // End of used space in the segment
//    int16_t     flag;
// #define SEG_MALLOC 1        // Segment was created by malloc
//    } SEG;

struct line_ref {
	struct line_ref * next;
	struct line_ref * prev;
	DF * frag;
	DL * line;
	int16_t finy;
	int16_t x;
	uint16_t width;
	uint16_t indent;
	uint16_t base;
	uchar flag;
	uint16_t type; // Copy of fragment's type
	uchar paragraph;
	uchar res[3];
};
typedef struct line_ref LREF;

//*****************************************************************************
typedef struct {
	int16_t dpi; // Resolution of scanner in dots per inch
	int16_t turn; // Turn of page: turn=(xideal-xreal)/yreal*2048=
	//                   =(yreal-yideal)/xreal*2048
	// Symbol substitutions
	char subst_bad; // Subst. for not recognized character
	char subst_dash[3]; // Subst. for dash

	// E	D
	int16_t edfile; // ED-file handle
	uchar *edbeg; // Begin address of the ED file
	uchar *edcur; // Current address in the ED-file
	uchar *edfree; // End ptr to ED-file
	SDD* psdd; // ED header
	int16_t font; // Font properties as in ED struct font_kegle

#define FONT_SERIFIC       1
#define FONT_HELVETIC      2
#define FONT_BOLD          4
#define FONT_LIGHT         8
#define FONT_ITALIC       16
#define FONT_STRAIGHT     32
#define FONT_UNDERLINE    64

	// Parts
	DP* part; // Table of parts

	// Fragments
	DF* frag; // Table of fragments
	DF* pf; // Current frag address
	int16_t nf; // It's number
	int16_t nfrag; // No of fragments

	// Lines
	DL* line; // Table of lines
	DL* pl; // Current line desc address
	int16_t nl; // It's number
	int16_t qlines; // No of lines

	// Graph of fragments
	int16_t* graph; // Array of exits
	int16_t qgr; // No of used elements in array


	// Debug
	int16_t debug; // 1-debug activated
	FILE *prt; // Debug print file

	//  External memory pool
	uchar *begfree;
	uchar *endfree;

	// Control of dynamic memory taken by malloc's
	uint32_t dynmem_size; // Total count of dynamic memory size
	int16_t tables_in_dynamic_memory; // Flag =1 if tables are in dynamic mem
	int16_t ed_in_dynamic_memory; // Flag =1 if ED is in dynalic memory


	// Follow coordinates in TIFF
	uint16_t page_pix_width; // page width in pixels
	uint16_t page_width; // page width in twips
	uint16_t column_width; // current column width
	uint16_t column_width_save; // column width saved
	uint16_t columns_quant; // number of columns

	// Tables support
	PWORD cells_w; // cell widthes array
	PWORD cells_x; // x of cells
	PWORD cells_g; // gaps of cells
	uint16_t cells_quant; // number of cells
	uint16_t current_cell; // current cell number
	uchar section_fill; // non empty section flag
	uchar table_flag; // table flag

	// ED save working area
	int16_t Xmin; // Minimal left coordinate of all fragments
	int16_t Xmax; // Maximal right coordinate of all fragments
	PWORD char_tab; // Tabulation table pointer
	DF ** sort_frag; // List of sorted by upper fragment
	uint16_t cn; // Local fragment counter for sortlist
	int16_t cy; // Current 'y' for plank
	int16_t finy; // Minimal of rows to finish active lines
	LREF * cfirst; // First fictive element in lines chain
	LREF * clast; // Last fictive element in lines chain
	LREF * cfree; // Free lines reference chain start
	LREF * collect; // Pointer to collect line

	// Line RTF buffer uses memory for graph during scan of ED file
	uchar *rtfbeg; // Address of buffer= address of graph
	uchar *rtfcur; // Pointer to free space in the buffer
	int32_t rtflth; // Length of text in buffer
	uchar *rtffree;
	uchar *rtfend;
	uchar rtfspec;
	uchar rtflchar;
	puchar rtf_hugep;
	uchar rtf_hugelist[2];
#define RTF_LINE_BUF_SIZE sizeof(L.graph)

	puchar hyp_buff; // Pointer to hyphination buffer start
	puchar hyp_ptr; // Pointer to current place in that buffer
	puchar hyp_in_rtf; // Pointer to defis in RTF file if it is last in a string
	puchar hyp_last; // Pointer to last defis in RTF file
	puchar hyp_line_start; // Pointer to first character of a line in RTF file


} dL;
//*****************************************************************************

int16_t ed_scan(Bool(*func_spec)(puchar p, int16_t lth), Bool(*func_char)(puchar p,
		int16_t lth), puchar start, puchar end);

#if RTF_DEBUG!=0
#define PGM " "
#define MSG(s)               if(L.debug)fprintf(L.prt,"\n" PGM s)
#define MSG2(s1,s2)          if(L.debug)fprintf(L.prt,"\n" PGM s1,s2)
#define MSG3(s1,s2,s3)       if(L.debug)fprintf(L.prt,"\n" PGM s1,s2,s3)
#define MSG4(s1,s2,s3,s4)    if(L.debug)fprintf(L.prt,"\n" PGM s1,s2,s3,s4)
#define MSG5(s1,s2,s3,s4,s5) if(L.debug)fprintf(L.prt,"\n" PGM s1,s2,s3,s4,s5)
#else
#define MSG(s)               /* */
#define MSG2(s1,s2)          /* */
#define MSG3(s1,s2,s3)       /* */
#define MSG4(s1,s2,s3,s4)    /* */
#define MSG5(s1,s2,s3,s4,s5) /* */
#endif
//*****************************************************************************
#define msg_6_1 "RTF: Open error in RTF-file."
#define msg_6_2 "RTF: Write error in RTF-file."
#define msg_6_3 "RTF: Out of memory."			/**/
#define msg_6_4 ""
#define msg_6_5 "RTF: Open error in ED-file."
#define msg_6_6 "RTF: Read error in ED-file."		/**/
#define msg_6_7 "RTF: Incorect length of ED-file header."
#define msg_6_8 "RTF: Too many lines."
#define msg_6_9 "RTF: ED-file is corrupted."
#define msg_6_10 "RTF: Too long line."
#define msg_6_11 "RTF: Write error in ED-file."
#define msg_6_12 "RTF: Too many fragments."
#define msg_6_13 "RTF: More fragments in ED-file than shown in header."
#define msg_6_14 "RTF: Duplicated fragment in table."

#endif
