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

#ifndef LINEPOOL_H_
#define LINEPOOL_H_

#include "bwsegment.h"
#include "common/singleton.h"

namespace CIF
{

class LinePool
{
    public:
        LinePool();
        int size() const;
        BWS * begin();
        BWS * end();
    private:
        static const int LINE_POOL_LENGTH = 512 * 4;
        // FIXME why +9? uliss
        BWS pool_[LINE_POOL_LENGTH + 9];
};

typedef Singleton<LinePool> EVN_LinePool;

}

#endif /* LINEPOOL_H_ */
