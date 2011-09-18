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

#include <cassert>
#include <cctype> // for isdigit
#include <iostream>

#include "crtfchar.h"
#include "rfrmtoptions.h"
#include "creatertf.h"

#include "cstr/cstr.h"
#include "recdefs.h"

// ced
#include "ced/ced.h"
#include "ced/cedchar.h"
#include "ced/cedcolumn.h"
#include "ced/cedframe.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "ced/cedsection.h"

extern cf::Point TemplateOffset;

namespace cf
{

RfrmtDrawCharFunction CRtfChar::draw_func_;

CRtfChar::CRtfChar() :
    language_(LANGUAGE_UNKNOWN), font_number_(0), font_size_(0), spelled_(false), drop_cap_(false),
            spell_nocarrying_(false) {

}

CRtfChar::CRtfChar(uchar chr, uchar probability) :
    language_(LANGUAGE_UNKNOWN), font_number_(0), font_size_(0), spelled_(false), drop_cap_(false),
            spell_nocarrying_(false) {
    addVersion(Letter(chr, probability));
}

void CRtfChar::addVersion(const Letter& version) {
    if (versions_.size() >= REC_MAX_VERS)
        throw std::out_of_range("[CRtfChar::addVersion] too many char versions");
    versions_.push_back(version);
}

void CRtfChar::drawLayout() const {
    if (draw_func_.empty())
        return;
    draw_func_(this);
}

bool CRtfChar::empty() const {
    return versions_.empty();
}

const Letter& CRtfChar::first() const {
    if (versions_.empty())
        throw std::out_of_range("[CRtfChar::first] no char versions");
    return versions_.front();
}

Letter& CRtfChar::first() {
    return const_cast<Letter&> (const_cast<const CRtfChar*> (this)->first());
}

font_number CRtfChar::font() const {
    return font_number_;
}

short CRtfChar::fontSize() const {
    return font_size_;
}

unsigned char CRtfChar::getChar() const {
    return versions_.empty() ? uchar(0) : versions_.front().getChar();
}

Rect CRtfChar::idealRect() const {
    return ideal_rect_;
}

bool CRtfChar::isDigit() const {
    if (versions_.empty())
        return false;

    return isdigit(versions_.front().getChar());
}

bool CRtfChar::isDropCap() const {
    return drop_cap_;
}

bool CRtfChar::isSpelled() const {
    return spelled_;
}

bool CRtfChar::isSpelledNoCarrying() const {
    return spell_nocarrying_;
}

language_t CRtfChar::language() const {
    return language_;
}

CEDChar * CRtfChar::makeCedSpace(int fontName, int fontSize, int fontAttrs) {
    CEDChar * chr = new CEDChar;

    chr->setFontLanguage(LANGUAGE_UNKNOWN);
    chr->addAlternative(Letter(' ', 0));
    chr->setColor(Color::null());
    chr->setBackgroundColor(Color::null());
    chr->setBoundingRect(Rect(Point(-1, -1), Point(-1, -1)));
    chr->setFontStyle(fontAttrs);
    chr->setFontHeight(fontSize);
    chr->setFontNumber(fontName);

    return chr;
}

CRtfChar * CRtfChar::read(FILE * in) {
    CRtfChar * chr = new CRtfChar;

    ::Rect16 SRect;
    fread(&SRect, sizeof(Rect16), 1, in); //Ideal BOX
    chr->setIdealRect(cf::Rect(Point(SRect.left, SRect.top), Point(SRect.right, SRect.bottom)));
    fread(&SRect, sizeof(Rect16), 1, in); //Real BOX
    chr->setRealRect(cf::Rect(Point(SRect.left, SRect.top), Point(SRect.right, SRect.bottom)));

    uint16_t num;
    fread(&num, sizeof(uint16_t), 1, in);
    assert(num <= REC_MAX_VERS);
    for (int i = 0; i < num; i++) {
        uchar letter, prob;
        fread(&letter, 1, 1, in);
        fread(&prob, 1, 1, in);
        chr->addVersion(Letter(letter, prob));
    }

    uchar spellnocarrying, capdrop, spell, base, language;
    fread(&language, 1, 1, in);
    fread(&spellnocarrying, 1, 1, in);
    fread(&capdrop, 1, 1, in);
    fread(&spell, 1, 1, in);
    fread(&base, 1, 1, in);

    chr->setLanguage(static_cast<language_t> (language));
    chr->setSpelledNoCarrying(spellnocarrying);
    chr->setDropCap(capdrop);
    chr->setSpelled(spell);

    return chr;
}

Rect CRtfChar::realRect() const {
    return real_rect_;
}

void CRtfChar::setDrawCallback(RfrmtDrawCharFunction f) {
    draw_func_ = f;
}

void CRtfChar::setDropCap(bool value) {
    drop_cap_ = value;
}

void CRtfChar::setFont(font_number fontNumber) {
    font_number_ = fontNumber;
}

void CRtfChar::setFontSize(short size) {
    font_size_ = size;
}

void CRtfChar::setIdealRect(const Rect& rect) {
    ideal_rect_ = rect;
}

void CRtfChar::setLanguage(language_t lang) {
    language_ = lang;
}

void CRtfChar::setRealRect(const Rect& rect) {
    real_rect_ = rect;
}

void CRtfChar::setSpelled(bool value) {
    spelled_ = value;
}

void CRtfChar::setSpelledNoCarrying(bool value) {
    spell_nocarrying_ = value;
}

CEDChar * CRtfChar::toCedChar(int font_name, int font_size, int font_style) const {
    CEDChar * chr = new CEDChar;

    chr->setBoundingRect(real_rect_.translated(TemplateOffset));
    chr->setFontLanguage(language_);
    chr->setFontStyle(font_style);
    chr->setFontNumber(font_name);
    chr->setFontHeight(font_size);

    for (size_t i = 0; i < versions_.size(); i++) {
        Letter alt = versions_[i];
        alt.normalizeNonPrintable();
        chr->addAlternative(alt);
    }

    chr->setColor(Color::null());
    chr->setBackgroundColor(Color::null());

    return chr;
}

CEDParagraph * CRtfChar::insertCedDropCap(SectorInfo * sector, int font_name, int font_size,
        int font_style, bool negative) const {
    Rect slayout;
    Rect playout;

    CEDFrame * frame = new CEDFrame(NULL, CEDFrame::HPOS_COLUMN, CEDFrame::VPOS_PAGE);
    sector->hColumn->addElement(frame);

    CEDParagraph * drop_cap_par = sector->hEDSector->createParagraph(frame, ALIGN_RIGHT, slayout,
            0, -1, playout);

    Color shading = negative ? Color(0, 0, 0) : Color::null();
    drop_cap_par->setBackgroundColor(shading);
    CEDLine * ced_line = new CEDLine(NULL, false, 6);
    drop_cap_par->addLine(ced_line);
    CEDChar * ced_char = toCedChar(font_name, font_size, font_style);
    ced_line->insertChar(ced_char);

    return drop_cap_par;
}

const Letter& CRtfChar::versionAt(size_t pos) const {
    return versions_.at(pos);
}

Letter& CRtfChar::versionAt(size_t pos) {
    return versions_.at(pos);
}

size_t CRtfChar::versionCount() const {
    return versions_.size();
}

CEDChar * CRtfChar::write(CEDLine * line) const {
    assert(line);
    assert(getChar());

    return line->insertChar(toCedChar(font_number_, font_size_, 0));
}

}

