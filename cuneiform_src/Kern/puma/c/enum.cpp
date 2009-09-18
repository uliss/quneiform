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

#include "mpuma.h"

static struct
{
    int format;
    int code[10];
}
enumFormatCode[] = {

    PUMA_TOEDNATIVE		,
    {
        PUMA_CODE_UNKNOWN         ,
        -1
    },
#ifdef _DEBUG
PUMA_DEBUG_TOTEXT           ,
{
        PUMA_CODE_ANSI            ,
        -1
    },
#endif
PUMA_TOTEXT           ,
{
        PUMA_CODE_ASCII           ,
        PUMA_CODE_ANSI            ,
        PUMA_CODE_KOI8            ,
        PUMA_CODE_ISO             ,
        PUMA_CODE_UTF8            ,
        -1
    },
PUMA_TOSMARTTEXT       ,
{
        PUMA_CODE_ASCII           ,
        PUMA_CODE_ANSI            ,
        PUMA_CODE_KOI8            ,
        PUMA_CODE_ISO             ,
        PUMA_CODE_UTF8            ,
        -1
    },
PUMA_TORTF               ,
{
        PUMA_CODE_ANSI            ,
        -1
    },
PUMA_TOTABLETXT           ,
{
        PUMA_CODE_ANSI            ,
        -1
    },
/*		PUMA_TOTABLECSV           ,
         {
                         PUMA_CODE_ANSI            ,
                        -1
         },*/
PUMA_TOTABLEDBF           ,
{
        PUMA_CODE_ASCII           ,
        PUMA_CODE_ANSI            ,
        -1
    },
/*	PUMA_TOTABLEWKS           ,
         {
                         PUMA_CODE_ANSI            ,
                        -1
         },*/
PUMA_TOHTML               ,
{
        PUMA_CODE_ANSI            ,
        PUMA_CODE_KOI8            ,
        PUMA_CODE_ISO             ,
        PUMA_CODE_UTF8            ,
        -1
    },
-1,
{
        -1
            }
};

static long _SearchFormat(long prev)
{
    long rc = -1;
    for(int i=0;
        enumFormatCode[i].format>=0 &&
        i<sizeof(enumFormatCode)/sizeof(enumFormatCode[0]);
    i++)
        if(prev == enumFormatCode[i].format)
        {
        rc = i;
        break;
    }
    return rc;
}

long	_EnumFormats(long prev)
{
    long rc = -1;
    if(prev<0)
        rc = enumFormatCode[0].format;
    else
    {
        int i = _SearchFormat(prev);
        if(i>=0)
            rc = enumFormatCode[i+1].format;
    }
    return rc;
}

long	_EnumCodes(long f,long prev)
{

    long rc = -1;
    int format = _SearchFormat(f);
    if(format>=0)
    {
        if(prev<0)
            rc = enumFormatCode[format].code[0];
        else
        {
            for(int i=0;
                enumFormatCode[format].code[i]>=0 &&
                i<sizeof(enumFormatCode[format].code)/sizeof(enumFormatCode[format].code[0]);
            i++)
                if(prev == enumFormatCode[format].code[i])
                {
                rc = enumFormatCode[format].code[i+1];
                break;
            }
        }
    }
    return rc;
}

static long enumLanguage[] = {
    LANG_ENGLISH   ,	// 0
    LANG_GERMAN    ,	// 1
    LANG_FRENCH    ,	// 2
    LANG_RUSSIAN   ,	// 3
    LANG_SWEDISH   ,	// 4
    LANG_SPANISH   ,	// 5
    LANG_ITALIAN   ,	// 6
    LANG_RUSENG    ,	// 7
    LANG_UKRAINIAN ,	// 8
    LANG_SERBIAN   ,	// 9
    LANG_CROATIAN  ,	// 10
    LANG_POLISH    ,	// 11
    LANG_DANISH    ,	// 12
    LANG_PORTUGUESE,	// 13
    LANG_DUTCH     ,	// 14
    LANG_DIG       ,	// 15
    LANG_UZBEK     ,	// 16
    LANG_KAZ       ,	// 17
    LANG_KAZ_ENG   ,	// 18
    LANG_CZECH	    ,	// 19 01.09.2000 E.P.
    LANG_ROMAN     ,   // 20
    LANG_HUNGAR    ,   // 21
    LANG_BULGAR    ,   // 22
    LANG_SLOVENIAN ,	// 23 25.05.2001 E.P.
    LANG_LATVIAN   ,	// 24 16.07.2001 E.P.
    LANG_LITHUANIAN,	// 25
    LANG_ESTONIAN  ,	// 26
    LANG_TURKISH   ,	// 27
    -1
};

long _EnumLanguage(long prev)
{
    long rc = -1;
    if(prev<0)
        rc = enumLanguage[0];
    else
    {
        long next = prev;
        for(int i=0;i<sizeof(enumLanguage)/sizeof(enumLanguage[0])-1;i++)
        {
            if(enumLanguage[i]==next)
            {
                next = enumLanguage[i + 1];
                //              if(REXC_IsLanguage((uchar)next) && RSTR_IsLanguage((uchar)next) )
                if(RRECCOM_IsLanguage((uchar)next) && RSTR_IsLanguage((uchar)next) )
                {
                    rc = next;
                    break;
                }
                else
                    rc = -1;
            }
        }
    }
    return rc;
}

static long enumFormatMode[] =
{
    PUMA_FORMAT_NONE,
    PUMA_FORMAT_ALL,
    //	PUMA_FORMAT_ONLY_FRAME,
    -1
};

long _EnumFormatMode(long prev)
{
    long rc = -1;
    if(prev<0)
        rc = enumFormatMode[0];
    else
    {
        for(int i=0;i<sizeof(enumFormatMode)/sizeof(enumFormatMode[0]);i++)
        {
            if(enumFormatMode[i]==prev)
            {
                rc = enumFormatMode[i + 1];
                break;
            }
        }
    }
    return rc;
}

static long enumTable[] =
{
    PUMA_TABLE_NONE,
    PUMA_TABLE_DEFAULT,
    /*	PUMA_TABLE_ONLY_LINE,
        PUMA_TABLE_ONLY_TEXT,
        PUMA_TABLE_LINE_TEXT,*/
    -1
};

long _EnumTable(long prev)
{
    long rc = -1;
    if(prev<0)
        rc = enumTable[0];
    else
    {
        for(int i=0;i<sizeof(enumTable)/sizeof(enumTable[0]);i++)
        {
            if(enumTable[i]==prev)
            {
                rc = enumTable[i + 1];
                break;
            }
        }
    }
    return rc;
}

static long enumPicture[] =
{
    PUMA_PICTURE_NONE,
    PUMA_PICTURE_ALL,
    -1
};

long _EnumPicture(long prev)
{
    long rc = -1;
    if(prev<0)
        rc = enumPicture[0];
    else
    {
        for(int i=0;i<sizeof(enumPicture)/sizeof(enumPicture[0]);i++)
        {
            if(enumPicture[i]==prev)
            {
                rc = enumPicture[i + 1];
                break;
            }
        }
    }
    return rc;
}
