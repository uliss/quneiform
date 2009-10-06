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
 *                                                                          *
 *              S T R I N G S   E X T R A C T I O N                         *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              EXTRACT.H - the header file                                 *
 *                                                                          *
 ***************************************************************************/

# ifndef __EXTRACT_H_INCLUDE
# define __EXTRACT_H_INCLUDE

# include "ltconfig.h"

# include "lterrors.h"
# include "roots.h"
# include "strings.h"
# include "madebug.h"
# include "layout.h"

/****************************************************************************
 *   Blocks access table                                                    *
 ***************************************************************************/
extern BLOCK **pBlockPointer;
extern int nBlocks;
extern int nMinBlock;
extern int nMaxBlock;
extern int nCurrentBlock;
extern BLOCK *pCurrentBlock;

void BlocksAccessTableBuild(void);
void BlocksAccessTableFree(void);

/****************************************************************************
 *   Working with strings                                                   *
 ***************************************************************************/

void StringPrepare(void);
void StringFree(void);

void StringNewDescriptor(void);
Bool StringIncludes(STRING *p, STRING *q);
void StringRemove(STRING *p);
void StringRemoveFromUpDownLists(STRING *p);
void StringSortLetters(STRING *pString);
void StringSortDust(STRING *pString);

void StringAddLetter1(int iRoot);
void StringAddLetter2(STRING *p, int iRoot);
void StringAddDust1(int iRoot);
void StringAddDust2(STRING *p, int iRoot);

void StringAccountRectangle1(int iRoot);
void StringAccountRectangle2(STRING *p, int iRoot);

void StringAccountRepresentationParameters1(int iRoot);
void StringAccountRepresentationParameters2(STRING *p, int iRoot);

STRING *StringAddToList(void);
void StringOutput(void);
void StringUpdate(STRING *p);

void StringsListOutput(void);
void StringsListUpdate(void);
void StringsFreeMemory(void);

void BlocksHystogramsAllocate(void);
void BlocksHystogramsBuild(void);
void BlockHystogramDiscountRoot(BLOCK *pBlock, ROOT *pRoot);
void BlocksHystogramsFreeData(void);
# ifdef SE_DEBUG
void BlockHystogramShow (BLOCK *pBlock);
# endif

extern uchar *pRaster;
extern int nRasterWidth;
extern int nRasterHeight;
extern int nRasterByteWidth;

extern int aRasterHystogram[];
extern int yRasterUpBreakLimit;
extern int yRasterDownBreakLimit;
extern int yRasterBreakLine;

void StringCalculateParameters(STRING *pString);
void StringsFill(void);
void StringsListEdit(void);
void StringsProcessSpecials(void);
void StringsDustAccount(void);
void StringsBreakOnVertical(void);

void StringsForming(void);
void StringsExtract(void);
void SE_FreeAllData(void);

extern int nCurrentFillingRoots;

# ifdef SE_DEBUG
extern int SE_DebugGraphicsLevel;
# endif
# endif
