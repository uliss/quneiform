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

#ifndef CF_CPAGE_TABLELINE_H
#define CF_CPAGE_TABLELINE_H

#include "cttypes.h"
#include "ptrname.h"

namespace cf {
namespace cpage {

class TableLine
{
private:
    int32_t m_nCoord; // координата X - для вертикальных Y - для горизонтальных
#define LINE_REALY  0x00000001 // Реальный элемент линии
#define LINE_VISUAL 0x00000003 // Видимый элемент линии ( всегда реальный !!)
    PtrName<uint32_t> m_lpProperty; // свойства элементов линии
public:
    TableLine();
    ~TableLine();

    Bool32 Create(int32_t nCoord, uint32_t nItems);
    void Delete();

    Bool32 Attach(CPageHandle hPage);
    Bool32 Store(CPageHandle hPage);
    void Remove(CPageHandle hPage);

    inline void SetRealy(uint32_t nItem, Bool32 b) {
        if (b)
            m_lpProperty[nItem] |= LINE_REALY;

        else
            m_lpProperty[nItem] &= ~LINE_REALY;
    }

    inline void SetVisual(uint32_t nItem, Bool32 b) {
        if (b)
            m_lpProperty[nItem] |= LINE_VISUAL;

        else {
            m_lpProperty[nItem] &= ~LINE_VISUAL;
            SetRealy(nItem, TRUE);
        }
    }

    inline Bool32 IsRealy(uint32_t nItem) {
        return m_lpProperty[nItem] & LINE_REALY;
    }

    inline Bool32 IsVisual(uint32_t nItem) {
        return m_lpProperty[nItem] & LINE_VISUAL;
    }

    inline operator int32_t() {
        return m_nCoord;
    }

};

} // namespace cpage
} // namespace cf

#endif // CF_CPAGE_TABLELINE_H
