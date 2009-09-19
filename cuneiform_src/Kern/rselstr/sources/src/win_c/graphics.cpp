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

#include "layout.h"
#include "extract.h"

#include "status.h"

#include "graphics.h"

#include "dpuma.h"
#include "pumadef.h"
#include "cstr.h"
#include "cpage.h"

////////////////////////////////////////////////

////////////////////////////////////////////////

void _setvideomode (int mode){}

void _clearscreen (int mode)
{
	LDPUMA_DeleteRects(NULL,PUMA_MODULE_RBLOCK);
	LDPUMA_DeleteLines(NULL,PUMA_MODULE_RBLOCK);
	LDPUMA_DeleteStrings(NULL,PUMA_MODULE_RBLOCK);
}

static uint32_t swColor = 0;

void _setcolor (int color)
{
	#define __RGB__(r,g,b)          ((uint32_t)(((uchar)(r)|((uint16_t)((uchar)(g))<<8))|(((uint32_t)(uchar)(b))<<16)))
	uint32_t clr[16] =
	{
		__RGB__(0,0,0),// 0 черный
		__RGB__(0,0,127),// 1 синий
		__RGB__(0,127,0),// 2 зеленый
		__RGB__(64,64,127),// 3 голубой
		__RGB__(127,0,0),// 4 красный
		__RGB__(127,64,64),// 5 розовый
		__RGB__(64,127,64),// 6 коричневый
		__RGB__(127,127,127),// 7 серый
		__RGB__(0,0,255),// 1 синий
		__RGB__(0,255,0),// 2 зеленый
		__RGB__(127,127,255),// 3 голубой
		__RGB__(255,0,0),// 4 красный
		__RGB__(255,127,127),// 5 розовый
		__RGB__(127,255,127),// 6 коричневый
		__RGB__(255,255,0),//15 белый
		__RGB__(255,0,255)//15 белый
	};
	swColor = clr[color];
	#undef __RGB__
}

void _rectangle (int t,int left,int top,int right,int bottom)
{
	Rect16 rect = {left,top,right,bottom};
	switch(t)
	{
	case _GFILLINTERIOR:
		LDPUMA_DrawRect(NULL,&rect,0,swColor,0,PUMA_MODULE_RBLOCK);
		break;
	default:
		LDPUMA_DrawRect(NULL,&rect,0,swColor,-16,PUMA_MODULE_RBLOCK);
	}
}

void _getfillmask (char * OldMask) {}

void _setfillmask (char * OldMask) {}

static Point16 sBeg = {0,0};

void _moveto (int x,int y)
{
	sBeg.x = x;
	sBeg.y = y;
}

void _lineto (int x,int y)
{
	Point16 end = {x,y};
	LDPUMA_DrawLine(NULL,&sBeg,&end,0,swColor,-16,PUMA_MODULE_RBLOCK);
	sBeg = end;
}

void _setlinestyle (int style) {}

#ifndef WIN32
typedef struct tagBITMAPINFOHEADER{
uint32_t      biSize;
        int32_t       biWidth;
        int32_t       biHeight;
        uint16_t       biPlanes;
        uint16_t       biBitCount;
uint32_t      biCompression;
uint32_t      biSizeImage;
        int32_t       biXPelsPerMeter;
        int32_t       biYPelsPerMeter;
uint32_t      biClrUsed;
uint32_t      biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;
#endif

void _gettextsettings (struct textsettings *ts)
{
	LPBITMAPINFOHEADER lp =(BITMAPINFOHEADER*) LDPUMA_GetDIBptr(NULL);
	if(lp)
	{
		ts->height = lp->biHeight;
		ts->width = lp->biWidth;
	}
}

static uint32_t swAlign = 0;
void _settextalign (int right,int top)
{
	swAlign = 0;
	switch(right)
	{
		case _RIGHT:
			swAlign |= TA_RIGHT;
			break;
		case _TOP:
			swAlign |= TA_TOP;
			break;
		case _CENTER:
			swAlign |= TA_CENTER;
			break;
		case _HALF:
			swAlign |= TA_CENTER;
			break;
	}
	switch(top)
	{
		case _RIGHT:
			swAlign |= TA_RIGHT;
			break;
		case _TOP:
			swAlign |= TA_TOP;
			break;
		case _CENTER:
			swAlign |= TA_CENTER;
			break;
		case _HALF:
			swAlign |= TA_CENTER;
			break;
	}
}

void _setcharsize (int n1, int n2) {}

void _grtext (int x, int y, const char * text)
{
	Point16 p = {x,y};
	LDPUMA_DrawString(NULL,&p,text,swAlign,swColor,120/*12*/,PUMA_MODULE_RBLOCK);
}

unsigned _getlinestyle (){return 0;}

void break_point_data (char * c){}

void write_prot () {}

Bool snapAutofont(uchar a){ a=a; return TRUE; } // Andrew Theer
////////////////////////////////////////////////
