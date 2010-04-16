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

#ifndef CRTFCHAR_H_
#define CRTFCHAR_H_

#include "cfcompat.h"
#include "lang_def.h"
#include "recdefs.h"

class CRtfChar
{
    public:
        struct
        {
                uchar m_bChar;
                uchar m_bProbability;
        } m_chrVersions[REC_MAX_VERS];//!!! Art

        RECT m_Idealrect;
        RECT m_Realrect;
        language_t m_blanguage;
        uchar m_bFlg_spell;
        uchar m_bFlg_spell_nocarrying;
        uchar m_bFlg_cup_drop;
        uint16_t m_wCountAlt;
        uint16_t m_wFontNumber;
        uint16_t m_wFontPointSize;
        uint16_t m_wIndex;
};

#endif /* CRTFCHAR_H_ */
