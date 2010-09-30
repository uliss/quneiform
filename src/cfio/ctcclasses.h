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
//
#ifndef _CTC_CLASSES_
#define _CTC_CLASSES_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "cfio.h"
#include "resource.h"
#include "ctcbaseclasses.h"
#include "ctcglobalheader.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define                 CFIO_MEMORY_GLOBAL           0x0001
#define                 CFIO_MEMORY_LOCK             0x0002
#define                 CFIO_MEMORY_UNUSED           0x0004
#define                 CFIO_MEMORY_FREE             0x0008

class CTCMemoryHeader : public cf::CTC::GlobalHeader
{
    public:
        CTCMemoryHeader();
        CTCMemoryHeader(Handle hMemory, uint32_t wBlockSize);
        ~CTCMemoryHeader();
    public:
        CTCMemoryHeader(Handle hMemory, uint32_t wBlockSize, const char *OwnerName, const char *Commentary);
        CTCMemoryHeader * GetNext()    {
            return (CTCMemoryHeader *)(cf::CTC::GlobalHeader::GetNext());
        };
    private:
        char mcComment[CFIO_MAX_COMMENT];
        char mcOwner[CFIO_MAX_OWNER];
};

typedef CTCMemoryHeader *PCTCMemoryHeader, **PPCTCMemoryHeader;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define                CFIO_FILE_CREATE                    CFIO_GF_CREATE
#define                CFIO_FILE_OPEN                      CFIO_GF_OPEN
#define                CFIO_FILE_WRITE                     CFIO_GF_WRITE
#define                CFIO_FILE_READ                      CFIO_GF_READ
#define                CFIO_FILE_APPEND                    CFIO_GF_APPEND
#define                CFIO_FILE_BINARY                    CFIO_GF_BINARY
#define                CFIO_FILE_COMMIT                    CFIO_GF_COMMIT
#define                CFIO_FILE_EXIST                     CFIO_GF_FILE
#define                CFIO_FILE_ATTACHED                  CFIO_GF_STORAGE
#define                CFIO_FILE_IN_MEMORY                 CFIO_GF_IN_MEMORY
#define                CFIO_FILE_TEMPORARY                 CFIO_GF_TEMPORARY
#define                CFIO_FILE_LOCKED                    CFIO_GF_LOCKED
#define                CFIO_FILE_SEEK_CUR                  CFIO_GF_SEEK_CURR
#define                CFIO_FILE_SEEK_BEG                  CFIO_GF_SEEK_BEG
#define                CFIO_FILE_SEEK_END                  CFIO_GF_SEEK_END

typedef struct {
    uint32_t    siHeaderSize;
    uint32_t    siItemSize;
    uint32_t    siFlag;
    char      siName[CFIO_MAX_PATH];
}   STORAGEITEM;

#define    FICTIV_BLOC             0xffffffff
#define    FICTIV_Handle           (void *)FICTIV_BLOC
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTCMemoryList
{
    private:
        uint32_t               wListSize;
        CTCMemoryHeader      mhFirstItem;
        CTCMemoryHeader      mhLastItem;
        uint32_t               wMemoryCounter;
        uint32_t               wItemCounter;

    public:
        CTCMemoryList();
        ~CTCMemoryList();

    public:
        Bool32 AddItem(Handle hMemory, uint32_t wSize, uint32_t wIsGlobal, const char *cOwner, const char *Coment);
        CTCMemoryHeader * GetItem(Handle hMemory);
        Bool32 LockUnlockItem(Handle hMemory, Bool32 bLock);
        Bool32 TakeItem(Handle hMemory, uint32_t * wSize, uint32_t * wFlag);
        Bool32 DeleteItem(Handle hMemory, uint32_t wParam = 0x0);

    private:
        Bool32 KillItem(PCTCMemoryHeader pItem, PCTCMemoryHeader pPrevItem);
        CTCMemoryHeader * pFirst();
        CTCMemoryHeader * pLast();
        uint32_t IncreaseMemoryCounter(uint32_t wSize);
        uint32_t DecreaseMemoryCounter(uint32_t wSize);
};

#endif        //_CTC_CLASSES_

