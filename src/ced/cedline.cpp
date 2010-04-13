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
    char_number_(0), hard_break_(false), current_char_(NULL), internal_number_(0),
            parent_number_(0), chars(NULL), prev_(NULL), next_(NULL), default_font_height_(-1) {
}

CEDLine::CEDLine(bool hardBreak, int fontHeight) :
    char_number_(0), hard_break_(hardBreak), current_char_(NULL), internal_number_(0),
            parent_number_(0), chars(NULL), prev_(NULL), next_(NULL), default_font_height_(
                    fontHeight) {
}

CEDChar * CEDLine::currentChar() {
    return current_char_;
}

int CEDLine::defaultFontHeight() const {
    return default_font_height_;
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

void CEDLine::setDefaultFontHeight(int height) {
    default_font_height_ = height;
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
    return insertChar(new CEDChar);
}

CEDChar * CEDLine::insertChar(CEDChar * chr) {
    assert(chr);
    char_number_++;
    chr->setParentNumber(internal_number_);

    if (current_char_) {
        chr->setNext(current_char_->next());
        chr->setPrev(current_char_);
    }

    else {
        chars = chr;
        CEDLine *ww = prev_;

        while (ww && !ww->chars)
            ww = ww->prev_;

        if (ww) {
            CEDChar *qq = ww->chars;

            while (qq->next())
                qq = qq->next();

            qq->setNext(chr);
        }

        ww = next_;

        while (ww && !ww->chars)
            ww = ww->next_;

        if (ww) {
            CEDChar *qq = ww->chars;

            qq->setPrev(chr);
        }
    }

    setCurrentChar(chr);
    return chr;
}

CEDChar * CEDLine::setCurrentChar(int _number) {
    int num = 0;
    CEDChar* chr = chars;

    for (; chr && num != _number; chr = chr->next())
        num++;

    setCurrentChar(chr);
    return current_char_;
}

CEDChar * CEDLine::nextChar() {
    if (!current_char_ || !current_char_->next())
        return NULL;

    if (current_char_->next()->parentNumber() == current_char_->parentNumber())
        return current_char_->next();
    else
        return NULL;
}

CEDChar * CEDLine::prevChar() {
    if (!current_char_ || !current_char_->prev())
        return NULL;

    if (current_char_->prev()->parentNumber() == current_char_->parentNumber())
        return current_char_->prev();
    else
        return NULL;
}

CEDChar* CEDLine::charAt(unsigned int _num) {
    unsigned int num = 0;
    CEDChar* chr = chars;

    for (; chr && num != _num; chr = chr->next())
        num++;

    return chr;
}

int CEDLine::charCount() {
    return char_number_;
}

}

