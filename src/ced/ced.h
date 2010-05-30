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

#ifndef __CED_H
#define __CED_H

#include "globus.h"
#include "lang_def.h"
#include "common/size.h"
#include "common/rect.h"
#include "common/letter.h"

#ifdef __CED__
#define CED_FUNC  FUN_EXPO
#else
#define CED_FUNC  FUN_IMPO
#endif

#define FUN_IMPO_VOID  FUN_IMPO__  __FUN_IMPO
#define FUN_EXPO_VOID  FUN_EXPO__  __FUN_EXPO

#pragma pack (push,8)

#include "edfile.h"

typedef struct edBox
{
        int x, w, y, h;
} EDBOX;

typedef struct edSize
{
        int32_t cx, cy;
} EDSIZE;

typedef struct edCol
{
        int32_t width, space;
} EDCOL;

#ifndef _WINGDI_

#define DEFAULT_PITCH           0
#define FIXED_PITCH             1
#define VARIABLE_PITCH          2
#define MONO_FONT               8

/* Font Families */
enum ced_font_t
{
    /* Don't care or don't know. */
    FF_DONTCARE = (0 << 4),
    /* Variable stroke width, serifed. */
    /* Times Roman, Century Schoolbook, etc. */
    FF_ROMAN = (1 << 4),
    /* Variable stroke width, sans-serifed. */
    /* Helvetica, Swiss, etc. */
    FF_SWISS = (2 << 4),
    /* Constant stroke width, serifed or sans-serifed. */
    /* Pica, Elite, Courier, etc. */
    FF_MODERN = (3 << 4),
    /* Cursive, etc. */
    FF_SCRIPT = (4 << 4),
    /* Old English, etc. */
    FF_DECORATIVE = (5 << 4)
};
#endif

//Для CreateFrame
#define HPOS_PAGE       (0<<4)
#define HPOS_MRG        (1<<4)
#define HPOS_COL        (2<<4)

#define VPOS_PAGE       0
#define VPOS_MRG        1
#define VPOS_PARA       2

//База для шрифта/признака картинки
const int ED_PICT_BASE = 0x10000;

//Для разрыва раздела
#define ED_SECT_NEW_PAGE       1
#define ED_SECT_CONTINUE       0
/* orientation selections */
#define ED_ORIENT_PORTRAIT   0
#define ED_ORIENT_LANDSCAPE  1

//Положение фрейма
#define ED_POSXR    (0x7fffffff-1)

//Стили символов
enum ed_char_style_t
{
    ED_ULINED = 1,
    ED_HIDDEN = 2,
    ED_BOLD = 4,
    ED_ITALIC = 16,
    ED_PROTECT = 32,
    ED_ULINE = 64,
    ED_STRIKE = 128,
    ED_SUPSCR = 256,
    ED_SUBSCR = 512,
    ED_FNOTE = 1024

};
//для абзацев
#define ED_PARA_KEEP    1
#define ED_PARA_KEEP_NEXT   3
//- Positions the paragraph to the right within the reference frame.
#define ED_POSXC    (0x7fffffff-2)
//- Centers the frame horizontally within the reference frame.
//Слияние ячеек
#define ED_CELL_MRGF    1
// - ячейка первая в объединяемой группе
#define ED_CELL_MRG     2
//- ячейка очередная в объедияемой группе, т.е.
#define ED_DROPCAP      1
// - Фрейм является буквицей
#define ED_TDIR_UP      2
#define ED_TDIR_DOWN    4
// - направление текста в ячейке таблицы или фрейме

//Типы границ ячейки, абзаца и др.
enum ed_border_t
{
    ED_BRDR_NONE = 0,
    ED_BRDR_SINGLE = 1,
    ED_BRDR_SHADOWED = 2,
    ED_BRDR_DOTTED = 3,
    ED_BRDR_DASHED = 4,
    ED_BRDR_DOUBLE = 5
};
CED_FUNC(Bool32) CED_Init(uint16_t wHeightCode, Handle hStorage);

