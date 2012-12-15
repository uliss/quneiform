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
#include "backup.h"
#include "namedata.h"
#include "pagestorage.h"

#define SAVE_COMPRESSED  // Сохранять данные в блоках в упакованном виде
extern Handle hCurPage;
extern PtrList<NAMEDATA> NameData;

Handle CPAGE_CreatePage(Handle type, void * lpdata, uint32_t size)
{
    PROLOG;
    using namespace cf::cpage;
    BackupPage tail;
    SetReturnCode_cpage(IDS_ERR_NO);
    Handle hPage = PageStorage::append(tail);

    if (hPage) {
        PageStorage::page(hPage).setData(type, lpdata, size);
        hCurPage = hPage;
    }

    EPILOG;
    return hPage;
}

Handle CPAGE_GetPageType(Handle page)
{
    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);
    Handle rc = PageStorage::pageType(page);
    EPILOG;
    return rc;
}

void CPAGE_DeletePage(Handle page)
{
    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);
    PageStorage::remove(page);

    if (hCurPage == page)
        hCurPage = NULL;

    EPILOG;
}

void CPAGE_ClearBackUp(Handle page)
{
    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);
    PageStorage::clearPage(page);
    EPILOG;
}

Handle CPAGE_BackUp(Handle page)
{
    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);
    Handle rc = PageStorage::backupPage(page);
    EPILOG;
    return rc;
}

bool CPAGE_Undo(Handle page, Handle num)
{
    PROLOG;
    using namespace cf::cpage;
    bool rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);

    if (num == NULL) {
        uint32_t number = CPAGE_GetBuckUpCount(page);

        if (number) {
            uint32_t cur = CPAGE_GetBuckUpCurPos(page);

            if (cur == 0) {
                rc = FALSE;
                goto lOut;
            }

            num = CPAGE_GetBuckUpHandle(page, cur - 1);
        }

        else {
            rc = FALSE;
            goto lOut;
        }
    }

    rc = PageStorage::undo(page, num);
lOut:
    EPILOG;
    return rc;
}

Bool32 CPAGE_Redo(Handle page, Handle num)
{
    PROLOG;
    using namespace cf::cpage;
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);

    if (num == NULL) {
        uint32_t number = CPAGE_GetBuckUpCount(page);

        if (number) {
            uint32_t cur = CPAGE_GetBuckUpCurPos(page);

            if (cur == (number - 1)) {
                rc = FALSE;
                goto lOut;
            }

            num = CPAGE_GetBuckUpHandle(page, cur + 1);
        }

        else {
            rc = FALSE;
            goto lOut;
        }
    }

    rc = PageStorage::page(page).Redo(num);
lOut:
    EPILOG;
    return rc;
}

uint32_t CPAGE_GetBuckUpCount(Handle page)
{
    PROLOG;
    using namespace cf::cpage;
    size_t rc = PageStorage::page(page).backupCount();
    EPILOG;
    return rc;
}

Handle CPAGE_GetBuckUpHandle(Handle page, uint32_t number)
{
    PROLOG;
    using namespace cf::cpage;
    Handle rc = PageStorage::page(page).backupAt(number);
    EPILOG;
    return rc;
}

uint32_t CPAGE_GetBuckUpCurPos(Handle page)
{
    PROLOG;
    using namespace cf::cpage;
    uint32_t rc = PageStorage::page(page).GetCurPos();
    EPILOG;
    return rc;
}

Handle CPAGE_CreateBlock(Handle page, Handle Type, uint32_t UserNum,
                         uint32_t Flags, void * lpData, uint32_t Size)
{
    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);

    assert(CPAGE_GetNameInternalType(Type));

    Handle rc = PageStorage::page(page).createBlock(Type, UserNum, Flags, lpData, Size);
    EPILOG;
    return rc;
}

Bool32 CPAGE_SetPageData(Handle page, Handle type, void * lpdata, uint32_t size)
{
    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
    assert(CPAGE_GetNameInternalType(type));
#endif
    PageStorage::page(page).setData(type, lpdata, size);
    EPILOG;
    return TRUE;
}

uint32_t CPAGE_GetPageData(Handle page, Handle type, void * lpdata,
                           uint32_t size)
{
    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
    assert(CPAGE_GetNameInternalType(type));
#endif
    DefConvertInit();
    uint32_t rc = PageStorage::page(page).getData(type, lpdata, size);
    EPILOG;
    return rc;
}

