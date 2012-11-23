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

namespace cf
{

CEDFrame::CEDFrame(BlockElement * parent, hposition_t hpos, vposition_t vpos) :
    BlockElement(parent),
    dxfrtextx(-1),
    dxfrtexty(-1),
    border_space_(-1),
    hpos_(hpos),
    vpos_(vpos) {
}


CEDFrame::CEDFrame(const CEDFrame& frame) :
    BlockElement(frame)
{
    border_space_ = frame.border_space_;
    hpos_= frame.hpos_;
    vpos_ = frame.vpos_;
}

CEDFrame * CEDFrame::clone() const
{
    return new CEDFrame(*this);
}

void CEDFrame::exportElement(CEDExporter& exp) {
    exp.exportFrame(*this);
}

void CEDFrame::setBorderSpace(int borderSpace) {
    border_space_ = borderSpace;
}

void CEDFrame::setHPosition(hposition_t pos) {
    hpos_ = pos;
}

void CEDFrame::setVPosition(vposition_t pos) {
    vpos_ = pos;
}

}
