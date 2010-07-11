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

#include "summaryexporter.h"
#include "ced/cedchar.h"
#include "ced/cedcolumn.h"
#include "ced/cedline.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"
#include "ced/cedsection.h"
#include "ced/cedpicture.h"

namespace CIF
{

SummaryExporter::SummaryExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts), total_columns_(0), total_pars_(0), total_lines_(0),
            total_chars_(0), total_pictures_(0) {
}

void parCount(CEDColumn * col, size_t * par_num, size_t * line_num, size_t * char_num,
        size_t * pict_num) {
    for (size_t i = 0; i < col->elementCount(); i++) {
        if (dynamic_cast<CEDParagraph*> (col->elementAt(i))) {
            (*par_num)++;
            CEDParagraph * p = static_cast<CEDParagraph*> (col->elementAt(i));
            (*line_num) += p->lineCount();

            for (size_t ln_i = 0; ln_i < p->lineCount(); ln_i++) {
                CEDLine * l = p->lineAt(ln_i);

                for (size_t ch_i = 0; ch_i < l->elementCount(); ch_i++) {
                    Element * el = l->elementAt(ch_i);
                    if (dynamic_cast<CEDChar*> (el)) {
                        (*char_num)++;
                    } else if (dynamic_cast<CEDPicture*> (el)) {
                        (*pict_num)++;
                    }
                }
            }
        }
    }
}

void SummaryExporter::calcPageStat() {
    total_columns_ = 0;
    total_pars_ = 0;
    total_lines_ = 0;
    total_chars_ = 0;
    total_pictures_ = 0;
    for (size_t i = 0; i < page()->sectionCount(); i++) {
        CEDSection * sect = page()->sectionAt(i);
        total_columns_ += sect->columnCount();

        for (size_t col_i = 0; col_i < sect->columnCount(); col_i++) {
            parCount(sect->columnAt(col_i), &total_pars_, &total_lines_, &total_chars_,
                    &total_pictures_);
        }
    }
}

void SummaryExporter::doExport(std::ostream& os) {
    calcPageStat();
    printPageStat(os);
}

void SummaryExporter::printPageStat(std::ostream& os) {
    os << "##################################\n";
    os << "Page summary: " << "\n";
    os << "    total sections:   " << page()->sectionCount() << "\n";
    os << "    total columns:    " << total_columns_ << "\n";
    os << "    total paragraphs: " << total_pars_ << "\n";
    os << "    total lines:      " << total_lines_ << "\n";
    os << "    total chars:      " << total_chars_ << "\n";
    os << "    total pictures:   " << total_pictures_ << "\n";
}

}
