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

#include "stdafx.h"

#include "resource.h"
#include "ced_struct.h"
#include "cedint.h"
//#include "mymem.h"
//#include "edfile.h"
//#include "edext.h"

Bool32 func_char(puchar p, uint32_t lth);
Bool32 func_spec(puchar p, uint32_t lth);

FNCFIOReadMemoryFromFile MemFromFile;
FNCFIOLockMemory Lock;
FNCFIOUnlockMemory Unlock;
FNCFIOFreeMemory Free;
//FNCFIOAllocMemory					Alloc;
FNCFIOOpenFreeFile Open;
FNCFIOCloseFreeFile Close;
FNCFIOWriteToFile Write;

static unsigned char ed_table[] = { sizeof(struct bit_map_ref), /* 0  SS_BITMAP_REF         */
sizeof(struct text_ref), /* 1  SS_TEXT_REF           */
sizeof(struct font_kegl), /* 2  SS_FONT_KEGL          */
sizeof(struct kegl), /* 3  SS_KEGL               */
sizeof(struct shift), /* 4  SS_SHIFT              */
sizeof(struct retrieve_level), /* 5  SS_RETR_LEVEL         */
sizeof(struct underline), /* 6  SS_UNDERLINE          */
sizeof(struct dens_print), /* 7  SS_DENS_PRINT         */
sizeof(struct tabul), /* 8  SS_TABUL              */
0x41, /* 9  SS_TABL_TABUL         */
0x84, /* 0A SS_SHEET_DESCR        */
sizeof(struct fragm_disk), /* 0B SS_FRAGMENT           */
sizeof(struct step_back), /* 0C SS_STEP_BACK          */
sizeof(struct line_beg), /* 0D SS_LINE_BEG           */
sizeof(struct position), /* 0E SS_POSITION           */
sizeof(struct EdTagLanguage), /* 0F SS_LANGUAGE           */
sizeof(struct table_conform_sizes),/* 10 SS_TABL_CONFORM_SIZES */
sizeof(struct group_words), /* 11 SS_GROUP_WORDS        */
sizeof(struct group_symbols), /* 12 SS_GROUP_SYMBOLS      */
0, /* 13 Unused code           */
0, /* 14 Unused code           */
2, /* 15 ASCII symbol 'Start Paragraph'      */
sizeof(struct border), /* 16 SS_BORDER             */
0x82, /* 17 SS_TABLE_HEADER       */
0x82, /* 18 SS_LIST_OF FRAGMENTS  */
0, /* 19 Unused code           */
0, /* 1A Unused code           */
0, /* 1B Unused code           */
0x83, /* 1C Special code of Extention ED */
sizeof(struct aksant), /* 1D SS_AKSANT             */
sizeof(struct neg_half_space), /* 1E SS_NEG_HALF_SPACE     */
sizeof(struct pos_half_space) /* 1F SS_POS_HALF_SPACE     */
};

//###########################################
void CED_SetRawDataProc(FNRDProc proc) {
#define DEC_FUN(a,b,c)  b=MyRawData##b

	DEC_FUN(void, CED_BitmapRef,(const bit_map_ref* pt));
	DEC_FUN(void, CED_TextRef,(const text_ref* pt));
	DEC_FUN(void, CED_FontKegl,(const font_kegl *pt));
	DEC_FUN(void, CED_Kegl,(const kegl* pt));
	DEC_FUN(void, CED_Shift,(const shift* pt));
	DEC_FUN(void, CED_RetrieveLevel,(const retrieve_level* pt));
	DEC_FUN(void, CED_Underline,(const underline* pt));
	DEC_FUN(void, CED_DensPrint,(const dens_print* pt));
	DEC_FUN(void, CED_Tabul,(const tabul* pt));
	DEC_FUN(void, CED_TablTabul,(const tabl_tabul* pt));
	DEC_FUN(void, CED_SheetDiskDescr,(const sheet_disk_descr* pt));
	DEC_FUN(void, CED_FragmDiskDescr,(const fragm_disk_descr* pt));
	DEC_FUN(void, CED_FragmDisk,(const fragm_disk* pt));
	DEC_FUN(void, CED_StepBack,(const step_back* pt));
	DEC_FUN(void, CED_LineBeg,(const line_beg* pt));
	DEC_FUN(void, CED_Position,(const position* pt));
	DEC_FUN(void, CED_EdTagLanguage,(const EdTagLanguage* pt));
	DEC_FUN(void, CED_TableConformSizes,(const table_conform_sizes* pt));
	DEC_FUN(void, CED_GroupWords,(const group_words* pt));
	DEC_FUN(void, CED_GroupSymbols,(const group_symbols* pt));
	DEC_FUN(void, CED_Border,(const border* pt));
	DEC_FUN(void, CED_TableHeader,(const table_header* pt));
	DEC_FUN(void, CED_ListOfFragments,(const list_of_fragments* pt));
	DEC_FUN(void, CED_Extention,(const edExtention* pt,const void* ptExt));
	DEC_FUN(void, CED_ExtentionNew,(const edExtentionNew* pt,const void* ptExt));
	DEC_FUN(void, CED_Aksant,(const aksant* pt));
	DEC_FUN(void, CED_Letter,(const letter* pt,const uint32_t alternatives));

#undef DEC_FUN

	RDProced = proc;
}

