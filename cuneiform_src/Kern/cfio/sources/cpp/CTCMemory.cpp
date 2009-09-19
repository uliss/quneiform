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

//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//                                                                              //
//                    Cognitive Technologies Ltd.                               //
//                                                                              //
//                    Data Storage Container                                    //
//                                                                              //
//                                                                              //
//                                                                              //
//                                                                              //
//                    started at 25 may 1998                                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
#include "ctcclasses.h"
//////////////////////////////////////////////////////////////////GLOBAL
//
extern void SetReturnCode_cfio(uint16_t rc);
//////////////////////////////////////////////////////////////////////////////////
//
CTCMemoryHeader::CTCMemoryHeader()
                :CTCGlobalHeader()
{
	mcComment[0] = mcOwner[0] = 0x0;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCMemoryHeader::CTCMemoryHeader(Handle hMemory, uint32_t wBlockSize)
                :CTCGlobalHeader(hMemory,NULL,wBlockSize)
{
	SetHeaderSize(sizeof( class CTCMemoryHeader));
	mcComment[0] = mcOwner[0] = 0x0;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCMemoryHeader::CTCMemoryHeader(Handle hMemory, uint32_t wBlockSize, const char *OwnerName, const char *Commentary)
                :CTCGlobalHeader(hMemory,NULL,wBlockSize)
{
	SetHeaderSize(sizeof( class CTCMemoryHeader));
	mcComment[0] = mcOwner[0] = 0x0;

	if ( OwnerName[0] != 0x0 && CFIO_STRLEN(OwnerName) < CFIO_MAX_OWNER )
	{
		CFIO_STRCPY(mcOwner, OwnerName);
	}
	else
	{
		strncpy(mcOwner, OwnerName, CFIO_MAX_OWNER - 1);
	}

	if ( Commentary[0] != 0x0 && CFIO_STRLEN(OwnerName) < CFIO_MAX_COMMENT )
	{
		CFIO_STRCPY(mcComment, Commentary);
	}
	else
	{
		strncpy(mcComment, Commentary, CFIO_MAX_COMMENT - 1);
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCMemoryHeader::~CTCMemoryHeader()
{
	Handle hToDelete = GetHandle();

	if ( hToDelete != NULL &&  hToDelete != FICTIV_Handle )
	{
#ifdef CFIO_USE_GLOBAL_MEMORY
		CFIO_FREE(GetHandle());
#else
		delete[] static_cast<char*>(hToDelete);
#endif //CFIO_USE_GLOBAL_MEMORY
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCMemoryList::CTCMemoryList()
              :mhFirstItem(FICTIV_Handle,0,"CFIO","First in list (fictiv)"),
               mhLastItem(FICTIV_Handle,0,"CFIO","Last in list (fictiv)")
{
	mhFirstItem.SetNext(&mhLastItem);
	wListSize = sizeof(class CTCMemoryList);
	wMemoryCounter = 0;
	wItemCounter = 0;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCMemoryList::~CTCMemoryList()
{
	while( pFirst()->GetNext() != pLast() )
	{
#ifdef TRACE
		TRACE("CFIO: Missed memory block { handle: 0x%x, data: 0x%x, size: %i, owner:\"%s\", comment:\"%s\" } allocated by CFIO \n",
			   pFirst()->GetNext()->GetHandle(),
			   pFirst()->GetNext()->GetData(),
			   pFirst()->GetNext()->GetSize(),
			   pFirst()->GetNext()->GetOwner(),
			   pFirst()->GetNext()->GetComment());
#endif
		KillItem(NULL, pFirst());
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCMemoryList::IncreaseMemoryCounter(uint32_t wSize)
{
	return ( wMemoryCounter = wMemoryCounter + wSize );
}
//////////////////////////////////////////////////////////////////////////////////
//
uint32_t CTCMemoryList::DecreaseMemoryCounter(uint32_t wSize)
{
	return ( wMemoryCounter = wMemoryCounter - wSize );
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCMemoryHeader * CTCMemoryList::pFirst()
{
	return &mhFirstItem;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCMemoryHeader * CTCMemoryList::pLast()
{
	return &mhLastItem;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCMemoryList::AddItem(Handle hMemory, uint32_t wSize, uint32_t wIsGlobal, const char *cOwner, const char *Coment)
{
	CTCMemoryHeader * Current, * NewBlock = NULL;

	for ( Current = pFirst(); Current->GetNext() != pLast(); Current = Current->GetNext() )
	{
		if ( Current->GetHandle() == hMemory )
		{
			return FALSE;
		}
	}

	NewBlock = new CTCMemoryHeader(hMemory, wSize, cOwner, Coment);
	NewBlock->SetNext(Current->GetNext());
	Current->SetNext(NewBlock);
	IncreaseMemoryCounter(NewBlock->GetSize());

	if ( wIsGlobal )
	{
		NewBlock->AddFlag(CFIO_MEMORY_GLOBAL);
	}
	else
	{
		//
		NewBlock->SetData(hMemory);
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCMemoryList::DeleteItem(Handle hMemory, uint32_t wParam)
{
	PCTCMemoryHeader Current, Last;
	uint32_t IsOK    = 0;

	for ( Last = Current = pFirst(); Current != pLast(); Current = Current->GetNext() )
	{
		if ( Current->GetHandle() == hMemory )
		{
			if ( KillItem(Current, Last) )
				IsOK++;
			Current = Last;
		}
		else
		{
			Last = Current;
		}
	}
		return ( IsOK == 1 );
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCMemoryList::KillItem(PCTCMemoryHeader pItem, PCTCMemoryHeader pPrevItem)
{
	PCTCMemoryHeader pErased;

	if ( pItem )
		pErased = pItem;
	else
		pErased = pPrevItem->GetNext();

	DecreaseMemoryCounter(pErased->GetSize());
	pPrevItem->SetNext(pErased->GetNext());
	delete pErased;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCMemoryHeader * CTCMemoryList::GetItem(Handle hMemory)
{
	CTCMemoryHeader * Current;

	for ( Current = pFirst(); Current != pLast(); Current = Current->GetNext() )
	{
		if ( !Current )
		{
			SetReturnCode_cfio(IDS_CFIO_ERR_INTERNAL);
			return NULL;
		}

		if ( Current->GetHandle() == hMemory )
			return Current;
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCMemoryList::TakeItem(Handle hMemory, uint32_t * wSize, uint32_t * wFlag)
{
	CTCMemoryHeader * Current = GetItem(hMemory);

	if ( Current )
	{
		*wSize = Current->GetSize();
		*wFlag = Current->GetFlag();
		return TRUE;
	}

	*wSize = 0;
	*wFlag = 0;
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
// Add and remove Lock Flag (depend by bLock)
Bool32 CTCMemoryList::LockUnlockItem(Handle hMemory, Bool32 bLock)
{
	CTCMemoryHeader * Current;

	for ( Current = pFirst(); Current != pLast(); Current = Current->GetNext() )
	{
		if ( Current->GetHandle() == hMemory )
		{
			if ( bLock && !Current->IsFlag(CFIO_MEMORY_LOCK) )
			{
				Current->AddFlag(CFIO_MEMORY_LOCK);
				return TRUE;
			}

			if ( !bLock && Current->IsFlag(CFIO_MEMORY_LOCK) )
			{
				Current->RemoveFlag(CFIO_MEMORY_LOCK);
				return TRUE;
			}
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file



