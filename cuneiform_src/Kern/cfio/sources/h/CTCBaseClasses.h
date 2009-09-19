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
#ifndef _CTC_BASE_CLASSES_
#define _CTC_BASE_CLASSES_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "cfio.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//////////////////////////////////////////////////////////////////////////////////
//
#ifdef ASSERT
#undef ASSERT
#endif
///////////////////////////////////////////////////////////////////////////////////////
# if defined (CFIO_USE_WIN32_API)
  #include <windows.h>
  #include <crtdbg.h>
# endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
  #define                CFIO_ALLOC(a,b)                  GlobalAlloc(a,b)
  #define                CFIO_REALLOC(a,b,c)              GlobalRealloc(a,b,c)
  #define                CFIO_FREE(a)                     GlobalFree(a)
  #define                CFIO_LOCK(a)                     GlobalLock(a)
  #define                CFIO_UNLOCK(a)                   GlobalUnlock(a)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTCGlobalHeader
{
protected:
	Handle                      hGlobalHandle;
	char*                      pcMemoryBlock;
	CTCGlobalHeader *           pNext;
	uint32_t                      wSize;
	uint32_t                      wHeaderSize;
	uint32_t                      wFlag;

public:
	CTCGlobalHeader();
	CTCGlobalHeader(Handle NewHandle,
		            void * NewData = NULL,
					uint32_t NewSize = 0,
					uint32_t NewFlag = 0,
					CTCGlobalHeader * NewNext = NULL);
	~CTCGlobalHeader();

public:
	Handle              GetHandle(void)                   { return hGlobalHandle; };
	void *              GetData(void)                     { return (void *)pcMemoryBlock; };
	uint32_t              GetFlag(void)                     { return wFlag; };
	uint32_t              GetSize(void)                     { return wSize; };
	uint32_t              GetHeaderSize(void)               { return wHeaderSize; };
	CTCGlobalHeader *   GetNext(void)                     { return pNext; };

public:
	Handle              SetHandle(Handle GlobalHandle)    { return (hGlobalHandle = GlobalHandle); };
	void *              SetData(void * Data)              { return (void *)(pcMemoryBlock = (char*)Data); };
	CTCGlobalHeader *   SetNext(CTCGlobalHeader * Next)   { return (pNext = Next); };
	uint32_t              SetSize(uint32_t Size)              { return (wSize = Size); };
	uint32_t              SetHeaderSize(uint32_t Size)        { return (wHeaderSize = Size ); };
	uint32_t              SetFlag(uint32_t Flag)              { return (wFlag = Flag ); };
	Bool32              IsFlag(uint32_t Flag)               { return (wFlag&Flag) ; };
	Bool32              AddFlag(uint32_t Flag)              { return (IsFlag( wFlag |= Flag )); };
	Bool32              RemoveFlag(uint32_t Flag)           { return (!IsFlag( wFlag &= ~Flag )); };
};

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
#define                HandleFILE                  Handle
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

struct CTC_mem_cluster
{
	struct CTC_mem_cluster *  mcNext;
	Handle                    mcHandle;
	void *                    mcPtr;
	uint32_t                    mcSize;
	uint32_t                    mcMemoryFlag;
	Bool32                    mcLocked;
	uint32_t                    mcFill;
	uint32_t                    mcNumber;
};

typedef    struct CTC_mem_cluster      CFIOMCLUSTER, *PCFIOMCLUSTER, **PPCFIOMCLUSTER;

class CTCGlobalFile
{
private:
	HandleFILE          hFile;
	//LPOFSTRUCT          pOpenFile;
	char                cFileName[CFIO_MAX_PATH];
	uint32_t              wSeeker;
	char                cFlag[32];
	Bool32              Deleted;
	Bool32              InMemory;
	Bool32              MoveToFile;
	uint32_t              wClusterCounter;
	uint32_t              wMemorySize;
	CFIOMCLUSTER        mcFirst;

public:
	CTCGlobalFile();
	CTCGlobalFile(char* Name, uint32_t Flag);
	~CTCGlobalFile();

public:
	uint32_t              Read(void * pData, uint32_t wDataSize, uint32_t wDataCounter);
	uint32_t              Write(void * pData, uint32_t wDataSize, uint32_t wDataCounter);
	uint32_t              Flush();
	uint32_t              Tell();
	uint32_t              Seek(uint32_t Position, uint32_t Flag);
	char*              GetFileName(char* lpName = NULL);
	uint32_t              GetFileLenght();
	char*              SetFileName(char* pcFileName );
	Bool32              Close();

public:
	Handle              GetFileHandle()                 { return ((Handle)hFile); };
	HandleFILE          GetHandle()                     { return hFile; };
	char*              GetFlagString(void)             { return cFlag; };
	Bool32              IsInString(const char* Flag );
	uint32_t              GetFileSize();
	uint32_t              GetHeaderSize()                 { return(sizeof(class CTCGlobalFile)); };
	Bool32              SetDelete()                     { return ( Deleted = TRUE ); };
	Bool32              KeepOnDisk()                    { return !( Deleted = FALSE ); };
	Bool32              IsDeleted()                     { return ( Deleted); };

protected:
	Handle              SetFileHandle(Handle NewFile)   { return( hFile = (HandleFILE)NewFile );};
	HandleFILE          SetHandle(HandleFILE NewHandle) { return( hFile = NewHandle);};
	Bool32              ProvideFileFolder( char* lpFileFuelName );

public:
	void                TranslateFlagToString(uint32_t Flag);
	void                ClearFlagString(void);
	void                AddFlagToString(const char*Flag);

private:
	Handle              CreateNewCluster(PPCFIOMCLUSTER pmcCluster = NULL);
	Bool32              KillLastCluster(PCFIOMCLUSTER pCluster = NULL);
	PCFIOMCLUSTER       TakeCluster(Handle hCluster);
	Handle              GetFirstCluster(){ return (mcFirst.mcNext)->mcHandle; };
	Handle              GetNextCluster(Handle Cluster, PPCFIOMCLUSTER pmcCluster = NULL);
	Handle              GetLastCluster(PPCFIOMCLUSTER pmcCluster = NULL);
	Handle              GetSeekedCluster(PPCFIOMCLUSTER pmcCluster = NULL);
	void *              GetPtrToMemoryCluster(Handle hCluster, PCFIOMCLUSTER pCluster = NULL);
	Bool32              ClosePtrToMemoryCluster(Handle hCluster, PCFIOMCLUSTER pCluster = NULL);
	uint32_t              WriteToMemory(void * pData, uint32_t wDataSwze, uint32_t wDataCounter);
	uint32_t              ReadFromMemory(void * pData, uint32_t wDataSize, uint32_t wDataCounter);
	Bool32              MoveFromMemory(Handle dFile);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

#endif        //_CTC_BASE_CLASSES_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//end of file
