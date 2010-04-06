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
#include "cedline.h"

using namespace CIF;

FILE *logStream;

//create font
CED_FUNC(Bool32) CED_CreateFont(Handle hEdPage, uchar fontNumber, uchar fontPitchAndFamily,
        uchar fontCharset, char* fontName) {
    if (logStream) {
        fprintf(logStream, "CreateFont params: %x,%hd,%hd,%hd,%s\n", hEdPage, fontNumber,
                fontPitchAndFamily, fontCharset, fontName);
        fflush(logStream);
    }

    Bool ret = ((CEDPage*) hEdPage)->CreateFont(fontNumber, fontPitchAndFamily, fontCharset,
            fontName);

    if (logStream) {
        fprintf(logStream, "CreateFont returned %i\n", ret);
        fflush(logStream);
    }

    return ret;
}

CED_FUNC(Bool32) CED_CreatePicture(Handle hEdPage, int pictNumber, const CIF::Size& pictSize,
        EDSIZE pictGoal, int pictAlign, int type, void * data, int len) {
    if (logStream) {
        fprintf(logStream, "CreatePicture params: %x,%i,(%i,%i),(%i,%i),%i,%i,%x,%i\n", hEdPage,
                pictNumber, pictSize.width(), pictSize.height(), pictGoal.cx, pictGoal.cy,
                pictAlign, type, data, len);
        fflush(logStream);
    }

    Bool ret = ((CEDPage*) hEdPage)->CreatePicture(pictNumber, pictSize, pictGoal, pictAlign, type,
            data, len);

    if (logStream) {
        fprintf(logStream, "CreatePicture returned %i\n", ret);
        fflush(logStream);
    }

    return ret;
}
//create section

CED_FUNC(Handle) CED_CreateSection(Handle hEdPage, const CIF::Rect& border, int colInterval,
        int numOfCols, EDCOL* colInfo, char sectionBreak, int width, int height, char orientation,
        int headerY, int footerY) {
    if (logStream) {
        fprintf(logStream, "CreateSection params: %x,(%i,%i,%i,%i),%i,%i,%x,%hd,%i,%i,%hd,%i,%i\n",
                hEdPage, border.left(), border.top(), border.right(), border.bottom(), colInterval,
                numOfCols, colInfo, sectionBreak, width, height, orientation, headerY, footerY);
        fflush(logStream);
    }

    CEDSection *sect = ((CEDPage*) hEdPage)->InsertSection();
    memcpy(&(sect->borders), &border, sizeof(border));
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

    if (logStream) {
        fprintf(logStream, "CreateSection returned %x\n", sect);
        fflush(logStream);
    }

    return (Handle) sect;
}

CED_FUNC(Bool32) CED_SetSectLineBetCol(Handle hEdSection, Bool32 lineBetCol) {
    if (logStream) {
        fprintf(logStream, "SetSectLineBetCol params: %x,%i\n", hEdSection, lineBetCol);
        fflush(logStream);
    }

    ((CEDSection*) hEdSection)->lineBetCol = lineBetCol;

    if (logStream) {
        fprintf(logStream, "SetSectLineBetCol returned %i\n", TRUE);
        fflush(logStream);
    }

    return TRUE;
}

//create column
CED_FUNC(Handle) CED_CreateColumn(Handle hEdSection) {
    if (logStream) {
        fprintf(logStream, "CreateColumn params: %x\n", hEdSection);
        fflush(logStream);
    }

    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateColumn());

    if (logStream) {
        fprintf(logStream, "CreateColumn returned %x\n", ret);
        fflush(logStream);
    }

    return ret;
}

//create frame
CED_FUNC(Handle) CED_CreateFrame(Handle hEdSection, Handle hEdColumn, edBox rect, char position,
        int borderSpace, int dxfrtextx, int dxfrtexty) {
    if (logStream) {
        fprintf(logStream, "CreateFrame params: %x,%x,(%i,%i,%i,%i),%hd,%i,%i,%i\n", hEdSection,
                hEdColumn, rect.x, rect.y, rect.w, rect.h, position, borderSpace, dxfrtextx,
                dxfrtexty);
        fflush(logStream);
    }

    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateFrame((CEDParagraph*) hEdColumn, rect,
            position, borderSpace, dxfrtextx, dxfrtexty));

    if (logStream) {
        fprintf(logStream, "CreateFrame returned %x\n", ret);
        fflush(logStream);
    }

    return ret;
}

