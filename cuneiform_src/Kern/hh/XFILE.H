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

//////////////////////////////////////////////////////////
// class XFile : C++ layer for file operations;
// based on open(), read(), write(), close() set of C-funcs;
// Features:	auto-closing,
// 		template reading-writing (TagRead(), TagWrite())
//
// 1997, Postnikov
///////////////////////////////////////////////////////////

#ifndef __XFILE_H
#define __XFILE_H

#if _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>
#include "globus.h"

#include "std.h"

#ifdef WIN32
#define PPS_WIN32
#include<windows.h>
#define sleep(a) Sleep(1000*a)
#define fsync(a)
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

enum XFileOpenMode
{  XF_UNDEFINED = 0,
   XF_READ      = O_RDONLY|O_BINARY,
   XF_READ_TXT  = O_RDONLY,
   XF_WRITE     = O_WRONLY|O_CREAT|O_TRUNC|O_BINARY,
   XF_WRITE_TXT = O_WRONLY|O_CREAT|O_TRUNC,
   XF_RDWR      = O_RDWR|O_CREAT|O_BINARY,
   XF_APPEND    = O_WRONLY|O_CREAT|O_APPEND|O_BINARY
                   };
class XFile
{
   // internal attributes
   int            hnd;
   XFileOpenMode  mode;
   Err16          err;
   void  Zero(void) { hnd=-1; mode = XF_UNDEFINED; err = ER_NONE; };
public:
   // status
   Bool  operator !() const  { return !IsReady(); };
   Bool  IsReady(void) const { return hnd != -1;  };
   Err16 GetError() const    { return err;        };

   // construction & initialization
   XFile(void) { Zero(); };
   XFile(char* name, XFileOpenMode _mode) { Zero(); Open( name, _mode ); };
   XFile( XFile & xf ) { if (xf.hnd != -1){ assert(0); }; }; // no copy for opened files!
   ~XFile(void) { Close(); };

#if _MSC_VER
   int32_t  Commit(void) { return (hnd != -1) ? FlushFileBuffers((HANDLE) _get_osfhandle(hnd)) : -1; };
#else
   int32_t  Commit(void) { return (hnd != -1) ? fsync(hnd) : -1; };
#endif
   Bool32 Open(char* name, XFileOpenMode mode_)
   {
      assert(hnd==-1);
      mode = mode_;
      hnd  = stdOpen( name, mode );
      if (hnd == -1)
      {
         err = ER_CANTOPEN;
         return FALSE;
      };
      return TRUE;
   };

   Bool  Close(void)
   {
      if (hnd!= -1)
      {  int n = stdClose(hnd);
         hnd = -1;
         return n == 0;
      };
      return TRUE;
   };

   Bool  Read( void* p, int32_t size )
      {  int32_t nr=nRead(p, size);
         return nr >=0 && size == nr;
      };

   Bool  Write( void* p, int32_t size )
      {  int32_t nw=nWrite(p, size);
         return nw >=0 && size == nw;
      };

   int32_t nRead( void* p, int32_t size )
   {  if (size ==0)
         {  return 0;  };
      if (hnd  == -1)
         { err = ER_NOTREADY; return -1; };
      return stdRead(hnd, p, size);   // char* - for Mac's
   };

   int32_t nWrite( void* p, int32_t size )
   {  if (size ==0)
         {  return 0;  };
      if (hnd  == -1)
         { err = ER_NOTREADY; return -1; };
      return stdWrite(hnd, p, size);  // char* - for Mac's
   };

   int32_t Tell(void)
      { return stdTell(hnd); };

   int32_t nSeek( int32_t offset, int32_t set = SEEK_SET )
      { return stdSeek(hnd, offset, set); };

   Bool32 Seek( int32_t offset ) // set from start to pos, true - success
   {  if (hnd ==-1 || offset < 0)
         return FALSE;
      return nSeek(offset, SEEK_SET) == offset;
   };

   int32_t GetLength( void )
      {   return stdFileLength(hnd); };
};

/*** "Omnitag" reading/writing functions *************/
#ifndef NO_OMNITAG_IO

template <class T>
Bool32  TagRead( XFile& xf, T& t )
   {  return xf.Read( (void*)&t, sizeof(t) );
   }

template <class T>
Bool32  TagWrite( XFile& xf, T& t )
   {  return xf.Write( (void*)&t, sizeof(t) );
   }

#endif // NO_OMNITAG_IO

#endif // __XFILE_H
