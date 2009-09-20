/*
 * ctcglobalfile.cpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#include <cassert>
#include <cstring>
#include <cstdio>

#include "ctcglobalfile.h"
#include "compat_defs.h"
#include "ctccontrol.h"

extern CIF::CTC::CTCControl * Control_ctc;

namespace CIF {
namespace CTC {

GlobalFile::GlobalFile() {
	assert(false);
	SetHandle(NULL);
}

GlobalFile::GlobalFile(const std::string& Name, uint32_t Flag) {
	ProvideFileFolder(Name);
	TranslateFlagToString(Flag);
	if (Flag & CFIO_GF_IN_MEMORY) {
		Deleted = true;
		InMemory = true;
		MoveToFile = true;
		wSeeker = 0;
		wClusterCounter = 0;
		mcFirst.mcPtr = mcFirst.mcHandle = mcFirst.mcNext = NULL;
		mcFirst.mcSize = 0;
		SetHandle(CreateNewCluster());
	} else {
		SetHandle(CFIO_OPEN(Name.c_str(), GetFlagString()));
		Deleted = true;
		InMemory = true;
		MoveToFile = true;
	}

	if (GetHandle()) {
		SetFileName(Name);
		wSeeker = 0;
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
GlobalFile::~GlobalFile() {
	if (GetHandle())
		Close();

	if (IsDeleted())
		CFIO_REMOVE(file_name_.c_str());
}
//////////////////////////////////////////////////////////////////////////////////
//
bool GlobalFile::IsInString(const char * Flag) {
	for (char * i = GetFlagString(); *i != 0x00; i++) {
		if (*i == *Flag) {
			if (*(Flag + 1) == 0x00)
				return true;
			else if (*(Flag + 1) == *(i + 1))
				return true;
		}
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t GlobalFile::Write(void * pData, uint32_t wDataSize,
		uint32_t wDataCounter) {
	if (IsInString(CFIO_GF_CWRITE) || IsInString(CFIO_GF_CWRITEREAD)
			|| IsInString(CFIO_GF_CREADWRITE) || IsInString(CFIO_GF_CAPPEND)
			|| IsInString(CFIO_GF_CAPPENDEOF)) {
		if (!InMemory) {
			uint32_t Writed = CFIO_WRITE(pData, wDataSize, wDataCounter,
					GetHandle());
			Flush();
			return Writed;
		} else {
			return WriteToMemory(pData, wDataSize, wDataCounter);
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t GlobalFile::Read(void * pData, uint32_t wDataSize,
		uint32_t wDataCounter) {
	if (IsInString(CFIO_GF_CREAD) || IsInString(CFIO_GF_CWRITEREAD)
			|| IsInString(CFIO_GF_CREADWRITE) || IsInString(CFIO_GF_CAPPEND)
			|| IsInString(CFIO_GF_CAPPENDEOF)) {
		if (!InMemory) {
			return CFIO_READ(pData, wDataSize, wDataCounter, GetHandle());
		} else {
			return ReadFromMemory(pData, wDataSize, wDataCounter);
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//
bool GlobalFile::Close() {
	uint32_t Closet;

	if (!InMemory) {
		Closet = CFIO_CLOSE(GetHandle());
	} else if (!IsDeleted()) {
		FILE * DiskFile;
		char DiskFileName[CFIO_MAX_PATH];

		// extract memory file name for disk file
		// if present
		if (!GetFileName(DiskFileName).empty()) {
			if (IsInString(CFIO_GF_CBINARY))
				DiskFile = CFIO_OPEN(DiskFileName, "wb");
			else
				DiskFile = CFIO_OPEN(DiskFileName, "w");
			//move from memory to disk
			MoveFromMemory(DiskFile);

			CFIO_CLOSE(DiskFile);
		}
	} else {
		MoveFromMemory(NULL);
	}

	if (Closet == 0) {
		SetHandle(NULL);
		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////////////
//move file from memroy to disk and delete memory copy
bool GlobalFile::MoveFromMemory(Handle dFile) {
	uint32_t DataLeft = wMemorySize;
	uint32_t wWriteCount = 0;
	uint32_t ReadFromCluster;
	PCFIOMCLUSTER pCurrentCluster = mcFirst.mcNext;
	void * lpData;

	if (dFile) {
		while (DataLeft) {
			if (DataLeft > CFIO_GF_MEMORY_FILE_CLUSTER) {
				ReadFromCluster = CFIO_GF_MEMORY_FILE_CLUSTER;
			} else {
				ReadFromCluster = DataLeft;
			}
			// take pointer to memory
			lpData = GetPtrToMemoryCluster(NULL, pCurrentCluster);
			// write memory to disk
			wWriteCount += CFIO_WRITE(lpData, sizeof(char), ReadFromCluster,
					dFile);
			DataLeft -= ReadFromCluster;
			ClosePtrToMemoryCluster(NULL, pCurrentCluster);
			pCurrentCluster = pCurrentCluster->mcNext;
		}
	}

	KillLastCluster(&mcFirst);

	return true;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t GlobalFile::Seek(uint32_t Position, uint32_t Flag) {
	int Direction;
	uint32_t NewSeeker;

	switch (Flag) {
	case CFIO_GF_SEEK_CURR:
		Direction = SEEK_CUR;
		break;
	case CFIO_GF_SEEK_BEG:
		Direction = SEEK_SET;
		break;
	case CFIO_GF_SEEK_END:
		Direction = SEEK_END;
		break;
	default:
		Direction = SEEK_CUR;
		break;
	}

	if (InMemory) {
		switch (Direction) {
		case SEEK_CUR:
			wSeeker += Position;

			if (wSeeker > wMemorySize)
				wSeeker = wMemorySize;

			break;

		case SEEK_END:

			if (Position < wMemorySize)
				wSeeker = wMemorySize - Position;
			else
				wSeeker = 0;

			break;

		case SEEK_SET:

			if (Position < wMemorySize)
				wSeeker = Position;
			else
				wSeeker = wMemorySize;
			break;
		}
	} else {
		if (CFIO_SEEK(GetHandle(), Position, Direction) == 0) {
			NewSeeker = Tell();
			wSeeker = NewSeeker;
		}
	}
	return wSeeker;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t GlobalFile::Tell() const {
	if (InMemory)
		return wSeeker;
	else {
		return CFIO_TELL(const_cast<GlobalFile*> (this)->GetHandle());
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t GlobalFile::Flush() {
	if (InMemory) {
		return 0;
	} else {
		return CFIO_FLUSH(GetHandle());
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
size_t GlobalFile::GetFileSize() const {
	uint32_t CurrentPosition = Tell();
	uint32_t Size;

	GlobalFile * non_const_this = const_cast<GlobalFile*> (this);

	if (non_const_this->Seek(0, CFIO_GF_SEEK_END)) {
		Size = Tell();
		non_const_this->Seek(CurrentPosition, CFIO_GF_SEEK_BEG);
	}
	return Size;
}
//////////////////////////////////////////////////////////////////////////////////
//
void GlobalFile::TranslateFlagToString(uint32_t Flag) {
	ClearFlagString();

	if (Flag & CFIO_GF_CREATE) {
		if (Flag & CFIO_GF_READ)
			AddFlagToString(CFIO_GF_CWRITEREAD);
		else
			AddFlagToString(CFIO_GF_CWRITE);
	} else {
		if (Flag & CFIO_GF_READ) {
			if (Flag & CFIO_GF_WRITE)
				AddFlagToString(CFIO_GF_CREADWRITE);
			else {
				if (Flag & CFIO_GF_APPEND)
					AddFlagToString(CFIO_GF_CAPPENDEOF);
				else
					AddFlagToString(CFIO_GF_CREAD);
			}
		} else {
			if (Flag & CFIO_GF_WRITE)
				AddFlagToString(CFIO_GF_CWRITE);
			else {
				if (Flag & CFIO_GF_APPEND)
					AddFlagToString(CFIO_GF_CAPPEND);
				else
					AddFlagToString(CFIO_GF_CWRITEREAD);
			}
		}
	}

	if (Flag & CFIO_GF_BINARY)
		AddFlagToString(CFIO_GF_CBINARY);
	else
		AddFlagToString(CFIO_GF_CTEXT);

	if (Flag & CFIO_GF_COMMIT)
		AddFlagToString(CFIO_GF_CCOMMIT);
}
//////////////////////////////////////////////////////////////////////////////////
//
void GlobalFile::ClearFlagString(void) {
	CFIO_STRCPY(cFlag, "");
}
//////////////////////////////////////////////////////////////////////////////////
//
void GlobalFile::AddFlagToString(const char * Flag) {
	CFIO_STRCAT(cFlag, Flag);
}
//////////////////////////////////////////////////////////////////////////////////
//
std::string GlobalFile::GetFileName(char * lpName) const {
	if (lpName && CFIO_STRLEN(lpName) <= CFIO_MAX_PATH)
		CFIO_STRCPY(lpName, file_name_.c_str());

	return file_name_;
}

void GlobalFile::SetFileName(const std::string& Name) {
	file_name_ = Name;
}

uint32_t GlobalFile::GetFileLenght() {
	if (InMemory) {
		return wMemorySize;
	} else {
		uint32_t wFileLenght;
		uint32_t wCurrentPos = Tell();

		wFileLenght = Seek(0, CFIO_GF_SEEK_END);
		Seek(wCurrentPos, CFIO_GF_SEEK_BEG);
		return wFileLenght;
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
static char Folder[CFIO_MAX_PATH];
static char File[CFIO_MAX_PATH];
static char Extension[CFIO_MAX_PATH];
static char Out[CFIO_MAX_PATH];
//////////////////////////////////////////////////////////////////////////////////
//
bool GlobalFile::ProvideFileFolder(const std::string& FileName) {
	/*
	 * Given a filename such as "/foo/bar/baz/file.txt" creates the
	 * directory "/foo/bar/baz".
	 */
	if (FileName.empty())
		return false;

	CFIO_GETFOLDERSITEMS(FileName.c_str(), Folder, File, Extension);
	CFIO_MAKEPATH(Out, Folder, NULL, NULL);

	if (Out[0] == 0x0)
		return true;

	CFIO_MAKEFOLDER(Out);
	CFIO_MAKEPATH(Out, Folder, File, Extension);
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////memory file//////////////////////////////////////////////
//
Handle GlobalFile::CreateNewCluster(PPCFIOMCLUSTER pmcCluster) {
	PCFIOMCLUSTER pCurrentCluster, pNewCluster;
	// if not first
	if (mcFirst.mcNext) {
		GetLastCluster(&pCurrentCluster);
	} else {
		pCurrentCluster = &mcFirst;
	}

	pNewCluster = new CFIOMCLUSTER;

	pNewCluster->mcNext = NULL;
	pNewCluster->mcLocked = FALSE;
	pNewCluster->mcPtr = NULL;
	// new Cluster Size for alloc()
	pNewCluster->mcSize = CFIO_GF_MEMORY_FILE_CLUSTER;
	// new Cluster memory flag for alloc()
	pNewCluster->mcMemoryFlag = MAF_GALL_GHND;
	// alloc new memory for new cluster
	pNewCluster->mcHandle = Control_ctc->Alloc(pNewCluster->mcSize,
			pNewCluster->mcMemoryFlag, "CFIO virtual file cluster",
			"no comment");
	// new cluster is clear
	pNewCluster->mcFill = 0;
	pNewCluster->mcNumber = wClusterCounter;

	if (pmcCluster) {
		*pmcCluster = pNewCluster;
	}

	if (pNewCluster->mcHandle) {
		pCurrentCluster->mcNext = pNewCluster;
	}
	// increase cluster counter
	wClusterCounter++;
	return pNewCluster->mcHandle;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
bool GlobalFile::KillLastCluster(PCFIOMCLUSTER pEndCluster) {
	PCFIOMCLUSTER pCluster, pLastCluster, pNextToDelete;

	// if 1 or more cluster present
	if (mcFirst.mcNext) {
		// set last cluster in chain
		pLastCluster = &mcFirst;
		// define end of cluster chain -
		if (pEndCluster) {
			// reset last cluster
			pLastCluster = pEndCluster;
			// set first cluster for delete
			pCluster = pEndCluster->mcNext;
		}
		// or delete las 1 cluster
		else {
			for (pCluster = mcFirst.mcNext; pCluster->mcNext != NULL; pCluster
					= pCluster->mcNext) {
				pLastCluster = pCluster;
			}
		}
		// breah\k chain
		pLastCluster->mcNext = NULL;

		// delete clusters from end cluster
		while (pCluster) {
			pNextToDelete = pCluster->mcNext;
			// unlock memory (for any case)
			if (pCluster->mcLocked)
				Control_ctc->Unlock(pCluster->mcHandle);

			// free memory cluster and ...
			Control_ctc->Free(pCluster->mcHandle);
			// delete memory cluster info
			delete pCluster;
			// decrease cluster counter;
			wClusterCounter--;
			pCluster = pNextToDelete;
		}
		return true;
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
PCFIOMCLUSTER GlobalFile::TakeCluster(Handle hCluster) {
	PCFIOMCLUSTER pCurrentCluster;

	for (pCurrentCluster = mcFirst.mcNext; pCurrentCluster != NULL; pCurrentCluster
			= pCurrentCluster->mcNext) {
		pCurrentCluster->mcHandle = hCluster;
		if (hCluster)
			return pCurrentCluster;
	}
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
Handle GlobalFile::GetLastCluster(PPCFIOMCLUSTER pmcCluster) {
	PCFIOMCLUSTER pLastCluster;

	if (!mcFirst.mcNext) {
		return &mcFirst;
	}

	for (pLastCluster = mcFirst.mcNext; pLastCluster->mcNext != NULL; pLastCluster
			= pLastCluster->mcNext)
		;

	if (pmcCluster) {
		*pmcCluster = pLastCluster;
	}

	return pLastCluster->mcHandle;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
Handle GlobalFile::GetNextCluster(Handle hCluster, PPCFIOMCLUSTER pmcCluster) {
	PCFIOMCLUSTER pCluster = TakeCluster(hCluster);

	if (pCluster) {
		if (pmcCluster) {
			*pmcCluster = pCluster;
		}
		return pCluster->mcNext->mcHandle;
	}

	if (pmcCluster) {
		*pmcCluster = NULL;
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
void * GlobalFile::GetPtrToMemoryCluster(Handle hCluster,
		PCFIOMCLUSTER pCluster) {
	PCFIOMCLUSTER pCurrentCluster;

	if (pCluster) {
		pCurrentCluster = pCluster;
	} else {
		pCurrentCluster = TakeCluster(hCluster);
	}

	if (!pCurrentCluster->mcLocked) {
		pCurrentCluster->mcPtr = Control_ctc->Lock(pCurrentCluster->mcHandle);
		pCurrentCluster->mcLocked = TRUE;
	}

	return pCurrentCluster->mcPtr;

}
//////////////////////////////////////////////////////////////////////////////////
//
bool GlobalFile::ClosePtrToMemoryCluster(Handle hCluster,
		PCFIOMCLUSTER pCluster) {
	PCFIOMCLUSTER pCurrentCluster;

	if (pCluster) {
		pCurrentCluster = pCluster;
	} else {
		pCurrentCluster = TakeCluster(hCluster);
	}

	if (pCurrentCluster->mcLocked) {
		pCurrentCluster->mcPtr = NULL;
		Control_ctc->Unlock(pCurrentCluster->mcHandle);
		pCurrentCluster->mcLocked = FALSE;
	}

	return !pCurrentCluster->mcLocked;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle GlobalFile::GetSeekedCluster(PPCFIOMCLUSTER pmcCluster) {
	uint32_t FilledClusters = wSeeker / CFIO_GF_MEMORY_FILE_CLUSTER;
	PCFIOMCLUSTER pCluster;

	// if allright
	if (FilledClusters <= wClusterCounter) {
		// seeker to end
		if (wSeeker == wMemorySize) {
			GetLastCluster(&pCluster);
		} else
		//seeker to begin
		if (wSeeker == 0) {
			pCluster = mcFirst.mcNext;
		}
		// seeker to midle of file
		else {
			for (pCluster = mcFirst.mcNext; pCluster != NULL && FilledClusters
					> 0; pCluster = pCluster->mcNext, FilledClusters--)
				;
		}

	} else // if seeker out of range
	{
		wSeeker = 0;
		pCluster = mcFirst.mcNext;
	}

	if (pmcCluster) {
		*pmcCluster = pCluster;
	}

	if (pCluster)
		return pCluster->mcHandle;
	else
		return NULL;
}

uint32_t GlobalFile::WriteToMemory(void * pData, uint32_t wDataSize,
		uint32_t wDataCounter) {
	uint32_t ClusterOffset;
	uint32_t wAllData = wDataSize * wDataCounter;
	uint32_t AvailableToWriteInCluster;
	//uint32_t i;
	uint32_t Counter = 0;
	char * Sorc = (char *) pData;
	char * Desc;
	PCFIOMCLUSTER pWritedCluster;

	// position to seeker
	GetSeekedCluster(&pWritedCluster);
	// set offset in cluster
	ClusterOffset = wSeeker - ((wSeeker / CFIO_GF_MEMORY_FILE_CLUSTER)
			* CFIO_GF_MEMORY_FILE_CLUSTER);

	//writing:
	while (wAllData) {
		//  available to write in this cluster
		AvailableToWriteInCluster = CFIO_GF_MEMORY_FILE_CLUSTER - ClusterOffset;
		// set ptr to writing
		Desc = ((char *) GetPtrToMemoryCluster(NULL, pWritedCluster))
				+ ClusterOffset;

		//  if need to write less then free space in last cluster
		if (wAllData < AvailableToWriteInCluster) {
			// write all data in one cluster
			AvailableToWriteInCluster = wAllData;
		} else {
			// or create new cluster if last can't be writen by all data
			if (pWritedCluster->mcNext == NULL)
				CreateNewCluster();

			// clear offset for next cluster
			ClusterOffset = 0;
		}
		memcpy(Desc, Sorc, AvailableToWriteInCluster);
		Sorc += AvailableToWriteInCluster;
		wAllData -= AvailableToWriteInCluster;
		Counter += AvailableToWriteInCluster;
		wSeeker += AvailableToWriteInCluster;

		ClosePtrToMemoryCluster(NULL, pWritedCluster);

		// switch to write on next cluster
		if (wAllData) {
			pWritedCluster = pWritedCluster->mcNext;
		}
	}
	// new file in memory size
	wMemorySize = wSeeker;
	return Counter;
}

uint32_t GlobalFile::ReadFromMemory(void * pData, uint32_t wDataSize,
		uint32_t wDataCounter) {
	uint32_t ClusterOffset;
	uint32_t wAllData = wDataSize * wDataCounter;
	uint32_t AvailableToReadInCluster;
	//	uint32_t i;
	uint32_t Counter = 0;
	char * Sorc;
	char * Desc = (char *) pData;
	PCFIOMCLUSTER pReadedCluster;

	// position to seeker
	GetSeekedCluster(&pReadedCluster);
	// set offset in cluster
	ClusterOffset = wSeeker - ((wSeeker / CFIO_GF_MEMORY_FILE_CLUSTER)
			* CFIO_GF_MEMORY_FILE_CLUSTER);

	if (wAllData > wMemorySize)
		wAllData = wMemorySize;

	//writing:
	while (wAllData) {
		//  available to write in this cluster
		AvailableToReadInCluster = CFIO_GF_MEMORY_FILE_CLUSTER - ClusterOffset;
		// set ptr to writing
		Sorc = ((char *) GetPtrToMemoryCluster(NULL, pReadedCluster))
				+ ClusterOffset;

		//  if need to write less then free space in last cluster
		if (wAllData < AvailableToReadInCluster) {
			// write all data in one cluster
			AvailableToReadInCluster = wAllData;
		}
		//copy data
		memcpy(Desc, Sorc, AvailableToReadInCluster);
		// move destination pointer to write next data
		Desc += AvailableToReadInCluster;
		// decrease data to copy counter
		wAllData -= AvailableToReadInCluster;
		// increase data copy counter
		Counter += AvailableToReadInCluster;
		// move virtual file position pointer
		wSeeker += AvailableToReadInCluster;

		ClosePtrToMemoryCluster(NULL, pReadedCluster);

		// switch to write on next cluster
		if (wAllData) {
			pReadedCluster = pReadedCluster->mcNext;
			ClusterOffset = 0;

			if (!pReadedCluster) {
				return 0;
			}
		}
	}
	return Counter;
}
}
}
