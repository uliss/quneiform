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

#ifndef _PRT_H_
#define _PRT_H_

#include <string>
#include <list>
#include <vector>
#ifdef __CYGWIN__
#include <stdarg.h>
#endif
#include "stdprt.h"
#include "xstack.h"
#include "xpath.h"
/*#include "wincon.h"
#include "direct.h"*/
#include "time.h"
/*#include "windows.h"*/
#pragma warning (disable:4530)
#pragma warning(disable:4786)
using namespace std;



#define TIMER_DELTA 10
#define CUR_SYS_EVN_COUNT  12
// file
class stdPrtFILE
{
   FILE* hfile;
public:
   operator FILE* (void) const { return hfile; };
   stdPrtFILE() {hfile = NULL;};
   stdPrtFILE(const char* opts)
   {
      hfile = NULL;
      char path_name[_MAX_PATH] = {0};
      ::GetModuleFileName(NULL,path_name,_MAX_PATH);
      if(path_name[0]!=0)
      {
         char dir[_MAX_DIR]={0};
         char name[_MAX_FNAME]={0};
         char ext[_MAX_EXT]={0};
         split_path(path_name, dir,name,ext);
         stdGoToHomeDirectory();
         XPath xFname = "tmp/";
         xFname += name;
         xFname += ".prt";
         hfile=fopen( (char*)xFname, opts );
      };
   };
   stdPrtFILE(const char* name, const char* opts)
   {
      XPath xstdPrtPath;
      xstdPrtPath = stdGetHomeDirectory();
      xstdPrtPath += "/";
      xstdPrtPath +=name;
      hfile=fopen( (char*)(xstdPrtPath), opts );
   };
   ~stdPrtFILE(void)
   {
      if (hfile)
         fclose(hfile);
   };
   void Open(const char* name)
   {
      if(name)
         hfile=fopen( name, "rt" );
   };
};

static stdPrtFILE* theFile;//("a+");
// table
class CTableEvnFiller
{
   char m_szTableName[MAX_PATH];
public:
   vector<StdPrtEvent> xsTblEventData;
   CTableEvnFiller();
   ~CTableEvnFiller();
   bool OpenEvnTable(char *TableName);
};

// console
class stdPrtConsole
{
   mutable HWND hConsoleOutput;
   uint uiMesasageHandle;
   Bool32 bUseConsole;
public:
   //operator HANDLE (void) const { return bUseConsole ? hConsoleOutput = FindWindow( NULL, "PrtConsole") : 0; };
    operator HANDLE (void) const { return NULL;};
   stdPrtConsole() : hConsoleOutput(0) , bUseConsole(0)
   {
      uiMesasageHandle = RegisterWindowMessage("SendTextToPrtConsole");
   };
   ~stdPrtConsole()
   {
      if(hConsoleOutput)
      {
         bUseConsole = FALSE;
         hConsoleOutput = NULL;
      };
   };
   Bool32 AllocPrtConsole();
   void FreePrtConsole()
   {
      bUseConsole = FALSE;
      hConsoleOutput = NULL;
   };
   Bool32 SendTextToConsole(char *text, int len);
};

static stdPrtConsole  stdPrtConsole;

// send to console
class CPrtSendEventToConsole
{
public:
   Bool32 operator()(char* EventText)
   {
      if((HANDLE)(stdPrtConsole) == NULL)
         return TRUE;
      if(!EventText)
         RET_FALSE;
      stdPrtConsole.SendTextToConsole(EventText, strlen(EventText));
      return TRUE;
   };
};
// send to file
class CPrtSendEventToFile
{
public:
   Bool32 operator()(char* EventText)
   {
      if (NULL==(FILE*)(*theFile))
         RET_FALSE;
      if(!EventText)
         RET_FALSE;
      fprintf((FILE*)(*theFile)/*theFile*/,"%s",EventText);
      return TRUE;
   };
};


// send to public
/*#include "windows.h"*/

