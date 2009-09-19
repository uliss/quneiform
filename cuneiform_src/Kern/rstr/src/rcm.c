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

// делать кластеризацию в памяти - не делать .ctb
#ifdef NDEBUG
#define _FON_CLU_MEMORY_1
#else
#define _FON_CLU_MEMORY_1
#endif
#define _STORE_LETTERS1

#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <fcntl.h>
#include "leo.h"
#include "fon.h"
#include "std.h"
#include "rstr.h"
#include "ctb.h"
#include "cstr.h"
#include "ccom.h"
#include "resource.h"
#include "common.h"
#include "lang_def.h"
#include "crling.h"
#include "dpuma.h"
#include "cline.h"

#include "compat_defs.h"

#define LOCAL_GREY_CTB "page6666"
#define LOCAL_CTB_NAME "ct666666"
unsigned char alpha_used_mode = 0;
/* FIXME: to compile under MS VC++, look at /ctb/src/ctb_tool.c */
/*
 extern char    local_grey_ctb[];
 extern char    local_ctb_name[];
 */
char local_grey_ctb[256] = "page6666";
char local_ctb_name[256] = "ct666666";

uchar * (*local_ret_error_str)(uint32_t dwError);
uint32_t local_ret_error_code = 0;
typedef uchar *(*fun_error)(uint32_t);
static uchar lnOcrPath[256], lnOcrLingPath[256];
static void store_colors(CSTR_line lino);
static void * rstr_realloc(uchar*buf, uint32_t len) {
	return realloc(buf, len);
}
static void * rstr_alloc(uint32_t len) {
	return calloc(len, 1);
}
static void rstr_free(void *ptr, uint32_t len) {
	free(ptr);
}
;
static void rstr_get_colors(int16_t row, int16_t col, int16_t w, int16_t h,
		int32_t *ColorLtr, int32_t *ColorBack) {
	*ColorBack = 0xFFFFFF;
	*ColorLtr = 0;
}
;
static void * (*my_realloc)(uchar*buf, uint32_t len) = rstr_realloc;
static void * (*my_alloc)(uint32_t len) = rstr_alloc;
static void (*my_free)(void *, uint32_t len) = rstr_free;
static void (*my_get_colors)(int16_t row, int16_t col, int16_t w, int16_t h,
		int32_t *ColorLrt, int32_t *ColorBack) = rstr_get_colors;
static int32_t RemoveDustIfPointLine(CSTR_line lin);
// RSTR_CON
int16_t rstr_cont_store1(RecRaster *r, uchar let, uchar nLns, Rect16 *rect,
		uchar IsPrint, uchar Prob, uchar Valid, RecVersions *v, uchar control,
		uchar kegl);
Bool32 rstr_open_cont1(void);
void rstr_close_cont1(void);
// P2_cour
Bool32 TestFontCourier(void);
Bool32 TestFontProtocol(void);
Bool32 p2_msk_init();
void p2_msk_done(void);
// p2_bold
void p2_BoldInit(int);
// linutil.c
void BaseLineStatisticInit(void);
// MMX_ADDR.C
void set_MMX_addr(void);
void set_all_addr(void);
// from ERECTION.C
extern int16_t erection_inc;
// from Acc_tabs.c
void correct_let_tables(void);
// from DFON.DLL
extern void DFon_NewPage(char*);
extern void DFON_Done(void);
// from EVN.DLL
extern Bool32 EVNInitLanguage(const char *tabevn1, const char *tabevn2,
		uchar lang);
extern Bool32 EVNSetLanguage(uchar lang);
extern Bool32 EVNSetAlphabet(char* char_tbl_put_to);
// from PROP.C
extern void prop_init(void);
// from PASS3.C
uchar convert_eng_liga(uchar c);

#define  CREATE_STATUS
#define  __KERNEL__
#include "status.h"
#include "func.h"
#include "ligas.h"
#include "alik_rec.h"
#include  "alphabet.h"

Bool32 enable_pass2 = TRUE;
static Bool32 stop_user = FALSE;
uchar valid_word_number = 0;
version * start_rec, *rec_ptr;
int16_t text_findstat(char * w);
uchar db_pass;
uchar string[8192];
uint16_t lpool_lth;
uchar lpool[LPOOL_SIZE];
int32_t Flag_Courier = FALSE;
uchar evfulln;
uchar double_fax; // setup in RSTR_SetOptions
uchar db_trace_flag;
uint16_t actual_resolution = 300; // setup in RSTR_SetOptions
#define MAX_LINE_COUNT 5000
STRLN page_lines[MAX_LINE_COUNT];
int32_t num_of_lines;
uchar decode_ASCII_to_[256][4] = {
/*       0      1      2      3      4      5      6      7      8      9      a      b      c      d      e      f  */
/* 0*/"\x00", "\x01", "\x02", "\x03", "\x04", "\x05", "\x06", "\x07", "\x08",
		"\x09", "\x0a", "\x0b", "\x0c", "\x0d", "\x0e", "\x0f",/* 0*/
		/* 1*/"\x10", "\x11", "\x12", "\x13", "\x14", "\x15", "\x16", "\x17",
		"\x18", "\x19", "\x1a", "\x1b", "\x1c", "\x1d", "\x1e", "\x1f",/* 1*/
		/* 2*/"\x20", "\x21", "\x22", "\xb9", "\x24", "\x25", "\x26", "\x27",
		"\x28", "\x29", "\x2a", "\x2b", "\x2c", "\x2d", "\x2e", "\x2f",/* 2*/
		/* 3*/"\x30", "\x31", "\x32", "\x33", "\x34", "\x35", "\x36", "\x37",
		"\x38", "\x39", "\x3a", "\x3b", "\x3c", "\x3d", "\x3e", "\x3f",/* 3*/
		/* 4*/"\x40", "\x41", "\x42", "\x43", "\x44", "\x45", "\x46", "\x47",
		"\x48", "\x49", "\x4a", "\x4b", "\x4c", "\x4d", "\x4e", "\x4f",/* 4*/
		/* 5*/"\x50", "\x51", "\x52", "\x53", "\x54", "\x55", "\x56", "\x57",
		"\x58", "\x59", "\x5a", "\x5b", "\x5c", "\x5d", "\x5e", "—", /* 5*/
		/* 6*/"\x60", "\x61", "\x62", "\x63", "\x64", "\x65", "\x66", "\x67",
		"\x68", "\x69", "\x6a", "\x6b", "\x6c", "\x6d", "\x6e", "\x6f",/* 6*/
		/* 7*/"\x70", "\x71", "\x72", "\x73", "\x74", "\x75", "\x76", "\x77",
		"\x78", "\x79", "\x7a", "\x7b", "\x7c", "\x7d", "\x7e", "\x7f",/* 7*/
		/* 8*/"\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5", "\xc6", "\xc7",
		"\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd", "\xce", "\xcf",/* 8*/
		/* 9*/"\xd0", "\xd1", "\xd2", "\xd3", "\xd4", "\xd5", "\xd6", "\xd7",
		"\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd", "\xde", "\xdf",/* 9*/
		/* a*/"\xe0", "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6", "\xe7",
		"\xe8", "\xe9", "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef",/* a*/
		/* b*/"\x7e", "\x20", "\x20", "\x20", "\x20", "\x20", "\x20", "\x20",
		"\x20", "\x20", "\x20", "\x20", "\x21", "\x3f", "\x20", "\x20",/* b*/
		/* c*/"\xB8", "\x20", "\xbf", "\x20", "\x20", "\xaf", "\xb2", "\xa5",
		"\xA8", "\xba", "\x20", "\x20", "\xaa", "\x20", "\x20", "\x20",/* c*/
		/* d*/"\x99", "\x20", "\x20", "\x20", "\x20", "\x22", "\xb4", "\xab",
		"\xbb", "\x20", "\xb3", "\x20", "\x20", "\xa9", "\xae", "\x20",/* d*/
		/* e*/"\xf0", "\xf1", "\xf2", "\xf3", "\xf4", "\xf5", "\xf6", "\xf7",
		"\xf8", "\xf9", "\xfa", "\xfb", "\xfc", "\xfd", "\xfe", "\xff",/* e*/
		/* f*/"\xe4", "\xe4", "\x20", "\x20", "\x20", "\xf2", "\x20", "\xe8",
		"\xe3", "\x20", "\x20", "\x20", "\x20", "\xe0", "\x95", "\x20" /* f*/
};
uchar decode_rus_ASCII_to_[256][4] = {
/*       0    1      2      3      4      5      6      7      8      9      a      b      c      d      e      f  */
/* 0*/"\x00", "\x01", "\x02", "\x03", "\x04", "\x05", "\x06", "\x07", "\x08",
		"\x09", "\x0a", "\x0b", "\x0c", "\x0d", "\x0e", "\x0f",
		/* 1*/"\x10", "\x11", "\x12", "\x13", "\x14", "\x15", "\x16", "\x17",
		"\x18", "\x19", "\x1a", "\x1b", "\x1c", "\x1d", "\x1e", "\x1f",
		/* 2*/"\x20", "\x21", "\x22", "\xb9", "\x24", "\x25", "\x26", "\x27",
		"\x28", "\x29", "\x2a", "\x2b", "\x2c", "\x2d", "\x2e", "\x2f",
		/* 3*/"\x30", "\x31", "\x32", "\x33", "\x34", "\x35", "\x36", "\x37",
		"\x38", "\x39", "\x3a", "\x3b", "\x3c", "\x3d", "\x3e", "\x3f",
		/* 4*/"\x40", "\x41", "\x42", "\x43", "\x44", "\x45", "\x46", "\x47",
		"\x48", "\x49", "\x4a", "\x4b", "\x4c", "\x4d", "\x4e", "\x4f",
		/* 5*/"\x50", "\x51", "\x52", "\x53", "\x54",
		"\x55",
		"\x56",
		"\x57",
		"\x58",
		"\x59",
		"\x5a",
		"\x5b",
		"\x5c",
		"\x5d",
		"\x5e",
		"—",//"\x2d\x2d",
		/* 6*/"\x60", "\x61", "\x62", "\x63", "\x64", "\x65", "\x66", "\x67",
		"\x68", "\x69", "\x6a", "\x6b", "\x6c", "\x6d", "\x6e", "\x6f",
		/* 7*/"\x70", "\x71", "\x72", "\x73", "\x74", "\x75", "\x76", "\x77",
		"\x78", "\x79", "\x7a", "\x7b", "\x7c", "\x7d", "\x7e", "\x7f",
		/* 8*/"\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5", "\xc6", "\xc7",
		"\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd", "\xce", "\xcf",
		/* 9*/"\xd0", "\xd1", "\xd2", "\xd3", "\xd4", "\xd5", "\xd6", "\xd7",
		"\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd", "\xde", "\xdf",
		/* a*/"\xe0", "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6", "\xe7",
		"\xe8", "\xe9", "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef",
		/* b*/"\x7e", "\x20", "\x20", "\x20", "\x20", "\x20", "\x20", "\x20",
		"\x20", "\x20", "\x20", "\x20", "\x21", "\x3f", "\x20", "\x20",
		/* c*/"\xB8", "\x20", "\xbf", "\x20", "\x20", "\xaf", "\xb2", "\xa5",
		"\xA8", "\xba", "\x20", "\x20", "\xaa", "\x20", "\x20", "\x20",
		/* d*/"\x99", "\x20", "\x20", "\x20", "\x20", "\x22", "\xb4", "\xab",
		"\xbb", "\x20", "\xb3", "\x20", "\x20", "\xa9", "\xae", "\x20",
		/* e*/"\xf0", "\xf1", "\xf2", "\xf3", "\xf4", "\xf5", "\xf6", "\xf7",
		"\xf8", "\xf9", "\xfa", "\xfb", "\xfc", "\xfd", "\xfe", "\xff",
		/* f*/"\xe4", "\xe4", "\x20", "\x20", "\x20", "\xf2", "\x20", "\xe8",
		"\xe3", "\x20", "\x20", "\x20", "\x20", "\xe0", "\x95", "\x20" };

