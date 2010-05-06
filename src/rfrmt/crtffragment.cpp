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

#include <cstring>

#include "rfrmtoptions.h"
#include "crtffragment.h"
#include "crtfchar.h"
#include "crtfstring.h"
#include "crtfsector.h"
#include "crtfpage.h"
#include "crtfword.h"
#include "creatertf.h"
#include "crtffunc.h"
#include "frmtpict.h"

#include "ced/ced.h"
#include "cstr/cstrdefs.h"
#include "common/cifconfig.h"
#include "common/debug.h"
#include "minmax.h"

void Rtf_CED_CreateChar(CIF::Rect* slayout, CIF::Letter* letter, CIF::CRtfChar* pRtfChar);
Handle Rtf_CED_CreateParagraph(int16_t FirstIndent, int16_t LeftIndent, int16_t RightIndent,
        int16_t IntervalBefore, RtfSectorInfo *SectorInfo, int AlignParagraph, int shad,
        int LenthStringInTwips, int LengthFragmInTwips);

namespace CIF
{

const int SMALL_FONT_SIZE = 5;
const int MEDIUM_FONT_SIZE = 10;
const int BIG_FONT_SIZE = 14;
const int PENALTY_FOR_SMALL_FONT_SIZE = 1;
const int PENALTY_FOR_MEDIUM_FONT_SIZE = 2;
const int PENALTY_FOR_BIG_FONT_SIZE = 4;
const int DEFAULT_MAX_CHAR_DISTANCE = 10;

CRtfFragment::CRtfFragment() :
    parent_(NULL), left_border_(0), right_border_(0), max_char_distance_(0), flag_carry_(false),
            mixed_fragment_(false) {
    m_CountLeftEqual = 0;
    m_CountRightEqual = 0;
    m_CountLeftRightEqual = 0;
    m_CountCentreEqual = 0;
    SetRect(&m_rect, 32000, 32000, 0, 0);
    m_bOutPutType = FOT_FRAME;
    m_LeftOffsetFragmentFromVerticalColumn = 0;
    m_RightOffsetFragmentFromVerticalColumn = 0;
    m_WidthVerticalColumn = 0;
    m_bFlagObjectInColumn = 0;
    m_wOffsetFromPrevTextFragment = 0;
    m_bFlagUsed = 0;
    m_Flag = 0;
}

CRtfFragment::~CRtfFragment() {
    clearStrings();
}

void CRtfFragment::addString(CRtfString * str) {
    strings_.push_back(str);
}

void CRtfFragment::adjustParagraph(int topMargin) {
    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it) {
        CRtfString * str = *it;

        if (it == strings_.begin()) {
            str->setTopMargin(topMargin);
            str->setParagraphBegin(true);
        } else {
            str->setTopMargin(0);
            str->setParagraphBegin(false);
        }

        if (str->isLineCarryNeeded())
            str->setLineCarry(true);
    }
}

void CRtfFragment::adjustStringIndents() {
    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it) {
        CRtfString * str = *it;
        str->setLeftIndent(str->leftBorder() - left_border_);
        str->setRightIndent(right_border_ - str->rightBorder());

        assert(0 <= str->leftIndent());
        assert(0 <= str->rightIndent());
    }
}

void CRtfFragment::calcFragmentBorders() {
    if (strings_.empty())
        return;

    left_border_ = minStringLeftBorder(strings_.begin(), strings_.end());
    right_border_ = maxStringRightBorder(strings_.begin(), strings_.end());

    assert(left_border_ <= right_border_);
}

void CRtfFragment::calcMaxCharDistance() {
    int char_count = charCount();

    if (char_count)
        max_char_distance_ = charTotalLength() / char_count;
    else
        max_char_distance_ = DEFAULT_MAX_CHAR_DISTANCE;
}

inline bool diffHasEqualSign(int first, int second) {
    return (first <= 0 && second <= 0) || (0 < first && 0 < second);
}

inline bool diffThreshold(int first, int second, int threshold) {
    return (abs(first) > threshold) && (abs(second) > threshold);
}

inline bool symmetricDiffChange(int first, int second, int delta) {
    return diffHasEqualSign(first, second) && diffThreshold(first, second, delta);
}

inline bool isAccuratelyCentered(int left_diff, int right_diff, int delta) {
    // center_diff == (right_diff - left_diff) / 2;
    // (abs(center_diff) < delta) equal to:
    // ((abs(right_diff - left_diff) / 2 ) < delta) equal to:
    // (abs(right_diff - left_diff) < 2 * delta)
    return (abs(right_diff - left_diff) < (2 * delta)) //
            && symmetricDiffChange(left_diff, right_diff, delta / 2);
}

inline bool isFreelyCentered(int left_diff, int right_diff, int max_delta) {
    return ((abs(right_diff + left_diff) / 2 < 2 * max_delta) //
            && (abs(right_diff - left_diff) < 3 * max_delta) //
            && diffThreshold(left_diff, right_diff, 5 * max_delta));
}

inline bool isStringCentered(int left_diff, int right_diff, int max_delta) {
    return isAccuratelyCentered(left_diff, right_diff, max_delta) //
            || isFreelyCentered(left_diff, right_diff, max_delta);
}

inline bool leftAligned(const CRtfString * first, const CRtfString * second, int max_delta) {
    return abs(first->leftIndent() - second->leftIndent()) <= max_delta;
}

inline bool rightAligned(const CRtfString * first, const CRtfString * second, int max_delta) {
    return abs(first->rightIndent() - second->rightIndent()) <= max_delta;
}

inline bool centered(const CRtfString * first, const CRtfString * second, int max_delta) {
    return isStringCentered(first->leftIndent() - second->leftIndent(), //
            first->rightIndent() - second->rightIndent(), max_delta);
}

void CRtfFragment::calcStringEndsEqual() {
    calcStringEndsEqual(strings_.begin(), strings_.end());
}

void CRtfFragment::calcStringEndsEqual(StringIterator begin, StringIterator end) {
    resetStringEndsEqual();

    if (begin == end)
        return;

    for (StringIterator it = begin + 1; it != end; ++it) {
        updateStringPairAlignment(*it, *(it - 1));

        if (it == (begin + 1))
            updateFirstStringPairAlignment();
    }
}

size_t CRtfFragment::charCount() const {
    size_t result = 0;
    for (StringIteratorConst it = strings_.begin(), e = strings_.end(); it != e; ++it)
        result += (*it)->charCount();
    return result;
}

int CRtfFragment::charTotalLength() const {
    int result = 0;
    for (StringIteratorConst it = strings_.begin(), e = strings_.end(); it != e; ++it)
        result += (*it)->charTotalLength();
    return result;
}

void CRtfFragment::clearStrings() {
    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it)
        delete *it;
    strings_.clear();
}

void CRtfFragment::correctParagraphIndents(StringIterator begin, StringIterator end) {
    int min_left_indent = minParagraphLeftIndent(begin, end);

    for (StringIterator it = begin; it != end; ++it) {
        if ((*it)->isParagraphBegin()) {
            if (abs((*it)->leftIndent() - min_left_indent) < max_char_distance_)
                (*it)->setLeftIndent(min_left_indent);
        }
    }
}

