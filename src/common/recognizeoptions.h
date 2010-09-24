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

#ifndef RECOGNIZEOPTIONS_H_
#define RECOGNIZEOPTIONS_H_

#include <iostream>
#include "lang_def.h"
#include "globus.h"

namespace CIF {

class CLA_EXPO RecognizeOptions
{
    public:
        RecognizeOptions();

        bool autoRotate() const;
        bool dotMatrix() const;
        bool fax() const;

        /**
         * Sets recognition language
         */
        language_t language() const;
        bool oneColumn() const;
        bool spellCorection() const;

        void setAutoRotate(bool value);
        void setFax(bool value);
        void setDotMatrix(bool value);
        void setLanguage(language_t language);

        /**
         * Sets one column layout
         */
        void setOneColumn(bool value);

        void setSpellCorrection(bool value);
    private:
        language_t language_;
        bool auto_rotate_;
        bool dot_matrix_;
        bool do_spell_correction_;
        bool fax100_;
        bool one_column_;
};

std::ostream& operator<<(std::ostream& os, const RecognizeOptions& opts);

}

#endif /* RECOGNIZEOPTIONS_H_ */