CED_FUNC(Bool32) CED_SetFrameFlag(Handle hEdFrame, int flag) {
    if (logStream) {
        fprintf(logStream, "SetFrameFlag params: %x,%x\n", hEdFrame, flag);
        fflush(logStream);
    }

    ((EDFRAMEDESCR *) (((CEDParagraph*) hEdFrame)->descriptor))->flag = flag;

    if (logStream) {
        fprintf(logStream, "SetFrameFlag returned %i\n", TRUE);
        fflush(logStream);
    }

    return TRUE;
}

//create table
CED_FUNC(Handle) CED_CreateTable(Handle hEdSection, Handle hObject) {
    if (logStream) {
        fprintf(logStream, "CreateTable params: %x,%x\n", hEdSection, hObject);
        fflush(logStream);
    }

    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateTable((CEDParagraph*) hObject));

    if (logStream) {
        fprintf(logStream, "CreateTable returned %x\n", ret);
        fflush(logStream);
    }

    return ret;
}

CED_FUNC(Handle) CED_CreateTableRow(Handle hEdSection, Handle hEdTable, int left, int rowHeight,
        int leftBrdrType, int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth,
        int topBrdrType, int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth, int gaph,
        int position, Bool32 header) {
    if (logStream) {
        fprintf(logStream, "CreateTableRow params: %x,%x,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i\n",
                hEdSection, hEdTable, left, rowHeight, leftBrdrType, leftBrdrWidth, rightBrdrType,
                rightBrdrWidth, topBrdrType, topBrdrWidth, bottomBrdrType, bottomBrdrWidth, gaph,
                position, header);
        fflush(logStream);
    }

    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateTableRow((CEDParagraph*) hEdTable,
            left, rowHeight, leftBrdrType, leftBrdrWidth, rightBrdrType, rightBrdrWidth,
            topBrdrType, topBrdrWidth, bottomBrdrType, bottomBrdrWidth, gaph, position, header));

    if (logStream) {
        fprintf(logStream, "CreateTableRow returned %x\n", ret);
        fflush(logStream);
    }

    return ret;
}

// create table's cell
CED_FUNC(Handle) CED_CreateCell(Handle hEdSection, Handle hEdRow, int cellX, int merging,
        int vertTextAlign, int leftBrdrType, int leftBrdrWidth, int rightBrdrType,
        int rightBrdrWidth, int topBrdrType, int topBrdrWidth, int bottomBrdrType,
        int bottomBrdrWidth, EDBOX layout, int shading, int color) {
    if (logStream) {
        fprintf(logStream,
                "CreateCell params: %x,%x,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,(%i,%i,%i,%i),%i,%i\n",
                hEdSection, hEdRow, cellX, merging, vertTextAlign, leftBrdrType, leftBrdrWidth,
                rightBrdrType, rightBrdrWidth, topBrdrType, topBrdrWidth, bottomBrdrType,
                bottomBrdrWidth, layout.x, layout.y, layout.w, layout.h, shading, color);
        fflush(logStream);
    }

    Handle ret = (Handle) (((CEDSection*) hEdSection) -> CreateCell((CEDParagraph*) hEdRow, cellX,
            merging, vertTextAlign, leftBrdrType, leftBrdrWidth, rightBrdrType, rightBrdrWidth,
            topBrdrType, topBrdrWidth, bottomBrdrType, bottomBrdrWidth, layout, shading, color));

    if (logStream) {
        fprintf(logStream, "CreateCell returned %x\n", ret);
        fflush(logStream);
    }

    return ret;
}

CED_FUNC(Bool32) CED_SetCellFlag(Handle hEdCell, int flag) {
    if (logStream) {
        fprintf(logStream, "SetCellFlag params: %x,%x\n", hEdCell, flag);
        fflush(logStream);
    }

    ((EDCELLDESCR *) (((CEDParagraph*) hEdCell)->descriptor))->flag = flag;

    if (logStream) {
        fprintf(logStream, "SetCellFlag returned %i\n", TRUE);
        fflush(logStream);
    }

    return TRUE;
}
//create paragraph

CED_FUNC(Handle) CED_CreateParagraph(Handle hEdSection, Handle hObject, int align,
        const CIF::Rect& indent, int UserNum, int FlagBorder, EDSIZE interval, EDBOX layout,
        int color, int shading, int spaceBetweenLines, char spcBtwLnsMult, char keep) {
    if (logStream) {
        fprintf(
                logStream,
                "CreateParagraph params: %x,%x,%i,(%i,%i,%i,%i),%i,%i,(%i,%i),(%i,%i,%i,%i),%i,%i,%i,%hd,%hd\n",
                hEdSection, hObject, align, indent.left(), indent.top(), indent.right(),
                indent.bottom(), UserNum, FlagBorder, interval.cx, interval.cy, layout.x, layout.y,
                layout.w, layout.h, color, shading, spaceBetweenLines, spcBtwLnsMult, keep);
        fflush(logStream);
    }

    Handle ret = (Handle) (((CEDSection*) hEdSection)->CreateParagraph((CEDParagraph*) hObject,
            align, indent, UserNum, FlagBorder, interval, layout, color, shading,
            spaceBetweenLines, spcBtwLnsMult, keep));

    if (logStream) {
        fprintf(logStream, "CreateParagraph returned %x\n", ret);
        fflush(logStream);
    }

    return ret;
}

