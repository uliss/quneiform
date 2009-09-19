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

#include "backup.h"
#include "block.h"
#include "mymem.h"

//##############################
BLOCK::BLOCK()
{
	UserNum = 0;
	Flags = 0;
	InterNum = 0;
}
//##############################
BLOCK::~BLOCK()
{
}
//##############################
Bool32  BLOCK::Create(Handle type, uint32_t usernum , uint32_t flags ,void * lpdata , uint32_t size )
{
	UserNum = usernum;
	Flags = flags;
	InterNum = 0;


	SetData(type,lpdata,size);
return TRUE;
}
//##############################
BLOCK & BLOCK::operator = (BLOCK & Block)
{
	UserNum = Block.UserNum;
	Flags = Block.Flags;
	InterNum = Block.InterNum;


	*(DATA *)this = Block;
return *this;
}
//##############################
Bool32 BLOCK::operator == (BLOCK & Block)
{
	if(	UserNum == Block.UserNum &&
		Flags == Block.Flags &&
		InterNum == Block.InterNum &&
		*(DATA *)this == Block)
		return TRUE;

	return FALSE;
}
//##############################
Bool32 BLOCK::Save(Handle to)
{
	if( myWrite(to,&UserNum,sizeof(UserNum))==sizeof(UserNum) &&
		myWrite(to,&Flags,sizeof(Flags))==sizeof(Flags) &&
		DATA::Save(to) &&
		myWrite(to,&InterNum,sizeof(InterNum))==sizeof(InterNum))
	   return TRUE;

	return FALSE;
}
//##############################
Bool32 BLOCK::Restore(Handle from)
{
	if( myRead(from,&UserNum,sizeof(UserNum))==sizeof(UserNum) &&
		myRead(from,&Flags,sizeof(Flags))==sizeof(Flags) &&
		DATA::Restore(from) &&
		myRead(from,&InterNum,sizeof(InterNum))==sizeof(InterNum))
	   return TRUE;

	return FALSE;
}
//##############################
Bool32 BLOCK::SaveCompress(Handle to)
{
	if( myWrite(to,&UserNum,sizeof(UserNum))==sizeof(UserNum) &&
		myWrite(to,&Flags,sizeof(Flags))==sizeof(Flags) &&
		DATA::SaveCompress(to) &&
		myWrite(to,&InterNum,sizeof(InterNum))==sizeof(InterNum))
	   return TRUE;

	return FALSE;
}
//##############################
Bool32 BLOCK::RestoreCompress(Handle from)
{
	if( myRead(from,&UserNum,sizeof(UserNum))==sizeof(UserNum) &&
		myRead(from,&Flags,sizeof(Flags))==sizeof(Flags) &&
		DATA::RestoreCompress(from) &&
		myRead(from,&InterNum,sizeof(InterNum))==sizeof(InterNum))
	   return TRUE;

	return FALSE;
}

static 	CPAGE_CONVERTOR s_ConvertorBlocks = {0,DefConvertBlock};
//#################################
CPAGE_CONVERTOR SetConvertorBlocks(CPAGE_CONVERTOR convertor)
{
	CPAGE_CONVERTOR old = s_ConvertorBlocks;

	s_ConvertorBlocks = convertor;

	return old;
}
//#################################
uint32_t BLOCK::Convert(Handle type,void * lpdata,uint32_t size)
{
	uint32_t rc = 0;
	rc = (*s_ConvertorBlocks.fnConvertor)(s_ConvertorBlocks.dwContext,
									Type,lpData,Size,
									type,lpdata,size);
return rc;
}
