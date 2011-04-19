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

namespace cf {
    class CEDPage;
}

class QTextDocument;
class QTextBlock;
class QTextFrame;

class CEDPageExporter
{
public:
    CEDPageExporter();
    ~CEDPageExporter();
    QTextCursor * cursor();
    QTextDocument * document();
    void doExport(QTextDocument * doc, cf::CEDPage * page);
    cf::CEDPage * page();
private:
    void exportBlock(const QTextBlock& block);
    void exportColumnTable(QTextTable * table);
    void exportPage();
    void exportSection(QTextFrame * frame);
private:
    QTextDocument * doc_;
    cf::CEDPage * page_;
    QTextCursor cursor_;
};

#endif // CEDPAGEEXPORTER_H
