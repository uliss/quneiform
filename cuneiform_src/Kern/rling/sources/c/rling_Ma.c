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

#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "compat_defs.h"
/*************************************************************************/
/* tg_spell.c -  general spelling loop. It scans ED-file,
 reads objects, i.e. info between reliable spaces (don't confuse
 with suspected ones), and estimates words generated from the objects
 to choose the only best word for each object. The best word is being
 outputed as the first alternatives for object positions
 */
/*************************************************************************/

//
// 08-13-93 07:58pm, Mike.
// Comments:  Updating for multi-lingual version of CuneiForm ( release
//            28-June-93 ). Release has been received from Joe by FEDERAL
//            mail.
//
// The main target: To use new stream technology : IOLIB.H.
//
// Process indicator:
//  1. Preliminary updation. 08-13-93 06:40pm, Mike
//
#include "tuner.h"
#include "spelmode.h"
#include "cttypes.h"
#include "tigeremulate.h"
#include "crldefines.h"
#include <ctype.h>
#include <time.h>
#include "crlmemory.h"

#include "recdefs.h"
#include "lang_def.h"

#include "speldefs.h"
#include "spelfunc.h"
#include "compat_defs.h"
//      NEED
//      1)      proceeding ED-file
//      2)      setup language and alphabet
//      3)      load vocabulare tables
//      4)      call function spelling


void * (*my_alloc)(uint32_t) = RLINGAlloc; //rling_alloc;
void (*my_free)(void *) = RLINGFree; //rling_free;
void user_voc_init(void);
void GetRecFileName(int16_t tab, int16_t lang, pchar wname);
void append_by_lang(int16_t lang, pchar src, pchar dst);
///////////////////////////////////////////////////////////
int16_t to_voc();
///////////////////////////////////////////////////////////
void trees_load_rling(void);
static void read_rec_file(int16_t fileno, puchar pool, puchar * end);
static puchar preload_font_BOX(puchar free);
static puchar load_BOX(puchar free);
static puchar preload_font_BOX(puchar free);
static void reload_lang_vocs();
static void correct_let_tables(void); // ўл§лў Ґвбп ў д ©«Ґ EMBBOX.C: load_BOX()
static void correct_letters_pidx_table(void);
static puchar list_BOX(puchar free, int16_t typl);
static void straight_BOX(puchar free);