bool CPAGE_GetPageInfo(Handle page, PAGEINFO * info)
{
    return CPAGE_GetPageData(page, PT_PAGEINFO, info, sizeof(PAGEINFO));
}

bool CPAGE_SetPageInfo(Handle page, const PAGEINFO& info)
{
    return CPAGE_SetPageData(page, PT_PAGEINFO, (void*) &info, sizeof(PAGEINFO));
}

Handle CPAGE_GetBlockType(Handle p, Handle block)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
    Handle rc = cf::cpage::PageStorage::page(p).blockData(block).type();
    EPILOG;
    return rc;
}

uint32_t CPAGE_GetBlockUserNum(Handle p, Handle block)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
    uint32_t rc = cf::cpage::PageStorage::page(p).blockData(block).userNum();
    EPILOG;
    return rc;
}

void CPAGE_SetBlockUserNum(Handle p, Handle block, uint32_t user)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
    cf::cpage::PageStorage::page(p).blockData(block).setUserNum(user);
    EPILOG;
}

uint32_t CPAGE_GetBlockFlags(Handle p, Handle block)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
    uint32_t rc = cf::cpage::PageStorage::page(p).blockData(block).flags();
    EPILOG;
    return rc;
}

void CPAGE_SetBlockFlags(Handle p, Handle block, uint32_t flags)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
    cf::cpage::PageStorage::page(p).blockData(block).setFlags(flags);
    EPILOG;
}

Bool32 CPAGE_SetBlockData(Handle p, Handle block, Handle Type,
                          void * lpData, uint32_t Size)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
    assert(CPAGE_GetNameInternalType(Type));
#endif
    cf::cpage::PageStorage::page(p).blockData(block).setData(Type, lpData, Size);
    EPILOG;
    return TRUE;
}

uint32_t CPAGE_GetBlockData(Handle p, Handle block, Handle Type,
                            void * lpData, uint32_t Size)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
    assert(CPAGE_GetNameInternalType(Type));
#endif
    DefConvertInit();
    uint32_t rc = cf::cpage::PageStorage::page(p).blockData(block).getData(Type, lpData, Size);
    EPILOG;
    return rc;
}

uint32_t CPAGE_GetCountPage()
{
    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);
    uint32_t rc = PageStorage::size();
    EPILOG;
    return rc;
}

uint32_t CPAGE_GetCountBlock(Handle page)
{
    if(!page)
        return 0;

    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);
    uint32_t rc = PageStorage::page(page).blockCount();
    EPILOG;
    return rc;
}

void CPAGE_DeleteBlock(Handle page, Handle block)
{
    PROLOG;
    using namespace cf::cpage;
    SetReturnCode_cpage(IDS_ERR_NO);
    PageStorage::page(page).removeBlock(static_cast<Block*>(block));
    EPILOG;
}

#define VERSION_FILE            0xF002
#define VERSION_FILE_COMPRESSED 0xF003

Bool32 CPAGE_SavePage(Handle page, const char * lpName)
{
    PROLOG;
    using namespace cf::cpage;
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    Handle file = myOpenSave(lpName);

    if (file) {
#ifdef SAVE_COMPRESSED
        uint32_t vers = VERSION_FILE_COMPRESSED;
#else
        uint32_t vers = VERSION_FILE;
#endif

        if (myWrite(file, &vers, sizeof(vers)) == sizeof(vers)) {
            if (page) {
                int count = 1;
                rc = myWrite(file, &count, sizeof(count)) == sizeof(count);
#ifdef SAVE_COMPRESSED
                rc = PageStorage::page(page).saveCompress(file);
#else
                rc = PageStorage::page(page).save(file);
#endif
            }

            else {
                int i;
                int count = PageStorage::size();
                rc = myWrite(file, &count, sizeof(count)) == sizeof(count);

                for (i = 0; i < count && rc == TRUE; i++)
#ifdef SAVE_COMPRESSED
                    rc = PageStorage::pageAt(i).saveCompress(file);
#else
                    rc = PageStorage::pageAt(i).save(file);
#endif
            }
        }

        myClose(file);
    }

    EPILOG;
    return rc;
}

