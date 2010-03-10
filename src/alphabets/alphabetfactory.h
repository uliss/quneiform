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

#ifndef ALPHABETFACTORY_H_
#define ALPHABETFACTORY_H_

#include <map>
#include <boost/noncopyable.hpp>
#include "common/language.h"
#include "alphabet.h"
#include "lang_def.h"
#include "globus.h"

namespace CIF
{

class CLA_EXPO AlphabetFactory : public boost::noncopyable
{
    public:
        typedef Alphabet * (*alphabetCreate)();

        /**
         * defines a pair of alphabet data tables
         * filename1 amd filename2
         */
        typedef std::pair<std::string, std::string> AlphabetTables;

        /**
         * Returns reference to alphabet factory instance
         */
        static AlphabetFactory& instance();
    public:
        /**
         * Returns pair of alphabets tables by given language
         * @param language language type
         * @return pair of language data alphabet table
         * @throw AlphabetException if can't find data
         */
        AlphabetTables alphabetTables(language_t language);

        /**
         * Checks if data files for given language exists
         * @param language
         * @return true if datafile for given language exists
         */
        bool isLanguageData(language_t language);

        /**
         * Checks if given language is registered
         * @param language type
         * @return true if language is registered
         */
        bool isLanguageRegistered(language_t language);

        /**
         * Returns pointer to alphabet by given language type
         * @throw AlphabetException if alphabet not found by given language type
         * @param language
         * @return pointer to alphabet
         */
        AlphabetPtr make(language_t language);

        /**
         * Registers function that creates alphabet by given language type
         * @param language type
         * @param creator pointer to function that creates concrete Alphabet
         * @return true
         */
        bool registerCreator(language_t language, alphabetCreate creator);

        /**
         * Registers alphabets table files by given language type
         * @param language type
         * @param table alphabet tables
         * @return true
         */
        bool registerAlphabetTables(language_t, const AlphabetTables& table);

        /**
         * Returns a list of supported languages
         */
        LanguageList supportedLanguages();
    private:
        typedef std::map<language_t, alphabetCreate> AlphabetMap;
        AlphabetMap alpha_map_;
        typedef std::map<language_t, AlphabetTables> AlphabetTablesMap;
        AlphabetTablesMap alphabet_tables_map_;
        AlphabetFactory();
};

}

#endif /* ALPHABETFACTORY_H_ */
