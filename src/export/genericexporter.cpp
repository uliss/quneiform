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

#include <cassert>
#include <cstring>

#include "genericexporter.h"
#include "ced/ced.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"
#include "ced/cedframe.h"
#include "ced/cedcolumn.h"
#include "ced/cedpicture.h"
#include "ced/cedsection.h"
#include "ced/cedparagraph.h"
#include "ced/cedpage.h"
#include "ced/cedtable.h"
#include "common/debug.h"
#include "common/cifconfig.h"
#include "common/imagerawdata.h"
#include "cfcompat.h"
#include "common/helper.h"
#include "common/iconv_local.h"
#include "rout_own.h" // for font_style_t
namespace CIF
{

GenericExporter::GenericExporter(CEDPage * page, const FormatOptions& opts) :
    Exporter(opts), page_(page), os_(NULL), num_chars_(0), num_columns_(0), num_frames_(0),
            num_lines_(0), num_paragraphs_(0), num_pictures_(0), num_sections_(0), num_tables_(0),
            skip_pictures_(false), skip_empty_paragraphs_(false), skip_empty_lines_(false) {

    if (isCharsetConversion())
        converter_.open(inputEncoding(), outputEncoding());
}

int GenericExporter::charNumInParagraph(CEDParagraph& par) {
    int num_of_chars = 0;
    for (int i = 0, num_lines = par.lineCount(); i < num_lines; i++)
        num_of_chars += par.lineAt(i)->elementCount();

    return num_of_chars;
}

std::string GenericExporter::createPicturesFolder() {
    std::string path = makeOutputPictureDir();

    // check if folder already exists
    if (_access(path.c_str(), 0) == 0) {
        Debug() << "[GenericExporter::createPicturesFolder]: folder \"" << path
                << "\" already exists.\n";
        return path;
    }

    if (!CreateDirectory(path.c_str(), 0))
        throw Exception("Can't create folder for pictures: " + path);

    return path;
}

void GenericExporter::doExport(std::ostream& os) {
    if (os.fail() || page_ == NULL)
        throw Exception("[GenericExporter::doExport] invalid stream given");

    setOutputStream(&os);
    page_->exportElement(*this);
}

void GenericExporter::exportChar(CEDChar& chr) {
    num_chars_++;
    writeCharacterBegin(chr);
    writeCharacter(chr);
    writeCharacterEnd(chr);
}

void GenericExporter::exportColumn(CEDColumn& col) {
    num_columns_++;

    writeColumnBegin(col);
    col.exportChildren(*this);
    writeColumnEnd(col);
}

void GenericExporter::exportFrame(CEDFrame& frame) {
    num_frames_++;

    writeFrameBegin(frame);
    frame.exportChildren(*this);
    writeFrameEnd(frame);
}

void GenericExporter::exportLine(CEDLine& line) {
    if (skipLine(line)) {
        if (Config::instance().debugHigh())
            Debug() << "[GenericExporter::exportLine] skipping empty line\n";
        return;
    }

    num_lines_++;
    writeLineBegin(line);
    line.exportChildren(*this);
    writeLineEnd(line);
}

void GenericExporter::exportPage(CEDPage& page) {
    writePageBegin(page);
    page.exportChildren(*this);
    writePageEnd(page);
}

void GenericExporter::exportParagraph(CEDParagraph& par) {
    if (skipParagraph(par)) {
        if (Config::instance().debugHigh())
            Debug() << "[GenericExporter::exportParagraph] skipping empty paragraph\n";
        return;
    }

    num_paragraphs_++;

    writeParagraphBegin(par);
    par.exportChildren(*this);
    writeParagraphEnd(par);
}

void GenericExporter::exportPicture(CEDPicture& picture) {
    if (skipPictures())
        return;

    num_pictures_++;
    writePicture(picture);
}

void GenericExporter::exportSection(CEDSection& sect) {
    num_sections_++;
    writeSectionBegin(sect);
    sect.exportChildren(*this);
    writeSectionEnd(sect);
}

void GenericExporter::exportTable(CEDTable& table) {
    num_tables_++;
    table.exportChildren(*this);
}

bool GenericExporter::isEmptyParagraph(CEDParagraph& par) {
    return charNumInParagraph(par) < 1;
}

bool GenericExporter::isLineBreak(const CEDLine& line) const {
    return line.hardBreak() || formatOptions().preserveLineBreaks();
}

int GenericExporter::numChars() const {
    return num_chars_;
}

int GenericExporter::numColumns() const {
    return num_columns_;
}

int GenericExporter::numFrames() const {
    return num_frames_;
}

int GenericExporter::numLines() const {
    return num_lines_;
}

int GenericExporter::numParagraphs() const {
    return num_paragraphs_;
}

int GenericExporter::numPictures() const {
    return num_pictures_;
}

int GenericExporter::numSections() const {
    return num_sections_;
}

int GenericExporter::numTables() const {
    return num_tables_;
}

std::ostream& GenericExporter::outputStream() {
    return *os_;
}

CEDPage * GenericExporter::page() {
    return page_;
}

const CEDPage * GenericExporter::page() const {
    return page_;
}

std::string GenericExporter::makePictureName(CEDPicture& picture) {
    std::ostringstream buf;
    buf << "image_" << picture.pictureNumber() << "." << imageExporter()->extension();
    return buf.str();
}

std::string GenericExporter::makePicturePathRelative(CEDPicture& picture) {
    return baseName(makeOutputPictureDir()) + "/" + makePictureName(picture);
}

std::string GenericExporter::makePicturePath(CEDPicture& picture) {
    return makeOutputPictureDir() + "/" + makePictureName(picture);
}

void GenericExporter::savePicture(CEDPicture& picture) {
    createPicturesFolder();
    savePictureData(picture, makePicturePath(picture));
}

void GenericExporter::savePictureData(CEDPicture& picture, const std::string& path) {
    imageExporter()->save(*(picture.image()), path);
}

void GenericExporter::savePictureData(CEDPicture& picture, std::ostream& os) {
    imageExporter()->save(*(picture.image()), os);
}

void GenericExporter::setSkipEmptyLines(bool value) {
    skip_empty_lines_ = value;
}

void GenericExporter::setSkipEmptyParagraphs(bool value) {
    skip_empty_paragraphs_ = value;
}

void GenericExporter::setSkipPictures(bool value) {
    skip_pictures_ = value;
}

bool GenericExporter::skipEmptyLines() const {
    return skip_empty_lines_;
}

bool GenericExporter::skipEmptyParagraphs() const {
    return skip_empty_paragraphs_;
}

bool GenericExporter::skipLine(CEDLine& line) const {
    return skip_empty_lines_ && line.empty();
}

bool GenericExporter::skipParagraph(CEDParagraph& par) const {
    return skip_empty_paragraphs_ && isEmptyParagraph(par);
}

bool GenericExporter::skipPictures() const {
    return skip_pictures_;
}

void GenericExporter::setOutputStream(std::ostream * os) {
    os_ = os;
}

void GenericExporter::writeCharacter(CEDChar&) {
}

void GenericExporter::writeCharacterBegin(CEDChar&) {
}

void GenericExporter::writeCharacterEnd(CEDChar&) {
}

void GenericExporter::writeColumnBegin(CEDColumn&) {
}

void GenericExporter::writeColumnEnd(CEDColumn&) {

}

void GenericExporter::writeFrameBegin(CEDFrame&) {

}

void GenericExporter::writeFrameEnd(CEDFrame&) {

}

void GenericExporter::writeLineBegin(CEDLine&) {

}

void GenericExporter::writeLineEnd(CEDLine&) {

}

void GenericExporter::writePageBegin(CEDPage&) {

}

void GenericExporter::writePageEnd(CEDPage&) {

}

void GenericExporter::writeParagraphBegin(CEDParagraph&) {

}

void GenericExporter::writeParagraphEnd(CEDParagraph&) {

}

void GenericExporter::writePicture(CEDPicture&) {

}

void GenericExporter::writeSectionBegin(CEDSection&) {

}

void GenericExporter::writeSectionEnd(CEDSection&) {

}

}
