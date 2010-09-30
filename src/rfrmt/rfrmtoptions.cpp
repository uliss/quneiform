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

#include "rfrmtoptions.h"
#include "creatertf.h"

namespace cf
{

unsigned int RfrmtOptions::format_mode_ = 0;
bool RfrmtOptions::line_transfer_ = false;

bool RfrmtOptions::useBold() {
    return !hasFlag(NOBOLD);
}

bool RfrmtOptions::useFrames() {
    return hasFlag(USE_FRAME);
}

bool RfrmtOptions::useFramesAndColumns() {
    return hasFlag(USE_FRAME_AND_COLUMN);
}

bool RfrmtOptions::useItalic() {
    return !hasFlag(NOCURSIV);
}

bool RfrmtOptions::useNone() {
    return hasFlag(USE_NONE);
}

bool RfrmtOptions::useSize() {
    return !hasFlag(NOSIZE);
}

}
