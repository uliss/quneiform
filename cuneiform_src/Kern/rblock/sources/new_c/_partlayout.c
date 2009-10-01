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

# include <stdlib.h>
//# include "iolib.h"
/*
# include "layout.h"
# include "extract.h"
# include "kernel.h"
# include "status.h"

# include "ccom.h"
# include "newfunc.h"
# include "cpage.h"
# include "polyblock.h"
*/
/*
# include "exc.h"
# include "excdefs.h"
# include "dpuma.h"
# include "ctp.h"
# include "pic&table.h"
*/
# include "new_c.h"
#include "minmax.h"
/////////////////////////////////////
//extern uint16_t run_options;
//extern int nComps;
//extern CCOM_comp *pComps;
//extern uchar *CellsPage; // *** Rom 03-03-99
/*
#define ROM_TYPE_TABLE 111;
#define BOUND 15
#define PICS_QUANTUM 16
#define COMPS_QUANTUM 128
*/
extern Handle hNotUseAntonCross;
static int IsInPoly(Point32 a, POLY_ * pPoly);

Handle hCcom;

////////////////////////////////////
uint32_t NumberOfLettersInArea(Rect32 rect, int Number)
{
	uint32_t Result = 0;
	ROOT *pRoot;
	//int i;

	/*
	for (i=0; i<nComps; i++)
	{
		if( (pComps[i].left > rect.right)||
			(pComps[i].upper > rect.bottom)||
			(pComps[i].left + pComps[i].w < rect.left)||
			(pComps[i].upper + pComps[i].h < rect.top))
		{
			continue;
		}
		else
		{
			if(pComps[i].numcomp == Number)	Result++;
		}
	}
	*/

	for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
	{
		if( (pRoot->xColumn + pRoot->nWidth - 1<= rect.right) &&
			(pRoot->yRow + pRoot->nHeight - 1 <= rect.bottom) &&
			(pRoot->xColumn  >= rect.left) &&
			(pRoot->yRow >= rect.top) &&
			(pRoot->nBlock == Number) )
		   	  Result++;


	}

	return Result;
}

Bool32 DeleteVertex(POLY_ * poly, int position)
{
	int i;

	poly->com.count--;
	for(i=position;i<=poly->com.count; i++)
	{
		poly->com.Vertex[i] = poly->com.Vertex[i+1];
	}

	return TRUE;
}

Bool32 InsertVertex(POLY_ * poly, int position, Point32 point)
{
	int i;

	poly->com.count++;
	for(i=poly->com.count-1;i>=position; i--)
	{
		poly->com.Vertex[i] = poly->com.Vertex[i-1];
	}
	poly->com.Vertex[position] = point;

	return TRUE;
}

Bool32 InsertBottom(POLY_ * rectangle, POLY_ * poly)
{
	int i;
	Point32 point;

	for (i=0;i<poly->com.count-1; i++)
	{
		if( (poly->com.Vertex[i].x < rectangle->com.Vertex[3].x)&&
			(poly->com.Vertex[i].y < rectangle->com.Vertex[3].y)&&
			(poly->com.Vertex[i+1].x > rectangle->com.Vertex[2].x))
		{
			point.x = rectangle->com.Vertex[3].x;
			point.y = poly->com.Vertex[i].y;
			InsertVertex(poly, i+1, point);
			InsertVertex(poly, i+2, rectangle->com.Vertex[3]);
			InsertVertex(poly, i+3, rectangle->com.Vertex[2]);
			point.x = rectangle->com.Vertex[2].x;
			InsertVertex(poly, i+4, point);
			return TRUE;
		}
	}

	return TRUE;
}

Bool32 InsertTop(POLY_ * rectangle, POLY_ * poly)
{
	int i;
	Point32 point;

	for (i=0;i<poly->com.count-1; i++)
	{
		if( (poly->com.Vertex[i].x > rectangle->com.Vertex[1].x)&&
			(poly->com.Vertex[i].y > rectangle->com.Vertex[1].y)&&
			(poly->com.Vertex[i+1].x < rectangle->com.Vertex[0].x))
		{
			point.x = rectangle->com.Vertex[1].x;
			point.y = poly->com.Vertex[i].y;
			InsertVertex(poly, i+1, point);
			InsertVertex(poly, i+2, rectangle->com.Vertex[1]);
			InsertVertex(poly, i+3, rectangle->com.Vertex[0]);
			point.x = rectangle->com.Vertex[0].x;
			InsertVertex(poly, i+4, point);
			return TRUE;
		}
	}

	return TRUE;
}

