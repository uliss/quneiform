/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé 
        îòêàç îò ãàðàíòèé. 
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â 
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.  
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò 
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ 
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ. 

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ, 
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ 
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ 
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ 
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ 
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ, 
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ 
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ, 
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ 
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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

# if defined (_DEBUG)
# pragma message( __FILE__" : Base defenition")
# endif // (_DEBUG)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "cfio.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>
//////////////////////////////////////////////////////////////////////////////////
//
#ifdef ASSERT
#undef ASSERT
#endif
//#include <assert.h>
//#define ASSERT(a)              a
//////////////////////////////////////////////////////////////////////////////////
// for provide functions for temporary file/folder names
#ifdef LPVOID
#undef LPVOID
#endif
//////////////////////////////////////////////////////
typedef char        Char8, *PChar8, **PPChar8;
///////////////////////////////////////////////////////////////////////////////////////
// èñïîëüçóêòñÿ â ñëó÷àå 
//#define CFIO_USE_GLOBAL_MEMORY
#define CFIO_USE_WIN32_API
///////////////////////////////////////////////////////////////////////////////////////
# if defined (CFIO_USE_WIN32_API) //& defined (CFIO_USE_GLOBAL_MEMORY)
  #include <windows.h>
#include <crtdbg.h>
  //#include <afx.h>
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
	PChar8                      pcMemoryBlock;
	CTCGlobalHeader *           pNext;
	Word32                      wSize;
	Word32                      wHeaderSize;
	Word32                      wFlag;

public:
	CTCGlobalHeader();
	CTCGlobalHeader(Handle NewHandle, 
		            void * NewData = NULL, 
					Word32 NewSize = 0,
					Word32 NewFlag = 0,
					CTCGlobalHeader * NewNext = NULL);
	~CTCGlobalHeader();

public:
	Handle              GetHandle(void)                   { return hGlobalHandle; };
	void *              GetData(void)                     { return (void *)pcMemoryBlock; };
	Word32              GetFlag(void)                     { return wFlag; };
	Word32              GetSize(void)                     { return wSize; };
	Word32              GetHeaderSize(void)               { return wHeaderSize; };
	CTCGlobalHeader *   GetNext(void)                     { return pNext; }; 
	
public:	
	Handle              SetHandle(Handle GlobalHandle)    { return (hGlobalHandle = GlobalHandle); };
	void *              SetData(void * Data)              { return (void *)(pcMemoryBlock = (PChar8)Data); };
	CTCGlobalHeader *   SetNext(CTCGlobalHeader * Next)   { return (pNext = Next); };
	Word32              SetSize(Word32 Size)              { return (wSize = Size); };
	Word32              SetHeaderSize(Word32 Size)        { return (wHeaderSize = Size ); };
	Word32              SetFlag(Word32 Flag)              { return (wFlag = Flag ); };
	Bool32              IsFlag(Word32 Flag)               { return (wFlag&Flag) ; };
	Bool32              AddFlag(Word32 Flag)              { return (IsFlag( wFlag |= Flag )); };
	Bool32              RemoveFlag(Word32 Flag)           { return (!IsFlag( wFlag &= ~Flag )); };
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
#define                CFIO_GETFOLDERSITEMS             _splitpath
#define                CFIO_MAKEPATH                    _makepath
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
	Word32                    mcSize;
	Word32                    mcMemoryFlag;
	Bool32                    mcLocked;
	Word32                    mcFill;
	Word32                    mcNumber;
};

typedef    struct CTC_mem_cluster      CFIOMCLUSTER, *PCFIOMCLUSTER, **PPCFIOMCLUSTER;

class CTCGlobalFile
{
private:
	HandleFILE          hFile;
	//LPOFSTRUCT          pOpenFile;
	char                cFileName[CFIO_MAX_PATH];
	Word32              wSeeker;
	char                cFlag[32];
	Bool32              Deleted;
	Bool32              InMemory;
	Bool32              MoveToFile;
	Word32              wClusterCounter;
	Word32              wMemorySize;
	CFIOMCLUSTER        mcFirst;

public:
	CTCGlobalFile();
	CTCGlobalFile(PChar8 Name, Word32 Flag);
	~CTCGlobalFile();

public:
	Word32              Read(void * pData, Word32 wDataSize, Word32 wDataCounter);
	Word32              Write(void * pData, Word32 wDataSize, Word32 wDataCounter);
	Word32              Flush();
	Word32              Tell();
	Word32              Seek(Word32 Position, Word32 Flag);
	PChar8              GetFileName(PChar8 lpName = NULL);
	Word32              GetFileLenght();
	PChar8              SetFileName(PChar8 pcFileName );
	Bool32              Close();

public:
	Handle              GetFileHandle()                 { return ((Handle)hFile); };
	HandleFILE          GetHandle()                     { return hFile; };
	PChar8              GetFlagString(void)             { return cFlag; };
	Bool32              IsInString(PChar8 Flag );
	Word32              GetFileSize();
	Word32              GetHeaderSize()                 { return(sizeof(class CTCGlobalFile)); };
	Bool32              SetDelete()                     { return ( Deleted = TRUE ); };
	Bool32              KeepOnDisk()                    { return !( Deleted = FALSE ); };
	Bool32              IsDeleted()                     { return ( Deleted); };

protected:
	Handle              SetFileHandle(Handle NewFile)   { return( hFile = (HandleFILE)NewFile );};
	HandleFILE          SetHandle(HandleFILE NewHandle) { return( hFile = NewHandle);};
	Bool32              ProvideFileFolder( PChar8 lpFileFuelName );

public:
	void                TranslateFlagToString(Word32 Flag);
	void                ClearFlagString(void);
	void                AddFlagToString(PChar8 Flag);

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
	Word32              WriteToMemory(void * pData, Word32 wDataSwze, Word32 wDataCounter);
	Word32              ReadFromMemory(void * pData, Word32 wDataSize, Word32 wDataCounter);
	Bool32              MoveFromMemory(Handle dFile);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

#endif        //_CTC_BASE_CLASSES_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//end of file