//set boundary of paragraph
CED_FUNC(Bool32) CED_SetParaBorders(Handle hEdParagraph, int leftBrdrType, int leftBrdrWidth,
        int rightBrdrType, int rightBrdrWidth, int topBrdrType, int topBrdrWidth,
        int bottomBrdrType, int bottomBrdrWidth, int brdrBtw) {
    if (logStream) {
        fprintf(logStream, "SetParaBorders params: %x,%i,%i,%i,%i,%i,%i,%i,%i,%i\n", hEdParagraph,
                leftBrdrType, leftBrdrWidth, rightBrdrType, rightBrdrWidth, topBrdrType,
                topBrdrWidth, bottomBrdrType, bottomBrdrWidth, brdrBtw);
        fflush(logStream);
    }

    ((CEDParagraph*) hEdParagraph)->leftBrdrType = leftBrdrType;
    ((CEDParagraph*) hEdParagraph)->leftBrdrWidth = leftBrdrWidth;
    ((CEDParagraph*) hEdParagraph)->rightBrdrType = rightBrdrType;
    ((CEDParagraph*) hEdParagraph)->rightBrdrWidth = rightBrdrWidth;
    ((CEDParagraph*) hEdParagraph)->topBrdrType = topBrdrType;
    ((CEDParagraph*) hEdParagraph)->topBrdrWidth = topBrdrWidth;
    ((CEDParagraph*) hEdParagraph)->bottomBrdrType = bottomBrdrType;
    ((CEDParagraph*) hEdParagraph)->bottomBrdrWidth = bottomBrdrWidth;
    ((CEDParagraph*) hEdParagraph)->brdrBtw = brdrBtw;

    if (logStream) {
        fprintf(logStream, "SetParaBorders returned %i\n", TRUE);
        fflush(logStream);
    }

    return TRUE;
}

//create line
CED_FUNC(Handle) CED_CreateLine(Handle hEdParagraph, bool hardBreak, int defChrFontHeight) {
    if (logStream) {
        fprintf(logStream, "CreateLine params: %x,%i,%i\n", hEdParagraph, hardBreak,
                defChrFontHeight);
        fflush(logStream);
    }

    CEDLine * lin = ((CEDParagraph*) hEdParagraph)->InsertLine();
    lin->setHardBreak(hardBreak);
    lin->setDefaultFontHeight(defChrFontHeight);

    if (logStream) {
        fprintf(logStream, "CreateLine returned %x\n", lin);
        fflush(logStream);
    }

    return (Handle) lin;
}

