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

#ifndef COMPONENTRECOGNIZER_H_
#define COMPONENTRECOGNIZER_H_

#include <string>
#include "lang_def.h"
#include "cttypes.h"
#include "globus.h"
#include "common/exception.h"

struct CCOM_comp;

namespace CIF
{

class CLA_EXPO ComponentRecognizer
{
    public:
        ComponentRecognizer();
        ~ComponentRecognizer();

        std::string ocrPath() const;
        void recognize(Handle ccom, int language);
        void setMaxComponentHeight(uint height);
        void setMaxComponentWidth(uint width);
        void setMinComponentHeight(uint height);
        void setMinComponentWidth(uint width);
        // if scaling > 0 => use long intervals
        void setMaxScale(uint scale);
        void setOcrPath(const std::string& ocrPath);
        typedef RuntimeExceptionImpl<ComponentRecognizer> Exception;
        static const int ALPHABET_SIZE = 256;
    private:
        uint max_comp_height_;
        uint max_comp_width_;
        uint min_comp_height_;
        uint min_comp_width_;
        uint max_scale_;
        std::string ocr_path_;
        char alphabet_[ALPHABET_SIZE];
    private:
        void alphabetInit(language_t language);
        void loadAlphabetTables(language_t language);
        uint maxComponentHeight() const;
        uint maxScale() const;
        uint maxComponentWidth() const;
        void recognizeComponents(Handle ccom);
        void recognizeComponent(CCOM_comp* pcomp);
};

}

#endif /* COMPONENTRECOGNIZER_H_ */
