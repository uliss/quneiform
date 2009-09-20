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
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include "control.h"
#include "compat_defs.h"
#include "resource.h"
#include "fileheader.h"
#include "storageheader.h"
#include "memoryheader.h"
#include "memorylist.h"
#include "storagelist.h"

#include <cstring>

using namespace CIF::CTC;

void SetReturnCode_cfio(uint16_t rc);
//////////////////////////////////////////////////////////////////////////////////
//
Control::Control() {
	char SystemTemp[_MAX_PATH];

	GetTempPath(CFIO_MAX_PATH, SystemTemp);
	SetFolder(CFIO_TEMP_FOLDER, SystemTemp);
	SetFolder(CFIO_FILE_FOLDER, SystemTemp);
	CFIO_STRCAT(SystemTemp, "STORAGE\\");
	SetFolder(CFIO_STORAGE_FOLDER, SystemTemp);
}
//////////////////////////////////////////////////////////////////////////////////
//
Control::~Control() {
}
//////////////////////////////////////////////////////////////////////////////////
//
static char SFolder[CFIO_MAX_PATH];
static char SFile[CFIO_MAX_PATH];
static char SExtension[CFIO_MAX_PATH];
static char SOut[CFIO_MAX_PATH];
//////////////////////////////////////////////////////////////////////////////////
//
char* Control::FileNameToFolder(char* Buffer, const char * FolderName,
		const char* FileName, uint32_t Size) {
	char *i, *j;
	uint32_t SizeOfFolder = CFIO_STRLEN(FolderName);
	uint32_t SizeOfFile = CFIO_STRLEN(FileName);
	uint32_t Shift = SizeOfFolder;

	if (FolderName == FileName)
		return NULL;

	if ((SizeOfFolder + SizeOfFile) < Size) {
		if (Buffer != FolderName && Buffer != FileName)
			CFIO_STRCPY(Buffer, FolderName);

		if (Buffer == FileName && Buffer != FolderName) {
			if (FolderName[SizeOfFolder - 1] == '\\' && FileName[0] == '\\') {
				Shift--;
			}

			if (FolderName[SizeOfFolder - 1] != '\\' && FileName[0] != '\\') {
				Shift++;
			}
			//	FileName = FolderName;
			//	FolderName = Buffer;
			for (i = Buffer, j = (char*) FolderName; i < &Buffer[Shift]; *(i
					+ Shift) = *(i), *(i++) = *(j++))
				;

			if (*i != '\\' && *(i - 1) != '\\') {
				*(i - 1) = '\\';
			}
			return Buffer;
		}

		if (Buffer[SizeOfFolder - 1] == '\\' && FileName[0] == '\\') {
			Buffer[SizeOfFolder - 1] = 0x0;
		}

		if (Buffer[SizeOfFolder - 1] != '\\' && FileName[0] != '\\') {
			CFIO_STRCAT(Buffer, "\\");
		}

		CFIO_STRCAT(Buffer, FileName);
	} else
		Buffer[0] = 0x0;

	return Buffer;

}
//////////////////////////////////////////////////////////////////////////////////
//
std::string Control::MakeNameForStorage(const std::string& FileName,
		StorageHeader * hStorageHead) {
	char* i;

	if (FileName.empty())
		return std::string();

	if (FileName.length() > CFIO_MAX_PATH)
		return FileName;

	// копируем папку хранилища
	if (hStorageHead) {
		// FIXME!!! no buffer length check
		CFIO_STRCPY(szBuffer, hStorageHead->GetStorageFolder().c_str());
	} else {
		GetFolder(CFIO_STORAGE_FOLDER, &szBuffer[0]);
	}
	//разделяем на компоненты
	CFIO_GETFOLDERSITEMS(szBuffer, SFolder, SFile, SExtension);
	// только путь в буфере
	CFIO_STRCPY(szBuffer, SFolder);
	// глупый пингвин не знает, что там был только путь
	CFIO_STRCAT(szBuffer, SFile);

	i = &szBuffer[CFIO_STRLEN(szBuffer) - 1];

	i++;
	*i = '/';
	*(i + 1) = 0x0;

	CFIO_GETFOLDERSITEMS(FileName.c_str(), SFolder, SFile, SExtension);

	//  для хранилища оставляем только имя файла с расширением

	CFIO_MAKEPATH(SOut, NULL, SFile, SExtension);

	return SOut;

}

