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
    \file   Anal_Line_AM.CPP
    \brief  Реализация функций технического анализа линий
    \author Александр Михайлов
    \date   25.04.2005
*/
/*------------------------------------------------------------------------------------------------*/
/*  N-я Редакция 25.04.2005:
    - приведение к стандарту алгоритмов, реализованных в модуле "Anal_Line_AM.CPP" к 16.04.2001  */
/*------------------------------------------------------------------------------------------------*/
/**********  Заголовок  **********/
/*  Автор      :  Александр Михайлов                                          */
/*  Редакция   :  06.03.01                                                    */
/*  Файл       :  'Anal_Line_AM.CPP'                                          */
/*  Содержание :  Блок работы с линиями своего формата.                       */
/*  Назначение :  Первичная обработка сырца для создания разделителей.        */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <windows.h>
/*  общесорсовые мои  */
#include "LineDesc.H"
/*  межпроектные чужие  */
#include "skew1024.h"
/*  межпроектные мои  */
#include "MarkDataDefs.H"
#include "UN_Buff.H"
#include "UN_Err.H"
/*----------    Управление локальное    ----------------------------------------------------------*/
/*+ (MarkLineCrimSkew) +*/
/** Минимальный разрешаемый разброс угла */
static const int MIN_DELTA_SKEW_NORM = 31;// 05.04.01 а было 27;//23;
/** Максимальный разрешаемый разброс угла */
static const int MAX_DELTA_SKEW_NORM = 50;
/** Минимальная длина длинной линии */
static const int MIN_LEN_LONG_LINE = 350;
//SkewWidForAnal = 9; как оказалось, нигде не используется
/*----------    Локальные функции    -------------------------------------------------------------*/
/**
    \brief Функция выявления линий, криминальных по углу
    \param pL [in/out] - массив линий
    \param nLine [in]  - количество линий в массиве
    \param pP [in]     - массив по квадрантам информации о пуле линий
*/
static void MarkLineCrimSkew (RLT_LINE *pL, const int nLine, const RLT_LINEATDIR *pP)
{
    for (int i = 0;  i < nLine;  i++)//перебираем линии
    {
        int Sect = ::LineSect (pL[i].Type);
//        DeltaNorm = (pL[i].Skew2Delta + 4)/2;
//        DeltaNorm = ((pL[i].Skew2Delta + 4)*3)/4;//04.04.01
        int DeltaNorm = pL[i].Skew2Delta;//16.04.01
        if (DeltaNorm < MIN_DELTA_SKEW_NORM)
            DeltaNorm = MIN_DELTA_SKEW_NORM;
        if ((DeltaNorm > MAX_DELTA_SKEW_NORM) || (pL[i].Len >= MIN_LEN_LONG_LINE))
            DeltaNorm = MAX_DELTA_SKEW_NORM;
        if (abs(pL[i].Skew - pP[Sect].SkewPool) > DeltaNorm)
            pL[i].Type |= RLT_LT_SkewConflict;
    }
}
/*------------------------------------------------------------------------------------------------*/
/**
    \brief Функция наполнения по квадрантам информации о пуле линий
    \param pL [in]    - массив линий
    \param nLine [in] - количество линий в массиве
    \param pP [out]   - массив по квадрантам информации о пуле линий
*/
static void MarkPoolLineInfo (const RLT_LINE *pL, int nLine, RLT_LINEATDIR *pP)
{
    for (int j = 0; j < 4; j++)//по четырём квадрантам
    {
        pP[j].nLine = 0;
        pP[j].nLongLine = 0;
        pP[j].nLongNoAtPool = 0;
        pP[j].nShortContrPool = 0;
    }
    for (int i = 0;  i < nLine;  i++)//перебираем линии
    {
        int Sect = ::LineSect (pL[i].Type);
        (pP[Sect].nLine)++;
        if ((pL[i].Type & RLT_LT_Long) == RLT_LT_Long)
            (pP[Sect].nLongLine)++;
        if ((pL[i].Type & RLT_LT_SkewConflict) == RLT_LT_SkewConflict)//забракованные по углу
        {
            if ((pL[i].Type & RLT_LT_Long) == RLT_LT_Long)
                (pP[Sect].nLongNoAtPool)++;
            else
                (pP[Sect].nShortContrPool)++;
        }
    }
}
/*----------    Переходные функции    ------------------------------------------------------------*/
/**
    \brief Функция анализа линий и их групп из хранилища
    \param pLinePool [in/out] - указатель на хранилище линий и групповой информации о них
    \param Skew [in]          - угол наклона разграфки страницы
    \note  Используется в SMetric и USAGE
*/
void AnalOfMyLines (RLT_LINEPOOL *pLinePool, const int32_t Skew)
{
    for (int j = 0;  j < 4;  j++)//по четырём квадрантам формальная инициализация
    {
        pLinePool->Pool[j].SkewPool = Skew;
        pLinePool->Pool[j].WeightSkew = 1; //для проформы
    }
    MarkLineCrimSkew (pLinePool->pLine, pLinePool->nLine, pLinePool->Pool);
    MarkPoolLineInfo (pLinePool->pLine, pLinePool->nLine, pLinePool->Pool);
}
/*------------------------------------------------------------------------------------------------*/
/**
    \brief Функция определяет, к какому квадранту относится такой тип линии
    \param Type [in]          - тип линии
    \retval RLT_LINEDIR_TYPE  - квадрант, в котором лежит линия
*/
RLT_LINEDIR_TYPE LineSect (const char Type)
{
    if ((Type & RLT_LT_Kosaya) == RLT_LT_Kosaya)//"неправильные" по ориентации линии
    {
        if ((Type & RLT_LT_Vertic) == RLT_LT_Vertic)
            return RLT_LDIR_Kosa1;
        else
            return RLT_LDIR_Kosa2;
    }
    else
    {
        if ((Type & RLT_LT_Vertic) == RLT_LT_Vertic)
            return RLT_LDIR_Verti;
        else
            return RLT_LDIR_Horiz;
    }
}
/*------------------------------------------------------------------------------------------------*/
/**
    \brief Функция вычисляет охватывающий прямоугольник линий со склада
    \param p_vB [in/out] - указатель на склад информации и свободную память
    \param pRcLin [out]  - указатель на охватывающий прямоугольник
    \param SkewReg[in]   - угол наклона идеальных координат
*/
bool FindLinesReg (const void *p_vB, Rect16 *pRcLin,  const int32_t SkewReg)
{
    Point16 A = {0}, B = {0};
    const UN_BUFF *pB = static_cast<const UN_BUFF *>(p_vB);
    /*  Загружаем данные  */
    int Ind = ::FindSuchAimedData (p_vB, UN_DT_RltLinePool, UN_DA_Linii);
    if (Ind < 0)
        return false;
    int nLine = (static_cast<RLT_LINEPOOL *>(pB->vPart[Ind]))->nLine;
    if (nLine <= 0)
        return false;
    Ind = ::FindSuchAimedData (p_vB, UN_DT_RltLine, UN_DA_Linii);
    if (Ind < 0)
        return false;
    RLT_LINE *pLine = reinterpret_cast<RLT_LINE *>(pB->vPart[Ind]);
    /*  Инициализируем искомый прямоугольник  */
    A.x = pLine[0].Beg.x;
    A.y = pLine[0].Beg.y;
    B.x = pLine[0].End.x;
    B.y = pLine[0].End.y;
    ::Deskew (A, -SkewReg);
    ::Deskew (B, -SkewReg);
    pRcLin->left   = A.x;
    pRcLin->right  = A.x;
    pRcLin->top    = A.y;
    pRcLin->bottom = A.y;
    /*  Цикл расширения искомой области перебором линий  */
    for (int i = 0;  i < nLine;  i++)
    {
        A.x = pLine[i].Beg.x;
        A.y = pLine[i].Beg.y;
        B.x = pLine[i].End.x;
        B.y = pLine[i].End.y;
        ::Deskew (A, -SkewReg);
        ::Deskew (B, -SkewReg);
        if (pRcLin->left   > A.x)
            pRcLin->left   = A.x;
        if (pRcLin->right  < A.x)
            pRcLin->right  = A.x;
        if (pRcLin->top    > A.y)
            pRcLin->top    = A.y;
        if (pRcLin->bottom < A.y)
            pRcLin->bottom = A.y;
        if (pRcLin->left   > B.x)
            pRcLin->left   = B.x;
        if (pRcLin->right  < B.x)
            pRcLin->right  = B.x;
        if (pRcLin->top    > B.y)
            pRcLin->top    = B.y;
        if (pRcLin->bottom < B.y)
            pRcLin->bottom = B.y;
    }
    return true;
}
/*------------------------------------------------------------------------------------------------*/
/*----------    Локальные функции    -------------------------------------------------------------*/
static void MarkLineCrimSkew (RLT_LINE *pL, const int nLine, const RLT_LINEATDIR *pP);
static void MarkPoolLineInfo (const RLT_LINE *pL, int nLine, RLT_LINEATDIR *pP);
/*------------------------------------------------------------------------------------------------*/