//Импорт
enum CED_IMPORT_ENTRIES
{
    CED_FNCED_BitmapRef = 1,
    CED_FNCED_TextRef,
    CED_FNCED_FontKegl,
    CED_FNCED_Kegl,
    CED_FNCED_Shift,
    CED_FNCED_RetrieveLevel,
    CED_FNCED_Underline,
    CED_FNCED_DensPrint,
    CED_FNCED_Tabul,
    CED_FNCED_TablTabul,
    CED_FNCED_SheetDiskDescr,
    CED_FNCED_FragmDiskDescr,
    CED_FNCED_FragmDisk,
    CED_FNCED_StepBack,
    CED_FNCED_LineBeg,
    CED_FNCED_Position,
    CED_FNCED_EdTagLanguage,
    CED_FNCED_TableConformSizes,
    CED_FNCED_GroupWords,
    CED_FNCED_GroupSymbols,
    CED_FNCED_Border,
    CED_FNCED_TableHeader,
    CED_FNCED_ListOfFragments,
    CED_FNCED_Extention,
    CED_FNCED_Aksant,
    CED_FNCED_Letter,
    CED_FNCED_ExtentionNew
};

typedef void (*FNRDProc)(uchar* ptr, uint32_t lth);

//Для CreatePicture
enum ed_align_t
{
    ED_ALIGN_BOT = 0, // align picture bottom to base line
    ED_ALIGN_MIDDLE = 1, // center picture vertically to baseline
    ED_ALIGN_TOP = 2
// align picture top to base line
};

#define DEC_FUN(a,b,c) typedef a (*FN##b)c

DEC_FUN(void, CED_BitmapRef, (const struct bit_map_ref* pt));
DEC_FUN(void, CED_TextRef, (const struct text_ref* pt));
DEC_FUN(void, CED_FontKegl, (const struct font_kegl *pt));
DEC_FUN(void, CED_Kegl, (const struct kegl* pt));
DEC_FUN(void, CED_Shift, (const struct shift* pt));
DEC_FUN(void, CED_RetrieveLevel, (const struct retrieve_level* pt));
DEC_FUN(void, CED_Underline, (const struct underline* pt));
DEC_FUN(void, CED_DensPrint, (const struct dens_print* pt));
DEC_FUN(void, CED_Tabul, (const struct tabul* pt));
DEC_FUN(void, CED_TablTabul, (const struct tabl_tabul* pt));
DEC_FUN(void, CED_SheetDiskDescr, (const struct sheet_disk_descr* pt));
DEC_FUN(void, CED_FragmDiskDescr, (const struct fragm_disk_descr* pt));
DEC_FUN(void, CED_FragmDisk, (const struct fragm_disk* pt));
DEC_FUN(void, CED_StepBack, (const struct step_back* pt));
DEC_FUN(void, CED_LineBeg, (const struct line_beg* pt));
DEC_FUN(void, CED_Position, (const struct position* pt));
DEC_FUN(void, CED_EdTagLanguage, (const struct EdTagLanguage* pt));
DEC_FUN(void, CED_TableConformSizes, (const struct table_conform_sizes* pt));
DEC_FUN(void, CED_GroupWords, (const struct group_words* pt));
DEC_FUN(void, CED_GroupSymbols, (const struct group_symbols* pt));
DEC_FUN(void, CED_Border, (const struct border* pt));
DEC_FUN(void, CED_TableHeader, (const struct table_header* pt));
DEC_FUN(void, CED_ListOfFragments, (const struct list_of_fragments* pt));
DEC_FUN(void, CED_Extention, (const struct edExtention* pt, const void* ptExt));
DEC_FUN(void, CED_ExtentionNew, (const struct edExtentionNew* pt, const void* ptExt));
DEC_FUN(void, CED_Aksant, (const struct aksant* pt));
DEC_FUN(void, CED_Letter, (const struct letter* pt, const uint32_t alternatives));

#undef DEC_FUN

//Экспорт

