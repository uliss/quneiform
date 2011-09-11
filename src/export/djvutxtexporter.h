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

#ifndef DJVUTXTEXPORTER_H_
#define DJVUTXTEXPORTER_H_

#include <deque>
#include <sstream>
#include "textexporter.h"
#include "common/rect.h"

namespace cf
{

class DjvuTxtExporter: public TextExporter
{
    public:
        DjvuTxtExporter(CEDPagePtr page, const FormatOptions& opts);
        void writeCharacter(CEDChar& chr);
        void writeCharacterBegin(CEDChar& chr);
        void writeLineBegin(CEDLine& line);
        void writeLineBreak();
        void writeLineEnd(CEDLine& line);
        void writePageBegin(CEDPage& page);
        void writePageEnd(CEDPage& page);
        void writeParagraphBegin(CEDParagraph& par);
        void writeParagraphEnd(CEDParagraph& par);
    private:
        void calcWordRectList(CEDLine& line);
        void closeWord();
        void startWord();
        void writeIndent();
        void writeRect(const Rect& r);
        void writeWordBegin();
        void writeWordBuffer();
        void writeWordEnd();
    private:
        typedef std::deque<Rect> WordRectList;
        WordRectList word_rects_;
        std::ostringstream word_buffer_;
        int indent_level_;
        bool word_flag_;
};

}

#endif /* DJVUTXTEXPORTER_H_ */
