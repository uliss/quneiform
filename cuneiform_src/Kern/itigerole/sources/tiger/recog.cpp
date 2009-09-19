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

#include "stdafx.h"

#if defined(__TIGER__)

#include <io.h>
#include "tgcall.h"
#include "tgcallbk.h"
#include "tigerdef.h"
#include "cpage.h"
#include "polyblock.h"
#include "puma.h"
#include "ctiimage.h"
#include "criimage.h"

#define _IMAGE_USER		"InputImage"
#define _IMAGE_ROTATE	"RotateImage"

typedef struct
	{
	uint32_t dwBeg;   // Начало текущего диапозона
	uint32_t dwEnd;	// Конец

	uint32_t dwStep;  // текущий номер шага
	char * name;	// текущее название шага
	} PRGTIME;

extern PRGTIME g_PrgTime;
// Функции прогресс индикатора.
// 1. Для инициализации внутреннего счетчика надо вызвать InitPRGTIME.
// 2. Внутри одной функции разбиение идет всегда от 0 до 100 процентов
// 3. Перед вызовом функции надо установить диапозон прогресса для этой функции в масштабе 0 - 100
//		Для этого надо выполнить StorePRGTIME(uint32_t beg, uint32_t end);
// 4. После возврата из этой функции надо восстановить счетчик функцией RestorePRGTIME(PRGTIME prev)
//		Параметром этой функции должно быть возвращаемое значение функции StorePRGTIME

Bool32	InitPRGTIME();
Bool32	DonePRGTIME();
PRGTIME	StorePRGTIME(uint32_t beg, uint32_t end);// Устанавливает дапозон изменений, который учитывается в  ProgressStep
void 	RestorePRGTIME(PRGTIME prev);// Устанавливает дапозон изменений, который учитывается в  ProgressStep


//////////////////////////////////////////////////////////////////////////
// Коррекция координат, связанная со спецификой поворота изображения

static Point32 CorrectPointTo(int32_t x,int32_t y,int32_t Incline2048)
{
	Point32 rc;
	// Вернем в реальные координаты
		y = (int16_t) (y + (int32_t) x * Incline2048 / 2048);
		x = (int16_t) (x - (int32_t) y * Incline2048 / 2048);
	// Переведем в идеальные
		y = (int16_t) (y - (int32_t) x * Incline2048 / 2048);
		x = (int16_t) (x + (int32_t) y * Incline2048 / 2048);
rc.x = x;
rc.y = y;
return rc;
}
//////////////////////////////////////////////////////////////////////////
static void CorrectPointFrom(Point32 p, int32_t & x,int32_t & y,int32_t Incline2048)
{
	x = p.x;
	y = p.y;
	// Вернем в реальные координаты
		x = (int16_t) (x - (int32_t) y * Incline2048 / 2048);
		y = (int16_t) (y + (int32_t) x * Incline2048 / 2048);
	// Переведем в идеальные
		x = (int16_t) (x + (int32_t) y * Incline2048 / 2048);
		y = (int16_t) (y - (int32_t) x * Incline2048 / 2048);
}
///////////////////////////////////////////////////////////////////////////
char * _GetVersion()
{
	return "Tiger";
}

short _GetLanguage()
{
	short rc = 0;

		rc = Tiger_GetLanguage();

	return rc;
}

void _SetLanguage(short newVal)
{
		Tiger_SetLanguage(newVal);
}

long _Status()
{
	long rc;
		rc = Tiger_Status();
	return rc;
}

char * _ErrorMessage(long rc)
{
	char * p;
		p = Tiger_ErrorMessage( (uint16_t)rc );
	return p;
}

Bool _Load(CRecognition * pThis)
{
	HINSTANCE hInstance = GetModuleHandle("ATIGER.DLL");
	char szFullPath[_MAX_PATH];
	Bool rc;

	GetModuleFileName(hInstance,szFullPath,sizeof(szFullPath));
	char * p = strstr(strupr(szFullPath),"ATIGER.DLL");
	if(p)
		*p='\0';

	g_pRecognition = pThis;

	rc = Tiger_Load (hInstance,szFullPath) ;
	if(rc)
	{
		char szPath[_MAX_PATH];
		if(GetTempPath(sizeof(szPath),szPath))
			Tiger_RW_Options(FUNC_SET_TEMP_DIR,szPath);
		if(GetSystemDirectory(szPath,sizeof(szPath)))
			Tiger_RW_Options(FUNC_SET_WINDOWS_DIR,szPath);
	}

	if(!rc)
	{
		g_pRecognition->SetError(_ErrorMessage(_Status()));
		return rc;
	}
// инициализируем контейнер описания страницы
	rc = CPAGE_Init(0,NULL);
	if(!rc)
		g_pRecognition->SetError((char *)CPAGE_GetReturnString(CPAGE_GetReturnCode()));
// инициализируем контейнер изображений
	rc = CIMAGE_Init(0,NULL);
	if(!rc)
		g_pRecognition->SetError((char *)CIMAGE_GetReturnString(CIMAGE_GetReturnCode()));
// инициализируем обработчик изображений
	rc = RIMAGE_Init(0,NULL);
	if(!rc)
		g_pRecognition->SetError((char *)RIMAGE_GetReturnString(RIMAGE_GetReturnCode()));

	return rc;
}

