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

// СRTurner.cpp: implementation of the CRTurner class.
//
//////////////////////////////////////////////////////////////////////
//#define RIMAGE_DUMP_TO_FILE
//////////////////////////////////////////////////////////////////////
#include "globus.h"
#include "crturner.h"
//#include "turn.h"
#include "criimage.h"
#include "crimemory.h"
#include "resource.h"
#include "memory.h"
//#include "dpuma.h"
#ifdef RIMAGE_DUMP_TO_FILE
#include "PumaMemoryToFileDumper.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTurner::CRTurner() {
	uint32_t i, j;
	uchar mask, mask2;

	hLineBuffer = 0x00000000; //NULL;

	wRightMask[0] = 0xfe;
	wLeftMask[0] = 0x7f;
	wRightShift[0] = 7;
	wBitMask[7] = 0x01;

	for (i = 1; i < 8; i++) {
		wRightMask[i] = wRightMask[i - 1] << 1;

		wLeftMask[i] = wLeftMask[i - 1] >> 1;

		wRightShift[i] = wRightShift[i - 1] - 1;

		wBitMask[7 - i] = wBitMask[8 - i] << 1;
	}

	for (i = 0; i < 256; i++) {
		Turn1at180[i] = 0;

		mask = 0x80;
		mask2 = 0x1;

		for (j = 0; j < 8; mask >>= 1, mask2 <<= 1, j++) {
			if ((i & mask) != 0)
				Turn1at180[i] += mask2;

		}
	}

	for (i = 0; i < 132; i++) {
		memset(Turn1at90[i], 0x0, 8);
	}

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) {
			Turn1at90[wBitMask[i]][j] = wBitMask[j];
		}
}

CRTurner::~CRTurner() {

}

void * CRTurner::TurnDIB(void *pDIB, uint32_t wAngle) {
	return NULL;
}

Bool32 CRTurner::FreeDIB(void *pDIB) {
	return TRUE;
}

Bool32 CRTurner::TurnDIB(PCTDIB pInDIB, PCTDIB pOutDIB, uint32_t wAngle) {
	Bool32 bRet;

	if (!pInDIB || !pOutDIB)
		return FALSE;

	WriteDIBtoBMP("BeforeTurn.bmp", pInDIB);

	switch (wAngle) {
	case RIMAGE_TURN_90:
		bRet = Turn90(pInDIB, pOutDIB);
		break;
	case RIMAGE_TURN_270:
		bRet = Turn270(pInDIB, pOutDIB);
		break;
	case RIMAGE_TURN_180:
		bRet = Turn180(pInDIB, pOutDIB);
		break;

	default:
		bRet = FALSE;

	}

	WriteDIBtoBMP("AfterTurn.bmp", pInDIB);
	if (bRet)
		WriteDIBtoBMP("DestinationTurn.bmp", pOutDIB);

	return bRet;
}

Bool32 CRTurner::Turn90(PCTDIB pInDIB, PCTDIB pOutDIB) {
	Bool32 bRet = FALSE;

	if (CheckInAndOut90(pInDIB, pOutDIB)) {
		switch (pInDIB->GetPixelSize()) {
		case 1:
			bRet = Turn90LA(pInDIB, pOutDIB);
			break;
		case 8:
		case 16:
		case 24:
		case 32:
			bRet = Turn90GC(pInDIB, pOutDIB);
			break;

		default:
			bRet = FALSE;
		}
	}

	return bRet;
}

Bool32 CRTurner::Turn180(PCTDIB pInDIB, PCTDIB pOutDIB) {
	Bool32 bRet = FALSE;

	if (CheckInAndOut180(pInDIB, pOutDIB)) {
		switch (pInDIB->GetPixelSize()) {
		case 1:
			bRet = Turn180LA(pInDIB, pOutDIB);
			break;
		case 8:
		case 16:
		case 24:
		case 32:
			bRet = Turn180GC(pInDIB, pOutDIB);
			break;

		default:
			bRet = FALSE;
		}
	}

	return bRet;
}