char alphabet[256];
uchar CodePages[LANG_TOTAL] = { // 29.08.2000 E.P.
		CSTR_ANSI_CHARSET, // LANG_ENGLISH    0
				CSTR_ANSI_CHARSET, // LANG_GERMAN     1
				CSTR_ANSI_CHARSET, // LANG_FRENCH     2
				CSTR_RUSSIAN_CHARSET, // LANG_RUSSIAN    3
				CSTR_ANSI_CHARSET, // LANG_SWEDISH    4
				CSTR_ANSI_CHARSET, // LANG_SPANISH    5
				CSTR_ANSI_CHARSET, // LANG_ITALIAN    6
				CSTR_RUSSIAN_CHARSET, // LANG_RUSENG     7
				CSTR_RUSSIAN_CHARSET, // LANG_UKRAINIAN  8
				CSTR_RUSSIAN_CHARSET, // LANG_SERBIAN    9
				CSTR_EASTEUROPE_CHARSET, // LANG_CROATIAN   10
				CSTR_EASTEUROPE_CHARSET, // LANG_POLISH     11
				CSTR_ANSI_CHARSET, // LANG_DANISH     12
				CSTR_ANSI_CHARSET, // LANG_PORTUGUESE 13
				CSTR_ANSI_CHARSET, // LANG_DUTCH      14
				CSTR_ANSI_CHARSET, // LANG_DIG        15
				CSTR_RUSSIAN_CHARSET, // LANG_UZBEK	  16 // 01.09.2000 E.P.
				CSTR_RUSSIAN_CHARSET, // LANG_KAZ	      17
				CSTR_RUSSIAN_CHARSET, // LANG_KAZ_ENG    18
				CSTR_EASTEUROPE_CHARSET, // LANG_CZECH	  19
				CSTR_EASTEUROPE_CHARSET, // LANG_ROMAN	  20
				CSTR_EASTEUROPE_CHARSET, // LANG_HUNGAR	  21
				CSTR_RUSSIAN_CHARSET, // LANG_BULGAR	  22
				CSTR_EASTEUROPE_CHARSET, // LANG_SLOVENIAN  23
				BALTIC_CHARSET, // LANG_LATVIAN	  24
				BALTIC_CHARSET, // LANG_LITHUANIAN 25
				BALTIC_CHARSET, // LANG_ESTONIAN	  26
				TURKISH_CHARSET // LANG_TURKISH	  27
		};

static char *tab3x5[] = { "rec3.dat", // LANG_ENGLISH   0
		"rec3.dat", // LANG_GERMAN    1
		"rec3.dat", // LANG_FRENCH    2
		"rec3rus.dat", // LANG_RUSSIAN   3
		"rec3.dat", // LANG_SWEDISH   4
		"rec3.dat", // LANG_SPANISH   5
		"rec3.dat", // LANG_ITALIAN   6
		"rec3r&e.dat", // LANG_RUSENG    7
		"rec3rus.dat", // LANG_UKRAINIAN 8
		"rec3rus.dat", // LANG_SERBIAN   9
		"rec3cen.dat", // LANG_CROATIAN  10
		"rec3cen.dat", // LANG_POLISH    11
		"rec3n.dat", // LANG_DANISH    12
		"rec3n.dat", // LANG_PORTUGUESE  13
		"rec3n.dat", // LANG_DUTCH		14
		"rec3.dat", // LANG_DIG			15
		"rec3uzb.dat", // LANG_UZBEK		16	// 01.09.2000 E.P.
		"rec3kaz.dat", // LANG_KAZ			17
		"rec3kaz.dat", // LANG_KAZ_ENG		18
		"rec3cen.dat", // LANG_CZECH		19
		"rec3cen.dat", // LANG_ROMAN		20
		"rec3cen.dat", // LANG_HUNGAR		21
		"rec3rus.dat", // LANG_BULGAR		22
		"rec3cen.dat", // LANG_SLOVENIAN	23
		"rec3blt.dat", // LANG_LATVIAN	    24
		"rec3blt.dat", // LANG_LITHUANIAN  25
		"rec3blt.dat", // LANG_ESTONIAN	26
		"rec3tur.dat" // LANG_TURKISH		27
		};

static char *tabevn1[] = { "rec1.dat", // LANG_ENGLISH   0
		"rec1.dat", // LANG_GERMAN    1
		"rec1.dat", // LANG_FRENCH    2
		"rec1rus.dat", // LANG_RUSSIAN   3
		"rec1.dat", // LANG_SWEDISH   4
		"rec1.dat", // LANG_SPANISH   5
		"rec1.dat", // LANG_ITALIAN   6
		"rec1r&e.dat", // LANG_RUSENG    7
		"rec1rus.dat", // LANG_UKRAINIAN 8
		"rec1rus.dat", // LANG_SERBIAN   9
		"rec1cen.dat", // LANG_CROATIAN  10
		"rec1cen.dat", // LANG_POLISH    11
		"rec1n.dat", // LANG_DANISH    12
		"rec1n.dat", // LANG_PORTUGUESE  13
		"rec1n.dat", // LANG_DUTCH       14
		"rec1.dat", // LANG_DIG         15
		"rec1uzb.dat", // LANG_UZBEK		16	// 01.09.2000 E.P.
		"rec1kaz.dat", // LANG_KAZ			17
		"rec1kaz.dat", // LANG_KAZ_ENG		18
		"rec1cen.dat", // LANG_CZECH		19
		"rec1cen.dat", // LANG_ROMAN		20
		"rec1cen.dat", // LANG_HUNGAR		21
		"rec1rus.dat", // LANG_BULGAR		22
		"rec1cen.dat", // LANG_SLOVENIAN	23
		"rec1blt.dat", // LANG_LATVIAN	    24
		"rec1blt.dat", // LANG_LITHUANIAN  25
		"rec1blt.dat", // LANG_ESTONIAN	26
		"rec1tur.dat" // LANG_TURKISH		27
		};
static char *tabevn2[] = { "rec2.dat", // LANG_ENGLISH   0
		"rec2.dat", // LANG_GERMAN    1
		"rec2.dat", // LANG_FRENCH    2
		"rec2rus.dat", // LANG_RUSSIAN   3
		"rec2.dat", // LANG_SWEDISH   4
		"rec2.dat", // LANG_SPANISH   5
		"rec2.dat", // LANG_ITALIAN   6
		"rec2r&e.dat", // LANG_RUSENG    7
		"rec2rus.dat", // LANG_UKRAINIAN 8
		"rec2rus.dat", // LANG_SERBIAN   9
		"rec2cen.dat", // LANG_CROATIAN  10
		"rec2cen.dat", // LANG_POLISH    11
		"rec2n.dat", // LANG_DANISH    12
		"rec2n.dat", // LANG_PORTUGUESE	13
		"rec2n.dat", // LANG_DUTCH		14
		"rec2.dat", // LANG_DIG			15
		"rec2uzb.dat", // LANG_UZBEK		16 // 01.09.2000 E.P.
		"rec2kaz.dat", // LANG_KAZ			17
		"rec2kaz.dat", // LANG_KAZ_ENG		18
		"rec2cen.dat", // LANG_CZECH		19
		"rec2cen.dat", // LANG_ROMAN		20
		"rec2cen.dat", // LANG_HUNGAR		21
		"rec2rus.dat", // LANG_BULGAR		22
		"rec2cen.dat", // LANG_SLOVENIAN	23
		"rec2blt.dat", // LANG_LATVIAN	    24
		"rec2blt.dat", // LANG_LITHUANIAN  25
		"rec2blt.dat", // LANG_ESTONIAN	26
		"rec2tur.dat" // LANG_TURKISH		27
		};

uchar * CellsPage_rstr, *CellsPageEnd_rstr;
uchar * MemForCutPoints, *MemForCutPointsTwo;
uchar * ForRaster1;
uchar * ForRaster2;
uchar * ForRaster3;
uchar * ED_file_bound, *ED_file_end;
puchar kit_curr, kit_end, kit_start;
int32_t kit_size = 65536* 4 ;
Bool32 kit_max_size = FALSE;
uint16_t wHeightRC = 0;
uint16_t wLowRC = RSTR_ERR_NO;

#define FIELD_RESTART 1
void ErrorExit(int Code);
void pass3(CSTR_line lin, CSTR_line lino);
void correct_letters_pidx_table(void);
//extern "C" Bool16 matrix_field();
//extern "C" Bool16 not_matrix_field();

//=============== Global data ==============
Err16 rcmErr;

int16_t nIncline; // setup in RcmSetup
int16_t current_fragment; // setup in RSTR_SetOptions
int16_t iFont = 0;
int16_t line_number;
Bool pass4_in;
MN * main_number_ptr;
BOX * boxchain, *dl_last_in_chain;
c_comp wcomp;
uchar work_raster[2048* 4 ], work_raster_1[2048*4];
//uchar language; // setup in RSTR_SetOptions
FIELD_INFO FieldInfo;
Bool FirstField;
int32_t small_size;
Bool16 first_number; // OLEG : 26-10-1998 : best cutting of glued "#"

//=============== External data ==============
extern cell *stopcell;
extern Bool snap_page_disable;
extern int16_t page_nIncline;
//=============== Local data ==============
static jmp_buf jumper;
static int32_t nResolutionY = 300; // setup in RSTR_SetOptions

//=============== Code implementation ======
puchar tableBOX = NULL; /* BOX table memory start */
puchar box_pool = NULL; /* boxes pool for extr_comp */
puchar memory_pool = NULL, memory_pool_end = NULL;
extern puchar load_BOX(puchar end);
int32_t memory_length;
puchar fontBOX = NULL;
uchar line_scale = 0, line_alphabet = 0, line_minus = 0, line_pointsusp = 0; // need setup after calculation in ExStr
Bool line_readyBL = FALSE, line_BL = FALSE, line_handfragment = FALSE,
		line_rerecog = FALSE;
uchar line_tabcell = 0;

#define MEMORY                  0x50000 //500000        //0x450000
Bool32 read_rec_file(int16_t lang, puchar pool, puchar * end) {
	uint32_t l;
	int16_t h;
	int32_t size;

#ifndef O_BINARY /* This is defined only in Windows. */
#define O_BINARY 0
#endif

	h = open_data_file(tab3x5[lang], O_RDONLY | O_BINARY);
	if (h == -1) {
		return FALSE;
	}
	size = filelength(h);
	l = read(h, pool, size);
	close(h);
	l = (l + 15) & -16;
	*end = pool + l;
	return TRUE;
}
static puchar box_pool_save;
Bool32 trees_load(void) {
	int lang = language;
	if (language >= LANG_TOTAL)
		return FALSE;
	if (language == LANG_RUSSIAN && multy_language)
		lang = LANG_RUSENG;
	//if( !EVNInitLanguage( tabevn1[lang], tabevn2[lang],language) )
	//   return FALSE;

	if (!read_rec_file((uchar) lang, tableBOX, &box_pool))
		return FALSE;
	memset(tableBOX, 0, 32);
	box_pool_save = box_pool;
	box_pool = load_BOX(box_pool);
	box_pool += (memory_pool - box_pool) & 0xf;
	if (!EVNSetAlphabet(alphabet))
		return FALSE;
	EVNSetLanguage(language);
	correct_let_tables();
	return TRUE;
}

Bool32 trees_load_fict(void) {
	correct_let_tables();
	if (!EVNSetAlphabet(alphabet))
		return FALSE;
	EVNSetLanguage(language);
	return TRUE;
}

// call trees_load_fict() restored standart alphabet
Bool32 set_user_alphabet(uchar * usa_ascii) {
	uchar *pusa;
	alpha_used_mode = 1;
	memset(alphabet, 0, 256);
	for (pusa = usa_ascii; *pusa; pusa++) {
		alphabet[*pusa]++;
	}
	EVNSetAlphabet(alphabet);
	return TRUE;
}

RSTR_FUNC(Bool32) RSTR_IsLanguage(uchar language)
{
	if(language<LANG_ENGLISH || language>=LANG_TOTAL )
	return FALSE;
	chdir(lnOcrPath);
	if(data_file_exists(tabevn1[language])==-1 )
	return FALSE;
	if(data_file_exists(tabevn2[language])==-1 )
	return FALSE;
	if(data_file_exists(tab3x5[language])==-1 )
	return FALSE;
	if(language==LANG_RUSSIAN || language==LANG_RUSENG )
	{
		if( 0 )
		return FALSE;
	}
#ifdef     _USE_SPELLING_
	if( language==LANG_RUSENG )
	{
		if( RLING_IsDictonaryAvailable( LANG_RUSSIAN , (pchar)lnOcrLingPath)<1 ||
				RLING_IsDictonaryAvailable( LANG_ENGLISH , (pchar)lnOcrLingPath)<1)
		return FALSE;
	}
	else //if( language!=LANG_DIG )
	{
		if( RLING_IsDictonaryAvailable( language , (pchar)lnOcrLingPath)<1 )
		return FALSE;
	}
#endif
	return TRUE;
}

