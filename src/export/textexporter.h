/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef TEXTEXPORTER_H_
#define TEXTEXPORTER_H_

#include <sstream>
#include "genericexporter.h"

namespace CIF
{

class TextExporter: public GenericExporter
{
    public:
        TextExporter(CEDPage * page, const FormatOptions& opts = FormatOptions());
        void exportChar(CEDChar * chr);
        void exportTo(std::ostream& os);
    protected:
        /**
         * Returns prepared line content
         */
        virtual std::string lineBufferPrepared();

        /**
         * Writes line break if needed
         */
        virtual void writeLineBreak(std::ostream& os, CEDLine * line);
    protected:
        /**
         * Returns raw line content
         */
        std::ostringstream& lineBuffer();

        /**
         * Writes BOM mark before text document if needed
         */
        void writeBOM(std::ostream& os);

        /**
         * Writes line and optional line break if needed
         * @see writeLineBreak
         */
        void writeLineEnd(std::ostream& os, CEDLine * line);

        /**
         * Writes new line after page
         */
        void writePageEnd(std::ostream& os, CEDPage * page);

        /**
         * Writes new line after paragraph
         */
        void writeParagraphEnd(std::ostream& os, CEDParagraph * par);

        /**
         * Writes stub "[picture]" on picture place
         */
        void writePicture(std::ostream& os, CEDChar * picture);
    private:
        /**
         * Clears line buffer
         */
        void clearLineBuffer();

        /**
         * Writes line buffer content to output stream
         */
        void writeLineBuffer(std::ostream& os, CEDLine * line);
    private:
        std::ostringstream line_buffer_;
};

}

#endif /* TEXTEXPORTER_H_ */