CRtfString * CRtfFragment::firstString() {
    return strings_.at(0);
}

const CRtfString * CRtfFragment::firstString() const {
    return strings_.at(0);
}

bool CRtfFragment::isMixed() const {
    return mixed_fragment_;
}

void CRtfFragment::initFragmentFonts(int fragment_count) {
    CRtfWord* first_word = firstString()->firstWord();
    firstString()->setFontSizePenalty(SMALL_FONT_SIZE, fontSizePenalty(fragment_count));
    m_wprev_font_name = fontName(first_word->fontNumber());
    m_wprev_font_size = first_word->realFontSize();
}

bool CRtfFragment::isBigIndent(const CRtfString* str) const {
    return str->leftIndent() > (2 * max_char_distance_);
}

bool CRtfFragment::isIndentsDiffer(const CRtfString* str, const CRtfString* prev) const {
    return abs(str->leftIndent() - prev->leftIndent()) > max_char_distance_;
}

bool CRtfFragment::isDirectSpeech(const CRtfString* str, const CRtfString* prev) const {
    /*
     * Example:
     *
     * .....bla-bla-bla.
     * -- Hello!
     */
    return str->startsWithDash() && //
            (prev->isEndOfSentence() || prev->endsWith(':') || //
                    (prev->rightIndent() > 2 * max_char_distance_));
}

bool CRtfFragment::isShortEndString(const CRtfString* str) const {
    // too short
    if (str->rightIndent() > 15 * max_char_distance_)
        return true;

    // medium short, but ends sentence '.'
    if ((str->rightIndent() > 5 * max_char_distance_) && //
            (str->isEndOfSentencePart() || str->isEndOfSentence()))
        return true;

    return false;
}

bool CRtfFragment::isParagraphIndent(const CRtfString* str, const CRtfString* prev) const {
    return isBigIndent(str) && isIndentsDiffer(str, prev);
}

bool CRtfFragment::isParagraphStartsWithDigit(const CRtfString* str, const CRtfString* prev) const {
    return str->startsWithDigit() && prev->isEndOfSentence();
}

CRtfString * CRtfFragment::lastString() {
    return const_cast<CRtfString*> (const_cast<const CRtfFragment*> (this)->lastString());
}

const CRtfString * CRtfFragment::lastString() const {
    if (strings_.empty())
        throw std::out_of_range("[CRtfFragment::lastString] fragment is empty");
    return strings_.back();
}

int CRtfFragment::minParagraphLeftIndent(StringIteratorConst begin, StringIteratorConst end) {
    int min_left_indent = 0;
    for (StringIteratorConst it = begin; it != end; ++it) {
        if (it == begin) {
            min_left_indent = (*it)->leftIndent();
            continue;
        }

        if ((*it)->isParagraphBegin()) {
            min_left_indent = MIN((*it)->leftIndent(), min_left_indent);
        }
    }
    return min_left_indent;
}

inline bool stringLeftBorderCompare(const CRtfString * first, const CRtfString * second) {
    return first->leftBorder() < second->leftBorder();
}

int CRtfFragment::minStringLeftBorder(StringIteratorConst begin, StringIteratorConst end) const {
    if (begin == end)
        throw std::out_of_range("[CRtfFragment::minStringLeftBorder] fragment is empty");

    StringIteratorConst it = std::min_element(begin, end, stringLeftBorderCompare);

    return (*it)->leftBorder();
}

int CRtfFragment::minStringLeftBorder() const {
    return minStringLeftBorder(strings_.begin(), strings_.end());
}

inline bool stringRightBorderCompare(const CRtfString * first, const CRtfString * second) {
    return first->rightBorder() < second->rightBorder();
}

int CRtfFragment::maxStringRightBorder(StringIteratorConst begin, StringIteratorConst end) const {
    if (begin == end)
        throw std::out_of_range("[CRtfFragment::maxStringRightBorder] fragment is empty");

    StringIteratorConst it = std::max_element(begin, end, stringRightBorderCompare);

    return (*it)->rightBorder();
}

int CRtfFragment::maxStringRightBorder() const {
    return maxStringRightBorder(strings_.begin(), strings_.end());
}

CRtfFragment::StringIterator CRtfFragment::findNextFragment(StringIterator begin) {
    if (begin == strings_.end())
        return strings_.end();

    for (StringIterator it = begin + 1, end = strings_.end(); it != end; ++it) {
        if ((*it)->hasAttributes())
            return it;
    }
    return strings_.end();
}

CRtfFragment::StringIterator CRtfFragment::findParagraph(StringIterator begin, StringIterator end) {
    for (StringIterator it = begin; it != end; ++it) {
        if ((*it)->isParagraphBegin())
            return it;
    }
    return strings_.end();
}

void CRtfFragment::updateFirstStringPairAlignment() {
    assert(strings_.size() > 1);
    CRtfString * current = strings_[1];
    CRtfString * previous = strings_.front();

    if (current->isEqualLeft()) {
        previous->setEqualLeft(true);
        m_CountLeftEqual++;
    } else if (current->isEqualRight()) {
        previous->setEqualRight(true);
        m_CountLeftRightEqual++;
        m_CountRightEqual++;
    } else if (current->isEqualCenter()) {
        previous->setEqualCenter(true);
        m_CountCentreEqual++;
    } else {
        Debug() << "[CRtfFragment::updateFirstStringPairAlignment] no string alignment\n";
    }
}

void CRtfFragment::updateStringPairAlignment(CRtfString * current, CRtfString * previous) {
    assert(current != NULL && previous != NULL);

    if (leftAligned(current, previous, max_char_distance_)) {
        current->setEqualLeft(true);
        m_CountLeftEqual++;
    }

    if (rightAligned(current, previous, max_char_distance_)) {
        current->setEqualRight(true);
        m_CountRightEqual++;

        if (current->isEqualLeft())
            m_CountLeftRightEqual++;
    }

    if (centered(current, previous, max_char_distance_)) {
        current->setEqualCenter(true);
        m_CountCentreEqual++;
    }
}

void CRtfFragment::processingUseNoneMode() {
    for (size_t i = 0; i < strings_.size(); i++) {
        CRtfString * str = strings_[i];

        if (i == 0)
            str->setParagraphBegin(true);
        else
            str->setParagraphBegin(false);

        if (i == stringCount() - 1)
            str->setLineTransfer(false);
        else
            str->setLineTransfer(true);

        str->setAlign(FORMAT_ALIGN_LEFT);
        str->setLeftIndent(0);
        str->setRightIndent(0);
        str->setFirstIndent(0);
    }
}

void CRtfFragment::setMixed(bool value) {
    mixed_fragment_ = value;
}

void CRtfFragment::setFragmentAlignment(RtfSectorInfo* SectorInfo) {
    if (RfrmtOptions::useNone())
        return processingUseNoneMode();

    Init(SectorInfo);

    if (SectorInfo->FlagOverLayed)
        return processingOverlayed();

    if (isMixed())
        determineMixedFragment(SectorInfo);
    else
        determineAlign(strings_.begin(), strings_.end());

    Done();
}

void CRtfFragment::setLineTransfer(StringIterator begin, StringIterator end) {
    for (StringIterator it = begin; it != end; ++it)
        (*it)->setLineTransfer(true);
}

