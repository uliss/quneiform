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
/*  Редакция   :  03.07.00                                                    */
/*  Файл       :  'LineDefs.H'                                                */
/*  Содержание :  Структуры и типы для линий.                                 */
/*  Назначение :  Унификация работы с линиями.                                */
/*----------------------------------------------------------------------------*/
#ifndef __LINEDEFS_H
#define __LINEDEFS_H
#include "cttypes.h"
#include "normdefs.h"


typedef enum
{
	LD_Unknown = 0,
	LD_Horiz,
	LD_Verti,
	LD_Kosa1,
	LD_Kosa2
	/***  вертикальная    :  90' - 22.5' < Skew <  90' + 22.5'  ***/
	/***  горизонтальная  :   0' - 22.5' < Skew <   0' + 22.5'  ***/
	/***  косая-1квадрант :  45' - 22.5' < Skew <  45' + 22.5'  ***/
	/***  косая-2квадрант : 135' - 22.5' < Skew < 135' + 22.5'  ***/
} LINE_DIR;

typedef struct tagLINE_COM
{
	NR_SimpLine     Line;     // простейшее описание линии
	int32_t           Status;   // история линии в программе
	uint32_t          Flags;    // common use info
	NORM_DATATYPES  Type;     //*Unknown, *Simple, *Defis, *Pointed
	LINE_DIR        Dir;
	char            Qual;     // quality (or estimation) at percents
	char            Dens;     // density at percents
	int32_t           nFrag;    // if==-1 - fragmentation was not made
	NR_Event       *pFrag;    // NULL for nFrag<1
	void           *Specline; //(NR_DefiLine), (NR_PoinLine)
	/* не NULL только для *Defis и *Pointed типов, и если информацию положили */
} LINE_COM;

//Line flags (from LNS):
      #define LI_NOISE      0x00000001   // is a noise line
      #define LI_ATTACHED   0x00000002   // Line  confidently
                                         // attached to form element
      #define LI_SMARTSWEEP 0x00000004   // must smartly dissolve it from image
      #define LI_CRUDESWEEP 0x00000008   //
      #define LI_SWEEP (LI_SMARTSWEEP | LI_CRUDESWEEP)
      #define LI_DOT        0x00000010   //
      #define LI_COVERED    0x00000020   // always with LI_NOISE
	/*****  Выставляются верификатором линий.  *****/
		// Либо только один из следующих, либо ни одного. Если ни одного, то
		// либо линия сомнительна, либо не иссдедовалась.
	#define LI_IsTrue       0x00000040
			// правильно выделенная линия
	#define LI_IsFalse      0x00000080
			// не линия
	#define LI_NOTWHOLE     0x00000100
			// Краевые захваты. Реальная линия короче - от Anew до Bnew.
	#define LI_COMPLEX      0x00000200
			// Составная линия : несколько реальных линий, иногда еще и одна
			// или несколько компонент связности. Информация о входящих в ее
			// состав реальных линиях и компонентах отписывается в совершенно
			// независимую структуру.
	#define LI_Pointed      0x00001000
	#define LI_Doubt        0x00002000
	#define LI_Used         0x00004000  // Использована как подчеркивание
 #define LI_FRMT_Used    0x00008000  // Formatter used
	/*****  Выставляются выделителем таблиц.  *****/
	#define LI_IsAtTable    0x00000400
	#define LI_IsNotAtTable 0x00000800
//end Line flags
#endif
