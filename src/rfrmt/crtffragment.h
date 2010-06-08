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

#ifndef CRTFFRAGMENT_H_
#define CRTFFRAGMENT_H_

#include <vector>
#include <iostream>
#include <boost/function.hpp>

#include "cfcompat.h"
#include "font.h"
#include "creatertf.h"
#include "common/rect.h"

namespace CIF
{

struct SectorInfo;
class CRtfPage;
class CRtfString;
class CEDParagraph;
class CRtfFragment;

typedef boost::function<void(const CRtfFragment*)> RfrmtDrawFragmentFunction;

class CLA_EXPO CRtfFragment
{
    public:
        CRtfFragment();
        ~CRtfFragment();

        /**
         * Adds string at the end of fragment
         */
        void addString(CRtfString * str);

        /**
         * Returns number of chars in fragment
         */
        size_t charCount() const;

        /**
         * Returns char total length
         */
        int charTotalLength() const;

        /**
         * Removes all string from fragment
         */
        void clearStrings();

        /**
         * Draws fragment layout via callback function
         */
        void drawLayout() const;

        /**
         * Returns pointer to first string in fragment
         * @throw std::out_of_range if fragment is empty
         */
        CRtfString * firstString();
        const CRtfString * firstString() const;

        /**
         * Returns true if object in column
         */
        bool inColumn() const;

        /**
         * Returns pointer to last string in fragment
         * @throw std::out_of_range if fragment is empty
         */
        CRtfString * lastString();
        const CRtfString * lastString() const;

        /**
         * Prints fragment content to given output stream
         */
        void printResult(std::ostream& os, const char* header = "") const;

        Rect rect() const;
        Rect realRect() const;

        void setInColumn(bool value);

        /**
         * Sets fragment type to mixed
         */
        void setMixed(bool value);

        /**
         * Sets pointer to parent root
         */
        void setParent(CRtfPage * page);

        /**
         * Sets fragment type
         */
        void setType(fragment_t type);

        /**
         * Returns pointer to string at @b pos
         * @throw std::out_of_range if wrong position given
         */
        CRtfString * stringAt(size_t pos);
        const CRtfString * stringAt(size_t pos) const;

        /**
         * Returns numbers of strings in fragment
         */
        size_t stringCount() const;

        /**
         * Returns content of fragment as string
         */
        std::string toString() const;

        /**
         * Returns fragment type
         */
        fragment_t type() const;

        /**
         * Exports fragment to CED document tree structure
         */
        void write(SectorInfo * sector, fragment_output_t type);

        /**
         * Exports fragment to CED picture
         */
        void writePicture(SectorInfo * sector, fragment_output_t type);

        /**
         * Exports fragment to CED tables
         */
        void writeTable(SectorInfo* SectorInfo, fragment_output_t type);

        /**
         * Exports fragment to CED text
         */
        void writeText(SectorInfo* SectorInfo);

        void FWritePicture(int NumberCurrentFragment, SectorInfo* SectorInfo, Bool OutPutType);

