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

// CTIMask.cpp: implementation of the CTIMask class.
//
//////////////////////////////////////////////////////////////////////

#include "ctimask.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTIMask::CTIMask()
        :mwMaskWidth(0),
		 mwMaskHeight(0),
		 mwSegments(0),
		 mcLine(0),
		 mwLines(0) {}

CTIMask::CTIMask(Word32 Width, Word32 Height)
        :mwMaskWidth(Width),
		 mwMaskHeight(Height),
		 mwSegments(0),
		 mcLine(0),
		 mwLines(0) {}

CTIMask::~CTIMask()
{
	PCTIMaskLine  pPL = mcLine.GetNext();
	PCTIMaskLine  pL = mcLine.GetNext();

	while ( pL )
	{
		pL = pL->GetNext();
		delete pPL;
		pPL = pL;
	}
}

Bool32 CTIMask::AddRectangle(PCIMAGE_Rect pRect)
{
	Word32        wXb;
	Word32        wXe;
	Word32        wYb;
	Word32        wYe;
	PCTIMaskLine  pPL = &mcLine;
	PCTIMaskLine  pL;
	Word32        wLine;

	if ( !IsRectOnMask(pRect) )
		return FALSE;

	wXb = pRect->dwX;
	wYb = pRect->dwY;
	wXe = wXb + pRect->dwWidth;
	wYe = wYb + pRect->dwHeight;

	CTIMaskLineSegment  Segm(wXb,wXe);

	if ( !SetPtrToPrevLine(wYb, &pPL) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_UNABLE_ADD_MASK);
		return FALSE;
	}
	pL = pPL->GetNext();

	for ( wLine = wYb; wLine < wYe; wLine++ )
	{
		if ( !pL )
			pPL->SetNext( pL = new CTIMaskLine(mwMaskWidth, wLine, &Segm) );
		else
		{
			if ( pL->GetLineNumber() == wLine )
			{
				// êëàäåì íîâûé ñåãìåíò â ëèíèþ
				if ( !pL->AddSegment(&Segm) )
				{
					SetReturnCode_cimage(IDS_CIMAGE_UNABLE_ADD_MASK);
					return FALSE;
				}
			}
			else
			{
				// äâñòàâëÿåì íîâóþ ëèíèþ
				pPL->SetNext( pL = new CTIMaskLine(mwMaskWidth, wLine, &Segm, pL) );
			}
		}

		if ( pL )
			pL = ( (pPL = pL)->GetNext() );

	}

	mwSegments++;
	return TRUE;
}

Bool32 CTIMask::RemoveRectangle(PCIMAGE_Rect pRect)
{
	Word32        wXb;
	Word32        wXe;
	Word32        wYb;
	Word32        wYe;
	PCTIMaskLine  pPL = &mcLine;
	PCTIMaskLine  pL;
	Word32        wLine;

	if ( !IsRectOnMask(pRect) )
		return FALSE;

	wXb = pRect->dwX;
	wYb = pRect->dwY;
	wXe = wXb + pRect->dwWidth;
	wYe = wYb + pRect->dwHeight;

	CTIMaskLineSegment  Segm(wXb,wXe);

	if ( !SetPtrToPrevLine(wYb, &pPL) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_UNABLE_REMOVE_MASK);
		return FALSE;
	}
	pL = pPL->GetNext();

	for ( wLine = wYb; wLine < wYe; wLine++ )
	{
		if ( !pL )
		{
			SetReturnCode_cimage(IDS_CIMAGE_UNABLE_REMOVE_MASK);
			return FALSE;
		}
		else
			if ( !pL->RemoveSegment(&Segm) )
			{
				SetReturnCode_cimage(IDS_CIMAGE_UNABLE_REMOVE_MASK);
				return FALSE;
			}

		if ( pL->GetSegmentsNumber() == 0 )
		{
			pPL->SetNext(pL->GetNext());
			delete pL;
			pL = pPL->GetNext();
		}

		if ( pL )
			pL = ( (pPL = pL)->GetNext() );
	}

	mwSegments--;
	return TRUE;
}


Bool32 CTIMask::IsRectOnMask(PCIMAGE_Rect pRect)
{
	return  ( pRect &&
		     ( pRect->dwX < (Int32)mwMaskWidth ||
		      (pRect->dwX + pRect->dwWidth) < mwMaskWidth ||
			  pRect->dwY < (Int32)mwMaskHeight ||
			  (pRect->dwY + pRect->dwHeight) < mwMaskHeight ) );
}

Bool32 CTIMask::SetPtrToPrevLine(Word32 wLine, PPCTIMaskLine ppLine)
{
	if ( !ppLine )
		return FALSE;

	while ( (*ppLine)->GetNext() )
	{
		if ( ((*ppLine)->GetNext())->GetLineNumber() >= wLine )
			break;

		(*ppLine) = ((*ppLine)->GetNext());
	}

	return TRUE;
}

Bool32 CTIMask::GetLine(Word32 wLine, PPCTIMaskLine ppcLine)
{
	PCTIMaskLine pL = mcLine.GetNext();
	Bool32 bLinePresent = FALSE;
	Int32 iLine;

	*ppcLine = NULL;

	if ( wLine > mwMaskHeight )
	{
		return FALSE;
	}

	while ( pL )
	{
		iLine = pL->GetLineNumber();

		if ( iLine < (Int32)wLine )
		{
			pL = pL->GetNext();
			continue;
		}

		if ( iLine == (Int32)wLine )
		{
			*ppcLine = pL;
			bLinePresent = TRUE;
			break;
		}

		if ( iLine > (Int32)wLine )
			break;
	}

	return bLinePresent;
}
