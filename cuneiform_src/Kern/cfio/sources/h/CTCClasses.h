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
	CTCMemoryHeader(Handle hMemory, Word32 wBlockSize);
	~CTCMemoryHeader();
	PChar8 GetOwner(void) { return mcOwner; };
	PChar8 GetComment(void) { return mcComment; };

public:
	CTCMemoryHeader(Handle hMemory, Word32 wBlockSize, const char *OwnerName, const char *Commentary);
	CTCMemoryHeader * GetNext()    { return (CTCMemoryHeader *)(CTCGlobalHeader::GetNext()); };
private:
	Char8 mcComment[CFIO_MAX_COMMENT];
	Char8 mcOwner[CFIO_MAX_OWNER];
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
	Word32               wFlag;
	Bool32 KeepFileName;

public:
	Bool32 UnlockFromStorage(void);
	Bool32 LockToStorage(void);
	CTCFileHeader();
	CTCFileHeader(CTCGlobalFile * pNewFile,
		          Word32 Flag =CFIO_FILE_READ|CFIO_FILE_WRITE,
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
	Word32               wContentsSize;
	CTCStorageContents * pNext;
	Handle               hItem;
	Word32               wItemID;
	Word32               wItemSize;

public:
	CTCStorageContents():wItemID(3),wItemSize(0),wContentsSize( sizeof ( class CTCStorageContents))
	{pNext = NULL; hItem = NULL;};
	CTCStorageContents(Handle H, Word32 ID = 0, Word32 Size = 0, CTCStorageContents * Next = NULL): wItemID(ID),wItemSize(Size),wContentsSize( sizeof ( class CTCStorageContents) )
	{ hItem = H;  pNext = Next;};
	~CTCStorageContents() {};

public:
	Handle       GetHandle() { return hItem; };
	Word32       GetID()     { return wItemID; };
    Word32       GetSize()   { return wItemSize; };
	Word32       GetContentsSize() { return wContentsSize; };
	CTCStorageContents * GetNext() { return pNext; };

public:
	Bool32     DeleteItemFromStorage(Handle Item);
	void       SetHandle(Handle Item) { hItem = Item; };
	void       SetID(Word32 ID)     { wItemID = ID; };
    void       SetSize(Word32 Size)   { wItemSize = Size; };
	void       SetNext(CTCStorageContents * Next) {  pNext = Next; };
};
*/
typedef struct
{
	Word32    siHeaderSize;
	Word32    siItemSize;
	Word32    siFlag;
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
	Word32               wContensCounter;

//private:

public:
	CTCStorageHeader();
	CTCStorageHeader( CTCGlobalFile * pNewStorage,
	                  Word32 Flag = CFIO_FILE_READ|CFIO_FILE_WRITE,
					  const char* pcNewStorageFolder = NULL );
	~CTCStorageHeader();

public:
	CTCStorageHeader * GetNext(void) { return (CTCStorageHeader *)(CTCGlobalHeader::GetNext()); };
	CTCGlobalFile *    GetStorageFile(void) { return pStorageFile; };
	Word32             IncreaseContentsCounter() { return ++wContensCounter; };
	Word32             DecreaseContentsCounter() { return --wContensCounter; };
	CTCGlobalFile *    GetStorage() { return pStorageFile; };
	PChar8             GetStorageFolder() { return pcFolder; };
//	Handle             EnumItemContents(Handle Item = NULL);
//	Bool32             DeleteItemFromStorage(Handle Item);
//	Bool32             AddItemToStorage(Handle hNewItem, Word32 wID = 0, Word32 wNewSize = 0);

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
	Word32               wListSize;
	CTCMemoryHeader      mhFirstItem;
    CTCMemoryHeader      mhLastItem;
	Word32               wMemoryCounter;
	Word32               wItemCounter;

public:
	CTCMemoryList();
	~CTCMemoryList();

public:
	Bool32 AddItem(Handle hMemory, Word32 wSize, Word32 wIsGlobal, const char *cOwner, const char *Coment);
	CTCMemoryHeader * GetItem(Handle hMemory);
	Bool32 LockUnlockItem(Handle hMemory, Bool32 bLock);
	Bool32 TakeItem(Handle hMemory, Word32 * wSize, Word32 * wFlag);
	Bool32 DeleteItem(Handle hMemory, Word32 wParam = 0x0);

private:
	Bool32 KillItem(PCTCMemoryHeader pItem, PCTCMemoryHeader pPrevItem);
	CTCMemoryHeader * pFirst();
	CTCMemoryHeader * pLast();
	Word32 IncreaseMemoryCounter(Word32 wSize);
	Word32 DecreaseMemoryCounter(Word32 wSize);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTCFileList
{
private:
	Word32               wListSize;
	CTCStorageHeader *   pList;
	Word32               wFileCounter;
	Word32               wSpaceCounter;
	CTCFileHeader        mfFirstItem;
	CTCFileHeader        mfLastItem;

public:
	Handle FindFile(PChar8 lpFileName);
	CTCFileList();
	~CTCFileList();

public:
	Handle              AddItem(CTCGlobalFile * pNewFile, Word32 wNewFlag, Handle Storage);
	Bool32              DeleteItem(Handle File, Word32 Flag = 0);
	Handle              GetAttachedFileHeader(Handle Storage, CTCFileHeader * File = NULL);
	CTCFileHeader *     GetItemHeader(Handle File);
	CTCGlobalFile *     GetItem(Handle File);

private:
	CTCFileHeader *     pLast() { return &mfLastItem; };
	CTCFileHeader *     pFirst() { return &mfFirstItem; };
	Word32              IncreaseFileCounter(void)    { return ++wFileCounter; }
	Word32              DecreaseFileCounter(void)    { return --wFileCounter; }
	Word32              IncreaseSpaceCounter(void)   { return ++wSpaceCounter; }
	Word32              DecreaseSpaceCounter(void)   { return --wSpaceCounter; }
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTCStorageList
{
private:
	Word32               wHeaderSize;
	CTCStorageHeader     msFirstItem;
	CTCStorageHeader     msLastItem;
	CTCStorageHeader   * pList;
	Word32               wItemCounter;

public:
	Handle FindStorage(PChar8 lpName);
	CTCStorageList();
	~CTCStorageList();

public:
	Handle             AddItem(CTCGlobalFile * NewStorage, Word32 wParametr);
	Bool32             DeleteItem(Handle Storage, Word32 Flag = 0);
	CTCGlobalFile *    GetItem(Handle Storage);
	CTCStorageHeader * GetFirstItemHeader() { return pFirst()->GetNext(); };
	CTCStorageHeader * GetItemHeader(Handle Storage);
	Bool32             IsEmpty(){ return wItemCounter == 0; };

private:
	CTCStorageHeader * pFirst(void) { return &msFirstItem; };
	CTCStorageHeader * pLast(void) { return &msLastItem; };
	Word32 IncreaseItemCounter(void) { return ++wItemCounter; };
	Word32 DecreaseItemCounter(void) { return --wItemCounter; };
	Word32 HowItems(void) { return wItemCounter; };
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif        //_CTC_CLASSES_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//end of file
