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

#include "cedline.h"

CEDLine::CEDLine() :
    hard_break_(FALSE) {
    extData = 0;
    extDataLen = 0;
    chars = 0;
    curChar = 0;
    internalNumber = 0;
    parentNumber = 0;
    prev = next = 0;
    numOfChars = 0;
    defChrFontHeight = -1;
}

CEDLine::~CEDLine() {
}

bool CEDLine::hardBreak() const {
    return hard_break_ == TRUE;
}

void CEDLine::setHardBreak(bool value) {
    hard_break_ = value ? TRUE : FALSE;
}

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
    }

    else {
        chars = chr;
        CEDLine *ww = prev;

        while (ww && !ww->chars)
            ww = ww->prev;

        if (ww) {
            CEDChar *qq = ww->chars;

            while (qq->next)
                qq = qq->next;

            qq->next = chr;
            chr->prev = qq;
        }

        ww = next;

        while (ww && !ww->chars)
            ww = ww->next;

        if (ww) {
            CEDChar *qq = ww->chars;
            qq->prev = chr;
            chr->next = qq;
        }
    }

    curChar = chr;
    return chr;
}

void CEDLine::SetCurChar(CEDChar* _char) {
    curChar = _char;
}

CEDChar * CEDLine::SetCurChar(int _number) {
    int num = 0;
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
    CEDChar* chr;

    for (chr = chars; chr && num != _num; chr = chr->next)
        num++;

    return chr;
}

int CEDLine::GetCountChar() {
    return numOfChars;
}
