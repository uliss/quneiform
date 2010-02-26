/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#include <cstdlib>
#include <cstring>
#include "cedparagraph.h"
#include "cedline.h"
#include "ced_struct.h"

CEDParagraph::CEDParagraph() {
    type = 0;
    alignment = 0;
    layout.x = layout.w = layout.y = layout.h = 0;
    indent.left = indent.bottom = indent.right = indent.top = 0;
    userNumber = 0;
    border = 0;
    interval.cx = interval.cy = 0;
    descriptor = 0;
    leftBrdrType = 0;
    rightBrdrType = 0;
    topBrdrType = 0;
    bottomBrdrType = 0;
    leftBrdrWidth = 0;
    rightBrdrWidth = 0;
    topBrdrWidth = 0;
    bottomBrdrWidth = 0;
    brdrBtw = 0;
    extData = 0;
    extDataLen = 0;
    lines = 0;
    curLine = 0;
    internalNumber = 0;
    parentNumber = 0;
    prev = next = 0;
    numOfLines = 0;
}

CEDParagraph::~CEDParagraph() {
}

CEDLine * CEDParagraph::InsertLine() {
    CEDLine * line = new CEDLine;
    numOfLines++;
    line->parentNumber = internalNumber;

    if (curLine) {
        line->next = curLine->next;

        if (line->next)
            (line->next)->prev = line;

        curLine->next = line;
        line->prev = curLine;
        line->internalNumber = curLine->internalNumber + 1;

        for (CEDLine * line1 = line->next; line1; line1 = line1->next)
            line1->internalNumber++;
    }

    else {
        //      if(internalNumber!=0)
        //      {
        CEDParagraph *ww = prev;

        while (ww && !ww->lines)
            ww = ww->prev;

        if (ww) {
            CEDLine *qq = ww->lines;

            while (qq->next && qq->next->parentNumber == ww->internalNumber)
                qq = qq->next;

            qq->next = line;
            line->prev = qq;
            line->internalNumber = qq->internalNumber + 1;
        }

        ww = next;

        while (ww && !ww->lines)
            ww = ww->next;

        if (ww) {
            CEDLine *qq = ww->lines;
            qq->prev = line;
            line->next = qq;

            while (qq) {
                qq->internalNumber++;
                qq = qq->next;
            }
        }

        //      }
        lines = line;
    }

    curLine = line;
    return line;
}

void CEDParagraph::SetCurLine(CEDLine* _line) {
    curLine = _line;
}

CEDLine * CEDParagraph::SetCurLine(int _number) {
    int i = 0;

    if (lines)
        i = lines->internalNumber;

    CEDLine* line;

    for (line = lines; line && line->internalNumber - i != _number; line = line->next)
        ;

    curLine = line;
    return line;
}

CEDLine * CEDParagraph::GetCurLine() {
    return curLine;
}

int CEDParagraph::GetNumOfCurLine() {
    return curLine->internalNumber;
}

CEDLine * CEDParagraph::NextLine(Bool32 _goThroughPara) {
    if (_goThroughPara)
        return curLine->next;

    if (curLine->next && curLine->next->parentNumber == curLine->parentNumber)
        return curLine->next;

    else
        return 0;
}

CEDLine * CEDParagraph::PrevLine(Bool32 _goThroughPara) {
    if (_goThroughPara)
        return curLine->prev;

    if (curLine->prev && curLine->prev->parentNumber == curLine->parentNumber)
        return curLine->prev;

    else
        return 0;
}

CEDParagraph* CEDParagraph::GetFirstObject() {
    if (next && next->type != COLUMN_BEGIN && next->type != LAST_IN_COLUMN && next->type != TAB_END
            && next->type != TAB_CELL_BEGIN && next->type != FRAME_END && next->type
            != TAB_ROW_BEGIN)
        return next;

    else
        return 0;
}

