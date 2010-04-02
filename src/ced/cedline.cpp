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
    prev_ = next_ = 0;
    char_number_ = 0;
    defChrFontHeight = -1;
}

CEDChar * CEDLine::currentChar() {
    return current_char_;
}

CEDChar * CEDLine::first() {
    return chars;
}

bool CEDLine::hardBreak() const {
    return hard_break_;
}

int CEDLine::internalNumber() const {
    return internal_number_;
}

CEDLine * CEDLine::next() {
    return next_;
}

int CEDLine::parentNumber() const {
    return parent_number_;
}

CEDLine * CEDLine::prev() {
    return prev_;
}

void CEDLine::setCurrentChar(CEDChar * chr) {
    current_char_ = chr;
}

void CEDLine::setHardBreak(bool value) {
    hard_break_ = value;
}

void CEDLine::setFirst(CEDChar * chr) {
    chars = chr;
}

void CEDLine::setParentNumber(int number) {
    parent_number_ = number;
}

CEDChar * CEDLine::insertChar() {
    CEDChar * chr = new CEDChar;
    char_number_++;
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
        CEDLine *ww = prev_;

        while (ww && !ww->chars)
            ww = ww->prev_;

        if (ww) {
            CEDChar *qq = ww->chars;

            while (qq->next)
                qq = qq->next;

            qq->next = chr;
            chr->prev = qq;
        }

        ww = next_;

        while (ww && !ww->chars)
            ww = ww->next_;

        if (ww) {
            CEDChar *qq = ww->chars;
            qq->prev = chr;
            chr->next = qq;
        }
    }

    setCurrentChar(chr);
    return chr;
}

CEDChar * CEDLine::setCurrentChar(int _number) {
    int num = 0;
    CEDChar* chr = chars;

    for (; chr && num != _number; chr = chr->next)
        num++;

    setCurrentChar(chr);
    return current_char_;
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

CEDChar* CEDLine::charAt(int _num) {
    int num = 0;
    CEDChar* chr;

    for (chr = chars; chr && num != _num; chr = chr->next)
        num++;

    return chr;
}

int CEDLine::charCount() {
    return char_number_;
}

}