void _Unload()
{

	Tiger_RW_Options(FUNC_DELETE_TEMP_FILES,"");
	Tiger_Unload();

	CPAGE_Done();
	CIMAGE_Done();
	RIMAGE_Done();
}

short  _EnumLanguage(short PrevLang)
{
	short rc;
		rc = Tiger_EnumLanguage(PrevLang);
	return rc;
}

Bool _Open(char * hDIB, char * FileName)
{
	Bool rc = FALSE;

	if(g_hPage)
		_Close( );

	if(hDIB==0)
	{
		g_pRecognition->SetError(IDS_ERROR_DIBISNULL);
		return FALSE;
	}

	g_pDIB = hDIB;
	strcpy(g_strFileName,FileName);

	//
	// Запишем изображение
	//
	if(!CIMAGE_WriteDIB((puchar)_IMAGE_USER,g_pDIB,1))
	{
		g_pRecognition->SetError((char *)CIMAGE_GetReturnString(CIMAGE_GetReturnCode()));
		rc = FALSE;
	}
	else
	{
		LPBITMAPINFOHEADER bmpInfo = (LPBITMAPINFOHEADER)g_pDIB;
		PAGEINFO info;
		strcpy((char*)info.szImageName,FileName);
		info.Page = 1; // номер страницы
		info.DPIX = bmpInfo->biXPelsPerMeter*254/10000;
		info.DPIY= bmpInfo->biYPelsPerMeter*254/10000;
		info.BitPerPixel = bmpInfo->biBitCount;
		info.X = 0; // смещение относительно глобальных координат
		info.Y = 0;
		info.Width = bmpInfo->biWidth; // размеры в пиксеалх
		info.Height = bmpInfo->biHeight;
		info.Incline2048 = 0; // наклон страницы в долях от 2048

		g_hPage = CreatePageWithPageInfo(info);

		if(!g_hPage)
		{
			g_pRecognition->SetError((char *)CPAGE_GetReturnString(CPAGE_GetReturnCode()));
			rc = FALSE;
		}
		else
			rc = TRUE;
	}
	return rc;
}

Bool _Close()
{
	g_pDIB = NULL;

	remove(g_strTempEdName);
		CPAGE_DeleteAll();
		CIMAGE_Reset();
	return TRUE;
}

Bool _Recognition()
{
Bool rc = FALSE;

	InitPRGTIME();

	uint16_t wOptions = TSRO_PICTURES_NO;
		 wOptions |= g_bSpeller ? TSRO_SPELLER : 0 ;
		 wOptions |= g_bOneColumn ? TSRO_FORCE_ONE_COLUMN : 0 ;
		 wOptions |= g_bFax100 ? TSRO_FAX100 : 0 ;
		 wOptions |= g_bDotMatrix ? TSRO_DOTMATRIX : 0 ;



	rc = Tiger_SetRecognitionOptions(wOptions);
	if(rc)
	{
			rc = Tiger_Recognition(g_strTempEdName);
	}

	if(!rc)
	{
		uint32_t status = _Status();
		if(status)
			g_pRecognition->SetError(_ErrorMessage(status));
	}

	DonePRGTIME();

return rc;
}

Bool _Save(void * hEdPage,char * lpFileName,long format, long code, Bool32 bAppend)
{
	Bool rc = FALSE;

	InitPRGTIME();

	#define USE_NONE	0x0000
	#define USE_FRAME	0x0001
	#define NOBOLD		0x0002
	#define NOCURSIV	0x0004
	#define NOFORMAT	0x0008
	#define NOSIZE		0x0010

	uint16_t wOptions = (int)!g_bBold   & NOBOLD |
			   (int)!g_bItalic & NOCURSIV |
			   (int)!g_bSize   & NOSIZE |
			   (int)!g_bFormat & NOFORMAT;

	rc = Tiger_SetEdSaveRtfOptions(wOptions,
			g_strSerifName,g_strSansSerifName,g_strCourierName,
			"","","");
	if(rc)
		rc = Tiger_ExtEdToText((char*)g_strTempEdName,lpFileName,(short)format|(bAppend ? TIGER_APPEND:0 ),(short)code,g_btUnrecogChar);


	if(!rc)
		g_pRecognition->SetError(_ErrorMessage(_Status()));

	DonePRGTIME();

	return rc;
}

