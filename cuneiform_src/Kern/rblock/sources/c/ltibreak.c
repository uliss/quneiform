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



/****************************************************************************
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              LTIBREAK.C - Initial breaking of page                       *
 *                                                                          *
 ***************************************************************************/

# include "layout.h"
# include "dpuma.h"

extern Handle hInitialBreaking;

// настроить поле nBlock рута на номер компоненты (связности)
void SetRootsBlocks (int nShift)
{
    ROOT *pRoot;
    RECTANGLE r;
    int  nBlock;

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
    {
        if (pRoot -> nBlock >= FIRST_REGULAR_BLOCK_NUMBER)
            continue;

        r.xLeft   = XY_COMPRESS (pRoot -> xColumn);
        r.yTop    = XY_COMPRESS (pRoot -> yRow);

        // берем идентификатор (типа) блока из матрицы страницы,
        // в которой к этому моменту уже сидит номер блока
        // (поднятый на три бита PAGE_MATRIX_MARK_SHIFT и т.п.,
        // но это уже не так существенно)
        nBlock = PageMatrix [(r.yTop << PAGE_MATRIX_WIDTH_SHIFT) + r.xLeft]
                 >> PAGE_MATRIX_MARK_SHIFT;

        if (nBlock >= FIRST_REGULAR_BLOCK_NUMBER)
        { // то есть если блок относится (как должен)
          // к пронумерованной компоненте ("зарегистрирован")
            pRoot -> nBlock = nBlock + nShift;
            continue;
        }

        // если блок НЕ относится к уже учтенным компонентам

        r.xRight  = XY_COMPRESS (pRoot -> xColumn + pRoot -> nWidth - 1);

        nBlock = PageMatrix [(r.yTop << PAGE_MATRIX_WIDTH_SHIFT) + r.xRight]
                 >> PAGE_MATRIX_MARK_SHIFT;

        if (nBlock >= FIRST_REGULAR_BLOCK_NUMBER)
        {
            pRoot -> nBlock = nBlock + nShift;
            continue;
        }

        r.yBottom = XY_COMPRESS (pRoot -> yRow    + pRoot -> nHeight - 1);

        nBlock = PageMatrix [(r.yBottom << PAGE_MATRIX_WIDTH_SHIFT) + r.xLeft]
                 >> PAGE_MATRIX_MARK_SHIFT;

        if (nBlock >= FIRST_REGULAR_BLOCK_NUMBER)
        {
            pRoot -> nBlock = nBlock + nShift;
            continue;
        }

        nBlock = PageMatrix [(r.yBottom << PAGE_MATRIX_WIDTH_SHIFT) + r.xRight]
                 >> PAGE_MATRIX_MARK_SHIFT;

        if (nBlock >= FIRST_REGULAR_BLOCK_NUMBER)
        {
            pRoot -> nBlock = nBlock + nShift;
            continue;
        }
    }
}

// чистка матрицы страницы с оставлением исключительно флагов
// рута, дуста и картинки; чистятся элементы матрицы,
// принадлежащие каждой из компонент в указанном списке:
void ClearSeries (COMP *pBegin, COMP *pEnd)
{
    COMP *pComp;

    // чистим матрицу страницы, оставляя только базовые флаги:
    // рута, дуста и картинки
    for (pComp = pBegin; pComp != pEnd; pComp = pComp -> pNext)
    {
        CompAND_Matrix (pComp, PMC_FLAGS_MASK);
    }
}

void InitialBreakingProceed (void)
{
    int i;
    int nCompShift;
    int nComp;
    COMP *pCompSeriesBegin;
    COMP *pComp;

    // отмечается, к какому типу блока сии руты более тяготеют:
    // (например -- к "дустовому")
    for (i = 0; i < nRoots; i++)
    {
        pRoots [i].nBlock =
            IS_LAYOUT_DUST (pRoots [i]) ?
                DUST_BLOCK_NUMBER :
                REMAINDER_BLOCK_NUMBER; // единственное место в коде, где ставится этот флаг
    }

    if (! bOptionInitialBreakingByPageMatrix)
        return;

    CompsBuild
    (
        PageMatrix,
        PAGE_MATRIX_WIDTH,   // стандартная ширина матрицы
        PAGE_MATRIX_HEIGHT,  // стандартная высота матрицы
        PAGE_MATRIX_SIZE,    // угу :)
        PMC_ROOT             // "цвет" выделяемых интервалов
    );

    nCompShift       = 0;
    nComp            = 0;          // текущее (учтенное) кол-во компонент
    pCompSeriesBegin = pCompsList; // начало списка компонент

    for (pComp = pCompsList; pComp != NULL; pComp = pComp -> pNext)
    {
        if (nComp == COMP_SERIES_LENGTH)
        {
            // настроить поле nBlock рута на номер компоненты (связности)
            SetRootsBlocks (nCompShift);

# ifdef LT_DEBUG
            //if (LT_DebugGraphicsLevel >= 4)
			// попросту снова выводим на экран матрицу страницы
      // (шо, в общем-то, интересно -- но не так уж и нужно)
      if (!LDPUMA_Skip(hInitialBreaking))
                LT_GraphicsPageMatrixOutput ("Comps series");
# endif

            // чистим матрицу страницы, оставляя только базовые флаги:
            // рута, дуста и картинки (для компонент с pCompSeriesBegin
            // по текущую; в данном случае -- по последнюю)
            ClearSeries (pCompSeriesBegin, pComp);

            nCompShift += nComp;
            nComp = 0;
            pCompSeriesBegin = pComp;
        }

        // используя стрипы, наложить "на компоненту" заданную
        // маску (там, где первоначально в матрице был флажок PMC_ROOT;
        // таким образом, здесь мы фактически имеем отрисовку нумерации
        // блоков на матрице страницы
        CompOR_Matrix
        (
            pComp,
            (nComp + FIRST_REGULAR_BLOCK_NUMBER) << PAGE_MATRIX_MARK_SHIFT
        );

        nComp++;
    }

    // настроить поле nBlock рута на номер компоненты (связности)
    // (вся разница -- сдвиг перенастроен на nComp (<- добавка к сдвигу))
    SetRootsBlocks (nCompShift);
    // чистим матрицу страницы, оставляя только базовые флаги:
    // рута, дуста и картинки (индексы компонент в рутах настроены
    // -- матрицу страницы и остальное можно возвращать в исходные (позиции))
    ClearSeries (pCompSeriesBegin, NULL);

    nNextBlockNumber = nCompShift + nComp + 1;
    CompsFreeData ();      // все стираем: и список компонент,
    IntervalsFreeData ();  // и стрипы, и (разумеется) интервалы.

    // остается: в каждом руте (в поле nBlock) -- данные о компоненте;
    // в матрице страницы -- то, что было до входа в эту функцию.
}

void InitialBreakingFreeData (void)
{
}
