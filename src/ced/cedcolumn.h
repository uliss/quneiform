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

#ifndef CEDCOLUMN_H_
#define CEDCOLUMN_H_

#include "blockelement.h"

namespace cf
{

class CEDSection;

class CLA_EXPO CEDColumn: public BlockElement
{
    public:
        CEDColumn(CEDSection * parent = NULL);

        void exportElement(CEDExporter& exp);
        void setSpace(int space);
        void setWidth(int width);
        int space() const;
        int width() const;
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar.template register_type<CEDParagraph>();
            ar & make_nvp("block-element", boost::serialization::base_object<BlockElement>(*this));
            ar & make_nvp("width", width_);
            ar & make_nvp("space", space_);
        }
#endif
    private:
        int width_;
        int space_;
};

}

#endif /* CEDCOLUMN_H_ */
