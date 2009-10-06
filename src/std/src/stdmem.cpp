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


#include "std.h"

#ifdef __BORLANDC__
#include "alloc.h"
#include "dos.h"
#endif

#ifdef _DEBUG
   static int32_t _stdMallocCounter=0;
   static int32_t _stdReallocCounter=0;
   static int32_t _stdFreeCounter=0;

   class __TestStdMem
   {
   public:
      ~__TestStdMem()
         {
#ifndef _NO_MEM_REPORT
			assert( _stdMallocCounter == _stdFreeCounter );
#endif
	  };
   };
   static __TestStdMem tester;

#endif // _DEBUG

STD_FUNC( void )  stdGetMemInfo( StdMemInfo* pMemInf )
{
   if (pMemInf)
   {
#ifdef _DEBUG
      pMemInf->nMallocCnt = _stdMallocCounter;
      pMemInf->nFreeCnt = _stdFreeCounter;
      pMemInf->nReallocCnt = _stdReallocCounter;
#endif // _DEBUG
   }
}


STD_FUNC( void* ) stdMalloc( int32_t size )
{
#if 0
   void * res = malloc(size);

#ifdef _DEBUG
   if (res==NULL)
   {
      CONSOLE("stdMalloc(%d)=>NULL {%d}", size, _stdMallocCounter);
   }
   else
   {
      _stdMallocCounter++; // need free later
   }
#endif

   return res;
#endif
   return malloc(size);
}

STD_FUNC( void* )  stdRealloc( void* old_ptr, int32_t new_size, int32_t old_size )
{

#if 0
   _stdReallocCounter++;
      if (res==NULL)
      {
         CONSOLE("stdRealloc(%d)=>NULL {%d}", new_size, _stdReallocCounter);
         if (old_ptr != NULL && new_size == 0)   // <=> free
            _stdFreeCounter++;
      }
      else
      {
         if (old_ptr == NULL) // <=> malloc
            _stdMallocCounter++;
      }
#endif
      return realloc(old_ptr, new_size);
}

STD_FUNC( void  ) stdFree( void* ptr )
{
   if (ptr==NULL)
      return;
   free(ptr);
#ifdef _DEBUG
   _stdFreeCounter++;
#endif
}

STD_FUNC( void  ) stdFreeX( void** pptr )
{
#ifdef _DEBUG
   assert(pptr);
#endif
   if (*pptr)
   {
      stdFree(*pptr);
      *pptr = NULL;
   };
}

STD_FUNC( void* ) stdMemcpy( void* dest , void* src , int32_t copy_count )
{

#ifdef _DEBUG
   assert(dest);
   assert(src);
#endif

   return memcpy(dest, src, copy_count);
}
