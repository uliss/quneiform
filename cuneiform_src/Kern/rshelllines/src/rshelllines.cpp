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

// RShellLines.cpp: implementation of the ShellLine's functions.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//
//	Description: ShellLine's functions implementation
//
//	Implemented: by B.M. Shahverdiev
//
//

#include "stdafx.h"
#include "resource.h"
#define __RSL__
#define __RSL_EXTERN__
#include "cttypes.h"
#include "linedefs.h"
#include "pumadef.h"
#include "cpage.h"
#include "ctiimage.h"
#include "dpuma.h"
#include "rshelllinescom.h"
#include "rshelllines.h"
#include "rsl.h"
#include "rverline.h"
#include "un_buff.h"
#include "cfio.h"

#define CRIME_CUT   50
#define RESULT      2
#define RSL_HANDLE 1
#define RSL_VERLINE CPAGE_GetInternalType("RVL_VERIFY")

#define     RSL_AboutLines_SizeMyBuff    492000
#define     RSL_AboutLines_SizeWorkMem   180000

//#define     RSL_ERR_NO_MEMORY "Ошибка выделения памяти системой"
#define RSL_ERR_NO_MEMORY 1
#define     MAF_GALL_GPTR              0x0010
char       cCommentBuffer[CFIO_MAX_COMMENT];

static char *Buffer = NULL;
static char *WorkMem = NULL;

static uint16_t            gwHeightRC = 0;
static uint16_t            gwLowRC = 0;
static HANDLE            ghStorage = NULL;
static HINSTANCE         ghInst =  NULL;
//DVP DLine*             tempLine;
HANDLE			  hSkipDeleteNoiseEvents = NULL;
HANDLE			  hPrintUnderlineTest = NULL;
HANDLE			  hSkipCheckUnderlining = NULL;
HANDLE			  hPrintUnderlines = NULL;
HANDLE			  hSkipCheckRaster = NULL;
HANDLE			  hPrintRasterCheck = NULL;
HANDLE			  hSkipCorrectPolynomia = NULL;
HANDLE			  hPrintCorrectPolynomia = NULL;
HANDLE			  hSkipDelFragLines = NULL;
HANDLE			  hPrintFragLines = NULL;
HANDLE			  hPreRSL_Root = NULL;
HANDLE			  hPreRSL_Control = NULL;
HANDLE			  hPreRSL_Debug = NULL;
HANDLE			  hDebugFragOwnerControl = NULL;


typedef Bool32 (*FNPUMA_XSetTemplate)(Rect32 rect);
typedef Bool32 (*FNPUMA_XGetTemplate)(Rect32 *pRect);
typedef struct tagRSPreProcessImage
{
    puchar	*pgpRecogDIB;
    Bool32	gbAutoRotate;
    Bool32  gbDotMatrix;
    Bool32  gbFax100;
    uint32_t  gnLanguage;
    uint32_t  gnTables;
    Handle	hCPAGE;
    Handle  hDebugCancelSearchPictures;
    Handle  hDebugCancelComponent;
    Handle  hDebugCancelTurn;
    Handle  hDebugCancelSearchLines;
    Handle  hDebugCancelVerifyLines;
    Handle  hDebugCancelSearchDotLines;
    Handle  hDebugCancelRemoveLines;
    Handle  hDebugCancelSearchTables;
    Handle  hDebugCancelAutoTemplate;
    Handle  hDebugEnableSearchSegment;
    char ** pglpRecogName;
    Handle* phCCOM;
    void *  pinfo;
    Handle* phLinesCCOM;
    void *  phCLINE;
    PBool32 pgneed_clean_line;
    int32_t *  pgnNumberTables;
    uint32_t  gnPictures;
    Bool32* pgrc_line;
    Rect32  gRectTemplate;
    FNPUMA_XSetTemplate fnXSetTemplate;
    FNPUMA_XGetTemplate fnXGetTemplate;
    char *szLayoutFileName;
} RSPreProcessImage, *PRSPreProcessImage;

Bool32 AboutLines (PRSPreProcessImage Image, Bool32 *BadScan, int32_t *ScanQual);

