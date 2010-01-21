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

#include <cassert>
#include <iostream>

#include "alphabetfactory.h"
#include "cfcompat.h"

namespace CIF
{

AlphabetFactory::AlphabetFactory()
{
}

AlphabetFactory::AlphabetTables AlphabetFactory::alphabetTables(language_t lang)
{
    AlphabetTablesMap::iterator it = alphabet_tables_map_.find(lang);
    if (it == alphabet_tables_map_.end())
        throw AlphabetException("[AlphabetFactory::alphabetTables] no alphabet tables found", lang);

    return it->second;
}

AlphabetFactory& AlphabetFactory::instance()
{
    static AlphabetFactory instance_;
    return instance_;
}

bool AlphabetFactory::isLanguageData(language_t language)
{
    try {
        AlphabetTables entry = alphabetTables(language);

        if (data_file_exists(entry.first.c_str()) == -1)
            return false;
        if (data_file_exists(entry.second.c_str()) == -1)
            return false;
    }
    catch (AlphabetException&) {
        return false;
    }

    return true;
}

AlphabetPtr AlphabetFactory::make(language_t language)
{
    AlphabetMap::iterator it = alpha_map_.find(language);

    if (alpha_map_.end() == it)
        throw AlphabetException("[AlphabetFactory::make] Unregistered language type", language);

    AlphabetPtr ret(it->second());
    assert(ret.get());
    return ret;
}

bool AlphabetFactory::registerAlphabetTables(language_t lang, const AlphabetTables& tables)
{
    alphabet_tables_map_.insert(AlphabetTablesMap::value_type(lang, tables));
    return true;
}

bool AlphabetFactory::registerCreator(language_t language, alphabetCreate creator)
{
    alpha_map_.insert(AlphabetMap::value_type(language, creator));
    return true;
}

LanguageList AlphabetFactory::supportedLanguages()
{
    LanguageList ret;
    for(AlphabetMap::iterator it = alpha_map_.begin(), end = alpha_map_.end(); it != end; ++it)
        ret.push_back(it->first);
    return ret;
}

}
