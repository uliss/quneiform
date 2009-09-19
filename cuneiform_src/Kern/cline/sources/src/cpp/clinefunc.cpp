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

#include<stdio.h>
#include<string.h>
#include "clinefunc.h"
#include "dpuma.h"
#include "compat_defs.h"

extern CLINE_handle hcode;
extern Handle MemStat;
#define ADD_LEN_MAS   50

extern int size_class_inv;
extern int size_class_event;
extern int size_class_cupoint;
extern int size_class_comp;
extern int size_class_line;
extern int size_class_hline;
extern int len_inv_mas;
extern int len_event_mas;
extern int len_cupoint_mas;
extern int len_comp_mas;
extern int len_line_mas;
extern int len_hline_mas;
extern CAllMem* pMyMem;
extern CInterval* pInvMem;
extern CEvent* pEventMem;
extern CCutPoint* pCupointMem;
extern CComponent* pCompMem;
extern CLine* pLineMem;
extern CHLine* pHLineMem;

extern LIST<CHLine>* pLCont;

CLINE_handle GetHandle(void* lp) {
	return (CLINE_handle)(lp);
}

void* GetLP(CLINE_handle handle) {
	return (void*) handle;
}

Bool CopyData(const void* from, void* to, int size) {
#ifdef _DEBUG
	if(!from||!to||(size<0))
	return FALSE;
	if(!size)
	return TRUE;
#endif
	if (!memcpy(to, from, size))
		return FALSE;
	return TRUE;
}

void DeleteData(void) {
	/*	if(!LDPUMA_Skip(MemStat) && LDPUMA_IsActive())
	 {
	 FILE* f=fopen("clstat.res","w");
	 fprintf(f,"Interval -   %d\n",pMyMem->pInvMas.m_count*len_inv_mas);
	 fprintf(f,"Event    -   %d\n",pMyMem->pEventMas.m_count*len_event_mas);
	 fprintf(f,"Comp     -   %d\n",pMyMem->pCompMas.m_count*len_comp_mas);
	 fprintf(f,"Cupoint  -   %d\n",pMyMem->pCupointMas.m_count*len_cupoint_mas);
	 fprintf(f,"Line     -   %d\n",pMyMem->pLineMas.m_count*len_line_mas);
	 fprintf(f,"HLine    -   %d\n",pMyMem->pHLineMas.m_count*len_hline_mas);
	 fprintf(f,"All Mem  -   %d\n",pMyMem->pInvMas.m_count*len_inv_mas*size_class_inv+pMyMem->pEventMas.m_count*len_event_mas*size_class_event+pMyMem->pCompMas.m_count*len_comp_mas*size_class_comp+pMyMem->pCupointMas.m_count*len_cupoint_mas*size_class_cupoint+pMyMem->pLineMas.m_count*len_line_mas*size_class_line+pMyMem->pHLineMas.m_count*len_hline_mas*size_class_hline);
	 fclose(f);
	 }
	 */
	delete pLCont;
	pLCont = NULL;
	CMem* pMem = pMyMem->pInvMas.m_root;
	while (pMem) {
		CInterval* pInvMas = (CInterval*) (pMem->lp);
		delete[] pInvMas;
		pMem = pMem->next;
	}
	pMem = pMyMem->pCompMas.m_root;
	while (pMem) {
		CComponent* pCompMas = (CComponent*) (pMem->lp);
		delete[] pCompMas;
		pMem = pMem->next;
	}
	pMem = pMyMem->pCupointMas.m_root;
	while (pMem) {
		CCutPoint* pCPMas = (CCutPoint*) (pMem->lp);
		delete[] pCPMas;
		pMem = pMem->next;
	}
	pMem = pMyMem->pEventMas.m_root;
	while (pMem) {
		CEvent* pEventMas = (CEvent*) (pMem->lp);
		delete[] pEventMas;
		pMem = pMem->next;
	}
	pMem = pMyMem->pHLineMas.m_root;
	while (pMem) {
		CHLine* pHLineMas = (CHLine*) (pMem->lp);
		delete[] pHLineMas;
		pMem = pMem->next;
	}
	pMem = pMyMem->pLineMas.m_root;
	while (pMem) {
		CLine* pLineMas = (CLine*) (pMem->lp);
		delete[] pLineMas;
		pMem = pMem->next;
	}
	pMyMem->pCompMas.RealDelAll();
	pMyMem->pCupointMas.RealDelAll();
	pMyMem->pEventMas.RealDelAll();
	pMyMem->pHLineMas.RealDelAll();
	pMyMem->pInvMas.RealDelAll();
	pMyMem->pLineMas.RealDelAll();
	delete pMyMem;
	pMyMem = NULL;

}

