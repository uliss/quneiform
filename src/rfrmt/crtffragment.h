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
#include "cfcompat.h"
#include "font.h"

class RtfSectorInfo;

namespace CIF
{

class CRtfPage;
class CRtfString;

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
         * Returns pointer to first string in fragment
         * @throw std::out_of_range if fragment is empty
         */
        CRtfString * firstString();
        const CRtfString * firstString() const;

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

        /**
         * Sets pointer to parent root
         */
        void setParent(CRtfPage * page);

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

        Bool FWriteText(int NumberCurrentFragment, RtfSectorInfo* SectorInfo, Bool OutPutType);
        void FWritePicture(int NumberCurrentFragment, RtfSectorInfo* SectorInfo, Bool OutPutType);

        void ReInit(RtfSectorInfo* SectorInfo, int beg, int end);
        Bool DeterminationOfListType(int beg, int end);
        void Done();

        void GetNextFragmentBegEnd(int32_t* beg, int32_t* end, Bool* Flag);

        Bool DeterminationOfLeftRightJustification(int beg, int end);
        Bool CheckLeftRightJustification(int beg, int end);
        void SetFlagBeginParagraphForLeftRightJustification(int beg, int end);
        Bool CheckStringForLeftRightJustification(int ns);
        Bool CheckStringForLeftJustification(int ns);

        void CheckOnceAgainImportancesFlagBeginParagraph(void);
        void SetFirstLeftAndRightIndentOfParagraph(void);
        Bool GetFlagStrongLeft(int beg, int end);
        Bool GetFlagRight(int beg, int end);

        RECT m_rect;
        RECT m_rectReal;
        RECT m_rectFree;
        uint16_t m_wType;
        uint32_t m_wUserNumber;
        uint32_t m_wUserNumberForFormattedMode;
        int32_t m_LeftOffsetFragmentFromVerticalColumn;
        int32_t m_RightOffsetFragmentFromVerticalColumn;
        uint16_t m_wOffsetFromPrevTextFragment;

        Bool m_bFlagObjectInColumn;
        Bool m_bFlagUsed;
        Bool m_bOutPutType;
        font_number m_wprev_font_name;
        uint16_t m_wprev_lang;
        uint16_t m_wprev_font_size;
        uint16_t m_wvid_parag;

        int16_t m_l_fragmentLocal;
        int16_t m_r_fragmentLocal;

        int16_t m_WidthVerticalColumn;

        int16_t m_li;
        int16_t m_ri;
        int16_t m_fi;
        int16_t m_sb;
        int16_t m_sa;
        int16_t m_sl;

        uchar m_Attr;
        uchar m_FlagLeft;
        uchar m_FlagStrongLeft;
        uchar m_FlagRight;
        uchar m_FlagBigSpace;
        uint32_t m_Flag;
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
        void correctParagraphIndents(StringIterator begin, StringIterator end);
        void defineLineTransfer();
        bool determinationOfCentreJustification(StringIterator begin, StringIterator end);
        bool determinationOfLeftJustification(StringIterator begin, StringIterator end, bool direct);
        bool determinationOfRightJustification(StringIterator begin, StringIterator end);
        bool determinationOfMixedFragment(RtfSectorInfo* SectorInfo);
        StringIteratorConst findNextFragment(StringIteratorConst begin) const;
        StringIterator findParagraph(StringIterator begin, StringIterator end);
        int fontSizePenalty(int fragment_count) const;
        bool hasBigSpaceBetweenWords(StringIteratorConst begin, StringIteratorConst end) const;
        bool hasFlagCarry(StringIteratorConst begin, StringIteratorConst end) const;
        bool hasFlagLeft(StringIterator begin, StringIterator end);
        void Init(RtfSectorInfo* SectorInfo);
        void initFragment(RtfSectorInfo* SectorInfo);
        void initFragmentFonts(int fragment_count);
        int minParagraphLeftIndent(StringIteratorConst begin, StringIteratorConst end) const;
        int minStringLeftBorder() const;
        int maxStringRightBorder() const;
        void processingUseNoneMode();
        void processingOverlayed();
        void setFlagBeginParagraphForLeftJustification(StringIterator begin, StringIterator end);
        void setFragmentAlignment(RtfSectorInfo* SectorInfo);
        void setLineTransfer(StringIterator begin, StringIterator end);
        void setParagraphAlignment(StringIterator begin, StringIterator end,
                format_align_t AlignType);
        void updateStringPairAlignment(CRtfString * current, CRtfString * previous);
        void updateFirstStringPairAlignment();
    private:
        static bool checkLeftAlign(StringIteratorConst begin, StringIteratorConst end);
        static bool checkLeftAlign(int left, int right, int center, int justify, int total);
        static bool checkRightAlign(StringIteratorConst begin, StringIteratorConst end);
        static bool checkRightAlign(int left, int right, int center, int justify, int total);
        static int countEqualAlign(StringIteratorConst begin, StringIteratorConst end,
                format_align_t AlignType);
        static int countStringEndDots(StringIteratorConst begin, StringIteratorConst end);
    private:
        CRtfPage * parent_;
        StringList strings_;
        int left_border_;
        int right_border_;
        int max_char_distance_;
        uint16_t m_CountLeftEqual;
        uint16_t m_CountRightEqual;
        uint16_t m_CountLeftRightEqual;
        uint16_t m_CountCentreEqual;
        bool flag_carry_;
};
}

#endif /* CRTFFRAGMENT_H_ */
