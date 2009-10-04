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

#include "clinefunc.h"

extern int CLINE_Debug;
extern int CLINE_Debug_2;
extern LIST<CHLine>* pLCont;

Bool IfExistContainer(CHLine* pelem)
{
 if(!CLINE_Debug_2)
	 return TRUE;
 CHLine* phline=NULL;
 for(phline=pLCont->GetRoot();phline;phline=pLCont->GetNext(phline))
 {
   if(phline==pelem)
	   return TRUE;
 }
 return FALSE;
}


Bool IfExistLine(CLine* pelem)
{
 if(!CLINE_Debug_2)
	 return TRUE;
 CHLine* phline=NULL;
 CLine* pline=NULL;
 for(phline=pLCont->GetRoot();phline;phline=pLCont->GetNext(phline))
 {
  for(pline=phline->m_line.GetRoot();pline;pline=phline->m_line.GetNext(pline))
  {
   if(pline==pelem)
	   return TRUE;
  }
 }
 return FALSE;
}

Bool IfExistEvent(CEvent* pelem)
{
 if(!CLINE_Debug_2)
	 return TRUE;
 CHLine* phline=NULL;
 CLine* pline=NULL;
 CEvent* pevent=NULL;
 for(phline=pLCont->GetRoot();phline;phline=pLCont->GetNext(phline))
 {
  for(pline=phline->m_line.GetRoot();pline;pline=phline->m_line.GetNext(pline))
  {
   for(pevent=pline->m_event.GetRoot();pevent;pevent=pline->m_event.GetNext(pevent))
   {
    if(pevent==pelem)
	    return TRUE;
   }
  }
 }
 return FALSE;
}

Bool IfExistCutPoint(CCutPoint* pelem)
{
 if(!CLINE_Debug_2)
	 return TRUE;
 CHLine* phline=NULL;
 CLine* pline=NULL;
 CCutPoint* pcupoint=NULL;
 for(phline=pLCont->GetRoot();phline;phline=pLCont->GetNext(phline))
 {
  for(pline=phline->m_line.GetRoot();pline;pline=phline->m_line.GetNext(pline))
  {
   for(pcupoint=pline->m_cut_point.GetRoot();pcupoint;pcupoint=pline->m_cut_point.GetNext(pcupoint))
   {
    if(pcupoint==pelem)
	    return TRUE;
   }
  }
 }
 return FALSE;
}

Bool IfExistComp(CComponent* pelem)
{
 if(!CLINE_Debug_2)
	 return TRUE;
 CHLine* phline=NULL;
 CLine* pline=NULL;
 CComponent* pcomp=NULL;
 for(phline=pLCont->GetRoot();phline;phline=pLCont->GetNext(phline))
 {
  for(pline=phline->m_line.GetRoot();pline;pline=phline->m_line.GetNext(pline))
  {
   for(pcomp=pline->m_comp.GetRoot();pcomp;pcomp=pline->m_comp.GetNext(pcomp))
   {
    if(pcomp==pelem)
	    return TRUE;
   }
  }
 }
 return FALSE;
}


Bool IfExistEventInv(CInterval* pelem)
{
 if(!CLINE_Debug_2)
	 return TRUE;
 CHLine* phline=NULL;
 CLine* pline=NULL;
 CEvent* pevent=NULL;
 CInterval* pinv=NULL;
 for(phline=pLCont->GetRoot();phline;phline=pLCont->GetNext(phline))
 {
  for(pline=phline->m_line.GetRoot();pline;pline=phline->m_line.GetNext(pline))
  {
   for(pevent=pline->m_event.GetRoot();pevent;pevent=pline->m_event.GetNext(pevent))
   {
	for(pinv=pevent->m_interval.GetRoot();pinv;pinv=pevent->m_interval.GetNext(pinv))
	{
     if(pinv==pelem)
	     return TRUE;
	}
   }
  }
 }
 return FALSE;
}

Bool IfExistCutPointInv(CInterval* pelem)
{
 if(!CLINE_Debug_2)
	 return TRUE;
 CHLine* phline=NULL;
 CLine* pline=NULL;
 CCutPoint* pcupoint=NULL;
 CInterval* pinv=NULL;
 for(phline=pLCont->GetRoot();phline;phline=pLCont->GetNext(phline))
 {
  for(pline=phline->m_line.GetRoot();pline;pline=phline->m_line.GetNext(pline))
  {
   for(pcupoint=pline->m_cut_point.GetRoot();pcupoint;pcupoint=pline->m_cut_point.GetNext(pcupoint))
   {
	for(pinv=pcupoint->m_interval.GetRoot();pinv;pinv=pcupoint->m_interval.GetNext(pinv))
	{
     if(pinv==pelem)
	     return TRUE;
	}
   }
  }
 }
 return FALSE;
}



