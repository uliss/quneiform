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

#include <memory.h>

#include "backup.h"
#include "page.h"
#include "common/log.h"

namespace cf {
namespace cpage {

Page::Page()
{
}

Page::~Page()
{
    clearBlocks();
}

Block * Page::createBlock(Handle Type, uint32_t UserNum , uint32_t Flags , void * lpData , uint32_t Size )
{
    blocks_.push_back(new Block);

    if (!blocks_.back()->create(Type, UserNum, Flags, lpData, Size))
        return NULL;

    return blocks_.back();
}

Page& Page::operator=(Page& page)
{
    int count = page.blockCount();
    clearBlocks();

    for (int i = 0; i < count; i++)
        appendBlock(*page.blockAt(i));

    *(Data *)this = page;
    return *this;
}

void Page::appendBlock(const Block& b)
{
    blocks_.push_back(new Block(b));
}

Block& Page::blockData(Handle b)
{
    return * (Block*) b;
}

Block * Page::blockAt(size_t pos)
{
    if(pos >= blocks_.size())
        cfError(cf::MODULE_CPAGE) << "invalid block index:" << pos;

    return blocks_.at(pos);
}

size_t Page::blockCount() const
{
    return blocks_.size();
}

void Page::clearBlocks()
{
    for(size_t i = 0; i < blocks_.size(); i++)
        delete blocks_[i];

    blocks_.clear();
}

int Page::findBlock(Block * b) const
{
    for(size_t i = 0; i < blocks_.size(); i++) {
        if(blocks_[i] == b)
            return static_cast<int>(i);
    }

    return -1;
}

bool Page::removeBlock(Block * b)
{
    int pos = findBlock(b);
    if(pos == -1)
        return false;

    blocks_.erase(blocks_.begin() + pos);
    delete b;
    return true;
}

bool Page::save(Handle to)
{
    int count = blockCount();
    bool rc = FALSE;
    rc = myWrite(to, &count, sizeof(count)) == sizeof(count);

    if (rc == TRUE && count) {
        for (int i = 0; i < count; i++) {
            blocks_[i]->save(to);
        }
    }

    if (rc)
        rc = Data::save(to);

    return rc;
}

Bool32  Page::restore(Handle from)
{
    Bool32 rc = FALSE;
    int count, i;
    clearBlocks();
    rc = myRead(from, &count, sizeof(count)) == sizeof(count);

    for (i = 0; i < count && rc == TRUE; i++) {
        Block block;
        rc = block.restore(from);

        if (rc)
            appendBlock(block);
    }

    if (rc)
        rc = Data::restore(from);

    return rc;
}

Bool32  Page::saveCompress(Handle to)
{
    int count = blockCount();
    Bool32 rc = FALSE;
    int i;
    rc = myWrite(to, &count, sizeof(count)) == sizeof(count);

    if (rc == TRUE && count) {
        for (i = 0; i < count; i++) {
            blocks_[i]->saveCompress(to);
        }
    }

    if (rc)
        rc = Data::saveCompress(to);

    return rc;
}

Bool32  Page::restoreCompress(Handle from)
{
    Bool32 rc = FALSE;
    int count, i;
    clearBlocks();
    rc = myRead(from, &count, sizeof(count)) == sizeof(count);

    for (i = 0; i < count && rc == TRUE; i++) {
        Block block;
        rc = block.restoreCompress(from);

        if (rc)
            appendBlock(block);
    }

    if (rc)
        rc = Data::restoreCompress(from);

    return rc;
}

static  CPAGE_CONVERTOR s_ConvertorPages = {0, DefConvertPage};

CPAGE_CONVERTOR SetConvertorPages(CPAGE_CONVERTOR convertor)
{
    CPAGE_CONVERTOR old = s_ConvertorPages;
    s_ConvertorPages = convertor;
    return old;
}

uint32_t Page::Convert(Handle type, void * lpdata, uint32_t size)
{
    uint32_t rc = 0;
    rc = (*s_ConvertorPages.fnConvertor)(s_ConvertorPages.dwContext,
                                         type_, data_, size_,
                                         type, lpdata, size);
    return rc;
}

PAGEINFO * Page::pageInfo()
{
    if(type_ != PT_PAGEINFO)
        return NULL;

    if(size_ != sizeof(PAGEINFO))
        return NULL;

    return (PAGEINFO*) data_;
}

}
}

