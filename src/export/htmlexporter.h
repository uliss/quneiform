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
#include <vector>
#include "xmlexporter.h"

namespace CIF
{

class HtmlExporter: public XmlExporter
{
    public:
        HtmlExporter(CEDPage * page, const FormatOptions& opts = FormatOptions());
        ~HtmlExporter();
    protected:
        void writeCharacter(std::ostream& os, CEDChar * chr);
        virtual void writeDoctype(std::ostream& os);
        virtual void writeFontStyle(std::ostream& os, long style);
        void writeFontStyleBegin(std::ostream& os, long newStyle, int style);
        void writeFontStyleClose(std::ostream& os, long newStyle, int style);
        void writeLineBreak(std::ostream& os, CEDLine * line);
        virtual void writeMeta(std::ostream& os);
        void writePageBegin(std::ostream& os, CEDPage * page);
        void writePageEnd(std::ostream& os, CEDPage * page);
        void writeParagraphBegin(std::ostream& os, CEDParagraph * par);
        void writeParagraphEnd(std::ostream& os, CEDParagraph * par);
        void writePicture(std::ostream& os, CEDChar * picture);
        void writeTableBegin(std::ostream& os, CEDParagraph * table);
        void writeTableEnd(std::ostream& os, CEDParagraph * table);
        virtual void writeTitle(std::ostream& os);
    protected:
        std::string fontStyleBegin(int style);
        std::string fontStyleEnd(int style);
        void closeFontStyle();
    private:
        int lines_left_;
        long current_font_style_;
        typedef std::vector<int> FontStyleStack;
        FontStyleStack font_styles_;
};

}

#endif /* HTMLEXPORTER_H_ */
