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

#ifdef _MSC_VER
#include <windows.h>
#include <crtdbg.h>
#else
#include <unistd.h>
#endif
#include "lst3_win.h"

#include "wind32.h"

#include "compat_defs.h"

int32_t filelength_m(FILE *stream)
{
   long pos=fseek(stream, 0L, SEEK_END);
   fseek(stream, 0L, SEEK_SET);
   return pos;
}

static uint cr=13,lf=10;
/*==Return:
    -1 - END_OF_FILE,
    -2 - int32_t STRING,
    >=0 - длина строки,если NULL_STRING,=0 (нормал. выход)
  Параметры:
    str - буфер строки,
    max_len - ограничение на длину строки
    f - идентификатор файла (текстового)*/
int fgets_m(char *str,int max_len,FILE *f)
/*=========*/
{ int len=-1;
  while(++len < max_len)
  { str[len]=get_kod(f);
    if(str[len]==0) return -1; /*END FILE*/
    if(len && (uint)str[len-1] == cr && (uint)str[len] == lf)
      { str[--len]=0; return len; }
  }
  str[len-1]=0; return -2; /*int32_t STRING*/
}
/*=========Return: >0 - код символа, 0 - END_OF_FILE*/
char get_kod(FILE *f)
/*=========*/
{ char sym;
  if(fread(&sym,1,1,f) == 0) return 0; /*END FILE*/
  return sym;
}
/*==Return:
    -1 - END_OF_FILE,
    -2 - int32_t STRING,
    >=0 - длина строки,если NULL_STRING,=0 (нормал. выход)
  Параметры:
    str - буфер строки,
    max_len - ограничение на длину строки
    f - идентификатор файла (текстового)*/
int fgets1_m(char *str,int max_len,FILE *f)
/*=========*/
{ int len=-1;
  if(f==NULL) { get1_kod(f); return 0; }
  while(++len < max_len)
  { str[len]=get1_kod(f);
    if(str[len]==0) return -1; /*END FILE*/
    if(len && (uint)str[len-1] == cr && (uint)str[len] == lf)
      { str[--len]=0; return len; }
  }
  str[len-1]=0; return -2; /*int32_t STRING*/
}
#define SIZE_BLOC 512
/*=========Return: >0 - код символа, 0 - END_OF_FILE*/
char get1_kod(FILE *f)
/*=========*/
{ static char *b; static int len=-1,pos=-1;
  if(f==NULL)
   { if(len!=-1 || pos!=-1) free(b); len=-1,pos=-1; return 0; }
  if(pos >= len)
  { if(pos == -1) b=(char*)malloc(SIZE_BLOC);
    if((len=fread(b,1,SIZE_BLOC,f)) == 0) {free(b);return 0;/*END FILE*/}
    pos=0;
  }
  return b[pos++];
}
/*=Параметры:
   str   - входная строка,
   param - строка, содержащая текущий выделенный параметр(разделитель - пробел),
   max_len-максимал. длина знаков в параметре.
   Return: указатель на строку, из которой удалены знаки параметра,
           если параметр не удалось выделить, возвращается NULL STRING
*/
char *get_param(char *str,char *param,int max_len)
/*=========*/
{ int len;
  len=-1; while(str[++len]==' '); str+=len;/*Убираем пробелы слева от параметра*/
  len=-1;
  while(++len < max_len && str[len] != ' ') /*Поиск первого пробела справа*/
  { if( (param[len]=str[len]) == 0) break; /*Detect END STRING*/
    if(len && (uint)str[len-1] == cr && (uint)str[len] == lf) /*Detect <cr><lf>*/
      { --len; break; }
  }
  /*Если параметр очень длинный, возвращаем неудачу его выделения*/
  if(len < max_len-1) param[len]=0; else param[len=0]=0;
  return str+len;
}
