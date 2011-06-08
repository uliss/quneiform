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

#include <QTest>
#include "testspellcheckerfactory.h"
#include "gui/spell/spellcheckerfactory.h"
#include "gui/spell/dummyspellchecker.h"

#ifdef HAVE_ASPELL
#include "gui/spell/aspellchecker.h"
#endif

#define CHECK_SPELLER(ptr, type) QVERIFY(dynamic_cast<type*>(ptr.data()));

void TestSpellCheckerFactory::testMake() {
    SpellCheckerPtr c = SpellCheckerFactory::instance().make(Language());
    CHECK_SPELLER(c, DummySpellChecker);

    c = SpellCheckerFactory::instance().make(Language::english());

#ifndef Q_OS_MAC
#ifdef HAVE_ASPELL
     CHECK_SPELLER(c, ASpellChecker);
#endif
#endif
}

QTEST_MAIN(TestSpellCheckerFactory);
