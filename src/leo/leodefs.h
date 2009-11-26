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

#ifndef __LEODEFS_H
#define __LEODEFS_H

#include "recdefs.h"
#include "point.h"

enum {
    LEO_CONT_STD_PAGE_ID = 999999
};

typedef struct tagLeoPageSetup {
    uint32_t Options;
    int32_t nResolutionX;
    int32_t nResolutionY;
    uchar AlphaStylesTable[256];
    // AlphaTable[i]==0 <=> no such letter on the page;
    // else
    // if  AlphaTable[i] & LS_HAND => such handprinted letter;
    // if  AlphaTable[i] & LS_INDX => such index letter; ...
    int32_t nIdPage;
    uchar _reserved[52 - 4];
} LeoPageSetup;

typedef struct tagLeoFieldSetup {
    uint32_t nStyle;
    CIF::Point16 BoxSize;
    uchar AlphaTable[256]; // AlphaTable[i]==0 <=> no such letter
    /// multi-passing:
    int32_t nLeoInternal; // initially set to 0; modifyed only by LEO
    int32_t nFieldNo; // field num
    uchar _reserved[240];
} LeoFieldSetup;

enum leo_methods_status_t {
    REC_STATUS_METHODS_FINAL = 0x0001,
    REC_STATUS_METHODS_3X5 = 0x0002,
    REC_STATUS_METHODS_NET = 0x0004,
    REC_STATUS_METHODS_TRE = 0x0008,
    REC_STATUS_METHODS_MSK = 0x0010,
    REC_STATUS_METHODS_EVN = 0x0020,
    REC_STATUS_METHODS_NDX = 0x0040,
    REC_STATUS_METHODS_VEC = 0x0080,
    // Global NET CUBE
    REC_STATUS_METHODS_NCU = 0x0800,
    // Sceleton
    REC_STATUS_METHODS_SCE = 0x1000,
    // Sceleton
    REC_STATUS_METHODS_PLN = 0x2000,
    // Cut left
    REC_STATUS_METHODS_CUT_LEFT = 0x80000000,
    // Cut right
    REC_STATUS_METHODS_CUT_RIGHT = 0x40000000,
    // Cut left or right
    REC_STATUS_METHODS_CUT = REC_STATUS_METHODS_CUT_LEFT
    + REC_STATUS_METHODS_CUT_RIGHT
};

enum leo_method_t {
    REC_METHOD_CNT = 255,
    REC_METHOD_DIC = 254,
    REC_METHOD_LEO = 253,
    REC_METHOD_FON = 100,
    REC_METHOD_GEN = 102,
    REC_METHOD_NULL = 0,
    REC_METHOD_3X5 = 1,
    REC_METHOD_NET = 2,
    REC_METHOD_TRE = 3,
    REC_METHOD_MSK = 4,
    REC_METHOD_EVN = 5,
    REC_METHOD_NDX = 6,
    REC_METHOD_VEC = 7,
    REC_METHOD_NCU = 13,
    REC_METHOD_SCE = 14,
    REC_METHOD_PLN = 15,
    REC_METHOD_PLD = 16,
    REC_METHOD_FINAL = 17,
    REC_METHOD_TOTAL = REC_METHOD_FINAL
};

enum leo_status_t {
    // vector 3x5 is ready
    REC_STATUS_V3X5 = 0x0001,
    // can used counter of components
    REC_STATUS_COMP = 0x0002,
    // can used array of sticks
    REC_STATUS_STIC = 0x0004,
    // consists of two sticks
    REC_STATUS_BROKEN_II = 0x0008,
    // cut at top side
    REC_STATUS_CUT_TOP = 0x0100,
    // cut at bottom side
    REC_STATUS_CUT_BOTTOM = 0x0200
};

enum leo_font_t {
    LEO_FONT_NONE = 0, LEO_FONT_TW = 2, LEO_FONT_MTR = 4
};

enum leo_valid_t {
    LEO_VALID_NONE = 0x00,
    LEO_VALID_DIGIT = 0x01,
    LEO_VALID_RUSSIAN = 0x02,
    LEO_VALID_LATIN = 0x04,
    LEO_VALID_ALL = 0x08,
    LEO_VALID_LINGVO = 0x10,
    LEO_VALID_FONT = 0x20,
    LEO_VALID_FINAL = 0x40
};

enum leo_control_t {
    LEO_CONTROL_NONE = 0x00,
    // первая альтернатива однозначно подтверждена
    LEO_CONTROL_FON_CONFIRMED = 0x01,
    // несколько первых альтернатив укладываются в близкие кластеры и плохо различимы
    LEO_CONTROL_FON_TWIN = 0x02,
    // знакоместо сомнително (плохой размер, наличие других кластеров с такой буквой...)
    // но не настолько, чтобы однозначно забраковать
    LEO_CONTROL_FON_ALERT = 0x04,
    // результат сегментации и распознавания знакоместа забракован шрифтом
    LEO_CONTROL_FON_REJECT = 0x08,
    LEO_CONTROL_CASE = 0x10,
    // произвел замену первой альтернативы
    LEO_CONTROL_FON_CHANGE = 0x20
};

#endif
