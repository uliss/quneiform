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
        void closeStyle(bool value);

        virtual void writeAlternativesBegin(const CEDChar& chr);
        virtual void writeAlternativesEnd(const CEDChar& chr);

        /**
         * Writes characters to line buffer
         */
        virtual void writeCharacterBegin(std::ostream& os, CEDChar& chr);
        virtual void writeCharacterEnd(std::ostream& os, CEDChar& chr);

        /**
         * Writes HTML document type
         */
        virtual void writeDoctype(std::ostream& os);

        void writeFontStyleBegin(int style);
        void writeFontStyleEnd(int style);

        /**
         * Writes open frame tag
         */
        virtual void writeFrameBegin(std::ostream& os, CEDFrame& frame);

        /**
         * Writes close frame tag
         */
        virtual void writeFrameEnd(std::ostream& os, CEDFrame& frame);

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
        virtual void writePageBegin(std::ostream& os, CEDPage& page);

        /**
         * Writes </body> and </html> tags
         */
        virtual void writePageEnd(std::ostream& os, CEDPage& page);

        /**
         * Writes <p> tag
         */
        virtual void writeParagraphBegin(std::ostream& os, CEDParagraph& par);

        /**
         * Writes </p> tag
         */
        virtual void writeParagraphEnd(std::ostream& os, CEDParagraph& par);

        /**
         * Writes <img> tag
         */
        virtual void writePicture(std::ostream& os, CEDPicture& picture);

        /**
         * Writes <div> for section
         */
        virtual void writeSectionBegin(std::ostream& os, CEDSection& sect);

        /**
         * Writes </div> for section
         */
        virtual void writeSectionEnd(std::ostream& os, CEDSection& sect);

        /**
         * Writes HTML title in header
         */
        virtual void writeTitle(std::ostream& os);
    private:
        int prev_char_style_;
        bool close_style_;
};

}

#endif /* HTMLEXPORTER_H_ */
