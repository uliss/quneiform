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

/**********  Заголовок  *******************************************************/
/*  Автор,                                                                    */
/*  комментарии                                                               */
/*  и далнейшая                                                               */
/*  правка     :  Алексей Коноплев                                            */
/*  Редакция   :  08.06.00                                                    */
/*  Файл       :  'Normalise.cpp'                                             */
/*  Содержание :  Нормализация сырья                                          */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/

#include <fstream>
#include <stdio.h>
#include <string.h>
#include "puma.h"
#include "rstuff.h"
#include "rsfunc.h"
#include "rsglobaldata.h"
#include "rsmemory.h"
#include "mpumatime.h"
#include "pumadef.h"
#include "lnsdefs.h"
#include "un_buff.h"
#include "line_vp_util.h"
#include "line_vp_2_am.h"
#include "markdataoper.h"
#include "un_err.h"
#include "specprj.h"

// модули
#include "ccom.h"
#include "cpage.h"
#include "criimage.h"
#include "ctiimage.h"
#include "dpuma.h"
#include "exc.h"
#include "puma.h"
#include "rpic.h"
#include "rline.h"
#include "rverline.h"
#include "smetric.h"
#include "rreccom.h"

const int MIN_BIG_H=30;
const int MIN_BIG_W=30;
# define  TYPE_BIG_COMP		CPAGE_GetInternalType("TYPE_BIG_COMP")
struct BIG_IMAGE
{
	CCOM_handle hCCOM;
	uchar ImageName[CPAGE_MAXNAME];
};

#define RSL_VERLINE CPAGE_GetInternalType("RVL_VERIFY")
extern Bool32 gbRSLT;

extern Handle ObvKillLines;
extern Handle hNewLine;
extern Handle NotKillPointed;
//extern Handle hUseCLine;
extern Handle hAngles;
extern Handle hCalcMuchSkew;
extern Handle hTalkMuchSkew;

extern Handle hPrep;
extern Handle hContBigComp;
extern Handle hSearchLine;
extern Handle hDotLine;
extern Handle hCalcIncline;
extern Handle hVerOrNewLine;
extern Handle hOrto;
extern Handle hKillLine;
extern Handle hPrep2;
extern Handle hKillLineAfter;
extern Handle hEnd;
extern Handle hDebugAutoTemplate;
extern Handle hWndTurn;
extern Handle hDebugPrintResolution;

