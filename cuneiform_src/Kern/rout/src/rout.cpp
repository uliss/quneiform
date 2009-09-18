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


// Rout.cpp

//********************************************************************
//
// Rout.cpp - экспортируемые функции модуля ROUT
//
// This file creation date: 20.05.99
// By Eugene Pliskin pliskin@cs.isa.ac.ru
//********************************************************************

#include <fcntl.h>
#include <string.h>
#include "stdafx.h"
#include "rout_own.h"

static Bool Static_GetTargetObject(Handle hObject, long reason);
static Bool Static_GetFirstTable(Handle hObject,long reason);
static Bool GetWorkMem();
static Bool FreeWorkMem();
static Bool LoadAlphabet(long language,char *rec6xxxFilename);
static Bool UpdateActiveAlphabet();
static Bool HaveTablesOnPage();

//********************************************************************
Bool32 ROUT_LoadEd
        (
                char *lpEdFile,		// Имя файла или адрес в памяти
                Bool32 readFromFile,	// TRUE, если задано имя файла
                uint32_t bufLen		// Длина (только при readFromFile=FALSE)
                )
{
    // Загрузка ED-файла.
    // Параметры как в CED_ReadFormattedEd.
    //DEBUG_PRINT("ROUT_LoadEd");
    ClearError();

    gPageHandle = CED_ReadFormattedEd(
            lpEdFile,
            readFromFile,
            bufLen);

    if ( !gPageHandle ||
         !CED_GetCountSection(gPageHandle)
         )
    {
        PAGE_NOT_LOADED;
        return FALSE;
    }

    // Страница была загружена мной
    gPageLoadedByMe = TRUE;

    // Установить имя страницы без расширения
    if(readFromFile)
    {
        char dir[_MAX_PATH],
        name[_MAX_PATH],ext[_MAX_EXT];
        split_path(lpEdFile, dir, name, ext);

        strcpy(gPageName, dir);
        strcat(gPageName, name);
    }

    //DEBUG_PRINT("ROUT_LoadEd successful");
    return TRUE;
}
//********************************************************************
Bool32 ROUT_UnloadEd(void)
{
    // Выгрузка ED-файла
    //	DEBUG_PRINT("ROUT_UnloadEd");
    ClearError();

    // Страница была загружена мной?
    if(gPageLoadedByMe)
    {
        CED_DeletePage(gPageHandle);
    }

    gPageHandle = NULL;
    gPageLoadedByMe = FALSE;

    return TRUE;
}
//********************************************************************
// Конвертирование в один формат и запись в файл
Bool32 ROUT_SaveObject(
        uint32_t objIndex,	// Индекс объекта начиная от 1
        char *path,		// Путь до выходного файла
        Bool32 append		// Дополнение в конец файла
        )
{
    // Конвертирование в один формат и запись в файл
    //	DEBUG_PRINT("ROUT_OneFile");
    ClearError();

    // Загружена ли страница
    if ( !gPageHandle )
    {
        PAGE_NOT_LOADED;
        return FALSE;
    }

    // Найти объкт
    gTargetObjectIndex = objIndex;
    gTargetObjectHandle = NULL;
    BrowsePage(Static_GetTargetObject,
               TRUE,	// wantSkipTableCells
               TRUE	// wantSkipParagraphs
               );

    if ( !gTargetObjectHandle )
    {
        OBJECT_NOT_FOUND;
        return FALSE;
    }

    // Запросить память для работы
    if ( !GetWorkMem() )
        return FALSE;

    // Выполнить конвертирование на памяти
    long sizeMem = 0;
    if ( !ROUT_GetObject(
            objIndex,
            0,	// lpMem
            &sizeMem)	// sizeMem
        )
        {
        FreeWorkMem();
        return FALSE;
    }

    // Открыть файл
    ulong pos = 0;

    FILE* f = fopen(path, "wb");
    if (!f)
    {
        ERR_OPEN_FILE;
        FreeWorkMem();
        return FALSE;
    }

    // Дополнение в конец файла
    if (append && (gFormat == ROUT_FMT_Text ||
                   gFormat == ROUT_FMT_SmartText ||
                   gFormat == ROUT_FMT_TableText ||
                   0))
    {
        pos = fseek(f, 0, SEEK_END);
        if ( pos > 0 )
        {
            // Вставить пустую строку
            if (fwrite(gEOL, strlen(gEOL), 1, f) != strlen(gEOL))
            {
                fclose(f);
                FreeWorkMem();
                return FALSE;
            }
        }
    }

    if ( pos == -1 )
    {
        fclose(f);
        FreeWorkMem();
        return FALSE;
    }

    // Записать данные из памяти
    ulong lth = gMemCur - gMemStart;
    if ( fwrite((char*)gMemStart, 1, lth, f) != lth)
    {
        fclose(f);
        FreeWorkMem();
        return FALSE;
    }

    // Закрыть файл
    if (fclose(f))
    {
        ERR_CLOSE_FILE;
        FreeWorkMem();
        return FALSE;
    }

    FreeWorkMem();
    return TRUE;
}
//********************************************************************
// Конвертирование в один формат на заданной памяти
Bool32 ROUT_GetObject
        (
                uint32_t objIndex,	// Индекс объекта начиная от 1
                Byte *lpMem,	// Адрес блока памяти ( 0 - старая память)
                long *sizeMem	// На входе: длина блока памяти
                // На выходе: длина использованной памяти
                )
{
    //	DEBUG_PRINT("ROUT_OneFormat");
    ClearError();

    // Загружена ли страница
    if ( !gPageHandle )
    {
        PAGE_NOT_LOADED;
        return FALSE;
    }

    // Установить рабочую память
    INIT_MEMORY(lpMem,*sizeMem);

    switch(gFormat)
    {
                case ROUT_FMT_Text:
                case ROUT_FMT_SmartText:
        MakeText();
        break;

                case ROUT_FMT_HOCR:
        MakeHOCR();
        break;

                case ROUT_FMT_HTML:
        MakeHTML();
        break;

                case ROUT_FMT_TableText:
        MakeTableText();
        break;

                case ROUT_FMT_DBF:
        MakeTableDBF();
        break;

                case ROUT_FMT_CSV:
                case ROUT_FMT_WKS:

                default:
        NOT_IMPLEMENTED; break;
    }

    // Сколько использовано памяти
    *sizeMem = gMemCur - gMemStart;

    return (gwLowRC_rout?FALSE:TRUE);
}
//********************************************************************
Bool SetLanguage(long language)
{
    // Настройка на язык распознавания.

    // Задан ли язык
    if ( language < 0 || language >= LANG_TOTAL )
    {

        // Язык 255 означает, что язык не задан,
        // это используется, в частности, для пробелов
        if (language == 255)
        {
            return TRUE;
        }
        else
        {
            DEBUG_PRINT ("Неправильный язык распознавания: %d",language);
            return FALSE;
        }
    }

    ASSERT(LANG_UZBEK==16);

    gLanguage = language;

    langUzbek = ( gLanguage == LANG_UZBEK );
    langKaz = ( gLanguage == LANG_KAZ ||
                gLanguage == LANG_KAZ_ENG
                );

    if ( !UpdateActiveAlphabet() )
        return FALSE;

    if ( !UpdateActiveCodeTable() )
        return FALSE;

    return TRUE;
}
//********************************************************************
Bool SetFormat(long format)
{
    // Настройка на формат

    if ( format == ROUT_FMT_Text ||
         format == ROUT_FMT_SmartText ||
         format == ROUT_FMT_TableText ||
         format == ROUT_FMT_CSV ||
         format == ROUT_FMT_DBF ||
         format == ROUT_FMT_WKS ||
         format == ROUT_FMT_HTML ||
         format == ROUT_FMT_HOCR ||
         0)
    {
        gFormat = format;
        return TRUE;
    }

    WRONG_ARGUMENT;
    return FALSE;
}
//********************************************************************
Bool SetActiveCode(long code)
{
    // Настройка на выходную кодировку

    if ( code < 1 || code > ROUT_CODE_MAX )
    {
        WRONG_ARGUMENT;
        return FALSE;
    }

    gActiveCode = code;

    UpdateActiveCodeTable();
    return TRUE;
}
//********************************************************************
long ROUT_ListFormats(uchar * buf, ulong sizeBuf)
{
    // Получение списка поддерживаемых форматов
    // Возвращает количество форматов или -1 при ошибке
    long count = 0;
    ROUT_ITEM *p = (ROUT_ITEM*)buf;

    ClearError();

    memset(buf, 0, sizeBuf);
    if (sizeBuf < ROUT_FMT_COUNT * sizeof(ROUT_ITEM))
    {
        NO_MEMORY;
        return -1;
    }

#define ITEM(a) {\
    p->code = ROUT_FMT_##a;\
              LoadString((HINSTANCE)ghInst_rout, IDS_FMT_##a,\
                         p->name,sizeof(p->name));\
              p++; count++;\
          }

