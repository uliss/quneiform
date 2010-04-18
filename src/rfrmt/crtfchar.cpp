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

#include "crtfchar.h"

namespace CIF
{

font_number CRtfChar::font() const {
    return font_number_;
}

Rect CRtfChar::idealRect() const {
    return ideal_rect_;
}

language_t CRtfChar::language() const {
    return language_;
}

Rect CRtfChar::realRect() const {
    return real_rect_;
}

void CRtfChar::setFont(font_number fontNumber) {
    font_number_ = fontNumber;
}

void CRtfChar::setIdealRect(const Rect& rect) {
    ideal_rect_ = rect;
}

void CRtfChar::setLanguage(language_t lang) {
    language_ = lang;
}

void CRtfChar::setRealRect(const Rect& rect) {
    real_rect_ = rect;
}

}

