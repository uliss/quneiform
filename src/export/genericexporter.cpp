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
#include "ced/cedint.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"
#include "ced/ced_struct.h"
#include "common/debug.h"
#include "common/cifconfig.h"
#include "common/imagerawdata.h"
#include "cfcompat.h"
#include "common/helper.h"
#include "common/iconv_local.h"
#include "rout_own.h" // for font_style_t
namespace CIF
{

GenericExporter::styleList supported_fontstyles;
namespace
{
bool initSupportedFontStyles() {
    CIF::supported_fontstyles.push_back(FONT_BOLD);
    CIF::supported_fontstyles.push_back(FONT_ITALIC);
    CIF::supported_fontstyles.push_back(FONT_UNDERLINE);
    CIF::supported_fontstyles.push_back(FONT_SUB);
    CIF::supported_fontstyles.push_back(FONT_SUPER);
    return true;
}

const bool init_supported_fontstyles = initSupportedFontStyles();
}

std::string picturesFolderPath(const std::string& path) {
    std::string res = removeFileExt(path);
    res += "_files";
    return res;
}

GenericExporter::GenericExporter(CEDPage * page, const FormatOptions& opts) :
    Exporter(opts), page_(page), no_pictures_(false), os_(NULL), num_chars_(0), num_columns_(0),
            num_frames_(0), num_lines_(0), num_paragraphs_(0), num_pictures_(0), num_sections_(0),
            num_tables_(0), table_nesting_level_(0), skip_empty_paragraphs_(false),
            skip_empty_lines_(false), previous_style_(0), current_picture_(NULL) {

    if (isCharsetConversion())
        converter_.open(inputEncoding(), outputEncoding());
}

int GenericExporter::charNumInParagraph(CEDParagraph * par) {
    if (!par)
        return 0;
    int num_of_chars = 0;
    for (int i = 0, num_lines = par->lineCount(); i < num_lines; i++)
        num_of_chars += par->lineAt(i)->elementCount();

    return num_of_chars;
}

std::string GenericExporter::createPicturesFolder() {
    if (outputFilename().empty())
        throw Exception("Page name not specified");
    std::string path = picturesFolderPath(outputFilename());

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
    if (os.fail())
        throw Exception("[GenericExporter::doExport] invalid stream given");

    if (outputFilename().empty()) {
        Debug()
                << "[GenericExporter::doExport] output filename is empty. Skipping picture export\n";
        setSkipPictures(true);
    }

    setOutputStream(&os);
    exportPage(page_);
}

void GenericExporter::exportChar(CEDChar * chr) {
    assert(chr);

    if (chr->isPicture())
        exportPicture(chr);
    else {
        // Обычный символ
        num_chars_++;

        // Скрытый ли символ
        //gEdCharHidden = CED_GetCharFontAttribs(charHandle) & ED_HIDDEN;

        writeFontStyle(*os_, chr);
        writeCharacter(*os_, chr);
        previous_style_ = chr->fontStyle();
    }
}

void GenericExporter::exportColumn(CEDParagraph * col) {
    assert(col);
    num_columns_++;

    writeColumnBegin(*os_, col);
    writeColumn(*os_, col);
    writeColumnEnd(*os_, col);
}

void GenericExporter::exportFrame(CEDParagraph * frame) {
    assert(frame);
    num_frames_++;

    writeFrameBegin(*os_, frame);
    writeFrame(*os_, frame);
    writeFrameEnd(*os_, frame);
}

void GenericExporter::exportLine(CEDLine * line) {
    assert(line);

    if (skipLine(line)) {
        if (Config::instance().debugHigh())
            Debug() << "[GenericExporter::exportLine] skipping empty line\n";
        return;
    }

    num_lines_++;
    writeLineBegin(*os_, line);
    writeLine(*os_, line);
    writeLineEnd(*os_, line);
}

void GenericExporter::exportObjects(CEDParagraph * objects) {
    for (CEDParagraph * obj = objects->GetFirstObject(); obj != NULL; obj = obj->GetNextObject()) {
        // Определить тип объекта
        if (CED_IsFrame(obj))
            exportFrame(obj);
        else if (CED_IsTable(obj))
            exportTable(obj);
        else if (CED_IsParagraph(obj))
            exportParagraph(obj);
        else {
            Debug() << "[GenericExporter::exportColumn] Skipping fictive paragraph in ED\n";
        }
    }
}

void GenericExporter::exportPage(CEDPage * page) {
    if (!page) {
        Debug() << "[GenericExporter::exportPage] nothing to export: empty page given\n";
        return;
    }

    writePageBegin(*os_, page);
    writePage(*os_, page);
    writePageEnd(*os_, page);
}

void GenericExporter::exportParagraph(CEDParagraph * par) {
    assert(par);

    if (skipParagraph(par)) {
        if (Config::instance().debugHigh())
            Debug() << "[GenericExporter::exportParagraph] skipping empty paragraph\n";
        return;
    }

    num_paragraphs_++;

    writeParagraphBegin(*os_, par);
    writeParagraph(*os_, par);
    writeParagraphEnd(*os_, par);
}

void GenericExporter::exportPicture(CEDChar * picture) {
    if (skipPictures()) {
        Debug() << "picture skipped\n";
        return;
    }

    assert(picture);
    num_pictures_++;
    writePicture(*os_, picture);
}

void GenericExporter::exportSection(CEDSection * sect) {
    assert(sect);
    num_sections_++;

    writeSectionBegin(*os_, sect);
    writeSection(*os_, sect);
    writeSectionEnd(*os_, sect);
}

void GenericExporter::exportTable(CEDParagraph * table) {
    assert(table);
    if (table_nesting_level_ > 0) {
        Debug() << "[GenericExporter::exportTable] Skipping table inside table\n";
        return;
    }

    num_tables_++;
    table_nesting_level_++;

    writeTableBegin(*os_, table);
    exportTableCells(table);
    writeTableEnd(*os_, table);
    table_nesting_level_--;
}

void GenericExporter::exportTableCells(CEDParagraph * table) {
    assert(table);
    // Количество строк и столбцов таблицы
    EDSIZE dim = CED_GetSize(table);
    const int table_rows = dim.cy;
    //const int table_cols = dim.cx;

    // Количество логических ячеек
    const unsigned int num_table_cells = CED_GetCountLogicalCell(table);

    // Массив логических номеров ячеек
    //gLogicalCells = (long*) CED_GetTableOfCells(gTableHandle);

    // Справочная обхода ячеек
    const int MAX_TABLE_CELLS = 10000;
    char sprav[MAX_TABLE_CELLS];
    assert(num_table_cells < sizeof(sprav));
    memset(&sprav, 0, sizeof(sprav));

    // Цикл по строкам таблицы
    for (int row_index = 0; row_index < table_rows; row_index++) {
        CEDParagraph * row = table->GetRow(row_index);
        exportTableRow(row);
    }
}

void GenericExporter::exportTableRow(CEDParagraph * row) {
    assert(row);

    writeTableRowBegin(*os_, row);

    //    // Цикл по столбцам таблицы
    //    for (gIndexTableCol = 0; gIndexTableCol < gTableCols; gIndexTableCol++) {
    //        // Логический номер ячейки
    //        gIndexTableCell = gLogicalCells[gIndexTableRow * gTableCols + gIndexTableCol];
    //
    //        // Проверить по справочной
    //        if (gIndexTableCell < 0 || sprav[gIndexTableCell]) {
    //            // В этой логической ячейке уже были
    //            gCellHandle = NULL;
    //            continue;
    //        }
    //
    //        sprav[gIndexTableCell] = 1;
    //
    //        // Начало ячейки
    //        gCellHandle = CED_GetLogicalCell(gTableHandle, gIndexTableCell);
    //        ASSERT(gCellHandle);
    //
    //        gHitCells++;
    //        BROWSE_FUNCTION(gCellHandle, BROWSE_CELL_START);
    //
    //        // Обход абзацев в ячейке
    //        if (!BrowseParagraphs(gCellHandle, BrowseFunction, FALSE, // wantSkipTableCells
    //                FALSE // wantSkipParagraphs
    //        ))
    //            return FALSE;
    //
    //        // Конец ячейки
    //        BROWSE_FUNCTION(gCellHandle, BROWSE_CELL_END);
    //        gCellHandle = NULL;
    //    }

    writeTableRowEnd(*os_, row);
}

bool GenericExporter::isEmptyParagraph(CEDParagraph * par) {
    return charNumInParagraph(par) < 1;
}

bool GenericExporter::isLineBreak(CEDLine * line) const {
    assert(line);
    return line->hardBreak() || formatOptions().preserveLineBreaks();
}

bool GenericExporter::isRemoveHyphens(CEDLine * line) const {
    assert(line);
    return !isLineBreak(line) && !formatOptions().preserveLineHyphens();
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

std::ostream * GenericExporter::outputStream() {
    return os_;
}

CEDPage * GenericExporter::page() {
    return page_;
}

PictureEntry * GenericExporter::pictureEntry(CEDChar * picture) const {
    assert(page_);
    assert(picture);

    int picture_num = picture->pictureNumber();

    if (picture_num < 0)
        throw Exception("[CEDPage::pictureEntry] not a picture given");

    PictureEntry * res = page_->findPictureByNumber(picture_num);

    if (!res) {
        std::ostringstream msg;
        msg << "[GenericExporter::savePictureData] picture with number " << picture_num
                << " not found\n";
        throw Exception(msg.str());
    }

    if (!res->data || res->len <= 0)
        throw Exception("[GenericExporter::savePicture] failed");

    return res;
}

std::string GenericExporter::pictureName(CEDChar * picture) {
    assert(picture);
    std::ostringstream buf;
    buf << "image_" << picture->pictureNumber() << "." << imageExporter()->extension();
    return buf.str();
}

void GenericExporter::resetFontStyle(std::ostream& os) {
    styleList off = styleEnd(previous_style_, 0);
    for (styleList::iterator it = off.begin(), end = off.end(); it != end; ++it)
        writeFontStyleEnd(os, *it);
    previous_style_ = 0;
}

std::string GenericExporter::savePicture(CEDChar * picture) {
    std::string path = createPicturesFolder() + "/" + pictureName(picture);
    savePictureData(picture, path);
    return path;
}

void GenericExporter::savePictureData(CEDChar * picture, const std::string& path) {
    current_picture_ = pictureEntry(picture);
    assert(current_picture_);
    ImageRawData raw_image((uchar*) current_picture_->data, current_picture_->len,
            ImageRawData::AllocatorNone);
    imageExporter()->save(raw_image, path);
}

void GenericExporter::savePictureData(CEDChar * picture, std::ostream& os) {
    current_picture_ = pictureEntry(picture);
    assert(current_picture_);
    ImageRawData raw_image((uchar*) current_picture_->data, current_picture_->len,
            ImageRawData::AllocatorNone);
    imageExporter()->save(raw_image, os);
}

void GenericExporter::setSkipEmptyLines(bool value) {
    skip_empty_lines_ = value;
}

void GenericExporter::setSkipEmptyParagraphs(bool value) {
    skip_empty_paragraphs_ = value;
}

void GenericExporter::setSkipPictures(bool value) {
    no_pictures_ = value;
}

bool GenericExporter::skipEmptyLines() const {
    return skip_empty_lines_;
}

bool GenericExporter::skipEmptyParagraphs() const {
    return skip_empty_paragraphs_;
}

bool GenericExporter::skipLine(CEDLine * line) const {
    return line == NULL || (skip_empty_lines_ && line->empty());
}

bool GenericExporter::skipParagraph(CEDParagraph * par) const {
    return par == NULL || (skip_empty_paragraphs_ && isEmptyParagraph(par));
}

bool GenericExporter::skipPictures() const {
    return no_pictures_;
}

void GenericExporter::setOutputStream(std::ostream * os) {
    os_ = os;
}

void GenericExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr && chr->hasAlternatives());
    os << chr->alternativeAt(0).getChar();
}

