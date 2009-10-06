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

#ifndef __XFINDFIL_H
#define __XFINDFIL_H

// files enumeration: _finsfirst() - _findnext()..._findnext() - _findclose()
/*#include <io.h>*/
#include "xpath.h"
#include "safestr.h"

#include "compat_defs.h"

// TODO: check formats, Microsoft io.h defines other values
#ifndef _A_ARCH
#   define _A_ARCH 1
#endif // _A_ARCH

#ifndef _A_HIDDEN
#   define _A_HIDDEN 2
#endif // _A_HIDDEN

#ifndef _A_NORMAL
#   define _A_NORMAL 4
#endif // _A_NORMAL

#ifndef _A_RDONLY
#   define _A_RDONLY 8
#endif // _A_RDONLY

#ifndef _A_SUBDIR
#   define _A_SUBDIR 16
#endif // _A_SUBDIR

#ifndef _A_SYSTEM
#    define _A_SYSTEM 32
#endif // _A_SYSTEM


class XFindFile   // xff
{
   _finddata_t fileinfo;
   long hFind;
   Bool can_reply;
   XPath xpTpl;

public:
   XFindFile( void ): hFind(-1), can_reply(FALSE){};
   XFindFile( const char* name_template ): hFind(-1), can_reply(FALSE){ Restart(name_template); };

   const char* GetSource () { return xpTpl; } /*VMK*/

   Bool Restart( const char* name_template=NULL )
   {
      if (name_template)
         xpTpl = name_template;
      if (hFind != -1)
         _findclose(hFind);
      hFind = _findfirst( xpTpl, &fileinfo );
      can_reply = (hFind != -1);
      return can_reply;
   }

   XFindFile& operator ++(void) // prefix form:  use '++xff' to get next file
   {
      can_reply = (_findnext(hFind, &fileinfo)==0);
      return *this;
   }

   operator char * (void) { return can_reply ? (char*)fileinfo.name : NULL; }
   char* SafeStr(void)    { return can_reply ? (char*)fileinfo.name : (char*)"";   }
   uint32_t FileAttrib(void)    { return can_reply ? (uint32_t)fileinfo.attrib : 0;   }
   Bool  IsArchive (void) { return fileinfo.attrib & _A_ARCH  ; };//  Archive. Set whenever the file is changed, and cleared by the BACKUP command. Value: 0x20
   Bool  IsHidden  (void) { return fileinfo.attrib & _A_HIDDEN; };//  Hidden file. Not normally seen with the DIR command, unless the /AH option is used. Returns information about normal files as well as files with this attribute. Value: 0x02
   Bool  IsNormal  (void) { return fileinfo.attrib & _A_NORMAL; };//  Normal. File can be read or written to without restriction. Value: 0x00
   Bool  IsReadOnly(void) { return fileinfo.attrib & _A_RDONLY; };//  Read-only. File cannot be opened for writing, and a file with the same name cannot be created. Value: 0x01
   Bool  IsSubdir  (void) { return fileinfo.attrib & _A_SUBDIR; };//  Subdirectory. Value: 0x10
   Bool  IsSystem  (void) { return fileinfo.attrib & _A_SYSTEM; };//  System file. Not normally seen with the DIR command, unless the /A or /A:S option is used. Value: 0x04
   ~XFindFile(void)
   {
      if (hFind != -1)
         _findclose(hFind);
      hFind = -1;
   }
};

class XEnumDirs // xed  - directories enumeration
{
   _finddata_t fileinfo;
   long hFind;
   Bool can_reply;
   int nOptions;

#define XED_NO_PARENT   0x0001                           // no ..
#define XED_NO_CURRENT  0x0002                           // no .
#define XED_ONLY_SUBDIR (XED_NO_CURRENT|XED_NO_PARENT)   // no .  and  ..

   void Init(void)      { can_reply = FALSE; hFind = -1; nOptions=0; };

public:
   XEnumDirs( void )  { Init(); };
   XEnumDirs( const char* name_template, int options = 0 )
      { Init(); Restart(name_template, options); };

   void Restart( const char* name_template, int options = 0  )
   {
      nOptions = options;
      if (hFind != -1)
      {
         _findclose(hFind);
         hFind = -1;
      }
      can_reply = FALSE;

      hFind = _findfirst( name_template, &fileinfo );
      if (hFind != -1)
      {
         while (!IsAcceptable())
         {
            if (_findnext(hFind, &fileinfo)!=0)
               return; // with can_reply == FALSE;
         }
         can_reply = TRUE;
      }
   }