uchar language;
uchar multy_language;
int16_t vocs_NOK;
uchar fax1x2;
Bool16 FlagMixedText;
puchar ED_file_start = NULL;
puchar ED_file_end = NULL;
puchar ED_out_end = NULL;
puchar svbox_pool = NULL;
int16_t CheckOpenBinType = (int16_t) (O_RDONLY | O_BINARY);
int16_t CheckOpenTxtType = (int16_t) (O_RDONLY | O_TEXT);
int16_t CheckOpenSubType = (int16_t) (S_IREAD);
static puchar memory_pool = NULL; /* start of memory pool */
static puchar memory_pool_end = NULL; /* end of memory pool */
static puchar auto_pool = NULL; /* boxes pool for auto & font tables */
static puchar box_pool = NULL; /* boxes pool for extr_comp */
static puchar events_tree = NULL;
static puchar events_tree_rt = NULL; /* events tree memory start */
static uchar sv_lang = 255;
static puchar tableBOX = NULL; /* BOX table memory start */
static uchar all_loaded = 0;
static puchar fontBOX = NULL; /* BOX table for font  */
static puchar omniBOX = NULL; /* BOX save table for omni  */
static int32_t box_n = BOX_NUM0;
static PROOT root_file = NULL; /* start of the root file */
static puchar full_list[512];
static puchar font_full_list[512];
static puchar omni_full_list[512];
static char seq_name[] = "rec0";
char own_dir[256] = { "./" };
static char txt_ext[16] = { "txt" };
static int16_t file_handle[16] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1 };
static int16_t file_stream[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0 };
static int16_t stream_op[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static char *lang_suff[] = { "", "grm", "frn", "rus", "swe", "spa", "ita",
		"r&e", "ukr", "ser", "cro", "pol", "dan", "por", "dut", "dig", "uzb",
		"kaz", "kaz", // 04.09.2000 E.P.
		"cze", "rom", "hun", "bul", // 04.09.2000 E.P. LANG_CZECH и т.д.
		"slo", // LANG_SLOVENIAN 25.05.2001 E.P.
		"lat", "lit", "est", // LANG_LATVIAN и т.д.
		"tur" // 03.06.2002 E.P.
		};
/////////////////////////////////////////////////////////////////////////////////////
//  exit generation functions
typedef void(*pExitByCatchFunc)(int32_t);
pExitByCatchFunc pExitFunc = NULL;

void SetErrorExit(pExitByCatchFunc pExternalExitFunc) {
	pExitFunc = pExternalExitFunc;
}

void ErrorExit(int Code) {
	if (pExitFunc != NULL)
		pExitFunc(Code);
}
////////////////////////////////////////////////////////////////////////////////////
//
void TE_make_name(int16_t stream, pchar name, pchar wname) {
	pchar pn;
	int16_t l;

	switch (stream) {
	case TG_STREAM:
	case TB_STREAM:
		/* FIXME: own_dir gets changed somewhere so it points to "/" rather than "./".
		 an = own_dir;
		 strcpy (wname, an);
		 */
		strcpy(wname, "./");
		strcat(wname, name);
		break;

	case ED_STREAM:
	case TX_STREAM:

		l = strlen(name);
		pn = name + l;

		while (l--) {
			if (*(--pn) == '.') {
				strcpy(wname, name);
				return;
			}

			if (*pn == DIRSEP)
				break;
		}
		strcpy(wname, name);
		switch (stream) {
		case ED_STREAM:

			strcat(wname, ".ed");
			break;

		case TX_STREAM:

			strcat(wname, txt_ext);
			break;
		}
		break;

	default:

		strcpy(wname, name);
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////
//
void GetRecFileName(int16_t tab, int16_t lang, pchar wname) {
	seq_name[3] = tab + '0';
	append_by_lang(lang, seq_name, wname);
	strcat(wname, ".dat");
}
////////////////////////////////////////////////////////////////////////////////////
//
/*
 int16_t to_voc()
 {
 if (vocs_NOK == 0)
 {
 ed_load();
 if (!spelling(comp_file, comp_lth))
 ed_flip();

 return 0;
 }
 return 1;
 }
 */
////////////////////////////////////////////////////////////////////////////////////
//
void append_by_lang(int16_t lang, pchar src, pchar dst) {
	strcpy(dst, src);

	if ((lang >= 0) && (lang < LANG_TOTAL)) {
		strcat(dst, lang_suff[lang]);
	}
	/* Закомментарил 09.07.2001 E.P.
	 else
	 {
	 if( is_cen_language(language) && // 08.09.2000 E.P.
	 n>0 && n<4 )
	 strcat(dst,"CEN"); // rec1,rec2,rec3 for croatian

	 strcat(dst,"R&E"); // rec1,rec2,rec3 for Rus/Eng mode
	 }
	 */
}
////////////////////////////////////////////////////////////////////////////////////
//
int16_t TE_open(int16_t Stream, pchar Name, int16_t Type, int16_t SubType) {
	char Wname[160];
	int16_t i;
	int16_t fn;

	TE_make_name(Stream, Name, Wname);

	for (fn = 1; fn < 16; fn++) {
		if (file_handle[fn] == -1) {
			i = open_data_file(Name, Type); /*, SubType); */

			if (i < 0)
				return -1;

			file_handle[fn] = i;
			file_stream[fn] = Stream;
			stream_op[Stream]++;

			return fn;
		}
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////////
//
int16_t TE_write(int16_t File, puchar Data, uint32_t Size) {
	int16_t i;

	if ((i = file_handle[File]) < 0)
		return -1;

	return write(i, Data, Size);
}
////////////////////////////////////////////////////////////////////////////////////
//
int16_t TE_table_op(int16_t Stream, int16_t Language, int16_t Type,
		int16_t SubType) {
	char Wname[20];

	GetRecFileName(Stream, Language, Wname);
	return TE_open(TB_STREAM, Wname, Type, SubType);
}

int32_t TE_read(int16_t File, pchar Buffer, int32_t Size) {
	int16_t i;

	if ((i = file_handle[File]) < 0)
		return -1;

	return read(i, Buffer, Size);
}

int16_t TE_close(int16_t File) {
	int16_t i, s;

	if ((File < 0) || (File > 9))
		return -1;

	if ((i = file_handle[File]) < 0)
		return -1;

	close(i);
	s = file_stream[File];
	stream_op[s]--;
	file_handle[File] = -1;
	file_stream[File] = 0;
	return 0;
}

pchar TE_handle_fgets(int16_t Handle, pchar Buff, int16_t Lenght) {
	char ch;
	pchar ptr = Buff;
	int16_t ret;

	if (Handle <= 0 || Buff == NULL || Lenght < 1) {
		return NULL;
	}

	while (Lenght > 1) {
		ret = (int16_t) TE_read(Handle, &ch, 1L);

		if (ret < 0) {
			return NULL;
		} /* ERROR */

		if (ret == 0) {
			break;
		} /* EOF   */

		/* MinGW file io seems to turn windows file endings into \n
		 so break -- no skip as when the \r is passed through */
#if defined(WIN32) || defined(__CYGWIN__)
		if ( ch == '\n' ) {
			break;
		}
#endif
		if (ch == '\r') /* JussiP: The data files have Windows line endings. */
		{
			TE_read(Handle, &ch, 1L); /* Skip the following \n. */
			break;
		} /* EOL   */

		*(ptr++) = ch;
		Lenght--;
	}

	*ptr = 0;

	if (ptr != Buff)
		return Buff;

	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
int32_t TE_file_length(int16_t fn) {
	int16_t i;

	if ((i = file_handle[fn]) < 0)
		return -1;

	return filelength(i);
}

void ed_out_write(puchar p, uint16_t size) {
	if (ED_out_end) {
		memcpy(ED_out_end, p, size);
	}

	ED_out_end += size;
}
///////////////////////////////////////////////////////////////////////////////////
// протащено
void trees_load_rling() {
	// exeption
	//////////////////////////////////////////////////////////////////////
	// грузим только словари, память только под них
	memory_pool = svbox_pool;
	memory_pool_end = memory_pool + SizeTables;

	reload_lang_vocs();

	//      all_loaded = 1;

}
////////////////////////////////////////////////////////
//
void read_rec_file(int16_t fileno, puchar pool, puchar * end) {
	uint32_t l;
	int16_t h;

	h = TBOPEN(fileno, -1, (int16_t) (O_RDONLY | O_BINARY), 0);

	if (h == -1)
		ErrorExit(RLING_ERROR_CANT_OPEN_TABLE);

	l = TGREAD(h, pool, 0x100000);
	TGCLOSE(h);
	l = (l + 15) & -16;
	*end = pool + l;
}
//////////////////////////////////////////////////////////////////////////////////////
// протащено из filesys.c
void reload_lang_vocs() {
	//int32_t roots_lth;

	correct_letters_pidx_table();
	correct_let_tables();
	vocs_NOK = 0;
	sv_lang = language;

	box_pool = svbox_pool;
	box_pool = load_stat_dict(box_pool);
	box_pool += (memory_pool - box_pool) & 0xf;
	user_voc_init();

	if (vocs_NOK)
		box_pool = svbox_pool;

	if ((memory_pool + SizeTables) < box_pool)
		ErrorExit(RLING_ERROR_MEMORY_FAULT);
	// Max size for recog tables storing
	box_pool = memory_pool + SizeTables;
	//#undef SizeTables

	//      roots_lth = box_n;
	//      roots_lth *= BOXSIZE;
	//      root_file = (PROOT)((puchar)box_pool + roots_lth);

	//      if (memory_pool_end <= (puchar)root_file)
	//              ErrorExit(9);
}
////////////////////////////////////////////////////////////////////////////////////////
// протащено из acc_tabs.c
void correct_let_tables(void) // ўл§лў Ґвбп ў д ©«Ґ EMBBOX.C: load_BOX()
{
	//
	//  пока ничего нет
	//
}
////////////////////////////////////////////////////////////////////////////////////////
// протащено из proptab.c
void correct_letters_pidx_table(void) {
	//
	//  пока ничего нет
	//
}
