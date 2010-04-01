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

#ifndef CEDLINE_H_
#define CEDLINE_H_

#include "globus.h"
#include "ced_struct.h"

namespace CIF {

class FUN_EXPO__ CEDLine
{
    public:
        CEDLine();
        ~CEDLine();

        CEDChar* GetChar(int _num);
        int GetCountChar();

        /**
         * Returns true if line have hard break
         */
        bool hardBreak() const;

        /**
         * Sets line hard breaking
         */
        void setHardBreak(bool value);

        CEDChar * InsertChar(); //inserts new symbol after current one. new symbol becomes current
        //returns pointer to new symbol
        //  CEDChar * DeleteChar(Bool32 _deleteSubItems);   //deletes current symbol. previous one becomes current
        //return it
        //_deleteSubItems - either delete all daughter elements or attach it to previous object
        void SetCurChar(CEDChar* _char);//set new value of current symbol
        CEDChar * SetCurChar(int _number);//set new value of current symbol

        CEDChar * GetCurChar(); //return current symbol
        int GetNumOfCurChar(); //return current symbol

        CEDChar * NextChar(Bool32 _goThroughLines); //returns next symbol, 0 if last
        CEDChar * PrevChar(Bool32 _goThroughLines); //returns previous symbol, 0 if first
        //if _goThroughSect = TRUE, then we consider last symbol in file, otherwise in line

        CEDChar * chars; //connected list of symbols
        int numOfChars;

        CEDChar * curChar; //current symbol
        CEDLine * prev, *next; //pointer to neibor elements in connected list
        int internalNumber; //number of line from start of file
        int parentNumber; //number of parent in a file
        friend class CEDParagraph;
        friend class CEDPage;
    public:
        int defChrFontHeight;
    private:
        Bool32 hard_break_;
};

}

#endif /* CEDLINE_H_ */
