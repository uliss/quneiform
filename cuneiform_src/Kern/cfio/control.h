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

#include <string>

#include "cfio.h"
#include "filelist.h"
#include "memorylist.h"
#include "storagelist.h"

namespace CIF {
namespace CTC {

class Control {
public:
	Control();
	~Control();

	// import functions
	Bool32 SetFolder(uint32_t wFolder, char* pcBuff);
	Bool32 GetFolder(uint32_t wFolder, char* pcBuff);

	// import functions
	Handle OpenStorage(char* lpName, uint32_t wTypes);
	Bool32 CloseStorage(Handle hStorage, uint32_t dwFlag);
	Bool32 DeleteStorage(char* lpName);
	Bool32 WriteFileToStorage(Handle hStorage, Handle hFile, char* lpName);
	Handle ReadFileFromStorage(Handle hStorage, char* lpName);
	Handle OpenFile(Handle hFile, char* lpName, uint32_t dwFlag);
	Bool32 CloseFile(Handle hFile, uint32_t dwFlag, Handle hStorage = NULL);
	uint32_t WriteFile(Handle hFile, pchar lpData, uint32_t dwSize);
	uint32_t ReadFromFile(Handle hFile, pchar lpData, uint32_t dwSize);
	uint32_t Seek(Handle hFile, uint32_t dwBytes, uint32_t dwFrom);
	uint32_t Tell(Handle hFile);
	Bool32 Flush(Handle hFile);
	Handle Alloc(uint32_t dwSize, uint32_t dwFlag, const char *cOwner,
			const char *Coment);
	Handle ReAlloc(Handle hMemory, uint32_t wNewSize, uint32_t wFlag);
	Bool32 Free(Handle hMem);
	pvoid Lock(Handle hMem);
	Bool32 Unlock(Handle hMem);
	uint32_t WriteMemToFile(Handle hMem, char* lpName);
	uint32_t ReadMemFromFile(char* lpName, Handle * phMem, uint32_t wFlag =
			MAF_GALL_GHND);
	uint32_t WriteMemToStorage(Handle hMem, Handle hStorage, char* lpName);
	uint32_t ReadMemFromStorage(Handle hStorage, char* lpName, Handle * phMem);

private:
	Handle AddFileInList(GlobalFile * File, uint32_t Flag, Handle Storage);
	Handle
			OpenFileAndAttach(char* lpNAme, uint32_t Flag, Handle Storage =
					NULL);
	Handle AllocNewMemory(uint32_t wFlag, uint32_t wSize, Bool32 Global,
			const char *cOwner, const char* Coment);
	Bool32 AddNewMemoryInList(Handle hMemory, uint32_t wSize,
			uint32_t IsGlobal, const char *cOwner, const char *Coment);
	Bool32 FreeMemory(Handle hMemory, uint32_t wFlag = 0x0);
	Bool32 TakeMemory(Handle hMemory, uint32_t * wMemorySize,
			uint32_t * wMemoryFlag);
	Bool32 GetMemory(Handle hMemory, PPMemoryHeader pHeader);
	Bool32 DeleteMemoryFromList(Handle hMemory);
	pvoid LockMemory(Handle hMemory);
	Bool32 UnlockMemory(Handle hMemory);
	Bool32 LockatorMemoryInList(Handle hMemory, Bool32 bLock);
	Bool32 AttachFileToStorage(Handle File, Handle Storage, uint32_t Flag);
	Bool32 DeleteFileFromList(Handle File, uint32_t Flag, Handle Stotrage =
			NULL);
	Bool32 FlushFile(Handle File);
	uint32_t TellFilePointer(Handle File);
	Bool32 CloseFileAndAttach(Handle File, uint32_t Flag, Handle Storage);
	Bool32 DeleteFileFromDisk(Handle File);
	uint32_t SeekFilePointer(Handle File, uint32_t Position, uint32_t From);
	uint32_t ReadDataFromFile(Handle File, void * lpData, uint32_t Size);
	uint32_t WriteDataToFile(Handle File, void * lpData, uint32_t Size);
	Bool32 CloseFileAndDettach(Handle File, uint32_t Flag, Handle Storage);
	Handle CompliteStorage(Handle Storage, uint32_t Flag);
	uint32_t WriteItemToStorage(StorageHeader * Storage, void * pItem,
			uint32_t Size);
	uint32_t WriteFileToStorage(StorageHeader * Storage, FileHeader * File);
	Bool32 CloseStorageFile(Handle Storage, uint32_t Flag = 0);
	Bool32 CloseAllStorageFile(Handle Storage, uint32_t Flag);
	uint32_t CompliteAllStorage(Handle Storage, uint32_t Flag);
	uint32_t DecompileStorage(Handle Storage);
	Handle AddStorageInList(GlobalFile * lpNewStorageName, uint32_t wNewFlag);
	Handle OpenNewStorage(char* lpName, uint32_t wFlag);
	Handle OpenCompliteStorage(char* lpName, uint32_t wFlag);
	uint32_t ReadFileFromStorage(StorageHeader * Storage, STORAGEITEM * pInfo,
			GlobalFile ** pFile);
	uint32_t ReadItemFromStorage(StorageHeader * Storage, void * lpData,
			uint32_t wSize);
	std::string MakeNameForStorage(const std::string& FileName,
			StorageHeader * phStorage = NULL);
	char* FileNameToFolder(char* Buffer, const char* FolderName,
			const char* FileName, uint32_t Size);

private:
	MemoryList memory_list_;
	FileList file_list_;
	StorageList storage_list_;
	char szTempFolder[CFIO_MAX_PATH];
	char szFileFolder[CFIO_MAX_PATH];
	char szStorageFolder[CFIO_MAX_PATH];
	char szBuffer[CFIO_MAX_PATH];
};
}
}
#endif //_CTC_CONTROL_HEADER_
