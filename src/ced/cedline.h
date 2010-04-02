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

namespace CIF
{

class FUN_EXPO__ CEDLine
{
    public:
        CEDLine();

        CEDChar* charAt(int num);

        /**
         * Returns number of chars in line
         */
        int charCount();

        /**
         * Returns pointer to current symbol
         * @see setCurrentChar()
         */
        CEDChar * currentChar();

        /**
         * Returns true if line have hard break
         * @see setHardBreak()
         */
        bool hardBreak() const;

        /**
         * Inserts new symbol after current one. new symbol becomes current
         * @return pointer to new symbol
         */
        CEDChar * insertChar();

        /**
         * Returns number of line from start of file
         */
        int internalNumber() const;

        /**
         * Returns number of parent in a file
         * @see setParentNumber()
         */
        int parentNumber() const;

        /**
         * Sets current char
         * @see currentChar()
         */
        void setCurrentChar(CEDChar * chr);

        /**
         * Sets new value of current symbol
         */
        CEDChar * setCurrentChar(int number);

        /**
         * Sets line hard breaking
         * @see hardBreak()
         */
        void setHardBreak(bool value);

        /**
         * Sets parent number
         * @see parentNumber()
         */
        void setParentNumber(int number);

        int GetNumOfCurChar(); //return current symbol

        CEDChar * NextChar(Bool32 _goThroughLines); //returns next symbol, 0 if last
        CEDChar * PrevChar(Bool32 _goThroughLines); //returns previous symbol, 0 if first
        //if _goThroughSect = TRUE, then we consider last symbol in file, otherwise in line

        CEDChar * chars; //connected list of symbols

        CEDLine * prev, *next; //pointer to neibor elements in connected list
        friend class CEDParagraph;
        friend class CEDPage;
        int char_number_;
    public:
        int defChrFontHeight;
    private:
        bool hard_break_;
        CEDChar * current_char_; //current symbol
        int internal_number_;
        int parent_number_;
};

}

#endif /* CEDLINE_H_ */
