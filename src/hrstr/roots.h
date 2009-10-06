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

/****************************************************************************
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              S T R I N G S   E X T R A C T I O N                         *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              ROOTS.H - the header file                                   *
 *                                                                          *
 ***************************************************************************/

#ifndef __ROOTS_H_INCLUDE
#define __ROOTS_H_INCLUDE

#include "ltconfig.h"
#include "cttypes.h"

#define ROOT_NULL           0x00
#define ROOT_LETTER         0x01
#define ROOT_USED           0x02
#define ROOT_RECOGNIZED     0x04
#define ROOT_DUST           0x08
#define ROOT_SPECIAL_LETTER 0x10
#define ROOT_SPECIAL_DUST   0x20
#define ROOT_HORZ_BREAKED   0x40

#if defined (LT_DEBUG) || defined (SE_DEBUG)
#define ROOT_DEBUG          0x80
#endif

#ifdef SE_DEBUG
#define ROOT_FIRST  0x10
#endif

#define DUST_BLOCK_NUMBER           0
#define REMAINDER_BLOCK_NUMBER      1
#define FIRST_REGULAR_BLOCK_NUMBER  2

#define REMOVED_BLOCK_NUMBER   ((int16_t)-1)

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

struct _RootExt;
typedef struct _RootExt ROOT_EXT;
typedef struct _RootExt *PROOT_EXT;

struct _RootExt {
	uint16_t wSegmentPtr;
	uint16_t wLength;
};

typedef struct {
	int xLeft;
	int yTop;
	int xRight;
	int yBottom;
} ROOT_RECT;

#ifndef LT_STAND_ALONE
#define pRoots root_file
#endif

extern ROOT *pRoots;
extern int16_t nRoots;
extern int16_t nOriginalRoots;
extern ROOT *pAfterRoots;
extern ROOT *pAfterOriginalRoots;

extern ROOT_EXT *pRootExts;
extern ROOT_EXT *pAfterRootExts;

extern int nPageHeight;
extern int nSuitablePageHeight;
extern int nPageOffset;

#define MAX_DUST_WIDTH   8
#define MAX_DUST_HEIGHT  8

#define IS_LAYOUT_DUST(a) \
             ((a).nWidth <= MAX_DUST_WIDTH && (a).nHeight <= MAX_DUST_HEIGHT)
#endif