   Bool32 IsAcceptable(void)
   {
      if (!(fileinfo.attrib & _A_SUBDIR))
         return FALSE;
      if (fileinfo.name[0]=='.')
      {
         if (nOptions & XED_NO_PARENT && strcmp(fileinfo.name, "..") == 0)
            return FALSE;
         if (nOptions & XED_NO_CURRENT && strcmp(fileinfo.name, ".") == 0)
            return FALSE;
      }
      return TRUE;
   }

   XEnumDirs& operator ++(void) // prefix form:  use '++xed' to get next subdirectory
   {
      can_reply = FALSE;
      do
      {  if (_findnext(hFind, &fileinfo)!=0)
            return *this; // with can_reply == FALSE;
      }  while (!IsAcceptable());
      can_reply = TRUE;
      return *this;
   }

   operator char * (void) { return can_reply ? (char*)fileinfo.name : NULL; }
   char* SafeStr(void)    { return can_reply ? (char*)fileinfo.name : (char*)"";   }

   ~XEnumDirs(void)
   {
      if (hFind != -1)
         _findclose(hFind);
      hFind = -1;
   }
};

class XEnumDirsRecursive // xedr  - перечислитель директорий с поддиректориями
{
   XEnumDirs xed;                 // перечислитель для текущей директории
   XEnumDirsRecursive* pxedr;     // перечислитель поддиректории
   int nDir;                      // длина строки szDir перед спуском в поддиректорию

static char szReply[260];         // буфер для формирования текущего ответа
static char szNameTpl[260];       // шаблон имени, напр. "*.*"
static char szDir[260];           // обследуемая директория без слэша на конце


   void Restart(void)                // на входе: в szDir - директория которую нужно обследовать,
                                     //           в szNameTpl - шаблон имени директории
   {
      char tpl[260];
      sprintf(tpl, "%s\\%s", szDir, szNameTpl);
      xed.Restart( tpl, XED_ONLY_SUBDIR );
      if (xed)
      {
         //strcpy(szReply, xed);        // подготовили первый ответ (в szReply)
         sprintf(szReply, "%s\\%s", szDir, SafeStr(xed) );
      }
      else
         szReply[0]=0;
   }

public:
   XEnumDirsRecursive(void): pxedr(NULL){};
   XEnumDirsRecursive(                // конструктор для стартового вызова
      const char* root_dir,   // например "C:\WINDOWS" или "\\server\d\backup\" или "..\.."
      const char* name_tpl = "*.*"
                     ):
      pxedr(NULL)
   {
      Restart(root_dir, name_tpl);
   }

   void Restart(
      const char* root_dir,   // например "C:\WINDOWS" или "\\server\d\backup\" или "..\.."
      const char* name_tpl = "*.*"
               )
   {
      szReply[0]=0;
      strcpy( szNameTpl, name_tpl );
      strcpy( szDir, root_dir);
      int n=strlen(szDir);
      if (n && szDir[n-1]=='\\')
         szDir[n-1]=0; // убираем слэш в конце
      Restart();
   }

   operator char * (void) const
   {
      return (szReply[0]) ? szReply : NULL;
   }


   XEnumDirsRecursive& operator ++(void) // prefix form:  use '++xed' to get next subdirectory
   {
      if (szReply[0] && pxedr == NULL) // на предыдущем шаге имели ответ, но вглубь еще не спускались
      {
         nDir = strlen( szDir ); // запоминаем длину директории для последующего восстановления
         strcpy( szDir, szReply );
         pxedr = new XEnumDirsRecursive();
         pxedr->Restart();
         if (*pxedr) // внутри тоже есть поддиректории, "ответ готов!"
            return *this;
      }
      else if (pxedr != NULL) // мы в процессе перечисления некоторой поддиректории
      {
         ++(*pxedr); // пытаемся сместиться внутри нее
         if (*pxedr) // удачно
            return *this;
      }

      if (pxedr)
      {  // перечисление ЭТОЙ поддиректории закончилось
         delete pxedr;
         pxedr = NULL;
         szDir[nDir] = 0; // восстанавливаем имя директории
      }

      if (!xed) // перечисление в текущей завершено
         return *this;

      // итак, перечисление в текущей не было завершено
      ++xed; // берем следующую поддиректорию в текущей
      if (!xed) // больше ничего нет
      {
         szReply[0]=0;
         return *this;
      }

      // итак, имеем новую поддиректорию на данном уровне,
      // кладем ее в ответ
      sprintf(szReply, "%s\\%s", szDir, xed.SafeStr());
      return *this;
   }