/*CED_FUNC(*/uint32_t/*)*/CED_ReadED(char * file, Bool32 readFromFile,
		uint32_t bufLen) {
	uchar code;
	HANDLE PedHandle;
	uint32_t lth, ret;
	puchar start, edscan_stop;

	if (readFromFile) {
		ret = MemFromFile((pchar) file, &PedHandle);
		if (ret == 0)
			return 0;
		start = (puchar) Lock(PedHandle);
		if (!start) {
			Unlock(PedHandle);
			Free(PedHandle);
			return 0;
		}
	} else {
		start = (uchar*) file;
		ret = bufLen;
	}
	puchar curr = edscan_stop = start;
	puchar end = start + ret;

	next_symb: if (curr >= end)
		goto quit;
	if (*curr >= ' ') {
		start = curr;
		while (end - curr >= 2) {
			curr += 2;
			if (!(*(curr - 1) & 1))
				goto char_proc;
		}
		edscan_stop = start;
		goto quit;

		char_proc: if (func_char(start, (uint16_t)(curr - start)))
			goto next_symb;
		edscan_stop = start;
		goto quit;
	}

	/* Search code */
	/*If the size exceeds 0x40, then actual size is stored in structure itself. Its offset is equal to (value in a table & 0xf). length of the size is 2 bytes if value in table >=0x80 and 1 byte otherwise.
	 */
	code = ed_table[*curr];

	if ((code & 0xc0) == 0) {
		if (code == 0) {
			curr += sizeof(*curr);
			goto next_symb;
		}
		// { edscan_stop = curr;        return 32; }
		if (end - curr < code) {
			edscan_stop = curr;
			goto quit;
		}
		if (func_spec(curr, code)) {
			curr += code;
			goto next_symb;
		}
		edscan_stop = curr;
		goto quit;
	}

	if (code & 0x80) {
		//SS_EXTENTION can be either 16 or 32 bit.
		if (*(uchar*) curr != SS_EXTENTION)
			lth = *(uint16_t*)(curr + (code & 0xf));
		else {
			if (!((*(uint16_t*) (curr + 1)) & 0x8000))
				lth = *(uint16_t*)(curr + (code & 0xf));
			else
				lth = *(uint32_t*)(curr + (code & 0xf));
		}
	} else
		lth = *(puchar)(curr + (code & 0xf));

	if (lth == 0) {
		curr += sizeof(*curr);
		goto next_symb;
	}
	//{ edscan_stop = curr; return 32; }
	if ((unsigned) (end - curr) < lth) {
		edscan_stop = curr;
		goto quit;
	}
	if (func_spec(curr, lth)) {
		curr += lth;
		goto next_symb;
	}
	edscan_stop = curr;
	goto quit;

	quit: ret = (int) (edscan_stop - start);
	if (readFromFile) {
		Unlock(PedHandle);
		Free(PedHandle);
	}
	return ret;
}

Bool32 func_char(puchar p, uint32_t lth) {
	const struct letter *pt = (struct letter*) p;
	CED_Letter(pt, lth / 2);
	return TRUE;
}

