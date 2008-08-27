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


// HTML.cpp

//********************************************************************
//
// HTML.cpp - формат HTML
//
// This file creation date: 27.05.99
// By Eugene Pliskin pliskin@cs.isa.ac.ru
//********************************************************************

#include <string.h>
#include "stdafx.h"
#include "rout_own.h"
#include "compat_defs.h"

static BOOL Static_MakeHTML(Handle hObject, long reason);

static BOOL FontStyle(ULONG newStyle);
static BOOL BeginParagraph(ULONG alignment);
static BOOL CellStart();
static BOOL CalcCellSpan();
static BOOL OptimizeTags();
static BOOL Picture();
static BOOL CreatePageFilesFolder();

static ULONG sFontStyle = 0;		// Стиль шрифта
static long rowspan = 0, colspan = 0;

//********************************************************************
BOOL MakeHTML()
{
/* Формат HTML.

   Включаются таблицы.
   Концы строк сохраняются, если gPreserveLineBreaks = TRUE.
*/
	sFontStyle = 0;			// Стиль шрифта

	return BrowsePage(Static_MakeHTML,
				FALSE,		// wantSkipTableCells
				FALSE);		// wantSkipParagraphs

}
//********************************************************************
BOOL Static_MakeHTML(
			Handle hObject,
			long reason	// См. enum BROWSE_REASON
			)
{
// В конце вызывается WordControl

	switch(reason)
		{
		case BROWSE_CHAR:
			// Символ
			// Установить язык
			{
			long lang = CED_GetCharFontLang(hObject);
			if (lang != gLanguage)
				SetLanguage(lang);
			}

			// Стиль шрифта
			FontStyle(CED_GetCharFontAttribs(hObject));

			// Записать символ
			ONE_CHAR(hObject);

			break;

		case BROWSE_LINE_END:
			// Конец строки текста
			if ( gPreserveLineBreaks || gEdLineHardBreak )
				PUT_STRING("<br>");
			break;

		case BROWSE_PARAGRAPH_START:
			// Начало абзаца
			FontStyle(0);
			BeginParagraph(CED_GetAlignment(hObject));
			break;

		case BROWSE_PARAGRAPH_END:
			// Конец абзаца
			FontStyle(0);
			PUT_STRING("</p>");
			break;

		case BROWSE_PAGE_START:
			// Начало страницы
			FontStyle(0);
			PUT_STRING("<html><body>");
			break;

		case BROWSE_PAGE_END:
			// Конец страницы
			PUT_STRING("</body></html>");
			break;

		case BROWSE_TABLE_START:
			// Начало таблицы
			FontStyle(0);
			PUT_STRING("<table border>");
			break;

		case BROWSE_TABLE_END:
			// Конец таблицы
			FontStyle(0);
			PUT_STRING("</table>");
			break;

		case BROWSE_ROW_START:
			// Начало строки таблицы
			PUT_STRING("<tr>");
			break;

		case BROWSE_CELL_START:
			// Начало ячейки таблицы
			CellStart();
			break;

		case BROWSE_PICTURE:
			// Картинка
			Picture();
			break;

		}

	// Слежение за словами и строками
	WORDS_CONTROL(reason);

	// Устранение избыточных тегов
	OptimizeTags();

	return TRUE;	// Продолжить просмотр
}
//********************************************************************
static BOOL FontStyle(ULONG newStyle)
{

if ((newStyle & FONT_BOLD) &&
				(!(sFontStyle & FONT_BOLD) ||
				  (sFontStyle & FONT_LIGHT)))
	{PUT_STRING("<b>");}

else if ((sFontStyle & FONT_BOLD) &&
				(!(newStyle & FONT_BOLD) ||
				  (newStyle & FONT_LIGHT)))
	{PUT_STRING("</b>");}

if ((newStyle & FONT_ITALIC) &&
				(!(sFontStyle & FONT_ITALIC) ))
	{PUT_STRING("<i>");}

else if ((sFontStyle & FONT_ITALIC) &&
				(!(newStyle & FONT_ITALIC) ))
	{PUT_STRING("</i>");}

if ((newStyle & FONT_UNDERLINE) &&
	!(sFontStyle & FONT_UNDERLINE))
	{PUT_STRING("<u>");}

else if ((sFontStyle & FONT_UNDERLINE) &&
		 !(newStyle & FONT_UNDERLINE))
	{PUT_STRING("</u>");}

// Запомнить шрифт
sFontStyle = newStyle;
return TRUE;
}
//********************************************************************
static BOOL BeginParagraph(ULONG alignment)
{
	char *p = 0;
	char buf[80] = "";

switch ( alignment & ALIGN_MASK)
	{
	case ALIGN_CENTER:
		p = "center";
		break;

	case (ALIGN_LEFT | ALIGN_RIGHT):
		p = "justify";
		break;

	case ALIGN_LEFT:
	default:
		// "left" by default
		PUT_STRING("<p>");
		return TRUE;
	}

sprintf(buf,"<p align=%s>",p);
PUT_STRING(buf);

return TRUE;
}
//********************************************************************
static BOOL CellStart()
{
// Ячейка таблицы
	char buf[80] = "";

	// Вычислить размер ячейки
	CalcCellSpan();

	if ( rowspan == 1 && colspan == 1 )
		strcpy(buf,"<td>");

	else if ( rowspan > 1 && colspan == 1 )
		sprintf(buf,"<td rowspan=%d>",rowspan);

	else if ( rowspan == 1 && colspan > 1 )
		sprintf(buf,"<td colspan=%d>",colspan);

	else // ( rowspan > 1 && colspan > 1 )
		sprintf(buf,"<td rowspan=%d colspan=%d>",rowspan,colspan);

	PUT_STRING(buf);
	return TRUE;
}
//********************************************************************
static BOOL CalcCellSpan()
{
// Вычислить размер ячейки
	long row,col;

	rowspan = 0;
	colspan = 0;

	// Просмотр вниз от текущей ячейки
	row = gIndexTableRow;
	col = gIndexTableCol;

	while ( row < gTableRows &&
			gIndexTableCell == gLogicalCells[row*gTableCols+col]
		  )
		{
		rowspan++;
		row++;
		}

	// Просмотр вправо от текущей ячейки
	row = gIndexTableRow;
	col = gIndexTableCol;

	while ( col < gTableCols &&
			gIndexTableCell == gLogicalCells[row*gTableCols+col]
		  )
		{
		colspan++;
		col++;
		}

	ASSERT(rowspan>0 && colspan>0);
	return TRUE;
}
//********************************************************************
static BOOL OptimizeTags()
{
	// Устранение избыточных тегов
	long l1 = 0, l2 = 0;
	char *p;

#define SUBST(a,b) {\
		l1 = strlen(a);\
		l2 = strlen(b);\
		p = (char*)gMemCur - l1;\
		if (!memcmp(a,p,l1))\
			{\
			strcpy(p,b);\
			gMemCur -= l1 - l2;\
			}\
		}

	SUBST("<td><p>","<td>");
	SUBST("</p><td>","<td>");
	SUBST("</p></table>","</table>");
	SUBST("<p></p>","");
	SUBST("<br></p>","</p>");

	return TRUE;
}
//********************************************************************
static BOOL Picture()
{
/* Картинка.

	gPictureNumber - Номер картинки от 1
	gPictureData   - Адрес DIB включая заголовок
	gPictureLength - Длина DIB включая заголовок

	1. Создать подпапку для картинок "<page>_files"
	2. Записать картинку в BMP-файл <номер>.bmp.
	3. Вставить тег "img" со ссылкой на файл картинки.
*/
	char buf[256] = "";
	char absPicFileName[256] = "";
	char relPicFileName[256] = "";
	char dir[_MAX_PATH], name[_MAX_PATH], ext[_MAX_EXT];

	// Создать подпапку для картинок gPageFilesFolder.
	if ( !CreatePageFilesFolder() )
		return FALSE;

	// Изготовить имя файла
	split_path(gPageName, dir, name, ext);

	// Записать картинку в BMP-файл
	sprintf (absPicFileName,"%s/%s/%d.bmp", dir,
		gPageFilesFolder, gPictureNumber);

	sprintf (relPicFileName,"%s/%d.bmp",
		gPageFilesFolder, gPictureNumber);

	if ( !WritePictureToBMP_File(
					gPictureData,
					gPictureLength,
					absPicFileName)
		)
		return FALSE;

	// Записать тег "img" со ссылкой на файл картинки.
	sprintf (buf,"<img src=%s "
"width=%d height=%d "
"alt=\"%s\">",
		relPicFileName,
		gPictureGoal.cx * 72L / 1440L,
		gPictureGoal.cy * 72L / 1440L,
		relPicFileName
		);

	PUT_STRING(buf);
	return TRUE;
}
//********************************************************************

/**
 * Create a subdirectory to hold image files for html document.
 */
static BOOL CreatePageFilesFolder() {
    // Создать подпапку для картинок gPageFilesFolder.
    char dir[_MAX_PATH], name[_MAX_PATH], ext[_MAX_EXT], path[_MAX_PATH];

    // Задано ли имя страницы?
    if (!gPageName[0])
        return FALSE;

    // Изготовить имя подпапки
    split_path(gPageName, dir, name, ext);
    memset(gPageFilesFolder, 0, sizeof(gPageFilesFolder));
    sprintf(gPageFilesFolder, "%s_files", name);

    // Создать подпапку
    if(dir[0])
        sprintf(path, "%s/%s", dir, gPageFilesFolder);
    else
        sprintf(path, "%s", gPageFilesFolder);
    if (CreateDirectory(&path[0], 0) == FALSE) {
        DWORD err = GetLastError();
        if (err != ERROR_ALREADY_EXISTS) {
            DEBUG_PRINT("CreatePageFilesFolder error = %d",err);
            return FALSE;
        }
    }

    return TRUE;
}
//********************************************************************
