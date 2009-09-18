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


# include "new_c.h"

# undef __LOAD_ROOTS_C__

extern Handle hDebugCancelGreatComp;
extern Handle hDebugCancelPicturesAndTables;
extern Handle CutStrPrintFILE;
extern Handle DifferenCutToHistory;
extern Rect16 oldRoot;
extern Handle hDebugCutStr;
extern Handle CutCompPrintFILE;
extern Handle hCutComp;
extern Handle DifferenCutCompToHistory;


FILE* f_cut_str=NULL;
FILE* f_old_cut=NULL;
FILE* f_temp_cut=NULL;
FILE* f_cut_comp=NULL;
FILE* f_old_cut_comp=NULL;
FILE* f_temp_cut_comp=NULL;

#define POSSIBLE_PICTURES CPAGE_GetInternalType("possible pictures")


Bool32 Open_Res_Log( void )
{

	logFile_comp   = LDPUMA_FOpen(LOGFILENAME_COMP  ,"at");
	resFile_comp   = LDPUMA_FOpen(RESFILENAME_COMP  ,"at");
	logFile_pict   = LDPUMA_FOpen(LOGFILENAME_PICT  ,"at");
	resFile_pict   = LDPUMA_FOpen(RESFILENAME_PICT  ,"at");
	logFile_blocks = LDPUMA_FOpen(LOGFILENAME_BLOCKS,"at");
	resFile_blocks = LDPUMA_FOpen(RESFILENAME_BLOCKS,"at");


 char* name;
 if(!LDPUMA_Skip(CutStrPrintFILE)&&!LDPUMA_Skip(hDebugCutStr))
 {
  f_cut_str=fopen("cutstr.res","a");
  if(f_cut_str==NULL)
 	 f_cut_str=fopen("cutstr.res","w+");
  if(f_cut_str)
  {
   name=LDPUMA_GetFileName (NULL);
   fprintf(f_cut_str,"%s\n",name);
  }
 oldRoot.top=-1;
 oldRoot.bottom=-1;
 oldRoot.left=-1;
 oldRoot.right=-1;

 }

 if(!LDPUMA_Skip(DifferenCutToHistory))
 {
  f_old_cut=fopen("oldcutst.res","r");
  f_temp_cut=fopen("tempcut.res","w");
  name=LDPUMA_GetFileName (NULL);
  if(f_temp_cut)
       fprintf(f_temp_cut,"%s\n",name);

 }

 if(!LDPUMA_Skip(CutCompPrintFILE)&&LDPUMA_Skip(hCutComp))
 {
  f_cut_comp=fopen("cutcomp.res","a");
  if(f_cut_comp==NULL)
 	 f_cut_comp=fopen("cutcomp.res","w+");
  if(f_cut_comp)
  {
   name=LDPUMA_GetFileName (NULL);
   fprintf(f_cut_comp,"%s\n",name);
  }
 }

 if(!LDPUMA_Skip(DifferenCutCompToHistory))
 {
  f_old_cut_comp=fopen("oldcutco.res","r");
  f_temp_cut_comp=fopen("tempcutc.res","w");
  name=LDPUMA_GetFileName (NULL);
  if(f_temp_cut_comp)
       fprintf(f_temp_cut_comp,"%s\n",name);

 }



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

	if(f_cut_str)
		fclose(f_cut_str);

	if(!LDPUMA_Skip(DifferenCutToHistory))
	{
     if(f_temp_cut)
         fclose(f_temp_cut);
	 if(f_old_cut)
	     fclose(f_old_cut);
	}

	if(f_cut_comp)
		fclose(f_cut_comp);

	if(!LDPUMA_Skip(DifferenCutToHistory))
	{
	 if(f_temp_cut_comp)
         fclose(f_temp_cut_comp);
	 if(f_old_cut_comp)
	     fclose(f_old_cut_comp);
	}



	return TRUE;
}


