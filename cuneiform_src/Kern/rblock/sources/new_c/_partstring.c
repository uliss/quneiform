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

# include <setjmp.h>
# include <string.h>

/*
# include "iolib.h"
# include "layout.h"
# include "extract.h"
# include "kernel.h"
# include "status.h"

# include "dpuma.h"
# include "pumadef.h"
# include "ccom.h"
# include "newfunc.h"
# include "exc.h"
# include "excdefs.h"
# include "cstr.h"
# include "cstrdefs.h"
# include "cpage.h"
# include "polyblock.h"
*/
# include "new_c.h"
#include "minmax.h"
/////////////////////////////////

# define __RGB__(r,g,b)          ((uint32_t)(((uchar)(r)|((uint16_t)((uchar)(g))<<8))|(((uint32_t)(uchar)(b))<<16)))

/////////////////////////////////
extern jmp_buf fatal_error_exit;	// For error handling
extern int16_t nStrings;
extern uint16_t run_options;

extern Handle hShowString;
extern Handle hShowCells;

//////////////////////////////////
static void LayoutFromCPAGE(Handle hCPAGE);
static int IsInPoly(Point16 a, void * pPoly);

// Piter /////////////////////////////////////
void RotatePageToIdeal (void);
void RotatePageToReal (void);

/////////////////////////////////
void PageLayoutStrings(Handle hCCOM,Handle hCPAGE)
{
	if(ReadRoots(hCCOM, FALSE))
	{
		run_options = FORCE_ONE_COLUMN;

		LayoutPart1 ();

		LayoutFromCPAGE(hCPAGE);

		LayoutPart2 ();
		if(!LDPUMA_Skip(hShowString))
		{
			LDPUMA_Console("Нажмите любую клавишу...");
			LDPUMA_WaitUserInput(NULL,NULL);
			LDPUMA_DeleteRects(NULL,PUMA_MODULE_RBLOCK);
		}
		//free (CellsPage); // *** Rom 03-03-99
		//CellsPage = NULL; // *** Rom 03-03-99
	}
}
/////////////////////////////////
/*
uint32_t myMonitorProc(Handle wnd,Handle hwnd,uint32_t message,uint32_t wParam,uint32_t lParam)
{
	uint32_t rc = 0; // если обработка не состоялась
	switch(message)
	{
	case WM_LBUTTONDBLCLK:
		{
			Point32 p = { (uint16_t)lParam, (uint16_t)(lParam>>16)};  // vertical position of cursor  }
			p = LDPUMA_GetRasterPixel(wnd,p);
			LDPUMA_Console("Растр [%i,%i].",p.x,p.y);
		}
		break;
	case WM_KEYDOWN:
		switch((int)wParam)
		{
		case VK_F1:
			LDPUMA_Console("Позиция %i из %i",LDPUMA_CSTR_GetPosition(NULL),LDPUMA_CSTR_GetLength());
			LDPUMA_RasterText("F1 - подсказка. см. консоль.");
			break;
		case VK_F2:
			LDPUMA_CSTR_Update();
			break;
		}
		break;
	}
	// Дефолтный обработчик вызывать не надо !!!
return rc;
}*/
/////////////////////////////////
void file_string(STRING * s)
{
	CSTR_line lin_in;
	Rect16 r = {s->xLeft,s->yTop,s->xRight,s->yBottom};
    Rect32  left, right ;
    Bool32  filtr=FALSE;

	nStrings++;

	if(!LDPUMA_Skip(hShowString))
		LDPUMA_DrawRect(NULL,&r,nIncline/2,__RGB__(0,127,0), 1,PUMA_MODULE_RBLOCK);

    lin_in = CSTR_NewLine(nStrings, 0, -1);
    if( lin_in == (CSTR_line)NULL )
        {
         SetReturnCode_rblock(CSTR_GetReturnCode());
		 longjmp (fatal_error_exit, -1);
        }
    else
        {
			int i = 0;
			ROOT * pRoot = NULL;
			CCOM_comp * com = NULL;

			CSTR_rast_attr  attr = {0};
			CSTR_rast       rst = { 0 };
			CSTR_attr       lattr = { 0 };


			lattr.incline	= nIncline;	// Incline*2048 = tg
			lattr.fragment	= s->nBlock - FIRST_REGULAR_BLOCK_NUMBER;
			lattr.col = s->xLeft;
			lattr.row = s->yTop;
			lattr.wid = s->xRight - s->xLeft + 1;
			lattr.hei = s->yBottom - s->yTop +1;
			strcpy(lattr.VersionName,"RBLOCKS"); // 64 bytes
			lattr.Flags = CSTR_STR_No;
			if(s->uFlags & CSTR_STR_CapDrop)
			{
				lattr.Flags += CSTR_STR_CapDrop;
			}


			CSTR_SetLineAttr (lin_in, &lattr);
// OLEG : Delete Garbage
if( s->nDust > s->nLetters*1 )
    {
    int32_t   le,     ri, nri, nle;
    for(le = 32000,ri=-16000,i=0;i<s->nLetters;i++)
		{
		com = (pRoots + s -> pLettersList [i])->pComp;
        if( le > com->left )        le=com->left;
        if( ri < com->left+com->w ) ri=com->left+com->w;
		}
    // find left & right letters limit
    left.left=right.left=32000;
    left.top=right.top=32000;
    left.right=right.right=-16000;
    left.bottom=right.bottom=-16000;
    for(nri=nle=i=0;i<s->nDust;i++)
        {
        com = (pRoots + s -> pDustList [i])->pComp;
        if( com->w*com->h <15 )
            {
            CCOM_comp   com1=*com;
            com1.upper=com1.upper-(int16_t)(nIncline*com1.left /2048);
            com1.left =com1.left +(int16_t)(nIncline*com1.upper/2048);
            if( com->left+com->w <= le )
                {
                nle++;
                if( left.left  > com1.left )        left.left  =com1.left;
                if( left.right < com1.left+com->w ) left.right =com1.left+com1.w;
                if( left.top   > com1.upper )       left.top   =com1.upper;
                if( left.bottom< com1.upper+com->h )left.bottom=com1.upper+com1.h;
                }
            if( com->left >= ri )
                {
                nri++;
                if( right.left  > com1.left )        right.left  =com1.left;
                if( right.right < com1.left+com->w ) right.right =com1.left+com1.w;
                if( right.top   > com1.upper )       right.top   =com1.upper;
                if( right.bottom< com1.upper+com->h )right.bottom=com1.upper+com1.h;
                }
            } // end of if select sizes
        } // end of dust cycl
    if( nle>10 && left.bottom-left.top>10 || nri>10 && right.bottom-right.top>10 )
        filtr= TRUE;
    } // end of if num dust > num let * ...
			// dust
			for(i=0;i<s->nDust;i++)
			{
			    pRoot = pRoots + s -> pDustList [i];
				com = pRoot->pComp;
                if( filtr &&  com->w*com->h <15 )
                    {
                    CCOM_comp   com1=*com;
                    com1.upper=com1.upper-(int16_t)(nIncline*com1.left /2048);
                    com1.left =com1.left +(int16_t)(nIncline*com1.upper/2048);
                    if( com1.upper>=left.top && com1.upper+com1.h<=left.bottom &&
                        com1.left>=left.left && com1.left+com1.w<=left.right )
                        continue;
                    if( com1.upper>=right.top && com1.upper+com1.h<=right.bottom &&
                        com1.left>=right.left && com1.left+com1.w<=right.right )
                        continue;
                    }
				if( (rst=(CSTR_rast)CSTR_NewRaster (lin_in ,com->left, com->upper,com->w)) != (CSTR_rast)NULL )
					{
					attr.row   = com->upper;
					attr.col   = com->left;
					attr.w     = com->w;
					attr.h     = com->h;
					attr.r_row = com->upper;
					attr.r_col = com->left;
					attr.flg   = CSTR_f_dust;
					CSTR_SetAttr (rst, &attr);
					CSTR_StoreComp (rst, (uchar*)com->linerep,1,com->scale);
                    if(com->vers )
					CSTR_StoreCollection(rst, com->vers);
					}
			}
			// letters
			for(i=0;i<s->nLetters;i++)
			{
			    pRoot = pRoots + s -> pLettersList [i];
				com = pRoot->pComp;
				if( (rst=(CSTR_rast)CSTR_NewRaster (lin_in ,com->left, com->upper, com->w)) != (CSTR_rast)NULL )
					{
					attr.row   = com->upper;
					attr.col   = com->left;
					attr.w     = com->w;
					attr.h     = com->h;
					attr.r_row = com->upper;
					attr.r_col = com->left;
					attr.flg   = CSTR_f_let|CSTR_f_bad;
					CSTR_SetAttr (rst, &attr);
 					CSTR_StoreComp (rst, (uchar*)com->linerep,1,com->scale);
                    if( s->uFlags & CSTR_STR_CapDrop )
                        {
                        CCOM_comp       *ci=com;
                        CCOM_comp       *co=CSTR_GetComp(rst);
                        CCOM_USER_BLOCK ub;
                        int32_t           n;

                        ub.code=CCOM_UB_CAPDROPLN;
                        ub.data=(uchar*)&n;
                        if( CCOM_GetUserBlock(ci,&ub) )
                            CCOM_SetUserBlock(co,&ub);
                        }
                    if( com->vers )
    					CSTR_StoreCollection(rst, com->vers);
					}
			}
        CSTR_PackLine(lin_in);
//		LDPUMA_CSTR_Monitor(NULL,lin_in,0,myMonitorProc);
        }

};
/////////////////////////////////////

