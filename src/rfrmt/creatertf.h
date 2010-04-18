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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 12.08.99
//
// CreateRtf.h
//
// ============================================================================

#ifndef __CreateRtf_h__
#define __CreateRtf_h__

#include "rtfedwrite.h"
#include "recdefs.h"
#include "cfcompat.h"
#include "lang_def.h"

#include <vector>
#include <cassert>
#include <climits>

namespace CIF
{
class CEDPage;
}

typedef std::vector<uint16_t> vectorWord;
#define  TwipsToEMU_Koef (360000 * 2.54)/1440

#define TIRE                  0x97        //'-'
//Fragment output types
#define FOT_SINGLE            0
#define FOT_FRAME             1
#define FOT_SINGLE_IN_COLUMN  2

//Fragment types
#define FT_TEXT               0
#define FT_FRAME              1
#define FT_TABLE              2
#define FT_PICTURE            3

//HorizontalColumn types
#define HC_SingleTerminal     0
#define HC_AllTerminal        1
#define HC_FrameAndColumn     2
#define HC_AllFrame           3
#define HC_ONLY_PICTURE_TABLE 4

#define MaxFontSize           72
#define DefFontSize           24
#define MAX_BUFFER_SIZE       2048
#define ChangedKeglSize       6

//       wFont bits
#define TG_EDW_SERIF          1      //сериф.
#define TG_EDW_GELV           2      //без сериф.
#define TG_EDW_BOLD           4
#define TG_EDW_ITALIC         16
#define TG_EDW_UNDERLINE      64
#define TG_EDW_NARROW         128     // user defined
#define NOBOLD                0x0004
#define NOCURSIV              0x0008
#define NOSIZE                0x0020

// Formatting Mode
#define USE_NONE              0x0040   // no formatting
#define USE_FRAME_AND_COLUMN  0x0001   // use columns & frames
#define USE_FRAME             0x0002   // use only frames
// Size A4
#define DefaultWidthPage      11906
#define DefaultHeightPage     16838

#define DefMargL              1800 // Left   margin in twips    (the default is 1800).
#define DefMargR              1800 // Right  margin in twips    (the default is 1800).
#define DefMargT              1440 // Top    margin in twips    (the default is 1440).
#define DefMargB              1440 // Bottom margin in twips    (the default is 1440).
#define RTF_TP_LEFT_ALLIGN           0 //- выравниваение по левому краю
#define RTF_TP_RIGHT_ALLIGN          1 //- выравнивание по правому краю
#define RTF_TP_LEFT_AND_RIGHT_ALLIGN 2 //- выравнивание по ширине
#define RTF_TP_CENTER                3 //- выравнивание по центру
#define RTF_TP_ONE                   4 //- каждая строка состоит из одного слова, выравнивание левому краю
#define RTF_TP_TYPE_LINE             5 //- список
namespace CIF
{
class CRtfSector;
class CRtfFragment;
class CRtfHorizontalColumn;
class CRtfVerticalColumn;
class CRtfPage;
class CRtfString;
class CRtfWord;
class CRtfChar;
}

#endif // __CreateRtf_h__
