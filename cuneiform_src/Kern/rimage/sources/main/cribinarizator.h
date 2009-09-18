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

// CRIBinarizator.h: interface for the CRIBinarizator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined( _RIIMAGE_BINARIZATOR_HEADER_)
#define _RIIMAGE_BINARIZATOR_HEADER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////
#include "globus.h"
#include "ctdib.h"	// Added by ClassView
#include "rprogressor.h"	// Added by ClassView
#include "cdezabinarizator.h"
//#include "CKronrodBinarizator.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef short int (* CRDezaImageRead)(unsigned char *, short, short);
///////////////////////////////////////////////////////////////////////////////////////////////////
//
enum CTBinarize
{
	CTBIN_UNKNOWN,
	CTBIN_DEZA,
	CTBIN_KRONROD
};
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class CRIBinarizator
{
private:

	Word8          wIndex4ToGray[16];
	Word8          wIndex8ToGray[256];
	Bool32          mbIndexColor;
	Word32     mwGreenK;
	Word32     mwRedK;
	Word32     mwBlueK;
	Float32    mfGreenK;
	Float32    mfRedK;
	Float32    mfBlueK;

public:
	Bool32 Binarize(CTBinarize eBinType, Word32 wFlag);
	Bool32 SetRasters(PCTDIB pSrcDIB, PCTDIB pDescDIB );
	CRIBinarizator();
	CRIBinarizator( PCRProgressor pProgressIndicator);
	virtual ~CRIBinarizator();

private:
	Bool32 DezaCloseBin(void);
	Bool32 DezaOpenBin(Word32 wDezaFlag);
	Bool32 KronrodCloseGray(void);
	Bool32 KronrodGreyFrom(PWord8 pGFrom);
	Bool32 KronrodGreyTo(PWord8 pGTo);

public:
	Int16  KronrodImageRead(PWord8, Int16 fstLine, Int16 nLines);
	Int32  KronrodImageRead(PWord8, Int32 fstLine, Int32 nLines);

private:
	Bool32 OnBinarizeLoop();
	Bool32 KronrodOpenBin(Word32 wHeight, Word32 wWeidth);
	Bool32 OnBinarizator( );
	Bool32 CloseBinarizator();
	Bool32 OpenBinarizator(Word32 wFlag);
	Word8           IndexPalleteToGray(PCTDIBRGBQUAD pQuad);
	Bool32          PrepareIndexTable(PCTDIB pDIB);
	Bool32          SupportedIndexColorImage(PCTDIB pImage);

private:
	Word32                 mwGreyBufferSize;
	PCTDIB                 mpOutcomeDIB;
	PCTDIB                 mpIncomeDIB;
	CTBinarize             meBinType;
	Word32                 mwSrcBitCount;
	PWord8                 mpSrcBuffer;
	Word32                 mwLineLenght;
	Handle                 mhszGreyBuffer;
	PWord8                 mpszGreyBuffer;
	Bool32                 mbKronrodInvert;
	CTBINTigerImageInfo    mDezaImageInfo;
	PCDezaBinarizator      mpDezaBinarizator;
	//PCKronrodBinarizator   mpKronrodBinarizator;
protected:
	PCRProgressor mpProgressor;
};

typedef CRIBinarizator  *PCRIBinarizator, **PPCRIBinarizator;

#endif // !defined(_RIIMAGE_BINARIZATOR_HEADER_)
