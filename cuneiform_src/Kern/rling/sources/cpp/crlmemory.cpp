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

// Для использования без CFIO.DLL
#ifndef _NO_CFIO
	#define _NO_CFIO
#endif

/*#define  RLING_USE_GLOBAL_MEM*/

#ifdef RLING_USE_GLOBAL_MEM
#include <windows.h>
#endif

#include "resource.h"
#include "crldefines.h"
#include "crlmemory.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef _NO_CFIO
#include "cfio.h"
#endif

// extern functions
//void SetReturnCode_rling(uint16_t rc);
//uint16_t GetReturnCode_rling();

/////////////////////////////////////////////////////////////////////////////////////////////////////
//memory Allocator for RLING module
void *	RLINGAlloc(uint32_t stAllocateBlock)
{
    /* JussiP: disabled all this, since it seems to be legacy from Win16 days. Just use malloc.
	char * mem = NULL;

#ifdef _NO_CFIO

  #ifdef  RLING_USE_GLOBAL_MEM
	mem = (char *)GlobalAlloc(GPTR, stAllocateBlock);
  #else
	mem = ::new char[stAllocateBlock];
  #endif

	if(!mem)
		SetReturnCode_rling(IDS_RLING_ERR_NO_MEMORY);
#else

	mem = (char *)AllocMemory(1024,MAF_GALL_GPTR);

	if(!mem)
		SetReturnCode_rling(IDS_RLING_ERR_NO_MEMORY);

#endif

	return mem;
	*/
    return malloc(stAllocateBlock);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
void	RLINGFree(void * mem)
{
    /*
#ifdef _NO_CFIO

  #ifdef  RLING_USE_GLOBAL_MEM
	GlobalFree(mem);
  #else
	::delete []	mem;
  #endif

#else

	FreeMemory(mem);

#endif
*/
    free(mem);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
void *    RLINGLock( void * mem )
{
#ifdef _NO_CFIO

	return mem;

#else

	return LockMemory(mem);

#endif
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
void     RLINGUnlock( void * mem )
{
#ifdef _NO_CFIO

	return;

#else

	UnlockMemory(mem);
	return;
#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Handle  RLINGOpenSave(char * lpName)
{
	Handle rc = NULL;
#ifdef _NO_CFIO
	rc = (Handle)fopen(lpName,"wb");
#endif
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
Handle  RLINGOpenRestore(char * lpName)
{
	Handle rc = NULL;
#ifdef _NO_CFIO
	rc = (Handle)fopen(lpName,"rb");
#endif
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
unsigned int  RLINGWrite(Handle h,void * lpdata,unsigned int size)
{
	uint32_t rc = 0;
#ifdef _NO_CFIO
	rc = fwrite(lpdata,1,size,(FILE*)h);
#endif
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
unsigned int  RLINGRead(Handle h,void * lpdata,unsigned int size)
{
	uint32_t rc = 0;
#ifdef _NO_CFIO
	rc = fread(lpdata,1,size,(FILE *)h);
#endif
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
void    RLINGClose(Handle h)
{
#ifdef _NO_CFIO
	fclose((FILE*)h);
#endif
}
/////////////////////////////////////////////////////////////////////////////////////////
// end of file
