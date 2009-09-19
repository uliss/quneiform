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

#include "globus.h"
#include "cpage.h"
#include "backup.h"
#include "polyblock.h"
#include "resource.h"

void CleanData(Handle Type,void * lpData,uint32_t Size)
// Неиспользуемые части массивов заполняем нулями для лучшей упаковки
{
    if(Type == TYPE_TEXT || Type == TYPE_IMAGE)
	{
		POLY_ *poly = (POLY_ *)lpData;
		COMMON *com = &poly->com;
		int maxnum=MaxNum;
		memset(&com->Vertex[com->count],0,(char*)&com->Vertex[maxnum]-(char*)&com->Vertex[com->count]);
		memset(&poly->mark[com->count],0,(char*)&poly->mark[maxnum]-(char*)&poly->mark[com->count]);
	}
	else
    if(Type == TYPE_TABLE)
	{
		TABLE_ *table = (TABLE_ *)lpData;
		COMMON *com = &table->com;
		memset(&com->Vertex[com->count],0,(char*)&com->Vertex[MaxNum]-(char*)&com->Vertex[com->count]);
		int32_t *line=table->LineY;
		memset(&line[table->num_rows+1],0,(char*)&line[MaxHorLines-1]-(char*)&line[table->num_rows+1]);
		line=table->LineX;
		memset(&line[table->num_colons+1],0,(char*)&line[MaxVerLines-1]-(char*)&line[table->num_colons+1]);
		for (int32_t j=0; j<MaxVerLines; j++)
			for (int32_t i=0; i<MaxHorLines; i++)
				if (i>=table->num_rows || j>=table->num_colons)
					table->Visible[i][j][0]=table->Visible[i][j][1]=table->TypeCell[i][j]=0;
	}
}

Bool32 ComplianceVersions(Handle Type, char ** lpData, uint32_t *Size)
//Сравнивает размер блока данных с размером структуры;
// если структура больше - дополняется нулями, если меньше - ошибка
{
	uint32_t NewSize=*Size;
	if (Type==TYPE_TEXT  ||  Type==TYPE_IMAGE)
		 NewSize = sizeof(POLY_);
	else
	if (Type==TYPE_TABLE)
		 NewSize = sizeof(TABLE_);
	if (NewSize < *Size)
		return FALSE;
	if (NewSize > *Size)
	{
		char *lpNewData = new char[NewSize];
		if (!lpNewData)
			return FALSE;
		memcpy(lpNewData,*lpData,*Size);
		memset(lpNewData+(*Size),0,NewSize-(*Size));
		delete *lpData;
		*lpData=lpNewData; 	*Size=NewSize;
	}
	return TRUE;
}

Bool32 Compress(char * lpData, uint32_t Size, char ** compressedData, uint32_t * compressedSize)
{
// Заменяем группу из не менее MIN_REPEAT одинаковых символов на счетчик повторений
#define MIN_REPEAT 2*sizeof(CompressHeader)

	if (Size==0)
		return FALSE;

   char *newData = new char[Size+sizeof(CompressHeader)]; //размер станет таким, если уплотнить не получилось,
   if (!newData)										  //иначе - не больше исходного
	   return FALSE;
   char *lpNewData = newData;

	// Находим пару - обычный фрагмент и фрагмент, заполненный одинаковыми символами;
	// затем оба отписываем
	char * ordinary=lpData,	  //обычный фрагмент
		 * end=ordinary+Size;
	do
	{
		uint32_t count=1;
		char * current=ordinary+1,
			 * repeating=ordinary; //фрагмент, заполненный одинаковыми символами;
		while (current<end)
		{
			if (*current != *repeating)
			{
				if (current-repeating>=MIN_REPEAT)  break;
				repeating=current;
			}
			current++;
		}
		count=current-repeating;

		if (count<MIN_REPEAT)  //дошли до конца, а повторений мало - отвергаем
		{
			repeating += count;  count=0;
		}
		if (repeating>ordinary)  //обычный фрагмент
		{
			CompressHeader head={0};
			head.bCompressed=FALSE;
			head.wCount=repeating-ordinary;
			memcpy(lpNewData,&head,sizeof(head));    lpNewData += sizeof(head);
			memcpy(lpNewData,ordinary,head.wCount); lpNewData += head.wCount;
		}
		if (count)				 //фрагмент, заполненный символом (*repeated)
		{
			CompressHeader head={0};
			head.bCompressed=TRUE;
			head.cRepeater=*repeating;
			head.wCount=count;
			memcpy(lpNewData,&head,sizeof(head));    lpNewData += sizeof(head);
		}
		ordinary=current;
	}
	while(ordinary<end);
	*compressedData = newData;  *compressedSize = lpNewData-newData;
	return TRUE;
}

//#################################
Bool32 Decompress(char * lpData, uint32_t Size, char ** decomData, uint32_t * decomSize)
{
	if (Size==0)
		return FALSE;

	char * old=lpData, *end=lpData+Size;

	//Определяем размер после декомпрессии
	uint32_t newSize=0;
	while (old<end)
	{
		CompressHeader *head = (CompressHeader *)old;
		old += sizeof(CompressHeader);
		if (!head->bCompressed) old += head->wCount;
		newSize += head->wCount;
	}

	//Распаковываем
	char *newData = new char[newSize], *modern=newData;
	if (!newData)
		return FALSE;

	old=lpData;
	while (old<end)
	{
		CompressHeader *head = (CompressHeader *)old;
		old += sizeof(CompressHeader);
		if (head->bCompressed)
			memset(modern,head->cRepeater,head->wCount);
		else
		{
			memcpy(modern,old,head->wCount);
			old += head->wCount;
		}
		modern += head->wCount;
	}
	*decomData=newData;  *decomSize=newSize;
	return TRUE;
}




