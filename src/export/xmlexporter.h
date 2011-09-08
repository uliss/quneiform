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

#ifndef XMLEXPORTER_H_
#define XMLEXPORTER_H_

#include <map>
#include <string>
#include "textexporter.h"

namespace cf
{

class XmlExporter: public TextExporter
{
    public:
        XmlExporter(CEDPagePtr page, const FormatOptions& opts);

        /**
         * Append to the end of existing XML document
         * @note not supported yet
         */
        void appendTo(const std::string& filename);

        typedef std::map<std::string, std::string> Attributes;

        /**
         * Replaces '<', '>', '"', '&' and ''\' with XML entities like &amp;
         * otherwise returns original character as string
         */
        static std::string escapeSpecialChar(uchar code);

        /**
         * Writes xml attributes to output stream
         */
        void writeAttributes(const Attributes& attrs);

        /**
         * Writes close tag to output stream
         */
        void writeCloseTag(const std::string& tagName, const std::string& newline = "");

        /**
         * Writes single tag to output stream
         */
        void writeSingleTag(const std::string& tagName, const Attributes& attrs,
                const std::string& newline = "");

        /**
         * Writes single tag to output stream
         */
        void writeSingleTag(const std::string& tagName, const std::string& newline = "");

        /**
         * Writes start tag to output stream
         */
        void writeStartTag(const std::string& tagName, const std::string& newline = "");

        /**
         * Writes start tag to output stream
         */
        void writeStartTag(const std::string& tagName, const Attributes& attrs,
                const std::string& newline = "");

        /**
         * Writes tag to output stream
         * @note tagText not escaped for XML special chars
         */
        void writeTag(const std::string& tagName, const std::string& tagText,
                const Attributes& attrs = Attributes(), const std::string& newline = "");
    protected:
        /**
         * Writes escaped character to line buffer
         */
        virtual void writeCharacter(CEDChar& chr);
    public:
        static void writeXmlDeclaration(std::ostream& os, const std::string& encoding = "UTF-8");
        static void writeAttributes(std::ostream& os, const Attributes& attrs);
        static void writeCloseTag(std::ostream& os, const std::string& tagName);
        static void writeSingleTag(std::ostream& os, const std::string& tagName,
                const Attributes& attrs = Attributes());
        static void writeStartTag(std::ostream& os, const std::string& tagName,
                const Attributes& attrs = Attributes());
        static void writeTag(std::ostream& os, const std::string& tagName, const std::string& tagText,
                const Attributes& attrs = Attributes());
};

}

#endif /* XMLEXPORTER_H_ */
