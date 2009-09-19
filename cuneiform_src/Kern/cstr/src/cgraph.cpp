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

#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#include "cstr.h"
#include "ccom.h"
#include "cgraph.h"

///////////////////////////////////////////////////////////////////////////////
#define FILE_VER 1
#define LINE_VER 2
#define IDTEXT_LEN 16

#define CGRAPH_ERR_WRITE  CSTR_ERR_WRITE
#define CGRAPH_ERR_READ   CSTR_ERR_READ
#define CGRAPH_ERR_OPEN   CSTR_ERR_OPEN
#define CGRAPH_ERR_PARAM  CSTR_ERR_VALUE
#define CGRAPH_ERR_MEMORY CSTR_ERR_NOMEMORY
#define CGRAPH_ERR_FILE   CSTR_ERR_OPEN

int32_t	memsize = 256;
uchar IDtext[IDTEXT_LEN] = "CCOM&CSTR file";	//Идентификатор файла

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Возвращает количество компонент в контейнере CSTR
//	ARGS:	объект CSTR_rast
//	RETS:	Количество компонент в контейнере CSTR_rast
/////////////////////////////////////////////////////////////////////////////////////////
int32_t CGRAPH_GetCompCount(CSTR_rast rast)
{
	CSTR_rast curr_rast = rast;
	int count = 0;

	if(!curr_rast)
		return -1;

	while(curr_rast)
	{
		++count;
		curr_rast = curr_rast->next;
	}
	return count;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Получить стоповый растр
//	ARGS:	rast	- текущий растр
//	RETS:	Стоповый растр
/////////////////////////////////////////////////////////////////////////////////////////
CSTR_rast CGRAPH_GetStopRaster(CSTR_rast rast)
{
	CSTR_rast	rst = rast;

	while(rst)
	{
		if(rst->next_up)
			return rst->next_up;
		rst = rst->next;
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Установить указатель массива al->loop на требуемую позицию
//	ARGS:	ptr	- указатель на al->loop
//			n	- позиция
//	RETS:	Указатель на затребованную позицию
/////////////////////////////////////////////////////////////////////////////////////////
intptr_t *SetPtr(intptr_t *ptr, int32_t n)
{
	intptr_t i;
	for(i = 0; i < n; i++)
		ptr++;
	return ptr;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Запомнить адрес текущего растра в массиве al->loop
//	ARGS:	al		- указатель на ALoop
//			rast	- текущий растр
//	RETS:	TRUE	- OK
//			FALSE	- нет памяти
/////////////////////////////////////////////////////////////////////////////////////////
Bool32 AddLoop(ALoop *al, CSTR_rast rast)
{
	CSTR_rast	rst = rast;
	intptr_t *ptr = SetPtr(al->loop, al->n);
	*(ptr) = reinterpret_cast<intptr_t> (rst);
	al->n++;

	if(al->n == memsize)
	{
		memsize *= 2;
		al->loop = static_cast<intptr_t*>(realloc(al->loop, sizeof(intptr_t) * memsize));
		if(!al->loop)
			return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Запомнить текущий уровень вложенности в массиве al->loop
//	ARGS:	al		- указатель на ALoop
//			level	- текущий уровень вложенности
//	RETS:	TRUE	- ОК
//			FALSE	- нет памяти
/////////////////////////////////////////////////////////////////////////////////////////
Bool32 AddLevel(ALoop *al, intptr_t level)
{
	intptr_t *ptr = SetPtr(al->loop, al->n);
	*(ptr) = level;
	al->n++;

	if(al->n == memsize)
	{
		memsize *= 2;
		al->loop = static_cast<intptr_t*> (realloc(al->loop, sizeof(intptr_t) * memsize));
		if(!al->loop)
			return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Получить число альтернативных ветвей для текущей ветви
//	ARGS:	rast	- текущая ветвь
//	RETS:	Число альтернативных ветвей для текущей ветви
/////////////////////////////////////////////////////////////////////////////////////////
int32_t GetCurrLoopCount(CSTR_rast rast)
{
	int32_t count = 0;
	CSTR_rast rst = rast;
	while(rst)
	{
		if(rst->next_down)
			++count;
		rst = rst->next;
	}
	return count;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Инизиализировать структуру LoopData
//	ARGS:	curr_rast	- начало предыдущей строки петли
//			next_rast	- начало текущей петли
//	RETS:	FALSE	- ошибка
//			TRUE	- ОК
/////////////////////////////////////////////////////////////////////////////////////////
Bool32 CGRAPH_GetLoopData(CSTR_rast curr_rast, CSTR_rast next_rast, LoopData *ld)
{
	int32_t count = 0;

	CSTR_rast	curr_rst = curr_rast;
	CSTR_rast	next_rst = next_rast;
	CSTR_rast	start_rst;
	CSTR_rast	stop_rst;

	ld->beg		= 0;
	ld->end		= 0;

	start_rst	= next_rst->prev_up;
	stop_rst	= CGRAPH_GetStopRaster(next_rst);

	if(!start_rst || !stop_rst || !curr_rst || !next_rst)
		return FALSE;

	while(curr_rst)
	{
		if(curr_rst == start_rst)
			ld->beg = count;

		if(curr_rst == stop_rst)
			ld->end = count;

		++count;
		curr_rst = curr_rst->next;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Получить число петлей и инициализировать Aloop
//	ARGS:	al		- указатель на ALoop
//			rast	- растр
//	RETS:	TRUE	- OK
//			FALSE	- ошибка
/////////////////////////////////////////////////////////////////////////////////////////
Bool32 CGRAPH_GetLoopCount(ALoop *al, CSTR_rast rast)
{
	int32_t	curr_level = 1;
	int32_t i;
	intptr_t *ptr;
	CSTR_rast	curr_rst;
	CSTR_rast	next_rst;

	curr_rst = rast;
	next_rst = rast;

	al->n = 0;
	al->loop = static_cast<intptr_t*> (malloc(sizeof(intptr_t) * memsize));

	if(!al->loop)
	{
		wLowRC = CGRAPH_ERR_MEMORY;
		return FALSE;
	}

	//get first part of loops
	while(next_rst)
	{
		if(next_rst->next_down)
		{
			if(!AddLoop(al, curr_rst))
			{
				wLowRC = CGRAPH_ERR_MEMORY;
				return FALSE;
			}

			if(!AddLoop(al, next_rst->next_down))
			{
				wLowRC = CGRAPH_ERR_MEMORY;
				return FALSE;
			}

			if(!AddLevel(al, curr_level))
			{
				wLowRC = CGRAPH_ERR_MEMORY;
				return FALSE;
			}
		}
		next_rst = next_rst->next;
	}

	//get next parts of loops
	for(i = 0; i < al->n; i++)
	{
		ptr = SetPtr(al->loop, ++i);
		next_rst = (CSTR_rast)*(ptr);
		ptr = SetPtr(al->loop, ++i);
		curr_level	= *(ptr);
		curr_rst	= next_rst;

		++curr_level;

		while(next_rst)
		{
			if(next_rst->next_down)
			{
				if(!AddLoop(al, curr_rst))
				{
					wLowRC = CGRAPH_ERR_MEMORY;
					return FALSE;
				}

				if(!AddLoop(al, next_rst->next_down))
				{
					wLowRC = CGRAPH_ERR_MEMORY;
					return FALSE;
				}

				if(!AddLevel(al, curr_level))
				{
					wLowRC = CGRAPH_ERR_MEMORY;
					return FALSE;
				}
			}
			next_rst = next_rst->next;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Сохранение на диск контейнера CSTR
//	ARGS:	rast	- объект CSTR_rast
//			attr	- указатель на структуру CSTR_attr
//			out		- указатель на FILE
//	RETS:	TRUE	- записано успешно
//			FALSE	- ошибка
/////////////////////////////////////////////////////////////////////////////////////////
Bool32 CGRAPH_SaveCSTR(CSTR_rast rast, CSTR_attr *attr, FILE *out)
{
	int32_t count;

	CGRAPH_Data	cstr;
	CSTR_rast	rst		= rast;

	if(!rst || !attr)
	{
		wLowRC = CGRAPH_ERR_PARAM;
		return FALSE;
	}

	count = CGRAPH_GetCompCount(rst);
	if(count == -1)
	{
		wLowRC = CGRAPH_ERR_PARAM;
		return FALSE;
	}

	fwrite(&count, sizeof(count), 1, out);
	fwrite(attr, sizeof(CSTR_attr), 1, out);

	while(rst)
	{
		if(rst->env)
		{
			cstr.env	= TRUE;

			cstr.left	= rst->env->left;
			cstr.upper	= rst->env->upper;
			cstr.w		= rst->env->w;
			cstr.h		= rst->env->h;

			cstr.scale	= rst->env->scale;

			cstr.numcomp		= rst->env->numcomp;
			cstr.size_linerep	= rst->env->size_linerep;

			if(rst->vers)
				cstr.uvers = TRUE;
			else
				cstr.uvers = FALSE;
		}
		else
		{
			cstr.env = FALSE;

			if(rst->vers)
				cstr.uvers = TRUE;
			else
				cstr.uvers = FALSE;
		}

		fwrite(&cstr, sizeof(cstr), 1, out);
		fwrite(&rst->attr, sizeof(CSTR_rast_attr), 1, out);

		if(cstr.uvers)
			fwrite(rst->vers, sizeof(UniVersions), 1, out);

		if(rst->env)
		{
			if(rst->env->size_linerep)
				fwrite(rst->env->linerep, cstr.size_linerep, 1, out);
		}

		rst = rst->next;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Сохранить альтернативные ветви
//	ARGS:	rast	- объект CSTR_rast
//			attr	- указатель на структуру CSTR_attr
//			out		- указатель на FILE
//	RETS:	TRUE	- записано успешно
//			FALSE	- ошибка
/////////////////////////////////////////////////////////////////////////////////////////
Bool32 CGRAPH_SaveLoop(CSTR_rast rast, CSTR_attr *attr, FILE *out)
{
	int32_t i;
	intptr_t *ptr;
	CSTR_rast	curr_rst = rast, next_rst = rast;
	ALoop		al;
	LoopData	ld;

	if(!CGRAPH_GetLoopCount(&al, curr_rst))
	{
		wLowRC = CGRAPH_ERR_PARAM;
		return FALSE;
	}

	i = al.n / 3;
	fwrite(&i, sizeof(i), 1, out);

	for(i = 0; i < al.n; i++)
	{
		ptr = SetPtr(al.loop, i);
		curr_rst = (CSTR_rast)*ptr;

		ptr = SetPtr(al.loop, ++i);
		next_rst = (CSTR_rast)*ptr;

		ptr = SetPtr(al.loop, ++i);
		ld.level = *ptr;

		if(!CGRAPH_GetLoopData(curr_rst, next_rst, &ld))
		{
			wLowRC = CGRAPH_ERR_PARAM;
			return FALSE;
		}

		ld.loop = GetCurrLoopCount(next_rst);
		fwrite(&ld, sizeof(LoopData), 1, out);

		if(!CGRAPH_SaveCSTR(next_rst, attr, out))
			return FALSE;
	}

	free(al.loop);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Запись контейнера CSTR на диск
//	ARGS:	filename	- имя файла
//	RETS:	TRUE	- успешно
//			FALSE	- ошибка
/////////////////////////////////////////////////////////////////////////////////////////
CSTR_FUNC(Bool32) CSTR_SaveCont(char *filename)
{
	int32_t		i, j, count;
	Bool32		lineFlg;
	FILE		*out;

	CSTR_rast	rst;
	CSTR_attr	attr;
	CSTR_line	linx;
	CGRAPH_FileData	fData;

	if(!(out = fopen(filename, "wb")))
	{
		wLowRC = CGRAPH_ERR_OPEN;
		return FALSE;
	}

	fData.FileVer	 = FILE_VER;
	fData.MaxLineVer = LINE_VER;

	count = CSTR_GetMaxNumber();

	fwrite(IDtext, sizeof(IDtext), 1, out);
	fwrite(&fData, sizeof(fData), 1, out);
	fwrite(&count, sizeof(count), 1, out);

	for(i = 1; i <= count; i++)
	{
		for(j = 0; j < fData.MaxLineVer; j++)
		{
			fwrite(&i, sizeof(i), 1, out);			//Номер строки
			fwrite(&j, sizeof(i), 1, out);			//Версия

			if(!(linx = CSTR_GetLineHandle(i, j)))
			{
				lineFlg = FALSE;
				fwrite(&lineFlg, sizeof(lineFlg), 1, out);
				continue;
			}
			else
			{
				lineFlg = TRUE;
				fwrite(&lineFlg, sizeof(lineFlg), 1, out);
			}

			if(!(rst = CSTR_GetFirstRaster(linx)))
			{
				wLowRC = CGRAPH_ERR_PARAM;
				fclose(out);
				return FALSE;
			}

			if(!(CSTR_GetLineAttr(linx, &attr)))
			{
				wLowRC = CGRAPH_ERR_PARAM;
				fclose(out);
				return FALSE;
			}

			if(!CGRAPH_SaveCSTR(rst->next, &attr, out))
			{
				fclose(out);
				return FALSE;
			}

			if(!CGRAPH_SaveLoop(rst->next, &attr, out))
			{
				fclose(out);
				return FALSE;
			}
		}
	}

	fclose(out);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Востановление с диска альтернативных ветвей растров
///	ARGS:	rast	- объект CSTR_rast
//			in		- указатель на FILE
//	RETS:	TRUE	- успешно
//			FALSE	- ошибка
/////////////////////////////////////////////////////////////////////////////////////////
Bool32 CGRAPH_RestoreLoop(CSTR_rast rast, FILE *in)
{
	int32_t	count, lcount, rcount, curr_level = 1;
	int32_t	i, j;
	intptr_t *ptr;
	int32_t	count_rast;
	Bool32	flg = FALSE;

	CSTR_rast		rst = rast, curr_rst = rast;
	CSTR_rast		beg, end;
	CSTR_attr		attr = {0};
	CSTR_rast_attr  rast_attr = {0};

	UniVersions		uvers = {0};
	CGRAPH_Data		cstr = {0};

	uchar		*lp = NULL;
	LoopData	ld;
	ALoop		al;

	al.n = 0;
	al.loop = static_cast<intptr_t *>(malloc(sizeof(intptr_t) * memsize));

	if(!al.loop)
		return FALSE;

	fread(&lcount, sizeof(lcount), 1, in);

	for(i = 0, count = 0, rcount = 0; i < lcount; i++, count = 0)
	{
		fread(&ld, sizeof(ld), 1, in);

		if(curr_level < ld.level)
			flg = TRUE;

		if(flg)
		{
			ptr = SetPtr(al.loop, rcount);
			rst = (CSTR_rast)*ptr;
			++rcount;
		}
		else
			rst = rast;

		//InsertRasterDown
		while(rst)
		{
			if(count == ld.beg)
				beg = rst;

			if(count == ld.end)
				end = rst;

			++count;
			rst = rst->next;
		}

		if(!beg || !end)
			return FALSE;

		if(!(curr_rst = CSTR_InsertRasterDown(beg, end)))
		{
			wLowRC = CGRAPH_ERR_PARAM;
			return FALSE;
		}

		if(ld.loop)
		{
			for(j = 0; j < ld.loop; j++)
			{
				if(!AddLoop(&al, curr_rst))
				{
					wLowRC = CGRAPH_ERR_MEMORY;
					return FALSE;
				}
			}
		}

		//Read Rasters
		fread(&count_rast, sizeof(count_rast), 1, in);
		fread(&attr, sizeof(CSTR_attr), 1, in);

		//InsertRaster
		for(j = 0; j < count_rast; j++)
		{
			fread(&cstr, sizeof(cstr), 1, in);
			fread(&rast_attr, sizeof(CSTR_rast_attr), 1, in);

			if(cstr.env)
			{
				if(cstr.uvers)
					fread(&uvers, sizeof(uvers), 1, in);

				if(cstr.size_linerep)
				{
					lp = (uchar *)malloc(cstr.size_linerep);
					if(!lp)
					{
						wLowRC = CGRAPH_ERR_MEMORY;
						return FALSE;
					}

					fread(lp, cstr.size_linerep, 1, in);
				}

				if(!(rst = CSTR_InsertRaster(curr_rst)))
				{
					wLowRC = CGRAPH_ERR_PARAM;
					return FALSE;
				}

				if(!CSTR_SetAttr(rst, &rast_attr))
				{
					wLowRC = CGRAPH_ERR_PARAM;
					return FALSE;
				}

				if(!CSTR_StoreComp(rst, (uchar*)((uchar*)lp), 1, cstr.scale))
				{
					wLowRC = CGRAPH_ERR_PARAM;
					return FALSE;
				}

				if(cstr.uvers)
				{
					if(!CSTR_StoreCollectionUni(rst, &uvers))
					{
						wLowRC = CGRAPH_ERR_PARAM;
						return FALSE;
					}
				}

				if(lp)
					free(lp);
			}
			else
			{
				if(cstr.uvers)
					fread(&uvers, sizeof(uvers), 1, in);

				if(!(rst = CSTR_InsertRaster(curr_rst)))
				{
					wLowRC = CGRAPH_ERR_PARAM;
					return FALSE;
				}

				if(!CSTR_SetAttr(rst, &rast_attr))
				{
					wLowRC = CGRAPH_ERR_PARAM;
					return FALSE;
				}

				if(cstr.uvers)
				{
					if(!CSTR_StoreCollectionUni(rst, &uvers))
					{
						wLowRC = CGRAPH_ERR_PARAM;
						return FALSE;
					}
				}
			}
		}
	}

	free(al.loop);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Востановление с диска контейнера CSTR
///	ARGS:	lin	- указатель на CSTR_line
//			in	- указатель на FILE
//	RETS:	TRUE	- успешно
//			FALSE	- ошибка
/////////////////////////////////////////////////////////////////////////////////////////
CSTR_rast curr_rast;
Bool32 CGRAPH_RestoreCSTR(CSTR_line *lin, FILE *in)
{
	int32_t i, count_rast;
	Bool32	flg;
	CSTR_rast		rst;
	CSTR_rast_attr  rast_attr = {0};
	CSTR_attr		attr = {0};
	UniVersions		uvers = {0};
	CGRAPH_Data		cstr = {0};
	uchar			*lp = NULL;

	fread(&count_rast, sizeof(count_rast), 1, in);
	fread(&attr, sizeof(CSTR_attr), 1, in);

	if(!(CSTR_SetLineAttr(*lin, &attr)))
	{
		wLowRC = CGRAPH_ERR_PARAM;
		return FALSE;
	}

	for(i = 0, flg = TRUE; i < count_rast; i++, lp = NULL)
	{
		fread(&cstr, sizeof(cstr), 1, in);
		fread(&rast_attr, sizeof(CSTR_rast_attr), 1, in);

		if(cstr.env)
		{
			if(cstr.uvers)
				fread(&uvers, sizeof(uvers), 1, in);

			if(cstr.size_linerep)
			{
				lp = (uchar *)malloc(cstr.size_linerep);
				if(!lp)
				{
					wLowRC = CGRAPH_ERR_MEMORY;
					return FALSE;
				}

				fread(lp, cstr.size_linerep, 1, in);
			}
		}
		else
		{
			if(cstr.uvers)
				fread(&uvers, sizeof(uvers), 1, in);
		}

		if(flg)
		{
			if(!(rst = CSTR_NewRaster(*lin, cstr.left, cstr.upper, cstr.w)))
			{
				wLowRC = CGRAPH_ERR_PARAM;
				return FALSE;
			}
			flg = FALSE;
		}
		else
		{
			if(!(rst = CSTR_InsertRaster(curr_rast)))
			{
				wLowRC = CGRAPH_ERR_PARAM;
				return FALSE;
			}
		}

		if(!(CSTR_SetAttr(rst, &rast_attr)))
		{
			wLowRC = CGRAPH_ERR_PARAM;
			return FALSE;
		}

		if(cstr.env)
		{
			if(!CSTR_StoreComp(rst, (uchar*)((uchar*)lp), 1, cstr.scale))
			{
				wLowRC = CGRAPH_ERR_PARAM;
				return FALSE;
			}
		}

		if(cstr.uvers)
		{
			if(!CSTR_StoreCollectionUni(rst, &uvers))
			{
				wLowRC = CGRAPH_ERR_PARAM;
				return FALSE;
			}
		}

		if(lp)
			free(lp);

		curr_rast = rst;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	DESC:	Востановление контейнера CSTR
//	ARGS:	filename	- имя файла
//	RETS:	TRUE	- успешно
//			FALSE	- ошибка
/////////////////////////////////////////////////////////////////////////////////////////
CSTR_FUNC(Bool32) CSTR_RestoreCont(char *filename)
{
	CSTR_line	linx;
	CSTR_rast	rst;
	FILE		*in;
	uchar		IDt[IDTEXT_LEN];
	int32_t		i, j, count;
	Bool32		lineFlg;
	CGRAPH_FileData	fData;


	if(!(in = fopen(filename, "rb")))
	{
		wLowRC = CGRAPH_ERR_OPEN;
		return FALSE;
	}

	fread(IDt, sizeof(IDt), 1, in);
	fread(&fData, sizeof(fData), 1, in);
	fread(&count, sizeof(count), 1, in);

	if(strcmp((char *)IDt, (char *)IDtext) != 0)
	{
		fclose(in);
		wLowRC = CGRAPH_ERR_FILE;
		return FALSE;
	}

	for(i = 1; i <= count; i++)
	{
		for(j = 0; j < fData.MaxLineVer; j++)
		{
			fread(&i, sizeof(i), 1, in);
			fread(&j, sizeof(j), 1, in);

			if(!(linx = CSTR_NewLine(i, j, -1)))
			{
				fclose(in);
				wLowRC = CGRAPH_ERR_PARAM;
				return FALSE;
			}

			fread(&lineFlg, sizeof(lineFlg), 1, in);

			if(!lineFlg)
				continue;

			if(!(CGRAPH_RestoreCSTR(&linx, in)))
			{
				fclose(in);
				return FALSE;
			}

			if(!(rst = CSTR_GetFirstRaster(linx)))
			{
				fclose(in);
				wLowRC = CGRAPH_ERR_PARAM;
				return FALSE;
			}

			if(!CGRAPH_RestoreLoop(rst->next, in))
			{
				fclose(in);
				return FALSE;
			}

			if(!CSTR_PackLine(linx))
			{
				fclose(in);
				wLowRC = CGRAPH_ERR_PARAM;
				return FALSE;
			}
		}
	}

	return TRUE;
}