Bool32 FillBigLetters (Handle hCCOM,Handle hCPAGE)
{
	Handle BlockType;
	Handle pBlock;
	RPIC_Comp_Rect CompRect;

	BlockType = CPAGE_GetInternalType("pic's to letters boxes");

	pBlock = CPAGE_GetBlockFirst(hCPAGE, BlockType);
	while(pBlock)
	{
		if (nBigLetters % BIG_LETTERS_QUANTUM == 0)
		{
			pBigLetters = (RPIC_Comp_Rect*)realloc (pBigLetters,
				(size_t) ((nBigLetters / BIG_LETTERS_QUANTUM + 1)
				* BIG_LETTERS_QUANTUM * sizeof (RPIC_Comp_Rect)));
		}

		CPAGE_GetBlockData(hCPAGE, pBlock, BlockType, &CompRect, sizeof(CompRect));
		nBigLetters++;
		pBigLetters[nBigLetters-1] = CompRect;

		pBlock = CPAGE_GetBlockNext(hCPAGE, pBlock, BlockType);
	}

	return TRUE;
}


Bool32 FillPicsInTables(Handle hCCOM, Handle hCPAGE)
{
	POLY_	block;
	Handle h = NULL;

	CCOM_comp * comp;

	for(h = CPAGE_GetBlockFirst(hCPAGE,POSSIBLE_PICTURES);
	h!=NULL;
	h = CPAGE_GetBlockNext(hCPAGE,h,POSSIBLE_PICTURES))
	{
		CPAGE_GetBlockData(hCPAGE,h,POSSIBLE_PICTURES, &block, sizeof(block));
		CPAGE_DeleteBlock(hCPAGE, h);

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

Bool32 ReadRoots(Handle hCCOM)
{
	CCOM_comp * comp = NULL;
	Handle		pPage;
	PAGEINFO	pInfo;
	uint32_t i;
	int max_h=50;

	exthCCOM = hCCOM;
    RootsFreeData ();
	nRoots = 0;


	pPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage( ));
	CPAGE_GetPageData( pPage, PT_PAGEINFO, (void*)&pInfo, sizeof(pInfo));

	max_h=(max_h*(pInfo.DPIY+1))/300;
	nBigLetters = 0;
	pBigLetters = NULL;
	FillBigLetters(hCCOM, pPage);
	FillPicsInTables(hCCOM, pPage);




	//**************************************

	for(i=0;i<MaxHeightOfLetter;i++)
	{
		pBlockHystogram[i] = 0;
	}


	comp = CCOM_GetFirst(hCCOM,NULL);
	int h;
    while (comp)
    {
		h=comp->h;
		if((h > MinHeightOfLetter) && (h < MaxHeightOfLetter))
		{
			nComps++;
			pBlockHystogram[h]++;
		}


		if(comp->scale>1)
		{
			if(comp->scale>2||h>max_h)
			{
			 if(IsNotBigLetter(comp))
				 goto lNextComp;
			}
		}

        AddRoot(comp, TRUE)
			;
	lNextComp:
			comp = CCOM_GetNext(comp,NULL);
    }


	//*********** Rom 08-02-99 *************

	nBigLetters = 0;
	if(pBigLetters!=NULL)
	{
		free(pBigLetters);
		pBigLetters = NULL;
	}


	//**************************************


    return (TRUE);

}

Bool AddRoot(CCOM_comp * comp, Bool32 FirstTime)
{
ROOT   RootRecord = {0};
int h=comp->h;
int w=comp->w;

        _ASSERT(comp);


		//***************Rom********************

		if ( (h/w > 20) || (w/h > 20))
			return FALSE;// Piter

		//**************************************

		//***************Rom********************
		if ( h*w < 6 )
			return FALSE;// Piter

		//**************************************

        RootRecord.bReached   = FALSE;
		RootRecord.pComp = comp;

		RootRecord.yRow = comp->upper;
        RootRecord.xColumn = comp->left;
		RootRecord.u1.pNext = NULL;
        RootRecord.nHeight = h;
        RootRecord.nWidth = w;
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
            pRoots =(ROOT*) realloc (pRoots,
                              (size_t) ((nRoots / ROOTS_QUANTUM + 10)
                                * ROOTS_QUANTUM * sizeof (ROOT)));

        }

        nRoots++;

        if (pRoots == NULL)
        {
            ErrorNoEnoughMemory ((uchar*)"in LTROOTS.C, AddRoot");
            nRoots = 0;
            return FALSE;
        }

        pRoots [nRoots - 1] = RootRecord;


		if(nRoots>1&&FirstTime)
			pRoots [nRoots - 2].u1.pNext = pRoots + nRoots - 1;

return TRUE;
}
