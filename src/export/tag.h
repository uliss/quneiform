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

#ifndef TAG_H_
#define TAG_H_

#include <string>
#include <map>
#include <iostream>

namespace CIF
{

class Tag
{
    public:
        typedef std::map<std::string, std::string> Attributes;
        typedef Attributes::value_type Attribute;

        Tag(const std::string& tagName, const std::string& content = "");
        Tag(const std::string& tagName, const Attributes& attr);
        virtual ~Tag();

        /**
         * Returns tag attribute value by name
         * If no attribute returns empty string
         */
        std::string attribute(const std::string& name) const;

        /**
         * Returns tag text content
         */
        std::string content() const;

        /**
         * Checks if single tag
         */
        virtual bool isSingle() const;

        /**
         * Checks if attribute exists
         */
        bool hasAttribute(const std::string& name) const;

        /**
         * Returns tag name
         */
        std::string name() const;

        /**
         * Allows setting tag attributes directly like tag["attr"] = "value"
         */
        std::string& operator[](const std::string& name);

        /**
         * Sets tag attribute
         */
        void setAttribute(const std::string& name, const std::string& value);

        /**
         * Sets tag content
         */
        void setContent(const std::string& content);

        /**
         * Sets tag name
         */
        void setName(const std::string& tagName);

        /**
         * Writes tag begin
         */
        virtual void writeBegin(std::ostream& os) const = 0;

        /**
         * Writes tag content
         */
        virtual void writeContent(std::ostream& os) const = 0;

        /**
         * Writes tag end
         */
        virtual void writeEnd(std::ostream& os) const = 0;

        /**
         * Writes single tag
         */
        virtual void writeSingle(std::ostream& os) const = 0;
    private:
        std::string tag_name_;
        std::string content_;
        Attributes attrs_;
};

std::ostream& operator<<(std::ostream& os, const Tag& tag);

inline std::string Tag::name() const {
    return tag_name_;
}

inline std::string& Tag::operator [](const std::string& key) {
    return attrs_[key];
}

}

#endif /* TAG_H_ */