//create symbol
CED_FUNC(Handle) CED_CreateChar(Handle hEdLine, const CIF::Rect& layout, letterEx* alternatives,
        int fontHeight, int fontNum, int fontAttribs, language_t fontLang, int foregroundColor,
        int backgroundColor) {
    CEDChar *chr = ((CEDLine*) hEdLine)->insertChar();
    chr->setFontHeight(fontHeight);
    chr->fontNum = fontNum;
    chr->fontAttribs = fontAttribs;
    chr->foregroundColor = foregroundColor;
    chr->backgroundColor = backgroundColor;
    chr->setFontLanguage(fontLang);
    chr->setBoundingRect(layout);

    int i = 0;

    if (alternatives != 0) {
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

    return (Handle) chr;
}

//get description of page

CED_FUNC(Handle) CED_ReadFormattedEd(char * lpEdFile, Bool32 readFromFile, uint32_t bufLen) {
    return (Handle) CED_FormattedLoad(lpEdFile, readFromFile, bufLen);
}

CED_FUNC(Bool32) CED_WriteFormattedEd(const char * lpEdFileName, Handle hEdPage) {
    if (logStream) {
        fprintf(logStream, "WriteFormattedEd params: %s,%x\n", lpEdFileName, hEdPage);
        fflush(logStream);
    }

    Bool32 ret = CED_FormattedWrite(lpEdFileName, (CEDPage*) hEdPage);

    if (logStream) {
        fprintf(logStream, "WriteFormattedEd returned %i\n", ret);
        fflush(logStream);
    }

    return ret;
}

CED_FUNC(uint32_t) CED_GetNumberOfParagraphs(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->GetNumberOfParagraphs();
}

CED_FUNC(Handle) CED_GetParagraph(Handle hEdPage, int _num) {
    return ((CEDPage*) hEdPage)->GetParagraph(_num);
}

//get font
CED_FUNC(Bool32) CED_GetFont(Handle hEdPage, int number, uchar* fontNumber,
        uchar* fontPitchAndFamily, uchar* fontCharset, char** fontName) {
    return ((CEDPage*) hEdPage)->GetFont(number, fontNumber, fontPitchAndFamily, fontCharset,
            fontName);
}
CED_FUNC(uint32_t) CED_GetNumOfFonts(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->fontsUsed;
}

CED_FUNC(Bool32) CED_GetPicture(Handle hEdPage, int number, int* pictNumber, CIF::Size& pictSize,
        EDSIZE* pictGoal, int* pictAlign, int* type, void ** data, int* len) {
    CEDPage * pg = (CEDPage*) hEdPage;

    if (number >= pg->picsCreated)
        return FALSE;

    pictEntry* tmp = &(pg->picsTable[number]);

    if (pictNumber)
        *pictNumber = tmp->pictNumber;

    pictSize = tmp->pictSize;

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
CED_FUNC(uint32_t) CED_GetNumOfPics(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->picsUsed;
}
////////////////////////////////////////////////////////////////////////////////////////
CED_FUNC(uint32_t) CED_GetCountSection(Handle hEdPage) {
    return ((CEDPage*) hEdPage)->GetNumberOfSections();
}

CED_FUNC(CIF::Rect) CED_GetSectionBorder(Handle hEdSection) {
    return ((CEDSection*) hEdSection)->borders;
}

CED_FUNC(Bool32) CED_GetSectionParams(Handle hEdSection, CIF::Rect& border, int* colInterval,
        char* sectionBreak, int* width, int* height, char* orientation, int* headerY, int* footerY) {
    if (!hEdSection)
        return FALSE;

    CEDSection *sect = (CEDSection*) hEdSection;

    border = sect->borders;

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

CED_FUNC(Bool32) CED_GetSectLineBetCol(Handle hEdSection) {
    return ((CEDSection*) hEdSection)->lineBetCol;
}

///////////////////////////////////////////////////////////////////////////////////////
CED_FUNC(uint32_t) CED_GetCountColumn(Handle hEdSection) {
    return ((CEDSection*) hEdSection)->numberOfColumns;
}

CED_FUNC(uint32_t) CED_GetNumSnakeCols(Handle hEdSection) {
    return ((CEDSection*) hEdSection)->numSnakeCols;
}

CED_FUNC(Handle) CED_GetColumn(Handle hEdSection, int number) {
    return (Handle) (((CEDSection*) hEdSection)->GetColumn(number));
}

CED_FUNC(int32_t) CED_GetSnakeColumnWidth(Handle hEdSection, int number) {
    return ((CEDSection*) hEdSection)->colInfo[number].width;
}

CED_FUNC(int32_t) CED_GetSnakeColumnSpacing(Handle hEdSection, int number) {
    return ((CEDSection*) hEdSection)->colInfo[number].space;
}
///////////////////////////////////////////////////////////

/*uint32_t CED_GetCountFrame(Handle hEdSection)
 {
 return ((CEDSection*)hEdSection)->numberOfFrames;
 }

 Handle CED_GetFrame(Handle hEdSection,int number)
 {
 return (Handle)(((CEDSection*)hEdSection)->GetFrame(number));
 }
 */
CED_FUNC(edBox) CED_GetFrameRect(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->rec;
}

CED_FUNC(int32_t) CED_GetFramePosition(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->position;
}

CED_FUNC(int32_t) CED_GetFrameBorderSpace(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->borderSpace;
}

CED_FUNC(int32_t) CED_GetFrameDxfrtextx(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->dxfrtextx;
}

CED_FUNC(int32_t) CED_GetFrameDxfrtexty(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->dxfrtexty;
}

CED_FUNC(int32_t) CED_GetFrameFlag(Handle hEdFrame) {
    return ((EDFRAMEDESCR*) (((CEDParagraph*) hEdFrame)->descriptor))->flag;
}

///////////////////////////////////////////////////////////////
CED_FUNC(Handle) CED_GetFirstObject(Handle hObject) {
    return (Handle) ((CEDParagraph*) hObject)->GetFirstObject();
}

CED_FUNC(Handle) CED_GetNextObject(Handle hObject) {
    return (Handle) ((CEDParagraph*) hObject)->GetNextObject();
}

CED_FUNC(Bool32) CED_IsTable(Handle hObject) {
    return ((CEDParagraph*) hObject)->type == TAB_BEGIN ? TRUE : FALSE;
}

CED_FUNC(Bool32) CED_IsFrame(Handle hObject) {
    return ((CEDParagraph*) hObject)->type == FRAME_BEGIN ? TRUE : FALSE;
}

CED_FUNC(Bool32) CED_IsParagraph(Handle hObject) {
    return ((CEDParagraph*) hObject)->type & FICTIVE ? FALSE : TRUE;
}

CED_FUNC(Bool32) CED_IsFictive(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->type & FICTIVE ? TRUE : FALSE;
}

CED_FUNC(uint32_t) CED_GetCountRow(Handle hEdTable) {
    return ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->numOfRows;
}

CED_FUNC(Handle) CED_GetTableRow(Handle hEdTable, int num) {
    return (Handle) ((CEDParagraph*) hEdTable)->GetRow(num);
}
CED_FUNC(Bool32) CED_GetTableRowParams(Handle hEdRow, int* left, int* rowHeight, int* leftBrdrType,
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
CED_FUNC(Handle) CED_GetCell(Handle hEdRow, int number) {
    return (Handle) ((CEDParagraph*) hEdRow)->GetCell(number);
}

CED_FUNC(Bool32) CED_GetCellParams(Handle hEdCell, int* cellX, int* merging, int* vertTextAlign,
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

CED_FUNC(int32_t) CED_GetCellFlag(Handle hEdCell) {
    return ((EDCELLDESCR*) (((CEDParagraph*) hEdCell)->descriptor))->flag;
}

CED_FUNC(uint32_t *) CED_GetTableOfCells(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();

    return (uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table;
}

CED_FUNC(int32_t *)CED_GetLinesX(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();

    return (int32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->linesX;
}

CED_FUNC(int32_t *)CED_GetRowsHeights(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();

    return (int32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->linesY;
}

CED_FUNC(edSize) CED_GetSize(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();

    return (edSize) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->size;
}
CED_FUNC(Handle) CED_GetLogicalCell(Handle hEdTable, int number) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();

    return (Handle) ((CEDParagraph*) hEdTable)->GetLogicalCell(number);
}
CED_FUNC(int) CED_GetCountLogicalCell(Handle hEdTable) {
    if ((uint32_t*) ((EDTABDESCR*) ((CEDParagraph*) hEdTable)->descriptor)->table == 0)
        ((CEDParagraph*) hEdTable)->CreateTableOfCells();

    return ((CEDParagraph*) hEdTable)->GetCountLogicalCell();
}
/////////////////////////////////////////////////////////////////
CED_FUNC(uint32_t) CED_GetCountLine(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->GetCountLine();
}

CED_FUNC(Handle) CED_GetLine(Handle hEdParagraph, int number) {
    return ((CEDParagraph*) hEdParagraph)->GetLine(number);
}

CED_FUNC(uint32_t) CED_GetAlignment(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->alignment;
}

CED_FUNC(edBox) CED_GetLayout(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->layout;
}

CED_FUNC(uint32_t) CED_GetUserNumber(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->userNumber;
}

CED_FUNC(EDSIZE) CED_GetInterval(Handle hEdParagraph) {
    return ((CEDParagraph*) hEdParagraph)->interval;
}

CED_FUNC(Bool32) CED_GetParaParams(Handle hEdParagraph, int* color, int* shading,
        int* spaceBetweenLines, char* spcBtwLnsMult, char* keep) {
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
CED_FUNC(Bool32) CED_GetParaBorders(Handle hEdParagraph, int* leftBrdrType, int* leftBrdrWidth,
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

CED_FUNC(struct) letterEx* CED_GetAlternatives(Handle hEdChar) {
    return ((CEDChar*) hEdChar)->alternatives;
}

CED_FUNC(Bool32) CED_WriteFormattedRtf(const char * fileName, Handle hEdPage) {
    if (logStream) {
        fprintf(logStream, "WriteFormattedRtf params: %s,%x\n", fileName, hEdPage);
        fflush(logStream);
    }

    Bool32 ret = ((CEDPage*) hEdPage)->FormattedWriteRtf(fileName, FALSE);

    if (logStream) {
        fprintf(logStream, "WriteFormattedRtf returned %i\n", ret);
        fflush(logStream);
    }

    return ret;
}

CED_FUNC(Bool32) CED_MergeFormattedRtf(const char * fileName, Handle hEdPage) {
    return ((CEDPage*) hEdPage)->FormattedWriteRtf(fileName, TRUE);
}