void CRtfFragment::setParent(CRtfPage * page) {
    parent_ = page;
}

CRtfString * CRtfFragment::stringAt(size_t pos) {
    return strings_.at(pos);
}

const CRtfString * CRtfFragment::stringAt(size_t pos) const {
    return strings_.at(pos);
}

size_t CRtfFragment::stringCount() const {
    return strings_.size();
}

std::string CRtfFragment::toString() const {
    std::string result;
    for (StringIteratorConst it = strings_.begin(), e = strings_.end(); it != e; ++it) {
        result.append((*it)->toString());
        if (it != strings_.begin())
            result += "\n";
    }
    return result;
}

void CRtfFragment::FWriteText(int NumberCurrentFragment, RtfSectorInfo *SectorInfo, Bool OutPutType) {
    CRtfChar* pRtfChar;
    uint16_t CountWords;
    int16_t flag_end_word_with_hiphen;
    int16_t tmp_font_name;
    Bool boPrevNega, boNega;
    int EDFontAttribs, EDFontPointSize;
    Handle hParagraph = NULL;
    Handle hString = NULL;
    CIF::Rect slayout;
    CIF::Letter Letter[REC_MAX_VERS];
    int shading = -1;
    initFragment(SectorInfo);
    //--- Цикл по строкам
    boPrevNega = false; //NEGA_STR

    for (int ns = 0; ns < strings_.size(); ns++) {
        CRtfString * str = strings_[ns];
        CRtfWord* pRtfWord = str->firstWord();
        pRtfChar = pRtfWord->firstChar();

        if (pRtfChar->isDropCap()) { //заносим буквицы во frame
            if (RfrmtOptions::useFrames() || OutPutType)
                pRtfChar->setDropCap(false);
            else
                str->setParagraphBegin(true);
        }

        // чтобы не смешивать в одном абзаце негатив. и позитив. строки, при смене
        // цвета стартуем новый абзац
        boNega = str->hasFlag(CSTR_STR_NEGATIVE); //NEGA_STR

        if (boNega != boPrevNega)
            str->setParagraphBegin(true);

        if (str->isParagraphBegin()) {
            if (str->align() == FORMAT_ALIGN_LIST) {
                str->setAlign(FORMAT_ALIGN_JUSTIFY);
                m_fi = -str->firstIndent();
            } else
                m_fi = str->firstIndent();

            m_wvid_parag = str->align();
            m_li = str->leftIndent();
            m_ri = str->rightIndent();
            m_sb = str->marginTop();

            if (OutPutType)
                m_sb = 0;

            m_wprev_font_size = pRtfWord->realFontSize();

            if (CIF::RfrmtOptions::useFramesAndColumns()) {
                if (SectorInfo->FlagOneString == TRUE) {
                    m_li = 0;
                    m_fi = MAX(0, (int16_t)(m_rect.left - SectorInfo->MargL));
                    m_ri = 0;
                }
            }

            pRtfWord = str->firstWord();
            pRtfChar = pRtfWord->firstChar();
            int colWidth = 0;

            if (parent_ && !RfrmtOptions::useNone()) {
                CRtfSector* curSect =
                        (CRtfSector*) parent_->m_arSectors[parent_->m_nCurSectorNumber];

                //Если пишем с форматированием и однострочная колонка
                if (RfrmtOptions::useFramesAndColumns() && curSect->SectorInfo.FlagOneString
                        == TRUE) {
                    colWidth = SectorInfo->PaperW - (SectorInfo->MargL + SectorInfo->MargR);
                } //Если пишем в колонку
                else if (SectorInfo->hColumn == SectorInfo->hObject) {
                    colWidth = curSect->m_arWidthTerminalColumns[curSect->m_VTerminalColumnNumber
                            - 1];
                }
            }

            //Если ничего не помогло
            if (!colWidth)
                colWidth = m_rectReal.right - m_rectReal.left;

            if (!pRtfChar->isDropCap())
                hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb, SectorInfo,
                        m_wvid_parag, str->flags(), str->realLength(), colWidth); //NEGA_STR
        }

        if (!pRtfChar->isDropCap()) {
#ifdef CHEREDOV
            hString = CED_CreateLine(hParagraph, str->line_break_, (int)((pRtfWord->real_font_size_ - 1) * 2));
#else

            if (!RfrmtOptions::useSize() && !RfrmtOptions::useFrames()) {
                hString = CED_CreateLine(hParagraph, str->lineTransfer(), DefFontSize); //line is text line
            } else {
                hString = CED_CreateLine(hParagraph, str->lineTransfer(), pRtfWord->realFontSize()
                        * 2);
            }

#endif
        }

        //--- Цикл по словам
        CountWords = str->wordCount();

        for (int nw = 0; nw < CountWords; nw++) {
            pRtfWord = str->wordAt(nw);
            pRtfChar = pRtfWord->firstChar();
            tmp_font_name = fontName(pRtfWord->fontNumber());
            EDFontAttribs = 0;

            if (pRtfWord->fontNumber() & FORMAT_FONT_UNDERLINE) {
                EDFontAttribs = EDFontAttribs | FORMAT_FONT_UNDERLINE;
            }

            if (RfrmtOptions::useBold() && (pRtfWord->fontNumber() & FORMAT_FONT_BOLD)) {
                EDFontAttribs = EDFontAttribs | FORMAT_FONT_BOLD;
            }

            if (RfrmtOptions::useItalic() && (pRtfWord->fontNumber() & FORMAT_FONT_ITALIC)) {
                EDFontAttribs = EDFontAttribs | FORMAT_FONT_ITALIC;
            }

            flag_end_word_with_hiphen = 0;
            pRtfWord->calcCoordinatesAndProbability();
            //--- Цикл по буквам

            for (int nz = 0, total = pRtfWord->charCount(); nz < total; nz++) {
                pRtfChar = pRtfWord->charAt(nz);

                if (!pRtfWord->isSpelled())
                    pRtfChar->first().setProbability(0);

                if (nw == 0 && nz == 0 && pRtfChar->isDropCap())
#ifdef CHEREDOV
                    EDFontPointSize = (int)((pRtfChar->font_size_ - 1) * 2);

#else

                    if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames())
                        EDFontPointSize = DefFontSize;
                    else
                        EDFontPointSize = pRtfChar->fontSize() * 2;

#endif
                else
#ifdef CHEREDOV
                EDFontPointSize = (int)((pRtfWord->real_font_size_ - 1) * 2);

#else

                if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames())
                    EDFontPointSize = DefFontSize;
                else
                    EDFontPointSize = pRtfWord->realFontSize() * 2;

#endif
                flag_end_word_with_hiphen = 0;

                if (nw == (total - 1) && nz == (total - 1) && pRtfChar->first().getChar() == '-')
                    flag_end_word_with_hiphen = 1;

                if (pRtfChar->first().getChar()) {
                    if (str->lineTransfer()) {
                        if (nw == 0 && nz == 0 && pRtfChar->isDropCap()) {
                            slayout = CIF::Rect();
                            EDBOX playout__ = { 0, 0, 0, 0 };
                            Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector,
                                    SectorInfo->hColumn, playout__, 0x22, -1, -1, -1);
                            CED_SetFrameFlag(hObject__, ED_DROPCAP);
                            EDSIZE interval__ = { 0, 0 };

                            //                          if(m_Flag & CSTR_STR_NEGATIVE) //nega
                            if (str->hasFlag(CSTR_STR_NEGATIVE)) //NEGA_STR
                                shading = 10000;

                            Handle hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector,
                                    hObject__, TP_LEFT_ALLIGN, slayout, 0, -1, interval__,
                                    playout__, -1, shading, -1, -1, FALSE);
                            Handle hString__ = CED_CreateLine(hParagraph__, FALSE, 6);
                            Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                            CED_CreateChar(hString__, slayout, Letter, EDFontPointSize,
                                    (int) tmp_font_name, EDFontAttribs, pRtfChar->language(), -1,
                                    -1);
                            hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb,
                                    SectorInfo, m_wvid_parag, str->flags(), str->realLength(),
                                    m_rectReal.right - m_rectReal.left); //NEGA_STR
#ifdef CHEREDOV
                            hString = CED_CreateLine(hParagraph, str->line_break_, (int)((pRtfWord->real_font_size_ - 1) * 2));
#else

                            if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames()) {
                                hString = CED_CreateLine(hParagraph, str->lineTransfer(),
                                        DefFontSize);
                            } else {
                                hString = CED_CreateLine(hParagraph, str->lineTransfer(),
                                        pRtfWord->realFontSize() * 2);
                            }

