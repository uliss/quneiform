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
#ifndef __MPUMA_H__
#define __MPUMA_H__
#include "resource.h"
#include "puma.h"
#include "dpuma.h"
#include "pumadef.h"
#include "ccom/ccom.h"
#include "ced/ced.h"
#include "cfio/cfio.h"
#include "cpage/cpage.h"
#include "rimage/criimage.h"
#include "cstr/cstr.h"
#include "cimage/ctiimage.h"
#include "cline.h"
#include "exc/exc.h"
#include "rblock/rblock.h"
#include "rline/rline.h"
#include "rfrmt/rfrmt.h"
#include "rout/rout.h"
#include "rpic/rpic.h"
#include "rpstr/rpstr.h"
#include "rstr/rstr.h"
#include "rstuff.h"
#include "rverline/rverline.h"
#include "rmarker/rmarker.h"
#include "rselstr.h"
#include "mpumatime.h"
#include "rshelllines/rsl.h"
#include "rreccom/rreccom.h"
#include "rcorrkegl/rcorrkegl.h"

#include <string>

// predefined
#ifdef __PUMA_CPP__
#define EXTERN
#define VAL(a)      = a
#define VALM(a)     = {a}
#define VAL2(a,b)   = { a,b }
#define VAL4(a,b,c,d)   = { a,b,c,d }
#else
#define EXTERN      extern
#define VAL(a)
#define VALM(a)
#define VAL2(a,b)
#define VAL4(a,b,c,d)
#endif
#define NOSAVEBACKUP    //не сохранять предысторию ручной фрагментации - Paul 29-12-2000
// global variables
// 1. Отладочная информаци
EXTERN Handle hDebugRoot VAL(NULL);
EXTERN Handle hDebugCancelStrings VAL(NULL);
EXTERN Handle hDebugCancelRecognition VAL(NULL);
EXTERN Handle hDebugCancelRemoveLines VAL(NULL);
EXTERN Handle hDebugCancelSearchLines VAL(NULL);
EXTERN Handle hDebugCancelOrtoMove VAL(NULL);
EXTERN Handle hDebugCancelAutoTemplate VAL(NULL);
EXTERN Handle hDebugCancelSearchTables VAL(NULL);
EXTERN Handle hDebugCancelVerifyLines VAL(NULL);
EXTERN Handle hDebugCancelSearchDotLines VAL(NULL);
EXTERN Handle hDebugCancelFormatted VAL(NULL);
EXTERN Handle hDebugCancelBinarize VAL(NULL);
EXTERN Handle hDebugCancelComponent VAL(NULL);
EXTERN Handle hDebugCancelComponentSecond VAL(NULL);
EXTERN Handle hDebugCancelExtractBlocks VAL(NULL);
EXTERN Handle hDebugCancelStringsPass2 VAL(NULL);
EXTERN Handle hDebugCancelStringsColor VAL(NULL);
EXTERN Handle hDebugCancelPostRecognition VAL(NULL);
EXTERN Handle hDebugCancelPostSpeller VAL(NULL);
EXTERN Handle hDebugCancelSearchPictures VAL(NULL);
EXTERN Handle hDebugCancelLinePass3 VAL(NULL);
EXTERN Handle hDebugCancelSearchNegatives VAL(NULL);
EXTERN Handle hDebugEnableSearchSegment VAL(NULL);
EXTERN Handle hDebugCancelVertCells VAL(NULL);
EXTERN Handle hDebugEnableSaveJtl VAL(NULL);
EXTERN Handle hDebugEnableSaveCstr1 VAL(NULL);
EXTERN Handle hDebugEnableSaveCstr2 VAL(NULL);
EXTERN Handle hDebugEnableSaveCstr3 VAL(NULL);
EXTERN Handle hDebugEnableSaveCstr4 VAL(NULL);
EXTERN Handle hDebugCPAGEStorage VAL(NULL);
// 2.
EXTERN Handle hDebugRootStuff VAL(NULL);
EXTERN Handle hDebugBinarize VAL(NULL);
EXTERN Handle hDebugAutoTemplate VAL(NULL);
EXTERN Handle hDebugLines VAL(NULL);
EXTERN Handle hDebugVerLines VAL(NULL);
EXTERN Handle hDebugRSL VAL(NULL);
// 3
EXTERN Handle hDebugRootLayout VAL(NULL);
EXTERN Handle hDebugLayout VAL(NULL);
EXTERN Handle hDebugPictures VAL(NULL);
EXTERN Handle hDebugSegment VAL(NULL);
EXTERN Handle hDebugNeg VAL(NULL);
EXTERN Handle hDebugFon VAL(NULL);
EXTERN Handle hDebugTables VAL(NULL);
EXTERN Handle hDebugStrings VAL(NULL);
EXTERN Handle hDebugPostLines VAL(NULL);
EXTERN Handle hDebugSVLines VAL(NULL);
EXTERN Handle hDebugSVLinesStep VAL(NULL);
EXTERN Handle hDebugSVLinesData VAL(NULL);
// 4
EXTERN Handle hDebugRootRecognition VAL(NULL);
EXTERN Handle hDebugRecognition VAL(NULL);
EXTERN Handle hDebugSpell VAL(NULL);
// 5
EXTERN Handle hDebugRootFormatting VAL(NULL);
EXTERN Handle hDebugConverters VAL(NULL);
EXTERN Handle hDebugCeglCorr VAL(NULL);
EXTERN Handle hDebugMainDebug VAL(NULL);
EXTERN Handle hDebugMainTimeControl VAL(NULL);
EXTERN Handle hDebugLayoutFromFile VAL(NULL);
EXTERN Handle hDebugLayoutToFile VAL(NULL);
EXTERN Handle hDebugHandLayout VAL(NULL);
EXTERN Handle hDebugCancelPropertyConsole VAL(NULL);
EXTERN Handle hDebugCancelConsoleOutputText VAL(NULL);
EXTERN Handle hDebugCancelConsoleKeglOutput VAL(NULL);
EXTERN Handle hDebugCancelRanalFrm VAL(NULL);
EXTERN Handle hDebugPrintBlocksCPAGE VAL(NULL);
EXTERN Handle hDebugCancelFictive VAL(NULL);
EXTERN Handle hDebugCancelTurn VAL(NULL);
EXTERN Handle hDebugEnablePrintFormatted VAL(NULL);

EXTERN unsigned nDebugReturnCode VAL(0);
//Allex  при разделении бинаризации и обработки сырь
//имя картинки 'lpRecogName' сделано глабольной переменной
EXTERN const char * glpRecogName VAL(NULL);
EXTERN Bool32 grc_line VAL(TRUE);
EXTERN Bool32 gneed_clean_line VAL(FALSE);
EXTERN Bool32 gKillVSLComponents VAL(TRUE);
EXTERN uchar gnSpecialProject VAL(0);
//allex
EXTERN uint32_t g_flgUpdate VAL(0);

Bool32 IsUpdate(uint32_t flg);
void SetUpdate(uint32_t flgAdd, uint32_t flgRemove);
//functions
void SetReturnCode_puma(uint32_t rc);

#endif
