/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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

// PtrName.h: interface for the PtrName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTRNAME_H__B7558641_0160_11D3_A5C9_E5EED2B2CF14__INCLUDED_)
#define AFX_PTRNAME_H__B7558641_0160_11D3_A5C9_E5EED2B2CF14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include <assert.h>
#include "cttypes.h"
#include "cpage.h"

#ifndef _ASSERT
#    define _ASSERT assert
#endif // _ASSERT

template<class TYPE> class PtrName
{
protected:
	Handle  m_Type;
	Word32  m_Size;
	TYPE *	m_Ptr;
	Bool32  m_bAttach;

public:
	PtrName();
	~PtrName();

	Bool32		operator==(PtrName & pn);
	PtrName &	operator=(PtrName & pn);
	TYPE	&	operator[](Word32 n);

	Bool32		Create(size_t nItem);
	void		Delete();

	Bool32		Attach(Handle hPage);
	Bool32		Store(Handle hPage);
	void        Remove(Handle hPage);
	inline Word32		GetSize(){ return m_Size/sizeof(TYPE);};

};
///////////////////////////////////////////////////////////
template<class TYPE> PtrName<TYPE>::PtrName()
{
	m_Type = 0;
	m_Size = 0;
	m_Ptr = NULL;
	m_bAttach = FALSE;
}
///////////////////////////////////////////////////////////
template<class TYPE> PtrName<TYPE>::~PtrName()
{
	Delete();
}
///////////////////////////////////////////////////////////
template<class TYPE> Bool32	 PtrName<TYPE>::operator==(PtrName & pn)
{
	return m_Size==pn.m_Size && !memcmp(m_Ptr,pn.m_Ptr,pn.m_Size);
}
///////////////////////////////////////////////////////////

template<class TYPE> PtrName<TYPE> & PtrName<TYPE>::operator=(PtrName & pn)
{
	Create(pn.m_Size/sizeof(TYPE));
	memcpy(m_Ptr,pn.m_Ptr,pn.m_Size);
	return *this;
}

///////////////////////////////////////////////////////////
template<class TYPE> TYPE & PtrName<TYPE>::operator[](Word32 n)
{
	return m_Ptr[n];
}
///////////////////////////////////////////////////////////
template<class TYPE> Bool32 PtrName<TYPE>::Create(size_t nItem)
{
	_ASSERT(nItem);
	if(m_bAttach == FALSE)
		Delete();

	m_bAttach = FALSE;
	m_Type = CPAGE_GetUserBlockType();
	m_Size = nItem * sizeof(TYPE);
	m_Ptr  = (TYPE *)myAlloc(m_Size);

	return m_Ptr!=NULL;
}
///////////////////////////////////////////////////////////
template<class TYPE> void PtrName<TYPE>::Delete()
{
	if(!m_bAttach && m_Ptr)
		myFree(	m_Ptr );

	m_Type = 0;
	m_Size = 0;
	m_Ptr = NULL;
}
///////////////////////////////////////////////////////////
template<class TYPE> Bool32		PtrName<TYPE>::Attach(Handle hPage)
{
	_ASSERT(m_Ptr);
	Handle hBlock = CPAGE_GetBlockFirst(hPage,m_Type);
	if(!hBlock)
		return FALSE;
	m_bAttach = CPAGE_GetBlockDataPtr(hPage,hBlock,m_Type,(void **)&m_Ptr);
	return m_bAttach;
}
///////////////////////////////////////////////////////////
template<class TYPE> void     	PtrName<TYPE>::Remove(Handle hPage)
{
	Handle hBlock = CPAGE_GetBlockFirst(hPage,m_Type);
	if(!hBlock)
		return ;
	CPAGE_DeleteBlock(hPage,hBlock);

	if(m_bAttach)
	{
		m_Type = 0;
		m_Size = 0;
		m_Ptr = NULL;
		m_bAttach = FALSE;
	}
}
///////////////////////////////////////////////////////////
template<class TYPE> Bool32    	PtrName<TYPE>::Store(Handle hPage)
{
	Handle hBlock = CPAGE_GetBlockFirst(hPage,m_Type);
	if(hBlock)
		CPAGE_DeleteBlock(hPage,hBlock);
	return CPAGE_CreateBlock(hPage,m_Type,0,0,m_Ptr,m_Size)!=NULL;
}
#endif // !defined(AFX_PTRNAME_H__B7558641_0160_11D3_A5C9_E5EED2B2CF14__INCLUDED_)
