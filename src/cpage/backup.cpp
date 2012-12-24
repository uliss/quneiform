/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>

#include "backup.h"

namespace cf {
namespace cpage {

BackupPage::BackupPage() :
    current_(NULL)
{}

BackupPage::BackupPage(const BackupPage& page) :
    Page(page)
{
    const size_t count = page.backupCount();

    for (size_t i = 0; i < count; i++)
        backups_.push_back(new Page(*page.backups_.at(i)));

    if (count) {
        PageList::const_iterator it = std::find(page.backups_.begin(), page.backups_.end(), page.current_);
        if(it != page.backups_.end()) {
            PageList::iterator curr_it = backups_.begin() + (it - page.backups_.begin());
            current_ = *curr_it;
        }
        else
            current_ = NULL;
    }
}

BackupPage::~BackupPage()
{
    clearBackups();
}

size_t BackupPage::backupCount() const
{
    return backups_.size();
}

Page * BackupPage::backupAt(size_t pos)
{
    if(pos < backups_.size())
        return backups_.at(pos);
    return NULL;
}

void BackupPage::clearBackups()
{
    for(size_t i = 0 ; i < backups_.size(); i++)
        delete backups_[i];

    backups_.clear();
    current_ = NULL;
}

Page * BackupPage::current()
{
    return current_;
}

Page * BackupPage::makeBackup()
{
    backups_.push_back(new Page(*this));
    current_ = backups_.back();
    return backups_.back();
}

bool BackupPage::redo()
{
    if(backups_.empty())
        return false;

    PageList::iterator curr_it;
    if(current_ == NULL) {
        curr_it = backups_.begin();
    }
    else {
        curr_it = std::find(backups_.begin(), backups_.end(), current_);
        if(curr_it == backups_.end()) {
            current_ = NULL;
            return false;
        }
    }

    curr_it++;
    if(curr_it == backups_.end())
        return false;

    current_ = *curr_it;
    Page::operator=(*current_);
    return true;
}

bool BackupPage::undo()
{
    if(backups_.empty())
        return false;

    PageList::iterator curr_it = std::find(backups_.begin(), backups_.end(), current_);
    if(curr_it == backups_.end())
        return false;

    if(curr_it == backups_.begin()) {
        current_ = NULL;
    }
    else {
        curr_it--;
        current_ = *curr_it;
    }

    Page::operator=(*(*curr_it));

    return true;
}

bool BackupPage::save(std::ostream& os) const
{
    uint32_t count = backupCount();

    os.write((char*) &count, sizeof(count));
    if(os.fail())
        return false;

    for (uint32_t i = 0; i < count; i++)
        backups_.at(i)->save(os);

    return Page::save(os);
}

bool BackupPage::restore(std::istream& is)
{
    clearBackups();

    uint32_t count = 0;
    is.read((char*) &count, sizeof(count));

    for (uint32_t i = 0; i < count; i++) {
        Page page;
        if(page.restore(is))
            backups_.push_back(new Page(page));
    }

    return Page::restore(is);
}

BackupPage& BackupPage::operator=(const BackupPage& page)
{
    clearBackups();
    size_t count = page.backupCount();

    for (size_t i = 0; i < count; i++)
        backups_.push_back(new Page(*page.backups_.at(i)));

    if (count) {
        PageList::const_iterator it = std::find(page.backups_.begin(), page.backups_.end(), page.current_);
        if(it != page.backups_.end()) {
            PageList::iterator curr_it = backups_.begin() + (it - page.backups_.begin());
            current_ = *curr_it;
        }
        else
            current_ = NULL;
    }

    Page::operator =((const Page&) page);
    return *this;
}

}
}

