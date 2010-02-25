/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef TEXTEXPORTER_H_
#define TEXTEXPORTER_H_

#include "genericexporter.h"

namespace CIF
{

class TextExporter: public GenericExporter
{
    public:
        TextExporter(CEDPage * page, const FormatOptions& opts = FormatOptions());
    private:
        void writeCharacter(std::ostream& os, CEDChar * chr);
        void writeLineEnd(std::ostream& os, CEDLine * line);
        void writePageBegin(std::ostream& os);
        void writePageEnd(std::ostream& os);
        void writeParagraphBegin(std::ostream& os, CEDParagraph * par);
        void writeParagraphEnd(std::ostream& os, CEDParagraph * par);
        void writePicture(std::ostream& os, CEDChar * picture);
        void writeTableBegin(std::ostream& os, CEDParagraph * table);
        void writeTableEnd(std::ostream& os, CEDParagraph * table);
};

}

#endif /* TEXTEXPORTER_H_ */
