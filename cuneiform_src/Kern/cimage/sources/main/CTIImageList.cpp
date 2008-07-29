/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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

Bool32 CTIImageList::AddImage(PChar8  lpName, Handle hDIB, Word32 wFlag)
{
	CTIImageHeader * NewImage = NULL;
	CTIImageHeader * LastImage = NULL;

	Word32 ImageSizeInPixels = 0;
	Word32 ImageSizeInBytes  = 0;
	Word32 HederSize = 0;

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

Bool32 CTIImageList::GetImage(PChar8  lpName, PHandle phDIB)
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

Bool32 CTIImageList::DeleteImage(PChar8  lpName)
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

CTIImageHeader * CTIImageList::FindImage(PChar8  lpName, CTIImageHeader ** Previos)
{
	CTIImageHeader * Current = NULL;
	CTIImageHeader * Prev    = Begin();
	Char8              Buff[CIMAGE_MAX_IMAGE_NAME];

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

Bool32 CTIImageList::SetImageWriteMask(PChar8 lpName, PCTIMask pWMask)
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

Bool32 CTIImageList::SetImageReadMask(PChar8 lpName,PCTIMask pAMask)
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

Bool32 CTIImageList::GetImageWriteMask(PChar8 lpName, PPCTIMask ppWMask, PBool32 pEnMask)
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

Bool32 CTIImageList::GetImageReadMask(PChar8 lpName, PPCTIMask ppMask, PBool32 pEnMask)
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

Bool32 CTIImageList::EnableMask(PChar8 pName, PChar8 pType, Bool32 mEnabled)
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
