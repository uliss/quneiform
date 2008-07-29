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
//  Module: detaildlg.h
//
//    Desc: Dialog class used to display and preview the report contents.
//
// Copyright (c) 2003 Michael Carruth
//
// Modified 27.12.2007 by Redart, Cognitive Tech
///////////////////////////////////////////////////////////////////////////////

#ifndef _DETAILDLG_H_
#define _DETAILDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"
#include "aboutdlg.h"

// Defines list control column attributes
LVCOLUMN _ListColumns[] =
{
   /*
   {
      mask,
      fmt,
      cx,
      pszText,
      cchTextMax,
      iSubItem,
      iImage,
      iOrder
   }
   */
   {  // Column 1: File name
      LVCF_FMT | LVCF_ORDER | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,
      LVCFMT_LEFT,
      114,
      (LPTSTR)IDS_NAME,
      0,
      0,
      0,
      0
   },
   {  // Column 2: File description
      LVCF_FMT | LVCF_ORDER | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,
      LVCFMT_LEFT,
      150,
      (LPTSTR)IDS_DESC,
      0,
      1,
      0,
      1
   },
   {  // Column 3: File type
      LVCF_FMT | LVCF_ORDER | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH, LVCFMT_LEFT,
      100,
      (LPTSTR)IDS_TYPE,
      0,
      2,
      0,
      2
   },
   {  // Column 4: File size
      LVCF_FMT | LVCF_ORDER | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,
      LVCFMT_RIGHT,
      100,
      (LPTSTR)IDS_SIZE,
      0,
      3,
      0,
      3
   },
};


////////////////////////////// Class Definitions /////////////////////////////

// ===========================================================================
// CDetailDlg
//
// See the module comment at top of file.
//
class CDetailDlg : public CDialogImpl<CDetailDlg>
{
public:
	enum { IDD = IDD_DETAILDLG };

   TStrStrMap  *m_pUDFiles;      // File <name,desc>
   CImageList  m_iconList;       // Shell icon list