Bool32 CRTurner::Turn270(PCTDIB pInDIB, PCTDIB pOutDIB) {
	Bool32 bRet = FALSE;

	if (CheckInAndOut90(pInDIB, pOutDIB)) {
		switch (pInDIB->GetPixelSize()) {
		case 1:
			bRet = Turn270LA(pInDIB, pOutDIB);
			break;
		case 8:
		case 16:
		case 24:
		case 32:
			bRet = Turn270GC(pInDIB, pOutDIB);
			break;

		default:
			bRet = FALSE;
		}
	}

	return bRet;
}

Bool32 CRTurner::Turn180LA(PCTDIB pInDIB, PCTDIB pOutDIB) {
	Bool32 bRet = FALSE;
	uint32_t nLine;
	uint32_t sLine;
	uint32_t sLineSize;
	uint32_t sShift;
	uint32_t rShift;
	puchar sPix;
	puchar nPix;
	uint32_t wLines = pInDIB->GetLinesNumber();
	uint32_t sLineWidth = pInDIB->GetLineWidth();
	uint32_t sBytesPerLine = pOutDIB->GetUsedLineWidthInBytes();
	uint32_t cByte;

	FreeBuffers();

	hLineBuffer = RIMAGEDAlloc((sLineSize = pInDIB->GetLineWidthInBytes()),
			"CRTurner::Turn180LA - line buffer");
	LineBuffer = (puchar) RIMAGELock(hLineBuffer);

	// позиция последнего пиксела
	sShift = pOutDIB->GetPixelShiftInByte(pOutDIB->GetLineWidth() - 1);
	rShift = 7 - sShift;

	for (nLine = 0, sLine = wLines - 1; nLine < wLines; nLine++, sLine--) {
		// копируем в буфер всю строку и при этом сдвигаем её
		sPix = (puchar) pInDIB->GetPtrToLine(sLine);

		if (sShift == 7) {
			nPix = (puchar) pOutDIB->GetPtrToLine(nLine) + sBytesPerLine - 1;
		} else {
			nPix = LineBuffer + sBytesPerLine - 1;
		}

		for (cByte = 0; cByte < sBytesPerLine; cByte++) {
			// просто копируем переворачивая
			*nPix = Turn1at180[*sPix];
			nPix--;
			sPix++;
		}

		if (sShift != 7) {
			nPix = LineBuffer;

			for (cByte = 0; cByte < sBytesPerLine; cByte++) {
				// сдвигаем
				*nPix = (*nPix) << rShift;
				*nPix |= (*(nPix + 1) >> (sShift + 1)) & (wLeftMask[sShift]);
				nPix++;
			}
			// теперь копируем в выходной
			sPix = LineBuffer;
			nPix = (puchar) pOutDIB->GetPtrToLine(nLine);
			memcpy(nPix, sPix, sBytesPerLine);
		}
		bRet = TRUE;
	}

	FreeBuffers();

	return bRet;
}

void CRTurner::FreeBuffers() {
	if (hLineBuffer) {
		RIMAGEFree(hLineBuffer);
		hLineBuffer = 0x00000000;//NULL;
	}
}

Bool32 CRTurner::CheckInAndOut180(PCTDIB pIn, PCTDIB pOut) {
	if (pIn->GetImageHeight() != pOut->GetImageHeight() || pIn->GetImageWidth()
			!= pOut->GetImageWidth() || pIn->GetPixelSize()
			!= pOut->GetPixelSize()) {
		SetReturnCode_rimage(IDS_RIMAGE_OUTCOME_DIB_NOT_LINK_TO_INCOME);
		return FALSE;
	}

	return TRUE;
}

Bool32 CRTurner::CheckInAndOut90(PCTDIB pIn, PCTDIB pOut) {
	if (pIn->GetImageHeight() != pOut->GetImageWidth() || pIn->GetImageWidth()
			!= pOut->GetImageHeight() || pIn->GetPixelSize()
			!= pOut->GetPixelSize()) {
		SetReturnCode_rimage(IDS_RIMAGE_OUTCOME_DIB_NOT_LINK_TO_INCOME);
		return FALSE;
	}

	return TRUE;
}