typedef enum
{
    CED_FNCED_ReadED = 1,
    CED_FNCED_SetRawDataProc,
    CED_FNCED_FormattedLoad,
    CED_FNCED_FormattedWrite,
    CED_FNCED_DeleteTree,
    CED_FNCED_CreatePage,
    CED_FNCED_CreateSection,
    CED_FNCED_CreateFont,
    CED_FNCED_CreateColumn,
    CED_FNCED_CreateParagraph,
    CED_FNCED_CreateLine,
    CED_FNCED_CreateChar,
    CED_FNCED_CreateFrame,
    CED_FNCED_CreateTable,
    CED_FNCED_CreateCell,
    CED_FNCED_DeletePage,
    CED_FNCED_ReadFormattedEd,
    CED_FNCED_WriteFormattedEd,
    CED_FNCED_GetPageImageName,
    CED_FNCED_GetPageImageSize,
    CED_FNCED_GetPageDpi,
    CED_FNCED_GetPageTurn,
    CED_FNCED_GetPageSize,
    CED_FNCED_GetPageUnrecogChar,
    CED_FNCED_GetCountSection,
    CED_FNCED_GetNumberOfParagraphs,
    CED_FNCED_GetFont,
    CED_FNCED_GetNumOfFonts,
    CED_FNCED_GetParagraph,
    CED_FNCED_GetSection,
    CED_FNCED_GetSectionBorder,
    CED_FNCED_GetCountColumn,
    CED_FNCED_GetColumn,
    CED_FNCED_GetSnakeColumnWidth,
    CED_FNCED_GetSnakeColumnSpacing,
    CED_FNCED_GetFrameRect,
    CED_FNCED_GetFirstObject,
    CED_FNCED_GetNextObject,
    CED_FNCED_IsTable,
    CED_FNCED_IsFrame,
    CED_FNCED_IsParagraph,
    CED_FNCED_IsFictive,
    CED_FNCED_GetCountCell,
    CED_FNCED_GetCell,
    CED_FNCED_GetLinesX,
    CED_FNCED_GetRowsHeights,
    CED_FNCED_GetTableOfCells,
    CED_FNCED_GetSize,
    CED_FNCED_GetIndent,
    CED_FNCED_GetAlignment,
    CED_FNCED_GetLayout,
    CED_FNCED_GetUserNumber,
    CED_FNCED_GetInterval,
    CED_FNCED_GetCountLine,
    CED_FNCED_GetLine,
    CED_FNCED_GetCountChar,
    CED_FNCED_GetChar,
    CED_FNCED_GetAlternatives,
    CED_FNCED_GetCharFontLang,
    CED_FNCED_IsEdFile,
    CED_FNCED_GetCharLayout,
    CED_FNCED_CreatePicture,
    CED_FNCED_CreateTableRow,
    CED_FNCED_GetPageNumber,
    CED_FNCED_GetPageBorders,
    CED_FNCED_GetPageResize,
    CED_FNCED_GetPicture,
    CED_FNCED_GetNumOfPics,
    CED_FNCED_GetNumSnakeCols,
    CED_FNCED_GetCountRow,
    CED_FNCED_GetTableRow,
    CED_FNCED_GetTableRowParams,
    CED_FNCED_GetCellParams,
    CED_FNCED_GetFramePosition,
    CED_FNCED_GetFrameBorderSpace,
    CED_FNCED_GetFrameDxfrtextx,
    CED_FNCED_GetFrameDxfrtexty,
    CED_FNCED_GetLineHardBreak,
    CED_FNCED_GetCharFontNum,
    CED_FNCED_GetLogicalCell,
    CED_FNCED_GetCountLogicalCell,
    CED_FNCED_GetLineDefChrFontHeight,
    CED_FNCED_WriteFormattedRtf,
    CED_FNCED_SetLogFileName,
    CED_FNCED_MergeFormattedRtf,
    CED_FNCED_SetLineParams,
    CED_FNCED_SetSectLineBetCol,
    CED_FNCED_GetSectLineBetCol,
    CED_FNCED_SetParaBorders,
    CED_FNCED_GetParaBorders,
    CED_FNCED_SetFrameFlag,
    CED_FNCED_GetFrameFlag,
    CED_FNCED_GetParaParams,
    CED_FNCED_SetCellFlag,
    CED_FNCED_GetCellFlag,
    CED_FNCED_SetPageRecogLang,
    CED_FNCED_GetPageRecogLang
} CED_EXPORT_ENTRIES;
#ifdef _DEBUG
#define     CED_FNCED_ShowTree 0xffff
#endif