static void SetAlphabet(char char_tbl_put_to[] // char string
) // Set alphabet for recognition

{
	Bool digital = TRUE, Punct = FALSE;
	char *c;

	memset(&FieldInfo.AlphaTable, 0, sizeof(ALPHA_TABLE));

	for (c = (char *) char_tbl_put_to; *c != 0; c++) {
		digital &= ((*c) & 0xE0) == 0x20 || (*c) == '\\' || (*c) == '_' || (*c)
				== ' ';
		Punct |= strchr("\"*+-.,:;=<>„«»", *c) != NULL;
		FieldInfo.AlphaTable[(uchar)(*c)] = 1;
	}
	if (digital)
		FieldInfo.Style |= FIS_DIGIT;
	if (Punct)
		FieldInfo.Style |= FIS_PUNCT;

}

Bool32 rstr_kit_realloc(void) {
	kit_max_size = TRUE;
	kit_size *= 2;
	kit_start = my_realloc(kit_start, kit_size);
	if (kit_start == NULL) {
		wLowRC = RSTR_ERR_NOMEMORY;
		return FALSE;
	}
	kit_curr = kit_start;
	kit_end = kit_start + kit_size;
	return TRUE;
}

RSTR_FUNC(Bool32) RSTRSnapInit( void )
{
	snap_init();
	snap_page_disable=FALSE;
	return TRUE;
}

RSTR_FUNC(Bool32) RSTRInit( MemFunc* mem )
{
	snap_init();
	local_ret_error_code=0;
	wLowRC = RSTR_ERR_NO;
	line_number=0;
	CellsPage_rstr = my_alloc(10*65536+3*R_S);
	if (CellsPage_rstr==NULL) {
		wLowRC = RSTR_ERR_NOMEMORY;
		fprintf(stderr, "RSTR_ERR_NOMEMORY");
		return FALSE;
	}
	CellsPageEnd_rstr = CellsPage_rstr + 65536*4;
	MemForCutPoints = CellsPageEnd_rstr;
	MemForCutPointsTwo = MemForCutPoints + 65536;
	ForRaster1 = MemForCutPointsTwo + 65536;
	ForRaster2 = ForRaster1 + R_S;
	ForRaster3 = ForRaster2 + R_S;
	ED_file_bound = ForRaster3 + R_S;
	ED_file_end = ED_file_bound + 65536*4;
	kit_start = my_alloc(kit_size);
	if (kit_start==NULL) {
		wLowRC = RSTR_ERR_NOMEMORY;
		fprintf(stderr, "RSTR_ERR_NOMEMORY");
		return FALSE;
	}
	kit_curr = kit_start;
	kit_end = kit_start + kit_size;
	num_of_lines=0;
	Flag_Courier=FALSE;
	snap_page_disable = FALSE;
	memory_length=MEMORY;
	memory_pool=my_alloc(memory_length);
	if (memory_pool==NULL) {
		wLowRC = RSTR_ERR_NOMEMORY;
		fprintf(stderr, "RSTR_ERR_NOMEMORY");
		return FALSE;
	}
	memory_pool_end = (puchar)(memory_pool + memory_length);
	tableBOX = memory_pool;
	cell_f()->next =cell_l();
	cell_f()->nextl =cell_l();
	cell_l()->prev =cell_f();
	cell_l()->prevl =cell_f();
#ifdef _USE_FON_
	// готовый шрифт
	if( FONInit(local_grey_ctb)<0 ) {
		//ErrorExit(RSTR_ERR_INTERNAL);
		//return FALSE;
	}
#endif
	chdir(lnOcrPath);
#ifdef     _USE_SPELLING_
	if (!RLING_Init( 102, NULL ))
	{
		wLowRC = RSTR_ERR_NOINITRSTR;
		local_ret_error_code=RLING_GetReturnCode();
		local_ret_error_str =(fun_error)RLING_GetReturnString;
		fprintf(stderr, "RLING - RSTR_ERR_NOINITRSTR\n");
		return FALSE;
	}
#endif
#ifdef _USE_LEO_
	if( !LEOInit(0) ) {
		wLowRC = RSTR_ERR_NOINITRSTR;
		fprintf(stderr, "LEO - RSTR_ERR_NOINITRSTR\n");
		return FALSE;
	}
	LEOSetPlatform(LEOGetCPU());
#endif
#ifdef _USE_MSK_
	if( !p2_msk_init() ) {
		wLowRC = RSTR_ERR_NOINITRSTR;
		fprintf(stderr, "MSK - RSTR_ERR_NOINITRSTR\n");
		return FALSE;
	}
#endif
#ifdef SPEC_PROJECT1
	strcpy(decode_rus_ASCII_to_['('],"<");
	strcpy(decode_rus_ASCII_to_['{'],"<");
	strcpy(decode_rus_ASCII_to_['['],"<");
	strcpy(decode_rus_ASCII_to_[left_quocket],"<<");
	strcpy(decode_ASCII_to_['('],"<");
	strcpy(decode_ASCII_to_['{'],"<");
	strcpy(decode_ASCII_to_['['],"<");
	strcpy(decode_ASCII_to_[left_quocket],"<<");
#endif
	return TRUE;
}

RSTR_FUNC(Bool32) RSTRNewPage(int32_t resolutiony, Handle myPage )
{
#ifdef _USE_FON_
#ifndef _FON_CLU_MEMORY_
	rstr_close_cont();
#endif
#endif
	line_number=0;
	local_ret_error_code=0;
	no_linpen=0;
	FirstField=TRUE;
	pass4_in = FALSE;
	wLowRC = RSTR_ERR_NO;
	nResolutionY=resolutiony;
	kit_curr= kit_start;
	kit_end = kit_curr + kit_size;
	num_of_lines=0;
	prop_init();
	erect_init_global_tab();
	Flag_Courier=FALSE;
#ifdef _USE_FON_
#ifndef _FON_CLU_MEMORY_
	if( !rstr_open_cont() )
	{
		wLowRC = RSTR_ERR_INTERNAL;
		return FALSE;
	}
#endif
#endif
#ifdef _USE_CPAGE_
	if( myPage )
	{
		CLINE_handle hCLINE=CLINE_GetMainContainer();

		//	if(!LDPUMA_Skip(hUseCLine))
		//    {
		CLINE_handle hline;
		CPDLine cpdata;
		hline=CLINE_GetFirstLine(hCLINE);
		if(!hline)
		return TRUE;
		while(hline)
		{
			if(num_of_lines>=MAX_LINE_COUNT) // Anton 29.05.2001
			break;
			cpdata=CLINE_GetLineData(hline);
			if(!cpdata)
			{
				wLowRC = RSTR_ERR_INTERNAL;
				return FALSE;
			}
			else
			{
				if( (cpdata->Flags & LI_IsTrue) &&
						!(cpdata->Flags & LI_IsAtTable) &&
						(abs(cpdata->Line.Beg_X-cpdata->Line.End_X)>
								abs(cpdata->Line.Beg_Y-cpdata->Line.End_Y)) &&
						(cpdata->Dir==LD_Horiz) )
				{
					page_lines[num_of_lines].beg.y=(int16_t)(cpdata->Line.Beg_Y);
					page_lines[num_of_lines].beg.x=(int16_t)(cpdata->Line.Beg_X);
					page_lines[num_of_lines].end.y=(int16_t)(cpdata->Line.End_Y);
					page_lines[num_of_lines].end.x=(int16_t)(cpdata->Line.End_X);
					page_lines[num_of_lines].type=HOR_LN;
					page_lines[num_of_lines].width=cpdata->Line.Wid10/10;
					num_of_lines++;
				}
				hline=CLINE_GetNextLine(hline);
			}
		}
		/*    }
		 else
		 {
		 myBlock = CPAGE_GetBlockFirst ( myPage, RLINE_BLOCK_TYPE );
		 while(myBlock)
		 {
		 size = CPAGE_GetBlockData( myPage, myBlock, RLINE_BLOCK_TYPE, &lti, sizeof(LinesTotalInfo));
		 if ( size!=sizeof(LinesTotalInfo) )
		 {
		 wLowRC  = RSTR_ERR_INTERNAL;
		 return FALSE;
		 }

		 myHor = CPAGE_GetBlockFirst ( myPage, (uint32_t)lti.Hor.Lns );
		 while(myHor)
		 {
		 size = CPAGE_GetBlockData( myPage, myHor, (uint32_t)lti.Hor.Lns, &lineinfo, sizeof(lineinfo));
		 if ( size!=sizeof(lineinfo) )
		 {
		 wLowRC  = RSTR_ERR_INTERNAL;
		 return FALSE;
		 }

		 if( (lineinfo.Flags&LI_IsTrue) &&
		 !(lineinfo.Flags&LI_IsAtTable) &&
		 abs(lineinfo.A.x-lineinfo.B.x)>
		 abs(lineinfo.A.y-lineinfo.B.y) )
		 {
		 page_lines[num_of_lines].beg.row=lineinfo.A.y;
		 page_lines[num_of_lines].beg.col=lineinfo.A.x;
		 page_lines[num_of_lines].end.row=lineinfo.B.y;
		 page_lines[num_of_lines].end.col=lineinfo.B.x;
		 page_lines[num_of_lines].type=HOR_LN;
		 page_lines[num_of_lines].width=lineinfo.Thickness;
		 num_of_lines++;
		 }

		 myHor = CPAGE_GetBlockNext ( myPage,myHor, (uint32_t)lti.Hor.Lns );
		 }
		 myBlock = CPAGE_GetBlockNext( myPage, myBlock, RLINE_BLOCK_TYPE );
		 }
		 }*/
	}
#endif
#ifdef _USE_DFON_
	DFon_NewPage(windows_tmp_dir);
#endif
	p2_BoldInit(0);

	BaseLineStatisticInit(); // Nick 26.01.2001

	return TRUE;
}

static Bool rcm_find(int16_t Ax, int16_t Ay, int16_t Bx, int16_t By) {
	int16_t i;
	for (i = 0; i < num_of_lines; i++) {
		if ((page_lines[i].type & UNDRLN) && page_lines[i].beg.x == Ax
				&& page_lines[i].beg.y == Ay && page_lines[i].end.x == Bx
				&& page_lines[i].end.y == By)
			return TRUE;
	}
	return FALSE;
}

