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

#include <stdlib.h>

#include "minmax.h"
#include "resource.h"
#include "mymem.h"
#include "cpage.h"
#include "backup.h"
#include "polyblock.h"

#include "decl.h"

// extern functions

//###########################################
CPAGE_FUNC(Handle)  CPAGE_PictureGetFirst		(Handle hPage)
{
	PROLOG;
  SetReturnCode_cpage(IDS_ERR_NO);

	Handle rc = CPAGE_GetBlockFirst(hPage, TYPE_CPAGE_PICTURE);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_PictureGetNext(Handle hPage,Handle hPicture)
{
	PROLOG;
  SetReturnCode_cpage(IDS_ERR_NO);

	Handle rc = CPAGE_GetBlockNext(hPage,hPicture, TYPE_CPAGE_PICTURE);
	EPILOG;
	return rc;
}
//###########################################
#define ROTATE_2048(p,a) {\
             p.y = (int32_t) (p.y - (int32_t) p.x * a / 2048);\
             p.x = (int32_t) (p.x + (int32_t) p.y * a / 2048);\
		}

CPAGE_FUNC(Bool32)  CPAGE_PictureGetPlace(Handle hPage,Handle hPicture,int32_t Skew2048, Point32 * lpLr, Point32 * lpWh)
{
	PROLOG;
	Bool32 rc = FALSE;

	SetReturnCode_cpage(IDS_ERR_NO);

	CPAGE_PICTURE pict = {0};
	Point32 lt = {0};
	Point32 rb = {0};

	_ASSERT(lpLr);
	_ASSERT(lpWh);
	if(CPAGE_GetBlockData(hPage,hPicture,TYPE_CPAGE_PICTURE,&pict,sizeof(pict))==sizeof(pict))
	{
		lt = pict.Corner[0];
		rb = pict.Corner[0];
		ROTATE_2048(lt,Skew2048);
		for(uint32_t i = 1; i<pict.Number;i++)
		{
			ROTATE_2048(pict.Corner[i],Skew2048);
			if(lt.x > pict.Corner[i].x)
				lt.x = pict.Corner[i].x;
			if(lt.y > pict.Corner[i].y)
				lt.y = pict.Corner[i].y;
			if(rb.x < pict.Corner[i].x)
				rb.x = pict.Corner[i].x;
			if(rb.y < pict.Corner[i].y)
				rb.y = pict.Corner[i].y;
		}
		*lpLr = lt;
		lpWh->x = rb.x - lt.x;
		lpWh->y = rb.y - lt.y;
		rc = TRUE;
	}
	EPILOG;
return rc;
}
//###########################################
#define MAXDIFF 0 // максимальное расхождение в координатах при определении верт или гор.
//При значениях >0 работает неверно при заполнении массивов lpVer и lpHor при коротких линиях !!! Art

static int _cdecl CompareLong( const void *arg1, const void *arg2 ){return (*(long*)arg1-*(long*)arg2);}
static int GetIndex(long * lpLong,long nLong,long n)
{
	int i = 0;
	for(i = 0;i<nLong;i++)
	{
		if(abs(n-lpLong[i])<=MAXDIFF)
			break;
	}
return i;
}

