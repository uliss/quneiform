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

#ifndef SPELLCHECKERFACTORY_H
#define SPELLCHECKERFACTORY_H

#include <QHash>
#include "common/singleton.h"
#include "ispellchecker.h"

class QTextDocument;

class SpellCheckerFactoryImpl
{
public:
    SpellCheckerFactoryImpl();
    SpellCheckerPtr make(QTextDocument * doc);
private:
    typedef QHash<QTextDocument*, SpellCheckerPtr> SpellCheckerMap;
    SpellCheckerMap spell_checkers_;
};

typedef cf::Singleton<SpellCheckerFactoryImpl> SpellCheckerFactory;

#endif // SPELLCHECKERFACTORY_H
