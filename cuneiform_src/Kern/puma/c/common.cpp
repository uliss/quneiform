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

#include <stdio.h>
#include <string.h>

#include "mpuma.h"
#include "ccom.h"
#include "cstr.h"

void ClearAll( void )
{
	PAGEINFO           PInfo = {0};
//
// Сохраним последенне состояние и очистим контейнер
//
	if(ghEdPage)
	{
		CED_DeletePage(ghEdPage);
		ghEdPage = NULL;
	}

	if(hCPAGE)
		GetPageInfo(hCPAGE,&PInfo);

    CSTR_DeleteAll();
	CPAGE_DeleteAll(); hCPAGE = CreateEmptyPage();

	strcpy((char*)PInfo.szImageName, PUMA_IMAGE_USER);
	PInfo.Incline2048 = 0;
	PInfo.Angle = 0;
	PInfo.Images=IMAGE_USER;
	SetPageInfo(hCPAGE,PInfo);

	CCOM_DeleteAll();  hCCOM = NULL;
	CIMAGE_DeleteImage((puchar)PUMA_IMAGE_BINARIZE );
	CIMAGE_DeleteImage((puchar)PUMA_IMAGE_DELLINE	);
//  Повернутое изображение ( PUMA_IMAGE_ROTATE) удалять нельзя, как и исходное,
//  поскольку оно отображается в интерфейсе. Его нужно удалять
//  либо при получении нового довернутого изображения, либо при
//  закрытии файла
	CIMAGE_DeleteImage((puchar)PUMA_IMAGE_TURN		);
	/*
	if(hCPAGE && CPAGE_GetCountBlock(hCPAGE))
	{
		CPAGE_BackUp(hCPAGE);
		Handle hBlock = CPAGE_GetBlockFirst(hCPAGE,0);
		while(hBlock)
		{
			CPAGE_DeleteBlock(hCPAGE,hBlock);
			hBlock = CPAGE_GetBlockNext(hCPAGE,hBlock,0);
		}
	}
	*/
}
///////////////////////////////////////////////////////
// Функции прогресс индикатора
Bool32 rexcProgressStep  (uint32_t step)
{
	return ProgressStep(2,NULL,step);
}
///////////////////////////////////////////////////////
Bool32  ExtractComponents( Bool32 bIsRotate, Handle * prev_ccom, puchar name)
{
	Bool32 rc = TRUE;
	ExcControl      exc = {0};

    if(prev_ccom)
    {
        *prev_ccom = hCCOM ? hCCOM: NULL;
        hCCOM=NULL;
    }
    else
    {
        CCOM_DeleteContainer((CCOM_handle)hCCOM);
        hCCOM=NULL;
    }

	if(!REXC_SetImportData(REXC_ProgressStep, (void*)rexcProgressStep))
	{
		SetReturnCode_puma(REXC_GetReturnCode());
		return FALSE;
	}

	// будет распознавания эвентами
	exc.Control  = Ex_ExtraComp|/*Ex_EvnRecog|*/Ex_Picture;
    //exc.Control |= Ex_NetRecog;

	//Andrey: orientation is obtained from new library RNORM
    //exc.Control |= ( bIsRotate ? Ex_Orient : 0 );
    if( gnPictures )
        exc.Control |= Ex_PictureLarge;
/*
	if(rc && !REXC_SetEVNProperties(exc, GetModulePath(),(uchar)gnLanguage) )
	{ // инициализировать распознавание по эвентам и задать алфавит
		SetReturnCode_puma(REXC_GetReturnCode());
		rc = FALSE;
	}
	else
*/
	{
		uchar w8 = (uchar)gbDotMatrix;
			REXC_SetImportData(REXC_Word8_Matrix,&w8);

		w8 = (uchar)gbFax100;
			REXC_SetImportData(REXC_Word8_Fax1x2,&w8);
	}
/*
	if(rc && !REXCExtraDIB( exc, lpdata,0,0,0,0) ) // поиск компонент в DIB-e
*/
    CIMAGEIMAGECALLBACK clbk;
    if(rc && !CIMAGE_GetCallbackImage(name, &clbk))
    {
		SetReturnCode_puma(CIMAGE_GetReturnCode());
        rc = FALSE;
    }
    if( rc && !REXCExtracomp3CB(exc, // поиск компонент by 3CallBacks
        (TImageOpen)clbk.CIMAGE_ImageOpen,
        (TImageClose)clbk.CIMAGE_ImageClose,
        (TImageRead)clbk.CIMAGE_ImageRead)
        )
		{
		SetReturnCode_puma(REXC_GetReturnCode());
		rc = FALSE;
		}

	if(rc)
	{
		hCCOM = (Handle)REXCGetContainer();
		if(hCCOM==0)
		{
			SetReturnCode_puma(REXC_GetReturnCode());
			rc = FALSE;
		}

	}
	if(rc)
	{
		hCCOM = (Handle)REXCGetContainer();
		if(hCCOM==0)
		{
			SetReturnCode_puma(REXC_GetReturnCode());
			rc = FALSE;
		}
	}
	if(rc)
		SetUpdate(FLG_UPDATE_NO,FLG_UPDATE_CCOM);
	return rc ;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Будет перенесено в RSource.dll
Bool32  comp_over(CCOM_comp *sour,CCOM_comp *cur)
{
	int32_t   le,ri,up,dn,w,h;

	if( sour->left+sour->w<=cur->left || cur->left+cur->w<=sour->left )
		return FALSE;
	if( sour->upper+sour->h<=cur->upper || cur->upper+cur->h<=sour->upper )
		return FALSE;

	if( sour->left<cur->left )
		le = cur->left;
	else
		le = sour->left;

	if( sour->left+sour->w<cur->left+cur->w )
		ri = sour->left+sour->w;
	else
		ri = cur->left+cur->w;
	w=ri-le;
	if( sour->upper<cur->upper )
		up = cur->upper;
	else
		up = sour->upper;

	if( sour->upper+sour->h<cur->upper+cur->h )
		dn = cur->upper+cur->h;
	else
		dn = sour->upper+sour->h;
	h=dn-up;

	if( w>0 && h>0 && cur->w<2*sour->w && cur->h<2*sour->h &&
		w*2>cur->w && h*2>cur->h )
		return TRUE;
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// будет перенесено в RSource.dll
Bool32  remove_overlayed(CCOM_comp *sour,CCOM_handle haCCOM)
{
CCOM_comp   *cur=CCOM_GetFirst(haCCOM,NULL), *curn;
int32_t       over=0;

do  {
    curn=CCOM_GetNext(cur,NULL);
    if( cur->upper<sour->upper && sour->upper-cur->upper>10*sour->h)
        { // upper zone
        cur=curn;
        continue;
        }
    if( comp_over(sour,cur) )
        {
        CCOM_Delete(haCCOM,cur);
        over++;
        }
    if( cur->upper-sour->upper>10*sour->h)
        break; // downer zone
    cur = curn;
    }while( cur!=NULL );
return (over>0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// будет перенесено в RSource.dll
// авторство принадлежит AlMi
Bool32 MyGetZher (void **vvZher, int32_t *nZher, int32_t MaxZher, Handle hCPage)
{
	uint32_t err32, nTeor, nReal;
	Handle hBlockZher;
	Handle hBlockPrev;
	int i;
	nTeor = sizeof (void *);
	i=0;
	while (1)
	{
		if (i==0)
			hBlockZher = CPAGE_GetBlockFirst (hCPage, RVERLINE_ZHERTVY_LINIY);
		else
			hBlockZher = CPAGE_GetBlockNext (hCPage, hBlockPrev, RVERLINE_ZHERTVY_LINIY);
		err32 = CPAGE_GetReturnCode ();
		if (err32!=0)
		{
			//if (i==0)
			//	Error_CPage ("[GetBlockFirst]");
			//else
			//	Error_CPage ("[GetBlockNext]");
			return FALSE;
		}
		if (!hBlockZher)
			break;
		if (i>=MaxZher)
			return FALSE;
		nReal = CPAGE_GetBlockData (hCPage, hBlockZher, RVERLINE_ZHERTVY_LINIY, (void *)&(vvZher[i]), nTeor);
		err32 = CPAGE_GetReturnCode ();
		if (!nReal||(err32!=0))
		{
			//Error_CPage ("[SetBlockData]");
			return FALSE;
		}
		hBlockPrev = hBlockZher;
		i++;
	}
	*nZher = i;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// будет перенесено в RSource.dll
Bool32 RemoveLines(Handle hccom,Handle hcpage,puchar * lppDIB)
{
	puchar hDIB = NULL;
	Bool32 rc = TRUE;
    hLinesCCOM = NULL;
    CCOM_comp   *victim[100];
    int32_t       nvict,i;
    Bool32      yes_victim = FALSE;
//
//	 Удалим линии
//
	if(rc && !RLINE_DeleteLines(hcpage,PUMA_IMAGE_DELLINE))
	{
		SetReturnCode_puma(RLINE_GetReturnCode());
		rc = FALSE;
	}
//
//	 Получим изображение с удаленными линиями
//
	if(rc && !CIMAGE_ReadDIB((puchar)PUMA_IMAGE_DELLINE,(Handle*)&hDIB,TRUE))
	{
		SetReturnCode_puma(CIMAGE_GetReturnCode());
		rc = FALSE;
	}
	if(hDIB)
	{
//
//		 Удалим компоненты и выделим их заново.
//
		*lppDIB = (puchar)hDIB;
		if(rc)
		{
        if( CCOM_GetContainerVolume((CCOM_handle)hCCOM)<60000 &&
            MyGetZher ((void**)victim, &nvict, 100, hcpage) && nvict )
            yes_victim = TRUE;

        if( !yes_victim )
            {
            CCOM_DeleteContainer((CCOM_handle)hCCOM);
            hCCOM=0;
            }

		if(!ExtractComponents(FALSE,&hLinesCCOM,(puchar)PUMA_IMAGE_DELLINE))
		{
				rc = FALSE;
		}
        else
        {
            PAGEINFO inf = {0};
            GetPageInfo(hCPAGE,&inf);
            strcpy((char*)inf.szImageName,PUMA_IMAGE_DELLINE);
            SetPageInfo(hCPAGE,inf);
        }

		if(rc)
		{

			hCCOM = (Handle)REXCGetContainer();
			if(hCCOM==0)
			{
				SetReturnCode_puma(REXC_GetReturnCode());
				rc = FALSE;
			}
			hccom = hCCOM;
            if( hLinesCCOM )
                {
//
//                 Refersh CCOM
//
                CCOM_comp   *exa=CCOM_GetFirst((CCOM_handle)hLinesCCOM,NULL);

                if( yes_victim )
                {
                /*
                Rect16 rect1;
	            uint32_t key = 111;
                for(i=0;i<nvict;i++)
                    {
                    exa = victim[i];
                    rect1.top = exa->upper;
		            rect1.left = exa->left;
		            rect1.bottom = exa->upper+exa->h;
		            rect1.right = exa->left+exa->w;
		            LDPUMA_DrawRect(NULL, &rect1, 0, 23635, 1, key);
                    }

                if(!LDPUMA_Skip(hShowCheckLetters))
	                {
		                LDPUMA_Console("Puma_Коробки жертв  \n");
		                LDPUMA_WaitUserInput(NULL, NULL);
		                LDPUMA_DeleteRects(NULL, key);
	                }
                    */
                for(i=0;i<nvict;i++)
                    {
                    exa = victim[i];
                    if( remove_overlayed(exa,(CCOM_handle)hCCOM) )
                        {
                        CCOM_comp   *dup=CCOM_New((CCOM_handle)hCCOM,exa->upper,exa->left,exa->w,exa->h);
                        if( dup )
                            {
                            CCOM_Store(dup,0,
                                exa->size_linerep,
                                exa->linerep,  exa->nl,
                                exa->begs, exa->ends,
                                exa->vers,   NULL);
                            dup->scale= exa->scale;
                            dup->type = exa->type;
                            dup->cs   = exa->cs;
                            }
                        }
                    }
                }
                CCOM_DeleteContainer((CCOM_handle)hLinesCCOM);
                }
			}
			else
				LDPUMA_Console("Пропущен этап выделения компонент после удаления линий.\n");
		}
	}
return rc;
}

void SetOptionsToFRMT()
{
	RFRMT_SetImportData(RFRMT_Bool32_Bold,&gbBold);
	RFRMT_SetImportData(RFRMT_Bool32_Italic,&gbItalic);
	RFRMT_SetImportData(RFRMT_Bool32_Size,&gbSize);
	RFRMT_SetImportData(RFRMT_Word32_Format,&gnFormat);
	RFRMT_SetImportData(RFRMT_char_SerifName,gpSerifName);
	RFRMT_SetImportData(RFRMT_char_SansSerifName,gpSansSerifName);
	RFRMT_SetImportData(RFRMT_char_CourierName,gpCourierName);
	RFRMT_SetImportData(RFRMT_Word8_UnRecogSymbol,&gnUnrecogChar);
	RFRMT_SetImportData(RFRMT_Word32_Language,&gnLanguage);
}

Bool32    SaveToText(char * lpOutFileName,int code)
{
	Bool32 rc = TRUE;
int count = CSTR_GetMaxNumber();

if(code != PUMA_CODE_ANSI)
{
	SetReturnCode_puma(IDS_ERR_NOTIMPLEMENT);
	return FALSE;
}

FILE * f = fopen(lpOutFileName,"wt");
	if(f)
	{
		for(int i=1;i<=count;i++)
		{
		CSTR_line lin_out;
		char	txt[500];

			lin_out = CSTR_GetLineHandle(i, 1); // OLEG
			if(lin_out == (CSTR_line)NULL)
			{
				SetReturnCode_puma(CSTR_GetReturnCode());
				rc = FALSE;
				break;
			}

			if( CSTR_LineToTxt(lin_out,txt) )
			{
				char szString[sizeof(txt)];
				sprintf(szString,"%s\n",txt);
				unsigned len = strlen(szString);
				if(fwrite(szString,sizeof(char),len,f) != len)
				{
					SetReturnCode_puma(IDS_ERR_FILEWRITE);
					rc = FALSE;
					break;
				}
			}
			else
			{
				SetReturnCode_puma(CSTR_GetReturnCode());
				rc = FALSE;
				break;
			}

		}
		fclose(f);
	}
	else
	{
		SetReturnCode_puma(IDS_ERR_FILEOPEN);
		rc = FALSE;
	}
	return rc;
}
////////////////////////////////////////////////////////////
void ProgressStart()
{
	LDPUMA_ProgressStart();
	if(fnProgressStart)
		fnProgressStart();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
void ProgressFinish()
{
	LDPUMA_ProgressFinish();
	if(fnProgressFinish)
		fnProgressFinish();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 ProgressStep(uint32_t step,char*name,uint32_t percent)
{
	Bool32 rc = TRUE;
	static uint32_t old = 0;

	g_PrgTime.dwStep = step;
	g_PrgTime.name = name ? name : g_PrgTime.name;

	uint32_t perc = g_PrgTime.dwBeg + percent*(g_PrgTime.dwEnd - g_PrgTime.dwBeg)/100;
	rc = LDPUMA_ProgressStep(step,g_PrgTime.name,perc);
//	_ASSERT(perc>=old);
	if(fnProgressStep)
		rc &= fnProgressStep(step,g_PrgTime.name,perc);

	old = perc;
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 ProgressStepLayout(uint32_t step,uint32_t percent)
{
	return ProgressStep(step, GetResourceString(IDS_PRG_OPEN), percent);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 ProgressStepLines(uint32_t step,uint32_t percent)
{
	return ProgressStep(step, GetResourceString(IDS_REMOVELINE), percent);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 ProgressStepTables(uint32_t step,uint32_t percent)
{
	return ProgressStep(step, GetResourceString(IDS_REMOVELINE), percent);
}
//////////////////////////////////////////////////////
Bool32 ProgressStepSearchTables(uint32_t step,uint32_t percent)
{
	return ProgressStep(step, GetResourceString(IDS_SEARCHTABLE), percent);
}
//////////////////////////////////////////////////////
Bool32 ProgressStepAutoLayout(uint32_t step,uint32_t percent)
{
	return ProgressStep(step, GetResourceString(IDS_AUTOLAYOUT), percent);
}

static uint32_t bInitPrgTime = 0;
void ResetPRGTIME()
{
	bInitPrgTime = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32  DonePRGTIME()
{
	Bool32 rc = FALSE;
	if(	bInitPrgTime )
		bInitPrgTime--;
	if(!bInitPrgTime)
		rc = TRUE;
return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32  InitPRGTIME()
{
	Bool32 rc = FALSE;
	if(!bInitPrgTime)
	{
		g_PrgTime.dwBeg  = 0;
		g_PrgTime.dwEnd  = 100;
		g_PrgTime.dwStep = 0;
		g_PrgTime.name   = "";
		rc = TRUE;
	}
	bInitPrgTime++;
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
PRGTIME	 StorePRGTIME(uint32_t beg, uint32_t end)
{
	PRGTIME rc = g_PrgTime;

	uint32_t newBeg = g_PrgTime.dwBeg + (g_PrgTime.dwEnd - g_PrgTime.dwBeg)*beg/100;
	uint32_t newEnd = g_PrgTime.dwBeg + (g_PrgTime.dwEnd - g_PrgTime.dwBeg)*end/100;

	g_PrgTime.dwBeg = newBeg;
	g_PrgTime.dwEnd = newEnd;

return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
void RestorePRGTIME(PRGTIME	prev)
{
	g_PrgTime  = prev;
}
/////////////////////////////////////////////////////////////////////
Bool32 PrintResult(int num,CSTR_line lout,Handle hCPAGE)
{
	Bool32 rc = FALSE;
	CSTR_rast       start = CSTR_GetFirstRaster (lout),
		stop = CSTR_GetLastRaster (lout), c;
	UniVersions     vers;
	CSTR_rast_attr  attr;
	CSTR_attr		line_attr = {0};

	Bool32 bold = 0;
	Bool32 italic = 0;
	Bool32 strikeout = 0;
	Bool32 underline = 0;
	int32_t height = 0;
	int32_t offset = 0;
	uint32_t textcolor = 0;
	int charset = RUSSIAN_CHARSET;
	const char * name = NULL;
	static int32_t  nFragment = -1;
	static uint32_t deftextcolor = 0;
	Bool32 bOutputKegl = TRUE;

	CSTR_GetLineAttr (lout,&line_attr);

	if(line_attr.fragment != nFragment)
	{
		nFragment = -1;
		deftextcolor = 0;
		Handle hBlock = CPAGE_GetBlockFirst(hCPAGE,0);
		while(hBlock)
		{
			if(CPAGE_GetBlockInterNum(hCPAGE,hBlock) == (uint32_t)line_attr.fragment)
			{
				nFragment = line_attr.fragment;
				goto lNext;
			}
			hBlock = CPAGE_GetBlockNext(hCPAGE,hBlock,0);
		}
	}
lNext:
	if(nFragment<0)
		deftextcolor = 254L + (127L<<8);


	LDPUMA_SetConsoleProperty(0,0,0,0,0,0,deftextcolor,RUSSIAN_CHARSET,"Courier New");
	LDPUMA_Console("Фр.%2i Стр.%3i: <",line_attr.fragment,num);

	if( start && stop )
	{
		c=CSTR_GetNextRaster (start,CSTR_f_all);
		for(; c && c!=stop; c=CSTR_GetNextRaster (c,CSTR_f_all))
		{
			CSTR_GetAttr(c, &attr);
			if( !(attr.flg & (CSTR_f_let|CSTR_f_punct|CSTR_f_bad|CSTR_f_space|CSTR_f_solid)) )
				continue;
			if( CSTR_GetCollectionUni(c, &vers) )
			{

				if(LDPUMA_Skip(hDebugCancelPropertyConsole))
				{
					if(attr.font&CSTR_fp_bold)
						bold = TRUE;
					else
						bold = FALSE;

					if(attr.font&CSTR_fp_it)
						italic = TRUE;
					else
						italic = FALSE;

					if(attr.font&CSTR_fp_undrln)
						underline = TRUE;
					else
						underline = FALSE;

					height = attr.keg*10*2;
                    switch( attr.font_spec )
					{
					case    CSTR_fs_none:
						if(attr.font & CSTR_fp_ser)
							name = "Times New Roman";
						else if(attr.font & CSTR_fp_gelv)
							name = "Arial";
						else
							name = "Courier New";
						if( attr.font & CSTR_fp_narrow )
							name = "Arial Narrow";
						break;
					case    CSTR_fs_courier:
						name = "Courier New";
						break;
					}


					textcolor = 0;
					if(attr.flg_spell & CSTR_fa_spell_reject)
						textcolor |= 200<<16;// RGB
                    if(attr.flg_spell & CSTR_fa_spell_nocarrying)
						textcolor |= 200<<8;

					if(!vers.lnAltCnt || vers.Alt[0].Prob<70)
						textcolor |= 200;
				}

				charset = vers.Alt[0].Charset;
				LDPUMA_SetConsoleProperty(bold,italic,strikeout,
					underline,height,offset,textcolor,charset,name);

				if( !vers.lnAltCnt )
					LDPUMA_Console("%c",'~');
				else
				{
                    if( vers.Alt[0].Code[0]=='%' )
                        LDPUMA_Console((char *)"%%");
                    else
						LDPUMA_Console((char *)vers.Alt[0].Code);
				}

				if(bOutputKegl && !LDPUMA_Skip(hDebugCancelConsoleKeglOutput))
				{
					LDPUMA_SetConsoleProperty(0,0,0,0,0,0,150L<<8,RUSSIAN_CHARSET,NULL);
					LDPUMA_Console(">%i<",attr.keg);
					bOutputKegl = FALSE;
				}
			}
		}
		rc = TRUE;
	}
	LDPUMA_SetConsoleProperty(0,0,0,0,0,0,deftextcolor,RUSSIAN_CHARSET,NULL);
	LDPUMA_Console(">\n");

	if(deftextcolor)
		LDPUMA_SetConsoleProperty(0,0,0,0,0,0,0,RUSSIAN_CHARSET,NULL);

	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32  HL_TableExtractEx( Handle hPAGE, uint32_t perc, Rect32 rect )
{
	return RMARKER_SearchTableInZone(hPAGE,hCCOM,perc,rect);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 IsUpdate(uint32_t flg)
{
	return (g_flgUpdate & flg) > 0;
}

void   SetUpdate(uint32_t flgAdd,uint32_t flgRemove)
{
	g_flgUpdate = (g_flgUpdate | flgAdd) & ~flgRemove;
}