Handle CPAGE_RestorePage(Bool32 remove, const char * lpName)
{
    PROLOG;
    using namespace cf::cpage;
    Handle rc = NULL;
    Bool decompress = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    Handle file = myOpenRestore(lpName);

    if (file) {
        int i;
        int count;
        uint32_t vers = 0;

        if (myRead(file, &vers, sizeof(vers)) == sizeof(vers)) {
            if (vers == VERSION_FILE_COMPRESSED)
                decompress = TRUE;

            else if (vers != VERSION_FILE) {
                SetReturnCode_cpage(IDS_ERR_OLDFILEVERSION);
                myClose(file);
                return FALSE;
            }

            {
                if (remove) {
                    PageStorage::clear();
                    NameData.Clear();
                }

                if (myRead(file, &count, sizeof(count)) == sizeof(count))
                    for (i = 0; i < count; i++) {
                        BackupPage page;

                        if (decompress ? page.restoreCompress(file)
                                : page.restore(file))
                            rc = PageStorage::append(page);//Page.AddTail(page);

                        else
                            break;
                    }
            }
        }

        myClose(file);
    }

    EPILOG;
    return rc;
}

Handle CPAGE_GetHandlePage(uint32_t pos)
{
    PROLOG;
    using namespace cf::cpage;
    Handle rc = PageStorage::pageHandleAt(pos);
    EPILOG;
    return rc;
}

CPAGE_CONVERTOR CPAGE_SetConvertorPages(CPAGE_CONVERTOR data)
{
    PROLOG;
    using namespace cf::cpage;
    CPAGE_CONVERTOR rc = SetConvertorPages(data);
    EPILOG;
    return rc;
}

CPAGE_CONVERTOR CPAGE_SetConvertorBlocks(Handle page, CPAGE_CONVERTOR data)
{
    PROLOG;
    using namespace cf::cpage;
    CPAGE_CONVERTOR rc = SetConvertorBlocks(data);
    EPILOG;
    return rc;
}

Handle CPAGE_GetUserPageType()
{
    PROLOG;
    Handle rc = CPAGE_GetUserBlockType();
    EPILOG;
    return rc;
}

Handle CPAGE_GetUserBlockType()
{
    PROLOG;
    static uint32_t number = 1;
    char Name[260];
    sprintf(Name, "UserType:%i", number++);
    Handle rc = CPAGE_GetInternalType(Name);
    EPILOG;
    return rc;
}

Handle CPAGE_GetPageFirst(Handle type)
{
    PROLOG;
    using namespace cf::cpage;
    int count = PageStorage::size();
    int i;
#ifdef _DEBUG
    assert(CPAGE_GetNameInternalType(type));
#endif
    DefConvertInit();

    for (i = 0; i < count; i++) {
        if (!type ||
                PageStorage::pageAt(i).type() == type ||
                PageStorage::pageAt(i).Convert(type, NULL, 0))
            break;
    }

    Handle rc = i < count ? PageStorage::pageHandleAt(i) : NULL;
    EPILOG;
    return rc;
}

Handle CPAGE_GetPageNext(Handle page, Handle type)
{
    PROLOG;
    using namespace cf::cpage;
    int count = PageStorage::size();
    int pos = PageStorage::pagePosition(page) + 1;
    int i;
#ifdef _DEBUG
    assert(CPAGE_GetNameInternalType(type));
#endif
    DefConvertInit();

    for (i = pos; i < count && i >= 0; i++) {
        if (!type ||
                PageStorage::pageAt(i).type() == type ||
                PageStorage::pageAt(i).Convert(type, NULL, 0))
            break;
    }

    Handle rc = i < count ? PageStorage::pageHandleAt(i) /*Page.GetHandle(i)*/ : NULL;
    EPILOG;
    return rc;
}

Handle CPAGE_GetBlockFirst(Handle p, Handle type)
{
    PROLOG;
    using namespace cf::cpage;
    int count = PageStorage::page(p).blockCount();
    int i;

    DefConvertInit();

    for (i = 0; i < count; i++) {
        if (!type ||
                PageStorage::page(p).blockAt(i)->type() == type ||
                PageStorage::page(p).blockAt(i)->Convert(type, NULL, 0))
            break;
    }

    Handle rc = i < count ? PageStorage::page(p).blockAt(i) : NULL;
    EPILOG;
    return rc;
}

