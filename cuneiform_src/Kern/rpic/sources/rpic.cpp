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

// ============================================================================
//
// ============================================================================

#define __RPIC_CPP__

/*#include <windows.h>*/
#include <stdio.h>
#include <stdlib.h>

#include "mainpic.h"
#undef __RPIC_CPP__

/************************* External Handles for DPUMA *************************/
extern Handle hRectanglePictures;
extern Handle hPolyPictures;
extern Handle hShowFirstAttempt;
extern Handle hShowAllGreat;
extern Handle hShowAllCancelledGreat;
extern Handle hShowBigLetters;
extern Handle hShowPossiblePics;
/******************************************************************************/

#define POSSIBLE_PICTURES CPAGE_GetInternalType("possible pictures")

/******************************************************************************/
/*                           Additional functions                             */
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
Bool32 OpenLogRes(void)
{
	logFile_comp   = LDPUMA_FOpen(LOGFILENAME_COMP  ,"at");
	resFile_comp   = LDPUMA_FOpen(RESFILENAME_COMP  ,"at");
	logFile_pict   = LDPUMA_FOpen(LOGFILENAME_PICT  ,"at");
	resFile_pict   = LDPUMA_FOpen(RESFILENAME_PICT  ,"at");
	logFile_blocks = LDPUMA_FOpen(LOGFILENAME_BLOCKS,"at");
	resFile_blocks = LDPUMA_FOpen(RESFILENAME_BLOCKS,"at");

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
Bool32 CloseLogRes(void)
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
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
Bool32 ClearLogRes(void)
{

	logFile_comp   = LDPUMA_FOpen(LOGFILENAME_COMP  ,"wt");
	resFile_comp   = LDPUMA_FOpen(RESFILENAME_COMP  ,"wt");
	logFile_pict   = LDPUMA_FOpen(LOGFILENAME_PICT  ,"wt");
	resFile_pict   = LDPUMA_FOpen(RESFILENAME_PICT  ,"wt");
	logFile_blocks = LDPUMA_FOpen(LOGFILENAME_BLOCKS,"wt");
	resFile_blocks = LDPUMA_FOpen(RESFILENAME_BLOCKS,"wt");

	CloseLogRes();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
Bool32 LastCheck(Handle hCCOM, Handle hCCOM_big,Handle hCPAGE)
{
	uint32_t i,j;
	CCOM_comp pic;
	CCOM_comp * comp;
	uint16_t pLocalHystogram[MaxHeightOfLetter];
	uint32_t nLocalComps;
	uint32_t MaxLocal, nMaxLocal;
	uint32_t Result1, Result2;

	for(i=0; i<nPics; i++)
	{
		if(pPics[i].large & CCOM_LR_KILLED)
		{
			continue;
		}
		pic = pPics[i];

		//Rom
		if(pPics[i].h<32||pPics[i].w<32)
		{
			DeleteFromPics(i);
			//i--;
			continue;
		}

		/* Filling "my_*" for function MyFiltrateIn */
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

		/* Filling the hystogram of local letter's heights */

//stepa_am
			comp = CCOM_GetFirst(hCCOM_big, &MyFiltrateIn);
		while(comp)
		{
			if( (comp->h < MaxHeightOfLetter) && (comp->h > MinHeightOfLetter) )
			{
				pLocalHystogram[comp->h]++;
				nLocalComps++;
			}
			comp = CCOM_GetNext(comp, &MyFiltrateIn);
		}
//end stepa_am

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

		/* if there is comonents in the picture, check picture for containing text blocks,
		   if result is sucsessful, delete it from pictures */
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

//stepa_am
			comp = CCOM_GetFirst(hCCOM_big, &MyFiltrateIn);
			while(comp)
			{
				if( (comp->h < MaxHeightOfLetter-1) && (comp->h > MinHeightOfLetter) )
				{
					Result1 += pHystogram[comp->h]/20+
						pHystogram[comp->h-1]/20+pHystogram[comp->h+1]/20;
					if(comp->h - nMaxLocal < DIFFERENCE1)
					{
						Result2 += pLocalHystogram[comp->h]+pLocalHystogram[comp->h-1]+pLocalHystogram[comp->h+1];
					}
				}
				comp = CCOM_GetNext(comp, &MyFiltrateIn);
			}
//end stepa_am

			comp = CCOM_GetFirst(hCCOM, &MyFiltrateIn);
			while(comp)
			{
				if( (comp->h < MaxHeightOfLetter-1) && (comp->h > MinHeightOfLetter) )
				{
					Result1 += pHystogram[comp->h]/20+
						pHystogram[comp->h-1]/20+pHystogram[comp->h+1]/20;
					if(comp->h - nMaxLocal < DIFFERENCE1)
					{
						Result2 += pLocalHystogram[comp->h]+pLocalHystogram[comp->h-1]+pLocalHystogram[comp->h+1];
					}
				}
				comp = CCOM_GetNext(comp, &MyFiltrateIn);
			}

			Result1 = Result1/10;
			Result2 = Result2/20;


			if(Result1 + Result2 > 2*CONDITION1)
			{
				DeleteFromPics(i);
				//i--;
				continue;
			}
		}
	}


	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
Bool32 DeleteFromPics(uint32_t position)
{
	/*
	uint32_t i;

	for(i=position;i<nPics-1;i++)
	{
		pPics[i] = pPics[i+1];
	}
	nPics--;
	*/

	pPics[position].large |= CCOM_LR_KILLED;

	return TRUE;
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
}
////////////////////////////////////////////////////////////////////////////////

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
		//if (comp->h*comp->w > pInfo.Height*pInfo.Width*0.6) return TRUE;
		if (comp->h*comp->w > pInfo.Height*pInfo.Width*0.8) return TRUE;
	}
	else
	{
		if ( (comp->h/comp->w > 10) || (comp->w/comp->h > 10)) return TRUE;
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////


/******************************************************************************/
/*                              Main function                                 */
/******************************************************************************/


////////////////////////////////////////////////////////////////////////////////
RPIC_FUNC(Bool32) RPIC_SearchPictures (Handle hCCOM, Handle hCCOM_big,Handle hCPAGE)
{
	CCOM_comp * comp =	NULL;
	CCOM_comp common;
	PAGEINFO			pInfo;
	POLY_ block;
	uint32_t i,j;

	Rect16 rect;
	uint32_t key;
	uint32_t color;
	uint32_t tmp_comp=0;

	Handle MainWindowD=NULL;
    MainWindowD=LDPUMA_GetWindowHandle ("Изображение после разворота");
    if(!MainWindowD)
       MainWindowD=LDPUMA_GetWindowHandle ("Main");


	OpenLogRes();
	file_name = LDPUMA_GetFileName(NULL);
	//color = 23650;
	color = 200;
	key = 111;
	nPics = 0;
	if(pPics)
	{
		free(pPics);
		pPics = NULL;
	}
	nComps = 0;

	CPAGE_GetPageData( hCPAGE, PT_PAGEINFO, (void*)&pInfo, sizeof(pInfo));

	for(i=0;i<MaxHeightOfLetter;i++)
	{
		pHystogram[i] = 0;
	}


	//LDPUMA_FPuts(hFile, (char*)&pInfo.szImageName);
	LDPUMA_FPuts(logFile_comp, "*******************************************************\n");
	LDPUMA_FPuts(logFile_comp, "new_file \n");

	if((pInfo.Angle == 90)||(pInfo.Angle == 270))
	{
		uint32_t tmp;
		tmp = pInfo.Width;
		pInfo.Width = pInfo.Height;
		pInfo.Height = tmp;
	}

//stepa_am
	comp = CCOM_GetFirst(hCCOM_big,NULL);
	while (comp)
	{
		tmp_comp++;

		if((comp->h > MinHeightOfLetter) && (comp->h < MaxHeightOfLetter))
		{
			nComps++;
			pHystogram[comp->h]++;
		}
		if( comp->scale < 1 )
			goto lNextComp_big;
			/*
			if( comp->cs == 255)
			{
			comp->type = CCOM_CH_LETTER;
			goto lNextComp;
			}
		*/
		if( IsNotGoodComp(pInfo, comp) )
		{
			if(!LDPUMA_Skip(hShowAllCancelledGreat))
			{
				rect.left = comp->left;
				rect.top = comp->upper;
				rect.right = comp->left + comp->w;
				rect.bottom = comp->upper + comp->h;
				LDPUMA_DrawRect(MainWindowD, &rect, 0, color, 4, key);
			}
			goto lNextComp_big;
		}

		if (nPics % PICS_QUANTUM == 0)
		{
			pPics = (CCOM_comp *) realloc (pPics,
				(size_t) ((nPics / PICS_QUANTUM + 1)* PICS_QUANTUM * sizeof (CCOM_comp)));
		}

		pPics[nPics++] = *comp;

lNextComp_big:
		comp = CCOM_GetNext(comp,NULL);

	}
//end stepa_am

	comp = CCOM_GetFirst(hCCOM,NULL);
	while (comp)
	{
		tmp_comp++;

		if((comp->h > MinHeightOfLetter) && (comp->h < MaxHeightOfLetter))
		{
			nComps++;
			pHystogram[comp->h]++;
		}
		if( /*comp->scale < */1 )
			goto lNextComp;
			/*
			if( comp->cs == 255)
			{
			comp->type = CCOM_CH_LETTER;
			goto lNextComp;
			}
		*/
		if( IsNotGoodComp(pInfo, comp) )
		{
			if(!LDPUMA_Skip(hShowAllCancelledGreat))
			{
				rect.left = comp->left;
				rect.top = comp->upper;
				rect.right = comp->left + comp->w;
				rect.bottom = comp->upper + comp->h;
				LDPUMA_DrawRect(MainWindowD, &rect, 0, color, 4, key);
			}
			goto lNextComp;
		}

		if (nPics % PICS_QUANTUM == 0)
		{
			pPics = (CCOM_comp *) realloc (pPics,
				(size_t) ((nPics / PICS_QUANTUM + 1)* PICS_QUANTUM * sizeof (CCOM_comp)));
		}

		pPics[nPics++] = *comp;

lNextComp:
		comp = CCOM_GetNext(comp,NULL);

	}

	if(!LDPUMA_Skip(hShowAllCancelledGreat))
	{
		LDPUMA_Console("RPIC_Отмененные большие компоненты \n");
		LDPUMA_WaitUserInput(hShowAllCancelledGreat, MainWindowD);
		LDPUMA_DeleteRects(MainWindowD, key);
	}

	if( !nComps ) // OLEG
    return TRUE; // STEPA_AM
	if( !nPics)
	  return TRUE;

	/**************************************************/
	for(i=0;i<MaxHeightOfLetter;i++)
	{
		nAverageHeight += pHystogram[i]*i;
		/*
		sprintf(tmp_str, " %i (i)\n", i);
		LDPUMA_FPuts(logFile,tmp_str);
		sprintf(tmp_str, " %i (pHystogram[i])\n", pHystogram[i]);
		LDPUMA_FPuts(logFile,tmp_str);
		*/
	}
	nAverageHeight /= nComps;
	sprintf(tmp_str, "Average height %i (nAverageHeight)\n", nAverageHeight);
	LDPUMA_FPuts(logFile_comp,tmp_str);
	/**************************************************/

	/************** first union all crossed and included pictures ******************/
	/*
	for (i=0; i<nPics; i++)
	{
		for (j=0; j<nPics; j++)
		{
			if ( (pPics[i].upper  < pPics[j].upper)&&
				 (pPics[i].left   < pPics[j].left)&&
				 (pPics[i].left   + pPics[i].w > pPics[j].left + pPics[j].w)&&
				 (pPics[i].upper  + pPics[i].h > pPics[j].upper + pPics[j].h) )
			{
				DeleteFromPics (j);
				j--;
			}
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
	*/
	/***********************************************************************************/

	color = 200;
	key = 111;
	if((!LDPUMA_Skip(hShowAllGreat))&&nPics)
	{
		for (i=0; i<nPics; i++)
		{
			rect.left = pPics[i].left;
			rect.top = pPics[i].upper;
			rect.right = pPics[i].left + pPics[i].w;
			rect.bottom = pPics[i].upper + pPics[i].h;
			LDPUMA_DrawRect(MainWindowD, &rect, 0, color, 2, key);
		}
		LDPUMA_Console("RPIC_Рассматриваемые большие компоненты \n");
		LDPUMA_WaitUserInput(hShowAllGreat,MainWindowD);
		LDPUMA_DeleteRects(MainWindowD, key);
	}

	//sprintf(tmp_str, "Amount of 'letters' on the first step %i (nComps)\n", nComps);
	//LDPUMA_FPuts(logFile_comp,tmp_str);
	//sprintf(tmp_str, "  <2 Н Страница = %d \n", page_count++);
	//LDPUMA_FPuts(resFile_comp,tmp_str);

	//char* str;
	//str = LDPUMA_GetFileName(NULL);

	LDPUMA_FPuts(resFile_comp, "  <2 Н Страница =");
	LDPUMA_FPuts(resFile_comp, file_name);
	LDPUMA_FPuts(resFile_comp, " \n");
	//LDPUMA_FPuts(resFile_comp, "  <2 Н Страница \n");
	LDPUMA_FPuts(resFile_comp, "  <3 Н Компоненты \n");
	sprintf(tmp_str, "  <4 П Перед выделением линий \n", nComps);
	LDPUMA_FPuts(resFile_comp,tmp_str);
	sprintf(tmp_str, "  <4 Р %d \n", nComps);
	LDPUMA_FPuts(resFile_comp, tmp_str);
	sprintf(tmp_str, "Amount of pictures on the before first step %i (nPics)\n", nPics);
	LDPUMA_FPuts(logFile_comp,tmp_str);

	/*******************************************************************/
	if(LDPUMA_Skip(hRectanglePictures))
		SearchRectanglePictures(hCCOM,hCCOM_big, hCPAGE);
	if(LDPUMA_Skip(hPolyPictures))
		SearchPolyPictures(hCCOM, hCPAGE);
	/*******************************************************************/

	/************** final union all crossed and included pictures ******************/

	for (i=0; i<nPics; i++)
	{
		if(pPics[i].large & CCOM_LR_KILLED)
		{
				continue;
		}
		for (j=0; j<nPics; j++)
		{
			if ( (pPics[i].upper  < pPics[j].upper)&&
				 (pPics[i].left   < pPics[j].left)&&
				 (pPics[i].left   + pPics[i].w > pPics[j].left + pPics[j].w)&&
				 (pPics[i].upper  + pPics[i].h > pPics[j].upper + pPics[j].h) )
			{
				/*
				if((pPics[i].type!=CCOM_CH_LETTER)||(pPics[j].type!=CCOM_CH_LETTER))
				{
					pPics[i].type = CCOM_CH_MERGE;
				}
				*/
				if(pPics[j].large & CCOM_LR_KILLED)
				{
					// it's bad, but will work cause pPics[].large not initialised in RPIC
					pPics[i].large |= CCOM_LR_TAKEN;
					continue;
				}
				DeleteFromPics (j);
				//j--;
			}
		}
	}




	for (i=0; i<nPics; i++)
	{
		if(pPics[i].large & CCOM_LR_KILLED)
		{
			continue;
		}
		for (j=0; j<nPics; j++)
		{
			if(pPics[j].large & CCOM_LR_KILLED)
			{
				continue;
			}
			if (i==j) continue;
			if ( (pPics[i].upper - VER_BOUND_SIZE < pPics[j].upper + pPics[j].h)&&
				 (pPics[i].left  - HOR_BOUND_SIZE < pPics[j].left  + pPics[j].w)&&
				 (pPics[i].left  + pPics[i].w + HOR_BOUND_SIZE > pPics[j].left)&&
				 (pPics[i].upper + pPics[i].h + VER_BOUND_SIZE > pPics[j].upper) )
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
				/*
				if((pPics[i].type!=CCOM_CH_LETTER)||(pPics[j].type!=CCOM_CH_LETTER))
				{
					pPics[i].type = CCOM_CH_MERGE;
				}
				*/
				DeleteFromPics (j);
				//j=0;
				//i=0;
			}
		}
	}

	/***********************************************************************************/

	/**/
	/* rom
	for (i=0; i<nPics; i++)
	{
		if(pPics[i].type == CCOM_CH_LETTER)
		{
			DeleteFromPics (i);
			i--;
		}
	}
	*/

	LastCheck( hCCOM,hCCOM_big, hCPAGE);

	LDPUMA_FPuts(resFile_pict, "  <2 Н Страница =");
	LDPUMA_FPuts(resFile_pict, file_name);
	LDPUMA_FPuts(resFile_pict, " \n");
	LDPUMA_FPuts(resFile_pict, "  <3 Н Картинки \n");
	LDPUMA_FPuts(resFile_pict, "  <4 Н После первого прохода \n");
	sprintf(tmp_str, "  <4 Р %d %d %d \n", nPics,0 ,0);
	LDPUMA_FPuts(resFile_pict, tmp_str);

	int32_t min_image_width = (int32_t)(100*((double)(pInfo.DPIX + 1)/300));
	int32_t min_image_height = (int32_t)(100*((double)(pInfo.DPIY + 1)/300));

	for(i=0;i<nPics;i++)
	{
		if(pPics[i].large & CCOM_LR_KILLED && !(pPics[i].large & CCOM_LR_TAKEN))
		{
			continue;
		}
		block.com.type = TYPE_TEXT;//Текст, Картинка, Таблица;
		block.com.number = 0;//порядковый номер
		block.com.Color = 0;
		block.com.count = 4;
		block.com.Flags = 0;
		block.com.Vertex[0].x = pPics[i].left;
		block.com.Vertex[0].y = pPics[i].upper;
		block.com.Vertex[1].x = pPics[i].left + pPics[i].w;
		block.com.Vertex[1].y = pPics[i].upper;
		block.com.Vertex[2].x = pPics[i].left + pPics[i].w;
		block.com.Vertex[2].y = pPics[i].upper + pPics[i].h;
		block.com.Vertex[3].x = pPics[i].left;
		block.com.Vertex[3].y = pPics[i].upper + pPics[i].h;
		block.alphabet = 0;

		sprintf(tmp_str, "  <4 О 1 %4d %4d %4d %4d %d \n",
			pPics[i].left, pPics[i].upper, pPics[i].left + pPics[i].w, pPics[i].upper, pPics[i].h);
		LDPUMA_FPuts(resFile_pict,tmp_str);

		if(pPics[i].large & CCOM_LR_TAKEN || pPics[i].w < min_image_width && pPics[i].h < min_image_height)
		{
			CPAGE_CreateBlock(hCPAGE, POSSIBLE_PICTURES,0,0,&block,sizeof(POLY_));
		}
		else
		{
			CPAGE_CreateBlock(hCPAGE, TYPE_IMAGE,0,0,&block,sizeof(POLY_));
		}
	}


	LDPUMA_FPuts(resFile_pict, "  <4 К После первого прохода \n");
	sprintf(tmp_str, "Amount of comps on the first step %i (tmp_comp) \n", tmp_comp);
	LDPUMA_FPuts(logFile_comp,tmp_str);
	sprintf(tmp_str, "Amount of pictures on the first step %i (nPics) \n", nPics);
	LDPUMA_FPuts(logFile_comp,tmp_str);
	LDPUMA_FPuts(logFile_comp, "*******************************************************\n");


	if(pPics != NULL)
	{
		free(pPics);
		pPics = NULL;
	}
	nPics = 0;

	CloseLogRes();


    Handle h = NULL;

	if(!LDPUMA_Skip(hShowFirstAttempt))
	{
	 h=NULL;
	 nPics = 0;
	 for(h = CPAGE_GetBlockFirst(hCPAGE,TYPE_IMAGE);
	 h!=NULL;
	 h = CPAGE_GetBlockNext(hCPAGE,h,TYPE_IMAGE))
	 {
	  nPics++;
	  CPAGE_GetBlockData(hCPAGE,h,TYPE_IMAGE, &block, sizeof(block));
       rect.left =(int16_t)(block.com.Vertex[0].x);
	   rect.top =(int16_t)(block.com.Vertex[0].y);
	   rect.right =(int16_t)(block.com.Vertex[1].x);
	   rect.bottom =(int16_t)(block.com.Vertex[2].y);
	  LDPUMA_DrawRect(MainWindowD, &rect, 0, color, 2, key);
	 }
	 if(nPics)
	 {
		LDPUMA_Console("RPIC_Картинки после первого прохода \n");
		LDPUMA_WaitUserInput(hShowFirstAttempt, MainWindowD);
		LDPUMA_DeleteRects(MainWindowD, key);
	 }
	}

	if(!LDPUMA_Skip(hShowPossiblePics))
	{
	 h=NULL;
	 nPics = 0;
	 for(h = CPAGE_GetBlockFirst(hCPAGE,POSSIBLE_PICTURES);
	 h!=NULL;
	 h = CPAGE_GetBlockNext(hCPAGE,h,POSSIBLE_PICTURES))
	 {
	  nPics++;
	  CPAGE_GetBlockData(hCPAGE,h,POSSIBLE_PICTURES, &block, sizeof(block));
       rect.left =(int16_t)(block.com.Vertex[0].x);
	   rect.top =(int16_t)(block.com.Vertex[0].y);
	   rect.right =(int16_t)(block.com.Vertex[1].x);
	   rect.bottom =(int16_t)(block.com.Vertex[2].y);
	  LDPUMA_DrawRect(MainWindowD, &rect, 0, color, 2, key);
	 }
	 if(nPics)
	 {
		LDPUMA_Console("RPIC_Неуверенные картинки после первого прохода \n");
		LDPUMA_WaitUserInput(hShowPossiblePics, MainWindowD);
		LDPUMA_DeleteRects(MainWindowD, key);
	 }
	}

    Handle BlockType = CPAGE_GetInternalType("pic's to letters boxes");
    RPIC_Comp_Rect CompRect;
    if(!LDPUMA_Skip(hShowBigLetters))
	{
	 h=NULL;
	 nPics = 0;
	 for(h = CPAGE_GetBlockFirst(hCPAGE,BlockType);
	 h!=NULL;
	 h = CPAGE_GetBlockNext(hCPAGE,h,BlockType))
	 {
	  nPics++;
	  CPAGE_GetBlockData(hCPAGE,h,BlockType, &CompRect, sizeof(CompRect));
       rect.left = CompRect.left;
	   rect.top = CompRect.upper;
	   rect.right = CompRect.left+CompRect.w-1;
	   rect.bottom = CompRect.upper+CompRect.h-1;
	  LDPUMA_DrawRect(MainWindowD, &rect, 0, color, 2, key);
	 }
	 if(nPics)
	 {
		LDPUMA_Console("RPIC_Буквицы \n");
		LDPUMA_WaitUserInput(hShowBigLetters, MainWindowD);
		LDPUMA_DeleteRects(MainWindowD, key);
	 }
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//end of file
