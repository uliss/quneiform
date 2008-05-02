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
# ifndef _CTC_CONTROL_HEADER_
# define _CTC_CONTROL_HEADER_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

# if defined (_DEBUG)
# pragma message( __FILE__" : CFIO control")
# endif // (_DEBUG)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <stdlib.h>
#include "cfio.h"
#include "resource.h"
#include "ctcclasses.h"
//////////////////////////////////////////////////////////////////////Memory alloc
#define   ALLOC                new
#define   FREE                 delete
#define   PRIVATE_NEW          new
#define   PRIVATE_DELETE       delete
#define   GLOBAL_NEW           new
#define   GLOBAL_DELETE        delete
//////////////////////////////////////////////////////////////////////////////////
class CTCControl
{
// data members
private:
	CTCMemoryList              MemoryList;
	CTCFileList                FileList;
	CTCStorageList             StorageList;
	char                       szTempFolder[CFIO_MAX_PATH];
	char                       szFileFolder[CFIO_MAX_PATH];
	char                       szStorageFolder[CFIO_MAX_PATH];
	char                       szBuffer[CFIO_MAX_PATH];
	//Word32                     wHeightRC;
	//Word32                     wLowRC;

public:
	CTCControl();
	~CTCControl();

// import functions
public:
	Bool32 SetFolder(Word32 wFolder, PChar8 pcBuff);
	Bool32 GetFolder(Word32 wFolder, PChar8 pcBuff);

// import functions
public:
	Handle OpenStorage(PChar8 lpName, Word32 wTypes);
	Bool32 CloseStorage(Handle  hStorage, Word32  dwFlag);
	Bool32 DeleteStorage(PChar8 lpName);
	Bool32 WriteFileToStorage(Handle hStorage, Handle hFile, PChar8 lpName);
	Handle ReadFileFromStorage(Handle hStorage, PChar8 lpName);
	Handle OpenFile(Handle hFile, PChar8 lpName, Word32 dwFlag);
	Bool32 CloseFile(Handle hFile, Word32 dwFlag, Handle hStorage = NULL);
	Word32 WriteFile(Handle hFile, PInt8 lpData, Word32 dwSize);
	Word32 ReadFromFile(Handle hFile, PInt8 lpData, Word32 dwSize);
	Word32 Seek(Handle hFile, Word32 dwBytes, Word32 dwFrom);
	Word32 Tell(Handle hFile);
	Bool32 Flush(Handle hFile);
	Handle Alloc(Word32 dwSize, Word32 dwFlag, const char *cOwner, const char *Coment);
	Handle ReAlloc(Handle hMemory, Word32 wNewSize, Word32 wFlag);
	Bool32 Free(Handle hMem);
	LPVOID Lock(Handle hMem);
	Bool32 Unlock(Handle hMem);
	Word32 WriteMemToFile(Handle hMem, PChar8 lpName);
	Word32 ReadMemFromFile(PChar8 lpName, Handle * phMem, Word32 wFlag = MAF_GALL_GHND);
	Word32 WriteMemToStorage(Handle hMem, Handle hStorage, PChar8 lpName);
	Word32 ReadMemFromStorage(Handle hStorage, PChar8 lpName, Handle * phMem);

private:
	Handle AddFileInList(CTCGlobalFile * File, Word32 Flag, Handle Storage);
	Handle OpenFileAndAttach(PChar8 lpNAme, Word32 Flag, Handle Storage = NULL);
	Handle AllocNewMemory(Word32 wFlag, Word32 wSize, Bool32 Global, const char *cOwner, const char* Coment);
	Bool32 AddNewMemoryInList( Handle hMemory, Word32 wSize, Word32 IsGlobal, const char *cOwner, const char *Coment);
	Bool32 FreeMemory(Handle hMemory, Word32 wFlag = 0x0);
	Bool32 TakeMemory(Handle hMemory, Word32 * wMemorySize, Word32 * wMemoryFlag);
	Bool32 GetMemory(Handle hMemory, PPCTCMemoryHeader pHeader);
	Bool32 DeleteMemoryFromList(Handle hMemory);
	LPVOID LockMemory(Handle hMemory);
	Bool32 UnlockMemory(Handle hMemory);
	Bool32 LockatorMemoryInList(Handle hMemory, Bool32 bLock);
	Bool32 AttachFileToStorage(Handle File, Handle Storage, Word32 Flag);
	Bool32 DeleteFileFromList(Handle File, Word32 Flag, Handle Stotrage = NULL);
	Bool32 FlushFile(Handle File);
	Word32 TellFilePointer(Handle File);
	Bool32 CloseFileAndAttach(Handle File, Word32 Flag, Handle Storage);
	Bool32 DeleteFileFromDisk(Handle File);
	Word32 SeekFilePointer(Handle File, Word32 Position, Word32 From);
	Word32 ReadDataFromFile(Handle File, void * lpData, Word32 Size);
	Word32 WriteDataToFile(Handle File, void * lpData, Word32 Size);
	Bool32 CloseFileAndDettach(Handle File, Word32 Flag, Handle Storage);
	Handle CompliteStorage(Handle Storage, Word32 Flag);
	Word32 WriteItemToStorage(CTCStorageHeader * Storage, void * pItem, Word32 Size);
	Word32 WriteFileToStorage(CTCStorageHeader * Storage, CTCFileHeader * File);
	Bool32 CloseStorageFile(Handle Storage, Word32 Flag = 0);
	Bool32 CloseAllStorageFile(Handle Storage, Word32 Flag);
	Word32 CompliteAllStorage(Handle Storage, Word32 Flag);
	Word32 DecompileStorage(Handle Storage);
	Handle AddStorageInList(CTCGlobalFile * lpNewStorageName, Word32 wNewFlag);
	Handle OpenNewStorage(PChar8 lpName, Word32 wFlag);
	Handle OpenCompliteStorage(PChar8 lpName, Word32 wFlag);
	Word32 ReadFileFromStorage(CTCStorageHeader * Storage, STORAGEITEM * pInfo, CTCGlobalFile ** pFile);
	Word32 ReadItemFromStorage(CTCStorageHeader * Storage, void * lpData, Word32 wSize);
	PChar8 MakeNameForStorage(PChar8 FileName, CTCStorageHeader * phStorage = NULL, Bool32 KeepName = FALSE);
	PChar8 FileNameToFolder(PChar8 Buffer, PChar8 FolderName, PChar8 FileName, Word32 Size);
};
//////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif //_CTC_CONTROL_HEADER_
//////////////////////////////////////////////////////////////////////////////////////////////////
//end of file
