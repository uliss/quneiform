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
#include "genericexporter.h"
#include "ced/ced.h"
#include "ced/cedint.h"
#include "common/debug.h"

namespace CIF
{

GenericExporter::GenericExporter(CEDPage * page, const FormatOptions& opts) :
    Exporter(opts), page_(page), num_chars_(0), num_columns_(0), num_frames_(0), num_lines_(0),
            num_paragraphs_(0), num_pictures_(0), num_sections_(0) {

}

void GenericExporter::doExport(std::ostream& os) {
    os_ = &os;
    writeHeader(os);
    exportPage();
    writeFooter(os);
}

void GenericExporter::exportChar(CEDChar * chr) {
    if (CED_IsPicture(chr))
        exportPicture(chr);
    else {
        // Обычный символ
        num_chars_++;

        // Скрытый ли символ
        //gEdCharHidden = CED_GetCharFontAttribs(charHandle) & ED_HIDDEN;

        //BROWSE_FUNCTION(charHandle, BROWSE_CHAR);
    }
}

void GenericExporter::exportColumn(CEDParagraph * col) {
    num_columns_++;
    // Цикл по всем объектам
    for (CEDParagraph * object = col->GetFirstObject(); object != NULL; object
            = object->GetNextObject()) {
        // Определить тип объекта
        if (CED_IsFrame(object))
            exportFrame(object);
        else if (CED_IsTable(object))
            exportTable(object);
        else if (CED_IsParagraph(object))
            exportParagraph(object);
        else {
            Debug() << "[GenericExporter::exportColumn] Skipping fictive paragraph in ED\n";
        }
    }
}

void GenericExporter::exportFrame(CEDParagraph * frame) {
    // Начало фрейма
    num_frames_++;
    //BROWSE_FUNCTION(gFrameHandle, BROWSE_FRAME_START);

    // Обход абзацев и таблиц внутри фрейма
    //if (!BrowseParagraphs(gFrameHandle, BrowseFunction, wantSkipTableCells, wantSkipParagraphs))
    //  return FALSE;

    // Конец фрейма
    //BROWSE_FUNCTION(gFrameHandle, BROWSE_FRAME_END);
    //gFrameHandle = NULL;
}

void GenericExporter::exportLine(CEDLine * line) {
    num_lines_++;
    //gEdLineHardBreak = CED_GetLineHardBreak(gLineHandle);
    //BROWSE_FUNCTION(gLineHandle, BROWSE_LINE_START);

    // Обход символов в строке
    int chars = line->GetCountChar();
    for (int i = 0; i < chars; i++) {
        CEDChar * charPtr = line->GetChar(i);
        exportChar(charPtr);
    }

    // Конец строки
    //BROWSE_FUNCTION(gLineHandle, BROWSE_LINE_END);
    //gLineHandle = NULL;
}

void GenericExporter::exportPage() {
    for (int i = 0, max = page_->GetNumberOfSections(); i < max; i++) {
        CEDSection * section = page_->GetSection(i);
        assert(section);
        exportSection(section);
    }
}

void GenericExporter::exportParagraph(CEDParagraph * par) {
    writeParagraph(*os_, par);
    // Пропускать абзацы?
    //if (wantSkipParagraphs)
    //  return;

    // Абзац
    // gParagraphHandle = hObject;

    // Начало абзаца
    num_paragraphs_++;

    // BROWSE_FUNCTION(gParagraphHandle, BROWSE_PARAGRAPH_START);

    // Обход строк в абзаце
    int lines = par->GetCountLine();
    for (int i = 0; i < lines; i++) {
        CEDLine * line = par->GetLine(i);
        exportLine(line);
    }

    // Конец абзаца
    //BROWSE_FUNCTION(gParagraphHandle, BROWSE_PARAGRAPH_END);
    //gParagraphHandle = NULL;
}

void GenericExporter::exportPicture(CEDChar * picture) {
    // Картинка
    // Прочесть описание картинки
    //if (!PictureFromChar(picture))
    //               return FALSE;

    num_pictures_;
    //BROWSE_FUNCTION(charHandle, BROWSE_PICTURE);

    // gPictureNumber = 0;
    //gPictureData = 0;
    //gPictureLength = 0;
}

void GenericExporter::exportSection(CEDSection * sect) {
    writeSection(*os_, sect);
    num_sections_++;

    for (int i = 0; i < sect->numberOfColumns; i++)
        exportColumn(sect->GetColumn(i));
}

void GenericExporter::exportTable(CEDParagraph * table) {
    // Таблица
    //               if (gTableHandle) {
    //                   DEBUG_PRINT("Skipping table inside table");
    //                   continue;
    //               }
    //
    //               gTableHandle = hObject;
    //
    //               // Количество строк и столбцов таблицы
    //               EDSIZE dim = CED_GetSize(gTableHandle);
    //               gTableRows = dim.cy;
    //               gTableCols = dim.cx;
    //
    //               // Количество логических ячеек
    //               gTableCells = CED_GetCountLogicalCell(gTableHandle);
    //
    //               // Массив логических номеров ячеек
    //               gLogicalCells = (long*) CED_GetTableOfCells(gTableHandle);
    //
    //               // Начало таблицы
    //               gHitTables++;
    //               BROWSE_FUNCTION(gTableHandle, BROWSE_TABLE_START);
    //
    //               // Обход ячеек таблицы
    //               if (!wantSkipTableCells && !BrowseCells(BrowseFunction))
    //                   return FALSE;
    //
    //               // Конец таблицы
    //               BROWSE_FUNCTION(gTableHandle, BROWSE_TABLE_END);
    //               gTableHandle = NULL;
    //               gTableRows = 0;
    //               gTableCols = 0;
    //               gTableCells = 0;
    //               gLogicalCells = NULL;
}

void GenericExporter::writeFooter(std::ostream& /*os*/) {
    Debug() << "[GenericExporter::writeFooter] not implemented\n";
}

void GenericExporter::writeHeader(std::ostream& /*os*/) {
    Debug() << "[GenericExporter::writeHeader] not implemented\n";
}

void GenericExporter::writeParagraph(std::ostream& os, CEDParagraph * par) {
    Debug() << "[GenericExporter::writeParagraph] not implemented\n";
}

void GenericExporter::writeSection(std::ostream& /*os*/, CEDSection * /*sect*/) {
    Debug() << "[GenericExporter::writeSection] not implemented\n";
}

}
