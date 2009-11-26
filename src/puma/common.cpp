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
#include <string.h>

#include "mpuma.h"
#include "ccom/ccom.h"
#include "cstr/cstr.h"


//// Функции прогресс индикатора
//Bool32 rexcProgressStep(uint32_t step) {
//  return ProgressStep(2, NULL, step);
//}
//
//void ProgressStart() {
//  LDPUMA_ProgressStart();
//  if (fnProgressStart)
//      fnProgressStart();
//}
//
//void ProgressFinish() {
//  LDPUMA_ProgressFinish();
//  if (fnProgressFinish)
//      fnProgressFinish();
//}
//
//Bool32 ProgressStep(uint32_t step, char*name, uint32_t percent) {
//  Bool32 rc = TRUE;
//  static uint32_t old = 0;
//
//  g_PrgTime.dwStep = step;
//  g_PrgTime.name = name ? name : g_PrgTime.name;
//
//  uint32_t perc = g_PrgTime.dwBeg + percent * (g_PrgTime.dwEnd
//          - g_PrgTime.dwBeg) / 100;
//  rc = LDPUMA_ProgressStep(step, g_PrgTime.name, perc);
//  //  assert(perc>=old);
//  if (fnProgressStep)
//      rc &= fnProgressStep(step, g_PrgTime.name, perc);
//
//  old = perc;
//  return rc;
//}
//
//Bool32 ProgressStepLayout(uint32_t step, uint32_t percent) {
//  return ProgressStep(step, GetResourceString(IDS_PRG_OPEN), percent);
//}
//
//Bool32 ProgressStepLines(uint32_t step, uint32_t percent) {
//  return ProgressStep(step, GetResourceString(IDS_REMOVELINE), percent);
//}
//
//Bool32 ProgressStepTables(uint32_t step, uint32_t percent) {
//  return ProgressStep(step, GetResourceString(IDS_REMOVELINE), percent);
//}
//
//Bool32 ProgressStepSearchTables(uint32_t step, uint32_t percent) {
//  return ProgressStep(step, GetResourceString(IDS_SEARCHTABLE), percent);
//}
//
//Bool32 ProgressStepAutoLayout(uint32_t step, uint32_t percent) {
//  return ProgressStep(step, GetResourceString(IDS_AUTOLAYOUT), percent);
//}
//
//static uint32_t bInitPrgTime = 0;
//void ResetPRGTIME() {
//  bInitPrgTime = 0;
//}
//
//Bool32 DonePRGTIME() {
//  Bool32 rc = FALSE;
//  if (bInitPrgTime)
//      bInitPrgTime--;
//  if (!bInitPrgTime)
//      rc = TRUE;
//  return rc;
//}
//
//Bool32 InitPRGTIME() {
//  Bool32 rc = FALSE;
//  if (!bInitPrgTime) {
//      g_PrgTime.dwBeg = 0;
//      g_PrgTime.dwEnd = 100;
//      g_PrgTime.dwStep = 0;
//      g_PrgTime.name = "";
//      rc = TRUE;
//  }
//  bInitPrgTime++;
//  return rc;
//}
//
//PRGTIME StorePRGTIME(uint32_t beg, uint32_t end) {
//  PRGTIME rc = g_PrgTime;
//
//  uint32_t newBeg = g_PrgTime.dwBeg + (g_PrgTime.dwEnd - g_PrgTime.dwBeg)
//          * beg / 100;
//  uint32_t newEnd = g_PrgTime.dwBeg + (g_PrgTime.dwEnd - g_PrgTime.dwBeg)
//          * end / 100;
//
//  g_PrgTime.dwBeg = newBeg;
//  g_PrgTime.dwEnd = newEnd;
//
//  return rc;
//}
//
//void RestorePRGTIME(PRGTIME prev) {
//  g_PrgTime = prev;
//}
//
//Bool32 HL_TableExtractEx(Handle hPAGE, uint32_t perc, Rect rect) {
//  return RMARKER_SearchTableInZone(hPAGE, hCCOM, perc, rect);
//}

Bool32 IsUpdate(uint32_t flg)
{
    return (g_flgUpdate & flg) > 0;
}

void SetUpdate(uint32_t flgAdd, uint32_t flgRemove)
{
    g_flgUpdate = (g_flgUpdate | flgAdd) & ~flgRemove;
}
