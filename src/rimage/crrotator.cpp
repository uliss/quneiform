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

// CRRotator.cpp: implementation of the CRRotator class.
//
//////////////////////////////////////////////////////////////////////
#include <memory.h>
#include <math.h>
#include <stdlib.h>

#include "ctdib.h"
#include "crrotator.h"
#include "crimemory.h"
#include "resource.h"
#include "minmax.h"

//////////////////////////////////////////////////////////////////////

static uint32_t BitsInHalfByte[16] =
{//0x0  0x1  0x2  0x3  0x4  0x5  0x6  0x7  0x8  0x9  0xa  0xb  0xc  x0d  0xe  0xf
	0,   1,   1,   2,   1,   2,   2,   3,   1,   2,   2,   3,   2,   3,   3,   4   };

	static uint32_t HalfBytetoFourBytes[16] =
{// 0x0         0x01        0x02        0x03
	0x00000000, 0x000000ff, 0x0000ff00, 0x0000ffff,
 // 0x04        0x05        0x06        0x07
	0x00ff0000, 0x00ff00ff, 0x00ffff00, 0x00ffffff,
 //	0x08        0x09        0x0a        0x0b
	0xff000000, 0xff0000ff, 0xff00ff00, 0xff00ffff,
 //	0x0c        0x0d        0x0e        0x0f
	0xffff0000, 0xffff00ff, 0xffffff00, 0xffffffff,
};
static uchar FourBytestoLeftHalfByte[16] =
{ 0x00, 0x10, 0x20, 0x30,
  0x40, 0x50, 0x60, 0x70,
  0x80, 0x90, 0xa0, 0xb0,
  0xc0, 0xd0, 0xe0, 0xf0
};
static uchar FourBytestoRightHalfByte[16] =
{ 0x00, 0x01, 0x02, 0x03,
  0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b,
  0x0c, 0x0d, 0x0e, 0x0f
};
static uchar MaskForRightShift[9] =
{
//  00000000   00000001   00000011   00000111   00001111
	0x00,      0x01,      0x03,      0x07,      0xf,
//  00011111   00111111   01111111   11111111
	0x1f,      0x3f,      0x7f,      0xff
};
static uchar MaskForLeftShift[9] =
{
//  00000000   10000000   11000000   11100000   11110000
	0x00,      0x80,      0xc0,      0xe0,      0xf0,
//  11111000   11111100   11111110   11111111
	0xf8,      0xfc,      0xfe,      0xff
};
static uchar OneBitMask[8] =
{
//  1-й вариант
//  00000001   00000010   00000100   00001000
//	0x01,      0x02,      0x04,      0x08,
//  00010000   00100000   01000000   10000000
//	0x10,      0x20,      0x40,      0x80
//  2-й вариант
//  10000000   01000000   00100000   00010000
	0x80,      0x40,      0x20,      0x10,
//  00001000   00000100   00000010   00000001
	0x08,      0x04,      0x02,      0x01
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//

void CRRotator::ResetItself()
{
	AtX              = 0;
	AtY              = 0;
	ToX              = 0;
	ToY              = 0;
	mfCos            = 0.0;
	mfSin            = 0.0;
	mfTang           = 0.0;
	mwDescewedHeight = 0;
	mwDescewedWidth  = 0;
	mwOriginalHeight = 0;
	mwOriginalWidth  = 0;
	mwTablesRange    = 0;
	mbEnableHS       = TRUE;
	mbEnableVS       = TRUE;
	mbEnableVSS      = TRUE;
	mpProgressor     = NULL;
	BufferForGray    = NULL;
	hBufferForGray   = NULL;
	HShiftTable      = NULL;
	hHShiftTable     = NULL;
	VShiftTable      = NULL;
	hVShiftTable     = NULL;
	VShiftLenght     = NULL;
	hVShiftLenght    = NULL;

}

CRRotator::CRRotator()
{
	ResetItself();
}
//////////////////////////////////////////////////////////////////////
//
CRRotator::CRRotator(PCRProgressor pProgressIndicator)
{
	ResetItself();
	mpProgressor = pProgressIndicator;
}
///////////////////////////////////////////////////////////////////////////////////////
//
CRRotator::~CRRotator()
{
	FreeWorkBuffers();
}
///////////////////////////////////////////////////////////////////////////////////////
//

Bool32 CRRotator::Rotate(PCTDIB cDIBIn, PCTDIB cDIBOut, int32_t Num, int32_t Denum)
{
	Bool32  Ret = FALSE;
	long double   fNum    = (long double)Num;
	long double   fDenum  = (long double)Denum;
	long double   fG      = sqrt( fNum * fNum + fDenum * fDenum );

	if ( Num == 0 || Denum == 0)
	{
		SetReturnCode_rimage(IDS_RIMAGE_ZERO_NUMERATOR_OR_DENUMERATOR);
		return FALSE;
	}

	/*
	if ( cDIBIn->GetPixelSize() == 4 )
	{
		SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
		return FALSE;
	}
	*/

	mfTang=  fNum / fDenum;
	mfCos =  fDenum / fG;
	mfSin =  fNum / fG;

	if (  fabs(mfTang) > CTROTATOR_MAX_TANG )
	{
		SetReturnCode_rimage(IDS_RIMAGE_ANGLE_GREAT_MAXIMUM);
		return FALSE;
	}

	if ( fabs(mfTang) < CTROTATOR_MIN_TANG )
	{
		SetReturnCode_rimage(IDS_RIMAGE_ANGLE_LEAST_MINIMUM);
		return FALSE;
	}

	if ( !ConstructOutDIB(cDIBIn, cDIBOut, FALSE) )
		return FALSE;
	try
	{
		FreeWorkBuffers();
		Ret = Descew(cDIBIn, cDIBOut);
	}

	catch ( uint32_t wExit )
	{
		if ( wExit = IDS_RIMAGE_EXIT_BY_USER)
		{
			SetReturnCode_rimage(IDS_RIMAGE_EXIT_BY_USER);
		}
		else
		{
			SetReturnCode_rimage(IDS_RIMAGE_UNKNOWN_ERROR);
		}

		FreeWorkBuffers();
		Ret =  FALSE;
	}

	return Ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
Bool32 CRRotator::Descew(PCTDIB cDIBIn, PCTDIB cDIBOut )
{
	# ifndef max
	#    define     MAX(a,b)             ( (a) > (b) ? (a) : (b) )
	# endif
	Bool32 Ret = TRUE;
	uint32_t Line;
	uint32_t wBitLenght = cDIBIn->GetPixelSize();
//	uint32_t wLines = cDIBIn->GetLinesNumber();
//	uint32_t wLineW = cDIBIn->GetLineWidth();
	uint32_t wLineBytesW   = cDIBIn->GetLineWidthInBytes();
	uint32_t wLineBytesWTo = cDIBOut->GetLineWidthInBytes();
	uint32_t HorisontalShiftInBytes = (((wBitLenght * HorisontalShift) + 7) / 8);
	int32_t  Direction = (mfTang > 0 ? 1 : -1);
	int32_t  ToShift;
	puchar pToCopy;

	mwOriginalHeight = cDIBIn->GetLinesNumber();
	mwOriginalWidth  = cDIBIn->GetLineWidth();
	mwDescewedHeight = cDIBOut->GetLinesNumber();
	mwDescewedWidth  = cDIBOut->GetLineWidth();
	mwTablesRange    = MAX (mwDescewedHeight, mwDescewedWidth);


	AllocWorkBuffers(TRUE);
	PrepareTables(mwTablesRange, mwDescewedWidth - 1);

	/////////////
	//начинаем сдвигать.
	mpProgressor->Start();

	for ( Line = 0; Line < mwDescewedHeight; Line++)
	{

		if ( mpProgressor->SetStep( (Line / mwDescewedHeight) * 100 ) )
		{
			mpProgressor->Finish();
			throw IDS_RIMAGE_EXIT_BY_USER;
		}

		memset(BufferForGray, BitFillforGray, HorisontalShiftInBytes);
		memset(BufferForGray + wLineBytesWTo - HorisontalShiftInBytes, BitFillforGray, HorisontalShiftInBytes);

		switch ( wBitLenght )
		{
		case 8:
		case 24:
		case 32:
			ComposeLineG(cDIBIn, Direction, Line, mwDescewedWidth);
			break;
		case 1:
			//// состовляем строку с учетом сдвигов
			// внимание! строка составляется неточно!!!!!
			ComposeLineLA(cDIBIn, Direction, Line, mwDescewedWidth);
			//забиваем конец строки
			SetEndLineLA(Line, mwOriginalWidth, wLineBytesWTo);
			//// сдвигаем по горизонтали
			ToShift = HShiftTable[Line]%8;             //(int32_t)( (i * dTan) + 0.5) % 8

			if ( ToShift != 0 )
			{
				ShiftLineLA(ToShift, wLineBytesWTo + HorisontalShiftInBytes - 1);
			}
			break;
		case 4:
			ComposeLine4(cDIBIn, Direction, Line, mwDescewedWidth);
		default:
			break;
		}
			// копируем строку в конечный диб
		pToCopy = (puchar)(Direction > 0 ? cDIBOut->GetPtrToLine(Line) :
		                                   cDIBOut->GetPtrToLine(mwDescewedHeight - Line - 1));
		if (pToCopy )
		{
			memcpy(pToCopy, BufferForGray, wLineBytesWTo);
		}
	}

	mpProgressor->Finish();

	return Ret;
}

void CRRotator::ComposeLineLA(PCTDIB cDIBIn, int32_t iDirect, uint32_t wLine, uint32_t wBuffLen)
{
	uint32_t j;
	uint32_t StartByte;
	uint32_t EndByte;
	uint32_t FulBytes;
	uint32_t Bits;
	int32_t  PosInBuff;
	uint32_t Copy = 0;
	puchar PixFrom;
	uchar  LastByte;
	uint32_t wLines = cDIBIn->GetLinesNumber();

	for ( j = 0; j < wBuffLen; )
	{
		StartByte = j / 8; //откуда начнем
//		EndByte   = (j + VShiftLenght[j])/8; //последний копируемый
		EndByte   = (j - 1 + VShiftLenght[j])/8; //последний копируемый

		FulBytes = EndByte - StartByte + 1;
//		Bits     = (j + VShiftLenght[j]) - EndByte * 8;
		Bits     = j - StartByte * 8;

		if ( VShiftLenght[j] == 0 )
		{
			j++;
			continue;
		}

		// если есть строка исходной картинки
		PixFrom = NULL;
		PosInBuff = ( HShiftTable[wLine]) / 8 + StartByte;
		if ( wLine + VShiftTable[j] >= AtY && wLine + VShiftTable[j] < ToY )
		{
			if ( iDirect > 0 )
			{
				PixFrom = (puchar)cDIBIn->GetPtrToPixel(j,wLine + VShiftTable[j] - AtY);
			}
			else
			{
				PixFrom = (puchar)cDIBIn->GetPtrToPixel(j,wLines - (wLine + VShiftTable[j] - AtY));
			}
		}
		//перед копированием дальше запоминаем первый байт
		if ( j != 0  && Bits > 0 && mbEnableVSS)
		{
			LastByte = 0x00;
			LastByte |= *(BufferForGray + PosInBuff)&MaskForLeftShift[Bits];
		}

		if ( PixFrom )
		{
			// копируем следующий кусок
			if (EndByte > cDIBIn->GetUsedLineWidthInBytes() - 1)
			{
				EndByte = cDIBIn->GetUsedLineWidthInBytes() - 1;
				FulBytes = EndByte - StartByte + 1;
			}

			memcpy(BufferForGray + PosInBuff,PixFrom, FulBytes );
		}
		else  //  if ( pixfrom )
		{
			memset(BufferForGray + PosInBuff,BitFillforGray, FulBytes );
		}
		//на стыке совмещаем побитно
		if ( Bits > 0 && j != 0 && mbEnableVSS)
		{
			LastByte |= *(BufferForGray + PosInBuff)&MaskForRightShift[8 - Bits];
			*(BufferForGray + PosInBuff) = LastByte;
		}

		Copy += FulBytes * 8;
		j+= VShiftLenght[j];
	}
}

void CRRotator::ComposeLine4(PCTDIB cDIBIn, int32_t iDirect, uint32_t wLine, uint32_t wBuffLen)
{
	uint32_t j;
	uint32_t StartByte;
	uint32_t EndByte;
	uint32_t FulBytes;
	uint32_t Pixels;
	int32_t  PosInBuff;
	uint32_t Copy = 0;
	puchar PixFrom;
	uchar  LastByte;
	uint32_t wLines = cDIBIn->GetLinesNumber();

	for ( j = 0; j < wBuffLen; )
	{
		StartByte = j / 2; //откуда начнем
		EndByte   = (j + VShiftLenght[j])/2; //последний копируемый

		FulBytes = EndByte - StartByte + 1;
		Pixels   = (j + VShiftLenght[j]) - EndByte * 2;


		if ( VShiftLenght[j] == 0 )
		{
			j++;
			continue;
		}

		// если есть строка исходной картинки
		PixFrom = NULL;
		PosInBuff = ( HShiftTable[wLine]) / 2 + StartByte;

		if ( wLine + VShiftTable[j] >= AtY && wLine + VShiftTable[j] < ToY )
		{
			if ( iDirect > 0 )
			{
				PixFrom = (puchar)cDIBIn->GetPtrToPixel(j,wLine + VShiftTable[j] - AtY);
			}
			else
			{
				PixFrom = (puchar)cDIBIn->GetPtrToPixel(j,wLines - (wLine + VShiftTable[j] - AtY));
			}
		}
		//перед копированием дальше запоминаем первый байт
		if ( j != 0  && Pixels > 0 && mbEnableVSS)
		{
			LastByte = 0x00;
			LastByte |= *(BufferForGray + PosInBuff)&MaskForLeftShift[Pixels];
		}

		if ( PixFrom )
		{
			// копируем следующий кусок
			memcpy(BufferForGray + PosInBuff,PixFrom, FulBytes );
		}
		else  //  if ( pixfrom )
		{
			memset(BufferForGray + PosInBuff,BitFillforGray, FulBytes );
		}
		//на стыке совмещаем побитно
		if ( Pixels > 0 && j != 0 && mbEnableVSS)
		{
			LastByte |= *(BufferForGray + PosInBuff)&MaskForRightShift[8 - Pixels];
			*(BufferForGray + PosInBuff) = LastByte;
		}

		Copy += FulBytes * 2;
		j+= VShiftLenght[j];
	}
}

void CRRotator::SetEndLineLA(uint32_t Line, uint32_t wLineW, uint32_t wLineBytes)
{
	uint32_t StartByte = wLineW/8 + HShiftTable[Line]/8;
	uint32_t EndByte   = wLineBytes;
	uint32_t Bits      = wLineW + (HShiftTable[Line]/8)*8 - StartByte*8;
	uchar  LastByte;

	if ( Bits > 0 )
	{
		LastByte = 0x00;
		LastByte  |= *(BufferForGray + StartByte)&MaskForLeftShift[Bits];
		LastByte  |= BitFillforGray&MaskForRightShift[8 - Bits];
	}

	if ( (int32_t)(EndByte - StartByte) > 0)
		memset(BufferForGray + StartByte, BitFillforGray, (EndByte - StartByte));

	if ( Bits > 0 )
		*(BufferForGray + StartByte) = LastByte;
}

void CRRotator::PrepareTables( uint32_t wMax, uint32_t wCheck)
{
	uint32_t i;
	int32_t  Shift;
	int32_t  PrevS;
	int32_t  PrevI;
	long double dmTan = (mfTang > 0 ? mfTang : -mfTang);

	for (PrevS = PrevI = i = 0; i < wMax; i++ )
	{
		Shift = (int32_t)( (i * dmTan) + 0.5) ;

		if ( i < AtY )
			HShiftTable[i] = Shift;
		else
			Shift = (Shift < 0 ? 0 : Shift);

		HShiftTable[i] = Shift /*( Shift < HorisontalShift ? Shift : HorisontalShift )*/;

		VShiftTable[i] = ( Shift < VerticalShift ? Shift : VerticalShift );

		VShiftLenght[i] = 0;
		if ( PrevS != Shift || i == wCheck)
		{
			VShiftLenght[PrevI] = i - PrevI;
			PrevI = i;
		}

		PrevS = Shift;
	}
}

void CRRotator::ShiftLineLA(uint32_t LocalShift, uint32_t wBytesTo )
{
	uint32_t LocalUnshift = 8 - LocalShift;
	uint32_t ShiftedByte;

	for ( ShiftedByte = wBytesTo;
		  ShiftedByte > 0;
		  ShiftedByte-- )
	{
		BufferForGray[ShiftedByte] = BufferForGray[ShiftedByte]>>LocalShift;
		BufferForGray[ShiftedByte] &= MaskForRightShift[LocalUnshift];
		BufferForGray[ShiftedByte] |= ( BufferForGray[ShiftedByte - 1]<<LocalUnshift )&MaskForLeftShift[LocalShift];
	}

	BufferForGray[0] |= BitFillforGray&MaskForLeftShift[LocalShift];


}
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
Bool32 CRRotator::Roll(PCTDIB cDIBIn, PCTDIB cDIBOut, int32_t Num, int32_t Denum)
{
	long double   fNum    = (long double)Num;
	long double   fDenum  = (long double)Denum;
	long double   fG      = sqrt( fNum * fNum + fDenum * fDenum );

	mfTang =  fNum / fDenum;
	mfCos  =  fDenum / fG;
	mfSin  =  fNum / fG;
	mdDIBOut = cDIBOut;

	/*
	if ( mfTang > CTROTATOR_MAX_TANG || mfTang < CTROTATOR_MIN_TANG )
		return FALSE;
	*/

	if ( !ConstructOutDIB(cDIBIn, cDIBOut, TRUE) )
		return FALSE;

	if ( !DescewGray( cDIBIn, cDIBOut) )
	{
		return FALSE;
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
Bool32 CRRotator::ConstructOutDIB(PCTDIB cDIBIn, PCTDIB cDIBOut, Bool32 Gray)
{
	uint32_t  NewWidth;
	uint32_t  NewHeigth;
	uint32_t  dpmX;
	uint32_t  dpmY;
	CTDIBRGBQUAD  Quad;

	if ( Gray )
	{
		HorisontalShift =  (int32_t)((cDIBIn->GetLinesNumber() * mfSin) + 0.5);
		HorisontalShift -= (int32_t)((cDIBIn->GetLineWidth() * (1.0 - mfCos))   + 0.5);
		VerticalShift   =  (int32_t)((cDIBIn->GetLineWidth() * mfSin)   + 0.5);
		VerticalShift   -= (int32_t)((cDIBIn->GetLinesNumber() * (1.0 - mfCos)) + 0.5);

		NewHeigth = (uint32_t)((cDIBIn->GetLineWidth() * mfSin)   + 0.5 ) +
			        (uint32_t)((cDIBIn->GetLinesNumber() * mfCos) + 0.5 );
		NewWidth  = (uint32_t)((cDIBIn->GetLineWidth() * mfCos)   + 0.5 ) +
			        (uint32_t)((cDIBIn->GetLinesNumber() * mfSin) + 0.5 );

		if ( mfTang > 0 )
		{
			AtX             = 0;
			AtY             = (int32_t)(cDIBIn->GetLineWidth() * mfSin); //VerticalShift;
		}
		else
		{
			AtX             = (int32_t)(cDIBIn->GetLinesNumber() * mfSin); //HorisontalShift;
			AtY             = 0;
		}
		ToX             = NewWidth;
		ToY             = NewHeigth;
	}
	else
	{
		HorisontalShift = (uint32_t)((cDIBIn->GetLinesNumber() * fabs(mfTang)) + 0.5);
		VerticalShift   = (uint32_t)((cDIBIn->GetLineWidth() * fabs(mfTang)) + 0.5);

		AtX             = 0;
		AtY             = VerticalShift;
		ToX             = AtX + cDIBIn->GetLineWidth();
		ToY             = AtY + cDIBIn->GetLinesNumber();

		NewWidth  = cDIBIn->GetLineWidth() + HorisontalShift;
		NewHeigth = cDIBIn->GetLinesNumber() + VerticalShift;
	}

	RIMAGEComment("ConstructOutDIB - out DIB");
	if ( !cDIBOut->SetExternals(RIMAGEAlloc, RIMAGEFree, RIMAGELock, RIMAGEUnlock) ||
		 !cDIBOut->CreateDIBBegin(NewWidth,
		                          NewHeigth,
								  Gray ? 8 : cDIBIn->GetPixelSize(),
								  0,
								  cDIBIn->GetVersion()) )
		return FALSE;

	cDIBIn->GetResolutionDPM( &dpmX, &dpmY );
	cDIBOut->SetResolutionDPM( dpmX, dpmY );

	cDIBOut->CreateDIBEnd();

	cDIBOut->CopyPalleteFromDIB(cDIBIn);

	BitFillforGray   = (uchar)cDIBIn->GetWhitePixel();

	//для однобитного изображения растягиваем его на весь байт
	if ( cDIBOut->GetPixelSize() == 1 )
		BitFillforGray += ( BitFillforGray << 1 ) +
		                  ( BitFillforGray << 2 ) +
						  ( BitFillforGray << 3 ) +
						  ( BitFillforGray << 4 ) +
		                  ( BitFillforGray << 5 ) +
						  ( BitFillforGray << 6 ) +
						  ( BitFillforGray << 7 );

	if ( Gray )
	{
		int32_t wQuadN;

		for ( wQuadN = 255; wQuadN >= 0; wQuadN--)
		{
			Quad.rgbBlue = Quad.rgbGreen = Quad.rgbRed = (uchar)wQuadN;
			cDIBOut->SetRGBQuad(wQuadN,Quad);
		}
	}

	return TRUE;
}

Bool32 CRRotator::DescewGray(PCTDIB cDIBIn, PCTDIB cDIBOut)
{
	Bool32 bRet = TRUE;
	// размеры
	uint32_t         OutLineLenght = cDIBOut->GetLineWidth();
	uint32_t         OunLines      = cDIBOut->GetLinesNumber();
	uint32_t         InLineLenght  = cDIBIn->GetLineWidth();
	uint32_t         InLines       = cDIBIn->GetLinesNumber();
	//координаты
	int32_t          CurrLAX;
	int32_t          CurrLAY;
	int32_t          CurrGX;
	int32_t          CurrGY;
	int32_t          OldPointX;
	int32_t          OldPointY;
	int32_t          NewPointX;
	int32_t          NewPointY;
	int32_t          GShiftX;
	int32_t          GShiftY;
	// пикселы
	puchar         pLAPix = NULL;
	puchar         pGrayPix = NULL;
	puchar         pGrayLine = NULL;
	uint32_t         LABitInByte;
	uchar          Decrement;
	uchar          Corner = 1;
	// маска
	Handle         hMask = NULL;
	puchar         pMask = NULL;

	hMask = RIMAGEDAlloc(OutLineLenght * OunLines, "Rotator::DescewGray - mask");
	pMask = (puchar)RIMAGELock(hMask);


	if ( cDIBOut->GetPixelSize() != 8 )
	{
		return FALSE;
	}

	if ( pMask )
	{
		for ( CurrGY = OunLines - 1; CurrGY >= 0; CurrGY-- )
		{
			pGrayLine = (puchar)cDIBOut->GetPtrToLine(CurrGY);
			memset(pGrayLine, 127, OutLineLenght);
			memset((pMask + (CurrGY * OutLineLenght)), 1, OutLineLenght);
		}
	}
	else
	{
		for ( CurrGY = OunLines - 1; CurrGY >= 0; CurrGY-- )
		{
			pGrayLine = (puchar)cDIBOut->GetPtrToLine(CurrGY);
			memset(pGrayLine, 255, OutLineLenght);
		}
	}

	for ( CurrLAY = 0; CurrLAY < (int32_t)InLines; CurrLAY++)
	{
		for ( CurrLAX = 0; CurrLAX < (int32_t)InLineLenght; CurrLAX++)
		{
			GetExtCoord(CurrLAX, CurrLAY, &OldPointX, &OldPointY);
			RollPoint(OldPointX, OldPointY, &NewPointX, &NewPointY);

			NewPointX += AtX * 256;
			NewPointY += AtY * 256;

			GetGrayCoord(NewPointX, NewPointY, &CurrGX, &CurrGY, &GShiftX, &GShiftY);

			//CurrGX += AtX;
			//CurrGY += AtY;

			if ( CurrGX < (int32_t)OutLineLenght &&
				 CurrGY < (int32_t)OunLines)
			{
				pGrayPix = (puchar)cDIBOut->GetPtrToPixel(CurrGX, CurrGY);

				if ( !pGrayPix )
					continue;
			}
			else
				continue;

			if ( pMask )
			{
				UnmaskPixels(pMask, pGrayPix, CurrGX, CurrGY, OutLineLenght, GShiftX, GShiftY);
			}

			//берем черно-белый пиксел
			if ( CurrLAX < (int32_t)InLineLenght &&
				 CurrLAY < (int32_t)InLines)
			{
				pLAPix = (puchar)cDIBIn->GetPtrToPixel(CurrLAX, CurrLAY);

				if ( !pLAPix )
					continue;

			}
			else
				continue;

			// раскидываем по серому
			LABitInByte = CurrLAX - ((CurrLAX / 8) * 8);
			if ( ((*pLAPix) & OneBitMask[LABitInByte]) == 0 )
			{
				Decrement = ((256 - abs(GShiftX)) * (256 - abs(GShiftY))) / 256;
				AddBlackToGray(CurrGX, CurrGY, Decrement);
				Corner = 1;

				if ( GShiftX != 0)
				{
					Decrement = ( abs(GShiftX) * (256 - abs(GShiftY))) / 256;

					if ( GShiftX < 0 )
					{
						AddBlackToGray(CurrGX - 1, CurrGY, Decrement);
						Corner *= 2;
					}
					else
					{
						AddBlackToGray(CurrGX + 1, CurrGY, Decrement);
						Corner *= 3;
					}
				}

				if (GShiftY != 0)
				{
					Decrement = ( abs(GShiftY) * (256 - abs(GShiftX))) / 256;

					if ( GShiftY < 0 )
					{
						AddBlackToGray(CurrGX , CurrGY - 1, Decrement);
						Corner *= 4;
					}
					else
					{
						AddBlackToGray(CurrGX, CurrGY + 1, Decrement);
						Corner *= 5;
					}

				}

				Decrement = ( abs(GShiftY) * (256 - abs(GShiftX))) / 256;
				switch (Corner)
				{
				case 8:
					AddBlackToGray(CurrGX - 1, CurrGY - 1, Decrement);
					break;
				case 12:
					AddBlackToGray(CurrGX + 1, CurrGY - 1, Decrement);
					break;
				case 15:
					AddBlackToGray(CurrGX + 1, CurrGY + 1, Decrement);
					break;
				case 10:
					AddBlackToGray(CurrGX - 1, CurrGY + 1, Decrement);
					break;
				default:
					pGrayPix = NULL;
				}
			}

		}
	}

	RIMAGEUnlock( hMask );
	RIMAGEFree( hMask );

	return bRet;
}

Bool32 CRRotator::GetExtCoord(int32_t X, int32_t Y, int32_t * peX, int32_t * peY)
{
	if ( peX )
	{
		*peX = 128 + X * 256;
	}

	if ( peY )
	{
		*peY = 128 + Y * 256;
	}

	return TRUE;
}

Bool32 CRRotator::RollPoint(int32_t X, int32_t Y, int32_t * pnX, int32_t * pnY)
{
	if ( pnX )
	{
		*pnX = (int32_t)(((long double)X * mfCos) + ((long double)Y * mfSin));
	}

	if ( pnY )
	{
		*pnY = (int32_t)(((long double)Y * mfCos) - ((long double)X * mfSin));
	}

	return TRUE;
}

Bool32 CRRotator::GetGrayCoord(int32_t eX, int32_t eY, int32_t * pgX, int32_t * pgY, int32_t * psX, int32_t * psY)
{
	if ( pgX )
	{
		*pgX = (int32_t)(eX / 256);

		if ( psX )
			*psX = (int32_t)(eX - (*pgX * 256) - 128);
	}

	if ( pgY )
	{
		*pgY = (int32_t)(eY / 256);

		if ( psY )
			*psY = (int32_t)(eY - (*pgY * 256) - 128);
	}

	return TRUE;
}



Bool32 CRRotator::UnmaskPixels(puchar pMask, puchar pGaryPix, int32_t X, int32_t Y, uint32_t wLineLen, int32_t ShiftX, int32_t ShiftY)
{
	if ( *(pMask + (Y * wLineLen + X)) )
	{
		*(pMask + (Y * wLineLen + X)) = 0;
		*pGaryPix = 255;
	}

	if ( ShiftX > 0 && X < (int32_t)(wLineLen - 1) )
	{
		if ( *(pMask + (Y * wLineLen + X + 1)) )
		{
			*(pMask + (Y * wLineLen + X + 1)) = 0;
			*( pGaryPix + 1 ) = 255;
		}
	}

	if ( ShiftX < 0 && X > 1 )
	{
		if ( *(pMask + (Y * wLineLen + X - 1)) )
		{
			*(pMask + (Y * wLineLen + X - 1)) = 0;
			*( pGaryPix - 1 ) = 255;
		}
	}

	return TRUE;
}

Bool32 CRRotator::AddBlackToGray(int32_t X, int32_t Y, uchar Gray)
{
	static puchar pGrayPixel;

	pGrayPixel = (puchar)mdDIBOut->GetPtrToPixel(X, Y);

	if ( pGrayPixel )
	{
		*pGrayPixel = (Gray > *pGrayPixel ? 0 : *pGrayPixel - Gray);
		return TRUE;
	}

	return FALSE;
}

void CRRotator::AllocWorkBuffers(uint32_t wStringBufferRange)
{
	if (mwTablesRange)
	{
		hHShiftTable   = RIMAGEDAlloc((mwTablesRange + 1)*4,"Rotator::AllocWorkBuffer - hor shift table");
		HShiftTable    = static_cast<int32_t*>(RIMAGELock(hHShiftTable));
		hVShiftTable   = RIMAGEDAlloc((mwTablesRange + 1)*4,"Rotator::AllocWorkBuffer - vert shift table");
		VShiftTable    = static_cast<uint32_t*>(RIMAGELock(hVShiftTable));
		hVShiftLenght  = RIMAGEDAlloc((mwTablesRange + 1)*4,"Rotator::AllocWorkBuffer - vl shift table");
		VShiftLenght   = static_cast<uint32_t*>(RIMAGELock(hVShiftLenght));
	}

	if ( wStringBufferRange )
	{
		//hBufferForGray = RIMAGEAlloc( wLineBytesWTo + HorisontalShiftInBytes * 2 + ( VShiftLenght[0] * 2 )  );     //буфер для конечной картинки
		hBufferForGray = RIMAGEDAlloc( wWorkGrayBufferLenght = ( mwDescewedWidth * 4 ), "Rotator::AllocWorkBuffer - buffer for gray");     //буфер для конечной картинки
		BufferForGray  = (puchar)RIMAGELock(hBufferForGray);
	}

}


void CRRotator::FreeWorkBuffers()
{
	mwTablesRange = 0;

	if ( hHShiftTable )
	{
		RIMAGEFree(hHShiftTable);
		hHShiftTable   = NULL;
		HShiftTable    = NULL;
	}

	if ( hVShiftTable )
	{
		RIMAGEFree(hVShiftTable);
		hVShiftTable   = NULL;
		VShiftTable    = NULL;
	}

	if ( hVShiftLenght )
	{
		RIMAGEFree(hVShiftLenght);
		hVShiftLenght  = NULL;
		VShiftLenght   = NULL;
	}

	if (hBufferForGray)
	{
		RIMAGEFree(hBufferForGray);
		hBufferForGray = NULL;
		BufferForGray  = NULL;
	}
}

Bool32 CRRotator::RotatePoint(int32_t iX, int32_t iY, int32_t * prX, int32_t * prY)
{
	int32_t tY;

	if ( VShiftTable == NULL ||  HShiftTable == NULL)
	{
		return FALSE;
	}

	if ( iX > -1 && iX < (int32_t)mwOriginalWidth &&
		 iY > -1 && iY < (int32_t)mwOriginalHeight    )
	{
		if ( mfTang > 0)
		{
			*prY = iY - VShiftTable[iX] + AtY;

			if ( *prY < 0 || *prY >= (int32_t)mwTablesRange )
			{
				*prX = -1;
				*prY = -1;
				return FALSE;
			}

			*prX = iX + HShiftTable[*prY] + AtX;
			return TRUE;
		}
		else
		{
			tY = mwOriginalHeight - (iY + 1);
			tY = tY - VShiftTable[iX] + AtY;

			if ( tY < 0 || tY >= (int32_t)mwTablesRange )
			{
				*prX = -1;
				*prY = -1;
				return FALSE;
			}

			*prX = iX + HShiftTable[tY] + AtX;
			*prY = mwDescewedHeight - (tY + 2);
			return TRUE;
		}
	}
	return FALSE;
}

void CRRotator::ComposeLineG(PCTDIB cDIBIn, int32_t iDirect, uint32_t wLine, uint32_t wBuffLen)
{
	uint32_t j;
	uint32_t StartByte;
	uint32_t EndByte;
	uint32_t FulBytes;
	uint32_t PixelSize = cDIBIn->GetPixelSize() / 8;
	int32_t  PosInBuff;
	uint32_t Copy = 0;
	puchar PixFrom;
	uint32_t wLines = cDIBIn->GetLinesNumber();

	for ( j = 0; j < wBuffLen; )
	{
		StartByte = j * PixelSize; //откуда начнем
		EndByte   = (j + VShiftLenght[j]) * PixelSize; //последний копируемый

		FulBytes = EndByte - StartByte + 1;

		if ( VShiftLenght[j] == 0 )
		{
			j++;
			continue;
		}

		// если есть строка исходной картинки
		PixFrom = NULL;
		PosInBuff = ( HShiftTable[wLine] * PixelSize) + StartByte;

		if ( wLine + VShiftTable[j] >= AtY && wLine + VShiftTable[j] < ToY )
		{
			if ( iDirect > 0 )
			{
				PixFrom = (puchar)cDIBIn->GetPtrToPixel(j,wLine + VShiftTable[j] - AtY);
			}
			else
			{
				PixFrom = (puchar)cDIBIn->GetPtrToPixel(j,wLines - (wLine + VShiftTable[j] - AtY));
			}
		}
        // Allex  27.03.00
        // слабое место. на некоторых файлах возникают прблемы с отписыванием в BufferForGray - пытается отписать
        // туда больше чем длина файла. возможно гсвязано с размером пиксела в цветном файле.
        if ( PosInBuff < (int32_t)wWorkGrayBufferLenght )
        {
            FulBytes = FulBytes > wWorkGrayBufferLenght - PosInBuff ? wWorkGrayBufferLenght - PosInBuff : FulBytes;

		    if ( PixFrom )
		    {
			    // копируем следующий кусок
			    memcpy(BufferForGray + PosInBuff,PixFrom, FulBytes );
		    }
		    else  //  if ( pixfrom )
		    {
			    memset(BufferForGray + PosInBuff,BitFillforGray, FulBytes );
		    }
        }

		Copy += FulBytes;
		j+= VShiftLenght[j];
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
