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

#ifndef XMLTAG_H_
#define XMLTAG_H_

#include "tag.h"

namespace CIF
{

class XmlTag: public Tag
{
    public:
        XmlTag(const std::string& name, const std::string& content = "");
        XmlTag(const std::string& name, const Attributes& attrs);

        /**
         * Set write new line after tag
         */
        void setNewline(bool value);

        /**
         * Writes attributes
         */
        virtual void writeAttributes(std::ostream& os) const;

        /**
         * Writes tag begin
         */
        virtual void writeBegin(std::ostream& os) const;

        /**
         * Writes tag content
         */
        virtual void writeContent(std::ostream& os) const;

        /**
         * Writes tag end
         */
        virtual void writeEnd(std::ostream& os) const;

        /**
         * Writes single tag
         */
        virtual void writeSingle(std::ostream& os) const;

    private:
        bool new_line_;
};

}

#endif /* XMLTAG_H_ */