RSTR_FUNC(Bool32) RSTR_EndPage( Handle myPage )
{
	uchar lang=language;
	snap_page_disable = FALSE;
	db_status=0;
	if( language==LANG_RUSSIAN && langSer)
	lang=LANG_SERBIAN;
	if( language==LANG_RUSSIAN && langUkr)
	lang=LANG_UKRAINIAN;
	if( language==LANG_RUSSIAN && langBul) // 01.09.2000 E.P.
	lang=LANG_BULGAR;

	if( p2_active==0 )
	{
#ifdef _USE_FON_
#ifndef _FON_CLU_MEMORY_
		if( !p2_disable )
		rstr_close_cont();
#endif
#endif
		enable_pass2=TRUE;
		line_number=0;
		if( !p2_disable )
		{
#ifdef _USE_FON_
#ifdef _FON_CLU_MEMORY_
			if( FONFontClusters(NULL,NULL,
							NULL,NULL,0,FONCLU_MultiFontRow,NULL,lang)<=0 )
			{
				enable_pass2=FALSE;
				wLowRC = RSTR_ERR_INTERNAL;
				return TRUE;
			}
#else
			if( FONFontClusters(local_ctb_name,local_grey_ctb,
							NULL,NULL,0,FONCLU_MultiFontRow,NULL,lang)<=0 )
			{
				enable_pass2=FALSE;
				wLowRC = RSTR_ERR_INTERNAL;
				return TRUE;
			}
#endif
#endif
		}
		erect_set_global();
		if( !p2_disable )
		{
#ifdef _USE_FON_
			if( FONInit(local_grey_ctb)<=0 )
			{
				ErrorExit(RSTR_ERR_INTERNAL);
				return TRUE;
			}
			// Nick test Couruier font
			Flag_Courier=TestFontCourier();
			TestFontProtocol();
			{ // Nick & Oleg - test clusters LEO
				int32_t TestFontClusters(void); // p2_cour.c
				TestFontClusters();
			}

#endif
		}
		p2_BoldInit(1);
#ifdef _USE_CPAGE_
		if( myPage )
		{
			uint32_t size_line_com=sizeof(LINE_COM);
			int size_line_data=sizeof(DLine);
			CLINE_handle hCLINE=CLINE_GetMainContainer();

			//	if(!LDPUMA_Skip(hUseCLine))
			//    {
			CLINE_handle hline;
			CPDLine cpdata;
			DLine data;
			hline=CLINE_GetFirstLine(hCLINE);
			if(!hline)
			return TRUE;
			while(hline)
			{
				cpdata=CLINE_GetLineData(hline);
				if(!cpdata)
				{
					wLowRC = RSTR_ERR_INTERNAL;
					return FALSE;
				}
				else
				{
					if( (cpdata->Flags & LI_IsTrue) &&
							!(cpdata->Flags & LI_IsAtTable) &&
							(abs(cpdata->Line.Beg_X-cpdata->Line.End_X)>
									abs(cpdata->Line.Beg_Y-cpdata->Line.End_Y)) &&
							(cpdata->Dir==LD_Horiz) &&
							rcm_find((int16_t)(cpdata->Line.Beg_X),(int16_t)(cpdata->Line.Beg_Y),
									(int16_t)(cpdata->Line.End_X),(int16_t)(cpdata->Line.End_Y)) )
					{
						CLINE_CopyData(&data,cpdata,size_line_data);
						data.Flags |= LI_Used;
						CLINE_SetLineData(hline,(CPDLine)(&data));
					}
					hline=CLINE_GetNextLine(hline);
				}
			}
			/*    }
			 else
			 {
			 myBlock = CPAGE_GetBlockFirst ( myPage, RLINE_BLOCK_TYPE );
			 while(myBlock)
			 {
			 size = CPAGE_GetBlockData( myPage, myBlock, RLINE_BLOCK_TYPE, &lti, sizeof(LinesTotalInfo));
			 if ( size!=sizeof(LinesTotalInfo) )
			 {
			 wLowRC  = RSTR_ERR_INTERNAL;
			 return FALSE;
			 }

			 myHor = CPAGE_GetBlockFirst ( myPage, (uint32_t)lti.Hor.Lns );
			 while(myHor)
			 {
			 size = CPAGE_GetBlockData( myPage, myHor, (uint32_t)lti.Hor.Lns, &lineinfo, sizeof(lineinfo));
			 if ( size!=sizeof(lineinfo) )
			 {
			 wLowRC  = RSTR_ERR_INTERNAL;
			 return FALSE;
			 }

			 if( (lineinfo.Flags&LI_IsTrue) &&
			 (lineinfo.Flags&LI_IsNotAtTable) &&
			 abs(lineinfo.A.x-lineinfo.B.x)>
			 abs(lineinfo.A.y-lineinfo.B.y) &&
			 rcm_find(lineinfo.A.x,lineinfo.A.y,
			 lineinfo.B.x,lineinfo.B.y) )
			 {
			 lineinfo.Flags |= LI_Used;
			 CPAGE_SetBlockData( myPage, myHor, (uint32_t)lti.Hor.Lns, &lineinfo, sizeof(lineinfo));
			 }

			 myHor = CPAGE_GetBlockNext ( myPage,myHor, (uint32_t)lti.Hor.Lns );
			 }
			 myBlock = CPAGE_GetBlockNext( myPage, myBlock, RLINE_BLOCK_TYPE );
			 }
			 }*/
		}
#endif
	} // end if p2_active=0
	else
	{
		if( !p2_disable )
		{
#ifdef _USE_FON_
			FONDone();
			FONEndSnap();
#endif
		}
#ifdef _USE_LEO_
		LEOFreeAlphabets();
#endif
	}
	return TRUE;
}

RSTR_FUNC(Bool32) RSTR_NeedPass2( void )
{
	return TRUE;
}

void save_to_ctb(CSTR_line lino, int32_t type) {
#ifdef _USE_CTB_
	CSTR_rast rst;
	RecVersions ver;
	RecAlt Zero= {0};
	UniVersions uni;
	RecRaster rast;
	Rect16 rect;
	CSTR_rast_attr attr;
	int32_t i;
	int16_t key;
	uchar flags;
	uchar print_type;
	CSTR_attr lattr;

	CSTR_GetLineAttr(lino,&lattr);
	if( type==0 )
	{
		for(rst = CSTR_GetNext(CSTR_GetFirstRaster(lino));rst;rst=CSTR_GetNext(rst))
		{
			if( CSTR_GetCollectionUni(rst,&uni) &&
					CSTR_GetAttr(rst,&attr) &&
					uni.lnAltCnt &&
					!(line_tabcell && uni.lnAltCnt && uni.Alt[0].Liga==liga_exm) &&
					CSTR_GetImage(rst,(uchar*)&rast,CSTR_TYPE_IMAGE_RS ) &&
					(attr.flg&(CSTR_f_let)) &&
					!(attr.dlang_dup&(CSTR_fd_equal|CSTR_fd_alias)) )
			{
				rect.top = attr.row;
				rect.bottom = attr.row+attr.h;
				rect.left = attr.col;
				rect.right = attr.col+attr.w;
				flags=LEO_VALID_FINAL;
				if( attr.flg & CSTR_f_solid )
				flags|=LEO_VALID_LINGVO;
				if( line_alphabet==ALPHA_DIGITAL_TRUE )
				flags|=LEO_VALID_DIGIT;
				print_type = CTB_PRINT_FONT;
				if( attr.font & CSTR_fp_ser )
				print_type |= CTB_PRINT_SERIFIC;
				if( attr.font & CSTR_fp_it )
				print_type |= CTB_PRINT_ITALIC;
				if( attr.font & CSTR_fp_bold )
				print_type |= CTB_PRINT_BOLD;
				if( attr.font & CSTR_fp_gelv )
				print_type |= CTB_PRINT_GELV;
				if( attr.font & CSTR_fp_narrow )
				print_type |= CTB_PRINT_NARROW;
				for(i=0;i<uni.lnAltCnt;i++)
				{
					ver.Alt[i] = Zero;
					ver.Alt[i].Code = uni.Alt[i].Liga;
					ver.Alt[i].Method = uni.Alt[i].Method;
					ver.Alt[i].Prob = uni.Alt[i].Prob;
				}
				ver.lnAltCnt =uni.lnAltCnt;
#ifdef _FON_CLU_MEMORY_
				key = (int16_t)FONStoreRaster(&rast,ver.Alt[0].Code,
						print_type,ver.Alt[0].Prob, flags,line_number,
						attr.keg,&rect, (uchar)lattr.tab_column/*, lattr.tab_number*/);
#else
				key=rstr_cont_store(&rast,ver.Alt[0].Code,
						0, &rect, print_type,ver.Alt[0].Prob, flags, &ver, 0,
						attr.keg, (uchar)lattr.tab_column, lattr.tab_number);
#endif

				if( key>0 )
				{
					for(i=0;i<uni.lnAltCnt;i++)
					{
						uni.Alt[i].Info =key;
					}
					CSTR_StoreCollectionUni(rst,&uni);
				}
			}
		}
	} // end of type==0
	else
	{ // start of type==1
		for(rst = CSTR_GetNext(CSTR_GetFirstRaster(lino));rst;rst=CSTR_GetNext(rst))
		{
#ifdef _STORE_LETTERS
			rstr_open_cont1();
#endif
			if( CSTR_GetCollection(rst,&ver) &&
					CSTR_GetAttr(rst,&attr) &&
					ver.lnAltCnt &&
					CSTR_GetImage(rst,(uchar*)&rast,CSTR_TYPE_IMAGE_RS ) &&
					(attr.flg&(CSTR_f_let)) )
			{
				rect.top = attr.row;
				rect.bottom = attr.row+attr.h;
				rect.left = attr.col;
				rect.right = attr.col+attr.w;
				flags=LEO_VALID_FINAL;
				if( attr.flg & CSTR_f_solid )
				flags|=LEO_VALID_LINGVO;
				print_type = CTB_PRINT_FONT;
				if( attr.font & CSTR_fp_ser )
				print_type |= CTB_PRINT_SERIFIC;
				if( attr.font & CSTR_fp_it )
				print_type |= CTB_PRINT_ITALIC;
				if( attr.font & CSTR_fp_bold )
				print_type |= CTB_PRINT_BOLD;
				if( attr.font & CSTR_fp_gelv )
				print_type |= CTB_PRINT_GELV;

#ifdef _STORE_LETTERS
				rstr_cont_store1(&rast,ver.Alt[0].Code,
						0, &rect,
						print_type,ver.Alt[0].Prob, flags, &ver, 0,
						attr.keg);
#endif

			}
		}
	} // end of type==1
#endif
	return;
}

RSTR_FUNC(void) RSTR_Save2CTB(CSTR_line lino,int32_t type, int16_t line_num)
{
	CSTR_attr attr;

	CSTR_GetLineAttr(lino,&attr);

	if( attr.Flags & CSTR_STR_Digital )
	line_alphabet=ALPHA_DIGITAL_TRUE;

	if (!(attr.Flags & CSTR_STR_PointSuspension) &&
			( attr.Flags & CSTR_STR_DigitalFuzzy ))
	line_alphabet=ALPHA_DIGITAL;

	if( attr.tab_number )
	line_tabcell= attr.tab_number;

	line_number = line_num;

	save_to_ctb(lino, type);
}

void CSTR_ligas(CSTR_line lino) {
	CSTR_rast rst = CSTR_GetFirstRaster(lino);
	UniVersions uni;
	uchar c;
	int32_t i, nconv;

	for (rst = CSTR_GetNext(rst); rst; rst = CSTR_GetNext(rst)) {
		if (CSTR_GetCollectionUni(rst, &uni)) {
			for (nconv = i = 0; i < uni.lnAltCnt; i++) {
				c = convert_eng_liga(uni.Alt[i].Liga);
				if (c != uni.Alt[i].Liga) {
					uni.Alt[i].Code[0] = c;
					uni.Alt[i].Code[1] = 0;
					uni.Alt[i].Liga = c;
					nconv++;
				}
			}
			if (nconv)
				CSTR_StoreCollectionUni(rst, &uni);
		}
	}

	return;
}

Bool32 free_line(CSTR_line ln) {
	CSTR_rast rst = CSTR_GetNext(CSTR_GetFirstRaster(ln));
	return (rst == (CSTR_rast) 0);
}

static void set_empty_line(CSTR_line ln) {
	CSTR_attr attrlin = { 0 };
	CSTR_rast rst = CSTR_GetNext(CSTR_GetFirstRaster(ln));

	CSTR_GetLineAttr(ln, &attrlin);

	if (!rst) {
		attrlin.Flags |= CSTR_STR_EMPTY;
		CSTR_SetLineAttr(ln, &attrlin);
	}
	return;
}

Bool copy_cap_drop(CSTR_line lin, CSTR_line lino) {
	CSTR_rast ri, ro;
	CSTR_attr lattr = { 0 };
	CCOM_USER_BLOCK ub;
	CCOM_comp *ci, *co;
	int32_t n;
	UniVersions uvs;
	CSTR_rast_attr attr;

	CSTR_GetLineAttr(lin, &lattr);
	CSTR_SetLineAttr(lino, &lattr);
	CSTR_CopyLine(lino, lin);
	ri = CSTR_GetNext(CSTR_GetFirstRaster(lin));
	ro = CSTR_GetNext(CSTR_GetFirstRaster(lino));
	CSTR_GetCollectionUni(ri, &uvs);
	if (!uvs.lnAltCnt)
		return FALSE;
	strcpy(uvs.Alt[0].Code, decode_ASCII_to_[uvs.Alt[0].Liga]);
	uvs.Alt[0].Charset = CodePages[language];
	uvs.lnAltCnt = 1;
	CSTR_StoreCollectionUni(ro, &uvs);
	CSTR_GetAttr(ri, &attr);

	ci = CSTR_GetComp(ri);
	attr.keg = (attr.h * 2 / 3);
	if (uvs.Alt[0].Charset == 204 && !attr.language)
		attr.language = LANG_RUSSIAN;
	CSTR_SetAttr(ro, &attr);
	co = CSTR_GetComp(ro);
	ub.code = CCOM_UB_CAPDROPLN;
	ub.data = (uchar*) &n;
	if (CCOM_GetUserBlock(ci, &ub))
		CCOM_SetUserBlock(co, &ub);
	return TRUE;
}

