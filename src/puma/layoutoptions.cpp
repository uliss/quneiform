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

#include "layoutoptions.h"
#include "mpuma.h"
#include "rstuff/rstuff.h"
#include "rmarker/rmarker.h"

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
    data.gbAutoRotate = auto_rotate_;
    data.gnPictures = pictures_;
    data.gbDotMatrix = dot_matrix_;
    data.pgrc_line = &rc_line_;
    data.gnTables = tables_;
    data.pgnNumberTables = &tables_num_;
    data.pgneed_clean_line = &need_clean_line_;
    data.gRectTemplate = rect_template_;
    data.szLayoutFileName = layout_filename_.c_str();
}

void LayoutOptions::setData(RMPreProcessImage& data) {
    data.gbAutoRotate = auto_rotate_;
    data.gnPictures = pictures_;
    data.gbDotMatrix = dot_matrix_;
    data.gbOneColumn = one_column_;
    data.pgrc_line = &rc_line_;
    data.gnTables = tables_;
    data.pgnNumberTables = &tables_num_;
    data.pgneed_clean_line = &need_clean_line_;
    data.szLayoutFileName = layout_filename_.c_str();
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

int LayoutOptions::tablesNum() const {
    return tables_num_;
}

}
