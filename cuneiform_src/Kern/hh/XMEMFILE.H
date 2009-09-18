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

/////////////////////////////////////////////////////////////////
// class XMemFile - utility to put/get misc data into binary pool,
// read/write it to disk
//
// 28.05.97 22:32, Postnikov
/////////////////////////////////////////////////////////////////
#ifndef __XMEMFILE_H
   #define __XMEMFILE_H

   #ifndef __XPOOL_H
   #  include "xpool.h"
   #endif

   #ifndef __SWABYTES_H
   #  include "swabytes.h"
   #endif

class XMemFile : public XPool
{
   int32_t    nFileLength;  // count of bytes in use (put into)
   uchar*   pCur;        // current ptr

public:
   XMemFile( int32_t init_size = 0 )
      : XPool(init_size)
      { pCur=(uchar*)Data; nFileLength = 0; };

   int32_t    Tell()
      { return pCur - (uchar*)Data; };

   int32_t    FileLength()
      { return nFileLength; };

   void    RestoreFileLength(int32_t len) // Oleg : knot for second recongition
      { nFileLength=len; };

   Bool     Seek( int32_t nOffset )
      {  if ((nOffset > nFileLength)||(nOffset < 0))
            RET_FALSE;
         pCur= ((uchar*)Data)+nOffset;
         return TRUE;
      };

   void     SeekToStart( void ) {  pCur= ((uchar*)Data); };
   void     SeekToFinish( void ) {  pCur= ((uchar*)Data)+nFileLength; };

   void     Reset() { nFileLength = 0; SeekToStart(); };

   Bool     Get( void* data, int32_t size )
      {
         if (size < 0 || size+Tell() > nFileLength)
            RET_FALSE;
         if (size==0)
            return TRUE;
         stdMemcpy(data, pCur, size);
         pCur+=size;
         return TRUE;
      };

   Bool     Writeln( const char* str )
      {
         return Put(str, strlen(str)+1);
      };
   char*    Readln(void) // be sure that there is zt string!
      {
	 int nRest = nFileLength - Tell();
         if (nRest <=0)
            return NULL;
	 char* pNext = (char*)memchr(pCur, 0, nRest);
	 if (pNext==NULL)
            return NULL;
         char* ret = (char*)pCur;
         pCur = (uchar*)pNext; pCur++;
         return ret;
      };

   Bool     Reserve( int32_t size )
      {
         if (size < 0)
            RET_FALSE; // too strange...
         int32_t tell = Tell();
         if (size+tell <= Volume)
            return TRUE; // there is enough place

         int32_t new_size = maxi(size+tell, Volume*2);
         if (!Realloc( new_size ))
         {
            if (   size+tell < Volume*2  &&
                  !Realloc(size+tell )
               )
            RET_FALSE;  // can't realloc
         }
         Seek(tell);
         return TRUE;
      }

   Bool     Put( const void* data, int32_t size )
      {
         if (size==0)
            return TRUE;
         if (!Reserve(size)) // prepeare place to put
            RET_FALSE;
         // put
         stdMemcpy(pCur, (void*)data, size);
         pCur+=size;
         nFileLength += size;
         return TRUE;
      };

   // TODO: optimize!
   Bool     Get( int32_t& t ) { return Get( &t, sizeof(int32_t) ); }
   Bool     Put( int32_t& t ) { return Put( &t, sizeof(int32_t) ); }
   Bool     Get( int16_t& t ) { return Get( &t, sizeof(int16_t) ); }
   Bool     Put( int16_t& t ) { return Put( &t, sizeof(int16_t) ); }
   Bool     Get( char& t ) { return Get( &t, sizeof(char) ); }
   Bool     Put( char& t ) { return Put( &t, sizeof(char) ); }
   Bool     Get( uint32_t& t ) { return Get( &t, sizeof(uint32_t) ); }
   Bool     Put( uint32_t& t ) { return Put( &t, sizeof(uint32_t) ); }
   Bool     Get( uint16_t& t ) { return Get( &t, sizeof(uint16_t) ); }
   Bool     Put( uint16_t& t ) { return Put( &t, sizeof(uint16_t) ); }
   Bool     Get( uchar& t ) { return Get( &t, sizeof(uchar) ); }
   Bool     Put( uchar& t ) { return Put( &t, sizeof(uchar) ); }
   Bool     Get( Rect16& t ) { return Get( &t, sizeof(Rect16) ); }
   Bool     Put( Rect16& t ) { return Put( &t, sizeof(Rect16) ); }
   Bool     Get( Point16& t ) { return Get( &t, sizeof(Point16) ); }
   Bool     Put( Point16& t ) { return Put( &t, sizeof(Point16) ); }
   Bool     Get( Rect32& t ) { return Get( &t, sizeof(Rect32) ); }
   Bool     Put( Rect32& t ) { return Put( &t, sizeof(Rect32) ); }
   Bool     Get( Point32& t ) { return Get( &t, sizeof(Point32) ); }
   Bool     Put( Point32& t ) { return Put( &t, sizeof(Point32) ); }
   // end optimize
/*
   template <class T>
   Bool     Get( T& t ) { return Get( &t, sizeof(T) ); }

   template <class T>
   Bool     Put( T& t ) { return Put( &t, sizeof(T) ); }
*/
   Bool     GetArray( void* p_start, int32_t& nCount, int32_t cbElemSize )
      {  if (!Get( nCount ))
            RET_FALSE;
         if (!Get( p_start, nCount*cbElemSize ))
            RET_FALSE;
         return TRUE;
      };

   Bool     PutArray( void* p_start, int32_t nCount, int32_t cbElemSize )
      {  if (!Put( nCount ))
            RET_FALSE;
         if (!Put( p_start, nCount*cbElemSize ))
            RET_FALSE;
         return TRUE;
      };

   void     Destroy() { XPool::Destroy(); pCur = NULL; nFileLength = 0; };
   /////////////////////////////////////////////////////
   // Disk IO
   Err16    Read( XFile & xf, Bool32 swap_bytes = FALSE )
      {
         RETIFERR( XPool::Read(xf, swap_bytes) );
         nFileLength = XPool::GetVolume();
         SeekToStart();
         return ER_NONE;
      };

   Bool     Write( XFile & xf ) const
      {
         if(!XPool::Write(xf, nFileLength))   RET_FALSE;
         return TRUE;
      }

   /////////////////////////////////////////////////////
   // Exchange
   Bool CopyFrom( XMemFile& src )
   {
      Destroy();
      if (!XPool::CopyFrom( *(XPool*)&src ))
         return FALSE;

      nFileLength = src.FileLength();
      Seek( src.Tell() );
      return TRUE;
   }
};

#endif // __XMEMFILE_H