Bool32 InsertLeft(POLY_ * rectangle, POLY_ * poly)
{
	int i;
	Point32 point;

	for (i=0;i<poly->com.count-1; i++)
	{
		if( (poly->com.Vertex[i].x > rectangle->com.Vertex[0].x)&&
			(poly->com.Vertex[i].y < rectangle->com.Vertex[0].y)&&
			(poly->com.Vertex[i+1].y > rectangle->com.Vertex[3].y))
		{
			point.x = poly->com.Vertex[i].x;
			point.y = rectangle->com.Vertex[0].y;
			InsertVertex(poly, i+1, point);
			InsertVertex(poly, i+2, rectangle->com.Vertex[0]);
			InsertVertex(poly, i+3, rectangle->com.Vertex[3]);
			point.y = rectangle->com.Vertex[3].y;
			InsertVertex(poly, i+4, point);
			return TRUE;
		}
	}

	return TRUE;
}

Bool32 InsertRight(POLY_ * rectangle, POLY_ * poly)
{
	int i;
	Point32 point;

	for (i=0;i<poly->com.count-1; i++)
	{
		if( (poly->com.Vertex[i].x < rectangle->com.Vertex[2].x)&&
			(poly->com.Vertex[i].y > rectangle->com.Vertex[2].y)&&
			(poly->com.Vertex[i+1].y < rectangle->com.Vertex[1].y))
		{
			point.x = poly->com.Vertex[i].x;
			point.y = rectangle->com.Vertex[2].y;
			InsertVertex(poly, i+1, point);
			InsertVertex(poly, i+2, rectangle->com.Vertex[2]);
			InsertVertex(poly, i+3, rectangle->com.Vertex[1]);
			point.y = rectangle->com.Vertex[1].y;
			InsertVertex(poly, i+4, point);
			return TRUE;
		}
	}

	if( (poly->com.Vertex[poly->com.count-1].x < rectangle->com.Vertex[2].x)&&
		(poly->com.Vertex[poly->com.count-1].y > rectangle->com.Vertex[2].y)&&
		(poly->com.Vertex[0].y < rectangle->com.Vertex[1].y))
	{
		point.x = poly->com.Vertex[poly->com.count-1].x;
		point.y = rectangle->com.Vertex[2].y;
		InsertVertex(poly, i+1, point);
		InsertVertex(poly, i+2, rectangle->com.Vertex[2]);
		InsertVertex(poly, i+3, rectangle->com.Vertex[1]);
		point.y = rectangle->com.Vertex[1].y;
		InsertVertex(poly, i+4, point);
	}

	return TRUE;
}

Bool32 InsertRectangleInPoly(POLY_ * rectangle, POLY_ * poly)
{
	if(IsInPoly(rectangle->com.Vertex[0], poly))
	{
		if(IsInPoly(rectangle->com.Vertex[1], poly))
			InsertTop(rectangle, poly);
		else
			InsertLeft(rectangle, poly);
	}
	else
	{
		if(IsInPoly(rectangle->com.Vertex[1], poly))
			InsertRight(rectangle, poly);
		else
			InsertBottom(rectangle, poly);
	}


	return TRUE;
}

Bool32 CrossedBy0 (POLY_ * poly, POLY_ * rectangle)
{
	int i;
	Point32 point;

	for (i=0;i<poly->com.count-2; i++)
	{
		if( (poly->com.Vertex[i].y < rectangle->com.Vertex[0].y)&&
			(poly->com.Vertex[i+1].x > rectangle->com.Vertex[0].x)&&
			(poly->com.Vertex[i+1].y > rectangle->com.Vertex[0].y)&&
			(poly->com.Vertex[i+2].x < rectangle->com.Vertex[0].x))
		{
			point.y = rectangle->com.Vertex[0].y;
			point.x = poly->com.Vertex[i].x;
			InsertVertex(poly, i+1, point);
			poly->com.Vertex[i+2] = rectangle->com.Vertex[0];
			point.y = poly->com.Vertex[i+3].y;
			point.x = rectangle->com.Vertex[0].x;
			InsertVertex(poly, i+3, point);
			return TRUE;
		}
	}

	return TRUE;
}

Bool32 CrossedBy1 (POLY_ * poly, POLY_ * rectangle)
{
	int i;
	Point32 point;


	for (i=0;i<poly->com.count-2; i++)
	{                   //STEPA_AM |
		if( (poly->com.Vertex[i].x > rectangle->com.Vertex[1].x)&&
			(poly->com.Vertex[i+1].x < rectangle->com.Vertex[1].x)&&
			(poly->com.Vertex[i+1].y > rectangle->com.Vertex[1].y)&&
			(poly->com.Vertex[i+2].y < rectangle->com.Vertex[1].y))
		{
			point.x = rectangle->com.Vertex[1].x;
			point.y = poly->com.Vertex[i].y;
			InsertVertex(poly, i+1, point);
			poly->com.Vertex[i+2] = rectangle->com.Vertex[1];
			point.x = poly->com.Vertex[i+3].x;
			point.y = rectangle->com.Vertex[1].y;
			InsertVertex(poly, i+3, point);
			return TRUE;
		}
	}
	if( (poly->com.Vertex[poly->com.count-2].x > rectangle->com.Vertex[1].x)&&
		(poly->com.Vertex[poly->com.count-1].x < rectangle->com.Vertex[1].x)&&
		(poly->com.Vertex[poly->com.count-1].y > rectangle->com.Vertex[1].y)&&
		(poly->com.Vertex[0].y < rectangle->com.Vertex[1].y))
	{
		point.x = rectangle->com.Vertex[1].x;
		point.y = poly->com.Vertex[poly->com.count-2].y;
		InsertVertex(poly, poly->com.count-1, point);
		//poly->com.Vertex[poly->com.count-1] = rectangle->com.Vertex[1];
		InsertVertex(poly, poly->com.count-1, rectangle->com.Vertex[1]);
		point.x = poly->com.Vertex[0].x;
		point.y = rectangle->com.Vertex[1].y;
		poly->com.Vertex[poly->com.count-1] = point;
		//InsertVertex(poly, poly->com.count-1, point);
		return TRUE;
	}


	return TRUE;
}

