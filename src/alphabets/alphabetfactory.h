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
#include <list>
#include <boost/shared_ptr.hpp>
#include "alphabet.h"
#include "lang_def.h"

namespace CIF
{

typedef boost::shared_ptr<Alphabet> AlphabetPtr;
typedef std::list<language_t> LanguageList;

class AlphabetFactory
{
    public:
        typedef Alphabet * (*alphabetCreate)();
        typedef std::pair<std::string, std::string> AlphabetTables;
        static AlphabetFactory& instance();
    public:
        /**
         * Returns pair of alphabets tables by given language
         * @param lang language type
         * @return pair of language data alphabet table
         * @throw AlphabetException if can't find data
         */
        AlphabetTables alphabetTables(language_t lang);

        /**
         * Checks if data files for given language exists
         * @param lang
         * @return true if datas file for given language exists
         */
        bool isLanguageData(language_t lang);

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
        AlphabetFactory(const AlphabetFactory&);
        void operator=(const AlphabetFactory&);
};

}

#endif /* ALPHABETFACTORY_H_ */
