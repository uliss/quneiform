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
//////////////////////////////////////////////////////////////////////

static Word32 BitsInHalfByte[16] =
{//0x0  0x1  0x2  0x3  0x4  0x5  0x6  0x7  0x8  0x9  0xa  0xb  0xc  x0d  0xe  0xf
	0,   1,   1,   2,   1,   2,   2,   3,   1,   2,   2,   3,   2,   3,   3,   4   };

	static Word32 HalfBytetoFourBytes[16] =
{// 0x0         0x01        0x02        0x03
	0x00000000, 0x000000ff, 0x0000ff00, 0x0000ffff,
 // 0x04        0x05        0x06        0x07
	0x00ff0000, 0x00ff00ff, 0x00ffff00, 0x00ffffff,
 //	0x08        0x09        0x0a        0x0b
	0xff000000, 0xff0000ff, 0xff00ff00, 0xff00ffff,
 //	0x0c        0x0d        0x0e        0x0f
	0xffff0000, 0xffff00ff, 0xffffff00, 0xffffffff,
};
static Word8 FourBytestoLeftHalfByte[16] =
{ 0x00, 0x10, 0x20, 0x30,
  0x40, 0x50, 0x60, 0x70,
  0x80, 0x90, 0xa0, 0xb0,
  0xc0, 0xd0, 0xe0, 0xf0
};
static Word8 FourBytestoRightHalfByte[16] =
{ 0x00, 0x01, 0x02, 0x03,
  0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b,
  0x0c, 0x0d, 0x0e, 0x0f
};
static Word8 MaskForRightShift[9] =
{
//  00000000   00000001   00000011   00000111   00001111
	0x00,      0x01,      0x03,      0x07,      0xf,
//  00011111   00111111   01111111   11111111
	0x1f,      0x3f,      0x7f,      0xff
};
static Word8 MaskForLeftShift[9] =
{
//  00000000   10000000   11000000   11100000   11110000
	0x00,      0x80,      0xc0,      0xe0,      0xf0,
//  11111000   11111100   11111110   11111111
	0xf8,      0xfc,      0xfe,      0xff
};
static Word8 OneBitMask[8] =
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

Bool32 CRRotator::Rotate(PCTDIB cDIBIn, PCTDIB cDIBOut, Int32 Num, Int32 Denum)
{
	Bool32  Ret = FALSE;
	Float80   fNum    = (Float80)Num;
	Float80   fDenum  = (Float80)Denum;
	Float80   fG      = sqrt( fNum * fNum + fDenum * fDenum );

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

	catch ( Word32 wExit )
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
	Word32 Line;
	Word32 wBitLenght = cDIBIn->GetPixelSize();
//	Word32 wLines = cDIBIn->GetLinesNumber();
//	Word32 wLineW = cDIBIn->GetLineWidth();
	Word32 wLineBytesW   = cDIBIn->GetLineWidthInBytes();
	Word32 wLineBytesWTo = cDIBOut->GetLineWidthInBytes();
	Word32 HorisontalShiftInBytes = (((wBitLenght * HorisontalShift) + 7) / 8);
	Int32  Direction = (mfTang > 0 ? 1 : -1);
	Int32  ToShift;
	PWord8 pToCopy;

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
			ToShift = HShiftTable[Line]%8;             //(Int32)( (i * dTan) + 0.5) % 8

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
		pToCopy = (PWord8)(Direction > 0 ? cDIBOut->GetPtrToLine(Line) :
		                                   cDIBOut->GetPtrToLine(mwDescewedHeight - Line - 1));
		if (pToCopy )
		{
			memcpy(pToCopy, BufferForGray, wLineBytesWTo);
		}
	}

	mpProgressor->Finish();

	return Ret;
}

