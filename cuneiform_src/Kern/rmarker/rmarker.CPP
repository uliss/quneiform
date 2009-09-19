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

#define _RMARKER_CPP

/*#include <windows.h>*/
#include <fstream>
#include "markpage.h"
#include "rmfunc.h"
#include "un_buff.h"
#include "line_vp_util.h"
#include "line_vp_2_am.h"
#include "markdataoper.h"
#include "amt_geom.h"
#include "un_err.h"
#include "puma_err.h"
#include "rneg.h"
#include "rselstr.h"
#include "rline.h"

#include "compat_defs.h"

# define INCLINE_FACTOR  2048

# define IDEAL_XY(x, y)   \
         {\
             y = (int16_t) (y - (int32_t) x * nIncline / INCLINE_FACTOR);\
             x = (int16_t) (x + (int32_t) y * nIncline / INCLINE_FACTOR);\
         }

# define REAL_XY(x, y)   \
         {\
             x = (int16_t) (x - (int32_t) y * nIncline / INCLINE_FACTOR);\
             y = (int16_t) (y + (int32_t) x * nIncline / INCLINE_FACTOR);\
		}

#define TYPE_FON      CPAGE_GetInternalType("TYPE_FON")


extern Handle DebugFill;
extern Handle hVertCells;
extern Handle hNegaCells;
extern Handle hVertCellsAuto;
extern Handle hNegaCellsAuto;
extern Handle hDebugUpDown;
extern Handle hDebugPictures;
extern Handle hPrintFileVertCells;
extern Handle hDebugNeg;
extern Handle hDebugLinePass3;
extern Handle hDebugLinePass2;
extern Handle hDebugVerifLine;
extern Handle hNoGiveNeg;
extern Handle hNoSeePict;

extern Handle hPrep;
extern Handle hPicture;
extern Handle hNegative;
extern Handle hPrintCrossedPics;
extern Handle hVCutInZones;
extern Handle hSVLP;
extern Handle hBlocks;
extern Handle hLines3;
extern Handle hNegaTestCells;
extern Handle hVertTestCells;
extern Handle hFon;
extern Handle hEnd;

static HINSTANCE ghInst =  NULL;
static uint16_t gwHeightRC = 0;
static uint32_t gwRC = 0;
Bool dpDebugUpDown;

static Bool32 rblockProgressStep(uint32_t perc)
{
	return ProgressStepAutoLayout(2, perc);
}
static void   rblockProgressFinish( void )
{
	ProgressStepAutoLayout(2, 100);
}

Bool32 PageMarkup(PRMPreProcessImage Image)
{

	LDPUMA_Skip(hPrep);
//	int32_t  Nominator = 0;   // числитель
//	int32_t  Denominator = 0;	// знаменатель определяемого угла поворота
	PAGEINFO           PInfo = {0};
	Bool32 rc=TRUE;

	gSVLBuffer.VLinefBufferA = NULL;
	gSVLBuffer.VLinefBufferB = NULL;
	gSVLBuffer.LineInfoA = (LinesTotalInfo*) CFIO_DAllocMemory( sizeof(LinesTotalInfo), MAF_GALL_GPTR, (pchar)"puma", (pchar)"SVL step I lines info pool");
	gSVLBuffer.LineInfoB = (LinesTotalInfo*) CFIO_DAllocMemory( sizeof(LinesTotalInfo), MAF_GALL_GPTR, (pchar)"puma", (pchar)"SVL step II lines info pool");

	if ( rc )
	{
		rc = ShortVerticalLinesProcess( PUMA_SVL_FIRST_STEP, Image );
	}

    Handle h = NULL;
    BIG_IMAGE big_Image;
	int i;

//default Image:
	PAGEINFO info ={0};
	GetPageInfo(Image->hCPAGE,&info);
    for(i=0;i<CPAGE_MAXNAME;i++)
		big_Image.ImageName[i]=info.szImageName[i];
	big_Image.hCCOM=NULL;

	h=CPAGE_GetBlockFirst(Image->hCPAGE,TYPE_BIG_COMP);
	if(h)
	{
     CPAGE_GetBlockData(Image->hCPAGE,h,TYPE_BIG_COMP, &big_Image, sizeof(BIG_IMAGE));
     CPAGE_DeleteBlock(Image->hCPAGE,h);
	}
//    if(big_Image.hCCOM==NULL)
//		big_Image.hCCOM=(CCOM_handle)(Image->hCCOM);

	LDPUMA_Skip(hPicture);

//Поиск очевидных картинок
	if(rc)
	 rc=SearchPictures(Image,big_Image);

	LDPUMA_Skip(hNegative);

//Поиск негативов
	if(rc)
     rc=SearchNeg(Image,big_Image,info.Incline2048);

	LDPUMA_Skip(hLines3);

//Третий проход по линиям
	if(LDPUMA_Skip(hDebugLinePass3)&&LDPUMA_Skip(hDebugVerifLine)&&LDPUMA_Skip(hDebugLinePass2))
	{
	 if(rc)
		 RLINE_LinesPass3(Image->hCPAGE, Image->hCLINE, Image->hCCOM, (uchar)Image->gnLanguage);
	}

	LDPUMA_Skip(hSVLP);

	//////////////////////////////////////////////////////////////////////////////////////////
	//
	////снова подсчитываем короткие вертикальные линии и сравниваем с предыдущим результатом
	if ( rc )
	{
		rc = ShortVerticalLinesProcess( PUMA_SVL_SECOND_STEP, Image );
	}

	ShortVerticalLinesProcess( PUMA_SVL_THRID_STEP, Image );

	CFIO_FreeMemory(gSVLBuffer.LineInfoA);
	CFIO_FreeMemory(gSVLBuffer.LineInfoB);
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//

	LDPUMA_Skip(hBlocks);

	if(!ProgressStepAutoLayout(1, 40))
		rc = FALSE;
	if(!LDPUMA_Skip(Image->hDebugLayoutFromFile))
	{
		Image->hCPAGE = CPAGE_RestorePage(TRUE,(pchar)(Image->szLayoutFileName));
		if(Image->hCPAGE==NULL)
		{
			SetReturnCode_rmarker(CPAGE_GetReturnCode());
			rc= FALSE;
		}
		else
		{
			CPAGE_SetCurrentPage(CPAGE_GetNumberPage(Image->hCPAGE));
			LDPUMA_Console("Layout восстановлен из файла '%s'\n",Image->szLayoutFileName);
		}
	}
	else {
		if(rc)
		{
			if(LDPUMA_Skip(Image->hDebugCancelExtractBlocks))
			{
                Bool32  bEnableSearchPicture;
				PRGTIME	prev = StorePRGTIME(40, 100);
				RBLOCK_SetImportData(RBLOCK_FNRBLOCK_ProgressStep,(void*)rblockProgressStep);
				RBLOCK_SetImportData(RBLOCK_FNRBLOCK_ProgressFinish,(void*)rblockProgressFinish);
                bEnableSearchPicture = Image->gnPictures ;
                RBLOCK_SetImportData(RBLOCK_Bool32_SearchPicture,&bEnableSearchPicture);
                RBLOCK_SetImportData(RBLOCK_Bool32_OneColumn,&(Image->gbOneColumn));

				if(!RBLOCK_ExtractTextBlocks(Image->hCCOM,Image->hCPAGE,Image->hCLINE))
				{
					SetReturnCode_rmarker(RBLOCK_GetReturnCode());
					rc = FALSE;
				}
				RestorePRGTIME( prev );

			}
			else
				LDPUMA_Console("Пропущен этап автоматического Layout.\n");
		}
	}
	if(!ProgressStepAutoLayout(1,100))
		rc = FALSE;

    CCOM_DeleteContainer(big_Image.hCCOM);

	LDPUMA_Skip(hEnd);

	return rc;
}

