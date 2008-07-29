/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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
//  Module: MailMsg.h
//
//    Desc: This class encapsulates the MAPI and CMC mail functions.
//
// Copyright (c) 2003 Michael Carruth
//
// Modified 27.12.2007 by Redart, Cognitive Tech
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAILMSG_H_
#define _MAILMSG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <xcmc.h>          // CMC function defs
#include <mapi.h>          // MAPI function defs

#ifndef TStrStrMap
#include <map>
#include <atlmisc.h>

typedef std::map<CString,CString> TStrStrMap;
#endif // !defined TStrStrMap

//
// Define CMC entry points
//
typedef CMC_return_code (FAR PASCAL *LPCMCLOGON) \
   (CMC_string, CMC_string, CMC_string, CMC_object_identifier, \
   CMC_ui_id, CMC_uint16, CMC_flags, CMC_session_id FAR*, \
   CMC_extension FAR*);

typedef CMC_return_code (FAR PASCAL *LPCMCSEND) \
   (CMC_session_id, CMC_message FAR*, CMC_flags, \
   CMC_ui_id, CMC_extension FAR*);

typedef CMC_return_code (FAR PASCAL *LPCMCLOGOFF) \
   (CMC_session_id, CMC_ui_id, CMC_flags, CMC_extension FAR*);

typedef CMC_return_code (FAR PASCAL *LPCMCQUERY) \
   (CMC_session_id, CMC_enum, CMC_buffer, CMC_extension FAR*);


////////////////////////////// Class Definitions /////////////////////////////

// ===========================================================================
// CMailMsg
//
// See the module comment at top of file.
//
class CMailMsg
{
public:
	CMailMsg();
	virtual ~CMailMsg();

   //-----------------------------------------------------------------------------
   // SetTo
   //    Sets the Email:To address
   //
   // Parameters
   //    sAddress    Address
   //    sName       Optional name
   //
   // Return Values
   //    CMailMsg reference
   //
   // Remarks
   //    Only one To address can be set.  If called more than once
   //    the last address will be used.
   //
   CMailMsg&
   SetTo(
      CString sAddress,
      CString sName = _T("")
      );

   //-----------------------------------------------------------------------------
   // SetCc
   //    Sets the Email:Cc address
   //
   // Parameters
   //    sAddress    Address
   //    sName       Optional name
   //
   // Return Values
   //    CMailMsg reference
   //
   // Remarks
   //    Multiple Cc addresses can be set.
   //
   CMailMsg&
   SetCc(
      CString sAddress,
      CString sName = _T("")
      );

   //-----------------------------------------------------------------------------
   // SetBc
   //    Sets the Email:Bcc address
   //
   // Parameters
   //    sAddress    Address
   //    sName       Optional name
   //
   // Return Values
   //    CMailMsg reference
   //
   // Remarks
   //    Multiple Bcc addresses can be set.
   //
   CMailMsg&
   SetBc(
      CString sAddress,
      CString sName = _T("")
      );

   //-----------------------------------------------------------------------------
   // SetFrom
   //    Sets the Email:From address
   //
   // Parameters
   //    sAddress    Address
   //    sName       Optional name
   //
   // Return Values
   //    CMailMsg reference
   //
   // Remarks
   //    Only one From address can be set.  If called more than once
   //    the last address will be used.
   //
   CMailMsg&
   SetFrom(
      CString sAddress,
      CString sName = _T("")
      );

   //-----------------------------------------------------------------------------
   // SetSubect
   //    Sets the Email:Subject
   //
   // Parameters
   //    sSubject    Subject
   //
   // Return Values
   //    CMailMsg reference
   //
   // Remarks
   //    none
   //
   CMailMsg&
   SetSubject(
      CString sSubject
      ) {m_sSubject = sSubject; return *this;};

   //-----------------------------------------------------------------------------
   // SetMessage
   //    Sets the Email message body
   //
   // Parameters
   //    sMessage    Message body
   //
   // Return Values
   //    CMailMsg reference
   //
   // Remarks
   //    none
   //
   CMailMsg&
   SetMessage(
      CString sMessage
      ) {m_sMessage = sMessage; return *this;};

   //-----------------------------------------------------------------------------
   // AddAttachment
   //    Attaches a file to the email
   //
   // Parameters
   //    sAttachment Fully qualified file name
   //    sTitle      File display name
   //
   // Return Values
   //    CMailMsg reference
   //
   // Remarks
   //    none
   //
   CMailMsg&
   AddAttachment(
      CString sAttachment,
      CString sTitle = _T("")
      );

   //-----------------------------------------------------------------------------
   // Send
   //    Send the email.
   //
   // Parameters
   //    none
   //
   // Return Values
   //    TRUE if succesful
   //
   // Remarks
   //    First simple MAPI is used if unsucessful CMC is used.
   //
   BOOL Send();

protected:

   //-----------------------------------------------------------------------------
   // CMCSend
   //    Send email using CMC functions.
   //
   // Parameters
   //    none
   //
   // Return Values
   //    TRUE if successful
   //
   // Remarks
   //    none
   //
   BOOL CMCSend();

   //-----------------------------------------------------------------------------
   // MAPISend
   //    Send email using MAPI functions.
   //
   // Parameters
   //    none
   //
   // Return Values
   //    TRUE if successful
   //
   // Remarks
   //    none
   //
   BOOL MAPISend();

   //-----------------------------------------------------------------------------
   // Initialize
   //    Initialize MAPI32.dll
   //
   // Parameters
   //    none
   //
   // Return Values
   //    TRUE if successful
   //
   // Remarks
   //    none
   //
   BOOL Initialize();

   //-----------------------------------------------------------------------------
   // Uninitialize
   //    Uninitialize MAPI32.dll
   //
   // Parameters
   //    none
   //
   // Return Values
   //    void
   //
   // Remarks
   //    none
   //
   void Uninitialize();

   TStrStrMap     m_from;                       // From <address,name>
   TStrStrMap     m_to;                         // To <address,name>
   TStrStrMap     m_cc;                         // Cc <address,name>
   TStrStrMap     m_bcc;                        // Bcc <address,name>
   TStrStrMap     m_attachments;                // Attachment <file,title>
   CString        m_sSubject;                   // EMail subject
   CString        m_sMessage;                   // EMail message

   HMODULE        m_hMapi;                      // Handle to MAPI32.DLL
   LPCMCQUERY     m_lpCmcQueryConfiguration;    // Cmc func pointer
   LPCMCLOGON     m_lpCmcLogon;                 // Cmc func pointer
   LPCMCSEND      m_lpCmcSend;                  // Cmc func pointer
   LPCMCLOGOFF    m_lpCmcLogoff;                // Cmc func pointer
   LPMAPILOGON    m_lpMapiLogon;                // Mapi func pointer
   LPMAPISENDMAIL m_lpMapiSendMail;             // Mapi func pointer
   LPMAPILOGOFF   m_lpMapiLogoff;               // Mapi func pointer

   BOOL           m_bInited;                     // MAPI is loaded
   BOOL           m_bHasMAPI;					 //Ïðèçíàê òîãî, ÷òî â ñèñòåìå íàñòðîåí MAPI
   BOOL           m_bHasCMC;					 //Ïðèçíàê òîãî, ÷òî â ñèñòåìå íàñòðîåí CMC
};

#endif	// #ifndef _MAILMSG_H_
