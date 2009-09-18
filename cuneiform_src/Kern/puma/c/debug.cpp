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

#include "mpuma.h"
#include "dpuma.h"
#include "pumadef.h"
////////////////////////
void InitDebug()
{
	Handle hDebugCancel = NULL;
	Handle hDebugOk = NULL;

        LDPUMA_Init(0,NULL);

LDPUMA_RegistryHelp(hDebugRoot,
"<Ответственный: П.Хлебутин> В переменных этой вершины \
можно посмотреть и изменить текущие значения параметров распознавания и отладки. "
                ,NULL);
        LDPUMA_RegVariable(NULL,"Язык распознавания.",&gnLanguage,"unsigned");
        LDPUMA_RegVariable(NULL,"Одна колонка.",&gbOneColumn,"unsigned");
        LDPUMA_RegVariable(NULL,"Словарный контроль.",&gbSpeller,"unsigned");
        LDPUMA_RegVariable(NULL,"Факс.",&gbFax100,"unsigned");
        LDPUMA_RegVariable(NULL,"Матричный принтер.",&gbDotMatrix,"unsigned");
        LDPUMA_RegVariable(NULL,"Выделять жирность в RTF.",&gbBold,"unsigned");
        LDPUMA_RegVariable(NULL,"Выделять курсив в RTF.",&gbItalic,"unsigned");
        LDPUMA_RegVariable(NULL,"Выделять размер в RTF.",&gbSize,"unsigned");
        LDPUMA_RegVariable(NULL,"Форматировать в RTF.",&gnFormat,"unsigned");
        LDPUMA_RegVariable(NULL,"Флаг выделения картинок.",&gnPictures,"unsigned");
        LDPUMA_RegVariable(NULL,"Флаг выделения таблиц.",&gnTables,"unsigned");
        LDPUMA_RegVariable(NULL,"Атоматический поворот.",&gbAutoRotate,"unsigned");
        LDPUMA_RegVariable(NULL,"Вызвать отладчик при получении кода возврата:",&nDebugReturnCode,"unsigned");
#ifdef _DEBUG
        LDPUMA_RegVariable(NULL,"Вызвать отладчик при аллокации блока памяти:",&nDebugAllocMemory,"unsigned");
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////
// общая отладочная информация
        LDPUMA_Registry(&hDebugRoot,SNAP_ROOT_, NULL);
        LDPUMA_RegistryHelp(hDebugRoot,
"<Ответственный: П.Хлебутин> В основных настройках можно отключать или включать в работу основные этапы работы программы. \
Работа программы зависит как от этих настроек, так и от параметров распознавания \
установленных пользователем при запуске программы.(Эти настройки можно увидеть и изменить в переменных \
корневой вершины )."
                ,FALSE);

//Главная отладка
 LDPUMA_Registry(&hDebugMainDebug,SNAP_ROOT_MAIN_DEBUG, NULL);
 LDPUMA_RegistryHelp(hDebugMainDebug,"При продавлении какой-либо из деток этой вершины будет использоваться соответствующий ей не используемый в текущей версии алгоритм.",FALSE);
//Главное этапирование
 LDPUMA_Registry(&hDebugMainTimeControl,SNAP_ROOT_MAIN_TIME_CONTROL, NULL);
 LDPUMA_RegistryHelp(hDebugMainTimeControl,"Временной контроль по основным этапам программы",FALSE);

        LDPUMA_Registry(&hDebugCancel,SNAP_SELECT_DISABLE, hDebugRoot);
        LDPUMA_RegistryHelp(hDebugCancel,
                "Здесь перечислены вершины, которые отменяют выполнение этапов при отладке."
                ,FALSE);
		//
		// ВНИМАНИЕ ! Все этапы расположены в порядке их работы !
		//

        LDPUMA_Registry(&hDebugCancelBinarize,          "Этап бинаризации.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelBinarize,
                "Этап бинаризации не однобитный изображений."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelComponent,         "Этап выделения компонент.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelComponent,
                "Этап выделения компонент на черно-белом изображении."
                ,FALSE);
		LDPUMA_Registry(&hDebugCancelTurn,				"Этап поворота изображения.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelTurn,
                "Этап поворота исходного изображения в случае установки опции автоповорота и его необходимости."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelSearchLines,       "Этап поиска линий.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelSearchLines,
                "Этап поиска линий на черно-белом изображении."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelOrtoMove,   "Этап ортогонализующего сдвига.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelOrtoMove,
                "Этап ортогонализующего сдвига."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelSearchDotLines,       "Этап поиска точечных линий.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelSearchDotLines,
                "Этап выделения точечных линий по компонентам."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelVerifyLines,       "Этап оценки линий.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelVerifyLines,
                "Этап пометки ранее выделенных линий для последующего удаления."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelRemoveLines,       "Этап удаления линий.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelRemoveLines,
                "Этап удаления ранее выделенных и помеченных для удаления линий. "
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelComponentSecond,	"Этап выделения компонент после удаления линий.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelComponentSecond,
                "Этап выделения компонент после удаления линий. "
                ,FALSE);
	    LDPUMA_Registry(&hDebugCancelSearchPictures,    "Этап поиска картинок.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelSearchPictures,
                "Этап поиска картинок по ранее выделенным компонентам."
                ,FALSE);
	    LDPUMA_Registry(&hDebugCancelSearchNegatives,    "Этап поиска негативов.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelSearchNegatives,
                "Этап поиска негативов."
                ,FALSE);

	    LDPUMA_Registry(&hDebugCancelLinePass3,    "Этап третьего прохода по линиям.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelLinePass3,
                "Этап третьего прохода по линиям."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelSearchTables,      "Этап поиска таблиц.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelSearchTables,
                "Этап поиска таблиц по ранее выделенным линиям."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelExtractBlocks,		"Этап автоматического Layout(а).", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelExtractBlocks,
                "Этап автоматического Layout(а). "
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelVertCells,		"Этап автоматической вертикальной диагностики ячеек таблиц.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelVertCells,
                "Этап автоматической вертикальной диагностики ячеек таблиц."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelStrings,           "Этап выделения строк.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelStrings,
                "Этап выделения строк. \
Этап выделения строк связан с последующими этапами работы программы. Его отмена приведет к остановке программы на этом этапе."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelRecognition,       "Этап распознавания строк.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelRecognition,
                "Этап распознавания строк связан с последующими этапами работы программы. Его отмена приведет к остановке программы на этом этапе."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelStringsPass2,      "Этап распознавания строк вторым проходом.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelStringsPass2,
                "Этап распознавания строк вторым проходом."
                ,FALSE);
	    LDPUMA_Registry(&hDebugCancelPostSpeller,        "Этап дораспознавания по словарю.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelPostSpeller,
                "Этап дораспознавания по словарю."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelPostRecognition,    "Этап обработки строк после распознавания.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelPostRecognition,
                "Этап обработки строк после распознавания. В нее входит определение кегля."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelConsoleOutputText,	 "Этап отладочной печати результатов.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelConsoleOutputText,
                "Этап отладочной печати результатов распознавания строк в консоль."
                ,FALSE);
			LDPUMA_Registry(&hDebugCancelPropertyConsole,  "Отменить RTF-печать результатов в консоль.", hDebugCancelConsoleOutputText);
			LDPUMA_RegistryHelp(hDebugCancelPropertyConsole,
					"Выделите эту вершину, если вы хотите, чтобы результаты отписывались простым текстом высотой 12 пунктов."
					,FALSE);
			LDPUMA_Registry(&hDebugCancelConsoleKeglOutput,"Печатать размер символа.", hDebugCancelConsoleOutputText);
			LDPUMA_RegistryHelp(hDebugCancelConsoleKeglOutput,"Выделите эту верщину, если хотите посмотреть размер символа. Размер будеи выделен зеленым цветом.",FALSE);
        LDPUMA_Registry(&hDebugCancelFormatted,         "Этап форматирования.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelFormatted,
                "Этап форматирования."
                ,FALSE);
        LDPUMA_Registry(&hDebugCancelFictive,         "Фиктивный этап.", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelFictive,
                "Фиктивный этап необходим для корректной работы профайлера."
                ,FALSE);

        LDPUMA_Registry(&hDebugOk,SNAP_SELECT_ENABLE, hDebugRoot);
        LDPUMA_RegistryHelp(hDebugOk,
                "Здесь перечислены вершины, которые разрешают выполнение этапов при отладке."
                ,FALSE);


        LDPUMA_Registry(&hDebugCancelAutoTemplate,   "Этап ограничения области распознавания.", hDebugOk);
        LDPUMA_RegistryHelp(hDebugCancelAutoTemplate,
                "Этап автоматического ограничения области распознавания."
                ,FALSE);
        LDPUMA_Registry(&hDebugHandLayout,   "Этап ручной разметки (Layout).", hDebugOk);
        LDPUMA_RegistryHelp(hDebugHandLayout,
"Разметка изображения будет производится через OLE контрол <Layout.ocx>. \
Для успешной работы этого этапа необходимо скопировать и зарегестрировать этот модуль системной командой \
REGSVR32 Layout.ocx"
                ,FALSE);
			LDPUMA_Registry(&hDebugLayoutToFile,  "Записать  LAYOUT в файл.", hDebugHandLayout);
			LDPUMA_RegistryHelp(hDebugLayoutToFile,
					"Активизация этой вершины позволяет сохранить текущий Layout в файле. \
	Имя файла хранится в переменных этой вершины. Имя файла можно изменить в птекущем сеансе работы."
					,FALSE);
			LDPUMA_RegVariable(hDebugLayoutToFile,"Имя файла для сохр./вост. LAYOUT",szLayoutFileName,"char *");

			LDPUMA_Registry(&hDebugLayoutFromFile,"Прочитать LAYOUT из файла.", hDebugHandLayout);
			LDPUMA_RegistryHelp(hDebugLayoutFromFile,
					"Активизация этой вершины позволяет прочитать текущий Layout из файла. \
	Имя файла хранится в переменных этой вершины. Имя файла можно изменить в птекущем сеансе работы."
					,FALSE);
			LDPUMA_RegVariable(hDebugLayoutFromFile,"Имя файла для сохр./вост. LAYOUT",szLayoutFileName,"char *");

        LDPUMA_Registry(&hDebugPrintBlocksCPAGE,"Этап отладочной печати контейнера CPAGE.", hDebugOk);
        LDPUMA_RegistryHelp(hDebugPrintBlocksCPAGE,
                "Этап отладочной печати содержимого контейнера описания страницы CPAGE . Результаты печатаются после этапа Layout"
                ,FALSE);
	    LDPUMA_Registry(&hDebugCancelRanalFrm,"Этап предраспознавание таблиц", hDebugCancel);
        LDPUMA_RegistryHelp(hDebugCancelRanalFrm,
                "Этап анализа и предраспознавания таблиц."
                ,FALSE);
		LDPUMA_Registry(&hDebugEnableSaveJtl,  "Этап записи JTL файла.", hDebugOk);
        LDPUMA_RegistryHelp(hDebugEnableSaveJtl,
                "Этап записи JTL файла."
                ,FALSE);

        LDPUMA_Registry(&hDebugEnableSaveCstr1,  "Этап сохранения CSTR before pass1.", hDebugOk);
        LDPUMA_RegistryHelp(hDebugEnableSaveCstr1,
                "Этап сохранения строк перед проходом 1."
                ,FALSE);
        LDPUMA_Registry(&hDebugEnableSaveCstr2,  "Этап сохранения CSTR before pass2.", hDebugOk);
        LDPUMA_RegistryHelp(hDebugEnableSaveCstr2,
                "Этап сохранения строк перед проходом 2."
                ,FALSE);
        LDPUMA_Registry(&hDebugEnableSaveCstr3,  "Этап сохранения CSTR before pass3.", hDebugOk);
        LDPUMA_RegistryHelp(hDebugEnableSaveCstr3,
                "Этап сохранения строк перед проходом 3."
                ,FALSE);
        LDPUMA_Registry(&hDebugEnableSaveCstr4,  "Этап сохранения финального CSTR.", hDebugOk);
        LDPUMA_RegistryHelp(hDebugEnableSaveCstr4,
                "Этап сохранения финальных строк."
                ,FALSE);

			LDPUMA_Registry(&hDebugEnablePrintFormatted,         "Этап отладочной печати результатов форматирования.", hDebugOk);
			LDPUMA_RegistryHelp(hDebugEnablePrintFormatted,
					"Этап отладочной печати результатов форматирования. Если этот этап выделен, тогда, в отладочной версии, печатаются два временных файла __tmp__.fed и __tmp__.rtf"
					,FALSE);

		LDPUMA_Registry(&hDebugEnableSearchSegment,    "Этап грубого поиска фрагментов.", hDebugOk);
        LDPUMA_RegistryHelp(hDebugEnableSearchSegment,
                "Этап грубого поиска фрагментов."
                ,FALSE);
		// Контэйнер CPAGE
        LDPUMA_Registry(&hDebugCPAGEStorage,SNAP_CPAGE_STORAGE, hDebugRoot);
/////////////////////////////////////////////////////////////////////////////////////////////////////
// обработка сырья
        LDPUMA_Registry(&hDebugRootStuff,SNAP_ROOT_STUFF,NULL);
		LDPUMA_RegistryHelp(hDebugRootStuff,
				"Бинаризация, выделение, снятие и верификация линий, выделение картинок"
				, FALSE);
		// бинаризация
        LDPUMA_Registry(&hDebugBinarize,SNAP_STUFF_BINARIZE,hDebugRootStuff/*NULL*/);
        /*
        LDPUMA_RegistryHelp(hDebugBinarize,
                "<Ответственный: Н.Бузикашвили> Этап цветновой обработки изображения. \
При успешной цветновой обработке исходное изображение записывается в окно "NAME_IMAGE_INPUT" \
Бинаризованное - в окно Main."
                ,FALSE);*/
        LDPUMA_RegistryHelp(hDebugBinarize,
                "<Ответственный: А.Коноплёв> Этап бинаризации цветных и серых изображений \"по Кронроду\". \
При успешной цветновой обработке исходное изображение записывается в окно "NAME_IMAGE_INPUT" \
Бинаризованное - в окно Main."
                ,FALSE);
		//автотемплейт
        LDPUMA_Registry(&hDebugAutoTemplate,SNAP_STUFF_AUTOTEMPLATE,hDebugRootStuff/*NULL*/);
        LDPUMA_RegistryHelp(hDebugAutoTemplate,SNAP_STUFF_AUTOTEMPLATE,FALSE);
		//выделение и снятие линий
        LDPUMA_Registry(&hDebugLines,SNAP_STUFF_LINES , hDebugRootStuff/*NULL*/ );
        LDPUMA_RegistryHelp(hDebugLines,
                "<Ответственный: Р.Воропаев> Этап выделения и снятия линий."
                ,FALSE);

          // alik оболочка над линиями
        LDPUMA_Registry(&hDebugRSL,SNAP_STUFF_RSL , hDebugLines );
        LDPUMA_RegistryHelp(hDebugRSL,
                "<Ответственный: Алик> Оболочка над линиями."
                ,FALSE);

		// верификация линий
        LDPUMA_Registry(&hDebugLines,SNAP_STUFF_VER_LINES , hDebugRootStuff );
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Этапы разметки
        LDPUMA_Registry(&hDebugRootLayout,SNAP_ROOT_LAYOUT, NULL);
		LDPUMA_RegistryHelp(hDebugRootLayout,
				"Выделение таблиц, фрагментов, строк, доработка линий"
				, FALSE);
		// поиск картинок
        LDPUMA_Registry(&hDebugPictures,SNAP_LAYOUT_PICTURES , hDebugRootLayout );
        // поиск негативов
        LDPUMA_Registry(&hDebugNeg,SNAP_LAYOUT_NEG , hDebugRootLayout );
		// грубый поиск фрагментов
        LDPUMA_Registry(&hDebugSegment,SNAP_LAYOUT_SEGMENT , hDebugRootLayout );
        // поиск фона
        LDPUMA_Registry(&hDebugFon,SNAP_LAYOUT_FON , hDebugRootLayout );

		// выделение таблиц
        LDPUMA_Registry(&hDebugTables,SNAP_LAYOUT_TABLES, hDebugRootLayout);
        LDPUMA_RegistryHelp(hDebugTables,
                "<Ответственный: A.Михайлов> Этап выделения таблиц. Выделение таблиц по линиям возможно \
только после выполнения этапа выделения линий."
                ,FALSE);
		//Работа с вертикальными фрагментами
		Handle hVertCells;
		Handle hNegaCells;
	LDPUMA_Registry(&hVertCells,"Работа с вертикальными ячейками таблиц",hDebugRootLayout);
	LDPUMA_Registry(&hNegaCells,"Работа с негативными ячейками таблиц",hDebugRootLayout);

		// выделение фрагментов
        LDPUMA_Registry(&hDebugLayout,SNAP_LAYOUT_LAYOUT, hDebugRootLayout);
        LDPUMA_RegistryHelp(hDebugLayout,
                "<Ответственный: Р.Воропаев, П.Хлебутин> Этап автоматического Layout(а). \
Значения Level указывает на уровень детализации показа. Чем выше уровень, тем подробнее. \
Оптимальное значенние - 3. В переменных указаны текущие значения отладки."
                ,FALSE);
		// выделение строк
        LDPUMA_Registry(&hDebugStrings,SNAP_LAYOUT_STRINGS, hDebugRootLayout);
        LDPUMA_RegistryHelp(hDebugStrings,
                "<Ответственный: Р.Воропаев, П.Хлебутин> Этап выделения строк. \
Вторая часть автоматического Layout(а). Выделение строк возможно только после этапа \
выделения компонент."
                ,FALSE);
		//доработка линий
        LDPUMA_Registry(&hDebugPostLines,SNAP_LAYOUT_POST_LINES, hDebugRootLayout);
		LDPUMA_RegistryHelp(hDebugRootLayout,
				"Короткие вертикальные линии"
				, FALSE);
		// короткие вертикальные линии
        LDPUMA_Registry(&hDebugSVLines,SNAP_LAYOUT_SHOR_LINES, hDebugPostLines);
        LDPUMA_RegistryHelp(hDebugSVLines,
                "<Ответственный: A.Коноплев> Поиск и удаление коротких вертикалтных линий."
                ,FALSE);
        LDPUMA_RegVariable(hDebugSVLines,"Убить найденышей",&gKillVSLComponents,"unsigned");
        LDPUMA_Registry(&hDebugSVLinesStep,SNAP_SHOR_LINES_STEP, hDebugSVLines);
        LDPUMA_RegistryHelp(hDebugSVLinesStep,
                "Пошаговый проход."
                ,FALSE);
        LDPUMA_Registry(&hDebugSVLinesData,SNAP_SHOR_LINES_DATA, hDebugSVLines);
        LDPUMA_RegistryHelp(hDebugSVLinesData,
                "Сама по себе не функционирует"
                ,FALSE);
/////////////////////////////////////////////////////////////////////////////////////////////////////
// распознование
        LDPUMA_Registry(&hDebugRootRecognition,SNAP_ROOT_RECOGNITION, NULL);
        LDPUMA_RegistryHelp(hDebugRootRecognition,
                "Распознование строк, словарь"
                ,FALSE);
		// распознавание строк
        LDPUMA_Registry(&hDebugRecognition,SNAP_RECG_RECOGNITION, hDebugRootRecognition/*NULL*/);
        LDPUMA_RegistryHelp(hDebugRecognition,
                "<Ответственный: О.Славин> Этап распознавания строк. Номер строки останова \
указывать тут."
                ,FALSE);
		// словарь
        LDPUMA_Registry(&hDebugSpell,SNAP_RECG_SPELL, hDebugRootRecognition/*NULL*/);
/////////////////////////////////////////////////////////////////////////////////////////////////////
// форматирование
        LDPUMA_Registry(&hDebugRootFormatting,SNAP_ROOT_CONVERTERS, NULL);
        LDPUMA_RegistryHelp(hDebugRootFormatting,
                "Форматирование, коррекция кегля"
                ,FALSE);
		//форматирование и конвертры
        LDPUMA_Registry(&hDebugConverters,SNAP_FRMT_CONVERTERS, hDebugRootFormatting/*NULL*/);
        LDPUMA_RegistryHelp(hDebugConverters,
                "<Ответственный: Б.Шахвердиев> Этап форматирования и конвертирования результатов \
в RTF формат."
                ,FALSE);
		// коррекция кегля
        LDPUMA_Registry(&hDebugCeglCorr,SNAP_KEGL_CORRECT, hDebugRootFormatting/*NULL*/);
        /*
        LDPUMA_Registry(&hDebugCancelStringsColor,              "Отменить определение цвета символа и фона.", hDebugRoot);
        LDPUMA_RegistryHelp(hDebugCancelStringsColor,
                "Отменяет определение цвета символа и фона"
                ,FALSE);
*/
        //LDPUMA_RegVariable(hDebugRoot,"Серифный шрифт в RTF.",gpSerifName,"char *");
        //LDPUMA_RegVariable(hDebugRoot,"Безсерифный шрифт в RTF.",gpSansSerifName,"char *");
        //LDPUMA_RegVariable(hDebugRoot,"Моноширинный шрифт в RTF.",gpCourierName,"char *");

}
///////////////////////
void DoneDebug()
{
        LDPUMA_UnregVariable(szLayoutFileName);
        LDPUMA_Done();
}
