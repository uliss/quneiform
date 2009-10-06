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

#ifndef __TIGER_EMULATION_H__
#define __TIGER_EMULATION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "cttypes.h"
#include "crldefines.h"

#define MEMSET(a,b,c)       memset(a,b,c)
#define MEMCHR(a,b,c)       memchr(a,b,c)
#define MEMCPY(a,b,c)       memcpy(a,b,c)

#define TG_STREAM  1
#define TB_STREAM  2
#define ED_STREAM  3
#define TX_STREAM  4
#define DB_STREAM  5
#define IM_STREAM  6
#define VC_STREAM  7
#define WK_STREAM  8

#define DIRSEP        '\\'

int16_t TE_open(int16_t, char *, int16_t, int16_t); /* / stream, name, ... */
int16_t TE_write(int16_t, puchar, uint32_t); /* / stream, name, ... */
int16_t TE_table_op(int16_t, int16_t, int16_t, int16_t); /* / table #, ...          */
int32_t TE_read(int16_t, pchar, int32_t); /* / handle, addr, lth */
int16_t TE_close(int16_t); /* / handle            */
char * TE_handle_fgets(int16_t handle, char * s, int16_t len);
int32_t TE_file_length(int16_t);

#define TGOPEN(a,b,c,d)        TE_open(a,b,c,d)
#define TBOPEN(a,b,c,d)        TE_table_op(a,b,c,d)
#define TGREAD(a,b,c)          TE_read(a,b,c)
#define TGWRITE(a,b,c)         TE_write(a,b,c)
#define TGCLOSE(a)             TE_close(a)
#define TGGETS(a,b,c)          TE_handle_fgets(a,b,c)
#define TGFILELTH(a)           TE_file_length(a)

#define BO_READ                O_RDONLY|O_BINARY
#define BO_READ_TEXT           O_RDONLY|O_TEXT

typedef struct {
	uchar let;
	int32_t tr1;
	int32_t tr2;
} StructTree;
////////////////////////////////////////////////////////////////////////////////////
typedef struct elidx {
	uchar ltr; // the letter
	uchar vnum; // the number of the classes
	uint16_t numel; // the number of the first class
} indBOX;
////////////////////////////////////////////////////////////////////////////////////
typedef struct eltab {
	uchar ltr; // the letter
	uchar fnt; // the font number
	uint16_t list; // next of same letter
	uint16_t bnd; // the bound of the typeface
	uint16_t vect[15]; // the vector of the letter
} elmBOX;
///////////////////////////////////////////////////////////////////////////////////
struct mn_struc {
	void *mnfirstbox; // address of the first box
	int16_t mncounter; // (was int16_t) number of living lines in the component
#define mnfree	mnfirstbox	// reference to next free main number
	int16_t mnupper; // upper bound of component
	int16_t mnlower; // lower bound of component
	int16_t mnboxcnt; // number of boxes in component
#define usual_box_count 20 	// heuristic of number of lines in a letter
#define great_box_count	200	// heuristic for number of boxes in a picture
	uchar mnlines; // number of lines in the component
	uchar mnbegs; // number of free line begins
	uchar mnends; // number of free line ends
	uchar mnflag; // flag byte for main number
#define mnpicture 1		// component is a picture
	struct mn_struc *mnnext; // address of next dead component
};
typedef struct mn_struc MN;
/////////////////////////////////////////////////////////////////////////////////////
struct _Root;
typedef struct _Root ROOT;
typedef struct _Root *PROOT;
struct _Root {
	int16_t yRow;
	int16_t xColumn;

	union {
		struct {
			uint16_t wSegmentPtr;
			uint16_t wLength;
		} u2;

		ROOT *pNext;
	} u1;

	int16_t nHeight;
	int16_t nWidth;
	uchar bType;
	uchar bReached;
	int16_t nBlock;
	int16_t nUserNum;
	void *pComp;
};
////////////////////////////////////////////////////////////////////////////////////
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

#endif  //#ifndef __TIGER_EMULATION_H__
