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

#include "internal.h"
#pragma hdrstop

#ifdef _MSC_VER
#include<io.h>
#else
#include <unistd.h>
#endif
#include <sys/stat.h>

#include "std.h"
/*#include <io.h>*/
#include <fcntl.h>
#include "xpath.h"
#include "xfindfil.h"
#ifdef WIN32
   #include "win_mini.h"
#endif

#include "compat_defs.h"

typedef unsigned char BYTE;

static Int32 _stdOpenCounter=0;
static Int32 _stdCloseCounter=0;
static Int32 _stdReadCounter=0;
static Int32 _stdWriteCounter=0;
static Int32 _stdSeekCounter=0;
static Int32 _stdTellCounter=0;
static Int32 _stdFileLengthCounter=0;

STD_FUNC( Int32 ) stdOpen( const char *filename, Int32 oflag, Int32 pmode )
{
      _stdOpenCounter++;
      assert(filename);

      if (pmode == 0)
         pmode = S_IREAD | S_IWRITE;

      Int32 hnd  = open( filename, oflag, pmode );
      if (hnd == -1)
      {
         CONSOLE("stdOpen('%s') failed {%ld}", filename, _stdOpenCounter);
      };
      return hnd;
}

STD_FUNC( Int32 ) stdClose( Int32 handle ) // ret: 0 -success, -1 -failed
{
      _stdCloseCounter++;
      if (handle == -1)
      {
         CONSOLE("stdClose(): invalid file handle");
         return -1;
      }

      int res = close(handle);

      if (res!=0)
      {
         CONSOLE("stdClose(%ld)=>%ld {%ld}", handle, res, _stdCloseCounter);
      }
      return res;
}

STD_FUNC( Int32 ) stdRead( Int32 handle, void *buffer, Int32 count )
{
   _stdReadCounter++;

   if (handle==-1 || buffer == NULL || count < 0)
   {  stdConsole("=>stdRead(%ld, %ld, %ld) {%ld}",
         handle, (intptr_t)buffer, count, _stdReadCounter);
      return -1;
   };

   Int32 res=0;

   if (count>0)
   {  res = read(handle, (char*)buffer, count); // char* - for Macintosh
   };

   if (res!=count)
   {  stdConsole("stdRead(%ld, %ld, %ld)=>%ld {%ld}",
         handle, (intptr_t)buffer, count, res, _stdReadCounter);
   };
   return res;
}

STD_FUNC( Int32 ) stdWrite( Int32 handle, void *buffer, Int32 count )
{
   _stdWriteCounter++;

   if (handle==-1 || buffer == NULL || count < 0)
   {  stdConsole("=>stdWrite(%ld, %ld, %ld) {%ld}",
         handle, (intptr_t)buffer, count, _stdWriteCounter);
      return -1;
   };

   Int32 res=0;
   if (count>0)
      res = write(handle, (char*)buffer, count);  // char* - for Macintosh

   if (res!=count)
   {  stdConsole("stdWrite(%ld, %ld, %ld)=>%ld {%ld}",
         handle, (intptr_t)buffer, count, res, _stdWriteCounter);
   };
   return res;
}


STD_FUNC( Int32 ) stdTell( Int32 handle )
{
   _stdTellCounter++;


   if (handle==-1 )
   {  stdConsole("=>stdTell(%ld) {%ld}",
         handle, _stdTellCounter);
      return -1;
   };

   Int32 res = _tell(handle);
   if (res == -1L)
   {
      stdConsole("stdTell(%ld)=>%ld {%ld}", handle, res, _stdTellCounter);
   }
   return res;
}

STD_FUNC( Int32 ) stdSeek( Int32 handle, Int32 offset, Int32 origin  )
{
   _stdSeekCounter++;

   if (handle==-1)
   {  stdConsole("=>stdSeek(%ld, %ld, %ld) {%ld}",
         handle, offset, origin, _stdSeekCounter);
      return -1;
   };

   Int32 res = lseek(handle, offset, origin);

   if (res == -1L)
   {
      stdConsole("stdSeek(%ld, %ld, %ld)=>%ld {%ld}", handle, offset, origin, res, _stdSeekCounter);
   }
   return res;
}

