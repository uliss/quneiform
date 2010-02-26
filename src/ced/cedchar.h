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

#ifndef CEDCHAR_H_
#define CEDCHAR_H_

#include "globus.h"
#include "ced_struct.h"

class CEDLine;
class CEDPage;

class FUN_EXPO__ CEDChar
{
    public:
        CEDChar();
        ~CEDChar();
        CEDChar * prev, *next; //pointer to neibor elements in connected list
    protected:
        int parentNumber; //number of parent in a file
        friend class CEDLine;
        friend class CEDPage;
        friend void FormattedTR(const text_ref* pt);
        friend void StripLines();
    public:
        edRect layout; //layout of symbol in input image (in pixel)
        int fontHeight, fontAttribs; //font parameters
        int fontNum;
        int fontLang;
        int foregroundColor;
        int backgroundColor;
        letterEx * alternatives; //array of alternatives
        int numOfAltern;
        char * extData; //data to be written in file after header
        int extDataLen; //their length
};

#endif /* CEDCHAR_H_ */
