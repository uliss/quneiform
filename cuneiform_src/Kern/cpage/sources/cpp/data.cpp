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
#include <memory.h>
#include <string.h>

#include "cpage.h"
#include "data.h"
#include "mymem.h"
#include "backup.h"

//##############################
DATA::DATA()
{
	Type = reinterpret_cast<void*> (-1);
	Size = 0;
	lpData = NULL;
}
//##############################
DATA::~DATA()
{
	if(lpData)
		delete []lpData;
}
//##############################
Bool32   DATA::SetData(Handle type, void * lpdata, uint32_t size)
{
	Type = type;
	Size = size;

	if(lpData)
	{
		delete [] lpData;
		lpData = NULL;
	}

	if(lpdata && size)
	{
		lpData = new char[size];
		if(lpData==NULL)
		{
			Size = 0;
			return FALSE;
		}
		memcpy(lpData,lpdata,size);
	}
return TRUE;
}
//##############################
uint32_t   DATA::GetData(Handle type, void * lpdata,uint32_t size)
{
	if(type==Type)
	{
		if(lpdata==NULL)
			return Size;
		if(Size && lpData)
			memcpy(lpdata,lpData,Size);
	}
	else
		return Convert(type,lpdata,size);

return Size;
}
//#################################
DATA & DATA::operator = (DATA & data)
{
	SetData(data.Type,data.lpData,data.Size);
return *this;
}
//#################################
Bool32 DATA::operator == (DATA & data)
{
	if( Type == data.Type &&
		Size == data.Size)
	{
		if(lpData == data.lpData && lpData == NULL)
			return TRUE;
		return memcmp(lpData,data.lpData,Size)==0;
	}
	return FALSE;
}
//#################################
Bool32 DATA::Save(Handle to)
{
	char * lpName = CPAGE_GetNameInternalType(Type);
	_ASSERT(lpName);
	uint32_t len = strlen(lpName) + 1;

	if(myWrite(to,&len,sizeof(len)) == sizeof(len) &&
	   myWrite(to,lpName,len) == len &&
	   myWrite(to,&Size,sizeof(Size)) == sizeof(Size) &&
	   (Size==0 ||	myWrite(to,lpData,Size)==Size))
		return TRUE;
	return FALSE;
}
//#################################
Bool32 DATA::Restore(Handle from)
{
	uint32_t len = 0;
	char Name[260];
	if(myRead(from,&len,sizeof(len)) == sizeof(len) &&
	   myRead(from,Name,len) == len)
	{
		Type = CPAGE_GetInternalType(Name);
		if(myRead(from,&Size,sizeof(Size)) == sizeof(Size))
		{
			Bool32 rc = FALSE;
		   if(!Size)
			   rc = TRUE;
		   else
		   {
			   if(lpData)
			   {
				   delete [] lpData;
				   lpData = NULL;
			   }
			   lpData = new char[Size];
			   if(lpData)
				   rc = myRead(from,lpData,Size) == Size;
		   }
		   return rc;
		}
	}
	return FALSE;
}
//#################################
Bool32 DATA::SaveCompress(Handle to)
{
	if (Size==0)
		return 	Save(to);

	Bool32 rv;
	char *compressedData, *lpDataSave=lpData;
	uint32_t compressedSize, SizeSave=Size;
	CleanData(Type,lpData,Size);
	if (!Compress(lpData,Size,&compressedData,&compressedSize))
		return FALSE;
	lpData=compressedData; 	Size=compressedSize;
	rv=Save(to);
	lpData=lpDataSave; 	Size=SizeSave;
	delete []compressedData;
	return rv;
}

//#################################
Bool32 DATA::RestoreCompress(Handle from)
{
	if (!Restore(from))
		return FALSE;

	if (Size==0)
		return TRUE;

	char *decomData;  uint32_t decomSize;
	if (!Decompress(lpData,Size,&decomData,&decomSize))
		return FALSE;

	if(lpData)	delete [] lpData;
	lpData=decomData;  Size=decomSize;

	if (!ComplianceVersions(Type,&lpData,&Size))
		return FALSE;

	return TRUE;
}
