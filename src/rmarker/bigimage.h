/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef BIGIMAGE_H_
#define BIGIMAGE_H_

#include "ccom/ccomdefs.h"
#include "cpage/cpagetyps.h"

namespace CIF {

class BigImage
{
public:
    BigImage(Handle CPage);
    ~BigImage();

    CCOM_handle ccom() const;
    const char * imageName() const;
    int incline() const;
    void setCCOM(CCOM_handle ccom);
    void setImageName(const char * name);
private:
    CCOM_handle ccom_;
    char image_name_[CPAGE_MAXNAME];
    int incline_;
};
}

#endif /* BIGIMAGE_H_ */
