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

namespace CIF
{

AlphabetFactory * AlphabetFactory::instance_ = 0;

AlphabetFactory::AlphabetFactory()
{
}

AlphabetFactory& AlphabetFactory::instance()
{
    if (!instance_) {
        instance_ = new AlphabetFactory;
    }

    return *instance_;
}

Alphabet * AlphabetFactory::make(language_t language)
{
    AlphabetMap::iterator it = alpha_map_.find(language);

    if (alpha_map_.end() == it)
        throw AlphabetException("Unregistered language type", language);

    return it->second();
}

bool AlphabetFactory::registerCreator(language_t language, alphabetCreate creator)
{
    alpha_map_.insert(AlphabetMap::value_type(language, creator));
    return true;
}

}