        RECT m_rect;
        RECT m_rectReal;
        uint32_t m_wUserNumber;
        uint32_t m_wUserNumberForFormattedMode;
        int32_t m_LeftOffsetFragmentFromVerticalColumn;
        int32_t m_RightOffsetFragmentFromVerticalColumn;
        uint16_t m_wOffsetFromPrevTextFragment;
        int16_t m_WidthVerticalColumn;
    public:
        static void setDrawCallback(RfrmtDrawFragmentFunction f);
    private:
        static RfrmtDrawFragmentFunction draw_func_;
    private:
        typedef std::vector<CRtfString*> StringList;
        typedef StringList::iterator StringIterator;
        typedef StringList::const_iterator StringIteratorConst;
    private:
        void adjustParagraph(int topMargin);
        void adjustStringIndents();
        void calcFragmentBorders();
        void calcMaxCharDistance();
        void calcStringEndsEqual();
        void calcStringEndsEqual(StringIterator begin, StringIterator end);
        bool checkAlignJustify(StringIterator begin, StringIterator end);
        bool checkStringForJustifyAlign(int ns);
        void checkOnceAgainImportancesFlagBeginParagraph();
        int columnWidth(SectorInfo* SectorInfo);
        void correctParagraphIndents(StringIterator begin, StringIterator end);
        void defineLineTransfer();
        bool determineAlign(StringIterator begin, StringIterator end);
        bool determineAlignCenter(StringIterator begin, StringIterator end);
        bool determineAlignJustify(StringIterator begin, StringIterator end);
        bool determineAlignLeft(StringIterator begin, StringIterator end, bool direct);
        bool determineAlignRight(StringIterator begin, StringIterator end);
        bool determineList(StringIterator begin, StringIterator end);
        bool determineMixedFragment(SectorInfo* SectorInfo);
        void Done();
        StringIterator findNextFragment(StringIterator begin);
        StringIterator findParagraph(StringIterator begin, StringIterator end);
        int fontSizePenalty(int fragment_count) const;
        bool hasBigSpaceBetweenWords(StringIteratorConst begin, StringIteratorConst end) const;
        bool hasFlagLeft(StringIterator begin, StringIterator end);
        bool hasFlagRight(StringIteratorConst begin, StringIteratorConst end) const;
        bool hasFlagStrongLeft(StringIteratorConst begin, StringIteratorConst end) const;
        void Init(SectorInfo* SectorInfo);
        void initFragment(SectorInfo* SectorInfo);
        void initFragmentFonts(int fragment_count);
        bool isMixed() const;
        bool isBigIndent(const CRtfString* str) const;
        bool isIndentsDiffer(const CRtfString* str, const CRtfString* prev) const;
        bool isDirectSpeech(const CRtfString* str, const CRtfString* prev) const;
        bool isShortEndString(const CRtfString* str) const;
        bool isParagraphIndent(const CRtfString* str, const CRtfString* prev) const;
        bool isParagraphStartsWithDigit(const CRtfString* str, const CRtfString* prev) const;
        int minStringLeftBorder() const;
        int minStringLeftBorder(StringIteratorConst begin, StringIteratorConst end) const;
        int maxStringRightBorder() const;
        int maxStringRightBorder(StringIteratorConst begin, StringIteratorConst end) const;
        void processingUseNoneMode();
        void processingOverlayed();
        void resetStringEndsEqual();
        void ReInit(SectorInfo* SectorInfo, StringIterator begin, StringIterator end);
        void setFirstLeftAndRightIndentOfParagraph();
        void setFlagBeginParagraphForLeftAlign(StringIterator begin, StringIterator end);
        void setFlagBeginParagraphForJustifyAlign(StringIterator begin, StringIterator end);
        void setFragmentAlignment(SectorInfo* SectorInfo);
        void updateStringPairAlignment(CRtfString * current, CRtfString * previous);
        void updateFirstStringPairAlignment();
    private:
        static bool checkCenterAlign(StringIteratorConst begin, StringIteratorConst end);
        static bool checkCenterAlign(int left, int right, int center, int justify, int total);
        static bool checkLeftAlign(StringIteratorConst begin, StringIteratorConst end);
        static bool checkLeftAlign(uint left, uint right, uint center, uint justify, uint total);
        static bool checkRightAlign(StringIteratorConst begin, StringIteratorConst end);
        static bool checkRightAlign(uint left, uint right, uint center, uint justify, uint total);
        static uint countEqualAlign(StringIteratorConst begin, StringIteratorConst end,
                format_align_t AlignType);
        static int countStringEndDots(StringIteratorConst begin, StringIteratorConst end);
        static bool hasFlagCarry(StringIteratorConst begin, StringIteratorConst end);
        static CEDParagraph * makeParagraph(SectorInfo * sector, int firstIndent,
                int leftIndent, int rightIndent, int marginTop, format_align_t align);
        static int minParagraphLeftIndent(StringIteratorConst begin, StringIteratorConst end);
        static void setLineTransfer(StringIterator begin, StringIterator end);
        static void setParagraphAlignment(StringIterator begin, StringIterator end,
                format_align_t AlignType);
    private:
        CRtfPage * parent_;
        StringList strings_;
        int left_border_;
        int right_border_;
        int max_char_distance_;
        uint16_t count_equal_left_;
        uint16_t count_equal_right_;
        uint16_t count_equal_justify_;
        uint16_t count_equal_center_;
        fragment_t type_;
        int local_fragment_left_;
        int local_fragment_right_;
        bool flag_carry_;
        bool flag_left_;
        bool flag_strong_left_;
        bool flag_right_;
        bool flag_big_space_;
        bool mixed_fragment_;
        bool in_column_;
        bool written_;
};
}

#endif /* CRTFFRAGMENT_H_ */
