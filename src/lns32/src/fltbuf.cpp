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

#include "lnslang.h"

	#ifndef __FLTBUF_H
	#	include "fltbuf.h"
	#endif

   #ifndef __MEMOPS_H
   #  include "memops.h"
   #endif
   #ifndef __XMACROS_H
   #  include "xmacros.h"
   #endif
TFltBuf::TFltBuf( int width_dword )
	{
		x16 = NULL;
      if ( (int32_t)32 * sizeof(int32_t) * (int32_t)width_dword  > (int32_t)0xFFF0){
			// size of buffer > 64K
			return;
		};

      x16 = (int32_t *)(malloc( (int32_t)32 * (int32_t)sizeof(int32_t) * (int32_t)width_dword  ));
		if (x16 == NULL) {
			return;
		};
		widthDWord = width_dword;
		x8 		= x16 + 16*widthDWord;
		x4 		= x8  + 8 *widthDWord;
		x2 		= x4  + 4 *widthDWord;
		cur16 	= x2  + 2 *widthDWord;
		last16 	= cur16 + widthDWord;

		nCurLine = 0;

	}

void TFltBuf::destroy( void )
	{
		if (x16!=NULL) {
         free( (void*)x16 );
			x16 = NULL;
		};
	}

void TFltBuf::addLine( void* new_line )
	{
      memmove( (void*)(x16+widthDWord*nCurLine), new_line, widthDWord*4 );
		nCurLine++;
	}

/***********************/
#define  A_B_to_AB() {  memcpy(curAB, curA, wdw<<2);  CopyOR(curAB, curB, wdw); }
/***********************/

void TFltBuf::preBuffer( void )
   {
      int wdw = widthDWord;
      int32_t* curA;
      int32_t* curB;
      int32_t* curAB;
      int i=0;

      curA = (int32_t*)x16;
      curB = (int32_t*)x16 + wdw;
      curAB = (int32_t*)x8;
		for( i = 0; i < 8; i++ ){
         A_B_to_AB();
         curAB += wdw;
         curA  += wdw*2;
         curB  += wdw*2;
		};

      curA = (int32_t*)x8;
      curB = (int32_t*)x8 + wdw;
      curAB = (int32_t*)x4;
		for( i = 0; i < 4; i++ ){
         A_B_to_AB();
         curAB += wdw;
         curA  += wdw*2;
         curB  += wdw*2;
		};

      curA = (int32_t*)x4;
      curB = (int32_t*)x4 + wdw;
      curAB = (int32_t*)x2;
		for( i = 0; i < 2; i++ ){
         A_B_to_AB();
         curAB += wdw;
         curA  += wdw*2;
         curB  += wdw*2;
		};

      curA = (int32_t*)x2;
      curB = (int32_t*)x2 + wdw;
      curAB = (int32_t*)cur16;
      A_B_to_AB();

		nCurLine = 0;
	}

void TFltBuf::updateByLine( void* new_line )
	{
      int wdw = widthDWord;
      int32_t* curA;
      int32_t* curB;
      int32_t* curAB;
      int32_t n_cur = nCurLine;


		swap( last16, cur16 );

      curA = (int32_t*)x16+wdw*n_cur;
      memmove( (void*)curA, new_line, wdw*4 );
      curB = (n_cur & (int32_t)1) ?
             (int32_t*)x16+wdw*(n_cur - 1):    // odd - get previous
             (int32_t*)x16+wdw*(n_cur + 1);    // even - get next
		n_cur >>= 1;
      curAB = (int32_t*)x8 + wdw*n_cur;
      A_B_to_AB();

      curA = (int32_t*)x8+wdw*n_cur;
      curB = (n_cur & (int32_t)1) ?
             (int32_t*)x8+wdw*(n_cur - 1):    // odd - get previous
             (int32_t*)x8+wdw*(n_cur + 1);    // even - get next
		n_cur >>= 1;
      curAB = (int32_t*)x4 + wdw*n_cur;
      A_B_to_AB();

      curA = (int32_t*)x4+wdw*n_cur;
      curB = (n_cur & (int32_t)1) ?
             (int32_t*)x4+wdw*(n_cur - 1):    // odd - get previous
             (int32_t*)x4+wdw*(n_cur + 1);    // even - get next
		n_cur >>= 1;
      curAB = (int32_t*)x2 + wdw*n_cur;
      A_B_to_AB();

      curA = (int32_t*)x2+wdw*n_cur;
      curB = (n_cur & (int32_t)1) ?
             (int32_t*)x2+wdw*(n_cur - 1):    // odd - get previous
             (int32_t*)x2+wdw*(n_cur + 1);    // even - get next
		n_cur >>= 1;
		assert( n_cur == 0 );
      curAB = (int32_t*)cur16;           // + wdw*n_cur;
      A_B_to_AB();

		nCurLine++;
		if (nCurLine > 15)
			nCurLine = 0;
	}
