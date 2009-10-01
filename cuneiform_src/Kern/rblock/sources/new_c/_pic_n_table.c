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
/*# include <crtdbg.h>*/

# include "new_c.h"
#include "minmax.h"

/************** External declaration ***********************/
//extern uchar *CellsPage;
//extern RPIC_Comp_Rect * pBigLetters;
//extern int nBigLetters;

//extern Handle logFile_comp;


//extern CCOM_comp *pComps;
//extern int nComps;

//int my_upper, my_left, my_bottom, my_right;
/***********************************************************/

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

int Max (int x1, int x2)
{
/*
	if (x1>x2) return x1;
		else return x2;
		*/
	//root_curr->bType = (siz==1) ? ROOT_DUST : 0;
	return (x1>x2) ? x1 : x2;

}

int Min (int x1, int x2)
{
/*
	if (x1<x2) return x1;
		else return x2;
		*/
	return x1<x2 ? x1 : x2;
}

Bool32 MyFiltrateOr (int32_t upper, int32_t left, int32_t w, int32_t h)
{
	if((upper<my_upper)&&(left<my_left)&&
	    (upper+h>my_bottom)&&(left+w>my_right)) return FALSE;
	if((upper>=my_bottom)||(left>=my_right)||
	    (upper+h<=my_upper)||(left+w<=my_left)) return FALSE;
	/*
	if(upper<my_bottom) return TRUE;
	if(left<my_right) return TRUE;
	if(upper+h>my_upper) return TRUE;
	if(left+w>my_left) return TRUE;
	*/

	return TRUE;
};


void DeleteRoot(ROOT * lpRoot)
{
	//ROOT* pRoot;
	int32_t nn;

	//for (pRoot = lpRoot; pRoot < pRoots + nRoots; pRoot++)
	//{
		//memcpy(pRoot,pRoot+1, sizeof(ROOT));
	//pRoot =
		nn = (lpRoot - pRoots)/sizeof(ROOT);
		nn = nRoots - nn-100;
		//nn = nn/2;
		nn = (pRoots+nRoots)-lpRoot-1;
		memcpy(lpRoot,lpRoot+1, nn*sizeof(ROOT));
	//}

	nRoots --;
}
Bool32 DeleteRootsFromTables (void)
{
	return TRUE;
}

Bool32 DeleteRootsFromPictures (void)
{
	Handle		pPage;
	Handle h = NULL;
	POLY_ block;
//	int i;
//	CCOM_comp comp;
	ROOT * pRoot;

	pPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage( ));

	for(h = CPAGE_GetBlockFirst(pPage,TYPE_IMAGE);
	h!=NULL;
	h = CPAGE_GetBlockNext(pPage,h,TYPE_IMAGE))
	{
		CPAGE_GetBlockData(pPage,h,TYPE_IMAGE, &block, sizeof(block));
		for (pRoot = pRoots; pRoot < pRoots + nRoots; pRoot++)
		{
			my_left   =  block.com.Vertex[0].x;
			my_right  =  block.com.Vertex[1].x;
			my_upper  =  block.com.Vertex[1].y;
			my_bottom =  block.com.Vertex[2].y;
			//if(MyFiltrateIn(pRoot -> yRow, pRoot -> xColumn, pRoot -> nWidth, pRoot -> nHeight ))
			if(MyFiltrateOr(pRoot -> yRow, pRoot -> xColumn, pRoot -> nWidth, pRoot -> nHeight ))
			{
				DeleteRoot(pRoot);
				pRoot--;
			}
		}
	}

	return TRUE;
}

Bool32 SearchPicturesSecond (Handle hCCOM, Bool32 BLOCKS, CCOM_comp * comp)
{
	return TRUE;
}
