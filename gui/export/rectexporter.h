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

#ifndef RECTEXPORTER_H
#define RECTEXPORTER_H

#include <QRect>
#include <QList>

#include "export/genericexporter.h"
#include "block.h"

namespace cf {

class RectExporter: public GenericExporter
{
public:
    RectExporter(CEDPagePtr page);
    typedef QList<Block> BlockList;
    void collect();
    const BlockList& chars() const;
    const BlockList& columns() const;
    const BlockList& lines() const;
    const BlockList& paragraphs() const;
    const BlockList& pictures() const;
    const BlockList& sections() const;
protected:
    void doExport(std::ostream& os);
private:
    void writeCharacterEnd(CEDChar& chr);
    void writeColumnBegin(CEDColumn& col);
    void writeColumnEnd(CEDColumn& col);
    void writeLineBegin(CEDLine& line);
    void writeLineEnd(CEDLine& line);
    void writeParagraphBegin(CEDParagraph& par);
    void writeParagraphEnd(CEDParagraph& par);
    void writePicture(CEDPicture& pict);
    void writeSectionBegin(CEDSection& sect);
    void writeSectionEnd(CEDSection& sect);
private:
    BlockList sections_;
    BlockList columns_;
    BlockList paragraphs_;
    BlockList lines_;
    BlockList pictures_;
    BlockList chars_;
    QRect current_column_;
    QRect current_line_;
    QRect current_par_;
    QRect current_section_;
    bool column_begin_;
    bool line_begin_;
    bool par_begin_;
    bool section_begin_;
};

}

#endif // RECTEXPORTER_H
