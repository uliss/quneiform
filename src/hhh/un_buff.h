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

/*------------------------------------------------------------------------------------------------*/
/**
    \file   UN_Buff.CPP
    \brief  Описание функций работы со складом информации
    \author Александр Михайлов
    \date   26.04.2005
*/
/*------------------------------------------------------------------------------------------------*/
/*  N-я Редакция 26.04.2005:
    - приведение к стандарту алгоритмов, реализованных в модуле "UN_Buff.CPP" к 02.03.2001  */
/*------------------------------------------------------------------------------------------------*/
/**********  Заголовок  **********/
/*  Автор      :  Александр Михайлов                                          */
/*  Редакция   :  03.02.01                                                    */
/*  Файл       :  'UN_Buff.H'                                                 */
/*  Содержание :  Структуры, макросы и прототипы функций работы с памятью.    */
/*  Назначение :  Оптимизация работы с памятью.                               */
/*----------------------------------------------------------------------------*/
const int MAX_BUF_PART = 100; ///< максимальное количество разнородных данных на складе
typedef struct tagUN_BUFF
{
    /*  отведенная изначально память  */
    void     *vBuff; ///< ИМЯ НЕ МЕНЯТЬ! указатель на склад информации и свободную память
    int       SizeBuff; ///< размер склада информации
    /*  пока свободная часть памяти  */
    void     *vCurr; ///< ИМЯ НЕ МЕНЯТЬ! указатель на свободную память в буфере
    int       SizeCurr; ///< размер свободную памяти
    /*  задействованная часть памяти  */
    int       nPart; ///< сколько частей памяти задействовашо
    void     *vPart[MAX_BUF_PART]; ///< массив адресов задействованных частей памяти
    char      TypePart[MAX_BUF_PART]; ///< массив типов складированных данных
    char      AimPart[MAX_BUF_PART]; ///< массив целей складированных данных
    int       SizePartUnits[MAX_BUF_PART]; ///< массив размеров атомов в складированных данных
    int       nPartUnits[MAX_BUF_PART]; ///< массив количества атомов в складированных данных
    int       SizePartTotal[MAX_BUF_PART]; ///< массив размеров складированных данных
}UN_BUFF;
/*------------------------------------------------------------------------------------------------*/
/**
    \brief Функция удаления последней записанной части на складе
    \param p_vB [in/out] - указатель на склад информации (и свободную память)
*/
void CleanLastDataPart (void *p_vB);
/**
    \brief Функция удаления последней записанной части на складе
    \param p_vB [in/out] - указатель на склад информации (и свободную память)
*/
void EndLastDataPart (void *p_vB, const char Aim, const char Type, const int SizeU, const int nU);
/**
    \brief Функция завершения последней записи данных на складе
    \param p_vB [in]  - указатель на склад информации (и свободную память)
    \param Aim [in]   - предназначение последних записанных данных
    \param Type [in]  - тип последних записанных данных
    \param SizeU [in] - размеров атомов в последних записанных данных
    \param nU [in]    - размеров атомов в последних записанных данных
*/
int  FindSuchData (const void *p_vB, const int WhatData);
/**
    \brief Функция нахождения на складе данных указанного типа и указанного целевого назначения
    \param p_vB [in]     - указатель на склад информации (и свободную память)
    \param WhatData [in] - тип данных, которые надо найти на складе
    \param WhatAim [in]  - предназначение данных, которые надо найти на складе
*/
int  FindSuchAimedData (const void *p_vB, const int WhatData, const int WhatAim);
/**
    \brief Функция нахождения на складе следующей порции данных указанного типа
    \param p_vB [in]     - указатель на склад информации (и свободную память)
    \param WhatData [in] - тип данных, которые надо найти на складе
    \param IndPrev [in]  - индекс последних найденных данных требуемого типа
*/
int  FindNextSuchData (const void *p_vB, const int WhatData, const int IndPrev);
/*------------------------------------------------------------------------------------------------*/