void SetReturnCode_rmarker(uint32_t rc)
{
	gwRC = rc;
}

uint32_t GetReturnCode_rmarker(void)
{
	return gwRC;
}


Bool32 SearchNeg( PRMPreProcessImage Image,BIG_IMAGE big_Image,int skew)
{
 if(!LDPUMA_Skip(hDebugNeg))
	 return TRUE;

 RNEG_RecogNeg(big_Image.hCCOM,Image->hCPAGE,big_Image.ImageName,skew);
 return TRUE;
}

Bool32 SearchPictures ( PRMPreProcessImage Image, BIG_IMAGE big_Image )
{
	Bool32 rc=TRUE;

	if(!LDPUMA_Skip(hDebugPictures))
		return TRUE;

//	rc = ProgressStep(1,/*GetResourceString(IDS_PRG_OPEN),*/10);

	if(rc && LDPUMA_Skip(Image->hDebugCancelSearchPictures))
	{
		if( Image->gnPictures )
		{
			if(!RPIC_SearchPictures(Image->hCCOM, big_Image.hCCOM, Image->hCPAGE))
			{
				uint32_t RPicRetCode = RPIC_GetReturnCode();

				SetReturnCode_rmarker(RPicRetCode);
				rc = FALSE;
			}
		}
	}
	return rc;
}

int GetCountNumbers(int num)
{
	int count=0;
	if(num==0)
		return 1;
	for(num;num>0;num=num/10)
		count++;
	return count;
}

void MySetNegative (void *vB, Handle hCPage)
{
	int i, Ind, nRc;
	Bool WasNegTabl;
	Rect16 *pRc;
	POLY_ block = {0};
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	Ind = FindSuchAimedData (vB, UN_DT_Rect16, UN_DA_NegTablCap);
	WasNegTabl = (Ind >= 0);
	if (WasNegTabl)
	{
		nRc = pB->nPartUnits[Ind];
		pRc = (Rect16*)pB->vPart[Ind];
		for (i=0; i<nRc; i++)
		{
			block.com.type = TYPE_PICTURE; //Текст, Картинка, Таблица;
			block.com.count = 4;
			block.com.Flags |= POS_NEGTABCAP;
			block.com.Vertex[0].x = pRc[i].left;
			block.com.Vertex[0].y = pRc[i].top;
			block.com.Vertex[1].x = pRc[i].right;
			block.com.Vertex[1].y = pRc[i].top;
			block.com.Vertex[2].x = pRc[i].right;
			block.com.Vertex[2].y = pRc[i].bottom;
			block.com.Vertex[3].x = pRc[i].left;
			block.com.Vertex[3].y = pRc[i].bottom;
			CPAGE_CreateBlock(hCPage, TYPE_IMAGE,0,0,&block,sizeof(POLY_));
		}
	}
}

