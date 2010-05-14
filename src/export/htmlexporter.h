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
    protected:
        virtual std::string fontStyleTag(int style) const;

        /**
         * Writes HTML document type
         */
        virtual void writeDoctype(std::ostream& os);

        /**
         * Writes <br/> tag if needed
         */
        virtual void writeLineBreak(std::ostream& os);

        /**
         * Writes HTML meta information in <meta> tags
         */
        virtual void writeMeta(std::ostream& os);

        /**
         * Writes HTML header, meta info and <body> tag
         */
        virtual void writePageBegin(std::ostream& os, CEDPage * page);

        /**
         * Writes </body> and </html> tags
         */
        virtual void writePageEnd(std::ostream& os, CEDPage * page);

        /**
         * Writes <p> tag
         */
        virtual void writeParagraphBegin(std::ostream& os, CEDParagraph * par);

        /**
         * Writes </p> tag
         */
        virtual void writeParagraphEnd(std::ostream& os, CEDParagraph * par);

        /**
         * Writes <img tag
         */
        virtual void writePicture(std::ostream& os, CEDChar * picture);

        /**
         * Writes <table> tag
         */
        virtual void writeTableBegin(std::ostream& os, CEDParagraph * table);

        /**
         * Writes </table> tag
         */
        virtual void writeTableEnd(std::ostream& os, CEDParagraph * table);

        /**
         * Writes HTML title in header
         */
        virtual void writeTitle(std::ostream& os);
};

}

#endif /* HTMLEXPORTER_H_ */
