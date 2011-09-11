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

#ifndef FB2EXPORTER_H
#define FB2EXPORTER_H

#include <vector>
#include "xmlexporter.h"

namespace cf {

class FB2FormatOptions : public FormatOptions {
public:
    FB2FormatOptions();
private:
    std::string author_;
    std::string title_;
    std::string book_title_;
    std::string book_name_;
    std::string annotation_;
    std::string coverpage_;
    std::string date_;
    std::string src_ocr_;
    std::string id_;
    std::string publisher_;
    std::string city_;
    std::string year_;
    language_t lang_;
    language_t src_lang_;
};

class CEDPicture;

class FB2Exporter : public XmlExporter
{
public:
    FB2Exporter(CEDPagePtr page, const cf::FormatOptions& opt);
protected:
    void writeCharacterBegin(CEDChar& c);
    void writePageBegin(CEDPage& page);
    void writePageEnd(CEDPage &page);
    void writeParagraphBegin(CEDParagraph& par);
    void writeParagraphEnd(CEDParagraph& par);
    void writePicture(CEDPicture& picture);
    void writeSectionBegin(CEDSection& sect);
    void writeSectionEnd(CEDSection& sect);
private:
    void changeCharacterFontStyle(int new_style);
    void writeBinary();
    void writeDescription();
    void writeDocumentInfo();
    void writeDocumentLanguage();
    void writeDocumentTitle();
    void writeFontStyleBegin(int style);
    void writeFontStyleEnd(int style);
    void writePictures();
    void writePictureBase64(const CEDPicture& p);
private:
    typedef std::vector<CEDPicture*> PictList;
private:
    int prev_char_style_;
    PictList pictures_;
};

}

#endif // FB2EXPORTER_H
