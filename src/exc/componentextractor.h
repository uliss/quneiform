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

#ifndef COMPONENTEXTRACTOR_H_
#define COMPONENTEXTRACTOR_H_

#include "globus.h"

namespace cf
{

class CLA_EXPO ComponentExtractor
{
    public:
        ComponentExtractor();
        ~ComponentExtractor();

        bool dotMatrixOption() const;
        bool faxOption() const;
        void setDotMatrixOption(bool value);
        void setFaxOption(bool value);
    private:
        bool dot_martix_;
        bool fax_;
};

}

#endif /* COMPONENTEXTRACTOR_H_ */
