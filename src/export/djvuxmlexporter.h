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

#ifndef DJVUXMLEXPORTER_H_
#define DJVUXMLEXPORTER_H_

#include <deque>
#include "common/rect.h"
#include "xmlexporter.h"

namespace CIF
{

class DjvuTxtExporter: public XmlExporter
{
    public:
        DjvuTxtExporter(CEDPage * page, const FormatOptions& opts);
        void writeCharacter(CEDChar& chr);
        void writeCharacterBegin(CEDChar& chr);
        void writeColumnBegin(CEDChar& col);
        void writeColumnEnd(CEDChar& col);
        void writeLineBegin(CEDLine& line);
        void writeLineBreak();
        void writeLineEnd(CEDLine& line);
        void writePageBegin(CEDPage& page);
        void writePageEnd(CEDPage& page);
        void writeParagraphBegin(CEDParagraph& par);
        void writeParagraphEnd(CEDParagraph& par);
        void writeSectionBegin(CEDSection& sect);
        void writeSectionEnd(CEDSection& sect);
    private:
        void calcWordRectList(CEDLine& line);
        void closeWord();
        void setCoordAttr(Attributes& attrs, const Rect& r);
        void startWord();
        void writeIndent();
        void writeWordBegin();
        void writeWordEnd();
    private:
        typedef std::deque<Rect> WordRectList;
        WordRectList word_rects_;
        int indent_level_;
        bool word_flag_;
};

}

#endif /* DJVUXMLEXPORTER_H_ */
