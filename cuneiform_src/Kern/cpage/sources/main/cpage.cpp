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

#include "resource.h"
#include "cpage.h"
#include "backup.h"
#include "namedata.h"

#define SAVE_COMPRESSED  // Сохранять данные в блоках в упакованном виде

extern PtrList<BACKUPPAGE> Page;
extern Handle              hCurPage;
extern PtrList<NAMEDATA>	NameData;

#define PAGE_H(page) Page.GetItem(page)
#define PAGE_N(page) PAGE_H(Page.GetHandle(page))

#define _BLOCK_H(page,block) page.Block.GetItem(block)
#define _BLOCK_N(page,block) _BLOCK_H(page,page.Block.GetHandle(block))

#define BLOCK_H_H(page,block) _BLOCK_H(PAGE_H(page),block)
#define BLOCK_H_N(page,block) _BLOCK_N(PAGE_H(page),block)
#define BLOCK_N_N(page,block) _BLOCK_N(PAGE_N(page),block)

//###########################################
CPAGE_FUNC(Handle) CPAGE_CreatePage(Handle type,void * lpdata, uint32_t size)
{
	PROLOG;
	BACKUPPAGE tail;
  SetReturnCode_cpage(IDS_ERR_NO);

  Handle hPage = Page.AddTail(tail);


  if(hPage)
  {
	if(!Page.GetItem(hPage).SetData(type,lpdata,size))
	{
		Page.Del(hPage);
		hPage = NULL;
		if(hCurPage == hPage)
			hCurPage = NULL;
	}
	else
		hCurPage = hPage;
  }
  EPILOG;
  return hPage;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetPageType(Handle page)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	Handle rc = PAGE_H(page).GetType();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(void) CPAGE_DeletePage(Handle page)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	Page.Del(page);
	if(hCurPage == page)
		hCurPage = NULL;
	EPILOG;
}
//###########################################
CPAGE_FUNC(void) CPAGE_ClearBackUp(Handle page)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	PAGE_H(page).Clear();
	EPILOG;
}
//###########################################
CPAGE_FUNC(Handle) CPAGE_BackUp(Handle page)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	Handle rc = PAGE_H(page).BackUp();
	EPILOG;
	return rc;
}
//###########################################

CPAGE_FUNC(Bool32) CPAGE_Undo(Handle page,Handle num)
{
	PROLOG;
	Bool32 rc = FALSE;
	SetReturnCode_cpage(IDS_ERR_NO);
	if(num==NULL)
	{
		uint32_t number = CPAGE_GetBuckUpCount(page);
		if(number)
		{
			uint32_t cur = CPAGE_GetBuckUpCurPos(page);
			if(cur == 0)
			{
				rc = FALSE;
				goto lOut;
			}
			num = CPAGE_GetBuckUpHandle(page,cur - 1);
		}
		else
		{
			rc = FALSE;
			goto lOut;
		}
	}
	rc = PAGE_H(page).Undo(num);
lOut:EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Bool32) CPAGE_Redo(Handle page,Handle num)
{
	PROLOG;
	Bool32 rc = FALSE;
	SetReturnCode_cpage(IDS_ERR_NO);
	if(num==NULL)
	{
		uint32_t number = CPAGE_GetBuckUpCount(page);
		if(number)
		{
			uint32_t cur = CPAGE_GetBuckUpCurPos(page);
			if(cur == (number - 1))
			{
				rc = FALSE;
				goto lOut;
			}
			num = CPAGE_GetBuckUpHandle(page,cur + 1);
		}
		else
		{
			rc = FALSE;
			goto lOut;
		}
	}
	rc = PAGE_H(page).Redo(num);
lOut:EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(uint32_t) CPAGE_GetBuckUpCount(Handle page)
{
	PROLOG;
	uint32_t rc = PAGE_H(page).BackUpPage.GetCount();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle) CPAGE_GetBuckUpHandle(Handle page,uint32_t number)
{
	PROLOG;
	Handle rc = PAGE_H(page).BackUpPage.GetHandle(number);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(uint32_t) CPAGE_GetBuckUpCurPos(Handle page)
{
	PROLOG;
	uint32_t rc = PAGE_H(page).GetCurPos();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)	CPAGE_CreateBlock(Handle page, Handle Type, uint32_t UserNum , uint32_t Flags,void * lpData, uint32_t Size)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);

#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(Type));
#endif

	Handle rc = PAGE_H(page).CreateBlock(Type ,UserNum ,Flags ,lpData , Size);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Bool32) CPAGE_SetPageData(Handle page, Handle type, void * lpdata, uint32_t size)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(type));
