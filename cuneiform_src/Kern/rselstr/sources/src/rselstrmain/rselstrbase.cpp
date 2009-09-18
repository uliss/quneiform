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

#include <setjmp.h>

# include "dpuma.h"
# include "rselstr.h"
# include "resource.h"
# include "newfunc.h"
# include "kernel.h"
# include "pumadef.h"
# include "lhstr.h"

#include "compat_defs.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static uint16_t gwHeightRC = 0;
static uint32_t gwRC = 0;
static HANDLE ghStorage = NULL;
static HINSTANCE ghInst =  NULL;
Handle Root;
Handle VertCells=NULL;
Handle NegaCells=NULL;
Handle hShowCells=NULL;
Handle ShowNegaCells=NULL;
Handle ShowVertCells=NULL;
Handle hShowString;
Handle hDebugCancelPicturesAndTables = NULL;
Handle CutStrPrintFILE=NULL;
Handle hCutStr=NULL;
Handle hDebugCutStr=NULL;
Handle CutStrD=NULL;
Handle DifferenCutToHistory=NULL;
Handle hCutComp=NULL;
Handle CutCompPrintFILE=NULL;
Handle CutCompD=NULL;
Handle DifferenCutCompToHistory=NULL;
Handle MainDebug=NULL;
//Handle hVertCellsWork=NULL;
//Handle hNegaCellsWork=NULL;
Handle hTime=NULL;
//Handle hUseCLine=NULL;
Handle hStrPointedD;
//Handle hStrPointed;
Handle RselstrTime=NULL;
Handle hExit=NULL;
Handle MainTime=NULL;
Handle hDrawComp;


CLhstr* list=NULL;
CLPrepHstr* prelist=NULL;
Handle Root_CHSTR=NULL;
Handle PutToCHSTR;
Handle PutToCSTR;
Handle StrD;
Handle MainWindowD;
Handle CutStrV;
Handle StrPix;
Handle CutStrDel;
Handle SkewFromRLTABLE;
double cut_white=.90;
double cut_str=.85;
double inf_let_black=.2;
int inf_str_h=15;
int inf_str_w=8;

/////////////////////////////////////////


