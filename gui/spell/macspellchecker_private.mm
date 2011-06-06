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

#import <Foundation/NSArray.h>
#import <AppKit/NSSpellChecker.h>
#include <QDebug>

#include "macspellchecker_private.h"
#include "macpool.h"
#include "macstring.h"
#include "language.h"

ISpellChecker::SpellList checkSpelling(const QString& str, int64_t docTag) {
    ISpellChecker::SpellList res;

    const MacPool pool;
    const MacString text(str);
    const int len = str.length();

    int index = 0;
    while (index < len) {
        NSRange r = [[NSSpellChecker sharedSpellChecker]
                checkSpellingOfString : (NSString *)(CFStringRef) text
                startingAt : index
                language : nil
                wrap : false
                inSpellDocumentWithTag : docTag
                wordCount : nil];

        Range range(r.location, r.length);

        index = range.location() + range.length();
        if (range.location() < len)
            res.append(range);
        else
            break;
    }

    return res;
}

bool setSpellLanguage(const Language& lang) {
    const MacPool pool;

    qDebug() << Q_FUNC_INFO << lang.name();

    BOOL ok = [[NSSpellChecker sharedSpellChecker]
                setLanguage : (NSString *) MacString::toCFStringRef(lang.isoCode2())];
    return ok;
}

void spellInit() {
    const MacPool pool;
    [[NSSpellChecker sharedSpellChecker] setAutomaticallyIdentifiesLanguages: NO];
}

QStringList suggest(const QString& word, int64_t docTag) {
    QStringList res;
    const MacPool pool;
    NSArray * const array  = [[NSSpellChecker sharedSpellChecker]
            guessesForWordRange : NSMakeRange(0, word.length())
            inString: (NSString *) MacString::toCFStringRef(word)
            language : (NSString *) nil
            inSpellDocumentWithTag: docTag];

    if (array == 0)
        return res;

    for (unsigned int i = 0; i < [array count]; ++i)
        res.append(MacString::toQString((CFStringRef)[array objectAtIndex: i]));

    return res;
}

QList<Language> supportedLanguages() {
    QList<Language> res;

    const MacPool pool;

    NSArray * lst = [[NSSpellChecker sharedSpellChecker] availableLanguages];
    for (uint i = 0; i < [lst count]; i++) {
        Language lang = Language::fromIsoCode2(MacString::toQString((CFStringRef)[lst objectAtIndex : i]));

        if(!lang.isValid())
            continue;

        if(res.contains(lang))
            continue;

        res.append(lang);
    }

    return res;
}
