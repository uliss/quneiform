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

#ifndef __PMONITOR_H
#define __PMONITOR_H
//
// PMONITOR.H - prototypes for progress monitor callbacks set.
//

#ifndef __GLOBUS_H
#include "Globus.h"
#endif

typedef Bool   ( *TPrgStart     )  (void);
typedef Bool   ( *TPrgFinish    )  (void);
typedef Bool   ( *TPrgNewStage  )  (int16_t StageId, char * szStage, int16_t stage_volume);
typedef Bool   ( *TPrgSetPercent)  (int16_t prc);
typedef Bool   ( *TPrgMessageBox)  (char  * szMessage, uint32_t *flags); // use "wmb.h" to obtain flags

typedef struct tagPrgSet
 {
   TPrgStart            Start; // Must  present
   TPrgFinish          Finish; // Must  present
   TPrgNewStage      NewStage; // Must  present
   TPrgSetPercent  SetPercent; // Must  present
   TPrgMessageBox      MsgBox; // May be NULL.
 } TPrgSet;

_SETTYPES(TPrgSet);

//
//  The brief description of Progress Monitor functions.
//
//1. Bool   ProgressStart  (void) - function starts progress monitor.
//2. Bool   ProgressFinish (void) - .  .  .  finishes  .  .   .  .
//3. Bool   ProgressNewStage    (int16_t StageId, char * szStage, int16_t stage_volume) -
//            new stage beginning.
//       Parameters:
//          StageId     - id of new stage in termes of server states.
//         szStage      - stage name;
//         stage_volume - the volume of this stage in the whole process
//                       (where the whole process is 0...100 range).
//4. Bool   ProgressSetPercent  (int16_t prc) -sets the progress monitor to
//                      prc value (prc should be in 0...100 range relatively
//                      to current stage). Actually progress monitor will
//                      normalize this value to WholeProcessMonitor as
//               WholeProcessMonitor =CurrStageStart +prc *stage_volume /100
//                      where CurrStageStart calculated automatically.
//         Return code:
//               TRUE - if all is OK or
//               FALSE - if user interrupted process.
//
//5. Bool   ProgressMessageBox  (char  * szMessage, uint32_t *flags) -
//                       frankly speaking it is not progress function,
//                       it is just Win like MessageBox.
//        Parameters:
//          szMessage  - message,
//          flags  - on entry: Win-like styles for MessageBox (MB_YESNO, etc.)
//                   on exit:  Win-like return value (IDOK, IDCANSEL, etc.)

#endif // __PMONITOR_H
