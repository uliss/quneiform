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

#include "backup.h"

Handle hCurPage = NULL;

namespace cf {
namespace cpage {

BackupPage::BackupPage()
{
    hCurBackUp = NULL;
}

BackupPage::~BackupPage()
{
    backups_.Clear();
}

size_t BackupPage::backupCount() const
{
    return backups_.GetCount();
}

Handle BackupPage::backupAt(size_t pos)
{
    return backups_.GetHandle(pos);
}

void BackupPage::Clear()
{
    backups_.Clear();
    hCurBackUp = NULL;
}
//#################################
Handle BackupPage::BackUp(Handle backup)
{
    Handle hBackupPage = backup == NULL ? hCurBackUp : backup;
// Удалить все страницы позднее текущей.
    Handle prevPage  = NULL;

    while (hBackupPage != prevPage) {
        prevPage = hBackupPage;
        Page & p = backups_.GetNext(hBackupPage);

        if (hCurBackUp != prevPage)
            backups_.Del(prevPage);
    }

    Handle hPage = backups_.AddTail(*this);

    if (hPage == NULL)
        return NULL;

    hCurBackUp = hPage;
    return  hCurBackUp;
}
//#################################
Bool32 BackupPage::Redo(Handle backup)
{
    if (hCurBackUp) {
        if (backup) {
            *(Page*)this = backups_.GetItem(backup);
            hCurBackUp = backup;
        }

        else
            *(Page*)this = backups_.GetNext(hCurBackUp);

        return TRUE;
    }

    return FALSE;
}
//#################################
Bool32 BackupPage::Undo(Handle backup)
{
    if (hCurBackUp) {
        if (backup) {
            *(Page*)this = backups_.GetItem(backup);
            hCurBackUp = backup;
        }

        else
            *(Page*)this = backups_.GetPrev(hCurBackUp);

        return TRUE;
    }

    return FALSE;
}
//#################################
Bool32 BackupPage::save(Handle to)
{
    int count = backups_.GetCount();
    Bool32 rc = FALSE;
    int i, position;
    rc = myWrite(to, &count, sizeof(count)) == sizeof(count);

    if (count) {
        position = backups_.GetPos(hCurBackUp);

        if (rc) rc = myWrite(to, &position, sizeof(position)) == sizeof(position);

        if (rc == TRUE && count)
            for (i = 0; i < count; i++)
                backups_.GetItem(backups_.GetHandle(i)).save(to);
    }

    if (rc)
        rc = Page::save(to);

    return rc;
}
//#################################
Bool32 BackupPage::restore(Handle from)
{
    Bool32 rc = FALSE;
    int count, i, position;
    backups_.Clear();
    rc = myRead(from, &count, sizeof(count)) == sizeof(count);

    if (count) {
        if (rc) rc = myRead(from, &position, sizeof(position)) == sizeof(position);

        for (i = 0; i < count && rc == TRUE; i++) {
            Page page;
            rc = page.restore(from);

            if (rc)
                backups_.AddTail(page);
        }

        if (rc && position >= 0)
            hCurBackUp = backups_.GetHandle(position);
    }

    if (rc)
        rc = Page::restore(from);

    return rc;
}
//#################################
Bool32 BackupPage::saveCompress(Handle to)
{
    int count = backups_.GetCount();
    Bool32 rc = FALSE;
    int i, position;
    rc = myWrite(to, &count, sizeof(count)) == sizeof(count);

    if (count) {
        position = backups_.GetPos(hCurBackUp);

        if (rc) rc = myWrite(to, &position, sizeof(position)) == sizeof(position);

        if (rc == TRUE && count)
            for (i = 0; i < count; i++)
                backups_.GetItem(backups_.GetHandle(i)).saveCompress(to);
    }

    if (rc)
        rc = Page::saveCompress(to);

    return rc;
}
//#################################
Bool32 BackupPage::restoreCompress(Handle from)
{
    Bool32 rc = FALSE;
    int count, i, position;
    backups_.Clear();
    rc = myRead(from, &count, sizeof(count)) == sizeof(count);

    if (count) {
        if (rc) rc = myRead(from, &position, sizeof(position)) == sizeof(position);

        for (i = 0; i < count && rc == TRUE; i++) {
            Page page;
            rc = page.restoreCompress(from);

            if (rc)
                backups_.AddTail(page);
        }

        if (rc && position >= 0)
            hCurBackUp = backups_.GetHandle(position);
    }

    if (rc)
        rc = Page::restoreCompress(from);

    return rc;
}

BackupPage & BackupPage::operator = (BackupPage& page) {

    int count = page.backups_.GetCount();
    backups_.Clear();

    for (int i = 0; i < count; i++)
        backups_.AddTail(page.backups_.GetItem(page.backups_.GetHandle(i)));

    if (count) {
        int curr = page.backups_.GetPos(page.hCurBackUp);
        hCurBackUp = backups_.GetHandle(curr);
    }

    *(Page *)this = page;
    return *this;
}

}
}