Bool32 CRTurner::Turn90LA(PCTDIB pInDIB, PCTDIB pOutDIB) {
	uint32_t dLines = pOutDIB->GetLinesNumber();
	uint32_t sLines = pInDIB->GetLinesNumber(); //
	int32_t sX;
	int32_t dX;
	int32_t dLine;
	int32_t sLine;
	puchar sPix;
	puchar dPix;
	uint32_t sBytesPerLine = pOutDIB->GetLineWidth() / 8;
	uint32_t cByte;
	uint32_t sShift;

	for (dLine = 0, sX = 0; dLine < (int32_t) dLines; dLine++, sX++) {
		// начало линии
		dPix = (puchar) pOutDIB->GetPtrToLine(dLine);
		// сдвиг
		sShift = pInDIB->GetPixelShiftInByte(sX);
		// по 8 пикселей
		for (cByte = 0, sLine = sLines - 1, dX = 0; cByte < sBytesPerLine; cByte++, dX
				+= 8) {
			*dPix = 0x0;

			//x.......
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine--);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][0];

			//.x......
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine--);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][1];

			//..x.....
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine--);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][2];

			//...x....
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine--);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][3];

			//....x...
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine--);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][4];

			//.....x..
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine--);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][5];

			//......x.
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine--);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][6];

			//.......x
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine--);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][7];

			dPix++;
		}

		// последний неполный пиксел
		if (sLine >= 0) {
			*dPix = 0x0;

			for (; sLine >= 0; sLine--, dX++) {
				sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine);
				*dPix
						|= Turn1at90[((*sPix) & wBitMask[sShift])][(pOutDIB->GetPixelShiftInByte(
								dX))];
			}
		}
	}
	return TRUE;
}

Bool32 CRTurner::Turn270LA(PCTDIB pInDIB, PCTDIB pOutDIB) {
	uint32_t dLines = pOutDIB->GetLinesNumber();
	uint32_t sLines = pInDIB->GetLinesNumber(); //
	int32_t sX;
	int32_t dX;
	int32_t dLine;
	int32_t sLine;
	puchar sPix;
	puchar dPix;
	uint32_t sBytesPerLine = pOutDIB->GetLineWidth() / 8;
	uint32_t cByte;
	uint32_t sShift;

	for (dLine = 0, sX = dLines - 1; dLine < (int32_t) dLines; dLine++, sX--) {
		// начало линии
		dPix = (puchar) pOutDIB->GetPtrToLine(dLine);
		// сдвиг
		sShift = pInDIB->GetPixelShiftInByte(sX);
		// по 8 пикселей
		for (cByte = 0, sLine = 0, dX = 0; cByte < sBytesPerLine; cByte++, dX
				+= 8) {
			*dPix = 0x0;

			//x.......
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine++);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][0];

			//.x......
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine++);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][1];

			//..x.....
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine++);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][2];

			//...x....
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine++);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][3];

			//....x...
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine++);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][4];

			//.....x..
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine++);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][5];

			//......x.
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine++);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][6];

			//.......x
			sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine++);
			*dPix |= Turn1at90[((*sPix) & wBitMask[sShift])][7];

			dPix++;
		}

		// последний неполный пиксел
		// если есть
		if (sLine < (int32_t) sLines) {
			*dPix = 0x0;

			for (; sLine < (int32_t) sLines; sLine++, dX++) {
				sPix = (puchar) pInDIB->GetPtrToPixel(sX, sLine);

				*dPix
						|= Turn1at90[((*sPix) & wBitMask[sShift])][(pOutDIB->GetPixelShiftInByte(
								dX))];
			}
		}
	}
	return TRUE;
}

