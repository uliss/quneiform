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
//
#ifndef _CTC_CLASSES_
#define _CTC_CLASSES_

#include "cfio.h"
#include "resource.h"

#include "ctc_def.h"
#include "ctcglobalfile.h"
#include "ctcglobalheader.h"
#include "ctcmemoryheader.h"
#include "ctcfileheader.h"
#include "ctcstorageheader.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

namespace CIF {
namespace CTC {

typedef struct {
	uint32_t siHeaderSize;
	uint32_t siItemSize;
	uint32_t siFlag;
	char siName[CFIO_MAX_PATH];
} STORAGEITEM;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

class CTCMemoryList {
private:
	uint32_t wListSize;
	MemoryHeader mhFirstItem;
	MemoryHeader mhLastItem;
	uint32_t wMemoryCounter;
	uint32_t wItemCounter;

public:
	CTCMemoryList();
	~CTCMemoryList();

public:
	Bool32 AddItem(Handle hMemory, uint32_t wSize, uint32_t wIsGlobal,
			const char *cOwner, const char *Coment);
    MemoryHeaderPtr GetItem(Handle hMemory);
	Bool32 LockUnlockItem(Handle hMemory, Bool32 bLock);
	Bool32 TakeItem(Handle hMemory, uint32_t * wSize, uint32_t * wFlag);
	Bool32 DeleteItem(Handle hMemory, uint32_t wParam = 0x0);

private:
	Bool32 KillItem(MemoryHeaderPtr pItem, MemoryHeaderPtr pPrevItem);
	MemoryHeaderPtr pFirst();
	MemoryHeaderPtr pLast();
	uint32_t IncreaseMemoryCounter(uint32_t wSize);
	uint32_t DecreaseMemoryCounter(uint32_t wSize);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTCFileList {
private:
	uint32_t wListSize;
	CTCStorageHeader * pList;
	uint32_t wFileCounter;
	uint32_t wSpaceCounter;
	FileHeader mfFirstItem;
	FileHeader mfLastItem;

public:
	Handle FindFile(const std::string& FileName);
	CTCFileList();
	~CTCFileList();

public:
	Handle AddItem(GlobalFile * pNewFile, uint32_t wNewFlag, Handle Storage);
	Bool32 DeleteItem(Handle File, uint32_t Flag = 0);
	Handle GetAttachedFileHeader(Handle Storage, FileHeader * File = NULL);
	FileHeader * GetItemHeader(Handle File);
	GlobalFile * GetItem(Handle File);

private:
	FileHeader * pLast() {
		return &mfLastItem;
	}

	FileHeader * pFirst() {
		return &mfFirstItem;
	}

	uint32_t IncreaseFileCounter() {
		return ++wFileCounter;
	}
	uint32_t DecreaseFileCounter() {
		return --wFileCounter;
	}
	uint32_t IncreaseSpaceCounter() {
		return ++wSpaceCounter;
	}
	uint32_t DecreaseSpaceCounter() {
		return --wSpaceCounter;
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTCStorageList {
private:
	uint32_t wHeaderSize;
	CTCStorageHeader msFirstItem;
	CTCStorageHeader msLastItem;
	CTCStorageHeader * pList;
	uint32_t wItemCounter;

public:
	CTCStorageList();
	~CTCStorageList();

	Handle FindStorage(const std::string& Name);
	Handle AddItem(GlobalFile * NewStorage, uint32_t wParametr);
	Bool32 DeleteItem(Handle Storage, uint32_t Flag = 0);
	GlobalFile * GetItem(Handle Storage);
	CTCStorageHeader * GetFirstItemHeader() {
		return pFirst()->GetNext();
	}

	CTCStorageHeader * GetItemHeader(Handle Storage);
	Bool32 IsEmpty() {
		return wItemCounter == 0;
	}

private:
	CTCStorageHeader * pFirst(void) {
		return &msFirstItem;
	}

	CTCStorageHeader * pLast(void) {
		return &msLastItem;
	}

	uint32_t IncreaseItemCounter(void) {
		return ++wItemCounter;
	}

	uint32_t DecreaseItemCounter(void) {
		return --wItemCounter;
	}

	uint32_t HowItems(void) {
		return wItemCounter;
	}
};

}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif        //_CTC_CLASSES_