Bool InitData(void) {
	pLCont = NULL;
	pLCont = new LIST<CHLine> ;
	pMyMem = new CAllMem;
	if (!pLCont)
		return FALSE;
	if (!pMyMem)
		return FALSE;
	CMem* pMem;
	CInterval* pInvMas = new CInterval[len_inv_mas];
	if (!pInvMas) {
		DeleteData();
		return FALSE;
	} else {
		pMem = pMyMem->pInvMas.RealAdd();
		if (!pMem) {
			delete[] pInvMas;
			DeleteData();
			return FALSE;
		}
		pMem->lp = pInvMas;
	}
	CEvent* pEventMas = new CEvent[len_event_mas];
	if (!pEventMas) {
		DeleteData();
		return FALSE;
	} else {
		pMem = pMyMem->pEventMas.RealAdd();
		if (!pMem) {
			delete[] pEventMas;
			DeleteData();
			return FALSE;
		}
		pMem->lp = pEventMas;
	}
	CCutPoint* pCupointMas = new CCutPoint[len_cupoint_mas];
	if (!pCupointMas) {
		DeleteData();
		return FALSE;
	} else {
		pMem = pMyMem->pCupointMas.RealAdd();
		if (!pMem) {
			delete[] pCupointMas;
			DeleteData();
			return FALSE;
		}
		pMem->lp = pCupointMas;
	}
	CComponent* pCompMas = new CComponent[len_comp_mas];
	if (!pCompMas) {
		DeleteData();
		return FALSE;
	} else {
		pMem = pMyMem->pCompMas.RealAdd();
		if (!pMem) {
			delete[] pCompMas;
			DeleteData();
			return FALSE;
		}
		pMem->lp = pCompMas;
	}
	CLine* pLineMas = new CLine[len_line_mas];
	if (!pLineMas) {
		DeleteData();
		return FALSE;
	} else {
		pMem = pMyMem->pLineMas.RealAdd();
		if (!pMem) {
			delete[] pLineMas;
			DeleteData();
			return FALSE;
		}
		pMem->lp = pLineMas;
	}
	CHLine* pHLineMas = new CHLine[len_hline_mas];
	if (!pHLineMas) {
		DeleteData();
		return FALSE;
	} else {
		pMem = pMyMem->pHLineMas.RealAdd();
		if (!pMem) {
			delete[] pHLineMas;
			DeleteData();
			return FALSE;
		}
		pMem->lp = pHLineMas;
	}

	int i;
	for (i = len_inv_mas - 1; i > 0; i--)
		pInvMas[i - 1].next = &(pInvMas[i]);

	for (i = len_event_mas - 1; i > 0; i--)
		pEventMas[i - 1].next = &(pEventMas[i]);

	for (i = len_cupoint_mas - 1; i > 0; i--)
		pCupointMas[i - 1].next = &(pCupointMas[i]);

	for (i = len_comp_mas - 1; i > 0; i--)
		pCompMas[i - 1].next = &(pCompMas[i]);

	for (i = len_line_mas - 1; i > 0; i--)
		pLineMas[i - 1].next = &(pLineMas[i]);

	for (i = len_hline_mas - 1; i > 0; i--)
		pHLineMas[i - 1].next = &(pHLineMas[i]);

	pInvMem = pInvMas;
	pEventMem = pEventMas;
	pCupointMem = pCupointMas;
	pCompMem = pCompMas;
	pLineMem = pLineMas;
	pHLineMem = pHLineMas;

	return TRUE;
}

