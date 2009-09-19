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
#include <assert.h>
#include "compat_defs.h"

# define __LOAD_ROOTS_C__

/*
# include "iolib.h"
# include "layout.h"
# include "extract.h"
# include "kernel.h"
# include "status.h"
# include "ccom.h"
# include "cpage.h" //Rom 08-02-99
# include "newfunc.h"
# include "exc.h"
# include "excdefs.h"
# include "dpuma.h"
# include "polyblock.h"
# include "ctp.h" //Rom 08-02-99
# include "pic&table.h"
# include "rpic.h"
*/
# include "new_c.h"
Bool32 NPFArbitr(CCOM_comp*,CCOM_comp*,Bool32);
int NPFGetBD(void);
#define NEGA   1<<14
#define VERTICA 1<<13
#define FROMDOWN 1<<12



# undef __LOAD_ROOTS_C__

extern Handle hDebugCancelGreatComp;
extern Handle hDebugCancelPicturesAndTables;
#define POSSIBLE_PICTURES CPAGE_GetInternalType("possible pictures")

Bool32 Open_Res_Log( void )
{

	logFile_comp   = LDPUMA_FOpen(LOGFILENAME_COMP  ,"at");
	resFile_comp   = LDPUMA_FOpen(RESFILENAME_COMP  ,"at");
	logFile_pict   = LDPUMA_FOpen(LOGFILENAME_PICT  ,"at");
	resFile_pict   = LDPUMA_FOpen(RESFILENAME_PICT  ,"at");
	logFile_blocks = LDPUMA_FOpen(LOGFILENAME_BLOCKS,"at");
	resFile_blocks = LDPUMA_FOpen(RESFILENAME_BLOCKS,"at");



	return TRUE;
}

Bool32 Close_Res_Log( void )
{

	if (logFile_comp!=NULL)
	{
		LDPUMA_FClose(logFile_comp  );
		logFile_comp=NULL;
	}
	if (resFile_comp!=NULL)
	{
		LDPUMA_FClose(resFile_comp  );
		resFile_comp=NULL;
	}
	if (logFile_pict!=NULL)
	{
		LDPUMA_FClose(logFile_pict  );
		logFile_pict=NULL;
	}
	if (resFile_pict!=NULL)
	{
		LDPUMA_FClose(resFile_pict  );
		resFile_pict=NULL;
	}
	if (logFile_blocks!=NULL)
	{
		LDPUMA_FClose(logFile_blocks  );
		logFile_blocks=NULL;
	}
	if (resFile_blocks!=NULL)
	{
		LDPUMA_FClose(resFile_blocks  );
		resFile_blocks=NULL;
	}


	return TRUE;
}


