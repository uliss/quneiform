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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "cticontrol.h"
#include "ctimemory.h"
#include "dpuma.h"

/////////////////////////////////////////////////
//#define CIMAGE_DUMP_ENABLE

#ifdef CIMAGE_DUMP_ENABLE
#include "PumaMemoryToFileDumper.h"
#endif
//////////////////////////////////////////////////
//#define _TRASING
//
#ifdef _TRASING
#include "afx.h"
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTIControl::CTIControl()
{
	mbSourceDIBCopy = FALSE;
	hCBImage      = NULL;
	pCBImage      = NULL;
	pCBBuffer     = NULL;
	wCBBufferSize = 0;
	wCBLine       = 0;
	wCBLines      = 0;
	wCBWidth      = 0;
	wCBStep       = 0;
	mCBSourceDIB        = NULL;
	mCBDestianationDIB  = NULL;
	mCBWSourceDIB       = NULL;
	mCBWDestianationDIB = NULL;
	mCBWInProcess       = FALSE;
	mCBWName[0]         = 0x0;
	mCBName[0]          = 0x0;
	mpDIBFromImage      = NULL;
	mpBitFildFromImage  = NULL;
	mhBitFildFromImage  = NULL;
	mpcSrcDIBReadMask   = NULL;
	mpcSrcDIBWriteMask  = NULL;
	mbEnableDIBReadMask = TRUE;
	mbEnableDIBWriteMask= TRUE;
	mwMemoryErrors      = 0;
	mbWriteFlag         = FALSE;
	mhCopyedDIB         = NULL;

	mwLAWhiteLeftMask[7]  = 0xff;
	mwLAWhiteRightMask[0] = 0xff;
	mwLABlackLeftMask[0]  = 0x7f;
	mwLABlackRightMask[7] = 0xfe;
	mwIndexMask[0]        = 0x01;
	for ( uint32_t i = 1; i < 8 ; i++ )
	{
		mwLAWhiteLeftMask[7 - i]  = mwLAWhiteLeftMask[8 - i] << 1;
		mwLAWhiteRightMask[i]     = mwLAWhiteRightMask[i - 1] >> 1;
		mwLABlackLeftMask[i]      = mwLABlackLeftMask[i - 1] >> 1;
		mwLABlackRightMask[7 - i] = mwLABlackRightMask[8 - i] << 1;
		mwIndexMask[i]            = mwIndexMask[i - 1] << 1;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTIControl::~CTIControl()
{
	FreeBuffers();

	if ( mCBSourceDIB != NULL )
	{
		delete mCBSourceDIB;
	}
	if ( mCBWSourceDIB != NULL )
	{
		delete mCBWSourceDIB;
	}
	if ( mCBDestianationDIB != NULL )
	{
		delete mCBDestianationDIB;
	}
	if ( mCBWDestianationDIB != NULL )
	{
		delete mCBWDestianationDIB;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::WriteCBImage(char*  lpName, CIMAGEIMAGECALLBACK Cbk )
{
	Handle hNewDIB;
	Bool32 Ret;
	uint32_t Readed;
	uint32_t i;
	Bool32 bInvert = false;
	CIMAGE_InfoDataInReplace   FrameToReplace;
	CIMAGE_ImageInfo           ImageInfo;
	CTDIBRGBQUAD               cdFQuad;
	CTDIBRGBQUAD               cdSQuad;

	//////////////////////////////////////////////////////////////////////
	//проверка имени картинки
	if ( !lpName || lpName[0] == 0x0 )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGE_NAME);
		return FALSE;
	}
	//
	ImageInfo.wImageWidth = 0;
	ImageInfo.wImageByteWidth = 0;
	ImageInfo.wImageHeight = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// Создаем новый DIB
	if ( mCBWDestianationDIB != NULL || mCBWSourceDIB != NULL )
	{
		return FALSE;
	}
	// для проверки на выявление узких мест
	mCBWInProcess = TRUE;
	if ( strlen( lpName ) < CIMAGE_MAX_IMAGE_NAME)
	{
		strcpy(mCBWName, lpName);
	}
	else
	{
		strncpy(mCBWName, lpName, CIMAGE_MAX_IMAGE_NAME - 1);
	}
	// этот в качестве буфера для закидывания по одной строке
	mCBWSourceDIB       = new CTDIB;
	// а этот будет основным
	mCBWDestianationDIB = new CTDIB;
	///////////////////////////////////////////////////////////////////////////////////
	// закачиваем информацию о создаваемом
	Ret = Cbk.CIMAGE_ImageOpen(&ImageInfo);

	if ( Ret == FALSE )
	{
		SetReturnCode_cimage(IDS_CIMAGE_UNABLE_USE_IMPORT_CALLBACK);
		mCBWInProcess = FALSE;
		return FALSE;
	}

	if ( ImageInfo.wImageByteWidth == 0 || ImageInfo.wImageHeight == 0 )
	{
		SetReturnCode_cimage(IDS_CIMAGE_UNABLE_USE_IMPORT_CALLBACK);
		mCBWInProcess = FALSE;
		return FALSE;
	}
	// выделение памяти под новый DIB и под буферный
	if ( mCBWDestianationDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock) &&
		 mCBWSourceDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock)    )
	{
		CIMAGEComment(lpName);
		if ( mCBWDestianationDIB->CreateDIBBegin(ImageInfo.wImageWidth, ImageInfo.wImageHeight, 1) &&
			 mCBWDestianationDIB->SetResolutionDPI(ImageInfo.wResolutionX, ImageInfo.wResolutionY) &&
			 mCBWDestianationDIB->CreateDIBEnd() )
		{
			CIMAGEComment("Temporary DIB - WriteCBImage");
			if ( mCBWSourceDIB->CreateDIBBegin(ImageInfo.wImageWidth, 1, 1) &&
				 mCBWSourceDIB->CreateDIBEnd() )
			{
				cdFQuad.rgbBlue = cdFQuad.rgbGreen = cdFQuad.rgbRed = 0x00;
				cdSQuad.rgbBlue = cdSQuad.rgbGreen = cdSQuad.rgbRed = 0xff;

				if (ImageInfo.bFotoMetrics == 0)
				{
					bInvert = true;
				}

				mCBWDestianationDIB->SetRGBQuad(0,cdFQuad);
				mCBWSourceDIB->SetRGBQuad(0,cdFQuad);
				mCBWDestianationDIB->SetRGBQuad(1,cdSQuad);
				mCBWSourceDIB->SetRGBQuad(1,cdSQuad);
				// заполняем структурку	для замещения строк
				FrameToReplace.byBit      = (uint16_t)mCBWSourceDIB->GetPixelSize();
				FrameToReplace.dwX        = 0;
				FrameToReplace.dwY        = 0;
				FrameToReplace.dwWidth    = (uint16_t)mCBWSourceDIB->GetLineWidth();
				FrameToReplace.dwHeight   = (uint16_t)mCBWSourceDIB->GetLinesNumber();
				FrameToReplace.wByteWidth = (uint16_t)mCBWSourceDIB->GetLineWidthInBytes();
				FrameToReplace.lpData     = (puchar)mCBWSourceDIB->GetPtrToLine(0);
				FrameToReplace.MaskFlag   = 0;

				// Заполняем его
				for ( i = 0; i < ImageInfo.wImageHeight; i++ )
				{
					// вызываем второй калбэк
					Readed = Cbk.CIMAGE_ImageRead((pchar)mCBWSourceDIB->GetPtrToLine(0),
												  (uint16_t)mCBWSourceDIB->GetLineWidthInBytes());

					//инвертируем битовое поле, ежели надо
					if ( bInvert )
					{
						pchar pBits = (pchar)mCBWSourceDIB->GetPtrToLine(0);
						int32_t Ii;

						for ( Ii = 0; Ii < (uint16_t)mCBWSourceDIB->GetLineWidthInBytes(); Ii++ )
						{
							*pBits++ = ~(*pBits);
						}
					}

					// номер строчки
					FrameToReplace.dwY = i;
					// записваем строчку в новый из буфера
					if (Readed)
					{
						SetFrame(mCBWSourceDIB, mCBWDestianationDIB, &FrameToReplace);
					}
				}
				// Закрываем
				// вывзываем третий калбэк
				Ret = Cbk.CIMAGE_ImageClose();
				// отписываем в контейнер
				mCBWDestianationDIB->GetDIBHandle(&hNewDIB);
				// Пишем картинку, и не каких масок!!!
				mbWriteFlag = TRUE;
				Ret = SetDIB(lpName, hNewDIB, 0);
				mbWriteFlag = FALSE;

				if ( Ret == FALSE )
				{
					mCBWInProcess = FALSE;
					return FALSE;
				}
			}
		}
	}


	if ( mCBWSourceDIB )
	{
		mCBWSourceDIB->DestroyDIB();
		delete mCBWSourceDIB;
		mCBWSourceDIB = NULL;
	}

#ifdef CIMAGE_DUMP_ENABLE

	//WriteDIBtoBMP("Allex.CBWrited.bmp", mCBWDestianationDIB);

#endif

	if ( mCBWDestianationDIB )
	{
		mCBWDestianationDIB->DestroyDIB();
		delete mCBWDestianationDIB;
		mCBWDestianationDIB = NULL;
	}

	mCBWInProcess = FALSE;
	return Ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetCBImage (char*  lpName, PCIMAGEIMAGECALLBACK pCbk)
{
	Handle  hImage = NULL;

	if ( !pCbk  )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
		return FALSE;
	}

	if ( !GetDIB(lpName, &hImage, TRUE ) )
	{
		return FALSE;
	}

#ifdef CIMAGE_DUMP_ENABLE
	PCTDIB  pDebugDIB;

	pDebugDIB = new CTDIB();
	pDebugDIB->SetDIBbyPtr(hImage);
	//WriteDIBtoBMP("Allex.CBReaded.bmp",pDebugDIB);
	delete pDebugDIB;
#endif

	if ( strlen( lpName ) < CIMAGE_MAX_IMAGE_NAME)
	{
		strcpy(mCBName, lpName);
	}
	else
	{
		strncpy(mCBName, lpName, CIMAGE_MAX_IMAGE_NAME - 1);
	}
	/////////////////////////////////////////
	pCbk->CIMAGE_ImageOpen  = CIMAGE_Callback_ImageOpen;
	pCbk->CIMAGE_ImageRead  = CIMAGE_Callback_ImageRead;
	pCbk->CIMAGE_ImageClose = CIMAGE_Callback_ImageClose;
	wCBStep = 0;

	hCBImage = hImage;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::SetDIB(char*  lpName, Handle hDIB, uint32_t wFlag)
{
	Handle  hImage = NULL;

	if ( wFlag == 0 )
	{  // создаем новую копию
		if ( !CopyDIB( hDIB, &hImage) )
		{
			SetReturnCode_cimage(IDS_CIMAGE_UNABLE_COPY_DIB);
			return FALSE;
		}
	}
	else
	{
		hImage = hDIB;
	}

	return mlImages.AddImage(lpName, hImage, wFlag);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetDIB(char*  lpName, Handle* phDIB, uint32_t wFlag)
{
	Handle  hImage = NULL;  // Handle

	// открываем картинку
	if ( !OpenDIBFromList( lpName, &hImage ) )
		return FALSE;

	//ALLEX Mask
	// ошибка не тут
	// открываем маски (если есть)
	if ( !OpenMaskFromList( lpName, &mpcSrcDIBReadMask, &mbEnableDIBReadMask, "r" ) ||
		 !OpenMaskFromList( lpName, &mpcSrcDIBWriteMask, &mbEnableDIBWriteMask, "w" )  )
		 return FALSE;


	mbSourceDIBCopy = (wFlag == 0 );
	if ( mbSourceDIBCopy )
	{  // создаем новую копию
		if ( CopyDIB( hImage, phDIB) )
		{
			return TRUE;
		}

		SetReturnCode_cimage(IDS_CIMAGE_UNABLE_COPY_DIB);
	}
	else
	{
		*phDIB = hImage;
		return TRUE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetImage(char*  lpName, PCIMAGE_InfoDataInGet lpIn, PCIMAGE_InfoDataOutGet lplpOut)
{
	Bool32   bRet = FALSE;
	PCTDIB   pDscDIB;
	void*    pDIBMemory;
	int32_t    iY;

	FreeBuffers();

	// берем кусок диба оттедова
	if ( GetDIBFromImage(lpName, lpIn, (pchar*)&pDIBMemory ) )
	{
		pDscDIB = new CTDIB;
		if ( pDscDIB->SetDIBbyPtr(pDIBMemory) )
		{
#ifdef CIMAGE_DUMP_ENABLE
			//WriteDIBtoBMP("Allex.AlmiData.bmp",pDscDIB);
#endif
			//////////////////////////////////////
			// смотрим, что там на выход;
			if ( lpIn->wByteWidth >= pDscDIB->GetUsedLineWidthInBytes()
				 && lpIn->dwWidth    == pDscDIB->GetLineWidth()
				 && lpIn->dwHeight   == pDscDIB->GetLinesNumber()
				)
			{
				uint32_t nOutLine;
				puchar pOutLine;
				uchar  WhiteBit;

				lplpOut->byBit      = (uint16_t)pDscDIB->GetPixelSize();
				lplpOut->dwHeight   = pDscDIB->GetLinesNumber();
				lplpOut->dwWidth    = pDscDIB->GetLineWidth();
				lplpOut->wByteWidth = (uint16_t)pDscDIB->GetUsedLineWidthInBytes();

				lplpOut->wBlackBit  = pDscDIB->GetBlackPixel();

				WhiteBit = (uchar)pDscDIB->GetWhitePixel();

				mhBitFildFromImage = CIMAGEDAlloc(lpIn->wByteWidth * lpIn->dwHeight,lpName);
				mpBitFildFromImage = (puchar)CIMAGELock(mhBitFildFromImage);

				if ( !mhBitFildFromImage || !mpBitFildFromImage )
				{
					if ( mhBitFildFromImage )
						CIMAGEFree(mhBitFildFromImage);

					delete pDscDIB;
					SetReturnCode_cimage(IDS_CIMAGE_ERR_NO_MEMORY);
					return FALSE;
				}

				lplpOut->lpData = pOutLine = mpBitFildFromImage;
				iY = (int32_t)lpIn->dwY;

			    // для Almi - заполняем белым пикселом
				/*
				for ( ;iY < 0; iY++, lplpOut->dwHeight++ )
				{
					// копируем полученное в lplpOut.lpData
					CIMAGE_MEMSET(pOutLine, WhiteBit, lplpOut->wByteWidth);
					pOutLine += lplpOut->wByteWidth;
				}
				*/
				//end для Almi

				for ( nOutLine = 0;nOutLine < lplpOut->dwHeight; nOutLine++ )
				{
					// копируем полученное в lplpOut.lpData
					CIMAGE_MEMCOPY(pOutLine, pDscDIB->GetPtrToLine(nOutLine), lplpOut->wByteWidth);
					pOutLine += lplpOut->wByteWidth;
				}

				bRet = TRUE;
			}
		}

		delete pDscDIB;
	}
	else
	{
		bRet = FALSE;
		lplpOut->lpData = NULL;
	}

	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::ReplaceImage(char*  lpName, PCIMAGE_InfoDataInReplace lpIn)
{
	Handle         hImage = NULL;
	void *         pImage;
	PCTDIB         pSrcDIB = NULL;
	PCTDIB         pDscDIB = NULL;
	Bool32         ret = TRUE;

	if ( !GetDIB(lpName, &hImage, TRUE ) )
	{
		return FALSE;

	}

	pImage = CIMAGELock(hImage);

	if ( !pImage )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		ret =  FALSE;
	}

	//инициализируем ctdib-чики
	pSrcDIB = new CTDIB;
	pDscDIB = new CTDIB;

	if ( pDscDIB->SetDIBbyPtr(pImage) )
	{
		if ( pSrcDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock) )
		{
			CIMAGEComment("Temporary DIB - ReplaceImage");
			if ( pSrcDIB->CreateDIBBegin(lpIn->dwWidth,lpIn->dwHeight,pDscDIB->GetPixelSize(),pDscDIB->GetActualColorNumber()) &&
				 pSrcDIB->CreateDIBEnd() )
			{
				if ( lpIn->wByteWidth <= pSrcDIB->GetUsedLineWidthInBytes() )
				{
					uint32_t nInLine;
					puchar pInLine = lpIn->lpData;

					for ( nInLine = 0; nInLine < lpIn->dwHeight; nInLine++ )
					{
						// копируем полученное в lplpOut.lpData
						CIMAGE_MEMCOPY( pSrcDIB->GetPtrToLine(nInLine), pInLine, lpIn->wByteWidth);
						pInLine += lpIn->wByteWidth;
					}

					if ( !SetFrame(pSrcDIB, pDscDIB, lpIn) )
					{
						SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
						ret = FALSE;
					}
				}
			}
		}
	}

	CIMAGEUnlock(hImage);
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetImageInfo(char*  lpName, PCIMAGEBITMAPINFOHEADER lpBIH)
{
	Handle hImage = NULL;
	void * pDIB;

	if ( !lpBIH )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
		return FALSE;
	}

	if ( !GetDIB(lpName, &hImage, TRUE) )
	{
		return FALSE;
	}

	pDIB = CIMAGELock(hImage);

	CIMAGE_MEMCOPY(lpBIH, pDIB, sizeof(CIMAGEBITMAPINFOHEADER));

	CIMAGEUnlock(hImage);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::RemoveImage(char*  lpName)
{
	return mlImages.DeleteImage(lpName);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CopyDIB(Handle hSrc, Handle* phCopyedDib)
{
	void *         pSrc = NULL;
	void *         pDsc = NULL;
	Handle         hDsc = NULL;
	uint32_t         DIBSize = 0;
	PCTDIB         pcSrcDIB = NULL;
	PCTDIB         pcDscDIB = NULL;

	pSrc = CIMAGELock(hSrc);

	if ( !pSrc )
	{
		return FALSE;
	}

	pcSrcDIB   = new CTDIB;

	if ( !pcSrcDIB->SetDIBbyPtr(pSrc) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_DIB);
		delete pcSrcDIB;
		return FALSE;
	}

	DIBSize = pcSrcDIB->GetDIBSize();

	// больше не нужен
	delete pcSrcDIB;

	hDsc = CIMAGEDAlloc(DIBSize, "Copyed DIB - CopyDIB");
	pDsc = CIMAGELock(hDsc);

	if ( !hDsc || !pDsc )
	{
		return FALSE;
	}

	CIMAGE_MEMCOPY(pDsc, pSrc, DIBSize);

	//ALLEX Mask/////////////////////////////////////
	// ошибка тут
	pcDscDIB   = new CTDIB;

	if ( !pcDscDIB->SetDIBbyPtr(pDsc) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_DIB);
		delete pcDscDIB;
		return FALSE;
	}
	// а сюда теперь надо вставить маску на чтение для картинки
	if ( mpcSrcDIBReadMask && mbEnableDIBReadMask && mbWriteFlag == FALSE )
	{
		ApplayMaskToDIB(pcDscDIB, mpcSrcDIBReadMask);
	}
	delete pcDscDIB;
	//////////////////////////////////////////////////
	CIMAGEUnlock(hDsc);
	CIMAGEUnlock(hSrc);

	*phCopyedDib = mhCopyedDIB = hDsc;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInGet pIn, puchar pMask)
{
	if ( pSrcDIB == NULL || pDscDIB == NULL )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	if ( ( pSrcDIB->GetLineWidth() < pDscDIB->GetLineWidth() + pIn->dwX ) ||
		 ( pSrcDIB->GetLinesNumber() < pDscDIB->GetLinesNumber() + pIn->dwY ) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}
	//копируем из одного диба в другой
	if ( !CopyToFrame(pSrcDIB, pDscDIB, pIn, pMask) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	//ALLEX Mask
	// ошибка не тут
	//теперь применим маску к копии
	if ( mpcSrcDIBReadMask && mbEnableDIBReadMask)
	{
		if ( !ApplayMaskToDIB(pDscDIB, mpcSrcDIBReadMask, pIn->dwX, pIn->dwY) )
			return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CopyToFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInGet pFrameInfo, puchar pMask)
{
	uint32_t StartY = pFrameInfo->dwY;
	uint32_t EndY   = StartY + pFrameInfo->dwHeight;
	uint32_t StartX = pFrameInfo->dwX;
	uint32_t nLine;
	uint32_t dLine = 0;
	Bool32 bRet = FALSE;

	if ( pSrcDIB->GetPixelSize() != pSrcDIB->GetPixelSize() )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	for ( nLine = StartY; nLine < EndY; nLine++)
	{
		if ( !pDscDIB->SetFuelLineFromDIB(pSrcDIB, nLine, dLine++, StartX) )
		{
			SetReturnCode_cimage(IDS_CIMAGE_INVALID_FRAME_SIZE);
			return FALSE;
		}
		bRet = TRUE;
	}
	// теперь используем битовую маску
	WriteDIBtoBMP("Allex.DIBCopyToFrame.Before Bit Mask.bmp", pDscDIB);

	if( pMask != NULL &&
		pDscDIB->GetLinesNumber() == pFrameInfo->dwHeight &&
		pDscDIB->GetLineWidth()   == pFrameInfo->dwWidth     )
	{
		bRet = ApplayBitMaskToDIB( pMask, pDscDIB );
		WriteDIBtoBMP("Allex.DIBCopyToFrame.After Bit Mask.bmp", pDscDIB);
	}
	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CopyFromFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInReplace pFrameIn)
{
	uint32_t StartY = pFrameIn->dwY;
	uint32_t EndY   = StartY + pFrameIn->dwHeight;
	uint32_t nLine;
	uint32_t dLine = 0;

	if ( pSrcDIB->GetPixelSize() != pSrcDIB->GetPixelSize() )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	for ( nLine = StartY; nLine < EndY; nLine++)
	{
		// если ширина совпадает, то
		if ( pDscDIB->GetLineWidth() == pSrcDIB->GetLineWidth() )
		{
			if ( !pDscDIB->SetFuelLineFromDIB(pSrcDIB, dLine++, nLine, 0) )
			{
				SetReturnCode_cimage(IDS_CIMAGE_INVALID_FRAME_SIZE);
				return FALSE;
			}
		}
		else
		{
			continue;
		}
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::ApplayBitMaskToFrame(PCIMAGE_InfoDataInGet pIn, PCIMAGE_InfoDataOutGet pOut)
{

	if ( pIn->MaskFlag == 0 )
	{
		return TRUE;
	}
	else
	{
		uint32_t SizeInBytes;
		uint32_t i;
		char * pMask = (char *)pIn;
		char * pRaster = (char *)(pOut->lpData);
		char * Msc;
		char * Dsc;

		pMask += sizeof(CIMAGE_InfoDataInGet);

		switch (pOut->byBit)
		{

		case 1:

			SizeInBytes = (pOut->dwWidth) * (pOut->dwHeight);
			SizeInBytes += 7;
			SizeInBytes /= 8;

			Dsc = pRaster;
			Msc = pMask;

			for ( i = 0; i < (SizeInBytes - 1); i++ )
			{
				 *Dsc = (*(Dsc++) & *(Msc++));
			}

			break;

		default:
			SetReturnCode_cimage(IDS_CIMAGE_NO_SUPPORT_DIB_FORMAT_FOR_THIS_ACTION);
			return FALSE;

		}

	}

	return TRUE;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::ApplayBitMaskToDIB(puchar pMask, PCTDIB pDIB)
{
	if ( pMask == 0 )
	{
		return TRUE;
	}
	else
	{
		puchar pMaskByte = pMask; //(puchar)&pIn[1];
		puchar pImageByte;
		uint32_t Lines = pDIB->GetLinesNumber();
		uint32_t Pixels = pDIB->GetLineWidth();
		int32_t i;
		int32_t j;
		int32_t k;
		uchar  WhiteByte = (uchar)pDIB->GetWhitePixel();
		uchar  tmpByte;

		///////////////////////////////////////////////////////////////////////////////////////
		//Allex 28.01.2000
		//ввиду неоднократных ошибок, связанных с  выделением памяти под передаваемую мне маску
		//ввожу следующую проверку:
		uchar TestByte;
		uint32_t MaskLenght = ((Pixels + 7)/8)*Lines;
		puchar pMaskBegin = pMask;
		puchar pMaskEnd = pMaskBegin + MaskLenght - 1;

		try
		{
			TestByte = *pMaskBegin;
			*pMaskBegin = TestByte;
			TestByte = *pMaskEnd;
			*pMaskEnd = TestByte;
		}

		catch(...)
		{
			SetReturnCode_cimage(IDS_CIMAGE_INVALID_MASK_MEMORY);
			LDPUMA_Console("CImage: Mask unvailable or mask memory blok less then needed!\n        Mask not applied.\n");
			return TRUE;
		}
		// конец проверки
		///////////////////////////////////////////////////////////////////////////////////////

		for ( i = 0; i < (int32_t)Lines; i++ )
		{
			pImageByte = (puchar)pDIB->GetPtrToLine( i );
			j = Pixels;
			//позиционируемся на строку в маске
			pMaskByte = pMask + ((Pixels + 7)/8)*i;

			switch ( pDIB->GetPixelSize() )
			{
			case 1:
				while ( (j > 0) )
				{
					tmpByte = ~(*(pMaskByte));

					if ( WhiteByte == 1 )
					{
						*(pImageByte++) = *(pImageByte) | tmpByte;
					}
					else
					{
						*(pImageByte++) = *(pImageByte) & tmpByte;
					}

					pMaskByte++;
					j -= 8;
				}
				break;

			case 8:
				while ( j > 7 )
				{
					tmpByte = ~(*(pMaskByte));

					for ( k = 7; k >= 0; k--)
					{
						if ( tmpByte & mwIndexMask[k] )
						{
							*(pImageByte) = WhiteByte;
						}

						pImageByte++;
					}

					pMaskByte++;
					j -= 8;
				}

				k = 7;
				while ( j-- )
				{
					tmpByte = ~(*(pMaskByte));

					if ( tmpByte & mwIndexMask[k--] )
					{
						*(pImageByte) = WhiteByte;
					}
				}
					pMaskByte++;
				break;

			case 24:
				while ( j > 7 )
				{
					tmpByte = ~(*(pMaskByte));

					for ( k = 7; k >= 0; k--)
					{
						if ( tmpByte & mwIndexMask[k] )
						{
							*(pImageByte)     = WhiteByte;
							*(pImageByte + 1) = WhiteByte;
							*(pImageByte + 2) = WhiteByte;
						}

						pImageByte += 3;
					}

					pMaskByte++;
					j -= 8;

				}

				k = 7;
				while ( j-- )
				{
					tmpByte = ~(*(pMaskByte));

					if ( tmpByte & mwIndexMask[k--] )
					{
							*(pImageByte) = WhiteByte;
							*(pImageByte + 1) = WhiteByte;
							*(pImageByte + 2) = WhiteByte;
					}
				}
				break;

			default:
				SetReturnCode_cimage(IDS_CIMAGE_NOT_REALIZED);
				return FALSE;
			}
		}

		return TRUE;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::SetFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInReplace pIn)
{
	uint32_t wFrameSize;

	if ( pSrcDIB == NULL || pDscDIB == NULL || !pIn)
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}
	// проверяем на ненулевость
	wFrameSize = (pIn->dwHeight) * (pIn->dwWidth);

	if ( wFrameSize == 0 )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_FRAME_SIZE);
		return FALSE;
	}
	// Проверяем на соответствие размеров
	if ( ( pDscDIB->GetLineWidth()   < pSrcDIB->GetLineWidth() + pIn->dwX ) ||
		 ( pDscDIB->GetLinesNumber() < pSrcDIB->GetLinesNumber() + pIn->dwY ) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_FRAME_SIZE);
		return FALSE;
	}

	if ( !CopyFromFrame(pSrcDIB, pDscDIB, pIn) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CBImageOpen(PCIMAGE_ImageInfo lpImageInfo)
{
	uint32_t wFrgb;
	uint32_t wSrgb;
	Handle hDescDIB;
	//CTDIBRGBQUAD tmpQ;

#if defined( _TRASING ) & defined ( _DEBUG )
	if ( wCBStep != 0 )
	{
		TRACE("!!!CIMAGE!!! Повторный вызов CBImageOpen для картинки %s без переинициализации CIMAGE_CBImageRead!\n",mCBName);
	}
#endif

	wCBStep++;

#if defined( _TRASING ) & defined ( _DEBUG )
	if ( mCBWInProcess == TRUE )
	{
		TRACE("!!!CIMAGE!!! CIMAGE_CBImageRead( %s, ...) вызывается из CIMAGE_WriteCBImage( %s, ...)!\n",mCBName,mCBWName);
	}
#endif


	if ( !hCBImage )
	{
		return FALSE;
	}

	pCBImage = CIMAGELock(hCBImage);

	if ( mCBDestianationDIB != NULL || mCBSourceDIB != NULL )
	{
		return FALSE;
	}

	mCBSourceDIB       = new CTDIB;
	mCBDestianationDIB = new CTDIB;

	if ( mCBSourceDIB->SetDIBbyPtr(pCBImage) )
	{
		wCBBufferSize = mCBSourceDIB->GetUsedLineWidthInBytes();
		wCBWidth      = mCBSourceDIB->GetLineWidth();
		wCBStep++;

		CIMAGEComment("Temporary DIB - CBImageOpen");
		if ( mCBDestianationDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock) &&
			 (hDescDIB = mCBDestianationDIB->CreateDIBBegin(wCBWidth,1,mCBSourceDIB->GetPixelSize())) &&
			 mCBDestianationDIB->CreateDIBEnd() )
		{
			wCBLines	= mCBSourceDIB->GetLinesNumber();

			lpImageInfo->wImageWidth        = (uint16_t)mCBSourceDIB->GetLineWidth();
			lpImageInfo->wImageHeight       = (uint16_t)wCBLines;
			lpImageInfo->wImageByteWidth    = (uint16_t)wCBBufferSize;
			lpImageInfo->wImageDisplacement = 0;

			mCBSourceDIB->GetResolutionDPI(&wFrgb, &wSrgb);
			lpImageInfo->wResolutionX       = (uint16_t)wFrgb;
			lpImageInfo->wResolutionY       = (uint16_t)wSrgb;

			lpImageInfo->bFotoMetrics       = (uchar)mCBSourceDIB->GetWhitePixel();

			lpImageInfo->wAddX              = 0;
			lpImageInfo->wAddY              = 0;

			wCBStep++;
		}
	}


	wCBLine = 0;

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTIControl::CBImageRead(char*  lpBuff, uint32_t wMaxSize)
{
	CIMAGE_InfoDataInGet   InFrame;
	uint32_t                 LinesAtOnce;
	uint32_t                 nOutLine;
	uint32_t                 CopiedBytes = 0;
	char*                 pNextLineInBuffer = lpBuff;

#ifndef CIMAGE_CBR_ONE_LINE
	LinesAtOnce = wMaxSize / wCBBufferSize;
#else
	LinesAtOnce = 1;
#endif

	if ( pCBImage )
	{
		if ( !(wMaxSize < wCBBufferSize) )
		{
			InFrame.dwX        = 0;
			InFrame.dwWidth    = mCBDestianationDIB->GetLineWidth();
			InFrame.dwHeight   = mCBDestianationDIB->GetLinesNumber();
			InFrame.wByteWidth = (uint16_t)(wCBBufferSize);
			InFrame.MaskFlag   = 0;

			for ( nOutLine = 0;nOutLine < LinesAtOnce; nOutLine++ )
			{
				InFrame.dwY        = wCBLine;

				if ( mCBDestianationDIB->GetPtrToLine(0) != NULL )
				{
					if ( GetFrame(mCBSourceDIB, mCBDestianationDIB, &InFrame, NULL) )
					{
						wCBLine++;
						wCBStep+=10;
						memcpy(pNextLineInBuffer, mCBDestianationDIB->GetPtrToLine(0), wCBBufferSize );
						CopiedBytes+=wCBBufferSize;
						pNextLineInBuffer += wCBBufferSize;
					}
				}
			}
			return CopiedBytes;
		}
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CBImageClose(void)
{
	if ( hCBImage )
		CIMAGEUnlock(hCBImage);

	if ( pCBBuffer )
	{
		delete [] pCBBuffer;
		pCBBuffer = NULL;
	}

	if ( mCBSourceDIB )
	{
		delete mCBSourceDIB;
		mCBSourceDIB = NULL;
	}

	if ( mCBDestianationDIB )
	{
		mCBDestianationDIB->DestroyDIB();
		delete mCBDestianationDIB;
		mCBDestianationDIB = NULL;
	}

	hCBImage = NULL;
	pCBBuffer = NULL;
	wCBBufferSize = 0;
	wCBLines = 0;
	wCBLine  = 0;
	wCBWidth = 0;
	wCBStep  = 0;

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::FreeAlloced(Handle hDIB)
{
	if (hDIB == NULL)
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
		return FALSE;
	}

	if ( mlImages.FindHandle(hDIB) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
		return FALSE;
	}

	if ( hDIB == mhCopyedDIB )
		mhCopyedDIB = NULL;

	CIMAGEFree(hDIB);

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::DumpToFile(char* FileName, puchar pData, uint32_t Size)
{
	FILE * fDump;

	fDump = fopen(FileName, "wb");

	if ( fDump == NULL )
		return FALSE;

	fwrite(pData, Size, 1, fDump);
	fclose(fDump);
	return TRUE;
}

Bool32 CTIControl::GetDIBFromImage(char* lpName, PCIMAGE_InfoDataInGet lpIn, pchar *pDIB)
{
	Handle hImage = NULL;
	void * pImage = NULL;
	Bool32   bRet = FALSE;
	PCTDIB   pSrcDIB;
	uint32_t   wResolutionX;
	uint32_t   wResolutionY;
	CIMAGE_InfoDataInGet NewIn;

	// в случае повторного вызова предыдущий диб стирается
	FreeBuffers();

	// при первом вызове
	//берем указатель на картинку
	if ( !GetDIB(lpName, &hImage, TRUE ) )
	{
		return bRet;

	}

	pImage = CIMAGELock(hImage);

	if ( !pImage )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return bRet;
	}

	//инициализируем ctdib-чики
	pSrcDIB = new CTDIB;
	mpDIBFromImage = new CTDIB;

	if ( pSrcDIB->SetDIBbyPtr(pImage) &&
		 pSrcDIB->GetResolutionDPM(&wResolutionX, &wResolutionY) )
	{
		// проверяем размеры картинки
		if ( CheckInData(pSrcDIB, lpIn, &NewIn) )
		{

			if ( mpDIBFromImage->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock) )
			{
				CIMAGEComment("Temporary DIB - GetDIBFromImage");
				if ( mpDIBFromImage->CreateDIBBegin(NewIn.dwWidth, NewIn.dwHeight,pSrcDIB->GetPixelSize(),pSrcDIB->GetActualColorNumber()) &&
					 mpDIBFromImage->CopyPalleteFromDIB( pSrcDIB ) &&
					 mpDIBFromImage->SetResolutionDPM(wResolutionX, wResolutionY) &&
					 mpDIBFromImage->CreateDIBEnd() )
				{
					bRet = GetFrame(pSrcDIB, mpDIBFromImage, &NewIn, (NewIn.MaskFlag == TRUE ? (puchar)&lpIn[1] : NULL) );
				}
				else
					SetReturnCode_cimage(IDS_CIMAGE_UNABLE_CREATE_DIB);
			}
			else
				SetReturnCode_cimage(IDS_CIMAGE_UNABLE_COPY_DIB);
		}
		else
			SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
	}

	pSrcDIB->DestroyDIB();
	delete pSrcDIB;

	WriteDIBtoBMP("Allex.DIBFromImage.bmp", mpDIBFromImage);

	if ( mpDIBFromImage->GetDIBPtr( (void**)pDIB ) )
		return TRUE;

	*pDIB = NULL;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file

Bool32 CTIControl::FreeBuffers()
{
	Bool32 bCrashedDIB = FALSE;

	if ( mpBitFildFromImage != NULL )
	{
		CIMAGEFree(mpBitFildFromImage);
		mpBitFildFromImage = NULL;
	}

	if ( mhCopyedDIB )
	{
		//CIMAGEFree(mhCopyedDIB);
		mhCopyedDIB = NULL;
	}

	if ( mpDIBFromImage != NULL )
	{
		if ( mpDIBFromImage->DestroyDIB() )
			delete mpDIBFromImage;
		else
		{
			bCrashedDIB = TRUE;
			mwMemoryErrors++;
		}

		mpDIBFromImage = NULL;
		// даем знать, что мы его стерли
		//if ( pDIB != NULL )
		//	*pDIB = NULL;
		//return TRUE;
	}

	return TRUE;
}

Bool32 CTIControl::CheckInData(PCTDIB pDIB, PCIMAGE_InfoDataInGet lpIn, PCIMAGE_InfoDataInGet lpNewIn)
{
	int32_t * NewX = (int32_t *)(lpNewIn ? &lpNewIn->dwX : &lpIn->dwX);
	int32_t * NewY = (int32_t *)(lpNewIn ? &lpNewIn->dwY : &lpIn->dwY);
	int32_t * NewH = (int32_t *)(lpNewIn ? &lpNewIn->dwHeight : &lpIn->dwHeight);
	int32_t * NewW = (int32_t *)(lpNewIn ? &lpNewIn->dwWidth  : &lpIn->dwWidth);
	int32_t DIBW;  //= (int32_t)pDIB->GetLineWidth();
	int32_t DIBH;  //= (int32_t)pDIB->GetLinesNumber();

	if ( pDIB == NULL )
		return FALSE;

	DIBW  = (int32_t)pDIB->GetLineWidth();
	DIBH  = (int32_t)pDIB->GetLinesNumber();

	// копируем
	if ( lpNewIn )
	{
		memcpy( lpNewIn, lpIn, sizeof(CIMAGE_InfoDataInGet) );
	}
	//проверяем ширину
	if ( *NewW  < 0 )
	{
		*NewW = DIBW;
	}
	// и высоту
	if ( *NewH  < 0 )
	{
		*NewH = DIBH;
	}

	//проверяем X
	if ( *NewX < 0 )
	{
		if ( *NewW > -*NewX )
			*NewW = *NewW + *NewX;

		*NewX = 0;
	}

	if ( *NewX > DIBW )
	{
		if ( *NewW < DIBW )
			*NewX = DIBW - *NewW;
		else
			*NewX = 0;
	}
	// проверяем Y
	if ( *NewY < 0 )
	{
		if ( *NewH > -*NewY )
			*NewH = *NewH + *NewY;

		*NewY = 0;
	}

	if ( *NewY > DIBH )
	{
		if ( *NewH < DIBH )
			*NewY = DIBH - *NewH;
		else
			*NewY = 0;
	}
	// проверяем размеры
	if ( (*NewX + *NewW) > DIBW )
	{
		*NewW = DIBW - *NewX;
	}

	if ( (*NewY + *NewH) > DIBH )
	{
		*NewH = DIBH - *NewY;
	}

	return TRUE;
}

Bool32 CTIControl::WriteDIBtoBMP(const char *cName, PCTDIB pDIB)
{
#ifdef CIMAGE_DUMP_ENABLE

	uint32_t  wBMPSize = pDIB->GetDIBSize() + 14;
	PumaMemoryToFileDumper BMPDump(cName);

	BMPDump.AddDump("BM",2);
	BMPDump.AddDump((char *)&wBMPSize,4);
	wBMPSize = 0;
	BMPDump.AddDump((char *)&wBMPSize,4);
	wBMPSize = 14 + pDIB->GetHeaderSize() + pDIB->GetRGBPalleteSize();
	BMPDump.AddDump((char *)&wBMPSize,4);
	BMPDump.AddDump(pDIB->GetPtrToHeader(), pDIB->GetDIBSize());
	BMPDump.EndDump();

#endif

	return TRUE;
}

Bool32 CTIControl::AddWriteRectangles(char* lpName, uint32_t wNumber, PCIMAGE_Rect pFirst)
{
	return AddRectsToMask(lpName, wNumber, pFirst, "w");
}

Bool32 CTIControl::RemoveWriteRectangles(char* lpName, uint32_t wNumber, PCIMAGE_Rect pFirst)
{
	return RemoveRectsFromMask(lpName, wNumber, pFirst, "w");
}

Bool32 CTIControl::AddReadRectangles(char* lpName, uint32_t wNumber, PCIMAGE_Rect pFirst)
{
	return AddRectsToMask(lpName, wNumber, pFirst, "r");
}

Bool32 CTIControl::RemoveReadRectangles(char* lpName, uint32_t wNumber, PCIMAGE_Rect pFirst)
{
	return RemoveRectsFromMask(lpName, wNumber, pFirst, "r");
}

Bool32 CTIControl::OpenDIBFromList(char* lpName, Handle* phImage)
{
	mlImages.GetImage(lpName, phImage);
	//ALLEX Mask
	// ошибка не тут
	OpenMaskFromList(lpName, &(mpcSrcDIBReadMask = NULL), &mbEnableDIBReadMask, "r");
	OpenMaskFromList(lpName, &(mpcSrcDIBWriteMask = NULL), &mbEnableDIBWriteMask, "w");

	if ( !(*phImage) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	return TRUE;
}

Bool32 CTIControl::OpenMaskFromList(const char *lpName, PPCTIMask ppMask, PBool32 pEnMask, const char *pcType)
{
	Bool32 bRet;

	if ( pcType[0] == 'r' )
		bRet = mlImages.GetImageReadMask(lpName, ppMask, pEnMask);
	else
		if ( pcType[0] == 'w' )
			bRet = mlImages.GetImageWriteMask(lpName, ppMask, pEnMask);

	if ( !bRet )
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);

	return bRet;
}

Bool32 CTIControl::SetMaskToList(char* pName, PCTIMask pMask, char* pcType)
{
	Bool32 bRet;

	if ( pcType[0] == 'r' )
		bRet = mlImages.SetImageReadMask(pName, pMask);
	else
		if ( pcType[0] == 'w' )
			bRet = mlImages.SetImageWriteMask(pName, pMask);

	if ( !bRet )
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);

	return bRet;
}

Bool32 CTIControl::OpenDIBFromList(const char *lpName, PCTDIB pcDIB)
{
	Handle hDIB;

	mlImages.GetImage(lpName, &hDIB);
	//ALLEX Mask
	// ошибка не тут
	OpenMaskFromList(lpName, &(mpcSrcDIBReadMask = NULL), &mbEnableDIBReadMask, "r");
	OpenMaskFromList(lpName, &(mpcSrcDIBWriteMask = NULL), &mbEnableDIBWriteMask, "w");

	if ( !hDIB )
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}
	pcDIB->SetDIBHandle(hDIB);
	return pcDIB->SetDIBbyPtr(CIMAGELock(hDIB));
}

Bool32 CTIControl::CloseDIBFromList(PCTDIB pDIB)
{
	Handle hDIB;

	if ( pDIB->GetDIBHandle( &hDIB ) )
		CIMAGEUnlock(hDIB);

	pDIB->DestroyDIB();

	return TRUE;
}

Bool32 CTIControl::AddRectsToMask(const char *lpName, uint32_t wNumber, PCIMAGE_Rect pFirstRect, const char *pcType)
{
	Bool32 bRet = FALSE;
	PCTIMask pMask;
	Bool32  bInvalidMask = FALSE;
	uint32_t  wRect;
	Bool32  bMaskEnabled;

	if ( !OpenMaskFromList(lpName, &pMask, &bMaskEnabled, pcType) )
	{
		return FALSE;
	}

	if ( pMask == NULL )
	{
		CTDIB   DIB;

		if ( !OpenDIBFromList(lpName, &DIB) )
			return FALSE;

		pMask = new CTIMask(DIB.GetLineWidth(), DIB.GetLinesNumber());
		CloseDIBFromList(&DIB);

		if ( !mlImages.SetImageReadMask(lpName, pMask) )
		{
			delete pMask;
			return FALSE;
		}
	}

	for ( wRect = 0; wRect < wNumber; wRect++ )
	{
		if ( pMask->AddRectangle(&pFirstRect[wRect]) )
			continue;

		bInvalidMask = TRUE;
		break;
	}

	bRet = (wRect == wNumber);
	return bRet;
}

Bool32 CTIControl::RemoveRectsFromMask(const char *lpName, uint32_t wNumber, PCIMAGE_Rect pFirstRect, const char *pcType)
{
	Bool32 bRet = true;
	PCTIMask pMask;
	Bool32  bInvalidMask = FALSE;
	uint32_t  wRect;
	Bool32  bMaskEnabled;

	if ( !OpenMaskFromList(lpName, &pMask, &bMaskEnabled, pcType) )
	{
		return FALSE;
	}

	if ( pMask == NULL )
	{
		return FALSE;
	}

	for ( wRect = 0; wRect < wNumber; wRect++ )
	{
		if ( pMask->RemoveRectangle(&pFirstRect[wRect]) )
			continue;

		bInvalidMask = TRUE;
		break;
	}

	return bRet;
}

Bool32 CTIControl::ApplayMaskToDIB(PCTDIB pDIB, PCTIMask pMask, uint32_t wAtX, uint32_t wAtY)
{
	uint32_t wX, wY;
	uint32_t wXb, wXe;
	uint32_t wYb, wYe;
	uint32_t wSegmentsOnLine;
	PCTIMaskLine  pcMaskLine;

	if ( !pDIB )
		return FALSE;

	wXe = (wXb = wAtX) + pDIB->GetLineWidth();
	wYe = (wYb = wAtY) + pDIB->GetLinesNumber();

	CIMAGE_Rect cRectAllDIB = {wXb, wYb, wXe - wXb, wYe - wYb};

	if ( !pMask->IsRectOnMask( &cRectAllDIB ) )
		return FALSE;

	for ( wY = wYb; wY < wYe; wY++ )
	{
		//если удалось открыть маску
		if ( pMask->GetLine(wY, &pcMaskLine ) )
		{
			// если есть линия
			if ( pcMaskLine )
			{
				wSegmentsOnLine = pcMaskLine->GetSegmentsNumber();
				wX = wXb;

				// если есть сегменты на этой линии
				if ( wSegmentsOnLine )
					while ( wX < wXe )
					{
						CTIMaskLineSegment Segm(wX, wXe);

						if ( pcMaskLine->GetLeftIntersection( &Segm ) )
						{
							wX = Segm.GetEnd() + 1;
							if ( !ApplayMaskToDIBLine( pDIB, &Segm, wY, wAtX, wAtY) )
							{
								SetReturnCode_cimage(IDS_CIMAGE_UNABLE_APPLAY_MASK);
								return FALSE;
							}
						}
						else
							break;
					}
			}
		}
	}

	return TRUE;
}

Bool32 CTIControl::ApplayMaskToDIBLine(PCTDIB pcDIB, PCTIMaskLineSegment pSegm, uint32_t wLine, uint32_t wAtX, uint32_t wAtY)
{
	uint32_t wBitCount;
	Bool32 bRet = FALSE;
	uint32_t wXb;
	uint32_t wXe;
	uint32_t wY;
	puchar pPixB;
	puchar pPixE;
	uchar  wWPLA;
	uint32_t wSegmLenght;
	uint32_t wLAStartPos;
	uint32_t wLAEndPos;
	int32_t wLAFullBits;

	if ( ( pSegm->GetStart() > (int32_t)(pcDIB->GetLineWidth() + wAtX )) ||
		 ( pSegm->GetEnd() > (int32_t)(pcDIB->GetLineWidth() + wAtX )) ||
		 ( wLine >= pcDIB->GetLinesNumber() + wAtY ) )
		 return FALSE;

	wBitCount = pcDIB->GetPixelSize();
	wXe = pSegm->GetEnd() - wAtX;
	wXb = pSegm->GetStart() - wAtX;
	wY  = wLine - wAtY;

	// проверяем на соответствие с картинкой
	if ( wXb >= pcDIB->GetLineWidth() )
		return true;

	if ( wXe >= pcDIB->GetLineWidth() )
		wXe = pcDIB->GetLineWidth() - 1;

	pPixB = (puchar)pcDIB->GetPtrToPixel(wXb, wY);
	pPixE = (puchar)pcDIB->GetPtrToPixel(wXe, wY);

	if ( pPixB == NULL || pPixE == NULL )
	{
		return bRet;
	}


	switch ( wBitCount )
	{
	case 1:
		wLAStartPos = pcDIB->GetPixelShiftInByte(wXb);
		wLAEndPos   = pcDIB->GetPixelShiftInByte(wXe);
		wWPLA       = (uchar)pcDIB->GetWhitePixel();
		wLAFullBits = pPixE - pPixB - 1;

		if ( wLAFullBits > 0 )
		{
			if ( wWPLA == 0 )
				CIMAGE_MEMSET(pPixB + 1, 0x00, wLAFullBits );
			else
				CIMAGE_MEMSET(pPixB + 1, 0xff, wLAFullBits );
		}

		if ( wWPLA == 0 )
		{
			*pPixB = *pPixB & mwLABlackRightMask[wLAStartPos];
			*pPixE = *pPixE & mwLABlackLeftMask[wLAEndPos];
		}
		else
		{
			*pPixB = *pPixB | mwLAWhiteRightMask[wLAStartPos];
			*pPixE = *pPixE | mwLAWhiteLeftMask[wLAEndPos];
		}
		bRet = true;
		break;
	case 4:
		break;
	case 8:
	case 16:
	case 24:
	case 32:
		wSegmLenght = ((pSegm->GetEnd() - pSegm->GetStart() ) * wBitCount ) / 8;
		CIMAGE_MEMSET( pPixB, (uchar)pcDIB->GetWhitePixel(), wSegmLenght );
		bRet = TRUE;
		break;
		//wSegmLenght = ((pSegm->GetEnd() - pSegm->GetStart() + 1) * wBitCount ) / 8;
		//wWP24 = (uchar)pcDIB->GetWhitePixel();
		//memset( pPix, wWP24, wSegmLenght );
		//bRet = TRUE;
		//break;
	}

	return bRet;
}

Bool32 CTIControl::EnableMask(char* pcName, char* pcType, Bool32 bEnable)
{
	return mlImages.EnableMask(pcName, pcType, bEnable);
}