Handle CPAGE_GetBlockNext(Handle p, Handle block, Handle type)
{
    PROLOG;
    using namespace cf::cpage;
    int count = PageStorage::page(p).blockCount();
    int pos = PageStorage::page(p).findBlockPos(block) + 1;
    int i;

    DefConvertInit();

    for (i = pos; i < count && i >= 0; i++) {
        if (!type ||
                PageStorage::page(p).blockAt(i)->type() == type ||
                PageStorage::page(p).blockAt(i)->Convert(type, NULL, 0))
            break;
    }

    Handle rc = i < count ? PageStorage::page(p).blockAt(i) : NULL;
    EPILOG;
    return rc;
}

Bool32 CPAGE_DeleteAll()
{
    CPAGE_CONVERTOR ConvertorPages = { 0, DefConvertPage }; // Piter
    PROLOG;
    using namespace cf::cpage;
    Bool32 rc = TRUE;
    PageStorage::clear();
    NameData.Clear();
    SetConvertorPages(ConvertorPages); // Piter
    hCurPage = NULL;
    EPILOG;
    return rc;
}

uint32_t CPAGE_GetCurrentPage()
{
    PROLOG;
    uint32_t rc = CPAGE_GetNumberPage(hCurPage);
    EPILOG;
    return rc;
}

Bool32 CPAGE_SetCurrentPage(uint32_t page)
{
    PROLOG;
    Bool32 rc = TRUE;

    if (page >= CPAGE_GetCountPage() || page == (uint32_t) - 1) {
        SetReturnCode_cpage(IDS_ERR_NOPAGE);
        rc = FALSE;
    }

    else
        hCurPage = CPAGE_GetHandlePage(page);

    EPILOG;
    return rc;
}

uint32_t CPAGE_GetNumberPage(Handle hPage)
{
    PROLOG;
    using namespace cf::cpage;
    uint32_t rc = (uint32_t) - 1;

    if (hPage)
        rc = (uint32_t) PageStorage::pages().GetPos(hPage);

    EPILOG;
    return rc;
}

// Если блоки конвертируемы один в другой, тогда имеет смысл оставить только один,
// наиболее полный тип. Именно это и делает эта функция.
//
Bool32 CPAGE_UpdateBlocks(Handle hPage, Handle type)
{
    PROLOG;
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
        Handle dwType = CPAGE_GetBlockType(hPage, hBlock); // dwType - Реальный тип блока

        if (dwType != type) { // Была произведена конвертация из типа dwType !
            uint32_t UserNum = CPAGE_GetBlockUserNum(hPage, hBlock);
            uint32_t Flags = CPAGE_GetBlockFlags(hPage, hBlock);

            if (lpData == NULL) { // Определим необходимый размер и отведем память.
                size = CPAGE_GetBlockData(hPage, hBlock, type, NULL, 0);

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

            if (CPAGE_GetBlockData(hPage, hBlock, type, lpData, size) == size) {
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

        for (hBlock = CPAGE_GetBlockFirst(hPage, temporaray); hBlock; hBlock
                = CPAGE_GetBlockNext(hPage, hBlock, temporaray)) {
            cf::cpage::PageStorage::page(hPage).blockData(hBlock).setType(type);
        }
    }

lOut:
    EPILOG;
    return rc;
}

uint32_t CPAGE_GetBlockInterNum(Handle p, Handle block)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
    uint32_t rc = cf::cpage::PageStorage::page(p).blockData(block).interNum();
    EPILOG;
    return rc;
}

void CPAGE_SetBlockInterNum(Handle p, Handle block, uint32_t inter)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
    cf::cpage::PageStorage::page(p).blockData(block).setInterNum(inter);
    EPILOG;
}

Bool32 CPAGE_GetBlockDataPtr(Handle p, Handle block, Handle Type,
                             void ** lpData)
{
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
    assert(CPAGE_GetNameInternalType(Type));
#endif
    Bool32 rc = cf::cpage::PageStorage::page(p).blockData(block).getDataPtr(Type, lpData);
    EPILOG;
    return rc;
}

Handle CPAGE_GetInternalType(const char * name)
{
    PROLOG;
    Handle rc = 0;
    NAMEDATA nd(name);
    SetReturnCode_cpage(IDS_ERR_NO);
    rc = NameData.FindFirst(nd);

    if (!rc)
        rc = NameData.AddTail(nd);

    EPILOG;
    return rc;
}

char * CPAGE_GetNameInternalType(Handle type)
{
    char * rc = NULL;
    PROLOG;
    SetReturnCode_cpage(IDS_ERR_NO);

    if (type != NULL)
        rc = NameData.GetItem(type);

    EPILOG;
    return rc;
}
