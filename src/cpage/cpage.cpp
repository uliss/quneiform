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

#include "resource.h"
#include "cpage.h"
#include "block.h"
#include "backup.h"
#include "namedata.h"
#include "pagestorage.h"
#include "common/log.h"

extern PtrList<NAMEDATA> NameData;

using namespace cf;
using namespace cf::cpage;

Handle CPAGE_CreatePage(Handle type, const void * lpdata, uint32_t size)
{
    PageHandle p = PageStorage::append(BackupPage());
    p->setData(type, lpdata, size);
    return p;
}

void CPAGE_DeletePage(Handle page)
{
    PageStorage::remove((PageHandle) page);
}

void CPAGE_ClearBackUp(Handle page)
{
    if(!page)
        return;

    static_cast<PageHandle>(page)->Clear();
}

Handle CPAGE_BackUp(Handle page)
{
    if(!page)
        return NULL;

    return static_cast<PageHandle>(page)->BackUp();
}

bool CPAGE_Undo(Handle page, Handle num)
{
    if (num == NULL) {
        uint32_t number = CPAGE_GetBackupCount(page);

        if (number) {
            uint32_t cur = CPAGE_GetBackupCurPos(page);

            if (cur == 0)
                return false;

            num = CPAGE_GetBackupHandle(page, cur - 1);
        }
        else
            return false;
    }

    return static_cast<PageHandle>(page)->Undo(num);
}

bool CPAGE_Redo(Handle page, Handle num)
{
    Bool32 rc = FALSE;

    if (num == NULL) {
        uint32_t number = CPAGE_GetBackupCount(page);

        if (number) {
            uint32_t cur = CPAGE_GetBackupCurPos(page);

            if (cur == (number - 1))
                return false;

            num = CPAGE_GetBackupHandle(page, cur + 1);
        }
        else
            return false;
    }

    return static_cast<PageHandle>(page)->Redo(num);
}

uint32_t CPAGE_GetBackupCount(Handle page)
{
    if(!page)
        return 0;

    return static_cast<PageHandle>(page)->backupCount();
}

Handle CPAGE_GetBackupHandle(Handle page, uint32_t number)
{
    if(!page)
        return 0;

    return static_cast<PageHandle>(page)->backupAt(number);
}

uint32_t CPAGE_GetBackupCurPos(Handle page)
{
    if(!page)
        return 0;

    return static_cast<PageHandle>(page)->GetCurPos();
}

Handle CPAGE_CreateBlock(Handle page, Handle Type, uint32_t UserNum,
                         uint32_t Flags, void * lpData, uint32_t Size)
{
    if(!page)
        return 0;

    assert(CPAGE_GetNameInternalType(Type));

    return static_cast<PageHandle>(page)->createBlock(Type, UserNum, Flags, lpData, Size);
}

bool CPAGE_SetPageData(Handle page, Handle type, void * lpdata, uint32_t size)
{
    if(!page)
        return false;

    assert(CPAGE_GetNameInternalType(type));
    static_cast<PageHandle>(page)->setData(type, lpdata, size);
    return true;
}

uint32_t CPAGE_GetPageData(Handle page, Handle type, void * lpdata,
                           uint32_t size)
{
    if(!page)
        return 0;

    assert(CPAGE_GetNameInternalType(type));
    DefConvertInit();
    return static_cast<PageHandle>(page)->getData(type, lpdata, size);
}

bool CPAGE_GetPageInfo(Handle page, PAGEINFO * info)
{
    return CPAGE_GetPageData(page, PT_PAGEINFO, info, sizeof(PAGEINFO));
}

bool CPAGE_SetPageInfo(Handle page, const PAGEINFO& info)
{
    return CPAGE_SetPageData(page, PT_PAGEINFO, (void*) &info, sizeof(PAGEINFO));
}

Handle CPAGE_GetBlockType(Handle block)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return NULL;
    }

    Block * b = static_cast<Block*>(block);
    return b->type();
}

uint32_t CPAGE_GetBlockUserNum(Handle block)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return (uint32_t) -1;
    }

    Block * b = static_cast<Block*>(block);
    return b->userNum();
}

void CPAGE_SetBlockUserNum(Handle block, uint32_t number)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return;
    }

    Block * b = static_cast<Block*>(block);
    b->setUserNum(number);
}

uint32_t CPAGE_GetBlockFlags(Handle block)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return 0;
    }

    Block * b = static_cast<Block*>(block);
    return b->flags();
}

void CPAGE_SetBlockFlags(Handle block, uint32_t flags)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return;
    }

    Block * b = static_cast<Block*>(block);
    b->setFlags(flags);
}

