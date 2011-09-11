/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef SHAREDOPTIONS_H
#define SHAREDOPTIONS_H

#include <cstdlib>

#include "globus.h"
#include "common/formatoptions.h"
#include "common/recognizeoptions.h"

namespace cf {

class CLA_EXPO SharedOptions
{
public:
    enum { MAXSIZE = 5 * 1024 };
public:
    /**
      * Constructs options from shared memory
      */
    SharedOptions();

    FormatOptions formatOptions() const;
    RecognizeOptions recognizeOptions() const;

    void store(const FormatOptions& opts);
    void store(const RecognizeOptions& opts);
private:
    void clear();
private:
    char format_data_[MAXSIZE];
    char recognize_data_[MAXSIZE];
    size_t format_size_;
    size_t recognize_size_;
};

}

#endif // SHAREDOPTIONS_H