Bool32 DeleteFromPics(uint32_t position)
{
	uint32_t i = 0;
	nPics--;
	for(i=position;i<nPics;i++)
	{
		pPics[i] = pPics[i+1];
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
Bool32 IsNotGoodComp (PAGEINFO	pInfo, CCOM_comp *comp)
{

	if ((comp->upper < pInfo.Height*0.1)||
		(comp->left < pInfo.Width*0.1)||
		(comp->upper + comp->h > pInfo.Height*0.9)||
		(comp->left + comp->w > pInfo.Width*0.9))
	{
		if ( (comp->h/comp->w > 6) || (comp->w/comp->h > 6)) return TRUE;
		//if (comp->scale < 2) return TRUE;
		if ((comp->h > pInfo.Height*0.95) || (comp->w > pInfo.Width*0.95)) return TRUE;
		if (comp->h*comp->w > pInfo.Height*pInfo.Width*0.6) return TRUE;
	}
	else
	{
		if ( (comp->h/comp->w > 10) || (comp->w/comp->h > 10)) return TRUE;
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
Bool32 MyFiltrateIn (int32_t upper, int32_t left, int32_t w, int32_t h)
{
	if( (upper<=my_upper) ||
		(left<=my_left) ||
		(upper+h>=my_bottom) ||
		(left+w>=my_right) )
	{
		return FALSE;
	}

	return TRUE;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
Bool32 PicturesSecondStage(Handle hCCOM,Handle hCPAGE)
{
	/************** final union all crossed and included pictures ******************/

	CCOM_comp common;
	POLY_	block;
	Handle h = NULL;

	uint32_t i,j;
	CCOM_comp pic;
	CCOM_comp * comp;
	uint16_t pLocalHystogram[MaxHeightOfLetter];
	uint32_t nLocalComps;
	uint32_t MaxLocal, nMaxLocal;
	uint32_t Result1, Result2;
    PAGEINFO info = {0};//Almi
	int PageWidth;//Almi

//NPF makros:
   int BOUND_DEFECT = NPFGetBD();


	/********/
	for(i=0; i<nPics; i++)
	{
		pPics[i].nl = 0;
		pic = pPics[i];
		my_upper  = pic.upper;
		my_left   = pic.left;
		my_bottom = pic.upper + pic.h;
		my_right  = pic.left + pic.w;


		//Additional parameter - local distribution of heights
		//in case of local fragment with different (from main) height
		nLocalComps = 0;
		for(j=0;j<MaxHeightOfLetter;j++)
		{
			pLocalHystogram[j] = 0;
		}

		comp = CCOM_GetFirst(hCCOM, &MyFiltrateIn);
		while(comp)
		{
			if( (comp->h < MaxHeightOfLetter) && (comp->h > MinHeightOfLetter) )
			{
				pLocalHystogram[comp->h]++;
				nLocalComps++;
			}
			comp = CCOM_GetNext(comp, &MyFiltrateIn);
		}

		if (nLocalComps!=0)
		{
			MaxLocal = pLocalHystogram[0];
			for(j=0;j<MaxHeightOfLetter;j++)
			{
				if(pLocalHystogram [j] > MaxLocal)
				{
					MaxLocal = pLocalHystogram [j];
					nMaxLocal = j;
				}
			}

			Result1 = 0;
			Result2 = 0;
			sprintf(tmp_str, " %i Result1 (must be 0)\n", Result1);
			LDPUMA_FPuts(logFile_comp,tmp_str);
			sprintf(tmp_str, " %i Result2 (must be 0)\n", Result2);
			LDPUMA_FPuts(logFile_comp,tmp_str);
			sprintf(tmp_str, " %i (nComps)\n", nComps);
			LDPUMA_FPuts(logFile_comp,tmp_str);

			comp = CCOM_GetFirst(hCCOM, &MyFiltrateIn);
			while(comp)
			{
				//sprintf(tmp_str, " %i  (comp)\n", comp);
				//LDPUMA_FPuts(hFile,tmp_str);
				if( (comp->h < MaxHeightOfLetter) && (comp->h > MinHeightOfLetter) )
				{

					sprintf(tmp_str, " %i  (comp->h)\n", comp->h);
					LDPUMA_FPuts(logFile_comp,tmp_str);
					sprintf(tmp_str, " %i  (pBlockHystogram[comp->h])\n", pBlockHystogram[comp->h]);
					LDPUMA_FPuts(logFile_comp,tmp_str);


					//Result1 += OVERALL*pBlockHystogram[comp->h];
					Result1 += pBlockHystogram[comp->h]/20+
						pBlockHystogram[comp->h-1]/20+pBlockHystogram[comp->h+1]/20;
					if(comp->h - nMaxLocal < DIFFERENCE1)
					{
						//Result2 += LOCAL*pLocalHystogram[comp->h];
						Result2 += pLocalHystogram[comp->h]+pLocalHystogram[comp->h-1]+pLocalHystogram[comp->h+1];
					}
				}
				comp = CCOM_GetNext(comp, &MyFiltrateIn);
			}

			//Result1 = Result1/nComps;
			//Result1 = Result1/10;
			Result1 = Result1/20;
			//Result2 = Result2/nLocalComps;
			//Result2 = Result2/20;
			Result2 = Result2/40;

			sprintf(tmp_str , " Picture %i (i)\n", i);
			LDPUMA_FPuts(logFile_comp,tmp_str);
			sprintf(tmp_str, " %i (Result1)\n", Result1);
			LDPUMA_FPuts(logFile_comp,tmp_str);
			sprintf(tmp_str, " %i (Result2)\n", Result2);
			LDPUMA_FPuts(logFile_comp,tmp_str);

			if(Result1 + Result2 > CONDITION1)
			{
				DeleteFromPics(i);
				i--;
				continue;
			}
		}
	}

	/***********/
	/* Добавляем картинки, выделенные на первом шаге и производим
	   финальное объединение */
	/***********/
	for(h = CPAGE_GetBlockFirst(hCPAGE,TYPE_IMAGE);
	h!=NULL;
//	h = CPAGE_GetBlockNext(hCPAGE,h,TYPE_IMAGE)	Paul 02-04-2002
	)
	{
		Handle hNext = CPAGE_GetBlockNext(hCPAGE,h,TYPE_IMAGE);	 // Paul 02-04-2002
		CPAGE_GetBlockData(hCPAGE,h,TYPE_IMAGE, &block, sizeof(block));
		CPAGE_DeleteBlock(hCPAGE, h);
		h = hNext;												 // Paul 02-04-2002

		if (nPics % PICS_QUANTUM == 0)
		{
			pPics = (CCOM_comp *) realloc (pPics,
				(size_t) ((nPics / PICS_QUANTUM + 1)* PICS_QUANTUM * sizeof (CCOM_comp)));
		}


		comp = &pPics[nPics++];
		comp->upper = block.com.Vertex[0].y;
		comp->left  = block.com.Vertex[0].x;
		comp->w     = block.com.Vertex[1].x - block.com.Vertex[0].x;
		comp->h     = block.com.Vertex[2].y - block.com.Vertex[1].y;
		/* У comp нету поля флагов, поэтому используем nl */
		comp->nl	= block.com.Flags;
        if (block.orient == TYPE_DOWNUP) comp->nl |= FROMDOWN;

	}
	/***********/

	for (i=0; i<nPics; i++)
	{
		for (j=0; j<nPics; j++)
		{
			if(i==j)
				continue;
			if ( (pPics[i].upper  < pPics[j].upper+BOUND_DEFECT)&&
				 (pPics[i].left   < pPics[j].left+BOUND_DEFECT)&&
				 (pPics[i].left   + pPics[i].w+BOUND_DEFECT > pPics[j].left + pPics[j].w)&&
				 (pPics[i].upper  + pPics[i].h+BOUND_DEFECT > pPics[j].upper + pPics[j].h) )
			{
				if(NPFArbitr(&(pPics[j]),&(pPics[i]),0))
				{
				 DeleteFromPics (i);
				 j=0;
				}
				else
				{
				 //NPF find error
				 if(i>j)
					 i--;
				 //end NPF
				 DeleteFromPics (j);
				 j--;
				}
			}
			if(i==nPics)
				break;
		}
	}


	for (i=0; i<nPics; i++)
	{
		for (j=0; j<nPics; j++)
		{
			if (i==j) continue;
			if ( (pPics[i].upper - VER_BOUND_SIZE < pPics[j].upper + pPics[j].h)&&
				 (pPics[i].left  - HOR_BOUND_SIZE < pPics[j].left  + pPics[j].w)&&
				 (pPics[i].left  + pPics[i].w + HOR_BOUND_SIZE > pPics[j].left)&&
				 (pPics[i].upper + pPics[i].h + VER_BOUND_SIZE > pPics[j].upper) )
			{
			   if(NPFArbitr(&(pPics[j]),&(pPics[i]),1))
					;
               else
			   {
				common = pPics[i];
				if (pPics[i].upper >= pPics[j].upper)
					common.upper = pPics[j].upper;
				if (pPics[i].left >= pPics[j].left)
					common.left = pPics[j].left;
				if (pPics[i].upper + pPics[i].h < pPics[j].upper + pPics[j].h)
					common.h = pPics[j].upper + pPics[j].h - pPics[i].upper;
				if (pPics[i].left  + pPics[i].w < pPics[j].left + pPics[j].w)
					common.w = pPics[j].left + pPics[j].w - pPics[i].left;
				common.h += pPics[i].upper - common.upper;
				common.w += pPics[i].left  - common.left;
				pPics[i] = common;
				DeleteFromPics (j);
				j=0;
				i=0;
			   }
			}
		}
	}

	/***********************************************************************************/


//	LastCheck( hCCOM, hCPAGE);

	LDPUMA_FPuts(resFile_pict, "  <4 Н После второго прохода \n");
	sprintf(tmp_str, "  <4 Р %d %d %d \n", nPics,0 ,0);
	LDPUMA_FPuts(resFile_pict, tmp_str);

//Almi 12.07.00
    GetPageInfo(hCPAGE, &info);
    switch (info.Angle)
	{
		case 90 :
		case 270 :
			PageWidth = info.Height;
			break;
		default :
			PageWidth = info.Width;
	}
//End of Almi
	for(i=0;i<nPics && bSearchPicture ;i++)
	{
		block.com.type = TYPE_TEXT;//Текст, Картинка, Таблица;
		block.com.number = 0;//порядковый номер
		block.com.Color = 0;
		block.com.count = 4;
		block.com.Flags = pPics[i].nl;
		block.com.Vertex[0].x = pPics[i].left;
		block.com.Vertex[0].y = pPics[i].upper;
		block.com.Vertex[1].x = pPics[i].left + pPics[i].w;
		block.com.Vertex[1].y = pPics[i].upper;
		block.com.Vertex[2].x = pPics[i].left + pPics[i].w;
		block.com.Vertex[2].y = pPics[i].upper + pPics[i].h;
		block.com.Vertex[3].x = pPics[i].left;
		block.com.Vertex[3].y = pPics[i].upper + pPics[i].h;
		block.alphabet = 0;
		if(pPics[i].nl&NEGA)
		{
			block.negative=TYPE_NEGATIVE;
			if(pPics[i].nl&VERTICA)
			{
                if (pPics[i].nl&FROMDOWN) block.orient = TYPE_DOWNUP;
                else block.orient = TYPE_UPDOWN;
            }
			else
				block.orient=TYPE_LEFTRIGHT;
		}
		else
		{
			block.negative=TYPE_POSITIVE;
		}

		sprintf(tmp_str, "  <4 О 1 %4d %4d %4d %4d %d \n",
			pPics[i].left, pPics[i].upper, pPics[i].left + pPics[i].w, pPics[i].upper, pPics[i].h);
		LDPUMA_FPuts(resFile_pict,tmp_str);


	  CPAGE_CreateBlock(hCPAGE, TYPE_IMAGE,0,0,&block,sizeof(POLY_));
	}

	LDPUMA_FPuts(resFile_pict, "  <4 К После второго прохода \n");


	return TRUE;
}

Bool32 FillBigLetters (Handle hCCOM,Handle hCPAGE)
{
	Handle BlockType;
	Handle pBlock;
	RPIC_Comp_Rect CompRect; // типичный Rect16

	BlockType = CPAGE_GetInternalType("pic's to letters boxes");

	pBlock = CPAGE_GetBlockFirst(hCPAGE, BlockType);
	while(pBlock)
	{
		if (nBigLetters % BIG_LETTERS_QUANTUM == 0)
		{
			pBigLetters = realloc (pBigLetters,
				(size_t) ((nBigLetters / BIG_LETTERS_QUANTUM + 1)
				* BIG_LETTERS_QUANTUM * sizeof (RPIC_Comp_Rect)));
		}

		//CPAGE_GetBlockData,(Handle page,Handle block,uint32_t Type, void * lpData, uint32_t Size));
		CPAGE_GetBlockData(hCPAGE, pBlock, BlockType, &CompRect, sizeof(CompRect));
		nBigLetters++;
		pBigLetters[nBigLetters-1] = CompRect;

		pBlock = CPAGE_GetBlockNext(hCPAGE, pBlock, BlockType);
	}

	return TRUE;
}


Bool32 FillPicsInTables (Handle hCCOM,Handle hCPAGE)
{
	POLY_	block;
	Handle h = NULL;

	CCOM_comp * comp;

	for(h = CPAGE_GetBlockFirst(hCPAGE,POSSIBLE_PICTURES);
	h!=NULL;
	)
	{
		Handle hNext = CPAGE_GetBlockNext(hCPAGE,h,POSSIBLE_PICTURES);	 // Paul 02-04-2002
		CPAGE_GetBlockData(hCPAGE,h,POSSIBLE_PICTURES, &block, sizeof(block));
		CPAGE_DeleteBlock(hCPAGE, h);
		h = hNext;												 // Paul 02-04-2002

		if (nPics % PICS_QUANTUM == 0)
		{
			pPics = (CCOM_comp *) realloc (pPics,
				(size_t) ((nPics / PICS_QUANTUM + 1)* PICS_QUANTUM * sizeof (CCOM_comp)));
		}


		comp = &pPics[nPics++];
		comp->upper = block.com.Vertex[0].y;
		comp->left  = block.com.Vertex[0].x;
		comp->w     = block.com.Vertex[1].x - block.com.Vertex[0].x;
		comp->h     = block.com.Vertex[2].y - block.com.Vertex[1].y;
	}

	return TRUE;
}


Bool32 IsNotBigLetter (CCOM_comp *comp)
{
	int i;

	if(comp->scale > 3) return TRUE;
	for(i=0; i<nBigLetters; i++)
	{
		if( (comp->upper >= pBigLetters[i].upper)&&
			(comp->left >= pBigLetters[i].left)&&
			(comp->upper + comp->h <= pBigLetters[i].upper + pBigLetters[i].h)&&
			(comp->left + comp->w <= pBigLetters[i].left + pBigLetters[i].w))
		{
			return FALSE;
		}
	}

	return TRUE;
}


/************************************************************************/

Bool32 ReadRoots(Handle hCCOM, Bool32 BLOCKS)
{
	CCOM_comp * comp = NULL;
	Handle		pPage;
	PAGEINFO	pInfo;
	uint32_t i;

	exthCCOM = hCCOM;
    RootsFreeData ();
	nRoots = 0;

	if (BLOCKS)
	{
		if(pPics)
		{
			free(pPics);
			pPics = NULL;
		}
		nPics = 0;
	}

	pPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage( ));
	CPAGE_GetPageData( pPage, PT_PAGEINFO, (void*)&pInfo, sizeof(pInfo));

	nBigLetters = 0;
	pBigLetters = NULL;
	FillBigLetters(hCCOM, pPage); // ну, hCCOM здесь ни при чем..
                             //и вообще непонятно, что он делает в этом вызове
	FillPicsInTables(hCCOM, pPage);



	if (BLOCKS)
	{
		if (!CellsPage)
		{
			CellsPage = malloc(PAGE_MATRIX_SIZE);
		}
		memset (CellsPage, PMC_NULL, PAGE_MATRIX_SIZE);
	}
	//**************************************

	for(i=0;i<MaxHeightOfLetter;i++)
	{
		pBlockHystogram[i] = 0; //uint16_t[]
	}


	comp = CCOM_GetFirst(hCCOM,NULL);
    while (comp)
    {
		i++;
		//if(bSearchPicture && !LDPUMA_Skip(hDebugCancelGreatComp) && comp->scale > 1)
		//if((comp->scale > 1)&&(comp->type!=CCOM_CH_LETTER))
		if((comp->h > MinHeightOfLetter) && (comp->h < MaxHeightOfLetter))
		{
			nComps++;
			pBlockHystogram[comp->h]++; // т.е., строим гистограмму высот
		}                             // компонент hCCOM


		if((comp->scale > 1)&&IsNotBigLetter(comp))
		{
			if (BLOCKS)  //Если мы находимся на этапе выделения фрагментов
			{
				//if(!SearchPicturesSecond(hCCOM,  BLOCKS, comp)) goto lNextComp;
				if(SearchPicturesSecond(hCCOM,  BLOCKS, comp))
				{
					if(!IsNotGoodComp(pInfo, comp))
					{
						if (nPics % PICS_QUANTUM == 0)
						{
							pPics = (CCOM_comp *) realloc (pPics,
								(size_t) ((nPics / PICS_QUANTUM + 1)* PICS_QUANTUM * sizeof (CCOM_comp)));
						}

						pPics[nPics++] = *comp; // составляем перечень картинок
					}
				}
			}
			goto lNextComp;
		}

        AddRoot(comp, TRUE)
			;
	lNextComp:
			comp = CCOM_GetNext(comp,NULL);
    }


	//*********** Rom 08-02-99 *************
	if (BLOCKS)
	{
		sprintf(tmp_str, "  <4 П После снятия линий %d компонент \n", nComps);
		LDPUMA_FPuts(resFile_comp,tmp_str);
		sprintf(tmp_str, "  <4 Р %d \n", i);
		LDPUMA_FPuts(resFile_comp, tmp_str);
		LDPUMA_FPuts(resFile_comp, "  <3 К Компоненты \n");
		LDPUMA_FPuts(resFile_comp, "  <2 К Страница \n");

		PicturesSecondStage(hCCOM, pPage); // объединить все картинки (теоретически)

		if (LDPUMA_Skip(hDebugCancelPicturesAndTables))
		{
			DeleteRootsFromTables();
			DeleteRootsFromPictures();
		}
	}

	nBigLetters = 0;
	if(pBigLetters!=NULL)
	{
		free(pBigLetters);
		pBigLetters = NULL;
	}

	if (BLOCKS)
	{
		if(pPics!=NULL)
		{
			free(pPics);
			pPics = NULL;
		}
		nPics = 0;
	}
	//**************************************


    return (TRUE);

}

Bool AddRoot(CCOM_comp * comp, Bool32 FirstTime)
{
ROOT   RootRecord = {0};

        _ASSERT(comp);


		//***************Rom********************

		if ( (comp->h/comp->w > 20) || (comp->w/comp->h > 20))
			return FALSE;// Piter

		//**************************************

		//***************Rom********************

		//if ( ((comp->h <= 2) && (comp->w <= 3)) || ((comp->h <= 3) && (comp->w <= 2)) )
		//if ( (comp->h <= 3) && (comp->w <= 3) )
		if ( comp->h*comp->w < 6 )
			return FALSE;// Piter

		//**************************************

        RootRecord.bReached   = FALSE;
		RootRecord.pComp = comp;

		RootRecord.yRow = comp->upper;
        RootRecord.xColumn = comp->left;
		RootRecord.u1.pNext = NULL;
        RootRecord.nHeight = comp->h;
        RootRecord.nWidth = comp->w;
        RootRecord.nBlock = 0;
		RootRecord.nUserNum = 0;

		RootRecord.bType = ROOT_DUST|ROOT_RECOGNIZED ;
		if(comp->type & CCOM_CH_LETTER)
			RootRecord.bType = ROOT_DUST | ROOT_LETTER | ROOT_RECOGNIZED;
		if(comp->type & (CCOM_CH_DUST | CCOM_CH_PUNCT ))
			RootRecord.bType = ROOT_DUST;
		if(comp->type & CCOM_CH_DUST && (comp->w+7)/8*comp->h<2 )
			RootRecord.bType = 0 ;
		if(comp->type & CCOM_CH_GREAT )
			RootRecord.bType = 0;

        if (nRoots % ROOTS_QUANTUM == 0 && FirstTime)
        {
            pRoots = realloc (pRoots,
                              (size_t) ((nRoots / ROOTS_QUANTUM + 10)
                                * ROOTS_QUANTUM * sizeof (ROOT)));

        }

        nRoots++;

        if (pRoots == NULL)
        {
            ErrorNoEnoughMemory ("in LTROOTS.C, AddRoot");
            nRoots = 0;
            return FALSE;
        }

        pRoots [nRoots - 1] = RootRecord;


		if(nRoots>1&&FirstTime)
			pRoots [nRoots - 2].u1.pNext = pRoots + nRoots - 1;

return TRUE;
}



//STEPA_AM's code:
Bool32 NPFArbitr(CCOM_comp* in,CCOM_comp* big,Bool32 flag)
{


 switch(flag)
 {
  case 0:
	  if((in->nl)&NEGA)
	  {
		if((in->nl)&VERTICA)
		  return TRUE;
		else
		{
		  if(in->h<(big->h)/2)
			  return FALSE;
		  else
			  return TRUE;
		}
	  }
	  else
		  return FALSE;
	  break;
  case 1:
	  if( ((in->nl)&NEGA)||((big->nl)&NEGA) )
	  {
		  return TRUE;
	  }
	  else
		  return FALSE;
	  break;
 }
 return FALSE;
}

int NPFGetBD()
{
	return 10;
}
