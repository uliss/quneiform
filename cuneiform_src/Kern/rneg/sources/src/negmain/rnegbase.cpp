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

#include "rneg.h"
#include "rnegbase.h"
#include "compat_defs.h"

double inf_neg_black=.65;
double sup_neg_black_del=.7;
double weak_neg_black_del=.1;

Handle NegRoot;
Handle NegSearch;
Handle NegRec;
Handle NegSearchSize;
Handle NegSize;
Handle NegNorm;
Handle NegCut;
Handle NegCutStr;
Handle NegColor;
Handle NegSearchSizeD;
Handle NegSizeD;
Handle NegNormD;
Handle NegCutD;
Handle NegCutStrD;
Handle NegColorD;
Handle NegRecD;
Handle NegResD;
Handle NegSearchSizeDC;
Handle NegSizeDC;
Handle NegNormDC;
Handle NegCutDC;
Handle NegCutStrDC;
Handle NegColorDC;
Handle NegRecDC;
Handle MainWindowD;
Handle RecOneLetter;
Handle NegPrintFILE;
Handle TestCHSTR;
Handle PutToCHSTR;
Handle PrintResConsole;
Handle SetSkew;
Handle PutToCPage;
Handle hTime;
Handle hShowNegComps;
int TOP;
int BOTTOM;
int LEFT;
int RIGHT;
int VERTICALE;
int NEGATIVE;
int Skew=0;
double inf_prob=.6;
int NegRecTYPEE=0;