Bool32 CrossedBy2 (POLY_ * poly, POLY_ * rectangle)
{
	int i;
	Point32 point;


	for (i=0;i<poly->com.count-2; i++)
	{
		if( (poly->com.Vertex[i].y > rectangle->com.Vertex[2].y)&&
			(poly->com.Vertex[i+1].x < rectangle->com.Vertex[2].x)&&
			(poly->com.Vertex[i+1].y < rectangle->com.Vertex[2].y)&&
			(poly->com.Vertex[i+2].x > rectangle->com.Vertex[2].x))
		{
			point.y = rectangle->com.Vertex[2].y;
			point.x = poly->com.Vertex[i].x;
			InsertVertex(poly, i+1, point);
			poly->com.Vertex[i+2] = rectangle->com.Vertex[2];
			point.y = poly->com.Vertex[i+3].y;
			point.x = rectangle->com.Vertex[2].x;
			InsertVertex(poly, i+3, point);
			return TRUE;
		}
	}
	if( (poly->com.Vertex[poly->com.count-1].y > rectangle->com.Vertex[2].y)&&
		(poly->com.Vertex[0].x < rectangle->com.Vertex[2].x)&&
		(poly->com.Vertex[0].y < rectangle->com.Vertex[2].y)&&
		(poly->com.Vertex[1].x > rectangle->com.Vertex[2].x))
	{
		//Special case - use with cautions :)
		point.y = poly->com.Vertex[0].y;
		point.x = rectangle->com.Vertex[2].x;
		InsertVertex(poly, 1, point);
		point.y = rectangle->com.Vertex[2].y;
		point.x = poly->com.Vertex[0].x;
		//InsertVertex(poly, poly->com.count, point);
		InsertVertex(poly, 1, rectangle->com.Vertex[2]);
		poly->com.Vertex[0] = point;
		//poly->com.Vertex[0] = rectangle->com.Vertex[2];

		return TRUE;
	}


	return TRUE;
}

Bool32 CrossedBy3 (POLY_ * poly, POLY_ * rectangle)
{
	int i;
	Point32 point;

	for (i=0;i<poly->com.count-2; i++)
	{
		if( (poly->com.Vertex[i].x < rectangle->com.Vertex[3].x)&&
			(poly->com.Vertex[i+1].x > rectangle->com.Vertex[3].x)&&
			(poly->com.Vertex[i+1].y < rectangle->com.Vertex[3].y)&&
			(poly->com.Vertex[i+2].y > rectangle->com.Vertex[3].y))
		{
			point.x = rectangle->com.Vertex[3].x;
			point.y = poly->com.Vertex[i].y;
			InsertVertex(poly, i+1, point);
			poly->com.Vertex[i+2] = rectangle->com.Vertex[3];
			point.x = poly->com.Vertex[i+3].x;
			point.y = rectangle->com.Vertex[3].y;
			InsertVertex(poly, i+3, point);
			return TRUE;
		}
	}

	return TRUE;
}

//STEPA_AM
int GetPOLYHeight(POLY_* poly)
{
 int i;
 int min_top;
 int max_bottom;

 if(!(poly))
	 return 0;
 if(!(poly->com.count))
	 return 0;

 max_bottom=min_top=poly->com.Vertex[0].y;
 for(i=1;i<poly->com.count;i++)
 {
	 if(poly->com.Vertex[i].y<min_top)
		 min_top=poly->com.Vertex[i].y;
	 else
	 {
		 if(poly->com.Vertex[i].y>max_bottom)
			 max_bottom=poly->com.Vertex[i].y;
	 }
 }
 return max_bottom-min_top;
}



Bool32 PolysIsCrossed (POLY_ * poly1, POLY_ * poly2,Bool32 pic)
{

//STEPA_AM
//Пока так определяем кто врезаемый, а кто обтекаемый,
//Но правильность этого сомнительна
	if(!pic)
	{
		if(GetPOLYHeight(poly1)<GetPOLYHeight(poly2))
			return TRUE;
	}

//	if(NumberOfLettersInArea(rect, poly1) < NumberOfLettersInArea(rect, poly2))
//		return TRUE;

	if (IsInPoly(poly2->com.Vertex[0], poly1))
	{
		CrossedBy0(poly1, poly2);
		return TRUE;
	}
	if (IsInPoly(poly2->com.Vertex[1], poly1))
	{
		CrossedBy1(poly1, poly2);
		return TRUE;
	}
	if (IsInPoly(poly2->com.Vertex[2], poly1))
	{
		CrossedBy2(poly1, poly2);
		return TRUE;
	}
	if (IsInPoly(poly2->com.Vertex[3], poly1))
	{
		CrossedBy3(poly1, poly2);
		return TRUE;
	}

	return TRUE;
}

