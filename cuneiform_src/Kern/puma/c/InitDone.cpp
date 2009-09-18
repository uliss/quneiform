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
#include <string.h>

#define __FRHSTRUC_H

#include "mpuma.h"

Bool32 ModulesInit(Handle  ghStorage)
{
	// CONTEINERS
	if(!CLINE_Init(PUMA_MODULE_CLINE,NULL))
	{
		SetReturnCode_puma(CLINE_GetReturnCode());
		goto lError;
	}
	if(!CFIO_Init(PUMA_MODULE_CFIO,NULL))
	{
		SetReturnCode_puma(CFIO_GetReturnCode());
		goto lError;
	}
	/*
	ghStorage = OpenStorage(szStorage,OS_CREATE);
	if(ghStorage==NULL)
	{
		SetReturnCode_puma(IDS_ERR_CREATESTORAGE);
		goto lError;
	}
	*/
	if(!CIMAGE_Init(PUMA_MODULE_CIMAGE,NULL))
	{
		SetReturnCode_puma(CIMAGE_GetReturnCode());
		goto lError;
	}
	if(!CCOM_Init(PUMA_MODULE_CCOM,NULL))   //  нужна инициализация контейнера CCOM перед
	{               //  вызовом поиска компонент
		SetReturnCode_puma(CCOM_GetReturnCode());
		goto lError;
	}
	if(!CPAGE_Init(PUMA_MODULE_CPAGE,ghStorage))
	{
		SetReturnCode_puma(CPAGE_GetReturnCode());
		goto lError;
	}
	if(!CSTR_Init(PUMA_MODULE_CSTR,ghStorage))
	{
		SetReturnCode_puma(CSTR_GetReturnCode());
		goto lError;
	}
	// RECOGNITIONS
	if(!REXC_Init(PUMA_MODULE_REXC,NULL)) // инициализация библиотеки поиска компонент
	{
		SetReturnCode_puma(REXC_GetReturnCode());
		goto lError;
	}
//	REXC_SetImportData(REXC_OcrPath, GetModulePath());
	if(!RLINE_Init(PUMA_MODULE_RLINE,ghStorage))
	{
		SetReturnCode_puma(RLINE_GetReturnCode());
		goto lError;
	}

    if(!RRECCOM_Init(PUMA_MODULE_RRECCOM,ghStorage))
	{
 		SetReturnCode_puma(RRECCOM_GetReturnCode());
		goto lError;
	}
	RRECCOM_SetImportData(RRECCOM_OcrPath, GetModulePath());

    if(!RSL_Init(PUMA_MODULE_RSL,ghStorage))
	{
 		SetReturnCode_puma(RSL_GetReturnCode());
		goto lError;
	}
	if(!RSTUFF_Init(PUMA_MODULE_RSTUFF,ghStorage))
	{
		SetReturnCode_puma(RSTUFF_GetReturnCode());
		goto lError;
	}
	if(!RMARKER_Init(PUMA_MODULE_RBLOCK,ghStorage))
	{
 		SetReturnCode_puma(RMARKER_GetReturnCode());
		goto lError;
	}
	if(!RBLOCK_Init(PUMA_MODULE_RBLOCK,ghStorage))
	{
		SetReturnCode_puma(RBLOCK_GetReturnCode());
		goto lError;
	}
	if(!RSELSTR_Init(PUMA_MODULE_RBLOCK,ghStorage))
	{
		SetReturnCode_puma(RSELSTR_GetReturnCode());
		goto lError;
	}
    RSTR_SetImportData(RSTR_OcrPath, GetModulePath());
	RSTR_SetImportData(RSTR_pchar_temp_dir, GetModuleTempPath());
	if(!RSTR_Init(PUMA_MODULE_RSTR,ghStorage))
	{
		SetReturnCode_puma(RSTR_GetReturnCode());
		goto lError;
	}

	if(!RFRMT_Init(PUMA_MODULE_RFRMT,ghStorage))
	{
		SetReturnCode_puma(RFRMT_GetReturnCode());
		goto lError;
	}
	if(!RIMAGE_Init(PUMA_MODULE_RIMAGE,ghStorage))
	{
		SetReturnCode_puma(RIMAGE_GetReturnCode());
		goto lError;
	}
	// Инициализируем виртуальные функции
	if(!RPSTR_Init(PUMA_MODULE_RPSTR,ghStorage))
	{
		SetReturnCode_puma(RPSTR_GetReturnCode());
		goto lError;
	}
	if(!RPIC_Init(PUMA_MODULE_RPIC,ghStorage))
	{
		SetReturnCode_puma(RPIC_GetReturnCode());
		goto lError;
	}
	if(!CED_Init(PUMA_MODULE_CED,ghStorage))
	{
		SetReturnCode_puma(CED_GetReturnCode());
		goto lError;
	}
	if(!ROUT_Init(PUMA_MODULE_ROUT,ghStorage))
	{
		SetReturnCode_puma(ROUT_GetReturnCode());
		goto lError;
	}
	else
	{
		if(!ROUT_LoadRec6List("rec6all.dat"))
		{
			SetReturnCode_puma(ROUT_GetReturnCode());
			goto lError;
		}
	}

#ifdef _USE_RVERLINE_
	if(!RVERLINE_Init(PUMA_MODULE_RVERLINE,ghStorage))
	{
		SetReturnCode_puma(RVERLINE_GetReturnCode());
		goto lError;
	}
#endif //_USE_RVERLINE_

#ifdef _USE_RMSEGMENT_
	if(!RMSEGMENT_Init(PUMA_MODULE_RMSEGMENT,ghStorage))
	{
		SetReturnCode_puma(RMSEGMENT_GetReturnCode());
		goto lError;
	}
#endif //_USE_RMSEGMENT_

	if(!RCORRKEGL_Init(PUMA_MODULE_RCORRKEGL,ghStorage))
	{
		goto lError;
	}
//#ifdef _DEBUG

	My_SJTL_Init();
	if( !LDPUMA_Skip(hDebugEnableSaveJtl) )
	{
		My_SJTL_open("one.frm", "one.jtl");
	}
/* FIXME: could not figure out what to do with this in Linux, so disabled it.
	_CrtMemCheckpoint( &g_dbgMemState );
*/
//#endif
return TRUE;
lError:
		ModulesDone(ghStorage);
return FALSE;
}
//////////////////////////////////////////
Bool32 ModulesDone(Handle  ghStorage)
{
	Bool32 rc = TRUE;

	ROUT_Done();
	CED_Done();
//#ifdef _DEBUG
if( !LDPUMA_Skip(hDebugEnableSaveJtl) )
{
    My_SJTL_close();
}
//#endif
	My_SJTL_Done();
	RCORRKEGL_Done();
	RPIC_Done();
	RIMAGE_Done();
	RFRMT_Done();
	RSL_Done();
    REXC_Done();
	RLINE_Done();
	RMARKER_Done();
	RBLOCK_Done();
	RSELSTR_Done();
	RSTR_Done();
	CSTR_Done();
	CCOM_Done();
	CPAGE_Done();
	CIMAGE_Done();
	CLINE_Done();
	RPSTR_Done();
	RSTUFF_Done();
	RRECCOM_Done();

#ifdef _USE_RVERLINE_
	RVERLINE_Done();
#endif //_USE_RVERLINE_

#ifdef _USE_RMSEGMENT_
	RMSEGMENT_Done();
#endif //_USE_RMSEGMENT_

/*
	if(ghStorage)
		CloseStorage(ghStorage,CS_ALL);
		*/
	CFIO_Done();
	return rc;
}
//////////////////////////////////////////
#ifdef _DEBUG
char * GetModulesString(uint32_t dwError)
{
	uint16_t low = (uint16_t)(dwError &  0xFFFF);
	uint16_t hei = (uint16_t)(dwError >> 16);
	static char szString[512] ;
	sprintf(szString,"Unknown code error 0x%X",dwError);

		switch(hei)
		{
		case PUMA_MODULE_RSTR:
			sprintf(szString,"RSTR: %s",RSTR_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_REXC:
			sprintf(szString,"REXC: %s",REXC_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_CCOM:
			sprintf(szString,"CCOM: %s",CCOM_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_CSTR:
			sprintf(szString,"CSTR: %s",CSTR_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_RLINE:
			sprintf(szString,"RLINE: %s",RLINE_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_CPAGE:
			sprintf(szString,"CPAGE: %s",CPAGE_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_RBLOCK:
			sprintf(szString,"RBLOCK: %s",RBLOCK_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_CFIO:
			sprintf(szString,"CFIO: %s",CFIO_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_CIMAGE:
			sprintf(szString,"CIMAGE: %s",CIMAGE_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_RFRMT:
			sprintf(szString,"RFRMT: %s",RFRMT_GetReturnString(dwError));
			return szString;

		case PUMA_MODULE_RSL:
			sprintf(szString,"RSL: %s",RSL_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_RIMAGE:
			sprintf(szString,"RIMAGE: %s",RIMAGE_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_RPSTR:
			sprintf(szString,"RPSTR: %s",RPSTR_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_RPIC:
			sprintf(szString,"RPIC: %s",RPIC_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_CED:
			sprintf(szString,"CED: %s",CED_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_ROUT:
			sprintf(szString,"ROUT: %s",ROUT_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_RSTUFF:
			sprintf(szString,"PUMA_MODULE_RSTUFF: %s",RSTUFF_GetReturnString(dwError));
			return szString;

#ifdef _USE_RVERLINE_
		case PUMA_MODULE_RVERLINE:
			sprintf(szString,"PUMA_MODULE_RVERLINE: %s",RVERLINE_GetReturnString(dwError));
			return szString;
#endif //_USE_RVERLINE_

		case PUMA_MODULE_RRECCOM:
			sprintf(szString,"RRECCOM: %s",RRECCOM_GetReturnString(dwError));
			return szString;
		case PUMA_MODULE_RCORRKEGL:
			sprintf(szString,"RCORRKEGL: %s",RCORRKEGL_GetReturnString(dwError));
			return szString;
#ifdef _USE_RMSEGMENT_
		case PUMA_MODULE_RMSEGMENT:
			sprintf(szString,"RMSEGMENT: %s",RMSEGMENT_GetReturnString(dwError));
			return szString;
#endif
		}
	return szString;
}

#else
//////////////////////////////////////////////////////////////////////
char * GetModulesString(uint32_t dwError)
{
	uint16_t low = (uint16_t)(dwError &  0xFFFF);
	uint16_t hei = (uint16_t)(dwError >> 16);
	static char szString[512];
	uint32_t 	id = IDS_ERR_UNKNOWN;


		switch(hei)
		{
		case PUMA_MODULE_RSTR:
			switch(low)
			{
				case 9: //	"User break", прервано пользователем
				case 1: //  "No error",
					id = IDS_ERR_NO;
					break;
				case 2: //	"Not enough memory", не хватает памяти
					id = IDS_ERR_NO_MEMORY;
					break;
				case 3: //	"Not implement", не поддерживаемая ф-ия
					id = IDS_ERR_NOTIMPLEMENT;
					break;
				case 4: //	"Null poiner assigned", нулевой указатель
				case 5: //	"Bad value of argument", некорректный аргумент
				case 6: //	"Internal error", грубая внутренняя ошибка
				case 7: //	"Tools: No place in the kit", нет места во внутреннем буфере
					break;
				case 8: //	"Can't load recog tables", нет нужной таблицы распознавания или словаря
					id = IDS_ERR_NOTABLE;
					break;
			}
		break;
		case PUMA_MODULE_REXC:
			switch(low)
			{
				case 1: //	"No error",
					id = IDS_ERR_NO;
					break;
                case 18: //	"Not enough memory", не хватает памяти
				case 2: //	"Not enough memory", не хватает памяти
					id = IDS_ERR_NO_MEMORY;
					break;
				case 3: //	"Not inmpement", не поддерживаемая ф-ия
					id = IDS_ERR_NOTIMPLEMENT;
					break;
				case 4: //	"Internal error - call developer", грубая внутренняя ошибка
				case 5: //	"Can't open DumpFile", устарело
				case 6: //	"DumpFile writing error", устарело
				case 7: //	"External CallBack error", устарело
				case 8: //	"Bad Parameters of call", конфликт параметров вызова
				case 12: //	"Can't import", ошибка импорта
				case 13: //	"Can't export", ошибка экспорта
				case 17: //	"Already init", уже проинициализировано
                    break;
/*				case 19: //	"Can't get orientation", нельзя определить ориентацию страницы
                    id = IDS_ERR_CANNOTGETORIENTATION;
                    break;
*/				case 20: //"Bad image information" противоречивая инф. в ВМР
					break;
				case 9: //	"Empty Page", пустая страница
					id = IDS_ERR_EMPTYPAGE;
					break;
				case 10: //"Can't init eventual-library", не инициализируется эвентуальная библиотека (нет таблицы: //
				case 15: //	"Can't init language", ошибка загрузки таблиц эвент
				case 16: //	"Can't init networks", ошиба загрузки нейронной сети
					id = IDS_ERR_NOTABLE;
					break;
				case 11: //"Different versions of REXC and CCOM", расхождение версий
					id = IDS_ERR_DIFVERSION;
					break;
				case 14: //	"Language not exist", не поддерживаемый язык
					id = IDS_ERR_UNKNOWN_LANGUAGE;
					break;
			}
		break;
		case PUMA_MODULE_CCOM:
			switch(low)
			{
				case 1: // "No error",
					id = IDS_ERR_NO;
					break;
				case 2: //	"Not enough memory", не хватает памяти
					id = IDS_ERR_NO_MEMORY;
					break;
				case 3: //	"Not implement", не поддерживаемая ф-ия
					id = IDS_ERR_NOTIMPLEMENT;
					break;
				case 4: //	"Null poiner assigned", нулевой указатель
				case 5: //	"Bad value of argument" некорректный аргумент
					break;
			}
			break;
		case PUMA_MODULE_CSTR:
			switch(low)
			{
				case 1: // "No error",
					id = IDS_ERR_NO;
					break;
				case 2: //	"Not enough memory", не хватает памяти
					id = IDS_ERR_NO_MEMORY;
					break;
				case 3: //	"Not implement", не поддерживаемая ф-ия
					id = IDS_ERR_NOTIMPLEMENT;
					break;
				case 4: //	"Null poiner assigned", нулевой указатель
				case 5: //	"Bad value of argument", некорректный аргумент
				case 6: //	"Assigned Line is absent", нет указанной строки
				case 7: //	"Can't rewrite used Line", существует строка с укзанными параметрами (при создании: //
				case 8: //	"Structure of line has become rotten", строка испорчена (при лечении: //
				case 9: //	"Internal error", грубая внутренняя алгоритмическая ошибка
				case 10: //	"Can't convert to raster" нельзя конвертировать (добавить: // компоненту в растр
					break;
			}
		break;
		case PUMA_MODULE_RLINE:
		break;
		case PUMA_MODULE_RLTABLE:
		break;
		case PUMA_MODULE_CPAGE:
			switch(low)
			{
			case 0://	ошибок нет
					id = IDS_ERR_NO;
					break;
			case 1://   не реализовано
					id = IDS_ERR_NOTIMPLEMENT;
					break;
 			case 2://   нет памяти
					id = IDS_ERR_NO_MEMORY;
					break;
 			case 3://   внутренняя ошибка
 			case 6://   внутренняя ошибка
				break;
 			case 4://   ошибка чтения файла
					id = IDS_ERR_FILEREAD;
					break;
 			case 5://   ошибка записи файла
					id = IDS_ERR_FILEWRITE;
					break;
 			case 7://   несоответсвие версий
					id = IDS_ERR_DIFVERSION;
					break;
			}
		break;
		case PUMA_MODULE_RBLOCK:
			switch (low)
			{
			case 0: //IDS_ERR_NO: ошибок нет
					id = IDS_ERR_NO;
					break;
			case 1:	// IDS_ERR_NOTIMPLEMENT	Функция не реализована.
					id = IDS_ERR_NOTIMPLEMENT;
					break;
			case 2: // IDS_ERR_NO_MEMORY	Не хватает оперативной памяти.
					id = IDS_ERR_NO_MEMORY;
					break;
			case 4: //IDS_ERR_EMPTYPAGE: пустая страница
					id = IDS_ERR_EMPTYPAGE;
					break;
			}
		break;
		case PUMA_MODULE_CFIO:
		break;
		case PUMA_MODULE_CIMAGE:
			switch(low)
			{
		case 0	: // IDS_CIMAGE_ERR_NO	Ошибок нет.
					id = IDS_ERR_NO;
					break;
		case 1	: // IDS_CIMAGE_ERR_NOTIMPLEMENT	Функция не реализована.
					id = IDS_ERR_NOTIMPLEMENT;
					break;
		case 2	: // IDS_CIMAGE_ERR_NO_MEMORY	Не хватает оперативной памяти.
					id = IDS_ERR_NO_MEMORY;
					break;
		case 3	: // IDS_CIMAGE_DLL_NOT_INITIALISING	Модуль не инициализирован
		case 4	: // IDS_CIMAGE_INVALID_IMAGE_NAME	Ошибка в имени картинки
		case 5	: // IDS_CIMAGE_INVALID_IMAGE_INFO	Ошибка в параметрах картинки
		case 6	: // IDS_CIMAGE_INVALID_IMAGE_DATA	Ошибка в растре картинки
		case 7	: // IDS_CIMAGE_INVALID_IMAGES_CONTAINER	Вунтренняя ошибка контейнера
		case 8	: // IDS_CIMAGE_NO_IMAGE_FOUND	Картинка не найlена
		case 9	: // IDS_CIMAGE_INVALID_DIB	Ошибка в DIB или неизвестный формат DIB
		case 10	: // IDS_CIMAGE_DIB_FORMAT_NOT_SUPPORTED	Данный формат DIB не поддерживается этим контейнером
		case 11	: // IDS_CIMAGE_UNABLE_COPY_DIB	Не удается копировать DIB
		case 12	: // IDS_CIMAGE_INVALID_FRAME_SIZE	Неверный размер области изображения
		case 13	: // IDS_CIMAGE_NO_SUPPORT_DIB_FORMAT_FOR_THIS_ACTION	Данную операцию с данным DIB произвести пока невозможно
		case 14	: // IDS_CIMAGE_DIB_AND_FRAME_COLORS_CONFLICT	Несоответствие размеров пикселов
		case 15	: // IDS_CIMAGE_UNABLE_REPLACE_DIB	Не удается заменить часть картинки DIB
		case 16	: // IDS_CIMAGE_UNABLE_USE_IMPORT_CALLBACK	Ошибка во внешних калбэках
		case 17	: // IDS_CIMAGE_CALLBACK_NO_PRESENT	Нет внешних калбэков
		case 18	: // IDS_CIMAGE_INVALID_PARAMETR	Фигня в параметрах
		case 19	: // IDS_CIMAGE_IMAGE_NAME_REPEATED	Картинка с таким именем уже есть
			break;
			}
		break;
		case PUMA_MODULE_RFRMT:
		break;

		case PUMA_MODULE_RSL:
		break;

		case PUMA_MODULE_SLINEDOT:
		break;

		case PUMA_MODULE_RIMAGE:
			switch(low)
			{
			case 0	: // IDS_RIMAGE_ERR_NO	Ошибок пока не обнаружено
					id = IDS_ERR_NO;
					break;
			case 1	: // IDS_RIMAGE_DLL_NOT_INITIALISING	Компонент не инициализирован
			case 2	: // IDS_RIMAGE_ERR_NOTIMPLEMENT	Данный модуль не является источником сей ошибки
				break;
			case 3	: // IDS_RIMAGE_ERR_NO_MEMORY	Не хватает памяти
					id = IDS_ERR_NO_MEMORY;
					break;
			case 32	: // IDS_RIMAGE_NOTSUPPORTED_INDEX_COLOR	Индексированыне цвета не могут быть бинаризованы по Кронроду
                    id = IDS_ERR_UNSUPPORTED_IMAGE_FORMAT;
                    break;
			case 4	: // IDS_RIMAGE_NO_IMAGE_FOUND	Не могу открыть картинку
			case 5	: // IDS_RIMAGE_UNDER_CONSTRUCTION	Данная функция находится в стадии переработки
			case 6	: // IDS_RIMAGE_INVALID_EXTERNAL_DIB	Ошибка в DIB
			case 7	: // IDS_RIMAGE_INVALID_FUNCTION_PARAMETR	Функции передан инвалид
			case 8	: // IDS_RIMAGE_DIB_COMPRESSION_NOT_SUPPORTED	Сжатые DIB пока не поддерживаем
			case 9	: // IDS_RIMAGE_DIB_JPEG_NOT_SUPPORTED	DIB версии 5 со JPEG каритнкой не поддерживается
			case 10	: // IDS_RIMAGE_INTERNAL_MODULE_ERROR	Внутренняя ошибка контейнера
			case 11	: // IDS_RIMAGE_ONLY_ONE_DIB	Один DIB уже обрабатывается!
			case 12	: // IDS_RIMAGE_UNABLE_WRITE_DIB	Немогу записать DIB в CIMAGE
			case 13	: // IDS_RIMAGE_DIB_NOT_ATTACHED	Невозможно получить информацию из заголовка DIB
			case 14	: // IDS_RIMAGE_CANNOT_CREATE_NEW_DIB	Немогу созоать новый DIB
			case 15	: // IDS_RIMAGE_NOT_OPENED	Исходный DIB не был открыт
			case 16	: // IDS_RIMAGE_PREPARE_TO_CREATE	Нечем создать новый DIB
			case 17	: // IDS_RIMAGE_CANNOT_SET_DIB	Немогу передать картинки для обработки
				break;
			case 18	: // IDS_RIMAGE_CANT_BINARYZE	Не могу бинаризовать картинку
						id = IDS_ERR_UNSUPPORTED_IMAGE_FORMAT;
						break;
			case 19	: // IDS_RIMAGE_CANNOT_INVERT_IMAGE	Не могу инвертировать картинку
			case 20	: // IDS_RIMAGE_DIB_CANT_TO_BE_BINARISED	Такие картинки не бинаризуются
			case 21	: // IDS_RIMAGE_DIB_OUT_FORMAT_NOT_BINARISED	Выходной формат DIB для бинаризаии не подходит
			case 22	: // IDS_RIMAGE_OUTCOME_DIB_NOT_LINK_TO_INCOME	Выходной  DIB не соответствует исходному по размерам
			case 23	: // IDS_RIMAGE_CANT_TURN_DIB	Неудается повернуть картинку
			case 24	: // IDS_RIMAGE_CIMAGE_MEMORY_ERROR	Ошибка контейнера CIMAGE при освобождении памяти
			case 25	: // IDS_RIMAGE_CANNOT_ROTATE_IMAGE	Дововорот картинки не завершен
			case 26	: // IDS_RIMAGE_CANNT_SAVE_OUTCOMING_DIB	Полученный результат не удалось сохранитть в контейнере Cimage
			case 27	: // IDS_RIMAGE_ANGLE_LEAST_MINIMUM	Слишком малый  угол для доворота
			case 28	: // IDS_RIMAGE_ANGLE_GREAT_MAXIMUM	Слишком большой угол для доворота
			case 29	: // IDS_RIMAGE_EXIT_BY_USER	Отменено вызывающим модулем
			case 30	: // IDS_RIMAGE_UNKNOWN_ERROR	Непридвиденная ошибка. Вероятно её источник в другом модуле.
			case 31	: // IDS_RIMAGE_ZERO_NUMERATOR_OR_DENUMERATOR	Один из двух - нумератор или денумератор - нулевой
				break;
			}
		break;
		case PUMA_MODULE_RPSTR:
			break;
		case PUMA_MODULE_RPIC:
			switch(low)
			{
			case 0	: // IDS_CIMAGE_ERR_NO	Ошибок нет.
					id = IDS_ERR_NO;
					break;
			case 1	: // IDS_CIMAGE_ERR_NOTIMPLEMENT	Функция не реализована.
					id = IDS_ERR_NOTIMPLEMENT;
					break;
			case 2	: // IDS_CIMAGE_ERR_NO_MEMORY	Не хватает оперативной памяти.
						id = IDS_ERR_NO_MEMORY;
						break;
			}
			break;
		case PUMA_MODULE_RANALFRM:
			/*switch(low)
			{
		case 0	: // IDS_CIMAGE_ERR_NO	Ошибок нет.
				id = IDS_ERR_NO;
				break;
		case 1	: // IDS_CIMAGE_ERR_NOTIMPLEMENT	Функция не реализована.
				id = IDS_ERR_NOTIMPLEMENT;
				break;
		case 2	: // IDS_CIMAGE_ERR_NO_MEMORY	Не хватает оперативной памяти.
					id = IDS_ERR_NO_MEMORY;
					break;
			}*/
			break;
		case PUMA_MODULE_CED:
			break;
		case PUMA_MODULE_ROUT:
			break;
		case PUMA_MODULE_RSTUFF:
			break;
		case PUMA_MODULE_RVERLINE:
			break;
		case PUMA_MODULE_RCORRKEGL:
			break;
		case PUMA_MODULE_RNORM:
			switch (low)
			{
				case 0://	ошибок нет
					id = IDS_ERR_NO;
					break;
				case 3: //	"Not inmpement", не поддерживаемая ф-ия
					id = IDS_ERR_NOTIMPLEMENT;
					break;
				case 19: //	"Can't get orientation", нельзя определить ориентацию страницы
                    id = IDS_ERR_CANNOTGETORIENTATION;
                    break;
			}
			break;

		case PUMA_MODULE_RRECCOM:
			switch (low)
			{
				case 0://	ошибок нет
					id = IDS_ERR_NO;
					break;
				case 3: //	"Not inmpement", не поддерживаемая ф-ия
					id = IDS_ERR_NOTIMPLEMENT;
					break;
			}
			break;

		}
		sprintf(szString," %s ",GetResourceString(id));

	return szString;
}
#endif
