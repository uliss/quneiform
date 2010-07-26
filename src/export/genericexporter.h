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

#ifndef GENERICEXPORTER_H_
#define GENERICEXPORTER_H_

#include <vector>
#include "exporter.h"
#include "common/size.h"
#include "common/iconv_local.h"

namespace CIF
{

class CEDPage;
class CEDSection;
class CEDParagraph;
class CEDLine;
class CEDChar;
class CEDColumn;
class CEDPicture;

class GenericExporter: public Exporter
{
    public:
        GenericExporter(CEDPage * page, const FormatOptions& opts);

        /**
         * Returns number of exported characters
         */
        int numChars() const;

        /**
         * Returns number of exported columns
         */
        int numColumns() const;

        /**
         * Returns number of exported frames
         */
        int numFrames() const;

        /**
         * Returns number of exported lines
         */
        int numLines() const;

        /**
         * Returns number of exported paragraphs
         */
        int numParagraphs() const;

        /**
         * Returns number of exported pictures
         */
        int numPictures() const;

        /**
         * Returns number of exported sections
         */
        int numSections() const;

        /**
         * Returns number of exported tables
         */
        int numTables() const;

        /**
         * Controls empty line export
         * @param value if true - empty lines skipped
         */
        void setSkipEmptyLines(bool value);

        /**
         * Controls empty paragraphs export
         * @param value if true - empty paragraphs skipped
         */
        void setSkipEmptyParagraphs(bool value);

        /**
         * Controls pictures export
         * @param value if true - pictures skipped
         */
        void setSkipPictures(bool value);

        /**
         * Checks if empty lines skipped
         */
        bool skipEmptyLines() const;

        /**
         * Checks if empty paragraphs skipped
         */
        bool skipEmptyParagraphs() const;

        /**
         * Checks if empty lines skipped
         */
        bool skipLine(CEDLine& line) const;

        /**
         * Checks if paragraph skipped
         */
        bool skipParagraph(CEDParagraph& par) const;

        /**
         * Checks if pictures skipped
         */
        bool skipPictures() const;
    protected:
        /**
         * Returns number of characters in given paragraph
         */

        static int charNumInParagraph(CEDParagraph& par);
        /**
         * Checks if given paragraph is empty
         */
        static bool isEmptyParagraph(CEDParagraph& par);

        /**
         * Checks if line break needed
         */
        bool isLineBreak(const CEDLine& line) const;

        /**
         * Makes picture path for export
         */
        std::string makePicturePath(const CEDPicture& picture);

        /**
         * Makes relative picture path
         */
        std::string makePicturePathRelative(const CEDPicture& picture);

        /**
         * Returns pointer to default output stream
         * @see setOutputStream()
         */
        std::ostream * outputStream();

        /**
         * Saves given picture
         * @see createPicturesFolder
         */
        void savePicture(const CEDPicture& picture);

        /**
         * Saves picture to file
         */
        void savePictureData(const CEDPicture& picture, const std::string& filepath);

        /**
         * Saves picture to stream
         */
        void savePictureData(const CEDPicture& picture, std::ostream& os);

        /**
         * Sets pointer to default output stream
         * @see outputStream()
         */
        void setOutputStream(std::ostream * os);
    public:
        void exportChar(CEDChar& chr);

        /**
         * Exports columns
         * calls column write functions:
         *   - writeColumnBegin
         *   - writeColumnEnd
         *  @see writeColumnBegin(), writeColumnEnd()
         */
        void exportColumn(CEDColumn& col);

        /**
         * Exports frame
         */
        void exportFrame(CEDFrame& frame);

        /**
         * Exports line
         * calls line write functions:
         *  - writeLineBegin
         *  - writeLineEnd
         */
        void exportLine(CEDLine& line);

        /**
         * Export page
         * calls write page functions:
         *  - writePageBegin
         *  - writePageEnd
         */
        void exportPage(CEDPage& page);

        /**
         * Exports paragraph
         * calls paragraph write functions:
         *  - writeParagraphBegin
         *  - writeParagraphEnd
         */
        void exportParagraph(CEDParagraph& par);

        /**
         * Exports picture
         */
        void exportPicture(CEDPicture& pict);

        /**
         * Exports section
         * calls section write functions:
         *  - writeSectionBegin
         *  - writeSectionEnd
         */
        void exportSection(CEDSection& sect);

        void exportTable(CEDTable& table);
    protected:
        virtual std::string createPicturesFolder();
        void doExport(std::ostream& os);

        /**
         * Returns pointer to exported page
         */
        CEDPage * page();
        const CEDPage * page() const;

        /**
         * Makes picture filename
         */
        std::string makePictureName(const CEDPicture& picture);

        /**
         * Writes character to output stream
         */
        virtual void writeCharacter(CEDChar& chr);

        virtual void writeCharacterBegin(CEDChar& chr);
        virtual void writeCharacterEnd(CEDChar& chr);

        /**
         * Reimplement this function if you want some actions before column export
         * @see exportColumn, writeColumnEnd
         */
        virtual void writeColumnBegin(CEDColumn& col);

        /**
         * Reimplement this function if you want some actions after column export
         * @see exportColumn, writeColumnBegin
         */
        virtual void writeColumnEnd(CEDColumn& col);

        /**
         * Reimplement this function if you want some actions before frame export
         * @see exportFrame, writeFrameEnd
         */
        virtual void writeFrameBegin(CEDFrame& frame);

        /**
         * Reimplement this function if you want some actions after frame export
         * @see exportFrame, writeFrameBegin
         */
        virtual void writeFrameEnd(CEDFrame& frame);

        /**
         * Called before writeLine performed
         * @see writeLine and writeLineEnd
         */
        virtual void writeLineBegin(CEDLine& line);

        /**
         * Called after writeLine performed
         * @see writeLine and writeLineBegin
         */
        virtual void writeLineEnd(CEDLine& line);

        /**
         * Called before writePage
         * @see writePage and writePageEnd
         */
        virtual void writePageBegin(CEDPage& page);

        /**
         * Called after writePage
         * @see writePage and writePageBegin
         */
        virtual void writePageEnd(CEDPage& page);

        /**
         * Called before writePragraph
         */
        virtual void writeParagraphBegin(CEDParagraph& par);

        /**
         * Called after writePragraph
         */
        virtual void writeParagraphEnd(CEDParagraph& par);

        /**
         * Writes picture
         */
        virtual void writePicture(CEDPicture& pict);

        /**
         * Called before writeSection
         */
        virtual void writeSectionBegin(CEDSection& sect);

        /**
         * Called after writeSection
         */
        virtual void writeSectionEnd(CEDSection& sect);
    private:
        CEDPage * page_;
        std::ostream * os_;
        int num_chars_;
        int num_columns_;
        int num_frames_;
        int num_lines_;
        int num_paragraphs_;
        int num_pictures_;
        int num_sections_;
        int num_tables_;
        bool skip_pictures_;
        bool skip_empty_paragraphs_;
        bool skip_empty_lines_;
    protected:
        Iconv converter_;
};

}

#endif /* GENERICEXPORTER_H_ */