Bool _PageAnalysis(Bool bUpdateCPAGE)
{
	Bool rc;
	uint16_t wOptions = TSRO_PICTURES_NO;
		 wOptions |= g_bSpeller ? TSRO_SPELLER : 0 ;
		 wOptions |= g_bOneColumn ? TSRO_FORCE_ONE_COLUMN : 0 ;
		 wOptions |= g_bFax100 ? TSRO_FAX100 : 0 ;
		 wOptions |= g_bDotMatrix ? TSRO_DOTMATRIX : 0 ;

	InitPRGTIME();

	rc = Tiger_SetRecognitionOptions(wOptions);
	if(rc)
		rc = Tiger_PageAnalysis();

	if(!rc)
	{
		uint32_t status = _Status();
		if(status)
			g_pRecognition->SetError(_ErrorMessage(status));
	}

	if(bUpdateCPAGE && rc)
	{// Заполним контейнер CPAGE
		POLY_	block;
		TIGER_LAYOUTBLOCK * lpBlocks = NULL;
		INT16 nBlocks = 0;

		rc = TigerLayout_UpdateBlocks(&nBlocks, &lpBlocks);
		if(!rc)
		{
			g_pRecognition->SetError(_ErrorMessage(_Status()));
			rc = FALSE;
		}
		else
		{

			uchar * Buffer = (uchar FAR *)lpBlocks;
			TIGER_LAYOUTSTRIP * pStrip = NULL;
			TIGER_LAYOUTSTRIPHANDLE * pStripHandle = NULL;


			int i = 0;
			while( i < nBlocks )
			{
				lpBlocks =(TIGER_LAYOUTBLOCK FAR *)Buffer;
				Buffer += sizeof(TIGER_LAYOUTBLOCK);

				if(lpBlocks -> Type == BLOCK_PICTURE){
				/*если блок-картинка, тогда надо прочитать структуры стрипов*/
					pStripHandle =(TIGER_LAYOUTSTRIPHANDLE FAR *)Buffer;
					Buffer += sizeof(TIGER_LAYOUTSTRIPHANDLE);

					pStrip = (TIGER_LAYOUTSTRIP FAR *)Buffer;
					Buffer += sizeof(TIGER_LAYOUTSTRIP) * pStripHandle->n_strip;
					}
				  else {
				   pStripHandle = NULL;
				   pStrip       = NULL;
				   }
				/* теперь:
						lpBlocks 	- указатель на TIGER_LAYOUTBLOCK
						pStripHandle 	- указатель на TIGER_LAYOUTSTRIPHANDLE или NULL
						pStrip		- указатель на TIGER_LAYOUTSTRIP или NULL
				*/
				i++;
				// ВНИМАНИЕ! Эта версия не поддерживает картинки !
				if(lpBlocks -> Type == BLOCK_PICTURE)
				  continue;
				switch(lpBlocks -> Type)
				{
				case BLOCK_PICTURE:
					block.com.type = TYPE_IMAGE;// Картинка
					break;
				case BLOCK_TEXT:
					block.com.type = TYPE_TEXT;// Текст
					break;
				}

			INT16 nNumerator = 1;
			INT16 nDenominator = 1;

				if(!TigerLayout_GetPageIncline (&nNumerator,&nDenominator))
				{
					g_pRecognition->SetError(_ErrorMessage(_Status()));
					rc = FALSE;
				}
				else
				{
					PAGEINFO info;
					GetPageInfo(g_hPage,&info);
					info.Incline2048 = nNumerator * 2048 / nDenominator;
					SetPageInfo(g_hPage,info);
					Point32 lt = CorrectPointTo(lpBlocks->xLeft, lpBlocks->yTop,info.Incline2048);
					Point32 rb = CorrectPointTo(lpBlocks->xRight, lpBlocks->yBottom,info.Incline2048);
					Point32 rt = {rb.x,lt.y};
					Point32 lb = {lt.x,rb.y};

					block.com.number = i;//порядковый номер
					block.com.Color = 0;
					block.com.count = 4;
					block.com.Vertex[0] = lt;
					block.com.Vertex[1] = rt;
					block.com.Vertex[2] = rb;
					block.com.Vertex[3] = lb;
					block.alphabet = 0;
					rc = CPAGE_CreateBlock(g_hPage, TYPE_TEXT,0,0,&block,sizeof(block)) > 0;
				}
				if(!rc)
					break; // код ошибки проветим после выхода из критической секции
			}
			// выход по ошибке может быть только при ошибке в CPAGE
			if(!rc)
				g_pRecognition->SetError((char *)CPAGE_GetReturnString(CPAGE_GetReturnCode()));
			CPAGE_BackUp(g_hPage);
		}
	}
	DonePRGTIME();

	return rc;
}

