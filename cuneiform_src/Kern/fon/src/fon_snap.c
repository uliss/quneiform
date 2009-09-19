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

//////////////////////////
#include "fon.h"
#include "sfont.h"
#include "fonrec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <windows.h>*/

#include "compat_defs.h"

#define NUM_IN_SNAP 9     // maximal pictures in snap
Bool IsSnap = FALSE;
static int InSnap = 0; // now pictures in snap
static RecRaster *snapRaster = NULL;
static Bool WasRegister = FALSE;
static HWND hwndSnap = NULL;
static char szGluName[32] = "FonGlueSnap";
static HINSTANCE hGluInstance = NULL;
static char recogResult[256];
static Bool RegisterGlu(HANDLE hInstance, char* szAppName);
static char nameSnap[NUM_IN_SNAP + 1];
static int probSnap[NUM_IN_SNAP + 1];
////////////
//
//  return -1  - no memory
//
///////////////

FON_FUNC(int32_t) FONInitSnap(Handle hwnd)
{
	memset(nameSnap,0,sizeof(nameSnap));
	InSnap=0;

	if(snapRaster==NULL)
	snapRaster=(RecRaster *)malloc(NUM_IN_SNAP*sizeof(RecRaster));
	if(!snapRaster)
	return -1;

	if( WasRegister == FALSE )
	{
		if( hwnd )
		hGluInstance=(HINSTANCE)GetWindowLong((HWND)hwnd,GWL_HINSTANCE);
		else
		hGluInstance = GetModuleHandle("Fon32.dll");

		WasRegister=RegisterGlu(hGluInstance, szGluName);
		if(WasRegister==FALSE)
		return -2;
	}

	if(hwndSnap==NULL)
	hwndSnap=CreateWindow(szGluName,"SnapFONGlue",
			WS_OVERLAPPEDWINDOW,
			0,300,400,300,
			NULL, NULL,
			hGluInstance, NULL);

	if(hwndSnap && !IsWindowVisible(hwndSnap) )
	ShowWindow(hwndSnap,TRUE);

	IsSnap=TRUE;
	return NUM_IN_SNAP;
}
//////////////////
FON_FUNC(int32_t) FONEndSnap(void)
{

	IsSnap=FALSE;
	InSnap = 0;
	if(snapRaster)
	free(snapRaster);
	snapRaster=NULL;

	if(hwndSnap)
	{
		SendMessage( hwndSnap, WM_DESTROY, 0, 0);
	}

	//	UnregisterClass(szGluName, hGluInstance);
	//	WasRegister = FALSE;
	return 1;
}
////////////////////
int PutNamesSnap(int nvar, uchar *names, int *probs) {
	int i;

	memset(recogResult, 0, sizeof(recogResult));
	for (i = 0; i < nvar; i++)
		sprintf(recogResult + strlen(recogResult), "%c(%d) ", names[i],
				(int) probs[i]);
	return 1;
}
/////////////////////////
int AddRasterToSnap(RecRaster *rr, int num) {
	if (num < 0)
		num = InSnap;
	else
		InSnap = num;

	if (num >= NUM_IN_SNAP)
		return -10;
	if (num == 0)
		recogResult[0] = 0;
	memcpy(snapRaster + num, rr, sizeof(RecRaster));
	InSnap++;
	return 1;
}
//////////////
int AddBitmapToSnap(uchar *buf, int xbit, int yrow, int name, int dist) {
	int bytesx = ((xbit + 63) / 64) * 8;
	int xbyte = (xbit + 7) >> 3;
	RecRaster *recRas;
	uchar *ras;

	if (!IsSnap)
		return 0;

	if (InSnap >= NUM_IN_SNAP)
		InSnap = 0;
	//return -10;

	if (name <= 0) {
		nameSnap[InSnap] = '~';
		probSnap[InSnap] = 0;
	}
	// to ANSI from ASCII
	else {
		if (name >= 128 && name < 176)
			nameSnap[InSnap] = name + 64;
		else if (name >= 224 && name < 240)
			nameSnap[InSnap] = name + 16;
		else
			nameSnap[InSnap] = name;
		probSnap[InSnap] = dist;
	}

	if (bytesx * yrow > REC_MAX_RASTER_SIZE)
		yrow = REC_MAX_RASTER_SIZE / bytesx;

	recRas = snapRaster + InSnap;
	recRas->lnPixHeight = yrow;
	recRas->lnPixWidth = xbit;
	recRas->lnRasterBufSize = REC_MAX_RASTER_SIZE;
	ras = recRas->Raster;
	for (; yrow > 0; yrow--, ras += bytesx, buf += xbyte)
		memcpy(ras, buf, xbyte);

	InSnap++;
	return 1;
}
//////////////
// rr-> at position fx,fy, size sx,sy
static int PutRecRaster(HDC hDC, RecRaster *rr, int fx, int fy, int sx, int sy) {
	int i, j;
	int wid = rr->lnPixWidth;
	int hei = rr->lnPixHeight;
	int xbyte = ((wid + 63) / 64) * 8;
	int step, x, y;
	uchar cc;
	uchar *buf = rr->Raster;

	step = MIN(sx / wid, sy / hei);
	if (step < 3)
		step = 3;

	for (i = 0, y = fy; i < hei; i++, buf += xbyte, y += step) {
		for (j = 0, x = fx, cc = 128; j < wid; j++, x += step) {
			if (buf[j >> 3] & cc)
				Rectangle(hDC, x, y, x + step, y + step);
			cc >>= 1;
			if (cc == 0)
				cc = 128;
		}
	}

	return step;
}
/*************************/
FON_FUNC(int32_t) FONShowSnap(void)
{
	RECT rect;
	int i,j,xstart,ystart;
	HDC hDC;
	int numRow=1;
	int all;

	if( IsSnap == FALSE) return -11;
	if(InSnap <= 0) return 0; // nothing in snap

	if(hwndSnap==NULL)
	return -3;

	if(IsIconic (hwndSnap )) return 0;

	GetClientRect(hwndSnap,&rect);

	numRow=1;
	if(InSnap > 6 ) numRow=3; // make 3 rows
	else if(InSnap > 3 ) numRow=2; // make 2 rows

	if(InSnap >=3 ) j=rect.right/3;
	else j=rect.right/InSnap;

	hDC=GetDC(hwndSnap);
	Rectangle(hDC,0,0,rect.right,rect.bottom);

	rect.bottom /=numRow;
	ystart=0;
	for(all=0;numRow;numRow--,ystart+=rect.bottom)
	{
		for(i=xstart=0;i<3 && all< InSnap;i++,xstart+=j,all++)
		{
			SelectObject(hDC,GetStockObject(NULL_BRUSH));
			Rectangle(hDC,xstart,ystart,xstart+j,ystart+rect.bottom);
			SelectObject(hDC,GetStockObject(GRAY_BRUSH)); // can gray?
			PutRecRaster(hDC,snapRaster+all,xstart,ystart,j,rect.bottom);
		}
	}

	ReleaseDC(hwndSnap,hDC);
	//  SetWindowText(hwndSnap,nameSnap);
	PutNamesSnap(InSnap,nameSnap,probSnap);
	SetWindowText(hwndSnap,recogResult);

	//  ShowWindow(hwndSnap,SW_SHOWNORMAL);

	return InSnap;
}
/////////////////
/*
 * Handle messages for the application window
 */