Bool32 func_spec(puchar p, uint32_t lth) {

	switch (*p) {
	case SS_BITMAP_REF: {
		const struct bit_map_ref *pt = (struct bit_map_ref*) p;
		CED_BitmapRef(pt);
		break;
	}

	case SS_TEXT_REF: {
		const struct text_ref *pt = (text_ref *) p;
		CED_TextRef(pt);
		break;
	}

	case SS_FONT_KEGL: {
		const struct font_kegl *pt = (struct font_kegl*) p;
		CED_FontKegl(pt);
		break;
	}

	case SS_KEGL: {
		const struct kegl *pt = (struct kegl*) p;
		CED_Kegl(pt);
		break;
	}

	case SS_SHIFT: {
		const shift *pt = (shift*) p;
		CED_Shift(pt);
		break;
	}

	case SS_RETR_LEVEL: {
		const retrieve_level *pt = (retrieve_level *) p;
		CED_RetrieveLevel(pt);
		break;
	}

	case SS_UNDERLINE: {
		const struct underline *pt = (struct underline*) p;
		CED_Underline(pt);
		break;
	}

	case SS_DENS_PRINT: {
		const struct dens_print *pt = (struct dens_print*) p;
		CED_DensPrint(pt);
		break;
	}

	case SS_TABUL: {
		const tabul *pt = (tabul*) p;
		CED_Tabul(pt);
		break;
	}

	case SS_TABL_TABUL: {
		const tabl_tabul *pt = (tabl_tabul*) p;
		CED_TablTabul(pt);
		break;
	}

	case SS_SHEET_DESCR: {
		const sheet_disk_descr* pt = (sheet_disk_descr*) p;
		CED_SheetDiskDescr(pt);
		for (int i = 0; i < pt->quant_fragm; i++) {
			const fragm_disk_descr *pt1 = (fragm_disk_descr*) (p + i
					* sizeof(fragm_disk_descr) + sizeof(sheet_disk_descr));
			CED_FragmDiskDescr(pt1);
		}
		break;
	}

	case SS_FRAGMENT: {
		const fragm_disk* pt = (fragm_disk*) p;
		CED_FragmDisk(pt);
		break;
	}

	case SS_STEP_BACK: {
		const step_back* pt = (step_back*) p;
		CED_StepBack(pt);
		break;
	}

	case SS_LINE_BEG: {
		const struct line_beg *pt = (struct line_beg*) p;
		CED_LineBeg(pt);
		break;
	}

	case SS_POSITION: {
		const struct position *pt = (struct position*) p;
		CED_Position(pt);
		break;
	}

	case SS_LANGUAGE: {
		const struct EdTagLanguage *pt = (struct EdTagLanguage*) p;
		CED_EdTagLanguage(pt);
		break;
	}

	case SS_TABL_CONFORM_SIZES: {
		const struct table_conform_sizes *pt = (struct table_conform_sizes*) p;
		CED_TableConformSizes(pt);
		break;
	}

	case SS_GROUP_WORDS: {
		const struct group_words *pt = (struct group_words*) p;
		CED_GroupWords(pt);
		break;
	}

	case SS_GROUP_SYMBOLS: {
		const struct group_symbols *pt = (struct group_symbols*) p;
		CED_GroupSymbols(pt);
		break;
	}

	case 0x15: {
		*(p + 1) &= 254;
		func_char(p, 2);
		break;
	}

	case SS_BORDER: {
		const struct border *pt = (struct border*) p;
		CED_Border(pt);
		break;
	}

	case SS_TABLE_HEADER: {
		const struct table_header *pt = (struct table_header*) p;
		CED_TableHeader(pt);
		break;
	}

	case SS_LIST_OF_FRAGMENTS: {
		const struct list_of_fragments *pt = (struct list_of_fragments*) p;
		CED_ListOfFragments(pt);
		break;
	}

	case SS_EXTENTION: {
		const struct edExtention *pt = (struct edExtention*) p;
		if (!(pt->Ecode & 0x8000))
			CED_Extention(pt, (const char*) (p + sizeof(edExtention)));
		else {
			const struct edExtentionNew *pt1 = (struct edExtentionNew*) p;
			CED_ExtentionNew(pt1, (const char*) (p + sizeof(edExtentionNew)));
		}
		break;
	}

	case SS_AKSANT: {
		const struct aksant *pt = (struct aksant*) p;
		CED_Aksant(pt);
		break;
	}

	case SS_NEG_HALF_SPACE: {
		*(p + 1) &= 254;
		func_char(p, 2);
		break;
	}

	case SS_POS_HALF_SPACE:
		*(p + 1) &= 254;
		func_char(p, 2);
		break;

	default: {
		return FALSE;
	}
	}
	return TRUE;
}
