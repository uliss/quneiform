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

#ifndef CEDFRAME_H_
#define CEDFRAME_H_

#include "globus.h"
#include "blockelement.h"

namespace CIF
{

class CLA_EXPO CEDFrame: public BlockElement
{
    public:
        CEDFrame(BlockElement * parent = NULL);

        void exportElement(CEDExporter& exp);

        int dxfrtextx;
        int dxfrtexty;

        void setBorderSpace(int borderSpace);
        void setPosition(char pos);
    private:
        int border_space_;
        char position_;
};

}

#endif /* CEDFRAME_H_ */