Bool32 PageRoatateBlocks (Handle hPage)
{
	Handle hBlock;
	POLY_	block;
	int i,j;
	div_t d1;
	int defect;

	// Теперь мы знаем угол, и можем повернуть картинки
	j = 0;
	hBlock = CPAGE_GetBlockFirst (hPage, TYPE_IMAGE);
	while (hBlock != NULL)
	{
		j++;
		CPAGE_GetBlockData(hPage, hBlock, TYPE_IMAGE, &block, sizeof(block));

		if(block.com.count==4)
		{
			if(nIncline>=0)
			{
				defect=((block.com.Vertex[1].x-block.com.Vertex[0].x)*nIncline)/INCLINE_FACTOR;
				block.com.Vertex[1].y+=defect;
				block.com.Vertex[3].y-=defect;
				defect=((block.com.Vertex[3].y-block.com.Vertex[0].y)*nIncline)/INCLINE_FACTOR;
				block.com.Vertex[0].x+=defect;
				block.com.Vertex[2].x-=defect;
			}
			else
			{
				defect=((block.com.Vertex[1].x-block.com.Vertex[0].x)*nIncline)/INCLINE_FACTOR;
				block.com.Vertex[0].y-=defect;
				block.com.Vertex[2].y+=defect;
				defect=((block.com.Vertex[3].y-block.com.Vertex[0].y)*nIncline)/INCLINE_FACTOR;
				block.com.Vertex[1].x+=defect;
				block.com.Vertex[3].x-=defect;
			}
		}
//		if(block.negative!=TYPE_NEGATIVE)
//		{
		 for (i=0; i<block.com.count; i++)
		 {
			IDEAL_XY(block.com.Vertex[i].x, block.com.Vertex[i].y);
		 }
//		}
		/*********************/

		for (i=0; i<block.com.count-1; i++)
		{
			//if(abs(block.com.Vertex[i].x - block.com.Vertex[i+1].x) <
			//	abs(block.com.Vertex[i].y - block.com.Vertex[i+1].y))
			d1 = div (i,2);
			if(d1.rem == 0)
			{
			  if(block.com.Vertex[i].y!=block.com.Vertex[i+1].y)
			     block.com.Vertex[i+1].y = block.com.Vertex[i].y;
			}
			else
			{
			  if(block.com.Vertex[i].x!=block.com.Vertex[i+1].x)
				block.com.Vertex[i].x=block.com.Vertex[i+1].x;

			}
		}
		if(block.com.Vertex[0].x!=block.com.Vertex[block.com.count-1].x)
			block.com.Vertex[block.com.count-1].x = block.com.Vertex[0].x;
		/*********************/
		sprintf(tmp_str, "  <4 О 1 %4d %4d %4d %4d %d \n",
			block.com.Vertex[0].x, block.com.Vertex[0].y,
			block.com.Vertex[1].x, block.com.Vertex[1].y,
			block.com.Vertex[2].y - block.com.Vertex[1].y);
		LDPUMA_FPuts(resFile_pict,tmp_str);
		/*********************/

		CPAGE_SetBlockData(hPage, hBlock, TYPE_IMAGE, &block, sizeof(block));
		hBlock = CPAGE_GetBlockNext(hPage, hBlock, TYPE_IMAGE);
	}

	sprintf(tmp_str, "  <4 Р %d %d %d \n", j, 0 ,0);
	LDPUMA_FPuts(resFile_pict, tmp_str);


	return TRUE;
}

////////////////////////////////////
void PageLayoutBlocks(Handle hCCOM)
{
	Handle hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage( ));
	file_name = LDPUMA_GetFileName(NULL);
	hCcom = hCCOM;

  //RootsLoadFile (DEFAULT_ROOTS_FILENAME);
	//run_options = 0;

	if(ReadRoots(hCCOM, TRUE))
	{
		LayoutPart1 ();

		LDPUMA_FPuts(resFile_pict, "  <4 Н После поворота \n");

		PageRoatateBlocks (hPage);

		LDPUMA_FPuts(resFile_pict, "  <4 К После поворота \n");
		LDPUMA_FPuts(resFile_pict, "  <3 К Картинки \n");
		LDPUMA_FPuts(resFile_pict, "  <2 К Страница \n");

		// ************************************************
		free (CellsPage); // *** Rom 03-03-99
		CellsPage = NULL; // *** Rom 03-03-99
	}

}

