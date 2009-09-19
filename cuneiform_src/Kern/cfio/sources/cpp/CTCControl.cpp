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
#include "ctccontrol.h"
#include "compat_defs.h"
void SetReturnCode_cfio(uint16_t rc);
//////////////////////////////////////////////////////////////////////////////////
//
CTCControl::CTCControl() {
	char SystemTemp[_MAX_PATH];

	GetTempPath(CFIO_MAX_PATH, SystemTemp);
	SetFolder(CFIO_TEMP_FOLDER, SystemTemp);
	SetFolder(CFIO_FILE_FOLDER, SystemTemp);
	CFIO_STRCAT(SystemTemp, "STORAGE\\");
	SetFolder(CFIO_STORAGE_FOLDER, SystemTemp);
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCControl::~CTCControl() {}
//////////////////////////////////////////////////////////////////////////////////
//
static char SFolder[CFIO_MAX_PATH];
static char SFile[CFIO_MAX_PATH];
static char SExtension[CFIO_MAX_PATH];
static char SOut[CFIO_MAX_PATH];
//////////////////////////////////////////////////////////////////////////////////
//
char* CTCControl::FileNameToFolder(char* Buffer, char* FolderName,
		char* FileName, uint32_t Size) {
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
			for (i = Buffer, j = FolderName; i < &Buffer[Shift]; *(i + Shift) = *(i), *(i++) = *(j++));

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
char* CTCControl::MakeNameForStorage(char* FileName,
		CTCStorageHeader * hStorageHead, Bool32 KeepFileName) {
	char* i;
	//char* j;
	//uint32_t StorageFolderNameSize = 0;
	//uint32_t FolderNameSize = 0;
	//uint32_t FileNameSize = 0;
	//uint32_t Append;

	if (!FileName)
		return NULL;

	if (strlen(FileName) > CFIO_MAX_PATH)
		return FileName;

	// копируем папку хранилища
	if (hStorageHead) {
		CFIO_STRCPY(szBuffer, hStorageHead->GetStorageFolder());
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

	CFIO_GETFOLDERSITEMS(FileName, SFolder, SFile, SExtension);

	//  для хранилища оставляем только имя файла с расширением

	CFIO_MAKEPATH(SOut, NULL, SFile, SExtension);

	return SOut;

}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::GetFolder(uint32_t wFolder, char* pcBuff) {
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
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::SetFolder(uint32_t wFolder, char* pcBuff) {
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
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::OpenStorage(char* lpName, uint32_t wFlag) {
	Handle OpenedStorage = NULL;
	if (wFlag & OS_OPEN) {
		OpenedStorage = OpenCompliteStorage(lpName, wFlag);
	} else {
		wFlag |= OS_CREATE;
		OpenedStorage = OpenNewStorage(lpName, wFlag);
	}

	return OpenedStorage;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::CloseStorage(Handle hStorage, uint32_t wFlag) {
	if (wFlag & CS_WITHOUT_SAVE || wFlag & CS_DELETE) {
		if (wFlag & CS_ALL)
			return CloseAllStorageFile(hStorage, wFlag);
		else
			return CloseStorageFile(hStorage, wFlag);
	}

	if (wFlag & CS_ALL) {
		CompliteAllStorage(hStorage, wFlag);
		return CloseAllStorageFile(hStorage, wFlag);
	} else {
		return CloseStorageFile(CompliteStorage(hStorage, wFlag));
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::DeleteStorage(char* lpName) {
	return (unlink(lpName) == 0);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::WriteFileToStorage(Handle hStorage, Handle hFile,
		char* lpName) {
	char FileName[CFIO_MAX_PATH];
	// берем хидер хрангилища.... или не берем, если нет
	CTCStorageHeader * pStorageHeader = StorageList.GetItemHeader(hStorage);
	// берем хидер файла ........ или не берем, ежели нема
	CTCFileHeader * pFileHeader = FileList.GetItemHeader(hFile);
	CTCGlobalFile * pFile;

	if (pStorageHeader && pFileHeader) {
		if (CFIO_STRLEN(lpName) < CFIO_MAX_PATH) {
			CFIO_STRCPY(FileName, lpName);
			AttachFileToStorage(hFile, hStorage, 0);
			pFile = pFileHeader->GetFile();
			pFile->SetFileName(FileName);
			pFileHeader->KeepName();

			return TRUE;
		}
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::ReadFileFromStorage(Handle hStorage, char* lpName) {
	char FileName[CFIO_MAX_PATH];
	// берем хидер хрангилища.... или не берем, если нет
	CTCStorageHeader * pStorageHeader = StorageList.GetItemHeader(hStorage);

	if (pStorageHeader) {
		if (FileNameToFolder(FileName, pStorageHeader->GetStorageFolder(),
				lpName, CFIO_MAX_PATH)) {
			return OpenFile(NULL, FileName, OSF_READ | OSF_WRITE | OSF_BINARY);
		}
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::OpenFile(Handle hStorage, char* lpName, uint32_t wFlag) {
	uint32_t FileFlag = 0x0;

	if (wFlag & OSF_CREATE)
		FileFlag |= CFIO_FILE_CREATE;

	if (wFlag & OSF_OPEN)
		FileFlag |= CFIO_FILE_OPEN;

	if (wFlag & OSF_READ)
		FileFlag |= CFIO_FILE_READ;

	if (wFlag & OSF_WRITE)
		FileFlag |= CFIO_FILE_WRITE;

	if (wFlag & OSF_BINARY)
		FileFlag |= CFIO_FILE_BINARY;

	if (wFlag & OSF_IN_MEMORY)
		FileFlag |= CFIO_FILE_IN_MEMORY;

	if (wFlag & OSF_TEMPORARY)
		FileFlag |= CFIO_FILE_TEMPORARY;

	return OpenFileAndAttach(lpName, FileFlag, hStorage);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::CloseFile(Handle hFile, uint32_t wFlag, Handle hStorage) {

	if (wFlag & CSF_SAVEDISK) {
		return CloseFileAndDettach(hFile, wFlag, hStorage);
	}
	if (wFlag & CSF_SAVESTORAGE) {
		return CloseFileAndAttach(hFile, wFlag, hStorage);
	}
	if (wFlag & CSF_DELETE) {
		return DeleteFileFromDisk(hFile);
	}
	if (wFlag & CSF_WRITE) {
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::WriteFile(Handle hFile, pchar lpData, uint32_t wSize) {
	return WriteDataToFile(hFile, (void *) lpData, wSize);
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::ReadFromFile(Handle hFile, pchar lpData, uint32_t wSize) {
	return ReadDataFromFile(hFile, (void *) lpData, wSize);
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::Seek(Handle hFile, uint32_t wBytes, uint32_t wFrom) {
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
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::Tell(Handle hFile) {
	return TellFilePointer(hFile);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::Flush(Handle hFile) {
	return FlushFile(hFile);
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::Alloc(uint32_t wSize, uint32_t wFlag, const char *cOwner,
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
Handle CTCControl::ReAlloc(Handle hMemory, uint32_t wNewSize, uint32_t wFlag) {
	Handle hNewMemory;
	uint32_t wOldSize = 0;
	uint32_t wOldFlag = 0;
	uint32_t GlobalFlag = 0;
	void * Sorc;
	void * Desc;

	switch (wFlag) {
	case MRF_NEW_MEMORY:

		MemoryList.TakeItem(hMemory, &wOldSize, &wOldFlag);
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
			CTCMemoryHeader * Memory = MemoryList.GetItem(hMemory);

			if (Memory) {
				hNewMemory = GlobalReAlloc(hMemory, wNewSize, GlobalFlag);
				Memory->SetHandle(hNewMemory);
				Memory->SetSize(wNewSize);
			}
		}
	}

	return hNewMemory;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::Free(Handle hMem) {
	return FreeMemory(hMem);
}
//////////////////////////////////////////////////////////////////////////////////
//
pvoid CTCControl::Lock(Handle hMem) {
	return LockMemory(hMem);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::Unlock(Handle hMem) {
	return UnlockMemory(hMem);
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::WriteMemToFile(Handle hMem, char* lpName) {
	Handle hFile = OpenFile(NULL, lpName, OSF_WRITE);
	uint32_t wMemorySize;
	uint32_t wMemoryFlag;
	uint32_t Counter = 0;
	pchar pMem;

	if (hFile && hMem) {
		MemoryList.TakeItem(hMem, &wMemorySize, &wMemoryFlag);
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
uint32_t CTCControl::ReadMemFromFile(char* lpName, Handle * phMem,
		uint32_t wFlag) {
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
uint32_t CTCControl::WriteMemToStorage(Handle hMem, Handle hStorage,
		char* lpName) {
	uint32_t wData;
	Handle hFile;

	if (wData = WriteMemToFile(hMem, lpName)) {
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
uint32_t CTCControl::ReadMemFromStorage(Handle hStorage, char* lpName,
		Handle * phMem) {
	char NameForStorage[CFIO_MAX_PATH];
	Handle hMem;
	uint32_t Readed = 0;

	*phMem = NULL;
	CTCStorageHeader * hStorageHead = StorageList.GetItemHeader(hStorage);

	if (hStorageHead) {
		FileNameToFolder(NameForStorage, hStorageHead->GetStorageFolder(),
				lpName, _MAX_PATH);

		Readed = ReadMemFromFile(NameForStorage, &hMem);

		if (Readed)
			*phMem = hMem;
	}

	return Readed;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::AllocNewMemory(uint32_t wFlag, uint32_t wSize,
		Bool32 bGlobal, const char *cOwner, const char *Coment) {
	Handle hNewMemory = NULL;

	if (bGlobal) {
		// alloc new memory;
		hNewMemory = CFIO_ALLOC(wFlag, wSize);
	} else {
		hNewMemory = (Handle) ::new char[wSize];
	}

	if (hNewMemory) {
		//add to list
		if (AddNewMemoryInList(hNewMemory, wSize, bGlobal, cOwner, Coment))
			return hNewMemory;
		else
			//or free back
			GlobalFree(hNewMemory);
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::AddNewMemoryInList(Handle hMemory, uint32_t wSize,
		uint32_t IsGlobal, const char *cOwner, const char *Coment) {
	return MemoryList.AddItem(hMemory, wSize, IsGlobal, cOwner, Coment);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::TakeMemory(Handle hMemory, uint32_t * wMemorySize,
		uint32_t * wMemoryFlag) {
	return MemoryList.TakeItem(hMemory, wMemorySize, wMemoryFlag);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::GetMemory(Handle hMemory, PPCTCMemoryHeader pHeader) {
	*pHeader = MemoryList.GetItem(hMemory);
	return pHeader != NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::DeleteMemoryFromList(Handle hMemory) {
	return MemoryList.DeleteItem(hMemory);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::LockatorMemoryInList(Handle hMemory, Bool32 bLock) {
	return MemoryList.LockUnlockItem(hMemory, bLock);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::FreeMemory(Handle hMemory, uint32_t wFlag) {
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
//////////////////////////////////////////////////////////////////////////////////
//
pvoid CTCControl::LockMemory(Handle hMemory) {
	uint32_t wMemoryStatus;
	uint32_t wMemorySize;
	pvoid pMemory = NULL;
	// ну , тут осталось немного. отписывать адрес в память,
	// но пока тут падает, а так как глобальная память не используется, то и
	// оставим это на потом
	//PCTCMemoryHeader pMemoryHeader;

	if (TakeMemory(hMemory, &wMemorySize, &wMemoryStatus)) {
		//if ( GetMemory(hMemory, &pMemoryHeader) )
		//{
		//	wMemorySize = pMemoryHeader->GetSize();
		//	wMemoryStatus = pMemoryHeader->GetFlag();

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
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::UnlockMemory(Handle hMemory) {
	uint32_t wMemoryStatus;
	uint32_t wMemorySize;
	Bool32 bUnlock = FALSE;

	if (TakeMemory(hMemory, &wMemorySize, &wMemoryStatus)) {
		if (wMemorySize && (wMemoryStatus & CFIO_MEMORY_GLOBAL)) {
			// if memory can be unlocked
			if (LockatorMemoryInList(hMemory, FALSE))
#ifdef CFIO_USE_GLOBAL_MEMORY
				bUnlock = CFIO_UNLOCK(hMemory);
#else
				bUnlock = TRUE;
#endif
		}
	}
	return bUnlock;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::OpenFileAndAttach(char* lpName, uint32_t Flag,
		Handle Storage) {
	CTCGlobalFile * pNewFile = NULL;
	Handle hOpened;

	// пока не используем
	// см так же DecompliteStorage
	//MAKEFULLPATH(szBuffer, lpName, _MAX_PATH);

	if (CFIO_STRLEN(lpName) < _MAX_PATH)
		CFIO_STRCPY(szBuffer, lpName);
	else
		return NULL;

	if (hOpened = FileList.FindFile(szBuffer)) {
		szBuffer[0] = 0x0;
		return hOpened;
	}

	pNewFile = new CTCGlobalFile(szBuffer, Flag);

	if (pNewFile) {
		szBuffer[0] = 0x0;
		return AddFileInList(pNewFile, Flag, Storage);
	}

	szBuffer[0] = 0x0;
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::AddFileInList(CTCGlobalFile * File, uint32_t Flag,
		Handle Storage) {
	Handle ret = FileList.AddItem(File, Flag, Storage);
	if (!ret)
		SetReturnCode_cfio(IDS_CFIO_ERR_CANT_OPEN_FILE);
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::DeleteFileFromList(Handle File, uint32_t Flag,
		Handle Stotrage) {
	return FileList.DeleteItem(File, Flag);
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::AddStorageInList(CTCGlobalFile * lpNewStorageName,
		uint32_t wNewFlag) {
	return StorageList.AddItem(lpNewStorageName, wNewFlag);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::CloseFileAndDettach(Handle File, uint32_t Flag,
		Handle Storage) {
	CTCFileHeader * CurrentFileHeader = FileList.GetItemHeader(File);
	CTCGlobalFile * CurrentFile;

	if (CurrentFileHeader) {
		CurrentFileHeader->DetachFromStorage();
		CurrentFile = CurrentFileHeader->GetFile();
		FileList.DeleteItem(File);

		if (CurrentFile) {
			//delete CurrentFile;        //AK   clear mistakes
			return TRUE;
		}
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::CloseFileAndAttach(Handle File, uint32_t Flag,
		Handle Storage) {
	CTCFileHeader * CurrentFileHeader = FileList.GetItemHeader(File);

	if (CurrentFileHeader) {
		if (CurrentFileHeader->AttachToStorage(Storage))
			return CurrentFileHeader->LockToStorage();
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::AttachFileToStorage(Handle File, Handle Storage,
		uint32_t Flag) {
	CTCFileHeader * AttachedFile = FileList.GetItemHeader(File);
	CTCStorageHeader * AttacherStorage = StorageList.GetItemHeader(Storage);

	if (AttacherStorage && AttachedFile) {
		return AttachedFile->AttachToStorage(Storage);
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::WriteDataToFile(Handle File, void * lpData, uint32_t Size) {
	CTCFileHeader * CurrentFileHeader = FileList.GetItemHeader(File);
	CTCGlobalFile * CurrentFile;
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
uint32_t CTCControl::ReadDataFromFile(Handle File, void * lpData, uint32_t Size) {
	CTCGlobalFile * CurrentFile = FileList.GetItem(File);
	uint32_t ReadedDataCount = 0;

	if (CurrentFile) {
		ReadedDataCount = CurrentFile->Read(lpData, sizeof(char), Size);
	}

	return ReadedDataCount;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::SeekFilePointer(Handle File, uint32_t Position,
		uint32_t From) {
	CTCGlobalFile * CurrentFile = FileList.GetItem(File);
	uint32_t Seeker = 0;

	if (CurrentFile) {
		Seeker = CurrentFile->Seek(Position, From);
	}

	return Seeker;

}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::TellFilePointer(Handle File) {
	CTCGlobalFile * CurrentFile = FileList.GetItem(File);
	uint32_t Seeker = 0;

	if (CurrentFile) {
		Seeker = CurrentFile->Tell();
	}

	return Seeker;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::FlushFile(Handle File) {
	CTCGlobalFile * CurrentFile = FileList.GetItem(File);

	if (CurrentFile) {
		return CurrentFile->Flush();
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::DeleteFileFromDisk(Handle File) {
	CTCGlobalFile * CurrentFile = FileList.GetItem(File);

	if (CurrentFile) {
		CurrentFile->SetDelete();

		if (FileList.DeleteItem(File)) {
			delete CurrentFile;
			return TRUE;
		}
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::WriteItemToStorage(CTCStorageHeader * Storage,
		void * pItem, uint32_t wSize) {
	CTCGlobalFile * CurrentStorage = Storage->GetStorage();
	uint32_t WritedDataCount = 0;

	if (CurrentStorage) {
		WritedDataCount = CurrentStorage->Write(pItem, sizeof(char), wSize);
	}
	return WritedDataCount;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::ReadItemFromStorage(CTCStorageHeader * Storage,
		void * lpData, uint32_t wSize) {
	CTCGlobalFile * CurrentStorage = Storage->GetStorage();
	uint32_t ReadedDataCount = 0;

	if (CurrentStorage) {
		ReadedDataCount = CurrentStorage->Read(lpData, sizeof(char), wSize);
	}
	return ReadedDataCount;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define          COPYBUFFER                     512
char CopyBuffer[COPYBUFFER];
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::WriteFileToStorage(CTCStorageHeader * Storage,
		CTCFileHeader * File) {
	CTCFileHeader * pItemHeader = File;
	CTCGlobalFile * pItem;
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
				Storage, pItemHeader->HowName()));

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
uint32_t CTCControl::ReadFileFromStorage(CTCStorageHeader * Storage,
		STORAGEITEM * pInfo, CTCGlobalFile ** pFile) {
	STORAGEITEM ItemInfo;
	CTCGlobalFile * NewFile;
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
	FileNameToFolder(ItemInfo.siName, Storage->GetStorageFolder(),
			ItemInfo.siName, _MAX_PATH);
	CFIO_STRCPY(pInfo->siName, ItemInfo.siName);

	if (ItemInfo.siName) {
		NewFile = new CTCGlobalFile(ItemInfo.siName, CFIO_GF_CREATE
				| CFIO_GF_READ | ItemInfo.siFlag);
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
Handle CTCControl::CompliteStorage(Handle Storage, uint32_t Flag) {
	CTCFileHeader * pItemHeader = NULL;
	CTCStorageHeader * pStorageHeader = StorageList.GetItemHeader(Storage);
	CTCGlobalFile * pStorage;
	uint32_t ComplitedItems = 0;
	uint32_t ComplitedSpace = 0;
	Handle ReStorage;
	// удаляем старое хранилище и создаем новое  fwrite
	// хотя надо бы просто очистить существующий файл
	if (pStorageHeader) {
		pStorage = pStorageHeader->GetStorage();
		CFIO_STRCPY(StorageName, pStorage->GetFileName());

		// папочка для файликов...
		CFIO_STRCPY(&StorageFolder[sizeof(*FolderSize)],
				pStorageHeader->GetStorageFolder());
		*FolderSize = strlen(&StorageFolder[sizeof(*FolderSize)])
				+ sizeof(*FolderSize);
		StorageFlag = pStorageHeader->GetFlag();

		// удаляем
		pStorage->SetDelete();
		StorageList.DeleteItem(Storage);
		//delete pStorage; // deleted at header destructor
		// создаем новое с тем же именем
		// GlobalFile
		pStorage = new CTCGlobalFile(&StorageName[0], CFIO_GF_CREATE);
		// Handle
		ReStorage = StorageList.AddItem(pStorage, StorageFlag);
		// Header
		pStorageHeader = StorageList.GetItemHeader(ReStorage);
	} else {
		return 0;
	}
	ComplitedSpace += WriteItemToStorage(pStorageHeader,
			(void *) StorageFolder, *FolderSize);

	// собираем хранилище
	while (pItemHeader = FileList.GetItemHeader(FileList.GetAttachedFileHeader(
			Storage, (pItemHeader == NULL ? NULL : pItemHeader->GetNext())))) {
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

	// закрываем вновь созданное хранилище
	// StorageList.DeleteItem(ReStorage);
	// delete pStorage; // deleted at header destructor

	return ReStorage;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::DecompileStorage(Handle Storage) {
	CTCGlobalFile * pExtractFile;
	CTCStorageHeader * StorageHeader = StorageList.GetItemHeader(Storage);
	CTCGlobalFile * pStorage = StorageHeader->GetStorage();
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
			FileList.GetItemHeader(hExtractFile)->AttachToStorage(Storage);
		} else {
			// close file if not successful
			delete pExtractFile;
		}
	}

	return ReadedFromStorage;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::OpenCompliteStorage(char* lpName, uint32_t Flag) {
	CTCGlobalFile * pNewStorage = NULL;
	CTCStorageHeader * hStorageHeader;
	Handle hNewStorage;

	pNewStorage = new CTCGlobalFile(lpName, CFIO_GF_WRITE | CFIO_GF_READ
			| CFIO_GF_BINARY);
	hNewStorage = AddStorageInList(pNewStorage, Flag);

	if (hNewStorage) {
		hStorageHeader = StorageList.GetItemHeader(hNewStorage);
		DecompileStorage(hNewStorage);
		return hNewStorage;
	} else {
		return NULL;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Handle CTCControl::OpenNewStorage(char* lpName, uint32_t Flag) {
	/* JussiP: This function is never called from within PUMA or TIGER. I disabled it
	 * because it uses Win32 api.

	 CTCGlobalFile * pNewStorage = NULL;
	 Handle          hOpened;

	 CFIO_MAKEFULLPATH(szBuffer, lpName,_MAX_PATH);

	 if ( hOpened = StorageList.FindStorage(szBuffer) )
	 return hOpened;

	 pNewStorage = new CTCGlobalFile(szBuffer, CFIO_GF_CREATE|CFIO_GF_READ|CFIO_GF_BINARY);

	 if ( pNewStorage )
	 {
	 return AddStorageInList(pNewStorage, Flag );
	 }

	 return NULL;
	 */
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Закрываем хранилище (если сборка не проводилась), иначе только файлы
Bool32 CTCControl::CloseStorageFile(Handle Storage, uint32_t Flag) {
	CTCStorageHeader * pStorageHeader = StorageList.GetItemHeader(Storage);
	CTCGlobalFile * pStorage;
	CTCGlobalFile * pFile;
	CTCFileHeader * pFileHeader = NULL;
	char StorageFolder[_MAX_PATH];

	if (Flag & CS_FILE_DELETE) {
		if (CFIO_STRLEN(pStorageHeader->GetStorageFolder()) < _MAX_PATH)
			CFIO_STRCPY(StorageFolder, pStorageHeader->GetStorageFolder());
	} else {
		StorageFolder[0] = 0x0;
	}

	// закрываем хранилище
	if (pStorageHeader) {
		pStorage = pStorageHeader->GetStorage();

		if (Flag & CS_DELETE)
			pStorage->SetDelete();

		StorageList.DeleteItem(Storage);
		//delete pStorage; deleted at destructor
	}
	// закрываем файлы прикрепленные к хранилищу
	while (pFileHeader = FileList.GetItemHeader(FileList.GetAttachedFileHeader(
			Storage/*, pFileHeader*/))) {
		pFile = pFileHeader->GetFile();

		if (Flag & CS_FILE_DELETE) {
			pFile->SetDelete();
		}

		FileList.DeleteItem(pFileHeader->GetHandle());
		//delete pFile; //deleted at header destructor

	}

	if (StorageFolder[0] != 0x0)
		CFIO_DELETEFOLDER(StorageFolder);

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCControl::CompliteAllStorage(Handle Storage, uint32_t Flag) {
	// если не указано хранилище - собираем все
	if (Storage == NULL) {
		while (!StorageList.IsEmpty()) {
			CompliteStorage(StorageList.GetFirstItemHeader()->GetHandle(), Flag);
		}

		return StorageList.IsEmpty();
	}
	// если явно указано хранилище - собираем только его
	else {
		return (CompliteStorage(Storage, Flag) != NULL);
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCControl::CloseAllStorageFile(Handle Storage, uint32_t Flag) {
	// если не указано хранилище - закрываем все
	if (Storage == NULL) {
		while (!StorageList.IsEmpty()) {
			CloseStorageFile(StorageList.GetFirstItemHeader()->GetHandle(),
					Flag);
		}

		return StorageList.IsEmpty();
	}
	// если явно указано хранилище - закрываем только его
	else {
		return CloseStorageFile(Storage, Flag);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//end of file
