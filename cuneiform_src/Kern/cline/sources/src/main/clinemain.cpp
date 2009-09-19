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
#include "clinefunc.h"
#include "dpuma.h"

extern LIST<CHLine>* pLCont;
extern CHLine* pMainCont;
extern int size_inv;
extern int size_event;
extern int size_cupoint;
extern int size_comp;
extern int size_line;
extern Handle MemStat;
extern CAllMem* pMyMem;
extern int len_inv_mas;
extern int len_event_mas;
extern int len_cupoint_mas;
extern int len_comp_mas;
extern int len_line_mas;
extern int len_hline_mas;
extern int size_class_inv;
extern int size_class_event;
extern int size_class_cupoint;
extern int size_class_comp;
extern int size_class_line;
extern int size_class_hline;

CLINE_FUNC void CLINE_Reset() {
	if (!LDPUMA_Skip(MemStat) && LDPUMA_IsActive()) {
		FILE* f = fopen("clstat.res", "w");
		fprintf(f, "Interval -   %d\n", pMyMem->pInvMas.m_count * len_inv_mas);
		fprintf(f, "Event    -   %d\n", pMyMem->pEventMas.m_count
				* len_event_mas);
		fprintf(f, "Comp     -   %d\n", pMyMem->pCompMas.m_count * len_comp_mas);
		fprintf(f, "Cupoint  -   %d\n", pMyMem->pCupointMas.m_count
				* len_cupoint_mas);
		fprintf(f, "Line     -   %d\n", pMyMem->pLineMas.m_count * len_line_mas);
		fprintf(f, "HLine    -   %d\n", pMyMem->pHLineMas.m_count
				* len_hline_mas);
		fprintf(f, "All Mem  -   %d\n", pMyMem->pInvMas.m_count * len_inv_mas
				* size_class_inv + pMyMem->pEventMas.m_count * len_event_mas
				* size_class_event + pMyMem->pCompMas.m_count * len_comp_mas
				* size_class_comp + pMyMem->pCupointMas.m_count
				* len_cupoint_mas * size_class_cupoint
				+ pMyMem->pLineMas.m_count * len_line_mas * size_class_line
				+ pMyMem->pHLineMas.m_count * len_hline_mas * size_class_hline);
		fclose(f);
	}

	pLCont->FastDel();
	pMainCont = NULL;
}

CLINE_FUNC CLINE_handle CLINE_CreateContainer(Bool Main) {
	if (Main) {
		pMainCont = pLCont->Add();
		return (CLINE_handle)(pMainCont);
	}
	return (CLINE_handle)(pLCont->Add());
}

CLINE_FUNC void CLINE_DeleteContainer(CLINE_handle handle) {
	if (!handle)
		return;
	if (handle == (CLINE_handle)(pMainCont))
		pMainCont = NULL;
	pLCont->Del((CHLine*) (handle));
}

CLINE_FUNC CLINE_handle CLINE_GetFirstContainer() {
	return (CLINE_handle)(pLCont->GetRoot());
}

CLINE_FUNC CLINE_handle CLINE_GetNextContainer(CLINE_handle container) {
	if (!container)
		return 0;
	CHLine* pcont = (CHLine*) (container);
	// _ASSERT(IfExistContainer(pcont));
	return (CLINE_handle)(pcont->next);
}

CLINE_FUNC CLINE_handle CLINE_GetMainContainer() {
	return (CLINE_handle)(pMainCont);
}

CLINE_FUNC void CLINE_CleanContainer(CLINE_handle container) {
	CLINE_DelAllLines(container);
}

CLINE_FUNC int CLINE_GetLineCount(CLINE_handle container) {
	if (!container)
		return 0;
	CHLine* phline = (CHLine*) (container);
	// _ASSERT( IfExistContainer(phline) );
	return phline->m_line.m_count;
}

CLINE_FUNC Bool32 CLINE_MoveLine(CLINE_handle container_to,
		CLINE_handle container_from, CLINE_handle line) {
	if (!container_from || !container_to || !line)
		return FALSE;

	CHLine* phline_from = (CHLine*) (container_from);
	CHLine* phline_to = (CHLine*) (container_to);
	CLine* pline = (CLine*) (line);

	if (!phline_from->m_line.Detach(pline))
		return FALSE;

	phline_to->m_line.Attach(pline);

	return TRUE;
}

CLINE_FUNC CLINE_handle CLINE_GetFirstLine(CLINE_handle container) {
	if (!container)
		return 0;
	CHLine* phline = (CHLine*) (container);
	// _ASSERT( IfExistContainer(phline) );
	return (CLINE_handle)(phline->m_line.GetRoot());
}

