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


# ifndef __CRI_CONTROL_H_
# define __CRI_CONTROL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"
#include "cridefines.h"
#include "cttypes.h"
#include "criimage.h"
#include "ctiimage.h"
#include "crinvertor.h"	// Added by ClassView
#include "ctdib.h"	// Added by ClassView
#include "rprogressor.h"	// Added by ClassView
#include "cribinarizator.h"
#include "crturner.h"
#include "crinvertor.h"
#include "crrotator.h"
#include "cttypes.h"	// Added by ClassView

class CRIControl
{
private:
	Handle           mhOpenedDIB;
	Handle           mhCreatedDIB;
	// pointer to class CTDIB
	PCTDIB           mpSourceDIB;
	PCTDIB           mpDestinationDIB;


protected:
	// Name of last processed image
	char                     mcLastDIBName[256];
	//
	void *                    mp_TurnedDIB;
	// pointer to class ProgressShow
	CRProgressor              mcProgress;
	// pointer to class Binarizator
	PCRIBinarizator           mpBinarizator;
	// pointer to clas  Invertor
	PCRInvertor               mpInvertor;
	// pointer to class Turn
	PCRTurner                 mpTurner;
	// pointer to class Rotate
	PCRRotator                mpRotator;

public:
	CRIControl();
	~CRIControl();

private:
	Bool32          DIBOpeningType;
	RIMAGEMARGINS   mrMargins;
	Bool32          mbMarginsFlag;

private:
	Bool32          WriteDIBtoBMP(const char *cName, PCTDIB pDIB);
	Bool32          GetDIB(char*   cDIB, Handle* phDIB);
	Bool32          CloseSourceDIB();
	Bool32          CreateDestinatonDIB(uint32_t BitCount);
	Bool32          SetDestinationDIBtoStorage(char*  cDIBName);
	Bool32          OpenDestinationDIBfromSource(char*  cSDIB);
	Bool32          CloseDestinationDIB(char*   cDIBName);
	Bool32          OpenSourceDIB(char*   cDIBName);
	Bool32          SetDIB(char*   cDIB, Handle hDIB);
	Bool32          WriteDIB(char*   cDIB, Handle hDIB);
	Bool32          ReadDIB(char*   cDIB, Handle* phDIB);

public:
	Bool32                    RotatePoint(char* cDIB, int32_t iX, int32_t iY, int32_t * prX, int32_t * prY);
	Bool32                    StartProgress(void);
	Bool32                    SetProgressCallBacks(PRIMAGECBPRogressStart pcbStart, PRIMAGECBPRogressStep pcbStep, PRIMAGECBPRogressFinish pcbFinish);
	Bool32                    SetMargins(PRIMAGEMARGINS pMargins);
	Bool32                    Binarise(char*   cDIBIn, char*   cDIBOut, uint32_t wFlag, uint32_t UseMargins);
	Bool32                    Rotate(char*   cDIBIn, char*   cDIBOut, int32_t High, int32_t Low, uint32_t UseMargins);
	Bool32                    Roll(char* cDIBIn, char* cDIBOut, int32_t Num, int32_t Denum, uint32_t bUseMargins);
	Bool32                    Turn(char*   cDIBIn, char*   cDIBOut, uint32_t wFlag, uint32_t UseMargins);
	Bool32                    Inverse(char*   cDIBIn, char*   cDIBOut, uint32_t UseMargins);
};
# endif    //__CRI_CONTROL_H_
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