namespace CIF
{
class CEDPage;
class CEDLine;
class CEDParagraph;
}
#define DEC_FUN(a,b,c) typedef a (*FN##b)c; CED_FUNC(a) b c;
DEC_FUN(uint32_t, CED_IsEdFile, (char * file, Bool32 readFromFile, uint32_t bufLen))
DEC_FUN(uint32_t, CED_ReadED, (char * file, Bool32 readFromFile, uint32_t bufLen))
DEC_FUN(void, CED_SetRawDataProc, (FNRDProc proc))
DEC_FUN(Bool32, CED_CreatePicture, (Handle hEdPage, int pictNumber, const CIF::Size& pictSize, EDSIZE pictGoal, int pictAlign, int type, void * data, int len))
DEC_FUN(Handle, CED_CreateSection, (Handle hEdPage, const CIF::Rect& border, int colInterval, int numOfCols, EDCOL* colInfo, char sectionBreak, int width, int height, char orientation, int headerY, int footerY))
DEC_FUN(Bool32, CED_SetSectLineBetCol, ( Handle hEdSection, Bool32 lineBetCol))
DEC_FUN(Handle, CED_CreateColumn, ( Handle hEdSection))
DEC_FUN(CIF::CEDParagraph *, CED_CreateParagraph, (Handle hEdSection, Handle hObject, int align, const CIF::Rect& indent, int UserNum, int FlagBorder, EDSIZE interval, EDBOX layout, int color, int shading, int spaceBetweenLines, char spcBtwLnsMult, char keep))
DEC_FUN(Bool32, CED_SetParaBorders, (Handle hEdParagraph, int leftBrdrType, int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth, int topBrdrType, int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth, int brdrBtw))
DEC_FUN(CIF::CEDLine*, CED_CreateLine, (Handle hEdParagraph, bool hardBreak, int defChrFontHeight))
DEC_FUN(Handle, CED_CreateChar, (Handle hEdLine, const CIF::Rect& layout, CIF::Letter* alternatives, int fontHeight, int fontNum, int fontAttribs, language_t fontLang, int foregroundColor, int backgroundColor))
DEC_FUN(Handle, CED_CreateFrame, (Handle hEdSection, Handle hEdColumn, EDBOX rect, char position, int borderSpace, int dxfrtextx, int dxfrtexty))
DEC_FUN(Bool32, CED_SetFrameFlag, (Handle hEdFrame, int flag))
DEC_FUN(Handle, CED_CreateTable, (Handle hEdSection, Handle hObject))
DEC_FUN(Handle, CED_CreateTableRow, (Handle hEdSection, Handle hEdTable, int left, int rowHeight, int leftBrdrType, int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth, int topBrdrType, int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth, int gaph, int position, Bool32 header))
DEC_FUN(Handle, CED_CreateCell, (Handle hEdSection, Handle hEdRow, int cellX, int merging, int vertTextAlign, int leftBrdrType, int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth, int topBrdrType, int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth, EDBOX layout, int shading, int color))
DEC_FUN(Bool32, CED_SetCellFlag, (Handle hEdCell, int flag))
DEC_FUN(Handle, CED_ReadFormattedEd, (char * lpEdFile, Bool32 readFromFile, uint32_t bufLen))
DEC_FUN(Bool32, CED_WriteFormattedEd, (const char * lpEdFileName, Handle hEdPage))
DEC_FUN(uint32_t, CED_GetNumberOfParagraphs, (Handle hEdPage))
DEC_FUN(uint32_t, CED_GetNumOfFonts, (Handle hEdPage))
DEC_FUN(Handle, CED_GetParagraph, (Handle hEdPage, int _num))
DEC_FUN(Bool32, CED_GetFont, (Handle hEdPage, int number, uchar* fontNumber, uchar* fontPitchAndFamily, uchar* fontCharset, char** fontName))
DEC_FUN(Bool32, CED_GetSectLineBetCol, (Handle hEdSection))
DEC_FUN(CIF::Rect, CED_GetSectionBorder, (Handle hEdSection))
DEC_FUN(uint32_t, CED_GetCountColumn, (Handle hEdSection))
DEC_FUN(uint32_t, CED_GetNumSnakeCols, (Handle hEdSection))
DEC_FUN(Handle, CED_GetColumn, (Handle hEdSection, int number))
DEC_FUN(int32_t, CED_GetSnakeColumnWidth, (Handle hEdSection, int number))
DEC_FUN(int32_t, CED_GetSnakeColumnSpacing, (Handle hEdSection, int number))
DEC_FUN(Bool32, CED_GetSectionParams, (Handle hEdSection, CIF::Rect& border, int* colInterval, char* sectionBreak, int* width, int* height, char* orientation, int* headerY, int* footerY))
DEC_FUN(EDBOX, CED_GetFrameRect, (Handle hEdFrame))
DEC_FUN(int32_t, CED_GetFramePosition, (Handle hEdFrame))
DEC_FUN(int32_t, CED_GetFrameBorderSpace, (Handle hEdFrame))
DEC_FUN(int32_t, CED_GetFrameDxfrtextx, (Handle hEdFrame))
DEC_FUN(int32_t, CED_GetFrameDxfrtexty, (Handle hEdFrame))
DEC_FUN(int32_t, CED_GetFrameFlag, (Handle hEdFrame))
DEC_FUN(Handle, CED_GetFirstObject, (Handle hObject))
DEC_FUN(Handle, CED_GetNextObject, (Handle hObject))
DEC_FUN(Bool32, CED_IsTable, (Handle hObject))
DEC_FUN(Bool32, CED_IsFrame, (Handle hObject))
DEC_FUN(Bool32, CED_IsParagraph, (Handle hObject))
DEC_FUN(Bool32, CED_IsFictive, (Handle hEdParagraph))
DEC_FUN(uint32_t, CED_GetCountRow, (Handle hEdTable))
DEC_FUN(Handle, CED_GetTableRow, (Handle hEdTable, int num))
DEC_FUN(Bool32, CED_GetTableRowParams, (Handle hEdRow, int* left, int* rowHeight, int* leftBrdrType, int* leftBrdrWidth, int* rightBrdrType, int* rightBrdrWidth, int* topBrdrType, int* topBrdrWidth, int* bottomBrdrType, int* bottomBrdrWidth, int* gaph, int* position, Bool32* header))
DEC_FUN(uint32_t, CED_GetCountCell, (Handle hEdRow))
DEC_FUN(Handle, CED_GetCell, (Handle hEdRow, int number))
DEC_FUN(Bool32, CED_GetCellParams, (Handle hEdCell, int* cellX, int* merging, int* vertTextAlign, int* leftBrdrType, int* leftBrdrWidth, int* rightBrdrType, int* rightBrdrWidth, int* topBrdrType, int* topBrdrWidth, int* bottomBrdrType, int* bottomBrdrWidth, EDBOX* layout, int* shading, int* color))
DEC_FUN(int32_t, CED_GetCellFlag, (Handle hEdCell))
DEC_FUN(int32_t*, CED_GetLinesX, (Handle hEdTable))
DEC_FUN(int32_t *, CED_GetRowsHeights, (Handle hEdTable))
DEC_FUN(uint32_t*, CED_GetTableOfCells, (Handle hEdTable))
DEC_FUN(EDSIZE, CED_GetSize, (Handle hEdTable))
DEC_FUN(Handle, CED_GetLogicalCell, (Handle hEdTable, int number))
DEC_FUN(int, CED_GetCountLogicalCell, (Handle hEdTable))
DEC_FUN(uint32_t, CED_GetAlignment, (Handle hEdParagraph))
DEC_FUN(EDBOX, CED_GetLayout, (Handle hEdParagraph))
DEC_FUN(uint32_t, CED_GetUserNumber, (Handle hEdParagraph))
DEC_FUN(EDSIZE, CED_GetInterval, (Handle hEdParagraph))
DEC_FUN(Bool32, CED_GetParaParams, (Handle hEdParagraph, int* color, int* shading, int* spaceBetweenLines, char* spcBtwLnsMult, char* keep))
DEC_FUN(Bool32, CED_GetParaBorders, (Handle hEdParagraph, int* leftBrdrType, int* leftBrdrWidth, int* rightBrdrType, int* rightBrdrWidth, int* topBrdrType, int* topBrdrWidth, int* bottomBrdrType, int* bottomBrdrWidth, int* brdrBtw))
DEC_FUN(uint32_t, CED_GetCountLine, (Handle hEdParagraph))
DEC_FUN(Handle, CED_GetLine, (Handle hEdParagraph, int number))
DEC_FUN(Bool32, CED_WriteFormattedRtf, (const char * fileName, Handle hEdPage))
DEC_FUN(Bool32, CED_MergeFormattedRtf, (const char * fileName, Handle hEdPage))

#ifdef _DEBUG
CED_FUNC(void) CED_ShowTree(char * name, CIF::CEDPage * EdPage);
#endif

#undef DEC_FUN

#pragma pack (pop)

#endif
