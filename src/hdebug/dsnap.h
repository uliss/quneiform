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

#ifndef _DSNAP_H_
#define _DSNAP_H_

// Snap
//Allex/////////////////////////
// переделка дерева в более логичный вид
// в корне остаются пять основных ветвей, остальные пристраиваются под них
//
// 1 общая отладочна
#define SNAP_ROOT_							"Основные настройки ядра..."
	#define SNAP_SELECT_DISABLE				"Выделить/Oтменить этап..."
	#define SNAP_SELECT_ENABLE				"Выделить/Разрешить этап..."
	#define SNAP_CPAGE_STORAGE				"Контейнер CPAGE"
//
// 2 обработка сырь
#define SNAP_ROOT_STUFF						"Сырье"
	#define SNAP_STUFF_BINARIZE				"Обработка изображения..."
	#define	SNAP_STUFF_AUTOTEMPLATE			"Автоматическое ограничение области распознавания."
	#define SNAP_STUFF_VER_LINES			"Верификация линий..."
	#define SNAP_ROOT_LINES					"Выделение и снятие линий..."
	#define SNAP_STUFF_LINES				"Выделение и снятие линий..."
		#define SNAP_PASS1_LINES			"Первый проход по линиям"
		#define SNAP_PASS2_LINES			"Второй проход по линиям"
		#define SNAP_PASS3_LINES			"Третий проход по линиям"
		#define SNAP_STUFF_KILL_LINES			"Удаление Линий"
			#define SNAP_KILL_LINES_STEP		"Убиваем по одной"
			#define SNAP_KILL_LINES_DATA		"Чуть подробнее о них же"
			#define SNAP_KILL_LINES_SHOW_BEFORE	"Показать компоненты до убития линий"
			#define SNAP_KILL_LINES_SHOW_AFTER	"Показать компоненты после убития линий"
 		#define SNAP_STUFF_RSL "Оболочка над линиями..."

//
// 3 разметка, выделение таблиц
#define SNAP_ROOT_LAYOUT					"Разметка"
 #define SNAP_LAYOUT_SEGMENT				"Грубое выделение фрагментов..."
 #define SNAP_LAYOUT_PICTURES				"Выделение картинок..."
 #define SNAP_LAYOUT_NEG	                "Поиск и распознование негативов"
 #define SNAP_LAYOUT_FON                    "Поиск полос заливки"
    #define SNAP_LAYOUT_LAYOUT				"Выделение фрагментов..."
	#define SNAP_ROOT_TABLES				"Выделение таблиц..."
	#define SNAP_LAYOUT_TABLES				"Выделение таблиц..."
	#define SNAP_LAYOUT_POST_LINES			"Доработка линий"
		#define SNAP_LAYOUT_SHOR_LINES		"Короткие вертикальные линии"
			#define SNAP_SHOR_LINES_STEP	"По одной"
			#define SNAP_SHOR_LINES_DATA	"Чуть подробнее"
	#define SNAP_ROOT_STRINGS				"Выделение строк..."
	#define SNAP_LAYOUT_STRINGS				"Выделение строк..."
//	#define SNAP_STUFF_RSL "Оболочка над линиями (III проход)"

//
// 4 распознавание
#define SNAP_ROOT_RECOGNITION				"Распознавание"
	#define SNAP_RECG_RECOGNITION			"Распознавание строк..."
	#define SNAP_RECG_SPELL					"Словарь и постобработка"
//
// 5 форматирование
#define SNAP_ROOT_CONVERTERS				"Форматирование"
	#define SNAP_FRMT_CONVERTERS			"Форматирование и конвертирование..."
	#define SNAP_KEGL_CORRECT				"Коррекция кегля..."


//Главная отладка
#define SNAP_ROOT_MAIN_DEBUG                "Главная отладка"
//Главное этапирование
#define SNAP_ROOT_MAIN_TIME_CONTROL         "Главное этапирование"

#endif
