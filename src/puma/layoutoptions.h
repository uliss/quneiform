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

#ifndef LAYOUTOPTIONS_H_
#define LAYOUTOPTIONS_H_

#include <string>

#include "pumadef.h"
#include "cttypes.h"
#include "rect.h"

class RMPreProcessImage;

namespace CIF {

class RSPreProcessImage;

class LayoutOptions
{
public:
    LayoutOptions();
    bool autoRotate() const;
    bool dotMatrix() const;
    std::string layoutFilename() const;
    puma_picture_t pictures() const;
    Rect rect() const;
    void setAutoRotate(bool val);
    void setData(RSPreProcessImage& data);
    void setData(RMPreProcessImage& data);
    void setDotMatrix(bool val);
    void setOneColumn(bool val);
    void setPictures(puma_picture_t type);
    void setRect(const Rect& rect);
    void setTable(puma_table_t mode);
    int tablesNum() const;
private:
    bool auto_rotate_;
    bool dot_matrix_;
    bool one_column_;
    puma_picture_t pictures_;
    puma_table_t tables_;
    int tables_num_;
    Rect rect_template_;
    std::string layout_filename_;
    Bool32 rc_line_;
    Bool32 need_clean_line_;
};

}

#endif /* LAYOUTOPTIONS_H_ */
