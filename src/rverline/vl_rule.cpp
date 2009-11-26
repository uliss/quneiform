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

/**********  Заголовок  **********/
/*  Автор      :  Александр Михайлов                                          */
/*  Редакция   :  17.12.99                                                    */
/*  Файл       :  'AM_RULE.CPP'                                              */
/*  Содержание :  Функции внутренннего управления работой библиотеки.         */
/*  Назначение :  Сервис по управлению библиотекой.                           */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
/*  interface my        */
#include "am_comm.h"
#include "vl_rule.h"
/*------------own objects-----------------------------------------------------*/
/*  Верификация линий  */
static Handle VL_Into;
static Handle VL_I_PrimLines; //+
static Handle VL_I_PrimBoxes; //+
static Handle VL_Out;
static Handle VL_O_VerifLines;//+
static Handle VL_O_CorrLines; //+
static Handle VL_O_ModelLines;
static Handle VL_O_SaveComps; //+
static Handle VL_Control;
static Handle VL_C_ContErr;
static Handle VL_C_ContWarn;
static Handle VL_C_ContRare;
static Handle VL_Upravlenie;
static Handle VL_U_NoPointedLines;
static Handle VL_U_AbleKillShortVert;
static Handle VL_U_NewFormalVerify;
static Handle VL_U_NoInvestLongLines;
static Handle VL_U_NoInvestImage;
static Handle VL_U_AbleHoriZher;
static Handle VL_Debug;
static Handle VL_D_Info;
static Handle VL_D_InvestImage;
static Handle VL_D_WrResLine;
//Handle hUseCLine;
/*----------------------------------------------------------------------------*/
void AM_MakeTreeRules_VerifLines (Handle parent)
{
	const char *pText;
/*  Корневая вершина  */
pText = " \
\n Корневая вершина первой половины [верификация линий] библиотеки 'RLTABLE'. \
\n Эта часть реализует две содержательные функции : \
\n   (I) верификацию выделенных библиотекой 'RLINE' линий \
\n  (II) моделирование 'пула спасительных даных' для каждого случая, когда \
\n       линия была выделена с фатальными ошибками \
\n Обе функции одинаково важны. \
\n Корневая вершина отладки имеет пять узлов: \
\n a) 'Входные данные' \
\n б) 'Результаты работы' \
\n в) 'Контроль работы' \
\n г) 'Управление' \
\n д) 'Отладка' \
\n Узлы а) б) и в) предназначены для службы тестирования. Они обеспечивают \
визуализацию в главном окне полной информации о входных и выходных данных \
библиотеки, а также печать на консоль подробной диагностики ошибок, \
неточностей и редких ситуаций при исполнении программы. \
\n Узел г) предназначен для управления алгоритмами библиотеки. \
\n Узел д) предназначен для разработчика библиотеки. \
\n Almi 26.05.99";
AM_RegiHelp (parent, pText, TRUE);
	/*  Первый узел  */
	AM_RegiVert (&VL_Into, "Входные данные /ВЛ/", parent);
	pText = " \
\n Входными данными для этой части библиотеки 'RLTABLE' являются : \
\n - линии \
\n - коробки компонент \
\n Almi 26.05.99";
	AM_RegiHelp (VL_Into, pText, FALSE);
		AM_RegiVert (&VL_I_PrimLines, "Линии /ВЛ/", VL_Into);
		AM_RegiHelp (VL_I_PrimLines, "Линии", FALSE);
		AM_RegiVert (&VL_I_PrimBoxes, "Коробки компонент /ВЛ/", VL_Into);
		AM_RegiHelp (VL_I_PrimBoxes, "Коробки компонент", FALSE);
	/*  Второй узел  */
	AM_RegiVert (&VL_Out, "Результаты работы /ВЛ/", parent);
	pText = " \
\n Результатами работы этой части библиотеки 'RLTABLE' являются : \
\n - выставленные флаги верификации линий \
\n - скорректированные линии \
\n - заново смоделированные линии \
\n - спасенные компоненты \
\n Almi 29.05.99";
	AM_RegiHelp (VL_Out, pText, FALSE);
		AM_RegiVert (&VL_O_VerifLines, "Верификация линий", VL_Out);
		pText = "Верификация линий : \
\n Синие - реальные, красные - ложные, желтые - сомнительные или неисследованные, \
\n фиолетовые - краевые захваты компонент, коричневые - составные";
		AM_RegiHelp (VL_O_VerifLines, pText, FALSE);
		AM_RegiVert (&VL_O_CorrLines, "Скорректированные линии", VL_Out);
		pText = "Скорректированные линии : \
\n Это - внесенные в пул линий общего пользования указания о истинном \
\n расположении линий, выделенных с краевыми захватами компонент.";
		AM_RegiHelp (VL_O_CorrLines, pText, FALSE);
		AM_RegiVert (&VL_O_ModelLines, "(н/р)Смоделированные линии", VL_Out);
		pText = "Смоделированные линии : \
\n Это - пул альтернативных данных, предназначенный  взамен составных линий \
\n для разбора страницы.";
		AM_RegiHelp (VL_O_ModelLines, pText, FALSE);
		AM_RegiVert (&VL_O_SaveComps, "Жертвы линий (спасенные компоненты)", VL_Out);
		pText = "Жертвы линий (спасенные компоненты) : \
\n Это - пул спасенных компонент, которые неизбежно пострадают от чистки \
\n некорректно выделенных линий. Он предназначен для использования в \
\n формировании и распознавании строк.";
		AM_RegiHelp (VL_O_SaveComps, pText, FALSE);
	/*  Третий узел  */
	AM_RegiVert (&VL_Control, "Контроль работы /ВЛ/", parent);
	AM_RegiHelp (VL_Control, "Контроль работы", FALSE);
		AM_RegiVert (&VL_C_ContErr , "Контроль ошибок /ВЛ/", VL_Control);
		AM_RegiHelp (VL_C_ContErr  , "Контроль ошибок", FALSE);
		AM_RegiVert (&VL_C_ContWarn, "Контроль неточностей /ВЛ/", VL_Control);
		AM_RegiHelp (VL_C_ContWarn , "Контроль неточностей", FALSE);
		AM_RegiVert (&VL_C_ContRare, "Контроль редких ситуаций /ВЛ/", VL_Control);
		AM_RegiHelp (VL_C_ContRare , "Контроль редких ситуаций", FALSE);
	/*  Четвертый узел  */
	AM_RegiVert (&VL_Upravlenie, "Управление алгоритмами /ВЛ/", parent);
	AM_RegiHelp (VL_Upravlenie, "Управление алгоритмами", FALSE);
		AM_RegiVert (&VL_U_NoPointedLines , "Не искать отточия", VL_Upravlenie);
		AM_RegiHelp (VL_U_NoPointedLines  , "Не искать отточия", FALSE);
		AM_RegiVert (&VL_U_AbleKillShortVert , "Можно опровергать короткие вертикальные", VL_Upravlenie);
		AM_RegiHelp (VL_U_AbleKillShortVert  , "Можно опровергать короткие вертикальные", FALSE);
		AM_RegiVert (&VL_U_NewFormalVerify , "Новая формальная верификация", VL_Upravlenie);
		AM_RegiHelp (VL_U_NewFormalVerify  , "Новая формальная верификация", FALSE);
		AM_RegiVert (&VL_U_NoInvestLongLines, "Отменить поиск линий с захватами", VL_Upravlenie);
		AM_RegiHelp (VL_U_NoInvestLongLines , "Отменить поиск линий с захватами", FALSE);
		AM_RegiVert (&VL_U_NoInvestImage, "Отменить проверку линий с анализом растра", VL_Upravlenie);
		AM_RegiHelp (VL_U_NoInvestImage , "Отменить проверку линий с анализом растра", FALSE);
		AM_RegiVert (&VL_U_AbleHoriZher, "Искать жертвы горизонтальных линий", VL_Upravlenie);
		AM_RegiHelp (VL_U_AbleHoriZher , "Искать жертвы горизонтальных линий", FALSE);
	/*  Пятый узел  */
	AM_RegiVert (&VL_Debug, "Отладка /ВЛ/", parent);
	AM_RegiHelp (VL_Debug, "Узел разработчика", FALSE);
		AM_RegiVert (&VL_D_Info , "Общая информация", VL_Debug);
		AM_RegiHelp (VL_D_Info  , "Общая информация", FALSE);
		AM_RegiVert (&VL_D_InvestImage , "Проверка по тифу", VL_Debug);
		AM_RegiHelp (VL_D_InvestImage  , "Проверка по тифу", FALSE);
		AM_RegiVert (&VL_D_WrResLine , "Отписывание результатов поиска линий", VL_Debug);
		AM_RegiHelp (VL_D_WrResLine  , "Отписывание результатов поиска линий", FALSE);
//	    AM_RegiVert(&hUseCLine,"Работа с контейнером линий",NULL);
}
/*----------------------------------------------------------------------------*/
Handle AM_GetKeyOfRule (VL_RULES Code)
{
	switch (Code)
	{
		/*  Верификация линий  */
		case RU_VL_Into :
			return VL_Into;
		case RU_VL_I_PrimLines :
			return VL_I_PrimLines;
		case RU_VL_I_PrimBoxes :
			return VL_I_PrimBoxes;
		case RU_VL_Out :
			return VL_Out;
		case RU_VL_O_VerifLines :
			return VL_O_VerifLines;
		case RU_VL_O_CorrLines :
			return VL_O_CorrLines;
		case RU_VL_O_ModelLines :
			return VL_O_ModelLines;
		case RU_VL_O_SaveComps :
			return VL_O_SaveComps;
		case RU_VL_Control :
			return VL_Control;
		case RU_VL_C_ContErr :
			return VL_C_ContErr;
		case RU_VL_C_ContWarn :
			return VL_C_ContWarn;
		case RU_VL_C_ContRare :
			return VL_C_ContRare;
		case RU_VL_Upravlenie :
			return VL_Upravlenie;
		case RU_VL_U_NoPointedLines :
			return VL_U_NoPointedLines;
		case RU_VL_U_AbleKillShortVert :
			return VL_U_AbleKillShortVert;
		case RU_VL_U_NewFormalVerify :
			return VL_U_NewFormalVerify;
		case RU_VL_U_NoInvestLongLines :
			return VL_U_NoInvestLongLines;
		case RU_VL_U_AbleHoriZher :
			return VL_U_AbleHoriZher;
		case RU_VL_Debug :
			return VL_Debug;
		case RU_VL_D_Info :
			return VL_D_Info;
		case RU_VL_D_InvestImage :
			return VL_D_InvestImage;
		case RU_VL_D_WrResLine :
			return VL_D_WrResLine;
		/*******************/
		case RU_Unknown :
		default :
			return NULL;
	}
}
/*----------------------------------------------------------------------------*/
