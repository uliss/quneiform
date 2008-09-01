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

#define __PUMA_CPP__
#define __PUMA__
//#include <windows.h>
//#include <stdio.h>
#include <string>
#include <cstring>
#include "mpuma.h"
#include "compat_defs.h"

extern "C"
{
	Bool32 rcolorStart( void )      { return TRUE; }
	Bool32 rcolorStep(Word32 perc)  {return ProgressStep(1,NULL,perc); }
	void rcolorFinish( void ) {};
}
//////////////////////////////////////////////////////////////////
// 07.07,2000 Allex
// Вынес Бинаризацию наверх из PreProcessImage
//////////////////////////////////////////////////////////Allex
//CIMAGEBITMAPINFOHEADER info;

Bool32 BinariseImage()
{
	Bool32 rc = TRUE;
	//char * lpRecogName = NULL;

	if(InitPRGTIME())
		ProgressStart();
	//
	// Бинаризуем изображение
	//
	gpRecogDIB = gpInputDIB;
	glpRecogName = PUMA_IMAGE_USER;

	if(!ProgressStep(1,GetResourceString(IDS_PRG_OPEN),10))
		rc = FALSE;

	/////////////////////////////////////////////////////Allex
	//CIMAGEBITMAPINFOHEADER info;
	if(!CIMAGE_GetImageInfo((PWord8)PUMA_IMAGE_USER,&info))
	{
		SetReturnCode_puma(CIMAGE_GetReturnCode());
		rc = FALSE;
	}

	if(!ProgressStep(2,GetResourceString(IDS_PRG_OPEN),100))
		rc = FALSE;

	LDPUMA_Console("The image depth is %d at this point.\n", (int)info.biBitCount);
	if(rc && LDPUMA_Skip(hDebugCancelBinarize) && info.biBitCount>1)
	{
		PRGTIME prev = StorePRGTIME(10, 100);
		if(!RIMAGE_Binarise((PWord8 )PUMA_IMAGE_USER,(PWord8)PUMA_IMAGE_BINARIZE,4,0))//RIMAGE_BINARISE_KRONROD
		{
			SetReturnCode_puma(RIMAGE_GetReturnCode());
			rc = FALSE;
		}
		else
		{
			if(!CIMAGE_ReadDIB((PWord8 )PUMA_IMAGE_BINARIZE,(Handle*)&gpRecogDIB,TRUE))
			{
				SetReturnCode_puma(CIMAGE_GetReturnCode());
				rc = FALSE;
			}
			else
			{
				LDPUMA_CreateWindow(PUMA_IMAGE_BINARIZE,gpRecogDIB);
				PAGEINFO info={0};
				GetPageInfo(hCPAGE,&info);
				info.Images|=IMAGE_BINARIZE;
				SetPageInfo(hCPAGE,info);
			}

			glpRecogName = PUMA_IMAGE_BINARIZE;
		}

		//              RCOLOR_SetProgress(rcolorStart,rcolorStep,rcolorFinish);
		//              if(!RCOLOR_Convert(gpInputDIB,&gpRecogDIB))
		//              {
		//                      SetReturnCode_puma(RCOLOR_GetReturnCode());
		//                      rc = FALSE;
		//              }
		//              else
		//                      LDPUMA_CreateWindow(PUMA_IMAGE_BINARIZE,gpRecogDIB);

		RestorePRGTIME(prev);
	}
	else
	{
		if(rc && info.biBitCount > 1)
		{ // Дальнейшая работа без бинаризатора невозможна !
			SetReturnCode_puma(IDS_ERR_NOBINARIZATION);
			rc = FALSE;
		}
	}

	if(DonePRGTIME())
		ProgressFinish();

	return rc;
}