	BEGIN_MSG_MAP(CDetailDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
      MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
      NOTIFY_HANDLER(IDC_FILE_LIST, LVN_ITEMCHANGED, OnItemChanged)
      NOTIFY_HANDLER(IDC_FILE_LIST, NM_DBLCLK, OnItemDblClicked)
		COMMAND_ID_HANDLER(IDOK, OnOK)
      COMMAND_ID_HANDLER(IDCANCEL, OnOK)
	END_MSG_MAP()

   //-----------------------------------------------------------------------------
   // ~CDetailDlg
   //
   //
   //
   ~CDetailDlg()
   {
      // Release shell icon list
      m_iconList.Detach();
   };


   //-----------------------------------------------------------------------------
   // OnInitDialog
   //
   //
   //
   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
      int i = 0;

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
*/
      // center the dialog on the screen
		CenterWindow();

      CListViewCtrl list;
      list.Attach(GetDlgItem(IDC_FILE_LIST));

      // Turn on full row select
      ListView_SetExtendedListViewStyle(list.m_hWnd, LVS_EX_FULLROWSELECT);

      //
      // Attach the system image list to the list control.
      //
      SHFILEINFO sfi = {0};

      HIMAGELIST hil = (HIMAGELIST)SHGetFileInfo(
                                    NULL,
                                    0,
                                    &sfi,
                                    sizeof(sfi),
                                    SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

      if (NULL != hil)
      {
         m_iconList.Attach(hil);
         list.SetImageList(m_iconList, LVSIL_SMALL);
      }

      //
      // Add column headings
      //
      for (i = 0; i < sizeof(_ListColumns) / sizeof(LVCOLUMN); i++)
      {
         list.InsertColumn(
            i,
            CString(_ListColumns[i].pszText),
            _ListColumns[i].fmt,
            _ListColumns[i].cx,
            _ListColumns[i].iSubItem);
      }

      //
      // Insert items
      //
      WIN32_FIND_DATA   findFileData   = {0};
      HANDLE            hFind          = NULL;
      CString           sSize;
      LVITEM            lvi            = {0};
      TStrStrMap::iterator p;
      for (i = 0, p = m_pUDFiles->begin(); p != m_pUDFiles->end(); p++, i++)
      {
         SHGetFileInfo(
            p->first,
            0,
            &sfi,
            sizeof(sfi),
            SHGFI_DISPLAYNAME | SHGFI_ICON | SHGFI_TYPENAME | SHGFI_SMALLICON);

         // Name
         lvi.mask          = LVIF_IMAGE | LVIF_TEXT;
         lvi.iItem         = i;
         lvi.iSubItem      = 0;
         lvi.iImage        = sfi.iIcon;
         lvi.pszText       = sfi.szDisplayName;
         list.InsertItem(&lvi);

         // Description
         list.SetItemText(i, 1, p->second);

         // Type
         list.SetItemText(i, 2, sfi.szTypeName);

         // Size
         hFind = FindFirstFile(p->first, &findFileData);
         if (INVALID_HANDLE_VALUE != hFind)
         {
	         FindClose(hFind);
            sSize.Format(TEXT("%d KB"), findFileData.nFileSizeLow);
            list.SetItemText(i, 3, sSize);
         }
      }

      // Select first file
      ListView_SetItemState(
         GetDlgItem(IDC_FILE_LIST),
         0,
         LVIS_SELECTED,
         LVIS_SELECTED);

      list.Detach();

      return TRUE;
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
   // OnCtlColor
   //
   // Force white background for read-only rich edit control
   //
   LRESULT OnCtlColor(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
   {
      LRESULT res = 0;
      if ((HWND)lParam == GetDlgItem(IDC_FILE_EDIT))
         res = (LRESULT)GetSysColorBrush(COLOR_WINDOW);

      bHandled = TRUE;

      return res;
   }


   //-----------------------------------------------------------------------------
   // OnOK
   //
   //
   //
   LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}


   //-----------------------------------------------------------------------------
   // OnItemChanged
   //
   // Update file preview
   //
   LRESULT OnItemChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      LPNMLISTVIEW lpItem           = (LPNMLISTVIEW)pnmh;
      int iItem                     = lpItem->iItem;

      if (lpItem->uChanged & LVIF_STATE
         && lpItem->uNewState & LVIS_SELECTED)
      {
         SelectItem(iItem);
      }

      return 0;
   }


   //-----------------------------------------------------------------------------
   // OnItemDblClicked
   //
   // Open file in associated application
   //
   LRESULT OnItemDblClicked(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      LPNMLISTVIEW lpItem           = (LPNMLISTVIEW)pnmh;
      int iItem                     = lpItem->iItem;
      DWORD_PTR dwRet               = 0;

      if (iItem < 0 || (int)m_pUDFiles->size() < iItem)
         return 0;

      TStrStrMap::iterator p = m_pUDFiles->begin();
      for (int i = 0; i < iItem; i++, p++);

      dwRet = (DWORD_PTR)::ShellExecute(
                              0,
                              _T("open"),
                              p->first,
                              0,
                              0,
                              SW_SHOWNORMAL
                              );
      ATLASSERT(dwRet > 32);

      return 0;
   }


   //-----------------------------------------------------------------------------
   // SelectItem
   //
   // Does the work of opening the file and displaying it in the preview edit control.
   //
   void SelectItem(int iItem)
   {
      const int MAX_FILE_SIZE          = 32768; // 32k file preview max
      DWORD dwBytesRead                = 0;
      TCHAR buffer[MAX_FILE_SIZE + 1]  = _T("");

      // Sanity check
      if (iItem < 0 || (int)m_pUDFiles->size() < iItem)
          return;

      TStrStrMap::iterator p = m_pUDFiles->begin();
      for (int i = 0; i < iItem; i++, p++);

      //
      // Update preview header info
      //
      ::SetWindowText(GetDlgItem(IDC_NAME), p->first);
      ::SetWindowText(GetDlgItem(IDC_DESCRIPTION), p->second);

      //
      // Display file contents in preview window
      //
      HANDLE hFile = CreateFile(
         p->first,
         GENERIC_READ,
         FILE_SHARE_READ | FILE_SHARE_WRITE,
         NULL,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL,
         0);

      if (NULL != hFile)
      {
         // Read up to first 32 KB
         ReadFile(hFile, buffer, MAX_FILE_SIZE, &dwBytesRead, 0);
         buffer[dwBytesRead] = 0;
         CloseHandle(hFile);
      }

      // Update edit control with file contents
      ::SetWindowText(GetDlgItem(IDC_FILE_EDIT), buffer);
   }
};

#endif	// #ifndef _DETAILDLG_H_
