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

#ifndef CEDSECTION_H_
#define CEDSECTION_H_

#include "globus.h"
#include "ced_struct.h"
#include "blockelement.h"
#include "common/rect.h"
#include "common/color.h"
#include "common/align.h"

namespace CIF
{

class CEDColumn;

class CLA_EXPO CEDSection: public BlockElement
{
    public:
        /**
         * Adds vertical column to page section
         */
        void addColumn(CEDColumn * col);

        /**
         * Returns  pointer to column by given position
         */
        CEDColumn * columnAt(size_t pos);

        /**
         * Returns number of columns in section
         */
        size_t columnCount() const;
        CEDColumn * createColumn();

        Rect borders; // padding from the edge of paper
        int colInterval;
        char sectionBreak;
        int width;
        int height;
        char orientation;
        int headerY;
        int footerY;
        int numSnakeCols;
        Bool lineBetCol;
        EDCOL *colInfo;

        CEDParagraph * CreateFrame(CEDParagraph* hObject, edBox rect, char position = -1,
                int borderSpace = -1, int dxfrtextx = -1, int dxfrtexty = -1);
        CEDParagraph * CreateParagraph(CEDParagraph * hObject, align_t align, const Rect& indent,
                int UserNum, int FlagBorder, EDSIZE interval, edBox layout, const Color& color,
                const Color& bgrnd, int spaceBetweenLines, char spcBtwLnsMult, char keep);

        CEDSection();
        ~CEDSection();

        CEDParagraph * columnsEnd;
        friend class CEDPage;
    private:

};

}

#endif /* CEDSECTION_H_ */
