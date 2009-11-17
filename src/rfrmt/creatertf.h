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
#include "edfile.h"
#include "compat_defs.h"

#include <vector>

typedef std::vector<uint16_t> vectorWord;

const char TIRE = 0x97; //'-'

enum rfrmt_fragment_output_t
{
    //Fragment output types
    FOT_SINGLE = 0,
    FOT_FRAME = 1,
    FOT_SINGLE_IN_COLUMN = 2
};

//Fragment types
enum rfrmt_fragment_t
{
    FT_TEXT = 0,
    FT_FRAME = 1,
    FT_TABLE = 2,
    FT_PICTURE = 3
};

//HorizontalColumn types
enum
{
    HC_SingleTerminal = 0,
    HC_AllTerminal = 1,
    HC_FrameAndColumn = 2,
    HC_AllFrame = 3,
    HC_ONLY_PICTURE_TABLE = 4
};

const int MaxFontSize = 72;
const int DefFontSize = 24;
const size_t MAX_BUFFER_SIZE = 2048;
const int ChangedKeglSize = 6;

//       wFont bits
enum
{
    TG_EDW_SERIF = 1, //сериф.
    TG_EDW_GELV = 2, //без сериф.
    TG_EDW_BOLD = 4,
    TG_EDW_ITALIC = 16,
    TG_EDW_UNDERLINE = 64,
    TG_EDW_NARROW = 128, // user defined
    NOBOLD = 0x0004,
    NOCURSIV = 0x0008,
    NOSIZE = 0x0020
};

// Formatting Mode
enum rfrmt_mode_t
{
    USE_NONE = 0x0040, // no formatting
    USE_FRAME_AND_COLUMN = 0x0001, // use columns & frames
    USE_FRAME = 0x0002
// use only frames
};

// Size A4
const int DefaultWidthPage = 11906;
const int DefaultHeightPage = 16838;

const int DefMargL = 1800; // Left   margin in twips    (the default is 1800).
const int DefMargR = 1800; // Right  margin in twips    (the default is 1800).
const int DefMargT = 1440; // Top    margin in twips    (the default is 1440).
const int DefMargB = 1440; // Bottom margin in twips    (the default is 1440).

enum rfrmt_align_t
{
    RTF_TP_LEFT_ALLIGN = 0, //- выравниваение по левому краю
    RTF_TP_RIGHT_ALLIGN = 1,//- выравнивание по правому краю
    RTF_TP_LEFT_AND_RIGHT_ALLIGN = 2, //- выравнивание по ширине
    RTF_TP_CENTER = 3, //- выравнивание по центру
    RTF_TP_ONE = 4, //- каждая строка состоит из одного слова, выравнивание левому краю
    RTF_TP_TYPE_LINE = 5
//- список
};

class CRtfSector;
class CRtfFragment;
class CRtfHorizontalColumn;
class CRtfVerticalColumn;
class CRtfParagraph;
class CRtfString;
class CRtfWord;
class CRtfChar;
class CRtfPage;

struct RtfPageElementCount
{
    uint16_t RtfSectors;
    uint16_t RtfTextFragments;
    uint16_t RtfFrameTextFragments;
    uint16_t RtfPictureFragments;
    uint16_t RtfTableFragments;
    uint16_t RtfStrings;
    uint16_t RtfWords;
    uint16_t RtfChars;
};

#define FONT struct h_font
FONT
{
    char *family;
    char *name;
    int16_t Bold, Italic, Underline;
};
typedef struct hfont_cod
{
    const char *name, *family;
} FONT_COD;

typedef struct RTFLETTER
{
    unsigned char alternative; //  ASCII code.  >= ' '.
    unsigned char probability;
} RTF_LETTER;

const float TwipsToEMU_Koef = (360000 * 2.54) / 1440;
const int K_TwipsInInch = 1440;
extern uint32_t FlagMode;
extern uint32_t RtfWriteMode;
extern CIF::Point16 TemplateOffset;

Handle Rtf_CED_CreateParagraph(int16_t FirstIndent, int16_t LeftIndent, int16_t RightIndent,
        int16_t IntervalBefore, RtfSectorInfo *SectorInfo, int AlignParagraph, int shad,
        int LenthStringInTwips, int LengthFragmInTwips);
void Rtf_CED_CreateChar(CIF::Rect* slayout, letterEx* Letter, CRtfChar* pRtfChar);
void RtfCalcRectSizeInTwips(CIF::Rect *s1, float Twips);
int16_t get_font_name(int16_t FontNumber);
Bool CheckLines(CIF::Rect* Rect, Bool FlagVer, RtfSectorInfo *SectorInfo);
Bool PageTree(FILE *InFileName, CRtfPage* RtfPage, const char* OutFileName);
int16_t GetRealSizeKegl(const char * str, int16_t width, int16_t FontPointSize, int16_t FontNumber);
int16_t GetRealSize(char* str, int16_t len, int16_t FontSize, int16_t FontNumber,
        int16_t* strHeight);
void Put(const char *Data);
void PutChar(uchar sym);
void PutCom(const char *Command, int32_t value, int16_t space);

#endif // __CreateRtf_h__
