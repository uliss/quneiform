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

/**********  Заголовок  ****************************************************************************/
/*  Автор,                                                                                         */
/*  комментарии                                                                                    */
/*  правка     : Алексей Коноплёв                                                                  */
/*  Редакция   :  15.11.00                                                                         */
/*  Файл       :  'LineKiller.cpp'                                                                 */
/*  Содержание :  Удаление остатков линий                                                          */
/*  Назначение :  Удаление остатков линий после снятия линий                                       */
/*  Комментарий:                                                                                   */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "rstuff.h"
#include "rsfunc.h"
#include "rsglobaldata.h"
#include "dpuma.h"

#include "puma/pumadef.h"
#include "ccom/ccom.h"
#include "cpage/cpage.h"
#include "rline.h"
#include "cline.h"
#include "linedefs.h"

using namespace CIF;

Bool32 TuneFilter(LineInfo *Line, Rect16 *Rect, uint32_t KillZone, uint32_t Rate) {
    Bool32 bRet;

    bRet = ChekComponentAndLine(Line, Rect, KillZone);

    if (bRet)
        bRet = CheckSquare(Line, Rect, KillZone, Rate);

    return bRet;
}

Bool32 ChekComponentAndLine(LineInfo *Line, Rect16 *Rect, uint32_t KillZone) {
    int32_t A0, B0, A1, B1; // начало и конец линии
    int32_t LineType; // Горизонтальная = 1 ( || > 1 ) или вертикальная = -1 ( || < 1 )
    int32_t As, Bs, C;
    Bool32 bRet = FALSE;
    uint32_t wN, wP, wZ;
    Point16 pPoints[4];
    int32_t Zone = KillZone;

    if (Line->A.x() == Line->B.x() || Line->A.y() == Line->B.y()) {
        bRet = TRUE;
    }

    if (!bRet) {
        float temp = (float) (Line->A.x() - Line->B.x()) / (float) (Line->A.y() - Line->B.y());
        int32_t iTemp;
        uint32_t SecondHand = 1;

        LineType = (temp > -1) && (temp < 1) ? -1 : 1;

        pPoints[3].rx() = pPoints[0].rx() = Rect->left();
        pPoints[1].ry() = pPoints[0].ry() = Rect->top();
        pPoints[2].rx() = pPoints[1].rx() = Rect->right();
        pPoints[3].ry() = pPoints[2].ry() = Rect->bottom();

        wN = wP = wZ = 0;

        do {
            switch (SecondHand) {
            case 1:
                Zone = -(((Line->Thickness + 1) / 2) + (int32_t) KillZone);
                break;
            case 0:
                Zone = (((Line->Thickness + 1) / 2) + (int32_t) KillZone);
                break;
            default:
                Zone = 0;
            }

            A0 = Line->A.x() + (LineType <= 0 ? Zone : 0);
            B0 = Line->A.y() + (LineType >= 0 ? Zone : 0);
            A1 = Line->B.x() + (LineType <= 0 ? Zone : 0);
            B1 = Line->B.y() + (LineType >= 0 ? Zone : 0);

            As = A0 - A1;
            Bs = B0 - B1;
            C = (B0 * As) - (A0 * Bs);

            for (int32_t i = 0; i < 4; i++) {
                iTemp = (pPoints[i].x() * Bs) - (pPoints[i].y() * As) + C;

                if (iTemp == 0) {
                    wZ++;
                    bRet = TRUE;
                }
                else if (iTemp > 0)
                    wP++;
                else
                    wN++;
            }

        }
        while (SecondHand-- && (bRet == FALSE));

        if (wP < 8 && wN < 8)
            bRet = TRUE;

        if (wZ != 0 && (wP == 0 || wN == 0))
            bRet = FALSE;

    }

    return bRet;
}

