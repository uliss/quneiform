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

//Filename wrapper.cpp
//Created 13.01.99 by Bozhenov Artem,(c) CT inc.
#include <cstring>
#include "ced_struct.h"
#include "cedint.h"
#include "cedpage.h"
#include "cedsection.h"
#include "cedparagraph.h"
#include "cedline.h"

using namespace CIF;

char logName[PATH_MAX];
FILE *logStream;

//create page
Handle CED_CreatePage(char * _imageName, const Size& _sizeOfImage, const Resolution& _dpi,
        int _turn, int _pageNumber, const Size& _sizeInTwips, const Rect& _pageBordersInTwips,
        char _unrecogChar, Bool32 _resizeToFit) {
    if (logName[0] && (!logStream))
        logStream = fopen(logName, "at");
    else if (logStream)
        fprintf(logStream, "ERROR: Page Was Not Deleted Properly\n");

    CEDPage * ret = new CEDPage;
    ret->imageName = strdup(_imageName);
    ret->setImageSize(_sizeOfImage);
    ret->turn = _turn;
    ret->pageNumber = _pageNumber;
    ret->pageSizeInTwips = _sizeInTwips;
    ret->pageBordersInTwips = _pageBordersInTwips;
    ret->unrecogChar = _unrecogChar;
    ret->resizeToFit = _resizeToFit;
    ret->recogLang = LANG_RUSENG;
    if (logStream) {
        fprintf(logStream, "CreatePage returned %x\n", ret);
        fflush(logStream);
    }
    return (Handle) ret;
}

//create foont
Bool32 CED_CreateFont(Handle hEdPage, uchar fontNumber, uchar fontPitchAndFamily,
        uchar fontCharset, char* fontName) {
    Bool ret = ((CEDPage*) hEdPage)->CreateFont(fontNumber, fontPitchAndFamily, fontCharset,
            fontName);
    return ret;
}

Bool32 CED_CreatePicture(Handle hEdPage, int pictNumber, const Size& pictSize,
        const Size& pictGoal, int pictAlign, int type, void * data, int len) {
    Bool ret = ((CEDPage*) hEdPage)->CreatePicture(pictNumber, pictSize, pictGoal, pictAlign, type,
            data, len);
    return ret;
}
//create section

Handle CED_CreateSection(Handle hEdPage, const Rect& border, int colInterval, int numOfCols,
        EDCOL* colInfo, char sectionBreak, int width, int height, char orientation, int headerY,
        int footerY) {
    CEDSection *sect = ((CEDPage*) hEdPage)->InsertSection();
    sect->borders = border;
    sect->colInterval = colInterval;
    sect->sectionBreak = sectionBreak;
    sect->width = width;
    sect->height = height;
    sect->orientation = orientation;
    sect->headerY = headerY;
    sect->footerY = footerY;
    sect->numSnakeCols = numOfCols;
    sect->colInfo = new EDCOL[numOfCols];
    if (colInfo)
        memcpy(sect->colInfo, colInfo, sizeof(EDCOL) * numOfCols);
    else
        memset(sect->colInfo, -1, sizeof(EDCOL) * numOfCols);
    return (Handle) sect;
}

Bool32 CED_SetSectLineBetCol(Handle hEdSection, Bool32 lineBetCol) {
    ((CEDSection*) hEdSection)->lineBetCol = lineBetCol;
    return TRUE;
}

//create column
Handle CED_CreateColumn(Handle hEdSection) {
    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateColumn());
    return ret;
}

//create frame
Handle CED_CreateFrame(Handle hEdSection, Handle hEdColumn, edBox rect, char position,
        int borderSpace, int dxfrtextx, int dxfrtexty) {
    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateFrame((CEDParagraph*) hEdColumn, rect,
            position, borderSpace, dxfrtextx, dxfrtexty));
    return ret;
}

Bool32 CED_SetFrameFlag(Handle hEdFrame, int flag) {
    ((EDFRAMEDESCR *) (((CEDParagraph*) hEdFrame)->descriptor))->flag = flag;
    return TRUE;
}

//create table
Handle CED_CreateTable(Handle hEdSection, Handle hObject) {
    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateTable((CEDParagraph*) hObject));
    return ret;
}

