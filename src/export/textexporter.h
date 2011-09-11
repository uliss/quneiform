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

namespace cf
{

class TextExporter: public GenericExporter
{
    public:
        TextExporter(CEDPagePtr page, const FormatOptions& opts);

        /**
         * Appends to the end of existing file
         */
        void appendTo(const std::string& filename);
    protected:
        /**
         * Writes character to line buffer
         */
        virtual void writeCharacter(CEDChar& chr);

        /**
         * Writes line breaks
         */
        virtual void writeLineBreak();

        virtual void writeLineBegin(CEDLine& line);

        /**
         * Writes line and optional line break if needed
         * @see writeLineBreak
         */
        virtual void writeLineEnd(CEDLine& line);

        /**
         * Writes new line after page
         */
        virtual void writePageEnd(CEDPage& page);

        virtual void writeParagraphBegin(CEDParagraph& par);

        /**
         * Writes new line after paragraph
         */
        virtual void writeParagraphEnd(CEDParagraph& par);

        /**
         * Writes stub "[picture]" on picture place
         */
        virtual void writePicture(CEDPicture& picture);

        /**
         * Writes new line after section
         */
        virtual void writeSectionEnd(CEDSection& sect);
    protected:
        std::ostringstream& buffer();
        void flushBuffer();
        void flushBufferConverted();
        void flushBufferRaw();

        /**
         * Writes BOM mark before text document if needed
         */
        void writeBOM(std::ostream& os);

        /**
         * Exports to stream
         * @note BOM will be inserted on __APPLE__ platform
         * @see writeBom
         */
        void doExport(std::ostream& os);
    private:
        bool notLastLine() const {
            return lines_left_in_paragraph_ > 1;
        }
    private:
        std::ostringstream buffer_;
        int lines_left_in_paragraph_;
    protected:
        int elements_left_in_line_;
        bool remove_last_line_hyphen_;
};

inline std::ostringstream& TextExporter::buffer() {
    return buffer_;
}

}

#endif /* TEXTEXPORTER_H_ */