#endif
                        } else {
                            Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                            CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                    (int) tmp_font_name, EDFontAttribs, pRtfChar->language(), -1,
                                    -1);
                        }
                    } else if (!((m_wvid_parag == FORMAT_ALIGN_JUSTIFY || m_wvid_parag
                            == FORMAT_ALIGN_LEFT) && flag_end_word_with_hiphen
                            && pRtfChar->m_bFlg_spell_nocarrying)) {
                        if (nw == 0 && nz == 0 && pRtfChar->isDropCap()) {
                            slayout = CIF::Rect();
                            EDBOX playout__ = { 0, 0, 0, 0 };
                            Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector,
                                    SectorInfo->hColumn, playout__, 0x22, -1, -1, -1);
                            CED_SetFrameFlag(hObject__, ED_DROPCAP);
                            EDSIZE interval__ = { 0, 0 };

                            Handle hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector,
                                    hObject__, TP_LEFT_ALLIGN, slayout, 0, -1, interval__,
                                    playout__, -1, -1, -1, -1, FALSE);
                            Handle hString__ = CED_CreateLine(hParagraph__, FALSE, 6);
                            Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                            CED_CreateChar(hString__, slayout, Letter, EDFontPointSize,
                                    (int) tmp_font_name, EDFontAttribs, pRtfChar->language(), -1,
                                    -1);
                            hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb,
                                    SectorInfo, m_wvid_parag, str->flags(), str->realLength(),
                                    m_rectReal.right - m_rectReal.left); //NEGA_STR
#ifdef CHEREDOV
                            hString = CED_CreateLine(hParagraph, str->line_break_, (int)((pRtfWord->real_font_size_ - 1) * 2));
#else

                            if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames()) {
                                hString = CED_CreateLine(hParagraph, str->lineTransfer(),
                                        DefFontSize);
                            } else {
                                hString = CED_CreateLine(hParagraph, str->lineTransfer(),
                                        pRtfWord->realFontSize() * 2);
                            }
#endif
                        } else {
                            Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                            CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                    (int) tmp_font_name, EDFontAttribs, pRtfChar->language(), -1,
                                    -1);
                        }
                    } else {
                        EDFontAttribs = EDFontAttribs | 0x02;
                        Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                        CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                (int) tmp_font_name, EDFontAttribs, pRtfChar->language(), -1, -1);
                    }
                }
            }

            //--- Конец цикла по буквам
            if (nw < CountWords - 1) {
                if (!pRtfChar->isDropCap()) {
                    Rtf_CED_CreateChar(&slayout, Letter, NULL);
#ifdef CHEREDOV
                    CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->real_font_size_ - 1)*2), (int)tmp_font_name,
                            EDFontAttribs, -1, -1, -1);
#else

                    if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames()) {
                        CED_CreateChar(hString, slayout, Letter, DefFontSize, (int) tmp_font_name,
                                EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);
                    } else {
                        CED_CreateChar(hString, slayout, Letter, pRtfWord->realFontSize() * 2,
                                (int) tmp_font_name, EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);
                    }
#endif
                }
            } else if ((ns < stringCount() - 1) && (nw == CountWords - 1) && (m_wvid_parag
                    == FORMAT_ALIGN_JUSTIFY || m_wvid_parag == FORMAT_ALIGN_LEFT)
                    && !flag_end_word_with_hiphen) {
                Rtf_CED_CreateChar(&slayout, Letter, NULL);
#ifdef CHEREDOV
                CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->real_font_size_ - 1)*2), (int)tmp_font_name,
                        EDFontAttribs, -1, -1, -1);
#else

                if (!RfrmtOptions::useSize() && RfrmtOptions::useFrames()) {
                    CED_CreateChar(hString, slayout, Letter, DefFontSize, (int) tmp_font_name,
                            EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);
                } else {
                    CED_CreateChar(hString, slayout, Letter, pRtfWord->realFontSize() * 2,
                            (int) tmp_font_name, EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);
                }
#endif
            }
        }

        //--- Конец цикла по словам
        boPrevNega = boNega; //NEGA_STR
    }

    //--- Конец цикла по строкам
}

int CRtfFragment::fontSizePenalty(int fragment_count) const {
    if (strings_.size() != 1 || fragment_count <= 1)
        return 0;

    int sz = strings_.front()->firstWord()->realFontSize();

    if (SMALL_FONT_SIZE < sz && sz <= MEDIUM_FONT_SIZE)
        return PENALTY_FOR_SMALL_FONT_SIZE;
    else if (MEDIUM_FONT_SIZE < sz && sz <= BIG_FONT_SIZE)
        return PENALTY_FOR_MEDIUM_FONT_SIZE;
    else if (BIG_FONT_SIZE < sz)
        return PENALTY_FOR_BIG_FONT_SIZE;
    else
        return 0;
}

void CRtfFragment::initFragment(RtfSectorInfo* SectorInfo) {
    assert(SectorInfo);
    initFragmentFonts(SectorInfo->CountFragments);
    m_wprev_lang = 1024;
    setFragmentAlignment(SectorInfo);
}

void CRtfFragment::FWritePicture(int NumberCurrentFragment, RtfSectorInfo *SectorInfo,
        Bool OutPutType) {
    WritePict(NumberCurrentFragment, SectorInfo, OutPutType);
}

void CRtfFragment::Init(RtfSectorInfo* SectorInfo) {
    assert(SectorInfo);

    adjustParagraph(SectorInfo->VerticalOffsetFragmentInColumn);

    //  Поиск:       Левой(m_l_fragment) и правой(m_r_fragment) границ фрагмента
    calcFragmentBorders();

    //  Вычисление:  m_max_dist, котороя используется при поиске начала абзаца
    calcMaxCharDistance();

    // Вычисляется отступ (leftIndent, rightIndent) строки от краев фрагмента
    adjustStringIndents();

    // Присваиваются признаки равенства концов и середины соседних строк
    calcStringEndsEqual(strings_.begin(), strings_.end());

    if (Config::instance().debugHigh())
        printResult(std::cerr, "\n ================== Init ==================");
}

