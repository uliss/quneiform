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

#ifndef CEDPAGEEXPORTER_H
#define CEDPAGEEXPORTER_H

#include <QTextCursor>
#include "language.h"

namespace cf {
    class Element;
    class BlockElement;
    class CEDPage;
    class CEDColumn;
    class CEDSection;
    class CEDParagraph;
    class CEDLine;
    class CEDChar;
    class Iconv;
}

class QTextDocument;
class QTextBlock;
class QTextFrame;
class QTextFragment;
class QTextCharFormat;
class QTextFormat;

class CEDPageExporter
{
public:
    CEDPageExporter();
    explicit CEDPageExporter(const Language& lang);
    ~CEDPageExporter();
    QTextCursor * cursor();
    QTextDocument * document();
    void doExport(QTextDocument * doc, cf::CEDPage * page);
    Language language() const;
    cf::CEDPage * page();
    void setLanguage(const Language& lang);
    void setPage(cf::CEDPage * page);
private:
    std::string convert(const QString& str);
    void exportBBox(cf::Element * el, const QTextFormat& fmt);
    void exportBlock(const QTextBlock& block);
    void exportChar(const QTextFragment& fragment, cf::CEDLine * line);
    void exportCharAlternatives(cf::CEDChar * c, const QTextCharFormat& fmt);
    void exportColumn();
    void exportColumnTable(QTextTable * table);
    void exportFontStyle(cf::CEDChar * c, const QTextCharFormat& fmt);
    void exportPage();
    bool exportPageChild(QTextFrame * child);
    void exportPageChildren(QTextFrame * page);
    void exportParagraph(const QTextBlock& block, cf::CEDColumn * col);
    void exportSection(QTextFrame * frame);
    void exportSectionChildren(QTextFrame * frame, cf::CEDColumn * col);
    void exportString(const QTextFragment& fragment, cf::CEDLine * line);
    cf::CEDChar * makeChar(wchar_t ch, const QTextCharFormat& fmt);
private:
    static void exportMargins(cf::BlockElement * block, const QTextFrame * frame);
    static bool isPage(QTextFrame * page);
    static bool isParagraph(const QTextBlock& par);
    static bool isSection(QTextFrame * section);
    static bool isSectionTable(QTextTable * table);
    static cf::CEDSection * makeSingleColumnSectionLayout(QTextFrame * frame);
private:
    QTextDocument * doc_;
    cf::CEDPage * page_;
    QTextCursor cursor_;
    Language language_;
    cf::Iconv * iconv_;
};

#endif // CEDPAGEEXPORTER_H
