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


// Words.cpp

//********************************************************************
//********************************************************************

#include <string.h>
#include "stdafx.h"
#include "rout_own.h"

//********************************************************************
Bool IsLetter(Byte c)
{
	Byte type = gAlphabetTable[c];

	return (type & CASE_UPPER) ||
		   (type & CASE_LOWER);
}
//********************************************************************
Bool IsEOL(Byte c)
{
	return c == 0x0d ||
		   c == 0x0a;
}
//********************************************************************
Bool WordsControl(
		long reason	// См. enum BROWSE_REASON
		)
{
// Слежение за словами и строками
// Вызывается после записи символа или конца строки
// или после иной обработки

	switch(reason)
		{
		case BROWSE_PARAGRAPH_START:
			// Начало абзаца
			gLastEOL = NULL;
			gBegWord = NULL;
			gDefis = NULL;
			gCharBack = NULL;
			break;

		case BROWSE_PARAGRAPH_END:
			// Конец абзаца
			// Обработать конец слова если есть
			if ( gBegWord && !WordEnd() )
				return FALSE;

			gLastEOL = NULL;
			gBegWord = NULL;
			gDefis = NULL;
			gCharBack = NULL;
			break;

		case BROWSE_LINE_END:
			// Конец строки
			// Запоминается адрес до записи конца строки
			// или пробела в начале следующей строки
			if ( gCharBack && IsEOL(*gCharBack) )
				gLastEOL = gCharBack;
			else
				gLastEOL = gMemCur;
			break;

		case BROWSE_LINE_START:
			// Начало строки
			{
			Byte c = *(gMemCur-1);	// Предыдущий символ
			if (gLastEOL &&			// Предыдущая строка
				c &&				// Не кончается нулем
				!IsEOL(c) &&		// Нет конца строки текста
				!(gFormat==ROUT_FMT_HTML && // Нет конца строки HTML
				  !memcmp("<br>",(char*)gMemCur-4,4)
				 ) &&
				c != ' '			// На конце не пробел
				)
				{
				// Вставить пробел перед новой строкой
				*gMemCur++ = ' ';
				}
			}
			break;

		case BROWSE_CHAR:
			// Символ

			// Буква
			if (IS_LETTER(gCharCode))
				{
				// Начало слова
				if(!gBegWord)
					gBegWord = gCharBack;
				}
			// Не буква. А слово есть.
			else if (gBegWord)
				{
				// Дефис
				if (gCharCode == '-')
					{
					Byte c = *(gMemCur-2);	// Предыдущий символ
					if ( c == '-' )
						{
						// Два дефиса подряд это тире
						if ( gDefis == gMemCur-2 )
							gDefis = 0;
						}
					else
						{
						// Скрытый дефис в "мягкой" строке может быть удален
						if ( gEdCharHidden &&
							!(gPreserveLineBreaks || gEdLineHardBreak)
						   )
 							gDefis = gCharBack;
						}
					}

				// Конец слова
				else
					{
					// Обработать конец слова
					if ( !WordEnd() )
						return FALSE;
					}
				}

			break;

		}

	return TRUE;
}
//********************************************************************
Bool WordEnd()
{
// Обработать конец слова
// Имеется: gBegWord > 0,
//			gDefis >= 0,
//			gLastEOL >= 0

	Bool hasWordBreak = (gBegWord < gDefis &&
					     gDefis + 1 == gLastEOL &&
						*gLastEOL == ' '
						 );

	// Обработка переноса
	if (hasWordBreak)
		{
		// Стереть дефис и пробел между строками
		long lth = 2;
		memcpy(gDefis, gDefis + lth,
			   gMemCur - gDefis - lth);
		gMemCur -= lth;
		*gMemCur = 0;
		}

	gBegWord = NULL;
	gDefis = NULL;
	return TRUE;
}
//********************************************************************
