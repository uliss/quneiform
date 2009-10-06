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
//   Использовать
//   Версии:     CImage.dll - 31 Jan 2000
//               RDIB.dll   - 31 Jan 2000
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "cricontrol.h"
#include "crimemory.h"
/////////////////////////////////////////////////
//#define RIMAGE_DUMP_ENABLE

#ifdef RIMAGE_DUMP_ENABLE
#include "PumaMemoryToFileDumper.h"
#endif
//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CRIControl::CRIControl()
{
	mpSourceDIB      = NULL;
	mpDestinationDIB = NULL;
	mpBinarizator    = NULL;
	mpTurner         = NULL;
	mpInvertor       = NULL;
	mpRotator        = NULL;
	mp_TurnedDIB     = NULL;
	mbMarginsFlag    = FALSE;
	DIBOpeningType   = FALSE;
	mcLastDIBName[0] = 0x0;

	/*
#define DecFunction(a) FNCIMAGE##a a; if(!CIMAGE_GetExportData(CIMAGE_FN_##a, &a)) \
				printf("CIMAGE_GetExportData == FALSE : %-3i,%s \n",a,CIMAGE_GetReturnString(CIMAGE_GetReturnCode()));
// \
	DecFunction(GetData);
	DecFunction(ReplaceData);
	DecFunction(GetCallbackImage);
	DecFunction(WriteCallbackImage);
	DecFunction(WriteDIB);
	DecFunction(ReadDIB);
	DecFunction(GetImageInfo);
	DecFunction(DeleteImage);
	DecFunction(FreeCopedDIB);

#undef  DecFunction
  */
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CRIControl::~CRIControl()
{
	if ( mpBinarizator )
		delete mpBinarizator;

	if ( mpTurner )
		delete mpTurner;

	if ( mpInvertor )
		delete mpInvertor;

	if ( mpRotator )
		delete mpRotator;

	if ( mpDestinationDIB )
		delete mpDestinationDIB;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRIControl::SetMargins(PRIMAGEMARGINS pMargins)
{
	SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRIControl::Binarise(char* cDIBIn, char* cDIBOut, uint32_t wFlag, uint32_t UseMargins)
{
	Bool32 Ret = TRUE;
	CTBinarize bType = CTBIN_UNKNOWN;
	// открываем исходный
	if ( !OpenSourceDIB(cDIBIn) )
	{
		return FALSE;
	}

		/*
    if ( !SupportedIndexColorImage(mpSourceDIB) )
    {
        CloseSourceDIB();
		SetReturnCode_rimage(IDS_RIMAGE_NOTSUPPORTED_INDEX_COLOR);
		return FALSE;
    }
		*/
	// создаем новый
	if ( ! CreateDestinatonDIB(1) )           // create DIB 1 bit per pixel
	{
        CloseSourceDIB();
		SetReturnCode_rimage(IDS_RIMAGE_CANNOT_CREATE_NEW_DIB);
		return FALSE;
	}
	//открываем бинаризатор
	if ( !mpBinarizator )
	{
		mpBinarizator = new CRIBinarizator(&mcProgress);
	}
	// закидываем туда картинки
	if ( !mpBinarizator->SetRasters(mpSourceDIB, mpDestinationDIB) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_CANNOT_SET_DIB);
		Ret = FALSE;
	}

	if ( wFlag < 4 )
	{
		bType = CTBIN_DEZA;
	}
	else
	{
		bType = CTBIN_KRONROD;
	}
	// бинаризуем
	if ( !mpBinarizator->Binarize(bType, wFlag) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_CANT_BINARYZE);
		Ret = FALSE;
	}
	//отписваем новый в контейнер и освобождаем
	if ( !CloseDestinationDIB(cDIBOut) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
		Ret = FALSE;
	}

	//закрываем исходный
	if ( !CloseSourceDIB() )
	{
		SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
		Ret = FALSE;
	}
	return Ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRIControl::Rotate(char*  cDIBIn, char*  cDIBOut, int32_t High, int32_t Low, uint32_t UseMargins)
{
	Bool32 Ret = TRUE;
	Bool32 NoDest = FALSE;

	// открываем исходный
	if ( !OpenSourceDIB(cDIBIn) )
	{
		return FALSE;
	}

	if ( mpDestinationDIB )
	{
		SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
		return FALSE;
	}

	mpDestinationDIB = new CTDIB;

	//открываем вертелку
	if ( !mpRotator )
	{
		mpRotator = new CRRotator(&mcProgress);
	}

	// забываем старое имя
	mcLastDIBName[0] = 0x0;

	if ( !mpRotator->Rotate(mpSourceDIB, mpDestinationDIB, High, Low) )
	{
		uint16_t wRet = GetReturnCode_rimage();
// !!! Art Изменил - теперь она заносит не хендлы, а указатели, а то память утекала
//почему-то...
		//		Handle hDIBtoSet;
		pvoid  pDIBtoSet;

		if ( (wRet == IDS_RIMAGE_ZERO_NUMERATOR_OR_DENUMERATOR ||
			  wRet == IDS_RIMAGE_ANGLE_LEAST_MINIMUM )         &&
			  mpSourceDIB->GetDIBPtr( &pDIBtoSet )
			  /*mpSourceDIB->GetDIBHandle(&hDIBtoSet)*/ )
		{
//			SetDIB(cDIBOut, hDIBtoSet);
			WriteDIB(cDIBOut, pDIBtoSet);
			SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);
			NoDest = Ret = TRUE;
		}
		else
		{
			SetReturnCode_rimage(IDS_RIMAGE_CANNOT_ROTATE_IMAGE);
			Ret = FALSE;
		}
	}
	else
	{
		WriteDIBtoBMP("Allex.DIBBeforeDeskew.bmp",mpSourceDIB);
		WriteDIBtoBMP("Allex.DIBAfterDeskew.bmp",mpDestinationDIB);
	}

	//отписваем новый в контейнер и освобождаем
	if ( !CloseDestinationDIB(cDIBOut) )
	{
		if ( NoDest == FALSE )
		{
			SetReturnCode_rimage(IDS_RIMAGE_CANNT_SAVE_OUTCOMING_DIB);
			Ret = FALSE;
		}
	}

	strcpy(mcLastDIBName, cDIBOut);

	//закрываем исходный
	if ( !CloseSourceDIB() )
	{
		SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
		Ret = FALSE;
	}
	return Ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRIControl::Turn(char*  cDIBIn, char*  cDIBOut, uint32_t wFlag, uint32_t UseMargins)
{
	int32_t     NewWidth;
	int32_t     NewHeight;
	Bool32    bRet = FALSE;
	//void *   pSDIB;

	if ( wFlag != RIMAGE_TURN_90 && wFlag != RIMAGE_TURN_270 && wFlag != RIMAGE_TURN_180 )
	{
		SetReturnCode_rimage(IDS_RIMAGE_INVALID_FUNCTION_PARAMETR);
		return FALSE;
	}

	if ( mp_TurnedDIB != NULL )
	{
		SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
		return FALSE;
	}
	// открываем исходный
	if ( !OpenSourceDIB(cDIBIn) )
	{
		return FALSE;
	}
	NewWidth = ( wFlag == RIMAGE_TURN_180 ? mpSourceDIB->GetImageWidth() : mpSourceDIB->GetImageHeight() );
	NewHeight = ( wFlag == RIMAGE_TURN_180 ? mpSourceDIB->GetImageHeight() : mpSourceDIB->GetImageWidth() );
	/*
	if ( !mpSourceDIB->GetDIBPtr( &pSDIB ) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_INVALID_EXTERNAL_DIB);
		return FALSE;
	}
	*/
	//открываем вертелку
	if ( !mpTurner )
	{
		mpTurner = new CRTurner;
	}

	// генерим новенький
	if ( mpDestinationDIB )
	{
		SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
		return FALSE;
	}

	mpDestinationDIB = new CTDIB;
	RIMAGEComment("Turn - temporary destination DIB");
	mpDestinationDIB->SetExternals( RIMAGEAlloc, RIMAGEFree, RIMAGELock, RIMAGEUnlock);

	if ( mpDestinationDIB->CreateDIBBegin(NewWidth,
		                                  NewHeight,
										  mpSourceDIB->GetPixelSize()) &&
	     mpDestinationDIB->CopyPalleteFromDIB(mpSourceDIB) &&
		 mpDestinationDIB->CopyDPIFromDIB(mpSourceDIB) &&
		 mpDestinationDIB->CreateDIBEnd()  )
	{
		bRet = mpTurner->TurnDIB(mpSourceDIB, mpDestinationDIB, wFlag );
	}



	// вертим
	/*
	if ( !(mp_TurnedDIB = mpTurner->TurnDIB(pSDIB, wFlag)) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_CANT_TURN_DIB);
		return FALSE;
	}
	*/

	/*
	//отписваем новый в контейнер и освобождаем
	if ( !WriteDIB(cDIBOutt, mp_TurnedDIB) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_UNABLE_WRITE_DIB);
		return FALSE;
	}

	if ( !mpTurner->FreeDIB(mp_TurnedDIB) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_INVALID_EXTERNAL_DIB);
		return FALSE;
	}

	mp_TurnedDIB = NULL;
	*/
	//отписваем новый в контейнер и освобождаем
	if ( !CloseDestinationDIB(cDIBOut) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_CANNT_SAVE_OUTCOMING_DIB);
		bRet = FALSE;
	}
	//закрываем исходный
	if ( !CloseSourceDIB() )
	{
		SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
		bRet = FALSE;
	}
	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRIControl::Inverse(char*  cDIBIn, char*  cDIBOut, uint32_t UseMargins)
{
	Bool32    bErrors = TRUE;
	// копируем из исходного DIB в обрабатываемый
	// получаем заполненный mpDescinationDIB и пустой mpSourceDIB
	if ( !OpenDestinationDIBfromSource( cDIBIn ) )
	{
		return FALSE;
	}

	//открываем инвертор
	if ( !mpInvertor )
	{
		mpInvertor = new CRInvertor;
	}

	// Инвертируем
	if ( !mpInvertor->Inverse(mpDestinationDIB) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_CANNOT_INVERT_IMAGE);
		bErrors = FALSE;
	}

	//отписваем новый в контейнер и освобождаем
	if ( !SetDestinationDIBtoStorage(cDIBOut) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_UNABLE_WRITE_DIB);
		bErrors = FALSE;
	}

	return bErrors;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// взять без копировыания
Bool32 CRIControl::GetDIB(char*  cDIB, Handle* phDIB)
{
	// берем с копированием, что б маска была!
	if ( CIMAGE_ReadDIB((puchar)cDIB, phDIB, TRUE) )
		return TRUE;

	SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
	return FALSE;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// положить без копировыания
Bool32 CRIControl::SetDIB(char*  cDIB, Handle hDIB)
{
	if ( CIMAGE_WriteDIB((puchar)cDIB, hDIB, TRUE) )
		return TRUE;

	SetReturnCode_rimage(IDS_RIMAGE_UNABLE_WRITE_DIB);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// положитьь c копировыанием
Bool32 CRIControl::WriteDIB(char*  cDIB, Handle hDIB)
{
	if ( CIMAGE_WriteDIB((puchar)cDIB, hDIB, FALSE) )
		return TRUE;

	SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// взять с копировыанием
Bool32 CRIControl::ReadDIB(char*  cDIB, Handle* phDIB)
{
	if ( CIMAGE_ReadDIB((puchar)cDIB, phDIB, FALSE) )
		return TRUE;

	SetReturnCode_rimage(IDS_RIMAGE_UNABLE_WRITE_DIB);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Close Source DIB
Bool32 CRIControl::CloseSourceDIB()
{

	Handle    hDIB;

	if ( mpSourceDIB == NULL )
	{
		SetReturnCode_rimage(IDS_RIMAGE_NOT_OPENED);
		return FALSE;
	}

	if ( mpSourceDIB->GetDIBHandle(&hDIB) )
	{
		RIMAGEUnlock(hDIB);
	}

	delete mpSourceDIB;
	CIMAGE_FreeCopedDIB(hDIB);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRIControl::OpenSourceDIB(char*  cDIBName)
{
	Handle  hDIBIn;
	pvoid   pDIB;

	if ( !ReadDIB(cDIBName, &hDIBIn) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	if ( !(pDIB = RIMAGELock(hDIBIn) ) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
		return FALSE;
	}

	mpSourceDIB = new CTDIB(hDIBIn);

	if ( !mpSourceDIB->SetDIBbyPtr(pDIB) )
	{
		delete mpSourceDIB;
		mpSourceDIB = NULL;
		SetReturnCode_rimage(IDS_RIMAGE_DIB_NOT_ATTACHED);
		return FALSE;
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Создаем временный DIB куда отпишем, что получили
Bool32 CRIControl::CloseDestinationDIB(char*  cDIBName)
{
	Handle hDIB = NULL;
	pvoid  pDIB = NULL;

	if ( DIBOpeningType == TRUE )
	{
		return SetDestinationDIBtoStorage(cDIBName);
	}

	if ( !mpDestinationDIB )
		return FALSE;

	if ( !mpDestinationDIB->GetDIBHandle(&hDIB) ||
		 !mpDestinationDIB->GetDIBPtr( &pDIB )     )
	{
		delete mpDestinationDIB;
		mpDestinationDIB = NULL;
		return FALSE;
	}

	if ( !WriteDIB(cDIBName, pDIB) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	/*
	while ( mpDestinationDIB )
	{
		if ( mpDestinationDIB->DestroyDIB() )
		{
			hDIB = NULL;
		}
		else
		{
			if ( mpDestinationDIB->GetDIBHandle(&hDIB) )
			{
				RIMAGEUnlock(hDIB);
				RIMAGEFree(hDIB);
			}
			else
			{
				delete mpDestinationDIB;
				mpDestinationDIB = NULL;
			}
		}
	}
	*/
	delete mpDestinationDIB;
	mpDestinationDIB = NULL;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creating new DIB by CTDIB class and 4 RIMAGE functions
Bool32 CRIControl::CreateDestinatonDIB(uint32_t BitCount)
{
	uint32_t wNewHeight;
	uint32_t wNewWidth;
	uint32_t wXResolution;
	uint32_t wYResolution;
	CTDIBRGBQUAD       BWQuads[2] = {{0x00,0x00,0x00,0x00},{0xff,0xff,0xff,0x00} };


	if ( mpSourceDIB == NULL )
	{
		SetReturnCode_rimage(IDS_RIMAGE_NOT_OPENED);
		return FALSE;
	}

	mpDestinationDIB = new CTDIB;

	RIMAGEComment("CreateDestinationDIB - temporary DIB");
	if ( !mpDestinationDIB->SetExternals(RIMAGEAlloc, RIMAGEFree, RIMAGELock, RIMAGEUnlock) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_PREPARE_TO_CREATE);
		delete mpDestinationDIB;
		mpDestinationDIB = NULL;
		return FALSE;
	}

	wNewHeight = (mbMarginsFlag ? abs(mrMargins.rmBottomMarg - mrMargins.rmTopMarg) : mpSourceDIB->GetLinesNumber());
	wNewWidth = (mbMarginsFlag ? abs(mrMargins.rmLeftMarg - mrMargins.rmRightMarg) : mpSourceDIB->GetLineWidth());
	mpSourceDIB->GetResolutionDPM( &wXResolution, &wYResolution);

	if ( !mpDestinationDIB->CreateDIBBegin( wNewWidth, wNewHeight, BitCount) )
	{
		return FALSE;
	}

	if ( !mpDestinationDIB->SetResolutionDPM(wXResolution, wYResolution) )
	{
		//return FALSE;
	}

	if ( !mpDestinationDIB->SetRGBQuad(0, BWQuads[0] ) )
	{
		return FALSE;
	}

	if ( !mpDestinationDIB->SetRGBQuad(1, BWQuads[1] ) )
	{
		return FALSE;
	}

	if ( !mpDestinationDIB->CreateDIBEnd() )
	{
		return FALSE;
	}
	return TRUE;
}

Bool32 CRIControl::OpenDestinationDIBfromSource(char*  cDIBName)
{
	Handle  hDIBIn;
	pvoid   pDIB;

	if ( mpSourceDIB != NULL )
		return FALSE;

	if ( !ReadDIB(cDIBName, &hDIBIn) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	if ( !(pDIB = RIMAGELock(hDIBIn) ) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
		return FALSE;
	}

	mpDestinationDIB = new CTDIB(hDIBIn);

	if ( !mpDestinationDIB->SetDIBbyPtr(pDIB) )
	{
		delete mpSourceDIB;
		mpDestinationDIB = NULL;
		SetReturnCode_rimage(IDS_RIMAGE_DIB_NOT_ATTACHED);
		return FALSE;
	}

	DIBOpeningType = TRUE;
	return TRUE;
}

Bool32 CRIControl::SetDestinationDIBtoStorage(char*  cDIBName)
{
	Handle hSDIB;
	Bool32  bErrors = TRUE;

	if ( DIBOpeningType == FALSE)
	{
		return CloseDestinationDIB(cDIBName);
	}
	// записывваем в блок выделеный в CIMAGE при открытии
	if ( mpDestinationDIB == NULL )
		return FALSE;

	if ( !mpDestinationDIB->GetDIBHandle(&hSDIB) )
		return FALSE;

	if ( !SetDIB(cDIBName, hSDIB ) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}


	if ( !CIMAGE_FreeCopedDIB(hSDIB) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_CIMAGE_MEMORY_ERROR);
		bErrors = FALSE;
	}

	delete mpDestinationDIB;
	mpDestinationDIB = NULL;
	DIBOpeningType = FALSE;

	return bErrors;
}

Bool32 CRIControl::Roll(char* cDIBIn, char* cDIBOut, int32_t Num, int32_t Denum, uint32_t bUseMargins)
{
	Bool32 Ret = TRUE;

	// открываем исходный
	if ( !OpenSourceDIB(cDIBIn) )
	{
		return FALSE;
	}

	if ( mpDestinationDIB )
	{
		SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
		return FALSE;
	}
	mpDestinationDIB = new CTDIB;

	//открываем вертелку
	if ( !mpRotator )
	{
		mpRotator = new CRRotator;
	}

	if ( !mpRotator->Roll(mpSourceDIB, mpDestinationDIB, Num, Denum) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_CANNOT_ROTATE_IMAGE);
		Ret = FALSE;
	}

	//отписваем новый в контейнер и освобождаем
	if ( !CloseDestinationDIB(cDIBOut) )
	{
		SetReturnCode_rimage(IDS_RIMAGE_CANNT_SAVE_OUTCOMING_DIB);
		Ret = FALSE;
	}

	//закрываем исходный
	if ( !CloseSourceDIB() )
	{
		SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
		Ret = FALSE;
	}
	return Ret;
}

Bool32 CRIControl::SetProgressCallBacks(PRIMAGECBPRogressStart pcbStart, PRIMAGECBPRogressStep pcbStep, PRIMAGECBPRogressFinish pcbFinish)
{
	return mcProgress.SetExternals((CRPStart)pcbStart, (CRPFinish)pcbFinish, (CRPStep)pcbStep);
}

Bool32 CRIControl::StartProgress()
{
	return mcProgress.Start();
}

Bool32 CRIControl::RotatePoint(char* cDIB, int32_t iX, int32_t iY, int32_t * prX, int32_t * prY)
{
	Bool32 bRet = FALSE;

	if ( mpRotator && strcmp(cDIB, mcLastDIBName) == 0 )
	{
		bRet = mpRotator->RotatePoint(iX, iY, prX, prY);
	}

	return bRet;
}

Bool32 CRIControl::WriteDIBtoBMP(const char *cName, PCTDIB pDIB)
{
#ifdef RIMAGE_DUMP_ENABLE

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
