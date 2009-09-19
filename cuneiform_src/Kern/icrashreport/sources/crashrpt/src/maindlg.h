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
uint32_t CALLBACK LoadRTFString(DWORD dwCookie, LPBYTE pbBuff, int32_t cb, int32_t *pcb)
{
   CString *sText = (CString*)dwCookie;
   int32_t lLen = sText->GetLength();

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
   LRESULT OnInitDialog(uint /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, Bool& /*bHandled*/)
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
   LRESULT OnLinkClick(uint16_t /*wNotifyCode*/, uint16_t wID, HWND /*hWndCtl*/, Bool& /*bHandled*/)
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
   LRESULT OnSysCommand(uint, WPARAM wParam, LPARAM , Bool& bHandled)
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
   LRESULT OnSend(uint16_t /*wNotifyCode*/, uint16_t wID, HWND /*hWndCtl*/, Bool& /*bHandled*/)
	{
      HWND     hWndEmail = GetDlgItem(IDC_EMAIL);
      HWND     hWndDesc = GetDlgItem(IDC_DESCRIPTION);
	   int      nEmailLen = ::GetWindowTextLength(hWndEmail);
      int      nDescLen = ::GetWindowTextLength(hWndDesc);

      char* lpStr = m_sEmail.GetBufferSetLength(nEmailLen);
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
   LRESULT OnCancel(uint16_t /*wNotifyCode*/, uint16_t wID, HWND /*hWndCtl*/, Bool& /*bHandled*/)
	{
      EndDialog(wID);
		return 0;
   }

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif	// #ifndef _MAINDLG_H_
