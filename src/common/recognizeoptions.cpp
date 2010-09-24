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
            do_spell_correction_(true), fax100_(false), one_column_(false) {

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

void RecognizeOptions::setSpellCorrection(bool value) {
    do_spell_correction_ = value;
}

bool RecognizeOptions::spellCorection() const {
    return do_spell_correction_;
}

std::ostream& operator<<(std::ostream& os, const RecognizeOptions& opts) {
    using namespace std;
    os
            << "##################################################################\n"
            << " Recognize options:\n" << boolalpha << std::left << setw(25)
            << "  Spell: " << opts.spellCorection() << "\n" << setw(25)
            << "  Fax:   " << opts.fax() << "\n" << setw(25) //
            << "  Single column layout: " << opts.oneColumn() << "\n" << setw(
            25) //
            << "  Dot matix: " << opts.dotMatrix() << "\n" << setw(25)
            << "  Autorotate: " << opts.autoRotate() << "\n" << setw(25)
            << "  Language: " << Language(opts.language()) << "\n" << setw(10)
            << "##################################################################\n";
    return os;
}

}