/**/
Bool32 DeletePoly(POLY_ * all_polys, int max, int i)
{
	int j;

	//!!! Redart 14.01.08 - Уменьшил с верхнюю границу цикла на 1
	for(j = i; j < max - 1; j++)
	{
		all_polys[j] = all_polys[j+1];
	}

	return TRUE;
}
/**/

/**/
Bool32 BlockAnalyse()
{
	BLOCK *p;
	//BLOCK *q;
	//BLOCK *r;
 uint32_t nAverageHeight;

	ROOT *pRoot;

	RecRaster rec;
	RecVersions vs;
	int16_t Language;


 uint32_t i = 0;
	int nRealLetters = 0;
	nAverageHeight = 0;

  if(nRoots == 0) return TRUE;

	for(i=0; i<nRoots; i++)
	{
		nAverageHeight += pRoots[i].nHeight;
	}
	nAverageHeight /= nRoots;



	//r = malloc(sizeof(BLOCK));
	//r->pNext = pBlocksList;

AGAIN_PQ:
	//for (p = r; p->pNext != NULL; p = p -> pNext)
	for (p = pBlocksList; p != NULL; p = p -> pNext)
    {
		//if(p->pNext->Rect.yBottom - p->pNext->Rect.yTop < nAverageHeight*0.7)
		if(p->Rect.yBottom - p->Rect.yTop < nAverageHeight*0.7)
		{
			/*
			q = p -> pNext;
			p->pNext = p->pNext->pNext;
			free(q);
			*/
			BlocksRemoveDescriptor(p);
			goto AGAIN_PQ;
		}

		if((uint32_t)(p->Rect.xRight-p->Rect.xLeft) < nAverageHeight)
		{
			/*
			q = p -> pNext;
			p->pNext = p->pNext->pNext;
			free(q);
			*/
			BlocksRemoveDescriptor(p);
			goto AGAIN_PQ;
		}

		/*
		if( (p->nLetters < 2)&&
			((p->pNext->nAverageHeight < nAverageHeight/0.7)||
			 (p->pNext->nAverageHeight > nAverageHeight*0.7)))
		*/
		if( (p->nLetters < 2)&&
			((p->nAverageHeight < nAverageHeight/0.7)||
			 (p->nAverageHeight > nAverageHeight*0.7)))
		{
			nRealLetters = 0;
			if(p->nRoots < 20)
			{
				//for(i=0;i<p->nRoots;i++)
				pRoot = p->pRoots;
				while(pRoot!=NULL)
				{
					/*
					if(p->pRoots[i].pComp == 0xcdcdcdcd)
						continue;
					*/
					if(!CCOM_GetRaster(pRoot->pComp, &rec)) continue;
					if(!CCOM_GetLanguage(hCcom, &Language)) continue;
					if(!RSTR_RecogOneLetter(&rec, Language, &vs))continue;
					if( (vs.lnAltCnt > 0)&&
						(vs.Alt[0].Prob > 240)&&
						(vs.Alt[0].Code != 'i')&&
						(vs.Alt[0].Code != 'I')&&
						(vs.Alt[0].Code != 'l'))
					{
						nRealLetters++;
					}
					pRoot = pRoot->u1.pNext;
				}
			}
			if((nRealLetters > 1)||(nRealLetters == 1 && p->nRoots<4))
			{
				continue;
			}
			/*
			q = p -> pNext;
			p->pNext = p->pNext->pNext;
			free(q);
			goto AGAIN_PQ;
			*/
			BlocksRemoveDescriptor(p);
			goto AGAIN_PQ;
		}
	}
	//pBlocksList = r->pNext;
	//free (r);

	return TRUE;
}
/**/
////////////////////////////////////
Bool32 OutputFragments(Handle hPage)
{
	extern BLOCK *pBlocksList;
	BLOCK * p = NULL;
	Bool32 rc = TRUE;
	int    count;
	uint32_t BlockNumber;
	int32_t  Min, PrevMin;
	Handle hBlock = NULL; // ***** Rom 01-03-99
	Handle h;
	Handle h_next;

	//Point16 Point;
	int i,j,max,nPics;
	POLY_  * pPics;
	Bool32 Same;


	POLY_	block;
	POLY_	* all_polys;

	LDPUMA_FPuts(resFile_blocks, "  <2 Н Страница =");
	LDPUMA_FPuts(resFile_blocks, file_name);
	LDPUMA_FPuts(resFile_blocks, " \n");
	//LDPUMA_FPuts(resFile_blocks, "  <2 Н Страница \n");
	LDPUMA_FPuts(resFile_blocks, "  <3 Н Фрагменты \n");
	//LDPUMA_FPuts(resFile_blocks, "  <4 Н Перед обработкой пересечений \n");
	LDPUMA_FPuts(resFile_blocks, "  <4 Н Результат работы старого фрагментатора \n");


	BlocksExtract(); // inserted by Rom to remove NULL-sized blocks
	BlockAnalyse();
	all_polys = malloc(COMPS_QUANTUM*sizeof(POLY_));
	pPics = malloc(PICS_QUANTUM*sizeof(POLY_));

	BlocksBuildLeftAndRightLists ();
	BlocksBuildTopAndBottomLists ();


//*************************************************
	count = 0;
	BlockNumber = 0;
	i=0;
	for (p = pTopBlocksList; p != NULL; p = p -> pDown)
    {
		if( p->Type == 111)
		{
			CPAGE_SetBlockUserNum(hPage, p->pHystogram, ++BlockNumber);
			continue;
		}

		if (i % COMPS_QUANTUM == 0)
				{
					all_polys = realloc (all_polys,
						(size_t) ((i / COMPS_QUANTUM + 1)
						* COMPS_QUANTUM * sizeof (POLY_)));
				}

		all_polys[i].com.type = TYPE_TEXT;//Текст, Картинка, Таблица;
		//all_polys[i].com.number = ++count;//порядковый номер
		all_polys[i].com.number = p->nNumber;//порядковый номер
		all_polys[i].com.Color = 0;
		all_polys[i].com.count = 4;
		all_polys[i].com.Vertex[0].x = p->Rect.xLeft;
		all_polys[i].com.Vertex[0].y = p->Rect.yTop;
		all_polys[i].com.Vertex[1].x = p->Rect.xRight;
		all_polys[i].com.Vertex[1].y = p->Rect.yTop;
		all_polys[i].com.Vertex[2].x = p->Rect.xRight;
		all_polys[i].com.Vertex[2].y = p->Rect.yBottom;
		all_polys[i].com.Vertex[3].x = p->Rect.xLeft;
		all_polys[i].com.Vertex[3].y = p->Rect.yBottom;
		all_polys[i].alphabet = 0;

		sprintf(tmp_str, "  <4 О 1 %4d %4d %4d %4d %d \n",
			all_polys[i].com.Vertex[0].x, all_polys[i].com.Vertex[0].y,
			all_polys[i].com.Vertex[1].x, all_polys[i].com.Vertex[1].y,
			all_polys[i].com.Vertex[2].y - all_polys[i].com.Vertex[1].y);
		LDPUMA_FPuts(resFile_blocks,tmp_str);

		i++;

	}
	max = i;

	sprintf(tmp_str, "  <4 Р %d %d %d \n", i,0 ,0);
	LDPUMA_FPuts(resFile_blocks, tmp_str);
	LDPUMA_FPuts(resFile_blocks, "  <4 К Результат работы старого фрагментатора \n");

	i=0;
	for(h = CPAGE_GetBlockFirst(hPage,TYPE_IMAGE);
	h!=NULL;
	h = CPAGE_GetBlockNext(hPage,h,TYPE_IMAGE))
	{
		if (i % COMPS_QUANTUM == 0)
				{
					pPics = realloc (pPics,
						(size_t) ((i / COMPS_QUANTUM + 1)
						* COMPS_QUANTUM * sizeof (POLY_)));
				}
		CPAGE_GetBlockData(hPage,h,TYPE_IMAGE, &pPics[i++], sizeof(POLY_));
	}
	nPics=i;


	/*Numeraciya Kartinok*/
	/*
	PrevMin = -2000;
	Min = -1000;
	while(Min != PrevMin)
	{
		PrevMin = Min;
		Min = 65535;

		for(i=0; i<nPics; i++)
		{
			if( (pPics[i].com.Vertex[0].y < Min) &&
				(pPics[i].com.Vertex[0].y > PrevMin))
			{
				Min = pPics[i].com.Vertex[0].y;
			}
		}

		for(i=0; i<nPics; i++)
		{
			if(pPics[i].com.Vertex[0].y == Min)
			{
				pPics[i].com.number = ++BlockNumber;
			}
		}
	}

	for(h = CPAGE_GetBlockFirst(hPage,TYPE_IMAGE);
	h!=NULL;
	h = CPAGE_GetBlockNext(hPage,h,TYPE_IMAGE))
	{
		CPAGE_GetBlockData(hPage, h, TYPE_IMAGE, &block, sizeof(POLY_));
		for(i=0; i<nPics; i++)
		{
			for(j=0; j<pPics[i].com.count; j++)
			{
				if(pPics[i].com.Vertex[j] != block.com.Vertex[i])
				{
					break;
				}
			}
			BlockNumber = all_polys[i].com.number;
			all_polys[i].com.number = 0;
		}
	}
	*/


	// **********************************************
	for ( i=0; i<max; i++)
    {
		for (j=0; j<nPics; j++)
		{
			if (pPics[j].com.count!=4) continue;
			block = pPics[j];
			block.com.Vertex[0].x-=BOUND;
			block.com.Vertex[0].y-=BOUND;
			block.com.Vertex[1].x+=BOUND;
			block.com.Vertex[1].y-=BOUND;
			block.com.Vertex[2].x+=BOUND;
			block.com.Vertex[2].y+=BOUND;
			block.com.Vertex[3].x-=BOUND;
			block.com.Vertex[3].y+=BOUND;
			count = 0;
			/*
			if(IsInPoly(all_polys[i].com.Vertex[0], &all_polys[j])) count++;
			if(IsInPoly(all_polys[i].com.Vertex[1], &all_polys[j])) count++;
			if(IsInPoly(all_polys[i].com.Vertex[2], &all_polys[j])) count++;
			if(IsInPoly(all_polys[i].com.Vertex[3], &all_polys[j])) count++;
			*/
			if(IsInPoly(block.com.Vertex[0], &all_polys[i])) count++;
			if(IsInPoly(block.com.Vertex[1], &all_polys[i])) count++;
			if(IsInPoly(block.com.Vertex[2], &all_polys[i])) count++;
			if(IsInPoly(block.com.Vertex[3], &all_polys[i])) count++;

			switch(count)
			{
			case 1:
				//PolysIsCrossed(&block, &all_polys[i]);
				PolysIsCrossed(&all_polys[i], &block,TRUE);
				break;
			case 2:
				InsertRectangleInPoly(&block, &all_polys[i]);
				break;
			case 3: break;
			case 4: break;
			default:break;
			}
		}
	}


	// ******** Processing crossed bloks ************
AGAIN:
	for ( i=0; i<max; i++)
    {
		for (j=0; j<max; j++)
		{
			if (all_polys[j].com.count!=4) continue;
			if (i==j) continue;
			block = all_polys[j];
			/*
			Check if fragments are not crossed without "bounding"
			*/
			if( !(IsInPoly(block.com.Vertex[0], &all_polys[i]))&&
				!(IsInPoly(block.com.Vertex[1], &all_polys[i]))&&
				!(IsInPoly(block.com.Vertex[2], &all_polys[i]))&&
				!(IsInPoly(block.com.Vertex[3], &all_polys[i])))
			{
				continue;
			}
			/**/
			if( (IsInPoly(block.com.Vertex[0], &all_polys[i]))&&
				(IsInPoly(block.com.Vertex[1], &all_polys[i]))&&
				(IsInPoly(block.com.Vertex[2], &all_polys[i]))&&
				(IsInPoly(block.com.Vertex[3], &all_polys[i])))
			{
				DeletePoly(all_polys, max, j);
				if(max > 0) max--;
				//goto AGAIN;
			}
			/**/

			/*
			if (all_polys[j].com.count!=4) continue;
			if (i==j) continue;
			block = all_polys[j];
			*/
			block.com.Vertex[0].x-=BOUND;
			block.com.Vertex[0].y-=BOUND;
			block.com.Vertex[1].x+=BOUND;
			block.com.Vertex[1].y-=BOUND;
			block.com.Vertex[2].x+=BOUND;
			block.com.Vertex[2].y+=BOUND;
			block.com.Vertex[3].x-=BOUND;
			block.com.Vertex[3].y+=BOUND;
			count = 0;
			/*
			if(IsInPoly(all_polys[i].com.Vertex[0], &all_polys[j])) count++;
			if(IsInPoly(all_polys[i].com.Vertex[1], &all_polys[j])) count++;
			if(IsInPoly(all_polys[i].com.Vertex[2], &all_polys[j])) count++;
			if(IsInPoly(all_polys[i].com.Vertex[3], &all_polys[j])) count++;
			*/
			if(IsInPoly(block.com.Vertex[0], &all_polys[i])) count++;
			if(IsInPoly(block.com.Vertex[1], &all_polys[i])) count++;
			if(IsInPoly(block.com.Vertex[2], &all_polys[i])) count++;
			if(IsInPoly(block.com.Vertex[3], &all_polys[i])) count++;

			switch(count)
			{
			case 1:
				//PolysIsCrossed(&block, &all_polys[i]);
				if(!LDPUMA_Skip(hNotUseAntonCross))
					PolysIsCrossed(&all_polys[i], &block,TRUE);
				else
				    PolysIsCrossed(&all_polys[i], &block,FALSE);
				break;
			case 2:
				if(all_polys[i].alphabet != block.com.number)
				{
					InsertRectangleInPoly(&block, &all_polys[i]);
					all_polys[i].alphabet = block.com.number;
					break;
				}
				else
				{
					goto AGAIN;
				}
			case 3: break;
			case 4: break;
			default:break;
			}
		}

	}
	// **********************************************

   	//LDPUMA_FPuts(resFile_blocks, "  <4 Н После обработки пересечений \n");
	LDPUMA_FPuts(resFile_blocks, "  <4 Н Финал \n");

	PrevMin = -2000;
	Min = -1000;
	//for (i=0; i<max; i++)
	while(Min != PrevMin)
	{
		PrevMin = Min;
		//Min = all_polys[0].com.Vertex[0].y;
		Min = 65535;

		for(i=0; i<max; i++)
		{
			if( (all_polys[i].com.Vertex[0].y < Min) &&
				(all_polys[i].com.Vertex[0].y > PrevMin))
			{
				Min = all_polys[i].com.Vertex[0].y;
			}
		}

		for(i=0; i<max; i++)
		{
			if(all_polys[i].com.Vertex[0].y == Min)
			{
				all_polys[i].com.number = ++BlockNumber;
			}
		}
	}

	j = 0;
	for ( i=0; i<max; i++)
    {
		all_polys[i].alphabet = 0;//Rom
		BlockNumber = all_polys[i].com.number;
		all_polys[i].com.number = 0;
		all_polys[i].negative=TYPE_POSITIVE;
		all_polys[i].orient=TYPE_LEFTRIGHT;

		if(all_polys[i].com.count == 4)
		{
			j++;
			sprintf(tmp_str, "  <4 О 1 %4d %4d %4d %4d %d \n",
				all_polys[i].com.Vertex[0].x, all_polys[i].com.Vertex[0].y,
				all_polys[i].com.Vertex[1].x, all_polys[i].com.Vertex[1].y,
				all_polys[i].com.Vertex[2].y - all_polys[i].com.Vertex[1].y);
			LDPUMA_FPuts(resFile_blocks,tmp_str);

		}

		hBlock = CPAGE_CreateBlock(hPage, TYPE_TEXT,0,0,&all_polys[i],sizeof(POLY_));
		if(!hBlock)
		{
			SetReturnCode_rblock(CPAGE_GetReturnCode());
			rc = FALSE;
			break;
		}
		CPAGE_SetBlockUserNum(hPage, hBlock, BlockNumber);
    }

	sprintf(tmp_str, "  <4 Р %d %d %d \n", j, 0 ,0);
	LDPUMA_FPuts(resFile_blocks, tmp_str);
	LDPUMA_FPuts(resFile_blocks, "  <4 К Финал \n");
	LDPUMA_FPuts(resFile_blocks, "  <3 К Фрагменты \n");
	LDPUMA_FPuts(resFile_blocks, "  <2 К Страница \n");



	/*Numeraciya Kartinok*/
	PrevMin = -2000;
	Min = -1000;
	while(Min != PrevMin)
	{
		PrevMin = Min;
		Min = 65535;

		for(i=0; i<nPics; i++)
		{
			if( (pPics[i].com.Vertex[0].y < Min) &&
				(pPics[i].com.Vertex[0].y > PrevMin))
			{
				Min = pPics[i].com.Vertex[0].y;
			}
		}

		for(i=0; i<nPics; i++)
		{
			if(pPics[i].com.Vertex[0].y == Min)
			{
				pPics[i].com.number = ++BlockNumber;
			}
		}
	}

	for(h = CPAGE_GetBlockFirst(hPage,TYPE_IMAGE);
	h!=NULL;
	h = CPAGE_GetBlockNext(hPage,h,TYPE_IMAGE))
	{
		CPAGE_GetBlockData(hPage, h, TYPE_IMAGE, &block, sizeof(POLY_));
		for(i=0; i<nPics; i++)
		{
			Same = TRUE;
			for(j=0; j<pPics[i].com.count; j++)
			{
				if( pPics[i].com.Vertex[j].x != block.com.Vertex[j].x ||
					pPics[i].com.Vertex[j].y != block.com.Vertex[j].y )
				{
					Same = FALSE;
					break;
				}
			}
			if (Same)
			{
				BlockNumber = pPics[i].com.number;
				pPics[i].com.number = 0;
				CPAGE_SetBlockUserNum(hPage, h, BlockNumber);
			}
		}
	}

//Негативы

	h = CPAGE_GetBlockFirst(hPage,TYPE_IMAGE);
	while(h)
	{
		h_next=CPAGE_GetBlockNext(hPage,h,TYPE_IMAGE);
		CPAGE_GetBlockData(hPage, h, TYPE_IMAGE, &block, sizeof(POLY_));
		if(block.negative==TYPE_NEGATIVE)
		{
			BlockNumber=CPAGE_GetBlockUserNum(hPage,h);
			CPAGE_DeleteBlock(hPage,h);
			block.alphabet=0;
			block.com.number=0;
			hBlock=CPAGE_CreateBlock(hPage,TYPE_TEXT,0,0,&block,sizeof(POLY_));
            CPAGE_SetBlockUserNum(hPage, hBlock, BlockNumber);
		}
		h=h_next;
	}






LT_FreeAllData ();

//CPAGE_BackUp(hPage); //Rom
free (all_polys);
free (pPics);

return rc;
}
//#######################################################
void CalculatePageIncline(Handle hCCOM, int32_t * lpNominator, int32_t * lpDenominator)
{
//  PageLayoutBlocks((int32_t)hCCOM);

  *lpDenominator = INCLINE_FACTOR;
  *lpNominator   = nIncline;
}
//#######################################################
//int IsInPoly(Point32 a, void * pPoly)
int IsInPoly(Point32 a, POLY_ * p)
{
	int i,y,n,ind;
	int Count = 0;
	//POLY_ *p;
	//p = (POLY_*)pPoly;
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

