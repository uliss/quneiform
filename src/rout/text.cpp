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

// Text.cpp

//********************************************************************
//********************************************************************

#include <string.h>

#include "rout_own.h"
#include "ligas.h"	// Теперь в HHH 01.09.2000 E.P.
static Bool Static_MakeText(Handle hObject, long reason);
static Bool TableStub();
static Bool IncludeTable();

//********************************************************************
Bool MakeText() {
	/* Текстовые форматы.

	 Таблицы выдаются в виде:
	 "<< Таблица 1: строк 1, столбцов 2, ячеек 2 >>"

	 Концы строк сохраняются, если gPreserveLineBreaks = TRUE.
	 */

	return BrowsePage(Static_MakeText, TRUE, // wantSkipTableCells
			FALSE); // wantSkipParagraphs

}
//********************************************************************
Bool Static_MakeText(Handle hObject, long reason // См. enum BROWSE_REASON
) {
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

		// Записать символ
		ONE_CHAR(hObject)
		;
		break;

	case BROWSE_LINE_END:
		// Конец строки
		if (0 || gFormat == ROUT_FMT_SmartText || (gPreserveLineBreaks
				|| gEdLineHardBreak))
			NEW_LINE
		;
		break;

	case BROWSE_PARAGRAPH_END:
		// Конец абзаца
		NEW_LINE
		;
		break;

	case BROWSE_TABLE_START:
		// Начало таблицы

		// Включить таблицу в текст страницы
		if (gTableTextOptions & ROUT_TABLE_TEXT_INCLUDED) {
			if (!IncludeTable())
				return FALSE;
		} else
		// Обозначить место таблицы в тексте страницы
		{
			if (!TableStub())
				return FALSE;
		}

		break;

	case BROWSE_TABLE_END:
		// Конец таблицы
		NEW_LINE
		;
		break;

	}

	// Слежение за словами и строками
	WORDS_CONTROL(reason);

	return TRUE; // Продолжить просмотр
}
//********************************************************************
Bool NewLine() {
	// Записать конец строки

	CHECK_MEMORY(10);

	// Для модуля Words.cpp
	gCharBack = gMemCur;

	Byte *p = (Byte*) gEOL;
	while (*p)
		*gMemCur++ = *p++;

	return TRUE;
}
//********************************************************************
Bool OneChar(Handle charHandle) {
	// write one symbol
	char sCodePageName[200] = { 0 };
	CHECK_MEMORY(10);

	struct letterEx *alt = CED_GetAlternatives(charHandle);
	ASSERT(alt);
	long codePage = GetCodePage();

	// source symbol
	Byte c1 = alt->alternative;

	Byte c2 = gActiveCodeTable[c1];

	// remember for the  Words.cpp module
	gCharHandle = charHandle;
	gAlt = alt;
	gCharCode = c1;
	gCharBack = gMemCur;

	// Пробелы, вставленные для выравнивания SmartText,
	// имеют оценку 253; их следует пропускать
	// для всех остальных форматов
	if (c1 == ' ' && alt->probability == 253 && gFormat != ROUT_FMT_SmartText)
		return TRUE;

	// Специальные случаи перекодировки
	switch (c1) {
	// Французский Макинтош - непонятно,
	// но оставляю как есть. 26.05.99
#ifdef __MAC__
	case (uchar)206: case (uchar)207:
	case (uchar)174: case (uchar)190:
	if( gLanguage==LANGUAGE_FRENCH )
	c2=с1;
	break;
#endif
	// unknows symbol
	case bad_char:
		c2 = gBadChar;
		break;

	case 0x97:
		//  long dash -> double hyphen
		// except SmartText и HTML
		// 29.02.2000
		if (gFormat == ROUT_FMT_HTML || gFormat == ROUT_FMT_HOCR || gActiveCode
				== ROUT_CODE_UTF8) {
			// there is long dash in html and unicode
			c2 = c1;
		} else {
			c2 = '-';
			if (gFormat != ROUT_FMT_SmartText)
				*gMemCur++ = '-';
		}
		break;

		// Угловые скобки в HTML заменяются на круглые
	case '<':
	case '>':
		if (gFormat == ROUT_FMT_HTML || gFormat == ROUT_FMT_HOCR) {
			*gMemCur++ = '&';
			*gMemCur++ = (c1 == '<' ? 'l' : 'g');
			*gMemCur++ = 't';
			*gMemCur++ = ';';
			return TRUE;
		}
		break;

	case '&':
		if (gFormat == ROUT_FMT_HTML || gFormat == ROUT_FMT_HOCR) {
			*gMemCur++ = '&';
			*gMemCur++ = 'a';
			*gMemCur++ = 'm';
			*gMemCur++ = 'p';
			*gMemCur++ = ';';
			return TRUE;
		}
		break;

		// Дифтонг oe / OE
	case oe_deaf_sound:
		if (FALSE == (gLanguage == LANGUAGE_RUSSIAN || langKaz || // 18.08.98 E.P.
				gLanguage == LANGUAGE_FRENCH && gActiveCode == ROUT_CODE_ANSI)
				|| gFormat == ROUT_FMT_HTML || gFormat == ROUT_FMT_HOCR) {
			*gMemCur++ = 'o';
			c2 = 'e';
		}
		break;

	case OE_cap_deaf_sound:
		if (FALSE == (gLanguage == LANGUAGE_RUSSIAN || langKaz || // 18.08.98 E.P.
				gLanguage == LANGUAGE_FRENCH && gActiveCode == ROUT_CODE_ANSI)
				|| gFormat == ROUT_FMT_HTML || gFormat == ROUT_FMT_HOCR) {
			*gMemCur++ = 'O';
			c2 = 'E';
		}
		break;

#ifndef __MAC__
		//	0xA9 © -> (C),
		//	0xAE ® -> (R)
	case 0xA9:
	case 0xAE:
		if (gActiveCode == ROUT_CODE_ASCII || gActiveCode == ROUT_CODE_ISO)
			*gMemCur++ = '(';

		*gMemCur++ = c2;

		if (gActiveCode == ROUT_CODE_ASCII || gActiveCode == ROUT_CODE_ISO)
			*gMemCur++ = ')';

		return TRUE;
		break;

#endif
		// 0x99 ™ -> (TM) except ANSI and UTF8
	case 0x99:
		if (gActiveCode != ROUT_CODE_ANSI && gActiveCode != ROUT_CODE_UTF8) {
			*gMemCur++ = '(';
			*gMemCur++ = c2; // 'T'
			*gMemCur++ = 'M';
			*gMemCur++ = ')';
			return TRUE;
		}

		break;

	} // switch(c1)
	if (gActiveCode != ROUT_CODE_UTF8) {
		*gMemCur++ = c2;
	} else {
		const char *utfchar;
		utfchar = getUTF8Str((char) c2, GetCodePage());
		int i;
		for (i = 0; utfchar[i] != '\0'; i++) {
			*gMemCur++ = utfchar[i];
		}
	}
	return TRUE;
}
//********************************************************************
Bool PutString(char *s) {
	// Запись строки в ответ.
	// Желательно избегать концов строк, для этого
	// рекомендуется использовать NEW_LINE

	ulong lth = strlen(s);

	if (!lth)
		return TRUE;

	CHECK_MEMORY(lth+10); // С запасом

	strcpy((char*) gMemCur, s);
	gMemCur += lth;
	return TRUE;
}
//********************************************************************
Bool PutSpaces(long lth) {
	// Запись пробелов в ответ.

	if (lth <= 0)
		return TRUE;

	CHECK_MEMORY(lth+10); // С запасом

	memset(gMemCur, ' ', lth);
	gMemCur += lth;
	return TRUE;
}
//********************************************************************
static Bool TableStub() {
	// Обозначение таблицы в тексте страницы

	// Слова "Таблица", "Строк:", "Столбцов:", "Ячеек:"
	char word1[40] = "Таблица:", word2[40] = "Строк:", word3[40] = "Столбцов",
			word4[40] = "Ячеек";

	char buf[1000] = "";
	snprintf(buf, sizeof(buf), "<< %s %d: %s %d, %s %d, %s %d >>", word1, gHitTables, word2,
			gTableRows, word3, gTableCols, word4, gTableCells);

	PUT_STRING(buf);
	NEW_LINE;
	return TRUE;
}
//********************************************************************
static Bool IncludeTable() {
	/*
	 Включить таблицу в текст страницы

	 */

	// Сохранить начало памяти
	Byte *saveMemStart = gMemStart;

	// Начало оставшейся свободной памяти
	gMemStart = gMemCur;

	// Обнулить свободную память
	memset(gMemStart, 0, gMemEnd - gMemStart);

	// Вызвать табличный текст
	Bool good = MakeTableText();

	// Восстановить начало памяти
	gMemStart = saveMemStart;

	return good;
}
//********************************************************************
