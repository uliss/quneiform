/***************************************************************************
 *   Copyright (C) 2008 by Сергей Полтавский   *
 *   poltava@gentoo   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CIFPARAGRAPH_H
#define CIFPARAGRAPH_H

#include <string>
#include "blockelement.h"

namespace CIF
{

    /**
 @author Serj Poltavskiy <serge.uliss at gmail.com>
*/
    class Paragraph : public BlockElement
    {
    public:
        Paragraph();
        Paragraph(const std::string& text);

        /**
         * Adds text at last paragraph line. if par is empty, creates new line.
         * @param text
         */
        void addText(const std::string& text);
        void exportTo(const Exporter& exp) const;
    private:
        bool isValid(const ElementPtr& el) const;
    };

}

#endif
