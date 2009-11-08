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

#include "iolib.h"
#include "layout.h"
#include "extract.h"
#include "kernel.h"
#include "status.h"

#include "dpuma.h"
#include "puma/pumadef.h"
#include "dsnap.h"

Handle hDebugLayout = NULL;
Handle hDebugStrings = NULL;
Handle hShowString = NULL;
Handle hShowCells = NULL;
Handle hDebugCancelPicturesAndTables = NULL;
Handle hDebugCancelGreatComp = NULL;
Handle hShowAlgoritm;
Handle hPageBeforeEditing = NULL;
Handle hPageMatrix = NULL;
Handle hInitialBreaking = NULL;
Handle hInclineCalculating = NULL;
Handle hBlocksBreaking = NULL;
Handle hBlocksGlueing = NULL;
Handle hFirstDustAbsorbtion = NULL;
Handle hRemoveEmptyBlocks = NULL;
Handle hSecondDustAbsorbtion = NULL;
Handle hAlgoritmWork = NULL;
Handle hNotWholeLine = NULL;
Handle hHystogramNull = NULL;
Handle hNotUseAntonCross = NULL;
Handle hSetTrivAverHeight = NULL;//George, 12.11.01 (dd.mm.yy)

uint32_t ZagolovokBreakingCoeff = 3;

void InitDebug() {
    layout = 0;
}

void DoneDebug() {
}

void EnableDebug() {

# ifdef LT_DEBUG
    switch (layout) {
    case 0:
        LT_DebugGraphicsLevel = 0;
        break;
    case 1:
        LT_DebugGraphicsLevel = 1;
        break;
    case 2:
        LT_DebugGraphicsLevel = 2;
        break;
    case 3:
        LT_DebugGraphicsLevel = 3;
        break;
    case 4:
        LT_DebugGraphicsLevel = 4;
        break;
    case 5:
        LT_DebugGraphicsLevel = 1;
        break;
    case 6:
        LT_DebugGraphicsLevel = 1;
        break;
    case 7:
        LT_DebugGraphicsLevel = 1;
        break;
    case 8:
        LT_DebugGraphicsLevel = 1;
        break;
    case 9:
        LT_DebugGraphicsLevel = 0;
        break;
    case 10:
        LT_DebugGraphicsLevel = 0;
        break;
    }
# endif
# ifdef SE_DEBUG
    switch (layout) {
    case 0:
        SE_DebugGraphicsLevel = 0;
        break;
    case 1:
        SE_DebugGraphicsLevel = 0;
        break;
    case 2:
        SE_DebugGraphicsLevel = 0;
        break;
    case 3:
        SE_DebugGraphicsLevel = 0;
        break;
    case 4:
        SE_DebugGraphicsLevel = 0;
        break;
    case 5:
        SE_DebugGraphicsLevel = 1;
        break;
    case 6:
        SE_DebugGraphicsLevel = 2;
        break;
    case 7:
        SE_DebugGraphicsLevel = 3;
        break;
    case 8:
        SE_DebugGraphicsLevel = 4;
        break;
    case 9:
        SE_DebugGraphicsLevel = 0;
        break;
    case 10:
        SE_DebugGraphicsLevel = 0;
        break;
    }
# endif
# ifdef MA_DEBUG
    switch (layout) {
    case 0:
        MA_DebugLevel = 0;
        break;
    case 1:
        MA_DebugLevel = 0;
        break;
    case 2:
        MA_DebugLevel = 0;
        break;
    case 3:
        MA_DebugLevel = 0;
        break;
    case 4:
        MA_DebugLevel = 0;
        break;
    case 5:
        MA_DebugLevel = 0;
        break;
    case 6:
        MA_DebugLevel = 0;
        break;
    case 7:
        MA_DebugLevel = 0;
        break;
    case 8:
        MA_DebugLevel = 0;
        break;
    case 9:
        MA_DebugLevel = 1;
        break;
    case 10:
        MA_DebugLevel = 2;
        break;
    }
#endif
}
