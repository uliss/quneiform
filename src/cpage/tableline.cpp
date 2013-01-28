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

#include "tableline.h"

namespace cf {
namespace cpage {

TableLine::TableLine()
{
    m_nCoord = 0;
    //m_nItems = 0;
}

TableLine::~TableLine()
{
}

Bool32 TableLine::Create(int32_t nCoord, uint32_t nItems)
{
    Bool32 rc = FALSE;
    m_nCoord = nCoord;
    //m_nItems = nItems;
    rc = m_lpProperty.Create(nItems);
    return rc;
}

void TableLine::Delete()
{
    m_nCoord = 0;
    //m_nItems = 0;
    m_lpProperty.Delete();
}

Bool32 TableLine::Attach(CPageHandle hPage)
{
    return m_lpProperty.Attach(hPage);
}

Bool32 TableLine::Store(CPageHandle hPage)
{
    return m_lpProperty.Store(hPage);
}

void TableLine::Remove(CPageHandle hPage)
{
    m_lpProperty.Remove(hPage);
}

} // namespace cpage
} // namespace cf
