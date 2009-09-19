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

// СRTurner.h: interface for the CRTurner class.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#if !defined(CRTURNER_H__)
#define CRTURNER_H__
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "globus.h"
#include "ctdib.h"

#define CONTINUEPIXEL(a)         if ( !(a) ) continue;
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class CRTurner
{
public:
	Bool32    TurnDIB(PCTDIB pInDIB, PCTDIB pOutDIB, uint32_t wAngle);
	Bool32    FreeDIB(void * pDIB);
	void *    TurnDIB( void * pDIB, uint32_t wAngle);
	CRTurner();
	virtual   ~CRTurner();

private:
	Bool32    WriteDIBtoBMP(const char *cName, PCTDIB pDIB);
	Bool32    Turn270GC(PCTDIB pIn, PCTDIB pOut);
	Bool32    Turn180GC(PCTDIB pIn, PCTDIB pOut);
	Bool32    Turn90GC(PCTDIB pIn, PCTDIB pOut);
	Bool32    Turn270LA(PCTDIB pInDIB, PCTDIB pOutDIB);
	Bool32    Turn90LA(PCTDIB pInDIB, PCTDIB pOutDIB);
	Bool32    CheckInAndOut180(PCTDIB pIn, PCTDIB pOut);
	Bool32    CheckInAndOut90(PCTDIB pIn, PCTDIB pOut);
	void      FreeBuffers(void);
	Bool32    Turn270(PCTDIB pInDIB, PCTDIB pOutDIB);
	Bool32    Turn180(PCTDIB pInDIB, PCTDIB pOutDIB);
	Bool32    Turn90(PCTDIB pInDIB, PCTDIB pOutDIB);
	Bool32    Turn180LA(PCTDIB pInDIB, PCTDIB pOutDIB);

protected:
	uint32_t    wRightShift[8];// { 7, 6, 5, 4, 3, 2, 1, 0};
	uchar     wRightMask[8]; // { 11111110b, 11111100b, 11111000b, 11110000b, 11100000b, 11000000b, 10000000b, 00000000b };
	uchar     wLeftMask[8];  // { 11111110b, 11111100b, 11111000b, 11110000b, 11100000b, 11000000b, 10000000b, 00000000b };
	uchar     wBitMask[8];  //  { 10000000b, 01000000b, 00100000b, 00010000b, 00001000b, 00000100b, 00000010b, 00000001b };
	Handle    hLineBuffer;
	puchar    LineBuffer;
	uchar     Turn1at180[256];
	uchar     Turn1at90[132][8];
};

typedef  CRTurner   *PCRTurner,  **PPCRTurner;
#endif // !defined(CRTURNER_H__)