Bool32    AutoTemplate          ( PRSPreProcessImage );
void checkResolution(Handle hCCOM, Handle hCPAGE);
////////////////////////////////////////////////////////////////////////////////
// Нормализация сырь
// (07.07.2000) Изначально взято из puma.dll без изменений
Bool32 Normalise( PRSPreProcessImage Image)
{
	Bool32 rc;

	LDPUMA_Skip(hPrep);

	rc =  PreProcessImage( Image );

  LDPUMA_Skip(hDebugAutoTemplate);

//	if( rc )
//		rc = AutoTemplate( Image );
//		rc = RNORM_AutoTemplate( Image );

  LDPUMA_Skip(hSearchLine);

	if ( rc )
		rc = SearchLines( Image );

    LDPUMA_Skip(hCalcIncline);

	if( rc )
		rc = CalcIncline( Image );

	LDPUMA_Skip(hOrto);

	if (rc)
		rc = OrtoMove( Image );

	LDPUMA_Skip(hContBigComp);

	if(rc)
	    rc= CreateContainerBigComp( Image );

	LDPUMA_Skip(hVerOrNewLine);

/*    if (rc &&

		//  В Турецком зазря удаляются точки над буквами. 02.07.2002 E.P..
		Image->gnLanguage != LANG_TURKISH &&

		LDPUMA_Skip(hDotLine) &&
		LDPUMA_Skip(Image->hDebugCancelSearchDotLines)
	   )
//    if (rc && LDPUMA_Skip(hDotLine) && LDPUMA_Skip(Image->hDebugCancelSearchDotLines))
        rc = SLINEDOT_SearchDotLines(*(Image->phCCOM), Image->hCPAGE, Image->phCLINE);//Almi 0822
//        SLINEDOT_SearchDotLines(*(Image->phCCOM), Image->hCPAGE, Image->phCLINE);*/

	if ( rc )
        rc = SearchNewLines( Image );

    LDPUMA_Skip(hKillLine);

	// снятие линий
	if ( rc )
		rc = KillLinesN( Image );

	LDPUMA_Skip(hKillLineAfter);

	// убиваем остатки линии после сняти
	if ( rc && LDPUMA_Skip(Image->hDebugCancelRemoveLines))
//		rc = //almi 28.11.00
             LineKiller(Image);

	LDPUMA_Skip(hEnd);

    return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//



Bool32    VerifyN( PRSPreProcessImage Image )
{
	Bool32 rc;

	rc = VerifyLines( Image );

	return rc;
}

Bool32 SearchNewLines( PRSPreProcessImage Image )
{
    Bool32 ret = TRUE;
    bool searchlines = LDPUMA_Skip(Image->hDebugCancelSearchDotLines) && !LDPUMA_Skip(hDotLine);
    Handle hSaveImage = CPAGE_CreateBlock(Image->hCPAGE, RSL_VERLINE, 0, 0, Image, sizeof (RSPreProcessImage));

    if(LDPUMA_Skip(Image->hDebugCancelVerifyLines))
    {
        ret=RLINE_LinesPass1(Image->hCPAGE,*(Image->phCCOM),Image->phCLINE,Image->pgneed_clean_line, searchlines, (uchar)Image->gnLanguage);

        if (ret && !gbRSLT)
            ret = RLINE_LinesPass2(*(Image->phCCOM),Image->phCLINE, Image->hCPAGE);
    }

    CPAGE_DeleteBlock(Image->hCPAGE, hSaveImage);

    return ret;
}

Bool32 CreateContainerBigComp( PRSPreProcessImage Image )
{
 CCOM_handle hCCOM_old=(CCOM_handle)(*(Image->phCCOM));
 Handle hCPage=Image->hCPAGE;
 CCOM_handle hCCOM_new=0;
 BIG_IMAGE big_Image;
 PAGEINFO info = {0};
 GetPageInfo(hCPage,&info);
 int i=0;

 for(i=0;i<CPAGE_MAXNAME;i++)
	  big_Image.ImageName[i]=info.szImageName[i];

 hCCOM_new = CCOM_CreateContainer();
 if(!hCCOM_new)
 {
	big_Image.hCCOM=NULL;
    return TRUE;
 }

 CCOM_comp* comp=NULL;
 CCOM_comp* new_comp;
 comp=CCOM_GetFirst(hCCOM_old,FALSE);

  while(comp)
  {
	  if( (comp->h>=MIN_BIG_H)&&(comp->w>=MIN_BIG_W) )
	  {
	   new_comp=CCOM_New(hCCOM_new,comp->upper,comp->left,comp->w,comp->h);
	   if(new_comp)
	   {
		if(comp->size_linerep<0)
			;
		else
		{
         if(!CCOM_Copy(new_comp,comp) )
		       CCOM_Delete(hCCOM_new,comp);
		}
	   }
	  }
	  comp=CCOM_GetNext(comp,FALSE);
  }

  big_Image.hCCOM=hCCOM_new;

   CPAGE_CreateBlock(hCPage,TYPE_BIG_COMP,0,0,&big_Image,sizeof(BIG_IMAGE));
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32    KillLinesN( PRSPreProcessImage Image )
{
	Bool32 rc;

	rc = KillLines( Image );

	return rc;
}
////////////////////////////////////////////////////////////////////////////////
// Предварительная обработка
// (07.07.2000) Изначально взято из puma.dll без изменений
// сильно привязана к пуме
// в начале окучиваем выделение компонент
Bool32 PreProcessImage( PRSPreProcessImage Image )
{

	Bool32 gbAutoRotate          = Image->gbAutoRotate;
	puchar *gpRecogDIB           = Image->pgpRecogDIB;
	Handle hCPAGE                = Image->hCPAGE;
	const char * glpRecogName    = *Image->pglpRecogName;
	PCIMAGEBITMAPINFOHEADER info = (PCIMAGEBITMAPINFOHEADER)Image->pinfo;
	/////////////////////////////////
	Bool32 rc = TRUE;
	//char * lpRecogName = NULL;
	uint32_t Angle = 0;

	hWndTurn = 0;

	if(InitPRGTIME())
		ProgressStart();

	if(!ProgressStep(1,5))
		rc = FALSE;

	// Andrey 12.11.01
	// Проинициализируем контейнер CPAGE
	//
	if(rc)
	{
		PAGEINFO           PInfo = {0};
		GetPageInfo(hCPAGE,&PInfo);
		strcpy((char*)PInfo.szImageName, glpRecogName);
		PInfo.BitPerPixel = info->biBitCount;
		PInfo.DPIX = info->biXPelsPerMeter*254L/10000;
//		PInfo.DPIX = PInfo.DPIX < 200 ? 200 : PInfo.DPIX;
		PInfo.DPIY = info->biYPelsPerMeter*254L/10000;
//		PInfo.DPIY = PInfo.DPIY < 200 ? 200 : PInfo.DPIY;
		PInfo.Height = info->biHeight;
		PInfo.Width = info->biWidth;
//		PInfo.X = 0; Уже установлено
//		PInfo.Y = 0;
		PInfo.Incline2048 = 0;
		PInfo.Page = 1;
		PInfo.Angle = Angle;

		SetPageInfo(hCPAGE,PInfo);
	}

	////////////////////////////////////////////////////////
	// Выделим компоненты
	//
	if(!ProgressStep(2,65))
		rc = FALSE;

	if(rc)
	{
		if(LDPUMA_Skip(Image->hDebugCancelComponent)/*DPumaSkipComponent()*/)
		{
//			uchar ori;
			PRGTIME prev = StorePRGTIME(65, 85);
			rc = ExtractComponents( gbAutoRotate, NULL, (puchar)glpRecogName, Image);
			RestorePRGTIME(prev);
/*			if(rc && gbAutoRotate)
			{
				//if(!REXC_GetOrient(&ori))
				//if(!REXC_GetOrient(&ori) && db_spec_prj!=SPEC_PRJ_GIP )
				if(!RNORM_GetOrient(&ori, *(Image->phCCOM)) && db_spec_prj!=SPEC_PRJ_GIP )
				{
					SetReturnCode_rstuff(RNORM_GetReturnCode());
					rc = FALSE;
				}
				else
				{
					//if(ori)
					if(ori && !(db_spec_prj==SPEC_PRJ_GIP&&ori==4))
					{
						uint32_t dwTurn = 0;
						switch(ori)
						{
						case 1:
							Angle = 270;
							dwTurn = RIMAGE_TURN_270;
							break;
						case 2:
							Angle = 90;
							dwTurn = RIMAGE_TURN_90;
							break;
						case 3:
							Angle = 180;
							dwTurn = RIMAGE_TURN_180;
							break;
						}

						if( LDPUMA_Skip(Image->hDebugCancelTurn) /*umaSkipTurn()*/ /*)
						{
							if(!RIMAGE_Turn((puchar)glpRecogName,(puchar)PUMA_IMAGE_TURN,dwTurn,0))
							{
								SetReturnCode_rstuff_rstuff(RIMAGE_GetReturnCode());
								rc = FALSE;
							}
							else
							{

								if(!CIMAGE_ReadDIB((puchar)PUMA_IMAGE_TURN,(Handle*)gpRecogDIB,TRUE))
								{
									SetReturnCode_rstuff_rstuff(CIMAGE_GetReturnCode());
									rc = FALSE;
								}
								else
								{
									//
									//  удалим общий контейнер
									//
									glpRecogName = PUMA_IMAGE_TURN;
									hWndTurn = LDPUMA_CreateWindow(PUMA_IMAGE_TURN,(*gpRecogDIB));
									PRGTIME prev = StorePRGTIME(85, 100);
									rc = ExtractComponents( FALSE, NULL, (puchar)glpRecogName, Image);
									PAGEINFO info = {0};
		                            GetPageInfo(hCPAGE,&info);
									info.Images|=IMAGE_TURN;
//									strcpy((char*)info.szImageName,PUMA_IMAGE_TURN);
		                            SetPageInfo(hCPAGE,info);
                                    RestorePRGTIME(prev);
								}
							}
						}
					}
				}
			}*/

			//проверим наличие разрешения и попытаемся определить по компонентам, если его нет
			checkResolution(*(Image->phCCOM), hCPAGE);
			if(!ProgressStep(2,100))
				rc = FALSE;

		}
		else
			LDPUMA_Console("Пропущен этап выделения компонент.\n");
	}
	//
	// Переинициализируем контейнер CPAGE
	//
	if(rc)
	{
		PAGEINFO           PInfo = {0};
		GetPageInfo(hCPAGE,&PInfo);
		strcpy((char*)PInfo.szImageName, glpRecogName);
		PInfo.BitPerPixel = info->biBitCount;
//		PInfo.DPIX = info->biXPelsPerMeter*254L/10000;
		PInfo.DPIX = PInfo.DPIX < 200 ? 200 : PInfo.DPIX;
//		PInfo.DPIY = info->biYPelsPerMeter*254L/10000;
		PInfo.DPIY = PInfo.DPIY < 200 ? 200 : PInfo.DPIY;
		PInfo.Height = info->biHeight;
		PInfo.Width = info->biWidth;
//		PInfo.X = 0; Уже установлено
//		PInfo.Y = 0;
		PInfo.Incline2048 = 0;
		PInfo.Page = 1;
		PInfo.Angle = Angle;

		SetPageInfo(hCPAGE,PInfo);
	}

	if(DonePRGTIME())
		ProgressFinish();

	return rc;
}
//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Выделение компонент
Bool32  ExtractComponents( Bool32 bIsRotate, Handle * prev_ccom, puchar name, PRSPreProcessImage Image)
{
	Bool32 rc = TRUE;
	ExcControl      exc = {0};
//	RSGETMODULEPATH  pGetModulePath;
//	RSSETUPDATE      pSetUpdate;
//
//	if ( ProgressPoints.pGetModulePath && ProgressPoints.pSetUpdate)
//	{
//		pGetModulePath = (RSGETMODULEPATH)ProgressPoints.pGetModulePath;
//		pSetUpdate     = (RSSETUPDATE)ProgressPoints.pSetUpdate;
//	}
//	else
//	{
//		return false;
//	}

    if(prev_ccom)
    {
        *prev_ccom = *Image->phCCOM ? *Image->phCCOM: NULL;
        *Image->phCCOM=NULL;
    }
    else
    {
        CCOM_DeleteContainer((CCOM_handle)*Image->phCCOM);
        *Image->phCCOM=NULL;
    }

	if(!REXC_SetImportData(REXC_ProgressStep, (void*)rexcProgressStep))
	{
		SetReturnCode_rstuff(REXC_GetReturnCode());
		return FALSE;
	}

	// будет распознавания эвентами
	//Andrey: опознавалка вынесена в отдельный модуль RRecCom
	exc.Control  = Ex_ExtraComp|/*Ex_EvnRecog|*/Ex_Picture;
    //exc.Control |= Ex_NetRecog;

	//Andrey: orientation is obtained from new library RNORM
    //exc.Control |= ( bIsRotate ? Ex_Orient : 0 );

//Andrey: without flag Ex_PictureLarge big comps aren't extracted which may cause loss of big negatives
//    if( Image->gnPictures )
        exc.Control |= Ex_PictureLarge;
/*//Andrey: опознавалка вынесена в отдельный модуль RRecCom
	if(rc && !REXC_SetEVNProperties(exc, GetModulePath(),(uchar)Image->gnLanguage) )
	{ // инициализировать распознавание по эвентам и задать алфавит
		SetReturnCode_rstuff(REXC_GetReturnCode());
		rc = FALSE;
	}
	else
*/	{
		uchar w8 = (uchar)Image->gbDotMatrix;
			REXC_SetImportData(REXC_Word8_Matrix,&w8);

		w8 = (uchar)Image->gbFax100;
			REXC_SetImportData(REXC_Word8_Fax1x2,&w8);
	}

//exc.Control ^= Ex_EvnRecog;
/*
	if(rc && !REXCExtraDIB( exc, lpdata,0,0,0,0) ) // поиск компонент в DIB-e
*/
    CIMAGEIMAGECALLBACK clbk;
    if(rc && !CIMAGE_GetCallbackImage(name, &clbk))
    {
		SetReturnCode_rstuff(CIMAGE_GetReturnCode());
        rc = FALSE;
    }
    if( rc && !REXCExtracomp3CB(exc, // поиск компонент by 3CallBacks
        (TImageOpen)clbk.CIMAGE_ImageOpen,
        (TImageClose)clbk.CIMAGE_ImageClose,
        (TImageRead)clbk.CIMAGE_ImageRead)
        )
		{
		SetReturnCode_rstuff(REXC_GetReturnCode());
		rc = FALSE;
		}

	if(rc)
	{
		*Image->phCCOM = (Handle)REXCGetContainer();
		if(*Image->phCCOM==0)
		{
			SetReturnCode_rstuff(REXC_GetReturnCode());
			rc = FALSE;
		}

		RRecComControl rec_control;
		memset(&rec_control, 0, sizeof(RRecComControl));
		rec_control.flags = RECOG_EVN;

		if (!RRECCOM_Recog(*(Image->phCCOM), rec_control, GetModulePath(), (uchar)Image->gnLanguage))
		{
			SetReturnCode_rstuff(RRECCOM_GetReturnCode());
			rc = FALSE;
		}
	}
	if(rc)
		SetUpdate(FLG_UPDATE_NO,FLG_UPDATE_CCOM);
	return rc ;
}
//////////////////////////////////////////////////////////////////////////////////

Bool32 SearchLines ( PRSPreProcessImage Image )
{
	Bool32 rc = TRUE;

	if(LDPUMA_Skip(Image->hDebugCancelSearchLines))
	{
		Bool32 b32 = !Image->gbDotMatrix;
		RLINE_SetImportData(RLINE_Bool32_NOFILLGAP3,&b32);
		b32 = TRUE;
		RLINE_SetImportData(RLINE_Bool32_NOHBORDER,&b32);
		RLINE_SetImportData(RLINE_Bool32_NOVBORDER,&b32);

		if(!RLINE_SearchLines(Image->hCPAGE,Image->phCLINE))
		{
			//SetReturnCode_rstuff(RLINE_GetReturnCode());
			//rc = FALSE;
			*Image->pgrc_line = FALSE;
			LDPUMA_Console("ПРЕДУПРЕЖДЕНИЕ: RLINE(0x%X) %s\n",RLINE_GetReturnCode(),RLINE_GetReturnString(RLINE_GetReturnCode()));
		}
	}
	else
		LDPUMA_Console("Пропущен этап поиска линий.\n");
	return rc;

}
//////////////////////////////////////////////////////////////////////////////////
Bool32 VerifyLines ( PRSPreProcessImage Image )
{
	Bool32 rc = TRUE;

#ifdef _USE_RVERLINE_
	if( *Image->pgrc_line )
	{
		if(LDPUMA_Skip(Image->hDebugCancelVerifyLines))
		{
			Regime_VerifyLines val = Image->gnTables ? RVL_FutuTablCorr:RVL_Default;

			if( !RVERLINE_SetImportData(RVERLINE_DTRVERLINE_RegimeOfVerifyLines,&val)||
				!RVERLINE_MarkLines(*Image->phCCOM, Image->hCPAGE))
			{
				SetReturnCode_rstuff(RVERLINE_GetReturnCode());
				rc = FALSE;
			}
			else
			{
				Bool32 BadScan = FALSE;
				int32_t  ScanQual= 0;
//				RVERLINE_AboutLines(Image->hCPAGE,*Image->phCCOM, Image->pgneed_clean_line, &BadScan, &ScanQual);			}
				AboutLines(Image, &BadScan, &ScanQual);
			}

			if(!*Image->pgneed_clean_line)
				LDPUMA_Console("Предупреждение: RSTUFF говорит, что снимать линии не надо.\n");
			}
		else
			LDPUMA_Console("Пропущен этап оценки линий.\n");
////		if(LDPUMA_Skip(hDebugCancelVerifyLines))
////		{
////			Regime_VerifyLines val = gnTables ? RVL_FutuTablCorr:RVL_Default;
////
////			if( !RLTABLE_SetImportData(RLTABLE_DTRLTABLE_RegimeOfVerifyLines,&val)||
////				!RLTABLE_MarkLines(hCCOM, hCPAGE))
////			{
////				SetReturnCode_rstuff(RLTABLE_GetReturnCode());
////				rc = FALSE;
////			}
////			else
////			{
////				Bool32 BadScan = FALSE;
////				int32_t  ScanQual= 0;
////				RLTABLE_AboutLines(hCPAGE,hCCOM, &gneed_clean_line, &BadScan, &ScanQual);			}
////				if(!gneed_clean_line)
////					LDPUMA_Console("Предупреждение: RLTABLE_AboutLines говорит, что снимать линии не надо.\n");
////			}
////		else
////			LDPUMA_Console("Пропущен этап оценки линий.\n");
	}
#endif //_USE_RVERLINE_

	return rc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/*Bool32 ShortVerticalLinesProcess ( PRSPreProcessImage Image, uint32_t Step )
{
	Bool32 bRet = FALSE;

	if ( !gLTInfo )
		return bRet;

	if ( Step == RS_SVL_FIRST_STEP )
	{

		//gLTInfo->Hor.Lns = (LineInfo *)RSTUFFAlloc(sizeof(LineInfo) * RStuffMaxNumLines * 2);
		gLinesData.VLinefBufferA = gLTInfo->Ver.Lns = (LineInfo *)RSTUFFAlloc(sizeof(LineInfo) * RStuffMaxNumLines * 2);

		if (gLTInfo->Ver.Lns != NULL )
			bRet = TRUE;

		if ( bRet )
			bRet = ReadSVLFromPageContainer( Image, (void *)gLTInfo );

	}

	if ( Step == RS_SVL_SECOND_STEP )
	{
		LinesTotalInfo       LTInfo = {0};

		//LTInfo.Hor.Lns = (LineInfo *)RSTUFFAlloc(sizeof(LineInfo) * RStuffMaxNumLines * 2);
		gLinesData.VLinefBufferB = LTInfo.Ver.Lns = (LineInfo *)RSTUFFAlloc(sizeof(LineInfo) * RStuffMaxNumLines * 2);

		if (LTInfo.Ver.Lns != NULL )
			bRet = TRUE;

		if ( bRet )
			bRet = ReadSVLFromPageContainer( Image, (void *)&LTInfo );

		//RSTUFFFree ( LTInfo.Hor.Lns );
		RSTUFFFree ( gLinesData.VLinefBufferA );
		//RSTUFFFree ( gLTInfo->Hor.Lns );
		RSTUFFFree ( gLinesData.VLinefBufferB );
	}

	return bRet;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/*Bool32 ReadSVLFromPageContainer ( PRSPreProcessImage Image, void * pInf )
{
	Bool32         bRet = TRUE;

	LinesTotalInfo *LTInfo = (LinesTotalInfo *) pInf;
	LineInfo       *pLHor, *pLVer;

	Handle         hBlockLine;
	Handle         hBlockLineHor;
	Handle         hBlockLineVer;
	Handle         hBlockLinePrev;
	uint32_t         nTagSize;
	uint32_t         nReal;
	uint32_t         wErr32;

	nTagSize = sizeof (LinesTotalInfo);
	pLHor = LTInfo->Hor.Lns;
	pLVer = LTInfo->Ver.Lns;

	hBlockLine = CPAGE_GetBlockFirst (Image->hCPAGE, RLINE_BLOCK_TYPE);
	if (!hBlockLine)
	{
		//sprintf (pStr, "Линии не выделялись.");
		//return RV_EMPTY;
		bRet =  FALSE;
	}

	wErr32 = CPAGE_GetReturnCode ();

	if (wErr32!=0)
	{
		//Error_CPage ("[GetBlockFirst]");
		bRet =  FALSE;
	}
	//берем.... что берем?
	if ( bRet )
	{
		nReal = CPAGE_GetBlockData (Image->hCPAGE, hBlockLine, RLINE_BLOCK_TYPE, (void *)LTInfo, nTagSize);

		wErr32 = CPAGE_GetReturnCode ();
	}

	if ((nReal!=nTagSize)||(wErr32!=0))
	{
		//Error_CPage ("[GetBlockData]");
		bRet = FALSE;
	}
	if (LTInfo->Hor.Cnt + LTInfo->Ver.Cnt >= RStuffMaxNumLines)
	{
		//sprintf (pStr, "Не хватило памяти под %d линии!", LTInfo.Hor.Cnt + LTInfo.Ver.Cnt);
		//return RV_DOUBT;
		bRet = FALSE;
	}
	if ((LTInfo->Hor.Cnt==0)&&(LTInfo->Ver.Cnt==0))
	{
		//sprintf (pStr, "Линии выделялись, но ни одной не выделено.");
		//return RV_EMPTY;
		bRet = FALSE;
	}
	//  Горизонтальные линии
	if ( bRet && pLHor != NULL )
	{
		for (int32_t i=0; i<LTInfo->Hor.Cnt; i++)
		{
			if (i==0)
				hBlockLineHor = CPAGE_GetBlockFirst (Image->hCPAGE, (uint32_t)(LTInfo->Hor.Lns));
			else
				hBlockLineHor = CPAGE_GetBlockNext (Image->hCPAGE, hBlockLinePrev, (uint32_t)(LTInfo->Hor.Lns));
			wErr32 = CPAGE_GetReturnCode ();
			if (wErr32!=0)
			{
				//if (i==0)
					//Error_CPage ("[GetBlockFirst]");
				//else
					//Error_CPage ("[GetBlockNext]");
				bRet = FALSE;
				break;
			}
			nTagSize = sizeof (LineInfo);
			nReal = CPAGE_GetBlockData (Image->hCPAGE, hBlockLineHor, (uint32_t)(LTInfo->Hor.Lns), (void *)&(pLHor[i]), nTagSize);
			wErr32 = CPAGE_GetReturnCode ();
			if ((nReal!=nTagSize)||(wErr32!=0))
			{
				//Error_CPage ("[GetBlockData]");
				bRet = FALSE;
				break;
			}
			hBlockLinePrev = hBlockLineHor;
		}
	}
	//  Вертикальные линии
	if ( bRet && pLVer != NULL )
	{
		for (int32_t i=0; i<LTInfo->Ver.Cnt; i++)
		{
			if (i==0)
				hBlockLineVer = CPAGE_GetBlockFirst (Image->hCPAGE, (uint32_t)(LTInfo->Ver.Lns));
			else
				hBlockLineVer = CPAGE_GetBlockNext (Image->hCPAGE, hBlockLinePrev, (uint32_t)(LTInfo->Ver.Lns));
			wErr32 = CPAGE_GetReturnCode ();
			if (wErr32!=0)
			{
				//if (i==0)
					//Error_CPage ("[GetBlockFirst]");
				//else
					//Error_CPage ("[GetBlockNext]");
				bRet = FALSE;
				break;
			}
			nTagSize = sizeof (LineInfo);
			nReal = CPAGE_GetBlockData (Image->hCPAGE, hBlockLineVer, (uint32_t)(LTInfo->Ver.Lns), (void *)&(pLVer[i]), nTagSize);
			wErr32 = CPAGE_GetReturnCode ();
			if ((nReal!=nTagSize)||(wErr32!=0))
			{
				//Error_CPage ("[GetBlockData]");
				bRet = FALSE;
				break;
			}
			hBlockLinePrev = hBlockLineVer;
		}
	}

	//*pHoriType = (uint32_t)LTInfo.Hor.Lns;
	//*pVertType = (uint32_t)LTInfo.Ver.Lns;
	//LTInfo->Hor.Lns = pLHor;
	//LTInfo->Ver.Lns = pLVer;

	return bRet;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////
// удаляем линии
Bool32    KillLines(PRSPreProcessImage Image)
{
	Bool32 rc = TRUE;

	if(!ProgressStepLines(1/*,GetResourceString(IDS_REMOVELINE)*/,30))
		rc = FALSE;

	if( rc && *Image->pgrc_line && *Image->pgneed_clean_line)
	{
		if(LDPUMA_Skip(Image->hDebugCancelRemoveLines)	)
		{
			puchar pDIB = NULL;
			PRGTIME	prev = StorePRGTIME(30, 40);

			rc = RemoveLines(Image, &pDIB);

			if(rc)
			{
				*Image->pgpRecogDIB = pDIB;
				LDPUMA_CreateWindow(NAME_IMAGE_DELLINE,*Image->pgpRecogDIB);
			}
			RestorePRGTIME(prev);
		}
		else
			LDPUMA_Console("Пропущен этап снятия линий.\n");
	}

	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 RemoveLines(PRSPreProcessImage Image, puchar * lppDIB)
{
	Handle hccom = *Image->phCCOM;
	Handle hcpage = Image->hCPAGE;
	Handle *hLinesCCOM = Image->phLinesCCOM;

	puchar hDIB = NULL;
	Bool32 rc = TRUE;
    *hLinesCCOM = NULL;
    CCOM_comp   *victim[100];
    int32_t       nvict,i;
    Bool32      yes_victim = FALSE;
	//
	// Удалим линии
	//
	if(!LDPUMA_Skip(ObvKillLines)||(LDPUMA_Skip(hNewLine)&&LDPUMA_Skip(Image->hDebugCancelVerifyLines)))
		rc=DeleteLines(hcpage,Image->phCLINE,PUMA_IMAGE_DELLINE);
	else
	{
	 if(rc && !RLINE_DeleteLines(hcpage,PUMA_IMAGE_DELLINE))
	 {
		SetReturnCode_rstuff(RLINE_GetReturnCode());
		rc = FALSE;
	 }
     if (rc && LDPUMA_Skip(NotKillPointed) && LDPUMA_Skip(Image->hDebugCancelSearchDotLines))
         rc = DeleteDotLines(Image->phCLINE, PUMA_IMAGE_DELLINE);
	}

	LDPUMA_Skip(hPrep2);
	//
	// Получим изображение с удаленными линиями
	//
	if(rc && !CIMAGE_ReadDIB((puchar)PUMA_IMAGE_DELLINE,(Handle*)&hDIB,TRUE))
	{
		SetReturnCode_rstuff(CIMAGE_GetReturnCode());
		rc = FALSE;
	}
	if(hDIB)
	{
		//
		// Удалим компоненты и выделим их заново.
		//
		*lppDIB = (puchar)hDIB;
		if(rc)
		{
		//if( CCOM_GetContainerVolume((CCOM_handle)*Image->phCCOM)>30000 )
		//	CCOM_CompressContatiner((CCOM_handle)*Image->phCCOM);
        if( CCOM_GetContainerVolume((CCOM_handle)*Image->phCCOM)<60000 &&
            MyGetZher ((void**)victim, &nvict, 100, hcpage) && nvict )
            yes_victim = TRUE;

        if( !yes_victim )
            {
            CCOM_DeleteContainer((CCOM_handle)*Image->phCCOM);
            *Image->phCCOM = 0;
            }

		if(!ExtractComponents(FALSE, hLinesCCOM, (puchar)PUMA_IMAGE_DELLINE, Image))
		{
				rc = FALSE;
		}
        else
        {
            PAGEINFO inf = {0};
            GetPageInfo(Image->hCPAGE,&inf);
            strcpy((char*)inf.szImageName,PUMA_IMAGE_DELLINE);
			inf.Images|=IMAGE_DELLINE;
            SetPageInfo(Image->hCPAGE,inf);
        }

		if(rc)
		{

			*Image->phCCOM = (Handle)REXCGetContainer();
			if(*Image->phCCOM == 0)
			{
				SetReturnCode_rstuff(REXC_GetReturnCode());
				rc = FALSE;
			}
			hccom = *Image->phCCOM;
            if( *hLinesCCOM )
                {
                //
                // Refersh CCOM
                //
                CCOM_comp   *exa=CCOM_GetFirst((CCOM_handle)*hLinesCCOM,NULL);

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
                    if( remove_overlayed(exa,(CCOM_handle)*Image->phCCOM) )
                        {
                        CCOM_comp   *dup=CCOM_New((CCOM_handle)*Image->phCCOM,exa->upper,exa->left,exa->w,exa->h);
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
                CCOM_DeleteContainer((CCOM_handle)*hLinesCCOM);
                }
			}
			else
				LDPUMA_Console("Пропущен этап выделения компонент после удаления линий.\n");
		}
	}
return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
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
//
Bool32  remove_overlayed(CCOM_comp *sour,CCOM_handle haCCOM)
{
	CCOM_comp   *cur=CCOM_GetFirst(haCCOM,NULL), *curn;
	int32_t       over=0;

	do
	{
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
//
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
//
/*
Bool32    SearchTables( PRSPreProcessImage Image)
{
	Bool32 rc = TRUE;

	if(!ProgressStepTables(1,20))
		rc = FALSE;
	if( rc && Image->gnTables != PUMA_TABLE_NONE )
	{
		if(LDPUMA_Skip(Image->hDebugCancelSearchTables))
		{
			if (!RLTABLE_SetImportData(RLTABLE_DTRLTABLE_WhereMustSearchTable, NULL))
			{
				SetReturnCode_rstuff(RLTABLE_GetReturnCode());
				rc = FALSE;
			}
			else
			{
				int HowToSearch;
////				 устанавливаем способ поиска
				HowToSearch = SST_Default;
				if(!RLTABLE_SetImportData(RLTABLE_DTRLTABLE_StyleOfSearchTable, (void *)(&HowToSearch)))
				{
					SetReturnCode_rstuff(RLTABLE_GetReturnCode());
					rc = FALSE;
				}
				else
				{
  				   if(!RLTABLE_SearchTable(*Image->phCCOM,Image->hCPAGE, TRUE, Image->pgnNumberTables))
					{
						SetReturnCode_rstuff(RLTABLE_GetReturnCode());
						rc = FALSE;
					}
				}
			}
		}
		else
			LDPUMA_Console("Пропущен этап поиска таблиц.\n");
	}

	return rc;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//Bool32 AboutLines(PRSPreProcessImage Image, Bool32 *BadScan, int32_t *ScanQual)
//{
//	return TRUE;
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CalcIncline(PRSPreProcessImage Image)
{
	int SizeWork,SizeMain;
	char Str[256];
	Bool ret, WasLine, ManyComp;
	Bool CalcMuchSkew, TalkMuchSkew;
	uint16_t Code;
	int32_t SkewReg, Skew, SkewLocVerLin;
	Rect16 RcReg;
    PAGEINFO info = {0};
	UN_BUFF MainBuff = {0};
	void *vMain;
	char *cWork;
	Handle hCPage=Image->hCPAGE;
	CLINE_handle hCLINE=*((CLINE_handle*)Image->phCLINE);

	GetPageInfo(hCPage,&info);

	/*  2. Инициализация.  */
		/***  переменные  ***/
    WasLine = 0;
		/***  организация памяти  ***/
	GiveMainBuff (&vMain, &SizeMain);
	MainBuff.vBuff    = vMain;
	MainBuff.SizeBuff = SizeMain;
	MainBuff.vCurr    = MainBuff.vBuff;
	MainBuff.SizeCurr = MainBuff.SizeBuff;
	GiveWorkBuff (&cWork, &SizeWork);
		/***  линии  ***/
//	if(!LDPUMA_Skip(hUseCLine))
		ret=LoadLinesVP_rv(hCLINE,UN_LD_LinesVP2,&MainBuff,Str,&Code);
//	else
//	    ret = LoadLinesVP_rv (hCPage, UN_LD_LinesVP, (void *)(&MainBuff), Str, &Code);
	if ((ret!=RV_TRUE)&&(ret!=RV_EMPTY))
	{
		SetReturnCode_rstuff (Code);
		return ret;
	}
	WasLine = (ret==RV_TRUE);
		/***  компоненты  ***/
	ret = LoadComps_rv (*(Image->phCCOM), (void *)(&MainBuff), Str, 0); //t-e-d
	if (ret==RV_DOUBT)
	{
		SetReturnCode_rstuff (Code);
		CleanLastDataPart ((void *)(&MainBuff));
	}
	ManyComp = (ret==RV_TRUE)&&(MainBuff.nPartUnits[MainBuff.nPart-1]>10000);
	if (ManyComp)
	{
		CleanLastDataPart ((void *)(&MainBuff));
	}
	if (ret==RV_DOUBT||ManyComp)
	{
			ret = LoadComps_rv (*(Image->phCCOM), (void *)(&MainBuff), Str, 3); //t-e-d
			if (ret==RV_DOUBT)
			{
				SetReturnCode_rstuff (Code);
				CleanLastDataPart ((void *)(&MainBuff));
			}
	}
	if (ret!=RV_TRUE)
	{
		return ret;
	}

	RcReg.left=0;
	RcReg.right=(int16_t)info.Width;
	RcReg.top=0;
    RcReg.bottom=(int16_t)info.Height;
	SkewReg=0;
	Bool ContWarn = 0;
	SkewLocVerLin = 0;
	CalcMuchSkew = (!LDPUMA_Skip(hCalcMuchSkew));
	TalkMuchSkew = (!LDPUMA_Skip(hTalkMuchSkew));

//  5.1 Определяем угол наклона страницы (ее заполнения в целом).
//	if(!LDPUMA_Skip(hUseCLine))
		ret=ConvertLinesToAM_rv (UN_LD_LinesVP2, UN_LD_LinesAM, (void *)(&MainBuff)
			,(int *)cWork, SizeWork / sizeof (int), &RcReg, SkewReg, Str, ContWarn);
//	else
//		ret = ConvertLinesToAM_rv (UN_LD_LinesVP, UN_LD_LinesAM, (void *)(&MainBuff)
//			,(int *)cWork, SizeWork / sizeof (int), &RcReg, SkewReg, Str, ContWarn); // f-t-e-d
	if (ret==RV_DOUBT)
	{
			SetReturnCode_rstuff (Code);
	}
	if (ret!=RV_TRUE)
		return ret;

	SMetric_SetImportData(SMetric_ResolX, (void *)info.DPIX);
	SMetric_SetImportData(SMetric_ResolY, (void *)info.DPIY);
	ret = SMetric_FindMainSkew ((void *)(&MainBuff), cWork, SizeWork, &Skew
			, &SkewLocVerLin, &RcReg, SkewReg, Str, CalcMuchSkew, TalkMuchSkew);
	if (ret!=RV_TRUE)
		return ret;
	info.Incline2048 = Skew*2;
	info.SkewLocVerLin2048 = SkewLocVerLin*2;
	if(!CPAGE_SetPageData(Image->hCPAGE,PT_PAGEINFO,&info,sizeof(PAGEINFO)))
	{
		SetReturnCode_rstuff(CPAGE_GetReturnCode());
		return FALSE;
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void checkResolution(Handle hCCOM, Handle hCPAGE)
{
	PAGEINFO page_info = {0};
	const int min_res = 99;
	CCOM_comp* pcomp=NULL;
	unsigned int Masy[100], Masx[100], i, Jy_m = 0, My_m = 0, Jx_m = 0, Mx_m = 0, M_t;
	bool flag_set = false;

	if (!GetPageInfo(hCPAGE, &page_info)) return;

	if (page_info.DPIX > min_res && page_info.DPIY > min_res) return;

	for (i=0; i<100; i++) Masx[i] = Masy[i] = 0;

	pcomp = CCOM_GetFirst(hCCOM, NULL);

	while (pcomp)
	{
		if (pcomp->h > 9 && pcomp->h < 100) Masy[pcomp->h]++;

		if (pcomp->w > 9 && pcomp->w < 100) Masx[pcomp->w]++;

		pcomp = CCOM_GetNext(pcomp, NULL);
	}

	for (i=11; i < 99; i++)
	{
		M_t = Masy[i-1] + Masy[i] + Masy[i+1];

		if (M_t > My_m)
		{
			Jy_m = i;
			My_m = M_t;
		}

		M_t = Masx[i-1] + Masx[i] + Masx[i+1];

		if (M_t > Mx_m)
		{
			Jx_m = i;
			Mx_m = M_t;
		}
	}

	if (Jy_m > 10 && My_m > 100 && !(page_info.DPIY*22 < 2*300*Jy_m && 2*page_info.DPIY*22 > 300*Jy_m))
	{
		page_info.DPIY = (300*Jy_m+11)/22;
		flag_set = true;
	}

	if (Jx_m > 10 && Mx_m > 100 && !(page_info.DPIX*22 < 2*300*Jx_m && 2*page_info.DPIX*22 > 300*Jx_m))
	{
		page_info.DPIX = (300*Jx_m+11)/22;
		flag_set = true;
	}

	if (flag_set)
	{
		SetPageInfo(hCPAGE, page_info);

		if (!LDPUMA_Skip(hDebugPrintResolution))
			LDPUMA_ConsoleN("новое разрешение: DPIX=%d, DPIY=%d", page_info.DPIX, page_info.DPIY);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
