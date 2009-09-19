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

/*#include <windows.h>*/
#include <setjmp.h>
#include <assert.h>

#include "resource.h"
#include "rblock.h"
#include "mymem.h"
#include "newfunc.h"
#include "dpuma.h"
#include "ccom.h"
#include "cline.h"
#include "cpage.h"

//# include "new_c.h"
#include "compat_defs.h"
////////////// KERNEL //////////////////

extern "C"
{
	void PageLayout (void);
	void progress_finish(void);
	void EnableDebug( void );

	jmp_buf fatal_error_exit;	// For error handling
	int16_t nStrings;
	CLINE_handle HCLINE=0;
	int16_t nIncline;
}

Handle MainWindowD;





//###########################################
RBLOCK_FUNC(Bool32) RBLOCK_ExtractTextBlocks(Handle hCCOM,Handle hCPAGE,CLINE_handle hCLINE)
{
  SetReturnCode_rblock(IDS_ERR_NO);
  Open_Res_Log();

  MainWindowD=NULL;
  MainWindowD=LDPUMA_GetWindowHandle ("Изображение после разворота");
  if(!MainWindowD)
    MainWindowD=LDPUMA_GetWindowHandle ("Main");
  HCLINE=hCLINE;

  PAGEINFO info={0};
  if(GetPageInfo(hCPAGE,&info))
	  nIncline=info.Incline2048;
  else
	  nIncline=0;

  if (setjmp (fatal_error_exit))
	{
		  progress_finish();
		  return FALSE;
	}

  EnableDebug();
  PageLayoutBlocks(hCCOM);
  OutputFragments(hCPAGE);
  Close_Res_Log();

  return TRUE;
}

////////////////////////////////////////////////
RBLOCK_FUNC(Bool32) RBLOCK_ExtractTextStrings(Handle hCCOM,Handle hCPAGE)
{
  SetReturnCode_rblock(IDS_ERR_NO);
  Open_Res_Log();

  if (setjmp (fatal_error_exit))
	{
		  progress_finish();
		  return FALSE;
	}

  nStrings = 0;
  EnableDebug();
  PageLayoutStrings(hCCOM,hCPAGE);
  Close_Res_Log();

  return TRUE;
}
////////////////////////////////////////////////
RBLOCK_FUNC(Bool32) RBLOCK_GetAnglePage(Handle hCCOM,int32_t * lpNominator,int32_t * lpDenominator)
{
 Bool32 rc = TRUE;
 SetReturnCode_rblock(IDS_ERR_NO);
 Open_Res_Log();

  if (setjmp (fatal_error_exit))
	{
		  progress_finish();
		  return FALSE;
	}
  EnableDebug();
  _ASSERT(lpNominator);
  _ASSERT(lpDenominator);
  _ASSERT(hCCOM);
  CalculatePageIncline(hCCOM, lpNominator,lpDenominator);
  Close_Res_Log();

  return rc;
}
