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

#ifndef CRTFWORD_H_
#define CRTFWORD_H_

#include <vector>
#include <iostream>

#include "cttypes.h"
#include "crtfchar.h"

class CRtfWord
{
public:
    CRtfWord();
    ~CRtfWord();

    void getCoordinatesAndProbability(void);

    typedef std::vector<CRtfChar*> vectorChar;
    vectorChar chars;

    CIF::Rect rect;

    int16_t m_wcl;
    int16_t m_wct;
    int16_t m_wcr;
    int16_t m_wcb;
    uchar m_wcs;
    uchar m_wcp;

    uint16_t font_number;
    uint16_t ideal_font_point_size;
    uint16_t real_font_point_size;
private:
    void calcCoordinates();
};

std::ostream& operator<<(std::ostream& os, const CRtfWord& word);

#endif /* CRTFWORD_H_ */