Handle CED_CreateTableRow(Handle hEdSection, Handle hEdTable, int left, int rowHeight,
        int leftBrdrType, int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth,
        int topBrdrType, int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth, int gaph,
        int position, Bool32 header) {
    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateTableRow((CEDParagraph*) hEdTable,
            left, rowHeight, leftBrdrType, leftBrdrWidth, rightBrdrType, rightBrdrWidth,
            topBrdrType, topBrdrWidth, bottomBrdrType, bottomBrdrWidth, gaph, position, header));
    return ret;
}

// create table's cell
Handle CED_CreateCell(Handle hEdSection, Handle hEdRow, int cellX, int merging, int vertTextAlign,
        int leftBrdrType, int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth,
        int topBrdrType, int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth, EDBOX layout,
        int shading, int color) {
    Handle ret = (Handle) (((CEDSection*) hEdSection) -> CreateCell((CEDParagraph*) hEdRow, cellX,
            merging, vertTextAlign, leftBrdrType, leftBrdrWidth, rightBrdrType, rightBrdrWidth,
            topBrdrType, topBrdrWidth, bottomBrdrType, bottomBrdrWidth, layout, shading, color));
    return ret;
}

Bool32 CED_SetCellFlag(Handle hEdCell, int flag) {
    ((EDCELLDESCR *) (((CEDParagraph*) hEdCell)->descriptor))->flag = flag;
    return TRUE;
}
//create paragraph

Handle CED_CreateParagraph(Handle hEdSection, Handle hObject, int align, const CIF::Rect& indent,
        int UserNum, int FlagBorder, const Size& interval, EDBOX layout, int color, int shading,
        int spaceBetweenLines, char spcBtwLnsMult, char keep) {
    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateParagraph((CEDParagraph*) hObject,
            align, indent, UserNum, FlagBorder, interval, layout, color, shading,
            spaceBetweenLines, spcBtwLnsMult, keep));
    return ret;
}

//set boundary of paragraph
Bool32 CED_SetParaBorders(Handle hEdParagraph, int leftBrdrType, int leftBrdrWidth,
        int rightBrdrType, int rightBrdrWidth, int topBrdrType, int topBrdrWidth,
        int bottomBrdrType, int bottomBrdrWidth, int brdrBtw) {
    ((CEDParagraph*) hEdParagraph)->leftBrdrType = leftBrdrType;
    ((CEDParagraph*) hEdParagraph)->leftBrdrWidth = leftBrdrWidth;
    ((CEDParagraph*) hEdParagraph)->rightBrdrType = rightBrdrType;
    ((CEDParagraph*) hEdParagraph)->rightBrdrWidth = rightBrdrWidth;
    ((CEDParagraph*) hEdParagraph)->topBrdrType = topBrdrType;
    ((CEDParagraph*) hEdParagraph)->topBrdrWidth = topBrdrWidth;
    ((CEDParagraph*) hEdParagraph)->bottomBrdrType = bottomBrdrType;
    ((CEDParagraph*) hEdParagraph)->bottomBrdrWidth = bottomBrdrWidth;
    ((CEDParagraph*) hEdParagraph)->brdrBtw = brdrBtw;
    return TRUE;
}

//create line
Handle CED_CreateLine(Handle hEdParagraph, Bool32 hardBreak, int defChrFontHeight) {
    CEDLine * lin = ((CEDParagraph*) hEdParagraph)->InsertLine();
    lin->hardBreak = hardBreak;
    lin->defChrFontHeight = defChrFontHeight;
    return (Handle) lin;
}

Bool32 CED_SetLineParams(Handle hEdLine, Bool32 hardBreak, int defChrFontHeight) {
    CEDLine * lin = ((CEDLine*) hEdLine);
    if (!lin)
        return FALSE;
    lin->hardBreak = hardBreak;
    lin->defChrFontHeight = defChrFontHeight;
    return TRUE;
}