bool Control::GetFolder(uint32_t wFolder, char* pcBuff) {
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
		return false;
	}
	return true;
}

bool Control::SetFolder(uint32_t wFolder, char* pcBuff) {
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

Handle Control::OpenStorage(const std::string& Name, uint Flag) {
	Handle OpenedStorage = NULL;
	if (Flag & OS_OPEN) {
		OpenedStorage = OpenCompliteStorage(Name, Flag);
	} else {
		Flag |= OS_CREATE;
		OpenedStorage = OpenNewStorage(Name, Flag);
	}

	return OpenedStorage;
}

bool Control::CloseStorage(Handle hStorage, uint Flag) {
	if (Flag & CS_WITHOUT_SAVE || Flag & CS_DELETE) {
		if (Flag & CS_ALL)
			return CloseAllStorageFile(hStorage, Flag);
		else
			return CloseStorageFile(hStorage, Flag);
	}

	if (Flag & CS_ALL) {
		CompliteAllStorage(hStorage, Flag);
		return CloseAllStorageFile(hStorage, Flag);
	} else {
		return CloseStorageFile(CompliteStorage(hStorage, Flag));
	}
}

bool Control::DeleteStorage(const std::string& Name) {
	return unlink(Name.c_str()) == 0;
}

bool Control::WriteFileToStorage(Handle hStorage, Handle hFile,
		const std::string& Name) {
	// берем хидер хрангилища.... или не берем, если нет
	StorageHeader * pStorageHeader = storage_list_.GetItemHeader(hStorage);
	// берем хидер файла ........ или не берем, ежели нема
	FileHeader * pFileHeader = file_list_.GetItemHeader(hFile);
	GlobalFile * pFile;

	if (pStorageHeader && pFileHeader) {
		AttachFileToStorage(hFile, hStorage, 0);
		pFile = pFileHeader->GetFile();
		pFile->SetFileName(Name);
		pFileHeader->KeepName();
		return true;
	}

	return false;
}

Handle Control::ReadFileFromStorage(Handle hStorage, char* lpName) {
	char FileName[CFIO_MAX_PATH];
	// берем хидер хранилища.... или не берем, если нет
	StorageHeader * pStorageHeader = storage_list_.GetItemHeader(hStorage);

	if (pStorageHeader) {
		if (FileNameToFolder(FileName,
				pStorageHeader->GetStorageFolder().c_str(), lpName,
				CFIO_MAX_PATH)) {
			return OpenFile(NULL, FileName, OSF_READ | OSF_WRITE | OSF_BINARY);
		}
	}

	return NULL;
}

Handle Control::OpenFile(Handle hStorage, const std::string& Name, uint Flag) {
	uint FileFlag = 0x0;

	if (Flag & OSF_CREATE)
		FileFlag |= CFIO_FILE_CREATE;

	if (Flag & OSF_OPEN)
		FileFlag |= CFIO_FILE_OPEN;

	if (Flag & OSF_READ)
		FileFlag |= CFIO_FILE_READ;

	if (Flag & OSF_WRITE)
		FileFlag |= CFIO_FILE_WRITE;

	if (Flag & OSF_BINARY)
		FileFlag |= CFIO_FILE_BINARY;

	if (Flag & OSF_IN_MEMORY)
		FileFlag |= CFIO_FILE_IN_MEMORY;

	if (Flag & OSF_TEMPORARY)
		FileFlag |= CFIO_FILE_TEMPORARY;

	return OpenFileAndAttach(Name, FileFlag, hStorage);
}

bool Control::CloseFile(Handle hFile, uint Flag, Handle hStorage) {

	if (Flag & CSF_SAVEDISK) {
		return CloseFileAndDettach(hFile, Flag, hStorage);
	}
	if (Flag & CSF_SAVESTORAGE) {
		return CloseFileAndAttach(hFile, Flag, hStorage);
	}
	if (Flag & CSF_DELETE) {
		return DeleteFileFromDisk(hFile);
	}
	if (Flag & CSF_WRITE) {
	}
	return false;
}

uint32_t Control::WriteFile(Handle hFile, pchar lpData, uint32_t wSize) {
	return WriteDataToFile(hFile, (void *) lpData, wSize);
}

uint32_t Control::ReadFromFile(Handle hFile, pchar lpData, uint32_t wSize) {
	return ReadDataFromFile(hFile, (void *) lpData, wSize);
}

uint32_t Control::Seek(Handle hFile, uint32_t wBytes, uint32_t wFrom) {
	uint32_t wDirect;

	switch (wFrom) {
	case FS_END:
		wDirect = CFIO_GF_SEEK_END;
		break;
	case FS_BEGIN:
		wDirect = CFIO_GF_SEEK_BEG;
		break;
	case FS_CUR:
	default:
		wDirect = CFIO_GF_SEEK_CURR;
		break;
	}
	return SeekFilePointer(hFile, wBytes, wDirect);
}

uint32_t Control::Tell(Handle hFile) const {
	return TellFilePointer(hFile);
}

bool Control::Flush(Handle hFile) {
	return FlushFile(hFile);
}

Handle Control::Alloc(uint32_t wSize, uint32_t wFlag, const char *cOwner,
		const char *Coment) {
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
//////////////////////////////////////////////////////////////////////////////////
// GlobalRealloc
Handle Control::ReAlloc(Handle hMemory, uint32_t wNewSize, uint32_t wFlag) {
	Handle hNewMemory;
	uint32_t wOldSize = 0;
	uint32_t wOldFlag = 0;
	uint32_t GlobalFlag = 0;
	void * Sorc;
	void * Desc;

	switch (wFlag) {
	case MRF_NEW_MEMORY:

		memory_list_.TakeItem(hMemory, &wOldSize, &wOldFlag);
		hNewMemory = Alloc(wNewSize, (wOldFlag == TRUE ? MAF_GALL_GHND
				: MAF_GALL_GPTR), "Realloced", "No comment");

		if (hNewMemory) {
			Desc = Lock(hNewMemory);
			Sorc = Lock(hMemory);
			memcpy(Desc, Sorc, (wOldSize < wNewSize ? wOldSize : wNewSize));
			Unlock(hNewMemory);
			Unlock(hMemory);
			Free(hMemory);
		}
		break;

	default:

		if (wFlag & MAF_GALL_GMEM_MOVEABLE)
			GlobalFlag |= GMEM_MOVEABLE;

		if (wFlag & MAF_GALL_GPTR)
			GlobalFlag |= GPTR;

		if (wFlag & MAF_GALL_GHND)
			GlobalFlag |= GHND;

		if (wFlag & MAF_GALL_GMEM_DDESHARE)
			GlobalFlag |= GMEM_DDESHARE;

		{
			MemoryHeader * Memory = memory_list_.GetItem(hMemory);

			if (Memory) {
				hNewMemory = GlobalReAlloc(hMemory, wNewSize, GlobalFlag);
				Memory->SetHandle(hNewMemory);
				Memory->SetSize(wNewSize);
			}
		}
	}

	return hNewMemory;
}

bool Control::Free(Handle hMem) {
	return FreeMemory(hMem);
}

pvoid Control::Lock(Handle hMem) {
	return LockMemory(hMem);
}

bool Control::Unlock(Handle hMem) {
	return UnlockMemory(hMem);
}

uint32_t Control::WriteMemToFile(Handle hMem, char* lpName) {
	Handle hFile = OpenFile(NULL, lpName, OSF_WRITE);
	uint32_t wMemorySize;
	uint32_t wMemoryFlag;
	uint32_t Counter = 0;
	pchar pMem;

	if (hFile && hMem) {
		memory_list_.TakeItem(hMem, &wMemorySize, &wMemoryFlag);
		Seek(hFile, 0, FS_END);
		pMem = (pchar) Lock(hMem);

		if (pMem) {
			Counter += WriteFile(hFile, pMem, wMemorySize);
		}

		Unlock(hMem);

		Flush(hFile);

		CloseFile(hFile, CSF_SAVEDISK, NULL);
	}
	return Counter;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t Control::ReadMemFromFile(char* lpName, Handle * phMem, uint32_t wFlag) {
	Handle hFile = OpenFile(NULL, lpName, OSF_READ | OSF_BINARY);
	Handle hMem;
	pchar pMem;
	uint32_t wFileSize;
	uint32_t Counter = 0;

	if (hFile) {
		wFileSize = Seek(hFile, 0, FS_END);
		Seek(hFile, 0, FS_BEGIN);
		hMem = Alloc(wFileSize, wFlag, "Readed from file", "lpName");
		*phMem = hMem;

		if (hMem) {
			pMem = (pchar) Lock(hMem);

			if (pMem) {
				Counter += ReadFromFile(hFile, pMem, wFileSize);
			}

			Unlock(hMem);
		}
		CloseFile(hFile, CSF_SAVEDISK, NULL);
	}
	return Counter;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t Control::WriteMemToStorage(Handle hMem, Handle hStorage, char* lpName) {
	Handle hFile;
	uint32_t wData = WriteMemToFile(hMem, lpName);

	if (wData) {
		hFile = OpenFile(hStorage, lpName, OSF_READ | OSF_BINARY);

		if (hFile) {
			if (!CloseFile(hFile, CSF_SAVESTORAGE, hStorage))
				wData = 0;
		} else {
			wData = 0;
		}
	}
	return wData;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t Control::ReadMemFromStorage(Handle hStorage, char* lpName,
		Handle * phMem) {
	char NameForStorage[CFIO_MAX_PATH];
	Handle hMem;
	uint32_t Readed = 0;

	*phMem = NULL;
	StorageHeader * hStorageHead = storage_list_.GetItemHeader(hStorage);

	if (hStorageHead) {
		FileNameToFolder(NameForStorage,
				hStorageHead->GetStorageFolder().c_str(), lpName, _MAX_PATH);

		Readed = ReadMemFromFile(NameForStorage, &hMem);

		if (Readed)
			*phMem = hMem;
	}

	return Readed;
}

Handle Control::AllocNewMemory(uint Flag, uint Size, bool Global,
		const std::string& Owner, const std::string& Comment) {
	Handle hNewMemory = NULL;

	if (Global) {
		// alloc new memory;
		hNewMemory = CFIO_ALLOC(Flag, Size);
	} else {
		hNewMemory = (Handle) ::new char[Size];
	}

	if (hNewMemory) {
		//add to list
		if (AddNewMemoryInList(hNewMemory, Size, Global, Owner, Comment))
			return hNewMemory;
		else
			//or free back
			GlobalFree(hNewMemory);
	}

	return NULL;
}

bool Control::AddNewMemoryInList(Handle hMemory, uint Size, uint32_t IsGlobal,
		const std::string& Owner, const std::string& Comment) {
	return memory_list_.AddItem(hMemory, Size, IsGlobal, Owner, Comment);
}

bool Control::TakeMemory(Handle hMemory, uint32_t * wMemorySize,
		uint32_t * wMemoryFlag) {
	return memory_list_.TakeItem(hMemory, wMemorySize, wMemoryFlag);
}

bool Control::GetMemory(Handle hMemory, PPMemoryHeader pHeader) {
	*pHeader = memory_list_.GetItem(hMemory);
	return pHeader != NULL;
}

bool Control::DeleteMemoryFromList(Handle hMemory) {
	return memory_list_.DeleteItem(hMemory);
}

bool Control::LockatorMemoryInList(Handle hMemory, Bool32 bLock) {
	return memory_list_.LockUnlockItem(hMemory, bLock);
}

bool Control::FreeMemory(Handle hMemory, uint32_t /*wFlag*/) {
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

void * Control::LockMemory(Handle hMemory) {
	uint32_t wMemoryStatus;
	uint32_t wMemorySize;
	pvoid pMemory = NULL;
	// ну , тут осталось немного. отписывать адрес в память,
	// но пока тут падает, а так как глобальная память не используется, то и
	// оставим это на потом
	//PCTCMemoryHeader pMemoryHeader;

	if (TakeMemory(hMemory, &wMemorySize, &wMemoryStatus)) {
		if (wMemorySize && (wMemoryStatus & CFIO_MEMORY_GLOBAL)) {
			// if memory can be locked
			if (LockatorMemoryInList(hMemory, TRUE))
#ifdef CFIO_USE_GLOBAL_MEMORY
				pMemory = CFIO_LOCK(hMemory);
#else
				pMemory = hMemory;
#endif
		} else {
			pMemory = hMemory;
		}
	}

	return pMemory;
}

bool Control::UnlockMemory(Handle hMemory) {
	uint32_t wMemoryStatus;
	uint32_t wMemorySize;
	Bool bUnlock = false;

	if (TakeMemory(hMemory, &wMemorySize, &wMemoryStatus)) {
		if (wMemorySize && (wMemoryStatus & CFIO_MEMORY_GLOBAL)) {
			// if memory can be unlocked
			if (LockatorMemoryInList(hMemory, false))
#ifdef CFIO_USE_GLOBAL_MEMORY
				bUnlock = CFIO_UNLOCK(hMemory);
#else
				bUnlock = true;
#endif
		}
	}
	return bUnlock;
}

Handle Control::OpenFileAndAttach(const std::string& Name, uint Flag,
		Handle Storage) {
	GlobalFile * pNewFile = NULL;
	Handle hOpened;

	// пока не используем
	// см так же DecompliteStorage
	//MAKEFULLPATH(szBuffer, lpName, _MAX_PATH);

	if (Name.length() >= _MAX_PATH)
		return NULL;

	hOpened = file_list_.FindFile(Name);
	if (hOpened)
		return hOpened;

	pNewFile = new GlobalFile(Name, Flag);
	if (pNewFile)
		return AddFileInList(pNewFile, Flag, Storage);

	return NULL;
}

Handle Control::AddFileInList(GlobalFile * File, uint Flag, Handle Storage) {
	Handle ret = file_list_.AddItem(File, Flag, Storage);
	if (!ret)
		SetReturnCode_cfio(IDS_CFIO_ERR_CANT_OPEN_FILE);
	return ret;
}

bool Control::DeleteFileFromList(Handle File, uint /*Flag*/, Handle /*Storage*/) {
	return file_list_.DeleteItem(File);
}

Handle Control::AddStorageInList(GlobalFile * lpNewStorageName,
		uint32_t NewFlag) {
	return storage_list_.AddItem(lpNewStorageName, NewFlag);
}

bool Control::CloseFileAndDettach(Handle File, uint32_t /*Flag*/, Handle /*Storage*/) {
	FileHeader * CurrentFileHeader = file_list_.GetItemHeader(File);
	GlobalFile * CurrentFile;

	if (CurrentFileHeader) {
		CurrentFileHeader->DetachFromStorage();
		CurrentFile = CurrentFileHeader->GetFile();
		file_list_.DeleteItem(File);

		if (CurrentFile) {
			//delete CurrentFile;        //AK   clear mistakes
			return true;
		}
	}

	return false;
}

bool Control::CloseFileAndAttach(Handle File, uint32_t /*Flag*/, Handle Storage) {
	FileHeader * CurrentFileHeader = file_list_.GetItemHeader(File);

	if (CurrentFileHeader) {
		if (CurrentFileHeader->AttachToStorage(Storage))
			return CurrentFileHeader->LockToStorage();
	}
	return false;
}

bool Control::AttachFileToStorage(Handle File, Handle Storage, uint32_t /*Flag*/) {
	FileHeader * AttachedFile = file_list_.GetItemHeader(File);
	StorageHeader * AttacherStorage = storage_list_.GetItemHeader(Storage);

	if (AttacherStorage && AttachedFile) {
		return AttachedFile->AttachToStorage(Storage);
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t Control::WriteDataToFile(Handle File, void * lpData, uint32_t Size) {
	FileHeader * CurrentFileHeader = file_list_.GetItemHeader(File);
	GlobalFile * CurrentFile;
	uint32_t WritedDataCount = 0;

	if (CurrentFileHeader) {
		if (CurrentFileHeader->CanWrite()) {
			CurrentFile = CurrentFileHeader->GetFile();
			WritedDataCount = CurrentFile->Write(lpData, sizeof(char), Size);
		}
	}
	if (WritedDataCount != Size)
		SetReturnCode_cfio(IDS_CFIO_ERR_CANT_WRITE);

	return WritedDataCount;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t Control::ReadDataFromFile(Handle File, void * lpData, uint32_t Size) {
	GlobalFile * CurrentFile = file_list_.GetItem(File);
	uint32_t ReadedDataCount = 0;

	if (CurrentFile) {
		ReadedDataCount = CurrentFile->Read(lpData, sizeof(char), Size);
	}

	return ReadedDataCount;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t Control::SeekFilePointer(Handle File, uint32_t Position, uint32_t From) {
	GlobalFile * CurrentFile = file_list_.GetItem(File);
	uint32_t Seeker = 0;

	if (CurrentFile) {
		Seeker = CurrentFile->Seek(Position, From);
	}

	return Seeker;

}

uint32_t Control::TellFilePointer(Handle File) const {
	FileList * fl = const_cast<FileList*> (&file_list_);
	GlobalFile * CurrentFile = fl->GetItem(File);
	uint32_t Seeker = 0;

	if (CurrentFile) {
		Seeker = CurrentFile->Tell();
	}

	return Seeker;
}

bool Control::FlushFile(Handle File) {
	GlobalFile * CurrentFile = file_list_.GetItem(File);

	if (CurrentFile)
		return CurrentFile->Flush();

	return false;
}

bool Control::DeleteFileFromDisk(Handle File) {
	GlobalFile * CurrentFile = file_list_.GetItem(File);

	if (CurrentFile) {
		CurrentFile->SetDelete();

		if (file_list_.DeleteItem(File)) {
			delete CurrentFile;
			return true;
		}
	}

	return true;
}

uint32_t Control::WriteItemToStorage(StorageHeader * Storage, void * pItem,
		uint32_t wSize) {
	GlobalFile * CurrentStorage = Storage->GetStorage();
	uint32_t WritedDataCount = 0;

	if (CurrentStorage) {
		WritedDataCount = CurrentStorage->Write(pItem, sizeof(char), wSize);
	}
	return WritedDataCount;
}

uint32_t Control::ReadItemFromStorage(StorageHeader * Storage, void * lpData,
		uint32_t wSize) {
	GlobalFile * CurrentStorage = Storage->GetStorage();
	uint32_t ReadedDataCount = 0;

	if (CurrentStorage) {
		ReadedDataCount = CurrentStorage->Read(lpData, sizeof(char), wSize);
	}
	return ReadedDataCount;
}

#define          COPYBUFFER                     512
static char CopyBuffer[COPYBUFFER];

uint32_t Control::WriteFileToStorage(StorageHeader * Storage, FileHeader * File) {
	FileHeader * pItemHeader = File;
	GlobalFile * pItem;
	STORAGEITEM ItemInfo;
	uint32_t FileSize = 0;
	uint32_t FileRealBuffer = 0;
	uint32_t WritedDataCount = 0;

	if (Storage) {
		pItem = pItemHeader->GetFile();
		ItemInfo.siHeaderSize = sizeof(ItemInfo);
		FileSize = ItemInfo.siItemSize = pItem->GetFileSize();
		ItemInfo.siFlag = pItemHeader->GetFlag();
		CFIO_STRCPY(ItemInfo.siName, MakeNameForStorage(pItem->GetFileName(),
						Storage).c_str());

		if (FileSize) {
			WritedDataCount = WriteItemToStorage(Storage, &ItemInfo,
					sizeof(ItemInfo));

			while (FileSize) {
				FileRealBuffer = FileSize > COPYBUFFER ? COPYBUFFER : FileSize;

				pItem->Read(CopyBuffer, sizeof(char), FileRealBuffer);
				WritedDataCount += WriteItemToStorage(Storage, CopyBuffer,
						FileRealBuffer);

				FileSize -= FileRealBuffer;
			}
		}
	}
	return WritedDataCount;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Storage - pointer to storage,
// pInfo - pointer to INFO for File
// pFile - pointer to opened new file
uint32_t Control::ReadFileFromStorage(StorageHeader * Storage,
		STORAGEITEM * pInfo, GlobalFile ** pFile) {
	STORAGEITEM ItemInfo;
	GlobalFile * NewFile;
	uint32_t ReadedDataCount = 0;
	uint32_t FileSize;
	uint32_t FileRealBuffer;
	//Handle                     hNewFile;

	ReadedDataCount
			+= ReadItemFromStorage(Storage, &ItemInfo, sizeof(ItemInfo));

	(*pInfo).siHeaderSize = ItemInfo.siHeaderSize;
	FileSize = (*pInfo).siItemSize = ItemInfo.siItemSize;
	(*pInfo).siFlag = ItemInfo.siFlag;
	// создаем имя файла
	FileNameToFolder(ItemInfo.siName, Storage->GetStorageFolder().c_str(),
			ItemInfo.siName, _MAX_PATH);
	CFIO_STRCPY(pInfo->siName, ItemInfo.siName);

	if (ItemInfo.siName) {
		NewFile = new GlobalFile(ItemInfo.siName, CFIO_GF_CREATE | CFIO_GF_READ
				| ItemInfo.siFlag);
		*pFile = NewFile;

		// проверяем, что смогли открыть
		if (!NewFile->GetHandle()) {
			return 0;
		}
	}

	while (FileSize) {
		FileRealBuffer = FileSize > COPYBUFFER ? COPYBUFFER : FileSize;

		ReadItemFromStorage(Storage, CopyBuffer, FileRealBuffer);
		ReadedDataCount += NewFile->Write(CopyBuffer, sizeof(char),
				FileRealBuffer);
		//NewFile->Flush();   // flushed when write

		FileSize -= FileRealBuffer;
	}

	return ReadedDataCount;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
char StorageName[_MAX_PATH];
char StorageFolder[_MAX_PATH + sizeof(uint32_t)];
uint32_t * FolderSize = (uint32_t *) StorageFolder;
uint32_t StorageFlag;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Handle Control::CompliteStorage(Handle Storage, uint32_t Flag) {
	FileHeader * pItemHeader = NULL;
	StorageHeader * pStorageHeader = storage_list_.GetItemHeader(Storage);
	GlobalFile * pStorage;
	uint32_t ComplitedItems = 0;
	uint32_t ComplitedSpace = 0;
	Handle ReStorage;
	// удаляем старое хранилище и создаем новое  fwrite
	// хотя надо бы просто очистить существующий файл
	if (pStorageHeader) {
		pStorage = pStorageHeader->GetStorage();
		CFIO_STRCPY(StorageName, pStorage->GetFileName().c_str());

		// папочка для файликов...
		// FIXME!!! no buffer length check
		CFIO_STRCPY(&StorageFolder[sizeof(*FolderSize)],
				pStorageHeader->GetStorageFolder().c_str());
		*FolderSize = strlen(&StorageFolder[sizeof(*FolderSize)])
				+ sizeof(*FolderSize);
		StorageFlag = pStorageHeader->GetFlag();

		// удаляем
		pStorage->SetDelete();
		storage_list_.DeleteItem(Storage);
		//delete pStorage; // deleted at header destructor
		// создаем новое с тем же именем
		// GlobalFile
		pStorage = new GlobalFile(&StorageName[0], CFIO_GF_CREATE);
		// Handle
		ReStorage = storage_list_.AddItem(pStorage, StorageFlag);
		// Header
		pStorageHeader = storage_list_.GetItemHeader(ReStorage);
	} else {
		return 0;
	}
	ComplitedSpace += WriteItemToStorage(pStorageHeader,
			(void *) StorageFolder, *FolderSize);

	// собираем хранилище
	while (NULL != (pItemHeader = file_list_.GetItemHeader(
			file_list_.GetAttachedFileHeader(Storage,
					(pItemHeader == NULL ? NULL : pItemHeader->GetNext()))))) {
		if (pItemHeader->CanWrite() && (Flag | CS_FILE_DELETE || Flag
				| CS_FILE_SAVE)) {
			pItemHeader->LockToStorage();
		}
		//  отписываем файл
		ComplitedSpace += WriteFileToStorage(pStorageHeader, pItemHeader);
		ComplitedItems++;

	}
	// вобщем бесполезная операция
	pStorageHeader->AddFlag(CFIO_GF_COMPLITE);

	return ReStorage;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
uint32_t Control::DecompileStorage(Handle Storage) {
	GlobalFile * pExtractFile;
	StorageHeader * StorageHeader = storage_list_.GetItemHeader(Storage);
	GlobalFile * pStorage = StorageHeader->GetStorage();
	STORAGEITEM ExtractInfo;
	uint32_t StorageSize = 0;
	uint32_t ReadedFromStorage = 0;
	uint32_t ItemSize;
	uint32_t NextItem = 0;
	uint32_t ReadItem;
	Handle hExtractFile;

	StorageSize = pStorage->Seek(0, CFIO_GF_SEEK_END);

	// читаем папочку для файликов
	pStorage->Seek(NextItem, CFIO_GF_SEEK_BEG);

	ReadItemFromStorage(StorageHeader, (void *) StorageFolder,
			sizeof(*FolderSize));
	NextItem += *FolderSize;
	StorageSize -= NextItem;
	ReadItemFromStorage(StorageHeader,
			(void *) &StorageFolder[sizeof(*FolderSize)], *FolderSize
					- sizeof(*FolderSize));
	StorageFolder[*FolderSize] = 0x0;
	// заносим в хидер
	// не заносим - будет во временной
	//STRCPY(StorageHeader->GetStorageFolder(),&StorageFolder[sizeof(*FolderSize)]);
	// и все остальное
	while (StorageSize) {
		pStorage->Seek(NextItem, CFIO_GF_SEEK_BEG);
		ReadItem = ReadFileFromStorage(StorageHeader, &ExtractInfo,
				&pExtractFile);
		ItemSize = ExtractInfo.siHeaderSize + ExtractInfo.siItemSize;

		StorageSize -= ItemSize;
		NextItem += ItemSize;

		if (ItemSize == ReadItem) {
			ReadedFromStorage += ItemSize;
			hExtractFile = AddFileInList(pExtractFile, ExtractInfo.siFlag,
					Storage);
			file_list_.GetItemHeader(hExtractFile)->AttachToStorage(Storage);
		} else {
			// close file if not successful
			delete pExtractFile;
		}
	}

	return ReadedFromStorage;
}

Handle Control::OpenCompliteStorage(const std::string& Name, uint Flag) {
	GlobalFile * pNewStorage = NULL;
	StorageHeader * hStorageHeader;
	Handle hNewStorage;

	pNewStorage = new GlobalFile(Name, CFIO_GF_WRITE | CFIO_GF_READ
			| CFIO_GF_BINARY);
	hNewStorage = AddStorageInList(pNewStorage, Flag);

	if (hNewStorage) {
		hStorageHeader = storage_list_.GetItemHeader(hNewStorage);
		DecompileStorage(hNewStorage);
		return hNewStorage;
	} else {
		return NULL;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Handle Control::OpenNewStorage(const std::string&, uint) {
	/* JussiP: This function is never called from within PUMA or TIGER. I disabled it
	 * because it uses Win32 api.

	 GlobalFile * pNewStorage = NULL;
	 Handle          hOpened;

	 CFIO_MAKEFULLPATH(szBuffer, lpName,_MAX_PATH);

	 if ( hOpened = StorageList.FindStorage(szBuffer) )
	 return hOpened;

	 pNewStorage = new GlobalFile(szBuffer, CFIO_GF_CREATE|CFIO_GF_READ|CFIO_GF_BINARY);

	 if ( pNewStorage )
	 {
	 return AddStorageInList(pNewStorage, Flag );
	 }

	 return NULL;
	 */
	return NULL;
}

// Закрываем хранилище (если сборка не проводилась), иначе только файлы
bool Control::CloseStorageFile(Handle Storage, uint32_t Flag) {
	StorageHeader * pStorageHeader = storage_list_.GetItemHeader(Storage);
	GlobalFile * pStorage;
	GlobalFile * pFile;
	FileHeader * pFileHeader = NULL;
	char StorageFolder[_MAX_PATH];

	if (Flag & CS_FILE_DELETE) {
		if (pStorageHeader->GetStorageFolder().length() < _MAX_PATH) {
			// FIXME!!! no buffer length check
			CFIO_STRCPY(StorageFolder, pStorageHeader->GetStorageFolder().c_str());
		}
	} else {
		StorageFolder[0] = 0x0;
	}

	// закрываем хранилище
	if (pStorageHeader) {
		pStorage = pStorageHeader->GetStorage();

		if (Flag & CS_DELETE)
			pStorage->SetDelete();

		storage_list_.DeleteItem(Storage);
		//delete pStorage; deleted at destructor
	}
	// закрываем файлы прикрепленные к хранилищу
	while (NULL != (pFileHeader = file_list_.GetItemHeader(
			file_list_.GetAttachedFileHeader(Storage/*, pFileHeader*/)))) {
		pFile = pFileHeader->GetFile();

		if (Flag & CS_FILE_DELETE) {
			pFile->SetDelete();
		}

		file_list_.DeleteItem(pFileHeader->GetHandle());
		//deleted at header destructor

	}

	if (StorageFolder[0] != 0x0)
		CFIO_DELETEFOLDER(StorageFolder);

	return false;
}

uint32_t Control::CompliteAllStorage(Handle Storage, uint32_t Flag) {
	// если не указано хранилище - собираем все
	if (Storage == NULL) {
		while (!storage_list_.IsEmpty()) {
			CompliteStorage(storage_list_.GetFirstItemHeader()->GetHandle(),
					Flag);
		}

		return storage_list_.IsEmpty();
	}
	// если явно указано хранилище - собираем только его
	else {
		return (CompliteStorage(Storage, Flag) != NULL);
	}

}

bool Control::CloseAllStorageFile(Handle Storage, uint32_t Flag) {
	// если не указано хранилище - закрываем все
	if (Storage == NULL) {
		while (!storage_list_.IsEmpty()) {
			CloseStorageFile(storage_list_.GetFirstItemHeader()->GetHandle(),
					Flag);
		}

		return storage_list_.IsEmpty();
	}
	// если явно указано хранилище - закрываем только его
	else {
		return CloseStorageFile(Storage, Flag);
	}
}
