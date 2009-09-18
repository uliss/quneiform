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
void SetReturnCode_cimage(Word16 rc);
Word16 GetReturnCode_cimage();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_WriteCallbackImage(PWord8                 lpName,
											  CIMAGEIMAGECALLBACK  Cbk)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->WriteCBImage( (PChar8)lpName, Cbk );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetCallbackImage(PWord8                  lpName,
											PCIMAGEIMAGECALLBACK  pCbk)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetCBImage( (PChar8)lpName, pCbk );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_WriteDIB(PWord8    lpName,
									Handle  lpDIB,
									Word32  wFlag)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->SetDIB( (PChar8)lpName, lpDIB, wFlag );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_ReadDIB(PWord8     lpName,
								   PHandle  lplpDIB,
								   Word32   wFlag)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetDIB( (PChar8)lpName, lplpDIB, wFlag );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetData(PWord8                     lpName,
								   PCIMAGE_InfoDataInGet       lpIn,
								   PCIMAGE_InfoDataOutGet      lpOut)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetImage( (PChar8)lpName, lpIn, lpOut );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetDIBData(PWord8                      lpName,
								      PCIMAGE_InfoDataInGet       lpIn,
								      pchar                       *lpDIB)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetDIBFromImage( (PChar8)lpName, lpIn, lpDIB );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_ReplaceData(PWord8                        lpName,
									   PCIMAGE_InfoDataInReplace   lpIn )
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->ReplaceImage( (PChar8)lpName, lpIn );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetImageInfo(PWord8                     lpName,
										PCIMAGEBITMAPINFOHEADER  lpBIH)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetImageInfo( (PChar8)lpName, lpBIH );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_DeleteImage( PWord8  lpName)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->RemoveImage( (PChar8)lpName );

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
CIMAGE_FUNC(Bool32) CIMAGE_AddReadCloseRects(PWord8 lpName, Word32 wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->AddReadRectangles( (PChar8)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_RemoveReadCloseRects(PWord8 lpName, Word32 wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->RemoveReadRectangles( (PChar8)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_AddWriteCloseRects(PWord8 lpName, Word32 wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->AddWriteRectangles( (PChar8)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_RemoveWriteCloseRects(PWord8 lpName, Word32 wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->RemoveWriteRectangles( (PChar8)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_EnableMask(PWord8 lpName, PWord8 lpType, Bool32 bEnabler)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->EnableMask((PChar8)lpName, (PChar8)lpType, bEnabler);

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
CIMAGE_CALLBACK_FUNC(CIMAGEWORD)   CIMAGE_Callback_ImageRead(pchar  lpImage, Word16 wMaxSize)
{
	if ( !Control_cti )
	{
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
		return 0;
	}

	return (CIMAGEWORD)(Control_cti->CBImageRead((PChar8)lpImage, wMaxSize));
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