ITEM (Text);
//ITEM (SmartText);
ITEM (TableText);
//ITEM (CSV);
ITEM (DBF);
//ITEM (WKS);
ITEM (HTML);

#undef ITEM

return count;
}
//********************************************************************
long ROUT_ListCodes(uchar * buf, ulong sizeBuf)
{
    // Получение списка кодировок для данного формата
    // Возвращает количество кодировок или -1 при ошибке
    long count = 0;
    ROUT_ITEM *p = (ROUT_ITEM*)buf;

    ClearError();

    memset(buf, 0, sizeBuf);
    if (sizeBuf < ROUT_CODE_COUNT * sizeof(ROUT_ITEM))
    {
        NO_MEMORY;
        return -1;
    }

#define ITEM(a) {\
    p->code = ROUT_CODE_##a;\
              LoadString((HINSTANCE)ghInst_rout, IDS_CODE_##a,\
                         p->name,sizeof(p->name));\
              p++; count++;\
          }

switch(gFormat)
{
                case ROUT_FMT_Text:
                case ROUT_FMT_SmartText:
                case ROUT_FMT_TableText:
    ITEM(ASCII);
    ITEM(ANSI);
    ITEM(KOI8R);
    ITEM(ISO);
    break;

                case ROUT_FMT_CSV:
                case ROUT_FMT_WKS:
    ITEM(ANSI);
    break;

                case ROUT_FMT_DBF:
    ITEM(ASCII);
    ITEM(ANSI);
    break;

                case ROUT_FMT_HTML:
                case ROUT_FMT_HOCR:
    ITEM(ANSI);
    ITEM(KOI8R);
    ITEM(ISO);
    break;

                default:
    return -1;
}

