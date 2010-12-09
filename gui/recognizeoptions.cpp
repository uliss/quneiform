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
#include "recognizeoptions.h"

RecognizeOptions::RecognizeOptions(QObject *parent) :
    QObject(parent), fax_(false), dot_matrix_(false),
    cf_spelling_(true), onecolumn_layout_(true),
    search_pictures_(true)
{
}

bool RecognizeOptions::dotMatrix() const {
    return dot_matrix_;
}

bool RecognizeOptions::fax() const {
    return fax_;
}

bool RecognizeOptions::oneColumn() const {
    return onecolumn_layout_;
}

bool RecognizeOptions::picturesSearch() const {
    return search_pictures_;
}

void RecognizeOptions::setPicturesSearch(bool value) {
    search_pictures_ = value;
    emit changed();
}

void RecognizeOptions::setDotMatrix(bool value) {
    dot_matrix_ = value;
    emit changed();
}

void RecognizeOptions::setFax(bool value) {
    fax_ = value;
    emit changed();
}

void RecognizeOptions::setOneColumn(bool value) {
    onecolumn_layout_ = value;
    emit changed();
}

bool RecognizeOptions::spelling() const {
    return cf_spelling_;
}

void RecognizeOptions::useSpelling(bool value) {
    cf_spelling_ = value;
    emit changed();
}

QDataStream& operator<<(QDataStream& os, const RecognizeOptions& opts) {
    os << opts.dot_matrix_
            << opts.fax_
            << opts.onecolumn_layout_
            << opts.search_pictures_
            << opts.cf_spelling_;
    return os;
}

QDataStream& operator>>(QDataStream& is, RecognizeOptions& opts) {
    is >> opts.dot_matrix_
            >> opts.fax_
            >> opts.onecolumn_layout_
            >> opts.search_pictures_
            >> opts.cf_spelling_;
    return is;
}
