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
#include "ctccontrol.h"
#include "compat_defs.h"

//GLOBAL
void SetReturnCode_cfio(uint16_t rc);
//GLOBAL VARIABLES
static uint16_t            wHeightRC =                         0;
static uint16_t            wLowRC =                            0;
static Handle            hInst =                             NULL;
CTCControl *      Control_ctc =                           NULL;
static int32_t             InitCount =                         0;
static Bool32            InitDone =                          FALSE;

Bool32 CFIO_Init(uint16_t wHeightCode, Handle hStorage)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( !Control_ctc ) {
        Control_ctc = new CTCControl;
        wHeightRC = wHeightCode;
    }

    if ( Control_ctc ) {
        InitCount++;
        return TRUE;
    }

    SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
    return FALSE;
}

Bool32 CFIO_Done()
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc ) {
        if (--InitCount == 0) {
            delete Control_ctc;
            Control_ctc = NULL;
        }

        return InitDone = TRUE;
    }

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
    }

    return FALSE;
}

uint32_t CFIO_GetReturnCode()
{
    return (wHeightRC << 16) | (wLowRC - IDS_ERR_MIN);
}

char * CFIO_GetReturnString(uint32_t dwError)
{
    return NULL;
}

void SetReturnCode_cfio(uint16_t rc)
{
    if ( rc == IDS_CFIO_ERR_NO || wLowRC == IDS_CFIO_ERR_NO )
        wLowRC = rc;
}

uint16_t GetReturnCode_cfio()
{
    return wLowRC;
}

Bool32 CFIO_SetImportData(uint32_t dwType, void * pData)
{
    uint32_t      Folder;
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);
    wLowRC = 0;

    switch (dwType) {
        case CFIO_PCHAR_TEMPORARY_FOLDER:
            Folder = CFIO_TEMP_FOLDER;
            break;
        case CFIO_PCHAR_STORAGE_FOLDER:
            Folder = CFIO_FILE_FOLDER;
            break;
        case CFIO_PCHAR_FILE_FOLDER:
            Folder = CFIO_STORAGE_FOLDER;
            break;
        default:
            wLowRC = IDS_ERR_NOTIMPLEMENT;
            return FALSE;
    }

    return Control_ctc->SetFolder(Folder, (char *)pData);
}

Handle CFIO_OpenStorage(pchar lpName, uint32_t dwTypes)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->OpenStorage((char*)lpName, dwTypes);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Bool32 CFIO_CloseStorage(Handle  hStorage, uint32_t  dwFlag)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->CloseStorage(hStorage, dwFlag);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Bool32 CFIO_DeleteStorage (pchar lpName)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->DeleteStorage((char*)lpName);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

uint32_t CFIO_WriteFileToStorage (Handle hStorage, Handle hFile, pchar lpNameInStorage)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->WriteFileToStorage(hStorage, hFile, (char*)lpNameInStorage);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Handle CFIO_ReadFileFromStorage (Handle hStorage, pchar lpName)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->ReadFileFromStorage(hStorage, (char*)lpName);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Handle CFIO_OpenFreeFile(Handle hStorage, const char * lpName, uint32_t dwFlag)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->OpenFile(hStorage, (char*)lpName, dwFlag);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Bool32 CFIO_CloseFreeFile(Handle hFile, uint32_t dwFlag)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->CloseFile(hFile, dwFlag);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

uint32_t CFIO_WriteToFile (Handle hFile, pchar lpData, uint32_t dwSize)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->WriteFile(hFile, lpData, dwSize);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

uint32_t CFIO_ReadFromFile (Handle hFile, pchar lpData, uint32_t dwSize)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->ReadFromFile(hFile, lpData, dwSize);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

uint32_t CFIO_SeekFilePointer (Handle hFile, uint32_t dwBytes, uint32_t dwFrom)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->Seek(hFile, dwBytes, dwFrom);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

uint32_t CFIO_TellFilePointer (Handle hFile)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->Tell(hFile);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Bool32 CFIO_FlushFile(Handle hFile)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->Flush(hFile);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Handle CFIO_AllocMemory (uint32_t dwSize, uint32_t dwFlag)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->Alloc(dwSize, dwFlag, "CFIO general memory block", "no comment");

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Handle CFIO_DAllocMemory (uint32_t dwSize, uint32_t dwFlag, const char* cOwner, const char* Comment)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->Alloc(dwSize, dwFlag, (char*)cOwner, (char*)Comment);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Handle CFIO_ReAllocMemory (Handle hMemory, uint32_t dwSize, uint32_t dwFlag)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->ReAlloc(hMemory, dwSize, dwFlag);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Bool32 CFIO_FreeMemory(Handle hMem)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->Free(hMem);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Handle CFIO_LockMemory(Handle hMem)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->Lock(hMem);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

Bool32 CFIO_UnlockMemory(Handle hMem)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->Unlock(hMem);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

uint32_t CFIO_WriteMemoryToFile(Handle hMem, pchar lpName)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->WriteMemToFile(hMem, (char*)lpName);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

uint32_t CFIO_ReadMemoryFromFile(const char * lpName, Handle * phMem)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->ReadMemFromFile((char*)lpName, phMem);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

uint32_t CFIO_WriteMemoryToStorage(Handle hMem, Handle hStorage, pchar lpName)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->WriteMemToStorage(hMem, hStorage, (char*)lpName);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}

uint32_t CFIO_ReadMemoryFromStorage(Handle hStorage, pchar lpName, Handle * phMem)
{
    SetReturnCode_cfio(IDS_CFIO_ERR_NO);

    if ( Control_ctc )
        return Control_ctc->ReadMemFromStorage(hStorage, (char*)lpName, phMem);

    else {
        if (InitDone)
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);

        else
            SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);

        return FALSE;
    }
}
