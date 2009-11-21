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

// CRRotator.h: interface for the CRRotator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CRROTATOR_H)
#define CRROTATOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ctdib.h"
#include "rprogressor.h"	// Added by ClassView
#include "cttypes.h"	// Added by ClassView

class CRRotator
{
#define   CTROTATOR_MIN_TANG             0.0001
#define   CTROTATOR_MAX_TANG             0.2
//	static uint32_t    LAtoBytes[512];
public:
	Bool32 RotatePoint(int32_t iX, int32_t iY, int32_t * prX, int32_t * prY);
	CRRotator( PCRProgressor pProgressIndicator);
	Bool32 Roll(PCTDIB cDIBIn, PCTDIB cDIBOut, int32_t Num, int32_t Denum);
	Bool32 Rotate(PCTDIB cDIBIn, PCTDIB cDIBOut, int32_t Num, int32_t Denum);
	CRRotator();
	virtual ~CRRotator();

private:
	void ComposeLine4(PCTDIB cDIBIn, int32_t iDirect, uint32_t wLine, uint32_t wBuffLen);
	void      AllocWorkBuffers(uint32_t wStringBufferRange);
	void      ResetItself(void);
	void      FreeWorkBuffers(void);
	Bool32    AddBlackToGray(int32_t X, int32_t Y, uchar Gray);
	Bool32    UnmaskPixels(puchar pMask, puchar pGrayPix, int32_t X, int32_t Y, uint32_t wLineLen, int32_t ShiftX, int32_t ShiftY);
	Bool32    GetGrayCoord(int32_t eX, int32_t eY, int32_t * pX, int32_t * pY, int32_t * psX, int32_t * psY);
	Bool32    RollPoint(int32_t X, int32_t Y, int32_t * pnX, int32_t * pnY);
	Bool32    GetExtCoord(int32_t X, int32_t Y, int32_t * peX, int32_t * peY);
	Bool32    DescewGray(PCTDIB cDIBIn, PCTDIB cDIBOut);
	Bool32    ConstructOutDIB(PCTDIB cDIBIn, PCTDIB cDIBOut, Bool32 Gray);
	void      ComposeLineLA(PCTDIB cDIBIn, int32_t iDirect, uint32_t wLine, uint32_t wBuffLen);
	void      ComposeLineG(PCTDIB cDIBIn, int32_t iDirect, uint32_t wLine, uint32_t wBuffLen);
	void      SetEndLineLA(uint32_t Line, uint32_t wLineW, uint32_t wLineBytes);
	void      PrepareTables( uint32_t wMax, uint32_t wCheck);
	void      ShiftLineLA(uint32_t LocalShift, uint32_t wBytesTo);
	Bool32    Descew(PCTDIB cDIBIn, PCTDIB cDIBOut);

protected:
	uint32_t           wWorkGrayBufferLenght;
	uint32_t           mwTablesRange;
	uint32_t           mwDescewedHeight;
	uint32_t           mwDescewedWidth;
	uint32_t           mwOriginalHeight;
	uint32_t           mwOriginalWidth;
	Handle             hBufferForGray;
	Handle             hVShiftLenght;
	Handle             hVShiftTable;
	Handle             hHShiftTable;
	PCRProgressor      mpProgressor;
	PCTDIB             mdDIBOut;
	Bool32             mbEnableVSS;
	Bool32             mbEnableVS;
	Bool32             mbEnableHS;
	uchar              BitFillforGray;
	uint32_t           ToY;
	uint32_t           ToX;
	uint32_t           AtY;
	uint32_t           AtX;
	int32_t            VerticalShift;
	int32_t            HorisontalShift;
	long double            mfCos;
	long double            mfSin;
	long double            mfTang;
	puchar             BufferForGray;
	int32_t *          HShiftTable;
	uint32_t *         VShiftTable;
	uint32_t *         VShiftLenght;
};

typedef CRRotator  *PCRRotator, **PPCRRotator;
#endif // !defined(CRROTATOR_H)
