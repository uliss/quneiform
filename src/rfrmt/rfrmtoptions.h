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

#ifndef RFRMTOPTIONS_H_
#define RFRMTOPTIONS_H_

#include "common/rect.h"
#include "globus.h"

namespace cf
{

// Formatting Mode
enum format_mode_t
{
    // no formatting
    USE_NONE = 0x0040,
    // use columns & frames
    USE_FRAME_AND_COLUMN = 0x0001,
    // use only frames
    USE_FRAME = 0x0002,
    NOBOLD = 0x0004,
    NOCURSIV = 0x0008,
    NOSIZE = 0x0020
};

class CLA_EXPO RfrmtOptions
{
    public:
        static unsigned int formatMode() {
            return format_mode_;
        }

        static bool hasFlag(format_mode_t flag) {
            return format_mode_ & flag;
        }

        static bool lineTransfer() {
            return line_transfer_;
        }

        template<class T, class F>
        static void setDrawCallback(F f) {
            T::setDrawCallback(f);
        }

        static void setFlag(format_mode_t flag) {
            format_mode_ |= flag;
        }

        static void setFormatMode(unsigned int mode) {
            format_mode_ = mode;
        }

        static void setLineTransfer(bool value) {
            line_transfer_ = value;
        }

        static bool useBold();
        static bool useFrames();
        static bool useFramesAndColumns();
        static bool useItalic();
        static bool useNone();
        static bool useSize();
    private:
        static unsigned int format_mode_;
        static bool line_transfer_;
    private:
        RfrmtOptions();
};

}

#endif /* RFRMTOPTIONS_H_ */