CPAGE_FUNC(Bool32)  CPAGE_PictureGetMask(Handle hPage, Handle hPicture, int32_t Skew2048,
									  char * lpData, uint32_t * lpSize)
{
	PROLOG;
	Bool32 rc = FALSE;

	SetReturnCode_cpage(IDS_ERR_NO);

	_ASSERT(lpSize);
/*
	if(CPAGE_PictureGetPlace(hPage,hPicture,Skew2048, &lt, &wh))
	{
		uint32_t size = (wh.x + 7)/8 * wh.y;
		*lpSize = size;
		if(lpData && *lpSize == size)
		{
			memset(lpData,0xFF,size);
		}
		rc = TRUE;
	}
	*/
	CPAGE_PICTURE pict = {0};
	if(CPAGE_GetBlockData(hPage,hPicture,TYPE_CPAGE_PICTURE,&pict,sizeof(pict))==sizeof(pict))
	{
		int i,j;
		int nVer,nHor,sz_x,sz_y;
		int nMaxVer = 0;
		long * lpVer = NULL;
		int nMaxHor = 0;
		long * lpHor = NULL;
		char * lpMatrix = NULL;
		// Подсчитаем число вертикальных разделителей
		for(i=0;i<pict.Number;i++)
		{
			int ci = (i+1)%pict.Number;
			if(abs(pict.Corner[i].x - pict.Corner[ci].x) <= MAXDIFF)
				nMaxVer++;
			/*else*/ if(abs(pict.Corner[i].y - pict.Corner[ci].y) <= MAXDIFF)
				nMaxHor++;
		}
		// создадим массивы линий
		_ASSERT(nMaxVer>1);
		_ASSERT(nMaxHor>1);
		if(nMaxVer<2 || nMaxHor<2 )
			return FALSE;
		lpVer = (long*)malloc(sizeof(long)*nMaxVer);
		lpHor = (long*)malloc(sizeof(long)*nMaxHor);
		lpMatrix = (char*)malloc(sizeof(char)*nMaxVer*(nMaxHor-1));
		if(lpVer && lpHor && lpMatrix)
		{
			memset(lpMatrix,0,sizeof(char)*nMaxVer*(nMaxHor-1));
			for(nVer=nHor=0,i=0;i<pict.Number;i++)
			{
				int ci = (i+1)%pict.Number;
				if(abs(pict.Corner[i].x - pict.Corner[ci].x) <= MAXDIFF)
					lpVer[nVer++] = pict.Corner[i].x;
				/*else*/ if(abs(pict.Corner[i].y - pict.Corner[ci].y) <= MAXDIFF)
					lpHor[nHor++] = pict.Corner[i].y;
			}
		}
		else
		{
			SetReturnCode_cpage(IDS_ERR_NO_MEMORY);
			goto lOut;
		}
		// Упорядочим их
		qsort(lpVer,nMaxVer,sizeof(long),CompareLong);
		// Уберем повторяющиеся величины
		for(i=1;i<nMaxVer;i++)
		{
			if(lpVer[i]==lpVer[i-1])
			{
				memcpy(lpVer+i-1,lpVer+i,sizeof(lpVer[0])*(nMaxVer-i));
				nMaxVer--;
				i--;
				continue;
			}
		}
		qsort(lpHor,nMaxHor,sizeof(long),CompareLong);
		for(i=1;i<nMaxHor;i++)
		{
			if(lpHor[i]==lpHor[i-1])
			{
				memcpy(lpHor+i-1,lpHor+i,sizeof(lpHor[0])*(nMaxHor-i));
				nMaxHor--;
				i--;
				continue;
			}
		}
		// Создадим матрицу описания границ
		for(i=0;i<pict.Number;i++)
		{
			int ci = (i+1)%pict.Number;
			int delta_x = pict.Corner[i].x - pict.Corner[ci].x;
			int delta_y = pict.Corner[i].y - pict.Corner[ci].y;
			if(abs(delta_x) <= MAXDIFF)
			{// вертикальная граница
				int sign = delta_y ? (delta_y/abs(delta_y)):1;
				int x  = GetIndex(lpVer,nMaxVer,pict.Corner[i].x);
				int y1 = GetIndex(lpHor,nMaxHor,pict.Corner[i].y);
				int y2 = GetIndex(lpHor,nMaxHor,pict.Corner[ci].y);
				if (x<nMaxVer&&y1<nMaxHor&&y2<nMaxHor)
					for(int y = MIN(y1,y2);y<MAX(y1,y2);y++)
						*(lpMatrix + x + y*nMaxVer) = sign;
			}
		}
		// Создадим маску по матрице
		sz_x = (lpVer[nMaxVer-1] - lpVer[0] +7)/8;
		sz_y = lpHor[nMaxHor-1] - lpHor[0];

		_ASSERT(sz_x>0 && sz_y>0);
		*lpSize = sz_x*sz_y;
		rc = TRUE;

		if(lpData)
		{
			int sign = 0;
			memset(lpData,0,*lpSize);
			for(int y = 0;y<(nMaxHor-1);y++)
			{
				int sp = 0;
				for(int x = 0;x<nMaxVer;x++)
				{
					int cs = *(lpMatrix + x + y*nMaxVer);
					if(cs)
					{
						if(!sign)
							sign = cs;
						if(cs==sign)
							sp = x;
						else
						{ // Записываем маску
							int beg_x = (lpVer[sp]-lpVer[0])/8;
							int end_x = (lpVer[x]-lpVer[0] +7)/8;
							int beg_y = lpHor[y] - lpHor[0];
							int end_y = lpHor[y+1] - lpHor[0];
							for(i = beg_y;i<end_y;i++)
								for(j = beg_x;j<end_x;j++)
								{
									*(lpData + i*sz_x + j)=(char)0xFF;
								}
						}
					}
				}
			}
		}
lOut:if(lpHor)
		 free(lpHor);
	 if(lpVer)
		 free(lpVer);
	 if(lpMatrix)
		 free(lpMatrix);
	}
	EPILOG;
return rc;
}
