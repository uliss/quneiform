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

// CRIBinarizator.cpp: implementation of the CRIBinarizator class.
//
//////////////////////////////////////////////////////////////////////
#include "resource.h"
#include <stdlib.h>
#include <math.h>
#include "cribinarizator.h"
#include "crimemory.h"

//#include "CRDezaG2BW_trk.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRIBinarizator::CRIBinarizator()
{

	meBinType         = CTBIN_UNKNOWN;
	mpIncomeDIB       = NULL;
	mpOutcomeDIB      = NULL;
	mpszGreyBuffer    = NULL;
	mpDezaBinarizator = NULL;
	//mpKronrodBinarizator = NULL;
	mhszGreyBuffer    = NULL;
	mpszGreyBuffer    = NULL;
	mpProgressor      = NULL;
    //mhszGreyBuffer
	mbIndexColor      = FALSE;
	mfBlueK  = ((Float32)(mwBlueK = 70)) / 255;
	mfGreenK = ((Float32)(mwGreenK = 130)) / 255;
	mfRedK   = ((Float32)(mwRedK = 220)) / 255;
}

CRIBinarizator::CRIBinarizator( PCRProgressor pProgressIndicator)
{
	CRIBinarizator();
	mpProgressor = pProgressIndicator;
}

CRIBinarizator::~CRIBinarizator()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Загрузка растра - wFlag
Bool32 CRIBinarizator::SetRasters(PCTDIB pSrcDIB, PCTDIB pDescDIB)
{
	if ( pSrcDIB && pDescDIB )
	{
		mpIncomeDIB = pSrcDIB;
		mpOutcomeDIB = pDescDIB;

		if ( (mwSrcBitCount = mpIncomeDIB->GetPixelSize()) < 4 )
		{
			SetReturnCode_rimage(IDS_RIMAGE_DIB_CANT_TO_BE_BINARISED);
			return FALSE;
		}

		if ( mpOutcomeDIB->GetPixelSize() != 1 )
		{
			SetReturnCode_rimage(IDS_RIMAGE_DIB_OUT_FORMAT_NOT_BINARISED);
			return FALSE;
		}

		if ( (mwLineLenght = mpIncomeDIB->GetLineWidth()) != mpOutcomeDIB->GetLineWidth() )
		{
			SetReturnCode_rimage(IDS_RIMAGE_OUTCOME_DIB_NOT_LINK_TO_INCOME);
			return FALSE;
		}

		if ( !SupportedIndexColorImage(mpIncomeDIB) )
		{

		}

		return TRUE;
	}
	return FALSE;
}

Bool32 CRIBinarizator::Binarize(CTBinarize eBinType, Word32 wFlag)
{
	Bool32 bRet = FALSE;

	if ( !mpIncomeDIB || !mpOutcomeDIB )
		return FALSE;

	meBinType = eBinType;

	try
	{
		if ( !OpenBinarizator(wFlag) )
			return FALSE;

		if ( !OnBinarizator() )
		{
			CloseBinarizator();
			return FALSE;
		}

		CloseBinarizator();
		bRet =  TRUE;
	}

	catch ( Word32 wExit)
	{
		if ( wExit = IDS_RIMAGE_EXIT_BY_USER)
		{
			SetReturnCode_rimage(IDS_RIMAGE_EXIT_BY_USER);
		}
		else
		{
			SetReturnCode_rimage(IDS_RIMAGE_UNKNOWN_ERROR);
		}

		bRet =  FALSE;
	}

	return bRet;
}

Bool32 CRIBinarizator::OpenBinarizator(Word32 wFlag)
{
	Bool32 bRet = FALSE;

	switch ( meBinType )
	{
	//open Deza
	case CTBIN_DEZA:
		if ( !mpDezaBinarizator )
		{
			mpDezaBinarizator = new CDezaBinarizator;
		}

		bRet = DezaOpenBin(wFlag);
		break;
	//Open Kronrod
	case CTBIN_KRONROD:
		/*
		if ( !mpKronrodBinarizator )
		{
			mpKronrodBinarizator = new CKronrodBinarizator;
		}
		*/
		mwGreyBufferSize = mpIncomeDIB->GetLineWidth() + 8;
		if ( !(mhszGreyBuffer = RIMAGEDAlloc( mwGreyBufferSize, "Binarizator - grey buffer")) ||
			 !(mpszGreyBuffer = (PWord8)RIMAGELock(mhszGreyBuffer)) )
		return FALSE;

		bRet = KronrodOpenBin(mpIncomeDIB->GetLinesNumber(), mpIncomeDIB->GetLineWidth());
		break;
	default:
		return FALSE;
	}
	return bRet;
}

