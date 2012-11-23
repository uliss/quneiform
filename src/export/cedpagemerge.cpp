/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include "cedpagemerge.h"
#include "common/log.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"

namespace cf {

CEDPageMerge::CEDPageMerge()
{
}

void CEDPageMerge::add(CEDPagePtr page)
{
    if(!page) {
        cfWarning(MODULE_CED) << "NULL page pointer given";
        return;
    }

    // first time create page clone
    if(!result_) {
        result_.reset(page->clone());
        return;
    }

    // append page sections
    for(size_t i = 0; i < page->sectionCount(); i++) {
        result_->addSection(page->sectionAt(i)->clone());
    }
}

bool CEDPageMerge::empty() const
{
    return !result_ || result_->empty();
}

CEDPagePtr CEDPageMerge::get()
{
    return result_;
}

}
