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

#include "backup.h"

PtrList<BACKUPPAGE>	Page;
Handle				hCurPage = NULL;


//#################################
BACKUPPAGE::BACKUPPAGE()
{
	hCurBackUp = NULL;
}
//#################################
BACKUPPAGE::~BACKUPPAGE()
{
	BackUpPage.Clear();
}
//#################################
void BACKUPPAGE::Clear()
{
	BackUpPage.Clear();
	hCurBackUp = NULL;
}
//#################################
Handle BACKUPPAGE::BackUp(Handle backup)
{
	Handle hBackupPage = backup == NULL ? hCurBackUp : backup;
// Удалить все страницы позднее текущей.
	Handle prevPage  = NULL;
	while(hBackupPage != prevPage)
	{
		prevPage = hBackupPage;
		PAGE & p = BackUpPage.GetNext(hBackupPage);
		if(hCurBackUp != prevPage)
			BackUpPage.Del(prevPage);
	}

	Handle hPage = BackUpPage.AddTail(*this);

	if(hPage==NULL)
		return NULL;

	hCurBackUp = hPage;
	return  hCurBackUp;
}
//#################################
Bool32 BACKUPPAGE::Redo(Handle backup)
{
	if(hCurBackUp)
	{
		if(backup)
		{
			*(PAGE*)this = BackUpPage.GetItem(backup);
			hCurBackUp = backup;
		}
		else
			*(PAGE*)this = BackUpPage.GetNext(hCurBackUp);

		return TRUE;
	}
return FALSE;
}
//#################################
Bool32 BACKUPPAGE::Undo(Handle backup)
{
	if(hCurBackUp)
	{
		if(backup)
		{
			*(PAGE*)this = BackUpPage.GetItem(backup);
			hCurBackUp = backup;
		}
		else
			*(PAGE*)this = BackUpPage.GetPrev(hCurBackUp);

		return TRUE;
	}
return FALSE;
}
//#################################
Bool32 BACKUPPAGE::Save(Handle to)
{
	int count = BackUpPage.GetCount();
	Bool32 rc = FALSE;
	int i,position;

	rc = myWrite(to,&count,sizeof(count))==sizeof(count);
	if(count)
	{
		position = BackUpPage.GetPos(hCurBackUp);
		if(rc) rc = myWrite(to,&position,sizeof(position))==sizeof(position);

		if(rc == TRUE && count)
			for(i=0;i<count;i++)
				BackUpPage.GetItem(BackUpPage.GetHandle(i)).Save(to);
	}
	if(rc)
		rc = PAGE::Save(to);


return rc;


}
//#################################
Bool32 BACKUPPAGE::Restore(Handle from)
{
 Bool32 rc = FALSE;
 int count,i,position;

	BackUpPage.Clear();

  	rc = myRead(from,&count,sizeof(count))==sizeof(count);
	if(count)
	{
		if(rc) rc = myRead(from,&position,sizeof(position))==sizeof(position);

		for(i=0;i<count && rc==TRUE;i++)
		{
			PAGE page;
			rc = page.Restore(from);
			if(rc)
				BackUpPage.AddTail(page);
		}

		if(rc && position>=0)
			hCurBackUp = BackUpPage.GetHandle(position);
	}
	if(rc)
		rc = PAGE::Restore(from);
return rc;
}
//#################################
Bool32 BACKUPPAGE::SaveCompress(Handle to)
{
	int count = BackUpPage.GetCount();
	Bool32 rc = FALSE;
	int i,position;

	rc = myWrite(to,&count,sizeof(count))==sizeof(count);
	if(count)
	{
		position = BackUpPage.GetPos(hCurBackUp);
		if(rc) rc = myWrite(to,&position,sizeof(position))==sizeof(position);

		if(rc == TRUE && count)
			for(i=0;i<count;i++)
				BackUpPage.GetItem(BackUpPage.GetHandle(i)).SaveCompress(to);
	}
	if(rc)
		rc = PAGE::SaveCompress(to);


return rc;


}
//#################################
Bool32 BACKUPPAGE::RestoreCompress(Handle from)
{
 Bool32 rc = FALSE;
 int count,i,position;

	BackUpPage.Clear();

  	rc = myRead(from,&count,sizeof(count))==sizeof(count);
	if(count)
	{
		if(rc) rc = myRead(from,&position,sizeof(position))==sizeof(position);

		for(i=0;i<count && rc==TRUE;i++)
		{
			PAGE page;
			rc = page.RestoreCompress(from);
			if(rc)
				BackUpPage.AddTail(page);
		}

		if(rc && position>=0)
			hCurBackUp = BackUpPage.GetHandle(position);
	}
	if(rc)
		rc = PAGE::RestoreCompress(from);
return rc;
}
//#################################
BACKUPPAGE & BACKUPPAGE::operator = (BACKUPPAGE & Page)
{
	int count = Page.BackUpPage.GetCount();

	BackUpPage.Clear();
	for(int i=0;i<count;i++)
		BackUpPage.AddTail(Page.BackUpPage.GetItem(Page.BackUpPage.GetHandle(i)));

	if(count)
	{
		int curr = Page.BackUpPage.GetPos(Page.hCurBackUp);
		hCurBackUp = BackUpPage.GetHandle(curr);
	}

	*(PAGE *)this = Page;

	return *this;
}

