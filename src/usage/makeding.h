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
    \file   MakeDing.H
    \brief  Описание функций заготовки и чистки разделителей
    \author Александр Михайлов
    \date   20.04.2005
*/
/*------------------------------------------------------------------------------------------------*/
/*  N-я Редакция 20.04.2005:
    - приведение к стандарту алгоритмов, реализованных в модуле "MakeDing.CPP" к 15.04.2002  */
/*------------------------------------------------------------------------------------------------*/
/**
    \brief Функция изготовления набора разделителей из набора обобщённых линий
    \param pLinePool [in]  - массив обобщённых линий
    \param pDingPool [out] - массив разделителей
    \param pParam [in]     - указатель на параметры управления джля изглтлвления разделителей
    \param SkewReg [in]    - угол наклона идеальных координат
    \note  Используется в RLTable и MZoning
*/
bool MakeDingFromLine (RLT_DINGPOOL *pDingPool, const RLT_LINEPOOL *pLinePool
    , const RLT_PARAM *pParam, const int32_t SkewReg);
/**
    \brief Функция летописи эволюции обобщённых линий
    \param pLinePool [in] - массив обобщённых линий
    \param pForw [out]    - массив пометок об эволюции обобщённых линий вперёд
    \param pBack [out]    - массив пометок об эволюции обобщённых линий назад
    \param MyMaxL [in]    - максимальное количество обобщённых линий для отслеживания их эволюции
    \param StepAlg [in]   - номер эволюционного шага
    \note  Используется только в RLTable
*/
void EvolFor_MakeDing (const RLT_LINEPOOL *pLinePool, int *pForw, int *pBack, const int MyMaxL
    , const int StepAlg);
/**
    \brief Функция удаления лишних и добавления отсутствующих разделителей
    \param pDingPool [in/out] - массив разделителей
    \param pForw [out]        - массив пометок об эволюции обобщённых линий вперёд
    \param pBack [out]        - массив пометок об эволюции обобщённых линий назад
    \param MyMaxL [in]        - максимальное количество обобщённых линий для отслеживания их эволюции
    \param StepAlg [in]       - номер эволюционного шага
    \param pParam [in]        - указатель на параметры управления для ревизии разделителей
    \note  Используется только в RLTable
*/
bool DelAndAddDing (RLT_DINGPOOL *pDingPool, int *pForw, int *pBack, const int MyMaxL
    , const int StepAlg, const RLT_REVIDING *pReviDing);
/*------------------------------------------------------------------------------------------------*/
