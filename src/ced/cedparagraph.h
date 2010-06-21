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
#include "common/color.h"
#include "common/align.h"

namespace CIF
{

class CLA_EXPO CEDParagraph
{
    public:
        int type; // Type paragraph
        Rect indent; // Indentation: left = left, right = width, top = red.line (in twip)
        int userNumber; // ID number, the user at the stage of fragmentation
        int border; //  frame around abzattsa
        EDSIZE interval; //  cx-upper indentation, cy-bottom

        edBox layout; //  Location paragraph on page
        int shading;
        int spaceBetweenLines;
        char spcBtwLnsMult;
        char keep;

        int leftBrdrType;
        int rightBrdrType;
        int topBrdrType;
        int bottomBrdrType;
        int leftBrdrWidth;
        int rightBrdrWidth;
        int topBrdrWidth;
        int bottomBrdrWidth;
        int brdrBtw;

        void * descriptor; // Pointer to advanced descriptor of special structures

        /**
         * Returns paragraph alignment
         */
        align_t align() const;

        /**
         * Returns paragraph color
         */
        const Color& color() const;

        /**
         * Returns pointer to line at given position
         */
        CEDLine * lineAt(size_t pos);

        /**
         * Returns number of lines
         */
        size_t lineCount() const;

        /**
         * Sets paragraph alignment
         */
        void setAlign(align_t align);

        /**
         * Sets paragraph foreground color
         * @see color()
         */
        void setColor(const Color& c);

        CEDParagraph();
        ~CEDParagraph();

        CEDParagraph* GetFirstObject();
        CEDParagraph* GetNextObject();
        CEDParagraph* GetCell(int cell);
        CEDParagraph* GetRow(int row);
        CEDParagraph* GetLogicalCell(int number);
        int GetCountLogicalCell();
        void CreateTableOfCells();
        CEDLine * insertLine(CEDLine * line);
        CEDLine * insertLine(); //inserts new line after current one. new line becomes current
        //returns pointer to new line

        CEDParagraph * prev, *next; //pointer to neibor elements in connected list
        int internalNumber; //number of paragraph from start of file
        int parentNumber; //number of parent in file
        friend class CEDSection;
        friend class CEDPage;
    private:
        typedef boost::shared_ptr<CEDLine> LinePtr;
        typedef std::vector<LinePtr> LineList;
        LineList lines; //connected list of lines
        Color color_;
        align_t align_;
};

}

#endif /* CEDPARAGRAPH_H_ */