STD_FUNC( Int32 ) stdFileLength( Int32 hnd )
{
   _stdFileLengthCounter++;
   if (hnd==-1)
   {
      stdConsole("=>stdFileLength(-1) {%ld}", _stdFileLengthCounter);
      return -1;
   }
#ifndef WIN32
   Int32 cur = stdSeek(hnd, 0, SEEK_CUR);
   if (cur==-1)
      return -1;
   Int32 ret = stdSeek(hnd,  0, SEEK_END);
   if (ret==-1)
      return -1;
   cur=stdSeek(hnd, cur, SEEK_SET);
   if (cur==-1)
      return -1;
   return ret;
#else
   Int32 ret = ::filelength(hnd);
#endif
   if (ret == -1)
   {
      stdConsole("stdFileLength(%ld)=>%ld {%ld}", hnd, ret, _stdFileLengthCounter);
   }
   return ret;
}

STD_FUNC( Int32 ) stdAccess( const char *path, Int32 mode)
{
/*
	mode: 00 - existence only
		02 - write permission
		04 - read permission
		06 - read and write permission

	Returns 0 - OK, -1 - bad
*/


   return access( path, mode );
}

//////////////////////////////////////////////////////////
// Handling with directories
#ifdef WIN32
#include <direct.h>
#endif
STD_FUNC( Bool32 ) stdCheckDirectory( const char* name, Bool32 bCreateIfNone )
   // checks directory for existance;
   // if bCreateIfNone==TRUE - creates if not found
   // returns TRUE if directory exists or successfully created
{
#ifdef WIN32
   if (_access(name, 0)==0)
   {  DWORD attr = GetFileAttributes( name );
      if ((attr != 0xFFFFFFFF) && (attr & FILE_ATTRIBUTE_DIRECTORY))
         return TRUE;
      else
         return FALSE;  // file, but not directory!
   }
   // no directory, no file - try to create
   if (bCreateIfNone)
   {
      if (mkdir(name)==0)
      {
         return TRUE;
      }
      CONSOLE("Cannot create directory '%s'", name);
   };

   return FALSE;
#else
   RETFALSE;   // to be implemented
#endif
}

STD_FUNC( Bool32 ) stdDeleteDirectory(
      const char * lpDirName)
{
    Bool32 bDeleteOK=TRUE;
    XPath xpPath((char*)lpDirName);
    xpPath.CheckSlash();
    XPath xpFileMask=(char*)(xpPath+"*.*");
#ifdef WIN32
    {
       XFindFile xffFile(xpFileMask);
       while(xffFile)
       {
           XPath xpName=xffFile.SafeStr();
           XPath xpCurrFileName=(char*)(xpPath+xffFile.SafeStr());
           Bool32 bCurrUppDir=xpName=="." || xpName=="..";
           Bool32 bIsDirectory=xffFile.FileAttrib() & _A_SUBDIR;
           if(bIsDirectory)
           {
               if(!bCurrUppDir && !stdDeleteDirectory(xpCurrFileName))
                   bDeleteOK=FALSE;
           }
           else
           {
               if(!stdDeleteFile(xpCurrFileName))
                   bDeleteOK=FALSE;
           }
           ++xffFile;
       }
    } // end of enumeration...

    {
       XPath xpDir((char*)lpDirName);
       xpDir.StripSlash();
       if(!RemoveDirectory(xpDir))
           bDeleteOK=FALSE;
    }

    return bDeleteOK;
#else
   RETFALSE;   // to be implemented
#endif
}