//  Bool APIENTRY DllMain( HANDLE hModule,
//                         uint32_t  ul_reason_for_call,
//                         pvoid lpReserved
//  				  )
//  {
//      switch (ul_reason_for_call)
//      {
//  	  case DLL_PROCESS_ATTACH:
//   	  case DLL_THREAD_ATTACH:
//        case DLL_THREAD_DETACH:
//        case DLL_PROCESS_DETACH:
//  	  break;
//      }
//      return TRUE;
//  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                     FindLineFrag
RSHELLLINES_FUNC(Bool32) FindLineFrag(CLINE_handle processedline, Bool32 OnlyPosyAndStat,
									Bool32 Is2ndPath, CLINE_handle hContainer,
									Bool32 IfNeedFragment, Bool32 IfStraightFrag)
{
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                     FindLineAttr
RSHELLLINES_FUNC(Bool32) FindLineAttr(CLINE_handle line, DLine* pCLine, Bool32 AbleSeeOldAttr)
{
    //SetWidth(line, pCLine);
    return TRUE;
}

///////////////////////////////////////////////////////////////
RSL_FUNC(Bool32) RSL_Init(uint16_t wHeightCode,HANDLE hStorage)
{
	LDPUMA_Init(0,NULL);
	LDPUMA_Registry(&hDebugRSL,SNAP_STUFF_RSL,NULL);
	LDPUMA_Registry(&hPreRSL_Root, "Предварительная обработка линий", hDebugRSL);
	LDPUMA_Registry(&hPreRSL_Debug, "Отладка работы (pre)", hPreRSL_Root);
	LDPUMA_Registry(&hPreRSL_Control, "Контроль работы (pre)", hPreRSL_Root);

	LDPUMA_Registry(&hSkipCheckUnderlining, "Выключить проверку подчерков", hPreRSL_Debug);
	LDPUMA_Registry(&hPrintUnderlineTest, "Вывод результатов проверки подчеркивания", hPreRSL_Control);
	LDPUMA_RegistryHelp(hPrintUnderlineTest, "В Console печатается количество черных точек (на 1000) в верхней части растра линии", FALSE);
	LDPUMA_Registry(&hPrintUnderlines, "Вывод координат подчерков", hPreRSL_Control);
	LDPUMA_RegistryHelp(hPrintUnderlines, "В Console печатаются координаты линий, определенных как подчерки", FALSE);

	LDPUMA_Registry(&hSkipCheckRaster, "Не проверять линии по растру", hPreRSL_Debug);
	LDPUMA_RegistryHelp(hSkipCheckRaster, "Если выключить проверку линий по растру, могут подтвердиться линии, выделенные по жирному тексту", FALSE);
	LDPUMA_Registry(&hPrintRasterCheck, "Вывод результатов проверки растра", hPreRSL_Control);
	LDPUMA_RegistryHelp(hPrintRasterCheck, "В Console печатается количество черных точек (на 1000) в растре линии", FALSE);

	LDPUMA_Registry(&hDebugFrag,"Поиск фрагментов линии",hDebugRSL);
    LDPUMA_Registry(&hDebugFragOwner,"Отладка поиска фрагментов линии",hDebugFrag);
    LDPUMA_Registry(&hDebugFragOwnerControl,"Контроль поиска фрагментов линии",hDebugFrag);

	LDPUMA_Registry(&hDebugDrowRectForRaster,"Рисуется прямоугольник линии",hDebugFragOwnerControl);
	LDPUMA_Registry(&hSkipCorrectPolynomia,"Не корректировать 4-хугольник линии",hDebugFragOwner);
	LDPUMA_Registry(&hPrintCorrectPolynomia,"Результат проверки 4-хугольника линии",hDebugFragOwnerControl);
	LDPUMA_RegistryHelp(hPrintCorrectPolynomia,"Вывод на консоль результатов проверки 4-хугольника линии",FALSE);
	LDPUMA_Registry(&hDebugDrowPolynomia,"Рисуется контур линии",hDebugFragOwnerControl);
	LDPUMA_Registry(&hDebugDrowCleanedRaster,"Рисуется прямоугольник линии после подчистки",hDebugFragOwnerControl);
	LDPUMA_Registry(&hDebugDrowGetCorrectedLineStripesIntervals,"Рисуется интервальное представление линии",hDebugFragOwnerControl);

	LDPUMA_Registry(&hDebugDrowVerticalLineRaster,"Рисуется растр вертикальной линии",hDebugFragOwnerControl);
	LDPUMA_Registry(&hDebugDrowVerticalLineStripesIntervals,"Рисуется интервальное представление вертикальной линии",hDebugFragOwnerControl);
	LDPUMA_Registry(&hDebugDrowVerticalLineCutPoints,"Рисуется точки пересечения вертикальной линии",hDebugFragOwnerControl);
	LDPUMA_Registry(&hDebugTypeIntervals,"Вывод на Consol координат интервалов линии",hDebugFragOwnerControl);
	LDPUMA_Registry(&hDebugTypeStripe,"Вывод на Consol интервальное представление линии",hDebugFragOwnerControl);
	LDPUMA_Registry(&hSkipDeleteNoiseEvents, "Не выполнять удаление шумовых фрагментов", hDebugFragOwner);
	LDPUMA_RegistryHelp(hSkipDeleteNoiseEvents, "Не удалять шумовые фрагменты", FALSE);
	LDPUMA_Registry(&hSkipDelFragLines, "Не удалять сильно фрагментированную линии", hDebugFragOwner);
	LDPUMA_RegistryHelp(hSkipDelFragLines, "Не удалять линии с числом фрагментов больше 5", FALSE);
	LDPUMA_Registry(&hPrintFragLines, "Печать результатов проверки фрагментации", hDebugFragOwnerControl);
	LDPUMA_RegistryHelp(hPrintFragLines, "Вывод на Consol результатов проверки кандидатов на удаление", FALSE);
	LDPUMA_Registry(&hDebugDrawLineFragmentsOwnWindow,"Рисуются фрагменты линии в отдельном окне",hDebugFragOwnerControl);
	LDPUMA_Registry(&hDebugDroweparationPoints,"Рисуются точки деления линии в отдельном окне",hDebugFragOwnerControl);
    LDPUMA_Registry(&hDebugDrawLineFragments,"Рисуются фрагменты линии в основном окне",hDebugFragOwnerControl);
    LDPUMA_RegistryHelp(hDebugDrawLineFragments, "Фрагменты рисуются для каждой линии в отдельности", FALSE);

	LDPUMA_Registry(&hDebugAttr,"Поиск атрибутов линий",hDebugRSL);
    LDPUMA_Registry(&hDebugAttrOwner,"Контроль поиска атрибутов линии",hDebugAttr);
	LDPUMA_Registry(&hDebugTypeWidth,"Вывод на Consol ширины линии",hDebugAttrOwner);
	LDPUMA_Registry(&hDebugTypeDegree,"Вывод на Consol угла наклона линии",hDebugAttrOwner);

    gwHeightRC = wHeightCode;

    return RESULT;
}

void SetReturnCode_rshelllines(uint16_t rc)
{
    gwLowRC = rc;
}

//////////////////////////////////////////////////////////////////////////////////
//
RSL_FUNC(Bool32) RSL_Done()
{
 	LDPUMA_Done();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//
RSL_FUNC(uint32_t) RSL_GetReturnCode()
{
    uint32_t rc = 0;
    if((gwLowRC - IDS_ERR_NO)>0)
        rc = (uint32_t)(gwHeightRC<<16)|(gwLowRC - IDS_ERR_NO);

    return rc;
}

//////////////////////////////////////////////////////////////////////////////////
//
RSL_FUNC(char *) RSL_GetReturnString(uint32_t dwError)
{
	uint16_t rc = (uint16_t)(dwError & 0xFFFF) + IDS_ERR_NO;
	static char szBuffer[512];

	if( dwError >> 16 != gwHeightRC)
		gwLowRC = IDS_ERR_NOTIMPLEMENT;

	if( rc >= IDS_ERR_NO )
		LoadString(ghInst,rc,(char *)szBuffer,sizeof(szBuffer));
	else
		return NULL;

	return szBuffer;
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//
RSL_FUNC(Bool32) RSL_GetExportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

return rc;
}

//////////////////////////////////////////////////////////////////////////////////
//
RSL_FUNC(Bool32) RSL_SetImportData(uint32_t dwType, void * pData)
{
	Bool32 rc = RESULT;

    if (dwType != RSL_HANDLE)
        return FALSE;

    Handle* phCPage = (Handle*) pData;

    RSPreProcessImage IImage;
    PRSPreProcessImage Image = &IImage;

    uint32_t nTeor = sizeof (RSPreProcessImage);
    Handle hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    Handle VerifyN = CPAGE_GetBlockFirst (*phCPage, RSL_VERLINE);//hPage, RSL_VERLINE);
    uint32_t nReal = CPAGE_GetBlockData (*phCPage, VerifyN, RSL_VERLINE, Image, nTeor);
//     uint32_t err32 = CPAGE_GetReturnCode ();
//     if (err32)
//         return FALSE;

    if( *Image->pgrc_line )
    {
        if(LDPUMA_Skip(Image->hDebugCancelVerifyLines))
        {
            Regime_VerifyLines val = Image->gnTables ? RVL_FutuTablCorr:RVL_Default;

            if( !RVERLINE_SetImportData(RVERLINE_DTRVERLINE_RegimeOfVerifyLines,&val)||
                !RVERLINE_MarkLines(*Image->phCCOM, Image->hCPAGE))
            {
                SetReturnCode_rshelllines((uint16_t)RVERLINE_GetReturnCode());
                rc = FALSE;
            }
            else
            {
                Bool32 BadScan = FALSE;
                int32_t  ScanQual= 0;
                AboutLines(Image, &BadScan, &ScanQual);
            }

            if(!*Image->pgneed_clean_line)
                LDPUMA_Console("Warning: RSL said that the lines don't need to be erased from the picture.\n");
        }
        else
            LDPUMA_Console("Missing stage of the evaluation lines.\n");
    }

    //CPAGE_DeleteBlock(&phCPage, VerifyN);

    return rc;
}

//////////////////////////////////////////////////////////////////////////////////
//
void *	RSLAlloc(uint32_t stAllocateBlock)
{
    char * mem = NULL;

#ifdef _NO_CFIO

#ifdef  RSL_USE_GLOBAL_MEM

    mem = (char *)GlobalAlloc(GPTR, stAllocateBlock);

#else

    mem = ::new char[stAllocateBlock];
    memset(mem, 0, stAllocateBlock );

#endif

    if(!mem)
        SetReturnCode_rshelllines((uint16_t)RSL_ERR_NO_MEMORY);
#else

    mem = (char *)CFIO_DAllocMemory(stAllocateBlock,MAF_GALL_GPTR,(char*)"RSL", (char*)cCommentBuffer);

    if(!mem)
        SetReturnCode_rshelllines((uint16_t)RSL_ERR_NO_MEMORY);

#endif

    return mem;
}

void	RSLFree(void * mem)
{
#ifdef _NO_CFIO

#ifdef  RSL_USE_GLOBAL_MEM

    GlobalFree(mem);

#else

    ::delete []	mem;

#endif
#else

    CFIO_FreeMemory(mem);

#endif
}

Bool32 AboutLines (PRSPreProcessImage Image, Bool32 *BadScan, int32_t *ScanQual)
{
	int SizeMain, SizeWork;

	UN_BUFF MainBuff = {0};

	void *vMain;
	char *cWork;
	Bool32 bRc = TRUE;

	//////////////////////////////////////////////////////////////////////////////////
	Buffer  = (char *)RSLAlloc(RSL_AboutLines_SizeMyBuff);
	WorkMem = (char *)RSLAlloc(RSL_AboutLines_SizeWorkMem);

	if (Buffer == NULL || WorkMem == NULL )
	{
		SetReturnCode_rshelllines((uint16_t)RSL_ERR_NO_MEMORY);
		bRc =  FALSE;
	}

	if ( bRc )
		do
		{
			//////////////////////////////////////////////////////////////////////////////////////
			/*  1. Контроль.  */
			if ((Image->pgneed_clean_line==NULL)&&(BadScan!=NULL))
				break;
				//return TRUE;

			/*  2. Инициализация.  */
			vMain = Buffer;
			SizeMain = RSL_AboutLines_SizeMyBuff;
			MainBuff.vBuff    = vMain;
			MainBuff.SizeBuff = SizeMain;
			MainBuff.vCurr    = MainBuff.vBuff;
			MainBuff.SizeCurr = MainBuff.SizeBuff;
			cWork = WorkMem;
			SizeWork = RSL_AboutLines_SizeWorkMem;


			if (Image->pgneed_clean_line!=NULL)
			{
				*Image->pgneed_clean_line = FALSE;
				CLINE_handle hCLINE=*((CLINE_handle*)(Image->phCLINE));
				Bool fl_break=FALSE;
				for(CLINE_handle hline=CLINE_GetFirstLine(hCLINE);hline;hline=CLINE_GetNextLine(hline))
				{
					CPDLine cpdata=CLINE_GetLineData(hline);
					if(!cpdata)
						continue;
					if(cpdata->Flags&LI_IsTrue)
					{
						*Image->pgneed_clean_line=TRUE;
						fl_break=TRUE;
					}
					if(fl_break)
						break;
				}
				if (1)
				{
					if (*Image->pgneed_clean_line)
						LDPUMA_ConsoleN ("RSource: Нужно снять линии.");
					else
						LDPUMA_ConsoleN ("RSource: Не надо снимать линии!");
				}
			}
//		   }

			if (BadScan!=NULL)
			{
				if (1)
					LDPUMA_ConsoleN ("RSource: Качество сканирования : не умею пока определять.");
				*BadScan = TRUE;
				*ScanQual = 100;
			}
		} while ( false );

	RSLFree(Buffer);
	RSLFree(WorkMem);

	return bRc;
}

//////////////////////////////////////////////////////////////////////////////////
//
uint16_t GetReturnCode_rshelllines()
{
	return gwLowRC;
}

#define two0         1
#define two1         2
#define two2         4
#define two3         8
#define two4         16
#define two5         32
#define two6         64
#define two7         128
#define two8         256
#define two9         512
#define two10        1024
#define two11        2048
#define two12        4096
#define two13        8192
#define two14        16384
#define two15        32768
#define two16        65536
#define two17        131072
#define two18        262144
#define two19        524288
#define two20        1048576
#define two21        2097152
#define two22        4194304
#define two23        8388608
#define two24        16777216
#define two25        33554432
#define two26        67108864
#define two27        134217728
#define two28        268435456
#define two29        536870912
#define two30        1073741824
#define two31        2147483648

struct FictInterval
{
	int32_t Level;
	int32_t Pos;
	int32_t End;
	int32_t RelIndex;
};

static int mycompare( const void *elem1, const void *elem2 )
{

  FictInterval *a = (FictInterval*) elem1;
  FictInterval *b = (FictInterval*) elem2;

   if(a->Level == b->Level)
   {
	   return a->Pos - b->Pos;
   }
   return a->Level - b->Level;
}

// struct TieComp
// {
// 	int32_t LeftBorder;
// 	int32_t RightBorder;
// 	int32_t Weight;
// 	Bool32 IsNoiseComp;
// 	int32_t VoteResult;
// };

void   DeleteNoiseEvents(CLINE_handle hLine, DLine* pLine)
{
	return;
}

void CheckUnderlining(CLINE_handle hLine, DLine* pLine, char* pSourceRaster)
{
	return;
}

Bool32 CompareRasterParts(CPDLine pLine, char* pSourceRaster, Bool32 CheckSerif)
{
    return TRUE;
}

RSHELLLINES_FUNC( Bool32) RSL_CorrectDoubleLines(CLINE_handle hLine1, CLINE_handle hLine2)
{
	return TRUE;
}

RSHELLLINES_FUNC( Bool32)     RSL_SplitLine(CLINE_handle hLine, CLINE_handle hContainer)
{
	return TRUE;
}

int SL_IsInPoly(Point32* a, CLINE_SL_POLY* p)
{
    return 0;
}

Bool SL_IsPointInAB(Point32 *P,Point32 *A,Point32 *B)
{
    return FALSE;
}

void CalculateRectForRaster(DLine* pCLine, PAGEINFO* page_info)
{
}

void CalculatePolynomia(DLine* pCLine, PAGEINFO* page_info)
{
}

void DrowRectForRaster(Rect32* rect)
{
}

void DrowPolynomia(DLine* pCLine)
{
}

void   DrowVerticalLineStripesIntervals(CLINE_handle line, Handle HndMyWindow)
{
}

void   CleaningRaster(DLine* pCLine, char* Buffer)
{
}

void   DrowCleanedRaster(DLine* pCLine, char* Buffer, CIMAGEBITMAPINFOHEADER* image_info, Handle* HndMyWindow)
{
}

void   DrowVerticalLineRaster(DLine* pCLine, char* Buffer, CIMAGEBITMAPINFOHEADER* image_info, Handle* HndMyWindow)
{
}

void   Transpose_bit_matrixes(pchar buf_in,pchar buf_out, Rect32* rect)
{
}

Bool GetLineStripesIntervals(CLINE_handle line, DLine* pCLine, pchar  pRaster, Bool FlagVerticalLine)
{
    return TRUE;
}

void   FillingStripes(CLINE_handle line, int y, int16_t Count, uint16_t* pIntervals, Bool FlagVerticalLine, DLine* pCLine)
{
}

void   CheckDotLines(DLine* pCLine, Bool FlagVerticalLine)
{
}

void   TypeIntervals(int y, int16_t Count, uint16_t* pIntervals)
{
}

void   TypeStripe(CLINE_handle hStripe, DEvent* pStripe)
{
}

Bool   GetLineFragments(CLINE_handle line, DLine* pCLine)
{
    return TRUE;
}

Handle DrawLineFragments(CLINE_handle line, CPDLine pCLine, Handle HndMyWindow, int ColorIndex, Bool32 ForAllLines)// = FALSE)
{
    return NULL;
}

RSHELLLINES_FUNC( void) DrawFriendLines(CLINE_handle hContainer, GLM* friendlinesmass)
{
}

void DrawGroupOfExtensibleLines(CLINE_handle hContainer, GLM* hGroup, int CountLines)
{
}

RSHELLLINES_FUNC( void) DrawLosedVerticalLines(GLM* friendlinesmass, int CountLines)
{
}

RSHELLLINES_FUNC( void) DrowAllLines(CLINE_handle hContainer, Handle hDrowAllLines)
{
}

RSHELLLINES_FUNC( void) DrawBigComps(CLINE_handle hContainer)
{
}

RSHELLLINES_FUNC( void) DrawFragsForAllLines(CLINE_handle hContainer, Handle hDebugDrawAllLineFragments)
{
}

RSHELLLINES_FUNC( void) InitLine(DLine* linedata)
{
}

RSHELLLINES_FUNC( void) FindDotLines(Handle hCCOM,Handle hCPAGE, CLINE_handle hContainer)
{
}

RSHELLLINES_FUNC( Bool32) CheckSeparationPoints(CLINE_handle hLine, CLINE_handle hComp)
{
    return FALSE;
}

RSHELLLINES_FUNC( Bool) SL_GetRaster(Rect32* rect, uchar** ppData, PAGEINFO* page_info)
{
    return TRUE;
}

RSHELLLINES_FUNC( void) SetLinesAndCompsRelationship(CLINE_handle hContainer, CLINE_handle hFictContainer)
{
}

RSHELLLINES_FUNC(void) FindFriendLines(CLINE_handle hContainer, GLM* friendlinesmass)
{
}

RSHELLLINES_FUNC(void) FindGroupOfExtensibleLines(CLINE_handle hContainer, GLM* friendlinesmass, Bool32 IfDrawResult)
{
}

RSHELLLINES_FUNC(int32_t) RSL_VerifyShortLine(CPDLine pLine, Handle hCCOM, PAGEINFO* page_info, uchar lang, uchar debug_flags, int32_t *cross_point)
{
    return 0;
}

RSHELLLINES_FUNC(Bool32) GetNextPartOfLine(CLINE_handle hContainer, CLINE_handle hLine)
{
    return FALSE;
}

Bool32 SetExtLines(CLINE_handle hExtContainer, CLINE_handle hContainer, CLINE_handle* hLinesMass,
                   int32_t CountLines)
{
    return FALSE;
}

int32_t CountBlackRaster(CPDLine pLine, CPDLine pLineExt, Bool32 IsHor, int32_t delta, Handle hDrawRaster)
{
    return 0;
}

