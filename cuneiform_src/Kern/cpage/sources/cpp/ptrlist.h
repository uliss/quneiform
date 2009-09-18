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

#ifndef __PTRLIST_H__
#define __PTRLIST_H__

#include <stdio.h>
#include <memory.h>

#include "mymem.h"

#ifndef IS_VALID
	#define IS_VALID(a)
#endif

template<class TYPE> class PtrList
{
protected:
	TYPE Data;

private:
	PtrList * Bot;// для корня - TAIL, остальные NEXT
	PtrList * Top;// для корня - HEAD, остальные PREV

protected:
	void * operator new ( size_t stAllocateBlock );
	void   operator delete( void * mem );


public:
	PtrList() ;
	~PtrList();

	void Clear();

	Handle AddTail(TYPE & Data);
	Handle AddHead(TYPE & Data);

	bool   Del(Handle ptr);

	TYPE & GetHead(Handle & hPos);
	TYPE & GetTail(Handle & hPos);
	TYPE & GetNext(Handle & hPos);
	TYPE & GetPrev(Handle & hPos);

	int	GetCount();

	TYPE & GetItem(Handle hPos);
	int    GetPos(Handle hPos);
	Handle GetHandle(int Number);

	Handle FindFirst(TYPE & type);
	Handle FindNext(TYPE & type,Handle hPos);
};

//#################################
template<class TYPE> PtrList<TYPE>::PtrList()
{
	Top = Bot = NULL;
}
//#################################
template<class TYPE> PtrList<TYPE>::~PtrList()
{
}
//#################################
template<class TYPE> int PtrList<TYPE>::GetCount()
{
	PtrList * tmp;
	int i;

	for(i=0,tmp=Top;tmp;i++,tmp=tmp->Bot) IS_VALID(tmp);
return i;
}
//#################################
template<class TYPE> Handle PtrList<TYPE>::AddTail(TYPE & tpData)
{
	PtrList * NewItem = new PtrList;

	if(NewItem)
	{
		NewItem->Data = tpData;
		if(Bot == NULL)
		{
			Top = Bot = NewItem;
		}
		else {
			Bot->Bot = NewItem;
			NewItem->Top = Bot;
			Bot = NewItem;
			}
	}

return NewItem;
}
//#################################
template<class TYPE> Handle PtrList<TYPE>::AddHead(TYPE & tpData)
{
	PtrList * NewItem = new PtrList;
	if(NewItem)
	{
		NewItem->Data = tpData;

		if(Top == NULL)
		{
			Top = Bot = NewItem;
		}
		else {
			Top->Top = NewItem;
			NewItem->Bot = Top;
			Top = NewItem;
			}
	}
return NewItem;
}
//#################################
template<class TYPE> bool  PtrList<TYPE>::Del(Handle ptr)
{
	PtrList * p = (PtrList *)ptr;
	if(ptr)
	{
		IS_VALID(ptr);

		PtrList * top = p->Top;
		PtrList * bot = p->Bot;

		if(top)
		{
			IS_VALID(top);
			top->Bot = bot;
		}
		else
			Top = bot;

		if(bot)
		{
			IS_VALID(bot);
			bot->Top = top;
		}
		else
			Bot = top;

		delete p;
		return true;
	}
return false;
}
//#################################
template<class TYPE> TYPE & PtrList<TYPE>::GetHead(Handle & hPos)
{
	IS_VALID(Top);

	hPos = Top;

	return Top->Data;
}
//#################################
template<class TYPE> TYPE & PtrList<TYPE>::GetTail(Handle & hPos)
{
	IS_VALID(Bot);

	hPos = Bot;

	return Bot->Data;
}
//#################################
template<class TYPE> TYPE & PtrList<TYPE>::GetNext(Handle & pos)
{
	IS_VALID(pos);

	PtrList * tmp = (PtrList *)pos;

	if(tmp->Bot)
		pos = tmp->Bot;
	return tmp->Data;
}
//#################################
template<class TYPE> TYPE & PtrList<TYPE>::GetPrev(Handle & pos)
{
	IS_VALID(pos);

	PtrList * tmp = (PtrList *)pos;

	if(tmp->Top)
		pos = tmp->Top;
	return tmp->Data;
}
//#################################
template<class TYPE> Handle PtrList<TYPE>::GetHandle(int pos)
{
	int i;
	PtrList * rc = NULL;
	PtrList * tmp;

	for(i=0,tmp=Top; tmp && i!=pos;i++,tmp=tmp->Bot) IS_VALID(tmp);
	if(i==pos)
		rc = tmp;

return rc;
}
//#################################
template<class TYPE> int PtrList<TYPE>::GetPos(Handle handle)
{
	int i;
	PtrList * tmp;

	IS_VALID(handle);

	for(i=0,tmp=Top; tmp && tmp!=handle;i++,tmp=tmp->Bot) IS_VALID(tmp);
	if(!tmp)
		i = -1;
return i;
}
//#################################
template<class TYPE> TYPE & PtrList<TYPE>::GetItem(Handle pos)
{
	IS_VALID(pos);
	return ((PtrList *)pos)->Data;
}
//#################################
template<class TYPE> void PtrList<TYPE>::Clear()
{
	PtrList * curr = Top;
	while(curr)
	{
	  IS_VALID(curr);

	  PtrList * next = curr->Bot;
	  Del(curr);
	  curr = next;
	}
}
//#################################
template<class TYPE> void * PtrList<TYPE>:: operator new ( size_t stAllocateBlock )
{
	return myAlloc(stAllocateBlock);
}
//#################################
template<class TYPE> void   PtrList<TYPE>::operator delete( void * mem )
{
	myFree(mem);
}
//#################################
template<class TYPE>	Handle PtrList<TYPE>::FindFirst(TYPE & type)
{
	Handle rc = NULL;
	PtrList * tmp = Top;
	for(;tmp;tmp=tmp->Bot)
	{
		if(type == tmp->Data)
		{
			rc = tmp;
			break;
		}
	}
	return rc;
}
//#################################
template<class TYPE>	Handle PtrList<TYPE>::FindNext(TYPE & type,Handle hPos)
{
	Handle rc = NULL;
	PtrList * tmp = hPos;
	for(tmp=tmp->Bot;tmp;tmp=tmp->Bot)
	{
		if(type == tmp->Data)
		{
			rc = tmp;
			break;
		}
	}
	return rc;
}

#endif
