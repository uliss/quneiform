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
//	static Word32    LAtoBytes[512];
public:
	Bool32 RotatePoint(Int32 iX, Int32 iY, PInt32 prX, PInt32 prY);
	CRRotator( PCRProgressor pProgressIndicator);
	Bool32 Roll(PCTDIB cDIBIn, PCTDIB cDIBOut, Int32 Num, Int32 Denum);
	Bool32 Rotate(PCTDIB cDIBIn, PCTDIB cDIBOut, Int32 Num, Int32 Denum);
	CRRotator();
	virtual ~CRRotator();

private:
	void ComposeLine4(PCTDIB cDIBIn, Int32 iDirect, Word32 wLine, Word32 wBuffLen);
	void      AllocWorkBuffers(Word32 wStringBufferRange);
	void      ResetItself(void);
	void      FreeWorkBuffers(void);
	Bool32    AddBlackToGray(Int32 X, Int32 Y, Word8 Gray);
	Bool32    UnmaskPixels(PWord8 pMask, PWord8 pGrayPix, Int32 X, Int32 Y, Word32 wLineLen, Int32 ShiftX, Int32 ShiftY);
	Bool32    GetGrayCoord(Int32 eX, Int32 eY, PInt32 pX, PInt32 pY, PInt32 psX, PInt32 psY);
	Bool32    RollPoint(Int32 X, Int32 Y, PInt32 pnX, PInt32 pnY);
	Bool32    GetExtCoord(Int32 X, Int32 Y, PInt32 peX, PInt32 peY);
	Bool32    DescewGray(PCTDIB cDIBIn, PCTDIB cDIBOut);
	Bool32    ConstructOutDIB(PCTDIB cDIBIn, PCTDIB cDIBOut, Bool32 Gray);
	void      ComposeLineLA(PCTDIB cDIBIn, Int32 iDirect, Word32 wLine, Word32 wBuffLen);
	void      ComposeLineG(PCTDIB cDIBIn, Int32 iDirect, Word32 wLine, Word32 wBuffLen);
	void      SetEndLineLA(Word32 Line, Word32 wLineW, Word32 wLineBytes);
	void      PrepareTables( Word32 wMax, Word32 wCheck);
	void      ShiftLineLA(Word32 LocalShift, Word32 wBytesTo);
	Bool32    Descew(PCTDIB cDIBIn, PCTDIB cDIBOut);

protected:
	Word32 wWorkGrayBufferLenght;
	Word32             mwTablesRange;
	Word32             mwDescewedHeight;
	Word32             mwDescewedWidth;
	Word32             mwOriginalHeight;
	Word32             mwOriginalWidth;
	Handle             hBufferForGray;
	Handle             hVShiftLenght;
	Handle             hVShiftTable;
	Handle             hHShiftTable;
	PCRProgressor      mpProgressor;
	PCTDIB             mdDIBOut;
	Bool32             mbEnableVSS;
	Bool32             mbEnableVS;
	Bool32             mbEnableHS;
	Word8              BitFillforGray;
	Word32             ToY;
	Word32             ToX;
	Word32             AtY;
	Word32             AtX;
	Int32              VerticalShift;
	Int32              HorisontalShift;
	Float80            mfCos;
	Float80            mfSin;
	Float80            mfTang;
	PWord8             BufferForGray;
	PInt32             HShiftTable;
	PWord32            VShiftTable;
	PWord32            VShiftLenght;
};

typedef CRRotator  *PCRRotator, **PPCRRotator;
#endif // !defined(CRROTATOR_H)