Bool _FinalRecognition()
{
	Bool rc = TRUE;

	int number_blocks = CPAGE_GetCountBlock(g_hPage);

	InitPRGTIME();

	if(number_blocks==0)
	{	// Фрагменты не выделены.
		// Распознаем по полной программе
		rc = _Recognition();
	}
	else // Фрагменты выделены. Взять описание из CPAGE
	{
		TIGER_LAYOUTBLOCK * lpBlocks = NULL;
		INT16 nBlocks = 0;
		int i;
		// Сохраним описание страницы перед выделением
		Handle hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());

		// Проинициализируем ядро выделением одной колонки
		Bool bOneColumn = g_bOneColumn;
		g_bOneColumn = TRUE;

		PRGTIME	prevAnalys = StorePRGTIME(0, 30);
		rc = _PageAnalysis( FALSE ); // CPAGE не изменяем !

		g_bOneColumn = bOneColumn;
		// Удалим все блоки текста и картинок
		if(rc)
			rc = TigerLayout_UpdateBlocks(&nBlocks, &lpBlocks);
		if(rc)
		{
			BOOL16 bRc;
			uchar * Buffer = (uchar FAR *)lpBlocks;
			for(i=0;i<nBlocks;i++)
			{
			TIGER_LAYOUTSTRIP * pStrip = NULL;
			TIGER_LAYOUTSTRIPHANDLE * pStripHandle = NULL;

				lpBlocks =(TIGER_LAYOUTBLOCK FAR *)Buffer;
				Buffer += sizeof(TIGER_LAYOUTBLOCK);

				if(lpBlocks -> Type == BLOCK_PICTURE){
				/*если блок-картинка, тогда надо прочитать структуры стрипов*/
					pStripHandle =(TIGER_LAYOUTSTRIPHANDLE FAR *)Buffer;
					Buffer += sizeof(TIGER_LAYOUTSTRIPHANDLE);

					pStrip = (TIGER_LAYOUTSTRIP FAR *)Buffer;
					Buffer += sizeof(TIGER_LAYOUTSTRIP) * pStripHandle->n_strip;
					}
				  else {
				   pStripHandle = NULL;
				   pStrip       = NULL;
				   }
				/* теперь:
						lpBlocks 	- указатель на TIGER_LAYOUTBLOCK
						pStripHandle 	- указатель на TIGER_LAYOUTSTRIPHANDLE или NULL
						pStrip		- указатель на TIGER_LAYOUTSTRIP или NULL
				*/
				rc = TigerLayout_DeleteBlock(lpBlocks->Number, &bRc)	;
				if(!rc)
					break;
			}
		// обновим ядро
		TigerLayout_UpdateBlocks(&nBlocks, &lpBlocks);
		}

	 	RestorePRGTIME(prevAnalys);
		if(!rc)
		{
			g_pRecognition->SetError(_ErrorMessage(_Status()));
			rc = FALSE;
		}
		// Прочитаем угол наклона страницы
		PAGEINFO info;
		GetPageInfo(g_hPage,&info);
		// Создадим новые блоки из контейнера CPAGE
		i = CPAGE_GetCountBlock(hPage);
		if(rc)
		{
			for(Handle h = CPAGE_GetBlockFirst(hPage,TYPE_TEXT);
				h!=NULL;
				h = CPAGE_GetBlockNext(hPage,h,TYPE_TEXT))
			{
			POLY_	block;
				if(CPAGE_GetBlockData(hPage,h,TYPE_TEXT, &block, sizeof(block))!=sizeof(block))
				{
					g_pRecognition->SetError((char *)CPAGE_GetReturnString(CPAGE_GetReturnCode()));
					rc = FALSE;
				}
				else
				{
					if(block.com.count != 4)
					{
						g_pRecognition->SetError(IDS_ERROR_NO4COUNT);
						rc = FALSE;
					}

					RECT r;

					CorrectPointFrom(block.com.Vertex[0],r.left,r.top,info.Incline2048);
					CorrectPointFrom(block.com.Vertex[2],r.right,r.bottom,info.Incline2048);

					BOOL16 b;
					rc = TigerLayout_ExtractRectBlock(r, &b);
					if(!rc)
					{
						g_pRecognition->SetError(_ErrorMessage(_Status()));
						rc = FALSE;
					}
				}
			}

		}
		// обновим ядро
		TigerLayout_UpdateBlocks(&nBlocks, &lpBlocks);
		if(rc)
		{
			PRGTIME	prevRecog = StorePRGTIME(31,100);
				rc = Tiger_FinalRecognition (g_strTempEdName);
	 		RestorePRGTIME(prevRecog);
		}
	}
	if(!rc)
		g_pRecognition->SetError(_ErrorMessage(_Status()));

	DonePRGTIME();

	return rc;
}

