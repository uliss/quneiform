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

#ifndef CEDFRAME_H_
#define CEDFRAME_H_

#include "globus.h"
#include "blockelement.h"

namespace CIF
{

class CLA_EXPO CEDFrame: public BlockElement
{
    public:
        enum vposition_t
        {
            /** y position relating top page border */
            VPOS_PAGE = 0,
            /** y position relating top page margin */
            VPOS_MARGIN = 1,
            /** y position relating */
            VPOS_PARAGRAPH = 2
        };

        enum hposition_t
        {
            /** x position relating left page border */
            HPOS_PAGE = (0 << 4),
            /** x position relating left page margin */
            HPOS_MARGIN = (1 << 4),
            /** x position relating column left border */
            HPOS_COLUMN = (2 << 4)
        };

        CEDFrame(BlockElement * parent = NULL, hposition_t hpos = HPOS_COLUMN, vposition_t vpos =
                VPOS_PARAGRAPH);

        void exportElement(CEDExporter& exp);

        int dxfrtextx;
        int dxfrtexty;

        void setBorderSpace(int borderSpace);

        /**
         * Sets horizontal frame snap position
         */
        void setHPosition(hposition_t pos);

        /**
         * Sets vertical frame snap position
         */
        void setVPosition(vposition_t pos);
    private:
        int border_space_;
        hposition_t hpos_;
        vposition_t vpos_;
};

}

#endif /* CEDFRAME_H_ */
