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

///////////////////////////////////////////////////////////////////////////////
//
//  Module: MailMsg.cpp
//
//    Desc: See MailMsg.h
//
// Copyright (c) 2003 Michael Carruth
//
// Modified 27.12.2007 by Redart, Cognitive Tech
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MailMsg.h"

CMailMsg::CMailMsg()
{
   m_sSubject        = _T("");
   m_sMessage        = _T("");
   m_hMapi           = NULL;
   m_lpCmcLogon      = NULL;
   m_lpCmcSend       = NULL;
   m_lpCmcLogoff     = NULL;
   m_lpMapiLogon     = NULL;
   m_lpMapiSendMail  = NULL;
   m_lpMapiLogoff    = NULL;
   //Признак того, что функция Initialize уже вызывалась
   m_bInited         = FALSE;
   //Признак того, что в системе настроен MAPI
   m_bHasMAPI        = FALSE;
   //Признак того, что в системе настроен CMC
   m_bHasCMC         = FALSE;
}

CMailMsg::~CMailMsg()
{
   if (m_bInited)
      Uninitialize();
}

CMailMsg& CMailMsg::SetFrom(CString sAddress, CString sName)
{
   if (Initialize())
   {
      // only one sender allowed
      if (m_from.size())
         m_from.empty();

      m_from[sAddress] = sName;
   }

   return *this;
}

CMailMsg& CMailMsg::SetTo(CString sAddress, CString sName)
{
   if (Initialize())
   {
      // only one recipient allowed
      if (m_to.size())
         m_to.empty();

      m_to[sAddress] = sName;
   }

   return *this;
}

CMailMsg& CMailMsg::SetCc(CString sAddress, CString sName)
{
   if (Initialize())
   {
      m_cc[sAddress] = sName;
   }

   return *this;
}

CMailMsg& CMailMsg::SetBc(CString sAddress, CString sName)
{
   if (Initialize())
   {
      m_bcc[sAddress] = sName;
   }

   return *this;
}

CMailMsg& CMailMsg::AddAttachment(CString sAttachment, CString sTitle)
{
   if (Initialize())
   {
      m_attachments[sAttachment] = sTitle;
   }

   return *this;
}

Bool CMailMsg::Send()
{
   // try mapi
   if (MAPISend())
      return TRUE;

   // try cmc
   if (CMCSend())
      return TRUE;

   return FALSE;
}

