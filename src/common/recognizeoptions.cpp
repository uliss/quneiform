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

#include <iomanip>
#include "recognizeoptions.h"
#include "language.h"

namespace CIF {

RecognizeOptions::RecognizeOptions() :
    language_(LANGUAGE_ENGLISH), auto_rotate_(false), dot_matrix_(false),
            do_spell_correction_(true), fax100_(false), one_column_(false),
            find_pictures_(true), table_mode_(TABLE_DEFAULT) {

}

bool RecognizeOptions::autoRotate() const {
    return auto_rotate_;
}

bool RecognizeOptions::dotMatrix() const {
    return dot_matrix_;
}

bool RecognizeOptions::fax() const {
    return fax100_;
}

language_t RecognizeOptions::language() const {
    return language_;
}

bool RecognizeOptions::oneColumn() const {
    return one_column_;
}

bool RecognizeOptions::pictureSearch() const {
    return find_pictures_;
}

void RecognizeOptions::setAutoRotate(bool value) {
    auto_rotate_ = value;
}

void RecognizeOptions::setFax(bool value) {
    fax100_ = value;
}

void RecognizeOptions::setDotMatrix(bool value) {
    dot_matrix_ = value;
}

void RecognizeOptions::setLanguage(language_t language) {
    language_ = language;
}

void RecognizeOptions::setOneColumn(bool value) {
    one_column_ = value;
}

void RecognizeOptions::setPictureSearch(bool value) {
    find_pictures_ = value;
}

void RecognizeOptions::setSpellCorrection(bool value) {
    do_spell_correction_ = value;
}

void RecognizeOptions::setTableMode(table_mode_t mode) {
    table_mode_ = mode;
}

void RecognizeOptions::setUserDict(const std::string& user_dict) {
    user_dict_name_ = user_dict;
}

bool RecognizeOptions::spellCorection() const {
    return do_spell_correction_;
}

RecognizeOptions::table_mode_t RecognizeOptions::tableMode() const {
    return table_mode_;
}

const std::string& RecognizeOptions::userDict() const {
    return user_dict_name_;
}

std::ostream& operator<<(std::ostream& os, const RecognizeOptions& opts) {
    using namespace std;
    const int FLD_WD = 25;
    os
            << "##################################################################\n"
            << " Recognize options:\n" << boolalpha << std::left
            << setw(FLD_WD) << "  Spell: " << opts.spellCorection() << "\n"
            << setw(FLD_WD) << "  Fax:   " << opts.fax() << "\n"
            << setw(FLD_WD) << "  Single column layout: " << opts.oneColumn() << "\n"
            << setw(FLD_WD) << "  Dot matix: " << opts.dotMatrix() << "\n"
            << setw(FLD_WD) << "  Pictures search: " << opts.pictureSearch() << "\n"
            << setw(FLD_WD) << "  Table mode: " << opts.tableMode() << "\n"
            << setw(FLD_WD) << "  Autorotate: " << opts.autoRotate() << "\n"
            << setw(FLD_WD) << "  Language: " << Language(opts.language()) << "\n"
            << setw(FLD_WD) << "  User dictionary: " << opts.userDict() << "\n"
            << "##################################################################\n";
    return os;
}

}
