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

#ifndef __XPATH_H
#define __XPATH_H

#ifndef __XTEXT_H
#include "xtext.h"
#endif

#include "compat_defs.h"

class XPath : public XText< _MAX_PATH >
{
public:
   XPath(void):
      XText< _MAX_PATH >(""){};

   XPath(const char* path ):
      XText< _MAX_PATH >(path)
   {};

   XPath& operator = (const char * path )
      {
         *(XText< _MAX_PATH >*)this = path;
         return *this;
      }

   XPath& operator = (const XText< _MAX_PATH >& path )
      {
         *(XText< _MAX_PATH >*)this = path;
         return *this;
      }

   XPath(const char* dir, const char* name, const char* ext):
      XText< _MAX_PATH >()
   {
      make_path(buf, dir, name, ext);
   };

   XPath& GetModuleFileName(HMODULE hModule = NULL)
   {
      ::GetModuleFileName( hModule, buf, sizeof(buf) );
      return *this;
   };

   XPath& GetCurrentDirectory(void)
   {
      curr_dir( sizeof(buf), buf );
      return *this;
   };

   XPath& GetTempPath(void)
   {
      ::GetTempPath( sizeof(buf), buf );
      return *this;
   }

   XPath& CutPath(void) // оставляем только путь (без имени и расширения)
   {
      char dir[_MAX_DIR];
      char name[_MAX_FNAME];
      char ext[_MAX_EXT];

      split_path(buf, dir, name, ext);
      make_path(buf, dir, NULL, NULL);
      return *this;
   }

   XPath& CutName(void) // оставляем только имя (без расширения)
   {
      char dir[_MAX_DIR];
      char name[_MAX_FNAME];
      char ext[_MAX_EXT];

      split_path(buf, dir, name, ext);
      make_path(buf, NULL, name, NULL);
      return *this;
   }

   XPath& CutNameEx(void) // оставляем только имя (с расширением)
   {
      char dir[_MAX_DIR];
      char name[_MAX_FNAME];
      char ext[_MAX_EXT];

      split_path(buf, dir, name, ext);
      make_path(buf, NULL, name, ext);
      return *this;
   }

   XPath& CutExt(void)
   {
      char dir[_MAX_DIR];
      char name[_MAX_FNAME];
      char ext[_MAX_EXT];

      split_path(buf, dir, name, ext);
      make_path(buf, NULL, NULL, ext);
      return *this;
   }


   XPath& SetExt(const char* new_ext) // заменяем или добавляем новое расширение
   {
      char dir[_MAX_DIR];
      char name[_MAX_FNAME];

      split_path(buf, dir, name, NULL);
      make_path(buf, dir, name, new_ext);
      return *this;
   };

   XPath& CheckSlash(void)        // проверяем и добавляем слэш в конце если нет
   {
      int len=strlen(buf);
      if (buf[0] && buf[len-1] != '/' )
      {
         buf[len]='/';
         buf[len+1]=0;
      }

      return *this;
   }

   XPath& StripSlash(void)        // проверяем и убираем слэш в конце если есть
   {
      int len=strlen(buf);
      if (buf[0] && buf[len-1] == '/' )
      {
         buf[len-1]=0;
      }

      return *this;
   }

   XPath& SetName(const char* new_name) // заменяем или добавляем новое имя+расш
   {
      char dir[_MAX_DIR];

      split_path(buf, dir, NULL, NULL);
      make_path(buf, dir, new_name,NULL);
      return *this;
   };

   XPath& EraseDrive() // удаляем название диска
   {
      char dir[_MAX_DIR];
      char name[_MAX_FNAME];
      char ext[_MAX_EXT];
      split_path(buf, dir, name, ext);
      make_path(buf, dir, name,ext);
      return *this;
   };

   XPath& SetDrive(const char* pDrive) // удаляем название диска
   {
      char dir[_MAX_DIR];
      char name[_MAX_FNAME];
      char ext[_MAX_EXT];
      split_path(buf, dir, name, ext);
      make_path(buf, dir, name,ext);
      return *this;
   };

   void Split(char* pDir, char* pFileName, char* pExt) // разбиваем путь на компоненты
   {
      split_path(buf, pDir, pFileName, pExt);
   };

};

#endif // XPath
