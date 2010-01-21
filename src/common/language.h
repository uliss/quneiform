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

#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#include "lang_def.h"
#include <string>
#include <map>
#include <iostream>

namespace CIF
{

typedef std::pair<std::string, std::string> LanguageCodeName;
typedef std::map<language_t, LanguageCodeName> LanguageMap;

class Language
{
    public:
        Language(language_t language);
        language_t get() const;
        std::string isoCode() const;
        std::string isoName() const;
    public:
        static LanguageMap languages();
    private:
        language_t language_;
};

inline language_t Language::get() const {
    return language_;
}

std::ostream& operator<<(std::ostream& os, const Language& language);

}

#endif /* LANGUAGE_H_ */
