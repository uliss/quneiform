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

#pragma pack(8)

#define LEO_CONT_STD_PAGE_ID 999999

typedef struct tagLeoPageSetup
{
   uint32_t     Options;
      // #define LEO_USEHISTORY 0x00000001
      // #define LEO_...        0x00000001
   int32_t      nResolutionX;
   int32_t      nResolutionY;
   uchar      AlphaStylesTable[256];
      // AlphaTable[i]==0 <=> no such letter on the page;
      // else
      // if  AlphaTable[i] & LS_HAND => such handprinted letter;
      // if  AlphaTable[i] & LS_INDX => such index letter; ...
   int32_t      nIdPage;
   uchar      _reserved[52-4];
}  LeoPageSetup;


typedef struct tagLeoFieldSetup
{
   uint32_t       nStyle;
   Point16     BoxSize;
   uchar       AlphaTable[256];  // AlphaTable[i]==0 <=> no such letter
   /// multi-passing:
   int32_t       nLeoInternal; // initially set to 0; modifyed only by LEO
   int32_t       nFieldNo;     // field num
   uchar       _reserved[240];
}  LeoFieldSetup;


#define REC_STATUS_METHODS_FINAL  0x0001
#define REC_STATUS_METHODS_3X5    0x0002
#define REC_STATUS_METHODS_NET    0x0004
#define REC_STATUS_METHODS_TRE    0x0008
#define REC_STATUS_METHODS_MSK    0x0010
#define REC_STATUS_METHODS_EVN    0x0020
#define REC_STATUS_METHODS_NDX    0x0040
#define REC_STATUS_METHODS_VEC    0x0080
#define REC_STATUS_METHODS_NCU    0x0800      // Global NET CUBE
#define REC_STATUS_METHODS_SCE    0x1000      // Sceleton
#define REC_STATUS_METHODS_PLN    0x2000      // Sceleton
#define REC_STATUS_METHODS_CUT_LEFT    0x80000000  // Cut left
#define REC_STATUS_METHODS_CUT_RIGHT   0x40000000  // Cut right
#define REC_STATUS_METHODS_CUT    REC_STATUS_METHODS_CUT_LEFT+REC_STATUS_METHODS_CUT_RIGHT  // Cut left or right

#define REC_METHOD_CNT   255
#define REC_METHOD_DIC   254
#define REC_METHOD_LEO   253
#define REC_METHOD_FON   100
#define REC_METHOD_GEN   102

#define REC_METHOD_NULL  0
#define REC_METHOD_3X5   1
#define REC_METHOD_NET   2
#define REC_METHOD_TRE   3
#define REC_METHOD_MSK   4
#define REC_METHOD_EVN   5
#define REC_METHOD_NDX   6
#define REC_METHOD_VEC   7
#define REC_METHOD_NCU   13
#define REC_METHOD_SCE   14
#define REC_METHOD_PLN   15
#define REC_METHOD_PLD   16
#define REC_METHOD_FINAL 17
#define REC_METHOD_TOTAL REC_METHOD_FINAL


#define REC_STATUS_V3X5		    0x0001 // vector 3x5 is ready
#define REC_STATUS_COMP		    0x0002 // can used counter of components
#define REC_STATUS_STIC 	    0x0004 // can used array of sticks
#define REC_STATUS_BROKEN_II    0x0008 // consists of two sticks
#define REC_STATUS_CUT_TOP      0x0100 // cut at top side
#define REC_STATUS_CUT_BOTTOM   0x0200 // cut at bottom side

#define LEO_FONT_NONE   0
#define LEO_FONT_TW     2
#define LEO_FONT_MTR    4

#define LEO_VALID_NONE      0x00
#define LEO_VALID_DIGIT     0x01
#define LEO_VALID_RUSSIAN   0x02
#define LEO_VALID_LATIN     0x04
#define LEO_VALID_ALL       0x08
#define LEO_VALID_LINGVO    0x10
#define LEO_VALID_FONT      0x20
#define LEO_VALID_FINAL     0x40

#define LEO_CONTROL_NONE            0x00
#define LEO_CONTROL_CASE            0x10

#define  LEO_CONTROL_FON_CONFIRMED  0x01
//#define  FFC_FON_CONFIRMED    0x8000  // первая альтернатива однозначно подтверждена
#define  LEO_CONTROL_FON_TWIN       0x02
//#define  FFC_FON_TWIN         0x4000  // несколько первых альтернатив укладываются в близкие кластеры и плохо различимы
#define  LEO_CONTROL_FON_ALERT      0x04
//#define  FFC_FON_ALERT        0x2000  // знакоместо сомнително (плохой размер, наличие других кластеров с такой буквой...)        // но не настолько, чтобы однозначно забраковать
#define  LEO_CONTROL_FON_REJECT     0x08
//#define  FFC_FON_REJECT       0x1000  // результат сегментации и распознавания знакоместа забракован шрифтом
#define  LEO_CONTROL_FON_CHANGE     0x20
//#define  FFC_FON_CHANGE       0x0800  // произвел замену первой альтернативы

#pragma pack()

#endif