Bool32 CRTurner::Turn90GC(PCTDIB pIn, PCTDIB pOut) {
	Bool32 bRet = FALSE;
	int32_t sX;
	int32_t dX;
	int32_t dLine;
	int32_t sLine;
	puchar sPix8;
	puchar dPix8;
	uint16_t * sPix16;
	uint16_t * dPix16;
	uint32_t * sPix32;
	uint32_t * dPix32;
	uint32_t dLines = pOut->GetLinesNumber();
	uint32_t sLines = pIn->GetLinesNumber(); //
	uint32_t wPixSize = pIn->GetPixelSize() / 8;

	switch (wPixSize) {
	case 1:
		for (dLine = 0, sX = 0; dLine < (int32_t) dLines; dLine++, sX++) {
			CONTINUEPIXEL(dPix8 = (puchar)pOut->GetPtrToLine( dLine ));

			for (sLine = sLines - 1, dX = 0; dX < (int32_t) sLines; sLine--, dX++, dPix8++) {
				CONTINUEPIXEL(sPix8 = (puchar)pIn->GetPtrToPixel( sX, sLine));
				*dPix8 = *sPix8;
			}
		}
		bRet = TRUE;
		break;

	case 2:
		for (dLine = 0, sX = 0; dLine < static_cast<int32_t>(dLines); dLine++, sX++) {
			CONTINUEPIXEL(dPix16 = (uint16_t *)pOut->GetPtrToLine( dLine ));

			for (sLine = sLines - 1, dX = 0; dX < (int32_t) sLines; sLine--, dX++, dPix16++) {
				CONTINUEPIXEL(sPix16 = (uint16_t*)pIn->GetPtrToPixel( sX, sLine));
				*dPix16 = *sPix16;
			}
		}
		bRet = TRUE;
		break;

	case 3:
		for (dLine = 0, sX = 0; dLine < (int32_t) dLines; dLine++, sX++) {
			CONTINUEPIXEL(dPix8 = (puchar)pOut->GetPtrToLine( dLine ));

			for (sLine = sLines - 1, dX = 0; dX < (int32_t) sLines; sLine--, dX++) {
				CONTINUEPIXEL(sPix8 = (puchar)pIn->GetPtrToPixel( sX, sLine));
				*dPix8++ = *sPix8++;
				*dPix8++ = *sPix8++;
				*dPix8++ = *sPix8++;
			}
		}
		bRet = TRUE;
		break;

	case 4:
		for (dLine = 0, sX = 0; dLine < (int32_t) dLines; dLine++, sX++) {
			CONTINUEPIXEL(dPix32 = (uint32_t *)pOut->GetPtrToLine( dLine ));

			for (sLine = sLines - 1, dX = 0; dX < (int32_t) sLines; sLine--, dX++, dPix32++) {
				CONTINUEPIXEL(sPix32 = (uint32_t *)pIn->GetPtrToPixel( sX, sLine));
				*dPix32 = *sPix32;
			}
		}
		bRet = TRUE;
		break;

	default:
		bRet = FALSE;
	}

	return bRet;
}

Bool32 CRTurner::Turn180GC(PCTDIB pIn, PCTDIB pOut) {
	Bool32 bRet = FALSE;
	uint32_t dLine;
	uint32_t sLine;
	uint32_t wPix;
	puchar sPix8;
	puchar dPix8;
	uint16_t * sPix16;
	uint16_t * dPix16;
	uint32_t * sPix32;
	uint32_t * dPix32;
	uint32_t wPixSize = pIn->GetPixelSize() / 8;
	uint32_t wLines = pIn->GetLinesNumber();
	uint32_t sLineWidth = pIn->GetLineWidth();

	switch (wPixSize) {
	case 1:
		for (dLine = 0, sLine = wLines - 1; dLine < wLines; dLine++, sLine--) {
			sPix8 = (puchar) pIn->GetPtrToPixel(sLineWidth - 1, sLine);
			dPix8 = (puchar) pOut->GetPtrToLine(dLine);

			for (wPix = 0; wPix < sLineWidth; wPix++, sPix8--, dPix8++) {
				*dPix8 = *sPix8;
			}
		}
		bRet = TRUE;
		break;

	case 2:
		for (dLine = 0, sLine = wLines - 1; dLine < wLines; dLine++, sLine--) {
			sPix16 = (uint16_t*) pIn->GetPtrToPixel(sLineWidth - 1, sLine);
			dPix16 = (uint16_t*) pOut->GetPtrToLine(dLine);

			for (wPix = 0; wPix < sLineWidth; wPix++, sPix16--, dPix16++) {
				*dPix16 = *sPix16;
			}
		}
		bRet = TRUE;
		break;

	case 3:
		for (dLine = 0, sLine = wLines - 1; dLine < wLines; dLine++, sLine--) {
			sPix8 = (puchar) pIn->GetPtrToPixel(sLineWidth - 1, sLine);
			dPix8 = (puchar) pOut->GetPtrToLine(dLine);

			for (wPix = 0; wPix < sLineWidth; wPix++, sPix8 -= 6) {
				*dPix8++ = *sPix8++;
				*dPix8++ = *sPix8++;
				*dPix8++ = *sPix8++;
			}
		}
		bRet = TRUE;
		break;

	case 4:
		for (dLine = 0, sLine = wLines - 1; dLine < wLines; dLine++, sLine--) {
			sPix32 = (uint32_t*) pIn->GetPtrToPixel(sLineWidth - 1, sLine);
			dPix32 = (uint32_t*) pOut->GetPtrToLine(dLine);

			for (wPix = 0; wPix < sLineWidth; wPix++, sPix32--, dPix32++) {
				*dPix32 = *sPix32;
			}
		}
		bRet = TRUE;
		break;

	default:
		bRet = FALSE;
	}

	return bRet;
}