CEDParagraph* CEDParagraph::GetNextObject() {
    CEDParagraph* ret = 0;

    if (type == TAB_BEGIN)
        ret = (((edTabDescr*) descriptor)->last)->next;

    if (type == FRAME_BEGIN)
        ret = (((edFrameDescr*) descriptor)->last)->next;

    //if this is a start of table/frame
    if (ret)

        //if it is last in column or frame
        if (ret->type == LAST_IN_COLUMN || ret->type == COLUMN_BEGIN || ret->type == FRAME_END)
            return 0;

        //if it is not last
        else
            return ret;

    //if there is next frame, which is simple one/table/frame --- return it
    if (next && ((next->type & FICTIVE) == 0 || next->type == TAB_BEGIN || next->type
            == FRAME_BEGIN))
        return next;

    else
        return 0;
}

CEDParagraph* CEDParagraph::GetRow(int row) {
    EDTABDESCR* td = (EDTABDESCR*) descriptor;

    if (row >= td->numOfRows)
        return 0;

    int i = 0;
    CEDParagraph * ret = td->next;

    while (i < row) {
        ret = ((EDROWDESCR*) ret->descriptor)->last;
        i++;
    }

    return ret;
}

CEDParagraph* CEDParagraph::GetCell(int cell) {
    if (cell >= ((EDROWDESCR*) descriptor)->numOfCells)
        return 0;

    int i = 0;
    CEDParagraph* ret = ((EDROWDESCR*) descriptor)->next;

    while (i < cell) {
        ret = ((EDTABDESCR*) ret->descriptor)->next;
        i++;
    }

    return ret;
}

