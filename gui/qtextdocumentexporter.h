/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef QTEXTDOCUMENTEXPORTER_H
#define QTEXTDOCUMENTEXPORTER_H

#include <QTextCursor>
#include <QTextDocument>
#include <QTextFormat>
#include <QMap>
#include "export/genericexporter.h"

namespace cf {
class Element;
class CEDChar;
class CEDPage;
class CEDLine;
class CEDParagraph;
class CEDPicture;
class FormatOptions;
}

class QTextTable;
class QTextTableFormat;

class QTextDocumentExporter : public cf::GenericExporter
{
public:
    QTextDocumentExporter(cf::CEDPage * page, const cf::FormatOptions& opts);

    enum TextItemProperty {
        BBOX = QTextFormat::UserProperty + 1,
        ALTERNATIVES = QTextFormat::UserProperty + 2
    };

    static const int BlockType = QTextFormat::UserProperty + 3;

    enum ElementType {
        PAGE = 1,
        SECTION,
        COLUMN,
        PARAGRAPH
    };

    /**
      * Clears document
      */
    void clear();

    /**
      * Returns document cursor
      */
    QTextCursor& cursor();

    /**
      * Returns pointer to document
      */
    QTextDocument * document();

    /**
      * Sets output document
      */
    void setDocument(QTextDocument * doc);
protected:
    void writePageBegin(cf::CEDPage& page);
    void writePageEnd(cf::CEDPage& page);
    void writeParagraphBegin(cf::CEDParagraph& par);
    void writeParagraphEnd(cf::CEDParagraph& par);
    void writeCharacter(cf::CEDChar& chr);
    void writeColumnBegin(cf::CEDColumn& col);
    void writeLineEnd(cf::CEDLine& line);
    void writePicture(cf::CEDPicture& pic);
    void writeSectionBegin(cf::CEDSection& section);
    void writeSectionEnd(cf::CEDSection& section);
private:
    typedef QMap<QString, QVariant> AltMap;
    AltMap charAlternatives(const cf::CEDChar& chr) const;
    void exportElementColor(QTextFormat& format, const cf::Element& el) const;
    void exportElementBgColor(QTextFormat& format, const cf::Element& el) const;
    void exportElementBBox(QTextFormat& format, const cf::Element& el) const;
    void exportCharAlternatives(QTextCharFormat& format, const cf::CEDChar& chr) const;
    void exportCharBold(QTextCharFormat& format, const cf::CEDChar& chr) const;
    void exportCharItalic(QTextCharFormat& format, const cf::CEDChar& chr) const;
    void exportCharUnderline(QTextCharFormat& format, const cf::CEDChar& chr) const;
    void exportCharFontSize(QTextCharFormat& format, const cf::CEDChar& chr) const;
    void insertColumnTable(int columns, const QTextTableFormat& fmt);
    void insertSectionFrame(cf::CEDSection& section);
    void insertSectionTable(cf::CEDSection& section);
    void moveCursorToColumn(int column);
private:
    QTextCursor cursor_;
    QTextDocument * doc_;
    QTextTable * column_table_;
    int column_num_;
    int line_num_in_par_;
    int par_line_count_;
    bool skip_columns_;
};

#endif // QTEXTDOCUMENTEXPORTER_H
