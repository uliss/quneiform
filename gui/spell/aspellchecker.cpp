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

#include <QDebug>
#include <aspell.h>

#include "aspellchecker.h"

ASpellChecker::ASpellChecker(const Language& lang)
    : ISpellChecker(lang),
    config_(NULL),
    speller_(NULL)
{
    configInit();
    ASpellChecker::setLanguage(lang);
}

ASpellChecker::~ASpellChecker() {
    delete_aspell_speller(speller_);
    delete_aspell_config(config_);
}

bool ASpellChecker::checkWord(const QString& word) {
    Q_CHECK_PTR(config_);
    Q_CHECK_PTR(speller_);

    QString prepared = prepareWord(word);
    if(prepared.isEmpty())
        return true;

    QByteArray str = prepared.toUtf8();

    switch(aspell_speller_check(speller_, str.data(), str.size())) {
    case 0:
        return false;
    case 1:
        return true;
    default:
        qDebug() << Q_FUNC_INFO << "aspell error for" << prepared;
        return false;
    }
}

void ASpellChecker::configInit() {
    config_ = new_aspell_config();
    aspell_config_replace(config_, "encoding", "utf-8");
}

bool ASpellChecker::hasErrors(const QString& text) {
    return !spellErrors(text).isEmpty();
}

QString ASpellChecker::prepareWord(const QString& word) {
    if(word.isEmpty())
        return QString();

    static const QRegExp non_digits("\\D");
    if(!word.contains(non_digits))
        return QString();

    if(word.isEmpty())
        return QString();

    QString res(word);
    QChar first_chr = res.at(0);
    if(!first_chr.isLetter())
        res.remove(0, 1);

    if(word.isEmpty())
        return QString();

    QChar last_chr = res.at(res.length() - 1);
    if(!last_chr.isLetter())
        res.chop(1);

    return res;
}

bool ASpellChecker::setLanguage(const Language& lang) {
    Q_CHECK_PTR(config_);

    ISpellChecker::setLanguage(lang);

    if(!aspell_config_replace(config_, "lang", lang.isoCode2().toAscii())) {
        qDebug() << Q_FUNC_INFO << "aspell_config_replace error for language:" << lang.name();
        qDebug() << aspell_config_error_message(config_);
        return false;
    }

    AspellCanHaveError * possible_err = new_aspell_speller(config_);
    if (aspell_error_number(possible_err) == 0) {
        if(speller_)
            delete_aspell_speller(speller_);

        speller_ = to_aspell_speller(possible_err);
    }
    else {
        qDebug() << Q_FUNC_INFO << aspell_error_message(possible_err);
        delete_aspell_can_have_error(possible_err);
        return false;
    }

    return true;
}

ISpellChecker::SpellList ASpellChecker::spellErrors(const QString& text) {
    SpellList res;

    int word_start = 0;
    int word_end = 0;
    for(int i = 0; i < text.length(); i++) {
        if(!text.at(i).isSpace()) {
            if(word_start == -1)
                word_start = i;

            word_end = i;

            if(word_end == text.length() - 1) {
                QString word = text.mid(word_start, word_end - word_start + 1);
                if(!checkWord(word)) {
                    res.append(Range(word_start, word.length()));
                }
            }
        }
        else {
            if(word_start == -1)
                continue;

            QString word = text.mid(word_start, word_end - word_start + 1);

            if(!checkWord(word)) {
                qDebug() << Q_FUNC_INFO << "spell error:" << word;
                res.append(Range(word_start, word.length()));
            }

            word_start = -1;
        }
    }
    return res;
}

QStringList ASpellChecker::suggest(const QString& word) {
    QStringList res;

    QByteArray b = word.toUtf8();
    const AspellWordList * words = aspell_speller_suggest(speller_, b.data(), b.size());
    if(!words)
        return res;

    AspellStringEnumeration * entries = aspell_word_list_elements(words);
    const char * suggest = NULL;

    while((suggest = aspell_string_enumeration_next(entries)) != 0) {
        res.append(QString::fromUtf8(suggest));
        if(res.count() > suggestLimit())
            break;
    }

    delete_aspell_string_enumeration(entries);

    return res;
}

QList<Language> ASpellChecker::supportedLanguages() const {
    QList<Language> res;

    AspellConfig * config = new_aspell_config();
    AspellDictInfoList * dictionary_list = get_aspell_dict_info_list(config);
    delete_aspell_config(config);
    AspellDictInfoEnumeration * entries = aspell_dict_info_list_elements(dictionary_list);

    const AspellDictInfo * entry;
    while ((entry = aspell_dict_info_enumeration_next(entries)) != 0) {
        Language l(Language::fromIsoCode2(entry->code));
        if(!res.contains(l))
            res.append(l);
    }

    delete_aspell_dict_info_enumeration(entries);

    return res;
}