Bool32 CRTurner::Turn270GC(PCTDIB pIn, PCTDIB pOut) {
	Bool32 bRet = FALSE;
	int32_t sX;
	int32_t dX;
	int32_t dLine;
	int32_t sLine;
	puchar sPix8;
	puchar dPix8;
	uint16_t * sPix16;
	uint16_t * dPix16;
	uint32_t * sPix32;
	uint32_t * dPix32;
	uint32_t dLines = pOut->GetLinesNumber();
	uint32_t sLines = pIn->GetLinesNumber(); //
	uint32_t wPixSize = pIn->GetPixelSize() / 8;

	switch (wPixSize) {
	case 1:
		for (dLine = 0, sX = dLines - 1; dLine < (int32_t) dLines; dLine++, sX--) {
			CONTINUEPIXEL(dPix8 = (puchar)pOut->GetPtrToLine( dLine ));

			for (sLine = 0, dX = 0; sLine < (int32_t) sLines; sLine++, dX++, dPix8++) {
				CONTINUEPIXEL(sPix8 = (puchar)pIn->GetPtrToPixel( sX, sLine));
				*dPix8 = *sPix8;
			}
		}
		bRet = TRUE;
		break;

	case 2:
		for (dLine = 0, sX = dLines - 1; dLine < static_cast<int32_t>(dLines); dLine++, sX--) {
			CONTINUEPIXEL(dPix16 = (uint16_t*)pOut->GetPtrToLine( dLine ));

			for (sLine = 0, dX = 0; sLine < static_cast<int32_t>(sLines); sLine++, dX++, dPix16++) {
				CONTINUEPIXEL(sPix16 = (uint16_t *)pIn->GetPtrToPixel( sX, sLine));
				*dPix16 = *sPix16;
			}
		}
		bRet = TRUE;
		break;

	case 3:
		for (dLine = 0, sX = dLines - 1; dLine < static_cast<int32_t>(dLines); dLine++, sX--) {
			CONTINUEPIXEL(dPix8 = (puchar)pOut->GetPtrToLine( dLine ));

			for (sLine = 0, dX = 0; sLine < static_cast<int32_t>(sLines); sLine++, dX++) {
				CONTINUEPIXEL(sPix8 = (puchar)pIn->GetPtrToPixel( sX, sLine));
				*dPix8++ = *sPix8++;
				*dPix8++ = *sPix8++;
				*dPix8++ = *sPix8++;
			}
		}
		bRet = TRUE;
		break;

	case 4:
		for (dLine = 0, sX = dLines - 1; dLine < (int32_t) dLines; dLine++, sX--) {
			CONTINUEPIXEL(dPix32 = (uint32_t *)pOut->GetPtrToLine( dLine ));

			for (sLine = 0, dX = 0; sLine < (int32_t) sLines; sLine++, dX++, dPix32++) {
				CONTINUEPIXEL(sPix32 = (uint32_t *)pIn->GetPtrToPixel( sX, sLine));
				*dPix32 = *sPix32;
			}
		}
		bRet = TRUE;
		break;

	default:
		bRet = FALSE;
	}

	return bRet;
}

Bool32 CRTurner::WriteDIBtoBMP(const char *cName, PCTDIB pDIB) {
#ifdef RIMAGE_DUMP_TO_FILE
	uint32_t wBMPSize = pDIB->GetDIBSize() + 14;
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
