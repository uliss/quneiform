/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include "cedsection.h"
#include "cedparagraph.h"

namespace CIF {
CEDSection::CEDSection() {
    numberOfColumns = 0;
    borders.top = borders.bottom = borders.right = borders.left = 0;
    sectionBreak = 0;
    width = 0;
    height = 0;
    orientation = 0;
    headerY = 0;
    footerY = 0;
    numSnakeCols = 0;
    lineBetCol = 0;
    colInfo = 0;
    extData = 0;
    extDataLen = 0;
    paragraphs = 0;
    curPara = 0;
    internalNumber = 0;
    prev = next = 0;
    columnsBeg = columnsEnd = columnsCur = 0;
}

CEDSection::~CEDSection() {
}

CEDParagraph * CEDSection::CreateColumn() {
    if (!columnsBeg) {
        //start column
        CEDParagraph * para = InsertParagraph();
        columnsBeg = para;
        para->type = COLUMN_BEGIN;
        EDCOLDESCR* colinf = (EDCOLDESCR *) malloc(sizeof(EDCOLDESCR));
        para->descriptor = colinf;
        //finish all columns
        CEDParagraph * para1 = InsertParagraph();
        columnsEnd = para1;
        para1->type = LAST_IN_COLUMN;
        ((EDCOLDESCR*) (para->descriptor))->next = para1;
        SetCurParagraph(columnsBeg);
        columnsCur = columnsBeg;
    }
    else {
        //start column
        SetCurParagraph(columnsEnd);
        CEDParagraph * para = InsertParagraph(FALSE);
        para->type = COLUMN_BEGIN;
        EDCOLDESCR* colinf = (EDCOLDESCR *) malloc(sizeof(EDCOLDESCR));
        para->descriptor = colinf;
        ((EDCOLDESCR*) (columnsCur->descriptor))->next = para;
        ((EDCOLDESCR*) (para->descriptor))->next = columnsEnd;
        columnsCur = para;
    }
    numberOfColumns++;
    return curPara;
}

CEDParagraph * CEDSection::GetColumn(int _num) {
    if (_num > numberOfColumns)
        return 0;
    CEDParagraph *para = columnsBeg;
    for (int i = 0; i < _num; i++)
        para = (CEDParagraph*) ((EDCOLDESCR*) para->descriptor)->next;
    return para;
}

CEDParagraph * CEDSection::CreateFrame(CEDParagraph* hObject, edBox rect, char position,
        int borderSpace, int dxfrtextx, int dxfrtexty) {
    if (hObject->type != COLUMN_BEGIN) {
#ifdef _DEBUG
        MessageBox(0,"Attempt of frame creation outside of table","CED error",MB_OK);
#endif
        return 0;
    }
    EDCOLDESCR *colde = (EDCOLDESCR*) (hObject->descriptor);
#ifdef _DEBUG
    if (colde==0)
    {
        MessageBox(0,"Attempt of frame creation in ordinary paragraph\n(not in column)","CED error",MB_OK);
        return 0;
    }
#endif
    //if this column is not not last one - go to column's last paragraph
    if (colde->next)
        SetCurParagraph((CEDParagraph *) (colde->next)->prev);
    //otherwise do the same
    else {
        if (hObject->type == COLUMN_BEGIN)
            SetCurParagraph(columnsEnd->prev);
        else
            return 0;
    }

    //start frame
    CEDParagraph * para = InsertParagraph();
    para->type = FRAME_BEGIN;
    edFrameDescr* framinf = (EDFRAMEDESCR *) malloc(sizeof(EDFRAMEDESCR));
    framinf->rec.x = rect.x;
    framinf->rec.y = rect.y;
    framinf->rec.w = rect.w;
    framinf->rec.h = rect.h;
    framinf->position = position;
    framinf->borderSpace = borderSpace;
    framinf->dxfrtextx = dxfrtextx;
    framinf->dxfrtexty = dxfrtexty;
    framinf->flag = 0;
    para->descriptor = framinf;
    //finish frame
    CEDParagraph * para1 = InsertParagraph();
    para1->type = FRAME_END;
    ((EDFRAMEDESCR *) (para->descriptor))->last = para1;
    SetCurParagraph(para);
    return curPara;
}

CEDParagraph * CEDSection::CreateTable(CEDParagraph * hObject) {
    if (hObject->type == TAB_BEGIN || hObject->type == TAB_CELL_BEGIN) {
#ifdef _DEBUG
        MessageBox(0,"Attempt of table creation in a table","CED error",MB_OK);
#endif
        return 0;
    }
    EDCOLDESCR *colde = (EDCOLDESCR*) (hObject->descriptor);
    if (colde == 0) {
#ifdef _DEBUG
        MessageBox(0,"Attempt of table creation in ordinary paragraph\n(not in column or frame)","CED error",MB_OK);
#endif
        return 0;
    }
    if (colde->next)
        SetCurParagraph((CEDParagraph *) (colde->next));
    else {
        return 0;
    }
    CEDParagraph *para = InsertParagraph(FALSE);
    CEDParagraph *para1 = InsertParagraph();
    if (!para || !para1)
        return 0;
    para->type = TAB_BEGIN;
    para1->type = TAB_END;
    EDTABDESCR * td = (EDTABDESCR*) malloc(sizeof(EDTABDESCR));
    para->descriptor = (void*) td;
    td->next = para1;
    td->last = para1;
    td->cur = para;
    td->numOfRows = 0;
    td->table = 0;
    SetCurParagraph(para);
    return curPara;
}
CEDParagraph * CEDSection::CreateTableRow(CEDParagraph * hTable, int left, int rowHeight,
        int leftBrdrType, int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth,
        int topBrdrType, int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth, int gaph,
        int position, Bool32 header) {
    if (hTable->type != TAB_BEGIN) {
#ifdef _DEBUG
        MessageBox(0,"Attempt to create table row outside of table","CED error",MB_OK);
#endif
        return 0;
    }
#ifdef _DEBUG
    if(!hTable->descriptor)
    {
        MessageBox(0,"Attempt to create table row in ordinary paragraph\n(not in table)","CED error",MB_OK);
        return 0;
    }
#endif
    //insert paragraph-header
    SetCurParagraph(((EDTABDESCR *) hTable->descriptor)->last);
    CEDParagraph * para = InsertParagraph(FALSE);
    if (!para)
        return 0;
    //number of rows in table+1
    ((EDTABDESCR *) (hTable->descriptor))->numOfRows++;
    //old current: next is the one just inserted
    CEDParagraph * para1 = ((EDTABDESCR *) hTable->descriptor)->cur;
    if (para1->type == TAB_BEGIN)
        ((EDTABDESCR *) para1->descriptor)->next = para;
    else
        ((EDROWDESCR *) para1->descriptor)->last = para;
    //next paragraph is the one just insterted
    ((EDTABDESCR *) hTable->descriptor)->cur = para;
    //create description
    edRowDescr * td = (EDROWDESCR*) malloc(sizeof(EDROWDESCR));
    //fill it in
    //next line is last paragraph in table
    td->last = para->next;
    //first cell
    td->next = para;
    //current cell --- to the beginning of line
    td->cur = para;
    td->numOfCells = 0;
    //put received data
    td->left = left;
    td->rowHeight = rowHeight;
    td->leftBrdrType = leftBrdrType;
    td->leftBrdrWidth = leftBrdrWidth;
    td->rightBrdrType = rightBrdrType;
    td->rightBrdrWidth = rightBrdrWidth;
    td->topBrdrType = topBrdrType;
    td->topBrdrWidth = topBrdrWidth;
    td->bottomBrdrType = bottomBrdrType;
    td->bottomBrdrWidth = bottomBrdrWidth;
    td->gaph = gaph;
    td->position = position;
    td->header = header;

    //put type and description of paragraph
    para->descriptor = (void*) td;
    para->type = TAB_ROW_BEGIN;

    return para;
}

CEDParagraph * CEDSection::CreateCell(CEDParagraph* hRow, int cellX, int merging,
        int vertTextAlign, int leftBrdrType, int leftBrdrWidth, int rightBrdrType,
        int rightBrdrWidth, int topBrdrType, int topBrdrWidth, int bottomBrdrType,
        int bottomBrdrWidth, EDBOX layout, int shading, int color) {
    if (hRow->type != TAB_ROW_BEGIN) {
#ifdef _DEBUG
        MessageBox(0,"Attempt to create cell of table outside of table's row","CED error",MB_OK);
#endif
        return 0;
    }
    CEDParagraph * para1 = SetCurParagraph(((EDTABDESCR *) hRow->descriptor)->last);
    CEDParagraph * para = InsertParagraph(FALSE);
    EDCELLDESCR * td = (EDCELLDESCR*) malloc(sizeof(EDCELLDESCR));
    //next cell is end of table/new line
    td->next = para1;
    para->descriptor = (void*) td;
    para->type = TAB_CELL_BEGIN;
    //number of cells++
    ((EDROWDESCR *) (hRow->descriptor))->numOfCells++;
    //previous cell:next=new
    para1 = ((EDTABDESCR *) hRow->descriptor)->cur;
    ((EDROWDESCR *) para1->descriptor)->next = para;
    //current cell in row is new one
    ((EDROWDESCR *) hRow->descriptor)->cur = para;
    td->cellX = cellX;
    td->merging = merging;
    td->vertTextAlign = vertTextAlign;
    td->leftBrdrType = leftBrdrType;
    td->leftBrdrWidth = leftBrdrWidth;
    td->rightBrdrType = rightBrdrType;
    td->rightBrdrWidth = rightBrdrWidth;
    td->topBrdrType = topBrdrType;
    td->topBrdrWidth = topBrdrWidth;
    td->bottomBrdrType = bottomBrdrType;
    td->bottomBrdrWidth = bottomBrdrWidth;
    memcpy(&(td->layout), &layout, sizeof(EDBOX));
    td->shading = shading;
    td->color = color;
    td->flag = 0;
    return para;
}

CEDParagraph * CEDSection::CreateParagraph(CEDParagraph * hObject, int align,
        const CIF::Rect& indent, int UserNum, int FlagBorder, EDSIZE interval, edBox layout,
        int color, int shading, int spaceBetweenLines, char spcBtwLnsMult, char keep) {
    if (hObject->type != TAB_CELL_BEGIN && hObject->type != FRAME_BEGIN && hObject->type
            != COLUMN_BEGIN) {
#ifdef _DEBUG
        MessageBox(0,"Attempt to create paragraph in table's row or in table or in ordinary paragraph\n(not in column or frame or table's cell","CED error",MB_OK);
#endif
        return 0;
    }
    EDCOLDESCR *colde = (EDCOLDESCR*) (hObject->descriptor);
#ifdef _DEBUG
    if (!colde)
    {
        MessageBox(0,"Attempt to create paragraph in ordinary paragraph\n(not in column or in frame or in table's cell)","CED error",MB_OK);
        return 0;
    }
#endif
    if (colde->next)
        SetCurParagraph((CEDParagraph *) (colde->next));
    else {
        return 0;
    }
    CEDParagraph *para = InsertParagraph(FALSE);
    para->alignment = align;
    para->indent = indent;
    para->userNumber = UserNum;
    para->border = FlagBorder;
    para->color = color;
    para->shading = shading;
    para->spaceBetweenLines = spaceBetweenLines;
    para->spcBtwLnsMult = spcBtwLnsMult;
    para->keep = keep;
    memcpy(&(para->interval), &interval, sizeof(interval));
    memcpy(&(para->layout), &layout, sizeof(layout));
    return para;
}

CEDParagraph * CEDSection::InsertParagraph(Bool32 AfterCurrent) {
    if (!AfterCurrent)
        curPara = curPara->prev;
    CEDParagraph * para = new CEDParagraph;
    para->parentNumber = internalNumber;
    if (curPara) {
        para->next = curPara->next;
        if (para->next)
            (para->next)->prev = para;
        curPara->next = para;
        para->prev = curPara;
        para->internalNumber = curPara->internalNumber + 1;
        for (CEDParagraph * para1 = para->next; para1; para1 = para1->next)
            para1->internalNumber++;
    }
    else {
        paragraphs = para;
        CEDSection *ww = prev;
        while (ww && !ww->paragraphs)
            ww = ww->prev;
        if (ww) {
            CEDParagraph *qq = ww->paragraphs;
            while (qq->next)
                qq = qq->next;
            qq->next = para;
            para->prev = qq;
            para->internalNumber = qq->internalNumber + 1;
        }
        ww = next;
        while (ww && !ww->paragraphs)
            ww = ww->next;
        if (ww) {
            CEDParagraph *qq = ww->paragraphs;
            qq->prev = para;
            para->next = qq;
            while (qq) {
                qq->internalNumber++;
                qq = qq->next;
            }
        }
    }
    curPara = para;
    return para;
}

CEDParagraph * CEDSection::SetCurParagraph(CEDParagraph* _para) {
    curPara = _para;
    return _para;
}

CEDParagraph * CEDSection::SetCurParagraph(int _number) {
    int i = 0;
    if (paragraphs)
        i = paragraphs->internalNumber;
    CEDParagraph* para;
    for (para = paragraphs; para && para->internalNumber - i != _number; para = para->next)
        ;
    curPara = para;
    return para;
}

CEDParagraph * CEDSection::GetCurParagraph() {
    return curPara;
}

int CEDSection::GetNumOfCurParagraph() {
    return curPara->internalNumber;
}

CEDParagraph * CEDSection::NextParagraph(Bool32 _goThroughSect) {
    if (_goThroughSect)
        return curPara->next;
    if (curPara->next && curPara->next->parentNumber == curPara->parentNumber)
        return curPara->next;
    else
        return 0;
}

CEDParagraph * CEDSection::PrevParagraph(Bool32 _goThroughSect) {
    if (_goThroughSect)
        return curPara->prev;
    if (curPara->prev && curPara->prev->parentNumber == curPara->parentNumber)
        return curPara->prev;
    else
        return 0;
}
}