CLINE_FUNC CLINE_handle CLINE_GetNextLine(CLINE_handle prev_line)
{
	if(!prev_line)
	return 0;
	CLine* pline=(CLine*)(prev_line);
	// _ASSERT( IfExistLine(pline) );
	return (CLINE_handle)( pline->next );
}

CLINE_FUNC CLINE_handle CLINE_AddNewLine(CLINE_handle container)
{
	if(!container)
	return 0;
	CHLine* phline;
	phline=(CHLine*)(container);
	// _ASSERT( IfExistContainer(phline) );
	CLine* pline=phline->m_line.Add();
	return (CLINE_handle)(pline);
}

CLINE_FUNC void CLINE_DelLine(CLINE_handle container,CLINE_handle line)
{
	if(!container||!line)
	return;
	CHLine* phline=(CHLine*)(container);
	// _ASSERT( IfExistContainer(phline) );
	CLine* pline=(CLine*)(line);
	phline->m_line.Del( pline );
}

CLINE_FUNC void CLINE_DelAllLines(CLINE_handle container)
{
	if(!container)
	return;
	CHLine* phline=(CHLine*)(container);
	// _ASSERT( IfExistContainer(phline) );
	phline->m_line.FastDel();
}

CLINE_FUNC CPDLine CLINE_GetLineData(CLINE_handle line)
{
	if(!line)
	return NULL;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	return (CPDLine)(&(pline->data));
}

CLINE_FUNC Bool32 CLINE_SetLineData(CLINE_handle line,CPDLine cpdata)
{
	if(!line||!cpdata)
	return FALSE;
	CLine* pline=(CLine*)((line));
	/// _ASSERT( IfExistLine(pline) );
	return CopyData(cpdata,&(pline->data),size_line);
}

CLINE_FUNC int CLINE_GetEventCount(CLINE_handle line)
{
	if(!line)
	return 0;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	return pline->m_event.m_count;
}

CLINE_FUNC CLINE_handle CLINE_GetFirstEvent(CLINE_handle line)
{
	if(!line)
	return 0;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	return (CLINE_handle)( pline->m_event.GetRoot() );
}

CLINE_FUNC CLINE_handle CLINE_GetNextEvent(CLINE_handle event)
{
	if(!event)
	return 0;
	CEvent* pevent=(CEvent*)((event));
	// _ASSERT( IfExistEvent(pevent) );
	return (CLINE_handle)( pevent->next );
}

CLINE_FUNC CLINE_handle CLINE_AddNewEvent(CLINE_handle line)
{
	if(!line)
	return 0;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	CLINE_handle hevent=(CLINE_handle)( pline->m_event.Add() );
	pline->data.n_event=pline->m_event.m_count;
	return hevent;
}

CLINE_FUNC void CLINE_DelEvent(CLINE_handle line,CLINE_handle event)
{
	if(!line||!event)
	return;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	CEvent* pevent=(CEvent*)((event));
	pline->m_event.Del(pevent);
	pline->data.n_event=pline->m_event.m_count;
}

CLINE_FUNC void CLINE_DelAllEvents(CLINE_handle line)
{
	if(!line)
	return;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	pline->m_event.FastDel();
	pline->data.n_event=pline->m_event.m_count;
}

CLINE_FUNC CPDEvent CLINE_GetEventData(CLINE_handle event)
{
	if(!event)
	return 0;
	CEvent* pevent=(CEvent*)((event));
	// _ASSERT( IfExistEvent(pevent) );
	return (CPDEvent)(&(pevent->data));
}

CLINE_FUNC Bool32 CLINE_SetEventData(CLINE_handle event,CPDEvent cpdata)
{
	if(!event)
	return 0;
	CEvent* pevent=(CEvent*)((event));
	// _ASSERT( IfExistEvent(pevent) );
	return CopyData(cpdata,&(pevent->data),size_event);
}

CLINE_FUNC int CLINE_GetCutPointCount(CLINE_handle line)
{
	if(!line)
	return 0;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	return pline->m_cut_point.m_count;
}

CLINE_FUNC CLINE_handle CLINE_GetFirstCutPoint(CLINE_handle line)
{
	if(!line)
	return 0;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	return (CLINE_handle)( pline->m_cut_point.GetRoot() );
}

CLINE_FUNC CLINE_handle CLINE_GetNextCutPoint(CLINE_handle cupoint)
{
	if(!cupoint)
	return 0;
	CCutPoint* pcupoint=(CCutPoint*)((cupoint));
	// _ASSERT( IfExistCutPoint(pcupoint) );
	return (CLINE_handle)( pcupoint->next );
}