// превратить русскую и английскую строки в строку с петлями
void rstr_make_multylang_loops(CSTR_line lino) {
	CSTR_rast rst, dup, dupend, eng, oldeng, start;
	CSTR_attr lattr;

	CSTR_GetLineAttr(lino, &lattr);
	if (lattr.language != LANG_RUSENG)
		return;

	for (dup = 0, rst = CSTR_GetNext(CSTR_GetFirstRaster(lino)); rst; rst
			= CSTR_GetNext(rst)) {
		if (!dup) {
			dup = CSTR_GetDup(rst); // начало слова-дубликата
			start = rst;
		}
		dupend = CSTR_GetDupEnd(rst); // конец слова-дубликата
		if (dup && dupend) { // слова-дубликат найдено
			eng = CSTR_InsertRasterDown(start, rst); // dup образовать петлю
			CSTR_CopyRaster(eng, dup);
			if (dup != dupend)
				for (oldeng = CSTR_GetNext(dup); oldeng && oldeng != dupend; oldeng
						= CSTR_GetNext(oldeng)) {
					eng = CSTR_InsertRaster(eng); // dup+1,...,dupend-1
					CSTR_CopyRaster(eng, oldeng);
				}
			if (oldeng == dupend) {
				eng = CSTR_InsertRaster(eng); // dupend конец петли
				CSTR_CopyRaster(eng, oldeng);
			}
			dup = 0;
		}
	}

	return;
}

// превратить строку с петлями в две строки
void rstr_make_second_line(CSTR_line lin, int32_t vers) {
	CSTR_line le;
	CSTR_attr la;
	CSTR_rast r, re, rr, dup, dupend, start;

	CSTR_GetLineAttr(lin, &la);
	le = CSTR_NewLine(la.number, vers, -1);
	re = CSTR_GetFirstRaster(le);
	for (dup = 0, r = CSTR_GetNext(CSTR_GetFirstRaster(lin)); r; r
			= CSTR_GetNext(r)) {
		if (!dup) {
			dup = CSTR_GetNextDown(r); // начало петли
			if (dup)
				start = r;
		}
		if (dup) {
			dupend = CSTR_GetPrevDown(r); // конец петли
		}
		if (dup && dupend) { // клпируем слово из петли в другую строку
			re = CSTR_InsertRaster(re); // first
			CSTR_CopyRaster(re, dup);
			CSTR_SetDup(start, re); // расставляем dup-ы
			if (dup != dupend)
				for (rr = CSTR_GetNext(dup); rr != dupend; rr
						= CSTR_GetNext(rr)) {
					re = CSTR_InsertRaster(re);
					CSTR_CopyRaster(re, rr);
				}
			if (dup != dupend) {
				re = CSTR_InsertRaster(re); // last
				CSTR_CopyRaster(re, rr);
			}
			CSTR_SetDupEnd(r, re);

			dup = 0;
		}
	}

	CSTR_DeleteLoops(lin); // удаляем петли
	return;
}

void rsrt_del_dupes(CSTR_line l) {
	CSTR_rast r;
	CSTR_rast_attr a;
	for (r = CSTR_GetNext(CSTR_GetFirstRaster(l)); r; r = CSTR_GetNext(r)) {
		if (CSTR_GetDup(r)) {
			CSTR_SetDup(r, (CSTR_rast) 0);
			CSTR_GetAttr(r, &a);
			a.dlang_dup = 0;
			CSTR_SetAttr(r, &a);
		}
		if (CSTR_GetDupEnd(r)) {
			CSTR_SetDupEnd(r, (CSTR_rast) 0);
			CSTR_GetAttr(r, &a);
			a.dlang_dup = 0;
			CSTR_SetAttr(r, &a);
		}
	}
	return;
}

Bool32 rstr_rerecog_plusminus(CSTR_line lino) {
	CSTR_rast rst;
	RecVersions vrs;

	rst = CSTR_GetNext(CSTR_GetFirstRaster(lino));
	if (CSTR_GetNext(rst))
		return FALSE;
	CSTR_GetCollection(rst, &vrs);
	if (!vrs.lnAltCnt)
		return FALSE;
	if (!strchr("-+", vrs.Alt[0].Code))
		return FALSE;
	return TRUE;
}

Bool32 rstr_is_plusminus(CSTR_line lino) {
	CSTR_rast rst;
	RecVersions vrs;
	CSTR_attr lattr;

	CSTR_GetLineAttr(lino, &lattr);
	if (lattr.Flags & CSTR_STR_PlusMinus)
		return TRUE;
	rst = CSTR_GetNext(CSTR_GetFirstRaster(lino));
	if (CSTR_GetNext(rst))
		return FALSE;
	CSTR_GetCollection(rst, &vrs);
	if (vrs.lnAltCnt && strchr("+-", vrs.Alt[0].Code)) {
		lattr.Flags |= CSTR_STR_PlusMinus;
		CSTR_SetLineAttr(lino, &lattr);
		return TRUE;
	}
	return FALSE;
}

void rstr_set_kegl(CSTR_line lino) {
	CSTR_rast rst;
	CSTR_rast_attr attr;
	CSTR_attr lattr;

	CSTR_GetLineAttr(lino, &lattr);
	rst = CSTR_GetNext(CSTR_GetFirstRaster(lino));
	CSTR_GetAttr(rst, &attr);
	attr.keg = (uchar) lattr.Ps;
	CSTR_SetAttr(rst, &attr);
	return;
}

Bool32 rstr_bad_recog_line(CSTR_line lout) {
	CSTR_rast rst;
	UniVersions uni;
	int32_t pmax = 0, pav = 0, n = 0, nbad = 0;

	for (rst = CSTR_GetNext(CSTR_GetFirstRaster(lout)); rst; rst
			= CSTR_GetNext(rst)) {
		CSTR_GetCollectionUni(rst, &uni);
		if (uni.lnAltCnt) {
			pav += uni.Alt[0].Prob;
			if (pmax < uni.Alt[0].Prob)
				pmax = uni.Alt[0].Prob;
		} else
			nbad++;
		n++;
	}
	if (!n)
		//return FALSE;
		return TRUE; // Nick 19.03.2001 -Saus032

	pav /= n;
	return (pav < 140 && pmax < 180 || pav < 100 && pmax < 200 && nbad);
}

//int32_t test_count_lines = 0;

RSTR_FUNC(Bool32) RSTRRecognize(
		CSTR_line lin, // pointer to raw string
		CSTR_line lino )
{
	RSTRRecognizeMain(lin, lino);

	RSTR_Save2CTB(lino, 0, line_number);

	RSTRRecognizePostMain(lin, lino);

	/* Andrey (10.06.2003): divided into 3 parts: 1. RSTRRecognizeMain, 2. RSTR_Save2CTB, 3. RSTRRecognizePostMain
	 int rc;
	 uchar   lang=language;
	 CSTR_attr       lattr={0};

	 //test_count_lines++;

	 local_ret_error_code=0;
	 line_BL=FALSE;
	 line_handfragment=FALSE;
	 line_rerecog=FALSE;
	 CSTR_GetLineAttr (lin, &lattr);
	 line_rerecog=FALSE;
	 start:;
	 if( multy_language && language==LANG_ENGLISH  )
	 language=LANG_RUSSIAN ;
	 if( multy_language && language==LANG_RUSSIAN )
	 {
	 lang=LANG_RUSENG;
	 CSTR_SetLineAttr (lin, &lattr);
	 if( p2_active )
	 correct_let_tables();
	 }
	 if( p2_active )
	 {
	 if( free_line(lino) )
	 return TRUE;
	 if( !enable_pass2 )
	 return TRUE;
	 }
	 if( lattr.Flags & CSTR_STR_CapDrop )
	 {
	 line_number++;
	 if( !p2_active )
	 if( !copy_cap_drop(lin, lino) )
	 {
	 lattr.Flags -= CSTR_STR_CapDrop;
	 CSTR_SetLineAttr (lin, &lattr);
	 CSTR_SetLineAttr (lino, &lattr);
	 }
	 return TRUE;
	 }
	 if((rc=setjmp(jumper)) != 0 && rc != FIELD_RESTART)
	 {
	 reset_snap();
	 if( rc == RSTR_ERR_NOPLACE )
	 { // can be warning
	 if(0)
	 if( !kit_max_size )
	 {
	 rstr_kit_realloc();
	 line_number--;
	 goto start;
	 }
	 return TRUE;
	 }
	 stop_user =( rc==RSTR_ERR_USERBREAK );
	 wLowRC         = rc;
	 set_empty_line(lino);
	 return FALSE;
	 }
	 if( p2_active )
	 {
	 if( stop_user )
	 ErrorExit(RSTR_ERR_USERBREAK);
	 }
	 stop_user = FALSE;
	 kit_curr= kit_start;
	 kit_end = kit_curr + kit_size;
	 stopcell=NULL;

	 line_scale=0;
	 line_minus=0;
	 line_alphabet=0;
	 line_pointsusp=0;
	 line_handfragment=FALSE;
	 line_readyBL=FALSE;
	 line_tabcell=0;
	 enable_table_recog=FALSE;
	 line_number++;
	 erection_inc=0;
	 valid_word_number=0;
	 if( line_number==1 )
	 {
	 nIncline = page_nIncline;
	 if( !p2_active )
	 snap_init_variables();
	 }
	 if( !spec_camera )
	 {
	 CSTR_UnpackLine(lin);
	 if( p2_active )
	 CSTR_UnpackLine(lino);
	 }
	 if( (lattr.Flags & CSTR_STR_PlusMinus) ||
	 (lattr.Flags & CSTR_STR_ReadyResults) )
	 {
	 if( !p2_active )
	 CSTR_CopyLine(lino,lin);
	 if( rstr_is_plusminus(lin) &&
	 (lattr.Flags & CSTR_STR_ReadyResults) )
	 {
	 rstr_set_kegl(lino);
	 }
	 if( !rstr_rerecog_plusminus(lin))
	 {
	 if( !p2_active )
	 CSTR_ClearLine(lino,-16000,32000);
	 goto step_pass3;
	 }

	 if( db_status && snap_activity('g') )
	 {
	 snap_show_text("end recog - plus_minus line");
	 snap_monitor_ori(&lino,1);
	 }
	 }
	 else
	 {
	 CSTR_head *lnhead =(CSTR_head *)lin;
	 step_pass3:
	 pass3( lin, lino ); // recognize
	 if( (lattr.Flags &  (CSTR_STR_Digital |CSTR_STR_DigitalFuzzy )) &&
	 rstr_bad_recog_line(lino) )
	 {
	 CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_SAVE));
	 CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_ENGOUT));
	 CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_ENG));
	 CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_PASS2));
	 CSTR_ClearLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_MAINOUT),-16000,32000);

	 if( lattr.Flags &  CSTR_STR_Digital )
	 lattr.Flags -=  CSTR_STR_Digital;
	 if( lattr.Flags &  CSTR_STR_DigitalFuzzy  )
	 lattr.Flags -=  CSTR_STR_DigitalFuzzy;
	 CSTR_SetLineAttr (lin, &lattr);
	 line_number--;
	 line_rerecog=TRUE;
	 goto start; // recognize
	 }
	 // Nick 14.05.2001
	 // удалить мелочь, если есть точечные линии
	 RemoveDustIfPointLine( lino );
	 }

	 set_empty_line(lino);
	 store_colors(lino);
	 if( !p2_disable )
	 {
	 #ifdef _USE_FON_
	 if( !p2_active )
	 {
	 save_to_ctb(lino,0);

	 CSTR_ligas(lino);
	 }
	 #endif
	 }


	 if( !p2_active )
	 {
	 rstr_make_multylang_loops(lino);
	 CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_ENGOUT));
	 rsrt_del_dupes(lino);
	 }

	 if( !spec_camera )
	 {
	 CSTR_PackLine(lin);
	 CSTR_PackLine(lino);
	 }
	 CSTR_GetLineAttr (lino, &lattr);
	 lattr.pageSkew2048=page_nIncline;
	 lattr.ResolutionY=(int16_t)nResolutionY;
	 if( fax1x2 )
	 lattr.Flags |= CSTR_STR_Fax100x200;
	 if( matrix )
	 lattr.Flags |= CSTR_STR_Matrix;
	 CSTR_SetLineAttr (lino, &lattr);
	 reset_snap();
	 */

	return TRUE;
}