STD_FUNC( Bool32 ) stdMoveDirectory(
      const char * lpDirNameDst,const char * lpDirNameSrc)
{
    Bool32 bMoveOK=TRUE;
    XPath xpPathDst((char*)lpDirNameDst);
    XPath xpPathSrc((char*)lpDirNameSrc);
    xpPathSrc.CheckSlash(); xpPathDst.CheckSlash();
    if(xpPathDst==xpPathSrc)
        return TRUE;
    XPath xpFileMask=(char*)(xpPathSrc+"*.*");
#ifdef WIN32
    {
       if(!stdCheckDirectory(xpPathDst,TRUE))
           bMoveOK=FALSE;
       XFindFile xffFile(xpFileMask);
       while(xffFile)
       {
           XPath xpName=xffFile.SafeStr();
           XPath xpCurrFileNameDst=(char*)(xpPathDst+xffFile.SafeStr());
           XPath xpCurrFileNameSrc=(char*)(xpPathSrc+xffFile.SafeStr());
           Bool32 bCurrUppDir=xpName=="." || xpName=="..";
           Bool32 bIsDirectory=xffFile.FileAttrib() & _A_SUBDIR;
           if(bIsDirectory)
           {
               if(!bCurrUppDir && !stdMoveDirectory(xpCurrFileNameDst,xpCurrFileNameSrc))
                   bMoveOK=FALSE;
           }
           else
           {
               if(!stdMoveFile(xpCurrFileNameDst,xpCurrFileNameSrc))
                   bMoveOK=FALSE;
           }
           ++xffFile;
       }
    } // end of enumeration...

   XPath xpDir((char*)lpDirNameSrc);
   xpDir.StripSlash();
   if(!RemoveDirectory(xpDir))
       bMoveOK=FALSE;
   if(bMoveOK==FALSE)
       stdDeleteDirectory(xpPathDst);
    return bMoveOK;
#else
   RETFALSE;   // to be implemented
#endif
}

STD_FUNC( Bool32 ) stdCopyDirectory(
      const char * lpDirNameDst,const char * lpDirNameSrc)
{
    Bool32 bCopyOK=TRUE;
    XPath xpPathDst((char*)lpDirNameDst);
    XPath xpPathSrc((char*)lpDirNameSrc);
    xpPathSrc.CheckSlash(); xpPathDst.CheckSlash();
    if(xpPathDst==xpPathSrc)
        return TRUE;
    XPath xpFileMask=(char*)(xpPathSrc+"*.*");
#ifdef WIN32
    {
       if(!stdCheckDirectory(xpPathDst,TRUE))
           bCopyOK=FALSE;
       XFindFile xffFile(xpFileMask);
       while(xffFile)
       {
           XPath xpName=xffFile.SafeStr();
           XPath xpCurrFileNameDst = (char*)(xpPathDst+xffFile.SafeStr());
           XPath xpCurrFileNameSrc = (char*)(xpPathSrc+xffFile.SafeStr());
           Bool32 bCurrUppDir=xpName=="." || xpName=="..";
           Bool32 bIsDirectory=xffFile.FileAttrib() & _A_SUBDIR;
           if(bIsDirectory)
           {
               if(!bCurrUppDir && !stdCopyDirectory(xpCurrFileNameDst,xpCurrFileNameSrc))
                   bCopyOK=FALSE;
           }
           else
           {
               if(!stdCopyFile(xpCurrFileNameDst,xpCurrFileNameSrc))
                   bCopyOK=FALSE;
           }
           ++xffFile;
       }
    } // end of enumeration...

   if(bCopyOK==FALSE)
       stdDeleteDirectory(xpPathDst);
    return bCopyOK;
#else
   RETFALSE;   // to be implemented
#endif
}

STD_FUNC( Word32 ) stdGetCurrentDirectory(
   Word32 nBufferLength,	// size, in characters, of directory buffer
   char * lpBuffer 	// address of buffer for current directory
   )
{
#ifdef WIN32
   return ::GetCurrentDirectory( nBufferLength, lpBuffer );
#else
   RETZERO;   // to be implemented
#endif
}

STD_FUNC( Bool32 ) stdSetCurrentDirectory(
   const char * lpBuffer 	// address of buffer for current directory
   )
{
#ifdef WIN32
   if (!::SetCurrentDirectory( lpBuffer ))
   {
      CONSOLE("stdSetCurrentDirectory('%s') failed", lpBuffer );
      return FALSE;
   };
   return TRUE;
#else
   RETFALSE;   // to be implemented
#endif
}

/* Return full path to the directory where the executable currently
 * running is located.
 */

