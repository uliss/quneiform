/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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

# if defined (_DEBUG)
# pragma message( __FILE__" : Final defenition")
# endif // (_DEBUG)
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
//   Äàííûé êëàññ ñëóæèò äëÿ ñîñòàâëåíèÿ ñïèñêà ñîõðàíåíûõ â õðàíèëèùå
// ôàéëîâ. â îñíîâíîì èñïîëüçóåòñÿ ïðè çàïèñè õðàíèëèùà, ïðè îòêðûòèè
// ñîçäàííîãî õðàíèëèùà ê êîíöó ðàáîòû ñ íèì íóæäàåòñÿ â êîððåêòèðîâêå!
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
					  PChar8 pcNewStorageFolder = NULL );
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
	CTCMemoryHeader      mhLastItem;
	CTCMemoryHeader      mhFirstItem;
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
