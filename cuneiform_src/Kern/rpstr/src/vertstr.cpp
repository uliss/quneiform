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

// FIXME: disabled everything Windows-related.

//#include <windows.h>
#include <stdlib.h>
#include "rpstr.h"

#include "cstr.h"
#include "dpuma.h"

void DrawRect(Handle wnd,uint32_t OperCode,uint32_t color,int top,int bottom,int left,int right);

extern "C" Handle hVertCompD;
//Bool dpVertCompD;

RPSTR_FUNC(void) RPSTR_NormalizeVertStr(void)
{

#if 0
 Handle MainWindowD=NULL;
 MainWindowD=LDPUMA_GetWindowHandle ("Изображение после разворота");
 if(!MainWindowD)
    MainWindowD=LDPUMA_GetWindowHandle ("Main");

 dpVertCompD=!LDPUMA_Skip(hVertCompD);

 int32_t version=0;
 int count_str=CSTR_GetMaxNumber();
 if(!count_str)
	 return;

 Bool WasDraw=FALSE;
 CSTR_line string;
 CSTR_rast rast;
 CSTR_attr attr;
 CSTR_rast_attr r_attr;
 int delta_col;
 int temp;
 uint32_t type=0;
 type=~type;

 string=CSTR_FirstLine(version);
 while(string)
 {
	 version++;
	 string=CSTR_FirstLine(version);
 }
 version--;

 for(string=CSTR_FirstLine(version);string;string=CSTR_NextLine(string,version))
 {
  if(!CSTR_GetLineAttr(string,&attr))
	  continue;

  if(attr.Flags&CSTR_STR_DOWNUP)
  {
    for(rast=CSTR_GetFirstRaster(string);rast;rast=CSTR_GetNextRaster(rast,type))
	{
	  if(!CSTR_GetAttr(rast,&r_attr))
		  continue;
      if(r_attr.flg&CSTR_f_fict)
		  continue;

      delta_col=r_attr.col-attr.col;
	  r_attr.col=attr.col-delta_col;

      delta_col=r_attr.r_col-attr.r_col;
	  r_attr.r_col=attr.r_col-delta_col;

	  temp=r_attr.col;
	  r_attr.col=r_attr.row;
	  r_attr.row=temp;
      temp=r_attr.r_col;
	  r_attr.r_col=r_attr.r_row;
	  r_attr.r_row=temp;

	  if(dpVertCompD)
	  {
       WasDraw=TRUE;
	   if(attr.Flags&CSTR_STR_UPDOWN)
       	  DrawRect(MainWindowD,117,RGB(0,0,255),r_attr.r_row,r_attr.r_row+r_attr.w-1,r_attr.r_col-r_attr.h+1,r_attr.r_col);
       else
	      DrawRect(MainWindowD,117,RGB(0,0,255),r_attr.r_row-r_attr.w+1,r_attr.r_row,r_attr.r_col,r_attr.r_col+r_attr.h-1);
	  }

	  CSTR_SetAttr(rast,&r_attr);
	}

	temp=attr.col;
	attr.col=attr.row;
	attr.row=temp;
    temp=attr.r_col;
	attr.r_col=attr.r_row;
	attr.r_row=temp;
	CSTR_SetLineAttr(string,&attr);
  }

  if(attr.Flags&CSTR_STR_UPDOWN)
  {
    for(rast=CSTR_GetFirstRaster(string);rast;rast=CSTR_GetNextRaster(rast,type))
	{
	  if(!CSTR_GetAttr(rast,&r_attr))
		  continue;
      if(r_attr.flg&CSTR_f_fict)
		  continue;

      r_attr.row+=r_attr.h-1;
	  r_attr.r_row+=r_attr.h-1;

      temp=r_attr.col;
	  r_attr.col=r_attr.row;
	  r_attr.row=temp;
      temp=r_attr.r_col;
	  r_attr.r_col=r_attr.r_row;
	  r_attr.r_row=temp;

      if(dpVertCompD)
	  {
       WasDraw=TRUE;
	   if(attr.Flags&CSTR_STR_UPDOWN)
       	  DrawRect(MainWindowD,117,RGB(0,0,255),r_attr.r_row,r_attr.r_row+r_attr.w-1,r_attr.r_col-r_attr.h+1,r_attr.r_col);
       else
	      DrawRect(MainWindowD,117,RGB(0,0,255),r_attr.r_row-r_attr.w+1,r_attr.r_row,r_attr.r_col,r_attr.r_col+r_attr.h-1);
	  }

	  CSTR_SetAttr(rast,&r_attr);
	}

    attr.row+=r_attr.h-1;
	attr.r_row+=r_attr.h-1;

    temp=attr.col;
	attr.col=attr.row;
	attr.row=temp;
    temp=attr.r_col;
	attr.r_col=attr.r_row;
	attr.r_row=temp;
	CSTR_SetLineAttr(string,&attr);
  }
 }
 if(dpVertCompD&&WasDraw)
 {
	 LDPUMA_WaitUserInput(hVertCompD,MainWindowD);
	 LDPUMA_DeleteRects(MainWindowD,117);
 }
#endif
}

void DrawRect(Handle wnd,uint32_t OperCode,uint32_t color,int top,int bottom,int left,int right)
{

	Rect16 Rect;

		Rect.bottom=bottom;
		Rect.left=left;
		Rect.right=right+1;
		Rect.top=top-1;

		LDPUMA_DrawRect (wnd, &(Rect),0, color, -100,OperCode );

}


