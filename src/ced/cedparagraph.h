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

#include "globus.h"
#include "ced_struct.h"

namespace CIF {

class FUN_EXPO__ CEDParagraph
{
    public:
        int type; // Type paragraph
        int alignment; // Alignment abzattsa
        EDRECT indent; // Indentation: left = left, right = width, top = red.line (in twip)
        int userNumber; // ID number, the user at the stage of fragmentation
        int border; //  frame around abzattsa
        EDSIZE interval; //  cx-upper indentation, cy-bottom

        edBox layout; //  Location paragraph on page
        int color;
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

        char * extData; // Data cat. will be recorded in the file after the title;
        int extDataLen; // Its size

        CEDParagraph();
        ~CEDParagraph();

        CEDLine* GetLine(int _num);
        int GetCountLine();

        CEDParagraph* GetFirstObject();
        CEDParagraph* GetNextObject();
        CEDParagraph* GetCell(int cell);
        CEDParagraph* GetRow(int row);
        CEDParagraph* GetLogicalCell(int number);
        int GetCountLogicalCell();
        void CreateTableOfCells();
        CEDLine * InsertLine(); //inserts new line after current one. new line becomes current
        //returns pointer to new line
        //CEDLine * DeleteLine(Bool32 _deleteSubItems); //deletes current line. previous one becomes current
        //return it
        //_deleteSubItems - either delete all daughter elements or attach it to previous object

        void SetCurLine(CEDLine* _line);//sets new value of current line
        CEDLine * SetCurLine(int _number);//sets new value of current line

        CEDLine * GetCurLine(); //returns current line
        int GetNumOfCurLine(); //returns current line

        CEDLine * NextLine(Bool32 _goThroughPara); //returns next line, 0 if last
        CEDLine * PrevLine(Bool32 _goThroughPara); //returns previous line, 0 if first
        //If _goThroughSect = TRUE, then we consider boundary lines in file, otherwise in paragraph

        CEDLine * lines; //connected list of lines
        int numOfLines;
        CEDLine * curLine; //current line
        CEDParagraph * prev, *next; //pointer to neibor elements in connected list
        int internalNumber; //number of paragraph from start of file
        int parentNumber; //number of parent in file
        friend class CEDSection;
        friend class CEDPage;
};

}

#endif /* CEDPARAGRAPH_H_ */