// если колонки остались несепарабельными, то они все будут отписаны как frames
void CRtfFragment::processingOverlayed() {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfStringNext;
    CRtfString *pRtfString;

    for (size_t i = 0; i < stringCount(); i++) {
        CRtfString * str = strings_[i];
        str->setAlign(FORMAT_ALIGN_JUSTIFY);
        str->setParagraphBegin(false);
        str->setRightIndent(0);
    }

    for (size_t ns = 0; ns < stringCount(); ns++) {
        pRtfString = strings_[ns];

        if (ns == 0) {
            if (pRtfString->leftIndent() > max_char_distance_ / 2)
                pRtfString->setFirstIndent(max_char_distance_ * getTwips());
            else
                pRtfString->setFirstIndent(0);

            pRtfString->setLeftIndent(0);
            pRtfString->setParagraphBegin(true);
            continue;
        }

        // ns >= 1
        pRtfStringPrev = strings_[ns - 1];

        if (ns == stringCount() - 1) {
            if ((pRtfString->leftIndent() - pRtfStringPrev->leftIndent())
                    > (max_char_distance_ / 2)) {
                pRtfString->setLeftIndent(0);
                pRtfString->setFirstIndent(max_char_distance_ * getTwips());
                pRtfString->setParagraphBegin(true);
            }
        } else {
            pRtfStringNext = strings_[ns + 1];

            if (((pRtfString->leftIndent() - pRtfStringPrev->leftIndent()) > (max_char_distance_
                    / 2)) && ((pRtfString->leftIndent() - pRtfStringNext->leftIndent())
                    > (max_char_distance_ / 2))) {
                pRtfString->setLeftIndent(0);
                pRtfString->setFirstIndent(max_char_distance_ * getTwips());
                pRtfString->setParagraphBegin(true);
            }
        }
    }
}

void CRtfFragment::resetStringEndsEqual() {
    m_CountLeftEqual = 0;
    m_CountRightEqual = 0;
    m_CountCentreEqual = 0;
    m_CountLeftRightEqual = 0;
}

bool CRtfFragment::determineAlignJustify(StringIterator begin, StringIterator end) {
    if (!checkAlignJustify(begin, end))
        return false;

    setParagraphAlignment(begin, end, FORMAT_ALIGN_JUSTIFY);
    setFlagBeginParagraphForJustifyAlign(begin, end);
    correctParagraphIndents(begin, end);
    return true;
}

bool CRtfFragment::checkAlignJustify(StringIterator begin, StringIterator end) {
    int CountCentreEqual = countEqualAlign(begin, end, FORMAT_ALIGN_CENTER);
    const int string_count = std::distance(begin, end);

    // all centered
    if (CountCentreEqual == string_count)
        return false;

    flag_carry_ = hasFlagCarry(begin, end);
    flag_left_ = hasFlagLeft(begin, end);
    flag_strong_left_ = hasFlagStrongLeft(begin, end);
    flag_right_ = hasFlagRight(begin, end);
    flag_big_space_ = hasBigSpaceBetweenWords(begin, end);

    if (flag_strong_left_)
        return false;

    int CountLeftEqual = countEqualAlign(begin, end, FORMAT_ALIGN_LEFT);
    int CountLeftRightEqual = countEqualAlign(begin, end, FORMAT_ALIGN_JUSTIFY);

    if (flag_carry_ == false && //
            string_count <= 2 && //
            CountLeftRightEqual == 0)
        return false;

    if (flag_carry_ == false && //
            flag_big_space_ == false && //
            (CountLeftRightEqual < string_count / 3))
        return false;

    if (flag_right_ == true && string_count <= 4)
        return false;

    if ((CountLeftEqual > string_count) && flag_big_space_ == false) {
        if ((string_count < 5) && (CountLeftEqual > CountLeftRightEqual))
            return false;

        if (CountLeftRightEqual <= string_count / 2)
            return false;
    }

    int Count = 0;
    if (string_count > 0) {
        for (StringIterator it = (begin + 1); it != end; ++it) {
            if ((*it)->isEqualLeft() && (*it)->isEqualRight())
                continue;

            if (checkStringForJustifyAlign(std::distance(strings_.begin(), it)))
                Count++;
        }
    }

    if (flag_left_ && ((Count + CountLeftRightEqual) < 4 * (string_count - 1) / 5))
        return false;

    if (flag_carry_) {
        if ((Count + CountLeftRightEqual) < (string_count - 1) / 3)
            return false;
    } else {
        if ((Count + CountLeftRightEqual) < 4 * (string_count - 1) / 5)
            return false;

        CountCentreEqual += countEqualAlign(begin, end, FORMAT_ALIGN_CENTER);

        if ((Count + CountLeftRightEqual) < CountCentreEqual)
            return false;
    }

    return true;
}

int CRtfFragment::countStringEndDots(StringIteratorConst begin, StringIteratorConst end) {
    int count = 0;
    for (StringIteratorConst it = begin; it != end; ++it) {
        if ((*it)->endsWith('.'))
            count++;
    }
    return count;
}

uint CRtfFragment::countEqualAlign(StringIteratorConst begin, StringIteratorConst end,
        format_align_t AlignType) {
    uint count = 0;

    for (StringIteratorConst it = begin; it != end; ++it) {
        switch (AlignType) {
        case FORMAT_ALIGN_LEFT:
            if ((*it)->isEqualLeft())
                count++;
            break;
        case FORMAT_ALIGN_RIGHT:
            if ((*it)->isEqualRight())
                count++;
            break;
        case FORMAT_ALIGN_JUSTIFY:
            if ((*it)->isEqualLeft() && (*it)->isEqualRight())
                count++;
            break;
        case FORMAT_ALIGN_CENTER:
            if ((*it)->isEqualCenter())
                count++;
            break;
        default:
            break;
        }
    }

    if (count == 1)
        count++;

    return count;
}

bool CRtfFragment::checkStringForJustifyAlign(int ns) {
    int LeftFragm, RightFragm;
    int LeftDif, RightDif;

    if (isMixed()) {
        LeftFragm = local_fragment_left_;
        RightFragm = local_fragment_right_;
        LeftDif = local_fragment_left_ - left_border_;
        RightDif = right_border_ - local_fragment_right_;
    } else {
        LeftFragm = left_border_;
        RightFragm = right_border_;
        LeftDif = 0;
        RightDif = 0;
    }

    CRtfString * str = strings_[ns];

    if ((str->leftIndent() - LeftDif) > max_char_distance_ && ns < stringCount() - 1) {
        CRtfString *str_next = strings_[ns + 1];

        if (((str->leftIndent() - LeftDif) < (RightFragm - LeftFragm) / 2) && //
                ((str->rightIndent() - RightDif) < max_char_distance_) && //
                ((str_next->leftIndent() - LeftDif) < max_char_distance_))
            return true;
    }

    if ((str->leftIndent() - LeftDif) < max_char_distance_ && ns > 1) {
        CRtfString *str_prev = strings_[ns - 1];

        if ((str_prev->rightIndent() - RightDif) < max_char_distance_)
            return true;
    }

    return false;
}