CLINE_FUNC CLINE_handle CLINE_AddNewCutPoint(CLINE_handle line)
{
	if(!line)
	return 0;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	return (CLINE_handle)( pline->m_cut_point.Add() );
}

CLINE_FUNC void CLINE_DelCutPoint(CLINE_handle line,CLINE_handle cupoint)
{
	if(!line||!cupoint)
	return;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	CCutPoint* pcupoint=(CCutPoint*)((cupoint));
	pline->m_cut_point.Del(pcupoint);
}

CLINE_FUNC void CLINE_DelAllCutPoints(CLINE_handle line)
{
	if(!line)
	return;
	CLine* pline=(CLine*)((line));
	/// _ASSERT( IfExistLine(pline) );
	pline->m_cut_point.FastDel();
}

CLINE_FUNC CPDCutPoint CLINE_GetCutPointData(CLINE_handle cupoint)
{
	if(!cupoint)
	return 0;
	CCutPoint* pcupoint=(CCutPoint*)((cupoint));
	// _ASSERT( IfExistCutPoint(pcupoint) );
	return (CPDCutPoint)(&(pcupoint->data));
}

CLINE_FUNC Bool32 CLINE_SetCutPointData(CLINE_handle cupoint,CPDCutPoint cpdata)
{
	if(!cupoint)
	return 0;
	CCutPoint* pcupoint=(CCutPoint*)((cupoint));
	// _ASSERT( IfExistCutPoint(pcupoint) );
	return CopyData(cpdata,&(pcupoint->data),size_cupoint);
}

CLINE_FUNC int CLINE_GetCompCount(CLINE_handle line)
{
	if(!line)
	return 0;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	return pline->m_comp.m_count;
}

CLINE_FUNC CLINE_handle CLINE_GetFirstComp(CLINE_handle line)
{
	if(!line)
	return 0;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	return (CLINE_handle)( pline->m_comp.GetRoot() );
}

CLINE_FUNC CLINE_handle CLINE_GetNextComp(CLINE_handle comp)
{
	if(!comp)
	return 0;
	CComponent* pcomp=(CComponent*)((comp));
	/// _ASSERT( IfExistComp(pcomp) );
	return (CLINE_handle)( pcomp->next );
}

CLINE_FUNC CLINE_handle CLINE_AddNewComp(CLINE_handle line)
{
	if(!line)
	return 0;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	return (CLINE_handle)( pline->m_comp.Add() );
}

CLINE_FUNC void CLINE_DelComp(CLINE_handle line,CLINE_handle comp)
{
	if(!line||!comp)
	return;
	CLine* pline=(CLine*)((line));
	// _ASSERT( IfExistLine(pline) );
	CComponent* pcomp=(CComponent*)((comp));
	pline->m_comp.Del(pcomp);
}

CLINE_FUNC void CLINE_DelAllComps(CLINE_handle line)
{
	if(!line)
	return;
	CLine* pline=(CLine*)((line));
	/// _ASSERT( IfExistLine(pline) );
	pline->m_comp.FastDel();
}

CLINE_FUNC CPDComponent CLINE_GetCompData(CLINE_handle comp)
{
	if(!comp)
	return 0;
	CComponent* pcomp=(CComponent*)((comp));
	// _ASSERT( IfExistComp(pcomp) );
	return (CPDComponent)(&(pcomp->data));
}

CLINE_FUNC Bool32 CLINE_SetCompData(CLINE_handle comp,CPDComponent cpdata)
{
	if(!comp)
	return 0;
	CComponent* pcomp=(CComponent*)((comp));
	// _ASSERT( IfExistComp(pcomp) );
	return CopyData(cpdata,&(pcomp->data),size_comp);
}

CLINE_FUNC int CLINE_GetEventInvCount(CLINE_handle event)
{
	if(!event)
	return 0;
	CEvent* pevent=(CEvent*)((event));
	// _ASSERT( IfExistLine(pline) );
	return pevent->m_interval.m_count;
}

CLINE_FUNC CLINE_handle CLINE_GetFirstEventInv(CLINE_handle event)
{
	if(!event)
	return 0;
	CEvent* pevent=(CEvent*)((event));
	// _ASSERT( IfExistEvent(pevent) );
	return (CLINE_handle)( pevent->m_interval.GetRoot() );
}

CLINE_FUNC CLINE_handle CLINE_GetNextEventInv(CLINE_handle inv)
{
	if(!inv)
	return 0;
	CInterval* pinv=(CInterval*)((inv));
	// _ASSERT( IfExistEventInv(pinv) );
	return (CLINE_handle)( pinv->next );
}

