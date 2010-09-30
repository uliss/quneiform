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
#include "blockelement.h"
#include "common/rect.h"
#include "common/color.h"
#include "common/align.h"

namespace cf
{

class CEDColumn;
class CEDFrame;

class CLA_EXPO CEDSection: public BlockElement
{
    public:
        CEDSection();

        void addColInfo(int width, int space);

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

        void exportElement(CEDExporter& exp);

        /**
         * Returns if line between columns should be drawn
         * @see setLineBetweenColumns()
         */
        bool lineBetweenColumns() const;

        /**
         * Returns section breaking
         * @see setSectionBreak()
         */
        bool sectionBreak() const;

        /**
         * Sets footer y-offset
         */
        void setFooterY(int value);

        /**
         * Sets header y-offset
         */
        void setHeaderY(int value);

        /**
         * Sets drawing line between columns
         * @see lineBetweenColumns()
         */
        void setLineBetweenColumns(bool value);

        /**
         * Sets page orientation
         * 0 - portrait, 1 - landscape
         */
        void setOrientation(char value);

        /**
         * Sets section break
         * if @b false - section starts at current page
         * if @b true - section starts at new page
         * @see sectionBreak()
         */
        void setSectionBreak(bool value);

        CEDParagraph * createParagraph(BlockElement * container, align_t align, const Rect& indent,
                int UserNum, int FlagBorder, EDSIZE interval, const Rect& layout,
                const Color& color, const Color& bgrnd, int spaceBetweenLines);
    private:
        int header_y_;
        int footer_y_;
        bool line_between_columns_;
        bool section_break_;
        char orientation_;
        std::vector<int> col_wd_;
        std::vector<int> col_space_;

};

}

#endif /* CEDSECTION_H_ */
