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

// PtrName.h: interface for the PtrName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTRNAME_H__B7558641_0160_11D3_A5C9_E5EED2B2CF14__INCLUDED_)
#define AFX_PTRNAME_H__B7558641_0160_11D3_A5C9_E5EED2B2CF14__INCLUDED_

#include <string.h>
#include <assert.h>

#include "cttypes.h"
#include "mymem.h"
#include "cpage.h"

template<class TYPE> class PtrName
{
    protected:
        Handle  m_Type;
        uint32_t  m_Size;
        TYPE *  m_Ptr;
        Bool32  m_bAttach;

    public:
        PtrName();
        ~PtrName();

        Bool32      operator==(PtrName & pn);
        PtrName &   operator=(PtrName & pn);
        TYPE    &   operator[](uint32_t n);

        Bool32      Create(size_t nItem);
        void        Delete();

        Bool32      Attach(CPageHandle hPage);
        Bool32      Store(CPageHandle hPage);
        void        Remove(CPageHandle hPage);
        inline uint32_t     GetSize() {
            return m_Size / sizeof(TYPE);
        }

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
template<class TYPE> Bool32  PtrName<TYPE>::operator==(PtrName & pn)
{
    return m_Size == pn.m_Size && !memcmp(m_Ptr, pn.m_Ptr, pn.m_Size);
}
///////////////////////////////////////////////////////////

template<class TYPE> PtrName<TYPE> & PtrName<TYPE>::operator=(PtrName & pn)
{
    Create(pn.m_Size / sizeof(TYPE));
    memcpy(m_Ptr, pn.m_Ptr, pn.m_Size);
    return *this;
}

///////////////////////////////////////////////////////////
template<class TYPE> TYPE & PtrName<TYPE>::operator[](uint32_t n)
{
    return m_Ptr[n];
}
///////////////////////////////////////////////////////////
template<class TYPE> Bool32 PtrName<TYPE>::Create(size_t nItem)
{
    assert(nItem);

    if (m_bAttach == FALSE)
        Delete();

    m_bAttach = FALSE;
    m_Type = CPAGE_GetUserBlockType();
    m_Size = nItem * sizeof(TYPE);
    m_Ptr  = (TYPE *)myAlloc(m_Size);
    return m_Ptr != NULL;
}
///////////////////////////////////////////////////////////
template<class TYPE> void PtrName<TYPE>::Delete()
{
    if (!m_bAttach && m_Ptr)
        myFree( m_Ptr );

    m_Type = 0;
    m_Size = 0;
    m_Ptr = NULL;
}
///////////////////////////////////////////////////////////
template<class TYPE> Bool32     PtrName<TYPE>::Attach(CPageHandle hPage)
{
    assert(m_Ptr);
    CBlockHandle hBlock = CPAGE_GetBlockFirst(hPage, m_Type);

    if (!hBlock)
        return FALSE;

    m_bAttach = CPAGE_GetBlockDataPtr(hBlock, m_Type, (void **) & m_Ptr);
    return m_bAttach;
}
///////////////////////////////////////////////////////////
template<class TYPE> void       PtrName<TYPE>::Remove(CPageHandle hPage)
{
    CBlockHandle hBlock = CPAGE_GetBlockFirst(hPage, m_Type);

    if (!hBlock)
        return ;

    CPAGE_DeleteBlock(hPage, hBlock);

    if (m_bAttach) {
        m_Type = 0;
        m_Size = 0;
        m_Ptr = NULL;
        m_bAttach = FALSE;
    }
}
///////////////////////////////////////////////////////////
template<class TYPE> Bool32     PtrName<TYPE>::Store(CPageHandle hPage)
{
    CBlockHandle hBlock = CPAGE_GetBlockFirst(hPage, m_Type);

    if (hBlock)
        CPAGE_DeleteBlock(hPage, hBlock);

    return CPAGE_CreateBlock(hPage, m_Type, 0, 0, m_Ptr, m_Size) != NULL;
}
#endif // !defined(AFX_PTRNAME_H__B7558641_0160_11D3_A5C9_E5EED2B2CF14__INCLUDED_)
