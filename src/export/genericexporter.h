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

class pictEntry;

namespace CIF
{

class CEDPage;
class CEDSection;
class CEDParagraph;
class CEDLine;
class CEDChar;

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
         * Checks if line skipped
         */
        bool skipLine(CEDLine * line) const;

        /**
         * Checks if paragraph skipped
         */
        bool skipParagraph(CEDParagraph * par) const;

        /**
         * Checks if pictures skipped
         */
        bool skipPictures() const;

        typedef std::vector<int> styleList;
    protected:
        /**
         * Returns number of characters in given paragraph
         */
        static int charNumInParagraph(CEDParagraph * par);

        /**
         * Checks if given paragraph is empty
         */
        static bool isEmptyParagraph(CEDParagraph * par);

        /**
         * Checks if line break needed
         */
        bool isLineBreak(CEDLine * line) const;

        /**
         * Checks if hyphens removal is needed
         */
        bool isRemoveHyphens(CEDLine * line) const;

        /**
         * Returns pointer to default output stream
         */
        std::ostream * outputStream();

        /**
         * Saves given picture
         * @see createPicturesFolder
         * @return path to saved pictures
         */
        std::string savePicture(CEDChar * picture);

        /**
         * Saves picture to file
         */
        void savePictureData(CEDChar * picture, const std::string& filepath);

        /**
         * Saves picture to stream
         */
        void savePictureData(CEDChar * picture, std::ostream& os);

        /**
         * Sets pointer to default output stream
         */
        void setOutputStream(std::ostream * os);
    private:
        /**
         * Exports columns
         * calls column write functions:
         *   - writeColumnBegin
         *   - writeColumn
         *   - writeColumnEnd
         */
        void exportColumn(CEDParagraph * col);

        /**
         * Exports frames
         * calls frame write functions:
         *  - writeFrameBegin
         *  - writeFrame
         *  - writeFrameEnd
         */
        void exportFrame(CEDParagraph * frame);

        /**
         * Exports line
         * calls line write functions:
         *  - writeLineBegin
         *  - writeLine
         *  - writeLineEnd
         */
        void exportLine(CEDLine * line);

        /**
         * Exports list of objects
         */
        void exportObjects(CEDParagraph * objects);

        /**
         * Export page
         * calls write page functions:
         *  - writePageBegin
         *  - writePage
         *  - writePageEnd
         */
        void exportPage(CEDPage * page);

        /**
         * Exports paragraph
         * calls paragraph write functions:
         *  - writeParagraphBegin
         *  - writeParagraph
         *  - writeParagraphEnd
         */
        void exportParagraph(CEDParagraph* par);

        /**
         * Exports picture
         */
        void exportPicture(CEDChar * picture);

        /**
         * Exports section
         * calls section write functions:
         *  - writeSectionBegin
         *  - writeSection
         *  - writeSectionEnd
         */
        void exportSection(CEDSection * sect);

        void exportTable(CEDParagraph * table);
        void exportTableCells(CEDParagraph * table);
        void exportTableRow(CEDParagraph * row);
    protected:
        virtual std::string createPicturesFolder();
        void doExport(std::ostream& os);
        virtual void exportChar(CEDChar * chr);

        /**
         * Returns pointer to exported page
         */
        CEDPage * page();

        /**
         * Returns pointer to picture entry
         * @throw Exception if entry not founds
         */
        pictEntry * pictureEntry(CEDChar * picture) const;

        /**
         * Makes picture filename
         */
        std::string pictureName(CEDChar * picture);

        /**
         * Resets font styles
         */
        void resetFontStyle(std::ostream& os);

        /**
         * Writes character to output stream
         */
        virtual void writeCharacter(std::ostream& os, CEDChar * chr);

        /**
         * Exports column content
         * @see writeColumnBegin and writeColumnEnd
         */
        virtual void writeColumn(std::ostream& os, CEDParagraph * col);