#undef ITEM

return count;
}
//********************************************************************
Byte ROUT_Byte(Byte c)
{
    // Перекодировать один байт по кодовой таблице
    ClearError();

    if (!gActiveCodeTable)
    {
        NOT_IMPLEMENTED;
        return c;
    }

    return gActiveCodeTable[c];
}
//********************************************************************
Bool32 ROUT_Block(
        Byte *lpMem,	// Адрес блока памяти
        long sizeMem	// Длина блока памяти
        )
{
    // Перекодировать блок памяти по кодовой таблице
    ClearError();

    if (!gActiveCodeTable)
    {
        NOT_IMPLEMENTED;
        return FALSE;
    }

    while ( sizeMem-- > 0 )
    {
        *lpMem = gActiveCodeTable[*lpMem];
        lpMem++;
    }

    return TRUE;
}
//********************************************************************
long ROUT_CountObjects()
{
    // Сосчитать количество объектов на странице.
    // Предварительно рекомендуется загрузить страницу (ROUT_LoadEd)
    // и установить формат выдачи (ROUT_LONG_Format).
    //
    // Для табличных форматов выдает количество таблиц на странице,
    // а для остальных форматов выдает всегда 1.
    //
    long count = 0;
    ClearError();

    switch ( gFormat )
    {
                case ROUT_FMT_Text:
                case ROUT_FMT_SmartText:
                case ROUT_FMT_HTML:
                case ROUT_FMT_HOCR:
        return 1;
        break;

                case ROUT_FMT_TableText:
                case ROUT_FMT_CSV:
                case ROUT_FMT_WKS:
                case ROUT_FMT_DBF:
        // Просмотреть страницу и вернуть
        // количество таблиц
        gTargetObjectIndex = LONG_MAX;
        gTargetObjectHandle = NULL;
        BrowsePage(NULL,
                   TRUE,	// wantSkipTableCells
                   TRUE	// wantSkipParagraphs
                   );

        return gHitTables;

                default:
        {
            DEBUG_PRINT("Wrong gFormat");
        }
    }

    return 0;
}
//********************************************************************
Bool Static_GetTargetObject(
        Handle hObject,
        long reason	// См. enum BROWSE_REASON
        )
{
    // Поиск объекта на странице.
    // gTargetObjectIndex - искомый номер объекта
    // Возвращает FALSE для того чтобы прекратить поиск.

    static long countObjectsFound; // Static не удалять...

    if(reason == BROWSE_PAGE_START)
        countObjectsFound = 0;

    switch ( gFormat )
    {
                case ROUT_FMT_Text:
                case ROUT_FMT_SmartText:
                case ROUT_FMT_HTML:
                case ROUT_FMT_HOCR:
        gTargetObjectHandle = gPageHandle;
        return FALSE;	// Закончить поиск
        break;

                case ROUT_FMT_TableText:
                case ROUT_FMT_CSV:
                case ROUT_FMT_WKS:
                case ROUT_FMT_DBF:
        if (reason == BROWSE_TABLE_START)
        {
            gTargetObjectHandle = hObject;

            // Сосчитать найденный объект
            countObjectsFound++;
            if ( countObjectsFound >= gTargetObjectIndex )
                return FALSE;	// Закончить поиск
        }
        break;

                default:
        {
            DEBUG_PRINT("Wrong gFormat");
        }
    }

    return TRUE;	// Продолжить поиск
}
//********************************************************************
Bool32 ROUT_SetAlphabet(
        uint32_t sizeAlphabet,// Количество букв
        char *upper,		// Прописные буквы
        char *lower,		// Строчные буквы
        char *vowels		// Гласные букы
        )
{
    // Импорт алфавита, загруженного из REC6.DAT в SPELABC.C.
    ClearError();

    gSizeAlphabet = 0;
    memset( &gUpper[0], 0, sizeof(gUpper));
    memset( &gLower[0], 0, sizeof(gLower));
    memset( &gVowels[0], 0, sizeof(gVowels));
    memset( &gAlphabetTable[0], 0, sizeof(gAlphabetTable));

    if (strlen(upper) != sizeAlphabet ||
        strlen(lower) != sizeAlphabet ||
        strlen(vowels) > sizeAlphabet ||
        sizeAlphabet > sizeof(gUpper) - 1
        )
    {
        WRONG_ARGUMENT;
        return FALSE;
    }

    gSizeAlphabet = sizeAlphabet;
    memcpy( &gUpper[0], upper, sizeAlphabet );
    memcpy( &gLower[0], lower, sizeAlphabet );
    memcpy( &gVowels[0], vowels, sizeAlphabet );

    // Добавить английские в список,
    // но количество букв не менять
    ASSERT(sizeAlphabet + 26 < 256);
    strcat((char*)gUpper, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    strcat((char*)gLower, "abcdefghijklmnopqrstuvwxyz");
    strcat((char*)gVowels,"^bcd^fgh^^klmn^pqrst^v^x^z");

    // Заполнить позиционную таблицу
    Byte *p = NULL;

    for ( p = gUpper; * p; p++)
        gAlphabetTable[*p] |= CASE_UPPER;

    for ( p = gLower; * p; p++)
        gAlphabetTable[*p] |= CASE_LOWER;

    for ( p = gVowels; * p; p++)
        gAlphabetTable[*p] |= CASE_VOWEL;

    for ( p = (Byte*)"0123456789"; * p; p++)
        gAlphabetTable[*p] |= CASE_DIGIT;

    return TRUE;
}
//********************************************************************
char *ROUT_GetDefaultObjectName(
        uint32_t objIndex	// Индекс объекта начиная от 1
        )
{
    /*
        Сформировать имя выходного файла из имени страницы
        для текущей комбинации формат-кодировка и для
        заданного индекса объекта.
*/
    char suffix[40] = "", ext[5] = "";
    static char name[_MAX_PATH] = "";

    switch(gFormat)
    {
                case ROUT_FMT_Text:
        //strcpy(suffix,"");
        strcpy(ext,".txt");
        break;
                case ROUT_FMT_SmartText:
        strcpy(suffix,"_s");
        strcpy(ext,".txt");
        break;

                case ROUT_FMT_TableText:
        strcpy(suffix,"_b");
        strcpy(ext,".txt");
        break;

                case ROUT_FMT_CSV:
        //strcpy(suffix,"");
        strcpy(ext,".csv");
        break;

                case ROUT_FMT_DBF:
        //strcpy(suffix,"");
        strcpy(ext,".dbf");
        break;

                case ROUT_FMT_WKS:
        //strcpy(suffix,"");
        strcpy(ext,".wks");
        break;

                case ROUT_FMT_HTML:
                case ROUT_FMT_HOCR:
        //strcpy(suffix,"");
        strcpy(ext,".htm");
        break;

                default:
        {
            NOT_IMPLEMENTED;
            return NULL;
        }
    }

    switch(gActiveCode)
    {
                case ROUT_CODE_ASCII:
        if ( gFormat != ROUT_FMT_Text &&
             gFormat != ROUT_FMT_SmartText &&
             gFormat != ROUT_FMT_TableText &&
             gFormat != ROUT_FMT_DBF
             )
        {
            NOT_IMPLEMENTED;
            return NULL;
        }

        strcat(suffix,"_a");
        break;

                case ROUT_CODE_ANSI:
        strcat(suffix,"_w");	// All formats
        break;

                case ROUT_CODE_KOI8R:
        if ( gFormat != ROUT_FMT_Text &&
             gFormat != ROUT_FMT_SmartText &&
             gFormat != ROUT_FMT_TableText &&
             gFormat != ROUT_FMT_DBF &&
             gFormat != ROUT_FMT_HTML &&
             gFormat != ROUT_FMT_HOCR
             )
        {
            NOT_IMPLEMENTED;
            return NULL;
        }

        strcat(suffix,"_k");
        break;

                case ROUT_CODE_ISO:
        if ( gFormat != ROUT_FMT_Text &&
             gFormat != ROUT_FMT_SmartText &&
             gFormat != ROUT_FMT_TableText &&
             gFormat != ROUT_FMT_DBF &&
             gFormat != ROUT_FMT_HTML &&
             gFormat != ROUT_FMT_HOCR
             )
        {
            NOT_IMPLEMENTED;
            return NULL;
        }

        strcat(suffix,"_i");
        break;

                case ROUT_CODE_UTF8:
        if ( gFormat != ROUT_FMT_Text &&
             gFormat != ROUT_FMT_SmartText &&
             gFormat != ROUT_FMT_TableText &&
             gFormat != ROUT_FMT_HTML
             )
        {
            NOT_IMPLEMENTED;
            return NULL;
        }

        strcat(suffix,"_u");
        break;

                default:
        {
            NOT_IMPLEMENTED;
            return NULL;
        }
    }

    strcpy(name,gPageName);	// Имя страницы без расширения
    strcat(name,suffix);

    // Добавить индекс объекта начиная от 2
    if (objIndex > 1 )
        sprintf(name+strlen(name),"%d",objIndex);

    strcat(name,ext);

    return name;
}
//*****************************************************************
static Bool GetWorkMem()
{
    // Запросить свободный блок общей памяти
    // А если не дадут, то использовать
    // свой кусок памяти, выделенный
    // на ROUT_Init()
    //
    Byte *p = NULL;
    long lth = 1024<<10;	// 1M

    p = (Byte*)MyAlloc(lth, 0);
    if (!p )
    {
        // Использовать собственный кусок памяти
        p = (Byte*)gOwnMemory;
        lth = gOwnMemorySize;

        DEBUG_PRINT("ROUT.CPP MyGetFreeMem: MyAlloc failed, using own memory");
    }

    // Установить рабочую память
    INIT_MEMORY(p,lth);

    return TRUE;

}
//********************************************************************
static Bool FreeWorkMem()
{
    // Освободить рабочую память
    Bool good = TRUE;

    // Собственную память не сдавать
    if (gMemStart &&
        gMemStart != gOwnMemory
        )
        good =  MyFree(gMemStart);

    // Забыть адрес памяти
    gMemStart = NULL;
    gMemEnd = NULL;
    gMemCur = NULL;

    return good;

}
//********************************************************************
uint32_t ROUT_GetObjectSize(
        uint32_t objIndex	// Индекс объекта начиная от 1
        )
{
    // Гадкая функция для определения длины объекта
    Byte *p = NULL;
    long lth = 256<<10;		// 256K
    long sizeMem = 0;

    ClearError();

    for (long attempt = 1; attempt < 3; attempt++)
    {
        p = (Byte*)MyAlloc(lth,0);
        if (!p)
        {
            NO_MEMORY;
            return 0;
        }

        InitMemory(p,lth);

        // Выполнить конвертирование на памяти
        sizeMem = lth;
        if ( ROUT_GetObject(
                objIndex,
                0,	// lpMem
                &sizeMem)	// sizeMem
            )
            {
            FreeWorkMem();
            return sizeMem + 2*gBumperSize;
        }

        // Повторить попытку с большей памятью
        FreeWorkMem();
        lth = lth <<1 ;
    }

    return 0;	// Неудача
}
//********************************************************************
Bool32 ROUT_LoadRec6List(
        const char *rec6AllFilename
        )
{
    /* Загрузка файла rec6all.dat, в котором
        перечислены имена таблиц rec6xxx.dat
        в виде:
                0 rec6.dat
                1 rec6grm.dat
                ... и т.д.

        Файлы rec6xxx.dat загружяются в таблицу алфавитов:

        gAT_sizeAlphabet[LANG_TOTAL]
        gAT_upper[LANG_TOTAL][kMaxSizeAlphabet]
        gAT_lower[LANG_TOTAL][kMaxSizeAlphabet]
        gAT_vowels[LANG_TOTAL][kMaxSizeAlphabet]

*/

    ClearError();

    // Открыть файл со списком таблиц
    FILE *f = NULL;
    char buf[256] = "";
    int fd;

    fd = open_data_file(rec6AllFilename, O_RDONLY); // Was "rt".
    if(fd == -1) {
        ERR_LOAD_REC6LIST;
        return FALSE;
    }
    f = fdopen(fd, "rt");
    if (!f)
    {
        ERR_LOAD_REC6LIST;
        return FALSE;
    }

    while (	fgets(buf,sizeof(buf)-1,f) )
    {
        int language=-1;
        char theName[_MAX_PATH] = "";

        // Пустые строки и строки комментариев,
        // начинающиеся с точки с запятой, пропускать
        char *p = buf;

        while (*p == ' ' || *p == '\t' )
            p++;

        if ( *p == ';' || !*p || *p == 0x0d || *p == 0x0a )
            continue;

        // Номер языка и имя таблицы rec6xxx.dat
        sscanf(buf, "%d%s", &language, &theName[0]);

        if (language <0 || language >= LANG_TOTAL ||
            !theName[0] ||
            !LoadAlphabet(language, theName)
            )
        {
            fclose(f);
            ERR_LOAD_REC6LIST;
            return FALSE;
        }
    }

    fclose(f);
    return TRUE;
}
//********************************************************************
static Bool LoadAlphabet(
        long language,
        char *rec6xxxFilename)
{
    // Загрузка файла REC6.DAT
    FILE *f = NULL;
    int fd;
    char buf[256] = "";
#if defined(WIN32) || defined(__CYGWIN__)
    const char line_end = '\n';
#else
    const char line_end = '\r';
#endif

    ASSERT (language >= 0 && language < LANG_TOTAL);

    long sizeAlphabet = 0;

    fd = open_data_file(rec6xxxFilename, O_RDONLY);
    if(fd == -1)
        return FALSE;
    f = fdopen(fd, "rt");
    if (!f)
        return FALSE;

    fgets(buf,sizeof(buf)-1,f);

    sscanf(buf,"%d",&sizeAlphabet);

    if( sizeAlphabet < 1 ||
        sizeAlphabet > kMaxSizeAlphabet
        )
    {
        fclose(f);
        ERR_LOAD_ALPHABET;
        return FALSE;
    }

    gAT_sizeAlphabet[language] = sizeAlphabet;

    char *upper = gAT_upper[language];
    char *lower = gAT_lower[language];
    char *vowels = gAT_vowels[language];

    fgets(upper,kMaxSizeAlphabet,f);
    fgets(lower,kMaxSizeAlphabet,f);
    fgets(vowels,kMaxSizeAlphabet,f);

    fclose(f);
    f = NULL;

    char *p1 = strchr(upper, line_end);
    char *p2 = strchr(lower, line_end);
    char *p3 = strchr(vowels, line_end);

    if ( p1 ) *p1 = 0;
    if ( p2 ) *p2 = 0;
    if ( p3 ) *p3 = 0;

    if ( (long)strlen(upper) != sizeAlphabet ||
         (long)strlen(lower) != sizeAlphabet ||
         (long)strlen(vowels)> sizeAlphabet
         )
    {
        ERR_LOAD_ALPHABET;
        return FALSE;
    }

    return TRUE;
}
//*****************************************************************
static Bool UpdateActiveAlphabet()
{
    /*  Смена алфавита.
        Алфавит выбирается по номеру языка gLanguage
    из таблицы алфавитов

        Данная функция вызывается из SetLanguage().

*/

    if (!gAT_sizeAlphabet[gLanguage] ||
        !gAT_upper[gLanguage] ||
        !gAT_lower[gLanguage] ||
        !gAT_vowels[gLanguage] ||
        !ROUT_SetAlphabet(
                gAT_sizeAlphabet[gLanguage],
                gAT_upper[gLanguage],
                gAT_lower[gLanguage],
                gAT_vowels[gLanguage]
                )
        )
    {
        ERR_UPDATE_ACTIVE_ALPHABET;
        return FALSE;
    }

    return TRUE;
}
//********************************************************************
long ROUT_ListAvailableFormats(uchar * buf, ulong sizeBuf)
{
    /*
        Получение списка возможных форматов сохранения
        для текущей загруженной страницы.
        Возвращает количество форматов или -1 при ошибке

        Табличные форматы включаются при наличии таблиц на странице.
        Если страница не загружена, то
        устанавливается ошибка PAGE_NOT_LOADED
*/

    long count = 0;
    ROUT_ITEM *p = (ROUT_ITEM*)buf;

    ClearError();

    memset(buf, 0, sizeBuf);
    if (sizeBuf < ROUT_FMT_COUNT * sizeof(ROUT_ITEM))
    {
        NO_MEMORY;
        return -1;
    }

    if ( !gPageHandle )
    {
        PAGE_NOT_LOADED;
        return -1;
    }

    // Наличие таблиц на странице
    Bool haveTables = HaveTablesOnPage();

#define ITEM(a) {\
    p->code = ROUT_FMT_##a;\
              LoadString((HINSTANCE)ghInst_rout, IDS_FMT_##a,\
                         p->name,sizeof(p->name));\
              p++; count++;\
          }

ITEM (Text);
//ITEM (SmartText);
if (haveTables )
{
    ITEM (TableText);
    //ITEM (CSV);
    ITEM (DBF);
    //ITEM (WKS);
}
ITEM (HTML);

#undef ITEM

return count;
}
//********************************************************************
Bool Static_GetFirstTable(
        Handle hObject,
        long reason	// См. enum BROWSE_REASON
        )
{
    // Поиск первой таблицы на странице.

    if (reason == BROWSE_TABLE_START)
        return FALSE;	// Закончить поиск

    return TRUE;	// Продолжить поиск
}
//********************************************************************
static Bool HaveTablesOnPage()
{
    // Наличие таблиц на странице
    if ( !gPageHandle )
        return FALSE;

    // Просмотреть страницу до первой таблицы
    BrowsePage(Static_GetFirstTable,
               TRUE,	// wantSkipTableCells
               TRUE	// wantSkipParagraphs
               );

    return (gHitTables > 0 );

}
//********************************************************************
