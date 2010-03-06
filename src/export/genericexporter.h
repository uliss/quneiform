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

class GenericExporter: public Exporter
{
    public:
        GenericExporter(CEDPage * page, const FormatOptions& opts);
        int numChars() const;
        int numColumns() const;
        int numFrames() const;
        int numLines() const;
        int numParagraphs() const;
        int numPictures() const;
        int numSections() const;
        int numTables() const;
        CEDPage * page();
        void setSkipEmptyLines(bool value);
        void setSkipEmptyParagraphs(bool value);
        void setSkipPictures(bool value);
        bool skipEmptyLines() const;
        bool skipEmptyParagraphs() const;
        bool skipPictures() const;
    protected:
        int charNumInParagraph(CEDParagraph * par);
        bool isEmptyParagraph(CEDParagraph * par);
        std::ostream * outputStream();
        std::string savePicture(CEDChar * picture);
        void savePictureData(CEDChar * picture, const std::string&);
        void setOutputStream(std::ostream * os);
    private:
        void exportColumn(CEDParagraph * col);
        void exportFrame(CEDParagraph * frame);
        void exportLine(CEDLine * line);
        void exportObjects(CEDParagraph * objects);
        void exportPage();
        void exportParagraph(CEDParagraph* par);
        void exportPicture(CEDChar * picture);
        void exportSection(CEDSection * sect);
        void exportTable(CEDParagraph * table);
        void exportTableCells(CEDParagraph * table);
        void exportTableRow(CEDParagraph * row);
        std::string pictureName(CEDChar * picture);
        int pictureNumber(CEDChar * picture);
    protected:
        virtual std::string createPicturesFolder();
        void doExport(std::ostream& os);
        virtual void exportChar(CEDChar * chr);
        virtual void writeCharacter(std::ostream& os, CEDChar * chr);
        virtual void writeColumnBegin(std::ostream& os, CEDParagraph * col);
        virtual void writeColumnEnd(std::ostream& os, CEDParagraph * col);
        virtual void writeFrameBegin(std::ostream& os, CEDParagraph * frame);
        virtual void writeFrameEnd(std::ostream& os, CEDParagraph * frame);
        virtual void writeLineBegin(std::ostream& os, CEDLine * line);
        virtual void writeLineEnd(std::ostream& os, CEDLine * line);
        virtual void writePageBegin(std::ostream& os);
        virtual void writePageEnd(std::ostream& os);
        virtual void writeParagraphBegin(std::ostream& os, CEDParagraph * par);
        virtual void writeParagraphEnd(std::ostream& os, CEDParagraph * par);
        virtual void writePicture(std::ostream& os, CEDChar * pict);
        virtual void writeSectionBegin(std::ostream& os, CEDSection * sect);
        virtual void writeSectionEnd(std::ostream& os, CEDSection * sect);
        virtual void writeTableBegin(std::ostream& os, CEDParagraph * table);
        virtual void writeTableEnd(std::ostream& os, CEDParagraph * table);
        virtual void writeTableRowBegin(std::ostream& os, CEDParagraph * row);
        virtual void writeTableRowEnd(std::ostream& os, CEDParagraph * row);
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
    protected:
        Iconv converter_;
        Size last_picture_size_;
};

}

#endif /* GENERICEXPORTER_H_ */
