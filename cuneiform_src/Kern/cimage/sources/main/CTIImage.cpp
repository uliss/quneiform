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

#include "resource.h"
#include "ctidefines.h"
#include "ctiimage.h"
#include "ctimemory.h"

#include "cticontrol.h"

// exteren globals
extern CTIControl * Control_cti;
// extern functions
void SetReturnCode_cimage(uint16_t rc);
uint16_t GetReturnCode_cimage();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_WriteCallbackImage(puchar                 lpName,
											  CIMAGEIMAGECALLBACK  Cbk)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->WriteCBImage( (char*)lpName, Cbk );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetCallbackImage(puchar                  lpName,
											PCIMAGEIMAGECALLBACK  pCbk)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetCBImage( (char*)lpName, pCbk );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_WriteDIB(puchar    lpName,
									Handle  lpDIB,
									uint32_t  wFlag)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->SetDIB( (char*)lpName, lpDIB, wFlag );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_ReadDIB(puchar     lpName,
								   Handle*  lplpDIB,
								   uint32_t   wFlag)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetDIB( (char*)lpName, lplpDIB, wFlag );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetData(puchar                     lpName,
								   PCIMAGE_InfoDataInGet       lpIn,
								   PCIMAGE_InfoDataOutGet      lpOut)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetImage( (char*)lpName, lpIn, lpOut );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetDIBData(puchar                      lpName,
								      PCIMAGE_InfoDataInGet       lpIn,
								      pchar                       *lpDIB)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetDIBFromImage( (char*)lpName, lpIn, lpDIB );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_ReplaceData(puchar                        lpName,
									   PCIMAGE_InfoDataInReplace   lpIn )
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->ReplaceImage( (char*)lpName, lpIn );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetImageInfo(puchar                     lpName,
										PCIMAGEBITMAPINFOHEADER  lpBIH)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetImageInfo( (char*)lpName, lpBIH );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_DeleteImage( puchar  lpName)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->RemoveImage( (char*)lpName );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_FreeCopedDIB(Handle hDIB)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->FreeAlloced( hDIB );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_FreeBuffers()
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->FreeBuffers();

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_Reset(void)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
	{
		delete Control_cti;

		Control_cti = new CTIControl;

		if ( Control_cti )
			return TRUE;
		return FALSE;
	}

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_AddReadCloseRects(puchar lpName, uint32_t wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->AddReadRectangles( (char*)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_RemoveReadCloseRects(puchar lpName, uint32_t wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->RemoveReadRectangles( (char*)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_AddWriteCloseRects(puchar lpName, uint32_t wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->AddWriteRectangles( (char*)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_RemoveWriteCloseRects(puchar lpName, uint32_t wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->RemoveWriteRectangles( (char*)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_EnableMask(puchar lpName, puchar lpType, Bool32 bEnabler)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->EnableMask((char*)lpName, (char*)lpType, bEnabler);

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// For GetCBImage
CIMAGE_CALLBACK_FUNC(CIMAGEBOOL16) CIMAGE_Callback_ImageOpen(PCIMAGE_ImageInfo lpImageInfo)
{
	Bool16 A = FALSE;

	if ( !Control_cti )
	{
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
		return FALSE;
	}

	A = (CIMAGEBOOL16)(Control_cti->CBImageOpen(lpImageInfo));

	return A;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_CALLBACK_FUNC(CIMAGEWORD)   CIMAGE_Callback_ImageRead(pchar  lpImage, uint16_t wMaxSize)
{
	if ( !Control_cti )
	{
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
		return 0;
	}

	return (CIMAGEWORD)(Control_cti->CBImageRead((char*)lpImage, wMaxSize));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_CALLBACK_FUNC(CIMAGEBOOL16) CIMAGE_Callback_ImageClose(void)
{
	if ( !Control_cti )
	{
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
		return FALSE;
	}

	return (CIMAGEBOOL16)(Control_cti->CBImageClose());
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
