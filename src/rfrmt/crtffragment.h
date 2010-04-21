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
#include "cfcompat.h"

class RtfSectorInfo;

namespace CIF
{

class CRtfPage;
class CRtfString;

class CRtfFragment
{
    public:
        CRtfFragment();
        ~CRtfFragment();

        CRtfPage* pRtfParent;
        CRtfString* GetNextString();
        void InitFragment(RtfSectorInfo* SectorInfo);
        void SetFragmentAlignment(RtfSectorInfo* SectorInfo);
        void new_paragraph(Bool OutPutType);
        Bool FWriteText(int16_t NumberCurrentFragment, RtfSectorInfo* SectorInfo, Bool OutPutType);
        Bool FWriteTable(int16_t NumberCurrentFragment, RtfSectorInfo* SectorInfo, Bool OutPutType);
        Bool FWritePicture(int16_t NumberCurrentFragment, RtfSectorInfo* SectorInfo,
                Bool OutPutType);

        Bool ProcessingUseNoneMode(void);
        void Init(RtfSectorInfo* SectorInfo);
        void ReInit(RtfSectorInfo* SectorInfo, int beg, int end);
        Bool ProcessingOverLayedFragment(RtfSectorInfo* SectorInfo);
        Bool DeterminationOfCentreJustification(int beg, int end);
        Bool DeterminationOfRightJustification(int beg, int end);
        Bool DeterminationOfListType(int beg, int end);
        void Done(void);

        Bool DeterminationOfMixedFragment(RtfSectorInfo* SectorInfo);
        void GetNextFragmentBegEnd(int32_t* beg, int32_t* end, Bool* Flag);

        Bool DeterminationOfLeftRightJustification(int beg, int end);
        Bool CheckLeftRightJustification(int beg, int end);
        void SetFlagBeginParagraphForLeftRightJustification(int beg, int end);
        Bool CheckStringForLeftRightJustification(int ns);

        Bool DeterminationOfLeftJustification(int beg, int end, Bool direct);
        Bool CheckLeftJustification(int beg, int end);
        void SetFlagBeginParagraphForLeftJustification(int beg, int end);
        Bool CheckStringForLeftJustification(int ns);

        void CheckOnceAgainImportancesFlagBeginParagraph(void);
        void DefineLineTransfer(void);
        int GetCountLine(int beg);
        void SetLineTransfer(int beg, int end);
        void SetFirstLeftAndRightIndentOfParagraph(void);
        void CorrectIndents(int beg, int end);
        void SetParagraphAlignment(int beg, int end, int AlignType);
        void CalculationLengthAndCount(CRtfString* pRtfString, int32_t* CountChars,
                int32_t* LengthChars);
        void GetCountEqual(int beg, int end, uint16_t* Count, int AlignType);
        Bool GetFlagCarry(int beg, int end);
        Bool GetFlagLeft(int beg, int end);
        Bool GetFlagStrongLeft(int beg, int end);
        Bool GetFlagRight(int beg, int end);
        Bool GetFlagBigSpace(int beg, int end);
        Bool CheckNumber(uchar FirstChar);

        void PrintTheResult(const char*);

        std::vector<CRtfString*> m_arStrings;

        uint16_t m_wStringsCount;
        RECT m_rect;
        RECT m_rectReal;
        RECT m_rectFree;
        uint16_t m_wType;
        uint32_t m_wUserNumber;
        uint32_t m_wUserNumberForFormattedMode;
        int32_t m_LeftOffsetFragmentFromVerticalColumn;
        int32_t m_RightOffsetFragmentFromVerticalColumn;
        uint16_t m_wOffsetFromPrevTextFragment;

        uint16_t m_wBegStr;
        uint16_t m_wEndStr;

        Bool m_bFlagObjectInColumn;
        Bool m_bFlagUsed;
        Bool m_bOutPutType;
        Bool m_bflag_new_column;
        Bool m_bflag_new_parag;
        Bool m_bflag_new_str;
        Bool m_bflag_new_line;
        Bool m_bflag_change;
        Bool m_bflag_LineTransfer;
        uint16_t m_wprev_font_name;
        uint16_t m_wprev_Underline;
        uint16_t m_wprev_Bold;
        uint16_t m_wprev_Italic;
        uint16_t m_wprev_lang;
        uint16_t m_wprev_font_size;
        uint16_t m_wvid_parag;

        uint16_t m_CountLeftEqual;
        uint16_t m_CountRightEqual;
        uint16_t m_CountLeftRightEqual;
        uint16_t m_CountCentreEqual;

        int16_t m_l_fragment;
        int16_t m_r_fragment;
        int16_t m_l_fragmentLocal;
        int16_t m_r_fragmentLocal;

        int16_t m_LengthStringInTwips;
        int16_t m_WidthVerticalColumn;

        int16_t m_posx;
        int16_t m_posy;
        int16_t m_absw;
        int16_t m_absh;

        int16_t m_tmp_fi;
        int16_t m_li;
        int16_t m_ri;
        int16_t m_fi;
        int16_t m_sb;
        int16_t m_sa;
        int16_t m_sl;

        int16_t m_max_dist;
        uchar m_Attr;
        uchar m_FlagCarry;
        uchar m_FlagLeft;
        uchar m_FlagStrongLeft;
        uchar m_FlagRight;
        uchar m_FlagBigSpace;
        uint32_t m_Flag;
};
}

#endif /* CRTFFRAGMENT_H_ */