Bool32 CRIBinarizator::CloseBinarizator()
{

	switch ( meBinType )
	{
	//close Deza////////////////////////////////
	case CTBIN_DEZA:
		DezaCloseBin();

		if ( mpDezaBinarizator )
		{
			delete mpDezaBinarizator;
			mpDezaBinarizator = NULL;
		}

		break;
	//close Kronrod/////////////////////////////
	case CTBIN_KRONROD:
		KronrodCloseGray();

		if (mpszGreyBuffer)
		{
			RIMAGEUnlock(mhszGreyBuffer);
			mpszGreyBuffer = NULL;
		}
		if (mhszGreyBuffer)
			RIMAGEFree(mhszGreyBuffer);

		mhszGreyBuffer = NULL;
		/*
		if ( mpKronrodBinarizator)
		{
			delete mpKronrodBinarizator;
			mpKronrodBinarizator = NULL;
		}
		*/
		break;
	////////////////////////////////////////////
	default:
		return FALSE;
	}

	return TRUE;
}

Bool32 CRIBinarizator::OnBinarizator()
{
	Bool32 bRet = FALSE;

	switch (meBinType)
	{
	case CTBIN_DEZA:
	case CTBIN_KRONROD:
		bRet = OnBinarizeLoop();
		break;
	default:
		return FALSE;
	}
	return bRet;
}