int32_t PASCAL GluFonWindowProc(HWND win, uint msg, WPARAM wparam, LPARAM lparam) {
	PAINTSTRUCT ps;

	switch (msg) {
	case WM_DESTROY:
		//FONEndSnap();
		hwndSnap = NULL;
		IsSnap = FALSE;
		break;
	case WM_SIZE:
		if (InSnap)
			FONShowSnap();
		break;
	case WM_PAINT:
		BeginPaint(win, &ps);
		if (InSnap)
			FONShowSnap();
		EndPaint(win, &ps);
		return TRUE;
	default:
		break;
	}
	return DefWindowProc(win, msg, wparam, lparam);
}
////////////////////////
static Bool RegisterGlu(HANDLE hInstance, char* szAppName) {
	WNDCLASS WndClass;
	Bool bSuccess;

	memset(&WndClass, 0, sizeof(WNDCLASS));

	WndClass.lpszClassName = szAppName;
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = GluFonWindowProc;
	WndClass.style = (unsigned int) NULL;
	WndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	WndClass.lpszMenuName = (char*) NULL;
	WndClass.cbClsExtra = (int) NULL;
	WndClass.cbWndExtra = (int) NULL;

	bSuccess = RegisterClass(&WndClass);

	return bSuccess;
}
//////////////////////////
