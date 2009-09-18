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

/*#include<windows.h>*/
#include "rmarker.h"
#include "rmarkermem.h"
#include "rmfunc.h"
#include "rneg.h"
#include "pumadef.h"
#include "dpuma.h"

Handle hLayout;
Handle MainDebug;
Handle DebugFill;
//Handle hUseCLine;
Handle hVertCells;
Handle hNegaCells;
Handle hDebugUpDown;
Handle hDebugSpell;
Handle hDebugPictures;
Handle hPrintFileVertCells;
Handle hVertCellsAuto;
Handle hNegaCellsAuto;
Handle hDebugNeg;
Handle hDebugLinePass3;
Handle hDebugLinePass2;
Handle hDebugVerifLine;
Handle hZone;
Handle hNoRazrTabl;
Handle hNoGiveNeg;
Handle hNoSeePict;

Handle hMainTime;
Handle hPrep;
Handle hPicture;
Handle hNegative;
Handle hTableZones;
Handle hPrintCrossedPics;
Handle hVCutInZones;
Handle hSearchTables;
Handle hAnalTables;
Handle hSVLP;
Handle hBlocks;
Handle hLines3;
Handle hNegaTestCells;
Handle hVertTestCells;
Handle hFon;
Handle hEnd;
Handle hSegment;
//extern uchar* Buffer;
//extern uchar* WorkMem;


RMARKER_FUNC(Bool32) RMARKER_PageMarkup(PRMPreProcessImage Image,void* vBuff,int Size,void* vWork,int SizeWork)
{
	SetMainBuff(vBuff,Size);
	SetWorkBuff(vWork,SizeWork);
	Bool32 rc=PageMarkup(Image);
	ReSetMem();
	return rc;
}

RMARKER_FUNC(Bool32) RMARKER_SearchTableInZone(Handle hPage,Handle hCCOM,uint32_t perc,Rect32 rect)
{
	return TRUE;
}

RMARKER_FUNC(Bool32) RMARKER_SetImportData(uint32_t dwType, void * pData)
{
	Bool rc = FALSE;

	rc = SetCBProgressPoints( pData );
	return rc;
}

