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
# ifndef _CTC_CONTROL_HEADER_
# define _CTC_CONTROL_HEADER_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