void CRRotator::ComposeLineLA(PCTDIB cDIBIn, Int32 iDirect, Word32 wLine, Word32 wBuffLen)
{
	Word32 j;
	Word32 StartByte;
	Word32 EndByte;
	Word32 FulBytes;
	Word32 Bits;
	Int32  PosInBuff;
	Word32 Copy = 0;
	PWord8 PixFrom;
	Word8  LastByte;
	Word32 wLines = cDIBIn->GetLinesNumber();

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
				PixFrom = (PWord8)cDIBIn->GetPtrToPixel(j,wLine + VShiftTable[j] - AtY);
			}
			else
			{
				PixFrom = (PWord8)cDIBIn->GetPtrToPixel(j,wLines - (wLine + VShiftTable[j] - AtY));
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

void CRRotator::ComposeLine4(PCTDIB cDIBIn, Int32 iDirect, Word32 wLine, Word32 wBuffLen)
{
	Word32 j;
	Word32 StartByte;
	Word32 EndByte;
	Word32 FulBytes;
	Word32 Pixels;
	Int32  PosInBuff;
	Word32 Copy = 0;
	PWord8 PixFrom;
	Word8  LastByte;
	Word32 wLines = cDIBIn->GetLinesNumber();

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
				PixFrom = (PWord8)cDIBIn->GetPtrToPixel(j,wLine + VShiftTable[j] - AtY);
			}
			else
			{
				PixFrom = (PWord8)cDIBIn->GetPtrToPixel(j,wLines - (wLine + VShiftTable[j] - AtY));
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

void CRRotator::SetEndLineLA(Word32 Line, Word32 wLineW, Word32 wLineBytes)
{
	Word32 StartByte = wLineW/8 + HShiftTable[Line]/8;
	Word32 EndByte   = wLineBytes;
	Word32 Bits      = wLineW + (HShiftTable[Line]/8)*8 - StartByte*8;
	Word8  LastByte;

	if ( Bits > 0 )
	{
		LastByte = 0x00;
		LastByte  |= *(BufferForGray + StartByte)&MaskForLeftShift[Bits];
		LastByte  |= BitFillforGray&MaskForRightShift[8 - Bits];
	}

	if ( (Int32)(EndByte - StartByte) > 0)
		memset(BufferForGray + StartByte, BitFillforGray, (EndByte - StartByte));

	if ( Bits > 0 )
		*(BufferForGray + StartByte) = LastByte;
}

void CRRotator::PrepareTables( Word32 wMax, Word32 wCheck)
{
	Word32 i;
	Int32  Shift;
	Int32  PrevS;
	Int32  PrevI;
	Float80 dmTan = (mfTang > 0 ? mfTang : -mfTang);

	for (PrevS = PrevI = i = 0; i < wMax; i++ )
	{
		Shift = (Int32)( (i * dmTan) + 0.5) ;

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

void CRRotator::ShiftLineLA(Word32 LocalShift, Word32 wBytesTo )
{
	Word32 LocalUnshift = 8 - LocalShift;
	Word32 ShiftedByte;

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
Bool32 CRRotator::Roll(PCTDIB cDIBIn, PCTDIB cDIBOut, Int32 Num, Int32 Denum)
{
	Float80   fNum    = (Float80)Num;
	Float80   fDenum  = (Float80)Denum;
	Float80   fG      = sqrt( fNum * fNum + fDenum * fDenum );

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
	Word32  NewWidth;
	Word32  NewHeigth;
	Word32  dpmX;
	Word32  dpmY;
	CTDIBRGBQUAD  Quad;

	if ( Gray )
	{
		HorisontalShift =  (Int32)((cDIBIn->GetLinesNumber() * mfSin) + 0.5);
		HorisontalShift -= (Int32)((cDIBIn->GetLineWidth() * (1.0 - mfCos))   + 0.5);
		VerticalShift   =  (Int32)((cDIBIn->GetLineWidth() * mfSin)   + 0.5);
		VerticalShift   -= (Int32)((cDIBIn->GetLinesNumber() * (1.0 - mfCos)) + 0.5);

		NewHeigth = (Word32)((cDIBIn->GetLineWidth() * mfSin)   + 0.5 ) +
			        (Word32)((cDIBIn->GetLinesNumber() * mfCos) + 0.5 );
		NewWidth  = (Word32)((cDIBIn->GetLineWidth() * mfCos)   + 0.5 ) +
			        (Word32)((cDIBIn->GetLinesNumber() * mfSin) + 0.5 );

		if ( mfTang > 0 )
		{
			AtX             = 0;
			AtY             = (Int32)(cDIBIn->GetLineWidth() * mfSin); //VerticalShift;
		}
		else
		{
			AtX             = (Int32)(cDIBIn->GetLinesNumber() * mfSin); //HorisontalShift;
			AtY             = 0;
		}
		ToX             = NewWidth;
		ToY             = NewHeigth;
	}
	else
	{
		HorisontalShift = (Word32)((cDIBIn->GetLinesNumber() * fabs(mfTang)) + 0.5);
		VerticalShift   = (Word32)((cDIBIn->GetLineWidth() * fabs(mfTang)) + 0.5);

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

	BitFillforGray   = (Word8)cDIBIn->GetWhitePixel();

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
		Int32 wQuadN;

		for ( wQuadN = 255; wQuadN >= 0; wQuadN--)
		{
			Quad.rgbBlue = Quad.rgbGreen = Quad.rgbRed = (Word8)wQuadN;
			cDIBOut->SetRGBQuad(wQuadN,Quad);
		}
	}

	return TRUE;
}

Bool32 CRRotator::DescewGray(PCTDIB cDIBIn, PCTDIB cDIBOut)
{
	Bool32 bRet = TRUE;
	// размеры
	Word32         OutLineLenght = cDIBOut->GetLineWidth();
	Word32         OunLines      = cDIBOut->GetLinesNumber();
	Word32         InLineLenght  = cDIBIn->GetLineWidth();
	Word32         InLines       = cDIBIn->GetLinesNumber();
	//координаты
	Int32          CurrLAX;
	Int32          CurrLAY;
	Int32          CurrGX;
	Int32          CurrGY;
	Int32          OldPointX;
	Int32          OldPointY;
	Int32          NewPointX;
	Int32          NewPointY;
	Int32          GShiftX;
	Int32          GShiftY;
	// пикселы
	PWord8         pLAPix = NULL;
	PWord8         pGrayPix = NULL;
	PWord8         pGrayLine = NULL;
	Word32         LABitInByte;
	Word8          Decrement;
	Word8          Corner = 1;
	// маска
	Handle         hMask = NULL;
	PWord8         pMask = NULL;

	hMask = RIMAGEDAlloc(OutLineLenght * OunLines, "Rotator::DescewGray - mask");
	pMask = (PWord8)RIMAGELock(hMask);


	if ( cDIBOut->GetPixelSize() != 8 )
	{
		return FALSE;
	}

	if ( pMask )
	{
		for ( CurrGY = OunLines - 1; CurrGY >= 0; CurrGY-- )
		{
			pGrayLine = (PWord8)cDIBOut->GetPtrToLine(CurrGY);
			memset(pGrayLine, 127, OutLineLenght);
			memset((pMask + (CurrGY * OutLineLenght)), 1, OutLineLenght);
		}
	}
	else
	{
		for ( CurrGY = OunLines - 1; CurrGY >= 0; CurrGY-- )
		{
			pGrayLine = (PWord8)cDIBOut->GetPtrToLine(CurrGY);
			memset(pGrayLine, 255, OutLineLenght);
		}
	}

	for ( CurrLAY = 0; CurrLAY < (Int32)InLines; CurrLAY++)
	{
		for ( CurrLAX = 0; CurrLAX < (Int32)InLineLenght; CurrLAX++)
		{
			GetExtCoord(CurrLAX, CurrLAY, &OldPointX, &OldPointY);
			RollPoint(OldPointX, OldPointY, &NewPointX, &NewPointY);

			NewPointX += AtX * 256;
			NewPointY += AtY * 256;

			GetGrayCoord(NewPointX, NewPointY, &CurrGX, &CurrGY, &GShiftX, &GShiftY);

			//CurrGX += AtX;
			//CurrGY += AtY;

			if ( CurrGX < (Int32)OutLineLenght &&
				 CurrGY < (Int32)OunLines)
			{
				pGrayPix = (PWord8)cDIBOut->GetPtrToPixel(CurrGX, CurrGY);

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
			if ( CurrLAX < (Int32)InLineLenght &&
				 CurrLAY < (Int32)InLines)
			{
				pLAPix = (PWord8)cDIBIn->GetPtrToPixel(CurrLAX, CurrLAY);

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

Bool32 CRRotator::GetExtCoord(Int32 X, Int32 Y, PInt32 peX, PInt32 peY)
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

Bool32 CRRotator::RollPoint(Int32 X, Int32 Y, PInt32 pnX, PInt32 pnY)
{
	if ( pnX )
	{
		*pnX = (Int32)(((Float80)X * mfCos) + ((Float80)Y * mfSin));
	}

	if ( pnY )
	{
		*pnY = (Int32)(((Float80)Y * mfCos) - ((Float80)X * mfSin));
	}

	return TRUE;
}

Bool32 CRRotator::GetGrayCoord(Int32 eX, Int32 eY, PInt32 pgX, PInt32 pgY, PInt32 psX, PInt32 psY)
{
	if ( pgX )
	{
		*pgX = (Int32)(eX / 256);

		if ( psX )
			*psX = (Int32)(eX - (*pgX * 256) - 128);
	}

	if ( pgY )
	{
		*pgY = (Int32)(eY / 256);

		if ( psY )
			*psY = (Int32)(eY - (*pgY * 256) - 128);
	}

	return TRUE;
}



Bool32 CRRotator::UnmaskPixels(PWord8 pMask, PWord8 pGaryPix, Int32 X, Int32 Y, Word32 wLineLen, Int32 ShiftX, Int32 ShiftY)
{
	if ( *(pMask + (Y * wLineLen + X)) )
	{
		*(pMask + (Y * wLineLen + X)) = 0;
		*pGaryPix = 255;
	}

	if ( ShiftX > 0 && X < (Int32)(wLineLen - 1) )
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

Bool32 CRRotator::AddBlackToGray(Int32 X, Int32 Y, Word8 Gray)
{
	static PWord8 pGrayPixel;

	pGrayPixel = (PWord8)mdDIBOut->GetPtrToPixel(X, Y);

	if ( pGrayPixel )
	{
		*pGrayPixel = (Gray > *pGrayPixel ? 0 : *pGrayPixel - Gray);
		return TRUE;
	}

	return FALSE;
}

void CRRotator::AllocWorkBuffers(Word32 wStringBufferRange)
{
	if (mwTablesRange)
	{
		hHShiftTable   = RIMAGEDAlloc((mwTablesRange + 1)*4,"Rotator::AllocWorkBuffer - hor shift table");
		HShiftTable    = (PInt32)RIMAGELock(hHShiftTable);
		hVShiftTable   = RIMAGEDAlloc((mwTablesRange + 1)*4,"Rotator::AllocWorkBuffer - vert shift table");
		VShiftTable    = (PWord32)RIMAGELock(hVShiftTable);
		hVShiftLenght  = RIMAGEDAlloc((mwTablesRange + 1)*4,"Rotator::AllocWorkBuffer - vl shift table");
		VShiftLenght   = (PWord32)RIMAGELock(hVShiftLenght);
	}

	if ( wStringBufferRange )
	{
		//hBufferForGray = RIMAGEAlloc( wLineBytesWTo + HorisontalShiftInBytes * 2 + ( VShiftLenght[0] * 2 )  );     //буфер для конечной картинки
		hBufferForGray = RIMAGEDAlloc( wWorkGrayBufferLenght = ( mwDescewedWidth * 4 ), "Rotator::AllocWorkBuffer - buffer for gray");     //буфер для конечной картинки
		BufferForGray  = (PWord8)RIMAGELock(hBufferForGray);
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

Bool32 CRRotator::RotatePoint(Int32 iX, Int32 iY, PInt32 prX, PInt32 prY)
{
	Int32 tY;

	if ( VShiftTable == NULL ||  HShiftTable == NULL)
	{
		return FALSE;
	}

	if ( iX > -1 && iX < (Int32)mwOriginalWidth &&
		 iY > -1 && iY < (Int32)mwOriginalHeight    )
	{
		if ( mfTang > 0)
		{
			*prY = iY - VShiftTable[iX] + AtY;

			if ( *prY < 0 || *prY >= (Int32)mwTablesRange )
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

			if ( tY < 0 || tY >= (Int32)mwTablesRange )
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

void CRRotator::ComposeLineG(PCTDIB cDIBIn, Int32 iDirect, Word32 wLine, Word32 wBuffLen)
{
	Word32 j;
	Word32 StartByte;
	Word32 EndByte;
	Word32 FulBytes;
	Word32 PixelSize = cDIBIn->GetPixelSize() / 8;
	Int32  PosInBuff;
	Word32 Copy = 0;
	PWord8 PixFrom;
	Word32 wLines = cDIBIn->GetLinesNumber();

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
				PixFrom = (PWord8)cDIBIn->GetPtrToPixel(j,wLine + VShiftTable[j] - AtY);
			}
			else
			{
				PixFrom = (PWord8)cDIBIn->GetPtrToPixel(j,wLines - (wLine + VShiftTable[j] - AtY));
			}
		}
        // Allex  27.03.00
        // слабое место. на некоторых файлах возникают прблемы с отписыванием в BufferForGray - пытается отписать
        // туда больше чем длина файла. возможно гсвязано с размером пиксела в цветном файле.
        if ( PosInBuff < (Int32)wWorkGrayBufferLenght )
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
