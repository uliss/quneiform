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

#include "common/tostring.h"
#include "layoutoptions.h"
#include "mpuma.h"
#include "rstuff/rstuff.h"

namespace CIF {

LayoutOptions::LayoutOptions() :
    auto_rotate_(false), dot_matrix_(false), one_column_(false), pictures_(PUMA_PICTURE_ALL),
            tables_(PUMA_TABLE_DEFAULT), tables_num_(0), rect_template_(Point(-1, -1),
                    Point(-1, -1)), layout_filename_("layout.bin"), rc_line_(TRUE),
            need_clean_line_(FALSE) {

}

bool LayoutOptions::autoRotate() const {
    return auto_rotate_;
}

bool LayoutOptions::dotMatrix() const {
    return dot_matrix_;
}

std::string LayoutOptions::layoutFilename() const {
    return layout_filename_;
}

bool LayoutOptions::oneColumn() const {
    return one_column_;
}

puma_picture_t LayoutOptions::pictures() const {
    return pictures_;
}
Rect LayoutOptions::rect() const {
    return rect_template_;
}

void LayoutOptions::setAutoRotate(bool val) {
    auto_rotate_ = val;
    SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
}

void LayoutOptions::setData(RSPreProcessImage& data) {
    data.pgrc_line = &rc_line_;
    data.pgnNumberTables = &tables_num_;
    data.pgneed_clean_line = &need_clean_line_;
}

void LayoutOptions::setDotMatrix(bool val) {
    dot_matrix_ = val;
    SetUpdate(FLG_UPDATE_CCOM, FLG_UPDATE_NO);
}

void LayoutOptions::setOneColumn(bool val) {
    one_column_ = val;
    SetUpdate(FLG_UPDATE_CPAGE, FLG_UPDATE_NO);
}

void LayoutOptions::setPictures(puma_picture_t type) {
    pictures_ = type;
    SetUpdate(FLG_UPDATE_CPAGE, FLG_UPDATE_NO);
}

void LayoutOptions::setTable(puma_table_t mode) {
    tables_ = mode;
    SetUpdate(FLG_UPDATE_CPAGE, FLG_UPDATE_NO);
}

void LayoutOptions::setRect(const Rect& rect) {
    rect_template_ = rect;
}

puma_table_t LayoutOptions::tableMode() const {
    return tables_;
}

int LayoutOptions::tablesNum() const {
    return tables_num_;
}

std::ostream& operator<<(std::ostream& os, const LayoutOptions& opts) {
    os << "Layout options:\n"
            << std::boolalpha
            << "  One Column:    " << opts.oneColumn() << "\n"
            << "  Pictures:      " << opts.pictures() << "\n"
            << "  Dot Matrix:    " << opts.dotMatrix() << "\n"
            << "  Auto Rotate:   " << opts.autoRotate() << "\n"
            << "  Tables number: " << opts.tablesNum() << "\n"
            << "  Geometry:      " << opts.rect() << "\n";

    return os;
}

}