struct quant
{
        int x, count;//count - number of cells with given x coordinate
};
int comp(const void *arg1, const void *arg2) {
    return ((quant*) arg1)->x - ((quant*) arg2)->x;
}
void CEDParagraph::CreateTableOfCells() {
    int numOfC = 0;
    int cx, cy;
    EDTABDESCR* tabDescriptor = (EDTABDESCR*) descriptor;
    //count horizontal lines
    cy = tabDescriptor->size.cy = tabDescriptor->numOfRows;
    tabDescriptor->linesY = new int[cy + 1];
    tabDescriptor->linesY[0] = 0;
    int r;

    for (r = 0; r < cy; r++) {
        CEDParagraph * row = GetRow(r);
        numOfC += ((EDROWDESCR*) (row->descriptor))->numOfCells + 1;
        tabDescriptor->linesY[r + 1] = ((EDROWDESCR*) row->descriptor)->rowHeight;
    }

    //take all x-coordinates of all cells
    //use complicated algorithm of counting cells with zero width (sometimes it happens)
    quant * x = new quant[numOfC];
    int i = 0;

    for (r = 0; r < cy; r++) {
        CEDParagraph * row = GetRow(r);
        int lastX = 0;
        //left boundary
        x[i].x = lastX;
        x[i].count = 1;
        i++;

        for (int c = 0; c < ((EDROWDESCR*) row->descriptor)->numOfCells; c++) {
            CEDParagraph * cell = row->GetCell(c);

            if (((EDCELLDESCR*) cell->descriptor)->cellX != lastX) {
                x[i].x = ((EDCELLDESCR*) cell->descriptor)->cellX;
                lastX = x[i].x;
                x[i].count = 1;
                i++;
            }

            else {
                x[i - 1].count++;
                numOfC--;//in order to know exactly, what is the written number of coordinates
            }
        }
    }

    qsort((void *) x, (size_t) numOfC, sizeof(quant), comp);
    //delete dublicates
    int i1 = x[0].x;
    int n = 0;

    for (i = 1; i < numOfC; i++) {
        if (x[i].x != i1) {
            i1 = x[i].x;
            x[++n] = x[i];
        }

        else if (x[i].count > x[n].count)
            x[n].count = x[i].count;//count maximal number of overlapping ones
    }

    cx = n;

    //add number of dublicating cells
    for (i = 0; i < n + 1; i++)
        cx += x[i].count - 1;

    tabDescriptor->size.cx = cx;
    //memorize
    int * linesX = tabDescriptor->linesX = new int[cx + 1];
    int j = 0;

    for (i = 0; i < n + 1; i++)
        for (int t = 0; t < x[i].count; t++) {
            tabDescriptor->linesX[j] = x[i].x;
            j++;
        }

    //  ((EDTABDESCR*)descriptor)->linesX[0]=0;
    //  memcpy(((EDTABDESCR*)descriptor)->linesX+1,x,cx*sizeof(int));
    int * table = tabDescriptor->table = new int[cx * cy];
    memset(table, -1, cx * cy * sizeof(int));
    int num = 0;

    for (r = 0; r < cy; r++) {
        CEDParagraph * row = GetRow(r);
        i = 0;

        for (int c = 0; c < ((EDROWDESCR*) row->descriptor)->numOfCells; c++) {
            CEDParagraph * cell = row->GetCell(c);

            if (((EDCELLDESCR*) cell->descriptor)->merging == 0
                    || ((EDCELLDESCR*) cell->descriptor)->merging == ED_CELL_MRGF) {
                while (((EDCELLDESCR*) cell->descriptor)->cellX >= linesX[i + 1]) {
                    table[r * cx + i] = num;
                    i++;

                    if (i == cx)
                        break;

                    //break if next vertical line has the same coordinates as current one
                    //and there are some more cells
                    if (((EDCELLDESCR*) cell->descriptor)->cellX == linesX[i] && linesX[i]
                            == linesX[i + 1] && c < ((EDROWDESCR*) row->descriptor)->numOfCells - 1)
                        break;
                }

                num++;
            }

            else {
                while (((EDCELLDESCR*) cell->descriptor)->cellX >= linesX[i + 1]) {
                    table[r * cx + i] = table[(r - 1) * cx + i];
                    i++;

                    if (i == cx)
                        break;

                    //break if next vertical line has the same coordinates as current one
                    if (((EDCELLDESCR*) cell->descriptor)->cellX == linesX[i] && linesX[i]
                            == linesX[i + 1] && c < ((EDROWDESCR*) row->descriptor)->numOfCells - 1)
                        break;
                }
            }
        }
    }

    delete[] x;
    return;
}

CEDParagraph* CEDParagraph::GetLogicalCell(int number) {
    int cx = ((EDTABDESCR*) descriptor)->size.cx;
    int cy = ((EDTABDESCR*) descriptor)->size.cy;
    int* table = ((EDTABDESCR*) descriptor)->table;
    int i;

    for (i = 0; i < cx * cy; i++)
        if (table[i] == number)
            break;

    int r = i / cx;
    CEDParagraph * row = GetRow(r);
    int n;

    if ((n = table[r * cx]) == number)
        return row->GetCell(0);

    int cell = 1;

    for (int c = 0; c < cx; c++) {
        if (table[r * cx + c] == number)
            break;

        if (table[r * cx + c] != n) {
            n = table[r * cx + c];
            cell++;
        }
    }

    return row->GetCell(cell);
}

int CEDParagraph::GetCountLogicalCell() {
    int i = 0;
    int cx = ((EDTABDESCR*) descriptor)->size.cx;
    int cy = ((EDTABDESCR*) descriptor)->size.cy;
    int* table = ((EDTABDESCR*) descriptor)->table;

    for (int q = 0; q < cx * cy; q++)
        if (i < table[q])
            i = table[q];

    return i + 1;
}
CEDLine* CEDParagraph::GetLine(int _num) {
    int i = 0;

    if (lines)
        i = lines->internalNumber;

    CEDLine* line;

    for (line = lines; line && line->internalNumber - i != _num; line = line->next)
        ;

    return line;
}

int CEDParagraph::GetCountLine() {
    return numOfLines;
}