void CRtfFragment::setFlagBeginParagraphForJustifyAlign(StringIterator begin, StringIterator end) {
    for (StringIterator it = begin; it != end; ++it) {
        CRtfString * curr = *it;

        if (it == begin) {
            curr->setParagraphBegin(true);
            continue;
        }

        CRtfString * prev = *(it - 1);

        if (isParagraphIndent(curr, prev) || //
                isShortEndString(prev) || //
                isParagraphStartsWithDigit(curr, prev) || //
                // uliss FIXME suspicious condition
                ((curr->leftIndent() > 3 * max_char_distance_ / 2) && //
                        (prev->endsWith('.') || curr->startsWithDash())) || //
                isDirectSpeech(curr, prev))
            curr->setParagraphBegin(true);
    }
}

bool CRtfFragment::checkLeftAlign(uint left, uint right, uint center, uint justify, uint total) {
    // less than half
    if (left == 0 || (left < (total / 2)))
        return false;

    const bool non_left_only = (right + justify + center) > 0;
    const bool left_in_minority = (left < right) || (left < justify) || (left < center);

    return (non_left_only && left_in_minority) ? false : true;
}

bool CRtfFragment::determineAlignLeft(StringIterator begin, StringIterator end, bool direct) {
    if (!direct && !checkLeftAlign(begin, end))
        return false;

    setParagraphAlignment(begin, end, FORMAT_ALIGN_LEFT);

    if (hasFlagCarry(begin, end) == false || hasFlagLeft(begin, end)) {
        if (std::distance(begin, end) > 0)
            setLineTransfer(begin, end - 1);
    }

    setFlagBeginParagraphForLeftAlign(begin, end);
    correctParagraphIndents(begin, end);
    return true;
}

bool CRtfFragment::checkLeftAlign(StringIteratorConst begin, StringIteratorConst end) {
    return checkLeftAlign(countEqualAlign(begin, end, FORMAT_ALIGN_LEFT), //
            countEqualAlign(begin, end, FORMAT_ALIGN_RIGHT), //
            countEqualAlign(begin, end, FORMAT_ALIGN_CENTER), //
            countEqualAlign(begin, end, FORMAT_ALIGN_JUSTIFY), //
            std::distance(begin, end));
}

void CRtfFragment::setFlagBeginParagraphForLeftAlign(StringIterator begin, StringIterator end) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfString;
    uchar FlagStringParagraph = FALSE;
    uchar FlagStringParagraphSoft = FALSE;
    int LeftFragm, RightFragm;
    int LeftDif, RightDif;

    if (isMixed()) {
        LeftFragm = local_fragment_left_;
        RightFragm = local_fragment_right_;
        LeftDif = local_fragment_left_ - left_border_;
        RightDif = right_border_ - local_fragment_right_;
    } else {
        LeftFragm = left_border_;
        RightFragm = right_border_;
        LeftDif = 0;
        RightDif = 0;
    }

    int Count = countStringEndDots(begin, end);
    const int str_number = std::distance(begin, end);

    if (Count > (4 * str_number / 5))
        FlagStringParagraph = TRUE;

    if (Count >= (str_number / 3))
        FlagStringParagraphSoft = TRUE;

    for (StringIterator it = begin; it != end; ++it) {
        pRtfString = *it;

        if (it == begin) {
            pRtfString->setParagraphBegin(true);
            continue;
        }

        pRtfStringPrev = *(it - 1);

        if (((pRtfString->leftIndent() - LeftDif) > 2 * max_char_distance_)
                || ((pRtfStringPrev->rightIndent() - RightDif) > (RightFragm - LeftFragm) / 3)
                || ((pRtfString->leftIndent() > max_char_distance_) && pRtfString->startsWithDash())
                || (pRtfString->startsWithDigit() && (pRtfStringPrev->endsWith(';')
                        || pRtfStringPrev->endsWith('.'))) //
                || (pRtfStringPrev->endsWith('.') && FlagStringParagraphSoft == TRUE
                        && (pRtfStringPrev->rightIndent() - RightDif) > 5 * max_char_distance_)
                || (pRtfStringPrev->endsWith('.') && FlagStringParagraph == TRUE)) {
            pRtfStringPrev->setLineTransfer(false);
            pRtfString->setParagraphBegin(true);
        }
    }
}

bool CRtfFragment::determineAlign(StringIterator begin, StringIterator end) {
    if (determineAlignJustify(begin, end))
        return true;

    if (determineList(begin, end))
        return true;

    if (determineAlignLeft(begin, end, false))
        return true;

    if (determineAlignCenter(begin, end))
        return true;

    if (determineAlignRight(begin, end) == false)
        return true;

    if (determineAlignLeft(begin, end, true))
        return true;

    return false;
}

bool CRtfFragment::checkCenterAlign(int left, int right, int center, int justify, int total) {
    // less then half
    if (center == 0 || (center < total / 2))
        return false;

    const bool center_only = ((left + right + justify) == 0);
    const bool center_in_minority = (center < right) || (center < justify) || (center < left);
    return (!center_only && center_in_minority) ? false : true;
}

bool CRtfFragment::checkCenterAlign(StringIteratorConst begin, StringIteratorConst end) {
    return checkCenterAlign(countEqualAlign(begin, end, FORMAT_ALIGN_LEFT), //
            countEqualAlign(begin, end, FORMAT_ALIGN_RIGHT), //
            countEqualAlign(begin, end, FORMAT_ALIGN_CENTER), //
            countEqualAlign(begin, end, FORMAT_ALIGN_JUSTIFY), //
            std::distance(begin, end));
}

bool CRtfFragment::determineAlignCenter(StringIterator begin, StringIterator end) {
    if (!checkCenterAlign(begin, end))
        return false;

    setParagraphAlignment(begin, end, FORMAT_ALIGN_CENTER);
    setLineTransfer(begin, end);
    (*begin)->setParagraphBegin(true);
    return true;
}

bool CRtfFragment::checkRightAlign(uint left, uint right, uint center, uint justify, uint total) {
    if (right == 0 || (right < (total / 2)))
        return false;

    const bool right_in_minority = (right < center) || (right < justify) || (right < left);
    const bool non_right_only = (center + justify + left) > 0;
    return (non_right_only && right_in_minority) ? false : true;
}

bool CRtfFragment::checkRightAlign(StringIteratorConst begin, StringIteratorConst end) {
    return checkRightAlign(countEqualAlign(begin, end, FORMAT_ALIGN_LEFT), //
            countEqualAlign(begin, end, FORMAT_ALIGN_RIGHT), //
            countEqualAlign(begin, end, FORMAT_ALIGN_CENTER), //
            countEqualAlign(begin, end, FORMAT_ALIGN_JUSTIFY), //
            std::distance(begin, end));
}