//*********************************************
//* part 1 of RSTRRecognize (before clusters) *
//*********************************************
RSTR_FUNC(Bool32) RSTRRecognizeMain(
		CSTR_line lin, // pointer to raw string
		CSTR_line lino )
{
	int rc;
	uchar lang=language;
	CSTR_attr lattr= {0};

	//test_count_lines++;

	local_ret_error_code=0;
	line_BL=FALSE;
	line_handfragment=FALSE;
	line_rerecog=FALSE;
	CSTR_GetLineAttr (lin, &lattr);
	line_rerecog=FALSE;
	start:;
	if( multy_language && language==LANG_ENGLISH )
	language=LANG_RUSSIAN;
	if( multy_language && language==LANG_RUSSIAN )
	{
		lang=LANG_RUSENG;
		CSTR_SetLineAttr (lin, &lattr);
		if( p2_active )
		correct_let_tables();
	}
	if( p2_active )
	{
		if( free_line(lino) )
		return TRUE;
		if( !enable_pass2 )
		return TRUE;
	}
	if( lattr.Flags & CSTR_STR_CapDrop )
	{
		line_number++;
		if( !p2_active )
		if( !copy_cap_drop(lin, lino) )
		{
			lattr.Flags -= CSTR_STR_CapDrop;
			CSTR_SetLineAttr (lin, &lattr);
			CSTR_SetLineAttr (lino, &lattr);
		}
		return TRUE;
	}
	if((rc=setjmp(jumper)) != 0 && rc != FIELD_RESTART)
	{
		reset_snap();
		if( rc == RSTR_ERR_NOPLACE )
		{ // can be warning
			if(0)
			if( !kit_max_size )
			{
				rstr_kit_realloc();
				line_number--;
				goto start;
			}
			return TRUE;
		}
		stop_user =( rc==RSTR_ERR_USERBREAK );
		wLowRC = rc;
		set_empty_line(lino);
		return FALSE;
	}
	if( p2_active )
	{
		if( stop_user )
		ErrorExit(RSTR_ERR_USERBREAK);
	}
	stop_user = FALSE;
	kit_curr= kit_start;
	kit_end = kit_curr + kit_size;
	stopcell=NULL;

	line_scale=0;
	line_minus=0;
	line_alphabet=0;
	line_pointsusp=0;
	line_handfragment=FALSE;
	line_readyBL=FALSE;
	line_tabcell=0;
	enable_table_recog=FALSE;
	line_number++;
	erection_inc=0;
	valid_word_number=0;
	if( line_number==1 )
	{
		nIncline = page_nIncline;
		if( !p2_active )
		snap_init_variables();
	}
	if( !spec_camera )
	{
		CSTR_UnpackLine(lin);
		if( p2_active )
		CSTR_UnpackLine(lino);
	}
	if( (lattr.Flags & CSTR_STR_PlusMinus) ||
			(lattr.Flags & CSTR_STR_ReadyResults) )
	{
		if( !p2_active )
		CSTR_CopyLine(lino,lin);
		if( rstr_is_plusminus(lin) &&
				(lattr.Flags & CSTR_STR_ReadyResults) )
		{
			rstr_set_kegl(lino);
		}
		if( !rstr_rerecog_plusminus(lin))
		{
			if( !p2_active )
			CSTR_ClearLine(lino,-16000,32000);
			goto step_pass3;
		}

		if( db_status && snap_activity('g') )
		{
			snap_show_text("end recog - plus_minus line");
			snap_monitor_ori(&lino,1);
		}
	}
	else
	{
		step_pass3:
		pass3( lin, lino ); // recognize
		if( (lattr.Flags & (CSTR_STR_Digital |CSTR_STR_DigitalFuzzy )) &&
				rstr_bad_recog_line(lino) )
		{
			CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_SAVE));
			CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_ENGOUT));
			CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_ENG));
			CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_PASS2));
			CSTR_ClearLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_MAINOUT),-16000,32000);

			if( lattr.Flags & CSTR_STR_Digital )
			lattr.Flags -= CSTR_STR_Digital;
			if( lattr.Flags & CSTR_STR_DigitalFuzzy )
			lattr.Flags -= CSTR_STR_DigitalFuzzy;
			CSTR_SetLineAttr (lin, &lattr);
			line_number--;
			line_rerecog=TRUE;
			goto start; // recognize
		}
		// Nick 14.05.2001
		// удалить мелочь, если есть точечные линии
		RemoveDustIfPointLine( lino );
	}

	set_empty_line(lino);
	store_colors(lino);

	if( !spec_camera )
	{
		CSTR_PackLine(lin);
	}

	reset_snap();
	return TRUE;
}

//********************************************
//* part 3 of RSTRRecognize (after clusters) *
//********************************************
RSTR_FUNC(Bool32) RSTRRecognizePostMain(
		CSTR_line lin, // pointer to raw string
		CSTR_line lino )
{
	CSTR_attr lattr= {0};

	CSTR_GetLineAttr (lin, &lattr);

	if( !p2_disable )
	{
#ifdef _USE_FON_
		if( !p2_active )
		{
			CSTR_ligas(lino);
		}
#endif
	}

	if( !p2_active )
	{
		rstr_make_multylang_loops(lino);
		CSTR_DeleteLine(CSTR_GetLineHandle(lattr.number, CSTR_LINVERS_ENGOUT));
		rsrt_del_dupes(lino);
	}

	if( !spec_camera )
	{
		CSTR_PackLine(lino);
	}
	CSTR_GetLineAttr (lino, &lattr);
	lattr.pageSkew2048=page_nIncline;
	lattr.ResolutionY=(int16_t)nResolutionY;
	if( fax1x2 )
	lattr.Flags |= CSTR_STR_Fax100x200;
	if( matrix )
	lattr.Flags |= CSTR_STR_Matrix;
	CSTR_SetLineAttr (lino, &lattr);
	return TRUE;
}

RSTR_FUNC(Bool32) RSTRRecognizeBL(
		CSTR_line lin )
{
	int rc;
	uchar lang=language;
	CSTR_attr lattr= {0};
	Bool32 ret;

	local_ret_error_code=0;
	if( p2_active )
	return FALSE;
	start:;
	if( multy_language && language==LANG_ENGLISH )
	language=LANG_RUSSIAN;
	if( multy_language && language==LANG_RUSSIAN )
	{
		CSTR_GetLineAttr (lin, &lattr);
		lang=LANG_RUSENG;
		CSTR_SetLineAttr (lin, &lattr);
		if( p2_active )
		correct_let_tables();
	}

	if((rc=setjmp(jumper)) != 0 && rc != FIELD_RESTART)
	{
		reset_snap();
		if( rc == RSTR_ERR_NOPLACE )
		{ // can be warning
			if(0)
			if( !kit_max_size )
			{
				rstr_kit_realloc();
				line_number--;
				goto start;
			}
			line_BL=FALSE;
			line_handfragment=FALSE;
			line_rerecog=FALSE;
			return TRUE;
		}
		stop_user =( rc==RSTR_ERR_USERBREAK );
		wLowRC = rc;
		line_BL=FALSE;
		line_handfragment=FALSE;
		line_rerecog=FALSE;
		return FALSE;
	}
	stop_user = FALSE;
	kit_curr= kit_start;
	kit_end = kit_curr + kit_size;
	stopcell=NULL;

	line_scale=0;
	line_minus=0;
	line_alphabet=0;
	line_pointsusp=0;
	line_handfragment=FALSE;
	line_rerecog=FALSE;
	line_readyBL=FALSE;
	line_tabcell=0;
	erection_inc=0;
	valid_word_number=0;
	nIncline = page_nIncline;

	ret = pass3BL( lin);
	reset_snap();
	line_BL=FALSE;
	line_handfragment=FALSE;
	line_rerecog=FALSE;
	return ret;
}

RSTR_FUNC(void) RSTRDone(void)
{
#ifdef _USE_DFON_
	DFON_Done();
	FONEndSnap();
#endif
	LDPUMA_Done();
	my_free(CellsPage_rstr,0);
	my_free(kit_start,0);
	my_free(memory_pool,0);
	if( !p2_disable )
	{
#ifdef _USE_FON_
#ifndef _FON_CLU_MEMORY_
		rstr_close_cont();
#endif
#ifdef _STORE_LETTERS
		rstr_close_cont1();
#endif
		FONDone();
#endif
	}
#ifdef     _USE_SPELLING_
	RLING_Done();
#endif
#ifdef _USE_LEO_
	LEODone();
#endif
	p2_msk_done();
	return;
}

RSTR_FUNC(Err16) RSTRGetErr(void)
{
	return rcmErr;
}

void ErrorExit(int Code) {
	longjmp(jumper, Code);
}

void CErrorExit(int Code) {
	longjmp(jumper, Code);
}

////// OLEG for PUMA
RSTR_FUNC(Bool32) RSTR_Init(uint16_t wHightCode, Handle hStorage)
{
	wHeightRC = wHightCode;
	wLowRC = RSTR_ERR_NO;
	return RSTRInit( NULL );
}

RSTR_FUNC(Bool32) RSTR_Done(void)
{
	RSTRDone();
	wLowRC = RSTR_ERR_NO;
	wHeightRC=0;
	return TRUE;
}

RSTR_FUNC(uint32_t) RSTR_GetReturnCode(void)
{
	if( wLowRC == RSTR_ERR_NO )
	return 0;

	return (wHeightRC<<16)|(wLowRC-RSTR_ERR_MIN);
}

RSTR_FUNC(uchar *) RSTR_GetReturnString(uint32_t dwError)
{
	uint16_t rc = (uint16_t)(dwError & 0xFFFF + RSTR_ERR_MIN);
	static uchar szBuffer[512];

	if( /*wLowRC==RSTR_ERR_MIN && */local_ret_error_code )
	return local_ret_error_str(local_ret_error_code);

	if( dwError >> 16 != wHeightRC)
	wLowRC = RSTR_ERR_NOTIMPLEMENT;

	if( rc > 0 && rc <= RSTR_ERR_MAX-RSTR_ERR_MIN )
	strcpy((char *)szBuffer,RSTR_error_name [rc]);
	else
	return NULL;

	return szBuffer;
}

RSTR_FUNC(Bool32) RSTR_NewPage(int32_t resolutiony, Handle Page)
{
	return RSTRNewPage( resolutiony, Page);
}

Bool32 Reload_lang_vocs(uchar lang) {
#ifdef     _USE_SPELLING_
	//if( lang==LANG_DIG )
	//    return TRUE;
	RLING_UnloadDictonary();
	if( lang == LANG_RUSENG )
	lang = LANG_RUSSIAN;
	if ( !RLING_LoadDictonary( lang , (pchar)lnOcrLingPath) )
	{
		wLowRC = RSTR_ERR_NOINITRSTR;
		local_ret_error_code=RLING_GetReturnCode();
		local_ret_error_str =(fun_error)RLING_GetReturnString;
		return FALSE;
	}
#endif
	return TRUE;
}

Bool32 Reload_lang_vocs_aux(uchar language) {
#ifdef     _USE_SPELLING_
	//if( language==LANG_DIG )
	//    return TRUE;
	RLING_UnloadSecDictonary();
	if( language == LANG_RUSSIAN || language == LANG_ENGLISH )
	{
		if ( !RLING_LoadSecDictonary( language , (pchar)lnOcrLingPath) )
		{
			wLowRC = RSTR_ERR_NOINITRSTR;
			local_ret_error_code=RLING_GetReturnCode();
			local_ret_error_str =(fun_error)RLING_GetReturnString;
			return FALSE;
		}
	}
#endif
	return TRUE;
}

