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

#ifndef HOCREXPORTER_H_
#define HOCREXPORTER_H_

#include <vector>
#include "htmlexporter.h"
#include "common/rect.h"

namespace CIF
{

class HocrExporter: public HtmlExporter
{
    public:
        HocrExporter(CEDPage * page, const FormatOptions& opts = FormatOptions());
        typedef std::vector<Rect> RectList;
    protected:
        void writeCharacter(std::ostream& os, CEDChar * chr);
        void writeCharBBoxesInfo(std::ostream& os);
        void writeLineBegin(std::ostream& os, CEDLine * line);
        void writeLineEnd(std::ostream& os, CEDLine * line);
        void writeMeta(std::ostream& os);
        void writePageBegin(std::ostream& os);
        void writePageEnd(std::ostream& os);
        void writeParagraphBegin(std::ostream& os, CEDParagraph * par);
    private:
        void addCharBBox(CEDChar * chr);
    private:
        RectList rects_;
        // true if last none-space character was in line (i.e had a valid bbox).
        bool is_in_line_;
        Rect line_rect_;
};

}

#endif /* HOCREXPORTER_H_ */
