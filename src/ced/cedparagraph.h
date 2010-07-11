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

#ifndef CEDPARAGRAPH_H_
#define CEDPARAGRAPH_H_

#include <vector>
#include <boost/shared_ptr.hpp>

#include "globus.h"
#include "ced_struct.h"
#include "blockelement.h"
#include "common/color.h"
#include "common/align.h"

namespace CIF
{

class CLA_EXPO CEDParagraph: public BlockElement
{
    public:
        CEDParagraph();
        ~CEDParagraph();

        int type; // Type paragraph
        int userNumber; // ID number, the user at the stage of fragmentation
        int border; //  frame around abzattsa
        EDSIZE interval; //  cx-upper indentation, cy-bottom

        char spcBtwLnsMult;
        char keep;

        int leftBrdrWidth;
        int rightBrdrWidth;
        int topBrdrWidth;
        int bottomBrdrWidth;
        int brdrBtw;

        void * descriptor; // Pointer to advanced descriptor of special structures

        /**
         * Adds line to the end of paragraph
         */
        void addLine(CEDLine * line);

        /**
         * Returns paragraph alignment
         */
        align_t align() const;

        void exportElement(CEDExporter& exp);

        /**
         * Returns paragraph indents
         * @see setIndent()
         */
        const Rect& indent() const;

        /**
         * Returns pointer to line at given position
         * @see lineCount()
         */
        CEDLine * lineAt(size_t pos);

        /**
         * Returns number of lines
         * @see lineAt()
         */
        size_t lineCount() const;

        /**
         * Returns space between lines
         * @see setLineSpace()
         */
        int lineSpace() const;

        /**
         * Sets paragraph alignment
         * @see align()
         */
        void setAlign(align_t align);

        /**
         * Sets paragraph indents
         * @see indent()
         */
        void setIndent(const Rect& ind);

        /**
         * Sets line space
         * @see lineSpace()
         */
        void setLineSpace(int value);
    private:
        align_t align_;
        Rect indent_;
        int internal_number_;
        int line_space_;
};

}

#endif /* CEDPARAGRAPH_H_ */