Bool32 CheckSquare(LineInfo *Line, Rect16 *Rect, uint32_t KillZone, uint32_t Rate) {
    Bool32 bRet = FALSE;

    float temp;
    float Halfs = 1.0;
    int32_t LineType;
    CIF::Rect S;
    uint32_t SRect;
    uint32_t SLine;
    int32_t iTemp;
    int32_t A0 = Line->A.x();
    //	int32_t B0 = Line->B.y();
    int32_t B0 = Line->A.y(); //Almi 30.11.00
    int32_t As = A0 - Line->B.x();
    int32_t Bs = B0 - Line->B.y();
    int32_t Thick = ((Line->Thickness + 1) / 2) + KillZone;

    if (Rate > 255)
        Rate = 255;

    if (Line->A.y() == Line->B.y())
        LineType = 1;
    else if (Line->A.x() == Line->B.x())
        LineType = -1;
    else {
        temp = (float) (Line->A.x() - Line->B.x()) / (float) (Line->A.y() - Line->B.y());
        LineType = (temp > -1) && (temp < 1) ? -1 : 1;
    }

    if (LineType > 0) {
        // горизонтальные линии

        S.rleft() = Line->A.x() - KillZone;
        S.rright() = Line->B.x() + KillZone;

        if (Line->A.y() > Line->B.y()) {
            S.rtop() = Line->B.y() - Thick;
            S.rbottom() = Line->A.y() + Thick;
        }
        else {
            //			S.bottom() = Line->B.y() - Thick;
            //			S.top() = Line->A.y() + Thick;
            S.rtop() = Line->A.y() - Thick;
            S.rbottom() = Line->B.y() + Thick; //Almi 01.12.00
        }

        //смотрим точки пересечения компоненты с линией
        /*		if ( !(Rect->left() < S.left && Rect->right() > S.left) )
         {
         S.left = Rect->left();
         }

         if ( !(Rect->left() < S.right && Rect->right() > S.right) )
         {
         S.right = Rect->right();
         }*///Всё неправильно! Almi 01.12.00
        if (S.left() <= Rect->left())
            S.rleft() = Rect->left();
        if (S.right() >= Rect->right())
            S.rright() = Rect->right();
        if (S.right() <= S.left())
            S.rright() = S.left(); //End Almi


        S.rbottom() = S.rtop() = B0 - (int32_t) (((float) Bs / (float) As)
                * (float) (A0 - S.left()));
        iTemp = B0 - (int32_t) (((float) Bs / (float) As) * (float) (A0 - S.right()));

        S.rtop() = S.rtop() < iTemp ? S.top() - Thick : iTemp - Thick;
        S.rbottom() = S.rbottom() > iTemp ? S.bottom() + Thick : iTemp + Thick;

        /*		if ( !(Rect->top() <= S.top() && Rect->bottom() >= S.top()) )
         {
         S.top() = Rect->top();
         Halfs = 0.5;
         }

         if ( !(Rect->top() <= S.bottom() && Rect->bottom() >= S.bottom()) )
         {
         S.bottom() = Rect->bottom();
         Halfs = (float)(Halfs < 1.0 ? 0 : 0.5);
         }*///Всё неправильно! Almi 01.12.00
        if (S.top() <= Rect->top())
            S.rtop() = Rect->top();
        if (S.bottom() >= Rect->bottom())
            S.rbottom() = Rect->bottom();
        if (S.bottom() <= S.top())
            S.rbottom() = S.top(); //End Almi

        //считаем площади
        iTemp = Rect->bottom() - Rect->top();
        SRect = 1 + (iTemp < 0 ? -iTemp : iTemp);
        iTemp = Rect->right() - Rect->left();
        SRect *= 1 + (iTemp < 0 ? -iTemp : iTemp);

        iTemp = S.bottom() - S.top();
        SLine = 1 + (iTemp < 0 ? -iTemp : iTemp);
        iTemp = S.right() - S.left();
        SLine *= 1 + (iTemp < 0 ? -iTemp : iTemp);
        /*		iTemp = (int32_t)(Halfs * (float)iTemp / temp);
         SLine -= iTemp < 0 ? -iTemp : iTemp;*/

        // отношение
        if (SLine != 0) {
            temp = (float) SLine / (float) SRect;
            temp *= (float) 255;

            if ((uint32_t) temp >= Rate)
                bRet = TRUE;
        }
    }
    else {
        // вертикальные линии
        S.rtop() = Line->A.y() - KillZone;
        S.rbottom() = Line->B.y() + KillZone;

        if (Line->A.x() > Line->B.x()) {
            S.rleft() = Line->B.x() - Thick;
            S.rright() = Line->A.x() + Thick;
        }
        else {
            //			S.right() = Line->B.x() - Thick;
            //			S.left() = Line->A.x() + Thick;
            S.rleft() = Line->A.x() - Thick;
            S.rright() = Line->B.x() + Thick; //Almi 01.12.00
        }

        //смотрим точки пересечения компоненты с линией
        /*		if ( !(Rect->top() <= S.top() && Rect->bottom() >= S.top()) )
         {
         S.top() = Rect->top();
         }

         if ( !(Rect->top() <= S.bottom() && Rect->bottom() >= S.bottom()) )
         {
         S.bottom() = Rect->bottom();
         }*///Всё неправильно! Almi 01.12.00
        if (S.top() <= Rect->top())
            S.rtop() = Rect->top();
        if (S.bottom() >= Rect->bottom())
            S.rbottom() = Rect->bottom();
        if (S.bottom() <= S.top())
            S.rbottom() = S.top(); //End Almi

        S.rleft() = S.rright() = A0
                - (int32_t) (((float) As / (float) Bs) * (float) (B0 - S.top()));
        iTemp = A0 - (int32_t) (((float) As / (float) Bs) * (float) (B0 - S.bottom()));

        S.rleft() = S.rleft() < iTemp ? S.left() - Thick : iTemp - Thick;
        S.rright() = S.rright() > iTemp ? S.right() + Thick : iTemp + Thick;

        /*		if ( !(Rect->left() <= S.left() && Rect->right() >= S.left()) )
         {
         S.left() = Rect->left();
         Halfs = 0.5;
         }

         if ( !(Rect->left() <= S.right() && Rect->right() >= S.right()) )
         {
         S.right() = Rect->right();
         Halfs = (float)(Halfs < 1.0 ? 0 : 0.5);
         }*///Всё неправильно! Almi 01.12.00
        if (S.left() <= Rect->left())
            S.rleft() = Rect->left();
        if (S.right() >= Rect->right())
            S.rright() = Rect->right();
        if (S.right() <= S.left())
            S.rright() = S.left(); //End Almi

        //считаем площади
        iTemp = Rect->right() - Rect->left();
        SRect = 1 + (iTemp < 0 ? -iTemp : iTemp);
        iTemp = Rect->bottom() - Rect->top();
        SRect *= 1 + (iTemp < 0 ? -iTemp : iTemp);

        iTemp = S.right() - S.left();
        SLine = 1 + iTemp < 0 ? -iTemp : iTemp;
        iTemp = 1 + S.bottom() - S.top();
        SLine *= iTemp < 0 ? -iTemp : iTemp;
        /*		iTemp = (int32_t)(Halfs * (float)iTemp / temp);
         SLine -= iTemp < 0 ? -iTemp : iTemp;*///Almi

        // отношение
        if (SLine != 0) {
            temp = (float) SLine / (float) SRect;
            temp *= (float) 255;

            if ((uint32_t) temp >= Rate)
                bRet = TRUE;
        }
    }

    return bRet;
}