void GenericExporter::writeColumn(std::ostream& /*os*/, CEDParagraph * col) {
    exportObjects(col);
}

void GenericExporter::writeColumnBegin(std::ostream& /*os*/, CEDParagraph * /*col*/) {

}

void GenericExporter::writeColumnEnd(std::ostream& /*os*/, CEDParagraph * /*col*/) {

}

void GenericExporter::writeFontStyle(std::ostream& os, CEDChar * c) {
    assert(c);
    styleList style_off = styleEnd(previous_style_, c->fontStyle());
    styleList style_on = styleBegin(previous_style_, c->fontStyle());

    for (styleList::iterator it = style_off.begin(), end = style_off.end(); it != end; ++it)
        writeFontStyleEnd(os, *it);

    for (styleList::iterator it = style_on.begin(), end = style_on.end(); it != end; ++it)
        writeFontStyleBegin(os, *it);
}

void GenericExporter::writeFontStyleBegin(std::ostream&, int) {
}

void GenericExporter::writeFontStyleEnd(std::ostream&, int) {
}

void GenericExporter::writeFrame(std::ostream& /*os*/, CEDParagraph * frame) {
    // Обход абзацев и таблиц внутри фрейма
    exportObjects(frame);
}

void GenericExporter::writeFrameBegin(std::ostream& /*os*/, CEDParagraph * /*frame*/) {

}

