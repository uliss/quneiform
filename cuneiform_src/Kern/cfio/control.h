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
	bool SetFolder(uint32_t wFolder, char* pcBuff);
	bool GetFolder(uint32_t wFolder, char* pcBuff);

	// import functions
	Handle OpenStorage(const std::string& Name, uint Types);
	bool CloseStorage(Handle hStorage, uint Flag);
	bool DeleteStorage(const std::string& Name);
	bool WriteFileToStorage(Handle hStorage, Handle hFile,
			const std::string& Name);
	Handle ReadFileFromStorage(Handle hStorage, char* lpName);
	Handle OpenFile(Handle hFile, const std::string& Name, uint Flag);
	bool CloseFile(Handle hFile, uint Flag, Handle hStorage = NULL);
	uint32_t WriteFile(Handle hFile, char * lpData, uint32_t Size);
	uint32_t ReadFromFile(Handle hFile, char * lpData, uint32_t Size);
	uint32_t Seek(Handle hFile, uint32_t Bytes, uint32_t From);
	uint32_t Tell(Handle hFile) const;
	bool Flush(Handle hFile);
	Handle Alloc(size_t Size, uint Flag, const std::string& Owner,
			const std::string& Comment);
	Handle ReAlloc(Handle hMemory, uint32_t wNewSize, uint Flag);
	bool Free(Handle hMem);
	void * Lock(Handle hMem);
	bool Unlock(Handle hMem);
	uint32_t WriteMemToFile(Handle hMem, const std::string& Name);
	uint32_t ReadMemFromFile(const std::string& Name, Handle * phMem, uint Flag =
			MAF_GALL_GHND);
	uint32_t WriteMemToStorage(Handle hMem, Handle hStorage, const std::string& Name);
	uint32_t ReadMemFromStorage(Handle hStorage, char* lpName, Handle * phMem);

private:
	Handle AddFileInList(GlobalFile * File, uint Flag, Handle Storage);
	Handle OpenFileAndAttach(const std::string& Name, uint Flag,
			Handle Storage = NULL);
	Handle AllocNewMemory(uint Flag, uint Size, bool Global,
			const std::string& Owner, const std::string& Comment);
	bool AddNewMemoryInList(Handle hMemory, uint Size, uint32_t IsGlobal,
			const std::string& Owner, const std::string& Comment);
	bool FreeMemory(Handle hMemory, uint Flag = 0x0);
	bool TakeMemory(Handle hMemory, uint32_t * wMemorySize,
			uint32_t * wMemoryFlag);
	bool GetMemory(Handle hMemory, PPMemoryHeader pHeader);
	bool DeleteMemoryFromList(Handle hMemory);
	void * LockMemory(Handle hMemory);
	bool UnlockMemory(Handle hMemory);
	bool LockatorMemoryInList(Handle hMemory, Bool32 bLock);
	bool AttachFileToStorage(Handle File, Handle Storage, uint Flag);
	bool DeleteFileFromList(Handle File, uint Flag, Handle Stotrage = NULL);
	bool FlushFile(Handle File);
	uint32_t TellFilePointer(Handle File) const;
	bool CloseFileAndAttach(Handle File, uint Flag, Handle Storage);
	bool DeleteFileFromDisk(Handle File);
	uint32_t SeekFilePointer(Handle File, uint32_t Position, uint32_t From);
	uint32_t ReadDataFromFile(Handle File, void * lpData, uint32_t Size);
	uint32_t WriteDataToFile(Handle File, void * lpData, uint32_t Size);
	bool CloseFileAndDettach(Handle File, uint Flag, Handle Storage);
	Handle CompliteStorage(Handle Storage, uint Flag);
	uint32_t WriteItemToStorage(StorageHeader * Storage, void * pItem,
			uint32_t Size);
	uint32_t WriteFileToStorage(StorageHeader * Storage, FileHeader * File);
	bool CloseStorageFile(Handle Storage, uint32_t Flag = 0);
	bool CloseAllStorageFile(Handle Storage, uint32_t Flag);
	uint32_t CompliteAllStorage(Handle Storage, uint32_t Flag);
	uint32_t DecompileStorage(Handle Storage);
	Handle AddStorageInList(GlobalFile * lpNewStorageName, uint NewFlag);
	Handle OpenNewStorage(const std::string& Name, uint Flag);
	Handle OpenCompliteStorage(const std::string& Name, uint Flag);
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
