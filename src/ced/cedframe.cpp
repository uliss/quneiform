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

#include "cedframe.h"
#include "cedexporter.h"

namespace CIF
{

CEDFrame::CEDFrame(BlockElement * parent) :
    BlockElement(parent), dxfrtextx(-1), dxfrtexty(-1), border_space_(-1), position_(-1) {
}

void CEDFrame::exportElement(CEDExporter& exp) {
    exp.exportFrame(*this);
}

void CEDFrame::setBorderSpace(int borderSpace) {
    border_space_ = borderSpace;
}

void CEDFrame::setPosition(char pos) {
    position_ = pos;
}

}