void* MyNew(int size) {
	void* ret;
	if (size == size_class_inv) {
		if (!pInvMem) {
			if (!MyRealloc((void**) &pInvMem, size))
				return NULL;
		}
		ret = pInvMem;
		pInvMem = pInvMem->next;
		return ret;
	}
	if (size == size_class_event) {
		if (!pEventMem) {
			if (!MyRealloc((void**) &pEventMem, size))
				return NULL;
		}
		ret = pEventMem;
		pEventMem = pEventMem->next;
		return ret;
	}
	if (size == size_class_cupoint) {
		if (!pCupointMem) {
			if (!MyRealloc((void**) &pCupointMem, size))
				return NULL;
		}
		ret = pCupointMem;
		pCupointMem = pCupointMem->next;
		return ret;
	}
	if (size == size_class_line) {
		if (!pLineMem) {
			if (!MyRealloc((void**) &pLineMem, size))
				return NULL;
		}
		ret = pLineMem;
		pLineMem = pLineMem->next;
		return ret;
	}
	if (size == size_class_comp) {
		if (!pCompMem) {
			if (!MyRealloc((void**) &pCompMem, size))
				return NULL;
		}
		ret = pCompMem;
		pCompMem = pCompMem->next;
		return ret;
	}
	if (size == size_class_hline) {
		if (!pHLineMem) {
			if (!MyRealloc((void**) &pHLineMem, size))
				return NULL;
		}
		ret = pHLineMem;
		pHLineMem = pHLineMem->next;
		return ret;
	}
	return NULL;
}

