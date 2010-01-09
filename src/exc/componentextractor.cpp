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

#include <cstddef>
#include <stdexcept>
#include "componentextractor.h"
#include "exc.h"

namespace CIF
{

ComponentExtractor::ComponentExtractor()
{
    if(!REXC_Init(0, NULL))
        throw std::runtime_error("ComponentExtractor init failure");
}

ComponentExtractor::~ComponentExtractor()
{
    REXC_Done();
}

bool ComponentExtractor::dotMatrixOption() const
{
    return dot_martix_;
}

bool ComponentExtractor::faxOption() const
{
    return fax_;
}

void ComponentExtractor::setDotMatrixOption(bool value)
{
    dot_martix_ = value;
//    matrix = dot_matrix_ ? TRUE : FALSE;
}

void ComponentExtractor::setFaxOption(bool value)
{
    fax_ = value;
}

}
