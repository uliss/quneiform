/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#ifndef CF_CPAGE_TABLECELL_H
#define CF_CPAGE_TABLECELL_H

#include "cttypes.h"
#include "commondata.h"

namespace cf {
namespace cpage {

class TableCell
{
    int32_t number_; // Номер ячейки физической таблицы ( начиная с 1 )
    Point32 coord_; // Координаты привязки к физической ячейке
    int32_t fragm_number_; // Номер фрагмента
    int32_t geom_count_; // число геометрических ячеек, входящих в физическую
public:
    TableCell();

    int number() const;
    void setNumber(int n);

    Point point() const;
    void setPoint(const Point& pt);

    int32_t geomCount() const {
        return geom_count_;
    }

    bool isPhysic() const {
        return geom_count_ > 1;
    }

    int32_t& Fragment() {
        return fragm_number_;
    }

    int32_t& GeCount() {
        return geom_count_;
    }
};

} // namespace cpage
} // namespace cf

#endif // CF_CPAGE_TABLECELL_H
