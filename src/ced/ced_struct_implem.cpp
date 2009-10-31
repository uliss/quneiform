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

//Filename ced_struct_implem.cpp
//Created 7.12.98 by Bozhenov Artem,(c) CT inc.
#include <cstring>
#include <cstdlib>

#include "ced_struct.h"
#include "cedint.h"

/////////////////////////////////////////////////////////////////////
CEDParagraph::CEDParagraph() {
	type = 0;
	alignment = 0;
	layout.x = layout.w = layout.y = layout.h = 0;
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
	/*	curLine=lines;
	 CEDLine * qq;
	 while (curLine)
	 {
	 qq=curLine->next;
	 delete curLine;
	 curLine=qq;
	 }
	 */}

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
	} else {
		//		if(internalNumber!=0)
		//		{
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
		//		}
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
	for (line = lines; line && line->internalNumber - i != _number; line
			= line->next)
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
	if (next && next->type != COLUMN_BEGIN && next->type != LAST_IN_COLUMN
			&& next->type != TAB_END && next->type != TAB_CELL_BEGIN
			&& next->type != FRAME_END && next->type != TAB_ROW_BEGIN)
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
		if (ret->type == LAST_IN_COLUMN || ret->type == COLUMN_BEGIN
				|| ret->type == FRAME_END)
			return 0;
		//if it is not last
		else
			return ret;
	//if there is next frame, which is simple one/table/frame --- return it
	if (next && ((next->type & FICTIVE) == 0 || next->type == TAB_BEGIN
			|| next->type == FRAME_BEGIN))
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

struct quant {
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
		tabDescriptor->linesY[r + 1]
				= ((EDROWDESCR*) row->descriptor)->rowHeight;
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
			} else {
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
		} else if (x[i].count > x[n].count)
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
	//	((EDTABDESCR*)descriptor)->linesX[0]=0;
	//	memcpy(((EDTABDESCR*)descriptor)->linesX+1,x,cx*sizeof(int));

	int * table = tabDescriptor->table = new int[cx * cy];
	memset(table, -1, cx * cy * sizeof(int));
	int num = 0;
	for (r = 0; r < cy; r++) {
		CEDParagraph * row = GetRow(r);
		i = 0;
		for (int c = 0; c < ((EDROWDESCR*) row->descriptor)->numOfCells; c++) {
			CEDParagraph * cell = row->GetCell(c);
			if (((EDCELLDESCR*) cell->descriptor)->merging == 0
					|| ((EDCELLDESCR*) cell->descriptor)->merging
							== ED_CELL_MRGF) {
				while (((EDCELLDESCR*) cell->descriptor)->cellX
						>= linesX[i + 1]) {
					table[r * cx + i] = num;
					i++;
					if (i == cx)
						break;
					//break if next vertical line has the same coordinates as current one
					//and there are some more cells
					if (((EDCELLDESCR*) cell->descriptor)->cellX == linesX[i]
							&& linesX[i] == linesX[i + 1] && c
							< ((EDROWDESCR*) row->descriptor)->numOfCells - 1)
						break;
				}
				num++;
			} else {
				while (((EDCELLDESCR*) cell->descriptor)->cellX
						>= linesX[i + 1]) {
					table[r * cx + i] = table[(r - 1) * cx + i];
					i++;
					if (i == cx)
						break;
					//break if next vertical line has the same coordinates as current one
					if (((EDCELLDESCR*) cell->descriptor)->cellX == linesX[i]
							&& linesX[i] == linesX[i + 1] && c
							< ((EDROWDESCR*) row->descriptor)->numOfCells - 1)
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
	for (line = lines; line && line->internalNumber - i != _num; line
			= line->next)
		;
	return line;
}

int CEDParagraph::GetCountLine() {
	return numOfLines;
}

/////////////////////////////////////////////////////////////////////
CEDLine::CEDLine() {
	extData = 0;
	extDataLen = 0;
	chars = 0;
	curChar = 0;
	internalNumber = 0;
	parentNumber = 0;
	prev = next = 0;
	numOfChars = 0;
	hardBreak = FALSE;
	defChrFontHeight = -1;
}

CEDLine::~CEDLine() {
	/*	curChar=chars;
	 CEDChar * qq;
	 while (curChar)
	 {
	 qq=curChar->next;
	 delete curChar;
	 curChar=qq;
	 }
	 */}

CEDChar * CEDLine::InsertChar() {
	CEDChar * chr = new CEDChar;
	numOfChars++;
	chr->parentNumber = internalNumber;
	if (curChar) {
		chr->next = curChar->next;
		if (chr->next)
			(chr->next)->prev = chr;
		curChar->next = chr;
		chr->prev = curChar;
		//		chr->internalNumber=curChar->internalNumber+1;
		//		for(CEDChar * chr1=chr->next;chr1;chr1=chr1->next)
		//			chr1->internalNumber++;
	} else {
		chars = chr;
		//		if(internalNumber!=0)
		//		{
		CEDLine *ww = prev;
		while (ww && !ww->chars)
			ww = ww->prev;
		if (ww) {
			CEDChar *qq = ww->chars;
			while (qq->next)
				qq = qq->next;
			qq->next = chr;
			chr->prev = qq;
			//				chr->internalNumber=qq->internalNumber+1;
		}
		ww = next;
		while (ww && !ww->chars)
			ww = ww->next;
		if (ww) {
			CEDChar *qq = ww->chars;
			qq->prev = chr;
			chr->next = qq;
		}
		//		}
	}
	curChar = chr;
	return chr;
}

void CEDLine::SetCurChar(CEDChar* _char) {
	curChar = _char;
}

CEDChar * CEDLine::SetCurChar(int _number) {
	int num = 0;
	//	if (chars)
	//		i=chars->internalNumber;
	CEDChar* chr;
	for (chr = chars; chr && num != _number; chr = chr->next)
		num++;
	curChar = chr;
	return chr;
}

CEDChar * CEDLine::GetCurChar() {
	return curChar;
}

int CEDLine::GetNumOfCurChar() {
	int num = 0;
	for (CEDChar* chr = chars; chr && chr != curChar; chr = chr->next)
		num++;
	return num;
}

CEDChar * CEDLine::NextChar(Bool32 _goThroughLines) {
	if (_goThroughLines)
		return curChar->next;
	if (curChar->next && curChar->next->parentNumber == curChar->parentNumber)
		return curChar->next;
	else
		return 0;
}

CEDChar * CEDLine::PrevChar(Bool32 _goThroughLines) {
	if (_goThroughLines)
		return curChar->prev;
	if (curChar->prev && curChar->prev->parentNumber == curChar->parentNumber)
		return curChar->prev;
	else
		return 0;
}

CEDChar* CEDLine::GetChar(int _num) {
	int num = 0;
	//	if (chars)
	//		i=chars->internalNumber;
	CEDChar* chr;
	for (chr = chars; chr && num != _num; chr = chr->next)
		num++;
	return chr;
}

int CEDLine::GetCountChar() {
	return numOfChars;
}

/////////////////////////////////////////////////////////////////////
CEDChar::CEDChar() {
	layout.top = layout.bottom = layout.left = layout.right = 0;
	fontHeight = fontNum = fontAttribs = fontNum = fontLang = 0;
	foregroundColor = backgroundColor = 0;
	alternatives = 0;
	numOfAltern = 0;
	extData = 0;
	extDataLen = 0;
	//	internalNumber=0;
	parentNumber = 0;
	prev = next = 0;
}

CEDChar::~CEDChar() {
}