static struct
{
	int format;
	int code[10];
}
 enumFormatCode[] = {
	 TIGER_TOEDNATIVE		,
	 {
			 TIGER_CODE_UNKNOWN         ,
			-1
	 },
			TIGER_TOASCII           ,
	 {
			 TIGER_CODE_ASCII           ,
			 TIGER_CODE_ANSI            ,
			 TIGER_CODE_KOI8            ,
			 TIGER_CODE_ISO             ,
			-1
	 },
			TIGER_TOSMARTASCII       ,
	 {
			 TIGER_CODE_ASCII           ,
			 TIGER_CODE_ANSI            ,
			 TIGER_CODE_KOI8            ,
			 TIGER_CODE_ISO             ,
			-1
	 },
			TIGER_TORTF               ,
	 {
			 TIGER_CODE_ANSI            ,
			-1
	 },
			TIGER_TOANSI               ,
	 {
			 TIGER_CODE_ANSI            ,
			-1
	 },
			TIGER_TOSMARTANSI          ,
	 {
			 TIGER_CODE_ANSI            ,
			-1
	 },
			TIGER_TOTABLETXT           ,
	 {
			 TIGER_CODE_ANSI            ,
			-1
	 },
			TIGER_TOTABLECSV           ,
	 {
			 TIGER_CODE_ANSI            ,
			-1
	 },
			TIGER_TOTABLEDBF           ,
	 {
			 TIGER_CODE_ASCII           ,
			 TIGER_CODE_ANSI            ,
			-1
	 },
			TIGER_TOTABLEWKS           ,
	 {
			 TIGER_CODE_ANSI            ,
			-1
	 },
			TIGER_TOHTML               ,
	 {
			 TIGER_CODE_ANSI            ,
			 TIGER_CODE_KOI8            ,
			 TIGER_CODE_ISO             ,
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

static long enumFormatMode[] =
{
	PUMA_FORMAT_NONE,
	PUMA_FORMAT_ALL,
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
	-1
};

long _EnumTableMode(long prev)
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
	-1
};

long _EnumPictureMode(long prev)
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


Bool _GetRotateDIB(long * phDIB,long * x0,long * y0)
{
	Bool rc = TRUE;
	PAGEINFO info;

	if(rc)
	{
		GetPageInfo(g_hPage,&info);

		if(info.Incline2048>0)
			{
				*x0 = (int32_t)info.Width * info.Incline2048 / 2048 * info.Incline2048 / 2048;
				*y0 = (int32_t)info.Width * info.Incline2048 / 2048;
			}
			else
			{
				*x0 = -(int32_t)info.Height * info.Incline2048 / 2048 + (int32_t)info.Width * info.Incline2048 / 2048 * info.Incline2048 / 2048;
				*y0 = 0;
			}
	}
	//
	// Создадим довернутое изображение
	//
	if(rc)
	{
		if(RIMAGE_Rotate((puchar)_IMAGE_USER,
							(puchar)_IMAGE_ROTATE,
							info.Incline2048,2048, 0) == FALSE)
		{
			g_pRecognition->SetError((char *)RIMAGE_GetReturnString(RIMAGE_GetReturnCode()));
			rc = FALSE;
		}

	}

	if(rc)
	{
		if(!CIMAGE_ReadDIB((puchar)_IMAGE_ROTATE,(void**)phDIB,TRUE))
		{
			g_pRecognition->SetError((char *)CIMAGE_GetReturnString(CIMAGE_GetReturnCode()));
			rc = FALSE;
		}
	}

	return rc;
}

#endif
