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

#ifndef CRTFPAGE_H_
#define CRTFPAGE_H_

#include <vector>
#include <boost/function.hpp>

#include "font.h"
#include "common/size.h"
#include "cfcompat.h"
#include "crtfstruct.h"

namespace CIF
{
class CEDPage;
class CRtfFragment;
class CRtfSector;
class CRtfPage;
class FormatOptions;

typedef boost::function<void(const CRtfPage*)> RfrmtDrawPageFunction;

class CRtfPage
{
    public:
        CRtfPage();
        ~CRtfPage();

        /**
         * Removes fragments
         */
        void clearFragments();

        /**
         * Removes sectors
         */
        void clearSectors();

        /**
         * Draws page layout via callback
         */
        void drawLayout() const;

        /**
         * Sets monospace font name
         */
        void setFontMonospace(const std::string& name);

        /**
         * Sets sans serif font name
         */
        void setFontSans(const std::string& name);

        /**
         * Sets serif font name
         */
        void setFontSerif(const std::string& name);

        /**
         * Sets formatting options
         */
        void setFormatOptions(const FormatOptions& opts);

        /**
         * Sets image name
         */
        void setImageName(const std::string& name);

        /**
         * Sets symbol that inserted instead of unrecognized characters
         */
        void setUnrecognizedChar(char ch);

        /**
         * Exports CRtfPage to ced
         * @return pointer to CED page
         * @note caller should free result
         */
        CEDPage * Write();

        void setFragmentsInColumn(const CRtfFragment * cur_fragm);

        CRtfFragment* GetNextFragment();
        Bool ReadInternalFile(FILE *FileNameIn);
        void CloseOutputFile(void);
        void SetTwips(void);
        Bool FindPageTree(FILE *FileNameIn, const char* FileNameOut);
        void ReCalcPageWidthAndHeight();
        Bool WriteHeaderRtf(void);
        void CorrectKegl(void);
        void ChangeKegl(void);
        void AddNewKegl(int16_t OldKegl, int16_t NewKegl);
        int16_t GetNewKegl(int16_t OldKegl);
        int16_t GetMinKegl(int16_t OldKegl);
        void AddPictures(void);
        void AddTables(void);
        void AddLines(void);
        void SortUserNumber(void);
        int16_t GetFlagAndNumberFragment(uchar* FragmentType, int16_t* CurrentSectorNumber);
        void WriteSectorsHeader(int16_t i);
        void ToPlacePicturesAndTables(void);
        uint16_t GetFreeSpaceBetweenSectors(CRtfSector* pRtfSector, CRtfSector* pRtfNextSector);
        void SetPaperSize(int32_t LeftPos, int32_t RightPos, int32_t TopPos, int32_t BottomPos,
                Size& size, int32_t* MargL, int32_t* MargR, int32_t* MargT, int32_t* MargB);

        typedef std::vector<CRtfFragment*> FragmentList;
        FragmentList m_arFragments;
        typedef std::vector<CRtfSector*> SectorList;
        SectorList m_arSectors;

        RtfPageElementCount Count;
        uint16_t m_wDpi;
        float m_fTwips;
        RECT m_rect;
        RECT m_rectReal;
        int32_t MargL;
        int32_t MargR;
        int32_t MargT;
        int32_t MargB;

        int32_t InitMargL;
        int32_t InitMargR;
        int32_t InitMargT;
        int32_t InitMargB;

        int m_nCurSectorNumber;
    private:
        void initCedPage();
        Bool writeUsingNone();
        Bool writeUsingFrames();
        Bool writeUsingFramesAndColumns();
    private:
        std::string image_name_;
        std::string font_sans_;
        std::string font_serif_;
        std::string font_monospace_;
        char unrecognized_char_;
        language_t language_;
        Size page_size_;
        CEDPage * ced_page_;
        bool bad_column_;
        std::vector<KEGL> arKegls;
    public:
        static void setDrawCallback(RfrmtDrawPageFunction f);
    private:
        static RfrmtDrawPageFunction draw_func_;
};
}

#endif /* CRTFPAGE_H_ */
