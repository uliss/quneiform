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

#include <algorithm>
#include "crtfword.h"
#include "crtfchar.h"
#include "creatertf.h"
#include "rfrmtoptions.h"

// ced
#include "ced/cedchar.h"

#include "cpage/cpage.h"
#include "common/helper.h"
#include "common/debug.h"
#include "common/tostring.h"

namespace CIF
{

RfrmtDrawWordFunction CRtfWord::draw_func_;

CRtfWord::CRtfWord() :
    font_number_(0), ideal_font_size_(0), real_font_size_(0), spelling_(false), probability_(0) {
}

CRtfWord::~CRtfWord() {
    clearChars();
}

void CRtfWord::addChar(uchar ch) {
    addChar(new CRtfChar(ch));
}

void CRtfWord::addChar(CRtfChar * chr) {
    chars_.push_back(chr);
}

Rect CRtfWord::bRect() const {
    return brect_;
}

void CRtfWord::calcBRect() {
    PAGEINFO PageInfo;
    Handle hCPAGE = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    if (!hCPAGE)
        throw Exception("[CRtfWord::calcBRect] CPAGE_GetHandlePage failed");

    GetPageInfo(hCPAGE, &PageInfo);

    Rect tmp = charsBRect();
    rotateRect(tmp, PageInfo.Angle, PageInfo.Width, PageInfo.Height);
    brect_ = tmp;
}

void CRtfWord::calcProbability() {
    probability_ = charProbability();
}

void CRtfWord::calcSpelling() {
    spelling_ = charSpelling();
}

CRtfChar * CRtfWord::charAt(size_t pos) {
    return chars_.at(pos);
}

size_t CRtfWord::charCount() const {
    return chars_.size();
}

Rect CRtfWord::charsBRect() const {
    if (chars_.empty())
        return Rect();

    // uliss TODO only first and last char are counted, is it better to
    // unite all character bboxes*
    return firstChar()->realRect().united(lastChar()->realRect());
}

int CRtfWord::charTotalLength() const {
    int result = 0;
    for (CharIteratorConst it = chars_.begin(), e = chars_.end(); it != e; ++it) {
        int wd = (*it)->realRect().width();
        if (wd > 0)
            result += wd;
    }
    return result;
}

bool CRtfWord::charSpelling() const {
    for (CharList::const_iterator it = chars_.begin(), end = chars_.end(); it != end; ++it) {
        if (!(*it)->isSpelled())
            return false;
    }

    return true;
}

void CRtfWord::clearChars() {
    for (CharList::iterator it = chars_.begin(), end = chars_.end(); it != end; ++it)
        delete *it;
    chars_.clear();
}

void CRtfWord::drawLayout() const {
    if(!draw_func_.empty())
        draw_func_(this);
}

bool CRtfWord::empty() const {
    return chars_.empty();
}

bool CRtfWord::endsWith(int c) const {
    return chars_.empty() ? false : (chars_.back()->getChar() == c);
}

CRtfChar * CRtfWord::firstChar() {
    return const_cast<CRtfChar*> (const_cast<const CRtfWord*> (this)->firstChar());
}

const CRtfChar * CRtfWord::firstChar() const {
    if (chars_.empty())
        throw std::out_of_range("[CRtfWord::firstChar] word is empty");
    return chars_.front();
}

int CRtfWord::fontAttrs() const {
    int ret = 0;

    if (font_number_ & FORMAT_FONT_UNDERLINE)
        ret |= FORMAT_FONT_UNDERLINE;

    if (RfrmtOptions::useBold() && (font_number_ & FORMAT_FONT_BOLD))
        ret |= FORMAT_FONT_BOLD;

    if (RfrmtOptions::useItalic() && (font_number_ & FORMAT_FONT_ITALIC))
        ret |= FORMAT_FONT_ITALIC;

    return ret;
}

font_number CRtfWord::fontNumber() const {
    return font_number_;
}

short CRtfWord::idealFontSize() const {
    return ideal_font_size_;
}

CRtfChar * CRtfWord::lastChar() {
    return const_cast<CRtfChar*> (const_cast<const CRtfWord*> (this)->lastChar());
}

const CRtfChar * CRtfWord::lastChar() const {
    if (chars_.empty())
        throw std::out_of_range("[CRtfWord::lastChar] word is empty");
    return chars_.back();
}

CEDChar * CRtfWord::makeCedSpace(int fontName, int fontAttrs) const {
    CEDChar * space = CRtfChar::makeCedSpace(fontName, DefFontSize, fontAttrs);
    if (RfrmtOptions::useSize() || !RfrmtOptions::useFrames())
        space->setFontHeight(realFontSize() * 2);
    return space;
}

inline bool compareCharProbability(CRtfChar * first, CRtfChar * second) {
    return first->first().probability() < second->first().probability();
}

short CRtfWord::charProbability() const {
    CharList::const_iterator it = std::min_element(chars_.begin(), chars_.end(),
            compareCharProbability);
    if (it == chars_.end())
        return Letter::LOWEST_PROBABILITY;
    else
        return (*it)->first().probability();
}

short CRtfWord::probability() const {
    return probability_;
}

short CRtfWord::realFontSize() const {
    return real_font_size_;
}

void CRtfWord::rotateRect(Rect& rect, int angle, int x_offset, int y_offset) {
    Rect result = rect;
    switch (angle) {
    case 0:
    case 360:
        break;
    case 90: //270
        result.rleft() = rect.top();
        result.rright() = rect.bottom();
        result.rtop() = y_offset - rect.right();
        result.rbottom() = y_offset - rect.left();
        break;
    case 180:
        result.rleft() = x_offset - rect.right();
        result.rright() = x_offset - rect.left();
        result.rtop() = y_offset - rect.bottom();
        result.rbottom() = y_offset - rect.top();
        break;
    case 270: //90
        result.rleft() = x_offset - rect.bottom();
        result.rright() = x_offset - rect.top();
        result.rtop() = rect.left();
        result.rbottom() = rect.right();
        break;
    default:
        throw Exception("[CRtfWord::rotateRect] invalid rotate angle: " + CIF::toString(angle));
    }
    rect = result;
}

void CRtfWord::set(const std::string& str) {
    clearChars();

    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == ' ')
            continue;

        addChar(str[i]);
    }
}

void CRtfWord::setDrawCallback(RfrmtDrawWordFunction f) {
    draw_func_ = f;
}

void CRtfWord::setFontNumber(font_number number) {
    font_number_ = number;
}

void CRtfWord::calcCoordinatesAndProbability() {
    calcProbability();
    calcSpelling();
    calcBRect();
}

void CRtfWord::setIdealFontSize(short size) {
    ideal_font_size_ = size;
}

void CRtfWord::setRealFontSize(short size) {
    real_font_size_ = size;
}

bool CRtfWord::startsWith(int c) const {
    return chars_.empty() ? false : (chars_.front()->getChar() == c);
}

std::string CRtfWord::toString() const {
    std::string result;
    result.reserve(charCount());
    for (CharIteratorConst it = chars_.begin(), e = chars_.end(); it != e; ++it) {
        result.append(1, (*it)->first().getChar());
    }
    return result;
}

bool CRtfWord::isSpelled() const {
    return spelling_ > 0;
}

void CRtfWord::write(CEDLine * line) const {
    //RfrmtOptions::draw(brect_);
    //Debug() << brect_ << "\n";

    for (CharIteratorConst it = chars_.begin(), end = chars_.end(); it != end; ++it) {
        CEDChar * chr = (*it)->write(line);
        chr->setFontStyle(fontAttrs());
    }
}

}

