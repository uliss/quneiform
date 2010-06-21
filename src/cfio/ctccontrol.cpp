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

//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//                                                                              //
//                    Cognitive Technologies Ltd.                               //
//                                                                              //
//                    Data Storage Container                                    //
//                                                                              //
//                                                                              //
//                                                                              //
//                                                                              //
//                    started at 25 may 1998                                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include "compat/filefunc.h"
#include "ctccontrol.h"
#include "cfcompat.h"
void SetReturnCode_cfio(uint16_t rc);
//////////////////////////////////////////////////////////////////////////////////
//
CTCControl::CTCControl()
{
    char SystemTemp[_MAX_PATH];
    GetTempPath(CFIO_MAX_PATH, SystemTemp);
    SetFolder(CFIO_TEMP_FOLDER, SystemTemp);
    SetFolder(CFIO_FILE_FOLDER, SystemTemp);
    CFIO_STRCAT(SystemTemp, "STORAGE\\");
    SetFolder(CFIO_STORAGE_FOLDER, SystemTemp);
}

CTCControl::~CTCControl() {}

Bool32 CTCControl::GetFolder(uint32_t wFolder, char* pcBuff)
{
    switch (wFolder) {
        case CFIO_TEMP_FOLDER:
            CFIO_STRCPY(pcBuff, (char*) szTempFolder);
            break;
        case CFIO_FILE_FOLDER:
            CFIO_STRCPY(pcBuff, (char*) szFileFolder);
            break;
        case CFIO_STORAGE_FOLDER:
            CFIO_STRCPY(pcBuff, (char*) szStorageFolder);
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

Bool32 CTCControl::SetFolder(uint32_t wFolder, char* pcBuff)
{
    if (strlen(pcBuff) < _MAX_PATH) {
        switch (wFolder) {
            case CFIO_TEMP_FOLDER:
                CFIO_STRCPY((char*) szTempFolder, pcBuff);
                break;
            case CFIO_FILE_FOLDER:
                CFIO_STRCPY((char*) szFileFolder, pcBuff);
                break;
            case CFIO_STORAGE_FOLDER:
                CFIO_STRCPY((char*) szStorageFolder, pcBuff);
                break;
            default:
                return FALSE;
        }

        return TRUE;
    }

    return FALSE;
}

Handle CTCControl::Alloc(uint32_t wSize, uint32_t wFlag, const char *cOwner,
                         const char *Coment)
{
    int iTestFixed = 0;
    Bool32 Global = FALSE;
    uint32_t GlobalFlag = 0x0;

    if (wFlag & MAF_GPTR || wFlag & MAF_GNHD) {
        return NULL;
    }

#ifdef CFIO_USE_GLOBAL_MEMORY
    Global = TRUE;
#else
    Global = FALSE;
#endif

    if (wFlag & MAF_GALL_GMEM_FIXED) {
        GlobalFlag |= GMEM_FIXED;
        iTestFixed = 1;
    }

    if (wFlag & MAF_GALL_GMEM_MOVEABLE && !iTestFixed)
        GlobalFlag |= GMEM_MOVEABLE;

    if (wFlag & MAF_GALL_GPTR)
        GlobalFlag |= GPTR;

    if (wFlag & MAF_GALL_GHND)
        GlobalFlag |= GHND;

    if (wFlag & MAF_GALL_GMEM_DDESHARE)
        GlobalFlag |= GMEM_DDESHARE;

    if (wFlag & MAF_GALL_GMEM_DISCARDABLE && iTestFixed)
        GlobalFlag |= GMEM_DISCARDABLE;

    if (wFlag & MAF_GALL_GMEM_LOWER)
        GlobalFlag |= GMEM_LOWER;

    if (wFlag & MAF_GALL_GMEM_NOCOMPACT)
        GlobalFlag |= GMEM_NOCOMPACT;

    if (wFlag & MAF_GALL_GMEM_NODISCARD)
        GlobalFlag |= GMEM_NODISCARD;

    if (wFlag & MAF_GALL_GMEM_NOT_BANKED)
        GlobalFlag |= GMEM_NOT_BANKED;

    if (wFlag & MAF_GALL_GMEM_NOTIFY)
        GlobalFlag |= GMEM_NOTIFY;

    if (wFlag & MAF_GALL_GMEM_SHARE)
        GlobalFlag |= GMEM_SHARE;

    if (wFlag & MAF_GALL_GMEM_ZEROINIT)
        GlobalFlag |= GMEM_ZEROINIT;

    return AllocNewMemory(GlobalFlag, wSize, Global, cOwner, Coment);
}

Bool32 CTCControl::Free(Handle hMem)
{
    return FreeMemory(hMem);
}
//////////////////////////////////////////////////////////////////////////////////
//
pvoid CTCControl::Lock(Handle hMem)
{
    return LockMemory(hMem);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::Unlock(Handle hMem)
{
    return UnlockMemory(hMem);
}

//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::AllocNewMemory(uint32_t wFlag, uint32_t wSize,
                                  Bool32 bGlobal, const char *cOwner, const char *Coment)
{
    Handle hNewMemory = NULL;

    if (bGlobal) {
        // alloc new memory;
        hNewMemory = CFIO_ALLOC(wFlag, wSize);
    }

    else {
        hNewMemory = (Handle) ::new char[wSize];
    }

    if (hNewMemory) {
        //add to list
        if (AddNewMemoryInList(hNewMemory, wSize, bGlobal, cOwner, Coment))
            return hNewMemory;

        else
            //or free back
            GlobalFree(hNewMemory);
    }

    return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::AddNewMemoryInList(Handle hMemory, uint32_t wSize,
                                      uint32_t IsGlobal, const char *cOwner, const char *Coment)
{
    return MemoryList.AddItem(hMemory, wSize, IsGlobal, cOwner, Coment);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::TakeMemory(Handle hMemory, uint32_t * wMemorySize,
                              uint32_t * wMemoryFlag)
{
    return MemoryList.TakeItem(hMemory, wMemorySize, wMemoryFlag);
}

//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::DeleteMemoryFromList(Handle hMemory)
{
    return MemoryList.DeleteItem(hMemory);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::LockatorMemoryInList(Handle hMemory, Bool32 bLock)
{
    return MemoryList.LockUnlockItem(hMemory, bLock);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::FreeMemory(Handle hMemory, uint32_t wFlag)
{
    uint32_t wMemoryStatus;
    uint32_t wMemorySize;

    if (TakeMemory(hMemory, &wMemorySize, &wMemoryStatus)) {
        // delete header from list
        if (DeleteMemoryFromList(hMemory)) {
            //now memory free at MemoryHeader destructor
            //if ( !GlobalFree(hMemory) )
            return TRUE;
        }
    }

    return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
pvoid CTCControl::LockMemory(Handle hMemory)
{
    uint32_t wMemoryStatus;
    uint32_t wMemorySize;
    pvoid pMemory = NULL;
    // ну , тут осталось немного. отписывать адрес в память,
    // но пока тут падает, а так как глобальная память не используется, то и
    // оставим это на потом
    //PCTCMemoryHeader pMemoryHeader;

    if (TakeMemory(hMemory, &wMemorySize, &wMemoryStatus)) {
        //if ( GetMemory(hMemory, &pMemoryHeader) )
        //{
        //  wMemorySize = pMemoryHeader->GetSize();
        //  wMemoryStatus = pMemoryHeader->GetFlag();
        if (wMemorySize && (wMemoryStatus & CFIO_MEMORY_GLOBAL)) {
            // if memory can be locked
            if (LockatorMemoryInList(hMemory, TRUE))
#ifdef CFIO_USE_GLOBAL_MEMORY
                pMemory = CFIO_LOCK(hMemory);

#else
                pMemory = hMemory;
#endif
        }

        else {
            pMemory = hMemory;
        }
    }

    return pMemory;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::UnlockMemory(Handle hMemory)
{
    uint32_t wMemoryStatus;
    uint32_t wMemorySize;
    Bool32 bUnlock = FALSE;

    if (TakeMemory(hMemory, &wMemorySize, &wMemoryStatus)) {
        if (wMemorySize && (wMemoryStatus & CFIO_MEMORY_GLOBAL)) {
            // if memory can be unlocked
            if (LockatorMemoryInList(hMemory, FALSE))
#ifdef CFIO_USE_GLOBAL_MEMORY
                bUnlock = CFIO_UNLOCK(hMemory);

#else
                bUnlock = TRUE;
#endif
        }
    }

    return bUnlock;
}