Bool32 CRIBinarizator::OnBinarizeLoop()
{
	Bool32 bRet = FALSE;
	Int32  i;
	PWord8 pLALine;
	Int32 NumberBWLines = 0;
	Int32 CurGreyLine = 0;
	Word32  nLines = mpOutcomeDIB->GetLinesNumber();

	/////////////////////////////////////////////////////////////////////////
	//   см. G2BW.cpp, дшту 67 - Binarize
	//   особое внимание для Kronrod-а
	switch (meBinType)
	{
	// DEZA line binarize//////////////////////////////////////////
	case CTBIN_DEZA:
		i = 0;
		do
		{
			pLALine = (PWord8)mpOutcomeDIB->GetPtrToLine(i++);

			if ( !pLALine )
				break;
		}
		while ( mpDezaBinarizator->GetBinarized(pLALine, (Word16)(mpOutcomeDIB->GetUsedLineWidthInBytes())) );          // was Outcome

		bRet = ( i == ( (Int32)nLines + 1 ) );
		break;
	// Kranrod line binarize///////////////////////////////////////
	case CTBIN_KRONROD:
		i = 0;
		CurGreyLine = 0;

		mpProgressor->Start();

		while (  i < (Int32)nLines )
		{
			pLALine = (PWord8)mpOutcomeDIB->GetPtrToLine(i++);

			if(!NumberBWLines)
			{
				do
				{
					if(!KronrodImageRead(mpszGreyBuffer,(Int16)CurGreyLine++,(Int16)1))
						return 0;

					if ( mpProgressor->SetStep( ((CurGreyLine / nLines ) * 50) ) )
					{
						mpProgressor->Finish();
						throw IDS_RIMAGE_EXIT_BY_USER;
					}
				}
				while((NumberBWLines=KronrodGreyTo(mpszGreyBuffer),!NumberBWLines));
			}
			KronrodGreyFrom(pLALine);
			NumberBWLines--;

			if ( mpProgressor->SetStep( ((i / nLines ) * 50) + 50 ) )
			{
				throw IDS_RIMAGE_EXIT_BY_USER;
			}
		}

		mpProgressor->Finish();

		bRet = TRUE;
		break;
	default:
		return FALSE;
	}
	return bRet;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// Kronrod binarization
	#undef FUN_IMPO
	#define FUN_IMPO(a)    a
	extern "C"
	{
		FUN_IMPO(void)   grey_open(Word16 Width, Word16 Height);
		FUN_IMPO(Word16) grey_to(PWord8);
		FUN_IMPO(void)   grey_from(PWord8);
		FUN_IMPO(void)   grey_close(void);
	}
	#undef FUN_IMPO
//////////////////////////////////////////////////////////
//
Bool32 CRIBinarizator::KronrodOpenBin(Word32 wHeight, Word32 wWeidth)
{
	/*
	if ( !mpKronrodBinarizator )
		return FALSE;
	*/
	//mpKronrodBinarizator->grey_open(wHeight, wWeidth);
	grey_open((Word16)wHeight, (Word16)wWeidth);
	mbKronrodInvert = TRUE;
	return TRUE;
}


Int16 CRIBinarizator::KronrodImageRead(PWord8 lpImage, Int16 fstLine, Int16 nLines)
{
	/*
	if ( !mpKronrodBinarizator )
		return FALSE;
	*/
	return (Int16)KronrodImageRead(lpImage, (Int32)fstLine, (Int32)nLines );
}

Int32 CRIBinarizator::KronrodImageRead(PWord8 lpImage, Int32 fstLine, Int32 nLines)
{
	Word32 i;
	Word32 j;
	Word32 ReadedPixelsperLine = 0;
	Word32 ReadedLines = 0;
	Word32 wFirstLine = fstLine;
	Word32 wLines = nLines;
	Word8 gray;
	Word8 halfgray;
	PWord8 pIn;

	if ( !lpImage )
		return 0;

    if( (wFirstLine + wLines) > mpIncomeDIB->GetLinesNumber() )
         return 0;

	switch ( mwSrcBitCount )
	{
	case 8:
		for(i=0;i<wLines;i++)
		{

			ReadedPixelsperLine = 0;

			for(j=0;j<mwLineLenght;j++)
			{
				if ( !(pIn = (PWord8)mpIncomeDIB->GetPtrToPixel(j,fstLine + i /*- 1*/)) )
					break;

				gray = mbIndexColor ? wIndex8ToGray[pIn[0]] : pIn[0];

				*(lpImage + mwLineLenght*i + j)	= gray ;

				ReadedPixelsperLine++;
			}

			ReadedLines += (ReadedPixelsperLine / mwLineLenght);
		}
		break;
	case 4:
		for(i=0;i<wLines;i++)
		{
			ReadedPixelsperLine = 0;

			for(j=0;j<mwLineLenght;j+=2)
			{
				if ( !(pIn = (PWord8)mpIncomeDIB->GetPtrToPixel(j,fstLine + i)) )
					break;

				gray = *pIn;
				halfgray = mbIndexColor ? wIndex4ToGray[((gray & 0xF0)>>4)] : ((gray & 0xF0)>>4);
				*(lpImage + mwLineLenght*i + j)	=  halfgray;
				halfgray = mbIndexColor ? wIndex4ToGray[(gray & 0x0F)] : (gray & 0x0F);
				*(lpImage + mwLineLenght*i + j + 1)	=  halfgray;

				ReadedPixelsperLine += 2;
			}

			ReadedLines += (ReadedPixelsperLine / mwLineLenght);
		}
		break;
	case 24:
		for(i=0;i<wLines;i++)
		{

			ReadedPixelsperLine = 0;

			for(j=0;j<mwLineLenght;j++)
			{
				if ( !(pIn = (PWord8)mpIncomeDIB->GetPtrToPixel(j,fstLine + i)) )
					break;

				gray = ((pIn[0] + pIn[1] + pIn[2]) / 3);
				*(lpImage + mwLineLenght*i + j)	= gray ;

				ReadedPixelsperLine++;
			}

			ReadedLines += (ReadedPixelsperLine / mwLineLenght);
		}
		break;
	default:
		return 0;
	}

	return ReadedLines;//nLines;
}

Bool32 CRIBinarizator::KronrodGreyTo(PWord8 pGTo)
{
	Word32 Size = mwGreyBufferSize;
	Word32 i;

	if(mbKronrodInvert)
	{
		for(i=0; i<Size; i++)
			*(pGTo + i)=~(*(pGTo + i));
	}

	//return mpKronrodBinarizator->grey_to(pGTo);
	return grey_to(pGTo);
}

Bool32 CRIBinarizator::KronrodGreyFrom(PWord8 pGFrom)
{
	Word32 Size = mpOutcomeDIB->GetLineWidthInBytes();
	Word32 i;

	//mpKronrodBinarizator->grey_from(pGFrom);
	grey_from(pGFrom);

	if(mbKronrodInvert)
	{
		for(i=0;i<Size;i++)
			*(pGFrom + i)=~(*(pGFrom + i));
	}
	return TRUE;
}

Bool32 CRIBinarizator::KronrodCloseGray()
{
	//mpKronrodBinarizator->grey_close();
	grey_close();

	return TRUE;
}
Bool32 CRIBinarizator::DezaOpenBin(Word32 wDezaFlag)
{
	Word32  wDpiX, wDpiY;
	PVOID fGetGreyBlock;

	fGetGreyBlock = (PVOID)this;

#ifdef DEZA_TIG_IMAGEINFO
	TIG_IMAGEINFO    DezaImageInfo;

	DezaImageInfo.wImageHeight       = (Word16)mpIncomeDIB->GetLinesNumber();
	DezaImageInfo.wImageWidth        = (Word16)mpIncomeDIB->GetLineWidth();
	DezaImageInfo.wImageByteWidth    = (Word16)mpIncomeDIB->GetUsedLineWidthInBytes();
	DezaImageInfo.wImageDisplacement = 0;
	mpIncomeDIB->GetResolutionDPI(&wDpiX, &wDpiY);
	DezaImageInfo.wResolutionX       = (Word16)wDpiX;
	DezaImageInfo.wResolutionY       = (Word16)wDpiY;
	DezaImageInfo.bFotoMetrics       = 1;
	DezaImageInfo.wAddX              = 0;
	DezaImageInfo.wAddY              = 0;

	return OpenTrackBin(&DezaImageInfo, fGetGreyBlock, (Word16)wDezaFlag);
#else
	mDezaImageInfo.wImageHeight       = (Word16)mpIncomeDIB->GetLinesNumber();
	mDezaImageInfo.wImageWidth        = (Word16)mpIncomeDIB->GetLineWidth();
	mDezaImageInfo.wImageByteWidth    = (Word16)mpIncomeDIB->GetUsedLineWidthInBytes();
	mDezaImageInfo.wImageDisplacement = 0;
	mpIncomeDIB->GetResolutionDPI(&wDpiX, &wDpiY);
	mDezaImageInfo.wResolutionX       = (Word16)wDpiX;
	mDezaImageInfo.wResolutionY       = (Word16)wDpiY;
	mDezaImageInfo.bFotoMetrics       = 1;
	mDezaImageInfo.wAddX              = 0;
	mDezaImageInfo.wAddY              = 0;

	return mpDezaBinarizator->OpenTrackBin(&mDezaImageInfo, this, wDezaFlag);
#endif

}

Bool32 CRIBinarizator::DezaCloseBin()
{
	mpDezaBinarizator->CloseTrackBin();
	return TRUE;
}
Bool32 CRIBinarizator::SupportedIndexColorImage(PCTDIB pImage)
{
    Word32 Colors = pImage->GetActualColorNumber();
    Word32 PalletteSize = pImage->GetRGBPalleteSize();
    CTDIBRGBQUAD Q, prQ;
    Word32 i;

	mbIndexColor = false;

    if ( PalletteSize == 0 || Colors == 2 || Colors > 256 )
        return TRUE;


    for ( i = 1; i < Colors; i++ )
    {
        if ( !pImage->GetRGBQuad(i, &Q) || !pImage->GetRGBQuad(i-1, &prQ) )
            return FALSE;

        if ( ( Q.rgbBlue  < prQ.rgbBlue ) ||
			 ( Q.rgbGreen < prQ.rgbGreen ) ||
			 ( Q.rgbRed   < prQ.rgbRed )      )
            break;
    }

	if ( i == Colors )
		return true;

	PrepareIndexTable(pImage);

    return true;
}

Bool32 CRIBinarizator::PrepareIndexTable(PCTDIB pDIB)
{
	Word32 i;
	CTDIBRGBQUAD Quad;
	PWord8 pTable = NULL;
	Word32 Colors = (pDIB->GetRGBPalleteSize())/4;

	switch ( pDIB->GetPixelSize() )
	{
	case 4:
		pTable = wIndex4ToGray;
		break;
	case 8:
		pTable = wIndex8ToGray;
		break;
	default:
		return false;
	}

	for ( i = 0; i < Colors; i++ )
	{
		if ( !pDIB->GetRGBQuad(i, &Quad) )
			return false;

		pTable[i] = IndexPalleteToGray( &Quad );
	}

	return (mbIndexColor = true);

}

//#include <math.h>
Word8 CRIBinarizator::IndexPalleteToGray(PCTDIBRGBQUAD pQuad)
{
	Float32 b = ((Float32)(pQuad->rgbBlue) * 70 ) / 255;
	Float32 g = ((Float32)(pQuad->rgbGreen) * 220 ) / 255;
	Float32 r = ((Float32)(pQuad->rgbRed) * 130 ) / 255;

	return (Word8)sqrt((((b * b) + (g * g) + (r * r)) / 70200 ) * 65025  );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//end of file