void GenericExporter::writeFrameEnd(std::ostream& /*os*/, CEDParagraph * /*frame*/) {

}

void GenericExporter::writeLine(std::ostream&, CEDLine * line) {
    assert(line);
    for (int i = 0, max_chars = line->elementCount(); i < max_chars; i++)
        exportChar(line->charAt(i));
}

void GenericExporter::writeLineBegin(std::ostream&, CEDLine*) {

}

void GenericExporter::writeLineEnd(std::ostream&, CEDLine*) {

}

void GenericExporter::writePage(std::ostream&, CEDPage * page) {
    assert(page);
    for (int i = 0, max = page->sectionCount(); i < max; i++) {
        CEDSection * section = page->section(i);
        exportSection(section);
    }
}

void GenericExporter::writePageBegin(std::ostream&, CEDPage*) {

}

void GenericExporter::writePageEnd(std::ostream&, CEDPage*) {

}

void GenericExporter::writeParagraph(std::ostream&, CEDParagraph * par) {
    assert(par);
    for (int i = 0, num_lines = par->lineCount(); i < num_lines; i++)
        exportLine(par->lineAt(i));
}

void GenericExporter::writeParagraphBegin(std::ostream&, CEDParagraph*) {

}

void GenericExporter::writeParagraphEnd(std::ostream&, CEDParagraph*) {

}