static char _HomeDirectory[256]={0};
STD_FUNC( const char* ) stdGetHomeDirectory(void)
{
#ifdef WIN32
   if (_HomeDirectory[0]==0)  // first call
   {
      if (!::GetModuleFileName( NULL, _HomeDirectory, sizeof(_HomeDirectory)))
      {
         CONSOLE("STD: GetModuleFileName failed");
      };
      char* p=strrchr(_HomeDirectory, '\\');
      if (p)
         *p=0;
   };
   return _HomeDirectory;
#else
   //   RETFALSE;   // to be implemented
   return "."; // FIXME, read from /proc/self/exe or something like that.
#endif
}

STD_FUNC( void ) stdGoToHomeDirectory(void)
{
#ifdef WIN32
   stdSetCurrentDirectory(stdGetHomeDirectory());
#else
   RETVOID;   // to be implemented
#endif
}

STD_FUNC( Int32 ) stdGetDirectoryInfo(const char * lpBuffer,
              Int32 nFlags, void* pExtParm)
{
    Int32 nResult=0;
#ifdef WIN32
    if(nFlags==STD_DIRINFO_FL_DRVTYPE)
    {
        char szRootDir[256]={0};
        char szComputerNameThis[256]={0};
        char szComputerName[256]={0};
        DWORD wBuffSize=256;
        Int32 nLen=strlen(lpBuffer);
        if(nLen<2)
            return 0;
        Bool32 bMapped=FALSE;
        if(lpBuffer[1]==':')
            bMapped=TRUE;
        if(bMapped)
        {
            strncpy(szRootDir,lpBuffer,2);
            szRootDir[2]=0;
            Word32 wDrvType=GetDriveType(szRootDir);
            if(wDrvType==DRIVE_REMOTE)
                nResult|=STD_DIRINFO_DRV_NET;
            else if(wDrvType!=0 && wDrvType!=1)
                nResult|=STD_DIRINFO_DRV_LOCAL;
            return nResult;
        }
        Bool32 bFullNetWorkName=FALSE;
        if(!strncmp(lpBuffer,"\\\\",2))
            bFullNetWorkName=TRUE;
        if(bFullNetWorkName)
        {
            char* pName=(char*)lpBuffer+2;
            while(*pName!=0 && *pName!='\\')
                pName++;
            strncpy(szComputerName,lpBuffer+2,pName-(lpBuffer+2));
            szComputerName[pName-(lpBuffer+2)]=0;
            if(!GetComputerName(szComputerNameThis,&wBuffSize))
                return 0;
            if(strcmp(szComputerNameThis,szComputerName))
                nResult|=STD_DIRINFO_DRV_NET;
            else
                nResult|=STD_DIRINFO_DRV_LOCAL;
            return nResult;
        }
    }
   return nResult;
#else
   return nResult;   // to be implemented
#endif
}

STD_FUNC( Int32 ) stdCmpFileTime(
       const char * lpFileName1,const char * lpFileName2,
      Int32 nFlags)
{
    Int32 nResult=STD_CMPFILETIME_ERR;
#ifdef WIN32
    SECURITY_ATTRIBUTES stSecurityAttributes=
        {sizeof(SECURITY_ATTRIBUTES),0,TRUE};
    HANDLE hFile1=CreateFile(
        lpFileName1, GENERIC_READ,FILE_SHARE_READ,
        &stSecurityAttributes,OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,NULL);
    HANDLE hFile2=CreateFile(
        lpFileName2, GENERIC_READ,FILE_SHARE_READ,
        &stSecurityAttributes,OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,NULL);
    if( hFile1==INVALID_HANDLE_VALUE ||
        hFile2==INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile1);
        CloseHandle(hFile2);
        return STD_CMPFILETIME_ERR;
    }
    FILETIME ftTime1={0}; FILETIME ftTime2={0};
    GetFileTime(hFile1,NULL,NULL,&ftTime1);
    GetFileTime(hFile2,NULL,NULL,&ftTime2);
    Int32 nCmpRes=CompareFileTime(&ftTime1,&ftTime2);
    if(nCmpRes==-1)
        nResult=STD_CMPFILETIME_LESS;
    if(nCmpRes==1)
        nResult=STD_CMPFILETIME_MORE;
    if(nCmpRes==0)
        nResult=STD_CMPFILETIME_EQUAL;
    CloseHandle(hFile1);
    CloseHandle(hFile2);
    return nResult;