#endif
	Bool32 rc = PAGE_H(page).SetData( type, lpdata, size);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(uint32_t) CPAGE_GetPageData(Handle page, Handle type, void * lpdata, uint32_t size)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);

#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(type));
#endif

	DefConvertInit();
	uint32_t rc = PAGE_H(page).GetData( type, lpdata, size);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetBlockType(Handle page,Handle block)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	Handle rc = BLOCK_H_H(page,block).GetType();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(uint32_t)  CPAGE_GetBlockUserNum(Handle page,Handle block)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	uint32_t rc = BLOCK_H_H(page,block).GetUserNum();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(void)    CPAGE_SetBlockUserNum(Handle page,Handle block,uint32_t user)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	BLOCK_H_H(page,block).SetUserNum(user);
	EPILOG;
}
//###########################################
CPAGE_FUNC(uint32_t)  CPAGE_GetBlockFlags(Handle page,Handle block)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	uint32_t rc = BLOCK_H_H(page,block).GetFlags();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(void)    CPAGE_SetBlockFlags(Handle page,Handle block,uint32_t flags)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	BLOCK_H_H(page,block).SetFlags(flags);
	EPILOG;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_SetBlockData(Handle page, Handle block, Handle Type, void * lpData, uint32_t Size)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(Type));
#endif
	Bool32 rc = BLOCK_H_H(page,block).SetData(Type,lpData, Size);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(uint32_t)  CPAGE_GetBlockData(Handle page, Handle block, Handle Type, void * lpData, uint32_t Size)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(Type));
#endif
	DefConvertInit();
	uint32_t rc = BLOCK_H_H(page,block).GetData(Type, lpData, Size);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(uint32_t) CPAGE_GetCountPage()
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	uint32_t rc = Page.GetCount();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(uint32_t) CPAGE_GetCountBlock(Handle page)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	uint32_t rc = PAGE_H(page).Block.GetCount();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(void)  CPAGE_DeleteBlock(Handle page,Handle block)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	PAGE_H(page).Block.Del(block);
	EPILOG;
}
//###########################################
#define VERSION_FILE            0xF002
#define VERSION_FILE_COMPRESSED 0xF003

