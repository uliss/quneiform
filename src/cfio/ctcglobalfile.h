/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#ifndef CTCGLOBALFILE_H_
#define CTCGLOBALFILE_H_

#include <cstddef>
#include <cstdio>

#include <string>

#include "cttypes.h"

namespace CIF
{
namespace CTC
{

#define                CFIO_ALLOC(a,b)                  GlobalAlloc(a,b)
#define                CFIO_REALLOC(a,b,c)              GlobalRealloc(a,b,c)
#define                CFIO_FREE(a)                     GlobalFree(a)
#define                CFIO_LOCK(a)                     GlobalLock(a)
#define                CFIO_UNLOCK(a)                   GlobalUnlock(a)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base operations with files and storages as files
// storageheader and fileheader translate HandleFILE to owns Handle
//     global flags
#define                CFIO_GF_CREATE              0x00000001
#define                CFIO_GF_OPEN                0x00000002
#define                CFIO_GF_WRITE               0x00000004
#define                CFIO_GF_READ                0x00000008
#define                CFIO_GF_APPEND              0x00000010
#define                CFIO_GF_BINARY              0x00000020
#define                CFIO_GF_COMMIT              0x00000040
#define                CFIO_GF_FILE                0x00000080
#define                CFIO_GF_STORAGE             0x00000100
#define                CFIO_GF_IN_MEMORY           0x00000200
#define                CFIO_GF_TEMPORARY           0x00000400
#define                CFIO_GF_LOCKED              0x00000800
#define                CFIO_GF_SEEK_CURR           0x00001000
#define                CFIO_GF_SEEK_BEG            0x00002000
#define                CFIO_GF_SEEK_END            0x00004000
#define                CFIO_GF_MEMORY_FILE_CLUSTER 0x00008000
#define                CFIO_GF_COMPLITE            0x00010000
//     local funcs

#define                CFIO_OPEN(a,b)                   fopen(a,b)
#define                CFIO_CLOSE(a)                    fclose((FILE *)a)
#define                CFIO_WRITE(a,b,c,d)              fwrite((void *)a,b,c,(FILE *)d)
#define                CFIO_READ(a,b,c,d)               fread((void *)a,b,c,(FILE *)d)
#define                CFIO_SEEK(a,b,c)                 fseek((FILE *)a,b,c)
#define                CFIO_TELL(a)                     ftell((FILE *)a)
#define                CFIO_FLUSH(a)                    fflush((FILE *)a)
#define                CFIO_REMOVE(a)                   remove(a)
#define                CFIO_STRCAT(a,b)                 strcat(a,b)
#define                CFIO_STRCPY(a,b)                 strcpy(a,b)
#define                CFIO_STRLEN(a)                   strlen(a)
#define                CFIO_GETFOLDERSITEMS             split_path
#define                CFIO_MAKEPATH                    make_path
#define                CFIO_MAKEFULLPATH                _fullpath
#define                CFIO_MAKEFOLDER(a)               CreateDirectory(a,NULL)      //_mkdir(a) //
#define                CFIO_DELETEFOLDER                RemoveDirectory         //_rmdir          //
//                                                 CreateFolder
//     local flags
#define                CFIO_GF_CWRITE              "w"
#define                CFIO_GF_CREAD               "r"
#define                CFIO_GF_CAPPEND             "a"
#define                CFIO_GF_CWRITEREAD          "w+"
#define                CFIO_GF_CREADWRITE          "r+"
#define                CFIO_GF_CAPPENDEOF          "a+"
#define                CFIO_GF_CTEXT               "t"
#define                CFIO_GF_CBINARY             "b"
#define                CFIO_GF_CCOMMIT             "c"
#define                CFIO_GF_CNOCOMMIT           "n"

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

class GlobalFile
{
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

#endif /* CTCGLOBALFILE_H_ */