   ~XEnumDirsRecursive(void)
   {
      if (pxedr) // уничтожаем вложенные перечислители
      {
         delete pxedr;
         pxedr = NULL;
      }
   }
};

#ifdef XEDR_INSTANTIATE   // turn on once per application or dll
char XEnumDirsRecursive::szReply[260]={0};         // буфер для формирования текущего ответа
char XEnumDirsRecursive::szNameTpl[260]={0};       // шаблон имени, напр. "*.*"
char XEnumDirsRecursive::szDir[260]={0};           // обследуемая директория без слэша на конце
#endif //XEDR_INSTANTIATE

class XFindFileRecursive
{
   XFindFile xff;             // перечислитель файлов в директории
   XEnumDirsRecursive xedr;   // перечислитель вложенных поддиректорий
   XPath xpReply;             // очередной ответ
   XText<260> xtNameTpl;
   XPath xpCurDir;

public:
   XFindFileRecursive(void){};
   XFindFileRecursive( const char* root_dir, const char* name_template = "*.*")
      { Restart( root_dir, name_template );}

   XFindFileRecursive& Restart( const char* root_dir, const char* name_template = "*.*" )
   {
      xtNameTpl = name_template;
      xpCurDir = root_dir;
      xpCurDir.StripSlash();
      xff.Restart( XText<256>().sprintf( "%s\\%s", SafeStr(xpCurDir), (char*)xtNameTpl) ); // заготавливаем очередной файл-ответ
      xedr.Restart(xpCurDir, "*.*"); // подготавливаем первую поддиректорию
      while (!xff && xedr) // в текущей поддиректории файлов нет, но есть другие поддиректории
      {
         xff.Restart(  XText<256>().sprintf( "%s\\%s", SafeStr(xedr), (char*)xtNameTpl ) ); // пытаемся загрузить очередной файл-ответ
         xpCurDir = SafeStr(xedr);
         ++xedr; // подготавливаем следующую поддиректорию
      }
      if (xff)
         xpReply.sprintf("%s\\%s", (char*)xpCurDir, SafeStr(xff));
      return *this;
   };


   operator char * (void) const { return (xpReply[0]) ? (char*)xpReply : NULL; }

   XFindFileRecursive& operator ++(void) // prefix form:  use '++xffr' to get next file
   {
      ++xff; // переходим к следующему файлу
      while (!xff && xedr) // в текущей поддиректории файлы кончились, но есть другие поддиректории
      {
         xff.Restart( XText<256>().sprintf( "%s\\%s", SafeStr(xedr), (char*)xtNameTpl ) ); // пытаемся загрузить очередной файл-ответ
         xpCurDir = SafeStr(xedr);
         ++xedr; // подготавливаем следующую поддиректорию
      }
      if (xff)
         xpReply.sprintf("%s\\%s", (char*)xpCurDir, SafeStr(xff));
      else
         xpReply[0]=0;
      return *this;
   }
   Bool  IsArchive () { return xff.IsArchive (); };//  Archive. Set whenever the file is changed, and cleared by the BACKUP command. Value: 0x20
   Bool  IsHidden  () { return xff.IsHidden  (); };//  Hidden file. Not normally seen with the DIR command, unless the /AH option is used. Returns information about normal files as well as files with this attribute. Value: 0x02
   Bool  IsNormal  () { return xff.IsNormal  (); };//  Normal. File can be read or written to without restriction. Value: 0x00
   Bool  IsReadOnly() { return xff.IsReadOnly(); };//  Read-only. File cannot be opened for writing, and a file with the same name cannot be created. Value: 0x01
   Bool  IsSubdir  () { return xff.IsSubdir  (); };//  Subdirectory. Value: 0x10
   Bool  IsSystem  () { return xff.IsSystem  (); };//  System file. Not normally seen with the DIR command, unless the /A or /A:S option is used. Value: 0x04
};

#endif //__XFINDFIL_H
