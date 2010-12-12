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

namespace cf {

class RectExporter: public GenericExporter
{
public:
    RectExporter(CEDPage * page);
    typedef QList<QRect> RectList;
    void collect();
    const RectList& chars() const;
    const RectList& paragraphs() const;
    const RectList& pictures() const;
protected:
    void doExport(std::ostream& os);
private:
    void addCharBBox(CEDChar& chr);
    void writeCharacterEnd(CEDChar &chr);
    void writeLineEnd(CEDLine& line);
    void writeParagraphEnd(CEDParagraph& par);
    void writePicture(CEDPicture& pict);
private:
    RectList paragraphs_;
    RectList lines_;
    RectList pictures_;
    RectList chars_;
};

}

#endif // RECTEXPORTER_H