void GenericExporter::writePicture(std::ostream&, CEDChar*) {

}

void GenericExporter::writeSection(std::ostream&, CEDSection * sect) {
    assert(sect);
    for (int i = 0; i < sect->numberOfColumns; i++)
        exportColumn(sect->GetColumn(i));
}

void GenericExporter::writeSectionBegin(std::ostream&, CEDSection*) {

}

void GenericExporter::writeSectionEnd(std::ostream&, CEDSection *) {

}

void GenericExporter::writeTableBegin(std::ostream&, CEDParagraph*) {

}

void GenericExporter::writeTableEnd(std::ostream&, CEDParagraph*) {

}

void GenericExporter::writeTableRowBegin(std::ostream&, CEDParagraph*) {

}

void GenericExporter::writeTableRowEnd(std::ostream&, CEDParagraph*) {
}

GenericExporter::styleList GenericExporter::styleEnd(int style_prev, int style_current) {
    styleList result;

    if (!style_prev)
        return result;

    // find font style changes
    for (styleList::reverse_iterator it = supported_fontstyles.rbegin(), end =
            supported_fontstyles.rend(); it != end; ++it) {
        // if flag exists in previous, but missing in current
        if ((style_prev & (*it)) && !(style_current & (*it))) {
            result.push_back(*it);
        }
    }

    return result;
}

GenericExporter::styleList GenericExporter::styleBegin(int style_prev, int style_current) {
    styleList result;

    // find font style changes
    for (styleList::iterator it = supported_fontstyles.begin(), end = supported_fontstyles.end(); it
            != end; ++it) {
        // no previous char and flag set in current
        if (!style_prev && (style_current & (*it))) {
            result.push_back(*it);
            continue;
        }
        // previous not contains flag but in current flag is set
        else if (!(style_prev & (*it)) && (style_current & (*it))) {
            result.push_back(*it);
        }
    }

    return result;

}

}