CLINE_FUNC CLINE_handle CLINE_AddNewEventInv(CLINE_handle event)
{
	if(!event)
	return 0;
	CEvent* pevent=(CEvent*)((event));
	// _ASSERT( IfExistEvent(pevent) );
	return (CLINE_handle)( pevent->m_interval.Add() );
}

CLINE_FUNC void CLINE_DelEventInv(CLINE_handle event,CLINE_handle inv)
{
	if(!event||!inv)
	return;
	CEvent* pevent=(CEvent*)((event));
	// _ASSERT( IfExistEvent(pevent) );
	CInterval* pinv=(CInterval*)((inv));
	pevent->m_interval.Del(pinv);
}

CLINE_FUNC void CLINE_DelAllEventInvs(CLINE_handle event)
{
	if(!event)
	return;
	CEvent* pevent=(CEvent*)((event));
	// _ASSERT( IfExistEvent(pevent) );
	pevent->m_interval.FastDel();
}

CLINE_FUNC CPDInterval CLINE_GetEventInvData(CLINE_handle inv)
{
	if(!inv)
	return 0;
	CInterval* pinv=(CInterval*)((inv));
	// _ASSERT( IfExistEventInv(pinv) );
	return (CPDInterval)(&(pinv->data));
}

CLINE_FUNC Bool32 CLINE_SetEventInvData(CLINE_handle inv,CPDInterval cpdata)
{
	if(!inv)
	return 0;
	CInterval* pinv=(CInterval*)((inv));
	// _ASSERT( IfExistEventInv(pinv) );
	return CopyData(cpdata,&(pinv->data),size_inv);
}

CLINE_FUNC int CLINE_GetCutPointInvCount(CLINE_handle cupoint)
{
	if(!cupoint)
	return 0;
	CCutPoint* pcupoint=(CCutPoint*)((cupoint));
	// _ASSERT( IfExistLine(pline) );
	return pcupoint->m_interval.m_count;
}

CLINE_FUNC CLINE_handle CLINE_GetFirstCutPointInv(CLINE_handle cupoint)
{
	if(!cupoint)
	return 0;
	CCutPoint* pcupoint=(CCutPoint*)((cupoint));
	// _ASSERT( IfExistCutPoint(pcupoint) );
	return (CLINE_handle)( pcupoint->m_interval.GetRoot() );
}

CLINE_FUNC CLINE_handle CLINE_GetNextCutPointInv(CLINE_handle inv)
{
	if(!inv)
	return 0;
	CInterval* pinv=(CInterval*)((inv));
	/// _ASSERT( IfExistCutPointInv(pinv) );
	return (CLINE_handle)( pinv->next );
}

CLINE_FUNC CLINE_handle CLINE_AddNewCutPointInv(CLINE_handle cupoint)
{
	if(!cupoint)
	return 0;
	CCutPoint* pcupoint=(CCutPoint*)((cupoint));
	// _ASSERT( IfExistCutPoint(pcupoint) );
	return (CLINE_handle)( pcupoint->m_interval.Add() );
}

CLINE_FUNC void CLINE_DelCutPointInv(CLINE_handle cupoint,CLINE_handle inv)
{
	if(!cupoint||!inv)
	return;
	CCutPoint* pcupoint=(CCutPoint*)((cupoint));
	// _ASSERT( IfExistCutPoint(pcupoint) );
	CInterval* pinv=(CInterval*)((inv));
	pcupoint->m_interval.Del(pinv);
}

CLINE_FUNC void CLINE_DelAllCutPointInvs(CLINE_handle cupoint)
{
	if(!cupoint)
	return;
	CCutPoint* pcupoint=(CCutPoint*)((cupoint));
	/// _ASSERT( IfExistCutPoint(pcupoint) );
	pcupoint->m_interval.FastDel();
}

CLINE_FUNC CPDInterval CLINE_GetCutPointInvData(CLINE_handle inv)
{
	if(!inv)
	return 0;
	CInterval* pinv=(CInterval*)((inv));
	// _ASSERT( IfExistCutPointInv(pinv) );
	return (CPDInterval)(&(pinv->data));
}

CLINE_FUNC Bool32 CLINE_SetCutPointInvData(CLINE_handle inv,CPDInterval cpdata)
{
	if(!inv)
	return 0;
	CInterval* pinv=(CInterval*)((inv));
	// _ASSERT( IfExistCutPointInv(pinv) );
	return CopyData(cpdata,&(pinv->data),size_inv);
}

CLINE_FUNC Bool32 CLINE_CopyData(void* to,const void* from,int size)
{
	return CopyData(from,to,size);
}
