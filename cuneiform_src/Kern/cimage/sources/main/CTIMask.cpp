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

CTIMask::CTIMask(uint32_t Width, uint32_t Height)
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
	uint32_t        wXb;
	uint32_t        wXe;
	uint32_t        wYb;
	uint32_t        wYe;
	PCTIMaskLine  pPL = &mcLine;
	PCTIMaskLine  pL;
	uint32_t        wLine;

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
				// кладем новый сегмент в линию
				if ( !pL->AddSegment(&Segm) )
				{
					SetReturnCode_cimage(IDS_CIMAGE_UNABLE_ADD_MASK);
					return FALSE;
				}
			}
			else
			{
				// двставляем новую линию
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
	uint32_t        wXb;
	uint32_t        wXe;
	uint32_t        wYb;
	uint32_t        wYe;
	PCTIMaskLine  pPL = &mcLine;
	PCTIMaskLine  pL;
	uint32_t        wLine;

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
		     ( pRect->dwX < (int32_t)mwMaskWidth ||
		      (pRect->dwX + pRect->dwWidth) < mwMaskWidth ||
			  pRect->dwY < (int32_t)mwMaskHeight ||
			  (pRect->dwY + pRect->dwHeight) < mwMaskHeight ) );
}

Bool32 CTIMask::SetPtrToPrevLine(uint32_t wLine, PPCTIMaskLine ppLine)
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

Bool32 CTIMask::GetLine(uint32_t wLine, PPCTIMaskLine ppcLine)
{
	PCTIMaskLine pL = mcLine.GetNext();
	Bool32 bLinePresent = FALSE;
	int32_t iLine;

	*ppcLine = NULL;

	if ( wLine > mwMaskHeight )
	{
		return FALSE;
	}

	while ( pL )
	{
		iLine = pL->GetLineNumber();

		if ( iLine < (int32_t)wLine )
		{
			pL = pL->GetNext();
			continue;
		}

		if ( iLine == (int32_t)wLine )
		{
			*ppcLine = pL;
			bLinePresent = TRUE;
			break;
		}

		if ( iLine > (int32_t)wLine )
			break;
	}

	return bLinePresent;
}
