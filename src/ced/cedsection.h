/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include "cedint.h"

namespace CIF {

class CEDPage;

class CEDSection
{
public:
    int numberOfColumns; // The number of columns in the section (n pieces)
    EDRECT borders; // padding from the edge of paper
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
    char * extData; // Data cat. will be recorded in the file after the title;
    int extDataLen; // Data size.

    CEDParagraph * CreateColumn();
    CEDParagraph * CreateFrame(CEDParagraph* hObject, edBox rect, char position = -1,
            int borderSpace = -1, int dxfrtextx = -1, int dxfrtexty = -1);
    CEDParagraph * CreateParagraph(CEDParagraph * hObject, int align, const CIF::Rect& indent,
            int UserNum, int FlagBorder, EDSIZE interval, edBox layout, int color, int shading,
            int spaceBetweenLines, char spcBtwLnsMult, char keep);

    CEDParagraph * CreateTable(CEDParagraph * hObject);
    //  CEDParagraph * ReCreateTable(CEDParagraph * hTable,EDSIZE sz, int * cx,int * cy, int * table,
    //                 Bool32 * bHorShow,Bool32 * bVerShow);
    CEDParagraph * CreateTableRow(CEDParagraph * hTable, int left, int rowHeight, int leftBrdrType,
            int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth, int topBrdrType,
            int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth, int gaph, int position,
            Bool32 header);

    CEDParagraph * CreateCell(CEDParagraph* hRow, int cellX, int merging, int vertTextAlign,
            int leftBrdrType, int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth,
            int topBrdrType, int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth,
            EDBOX layout, int shading, int color);

    CEDParagraph * GetColumn(int _num);

    CEDSection();
    ~CEDSection();

    CEDParagraph * InsertParagraph(Bool32 AfterCurrent = TRUE); // Inserts new paragraph after the current. Current becomes a new paragraph
    // Returns pointer to the newly created bullet
    CEDParagraph * SetCurParagraph(CEDParagraph* _para);//sets new value of current paragraph
    CEDParagraph * SetCurParagraph(int _number);//sets new value of current paragraph

    CEDParagraph * GetCurParagraph(); //returns current paragraph
    int GetNumOfCurParagraph(); //returns current paragraph

    CEDParagraph * NextParagraph(Bool32 _goThroughSect); //returns next paragraph, 0 if last
    CEDParagraph * PrevParagraph(Bool32 _goThroughSect); //returns previous paragraph, 0 if first
    //If _goThroughSect = TRUE, then we consider boundary paragraphs in file, othrwise in section

    CEDParagraph * paragraphs; //connected list of paragraphs
    CEDParagraph * columnsBeg, *columnsEnd, *columnsCur;

    CEDParagraph * curPara;//current paragraph
    CEDSection * prev, *next; //pointer to neibor elements in connected list
    int internalNumber; //number of paragraph from start of the file

    friend class CEDPage;
};

}

#endif /* CEDSECTION_H_ */