/////////////////////////////////////////////////////////////////////////////////
Bool32 PreProcessImage()
{
	Bool32 rc = TRUE;
	char * lpRecogName = NULL;
	Word32 Angle = 0;

	if(InitPRGTIME())
		ProgressStart();

	if(!ProgressStep(1,GetResourceString(IDS_PRG_OPEN),5))
		rc = FALSE;

//	////////////////////////////////////////////////////////////////////Allex
//	// 07.07.2000 Allex
//	// вызывается уровнем выше перед вызовом PreProcessImage
//	//ClearAll();
//	// далее перенесено в наверх
//	/*
//	//
//	// Бинаризуем изображение
//	//
//	gpRecogDIB = gpInputDIB;
//	lpRecogName = PUMA_IMAGE_USER;
//
//	CIMAGEBITMAPINFOHEADER info;
//	if(!CIMAGE_GetImageInfo((PWord8)PUMA_IMAGE_USER,&info))
//	{
//		SetReturnCode_puma(CIMAGE_GetReturnCode());
//		rc = FALSE;
//	}
//
//	if(!ProgressStep(2,GetResourceString(IDS_PRG_OPEN),10))
//		rc = FALSE;
//
//	LDPUMA_Console("Изображение имеет %i бит на точку.\n",info.biBitCount);
//	if(rc && LDPUMA_Skip(hDebugCancelBinarize) && info.biBitCount>1)
//	{
//		PRGTIME prev = StorePRGTIME(10, 60);
//		if(!RIMAGE_Binarise((PWord8 )PUMA_IMAGE_USER,(PWord8)PUMA_IMAGE_BINARIZE,4,0))//RIMAGE_BINARISE_KRONROD
//		{
//			SetReturnCode_puma(RIMAGE_GetReturnCode());
//			rc = FALSE;
//		}
//		else
//		{
//			if(!CIMAGE_ReadDIB((PWord8 )PUMA_IMAGE_BINARIZE,(Handle*)&gpRecogDIB,TRUE))
//			{
//				SetReturnCode_puma(CIMAGE_GetReturnCode());
//				rc = FALSE;
//			}
//			else
//				LDPUMA_CreateWindow(PUMA_IMAGE_BINARIZE,gpRecogDIB);
//			lpRecogName = PUMA_IMAGE_BINARIZE;
//		}
//
//		//              RCOLOR_SetProgress(rcolorStart,rcolorStep,rcolorFinish);
//		//              if(!RCOLOR_Convert(gpInputDIB,&gpRecogDIB))
//		//              {
//		//                      SetReturnCode_puma(RCOLOR_GetReturnCode());
//		//                      rc = FALSE;
//		//              }
//		//              else
//		//                      LDPUMA_CreateWindow(PUMA_IMAGE_BINARIZE,gpRecogDIB);
//
//		RestorePRGTIME(prev);
//	}
//	else
//	{
//		if(rc && info.biBitCount > 1)
//		{ // Дальнейшая работа без бинаризатора невозможна !
//			SetReturnCode_puma(IDS_ERR_NOBINARIZATION);
//			rc = FALSE;
//		}
//	}
//	*/
//	//////////////////////////////////////////////////////////////////////Allex
//	//
//	// Запишем изображение
//	//
//	//      if(rc)
//	//      {
//	//              if(!CIMAGE_WriteDIB((PWord8)PUMA_IMAGE_BINARIZE,gpRecogDIB,TRUE))
//	//              {
//	//                      SetReturnCode_puma(CIMAGE_GetReturnCode());
//	//                      rc = FALSE;
//	//              }
//	//      }

	//
	// Выделим компоненты
	//
	if(!ProgressStep(2,GetResourceString(IDS_PRG_OPEN),65))
		rc = FALSE;

	if(rc)
	{
		if(LDPUMA_Skip(hDebugCancelComponent))
		{
			PRGTIME prev = StorePRGTIME(65, 85);
			rc = ExtractComponents( gbAutoRotate, NULL, (PWord8)glpRecogName);
			RestorePRGTIME(prev);

			if(!ProgressStep(2,NULL,100))
				rc = FALSE;

		}
		else
			LDPUMA_Console("Пропущен этап выделения компонент.\n");
	}
	//
	// Проинициализируем контейнер CPAGE
	//
	if(rc)
	{
		PAGEINFO           PInfo = {0};
		GetPageInfo(hCPAGE,&PInfo);
		strcpy((char*)PInfo.szImageName, glpRecogName);
		PInfo.BitPerPixel = info.biBitCount;
		PInfo.DPIX = info.biXPelsPerMeter*254L/10000;
		PInfo.DPIX = PInfo.DPIX < 200 ? 200 : PInfo.DPIX;
		PInfo.DPIY = info.biYPelsPerMeter*254L/10000;
		PInfo.DPIY = PInfo.DPIY < 200 ? 200 : PInfo.DPIY;
		PInfo.Height = info.biHeight;
		PInfo.Width = info.biWidth;
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
static Int32 s_ConsoleLine = 0;
static Bool32 PreOpenInitialize(const char * lpFileName)
{
Bool32 rc = TRUE;
#ifdef _DEBUG// Piter
	if(nDebugAllocMemory>0)
		_CrtSetBreakAlloc(nDebugAllocMemory);
#endif// Piter
	// Удалим предыдущие окна отладки.
	Handle hRemWnd= LDPUMA_GetWindowHandle(NAME_IMAGE_DELLINE);
	if(hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd= LDPUMA_GetWindowHandle(NAME_IMAGE_BINARIZE);
	if(hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd= LDPUMA_GetWindowHandle(NAME_IMAGE_INPUT);
	if(hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd= LDPUMA_GetWindowHandle(PUMA_IMAGE_TURN);
	if(hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd= LDPUMA_GetWindowHandle(NAME_IMAGE_ORTOMOVE);
	if(hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);

	PUMA_XClose();
	ResetPRGTIME();
	if(LDPUMA_Skip(hDebugRoot))
	{
		if(s_ConsoleLine)
			LDPUMA_ConsoleClear(s_ConsoleLine);
		s_ConsoleLine = LDPUMA_ConsoleGetCurLine();
	}
	else
	{
#ifdef _DEBUG
		// Память не должна накапливаться. К этой точке вся память
		// должна быть почищена!!
		/*
		_CrtMemState dbgMemState,dbgMemDiff;
		static _CrtMemState dbgPrevMemState = {0};
		static bInitMemState = TRUE;

 		_CrtMemCheckpoint( &dbgMemState );
		if ( !bInitMemState && _CrtMemDifference( &dbgMemDiff, &dbgPrevMemState, &dbgMemState ) )
		{

			_CrtMemDumpAllObjectsSince(&dbgMemDiff);
		}
		dbgPrevMemState = dbgMemState;
		bInitMemState = FALSE;
		*/
//		_CrtDumpMemoryLeaks();
		//_CrtMemDumpAllObjectsSince(&g_dbgMemState);
		_CrtMemDumpAllObjectsSince(NULL);
		//LDPUMA_Console("Press any key...\n");
		//LDPUMA_WaitUserInput(NULL,NULL);
		_CrtMemCheckpoint( &g_dbgMemState );

		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "OPEN FILE <%s>\n",lpFileName);
#endif
	}
	SetUpdate(FLG_UPDATE,FLG_UPDATE_NO);
	SetReturnCode_puma(IDS_ERR_NO);
	return rc;
}
//////////////////////////////////////////////////
static Bool32  PostOpenInitialize(const char * lpFileName)
{
	Bool32 rc = TRUE;
		CIMAGEBITMAPINFOHEADER info;
		if(lpFileName)
			LDPUMA_SetFileName(NULL,(char*)lpFileName);
		if(!CIMAGE_GetImageInfo((PWord8)PUMA_IMAGE_USER,&info))
		{
			SetReturnCode_puma(CIMAGE_GetReturnCode());
			rc = FALSE;
		}
		else
		{
			gRectTemplate.left = 0;
			gRectTemplate.right = info.biWidth;
			gRectTemplate.top = 0;
			gRectTemplate.bottom = info.biHeight;
		}
		if(lpFileName)
		{
			strcpy(szInputFileName,lpFileName);
			strcpy(szLayoutFileName,lpFileName);
			char * s = strrchr(szLayoutFileName,'.');
			if(s)
				*s = 0;
			strcat(szLayoutFileName,".bin");
		}
		else
			szInputFileName[0]='\0';
		hCPAGE = CreateEmptyPage();
		return rc;
}
/////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_XOpen(void * pDIB,const char * lpFileName)
{
	Bool32 rc = TRUE;
	PreOpenInitialize(lpFileName);
	if( pDIB == NULL)
	{
		SetReturnCode_puma(IDS_ERR_IMAGE);
		rc = FALSE;
	}
	else
		gpInputDIB = (PWord8)pDIB;
	//
	// Запишем изображение
	//
	if(rc)
	{
		if(!CIMAGE_WriteDIB((PWord8)PUMA_IMAGE_USER,pDIB,1))
		{
			SetReturnCode_puma(CIMAGE_GetReturnCode());
			rc = FALSE;
		}
	}
	if(rc)
		rc = PostOpenInitialize(lpFileName);
	return rc;
}
/////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_XOpenClbk(PUMAIMAGECALLBACK CallBack,const char * lpFileName)
{
	Bool32 rc = TRUE;
	PUMAIMAGECALLBACK * lpCallBack = &CallBack;

	PreOpenInitialize(lpFileName);
	if( lpCallBack == NULL ||
		lpCallBack->CIMAGE_ImageClose==NULL ||
		lpCallBack->CIMAGE_ImageOpen ==NULL ||
		lpCallBack->CIMAGE_ImageRead==NULL )
	{
		SetReturnCode_puma(IDS_ERR_IMAGE);
		rc = FALSE;
	}
	//
	// Запишем изображение
	//
	if(rc)
	{
		if(!CIMAGE_WriteCallbackImage((PWord8)PUMA_IMAGE_USER,*(CIMAGEIMAGECALLBACK*)lpCallBack))
		{
			SetReturnCode_puma(CIMAGE_GetReturnCode());
			rc = FALSE;
		}
		else
		{
			if(!CIMAGE_ReadDIB((PWord8)PUMA_IMAGE_USER,(Handle *)&gpInputDIB,1))
			{
				SetReturnCode_puma(CIMAGE_GetReturnCode());
				rc = FALSE;
			}
			//gpInputDIB = (PWord8)pDIB;
		}
	}
	if(rc)
		rc = PostOpenInitialize(lpFileName);
	return rc;
}//////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_XGetRotateDIB(void ** lpDIB, Point32 * p)
{
	Bool32 rc = TRUE;
	PWord8 lpImage = (PWord8)PUMA_IMAGE_USER;
	//
	// Определим угол поворота страницы
	//
	PAGEINFO    PInfo = {0};

	IS_VALID(p);
	IS_VALID(lpDIB);


	if(!CPAGE_GetPageData(hCPAGE,PT_PAGEINFO,(void*)&PInfo,sizeof(PInfo)))
	{
		SetReturnCode_puma(CPAGE_GetReturnCode());
		rc = FALSE;
	}
	else
	{
		CIMAGEBITMAPINFOHEADER info;
		if(PInfo.BitPerPixel>1)
			lpImage = (PWord8)PUMA_IMAGE_BINARIZE;

		if(CIMAGE_GetImageInfo(lpImage,&info))
        {
			if(PInfo.Incline2048>0)
			{
				p->x = (Int32)info.biWidth * PInfo.Incline2048 / 2048 * PInfo.Incline2048 / 2048;
				p->y = (Int32)(info.biWidth ) * PInfo.Incline2048 / 2048;
			}
			else
			{
				p->x = -(Int32)info.biHeight * PInfo.Incline2048 / 2048+(Int32)info.biWidth * PInfo.Incline2048 / 2048 * PInfo.Incline2048 / 2048;
				p->y = 0;
			}
        }
        else
        {
			SetReturnCode_puma(CIMAGE_GetReturnCode());
			rc = FALSE;
        }
	}
	//
	// Создадим довернутое изображение
	//
	if(rc)
	{
		PAGEINFO           PInfo = {0};
		GetPageInfo(hCPAGE,&PInfo);

		CIMAGE_DeleteImage((PWord8)PUMA_IMAGE_ROTATE	);

		CIMAGE_EnableMask(lpImage,(PWord8)"r",FALSE);
		if(!RIMAGE_Rotate(lpImage,
			(PWord8)PUMA_IMAGE_ROTATE,
			PInfo.Incline2048,2048, 0))
		{
			SetReturnCode_puma(RIMAGE_GetReturnCode());
			rc = FALSE;
		}

		if(rc)
		{
			if(!CIMAGE_ReadDIB((PWord8)PUMA_IMAGE_ROTATE,lpDIB,TRUE))
			{
				SetReturnCode_puma(CIMAGE_GetReturnCode());
				rc = FALSE;
			}
		}
		CIMAGE_EnableMask(lpImage,(PWord8)"r",TRUE);
		PInfo.Images|=IMAGE_ROTATE;
		SetPageInfo(hCPAGE,PInfo);
	}
	return rc;
}
//////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_XClose()
{
	Bool32 rc = TRUE;

	CLINE_Reset();
	ClearAll();
	// очистим
	CIMAGE_Reset();
	CPAGE_DeleteAll();
	RIMAGE_Reset();
	hCPAGE=NULL;

	gpRecogDIB = gpInputDIB = NULL;
	//memset(szInputFileName,0,sizeof(szInputFileName));

	return rc;
}
//////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_XPageAnalysis( )
{
	Bool32 rc = FALSE;

	ResetPRGTIME();
	if(InitPRGTIME())
		ProgressStart();

#ifdef _DEBUG
	_CrtMemState checkPt1;
	_CrtMemCheckpoint( &checkPt1 );
	_CrtMemDumpStatistics( &checkPt1 );
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "WARNING: Page Analysis.\n");
	//	_CrtMemDumpAllObjectsSince(NULL);
#endif

	rc = Layout(gpRecogDIB);
	if(DonePRGTIME())
		ProgressFinish();
	return rc;
}
//////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_XFinalRecognition( )
{
	Bool32 rc = TRUE;
	ResetPRGTIME();
	if(InitPRGTIME())
		ProgressStart();
	// Проверим: выделены ли фрагменты.
	if(!CPAGE_GetCountBlock(hCPAGE) || IsUpdate(FLG_UPDATE_CPAGE))
	{
		PRGTIME prev = StorePRGTIME(0,20);
		rc = Layout(gpRecogDIB);
		RestorePRGTIME( prev );
		if(rc)
		{
			PRGTIME prev = StorePRGTIME(20,100);
			rc = Recognize();
			RestorePRGTIME( prev );
		}
	}
	else
	{
		if(rc)
		{
			//                      PRGTIME prev = StorePRGTIME(20,100);
			rc = Recognize();
			//                      RestorePRGTIME( prev );
		}
	}
	if(DonePRGTIME())
		ProgressFinish();

	return rc;
}
//////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_XSave(const char * lpOutFileName, Int32 lnFormat, Int32 lnCode ) {
    char *fname = strdup(lpOutFileName);
    Bool32 res;
    winpath_to_internal(fname);
    res = PUMA_Save(ghEdPage, fname, lnFormat, lnCode,FALSE );
    free(fname);
    return res;
}
//////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_Save(Handle hEdPage, const char * lpOutFileName, Int32 lnFormat, Int32 lnCode, Bool32 bAppend )
{
	Bool32 rc = TRUE;
	Handle prevEdPage = ghEdPage;

	if(hEdPage == NULL)
		hEdPage = ghEdPage;

	ghEdPage = hEdPage;

	if(ghEdPage==NULL)
	{
		SetReturnCode_puma(IDS_ERR_PARAM);
		return FALSE;
	}

	if(InitPRGTIME())
		ProgressStart();
	if(LDPUMA_Skip(hDebugCancelFormatted))
	{
		switch(lnFormat)
		{
#ifdef _DEBUG
		case PUMA_DEBUG_TOTEXT:
			rc = SaveToText((char*)lpOutFileName,lnCode);
			break;
#endif
		case PUMA_TORTF:
			/*
			SetOptionsToFRMT();
			rc = RFRMT_SaveRtf((char*)lpOutFileName,lnCode);
			if(!rc)
				SetReturnCode_puma(RFRMT_GetReturnCode());
				*/
			if(bAppend)
				rc = CED_MergeFormattedRtf((char*)lpOutFileName, ghEdPage);
			else
			    rc = CED_WriteFormattedRtf((char*)lpOutFileName, ghEdPage);

			if(!rc)
				SetReturnCode_puma(CED_GetReturnCode());
			break;
		case PUMA_TOEDNATIVE:
			rc = CED_WriteFormattedEd((char*)lpOutFileName, ghEdPage);
			if(!rc)
				SetReturnCode_puma(CED_GetReturnCode());
			break;
		case PUMA_TOTEXT:
		case PUMA_TOSMARTTEXT:
		case PUMA_TOTABLETXT:
		case PUMA_TOTABLEDBF:
		case PUMA_TOHTML:
			rc = ConverROUT((char *)lpOutFileName, lnFormat, lnCode, bAppend);
			break;
		default:
			SetReturnCode_puma(IDS_ERR_NOTIMPLEMENT);
			rc = FALSE;
		}
	}
	LDPUMA_Skip(hDebugCancelFictive);
	if(DonePRGTIME())
		ProgressFinish();

	ghEdPage = prevEdPage;
	return rc;
}
/////////////////////////////////////////////////////////////////////////
PUMA_FUNC(Word32) PUMA_SaveToMemory(Handle hEdPage, Int32 lnFormat, Int32 lnCode, char * lpMem, Word32 size )
{
	Bool32 rc = 0;
	Handle prevEdPage = ghEdPage;

	if(hEdPage == NULL)
		hEdPage = ghEdPage;

	ghEdPage = hEdPage;

	if(ghEdPage==NULL)
	{
		SetReturnCode_puma(IDS_ERR_PARAM);
		return rc;
	}

	if(InitPRGTIME())
		ProgressStart();
	if(LDPUMA_Skip(hDebugCancelFormatted))
	{
		switch(lnFormat)
		{
		case PUMA_TOTEXT:
		case PUMA_TOSMARTTEXT:
		case PUMA_TOTABLETXT:
		case PUMA_TOTABLEDBF:
		case PUMA_TOHTML:
			rc = ConverROUTtoMemory(hEdPage, lnFormat, lnCode, (Byte*)lpMem, size);
			break;
		default:
			SetReturnCode_puma(IDS_ERR_NOTIMPLEMENT);
		}
	}
	LDPUMA_Skip(hDebugCancelFictive);
	if(DonePRGTIME())
		ProgressFinish();

	ghEdPage = prevEdPage;
	return rc;
}
//////////////////////////////////////////////////
PUMA_FUNC(Int32) PUMA_EnumLanguages(Int32 nPrev )
{
	return _EnumLanguage(nPrev);
}
//////////////////////////////////////////////////
PUMA_FUNC(Int32) PUMA_EnumFormats(Int32 nPrev )
{
	return _EnumFormats(nPrev);
}
//////////////////////////////////////////////////
PUMA_FUNC(Int32) PUMA_EnumCodes(Int32 format, Int32 nPrev )
{
	return _EnumCodes(format,nPrev);
}
//////////////////////////////////////////////////
PUMA_FUNC(Int32) PUMA_EnumFormatMode(Int32 nPrev )
{
	return _EnumFormatMode(nPrev);
}
//////////////////////////////////////////////////
PUMA_FUNC(Int32) PUMA_EnumTable(Int32 nPrev )
{
	return _EnumTable(nPrev);
}
//////////////////////////////////////////////////
PUMA_FUNC(Int32) PUMA_EnumPicture(Int32 nPrev )
{
	return _EnumPicture(nPrev);
}
////////////////////////////////////////////////////
PUMA_FUNC(void) PUMA_RenameImageName(char * name)
{
	if(name)
		strcpy(szInputFileName,name);
	else
		szInputFileName[0]='\0';
}
////////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_XGetTemplate(Rect32 *pRect)
{
	*pRect=gRectTemplate;
	return TRUE;
}
////////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_XSetTemplate(Rect32 rect)
{
	Rect32 old_rect = gRectTemplate;
	Bool32 rc = FALSE;
	CIMAGEBITMAPINFOHEADER info = {0};

	if(CIMAGE_GetImageInfo((PWord8)PUMA_IMAGE_USER,&info))
	{
		CIMAGE_Rect full = {0,0,info.biWidth,info.biHeight};
		PAGEINFO           PInfo = {0};

		GetPageInfo(hCPAGE,&PInfo);
//		PInfo.status &= ~(PINFO_USERTEMPLATE | PINFO_AUTOTEMPLATE);
		PInfo.status &= ~3;
		if(rect.left<0 && rect.right<0 &&
			rect.bottom<0 && rect.top<0)
		{
			rect.bottom = full.dwHeight;
			rect.left   = full.dwX;
			rect.right  = full.dwWidth;
			rect.top	= full.dwY;
		}

		if(old_rect.bottom == rect.bottom &&
			old_rect.left == rect.left &&
			old_rect.right== rect.right &&
			old_rect.top == rect.top)
		{
			PInfo.X = rect.left;
			PInfo.Y = rect.top;
			SetPageInfo(hCPAGE,PInfo);
			return TRUE;
		}
		if(CIMAGE_AddReadCloseRects((PWord8)PUMA_IMAGE_USER,1, &full))
		{
			if(rect.left>=0 && rect.top>=0 &&
				(rect.right-rect.left)<=info.biWidth &&
				(rect.bottom - rect.top)<=info.biHeight)

			{
				CIMAGE_Rect r = {rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top};
				rc = CIMAGE_RemoveReadCloseRects((PWord8)PUMA_IMAGE_USER,1, &r);
				PInfo.X = rect.left;
				PInfo.Y = rect.top;
			}
			else
			{
				CIMAGE_Rect r = {0,0,info.biWidth-1,info.biHeight-1};
				rc = CIMAGE_RemoveReadCloseRects((PWord8)PUMA_IMAGE_USER,1, &r);
				PInfo.X = 0;
				PInfo.Y = 0;
			}
			SetPageInfo(hCPAGE,PInfo);
		    SetUpdate(FLG_UPDATE,FLG_UPDATE_NO);
			gRectTemplate = rect;
		}
	}
	return rc;
}
///////////////////////////////
//OLEG fot Consistent
PUMA_FUNC(void) PUMA_GetSpecialBuffer(char * szResult,Int32 *nResultLength)
{
    extern char    global_buf[];
    extern Int32   global_buf_len;

    if( nResultLength )
        *nResultLength = global_buf_len;
    if( szResult )
    {
        if( global_buf_len )
            memcpy(szResult,global_buf,global_buf_len);
        else
            szResult[0]='\0';
    }
}

PUMA_FUNC(Bool32) PUMA_SetSpecialProject(Word8 nSpecialProject)
{

	gnSpecialProject = nSpecialProject;
	RSTUFF_RSSetSpecPrj(nSpecialProject);
	RSTR_SetSpecPrj(nSpecialProject);

	return TRUE;
}
