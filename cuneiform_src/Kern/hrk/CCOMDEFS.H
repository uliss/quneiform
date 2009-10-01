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

#ifndef H_ccomdef_h
#define H_ccomdef_h

#include "recdefs.h"
#pragma pack (push,8)

#define CCOM_VERSION_CODE   1
#define CCOM_DUST_SIZE      8
//-------------------- user blocks      --------------------------
struct _USER_BLOCK {
	uint32_t code;
	int32_t size;
	uchar * data;
	struct _USER_BLOCK * next_block;
};
typedef struct _USER_BLOCK CCOM_USER_BLOCK;
#define CCOM_UB_DENSITY     1
#define CCOM_UB_BEGENDS     2
#define CCOM_UB_CAPDROPLN   3
#define CCOM_UB_MERGEFRAME  4
#define CCOM_UB_SIZELINEREP 5
//-------------------- component in kit --------------------------

struct _CCOM_comp {
	int16_t upper; // upper boundary of component
	int16_t left; // left boundary of component
	int16_t h; // height of component
	int16_t w; // width of component
	// 8
	uchar rw; // raster width in bytes
	uchar type; // recognition type :
#define CCOM_CH_PERFECT    1      // perfect type defined
#define CCOM_CH_LETTER     2      // letter type
#define CCOM_CH_DUST       4      // dust type
#define CCOM_CH_PUNCT      8      // punctuation
#define CCOM_CH_STICK     16      // vertical bar component
#define CCOM_CH_GREAT     32      // great component
#define CCOM_CH_MERGE     64      // merged components
#define CCOM_CH_NOTLTR   128      // not letter or punctuation
	int16_t nl; // number of lines
	int16_t size_linerep; // sizeof to line representation
	uchar * linerep; // linear (inteval) representation
	uchar begs; // number of free begins
	// 16
	uchar ends; // number of free ends
	uchar large; // large type :
#define CCOM_LR_UNDERLINED  1   // component was underlined
#define CCOM_LR_TAKEN       2   // taken to line at dust_ini
#define CCOM_LR_LINEREP     4   // store line representation
#define CCOM_LR_KILLED      8   // killed component
	uchar scale; // scale of the component
#define CCOM_LONGLINES      128 // int16 end, int16 len
	uchar cs; // respond from events
	uchar pidx; // proportional index (ref.)            *
	uchar reasno; // proportional criteria messages
	int16_t numcomp;
	CCOM_USER_BLOCK * user_block; // user information
	// 24
#define CCOM_USER_NO    0
	struct _CCOM_comp * next_comp; // goto next comp from the global list
	RecVersions *vers; // recognition collection
	// 32
};
typedef struct _CCOM_comp CCOM_comp;

//-------------------- linear (interval) representation ----------------------

//	At the beginning of line representation - word of total length -
//	not use it, simply skip
//	At end of each line zero byte as mark of line end
//	After last line zero word

//	line header
typedef struct _lnhead {
	int16_t lth; // length of one line representation
	int16_t h; // height of line
	int16_t row; // relative row of line start
	uint16_t flg; // flags of free beg and free end
#define CCOM_l_fbeg     0x20
#define CCOM_l_fend     0x80
#define CCOM_l_cbeg     0x02
#define CCOM_l_cend     0x08
} CCOM_lnhead;
typedef struct _lnhead CCOM_linerep; // alias

//	one interval
typedef struct _interval {
	uchar l; // length of interval
	uchar e; // end of interval coordinates
} CCOM_interval;

typedef struct _interval16 {
	uint16_t l; // length of interval
	uint16_t e; // end of interval coordinates
} CCOM_interval16;

typedef struct _CCOM_cont {
	CCOM_comp * first;
	CCOM_comp * last;
	struct _CCOM_cont * next;
	struct _CCOM_cont * prev;
	int32_t nall, nsmall[8];
	Bool16 kill_dust_mode, language, style_new;
} CCOM_cont;

typedef void* CCOM_handle;
#pragma pack (pop)
#endif