//create symbol
Handle CED_CreateChar(Handle hEdLine, const CIF::Rect& layout, letterEx* alternatives,
        int fontHeight, int fontNum, int fontAttribs, int fontLang, int foregroundColor,
        int backgroundColor) {
    CEDChar *chr = ((CEDLine*) hEdLine)->InsertChar();
    chr->fontHeight = fontHeight;
    chr->fontNum = fontNum;
    chr->fontAttribs = fontAttribs;
    chr->foregroundColor = foregroundColor;
    chr->backgroundColor = backgroundColor;
    chr->fontLang = fontLang;
    chr->layout = layout;

    if (alternatives != 0) {
        int i = 0;
        while (alternatives[i].probability & 1 == 1) {
            if (alternatives[i].alternative < ' ')
                alternatives[i].alternative = ' ';
            i++;
        }
        chr->numOfAltern = i + 1;
        chr->alternatives = new letterEx[i + 1];
        memcpy(chr->alternatives, alternatives, (i + 1) * sizeof(letterEx));
    }
    else {
        chr->numOfAltern = 1;
        chr->alternatives = new letterEx[1];
        chr->alternatives[0].alternative = ' ';
        chr->alternatives[0].probability = 254;
    }
    chr->layout = layout;
    return (Handle) chr;

}

//delete all
void CED_DeletePage(Handle hEdPage) {
    delete (CEDPage*) hEdPage;
}

//get description of page

Handle CED_ReadFormattedEd(char * lpEdFile, Bool32 readFromFile, uint32_t bufLen) {
    return CED_FormattedLoad(lpEdFile, readFromFile, bufLen);
}

Bool32 CED_WriteFormattedEd(const char * lpEdFileName, Handle hEdPage) {
    Bool32 ret = CED_FormattedWrite(lpEdFileName, (CEDPage*) hEdPage);
    return ret;
}

char* CED_GetPageImageName(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->imageName;
}

Size CED_GetPageImageSize(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->imageSize();
}

Resolution CED_GetPageDpi(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->dpi();
}

uint32_t CED_GetPageTurn(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->turn;
}

Size CED_GetPageSize(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->pageSizeInTwips;
}

uint32_t CED_GetPageNumber(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->pageNumber;
}

Rect CED_GetPageBorders(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->pageBordersInTwips;
}
char CED_GetPageUnrecogChar(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->unrecogChar;
}
char CED_GetPageRecogLang(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->recogLang;
}
Bool32 CED_SetPageRecogLang(Handle hEdPage, char _recogLang) {
    ((CEDPage*) hEdPage)->recogLang = _recogLang;
    return TRUE;
}

Bool32 CED_GetPageResize(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->resizeToFit;
}

uint32_t CED_GetNumberOfParagraphs(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->GetNumberOfParagraphs();
}

Handle CED_GetParagraph(Handle hEdPage, int _num) {
    return ((CEDPage*) hEdPage)->GetParagraph(_num);
}

//get font
Bool32 CED_GetFont(Handle hEdPage, int number, uchar* fontNumber, uchar* fontPitchAndFamily,
        uchar* fontCharset, char** fontName) {
    return ((CEDPage*) hEdPage)->GetFont(number, fontNumber, fontPitchAndFamily, fontCharset,
            fontName);
}
uint32_t CED_GetNumOfFonts(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->fontsUsed;
}

Bool32 CED_GetPicture(Handle hEdPage, int number, int* pictNumber, Size * pictSize,
        Size * pictGoal, int* pictAlign, int* type, void ** data, int* len) {
    CEDPage * pg = (CEDPage*) hEdPage;
    if (number >= pg->picsCreated)
        return FALSE;
    pictEntry* tmp = &(pg->picsTable[number]);
    if (pictNumber)
        *pictNumber = tmp->pictNumber;
    if (pictSize)
        *pictSize = tmp->pictSize;
    if (pictGoal)
        *pictGoal = tmp->pictGoal;
    if (pictAlign)
        *pictAlign = tmp->pictAlign;
    if (type)
        *type = tmp->type;
    if (data)
        *data = tmp->data;
    if (len)
        *len = tmp->len;
    return TRUE;
}
uint32_t CED_GetNumOfPics(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->picsUsed;
}

uint32_t CED_GetCountSection(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->GetNumberOfSections();
}

Handle CED_GetSection(Handle hEdPage, uint32_t number) {
    return (Handle) ((CEDPage*) hEdPage)->GetSection(number);
}

Rect CED_GetSectionBorder(Handle hEdSection) {
    return ((CEDSection*) hEdSection)->borders;
}

