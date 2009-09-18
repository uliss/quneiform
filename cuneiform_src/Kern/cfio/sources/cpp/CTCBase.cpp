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
#include <assert.h>
#include "ctccontrol.h"
#include "compat_defs.h"
//////////////////////////////////////////////////////////////////////////////////
extern CTCControl * Control_ctc;
//////////////////////////////////////////////////////////////////////////////////
//
CTCGlobalHeader::CTCGlobalHeader()
{
	//ASSERT( FALSE );
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCGlobalHeader::CTCGlobalHeader(Handle NewHandle,
		                         void * NewData,
					             uint32_t NewSize,
					             uint32_t NewFlag,
					             CTCGlobalHeader * NewNext)
{
	SetHandle(NewHandle);
	SetData(NewData);
	SetNext(NewNext);
	SetSize(NewSize);
	SetFlag(NewFlag);
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCGlobalHeader::~CTCGlobalHeader()
{

}

//////////////////////////////////////////////////////////////////////////////////
//
CTCGlobalFile::CTCGlobalFile()
{
	_ASSERT( FALSE );
	SetHandle(NULL);
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCGlobalFile::CTCGlobalFile(char * Name, uint32_t Flag)
{
	ProvideFileFolder(Name);
	TranslateFlagToString(Flag);
	if ( Flag & CFIO_GF_IN_MEMORY )
	{
		Deleted = TRUE;
		InMemory = TRUE;
		MoveToFile = TRUE;
		wSeeker = 0;
		wClusterCounter = 0;
		mcFirst.mcPtr = mcFirst.mcHandle = mcFirst.mcNext = NULL;
		mcFirst.mcSize = 0;
		SetHandle(CreateNewCluster());
	}
	else
	{
		SetHandle(CFIO_OPEN(Name, GetFlagString()));
		Deleted = FALSE;
		InMemory = FALSE;
		MoveToFile = FALSE;
	}

	if ( GetHandle() )
	{
		SetFileName(Name);
		wSeeker = 0;
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCGlobalFile::~CTCGlobalFile()
{
	if ( GetHandle() )
		Close();

	if ( IsDeleted() )
		CFIO_REMOVE( cFileName );
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCGlobalFile::IsInString(const char * Flag)
{
	for (char * i = GetFlagString(); *i != 0x00; i++ )
	{
		if( *i == *Flag )
		{
			if( *(Flag + 1) == 0x00 )
				return TRUE;
			else
				if ( *(Flag + 1) == *(i + 1) )
				return TRUE;
		}
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCGlobalFile::Write(void * pData, uint32_t wDataSize, uint32_t wDataCounter)
{
	if ( IsInString(CFIO_GF_CWRITE) || IsInString(CFIO_GF_CWRITEREAD) ||
		 IsInString(CFIO_GF_CREADWRITE) || IsInString(CFIO_GF_CAPPEND) ||
		 IsInString(CFIO_GF_CAPPENDEOF) )
	{
		if ( !InMemory )
		{
			uint32_t Writed = CFIO_WRITE(pData, wDataSize, wDataCounter, GetHandle());
			Flush();
			return Writed;
		}
		else
		{
			return WriteToMemory(pData, wDataSize, wDataCounter);
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCGlobalFile::Read(void * pData, uint32_t wDataSize, uint32_t wDataCounter)
{
	if ( IsInString(CFIO_GF_CREAD) || IsInString(CFIO_GF_CWRITEREAD) ||
		 IsInString(CFIO_GF_CREADWRITE) || IsInString(CFIO_GF_CAPPEND) ||
		 IsInString(CFIO_GF_CAPPENDEOF) )
	{
		if ( !InMemory )
		{
			return CFIO_READ(pData, wDataSize, wDataCounter, GetHandle());
		}
		else
		{
			return ReadFromMemory(pData, wDataSize, wDataCounter);
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCGlobalFile::Close()
{
	uint32_t Closet;

	if ( !InMemory )
	{
		Closet = CFIO_CLOSE(GetHandle());
	}
	else
		if ( !IsDeleted() )
		{
			FILE * DiskFile;
			char DiskFileName[CFIO_MAX_PATH];

			// extract memroy file name for disk file
			// if present
			if ( *(GetFileName(DiskFileName)) != 0x00 )
			{
				if ( IsInString( CFIO_GF_CBINARY ) )
					DiskFile = CFIO_OPEN( DiskFileName, "wb" );
				else
					DiskFile = CFIO_OPEN( DiskFileName, "w" );
				//move from memory to disk
				MoveFromMemory(DiskFile);

				CFIO_CLOSE(DiskFile);
			}
		}
		else
		{
			MoveFromMemory(NULL);
		}

	if ( Closet == 0 )
	{
		SetHandle(NULL);
		return TRUE;
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//move file from memroy to disk and delete memory copy
Bool32 CTCGlobalFile::MoveFromMemory(Handle dFile)
{
	uint32_t          DataLeft = wMemorySize;
	uint32_t          wWriteCount = 0;
	uint32_t          ReadFromCluster;
	PCFIOMCLUSTER  	pCurrentCluster = mcFirst.mcNext;
	void *          lpData;

	if ( dFile )
	{
		while(DataLeft)
		{
			if ( DataLeft > CFIO_GF_MEMORY_FILE_CLUSTER )
			{
				ReadFromCluster = CFIO_GF_MEMORY_FILE_CLUSTER;
			}
			else
			{
				ReadFromCluster = DataLeft;
			}
			// take pointer to memory
			lpData = GetPtrToMemoryCluster(NULL, pCurrentCluster);
			// write memory to disk
			wWriteCount += CFIO_WRITE( lpData, sizeof(char), ReadFromCluster, dFile);
			DataLeft -= ReadFromCluster;
			ClosePtrToMemoryCluster(NULL, pCurrentCluster);
			pCurrentCluster = pCurrentCluster->mcNext;
		}
	}

	KillLastCluster( &mcFirst );

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
/*
Handle CTCGlobalFile::AssemblyToOne()
{
	uint32_t      DataLeft = wMemorySize;
	uint32_t      ReadFromCluster;
	MCLUSTER  *	pCurrentCluster = mcFirst.mcNext;
	Handle      hMemoryBlock;
	void *      lpData;
	void *      lpDest;
	char *      lpSeek;

	hMemoryBlock = 	Control_ctc->Alloc(wMemorySize, MAF_GALL_GHND);

	if ( hMemoryBlock )
	{
		lpSeek = (char *)(lpDest = Control_ctc->Lock(hMemoryBlock));

		while(DataLeft)
		{
			if ( DataLeft > CFIO_GF_MEMORY_FILE_CLUSTER )
			{
				ReadFromCluster = CFIO_GF_MEMORY_FILE_CLUSTER;
			}
			else
			{
				ReadFromCluster = DataLeft;
			}
			// take pointer to memory
			lpData = GetPtrToMemoryCluster(NULL, pCurrentCluster);
			// write memory to one block
			memcpy( (void *)lpSeek, lpData, ReadFromCluster);
			lpSeek += ReadFromCluster;

			ClosePtrToMemoryCluster(NULL, pCurrentCluster);

			pCurrentCluster = pCurrentCluster->mcNext;
		}

		Control_ctc->Unlock(hMemoryBlock);
	}

	return hMemoryBlock;
}
*/
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCGlobalFile::Seek(uint32_t Position, uint32_t Flag)
{
	int Direction;
	uint32_t NewSeeker;

	switch(Flag)
	{
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

	if ( InMemory )
	{
		switch ( Direction )
		{
		case SEEK_CUR:
			wSeeker += Position;

			if ( wSeeker > wMemorySize )
				wSeeker = wMemorySize;

			break;

		case SEEK_END:

			if ( Position < wMemorySize )
				wSeeker = wMemorySize - Position;
			else
				wSeeker = 0;

			break;

		case SEEK_SET:

			if ( Position < wMemorySize )
				wSeeker = Position;
			else
				wSeeker = wMemorySize;
			break;
		}
	}
	else
	{
		if ( CFIO_SEEK( GetHandle(), Position, Direction) == 0 )
		{
			NewSeeker = Tell();
			wSeeker = NewSeeker;
		}
	}
	return wSeeker;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCGlobalFile::Tell()
{
	if ( InMemory )
	{
		return wSeeker;
	}
	else
	{
		return CFIO_TELL(GetHandle());
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCGlobalFile::Flush()
{
	if ( InMemory )
	{
		return 0;
	}
	else
	{
		return CFIO_FLUSH(GetHandle());
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCGlobalFile::GetFileSize()
{
	uint32_t CurrentPosition = Tell();
	uint32_t Size;

	if ( Seek(0, CFIO_GF_SEEK_END) )
	{
		Size = Tell();
		Seek(CurrentPosition,CFIO_GF_SEEK_BEG);
	}
	return Size;
}
//////////////////////////////////////////////////////////////////////////////////
//
void CTCGlobalFile::TranslateFlagToString(uint32_t Flag)
{
	ClearFlagString();

	if ( Flag&CFIO_GF_CREATE )
	{
		if (Flag&CFIO_GF_READ )
			AddFlagToString(CFIO_GF_CWRITEREAD);
		else
			AddFlagToString(CFIO_GF_CWRITE);
	}
	else
	{
		if ( Flag&CFIO_GF_READ )
		{
			if( Flag&CFIO_GF_WRITE )
				AddFlagToString(CFIO_GF_CREADWRITE);
			else
			{
				if ( Flag&CFIO_GF_APPEND )
					AddFlagToString(CFIO_GF_CAPPENDEOF);
				else
					AddFlagToString(CFIO_GF_CREAD);
			}
		}
		else
		{
			if( Flag&CFIO_GF_WRITE )
				AddFlagToString(CFIO_GF_CWRITE);
			else
			{
				if ( Flag&CFIO_GF_APPEND )
					AddFlagToString(CFIO_GF_CAPPEND);
				else
					AddFlagToString(CFIO_GF_CWRITEREAD);
			}
		}
	}


	if ( Flag&CFIO_GF_BINARY )
		AddFlagToString(CFIO_GF_CBINARY);
	else
		AddFlagToString(CFIO_GF_CTEXT);

	if ( Flag&CFIO_GF_COMMIT )
		AddFlagToString(CFIO_GF_CCOMMIT);
}
//////////////////////////////////////////////////////////////////////////////////
//
void CTCGlobalFile::ClearFlagString(void)
{
	CFIO_STRCPY(cFlag,"");
}
//////////////////////////////////////////////////////////////////////////////////
//
void CTCGlobalFile::AddFlagToString(const char * Flag )
{
	CFIO_STRCAT(cFlag, Flag);
}
//////////////////////////////////////////////////////////////////////////////////
//
char * CTCGlobalFile::GetFileName(char * lpName)
{
	if ( lpName && CFIO_STRLEN(lpName) <= CFIO_MAX_PATH )
		CFIO_STRCPY(lpName, cFileName);

	return cFileName;
}
//////////////////////////////////////////////////////////////////////////////////
//
char * CTCGlobalFile::SetFileName(char * lpName )
{
	if ( lpName && CFIO_STRLEN(lpName) <= CFIO_MAX_PATH )
		CFIO_STRCPY(cFileName, lpName);

	return cFileName;
}

uint32_t CTCGlobalFile::GetFileLenght()
{
	if ( InMemory )
	{
		return wMemorySize;
	}
	else
	{
		uint32_t wFileLenght;
		uint32_t wCurrentPos = Tell();

		wFileLenght = Seek(0, CFIO_GF_SEEK_END);
		Seek(wCurrentPos, CFIO_GF_SEEK_BEG );
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
Bool32 CTCGlobalFile::ProvideFileFolder(char * lpFileName)
{
/* Given a filename such as "/foo/bar/baz/file.txt" creates the
 * directory "/foo/bar/baz".
 */
	if ( strlen(lpFileName) == 0 )
		return FALSE;

	//MAKEFULLPATH(FullPath, lpFileName, _MAX_PATH);
	CFIO_GETFOLDERSITEMS(lpFileName, Folder, File, Extension);
	CFIO_MAKEPATH ( Out, Folder, NULL, NULL);

	if ( Out[0] == 0x0 )
		return TRUE;

	CFIO_MAKEFOLDER(Out);
	//CreateDirectory(Out, NULL);

	CFIO_MAKEPATH(Out, Folder, File, Extension);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////memory file//////////////////////////////////////////////
//
Handle CTCGlobalFile::CreateNewCluster(PPCFIOMCLUSTER pmcCluster)
{
	PCFIOMCLUSTER pCurrentCluster, pNewCluster;
	// if not first
	if ( mcFirst.mcNext )
	{
		GetLastCluster(&pCurrentCluster);
	}
	else
	{
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
	pNewCluster->mcHandle = Control_ctc->Alloc(pNewCluster->mcSize, pNewCluster->mcMemoryFlag,"CFIO virtual file cluster","no comment");
	// new cluster is clear
	pNewCluster->mcFill = 0;
	pNewCluster->mcNumber = wClusterCounter;

	if ( pmcCluster )
	{
		*pmcCluster = pNewCluster;
	}

	if ( pNewCluster->mcHandle )
	{
		pCurrentCluster->mcNext = pNewCluster;
	}
	// increase cluster counter
	wClusterCounter++;
	return pNewCluster->mcHandle;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCGlobalFile::KillLastCluster(PCFIOMCLUSTER pEndCluster)
{
	PCFIOMCLUSTER pCluster, pLastCluster, pNextToDelete;

	// if 1 or more cluster present
	if ( mcFirst.mcNext )
	{
		// set last cluster in chain
		pLastCluster = &mcFirst;
		// define end of cluster chain -
		if ( pEndCluster )
		{
			// reset last cluster
			pLastCluster = pEndCluster;
			// set first cluster for delete
			pCluster = pEndCluster->mcNext;
		}
		// or delete las 1 cluster
		else
		{
			for ( pCluster = mcFirst.mcNext;
				  pCluster->mcNext != NULL;
				  pCluster = pCluster->mcNext)
			{
				pLastCluster = pCluster;
			}
		}
		// breah\k chain
		pLastCluster->mcNext = NULL;

		// delete clusters from end cluster
		while ( pCluster )
		{
			pNextToDelete = pCluster->mcNext;
			// unlock memory (for any case)
			if ( pCluster->mcLocked )
				Control_ctc->Unlock(pCluster->mcHandle);

			// free memory cluster and ...
			Control_ctc->Free(pCluster->mcHandle);
			// delete memory cluster info
			delete pCluster;
			// decrease cluster counter;
			wClusterCounter--;
			pCluster = pNextToDelete;
		}
		return TRUE;
	}

	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
PCFIOMCLUSTER CTCGlobalFile::TakeCluster(Handle hCluster)
{
	PCFIOMCLUSTER pCurrentCluster;

	for ( pCurrentCluster = mcFirst.mcNext;
	      pCurrentCluster != NULL;
		  pCurrentCluster = pCurrentCluster->mcNext)
		{
			if ( pCurrentCluster->mcHandle = hCluster )
			{
				return pCurrentCluster;
			}
		}
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
Handle CTCGlobalFile::GetLastCluster(PPCFIOMCLUSTER pmcCluster)
{
	PCFIOMCLUSTER pLastCluster;

	if ( !mcFirst.mcNext )
	{
		return &mcFirst;
	}

	for ( pLastCluster = mcFirst.mcNext;
	      pLastCluster->mcNext != NULL;
		  pLastCluster = pLastCluster->mcNext);

	if ( pmcCluster )
	{
		*pmcCluster = pLastCluster;
	}

	return pLastCluster->mcHandle;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
Handle CTCGlobalFile::GetNextCluster(Handle hCluster, PPCFIOMCLUSTER pmcCluster)
{
	PCFIOMCLUSTER pCluster = TakeCluster(hCluster);

	if ( pCluster )
	{
		if ( pmcCluster )
		{
			*pmcCluster = pCluster;
		}
		return pCluster->mcNext->mcHandle;
	}

	if ( pmcCluster )
	{
		*pmcCluster = NULL;
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
void * CTCGlobalFile::GetPtrToMemoryCluster(Handle hCluster, PCFIOMCLUSTER pCluster)
{
	PCFIOMCLUSTER pCurrentCluster;

	if ( pCluster )
	{
		pCurrentCluster = pCluster;
	}
	else
	{
		pCurrentCluster = TakeCluster(hCluster);
	}

	if ( !pCurrentCluster->mcLocked )
	{
		pCurrentCluster->mcPtr = Control_ctc->Lock(pCurrentCluster->mcHandle);
		pCurrentCluster->mcLocked = TRUE;
	}

	return pCurrentCluster->mcPtr;

}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCGlobalFile::ClosePtrToMemoryCluster(Handle hCluster, PCFIOMCLUSTER pCluster)
{
	PCFIOMCLUSTER pCurrentCluster;

	if ( pCluster )
	{
		pCurrentCluster = pCluster;
	}
	else
	{
		pCurrentCluster = TakeCluster(hCluster);
	}

	if ( pCurrentCluster->mcLocked )
	{
		pCurrentCluster->mcPtr = NULL;
		Control_ctc->Unlock(pCurrentCluster->mcHandle);
		pCurrentCluster->mcLocked = FALSE;
	}

	return !pCurrentCluster->mcLocked;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCGlobalFile::GetSeekedCluster(PPCFIOMCLUSTER pmcCluster)
{
	uint32_t FilledClusters = wSeeker/CFIO_GF_MEMORY_FILE_CLUSTER;
	PCFIOMCLUSTER pCluster;

	// if allright
	if ( FilledClusters <= wClusterCounter )
	{
		// seeker to end
		if ( wSeeker == wMemorySize )
		{
			GetLastCluster(&pCluster);
		}
		else
			//seeker to begin
			if ( wSeeker == 0 )
			{
				pCluster = mcFirst.mcNext;
			}
			// seeker to midle of file
			else
			{
				for ( pCluster = mcFirst.mcNext;
					  pCluster != NULL && FilledClusters > 0;
					  pCluster = pCluster->mcNext, FilledClusters--);
			}

	}
	else       // if seeker out of range
	{
		wSeeker = 0;
		pCluster = mcFirst.mcNext;
	}

	if ( pmcCluster )
	{
		*pmcCluster = pCluster;
	}

	if ( pCluster )
		return pCluster->mcHandle;
	else
		return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCGlobalFile::WriteToMemory(void * pData, uint32_t wDataSize, uint32_t wDataCounter)
{
	uint32_t ClusterOffset;
	uint32_t wAllData = wDataSize * wDataCounter;
	uint32_t AvailableToWriteInCluster;
	//uint32_t i;
	uint32_t Counter = 0;
	char *     Sorc = (char *)pData;
	char *     Desc;
	PCFIOMCLUSTER pWritedCluster;

	// position to seeker
	GetSeekedCluster(&pWritedCluster);
	// set offset in cluster
	ClusterOffset = wSeeker - ((wSeeker / CFIO_GF_MEMORY_FILE_CLUSTER) * CFIO_GF_MEMORY_FILE_CLUSTER);

	//writing:
	while(wAllData)
	{
		//  available to write in this cluster
		AvailableToWriteInCluster = CFIO_GF_MEMORY_FILE_CLUSTER - ClusterOffset;
		// set ptr to writing
		Desc = ((char *) GetPtrToMemoryCluster(NULL,pWritedCluster)) + ClusterOffset;

		//  if need to write less then free space in last cluster
		if ( wAllData < AvailableToWriteInCluster )
		{
			// write all data in one cluster
			AvailableToWriteInCluster = wAllData;
		}
		else
		{
			// or create new cluster if last can't be writen by all data
			if ( pWritedCluster->mcNext == NULL )
				CreateNewCluster();

			// clear offset for next cluster
			ClusterOffset = 0;
		}
		memcpy(Desc,Sorc,AvailableToWriteInCluster);
		Sorc += AvailableToWriteInCluster;
		wAllData -= AvailableToWriteInCluster;
		Counter += AvailableToWriteInCluster;
		wSeeker += AvailableToWriteInCluster;

		ClosePtrToMemoryCluster(NULL,pWritedCluster);

		// switch to write on next cluster
		if ( wAllData )
		{
			pWritedCluster = pWritedCluster->mcNext;
		}
	}
	// new file in memory size
	wMemorySize = wSeeker;
	return Counter;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCGlobalFile::ReadFromMemory(void * pData, uint32_t wDataSize, uint32_t wDataCounter)
{
	uint32_t ClusterOffset;
	uint32_t wAllData = wDataSize * wDataCounter;
	uint32_t AvailableToReadInCluster;
//	uint32_t i;
	uint32_t Counter = 0;
	char *     Sorc;
	char *     Desc = (char *)pData;
	PCFIOMCLUSTER pReadedCluster;

	// position to seeker
	GetSeekedCluster(&pReadedCluster);
	// set offset in cluster
	ClusterOffset = wSeeker - ((wSeeker / CFIO_GF_MEMORY_FILE_CLUSTER) * CFIO_GF_MEMORY_FILE_CLUSTER);

	if ( wAllData > wMemorySize )
		wAllData = wMemorySize;

	//writing:
	while(wAllData)
	{
		//  available to write in this cluster
		AvailableToReadInCluster = CFIO_GF_MEMORY_FILE_CLUSTER - ClusterOffset;
		// set ptr to writing
		Sorc = ((char *)GetPtrToMemoryCluster(NULL,pReadedCluster)) + ClusterOffset;

		//  if need to write less then free space in last cluster
		if ( wAllData < AvailableToReadInCluster )
		{
			// write all data in one cluster
			AvailableToReadInCluster = wAllData;
		}
		//copy data
		memcpy(Desc,Sorc,AvailableToReadInCluster);
		// move destination pointer to write next data
		Desc += AvailableToReadInCluster;
		// decrease data to copy counter
		wAllData -= AvailableToReadInCluster;
		// increase data copy counter
		Counter += AvailableToReadInCluster;
		// move virtual file position pointer
		wSeeker += AvailableToReadInCluster;

		ClosePtrToMemoryCluster(NULL,pReadedCluster);

		// switch to write on next cluster
		if ( wAllData )
		{
			pReadedCluster = pReadedCluster->mcNext;
			ClusterOffset = 0;

			if ( !pReadedCluster )
			{
				return 0;
			}
		}
	}
	return Counter;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file










