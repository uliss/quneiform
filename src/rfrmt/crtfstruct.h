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

#ifndef CRTFSTRUCT_H_
#define CRTFSTRUCT_H_

class RtfPageElementCount
{
    public:
        uint16_t RtfSectors;
        uint16_t RtfTextFragments;
        uint16_t RtfFrameTextFragments;
        uint16_t RtfPictureFragments;
        uint16_t RtfTableFragments;
        uint16_t RtfStrings;
        uint16_t RtfWords;
        uint16_t RtfChars;
};

class KEGL
{
    public:
        int16_t OldKegl;
        int16_t NewKegl;
        int16_t Count;
};

class FONT
{
    public:
        char *family;
        char *name;
        int16_t Bold, Italic, Underline;
};

class FONT_COD
{
    public:
        const char *name;
        const char *family;
};

extern float Twips;
extern uint32_t FlagMode;
extern char UnRecogSymbol;
extern uint32_t gnLanguage;

#endif /* CRTFSTRUCT_H_ */