Bool32 CED_GetSectionParams(Handle hEdSection, Rect * border, int* colInterval, char* sectionBreak,
        int* width, int* height, char* orientation, int* headerY, int* footerY) {
    if (!hEdSection)
        return FALSE;
    CEDSection *sect = (CEDSection*) hEdSection;
    if (border)
        *border = sect->borders;
    if (colInterval)
        (*colInterval) = sect->colInterval;
    if (sectionBreak)
        (*sectionBreak) = sect->sectionBreak;
    if (width)
        (*width) = sect->width;
    if (height)
        (*height) = sect->height;
    if (orientation)
        (*orientation) = sect->orientation;
    if (headerY)
        (*headerY) = sect->headerY;
    if (footerY)
        (*footerY) = sect->footerY;
    return TRUE;
}

Bool32 CED_GetSectLineBetCol(Handle hEdSection) {
    return ((CEDSection*) hEdSection)->lineBetCol;
}

uint32_t CED_GetCountColumn(Handle hEdSection) {
    return ((CEDSection*) hEdSection)->numberOfColumns;
}

uint32_t CED_GetNumSnakeCols(Handle hEdSection) {
    return ((CEDSection*) hEdSection)->numSnakeCols;
}

Handle CED_GetColumn(Handle hEdSection, int number) {
    return (Handle) (((CEDSection*) hEdSection)->GetColumn(number));
}

int32_t CED_GetSnakeColumnWidth(Handle hEdSection, int number) {
    return ((CEDSection*) hEdSection)->colInfo[number].width;
}

int32_t CED_GetSnakeColumnSpacing(Handle hEdSection, int number) {
    return ((CEDSection*) hEdSection)->colInfo[number].space;
}

edBox CED_GetFrameRect(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->rec;
}

int32_t CED_GetFramePosition(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->position;
}

int32_t CED_GetFrameBorderSpace(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->borderSpace;
}

int32_t CED_GetFrameDxfrtextx(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->dxfrtextx;
}

int32_t CED_GetFrameDxfrtexty(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->dxfrtexty;
}

int32_t CED_GetFrameFlag(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->flag;
}

Handle CED_GetFirstObject(Handle hObject) {
    return (Handle) ((CEDParagraph*) hObject)->GetFirstObject();
}

Handle CED_GetNextObject(Handle hObject) {
    return (Handle) ((CEDParagraph*) hObject)->GetNextObject();
}

Bool32 CED_IsTable(Handle hObject) {
    return ((CEDParagraph*) hObject)->type == TAB_BEGIN ? TRUE : FALSE;
}

Bool32 CED_IsFrame(Handle hObject) {
    return ((CEDParagraph*) hObject)->type == FRAME_BEGIN ? TRUE : FALSE;
}

Bool32 CED_IsParagraph(Handle hObject) {
    return ((CEDParagraph*) hObject)->type & FICTIVE ? FALSE : TRUE;
}

Bool32 CED_IsFictive(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->type & FICTIVE ? TRUE : FALSE;
}

uint32_t CED_GetCountRow(Handle hEdTable) {
    return ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->numOfRows;
}

Handle CED_GetTableRow(Handle hEdTable, int num) {
    return (Handle) ((CEDParagraph*) hEdTable)->GetRow(num);
}
Bool32 CED_GetTableRowParams(Handle hEdRow, int* left, int* rowHeight, int* leftBrdrType,
        int* leftBrdrWidth, int* rightBrdrType, int* rightBrdrWidth, int* topBrdrType,
        int* topBrdrWidth, int* bottomBrdrType, int* bottomBrdrWidth, int* gaph, int* position,
        Bool32* header) {
    EDROWDESCR* td = ((EDROWDESCR*) ((CEDParagraph*) hEdRow)->descriptor);
    if (!td)
        return FALSE;
    if (left)
        *left = td->left;
    if (rowHeight)
        *rowHeight = td->rowHeight;
    if (leftBrdrType)
        *leftBrdrType = td->leftBrdrType;
    if (leftBrdrWidth)
        *leftBrdrWidth = td->leftBrdrWidth;
    if (rightBrdrType)
        *rightBrdrType = td->rightBrdrType;
    if (rightBrdrWidth)
        *rightBrdrWidth = td->rightBrdrWidth;
    if (topBrdrType)
        *topBrdrType = td->topBrdrType;
    if (topBrdrWidth)
        *topBrdrWidth = td->topBrdrWidth;
    if (bottomBrdrType)
        *bottomBrdrType = td->bottomBrdrType;
    if (bottomBrdrWidth)
        *bottomBrdrWidth = td->bottomBrdrWidth;
    if (gaph)
        *gaph = td->gaph;
    if (position)
        *position = td->position;
    if (header)
        *header = td->header;
    return TRUE;
}

