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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define                 CFIO_MEMORY_GLOBAL           0x0001
#define                 CFIO_MEMORY_LOCK             0x0002
#define                 CFIO_MEMORY_UNUSED           0x0004
#define                 CFIO_MEMORY_FREE             0x0008

class CTCMemoryHeader : public CTCGlobalHeader
{
public:
	CTCMemoryHeader();
	CTCMemoryHeader(Handle hMemory, uint32_t wBlockSize);
	~CTCMemoryHeader();
	char* GetOwner(void) { return mcOwner; };
	char* GetComment(void) { return mcComment; };

public:
	CTCMemoryHeader(Handle hMemory, uint32_t wBlockSize, const char *OwnerName, const char *Commentary);
	CTCMemoryHeader * GetNext()    { return (CTCMemoryHeader *)(CTCGlobalHeader::GetNext()); };
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
class CTCFileHeader : public CTCGlobalHeader
{
private:
	CTCGlobalFile *      pFile;
	Handle               hStorage;
	uint32_t               wFlag;
	Bool32 KeepFileName;

public:
	Bool32 UnlockFromStorage(void);
	Bool32 LockToStorage(void);
	CTCFileHeader();
	CTCFileHeader(CTCGlobalFile * pNewFile,
		          uint32_t Flag =CFIO_FILE_READ|CFIO_FILE_WRITE,
				  Handle hStorage = NULL);
	~CTCFileHeader();

public:
	Bool32              AttachToStorage(Handle Storage);
	Bool32              DetachFromStorage();
	CTCFileHeader *     GetNext(void) { return (CTCFileHeader *)(CTCGlobalHeader::GetNext()); };
	CTCGlobalFile *     GetFile(void) { return pFile; };
	Handle              GetAttaching(void) { return hStorage; };
	Bool32              CanWrite(void) { return !IsFlag(CFIO_FILE_LOCKED); };
	Bool32              KeepName(void) { return KeepFileName = TRUE; };
	Bool32              BreakName(void) { return !( KeepFileName = FALSE); };
	Bool32              HowName(void) { return KeepFileName; };

private:
	Handle              AcceptFile(CTCGlobalFile * File) { return (pFile = File)->GetFileHandle(); };

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Данный класс служит для составления списка сохраненых в хранилище
// файлов. в основном используется при записи хранилища, при открытии
// созданного хранилища к концу работы с ним нуждается в корректировке!
/*
class  CTCStorageContents
{
private:
	uint32_t               wContentsSize;
	CTCStorageContents * pNext;
	Handle               hItem;
	uint32_t               wItemID;
	uint32_t               wItemSize;

public:
	CTCStorageContents():wItemID(3),wItemSize(0),wContentsSize( sizeof ( class CTCStorageContents))
	{pNext = NULL; hItem = NULL;};
	CTCStorageContents(Handle H, uint32_t ID = 0, uint32_t Size = 0, CTCStorageContents * Next = NULL): wItemID(ID),wItemSize(Size),wContentsSize( sizeof ( class CTCStorageContents) )
	{ hItem = H;  pNext = Next;};
	~CTCStorageContents() {};

public:
	Handle       GetHandle() { return hItem; };
	uint32_t       GetID()     { return wItemID; };
    uint32_t       GetSize()   { return wItemSize; };
	uint32_t       GetContentsSize() { return wContentsSize; };
	CTCStorageContents * GetNext() { return pNext; };

public:
	Bool32     DeleteItemFromStorage(Handle Item);
	void       SetHandle(Handle Item) { hItem = Item; };
	void       SetID(uint32_t ID)     { wItemID = ID; };
    void       SetSize(uint32_t Size)   { wItemSize = Size; };
	void       SetNext(CTCStorageContents * Next) {  pNext = Next; };
};
*/
typedef struct
{
	uint32_t    siHeaderSize;
	uint32_t    siItemSize;
	uint32_t    siFlag;
	char      siName[CFIO_MAX_PATH];
}   STORAGEITEM;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTCStorageHeader : public CTCGlobalHeader
{
private:
	CTCGlobalFile *      pStorageFile;
	char                 pcName[CFIO_MAX_PATH];
	char                 pcFolder[CFIO_MAX_PATH];
//	CTCStorageContents   Contents;
	uint32_t               wContensCounter;

//private:

public:
	CTCStorageHeader();
	CTCStorageHeader( CTCGlobalFile * pNewStorage,
	                  uint32_t Flag = CFIO_FILE_READ|CFIO_FILE_WRITE,
					  const char* pcNewStorageFolder = NULL );
	~CTCStorageHeader();

public:
	CTCStorageHeader * GetNext(void) { return (CTCStorageHeader *)(CTCGlobalHeader::GetNext()); };
	CTCGlobalFile *    GetStorageFile(void) { return pStorageFile; };
	uint32_t             IncreaseContentsCounter() { return ++wContensCounter; };
	uint32_t             DecreaseContentsCounter() { return --wContensCounter; };
	CTCGlobalFile *    GetStorage() { return pStorageFile; };
	char*             GetStorageFolder() { return pcFolder; };
//	Handle             EnumItemContents(Handle Item = NULL);
//	Bool32             DeleteItemFromStorage(Handle Item);
//	Bool32             AddItemToStorage(Handle hNewItem, uint32_t wID = 0, uint32_t wNewSize = 0);

private:
	Handle             AcceptFile(CTCGlobalFile * File) { return (pStorageFile = File)->GetFileHandle(); };
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTCFileList
{
private:
	uint32_t               wListSize;
	CTCStorageHeader *   pList;
	uint32_t               wFileCounter;
	uint32_t               wSpaceCounter;
	CTCFileHeader        mfFirstItem;
	CTCFileHeader        mfLastItem;

public:
	Handle FindFile(char* lpFileName);
	CTCFileList();
	~CTCFileList();

public:
	Handle              AddItem(CTCGlobalFile * pNewFile, uint32_t wNewFlag, Handle Storage);
	Bool32              DeleteItem(Handle File, uint32_t Flag = 0);
	Handle              GetAttachedFileHeader(Handle Storage, CTCFileHeader * File = NULL);
	CTCFileHeader *     GetItemHeader(Handle File);
	CTCGlobalFile *     GetItem(Handle File);

private:
	CTCFileHeader *     pLast() { return &mfLastItem; };
	CTCFileHeader *     pFirst() { return &mfFirstItem; };
	uint32_t              IncreaseFileCounter(void)    { return ++wFileCounter; }
	uint32_t              DecreaseFileCounter(void)    { return --wFileCounter; }
	uint32_t              IncreaseSpaceCounter(void)   { return ++wSpaceCounter; }
	uint32_t              DecreaseSpaceCounter(void)   { return --wSpaceCounter; }
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTCStorageList
{
private:
	uint32_t               wHeaderSize;
	CTCStorageHeader     msFirstItem;
	CTCStorageHeader     msLastItem;
	CTCStorageHeader   * pList;
	uint32_t               wItemCounter;

public:
	Handle FindStorage(char* lpName);
	CTCStorageList();
	~CTCStorageList();

public:
	Handle             AddItem(CTCGlobalFile * NewStorage, uint32_t wParametr);
	Bool32             DeleteItem(Handle Storage, uint32_t Flag = 0);
	CTCGlobalFile *    GetItem(Handle Storage);
	CTCStorageHeader * GetFirstItemHeader() { return pFirst()->GetNext(); };
	CTCStorageHeader * GetItemHeader(Handle Storage);
	Bool32             IsEmpty(){ return wItemCounter == 0; };

private:
	CTCStorageHeader * pFirst(void) { return &msFirstItem; };
	CTCStorageHeader * pLast(void) { return &msLastItem; };
	uint32_t IncreaseItemCounter(void) { return ++wItemCounter; };
	uint32_t DecreaseItemCounter(void) { return --wItemCounter; };
	uint32_t HowItems(void) { return wItemCounter; };
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif        //_CTC_CLASSES_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//end of file
