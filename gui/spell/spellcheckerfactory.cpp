/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include "spellcheckerfactory.h"
#include "dummyspellchecker.h"

#ifdef Q_OS_MAC
#include "macspellchecker.h"
#endif

#ifdef HAVE_ASPELL
#include "aspellchecker.h"
#endif

SpellCheckerFactoryImpl::SpellCheckerFactoryImpl()
{
}

SpellCheckerPtr SpellCheckerFactoryImpl::make(const Language& lang) {
    SpellCheckerMap::iterator it = spell_checkers_.find(lang);

    if(it != spell_checkers_.end())
        return it.value();

#ifdef Q_OS_MAC
    SpellCheckerPtr mac(new MacSpellChecker(lang));
    if(mac->isSupported(lang)) {
        spell_checkers_.insert(lang, mac);
        return mac;
    }
#endif

#ifdef HAVE_ASPELL
    SpellCheckerPtr aspell(new ASpellChecker(lang));
    if(aspell->isSupported(lang)) {
        spell_checkers_.insert(lang, aspell);
        return aspell;
    }
#endif

    SpellCheckerPtr dummy(new DummySpellChecker(lang));
    spell_checkers_.insert(lang, dummy);
    return dummy;
}