RMARKER_FUNC(Bool32) RMARKER_Init(uint16_t wHeightCode,Handle hStorage)
{

 LDPUMA_Init  (0,NULL);

// Buffer=NULL;
// Buffer=(uchar*)RMARKERAlloc(BufferSize*sizeof(uchar));
// if(!Buffer)
//	return FALSE;
// WorkMem=(uchar*)RMARKERAlloc(WorkMemSize*sizeof(uchar));
// if(!WorkMem)
//	return FALSE;

/*------------------------------------------------------------------------------------------*/

	RNEG_Init(wHeightCode,NULL);


/*------------------------------------------------------------------------------------------*/
#ifdef _USE_RFILL_
	if (!RFILL_Init())
	{
		uint32_t  retCode = RFILL_GetReturnCode   ();
		char   *pszMsg  = RFILL_GetReturnString (retCode);
		return  FALSE;
	}
#endif //_USE_RFILL_
/*------------------------------------------------------------------------------------------*/


 LDPUMA_Registry(&hMainTime,SNAP_ROOT_MAIN_TIME_CONTROL,NULL);
	LDPUMA_Registry(&hPrep,"Подготовка.Верт. лин. проц.",hMainTime);
	LDPUMA_Registry(&hPicture,"Выделение трив. картинок.",hMainTime);
	LDPUMA_Registry(&hNegative,"Выделение негативов..",hMainTime);
	LDPUMA_Registry(&hTableZones,"Выделение зон таблиц..",hMainTime);
	LDPUMA_Registry(&hLines3,"Линии.Третий проход..",hMainTime);
	LDPUMA_Registry(&hVCutInZones,"Верт. резка в ЗТ.",hMainTime);
	LDPUMA_Registry(&hSearchTables,"Поиск таблиц..",hMainTime);
	LDPUMA_Registry(&hAnalTables,"Анализ структуры таблиц..",hMainTime);
	LDPUMA_Registry(&hSVLP,"Верт. лин. проц.",hMainTime);
	LDPUMA_Registry(&hBlocks,"Выделение фрагментов..",hMainTime);
	LDPUMA_Registry(&hNegaTestCells,"Нег. диаг. ячеек таблиц..",hMainTime);
	LDPUMA_Registry(&hVertTestCells,"Верт. диаг. ячеек таблиц..",hMainTime);
	LDPUMA_Registry(&hFon,"Поис фона..",hMainTime);
	LDPUMA_Registry(&hEnd,"От RMARKER до выделения строк..",hMainTime);
	LDPUMA_Registry(&hSegment,"Грубое выделение фрагментов...",hMainTime);

 LDPUMA_Registry(&MainDebug,SNAP_ROOT_MAIN_DEBUG, NULL );


// LDPUMA_Registry (&hUseCLine,"Работа с контейнером линий",NULL);

 LDPUMA_Registry(&DebugFill,"Поиск фона в таблицах",MainDebug);
 LDPUMA_RegistryHelp(DebugFill,"Активизация вершины разрешит поиск фона в таблицах.",FALSE);

 LDPUMA_Registry(&hDebugNeg,"Этап поиска негативов.",NULL);

 LDPUMA_Registry(&hDebugLinePass3,"Этап оценки линий.",NULL);
 LDPUMA_Registry(&hDebugLinePass2,"Этап третьего прохода по линиям.",NULL);
 LDPUMA_Registry(&hDebugVerifLine,"Верификация линий.",NULL);

 LDPUMA_Registry(&hNegaCellsAuto,"Диагностика негативных ячеек таблиц",MainDebug);
 LDPUMA_RegistryHelp(hNegaCellsAuto,"Активизация вершины разрешит диагностику негативных ячеек таблиц.",FALSE);

 LDPUMA_Registry(&hVertCellsAuto,"Этап автоматической вертикальной диагностики ячеек таблиц.",NULL);

 LDPUMA_Registry(&hVertCells,"Работа с вертикальными ячейками таблиц",NULL);

 LDPUMA_Registry(&hPrintFileVertCells,"Печать в файл вертикальных ячеек",hVertCells);
 LDPUMA_RegistryHelp(hPrintFileVertCells,"Разрешить печать в файл vertcell.res вертикальных ячеек таблиц.",FALSE);

 LDPUMA_Registry(&hNegaCells,"Работа с негативными ячейками таблиц",NULL);

 LDPUMA_Registry(&hDebugUpDown,"Сверху вниз",hVertCells);
 LDPUMA_RegistryHelp(hDebugUpDown,"При продавлении вершины вертикальные строки будут считаться типа Сверху-Вниз.",FALSE);

 LDPUMA_Registry(&hDebugPictures,SNAP_LAYOUT_PICTURES,NULL);

 LDPUMA_Registry(&hLayout,SNAP_ROOT_LAYOUT,NULL);

 LDPUMA_RegistryHelp(hZone,"Вершина поиска зон таблиц.",FALSE);

 LDPUMA_Registry(&hPrintCrossedPics, "Отпись пересекаемых таблицами картинок", hLayout);
 LDPUMA_RegistryHelp(hPrintCrossedPics, "Печать в файл crossed.txt координат картинок, пересекаеющи зоны таблиц", FALSE);

 LDPUMA_Registry(&hNoRazrTabl, "Отменить разрезание склеенных таблиц.",hZone);
 LDPUMA_RegistryHelp(hNoRazrTabl, "Отменить разрезание склеенных таблиц.", FALSE);

 LDPUMA_Registry(&hNoGiveNeg, "Не сдавать негативы.",hZone);
 LDPUMA_RegistryHelp(hNoGiveNeg, "Не сдавать негативы.", FALSE);

 LDPUMA_Registry(&hNoSeePict,"Не учитывать картинки.",hZone);
 LDPUMA_RegistryHelp(hNoSeePict,"Не учитывать картинки.",FALSE);


 remove("crossed.txt");

	return TRUE;
}

RMARKER_FUNC(Bool32) RMARKER_Done()
{
//	if ( Buffer )
//		RMARKERFree(Buffer);
//	if ( WorkMem )
//		RMARKERFree(WorkMem);

/*------------------------------------------------------------------------------------------*/

	RNEG_Done();

/*------------------------------------------------------------------------------------------*/
#ifdef _USE_RFILL_
    if (!RFILL_Done())
	{
		uint32_t  retCode = RFILL_GetReturnCode   ();
		char   *pszMsg  = RFILL_GetReturnString (retCode);
		return  FALSE;
	}
#endif //_USE_RFILL_
/*------------------------------------------------------------------------------------------*/

	LDPUMA_Done();
	return TRUE;
}

RMARKER_FUNC(char *) RMARKER_GetReturnString(uint32_t dwError)
{return NULL;
}

RMARKER_FUNC(Bool32) RMARKER_GetExportData(uint32_t dwType, void * pData)
{return FALSE;
}

uint32_t RMARKER_GetReturnCode(void)
{
	return GetReturnCode_rmarker();
}
