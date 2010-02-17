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
#include "common/debug.h"

namespace CIF
{

GenericExporter::GenericExporter(CEDPage * page, const FormatOptions& opts) :
    Exporter(opts), page_(page), no_pictures_(false), os_(NULL), num_chars_(0), num_columns_(0),
            num_frames_(0), num_lines_(0), num_paragraphs_(0), num_pictures_(0), num_sections_(0),
            num_tables_(0), table_nesting_level_(0) {

}

void GenericExporter::doExport(std::ostream& os) {
    if(os.fail())
        throw Exception("[GenericExporter::doExport] invalid stream given");

    no_pictures_ = true;
    os_ = &os;
    exportPage();
}

void GenericExporter::exportChar(CEDChar * chr) {
    assert(chr);

    if (CED_IsPicture(chr))
        exportPicture(chr);
    else {
        // Обычный символ
        num_chars_++;

        // Скрытый ли символ
        //gEdCharHidden = CED_GetCharFontAttribs(charHandle) & ED_HIDDEN;

        writeCharacter(*os_, chr);
    }
}

void GenericExporter::exportColumn(CEDParagraph * col) {
    assert(col);
    num_columns_++;

    writeColumnBegin(*os_, col);

    // Цикл по всем объектам
    exportObjects(col);

    writeColumnEnd(*os_, col);
}

void GenericExporter::exportFrame(CEDParagraph * frame) {
    assert(frame);
    // Начало фрейма
    num_frames_++;

    writeFrameBegin(*os_, frame);

    // Обход абзацев и таблиц внутри фрейма
    exportObjects(frame);

    writeFrameEnd(*os_, frame);
}

void GenericExporter::exportLine(CEDLine * line) {
    assert(line);
    num_lines_++;
    //gEdLineHardBreak = CED_GetLineHardBreak(gLineHandle);
    writeLineBegin(*os_, line);

    // Обход символов в строке
    for (int i = 0, max_chars = line->GetCountChar(); i < max_chars; i++)
        exportChar(line->GetChar(i));

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

void GenericExporter::exportPage() {
    if(!page_) {
        Debug() << "[GenericExporter::exportPage] nothing to export: empty page given\n";
        return;
    }

    writePageBegin(*os_);

    for (int i = 0, max = page_->GetNumberOfSections(); i < max; i++) {
        CEDSection * section = page_->GetSection(i);
        assert(section);
        exportSection(section);
    }

    writePageEnd(*os_);
}

void GenericExporter::exportParagraph(CEDParagraph * par) {
    // Пропускать абзацы?
    //if (wantSkipParagraphs)
    //  return;

    assert(par);
    num_paragraphs_++;

    writeParagraphBegin(*os_, par);

    // Обход строк в абзаце
    for (int i = 0, max_lines = par->GetCountLine(); i < max_lines; i++)
        exportLine(par->GetLine(i));

    writeParagraphEnd(*os_, par);
}

void GenericExporter::exportPicture(CEDChar * picture) {
    if (no_pictures_)
        return;
    // Картинка
    // Прочесть описание картинки
    //if (!PictureFromChar(picture))
    //               return FALSE;

    assert(picture);
    num_pictures_++;
    writePicture(picture);
}

void GenericExporter::exportSection(CEDSection * sect) {
    assert(sect);
    num_sections_++;

    writeSectionBegin(*os_, sect);

    for (int i = 0; i < sect->numberOfColumns; i++)
        exportColumn(sect->GetColumn(i));

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
    const int table_cols = dim.cx;

    // Количество логических ячеек
    const int num_table_cells = CED_GetCountLogicalCell(table);

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

CEDPage * GenericExporter::page() {
    return page_;
}

void GenericExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    letterEx *alt = chr->alternatives;
    assert(alt);
    os << alt->alternative;
}

void GenericExporter::writeColumnBegin(std::ostream& /*os*/, CEDParagraph * /*col*/) {

}

void GenericExporter::writeColumnEnd(std::ostream& /*os*/, CEDParagraph * /*col*/) {

}

void GenericExporter::writeFrameBegin(std::ostream& /*os*/, CEDParagraph * /*frame*/) {

}

void GenericExporter::writeFrameEnd(std::ostream& /*os*/, CEDParagraph * /*frame*/) {

}

void GenericExporter::writeLineBegin(std::ostream& /*os*/, CEDLine * /*line*/) {

}

void GenericExporter::writeLineEnd(std::ostream& /*os*/, CEDLine * /*line*/) {

}

void GenericExporter::writePageBegin(std::ostream& /*os*/) {

}

void GenericExporter::writePageEnd(std::ostream& /*os*/) {

}

void GenericExporter::writeParagraphBegin(std::ostream& /*os*/, CEDParagraph * /*par*/) {

}

void GenericExporter::writeParagraphEnd(std::ostream& /*os*/, CEDParagraph * /*par*/) {

}

void GenericExporter::writePicture(CEDChar * /*pict*/) {

}

void GenericExporter::writeSectionBegin(std::ostream& /*os*/, CEDSection * /*sect*/) {

}

void GenericExporter::writeSectionEnd(std::ostream& /*os*/, CEDSection * /*sect*/) {

}

void GenericExporter::writeTableBegin(std::ostream& /*os*/, CEDParagraph * /*table*/) {

}

void GenericExporter::writeTableEnd(std::ostream& /*os*/, CEDParagraph * /*table*/) {

}

void GenericExporter::writeTableRowBegin(std::ostream& /*os*/, CEDParagraph * /*row*/) {

}

void GenericExporter::writeTableRowEnd(std::ostream& /*os*/, CEDParagraph * /*row*/) {

}

}
