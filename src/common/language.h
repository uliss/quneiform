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

#include <string>
#include <map>
#include <list>
#include <iostream>

#include "lang_def.h"
#include "globus.h"

namespace cf
{

typedef std::list<language_t> LanguageList;

class CLA_EXPO Language
{
    public:
        Language(language_t language);

        /**
          * Returns encoding used for language in cuneiform
          */
        std::string encoding() const;

        /**
         * Returns language type
         */
        language_t get() const;

        /**
         * Returns ISO 639-1 language code (2-character)
         * @return empty string if unknown language
         */
        std::string isoCode2() const;

        /**
         * Returns ISO 639-2 language code (3-character)
         * @return empty string if unknown language
         */
        std::string isoCode3() const;

        /**
         * Returns language name
         */
        std::string isoName() const;

        /**
         * Checks if language is valid
         */
        bool isValid() const;

        enum sort_t
        {
            SORT_NONE, SORT_BY_CODE2, SORT_BY_CODE3, SORT_BY_NAME
        };
    public:
        /**
         * Returns list of known language
         * @param sort - sorts returned list
         */
        static LanguageList allLanguages(sort_t sort = SORT_NONE);

        /**
         * Returns language instance by given language iso code
         * @param code - ISO 639-1 language code (2-characters)
         * @return Language(LANGUAGE_UNKNOWN) if unknown code given
         */
        static Language byCode2(const std::string& code);

        /**
         * Returns language instance by given language iso code
         * @param code - ISO 639-2 language code
         * @return Language(LANGUAGE_UNKNOWN) if unknown code given
         */
        static Language byCode3(const std::string& code);

        /**
         * Returns language instance by given language name
         * @return Language(LANGUAGE_UNKNOWN) if unknown code given
         */
        static Language byName(const std::string& name);

        /**
          * Returns encoding for language used in cuneiform
          */
        static std::string encoding(const Language& Language);

        /**
         * Returns ISO 639-1 language code (2-character)
         */
        static std::string isoCode2(language_t language);

        /**
         * Returns ISO 639-2 language code (3-character)
         */
        static std::string isoCode3(language_t language);

        /**
         * Returns language name by given type
         * If language type is invalid returns empty string
         */
        static std::string isoName(language_t language);

        /**
         * Sorts given language list by ISO 639-1 iso code
         * (2-characters)
         */
        static void sortByCode2(LanguageList& lst);

        /**
         * Sorts given language list by ISO 639-2 iso code
         * (3-characters)
         */
        static void sortByCode3(LanguageList& lst);

        /**
         * Sorts given language list by language name
         */
        static void sortByName(LanguageList& lst);
    private:
        language_t language_;
    private:
        static LanguageList languagesAll();
};

inline language_t Language::get() const {
    return language_;
}
FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const Language& language);

}

#endif /* LANGUAGE_H_ */
