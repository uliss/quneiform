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

#ifndef PAGEINFO_H_
#define PAGEINFO_H_

#include <string>

namespace cf
{
namespace CPAGE
{

class PageInfo
{
    public:
        PageInfo() :
                Page(0), DPIX(0), DPIY(0), BitPerPixel(0), X(0), Y(0), Width(0),
                Height(0), Incline2048(0), Angle(0), SkewLocVerLin2048(0),
                Images(0), status(0) {
        }

        unsigned short dpix() const {
            return DPIX;
        }

        unsigned short dpiy() const {
            return DPIY;
        }

        size_t pageNumber() const {
            return Page;
        }

    public:
        // имя блока распознаваемого изображения в контейнере RIMAGE
        std::string ImageName;
        size_t Page; // номер страницы
        unsigned short DPIX;
        unsigned short DPIY;
        unsigned int BitPerPixel;
        int X; // смещение относительно глобальных координат
        int Y;
        unsigned int Width; // размеры в пиксеалх
        unsigned int Height;
        int Incline2048; // наклон страницы в долях от 2048
        unsigned int Angle;// угол поворота страницы 0, 90, 180, 270
        int SkewLocVerLin2048;//вертикальное отклонение в долях 2048

        enum image_t {
            IMAGE_USER = 1,
            IMAGE_BINARIZE = 2,
            IMAGE_DELLINE = 4,
            IMAGE_ROTATE = 8,
            IMAGE_ORTOMOVE = 16,
            IMAGE_TURN = 32
        };

        unsigned int Images;//существующие изображения
        unsigned int status;

        enum {
            // темплейт установлен пользователем
            PINFO_USERTEMPLATE = 1,
            // темплейт установлен автоматически
            PINFO_AUTOTEMPLATE = 2,
        };
};

}
}

#endif /* PAGEINFO_H_ */
