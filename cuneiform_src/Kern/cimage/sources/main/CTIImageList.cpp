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

#include "ctiimagelist.h"

CTIImageList::CTIImageList()
{
	Begin()->SetNext(End());
	End()->SetNext(NULL);
}

CTIImageList::~CTIImageList()
{
	CTIImageHeader * Previos;
	CTIImageHeader * LastImage;

	while ( (LastImage = FindImage("", &Previos)) != Begin() )
	{
		Previos->SetNext(LastImage->GetNext());
		delete LastImage;
	}
}

Bool32 CTIImageList::AddImage(const char *lpName, Handle hDIB, uint32_t wFlag)
{
	CTIImageHeader * NewImage = NULL;
	CTIImageHeader * LastImage = NULL;

/* commented by Andrey
    if ( FindImage(lpName, &LastImage) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_IMAGE_NAME_REPEATED);
		return FALSE;
	}
*/
    while (FindImage(lpName, &LastImage))
    {
        LastImage = NULL;
        DeleteImage(lpName);
    }

	if ( lpName == NULL && lpName[0] == 0x0 )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGE_NAME);
		return FALSE;
	}

	if ( hDIB == NULL )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGE_INFO);
		return FALSE;
	}

	if ( LastImage == NULL )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	NewImage = new CTIImageHeader(lpName, hDIB, wFlag);


	if ( NewImage == NULL )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGE_INFO);
		return FALSE;
	}

	NewImage->SetNext(LastImage->GetNext());
	LastImage->SetNext(NewImage);

	return TRUE;
}

Bool32 CTIImageList::GetImage(const char *lpName, Handle* phDIB)
{
	CTIImageHeader * Image = FindImage(lpName);

	if (Image == NULL)
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	*phDIB = Image->GetImageHandle();

	return TRUE;
}

Bool32 CTIImageList::DeleteImage(const char *lpName)
{
	CTIImageHeader * Previos = NULL;
	CTIImageHeader * ToDelete = FindImage(lpName, &Previos );

	if ( ToDelete != NULL  )
	{
		Previos->SetNext(ToDelete->GetNext());
		delete ToDelete;
		return TRUE;
	}

	return FALSE;
}

CTIImageHeader * CTIImageList::FindImage(const char *lpName, CTIImageHeader ** Previos)
{
	CTIImageHeader * Current = NULL;
	CTIImageHeader * Prev    = Begin();
	char              Buff[CIMAGE_MAX_IMAGE_NAME];

	if ( lpName != NULL && CIMAGE_STRING_LENGHT(lpName) < CIMAGE_MAX_IMAGE_NAME )
	{
		CIMAGE_STRING_COPY(Buff, lpName);

		if ( Buff[0] != 0x0 )
		{
			for ( Current = Begin()->GetNext(); Current != End(); Current = Current->GetNext() )
			{
				if ( Current->CheckName(Buff) )
						break;

				Prev = Current;
			}

			if (Previos)
			{
				*Previos = Prev;
			}

			if ( Current == End() )
			{
				SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_IN_CONTAINER);
				Current = NULL;
			}

			return Current;
		}
		else
		{
			if ( Begin()->GetNext() == End() )
			{
				Current = Prev = Begin();
			}
			else
				for ( Current = Begin()->GetNext(); Current != End() && Current->GetNext() != End(); Current = Current->GetNext() )
				{
					Prev = Current;
				}

			if (Previos)
			{
				*Previos = Prev;
			}

			return Current;
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file

Bool32 CTIImageList::SetImageWriteMask(const char *lpName, PCTIMask pWMask)
{
	Bool32 bRet;
	CTIImageHeader * Image = FindImage(lpName);

	if (Image == NULL)
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	bRet = Image->SetWriteMask(pWMask);

	return TRUE;
}

Bool32 CTIImageList::SetImageReadMask(const char *lpName,PCTIMask pAMask)
{
	Bool32 bRet;
	CTIImageHeader * Image = FindImage(lpName);

	if (Image == NULL)
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	bRet = Image->SetReadMask(pAMask);

	return TRUE;
}

Bool32 CTIImageList::GetImageWriteMask(const char *lpName, PPCTIMask ppWMask, PBool32 pEnMask)
{
	CTIImageHeader * Image = FindImage(lpName);

	if (Image == NULL)
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	*ppWMask = Image->GetWriteMask();
	*pEnMask = Image->IsMaskEnabled("w");

	return TRUE;
}

Bool32 CTIImageList::GetImageReadMask(const char *lpName, PPCTIMask ppMask, PBool32 pEnMask)
{
	CTIImageHeader * Image = FindImage(lpName);

	if (Image == NULL)
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	*ppMask = Image->GetReadMask();
	*pEnMask = Image->IsMaskEnabled("r");
	return TRUE;
}

Bool32 CTIImageList::EnableMask(const char *pName, char* pType, Bool32 mEnabled)
{
	CTIImageHeader * Image = FindImage(pName);

	if (Image == NULL)
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	return Image->EnableMask(pType, mEnabled);
}

Bool32 CTIImageList::FindHandle(Handle hImage)
{
	CTIImageHeader * Current = NULL;
	CTIImageHeader * Prev    = Begin();
	Bool32 bRet              = FALSE;

	if ( hImage )
	{

		for ( Current = Begin()->GetNext(); Current != End(); Current = Current->GetNext() )
		{
			if ( Current->GetImageHandle() == hImage )
			{
				bRet = TRUE;
				break;
			}

			Prev = Current;
		}
	}
	return bRet;
}
