/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#include <QDataStream>
#include "recognitionsettings.h"

RecognitionSettings::RecognitionSettings() :
    fax_(false), dot_matrix_(false),
    cf_spelling_(true), onecolumn_layout_(false),
    search_pictures_(true)
{
}

bool RecognitionSettings::operator==(const RecognitionSettings& opts) const {
    return fax_ == opts.fax_
            && dot_matrix_ == opts.dot_matrix_
            && cf_spelling_ == opts.cf_spelling_
            && onecolumn_layout_ == opts.onecolumn_layout_
            && search_pictures_ == opts.search_pictures_;
}

bool RecognitionSettings::dotMatrix() const {
    return dot_matrix_;
}

bool RecognitionSettings::fax() const {
    return fax_;
}

bool RecognitionSettings::oneColumn() const {
    return onecolumn_layout_;
}

bool RecognitionSettings::picturesSearch() const {
    return search_pictures_;
}

void RecognitionSettings::setPicturesSearch(bool value) {
    search_pictures_ = value;
}

void RecognitionSettings::setDotMatrix(bool value) {
    dot_matrix_ = value;
}

void RecognitionSettings::setFax(bool value) {
    fax_ = value;
}

void RecognitionSettings::setOneColumn(bool value) {
    onecolumn_layout_ = value;
}

bool RecognitionSettings::spelling() const {
    return cf_spelling_;
}

void RecognitionSettings::useSpelling(bool value) {
    cf_spelling_ = value;
}

QDataStream& operator<<(QDataStream& os, const RecognitionSettings& opts) {
    os << opts.dot_matrix_
            << opts.fax_
            << opts.onecolumn_layout_
            << opts.search_pictures_
            << opts.cf_spelling_;
    return os;
}

QDataStream& operator>>(QDataStream& is, RecognitionSettings& opts) {
    is >> opts.dot_matrix_
            >> opts.fax_
            >> opts.onecolumn_layout_
            >> opts.search_pictures_
            >> opts.cf_spelling_;
    return is;
}