RSTR_FUNC(Bool32) RSTR_SetOptions (RSTR_Options *opt)
{
	char *p;
	int i;
	Bool32 ret=TRUE;
	uchar slanguage;
	static uchar old_language=-1;

	if( opt->language )
	{
		p=opt->szAlphaString;
		for(i=0;i<256;i++)
		if( alphabet[i] )
		*p++ =(char)i;
		*p=0;
	}
	else if( opt->Alphabet )
	{
		p=opt->szAlphaString;
		for(i=0;i<256;i++)
		if( opt->Alphabet[i] )
		*p++ =(char)i;
		*p=0;
	}
	language = opt->language;
	page_nIncline=(int16_t)opt->pageSkew2048;
	actual_resolution=300;
	nResolutionY=actual_resolution;
	current_fragment=0;
	fax1x2=0;
	double_fax=fax1x2;
	line_scale=0;
	line_minus=0;
	line_alphabet=0;
	line_pointsusp=0;
	line_handfragment=FALSE;
	line_rerecog=FALSE;
	line_readyBL=FALSE;
	line_tabcell=0;
	langSer=langUkr=langBul=0; // langBul 01.09.2000 E.P.

	if( mmx )
	set_MMX_addr();
	else
	set_all_addr();
	chdir(lnOcrPath);
	multy_language=FALSE;
	slanguage=language;

	if( language==LANG_RUSENG )
	{
		language = LANG_RUSSIAN;
		multy_language=TRUE;
	}
	if( language==LANG_UKRAINIAN )
	{
		language = LANG_RUSSIAN;
		langUkr =TRUE;
	}
	if( language==LANG_SERBIAN )
	{
		language = LANG_RUSSIAN;
		langSer =TRUE;
	}
	if( language==LANG_BULGAR ) // 01.09.2000 E.P.
	{
		language = LANG_RUSSIAN;
		langBul =TRUE;
	}

	if( old_language!=opt->language )
	{
		if( !trees_load() )
		{
			wLowRC = RSTR_ERR_NOINITRSTR;
			return FALSE;
		}

		if( !Reload_lang_vocs(slanguage) )
		{
			wLowRC = RSTR_ERR_NOINITRSTR;
			return FALSE;
		}
		if( language==LANG_RUSSIAN && multy_language )
		{
			if( !Reload_lang_vocs_aux(LANG_ENGLISH) )
			{
				wLowRC = RSTR_ERR_NOINITRSTR;
				return FALSE;
			}
		}
	}
	if( language== LANG_RUSSIAN )
	{
		cuts_glues_methode = 1;
		cuts_point_methode = 1;
	}
	else
	{
		cuts_glues_methode = 0;
		cuts_point_methode = 0;
	}
	old_language=slanguage;
	trees_load_fict();
	return ret;
}

RSTR_FUNC(Bool32) RSTR_RecogBitmap (Rect16 *r, Handle bm,CSTR_line lino)
{
	wLowRC = RSTR_ERR_NOTIMPLEMENT;
	return FALSE;
}

RSTR_FUNC(Bool32) RSTR_Recog (CSTR_line lini, CSTR_line lino)
{
	return RSTRRecognize( lini, lino);
}

RSTR_FUNC(Bool32) RSTR_RecogBL (CSTR_line lini)
{
	return RSTRRecognizeBL( lini);
}

RSTR_FUNC(Bool32) RSTR_RecogOneLetter (RecRaster *Rs,uchar Language,RecVersions *Vs)
{
	extern Bool32 RecogLEOcap(RecRaster *Rs,uchar Language,RecVersions *Vs);
	return RecogLEOcap(Rs,Language,Vs);
}

RSTR_FUNC(Bool32) RSTR_RecogOneLetter_all (RecRaster *Rs,char *letters,RecVersions *Vs,int32_t nType)
{
	extern Bool32 RecogLEOall(RecRaster *Rs,RecVersions *Vs,int32_t nType);
	extern Bool32 RecogLEO_SetAlphabet(char *letters);
	RecogLEO_SetAlphabet(letters);
	return RecogLEOall(Rs,Vs,nType);
}

RSTR_FUNC(Bool32) RSTR_TestAlphabet(RSTR_info *str,CSTR_line lin)
{
	str_info str0;
	int rc;

	if((rc=setjmp(jumper)) != 0 && rc != FIELD_RESTART)
	{
		stop_user =( rc==RSTR_ERR_USERBREAK );
		wLowRC = rc;
		return FALSE;
	}

	stop_user = FALSE;
	kit_curr= kit_start;
	kit_end = kit_curr + kit_size;
	stopcell=NULL;
	line_scale=0;
	line_minus=0;
	line_alphabet=0;
	line_pointsusp=0;
	line_handfragment=FALSE;
	line_rerecog=FALSE;
	line_readyBL=FALSE;
	line_tabcell=0;
	str0.all = (int16_t)str->nall;
	str0.dig = (int16_t)str->ndig;
	str0.fuzzy_dig = (int16_t)str->nfdig;
	str0.dollars = (int16_t)str->ndollars;
	str0.alphabet = (int16_t)str->alphabet;
	str0.pm = (int16_t)str->nplus_minus;
	pass1_test_alphabet(&str0,lin);
	str->nall = str0.all;
	str->ndig = str0.dig;
	str->nfdig = str0.fuzzy_dig;
	str->ndollars = str0.dollars;
	str->alphabet = str0.alphabet;
	str->nplus_minus= str0.pm;
	return TRUE;
}

RSTR_FUNC(Bool32) RSTR_StudyAlphabet(RSTR_info *str)
{
	str_info str0;
	str0.all = (int16_t)str->nall;
	str0.dig = (int16_t)str->ndig;
	str0.fuzzy_dig = (int16_t)str->nfdig;
	str0.dollars = (int16_t)str->ndollars;
	str0.alphabet = (int16_t)str->alphabet;
	str0.pm = (int16_t)str->nplus_minus;
	set_column_alphabets(&str0,1); // 1 column
	str->nall = str0.all;
	str->ndig = str0.dig;
	str->nfdig = str0.fuzzy_dig;
	str->ndollars = str0.dollars;
	str->alphabet = str0.alphabet;
	str->nplus_minus= str0.pm;
	return TRUE;
}

///////////////////////////////////////
RSTR_FUNC(Bool32) RSTR_TestAlphabetPass2(RSTR_info *str,CSTR_line lin)
{
	str_info str0;
	int rc;

	if((rc=setjmp(jumper)) != 0 && rc != FIELD_RESTART)
	{
		stop_user =( rc==RSTR_ERR_USERBREAK );
		wLowRC = rc;
		return FALSE;
	}

	stop_user = FALSE;
	kit_curr= kit_start;
	kit_end = kit_curr + kit_size;
	stopcell=NULL;
	line_scale=0;
	line_minus=0;
	line_alphabet=0;
	line_pointsusp=0;
	line_handfragment=FALSE;
	line_rerecog=FALSE;
	line_readyBL=FALSE;
	line_tabcell=0;
	str0.all = (int16_t)str->nall;
	str0.dig = (int16_t)str->ndig;
	str0.fuzzy_dig = (int16_t)str->nfdig;
	str0.dollars = (int16_t)str->ndollars;
	str0.alphabet = (int16_t)str->alphabet;
	str0.pm = (int16_t)str->nplus_minus;
	pass2_test_alphabet(&str0,lin);
	str->nall = str0.all;
	str->ndig = str0.dig;
	str->nfdig = str0.fuzzy_dig;
	str->ndollars = str0.dollars;
	str->alphabet = str0.alphabet;
	str->nplus_minus= str0.pm;
	return TRUE;
}

RSTR_FUNC(Bool32) RSTR_StudyAlphabetPass2(RSTR_info *str)
{
	str_info str0;
	str0.all = (int16_t)str->nall;
	str0.dig = (int16_t)str->ndig;
	str0.fuzzy_dig = (int16_t)str->nfdig;
	str0.dollars = (int16_t)str->ndollars;
	str0.alphabet = (int16_t)str->alphabet;
	str0.pm = (int16_t)str->nplus_minus;
	set_column_alphabets_pass2(&str0,1); // 1 column
	str->nall = str0.all;
	str->ndig = str0.dig;
	str->nfdig = str0.fuzzy_dig;
	str->ndollars = str0.dollars;
	str->alphabet = str0.alphabet;
	str->nplus_minus= str0.pm;
	return TRUE;
}

///////////////////////////////////////

Handle hSnapMain;
static Bool32 RecognizeStringsPass1(void) {
	Bool32 rc = TRUE;
	// распознавание строк
	if (rc) {
		int count = CSTR_GetMaxNumber();
		int i;

		LDPUMA_StartLoop(hSnapMain, count);
		//if(!ProgressStep(2,GetResourceString(IDS_PRG_RECOG),0))
		//	rc = FALSE;

		for (i = 1; rc && i <= count; i++) {
			CSTR_line lin_out, lin_in;
			//	if(!ProgressStep(2,NULL,i*100/count))
			//		rc = FALSE;

			LDPUMA_LoopNext(hSnapMain);
			if (!LDPUMA_Skip(hSnapMain)) {
				LDPUMA_Console("Остановились перед %i строкой.\n", i);
				LDPUMA_Console("Нажмите любую клавишу...\n");
				LDPUMA_WaitUserInput(hSnapMain, NULL);
			}

			lin_out = CSTR_NewLine(i, CSTR_LINVERS_MAINOUT, -1); // OLEG
			if (lin_out == (CSTR_line) NULL) {
				wLowRC = RSTR_ERR_NOINITRSTR;
				rc = FALSE;
				break;
			}

			lin_in = CSTR_GetLineHandle(i, CSTR_LINVERS_MAIN);
			if (lin_in == (CSTR_line) NULL) {
				wLowRC = RSTR_ERR_NOINITRSTR;
				rc = FALSE;
				break;
			}

			if (!RSTR_Recog(lin_in, lin_out)) // Recognition
			{
				wLowRC = RSTR_ERR_NOINITRSTR;
				rc = FALSE;
				break;
			}

		}
		LDPUMA_DestroyRasterWnd();
	}
	if (rc) {
		//if(!ProgressStep(2,GetResourceString(IDS_PRG_RECOG2),100))
		//	rc = FALSE;
		rc = RSTR_EndPage(NULL);
		if (!rc)
			wLowRC = RSTR_ERR_NOINITRSTR;
	}

	return rc;
}
//////////////////////////////////////////

static Bool32 RecognizeStringsPass2() {
	Bool32 rc = TRUE;
	// рапознавание строк

	//	uchar w8 = 1;
	int count = CSTR_GetMaxNumber();
	int i;

	if (!p2_active)
		return TRUE;

	//    RSTR_SetImportData(RSTR_Word8_P2_active,&w8);
	LDPUMA_StartLoop(hSnapMain, count);
	//if(!ProgressStep(2,GetResourceString(IDS_PRG_RECOG2),0))
	//	rc = FALSE;

	for (i = 1; rc && i <= count; i++) {
		CSTR_line lin_out, lin_in;
		//	if(!ProgressStep(2,NULL,i*100/count))
		//		rc = FALSE;

		LDPUMA_LoopNext(hSnapMain);
		if (!LDPUMA_Skip(hSnapMain)) {
			LDPUMA_Console("Остановились перед %i строкой.\n", i);
			LDPUMA_Console("Нажмите любую клавишу...\n");
			LDPUMA_WaitUserInput(hSnapMain, NULL);
		}

		lin_out = CSTR_GetLineHandle(i, CSTR_LINVERS_MAINOUT);
		if (lin_out == (CSTR_line) NULL) {
			wLowRC = RSTR_ERR_NOINITRSTR;
			rc = FALSE;
			break;
		}

		lin_in = CSTR_GetLineHandle(i, CSTR_LINVERS_MAIN);
		if (lin_in == (CSTR_line) NULL) {
			wLowRC = RSTR_ERR_NOINITRSTR;
			rc = FALSE;
			break;
		}

		if (!RSTR_Recog(lin_in, lin_out)) // Recognition
		{
			wLowRC = RSTR_ERR_NOINITRSTR;
			break;
		}

	}
	LDPUMA_DestroyRasterWnd();
	if (rc) {
		rc = RSTR_EndPage(NULL);
		if (!rc)
			wLowRC = RSTR_ERR_NOINITRSTR;
	}

	return rc;
}
//////////////////////////////////////
RSTR_FUNC(Bool32) RSTR_RecogContainer (void)
{
	return RecognizeStringsPass1() && RecognizeStringsPass2();
}