class CPrtSendEventToPublic
{
   XPath file_name;
   FILE  *file;
   bool bUse;
//   HANDLE  file;
public:
   CPrtSendEventToPublic ()
   {
      char szString[_MAX_PATH] = {0};
      int32_t size = _MAX_PATH;
      bUse = false;
      file = NULL;
      stdGetProfileString(szString,&size,"protocol.ini","Options","Path",STD_SETPROF_DIR_PROJECT);
      if(!szString[0])
          return;
      file_name = szString;
      bUse = true;

      /*char szString[_MAX_PATH] = {0};
      int32_t size = _MAX_PATH;

      stdGetProfileString(szString,&size,"protocol.ini","Options","Path",STD_SETPROF_DIR_PROJECT);
      if(!szString[0])
          return;
      file_name = szString;
      FILE* fl = fopen(szString, "a+");

      //OFSTRUCT of={0};
      file = ::CreateFile(szString,
         GENERIC_READ|GENERIC_WRITE,
         FILE_SHARE_WRITE|FILE_SHARE_READ,
         NULL,
         OPEN_ALWAYS,
         FILE_ATTRIBUTE_NORMAL|FILE_FLAG_RANDOM_ACCESS,
         NULL
         );*/
/*      if (file == INVALID_HANDLE_VALUE)
         stdMessageBox("Open failed","Protocol");
      else
      {
         /*if (n != 0)
         {
            char mess[256]; sprintf(mess, "setvbuf()->%d", n);
            stdMessageBox(mess,"Protocol");
         };
      }*/

   };
   ~CPrtSendEventToPublic()
   {
      if (file)
         fclose(file);
      /*if (file!=INVALID_HANDLE_VALUE)
         ::CloseHandle(file);*/

   };
/*#include "Winbase.h"*/
   Bool32 operator()(char* EventText)
   {
/*      if(!file||file==INVALID_HANDLE_VALUE)
         RET_FALSE;
      if (file!=INVALID_HANDLE_VALUE)
      {*/

      if(!bUse)
         return TRUE;
      XPath spath = file_name;
      spath.SetExt("id");
      HANDLE hnd = CreateFile((char*)spath,GENERIC_READ|GENERIC_WRITE,0,
         NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
      // поставить таймаут
	   while( hnd==INVALID_HANDLE_VALUE )
      {
         hnd = CreateFile(spath,GENERIC_READ|GENERIC_WRITE,0,
            NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
         sleep(30);
       };
      ::CloseHandle(hnd);
      file = fopen((char*)file_name, "at+");

      if (file == NULL)
      {
         stdMessageBox("Open failed","Protocol");
         RET_FALSE;
      }
      else
      {
         int n =  setvbuf(file, NULL, _IONBF, 0);
         if (n != 0)
         {
            char mess[256]; sprintf(mess, "setvbuf()->%d", n);
            stdMessageBox(mess,"Protocol");
         };
      }
      fflush(file);
/*      __int64 pos = 0;
      fgetpos(file,&pos);
      fseek(file,pos,SEEK_END);*/
      int n = fputs(EventText, file);
      fflush(file);
      fclose(file);
      file = NULL;
      while(!unlink(spath.buf))
          sleep(30);
      if (n < 0)
         RET_FALSE;

/*         unsigned long size = strlen(EventText)*sizeof(char);
         unsigned long size_out = 0;
         XPath spath = file_name;*/
/*         spath.SetExt("id");
         HANDLE hnd = CreateFile((char*)spath,GENERIC_READ|GENERIC_WRITE,0,
            NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
         // поставить таймаут
	      while( hnd==INVALID_HANDLE_VALUE )
         {
            hnd = CreateFile(spath,GENERIC_READ|GENERIC_WRITE,0,
               NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
         };
         ::CloseHandle(hnd);
         WriteFile(file,EventText,size,&size_out,NULL);
         _unlink((char*)spath);
         if(size!=size_out)
            RET_FALSE;
      };*/
      return true;
   };
};
// transaction

class CPrtTransactionBuffer
{

   list<string> m_buffer;
   int  m_buffer_len;
   bool FlushToDisk();
   bool AddToBuffer(char* str);
   bool Clear();
   bool m_bStart;
public:
   CPrtTransactionBuffer()  {m_bStart=false;m_buffer_len=0;};
   ~CPrtTransactionBuffer() {m_bStart=false;m_buffer_len=0;};
   bool Start();
   bool Finish();
   bool Rollback();
   bool Add(char* str);
   void FreeBuffer();
};

// Evn Sender
class CPrtEventSender
{
   CPrtSendEventToFile SenderToFile;
   CPrtSendEventToPublic SenderToPublic;
   CPrtSendEventToConsole SenderToConsole;
   Bool32 SendEvent(char* EventText,int32_t EvnType);
   char MessageBuffer[8*1024];
public:
   CPrtEventSender() {memset(MessageBuffer,0,sizeof(MessageBuffer));};
   Bool32 SendEvent(StdPrtEvent* pspe, va_list& List);
};


class CPrtSysEventSender
{
   char buffer[4097];
   CPrtEventSender EventSender;
   Bool32 SendTimerEvent(); // сообщение системного таймера
   Bool32 SendEnvRegistrationEvent(StdPrtEvent* pspe); // сообщение о регистрации
   CTableEvnFiller* m_TableEvnCreator;
public:
   CPrtSysEventSender() {memset(buffer,0,sizeof(buffer));m_TableEvnCreator=NULL;};
   ~CPrtSysEventSender() {if(m_TableEvnCreator){ delete m_TableEvnCreator;m_TableEvnCreator=NULL;}};
   Bool32 SendSysEvent(int32_t SysEvnNo,...); // вызов посылки сообщения
   Bool32 SendSysEvent(int32_t SysEvnNo, va_list& List);
   Bool32 SendEvent(StdPrtEvent* pspe); // посылка необходимых(необходимость определяется по StdPrtEvent) системных сообщений
   void Destroy();
};

class CPrtSendEvent
{
   CPrtSysEventSender   SysEventSender;
   CPrtEventSender      EventSender;
public:
   Bool32 operator()(StdPrtEvent* pspe,va_list& List)
   {
      Bool32 res2 = SysEventSender.SendEvent(pspe);
      Bool32 res1 = EventSender.SendEvent(pspe, List);
      return (res1&&res2);
   };
};


typedef struct tagStdPrtEventData
{
   char FrmtEvnString[256]; // строка содержащая формат вывода события
   int32_t iEvnNo;            // Идентиф. номер события
} StdPrtEventData;



bool ReadFromEvnTable(char* TableName);
Bool32 EvnParseFormat(char* in, char* out);
bool PrtLoad();
void PrtUnload();

#endif