bool CPAGE_SetBlockData(Handle block, Handle type, const void * data, uint32_t size)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return false;
    }

    Block * b = static_cast<Block*>(block);
    b->setData(type, data, size);
    return true;
}

uint32_t CPAGE_GetBlockData(Handle block, Handle type, void * data, uint32_t size)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return false;
    }

    DefConvertInit();
    Block * b = static_cast<Block*>(block);
    return b->getData(type, data, size);
}

size_t CPAGE_GetCountPage()
{
    return PageStorage::pageCount();
}

uint32_t CPAGE_GetCountBlock(Handle page)
{
    if(!page)
        return 0;

    return static_cast<PageHandle>(page)->blockCount();
}

void CPAGE_DeleteBlock(Handle page, Handle block)
{
    if(!page)
        return;

    static_cast<PageHandle>(page)->removeBlock(static_cast<Block*>(block));
}

#define VERSION_FILE            0xF002

Bool32 CPAGE_SavePage(Handle page, const char * lpName)
{
    if(!page)
        return false;

    std::ofstream os(lpName);

    if(!os)
        return false;

    const uint32_t vers = VERSION_FILE;
    os.write((char*) &vers, sizeof(vers));

    if (page) {
        const uint32_t count = 1;
        os.write((char*) &count, sizeof(count));
        return static_cast<PageHandle>(page)->save(os);
    }
    else {
        const uint32_t count = PageStorage::pageCount();
        os.write((char*) &count, sizeof(count));
        for (uint32_t i = 0; i < count; i++) {
            if(!PageStorage::pageAt(i)->save(os))
                return false;
        }
    }

    os.close();
    return true;
}

Handle CPAGE_RestorePage(Bool32 remove, const char * lpName)
{
    Handle rc = NULL;

    std::ifstream is(lpName);

    if (is) {
        uint32_t vers = 0;
        is.read((char*) &vers, sizeof(vers));

        if (vers != VERSION_FILE) {
            SetReturnCode_cpage(IDS_ERR_OLDFILEVERSION);
            is.close();
            return NULL;
        }

        if (remove) {
            PageStorage::clear();
            NameData.Clear();
        }

        uint32_t count = 0;
        is.read((char*) &count, sizeof(count));

        for (uint32_t i = 0; i < count; i++) {
            BackupPage page;
            if(page.restore(is))
                rc = PageStorage::append(page);
            else
                break;
        }

        is.close();
    }

    return rc;
}

Handle CPAGE_GetHandlePage(uint32_t pos)
{
    return PageStorage::pageAt(pos);
}

Handle CPAGE_GetUserPageType()
{
    return CPAGE_GetUserBlockType();
}

Handle CPAGE_GetUserBlockType()
{
    static uint32_t number = 1;
    char Name[260];
    sprintf(Name, "UserType:%i", number++);
    Handle rc = CPAGE_GetInternalType(Name);
    return rc;
}

Handle CPAGE_GetPageFirst(Handle type)
{
    int count = PageStorage::pageCount();
    int i;
    DefConvertInit();

    for (i = 0; i < count; i++) {
        if (!type ||
                PageStorage::pageAt(i)->type() == type ||
                PageStorage::pageAt(i)->Convert(type, NULL, 0))
            break;
    }

    Handle rc = i < count ? PageStorage::pageAt(i) : NULL;
    return rc;
}

Handle CPAGE_GetPageNext(Handle page, Handle type)
{
    int count = PageStorage::pageCount();
    int pos = PageStorage::findPage((PageHandle) page) + 1;
    int i;

    DefConvertInit();

    for (i = pos; i < count && i >= 0; i++) {
        if (!type ||
                PageStorage::pageAt(i)->type() == type ||
                PageStorage::pageAt(i)->Convert(type, NULL, 0))
            break;
    }

    Handle rc = i < count ? PageStorage::pageAt(i) : NULL;
    return rc;
}

Handle CPAGE_GetBlockFirst(Handle p, Handle type)
{
    if(!p)
        return NULL;

    PageHandle page = static_cast<PageHandle>(p);
    int count = page->blockCount();
    int i;

    DefConvertInit();

    for (i = 0; i < count; i++) {
        if (!type ||
                page->blockAt(i)->type() == type ||
                page->blockAt(i)->Convert(type, NULL, 0))
            break;
    }

    Handle rc = i < count ? page->blockAt(i) : NULL;
    return rc;
}

Handle CPAGE_GetBlockNext(Handle p, Handle block, Handle type)
{
    if(!p)
        return NULL;

    PageHandle page = static_cast<PageHandle>(p);
    int count = page->blockCount();
    int pos = page->findBlock((Block*) block) + 1;
    int i;

    DefConvertInit();

    for (i = pos; i < count && i >= 0; i++) {
        if (!type ||
                page->blockAt(i)->type() == type ||
                page->blockAt(i)->Convert(type, NULL, 0))
            break;
    }

    Handle rc = i < count ? page->blockAt(i) : NULL;
    return rc;
}

