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
    \file   AMU_Geom.CPP
    \brief  Описания типовых геометрических функций
    \author Александр Михайлов
    \date   19.04.2005
*/
/*------------------------------------------------------------------------------------------------*/
/*  N-я Редакция 19.04.2005:
    - приведение к стандарту алгоритмов, реализованных в модуле "AMU_Geom.CPP" к 02.03.2001  */
/*------------------------------------------------------------------------------------------------*/
/*  межпроектные чужие  */
#include "cttypes.h"
/*------------------------------------------------------------------------------------------------*/
/**
    \brief Функция поворота прямоугольника
    \param pRcRe [in]  - указатель на прямоугольник в реальных координатах
    \param pRcId [out] - указатель на прямоугольник в идеальных координатах
    \param Skew[in]    - угол наклона идеальных координат
    \note  Используется в RLTable и USAGE
*/
void RotateRect (const Rect16 *pRcRe, Rect16 *pRcId, int32_t Skew);
/**
    \brief Функция нахождения минимального прямоугольника, охватывающего заданные
    \param nRc [in]      - количество охватываемых прямоугольников
    \param pDo [in]      - массив пометок об охватываемых прямоугольниках
    \param pRcIdeal [in] - массив охватываемых прямоугольников
    \param pMIR [out]    - указатель на охватывающий прямоугольник
    \note  Используется только в SStreak
*/
void FindMinimIdealRegion (const int nRc, const int *pDo, const Rect16 *pRcIdeal, Rect16 *pMIR);
/**
    \brief Функция проверяет, пересекает ли линия прямоугольник
    \param pA [in/out]  - указатель на точку - начало линии в реальных координатах
    \param pB [in/out]  - указатель на точку - конец линии в реальных координатах
    \param pRc [in]     - указатель на исследуемый прямоугольник в идеальных координатах
    \param Skew [in]    - угол наклона идеальных координат
    \param NearHor [in] - странный параметр, смешивающий логическое и целое
    \param NearVer [in] - странный параметр, смешивающий логическое и целое
    \retval bool        - так пересекает или нет
    \note  Используется в RVerLine и USAGE
*/
bool HaveLinePartAtReg (Point16 *pA, Point16 *pB, const Rect16 *pRc, const int32_t Skew
    , const int NearHor, const int NearVer);
/**
    \brief Функция проверяет, пересекаются ли два прямоугольника
    \param pRc1 [in] - указатель на первый прямоугольник
    \param pRc2 [in] - указатель на второй прямоугольник
    \retval bool     - так пересекаются или нет
    \note  Используется только в MZoning
*/
bool IsRectsCross (const Rect16 *pRc1, const Rect16 *pRc2);
/**
    \brief Функция поворота прямоугольника - выглядит копией функции RotateRect
    \param pRcRe [in]  - указатель на прямоугольник в реальных координатах
    \param pRcId [out] - указатель на прямоугольник в идеальных координатах
    \param Skew[in]    - угол наклона идеальных координат
    \note  Используется только в MZoning
*/
bool MakeInternalRectId (const Rect16 *pRcRe, Rect16 *pRcId, int32_t Skew);
/*------------------------------------------------------------------------------------------------*/