Bool CMailMsg::MAPISend()
{
   TStrStrMap::iterator p;
   int                  nIndex = 0;
   int                  nRecipients = 0;
   MapiRecipDesc*       pRecipients = NULL;
   int                  nAttachments = 0;
   MapiFileDesc*        pAttachments = NULL;
   ulong                status = 0;
   MapiMessage          message;

   if (Initialize() && m_bHasMAPI)
   {
      nRecipients = m_to.size() + m_cc.size() + m_bcc.size() + m_from.size();
      if (nRecipients)
         pRecipients = new MapiRecipDesc[nRecipients];

      nAttachments = m_attachments.size();
      if (nAttachments)
         pAttachments = new MapiFileDesc[nAttachments];

      if (pRecipients)
      {
         if (m_from.size())
         {
            // set from
            pRecipients[nIndex].ulReserved                 = 0;
            pRecipients[nIndex].ulRecipClass               = MAPI_ORIG;
            pRecipients[nIndex].lpszAddress                = (char*)(const char *)m_from.begin()->first;
            pRecipients[nIndex].lpszName                   = (char*)(const char *)m_from.begin()->second;
            pRecipients[nIndex].ulEIDSize                  = 0;
            pRecipients[nIndex].lpEntryID                  = NULL;
            nIndex++;
         }

         if (m_to.size())
         {
            // set to
            pRecipients[nIndex].ulReserved                 = 0;
            pRecipients[nIndex].ulRecipClass               = MAPI_TO;
            pRecipients[nIndex].lpszAddress                = (char*)(const char *)m_to.begin()->first;
            pRecipients[nIndex].lpszName                   = (char*)(const char *)m_to.begin()->second;
            pRecipients[nIndex].ulEIDSize                  = 0;
            pRecipients[nIndex].lpEntryID                  = NULL;
            nIndex++;
         }

         if (m_cc.size())
         {
            // set cc's
            for (p = m_cc.begin(); p != m_cc.end(); p++, nIndex++)
            {
               pRecipients[nIndex].ulReserved         = 0;
               pRecipients[nIndex].ulRecipClass       = MAPI_CC;
               pRecipients[nIndex].lpszAddress        = (char*)(const char *)p->first;
               pRecipients[nIndex].lpszName           = (char*)(const char *)p->second;
               pRecipients[nIndex].ulEIDSize          = 0;
               pRecipients[nIndex].lpEntryID          = NULL;
            }
         }

         if (m_bcc.size())
         {
            // set bcc
            for (p = m_bcc.begin(); p != m_bcc.end(); p++, nIndex++)
            {
               pRecipients[nIndex].ulReserved         = 0;
               pRecipients[nIndex].ulRecipClass       = MAPI_BCC;
               pRecipients[nIndex].lpszAddress        = (char*)(const char *)p->first;
               pRecipients[nIndex].lpszName           = (char*)(const char *)p->second;
               pRecipients[nIndex].ulEIDSize          = 0;
               pRecipients[nIndex].lpEntryID          = NULL;
            }
         }
      }

      if (pAttachments)
      {
         // add attachments
         for (p = m_attachments.begin(), nIndex = 0;
              p != m_attachments.end(); p++, nIndex++)
         {
            pAttachments[nIndex].ulReserved        = 0;
            pAttachments[nIndex].flFlags           = 0;
            pAttachments[nIndex].nPosition         = 0xFFFFFFFF;
            pAttachments[nIndex].lpszPathName      = (char*)(const char *)p->first;
            pAttachments[nIndex].lpszFileName      = (char*)(const char *)p->second;
            pAttachments[nIndex].lpFileType        = NULL;
         }
      }

      message.ulReserved                        = 0;
      message.lpszSubject                       = (char*)(const char *)m_sSubject;
      message.lpszNoteText                      = (char*)(const char *)m_sMessage;
      message.lpszMessageType                   = NULL;
      message.lpszDateReceived                  = NULL;
      message.lpszConversationID                = NULL;
      message.flFlags                           = 0;
      message.lpOriginator                      = m_from.size() ? pRecipients : NULL;
      message.nRecipCount                       = nRecipients - m_from.size(); // don't count originator
      message.lpRecips                          = nRecipients - m_from.size() ? &pRecipients[m_from.size()] : NULL;
      message.nFileCount                        = nAttachments;
      message.lpFiles                           = nAttachments ? pAttachments : NULL;

      status = m_lpMapiSendMail(0, 0, &message, MAPI_DIALOG | MAPI_LOGON_UI, 0);

      if (pRecipients)
         delete [] pRecipients;

      if (nAttachments)
         delete [] pAttachments;
   }

   return (SUCCESS_SUCCESS == status);
}