bool CPAGE_DeleteAll()
{
    DataConvertor ConvertorPages(DefConvertPage);
    PageStorage::clear();
    NameData.Clear();
    Page::setConvertor(ConvertorPages);
    return true;
}

uint32_t CPAGE_GetCurrentPage()
{
    return (uint32_t) PageStorage::currentPageNumber();
}

bool CPAGE_SetCurrentPage(uint32_t number)
{
    return PageStorage::setCurrentPage(number);
}

uint32_t CPAGE_GetNumberPage(Handle hPage)
{
    if(!hPage)
        return (uint32_t) - 1;

    return PageStorage::findPage((PageHandle) hPage);
}

// Если блоки конвертируемы один в другой, тогда имеет смысл оставить только один,
// наиболее полный тип. Именно это и делает эта функция.
//
Bool32 CPAGE_UpdateBlocks(Handle hPage, Handle type)
{
    Bool32 rc = TRUE;
    uint32_t size = 0;
    char * lpData = NULL;
    Handle temporaray = 0;
    SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
    assert(CPAGE_GetNameInternalType(type));
#endif
    SetReturnCode_cpage(IDS_ERR_NO);
    Handle hBlock = CPAGE_GetBlockFirst(hPage, type);

    if (!hBlock) {
        rc = TRUE;
        goto lOut;
    }

    // Создадим временные блоки и удалим старые.
    // Тем самым предотвращаем зацикливание.
    temporaray = CPAGE_GetInternalType("temporary");

    while (hBlock) {
        Handle hNextBlock = CPAGE_GetBlockNext(hPage, hBlock, type);// type - запрашиваемый тип блока
        Handle dwType = CPAGE_GetBlockType(hBlock); // dwType - Реальный тип блока

        if (dwType != type) { // Была произведена конвертация из типа dwType !
            uint32_t UserNum = CPAGE_GetBlockUserNum(hBlock);
            uint32_t Flags = CPAGE_GetBlockFlags(hBlock);

            if (lpData == NULL) { // Определим необходимый размер и отведем память.
                size = static_cast<Block*>(hBlock)->dataSize();

                if (size) {
                    lpData = (char *) myAlloc(size);

                    if (!lpData) {
                        rc = FALSE;
                        SetReturnCode_cpage(IDS_ERR_NO_MEMORY);
                        break;
                    }
                }

                else {
                    SetReturnCode_cpage(IDS_ERR_DISCREP);
                    rc = FALSE;
                    break;
                }
            }

            if (CPAGE_GetBlockData(hBlock, type, lpData, size) == size) {
                CPAGE_DeleteBlock(hPage, hBlock);

                if (!CPAGE_CreateBlock(hPage, temporaray, UserNum, Flags,
                                       lpData, size)) {
                    SetReturnCode_cpage(IDS_ERR_NO_MEMORY);
                    rc = FALSE;
                    break;
                }
            }
        }

        hBlock = hNextBlock;
    }

    // Переименуем временные блоки
    if (lpData) { // Проверка на существование таких блоков
        myFree(lpData);

        for (hBlock = CPAGE_GetBlockFirst(hPage, temporaray); hBlock;
             hBlock = CPAGE_GetBlockNext(hPage, hBlock, temporaray))
        {
            static_cast<Block*>(hBlock)->setType(type);
        }
    }

lOut:
    return rc;
}

uint32_t CPAGE_GetBlockInterNum(Handle block)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return (uint32_t) -1;
    }

    Block * b = static_cast<Block*>(block);
    return b->interNum();
}

void CPAGE_SetBlockInterNum(Handle block, uint32_t inter)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return;
    }

    Block * b = static_cast<Block*>(block);
    b->setInterNum(inter);
}

bool CPAGE_GetBlockDataPtr(Handle block, Handle type, void ** data)
{
    if(!block) {
        cfError(MODULE_CPAGE) << "NULL block handle given";
        return false;
    }

    Block * b = static_cast<Block*>(block);
    return  b->getDataPtr(type, data);
}

Handle CPAGE_GetInternalType(const char * name)
{
    Handle rc = 0;
    NAMEDATA nd(name);
    SetReturnCode_cpage(IDS_ERR_NO);
    rc = NameData.FindFirst(nd);

    if (!rc)
        rc = NameData.AddTail(nd);

    return rc;
}

const char * CPAGE_GetNameInternalType(Handle type)
{
    if (type == NULL || type == reinterpret_cast<void*>(-1))
        return NULL;

    return NameData.GetItem(type);
}
