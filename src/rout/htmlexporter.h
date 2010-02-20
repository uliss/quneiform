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

#ifndef HTMLEXPORTER_H_
#define HTMLEXPORTER_H_

#include <map>
#include "genericexporter.h"

namespace CIF
{

class Iconv;

class HtmlExporter: public GenericExporter
{
    public:
        HtmlExporter(CEDPage * page, const FormatOptions& opts);
        ~HtmlExporter();
    private:
        std::string escapeHtmlSpecialChar(unsigned char code);
        void setEncodings();
        void writeCharacter(std::ostream& os, CEDChar * chr);
        virtual void writeDoctype(std::ostream& os);
        virtual void writeFontStyle(std::ostream& os, long style);
        void writeLineEnd(std::ostream& os, CEDLine * line);
        virtual void writeMeta(std::ostream& os);
        void writePageBegin(std::ostream& os);
        void writePageEnd(std::ostream& os);
        void writeParagraphBegin(std::ostream& os, CEDParagraph * par);
        void writeParagraphEnd(std::ostream& os, CEDParagraph * par);
        void writePicture(std::ostream& os, CEDChar * picture);
        void writeTableBegin(std::ostream& os, CEDParagraph * table);
        void writeTableEnd(std::ostream& os, CEDParagraph * table);
        virtual void writeTitle(std::ostream& os);
    protected:
        typedef std::map<std::string, std::string> Attributes;
        void writeAttributes(std::ostream& os, const Attributes& attr);
        void writeSingleTag(std::ostream& os, const std::string& tagName, const Attributes& attrs);
        void writeStartTag(std::ostream& os, const std::string&  tagName);
        void writeStartTag(std::ostream& os, const std::string&  tagName, const Attributes& attr);
    private:
        Iconv * converter_;
        int num_lines_;
        long current_font_style_;
};

}

#endif /* HTMLEXPORTER_H_ */
