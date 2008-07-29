/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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
			-1
	 },
			PUMA_TOSMARTTEXT       ,
	 {
			 PUMA_CODE_ASCII           ,
			 PUMA_CODE_ANSI            ,
			 PUMA_CODE_KOI8            ,
			 PUMA_CODE_ISO             ,
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

static long enumLanguage[] =
{
	PUMA_LANG_ENGLISH   ,	// 0
	PUMA_LANG_GERMAN    ,	// 1
	PUMA_LANG_FRENCH    ,	// 2
	PUMA_LANG_RUSSIAN   ,	// 3
	PUMA_LANG_SWEDISH   ,	// 4
	PUMA_LANG_SPANISH   ,	// 5
	PUMA_LANG_ITALIAN   ,	// 6
	PUMA_LANG_RUSENG    ,	// 7
	PUMA_LANG_UKRAINIAN ,	// 8
	PUMA_LANG_SERBIAN   ,	// 9
	PUMA_LANG_CROATIAN  ,	// 10
	PUMA_LANG_POLISH    ,	// 11
	PUMA_LANG_DANISH    ,	// 12
	PUMA_LANG_PORTUGUESE,	// 13
	PUMA_LANG_DUTCH     ,	// 14
	PUMA_LANG_DIG       ,	// 15
   	PUMA_LANG_UZBEK     ,	// 16
   	PUMA_LANG_KAZ       ,	// 17
   	PUMA_LANG_KAZ_ENG   ,	// 18
	PUMA_LANG_CZECH	    ,	// 19 01.09.2000 E.P.
	PUMA_LANG_ROMAN     ,   // 20
	PUMA_LANG_HUNGAR    ,   // 21
	PUMA_LANG_BULGAR    ,   // 22
	PUMA_LANG_SLOVENIAN ,	// 23 25.05.2001 E.P.
	PUMA_LANG_LATVIAN   ,	// 24 16.07.2001 E.P.
	PUMA_LANG_LITHUANIAN,	// 25
	PUMA_LANG_ESTONIAN  ,	// 26
	PUMA_LANG_TURKISH   ,	// 27
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
//              if(REXC_IsLanguage((Word8)next) && RSTR_IsLanguage((Word8)next) )
                if(RRECCOM_IsLanguage((Word8)next) && RSTR_IsLanguage((Word8)next) )
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
