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
/*  правка     :                                                                                   */
/*  Редакция   :  08.09.00                                                                         */
/*  Файл       :  'AboutLines.AboutLin.CPP'                                                        */
/*  Содержание :                                                                                   */
/*  Назначение :                                                                                   */
/*-------------------------------------------------------------------------------------------------*/
#include <stdio.h>
/*#include <windows.h>*/
/*  interface our-other  */
#include "lnsdefs.h"
#include "dpuma.h"
#include "rsdefines.h"
#include "rsmemory.h"
#include "rsfunc.h"
#include "resource.h"
#include "cline.h"
/*  interface our-my     */
/*  interface my        */
#include "un_err.h"
#include "aboutlines.buffer.h"
/*  interface my-my      */
#include "un_buff.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
static char *Buffer = NULL;/*[SizeMyBuff];*/
static char *WorkMem = NULL;/*[SizeWorkMem];*/
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 AboutLines (PRSPreProcessImage Image, Bool32 *BadScan, int32_t *ScanQual)
{
	int SizeMain, SizeWork;
//	char Str[256];
//	Bool ret;
//	LineInfo *pLns;
//	LinesTotalInfo *pLti;
	UN_BUFF MainBuff = {0};
//	void *vLti;
	void *vMain;
	char *cWork;
	Bool32 bRc = TRUE;

	//////////////////////////////////////////////////////////////////////////////////
	Buffer  = (char *)RSTUFFDAlloc(RSTUFF_AboutLines_SizeMyBuff,"Буфер в AboutLines");
	WorkMem = (char *)RSTUFFDAlloc(RSTUFF_AboutLines_SizeWorkMem,"Рабочий буфер в AboutLines");

	if (Buffer == NULL || WorkMem == NULL )
	{
		SetReturnCode_rstuff((uint16_t)IDS_RSTUFF_ERR_NO_MEMORY);
		bRc =  FALSE;
	}

	if ( bRc )
		do
		{
			//////////////////////////////////////////////////////////////////////////////////////
			/*  1. Контроль.  */
			if ((Image->pgneed_clean_line==NULL)&&(BadScan!=NULL))
				break;
				//return TRUE;

			/*  2. Инициализация.  */
			vMain = Buffer;
			SizeMain = RSTUFF_AboutLines_SizeMyBuff;
			MainBuff.vBuff    = vMain;
			MainBuff.SizeBuff = SizeMain;
			MainBuff.vCurr    = MainBuff.vBuff;
			MainBuff.SizeCurr = MainBuff.SizeBuff;
			cWork = WorkMem;
			SizeWork = RSTUFF_AboutLines_SizeWorkMem;

//		   extern Handle hUseCLine;
/*		   if(LDPUMA_Skip(hUseCLine))
		   {
			//  3. Загружаем первичные данные - линии.
			ret = LoadData_rv (Image->hCPAGE, UN_LD_LinesVP, (void *)(&MainBuff), Str, 0);
			if ((ret!=RV_TRUE)&&(ret!=RV_EMPTY))
			{
				if (1)
					LDPUMA_ConsoleN (Str);

				bRc = FALSE;
				break;
				//return FALSE;
			}
			if (ret==RV_EMPTY)
			{
				if (Image->pgneed_clean_line != NULL)
				{
					*Image->pgneed_clean_line = FALSE;
					if (1)
						LDPUMA_ConsoleN ("RSource: Не надо снимать линии!");
				}
				if (BadScan!=NULL)
				{
					if (1)
						LDPUMA_ConsoleN ("RSource: Качество сканирования : не знаю, какое.");
					*BadScan = TRUE;
					*ScanQual = 100;
				}
				break;
				//return TRUE;
			}
			if (Image->pgneed_clean_line!=NULL)
			{
				*Image->pgneed_clean_line = FALSE;
				ret = GetComplexData_rv (UN_LD_LinesVP, UN_DA_Unknown
					, (void *)(&MainBuff), &vLti);
				if ((ret!=RV_TRUE)&&(ret!=RV_EMPTY))
				{
					bRc = FALSE;
					break;
					//return FALSE;
				}
				pLti = (LinesTotalInfo *)vLti;
				n = pLti->Hor.Cnt;
				pLns = pLti->Hor.Lns;
				for (i=0; i<n; i++)
				{
					if (pLns[i].Flags & LI_IsTrue)
						*Image->pgneed_clean_line = TRUE;
				}
				n = pLti->Ver.Cnt;
				pLns = pLti->Ver.Lns;
				for (i=0; i<n; i++)
				{
					if (pLns[i].Flags & LI_IsTrue)
						*Image->pgneed_clean_line = TRUE;
				}
				if (1)
				{
					if (*Image->pgneed_clean_line)
						LDPUMA_ConsoleN ("RSource: Нужно снять линии.");
					else
						LDPUMA_ConsoleN ("RSource: Не надо снимать линии!");
				}
			}
		   }
		   else
		   {*/
			if (Image->pgneed_clean_line!=NULL)
			{
				*Image->pgneed_clean_line = FALSE;
				CLINE_handle hCLINE=*((CLINE_handle*)(Image->phCLINE));
				Bool fl_break=FALSE;
				for(CLINE_handle hline=CLINE_GetFirstLine(hCLINE);hline;hline=CLINE_GetNextLine(hline))
				{
					CPDLine cpdata=CLINE_GetLineData(hline);
					if(!cpdata)
						continue;
					if(cpdata->Flags&LI_IsTrue)
					{
						*Image->pgneed_clean_line=TRUE;
						fl_break=TRUE;
					}
					if(fl_break)
						break;
				}
				if (1)
				{
					if (*Image->pgneed_clean_line)
						LDPUMA_ConsoleN ("RSource: Нужно снять линии.");
					else
						LDPUMA_ConsoleN ("RSource: Не надо снимать линии!");
				}
			}
//		   }

			if (BadScan!=NULL)
			{
				if (1)
					LDPUMA_ConsoleN ("RSource: Качество сканирования : не умею пока определять.");
				*BadScan = TRUE;
				*ScanQual = 100;
			}
		} while ( false );

	RSTUFFFree(Buffer);
	RSTUFFFree(WorkMem);

	return bRc;
}
/*----------------------------------------------------------------------------*/