        /**
         * Called before writeColumn
         * @see writeColumn
         */
        virtual void writeColumnBegin(std::ostream& os, CEDParagraph * col);

        /**
         * Called after writeColumn
         * @see writeColumn
         */
        virtual void writeColumnEnd(std::ostream& os, CEDParagraph * col);

        /**
         * Writes font styles
         */
        void writeFontStyle(std::ostream& os, CEDChar * chr);

        /**
         * Writes style begin to output stream
         */
        virtual void writeFontStyleBegin(std::ostream& os, int style);

        /**
         * Writes font style end to output stream
         */
        virtual void writeFontStyleEnd(std::ostream& os, int style);

        /**
         * Exports frame content
         */
        virtual void writeFrame(std::ostream& os, CEDParagraph * frame);

        /**
         * Called before writeFrame
         */
        virtual void writeFrameBegin(std::ostream& os, CEDParagraph * frame);

        /**
         * Called after writeFrame
         */
        virtual void writeFrameEnd(std::ostream& os, CEDParagraph * frame);

        /**
         * Exports line characters. You should redefine it if other character order
         * export required.
         * @see writeLineBegin and writeLineEnd
         */
        virtual void writeLine(std::ostream& os, CEDLine * line);

        /**
         * Called before writeLine performed
         * @see writeLine and writeLineEnd
         */
        virtual void writeLineBegin(std::ostream& os, CEDLine * line);

        /**
         * Called after writeLine performed
         * @see writeLine and writeLineBegin
         */
        virtual void writeLineEnd(std::ostream& os, CEDLine * line);

        /**
         * Exports page sections.
         * @see writePageBegin and writePageEnd
         */
        virtual void writePage(std::ostream& os, CEDPage * page);

        /**
         * Called before writePage
         * @see writePage and writePageEnd
         */
        virtual void writePageBegin(std::ostream& os, CEDPage * page);

        /**
         * Called after writePage
         * @see writePage and writePageBegin
         */
        virtual void writePageEnd(std::ostream& os, CEDPage * page);

        /**
         * Exports paragraph lines
         */
        virtual void writeParagraph(std::ostream& os, CEDParagraph * par);

        /**
         * Called before writePragraph
         */
        virtual void writeParagraphBegin(std::ostream& os, CEDParagraph * par);

        /**
         * Called after writePragraph
         */
        virtual void writeParagraphEnd(std::ostream& os, CEDParagraph * par);

        /**
         * Writes picture
         */
        virtual void writePicture(std::ostream& os, CEDChar * pict);

        /**
         * Exports section content
         */
        virtual void writeSection(std::ostream& os, CEDSection * sect);

        /**
         * Called before writeSection
         */
        virtual void writeSectionBegin(std::ostream& os, CEDSection * sect);

        /**
         * Called after writeSection
         */
        virtual void writeSectionEnd(std::ostream& os, CEDSection * sect);
        virtual void writeTableBegin(std::ostream& os, CEDParagraph * table);
        virtual void writeTableEnd(std::ostream& os, CEDParagraph * table);
        virtual void writeTableRowBegin(std::ostream& os, CEDParagraph * row);
        virtual void writeTableRowEnd(std::ostream& os, CEDParagraph * row);

        static styleList styleEnd(int style_prev, int style_current);
        static styleList styleBegin(int style_prev, int style_current);
    private:
        CEDPage * page_;
        bool first_paragraph_;
        bool no_pictures_;
        std::ostream * os_;
        int num_chars_;
        int num_columns_;
        int num_frames_;
        int num_lines_;
        int num_paragraphs_;
        int num_pictures_;
        int num_sections_;
        int num_tables_;
        int table_nesting_level_;
        bool skip_empty_paragraphs_;
        bool skip_empty_lines_;
        int previous_style_;
    protected:
        Iconv converter_;
        pictEntry * current_picture_;
};

}

#endif /* GENERICEXPORTER_H_ */