bool CRtfFragment::determineAlignRight(StringIterator begin, StringIterator end) {
    flag_carry_ = hasFlagCarry(begin, end);

    if (flag_carry_ && flag_right_ == FALSE)
        return false;

    if (!checkRightAlign(begin, end))
        return false;

    setParagraphAlignment(begin, end, FORMAT_ALIGN_RIGHT);

    for (StringIterator it = begin; it != end; ++it) {
        CRtfString * str = *it;

        if (it == begin) {
            str->setParagraphBegin(true);
            str->setLineTransfer(true);
            continue;
        }

        str->setLineTransfer(true);
        CRtfString * prev = *(it - 1);

        if (prev->endsWith('.')) {
            str->setParagraphBegin(true);
            prev->setLineTransfer(false);
        }
    }
    return true;
}

bool CRtfFragment::determineList(StringIterator begin, StringIterator end) {
    // if all centered
    if ((int) countEqualAlign(begin, end, FORMAT_ALIGN_CENTER) == std::distance(begin, end))
        return false;

    int MinLeft = firstString()->leftBorder();
    int MaxLeft = MinLeft;
    int MaxRight = firstString()->rightBorder();

    //find list body
    for (StringIterator it = begin; it != end; ++it) {
        CRtfString * str = *it;
        MinLeft = std::min(MinLeft, str->leftBorder());
        MaxLeft = std::max(MaxLeft, str->leftBorder());
        MaxRight = std::max(MaxRight, str->rightBorder());
    }

    if ((MaxLeft - MinLeft) > (MaxRight - MaxLeft) / 2)
        return false;

    if ((MaxLeft - MinLeft) < max_char_distance_)
        return false;

    int CountMinLeft = 0;
    int CountMaxLeft = 0;
    int CountMaxRight = 0;

    for (StringIterator it = begin; it != end; ++it) {
        CRtfString *pRtfString = *it;

        if ((abs(MinLeft - pRtfString->leftBorder()) > 5 * max_char_distance_) && (abs(MaxLeft
                - pRtfString->leftBorder()) > 5 * max_char_distance_))
            return false;

        if (abs(MinLeft - pRtfString->leftBorder()) < max_char_distance_)
            CountMinLeft++;

        if (abs(MaxLeft - pRtfString->leftBorder()) < max_char_distance_)
            CountMaxLeft++;

        if (abs(MaxRight - pRtfString->rightBorder()) < max_char_distance_)
            CountMaxRight++;
    }

    if ((CountMinLeft > CountMaxLeft) || //
            ((CountMinLeft + CountMaxLeft) < 4 * std::distance(begin, end) / 5) || //
            (CountMaxRight < std::distance(begin, end) / 2))
        return false;

    setParagraphAlignment(begin, end, FORMAT_ALIGN_LIST);

    for (StringIterator it = begin; it != end; ++it) {
        if ((it == begin) || abs(MinLeft - (*it)->leftBorder()) < max_char_distance_)
            (*it)->setParagraphBegin(true);
    }

    return true;
}

bool CRtfFragment::determineMixedFragment(RtfSectorInfo* SectorInfo) {
    StringIterator it = strings_.begin();

    while (true) {
        if (it == strings_.end())
            break;

        StringIterator end = findNextFragment(it);
        ReInit(SectorInfo, it, end);
        determineAlign(it, end);
        it = end;
    }

    return true;
}

void CRtfFragment::ReInit(RtfSectorInfo* SectorInfo, StringIterator begin, StringIterator end) {
    assert(begin != end);

    for (StringIterator it = begin; it != end; ++it) {
        (*it)->setEqualLeft(false);
        (*it)->setEqualCenter(false);
        (*it)->setEqualRight(false);
    }

    for (StringIterator it = begin; it != end; ++it) {
        CRtfString *str = *it;

        if (it == begin) {
            // first string
            if (it == strings_.begin()) {
                str->setTopMargin(SectorInfo->VerticalOffsetFragmentInColumn);
            } else {
                CRtfString * prev = *(it - 1);
                str->setTopMargin(prev->firstChar()->idealRect().height());
            }

            str->setParagraphBegin(true);
        } else
            str->setTopMargin(0);
    }

    local_fragment_left_ = minStringLeftBorder(begin, end);
    local_fragment_right_ = maxStringRightBorder(begin, end);

    calcStringEndsEqual(begin, end);
}

void CRtfFragment::Done() {
    checkOnceAgainImportancesFlagBeginParagraph();
    setFirstLeftAndRightIndentOfParagraph();
    defineLineTransfer();
}

void CRtfFragment::checkOnceAgainImportancesFlagBeginParagraph() {
    if (strings_.empty())
        return;

    for (StringIterator it = (strings_.begin() + 1), end = strings_.end(); it != end; ++it) {
        CRtfString * prev = *(it - 1);
        CRtfString * curr = *it;

        // если выр. другая, то необходимо начать новый параграф
        if (curr->align() != prev->align()) {
            prev->setLineTransfer(false);
            curr->setParagraphBegin(true);
        }
    }

    for (StringIterator it = (strings_.begin() + 1), end = strings_.end(); it != end; ++it) {
        CRtfString * prev = *(it - 1);
        CRtfString * curr = *it;

        if (curr->align() != FORMAT_ALIGN_CENTER && //
                abs(curr->firstWord()->realFontSize() - prev->firstWord()->realFontSize()) > 1) {
            prev->setLineTransfer(false);
            curr->setParagraphBegin(true);
        }
    }

    for (StringIterator it = (strings_.begin() + 1), end = strings_.end(); it != end; ++it) {
        CRtfString * prev = *(it - 1);
        CRtfString * str = *it;

        // skip non paragraph
        if (!str->isParagraphBegin())
            continue;

        CRtfChar * last_prev_char = prev->lastChar();

        if (last_prev_char->first().isHyphen() && last_prev_char->m_bFlg_spell_nocarrying) {
            if (str->align() == prev->align())
                str->setParagraphBegin(false);
            else if (prev->align() == FORMAT_ALIGN_JUSTIFY && str->align() == FORMAT_ALIGN_LEFT) {
                str->setAlign(FORMAT_ALIGN_JUSTIFY);
                str->setParagraphBegin(false);
                prev->setLineTransfer(false);
            }
        }
    }
}

