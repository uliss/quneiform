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