Bool CMailMsg::CMCSend()
{
   TStrStrMap::iterator p;
   int                  nIndex = 0;
   CMC_recipient*       pRecipients;
   CMC_attachment*      pAttachments;
   CMC_session_id       session;
   CMC_return_code      status = 0;
   CMC_message          message;
   CMC_boolean          bAvailable = FALSE;
   CMC_time             t_now = {0};

   if (Initialize() && m_bHasCMC)
   {
      pRecipients = new CMC_recipient[m_to.size() + m_cc.size() + m_bcc.size() + m_from.size()];
      pAttachments = new CMC_attachment[m_attachments.size()];

      // set cc's
      for (p = m_cc.begin(); p != m_cc.end(); p++, nIndex++)
      {
         pRecipients[nIndex].name                = (char*)(const char *)p->second;
         pRecipients[nIndex].name_type           = CMC_TYPE_INDIVIDUAL;
         pRecipients[nIndex].address             = (char*)(const char *)p->first;
         pRecipients[nIndex].role                = CMC_ROLE_CC;
         pRecipients[nIndex].recip_flags         = 0;
         pRecipients[nIndex].recip_extensions    = NULL;
      }

      // set bcc
      for (p = m_bcc.begin(); p != m_bcc.end(); p++, nIndex++)
      {
         pRecipients[nIndex].name                = (char*)(const char *)p->second;
         pRecipients[nIndex].name_type           = CMC_TYPE_INDIVIDUAL;
         pRecipients[nIndex].address             = (char*)(const char *)p->first;
         pRecipients[nIndex].role                = CMC_ROLE_BCC;
         pRecipients[nIndex].recip_flags         = 0;
         pRecipients[nIndex].recip_extensions    = NULL;
      }

      // set to
      pRecipients[nIndex].name                   = (char*)(const char *)m_to.begin()->second;
      pRecipients[nIndex].name_type              = CMC_TYPE_INDIVIDUAL;
      pRecipients[nIndex].address                = (char*)(const char *)m_to.begin()->first;
      pRecipients[nIndex].role                   = CMC_ROLE_TO;
      pRecipients[nIndex].recip_flags            = 0;
      pRecipients[nIndex].recip_extensions       = NULL;

      // set from
      pRecipients[nIndex+1].name                 = (char*)(const char *)m_from.begin()->second;
      pRecipients[nIndex+1].name_type            = CMC_TYPE_INDIVIDUAL;
      pRecipients[nIndex+1].address              = (char*)(const char *)m_from.begin()->first;
      pRecipients[nIndex+1].role                 = CMC_ROLE_ORIGINATOR;
      pRecipients[nIndex+1].recip_flags          = CMC_RECIP_LAST_ELEMENT;
      pRecipients[nIndex+1].recip_extensions     = NULL;

      // add attachments
      for (p = m_attachments.begin(), nIndex = 0;
           p != m_attachments.end(); p++, nIndex++)
      {
         pAttachments[nIndex].attach_title       = (char*)(const char *)p->second;
         pAttachments[nIndex].attach_type        = NULL;
         pAttachments[nIndex].attach_filename    = (char*)(const char *)p->first;
         pAttachments[nIndex].attach_flags       = 0;
         pAttachments[nIndex].attach_extensions  = NULL;
      }
      pAttachments[nIndex-1].attach_flags        = CMC_ATT_LAST_ELEMENT;

      message.message_reference                 = NULL;
      message.message_type                      = NULL;
      message.subject                           = (char*)(const char *)m_sSubject;
      message.time_sent                         = t_now;
      message.text_note                         = (char*)(const char *)m_sMessage;
      message.recipients                        = pRecipients;
      message.attachments                       = pAttachments;
      message.message_flags                     = 0;
      message.message_extensions                = NULL;

      status = m_lpCmcQueryConfiguration(
                  0,
                  CMC_CONFIG_UI_AVAIL,
                  (void*)&bAvailable,
                  NULL
                  );

      if (CMC_SUCCESS == status && bAvailable)
      {
         status = m_lpCmcLogon(
                     NULL,
                     NULL,
                     NULL,
                     NULL,
                     0,
                     CMC_VERSION,
                     CMC_LOGON_UI_ALLOWED |
                     CMC_ERROR_UI_ALLOWED,
                     &session,
                     NULL
                     );

         if (CMC_SUCCESS == status)
         {
            status = m_lpCmcSend(session, &message, 0, 0, NULL);

            m_lpCmcLogoff(session, NULL, CMC_LOGON_UI_ALLOWED, NULL);
         }
      }

      delete [] pRecipients;
      delete [] pAttachments;
   }

   return ((CMC_SUCCESS == status) && bAvailable);
}

Bool CMailMsg::Initialize()
{
   if(m_bInited)
	   return m_bHasMAPI || m_bHasCMC;
   m_hMapi = ::LoadLibrary(_T("mapi32.dll"));

   if (!m_hMapi)
      return FALSE;

   m_lpCmcQueryConfiguration = (LPCMCQUERY)::GetProcAddress(m_hMapi, _T("cmc_query_configuration"));
   m_lpCmcLogon = (LPCMCLOGON)::GetProcAddress(m_hMapi, _T("cmc_logon"));
   m_lpCmcSend = (LPCMCSEND)::GetProcAddress(m_hMapi, _T("cmc_send"));
   m_lpCmcLogoff = (LPCMCLOGOFF)::GetProcAddress(m_hMapi, _T("cmc_logoff"));
   m_lpMapiLogon = (LPMAPILOGON)::GetProcAddress(m_hMapi, _T("MAPILogon"));
   m_lpMapiSendMail = (LPMAPISENDMAIL)::GetProcAddress(m_hMapi, _T("MAPISendMail"));
   m_lpMapiLogoff = (LPMAPILOGOFF)::GetProcAddress(m_hMapi, _T("MAPILogoff"));

   m_bHasMAPI = m_lpMapiLogon && m_lpMapiSendMail && m_lpMapiLogoff;
   m_bHasCMC = m_lpCmcLogon && m_lpCmcSend && m_lpCmcLogoff;
   m_bInited = true;

   return m_bHasMAPI || m_bHasCMC;
}

void CMailMsg::Uninitialize()
{
   if (m_hMapi)
	::FreeLibrary(m_hMapi);
}