void CRtfFragment::setFirstLeftAndRightIndentOfParagraph() {
    CRtfString *pRtfStringNext;
    int MinLeftIndent;
    int MinRightIndent;
    int twp_dist = 3 * max_char_distance_ * getTwips();
    int Dif = 0;

    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it) {
        CRtfString * str = *it;
        str->calcRealLength();
        str->setLeftIndent(str->leftIndent() * getTwips() + m_LeftOffsetFragmentFromVerticalColumn);
        str->setRightIndent(str->rightIndent() * getTwips()
                + m_RightOffsetFragmentFromVerticalColumn);
        str->setRightIndent(
                MIN(str->rightIndent(),
                        m_WidthVerticalColumn - (str->realLength() + str->leftIndent() + str->rightIndent())));
    }

    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it) {
        CRtfString * str = *it;

        if (!str->isParagraphBegin())
            continue;

        switch (str->align()) {
        case FORMAT_ALIGN_LEFT: {
            Dif = MAX(0, m_WidthVerticalColumn - str->realLength());
            MinLeftIndent = str->leftIndent();
            StringIterator j;

            for (j = (it + 1); j != end; j++) {
                pRtfStringNext = *j;

                if (pRtfStringNext->isParagraphBegin() || pRtfStringNext->align()
                        != FORMAT_ALIGN_LEFT)
                    break;

                MinLeftIndent = MIN(pRtfStringNext->leftIndent(), MinLeftIndent);
            }

            if (m_WidthVerticalColumn > str->lengthInTwips()) {
                str->setFirstIndent(str->leftIndent() - MinLeftIndent);

                if (str->firstIndent() < (twp_dist / 3))
                    str->setFirstIndent(0);

                str->setLeftIndent(MIN(Dif, MinLeftIndent));
            } else {
                str->setLeftIndent(0);
                str->setFirstIndent(0);
            }

            str->setRightIndent(0);

            if (str->leftIndent() < (twp_dist / 2))
                str->setLeftIndent(0);

            // first string
            if (j == (it + 1)) {
                str->setFirstIndent(str->leftIndent());
                str->setLeftIndent(0);
            }
        }
            break;
        case FORMAT_ALIGN_RIGHT:
            str->setLeftIndent(0);
            str->setFirstIndent(0);
            str->setRightIndent(0);
            break;
        case FORMAT_ALIGN_JUSTIFY: {
            MinLeftIndent = str->leftIndent();
            MinRightIndent = str->rightIndent();

            StringIterator i;
            for (i = (it + 1); i != end; ++i) {
                pRtfStringNext = *i;

                if (pRtfStringNext->isParagraphBegin() || pRtfStringNext->align()
                        != FORMAT_ALIGN_JUSTIFY)
                    break;

                MinLeftIndent = MIN(pRtfStringNext->leftIndent(), MinLeftIndent);
                MinRightIndent = MIN(pRtfStringNext->rightIndent(), MinRightIndent);
            }

            if (MinLeftIndent < (twp_dist / 3))
                MinLeftIndent = 0;

            str->setFirstIndent(str->leftIndent() - MinLeftIndent);

            if (str->firstIndent() < (twp_dist / 3))
                str->setFirstIndent(0);

            if (MinLeftIndent < twp_dist)
                str->setLeftIndent(0);
            else
                str->setLeftIndent(MinLeftIndent);

            if (MinRightIndent < twp_dist)
                str->setRightIndent(0);

            if (i == (it + 1)) {
                if (MinLeftIndent > ((2 * twp_dist) / 3))
                    str->setLeftIndent(MinLeftIndent);

                str->setFirstIndent(str->leftIndent());
                str->setLeftIndent(0);
                str->setRightIndent(0);
            }
        }
            break;
        case FORMAT_ALIGN_CENTER:
            MinLeftIndent = str->leftIndent();
            MinRightIndent = str->rightIndent();

            for (StringIterator i = it; i != end; ++i) {
                pRtfStringNext = *i;
                MinLeftIndent = MIN(pRtfStringNext->leftIndent(), MinLeftIndent);
                MinRightIndent = MIN(pRtfStringNext->rightIndent(), MinRightIndent);

                if (str->align() != FORMAT_ALIGN_CENTER)
                    break;
            }

            if (abs(MinLeftIndent - MinRightIndent) < (2 * max_char_distance_))
                MinLeftIndent = MinRightIndent = 0;

            str->setFirstIndent(0);
            str->setLeftIndent(MinLeftIndent);
            str->setRightIndent(MinRightIndent);
            break;
        case FORMAT_ALIGN_LIST:
            if ((it + 1) != end) {
                pRtfStringNext = *(it + 1);

                if (!pRtfStringNext->isParagraphBegin() && pRtfStringNext->align()
                        == FORMAT_ALIGN_LIST) {
                    str->setFirstIndent(pRtfStringNext->leftIndent() - str->leftIndent());
                    str->setLeftIndent(pRtfStringNext->leftIndent());
                } else
                    str->setFirstIndent(0);
            } else
                str->setFirstIndent(0);
            break;
        }
    }
}

void CRtfFragment::defineLineTransfer() {
    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it) {
        if ((*it)->isParagraphBegin() && (RfrmtOptions::lineTransfer() //
                || (*it)->align() == FORMAT_ALIGN_CENTER)) {
            setLineTransfer(it, findParagraph(it, end));
        }
    }
}

bool CRtfFragment::hasFlagLeft(StringIterator begin, StringIterator end) {
    int Count = 0;
    bool PriznakLeft = FALSE;

    for (StringIteratorConst it = begin; it != end; ++it) {
        if ((*it)->endsWith(',')) {
            Count++;

            if ((*it)->rightIndent() > ((*it)->width()) / 4)
                PriznakLeft = true;
        }
    }

    if (Count > 1 && PriznakLeft)
        return true;

    int CountLeftEqual = countEqualAlign(begin, end, FORMAT_ALIGN_LEFT);

    if (CountLeftEqual == std::distance(begin, end))
        return true;

    flag_big_space_ = hasBigSpaceBetweenWords(begin, end);
    flag_carry_ = hasFlagCarry(begin, end);

    if (!flag_big_space_ && !flag_carry_)
        return true;

    return false;
}

bool CRtfFragment::hasFlagStrongLeft(StringIteratorConst begin, StringIteratorConst end) const {
    int count = 0;
    for (StringIteratorConst it = begin; it != end; ++it) {
        if ((*it)->endsWith('.') || (*it)->endsWith(','))
            count++;
    }

    if (count == std::distance(begin, end))
        return true;

    // small fragment
    if ((std::distance(begin, end) < 5) && count > 2)
        return true;

    return false;
}

bool CRtfFragment::hasFlagRight(StringIteratorConst begin, StringIteratorConst end) const {
    for (StringIteratorConst it = begin; it != end; ++it) {
        if ((*it)->rightIndent() > max_char_distance_)
            return false;
    }

    for (StringIteratorConst it = begin; it != end; ++it) {
        if ((*it)->leftIndent() > ((right_border_ - left_border_) / 2))
            return true;
    }

    return false;
}

bool CRtfFragment::hasFlagCarry(StringIteratorConst begin, StringIteratorConst end) {
    for (StringIteratorConst it = begin; it != end; ++it) {
        if ((*it)->lineCarry())
            return true;
    }

    return false;
}

bool CRtfFragment::hasBigSpaceBetweenWords(StringIteratorConst begin, StringIteratorConst end) const {
    for (StringIteratorConst it = begin; it != end; ++it) {
        if ((*it)->hasBigSpace(2 * max_char_distance_))
            return true;
    }

    return false;
}

void CRtfFragment::setParagraphAlignment(StringIterator begin, StringIterator end,
        format_align_t AlignType) {
    for (StringIterator it = begin; it != end; ++it)
        (*it)->setAlign(AlignType);
}

void CRtfFragment::printResult(std::ostream& os, const char * header_str) const {
    os << header_str << std::endl;

    os << "max char distance: " << max_char_distance_ << "\n";

    for (StringIteratorConst it = strings_.begin(), e = strings_.end(); it != e; ++it) {
        os << "String #" << std::distance(it, strings_.begin()) << "\n";
        (*it)->print(os);
        os << std::endl;
    }
}

}
