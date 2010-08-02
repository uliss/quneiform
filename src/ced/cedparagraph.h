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

#ifndef CEDPARAGRAPH_H_
#define CEDPARAGRAPH_H_

#include <vector>
#include <boost/shared_ptr.hpp>

#include "globus.h"
#include "blockelement.h"
#include "common/color.h"
#include "common/align.h"
#include "common/serialize.h"

namespace CIF
{

class CEDLine;

class CLA_EXPO CEDParagraph: public BlockElement
{
    public:
        CEDParagraph();

        int border; //  frame around abzattsa
        char spcBtwLnsMult;

        /**
         * Adds line to the end of paragraph
         */
        void addLine(CEDLine * line);

        /**
         * Returns paragraph alignment
         * @see setAlign()
         */
        align_t align() const;

        /**
         * Call appropriate exporter function
         */
        void exportElement(CEDExporter& exp);

        /**
         * Returns paragraph indents
         * @see setIndent()
         */
        int indent() const;

        /**
         * Returns pointer to line at given position
         * @see lineCount()
         */
        CEDLine * lineAt(size_t pos);

        /**
         * Returns number of lines
         * @see lineAt()
         */
        size_t lineCount() const;

        /**
         * Returns space between lines
         * @see setLineSpace()
         */
        int lineSpace() const;

        /**
         * Sets paragraph alignment
         * @see align()
         */
        void setAlign(align_t align);

        /**
         * Sets indent for paragraph
         * @see indent()
         */
        void setIndent(int value);

        /**
         * Sets line space
         * @see lineSpace()
         */
        void setLineSpace(int value);

        /**
         * Sets left padding
         * @see rightPadding()
         */
        void setPaddingLeft(int value);

        /**
         * Sets right padding
         * @see setPaddingLeft()
         */
        void setPaddingRight(int value);

        /**
         * Sets user number at the stage of fragmentation
         */
        void setUserNumber(int value);
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            ar.template register_type<CEDLine>();
            ar & boost::serialization::base_object<BlockElement>(*this);
            ar & align_;
            ar & line_space_;
            ar & indent_;
            ar & padding_left_;
            ar & padding_right_;
            ar & user_number_;
            ar & border;
        }
#endif
    private:
        align_t align_;
        int line_space_;
        int indent_;
        int padding_left_;
        int padding_right_;
        int user_number_;
};

}

#endif /* CEDPARAGRAPH_H_ */
