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
        void writeLineEnd(std::ostream& os, CEDLine * line);
        virtual void writeMeta(std::ostream& os);
        void writePageBegin(std::ostream& os);
        void writePageEnd(std::ostream& os);
        void writeParagraphBegin(std::ostream& os, CEDParagraph * par);
        void writeParagraphEnd(std::ostream& os, CEDParagraph * par);
        void writePicture(std::ostream& os, CEDChar * picture);
        virtual void writeTitle(std::ostream& os);
    private:
        Iconv * converter_;
        int num_lines_;
};

}

#endif /* HTMLEXPORTER_H_ */