CPAGE_FUNC(Bool32)  CPAGE_SavePage(Handle page,char * lpName)
{
	PROLOG;
	Bool32 rc = FALSE;
	SetReturnCode_cpage(IDS_ERR_NO);
	Handle file = myOpenSave((char *)lpName);

	if(file)
	{
#ifdef SAVE_COMPRESSED
		uint32_t vers = VERSION_FILE_COMPRESSED;
#else
		uint32_t vers = VERSION_FILE;
#endif
		if(myWrite(file,&vers,sizeof(vers))==sizeof(vers))
		{
			if(page)
			{
				int count = 1;
				rc = myWrite(file,&count,sizeof(count))==sizeof(count);
#ifdef SAVE_COMPRESSED
				rc = PAGE_H(page).SaveCompress(file);
#else
				rc = PAGE_H(page).Save(file);
#endif
			}
			else
			{	int i;
				int count = Page.GetCount();
				rc = myWrite(file,&count,sizeof(count))==sizeof(count);
				for(i=0;i<count && rc == TRUE;i++)
#ifdef SAVE_COMPRESSED
					rc = PAGE_N(i).SaveCompress(file);
#else
					rc = PAGE_N(i).Save(file);
#endif
			}
		}
		myClose(file);
	}
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_RestorePage(Bool32 remove,char * lpName)
{
	PROLOG;
	Handle rc = NULL;
	Bool decompress=FALSE;

	SetReturnCode_cpage(IDS_ERR_NO);

	Handle file = myOpenRestore((char *)lpName);
	if(file)
	{
		int i;
		int count;

		uint32_t vers = 0;
		if(myRead(file,&vers,sizeof(vers))==sizeof(vers))
		{
			if (vers==VERSION_FILE_COMPRESSED)
			   decompress=TRUE;
			else
				if(vers!=VERSION_FILE)
				{
					SetReturnCode_cpage(IDS_ERR_OLDFILEVERSION);
					myClose(file);
					return FALSE;
				}
			{
				if(remove)
				{
					Page.Clear();
					NameData.Clear();
				}

				if(myRead(file,&count,sizeof(count))==sizeof(count))
					for(i=0;i<count ;i++)
					{
						BACKUPPAGE page;
						if(decompress ? page.RestoreCompress(file) : page.Restore(file))
							rc = Page.AddTail(page);
						else
							break;
					}
			}
		}
		myClose(file);
	}
	EPILOG;
return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetHandlePage(uint32_t page)
{
	PROLOG;
	Handle rc = Page.GetHandle(page);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetHandleBlock(Handle page,uint32_t block)
{
	PROLOG;
	Handle rc = PAGE_H(page).Block.GetHandle(block);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(CPAGE_CONVERTOR)  CPAGE_SetConvertorPages(CPAGE_CONVERTOR data)
{
	PROLOG;
	CPAGE_CONVERTOR rc = SetConvertorPages(data);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(CPAGE_CONVERTOR)  CPAGE_SetConvertorBlocks(Handle page,CPAGE_CONVERTOR data)
{
	PROLOG;
CPAGE_CONVERTOR rc = SetConvertorBlocks(data);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetUserPageType()
{
	PROLOG;
	/*
	static uint32_t number = 0x10000000;
	return number++;
	*/
	Handle rc = CPAGE_GetUserBlockType();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetUserBlockType()
{
	PROLOG;
	static uint32_t number = 1;
	char Name[260];
	sprintf(Name,"UserType:%i",number++);
	Handle rc = CPAGE_GetInternalType(Name);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetPageFirst(Handle type)
{
	PROLOG;
	int count = Page.GetCount();
	int i;
#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(type));
#endif
	DefConvertInit();
	for(i = 0;i<count;i++)
	{
		if(!type ||
		   PAGE_N(i).GetType()==type ||
		   PAGE_N(i).Convert(type,NULL,0))
			break;
	}
	Handle rc = i < count ? Page.GetHandle(i):NULL;
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetPageNext(Handle page, Handle type)
{
	PROLOG;
	int count = Page.GetCount();
	int pos   = Page.GetPos(page) + 1;
	int i;

#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(type));
#endif
	DefConvertInit();
	for(i = pos; i<count && i>=0 ;i++)
	{
		if(!type ||
			PAGE_N(i).GetType()==type ||
		    PAGE_N(i).Convert(type,NULL,0))
			break;
	}
	Handle rc = i < count ? Page.GetHandle(i):NULL;
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetBlockFirst(Handle page, Handle type)
{
	PROLOG;
	int count = PAGE_H(page).Block.GetCount();
	int i;
/*
#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(type));
#endif
*/
	DefConvertInit();
	for(i = 0;i<count;i++)
	{
		if(!type ||
			BLOCK_H_N(page,i).GetType()==type ||
		    BLOCK_H_N(page,i).Convert(type,NULL,0))
			break;
	}
	Handle rc = i < count ? PAGE_H(page).Block.GetHandle(i):NULL;
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetBlockNext(Handle page,Handle block, Handle type)
{
	PROLOG;
	int count = PAGE_H(page).Block.GetCount();
	int pos   = PAGE_H(page).Block.GetPos(block) + 1;
	int i;
/*
#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(type));
#endif
*/
	DefConvertInit();
	for(i = pos; i<count && i>=0 ;i++)
	{
		if(!type ||
		    BLOCK_H_N(page,i).GetType()==type ||
		    BLOCK_H_N(page,i).Convert(type,NULL,0))
			break;
	}
	Handle rc = i < count ? PAGE_H(page).Block.GetHandle(i):NULL;
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_DeleteAll()
{
	CPAGE_CONVERTOR ConvertorPages = {0,DefConvertPage}; // Piter
	PROLOG;
	Bool32 rc = TRUE;
	Page.Clear();
	NameData.Clear();
	SetConvertorPages(ConvertorPages); // Piter
	hCurPage = NULL;
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(uint32_t)  CPAGE_GetCurrentPage( )
{
	PROLOG;
	uint32_t rc = CPAGE_GetNumberPage(hCurPage);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_SetCurrentPage(uint32_t page)
{
	PROLOG;
	Bool32 rc = TRUE;
	if( page>=CPAGE_GetCountPage() ||
		page == (uint32_t)-1)
	{
		SetReturnCode_cpage(IDS_ERR_NOPAGE);
		rc = FALSE;
	}
	else
		hCurPage = CPAGE_GetHandlePage(page);

	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(uint32_t)  CPAGE_GetNumberPage( Handle hPage )
{
	PROLOG;
	uint32_t rc = (uint32_t)-1;
	if(hPage)
		rc = (uint32_t)Page.GetPos(hPage);
	EPILOG;
	return rc;
}
//###########################################
// Если блоки конвертируемы один в другой, тогда имеет смысл оставить только один,
// наиболее полный тип. Именно это и делает эта функция.
//
CPAGE_FUNC(Bool32)  CPAGE_UpdateBlocks( Handle hPage, Handle type )
{
	PROLOG;
	Bool32  rc = TRUE;
	uint32_t	size = 0;
	char *	lpData = NULL;
	Handle  temporaray = 0;

	SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(type));
#endif

	SetReturnCode_cpage(IDS_ERR_NO);

	Handle hBlock = CPAGE_GetBlockFirst(hPage,type);
	if(!hBlock)
	{
		rc = TRUE;
		goto lOut;
	}
	// Создадим временные блоки и удалим старые.
	// Тем самым предотвращаем зацикливание.
	temporaray = CPAGE_GetInternalType("temporary");
	while(hBlock)
	{
		Handle hNextBlock = CPAGE_GetBlockNext(hPage,hBlock,type);// type - запрашиваемый тип блока
		Handle dwType	  = CPAGE_GetBlockType(hPage,hBlock); // dwType - Реальный тип блока
		if(dwType != type) // Была произведена конвертация из типа dwType !
		{
			uint32_t UserNum = CPAGE_GetBlockUserNum(hPage,hBlock);
			uint32_t Flags   = CPAGE_GetBlockFlags(hPage,hBlock);
			if(lpData == NULL)
			{ // Определим необходимый размер и отведем память.
				size = CPAGE_GetBlockData(hPage,hBlock,type,NULL,0);
				if(size)
				{
					lpData = (char *)myAlloc(size);
					if(!lpData)
					{
						rc = FALSE;
						SetReturnCode_cpage(IDS_ERR_NO_MEMORY);
						break;
					}
				}
				else
				{
					SetReturnCode_cpage(IDS_ERR_DISCREP);
					rc = FALSE;
					break;
				}
			}
			if(CPAGE_GetBlockData(hPage,hBlock,type,lpData,size)==size)
			{
				CPAGE_DeleteBlock(hPage,hBlock);
				if(!CPAGE_CreateBlock(hPage,temporaray, UserNum , Flags, lpData, size))
				{
					SetReturnCode_cpage(IDS_ERR_NO_MEMORY);
					rc = FALSE;
					break;
				}
			}
		}
		hBlock = hNextBlock;
	}
	// Переименуем временные блоки
	if(lpData) // Проверка на существование таких блоков
	{
		myFree(lpData);
		for(hBlock = CPAGE_GetBlockFirst(hPage,temporaray);
			hBlock;
			hBlock = CPAGE_GetBlockNext(hPage,hBlock,temporaray))
		{
			BLOCK_H_H(hPage,hBlock).SetType(type);
		}
	}
lOut:EPILOG;
return rc;
}
//###########################################
CPAGE_FUNC(uint32_t)  CPAGE_GetBlockInterNum(Handle page,Handle block)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	uint32_t rc = BLOCK_H_H(page,block).GetInterNum();
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(void)    CPAGE_SetBlockInterNum(Handle page,Handle block,uint32_t inter)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
	BLOCK_H_H(page,block).SetInterNum(inter);
	EPILOG;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_GetBlockDataPtr(Handle page, Handle block, Handle Type, void ** lpData)
{
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);
#ifdef _DEBUG
	_ASSERT(CPAGE_GetNameInternalType(Type));
#endif
	Bool32 rc = BLOCK_H_H(page,block).GetDataPtr(Type, lpData);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_GetInternalType(const char * name)
{
	PROLOG;
	Handle rc = 0;
	NAMEDATA nd(name);
	SetReturnCode_cpage(IDS_ERR_NO);
	rc = NameData.FindFirst(nd);
	if(!rc)
		rc = NameData.AddTail(nd);
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(char *)  CPAGE_GetNameInternalType(Handle type)
{
	char * rc = NULL;
	PROLOG;
	SetReturnCode_cpage(IDS_ERR_NO);

	if(type != NULL)
		rc = NameData.GetItem(type);
	EPILOG;
	return rc;
}
