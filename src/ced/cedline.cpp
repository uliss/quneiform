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
#include "cedchar.h"

namespace CIF
{

CEDLine::CEDLine() :
    hard_break_(false) {
    chars = 0;
    current_char_ = 0;
    internal_number_ = 0;
    parent_number_ = 0;
    prev = next = 0;
    numOfChars = 0;
    defChrFontHeight = -1;
}

CEDChar * CEDLine::currentChar() {
    return current_char_;
}

bool CEDLine::hardBreak() const {
    return hard_break_;
}

int CEDLine::internalNumber() const {
    return internal_number_;
}

int CEDLine::parentNumber() const {
    return parent_number_;
}

void CEDLine::setCurrentChar(CEDChar * chr) {
    current_char_ = chr;
}

void CEDLine::setHardBreak(bool value) {
    hard_break_ = value;
}

void CEDLine::setParentNumber(int number) {
    parent_number_ = number;
}

CEDChar * CEDLine::InsertChar() {
    CEDChar * chr = new CEDChar;
    numOfChars++;
    chr->setParentNumber(internal_number_);

    if (current_char_) {
        chr->next = current_char_->next;

        if (chr->next)
            (chr->next)->prev = chr;

        current_char_->next = chr;
        chr->prev = current_char_;
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

    current_char_ = chr;
    return chr;
}

CEDChar * CEDLine::SetCurChar(int _number) {
    int num = 0;
    CEDChar* chr;

    for (chr = chars; chr && num != _number; chr = chr->next)
        num++;

    current_char_ = chr;
    return chr;
}

int CEDLine::GetNumOfCurChar() {
    int num = 0;

    for (CEDChar* chr = chars; chr && chr != current_char_; chr = chr->next)
        num++;

    return num;
}

CEDChar * CEDLine::NextChar(Bool32 _goThroughLines) {
    if (_goThroughLines)
        return current_char_->next;

    if (current_char_->next && current_char_->next->parentNumber() == current_char_->parentNumber())
        return current_char_->next;

    else
        return 0;
}

CEDChar * CEDLine::PrevChar(Bool32 _goThroughLines) {
    if (_goThroughLines)
        return current_char_->prev;

    if (current_char_->prev && current_char_->prev->parentNumber() == current_char_->parentNumber())
        return current_char_->prev;

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

}