Bool MyRealloc(void** pMem, int size) {
	int i;
	void* pTemp;
	CMem* pMemory;

	if (size == size_class_inv) {
		if (!(pMemory = pMyMem->pInvMas.RealAdd()))
			return FALSE;

		pTemp = NULL;
		pTemp = new CInterval[len_inv_mas];
		if (!pTemp)
			return FALSE;

		pMemory->lp = pTemp;

		for (i = len_inv_mas - 1; i > 0; i--)
			((CInterval*) pTemp)[i - 1].next = &(((CInterval*) pTemp)[i]);

		*pMem = (CInterval*) pTemp;
		return TRUE;
	} else {
		if (size == size_class_event) {
			if (!(pMemory = pMyMem->pEventMas.RealAdd()))
				return FALSE;

			pTemp = NULL;
			pTemp = new CEvent[len_event_mas];
			if (!pTemp)
				return FALSE;

			pMemory->lp = pTemp;

			for (i = len_event_mas - 1; i > 0; i--)
				((CEvent*) pTemp)[i - 1].next = &(((CEvent*) pTemp)[i]);

			*pMem = (CEvent*) pTemp;
			return TRUE;
		} else {
			if (size == size_class_comp) {
				if (!(pMemory = pMyMem->pCompMas.RealAdd()))
					return FALSE;

				pTemp = NULL;
				pTemp = new CComponent[len_comp_mas];
				if (!pTemp)
					return FALSE;

				pMemory->lp = pTemp;

				for (i = len_comp_mas - 1; i > 0; i--)
					((CComponent*) pTemp)[i - 1].next
							= &(((CComponent*) pTemp)[i]);

				*pMem = (CComponent*) pTemp;
				return TRUE;
			} else {
				if (size == size_class_cupoint) {
					if (!(pMemory = pMyMem->pCupointMas.RealAdd()))
						return FALSE;

					pTemp = NULL;
					pTemp = new CCutPoint[len_cupoint_mas];
					if (!pTemp)
						return FALSE;

					pMemory->lp = pTemp;

					for (i = len_cupoint_mas - 1; i > 0; i--)
						((CCutPoint*) pTemp)[i - 1].next
								= &(((CCutPoint*) pTemp)[i]);

					*pMem = (CCutPoint*) pTemp;
					return TRUE;
				} else {
					if (size == size_class_line) {
						if (!(pMemory = pMyMem->pLineMas.RealAdd()))
							return FALSE;

						pTemp = NULL;
						pTemp = new CLine[len_line_mas];
						if (!pTemp)
							return FALSE;

						pMemory->lp = pTemp;

						for (i = len_line_mas - 1; i > 0; i--)
							((CLine*) pTemp)[i - 1].next
									= &(((CLine*) pTemp)[i]);

						*pMem = (CLine*) pTemp;
						return TRUE;
					} else {
						if (size == size_class_hline) {
							if (!(pMemory = pMyMem->pHLineMas.RealAdd()))
								return FALSE;

							pTemp = NULL;
							pTemp = new CHLine[len_hline_mas];
							if (!pTemp)
								return FALSE;

							pMemory->lp = pTemp;

							for (i = len_hline_mas - 1; i > 0; i--)
								((CHLine*) pTemp)[i - 1].next
										= &(((CHLine*) pTemp)[i]);

							*pMem = (CHLine*) pTemp;
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

void MyDelete(void* vType, int size) {
	if (!vType)
		return;
	if (size == size_class_event) {
		CEvent* pEvent = (CEvent*) vType;
		pEvent->next = pEventMem;
		pEventMem = pEvent;
		MyDeleteAll(pEvent->m_interval.m_root, pEvent->m_interval.m_tail,
				size_class_inv);
		pEvent->m_interval.m_root = pEvent->m_interval.m_tail = NULL;
		pEvent->m_interval.m_count = 0;
		return;
	}
	if (size == size_class_cupoint) {
		CCutPoint* pCupoint = (CCutPoint*) vType;
		pCupoint->next = pCupointMem;
		pCupointMem = pCupoint;
		MyDeleteAll(pCupoint->m_interval.m_root, pCupoint->m_interval.m_tail,
				size_class_inv);
		pCupoint->m_interval.m_root = pCupoint->m_interval.m_tail = NULL;
		pCupoint->m_interval.m_count = 0;
		return;
	}
	if (size == size_class_line) {
		CLine* pLine = (CLine*) vType;
		pLine->next = pLineMem;
		pLineMem = pLine;
		pLine->m_cut_point.FastDel();
		pLine->m_event.FastDel();
		MyDeleteAll(pLine->m_comp.m_root, pLine->m_comp.m_tail, size_class_comp);
		pLine->m_comp.m_root = pLine->m_comp.m_tail = NULL;
		pLine->m_comp.m_count = 0;
		return;
	}
	if (size == size_class_hline) {
		CHLine* pHLine = (CHLine*) vType;
		pHLine->next = pHLineMem;
		pHLineMem = pHLine;
		pHLine->m_line.FastDel();
		return;
	}
	if (size == size_class_inv) {
		CInterval* pInv = (CInterval*) vType;
		pInv->next = pInvMem;
		pInvMem = pInv;
		return;
	}
	if (size == size_class_comp) {
		CComponent* pComp = (CComponent*) vType;
		pComp->next = pCompMem;
		pCompMem = pComp;
		return;
	}
}

void MyDeleteAll(void* vRoot, void* vTail, int size) {
	if (!vTail)
		return;
	if (size == size_class_inv) {
		((CInterval*) vTail)->next = pInvMem;
		pInvMem = (CInterval*) vRoot;
		return;
	}
	if (size == size_class_comp) {
		((CComponent*) vTail)->next = pCompMem;
		pCompMem = (CComponent*) vRoot;
		return;
	}
}
