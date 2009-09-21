/*
 * globalfile.h
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef CIF_CTC_GLOBALFILE_H_
#define CIF_CTC_GLOBALFILE_H_

#include <cstddef>
#include <cstdio>

#include <string>

#include "cttypes.h"
#include "ctc_def.h"

namespace CIF {
namespace CFIO {

struct CTC_mem_cluster {
	struct CTC_mem_cluster * mcNext;
	Handle mcHandle;
	void * mcPtr;
	uint32_t mcSize;
	uint32_t mcMemoryFlag;
	Bool32 mcLocked;
	uint32_t mcFill;
	uint32_t mcNumber;
};

typedef struct CTC_mem_cluster CFIOMCLUSTER, *PCFIOMCLUSTER, **PPCFIOMCLUSTER;
typedef Handle HandleFILE;

class GlobalFile {
private:
	HandleFILE hFile;
	std::string file_name_;
	uint32_t wSeeker;
	char cFlag[32];
	bool Deleted;
	bool InMemory;
	bool MoveToFile;
	uint32_t wClusterCounter;
	uint32_t wMemorySize;
	CFIOMCLUSTER mcFirst;

public:
	GlobalFile();
	GlobalFile(const std::string& Name, uint32_t Flag);
	~GlobalFile();

public:
	uint32_t Read(void * pData, uint32_t wDataSize, uint32_t wDataCounter);
	uint32_t Write(void * pData, uint32_t wDataSize, uint32_t wDataCounter);
	uint32_t Flush();
	uint32_t Tell() const;
	uint32_t Seek(uint32_t Position, uint32_t Flag);
	std::string GetFileName(char* lpName = NULL) const;
	uint32_t GetFileLenght();
	void SetFileName(const std::string& FileName);
	bool Close();

public:
	Handle GetFileHandle() {
		return ((Handle) hFile);
	}

	HandleFILE GetHandle() {
		return hFile;
	}

	char* GetFlagString() {
		return cFlag;
	}

	bool IsInString(const char* Flag);

	size_t GetFileSize() const;

	size_t GetHeaderSize() const {
		return (sizeof(class GlobalFile));
	}

	bool SetDelete() {
		return (Deleted = TRUE);
	}

	bool KeepOnDisk() {
		return !(Deleted = FALSE);
	}

	bool IsDeleted() const {
		return (Deleted);
	}

protected:
	Handle SetFileHandle(Handle NewFile) {
		return (hFile = static_cast<HandleFILE> (NewFile));
	}

	HandleFILE SetHandle(HandleFILE NewHandle) {
		return (hFile = NewHandle);
	}

	bool ProvideFileFolder(const std::string& FileFullName);

public:
	void TranslateFlagToString(uint32_t Flag);
	void ClearFlagString(void);
	void AddFlagToString(const char*Flag);

private:
	Handle CreateNewCluster(PPCFIOMCLUSTER pmcCluster = NULL);
	bool KillLastCluster(PCFIOMCLUSTER pCluster = NULL);
	PCFIOMCLUSTER TakeCluster(Handle hCluster);
	Handle GetFirstCluster() {
		return (mcFirst.mcNext)->mcHandle;
	}

	Handle GetNextCluster(Handle Cluster, PPCFIOMCLUSTER pmcCluster = NULL);
	Handle GetLastCluster(PPCFIOMCLUSTER pmcCluster = NULL);
	Handle GetSeekedCluster(PPCFIOMCLUSTER pmcCluster = NULL);
	void
			* GetPtrToMemoryCluster(Handle hCluster, PCFIOMCLUSTER pCluster =
					NULL);
	bool
			ClosePtrToMemoryCluster(Handle hCluster, PCFIOMCLUSTER pCluster =
					NULL);
	uint32_t WriteToMemory(void * pData, uint32_t wDataSwze,
			uint32_t wDataCounter);
	uint32_t ReadFromMemory(void * pData, uint32_t wDataSize,
			uint32_t wDataCounter);
	bool MoveFromMemory(Handle dFile);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
}
}

#endif /* CIF_CTC_GLOBALFILE_H_ */
