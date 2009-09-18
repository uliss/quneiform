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
/*  Редакция   :  01.03.01                                                    */
/*  Файл       :  'UN_Err.H'                                                  */
/*  Содержание :  Коды возврата.                                              */
/*  Назначение :  Унификация стандартных ответов.                             */
/*----------------------------------------------------------------------------*/
#define RV_FALSE       0   // FALSE  все плохо (форс-мажор)
#define RV_TRUE        1   // TRUE   все в порядке
#define RV_EMPTY       2   // EMPTY  все в порядке, но реально не работал
#define RV_DOUBT       3   // DOUBT  не все в порядке, но не форс-мажор
#define RV_POSITIVE    4   // TRUE   все в порядке, ответ положительный
#define RV_NEGATIVE    5   // TRUE   все в порядке, ответ отрицательный
#define RV_CONTINUE    6   // TRUE   все в порядке, продолжаем алгоритм
/*----------------------------------------------------------------------------*/
#define ER_ROUGH_NONE                0   // все в порядке
#define ER_ROUGH_OTHER_LIBRARY       1   // ошибка другой библиотеки
#define ER_ROUGH_NOT_SUCH_ERROR_CODE 2   // нет такого кода ошибки
#define ER_ROUGH_CALL_REFUSED        3   // вызов функции проигнорирован
#define ER_ROUGH_NORMAL              4   // моя ошибка
/*----------------------------------------------------------------------------*/
#define ER_DETAIL_NONE               0   // все в порядке
#define ER_DETAIL_WAS_YET_INIT       1   // уже была инициализация
#define ER_DETAIL_WAS_NOT_INIT       2   // еще не было инициализации
#define ER_DETAIL_BAD_UNICAL_NUMBER  3   // плохой уникальный номер
#define ER_DETAIL_TOO_MUCH_CALLS     4   // слишком много вызовов
#define ER_DETAIL_NOT_MADE_SUCH_DATA 5   // не изготовляю такие данные
#define ER_DETAIL_NO_COMMENT         6   // без комментариев
#define ER_DETAIL_EMPTY_FUNC         7   // функция-пустышка
#define ER_DETAIL_MAKET_FUNC         8   // функция-макет (выдуман.вход.данные)
#define ER_DETAIL_BAD_PARAMETRS      9   // плохие параметры
#define ER_DETAIL_NO_MEMORY         10   // нет памяти
/*--------- специфику пумы надо бы убрать отсюда -----------------------------*/
#define ER_DETAIL_FUNC_CPAGE        11   // ошибка функции из 'CPAGE'
#define ER_DETAIL_FUNC_DPUMA        12   // ошибка функции из 'DPUMA'
/*----------------------------------------------------------------------------*/
