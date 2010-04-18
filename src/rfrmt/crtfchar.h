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
#include "crtfstruct.h"
#include "common/rect.h"

namespace CIF
{

class CRtfChar
{
    public:
        /**
         * Returns font number
         */
        font_number font() const;

        /**
         * Returns font size
         */
        short fontSize() const;

        /**
         * Returns ideal char bounding rectangle
         */
        Rect idealRect() const;

        /**
         * Returns character language
         */
        language_t language() const;

        /**
         * Returns bounding rectangle
         */
        Rect realRect() const;

        /**
         * Sets font number
         */
        void setFont(font_number fontNumber);

        /**
         * Sets font size
         */
        void setFontSize(short size);

        /**
         * Sets ideal rectangle
         * @param rect
         */
        void setIdealRect(const Rect& rect);

        /**
         * Sets character language
         * @param lang - new language
         */
        void setLanguage(language_t lang);

        /**
         * Sets real rectangle
         */
        void setRealRect(const Rect& rect);

        struct
        {
                uchar m_bChar;
                uchar m_bProbability;
        } m_chrVersions[REC_MAX_VERS];//!!! Art

        uchar m_bFlg_spell;
        uchar m_bFlg_spell_nocarrying;
        uchar m_bFlg_cup_drop;
        uint16_t m_wCountAlt;
    private:
        language_t language_;
        font_number font_number_;
        Rect ideal_rect_;
        Rect real_rect_;
        uint16_t m_wFontPointSize;
};

}

#endif /* CRTFCHAR_H_ */