static void LayoutFromCPAGE(Handle hCPAGE)
{
	Handle h = NULL;
	POLY_	block;
	int nBlocks = FIRST_REGULAR_BLOCK_NUMBER;
	Point16 pLeftTop, pRightTop, pLeftBottom, pRightBottom;
	ROOT * pRoot = NULL;
	uint32_t BlockNumber;
    // piter 08.07.99
	PAGEINFO    PInfo = {0};

    RotatePageToReal();

    if(CPAGE_GetPageData(hCPAGE,PT_PAGEINFO,(void*)&PInfo,sizeof(PInfo)))
        nIncline = PInfo.Incline2048;

    RotatePageToIdeal();
    // piter
	// remove ALL
	for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
	{
			pRoot -> nBlock = REMOVED_BLOCK_NUMBER;
	}

	/*
	//Rom 30 03 99
	BlocksFreeData();
	BlockNumber = 1;
	*/

	BlockNumber = 1;

	for(h = CPAGE_GetBlockFirst(hCPAGE,TYPE_TEXT);
	h!=NULL;
	h = CPAGE_GetBlockNext(hCPAGE,h,TYPE_TEXT))
	{
        uint32_t f = CPAGE_GetBlockFlags(hCPAGE,h);
		//BlockNumber = CPAGE_GetBlockUserNum(hCPAGE,h)*64000;// Piter 030399
		if(CPAGE_GetBlockData(hCPAGE,h,TYPE_TEXT, &block, sizeof(block))!=sizeof(block))
		{
			SetReturnCode_rblock(CPAGE_GetReturnCode());
			longjmp (fatal_error_exit, -1);
		}
		nBlocks++;
		for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
		{
			pLeftTop.x  = pRoot->xColumn + 1;
			pLeftTop.y  = pRoot->yRow + 1;
			pRightTop.x = pRoot->xColumn + pRoot->nWidth - 1;
			pRightTop.y = pRoot->yRow + 1;
			pLeftBottom.x  = pRoot->xColumn + 1;
			pLeftBottom.y  = pRoot->yRow + pRoot->nHeight - 1;
			pRightBottom.x = pRoot->xColumn + pRoot->nWidth - 1;
			pRightBottom.y = pRoot->yRow + pRoot->nHeight - 1;

			//if(IsInPoly(pLeftTop,&block) && IsInPoly(pRightBottom,&block))
			if(IsInPoly(pLeftTop,&block) ||
				IsInPoly(pRightTop,&block) ||
				IsInPoly(pLeftBottom,&block) ||
				IsInPoly(pRightBottom,&block) )
			{
				pRoot->nBlock = BlockNumber + FIRST_REGULAR_BLOCK_NUMBER;
				pRoot->nUserNum = BlockNumber;
			}
		}
		//CPAGE_SetBlockUserNum(hCPAGE,h,BlockNumber);// Piter 030399
		CPAGE_SetBlockInterNum(hCPAGE,h,BlockNumber);
		BlockNumber++;
	}

	//Rom
	//BlocksExtract();

	//**********************************************

	if(CPAGE_GetReturnCode())
	{
		SetReturnCode_rblock(CPAGE_GetReturnCode());
		longjmp (fatal_error_exit, -1);
	}

	BlocksExtract();
}
////////////////////////////////////////
int IsInPoly(Point16 a, void * pPoly)
{
	int i,y,n,ind;
	int Count = 0;
	POLY_ *p;
	p = (POLY_*)pPoly;
	n = p->com.count;
	for(i=0;i < n;i++)
	{
		int j = (i+1) % n;
		if(p->com.Vertex[i].y==p->com.Vertex[j].y) continue;
		if(p->com.Vertex[i].y> a.y && p->com.Vertex[j].y> a.y) continue;
		if(p->com.Vertex[i].y< a.y && p->com.Vertex[j].y< a.y) continue;
		y = p->com.Vertex[i].y;ind = i;
		if(p->com.Vertex[j].y > y)
		{
			y = p->com.Vertex[j].y;
			ind = j;
		}
		if((y== a.y)&&(p->com.Vertex[ind].x >= a.x))
			Count++;
		else if(MIN(p->com.Vertex[i].y,p->com.Vertex[j].y)== a.y)
				continue;
		else
		{
			double t = ((double)(a.y-p->com.Vertex[i].y)/((double)(p->com.Vertex[j].y-(double)p->com.Vertex[i].y)));
			if(t>0 && t<1 && (double)p->com.Vertex[i].x+t*((double)p->com.Vertex[j].x-(double)p->com.Vertex[i].x)>=(double)a.x)
				Count++;
		}
	}
	return Count & 1;
}
