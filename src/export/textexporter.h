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
        TextExporter(CEDPage * page, const FormatOptions& opts);

        /**
         * Appends to the end of existing file
         */
        void appendTo(const std::string& filename);

        /**
         * Exports to stream
         * @note BOM will be inserted on __APPLE__ platform
         * @see writeBom
         */
        void exportTo(std::ostream& os);
    protected:
        /**
         * Preprocess line buffer
         * if overloaded can be used for character replacement
         */
        virtual void bufferPreprocess(std::string& buffer);

        /**
         * Writes character to line buffer
         */
        virtual void writeCharacter(std::ostream& os, CEDChar * chr);

        /**
         * Writes line break if needed
         */
        virtual void writeLineBreak(std::ostream& os, CEDLine * line);

        /**
         * Writes preprocessed line buffer content to output stream
         * after that line buffer is cleared
         * @see clearLineBuffer
         */
        virtual void writeLineBuffer(std::ostream& os, CEDLine * line);

        /**
         * Writes line and optional line break if needed
         * @see writeLineBreak
         */
        virtual void writeLineEnd(std::ostream& os, CEDLine * line);

        /**
         * Writes new line after page
         */
        virtual void writePageEnd(std::ostream& os, CEDPage * page);

        virtual void writeParagraphBegin(std::ostream& os, CEDParagraph * par);

        /**
         * Writes new line after paragraph
         */
        virtual void writeParagraphEnd(std::ostream& os, CEDParagraph * par);

        /**
         * Writes stub "[picture]" on picture place
         */
        virtual void writePicture(std::ostream& os, CEDChar * picture);
    protected:
        /**
         * Clears line buffer
         */
        void clearLineBuffer();

        /**
         * Returns raw line content
         */
        std::ostringstream& lineBuffer();

        /**
         * Returns prepared line content
         */
        std::string lineBufferPrepared();

        /**
         * Returns number of line in paragraph that not exported yet
         */
        int lineLeftInParagraph() const;

        /**
         * Writes BOM mark before text document if needed
         */
        void writeBOM(std::ostream& os);

        /**
         * Writes raw line buffer content to output stream
         * after that line buffer is cleared
         */
        void writeLineBufferRaw(std::ostream& os);
    private:
        std::ostringstream line_buffer_;
        int lines_left_in_paragraph_;
};

}

#endif /* TEXTEXPORTER_H_ */