Bool APIENTRY DllMain( HANDLE hModule,
uint32_t ul_reason_for_call,
                        pvoid lpReserved )
{
    switch( ul_reason_for_call )
	{
    case DLL_PROCESS_ATTACH:
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

/*----------------------------------------------------------------------------*/

RNEG_FUNC(Bool32) RNEG_Init(uint16_t wHeightCode,Handle parent)
{
 LDPUMA_Init(0,NULL);


 LDPUMA_Registry (&NegRoot,"Поиск и распознование негативов", parent);
 const char *pText;
 /*  Корневая вершина  */
 pText = "<Ответственный тов. Степаненков> \
 \n Корневая вершина отладки поиска и распознавания негативов.";
 LDPUMA_RegistryHelp (NegRoot, pText, FALSE);
 LDPUMA_RegVariable (NegRoot,"inf_neg_black",&inf_neg_black,"double");
 LDPUMA_RegVariable (NegRoot,"sup_neg_black_del",&sup_neg_black_del,"double");
 LDPUMA_RegVariable (NegRoot,"weak_neg_black_del",&weak_neg_black_del,"double");

 /*Вершина задания угла наклона*/
 LDPUMA_Registry (&SetSkew,"Задать угол наклона", NegRoot);
 pText = "Задать угол наклона страницы";
 LDPUMA_RegistryHelp (SetSkew, pText, FALSE);
 LDPUMA_RegVariable (SetSkew,"Skew",&Skew,"int");

 /*Вершина поиска*/
 LDPUMA_Registry (&NegSearch,"Поиск кандидатов в негативы", NegRoot);
 pText = "Поиск кандидатов в негативы";
 LDPUMA_RegistryHelp (NegSearch, pText, FALSE);

 /*Вершина pacпознования*/
 LDPUMA_Registry (&NegRec,"Распознавание отловленных негативов", NegRoot);
 pText = "Распознавание отловленных негативов";
 LDPUMA_RegistryHelp (NegRec, pText, FALSE);
 LDPUMA_RegVariable (NegRec,"NegRecTYPEE",&NegRecTYPEE,"int");


 /*Вершина поиска по размерам*/
 LDPUMA_Registry (&NegSearchSize,"Поиск по размерам", NegSearch);
 pText = "Поиск кандидатов в негативы по размерам";
 LDPUMA_RegistryHelp (NegSearchSize, pText, FALSE);

 /*Вершина отсева по размерам*/
 LDPUMA_Registry (&NegSize,"Отсев по размерам", NegSearch);
 pText = "Отсев кандидатов в негативы по размерам";
 LDPUMA_RegistryHelp (NegSize, pText, FALSE);

 /*Вершина нормализации найденных кандидатов*/
 LDPUMA_Registry (&NegNorm,"Нормализация", NegSearch);
 pText = "Нормализация найденных кандидатов в негативы";
 LDPUMA_RegistryHelp (NegNorm, pText, FALSE);

 /*Вершина резки найденных кандидатов*/
 LDPUMA_Registry (&NegCut,"Резка", NegSearch);
 pText = "Резка найденных кандидатов в негативы";
 LDPUMA_RegistryHelp (NegCut, pText, FALSE);

 /*Вершина резки многострочных*/
 LDPUMA_Registry (&NegCutStr,"Резка многострочных", NegSearch);
 pText = "Резка найденных многострочных кандидатов в негативы";
 LDPUMA_RegistryHelp (NegCutStr, pText, FALSE);

 /*Вершина отсева по цветовой гамме*/
 LDPUMA_Registry (&NegColor,"Отсев по цветовой гамме", NegSearch);
 pText = "Отсев по цветовой гамме найденных кандидатов в негативы";
 LDPUMA_RegistryHelp (NegColor, pText, FALSE);

 /*Вершина прорисовки найденных кандидатов в негативы по размерам*/
 LDPUMA_Registry (&NegSearchSizeD,"Прорисовка по размерам", NegSearchSize);
 pText = "Прорисовка найденных кандидатов в негативы по размерам";
 LDPUMA_RegistryHelp (NegSearchSizeD, pText, FALSE);

 /*Надо ли подчищать ?*/
 LDPUMA_Registry (&NegSearchSizeDC,"Не чистить!!!", NegSearchSizeD);
 pText = "Специально для ГРЯЗНУЛЬ";
 LDPUMA_RegistryHelp (NegSearchSizeDC, pText, FALSE);

 /*Вершина прорисовки отсеянных кандидатов в негативы по размерам*/
 LDPUMA_Registry (&NegSizeD,"Окончательная прорисовка по размерам", NegSize);
 pText = "Прорисовка отсеянных кандидатов в негативы по размерам";
 LDPUMA_RegistryHelp (NegSizeD, pText, FALSE);

/*Надо ли подчищать ?*/
 LDPUMA_Registry (&NegSizeDC,"Не чистить!!!!", NegSizeD);
 pText = "Специально для ГРЯЗНУЛЬ";
 LDPUMA_RegistryHelp (NegSizeDC, pText, FALSE);

 /*Вершина прорисовки нормализованных кандидатов в негативы*/
 LDPUMA_Registry (&NegNormD,"Прорисовка нормализованных", NegNorm);
 pText = "Прорисовка нормализованных кандидатов в негативы";
 LDPUMA_RegistryHelp (NegNormD, pText, FALSE);

 /*Надо ли подчищать ?*/
 LDPUMA_Registry (&NegNormDC,"Не подтирать!!!", NegNormD);
 pText = "Специально для ГРЯЗНУЛЬ";
 LDPUMA_RegistryHelp (NegNormDC, pText, FALSE);

 /*Вершина прорисовки разрезанных кандидатов в негативы*/
 LDPUMA_Registry (&NegCutD,"Прорисовка разрезанных", NegCut);
 pText = "Прорисовка разрезанных кандидатов в негативы";
 LDPUMA_RegistryHelp (NegCutD, pText, FALSE);

 /*Надо ли подчищать ?*/
 LDPUMA_Registry (&NegCutDC,"Не подтирать!!!!", NegCutD);
 pText = "Специально для ГРЯЗНУЛЬ";
 LDPUMA_RegistryHelp (NegCutDC, pText, FALSE);

 /*Вершина прорисовки разрезанных многострочных кандидатов в негативы*/
 LDPUMA_Registry (&NegCutStrD,"Прорисовка разрезанных многострочных", NegCutStr);
 pText = "Прорисовка разрезанных многострочных кандидатов в негативы";
 LDPUMA_RegistryHelp (NegCutStrD, pText, FALSE);

 /*Надо ли подчищать ?*/
 LDPUMA_Registry (&NegCutStrDC,"Не подтирать!!!!!", NegCutStrD);
 pText = "Специально для ГРЯЗНУЛЬ";
 LDPUMA_RegistryHelp (NegCutStrDC, pText, FALSE);

 /*Вершина прорисовки после отсева по цветовой гамме*/
 LDPUMA_Registry (&NegColorD,"Прорисовка по цветовой гамме", NegColor);
 pText = "Прорисовка кандидатов в негативы после отсева по цветовой гамме";
 LDPUMA_RegistryHelp (NegColorD, pText, FALSE);

 /*Надо ли подчищать ?*/
 LDPUMA_Registry (&NegColorDC,"Не подчищать!!!", NegColorD);
 pText = "Специально для ГРЯЗНУЛЬ";
 LDPUMA_RegistryHelp (NegColorDC, pText, FALSE);

 /*Вершина прорисовки компонент в негативе*/
 LDPUMA_Registry (&NegRecD,"Прорисовка компонент", NegRec);
 pText = "Прорисовка компонент в негативе";
 LDPUMA_RegistryHelp (NegRecD, pText, FALSE);

 /*Надо ли подчищать ?*/
 LDPUMA_Registry (&NegRecDC,"Не стирать!!!", NegRecD);
 pText = "Специально для ГРЯЗНУЛЬ";
 LDPUMA_RegistryHelp (NegRecDC, pText, FALSE);

 /*Вершина прорисовки результата*/
 LDPUMA_Registry (&NegResD,"Визуальный просмотр результата", NegRoot);
 pText = "Визуальный просмотр результата";
 LDPUMA_RegistryHelp (NegResD, pText, FALSE);
 PrintResConsole=NegResD;

 /*Показ компонент в вертикаьном негативе в различных направлениях*/
 LDPUMA_Registry (&hShowNegComps, "Просмотр компонент в отдельном окне", NegResD);
 LDPUMA_RegistryHelp (hShowNegComps, "Каждая компонента в вертикальном негативе показывается для двух напрвлений: снизу-вверх и сверху-вниз", FALSE);

 /*Вершина печати в файл результата*/
 LDPUMA_Registry (&NegPrintFILE,"Печать в файл результатов", NegRoot);
 pText = "Печать в файл recneg.res результатов";
 LDPUMA_RegistryHelp (NegPrintFILE, pText, FALSE);

/*Вершина печати в консоль результата*/
/*
 LDPUMA_Registry (&PrintResConsole,"Печать в консоль результатов", NegRoot);
 pText = "Печать в консоль результатов распознавания и вероятностных оценок";
 LDPUMA_RegistryHelp (PrintResConsole, pText, FALSE);
*/
 /*Использовать функцию RecogOneLetter*/
 LDPUMA_Registry (&RecOneLetter,"Использовать функцию RecogOneLetter", NegRec);
 pText = "Использовать функцию RecogOneLetter";
 LDPUMA_RegistryHelp (RecOneLetter, pText, FALSE);

 /*Тест контейнера CHSTR*/
 LDPUMA_Registry (&TestCHSTR,"Тест контейнера CHSTR", NegRoot);
 pText = "Положить в CHSTR объект с параметрами, заданными в данной вершине";
 LDPUMA_RegistryHelp (TestCHSTR, pText, FALSE);
 LDPUMA_RegVariable (TestCHSTR,"TOP",&TOP,"int");
 LDPUMA_RegVariable (TestCHSTR,"BOTTOM",&BOTTOM,"int");
 LDPUMA_RegVariable (TestCHSTR,"LEFT",&LEFT,"int");
 LDPUMA_RegVariable (TestCHSTR,"RIGHT",&RIGHT,"int");
 LDPUMA_RegVariable (TestCHSTR,"VERTICALE",&VERTICALE,"int");
 LDPUMA_RegVariable (TestCHSTR,"NEGATIVE",&NEGATIVE,"int");

 /*Тест контейнера CHSTR*/
 LDPUMA_Registry (&PutToCHSTR,"Складывать результаты в CHSTR", NegRoot);
 pText = "Складывать приемлимые результаты в CHSTR";
 LDPUMA_RegistryHelp (PutToCHSTR, pText, FALSE);

 /*Складывать результаты в CPage*/
 LDPUMA_Registry (&PutToCPage,"Складывать результаты в CPage", NegRoot);
 pText = "Складывать приемлимые результаты в CPage";
 LDPUMA_RegistryHelp (PutToCPage, pText, FALSE);
 LDPUMA_RegVariable (PutToCPage,"inf_prob",&inf_prob,"double");

 LDPUMA_Registry (&hTime,"Контроль времени!!!!!!", NegRoot);
 pText = "Контроль времени!";
 LDPUMA_RegistryHelp (hTime, pText, FALSE);


 return TRUE;
}

/*---------------------------------------------------------------------------*/
RNEG_FUNC(Bool32) RNEG_Done()
{
	LDPUMA_Done();
	return TRUE;
}
/*---------------------------------------------------------------------------*/
RNEG_FUNC(uint32_t) RNEG_GetReturnCode()
{
	return (uint32_t)(0);
}
/*---------------------------------------------------------------------------*/
RNEG_FUNC(char *) RNEG_GetReturnString(uint32_t dwError)
{
	return NULL;
}
/*---------------------------------------------------------------------------*/
RNEG_FUNC(Bool32) RNEG_GetExportData(uint32_t dwType, void * pData)
{


#define CASE_FUNCTION(a)	case RNEG_FN##a:	*(FN##a *)pData = a; break

	switch(dwType)
	{
		CASE_FUNCTION(RNEG_RecogNeg);
		CASE_FUNCTION(RNEG_TestForNegative);

		default:
			*(Handle *)pData = NULL;
			return FALSE;
	}
	return TRUE;
}

/*---------------------------------------------------------------------------*/
RNEG_FUNC(Bool32) RNEG_SetImportData(uint32_t dwType, void *pData)
{

	return FALSE;
}
/*----------------------------------------------------------------------------*/
