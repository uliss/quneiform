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

#include "internal.h"

#include <stdarg.h>
#include "std.h"
#include "cfcompat.h"

FILE *gStdOutputFile = 0;

static FTConsole _ConsoleHandler = NULL;
STD_FUNC( void ) stdSetConsoleHandler(FTConsole handler)
{
    _ConsoleHandler = handler;
}

static char _ConsoleFileName[256] = { 0 };
STD_FUNC( void ) stdSetConsoleFile(char * file_name)
{
    _ConsoleFileName[0] = 0;
    if (file_name)
        strcpy(_ConsoleFileName, file_name);
}

STD_FUNC( int ) stdConsole(const char* str, ...)
{
    if (str == NULL)
        return 0;
    int count = 0;

    char strbuf[4096]; // string to be put
    strbuf[0] = '\n';
    va_list list;
    va_start(list, str );
    count = vsprintf(strbuf + 1, str, list);
    va_end( list );
    if (_ConsoleHandler != NULL)
        return (*_ConsoleHandler)(strbuf);

    char* res_str = strbuf[1] == '\n' ? strbuf + 1 : strbuf;

#if defined( WIN32 )
    {
        if (res_str[0]!='\n')
        OutputDebugString("\n"); // cover MSVC Bug
        OutputDebugString(res_str);
    }
#endif

    if (_ConsoleFileName[0] != 0) { // drop message to file
        FILE *ff;
        ff = fopen(_ConsoleFileName, "a");
        if (ff) {
            strcat(res_str, "\n");
            fprintf(ff, res_str);
            fclose(ff);
        }
    }
    return count;
} // stdConsole()

STD_FUNC( int ) stdConsole_(const char* str, ...) // без перевода строки
{
    if (str == NULL)
        return 0;
    int count = 0;

    char strbuf[4096]; // string to be put
    strbuf[0] = 0;
    va_list list;
    va_start(list, str );
    count = vsprintf(strbuf, str, list);
    va_end( list );
    if (_ConsoleHandler != NULL)
        return (*_ConsoleHandler)(strbuf);
    char* res_str = strbuf;

#if defined( WIN32 )
    {
        if (res_str[0]!='\n')
        OutputDebugString("\n"); // cover MSVC Bug
        OutputDebugString(res_str);
    }
#endif

    if (_ConsoleFileName[0] != 0) { // drop message to file
        FILE *ff;
        ff = fopen(_ConsoleFileName, "a");
        if (ff) {
            strcat(res_str, "\n");
            fprintf(ff, res_str);
            fclose(ff);
        }
    }
    return count;
} // stdConsole()

STD_FUNC(void) stdSetOutputFile(FILE *theFile)
{
    gStdOutputFile = theFile;
}
