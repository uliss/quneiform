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

#include "rout_own.h"
#include "compat_defs.h"

static Bool Static_MakeHTML(Handle hObject, long reason);

static Bool FontStyle(ulong newStyle);
static Bool BeginParagraph(Handle hObject);
static Bool CellStart();
static Bool CalcCellSpan();
static Bool OptimizeTags();
static Bool Picture();
static Bool CreatePageFilesFolder();

static ulong sFontStyle = 0; // Стиль шрифта
static long rowspan = 0, colspan = 0;
static Bool hocrmode = FALSE; // If true, print hOCR tags to output.

//********************************************************************
Bool MakeHTML() {
	/* Формат HTML.

	 Включаются таблицы.
	 Концы строк сохраняются, если gPreserveLineBreaks = TRUE.
	 */
	sFontStyle = 0; // Стиль шрифта
	hocrmode = FALSE;

	return BrowsePage(Static_MakeHTML, FALSE, // wantSkipTableCells
			FALSE); // wantSkipParagraphs

}
//********************************************************************
Bool MakeHOCR() {
	sFontStyle = 0;
	hocrmode = TRUE;
	return BrowsePage(Static_MakeHTML, FALSE, FALSE);
}
//********************************************************************
Bool Static_MakeHTML(Handle hObject, long reason // См. enum BROWSE_REASON
) {
	char buf[256] = "";
	edRect r;
	// В конце вызывается WordControl

	switch (reason) {
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

		r = CED_GetCharLayout(hObject);
		// Записать символ
		if (r.left != -1 && hocrmode) {
			sprintf(buf, "<span title=\"bbox %d %d %d %d\">", r.left, r.top,
					r.right, r.bottom);
			PUT_STRING(buf);
		}

		ONE_CHAR(hObject)
		;
		if (r.left != -1 && hocrmode)
			PUT_STRING("</span>")
		;

		break;

	case BROWSE_LINE_END:
		// Конец строки текста
		if (gPreserveLineBreaks || gEdLineHardBreak)
			PUT_STRING("<br>")
		;
		break;

	case BROWSE_PARAGRAPH_START:
		// Начало абзаца
		FontStyle(0);
		BeginParagraph(hObject);
		break;

	case BROWSE_PARAGRAPH_END:
		// Конец абзаца
		FontStyle(0);
		PUT_STRING("</p>")
		;
		break;

	case BROWSE_PAGE_START:
		// Start of page.
		FontStyle(0);
		PUT_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"   \"http://www.w3.org/TR/html4/loose.dtd\">\n")
		;
		PUT_STRING("<html><head><title></title>")
		;
		if (gActiveCode == ROUT_CODE_UTF8)
			PUT_STRING("<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" >")
		;
		PUT_STRING("</head><body>")
		;

		break;

	case BROWSE_PAGE_END:
		// Конец страницы
		PUT_STRING("</body></html>")
		;
		break;

	case BROWSE_TABLE_START:
		// Начало таблицы
		FontStyle(0);
		PUT_STRING("<table border>")
		;
		break;

	case BROWSE_TABLE_END:
		// Конец таблицы
		FontStyle(0);
		PUT_STRING("</table>")
		;
		break;

	case BROWSE_ROW_START:
		// Начало строки таблицы
		PUT_STRING("<tr>")
		;
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

	return TRUE; // Продолжить просмотр
}
//********************************************************************
static Bool FontStyle(ulong newStyle) {

	if ((newStyle & FONT_BOLD) && (!(sFontStyle & FONT_BOLD) || (sFontStyle
			& FONT_LIGHT))) {
		PUT_STRING("<b>");
	}

	else if ((sFontStyle & FONT_BOLD) && (!(newStyle & FONT_BOLD) || (newStyle
			& FONT_LIGHT))) {
		PUT_STRING("</b>");
	}

	if ((newStyle & FONT_ITALIC) && (!(sFontStyle & FONT_ITALIC))) {
		PUT_STRING("<i>");
	}

	else if ((sFontStyle & FONT_ITALIC) && (!(newStyle & FONT_ITALIC))) {
		PUT_STRING("</i>");
	}

	if ((newStyle & FONT_UNDERLINE) && !(sFontStyle & FONT_UNDERLINE)) {
		PUT_STRING("<u>");
	}

	else if ((sFontStyle & FONT_UNDERLINE) && !(newStyle & FONT_UNDERLINE)) {
		PUT_STRING("</u>");
	}

	// Запомнить шрифт
	sFontStyle = newStyle;
	return TRUE;
}
//********************************************************************
static Bool BeginParagraph(Handle hObject) {
	const char *p = NULL;
	char buf[80] = "";
	edBox b = CED_GetLayout(hObject);
	ulong alignment = CED_GetAlignment(hObject);

	switch (alignment & ALIGN_MASK) {
	case ALIGN_CENTER:
		p = "center";
		break;

	case (ALIGN_LEFT | ALIGN_RIGHT):
		p = "justify";
		break;

	case ALIGN_LEFT:
	default:
		// "left" by default
		;
	}

	PUT_STRING("<p");
	if (p) {
		sprintf(buf, " align=\"%s\"", p);
		PUT_STRING(buf);
	}

	if (b.x != -1 && hocrmode) {
		sprintf(buf, " title=\"bbox %d %d %d %d\"", b.x, b.y, b.x + b.w, b.y
				+ b.h);
		PUT_STRING(buf);
	}
	PUT_STRING(">");

	return TRUE;
}
//********************************************************************
static Bool CellStart() {
	// Ячейка таблицы
	char buf[80] = "";

	// Вычислить размер ячейки
	CalcCellSpan();

	if (rowspan == 1 && colspan == 1)
		strcpy(buf, "<td>");

	else if (rowspan > 1 && colspan == 1)
		sprintf(buf, "<td rowspan=\"%ld\">", rowspan);

	else if (rowspan == 1 && colspan > 1)
		sprintf(buf, "<td colspan=\"%ld\">", colspan);

	else
		// ( rowspan > 1 && colspan > 1 )
		sprintf(buf, "<td rowspan=\"%ld\" colspan=\"%ld\">", rowspan, colspan);

	PUT_STRING(buf);
	return TRUE;
}
//********************************************************************
static Bool CalcCellSpan() {
	// Вычислить размер ячейки
	long row, col;

	rowspan = 0;
	colspan = 0;

	// Просмотр вниз от текущей ячейки
	row = gIndexTableRow;
	col = gIndexTableCol;

	while (row < gTableRows && gIndexTableCell == gLogicalCells[row
			* gTableCols + col]) {
		rowspan++;
		row++;
	}

	// Просмотр вправо от текущей ячейки
	row = gIndexTableRow;
	col = gIndexTableCol;

	while (col < gTableCols && gIndexTableCell == gLogicalCells[row
			* gTableCols + col]) {
		colspan++;
		col++;
	}

	ASSERT(rowspan>0 && colspan>0);
	return TRUE;
}
//********************************************************************
static Bool OptimizeTags() {
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
static Bool Picture() {
	/* Картинка.

	 gPictureNumber - img number 1
	 gPictureData   - DIB address, wiith header
	 gPictureLength - DIB length, with header

	 1. Создать подпапку для картинок "<page>_files"
	 2. Записать картинку в BMP-файл <номер>.bmp.
	 3. Вставить тег "img" со ссылкой на файл картинки.
	 */
	char buf[256] = "";
	char absPicFileName[256] = "";
	char relPicFileName[256] = "";
	char dir[PATH_MAX], name[PATH_MAX], ext[_MAX_EXT];

	// create folder for images gPageFilesFolder.
	if (!CreatePageFilesFolder())
		return FALSE;

	// create file name
	split_path(gPageName, dir, name, ext);

	// write picture to bmp file
	if (dir[0])
		sprintf(absPicFileName, "%s/%s/%ld.bmp", dir, gPageFilesFolder,
				gPictureNumber);
	else
		sprintf(absPicFileName, "%s/%ld.bmp", gPageFilesFolder, gPictureNumber);

	sprintf(relPicFileName, "%s/%ld.bmp", gPageFilesFolder, gPictureNumber);

	if (!WritePictureToBMP_File(gPictureData, gPictureLength, absPicFileName))
		return FALSE;

	// write img html tag.
	sprintf(buf, "<img src=\"%s\" "
		"width=\"%ld\" height=\"%ld\" "
		"alt=\"%s\">", relPicFileName, gPictureGoal.cx * 72L / 1440L,
			gPictureGoal.cy * 72L / 1440L, relPicFileName);

	PUT_STRING(buf);
	return TRUE;
}
//********************************************************************

/**
 * Create a subdirectory to hold image files for html document.
 */
static Bool CreatePageFilesFolder() {
	// Создать подпапку для картинок gPageFilesFolder.
	char dir[PATH_MAX], name[PATH_MAX], ext[_MAX_EXT], path[PATH_MAX];

	// Задано ли имя страницы?
	if (!gPageName[0])
		return FALSE;

	// Изготовить имя подпапки
	split_path(gPageName, dir, name, ext);
	memset(gPageFilesFolder, 0, sizeof(gPageFilesFolder));
	sprintf(gPageFilesFolder, "%s_files", name);

	// Создать подпапку
	if (dir[0])
		sprintf(path, "%s/%s", dir, gPageFilesFolder);
	else
		sprintf(path, "%s", gPageFilesFolder);
	if (CreateDirectory(&path[0]) == FALSE) {
		uint32_t err = GetLastError();
		if (err != ERROR_ALREADY_EXISTS) {
			DEBUG_PRINT("CreatePageFilesFolder error = %d", err);
			return FALSE;
		}
	}

	return TRUE;
}
//********************************************************************
