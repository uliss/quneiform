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
#include <cstdio>
#include "ced_struct.h"
#include "cedline.h"
#include "cedpage.h"
#include "cedsection.h"
#include "cedparagraph.h"

using namespace CIF;

FILE *logStream;

CED_FUNC(CIF::CEDSection*) CED_CreateSection(Handle hEdPage, const CIF::Rect& border,
        int colInterval, int numOfCols, EDCOL* colInfo, char sectionBreak, int width, int height,
        char orientation, int headerY, int footerY) {
    if (logStream) {
        fprintf(logStream, "CreateSection params: %x,(%i,%i,%i,%i),%i,%i,%x,%hd,%i,%i,%hd,%i,%i\n",
                hEdPage, border.left(), border.top(), border.right(), border.bottom(), colInterval,
                numOfCols, colInfo, sectionBreak, width, height, orientation, headerY, footerY);
        fflush(logStream);
    }

    CEDSection * sect = new CEDSection;
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

    ((CEDPage*) hEdPage)->addSection(sect);

    if (colInfo)
        memcpy(sect->colInfo, colInfo, sizeof(EDCOL) * numOfCols);
    else
        memset(sect->colInfo, -1, sizeof(EDCOL) * numOfCols);

    if (logStream) {
        fprintf(logStream, "CreateSection returned %x\n", sect);
        fflush(logStream);
    }

    return sect;
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

CEDParagraph * CED_CreateParagraph(Handle hEdSection, Handle hObject, CIF::align_t align,
        const CIF::Rect& indent, int UserNum, int FlagBorder, EDSIZE interval, EDBOX layout,
        const CIF::Color& color, const CIF::Color& shading, int spaceBetweenLines,
        char spcBtwLnsMult, char keep) {
    if (logStream) {
        fprintf(
                logStream,
                "CreateParagraph params: %x,%x,%i,(%i,%i,%i,%i),%i,%i,(%i,%i),(%i,%i,%i,%i),%i,%i,%i,%hd,%hd\n",
                hEdSection, hObject, align, indent.left(), indent.top(), indent.right(),
                indent.bottom(), UserNum, FlagBorder, interval.cx, interval.cy, layout.x, layout.y,
                layout.w, layout.h, color.toT<int> (), shading, spaceBetweenLines, spcBtwLnsMult,
                keep);
        fflush(logStream);
    }

    CEDParagraph * ret = (((CEDSection*) hEdSection)->CreateParagraph((CEDParagraph*) hObject,
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

CED_FUNC(Bool32) CED_IsTable(Handle hObject) {
    return ((CEDParagraph*) hObject)->type == TAB_BEGIN ? TRUE : FALSE;
}

CED_FUNC(Bool32) CED_IsFrame(Handle hObject) {
    return ((CEDParagraph*) hObject)->type == FRAME_BEGIN ? TRUE : FALSE;
}

CED_FUNC(Bool32) CED_IsParagraph(Handle hObject) {
    return ((CEDParagraph*) hObject)->type & FICTIVE ? FALSE : TRUE;
}

