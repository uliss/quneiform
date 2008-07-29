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
//  Module: maindlg.h
//
//    Desc: Main crash report dialog, responsible for gathering additional
//          user information and allowing user to examine crash report.
//
// Copyright (c) 2003 Michael Carruth
//
// Modified 27.12.2007 by Redart, Cognitive Tech
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAINDLG_H_
#define _MAINDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Utility.h"
#include "DeadLink.h"
#include "detaildlg.h"
#include "aboutdlg.h"

//
// RTF load callback
//
DWORD CALLBACK LoadRTFString(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CString *sText = (CString*)dwCookie;
   LONG lLen = sText->GetLength();

   for (*pcb = 0; *pcb < cb && *pcb < lLen; (*pcb)++)
   {
      pbBuff[*pcb] = sText->GetAt(*pcb);
   }

   return 0;
}


////////////////////////////// Class Definitions /////////////////////////////

// ===========================================================================
// CMainDlg
//
// See the module comment at top of file.
//
class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

   CString     m_sEmail;         // Email: From
   CString     m_sDescription;   // Email: Body
   CDeadLink   m_link;           // Dead link
   TStrStrMap  *m_pUDFiles;      // Files <name,desc>

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_ID_HANDLER(IDC_LINK, OnLinkClick)
      MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		COMMAND_ID_HANDLER(IDOK, OnSend)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()


   //-----------------------------------------------------------------------------
   // CMainDlg
   //
   // Loads RichEditCtrl library
   //
   CMainDlg()
   {
      LoadLibrary(CRichEditCtrl::GetLibraryName());
   };


   //-----------------------------------------------------------------------------
   // OnInitDialog
   //
   //
   //
   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

	   // Add "About..." menu item to system menu.
/*
	   // IDM_ABOUTBOX must be in the system command range.
      ATLASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
      ATLASSERT(IDM_ABOUTBOX < 0xF000);

      CMenu sysMenu;
      sysMenu.Attach(GetSystemMenu(FALSE));
      if (sysMenu.IsMenu())
      {
		   CString strAboutMenu;
		   strAboutMenu.LoadString(IDS_ABOUTBOX);
		   if (!strAboutMenu.IsEmpty())
		   {
            sysMenu.AppendMenu(MF_SEPARATOR);
			   sysMenu.AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		   }
	   }
*/    //
      // Set title using app name
      //
      SetWindowText(CUtility::getAppName());

      //
      // Use app icon
      //
      CStatic icon;
      icon.Attach(GetDlgItem(IDI_APPICON));
      icon.SetIcon(::LoadIcon((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)));
      icon.Detach();

      //
      // Set failure heading
      //
      EDITSTREAM es;
      es.pfnCallback = LoadRTFString;

      CString sText;
      sText.Format(IDS_HEADER, CUtility::getAppName());
      es.dwCookie = (DWORD)&sText;

      CRichEditCtrl re;
      re.Attach(GetDlgItem(IDC_HEADING_TEXT));
      re.StreamIn(SF_RTF, es);
      re.Detach();

      //
      // Hook dead link
      //
      m_link.SubclassWindow(GetDlgItem(IDC_LINK));

      return TRUE;
	}


   //-----------------------------------------------------------------------------
   // OnLinkClick
   //
   // Display details dialog instead of opening URL
   //
   LRESULT OnLinkClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      CDetailDlg dlg;
      dlg.m_pUDFiles = m_pUDFiles;
      dlg.DoModal();
      return 0;
   }

   //-----------------------------------------------------------------------------
   // OnSysCommand
   //
   //
   //
   LRESULT OnSysCommand(UINT, WPARAM wParam, LPARAM , BOOL& bHandled)
   {
      bHandled = FALSE;

      if ((wParam & 0xFFF0) == IDM_ABOUTBOX)
      {
         CAboutDlg dlg;
         dlg.DoModal();
         bHandled = TRUE;
      }

      return 0;
   }


   //-----------------------------------------------------------------------------
   // OnSend
   //
   // Send handler, validates email address entered, if one, and returns.
   //
   LRESULT OnSend(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
      HWND     hWndEmail = GetDlgItem(IDC_EMAIL);
      HWND     hWndDesc = GetDlgItem(IDC_DESCRIPTION);
	   int      nEmailLen = ::GetWindowTextLength(hWndEmail);
      int      nDescLen = ::GetWindowTextLength(hWndDesc);

      LPTSTR lpStr = m_sEmail.GetBufferSetLength(nEmailLen);
      ::GetWindowText(hWndEmail, lpStr, nEmailLen);
      m_sEmail.ReleaseBuffer();

      lpStr = m_sDescription.GetBufferSetLength(nDescLen);
      ::GetWindowText(hWndDesc, lpStr, nDescLen);
      m_sDescription.ReleaseBuffer();

      //
      // If an email address was entered, verify that
      // it [1] contains a @ and [2] the last . comes
      // after the @.
      //
      if (m_sEmail.GetLength() &&
          (m_sEmail.Find(_T('@')) < 0 ||
           m_sEmail.ReverseFind(_T('.')) < m_sEmail.Find(_T('@'))))
      {
         // alert user
         TCHAR szBuf[256];
		   ::LoadString(_Module.GetResourceInstance(), IDS_INVALID_EMAIL, szBuf, 255);
         MessageBox(szBuf, CUtility::getAppName(), MB_OK);
         // select email
         ::SetFocus(hWndEmail);
      }
      else
      {
         EndDialog(wID);
      }

      return 0;
   }

   //-----------------------------------------------------------------------------
   // OnCancel
   //
   //
   //
   LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
      EndDialog(wID);
		return 0;
   }

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif	// #ifndef _MAINDLG_H_