uint32_t CED_GetCountCell(Handle hEdRow) {
    return ((EDROWDESCR*) ((CEDParagraph*) hEdRow)->descriptor)->numOfCells;
}
Handle CED_GetCell(Handle hEdRow, int number) {
    return (Handle) ((CEDParagraph*) hEdRow)->GetCell(number);
}

Bool32 CED_GetCellParams(Handle hEdCell, int* cellX, int* merging, int* vertTextAlign,
        int* leftBrdrType, int* leftBrdrWidth, int* rightBrdrType, int* rightBrdrWidth,
        int* topBrdrType, int* topBrdrWidth, int* bottomBrdrType, int* bottomBrdrWidth,
        EDBOX* layout, int* shading, int* color) {
    EDCELLDESCR* cd = ((EDCELLDESCR*) ((CEDParagraph*) hEdCell)->descriptor);
    if (!cd)
        return FALSE;

    if (cellX)
        (*cellX) = cd->cellX;
    if (merging)
        (*merging) = cd->merging;
    if (vertTextAlign)
        (*vertTextAlign) = cd->vertTextAlign;
    if (leftBrdrType)
        (*leftBrdrType) = cd->leftBrdrType;
    if (leftBrdrWidth)
        (*leftBrdrWidth) = cd->leftBrdrWidth;
    if (rightBrdrType)
        (*rightBrdrType) = cd->rightBrdrType;
    if (rightBrdrWidth)
        (*rightBrdrWidth) = cd->rightBrdrWidth;
    if (topBrdrType)
        (*topBrdrType) = cd->topBrdrType;
    if (topBrdrWidth)
        (*topBrdrWidth) = cd->topBrdrWidth;
    if (bottomBrdrType)
        (*bottomBrdrType) = cd->bottomBrdrType;
    if (bottomBrdrWidth)
        (*bottomBrdrWidth) = cd->bottomBrdrWidth;
    if (layout)
        memcpy(layout, &(cd->layout), sizeof(EDBOX));
    if (shading)
        (*shading) = cd->shading;
    if (color)
        (*color) = cd->color;
    return TRUE;
}

int32_t CED_GetCellFlag(Handle hEdCell) {
    return ((EDCELLDESCR*) (((CEDParagraph*) hEdCell)->descriptor))->flag;
}