#else
    return nResult;
#endif
}

STD_FUNC( Bool32 ) stdCheckFile(const char * lpFileName )
{
#ifdef WIN32
    OFSTRUCT ostOpenBuff={0};
    HFILE hFile=OpenFile(lpFileName,&ostOpenBuff,OF_EXIST);
    return (hFile!=HFILE_ERROR ? TRUE : FALSE);
#else
    return FALSE;
#endif
}

STD_FUNC( Bool32 ) stdMoveFile(
      const char * lpFileNameDst,const char * lpFileNameSrc,
      Int32 nFlags)
{
#ifdef WIN32
    return MoveFile(lpFileNameSrc,lpFileNameDst);
#else
    return FALSE;
#endif
}

STD_FUNC( Bool32 ) stdCopyFile(
      const char * lpFileNameDst,
      const char * lpFileNameSrc )
{
#ifdef WIN32
    if(!CopyFile(lpFileNameSrc,lpFileNameDst,FALSE))
        return FALSE;
    return TRUE;
#else
    return FALSE;
#endif
}

STD_FUNC( Bool32 ) stdDeleteFile(
      const char * lpFileName
      )
{
#ifdef WIN32
    return DeleteFile(lpFileName);
#else
    return unlink(lpFileName) == 0;
#endif
}

/*#include "WinReg.h"
#include "Windows.h"*/
#include "xpath.h"

#if WIN32

Bool32 stdNetPathFromLocal(char* pszNetPath,Int32 nNetPathSize,const char* pszLocalPath)
{
    if(strlen(pszLocalPath)>=2 && !strncmp(pszLocalPath,"\\\\",2))
	{
		strcpy(pszNetPath,pszLocalPath);
		return TRUE;
	}
    pszNetPath[0]=0;
    char szDrv[128]={0};
    XPath xpLocPath(pszLocalPath);
    xpLocPath.Split(szDrv, NULL, NULL);
    if(strlen(szDrv)!=2 || szDrv[1]!=':')
    {
       // VVA
       XPath xpLocalDir;
       xpLocalDir=stdGetHomeDirectory();
       xpLocalDir.CheckSlash();
       xpLocalDir+=pszLocalPath;
       xpLocPath=xpLocalDir;
       return stdNetPathFromLocal(pszNetPath,nNetPathSize,xpLocPath.buf);
    }
    Word32 nDrvType=GetDriveType(szDrv);
    szDrv[0]=(char)stdUpperAscii((char)szDrv[0]);
    XPath xpPathWoDrv(pszLocalPath);
    xpPathWoDrv.EraseDrive();
    DWORD nValType=0; BYTE szValData[256]={0}; DWORD nDataLen=sizeof(szValData);
    if(nDrvType==DRIVE_FIXED)
    {
        char szComputer[128]={0}; ULONG nSize=sizeof(szComputer);
        ::GetComputerName(szComputer,&nSize);
        sprintf((char*)szValData,"//%s/%c",szComputer,szDrv[0]);
    }
    else
    {
        char szKeyPath[256]={0};
        sprintf(szKeyPath,"%s\\%c","Network\\Persistent",szDrv[0]);
        HKEY hkThisDrive;
        if(RegOpenKeyEx(HKEY_CURRENT_USER,szKeyPath,0,KEY_ALL_ACCESS,&hkThisDrive)!=ERROR_SUCCESS)
            return FALSE;
        if(RegQueryValueEx(hkThisDrive,"RemotePath",NULL,
            &nValType,szValData,&nDataLen)!=ERROR_SUCCESS)
            return FALSE;
    }
    sprintf(pszNetPath,"%s%s",szValData,xpPathWoDrv.buf);
    return TRUE;
}

#endif /* WIN32 */