RSTR_FUNC(Bool32) RSTR_SetSpecPrj(uchar nSpecPrj)
{
	db_special_project = nSpecPrj;
	return TRUE;
}

RSTR_FUNC(Bool32) RSTR_GetExportData (uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;
	int32_t vers = RSTR_VERSION_CODE;
#define EXPORT(name) *(uint32_t*)(pData)=(uint32_t)name;

#define CASE_DATA(a,b,c)        case a: *(b *)pData = c; break

	wLowRC = RSTR_ERR_NO;
	switch(dwType)
	{
		CASE_DATA(RSTR_Word8_Language ,uchar,language);
		CASE_DATA(RSTR_Word16_Resolution ,uint16_t,actual_resolution);
		CASE_DATA(RSTR_Word8_Fax1x2 ,uchar,fax1x2);
		CASE_DATA(RSTR_Word8_Matrix ,uchar,matrix);
		CASE_DATA(RSTR_Word8_P2_active ,uchar,p2_active);
		CASE_DATA(RSTR_Word8_Spell_check ,uchar,spell_check);
		CASE_DATA(RSTR_pchar_user_dict_name ,char*,user_dict_name);
		case RSTR_FNNEWPAGE: //      новая страница
		EXPORT(RSTR_NewPage );
		break;
		case RSTR_FNSETOPTIONS: //      установка опций
		EXPORT(RSTR_SetOptions );
		break;
		case RSTR_FNBITMAP: //       распознать по зоне
		EXPORT(RSTR_RecogBitmap);
		break;
		case RSTR_FNLINE: //       распознать
		EXPORT(RSTR_Recog);
		break;
		case RSTR_FNVERSION: //      код версии
		EXPORT(vers);
		break;
		case REXC_FNRSTR_ISLANGUAGE: // возможен ли язык
		EXPORT(RSTR_IsLanguage);
		break;
		case RSTR_FNSTRENDPAGE: //  конец распознавания списка строк
		EXPORT(RSTR_EndPage);
		break;
		case RSTR_FNNEEDPASS2:
		EXPORT(RSTR_NeedPass2);
		break;
		case RSTR_FNLINEBL: //       распознать BL
		EXPORT(RSTR_RecogBL);
		break;
		case RSTR_FNTESTALPHABET:// сбор алфавитной статистики в столбце
		EXPORT(RSTR_TestAlphabet);
		break;
		case RSTR_FNSTUDYALPHABET://выбор алфавита столбца по статистике
		EXPORT(RSTR_StudyAlphabet);
		break;
		case RSTR_RECONEWORD: // распознать фиксированное слово
		EXPORT(RSTR_recog_one_word);
		break;
		case RSTR_FNRECOGONELETTER: // распознать как букву
		EXPORT( RSTR_RecogOneLetter );
		break;
		case RSTR_FNRECOGCONTAINER: // распознать контейнер строк
		EXPORT(RSTR_RecogContainer);
		break;
		case RSTR_FNRECOGONELETTERALL: // распознать как букву из алфавита
		EXPORT( RSTR_RecogOneLetter_all );
		break;
		case RSTR_FNTESTALPHABETPASS2:// сбор алфавитной статистики в столбце перед вторым проходом
		EXPORT(RSTR_TestAlphabet);
		break;
		case RSTR_FNSTUDYALPHABETPASS2://выбор алфавита столбца по статистике перед вторым проходом
		EXPORT(RSTR_StudyAlphabetPass2);
		break;
		case RSTR_FNCHANGELINENUMBER://изменить номер строки
		EXPORT(RSTR_ChangeLineNumber);
		break;
		case RSTR_SETSPECPRJ: // установить признак спец-проекта
		EXPORT(RSTR_SetSpecPrj);
		break;
		default:
		wLowRC = RSTR_ERR_NOTIMPLEMENT;
		rc = FALSE;
		break;
	}
#undef EXPORT
#undef CASE_DATA

	return rc;
}

RSTR_FUNC(Bool32) RSTR_SetImportData (uint32_t dwType, const void * pData)
{
	wLowRC = RSTR_ERR_NO;
#define CASE_DATA(a,b,c)        case a: c = *(b *)pData; break
#define CASE_PDATA(a,b,c)       case a: c = (b)pData; break
#define CASE_ADATA(a,c) case a: strcpy(c,(char *)pData); break

	switch(dwType)
	{
		case RSTR_Word8_Language:
		language=*(uchar*)pData;
		multy_language=FALSE;
		if( language==LANG_RUSENG )
		{
			language= LANG_RUSSIAN;
			multy_language=TRUE;
		}
		if( language==LANG_SERBIAN )
		{
			language= LANG_RUSSIAN;
			langSer=TRUE;
		}
		if( language==LANG_UKRAINIAN )
		{
			language= LANG_RUSSIAN;
			langUkr=TRUE;
		}
		if( language==LANG_BULGAR ) // 01.09.2000 E.P.
		{
			language= LANG_RUSSIAN;
			langBul=TRUE;
		}

		break;
		CASE_DATA( RSTR_Word16_Resolution ,uint16_t,actual_resolution);
		CASE_DATA( RSTR_Word8_Fax1x2,uchar ,fax1x2);
		CASE_DATA( RSTR_Word8_P2_disable,uchar ,p2_disable);
		CASE_DATA( RSTR_Word8_Matrix,uchar ,matrix);
		CASE_DATA( RSTR_Word8_P2_active,uchar ,p2_active);
		CASE_DATA( RSTR_Word8_Spell_check,uchar ,spell_check);
		CASE_DATA( RSTR_Word8_spec_camera,uchar ,spec_camera);
		CASE_DATA( RSTR_Word8_spec_nolinpen,uchar ,no_linpen);
		CASE_ADATA(RSTR_pchar_user_dict_name ,user_dict_name);
		case RSTR_OcrPath:
		strcpy(lnOcrPath,(char *)pData);
		strcpy(lnOcrLingPath,(char *)pData);
		/*strcat(lnOcrLingPath,"\\BIN\\");*/
		strcat(lnOcrLingPath, "/");
		break;
		case RSTR_Word8_mmx:
		mmx=*(uchar*)pData;
		if( mmx )
		set_MMX_addr();
		else
		set_all_addr();
		break;
		case RSTR_pchar_temp_dir:
		{
			char tmp_dir[256] = "";
			char* tmp_ctb_name = strrchr(local_ctb_name, '/');
			char* tmp_gray_name = strrchr(local_grey_ctb, '/');
			char tmp_ctb[256] = "";
			char tmp_gray[256] = "";
			if (strlen((char*)pData) > 256)
			{
				wLowRC = RSTR_ERR_VALUE;

				return FALSE;
			}

			strcpy(tmp_dir, (char *)pData);

			if( tmp_dir[strlen(tmp_dir)-1]!='/' )
			{
				if (strlen(tmp_dir) < 255)
				strcat(tmp_dir,"/");
				else
				{
					wLowRC = RSTR_ERR_VALUE;

					return FALSE;
				}
			}

			if (!tmp_ctb_name)
			tmp_ctb_name = local_ctb_name;
			else
			tmp_ctb_name++;

			if (!tmp_gray_name)
			tmp_gray_name = local_grey_ctb;
			else
			tmp_gray_name++;

			if (MAX(strlen(tmp_ctb_name), strlen(tmp_gray_name)) + strlen(tmp_dir) > 256)
			{
				wLowRC = RSTR_ERR_VALUE;

				return FALSE;
			}

			strcpy(tmp_ctb, tmp_ctb_name);
			// strcpy(local_ctb_name, tmp_dir);
			// strcat(local_ctb_name, tmp_ctb);

			strcpy(tmp_gray, tmp_gray_name);
			// strcpy(local_grey_ctb, tmp_dir);
			// strcat(local_grey_ctb, tmp_gray);
			/*
			 strcpy(local_ctb_name,(char *)pData);
			 if( local_ctb_name[strlen(local_ctb_name)-1]!='\\' )
			 strcat(local_ctb_name,"\\");
			 strcat(local_ctb_name,LOCAL_CTB_NAME);
			 strcpy(local_grey_ctb,(char *)pData);
			 if( local_grey_ctb[strlen(local_grey_ctb)-1]!='\\' )
			 strcat(local_grey_ctb,"\\");
			 strcat(local_grey_ctb,LOCAL_GREY_CTB);
			 strcpy(windows_tmp_dir,(char *)pData);
			 */
			break;

		}
		case RSTR_FNIMP_FREE:
		my_free=(void*)pData;
		break;
		case RSTR_FNIMP_ALLOC:
		my_alloc=(void*)pData;
		break;
		case RSTR_FNIMP_GETCOLORS:
		my_get_colors=(void*)pData;
		break;
		case RSTR_CTB_BASE_NAME:
		if (strlen((char*)pData) > 256)
		{
			wLowRC = RSTR_ERR_VALUE;

			return FALSE;
		}

		strcpy(local_ctb_name,(char *)pData);
		break;
		case RSTR_CTB_GRAY_NAME:
		if (strlen((char*)pData) > 256)
		{
			wLowRC = RSTR_ERR_VALUE;

			return FALSE;
		}

		strcpy(local_grey_ctb,(char *)pData);
		break;

		default:
		wLowRC = RSTR_ERR_NOTIMPLEMENT;
		return FALSE;
	}
#undef CASE_DATA
#undef CASE_PDATA

	return TRUE;
}

void store_colors(CSTR_line lino) {
	CSTR_rast rst = CSTR_GetFirstRaster(lino);
	CSTR_rast_attr attr;

	for (rst = CSTR_GetNext(rst); rst; rst = CSTR_GetNext(rst)) {
		CSTR_GetAttr(rst, &attr);
		my_get_colors(attr.row, attr.col, attr.w, attr.h, &attr.ColorLtr,
				&attr.ColorBack);
		CSTR_GetAttr(rst, &attr);
	}

	return;
}

int16_t text_findstat(char * w) {
#ifdef     _USE_SPELLING_
	int32_t Check = 0;
	if( strlen(w)>32 )
	return 0;
	if( !RLING_CheckWord((pchar)w, &Check) )
	return 0;
	return (int16_t)Check;
#else
	return 0;
#endif
}

int16_t text_findstat_aux(char * w) {
#ifdef     _USE_SPELLING_
	int32_t Check = 0;
	if( strlen(w)>32 )
	return 0;
	if( !RLING_CheckSecWord((pchar)w, &Check) )
	return 0;
	return (int16_t)Check;
#else
	return 0;
#endif
}

int16_t text_findstat_agressive(char * w) {
	return 0;
}

jmp_buf * Control_Point() {
	return jumper;
}

/////////////
static int32_t RemoveDustIfPointLine(CSTR_line lin) {
	CSTR_attr lattr;
	CSTR_rast rast, end;
	CSTR_rast_attr attr;
	int32_t ret = 0;

	CSTR_GetLineAttr(lin, &lattr);

	// удалить мелочь от начала до первого большого
	if (lattr.Flags & CSTR_STR_BEGIN_POINTED) {
		rast = CSTR_GetNext(CSTR_GetFirstRaster(lin));
		end = CSTR_GetLastRaster(lin);
		for (; rast && rast != end;) {
			CSTR_GetAttr(rast, &attr);
			if (attr.flg & (CSTR_f_let | CSTR_f_bad))
				break;
			rast = CSTR_DelRaster(rast);
		}

		ret++;
	}

	// удалить мелочь от конца до первого большого
	if (lattr.Flags & CSTR_STR_END_POINTED) {
		rast = CSTR_GetPrev(CSTR_GetLastRaster(lin));
		end = CSTR_GetFirstRaster(lin);
		for (; rast && rast != end;) {
			CSTR_GetAttr(rast, &attr);
			if (attr.flg & (CSTR_f_let | CSTR_f_bad))
				break;
			rast = CSTR_DelRaster(rast);
			rast = CSTR_GetPrev(rast);
		}

		ret++;
	}

	return ret;
}
/////////////////
RSTR_FUNC(Bool32) RSTR_ChangeLineNumber(int32_t add)
{
	line_number += (int16_t)add;
	return TRUE;
}