uint32_t * CED_GetTableOfCells(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();
    return (uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table;
}

int32_t * CED_GetLinesX(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();
    return (int32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->linesX;
}

int32_t *CED_GetRowsHeights(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();
    return (int32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->linesY;
}

Size CED_GetSize(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();
    return ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->size;
}
Handle CED_GetLogicalCell(Handle hEdTable, int number) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();
    return (Handle) ((CEDParagraph*) hEdTable)->GetLogicalCell(number);
}
int CED_GetCountLogicalCell(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();
    return ((CEDParagraph*) hEdTable)->GetCountLogicalCell();
}

uint32_t CED_GetCountLine(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->GetCountLine();
}

Handle CED_GetLine(Handle hEdParagraph, int number) {
    return ((CEDParagraph*) hEdParagraph)->GetLine(number);
}

CIF::Rect CED_GetIndent(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->indent;
}

uint32_t CED_GetAlignment(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->alignment;
}

edBox CED_GetLayout(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->layout;
}

uint32_t CED_GetUserNumber(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->userNumber;
}

Point CED_GetInterval(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->interval;
}

Bool32 CED_GetParaParams(Handle hEdParagraph, int* color, int* shading, int* spaceBetweenLines,
        char* spcBtwLnsMult, char* keep) {
    if (color)
        *color = ((CEDParagraph*) hEdParagraph)->color;
    if (shading)
        *shading = ((CEDParagraph*) hEdParagraph)->shading;
    if (spaceBetweenLines)
        *spaceBetweenLines = ((CEDParagraph*) hEdParagraph)->spaceBetweenLines;
    if (spcBtwLnsMult)
        *spcBtwLnsMult = ((CEDParagraph*) hEdParagraph)->spcBtwLnsMult;
    if (keep)
        *keep = ((CEDParagraph*) hEdParagraph)->keep;
    return TRUE;
}
//obtain boundary of paragraph
Bool32 CED_GetParaBorders(Handle hEdParagraph, int* leftBrdrType, int* leftBrdrWidth,
        int* rightBrdrType, int* rightBrdrWidth, int* topBrdrType, int* topBrdrWidth,
        int* bottomBrdrType, int* bottomBrdrWidth, int* brdrBtw) {
    if (leftBrdrType)
        *leftBrdrType = ((CEDParagraph*) hEdParagraph)->leftBrdrType;
    if (leftBrdrWidth)
        *leftBrdrWidth = ((CEDParagraph*) hEdParagraph)->leftBrdrWidth;
    if (rightBrdrType)
        *rightBrdrType = ((CEDParagraph*) hEdParagraph)->rightBrdrType;
    if (rightBrdrWidth)
        *rightBrdrWidth = ((CEDParagraph*) hEdParagraph)->rightBrdrWidth;
    if (topBrdrType)
        *topBrdrType = ((CEDParagraph*) hEdParagraph)->topBrdrType;
    if (topBrdrWidth)
        *topBrdrWidth = ((CEDParagraph*) hEdParagraph)->topBrdrWidth;
    if (bottomBrdrType)
        *bottomBrdrType = ((CEDParagraph*) hEdParagraph)->bottomBrdrType;
    if (bottomBrdrWidth)
        *bottomBrdrWidth = ((CEDParagraph*) hEdParagraph)->bottomBrdrWidth;
    if (brdrBtw)
        *brdrBtw = ((CEDParagraph*) hEdParagraph)->brdrBtw;
    return TRUE;
}

Bool32 CED_GetLineHardBreak(Handle hEdLine) {
    return ((CEDLine*) hEdLine)->hardBreak;
}

int32_t CED_GetLineDefChrFontHeight(Handle hEdLine) {
    return ((CEDLine*) hEdLine)->defChrFontHeight;
}
uint32_t CED_GetCountChar(Handle hEdLine) {
    return ((CEDLine*) hEdLine)->GetCountChar();
}

Handle CED_GetChar(Handle hEdLine, int number) {
    return ((CEDLine*) hEdLine)->GetChar(number);
}

Bool32 CED_IsPicture(Handle hEdChar) {
    int n = ((CEDChar*) hEdChar)->fontNum;
    return (n >= ED_PICT_BASE) && (n != 0xffffffff) && (n != 0xfffffffe);
}

struct letterEx* CED_GetAlternatives(Handle hEdChar) {
    return ((CEDChar*) hEdChar)->alternatives;
}

int32_t CED_GetCharFontHeight(Handle hEdChar) {
    return ((CEDChar*) hEdChar)->fontHeight;
}

int32_t CED_GetCharFontAttribs(Handle hEdChar) {
    return ((CEDChar*) hEdChar)->fontAttribs;
}

int32_t CED_GetCharFontNum(Handle hEdChar) {
    return ((CEDChar*) hEdChar)->fontNum;
}

Rect CED_GetCharLayout(Handle hEdChar) {
    return ((CEDChar*) hEdChar)->layout;
}
int32_t CED_GetCharForegroundColor(Handle hEdChar) {
    return ((CEDChar*) hEdChar)->foregroundColor;
}

int32_t CED_GetCharBackgroundColor(Handle hEdChar) {
    return ((CEDChar*) hEdChar)->backgroundColor;
}
int32_t CED_GetCharFontLang(Handle hEdChar) {
    return ((CEDChar*) hEdChar)->fontLang;
}

Bool32 CED_WriteFormattedRtf(const char * fileName, Handle hEdPage) {
    return ((CEDPage*) hEdPage)->FormattedWriteRtf(fileName);
}

void CED_SetLogFileName(const char * name) {
    strcpy(logName, name);
}
