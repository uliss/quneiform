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

#include "internal.h"
#pragma hdrstop
#include "std.h"
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////  Функции работы с INI-файлами

#ifdef WIN32
   #include "win_mini.h"
#endif

#include "xpath.h"

#include "compat_defs.h"

/* FIXME */
#define itoa(a, b, c)

static Bool32 IsFullName(const char* szFileName)
{
    int len=strlen(szFileName);
    if(len<2)
        return FALSE;
    if(szFileName[1]==':')
        return TRUE;
    if(!strncmp(szFileName,"\\\\",2))
        return TRUE;
    return FALSE;
}

static void SetFullName(char* szFullFileName,const char* szIniFileName,int32_t nFlags)
{
    strcpy(szFullFileName,szIniFileName);
    if(!IsFullName(szIniFileName) && !(nFlags & STD_SETPROF_DIR_WINDOWS))
    {
       sprintf(szFullFileName,"%s/%s",stdGetHomeDirectory(),szIniFileName);
       XPath xpini = szFullFileName; XPath xpfl = szFullFileName;
	   xpini.CutPath().CheckSlash();
	   if(!(nFlags & STD_SETPROF_DIR_VERSION))
		   xpini +="..\\CONFIG\\";
       xpini += xpfl.CutNameEx();
       strcpy(szFullFileName,(const char*)xpini);
    }
}


STD_FUNC( Bool32 ) stdGetProfileString(char* szString,int32_t* nStrLen,const char* szIniFileName,const char* szSection,const char* szKey,int32_t nFlags, const char* pszStrDefault)
{
    char szFullFileName[_MAX_PATH]={0};
	SetFullName(szFullFileName,szIniFileName,nFlags);
    if(GetPrivateProfileString(szSection,szKey,pszStrDefault,szString,*nStrLen,szFullFileName)==(uint32_t)(*nStrLen-1))
       return FALSE;
    return TRUE;
}

STD_FUNC( int32_t ) stdGetProfileInt(const char* szIniFileName,const char* szSection,const char* szKey,int32_t nFlags,int32_t nValDefault)
{
    char szFullFileName[_MAX_PATH]={0};
	SetFullName(szFullFileName,szIniFileName,nFlags);
    return GetPrivateProfileInt(szSection,szKey,nValDefault,szFullFileName);
}

STD_FUNC( Bool32 ) stdSetProfileString(const char* szString,const char* szIniFileName,const char* szSection,const char* szKey,int32_t nFlags)
{
    char szFullFileName[_MAX_PATH]={0};
	SetFullName(szFullFileName,szIniFileName,nFlags);
    if(!WritePrivateProfileString(szSection,szKey,szString,szFullFileName))
        return FALSE;
    return TRUE;
}

STD_FUNC( Bool32 ) stdSetProfileInt(int32_t nValue,const char* szIniFileName,const char* szSection,const char* szKey,int32_t nFlags)
{
    char szStrNumber[256]={0};
    itoa(nValue,szStrNumber,10);
    char szFullFileName[_MAX_PATH]={0};
	SetFullName(szFullFileName,szIniFileName,nFlags);
    if(!WritePrivateProfileString(szSection,szKey,szStrNumber,szFullFileName))
        return FALSE;
    return TRUE;
}