/////////////////////////////////////////
Bool APIENTRY DllMain( HINSTANCE  hModule,
uint32_t ul_reason_for_call,
                        pvoid lpReserved )
{
    switch( ul_reason_for_call )
	{
    case DLL_PROCESS_ATTACH:
		ghInst = hModule;
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(Bool32) RSELSTR_Init(uint16_t wHeightCode,HANDLE hStorage)
{
	gwHeightRC = wHeightCode;
	LDPUMA_Init(0,NULL);
/*------------------------------------------------------------------------------------------*/

//	CHSTR_Init(wHeightCode,NULL);

/*------------------------------------------------------------------------------------------*/

    LDPUMA_Registry(&MainDebug,SNAP_ROOT_MAIN_DEBUG,NULL);
    LDPUMA_Registry(&MainTime,SNAP_ROOT_MAIN_TIME_CONTROL,NULL);

    LDPUMA_Registry(&RselstrTime,"Выделение строк..",MainTime);
	LDPUMA_Registry(&hExit,"От RSELSTR до распознавания",MainTime);


//	LDPUMA_Registry(&hVertCellsWork,"Обработка вертикальных ячеек таблиц",MainDebug);
//    LDPUMA_RegistryHelp(hVertCellsWork,"Активизация вершины разрешит обработку вертикальных ячеек таблиц.",FALSE);

//	LDPUMA_Registry(&hNegaCellsWork,"Обработка негативных ячеек таблиц",MainDebug);
//    LDPUMA_RegistryHelp(hNegaCellsWork,"Активизация вершины разрешит обработку негативных ячеек таблиц.",FALSE);

	LDPUMA_Registry(&hDebugCutStr,"Резка слипшихся компонент повсеместно",MainDebug);
	LDPUMA_RegistryHelp(hDebugCutStr,"Разрешить резку слипшихся компонент повсеместно",FALSE);

//	LDPUMA_Registry(&hStrPointed,"Проставка флагов точечных линий на строки",MainDebug);
//    LDPUMA_RegistryHelp(hStrPointed,"Активизация вершины разрешит проставку флагов точечных линий на строки.",FALSE);


	LDPUMA_Registry(&Root,SNAP_ROOT_STRINGS,NULL);
    LDPUMA_RegVariable(Root,"Установить уровень отладки.",&layout,"unsigned char");
	LDPUMA_RegistryHelp(Root,"В переменных можно установить значение уровня \
отладки от 5 до 8. При продавление вершины строки будут выделяться модулем RBLOCK.",TRUE);


	LDPUMA_Registry(&hShowString,"Показать выделенные строки",Root);
	LDPUMA_RegistryHelp(hShowString,"Будут показаны позитивные строки с нормальной ориентацией в виде прямоугольников.",FALSE);

	LDPUMA_Registry(&hStrPointedD,"Показать строки с флагом пунктирности",Root);
	LDPUMA_RegistryHelp(hStrPointedD,"Будут показаны строки с флагом пунктирности.",FALSE);

	LDPUMA_Registry(&hShowCells,"Показать выделенные ячейки с компонентами!",Root);
	LDPUMA_RegistryHelp(hShowCells,"Будут подсвечиваться ячейка и входящие \
в нее компоненты.",FALSE);


	LDPUMA_Registry(&VertCells,"Работа с вертикальными ячейками таблиц",NULL);
	LDPUMA_RegistryHelp(VertCells,"Работа с вертикальными ячейками таблиц",FALSE);

	LDPUMA_Registry(&NegaCells,"Работа с негативными ячейками таблиц",NULL);
	LDPUMA_RegistryHelp(NegaCells,"Работа с негативными ячейками таблиц",FALSE);

	LDPUMA_Registry(&ShowVertCells,"Показать вертикальные ячейки таблиц",VertCells);
	LDPUMA_RegistryHelp(ShowVertCells,"Будут показаны ячейки таблиц с вертикальным расположением строк.",FALSE);

	LDPUMA_Registry(&ShowNegaCells,"Показать негативные ячейки таблиц",NegaCells);
	LDPUMA_RegistryHelp(ShowNegaCells,"Будут показаны негативные ячейки таблиц.",FALSE);

    LDPUMA_Registry(&hCutStr,"Резка слипшихся компонент повсеместно!",Root);
	LDPUMA_RegistryHelp(hCutStr,"Вершина резки слипшихся компонент повсеместно",FALSE);

    LDPUMA_Registry(&CutStrPrintFILE,"Печать в файл разрезанных компонент повсеместно",hCutStr);
	LDPUMA_RegistryHelp(CutStrPrintFILE,"Разрешить печать в файл cutstr.res разрезанных компонент повсеместно",FALSE);

    LDPUMA_Registry(&CutStrD,"Визуальный просмотр результатов резки",hCutStr);
	LDPUMA_RegistryHelp(CutStrD,"Разрешить визуальный просмотр результатов резки компонент повсеместнов",FALSE);

    LDPUMA_Registry(&DifferenCutToHistory,"Визуальный просмотр отличий",hCutStr);
	LDPUMA_RegistryHelp(DifferenCutToHistory,"Разрешить визуальный просмотр отличий от предыдущего варианта",FALSE);

    LDPUMA_Registry(&hCutComp,"Резка слипшихся компонент",Root);
	LDPUMA_RegistryHelp(hCutComp,"Запретить резку слипшихся компонент на пересечении линий таблицы",FALSE);


    LDPUMA_Registry(&CutCompPrintFILE,"Печать в файл разрезанных компонент",hCutComp);
	LDPUMA_RegistryHelp(CutCompPrintFILE,"Разрешить печать в файл cutcomp.res разрезанных компонент",FALSE);

    LDPUMA_Registry(&CutCompD,"Визуальный просмотр результатов резки!!",hCutComp);
	LDPUMA_RegistryHelp(CutCompD,"Разрешить визуальный просмотр результатов резки компонент",FALSE);

    LDPUMA_Registry(&DifferenCutCompToHistory,"Визуальный просмотр отличий!",hCutComp);
	LDPUMA_RegistryHelp(DifferenCutCompToHistory,"Разрешить визуальный просмотр отличий от предыдущего варианта",FALSE);

    LDPUMA_Registry(&hTime,"Контроль времени!!!",Root);
	LDPUMA_RegistryHelp(hTime,"Разрешить контроль времени",FALSE);

    LDPUMA_Registry(&hDrawComp,"Прорисовка компонент !!!",Root);
	LDPUMA_RegistryHelp(hDrawComp,"Прорисовка компонент.",FALSE);


//	LDPUMA_Registry (&hUseCLine,"Работа с контейнером линий",NULL);

 list=NULL;
 prelist=NULL;

// list=new CLhstr;
// prelist=new CLPrepHstr;

 LDPUMA_Registry (&Root_CHSTR,"Выделитель строк в нестандартных объектах", Root);
 const char *pText;

 /*  Корневая вершина  */
 pText = "<Ответственный тов. Степаненков> \
 \n Корневая вершина выделителя строк в нестандартных объектах.";
 LDPUMA_RegistryHelp (Root_CHSTR, pText, FALSE);

 /*  Положить в контейнер*/
 LDPUMA_Registry (&PutToCHSTR,"Положить в контейнер", Root_CHSTR);
 pText = "Положить объект в контейнер сложных строк";
 LDPUMA_RegistryHelp (PutToCHSTR, pText, FALSE);

 /*  Вершина резки на строки*/
 LDPUMA_Registry (&CutStrV,"Резка на строки",PutToCHSTR);
 pText = "Осуществляется резка объектов на строки";
 LDPUMA_RegistryHelp (CutStrV, pText, FALSE);
 LDPUMA_RegVariable (CutStrV,"cut_white",&cut_white,"double");
 LDPUMA_RegVariable (CutStrV,"cut_str",&cut_str,"double");
 LDPUMA_RegVariable (CutStrV,"inf_let_black",&inf_let_black,"double");
 LDPUMA_RegVariable (CutStrV,"inf_str_h",&inf_str_h,"int");
 LDPUMA_RegVariable (CutStrV,"inf_str_w",&inf_str_w,"int");

 /*  Положить в контейнер CSTR*/
 LDPUMA_Registry (&PutToCSTR,"Положить в контейнер CSTR", Root_CHSTR);
 pText = "Выгрузить строки из CHSTR в CSTR";
 LDPUMA_RegistryHelp (PutToCSTR, pText, FALSE);

 /*Вершина прорисовки положенных только что строк*/
 LDPUMA_Registry (&StrD,"Прорисовать строки", CutStrV);
 pText = "Прорисовать строки, которые кладутся в данный момент";
 LDPUMA_RegistryHelp (StrD, pText, FALSE);

 /*Вершина прорисовки строки пикселей при резке*/
 LDPUMA_Registry (&StrPix,"Прорисовать строку пикселей при резке", CutStrV);
 pText = "Прорисовать строку пикселей при резке";
 LDPUMA_RegistryHelp (StrPix, pText, FALSE);

 /*Вершина чистки при резке*/
 LDPUMA_Registry (&CutStrDel,"Чистка при резке", CutStrV);
 pText = "Удалять по возможности мусор при резке";
 LDPUMA_RegistryHelp (CutStrDel, pText, FALSE);


 return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(Bool32) RSELSTR_Done()
{

/*------------------------------------------------------------------------------------------*/

//	CHSTR_Done();

/*------------------------------------------------------------------------------------------*/
	delete list;
	delete prelist;

	LDPUMA_Done();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(uint32_t) RSELSTR_GetReturnCode()
{
	return gwRC;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(char *) RSELSTR_GetReturnString(uint32_t dwError)
{
return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(Bool32) RSELSTR_GetExportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

#define CASE_FUNCTION(a)	case RSELSTR_FN##a:	*(FN##a *)pData = a; break

	switch(dwType)
	{
	CASE_FUNCTION(RSELSTR_ExtractTextStrings);
	CASE_FUNCTION(RSELSTR_PutObjects);
	CASE_FUNCTION(RSELSTR_CutStr);
	CASE_FUNCTION(RSELSTR_RotateRaster);
	CASE_FUNCTION(RSELSTR_UnRotateRect);
	CASE_FUNCTION(RSELSTR_CleanStr);
	CASE_FUNCTION(RSELSTR_TestForVertical);
	CASE_FUNCTION(RSELSTR_CutCompInTableZones);

	default:
		*(Handle *)pData = NULL;
		rc = FALSE;
	}
return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(Bool32) RSELSTR_SetImportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwRC = 0;

#define CASE_DATA(a,b,c)	case a: c = *(b *)pData; break
#define CASE_PDATA(a,b,c)	case a: c = (b)pData; break

	switch(dwType)
	{
	 CASE_PDATA(RSELSTR_FNRSELSTR_ProgressStart,	FNRSELSTR_ProgressStart ,fnProgressStart_rsel);
	 CASE_PDATA(RSELSTR_FNRSELSTR_ProgressStep,	FNRSELSTR_ProgressStep,	fnProgressStep_rsel);
	 CASE_PDATA(RSELSTR_FNRSELSTR_ProgressFinish,	FNRSELSTR_ProgressFinish,fnProgressFinish_rsel);
	 default:
		SetReturnCode_rselstr(IDS_ERR_NOTIMPLEMENT);
		rc = FALSE;
	}

#undef CASE_DATA
#undef CASE_PDATA

return rc;

}

RSELSTR_FUNC(uint32_t) RSELSTR_SetReturnCode(uint32_t rc)
{
return (uint32_t)(0);
}

void SetReturnCode_rselstr(uint32_t rc)
{
uint16_t low = (uint16_t)(rc &  0xFFFF);
uint16_t hei = (uint16_t)(rc >> 16);

	if(hei)
		gwRC = rc;
	else
	{
		if(low - IDS_ERR_NO)
			gwRC = (uint32_t)(gwHeightRC<<16)|(low - IDS_ERR_NO);
		else
			gwRC = 0;
	}
}

uint32_t GetReturnCode_rselstr()
{
uint32_t rc = gwRC;
uint16_t low = (uint16_t)(gwRC &  0xFFFF);
uint16_t hei = (uint16_t)(gwRC >> 16);

	if(hei == gwHeightRC || hei == 0)
		rc = low + IDS_ERR_NO;

return rc;
}

void EnableDebug()
{

# ifdef LT_DEBUG
    switch (layout)
    {
        case 0:  LT_DebugGraphicsLevel = 0;  break;
        case 1:  LT_DebugGraphicsLevel = 1;  break;
        case 2:  LT_DebugGraphicsLevel = 2;  break;
        case 3:  LT_DebugGraphicsLevel = 3;  break;
        case 4:  LT_DebugGraphicsLevel = 4;  break;
        case 5:  LT_DebugGraphicsLevel = 1;  break;
        case 6:  LT_DebugGraphicsLevel = 1;  break;
        case 7:  LT_DebugGraphicsLevel = 1;  break;
        case 8:  LT_DebugGraphicsLevel = 1;  break;
        case 9:  LT_DebugGraphicsLevel = 0;  break;
        case 10: LT_DebugGraphicsLevel = 0;  break;
    }
# endif
# ifdef SE_DEBUG
    switch (layout)
    {
        case 0:  SE_DebugGraphicsLevel = 0;  break;
        case 1:  SE_DebugGraphicsLevel = 0;  break;
        case 2:  SE_DebugGraphicsLevel = 0;  break;
        case 3:  SE_DebugGraphicsLevel = 0;  break;
        case 4:  SE_DebugGraphicsLevel = 0;  break;
        case 5:  SE_DebugGraphicsLevel = 1;  break;
        case 6:  SE_DebugGraphicsLevel = 2;  break;
        case 7:  SE_DebugGraphicsLevel = 3;  break;
        case 8:  SE_DebugGraphicsLevel = 4;  break;
        case 9:  SE_DebugGraphicsLevel = 0;  break;
        case 10: SE_DebugGraphicsLevel = 0;  break;
    }
# endif
# ifdef MA_DEBUG
    switch (layout)
    {
        case 0:  MA_DebugLevel = 0;  break;
        case 1:  MA_DebugLevel = 0;  break;
        case 2:  MA_DebugLevel = 0;  break;
        case 3:  MA_DebugLevel = 0;  break;
        case 4:  MA_DebugLevel = 0;  break;
        case 5:  MA_DebugLevel = 0;  break;
        case 6:  MA_DebugLevel = 0;  break;
        case 7:  MA_DebugLevel = 0;  break;
        case 8:  MA_DebugLevel = 0;  break;
        case 9:  MA_DebugLevel = 1;  break;
        case 10: MA_DebugLevel = 2;  break;
    }
#endif
}

//end of file
