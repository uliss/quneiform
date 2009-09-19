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

// Windows Template Library - WTL version 8.0
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// This file is a part of the Windows Template Library.
// The use and distribution terms for this software are covered by the
// Microsoft Permissive License (Ms-PL) which can be found in the file
// Ms-PL.txt at the root of this distribution.

#ifndef __ATLUSER_H__
#define __ATLUSER_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atluser.h requires atlapp.h to be included first
#endif


///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CMenuItemInfo
// CMenuT<t_bManaged>
// CAcceleratorT<t_bManaged>
// CIconT<t_bManaged>
// CCursorT<t_bManaged>
// CResource
//
// Global functions:
//   AtlMessageBox()


namespace WTL
{

///////////////////////////////////////////////////////////////////////////////
// AtlMessageBox - accepts both memory and resource based strings

inline int AtlMessageBox(HWND hWndOwner, ATL::_U_STRINGorID message, ATL::_U_STRINGorID title = (const char *)NULL, uint uType = MB_OK | MB_ICONINFORMATION)
{
	ATLASSERT(hWndOwner == NULL || ::IsWindow(hWndOwner));

	char* lpstrMessage = NULL;
	if(IS_INTRESOURCE(message.m_lpstr))
	{
		for(int nLen = 256; ; nLen *= 2)
		{
			ATLTRY(lpstrMessage = new TCHAR[nLen]);
			if(lpstrMessage == NULL)
			{
				ATLASSERT(FALSE);
				return 0;
			}
			int nRes = ::LoadString(ModuleHelper::GetResourceInstance(), LOWORD(message.m_lpstr), lpstrMessage, nLen);
			if(nRes < nLen - 1)
				break;
			delete [] lpstrMessage;
			lpstrMessage = NULL;
		}

		message.m_lpstr = lpstrMessage;
	}

	char* lpstrTitle = NULL;
	if(IS_INTRESOURCE(title.m_lpstr) && LOWORD(title.m_lpstr) != 0)
	{
		for(int nLen = 256; ; nLen *= 2)
		{
			ATLTRY(lpstrTitle = new TCHAR[nLen]);
			if(lpstrTitle == NULL)
			{
				ATLASSERT(FALSE);
				return 0;
			}
			int nRes = ::LoadString(ModuleHelper::GetResourceInstance(), LOWORD(title.m_lpstr), lpstrTitle, nLen);
			if(nRes < nLen - 1)
				break;
			delete [] lpstrTitle;
			lpstrTitle = NULL;
		}

		title.m_lpstr = lpstrTitle;
	}

	int nRet = ::MessageBox(hWndOwner, message.m_lpstr, title.m_lpstr, uType);

	delete [] lpstrMessage;
	delete [] lpstrTitle;

	return nRet;
}


///////////////////////////////////////////////////////////////////////////////
// CMenu

#if (WINVER >= 0x0500)
  #ifndef MII_SIZEOF_STRUCT
    #define MII_SIZEOF_STRUCT(structname, member)  (((int)((LPBYTE)(&((structname*)0)->member) - ((LPBYTE)((structname*)0)))) + sizeof(((structname*)0)->member))
  #endif
  #define MENUITEMINFO_SIZE_VERSION_400A  MII_SIZEOF_STRUCT(MENUITEMINFOA, cch)
  #define MENUITEMINFO_SIZE_VERSION_400W  MII_SIZEOF_STRUCT(MENUITEMINFOW, cch)
  #ifdef UNICODE
    #define MENUITEMINFO_SIZE_VERSION_400  MENUITEMINFO_SIZE_VERSION_400W
  #else
    #define MENUITEMINFO_SIZE_VERSION_400  MENUITEMINFO_SIZE_VERSION_400A
  #endif // !UNICODE
#endif // (WINVER >= 0x0500)

class CMenuItemInfo : public MENUITEMINFO
{
public:
	CMenuItemInfo()
	{
		memset(this, 0, sizeof(MENUITEMINFO));
		cbSize = sizeof(MENUITEMINFO);
#if (WINVER >= 0x0500)
		// adjust struct size if running on older version of Windows
		if(AtlIsOldWindows())
		{
			ATLASSERT(cbSize > MENUITEMINFO_SIZE_VERSION_400);   // must be
			cbSize = MENUITEMINFO_SIZE_VERSION_400;
		}
#endif // (WINVER >= 0x0500)
	}
};


// forward declarations
template <bool t_bManaged> class CMenuT;
typedef CMenuT<false>   CMenuHandle;
typedef CMenuT<true>    CMenu;


template <bool t_bManaged>
class CMenuT
{
public:
// Data members
	HMENU m_hMenu;

// Constructor/destructor/operators
	CMenuT(HMENU hMenu = NULL) : m_hMenu(hMenu)
	{ }

	~CMenuT()
	{
		if(t_bManaged && m_hMenu != NULL)
			DestroyMenu();
	}

	CMenuT<t_bManaged>& operator =(HMENU hMenu)
	{
		Attach(hMenu);
		return *this;
	}

	void Attach(HMENU hMenuNew)
	{
		ATLASSERT(::IsMenu(hMenuNew));
		if(t_bManaged && m_hMenu != NULL && m_hMenu != hMenuNew)
			::DestroyMenu(m_hMenu);
		m_hMenu = hMenuNew;
	}

	HMENU Detach()
	{
		HMENU hMenu = m_hMenu;
		m_hMenu = NULL;
		return hMenu;
	}

	operator HMENU() const { return m_hMenu; }

	bool IsNull() const { return (m_hMenu == NULL); }

	Bool IsMenu() const
	{
		return ::IsMenu(m_hMenu);
	}

// Create/destroy methods
	Bool CreateMenu()
	{
		ATLASSERT(m_hMenu == NULL);
		m_hMenu = ::CreateMenu();
		return (m_hMenu != NULL) ? TRUE : FALSE;
	}

	Bool CreatePopupMenu()
	{
		ATLASSERT(m_hMenu == NULL);
		m_hMenu = ::CreatePopupMenu();
		return (m_hMenu != NULL) ? TRUE : FALSE;
	}

	Bool LoadMenu(ATL::_U_STRINGorID menu)
	{
		ATLASSERT(m_hMenu == NULL);
		m_hMenu = ::LoadMenu(ModuleHelper::GetResourceInstance(), menu.m_lpstr);
		return (m_hMenu != NULL) ? TRUE : FALSE;
	}

#ifndef _WIN32_WCE
	Bool LoadMenuIndirect(const void* lpMenuTemplate)
	{
		ATLASSERT(m_hMenu == NULL);
		m_hMenu = ::LoadMenuIndirect(lpMenuTemplate);
		return (m_hMenu != NULL) ? TRUE : FALSE;
	}
#endif // !_WIN32_WCE

	Bool DestroyMenu()
	{
		if (m_hMenu == NULL)
			return FALSE;
		Bool bRet = ::DestroyMenu(m_hMenu);
		if(bRet)
			m_hMenu = NULL;
		return bRet;
	}

// Menu Operations
	Bool DeleteMenu(uint nPosition, uint nFlags)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::DeleteMenu(m_hMenu, nPosition, nFlags);
	}

	Bool TrackPopupMenu(uint nFlags, int x, int y, HWND hWnd, LPCRECT lpRect = NULL)
	{
		ATLASSERT(::IsMenu(m_hMenu));
#ifndef _WIN32_WCE
#if (WINVER >= 0x0500)
		x = _FixTrackMenuPopupX(x, y);
#endif // !(WINVER >= 0x0500)
		return ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0, hWnd, lpRect);
#else // CE specific
		lpRect;
		return ::TrackPopupMenuEx(m_hMenu, nFlags, x, y, hWnd, NULL);
#endif // _WIN32_WCE
	}

	Bool TrackPopupMenuEx(uint uFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm = NULL)
	{
		ATLASSERT(::IsMenu(m_hMenu));
#if (WINVER >= 0x0500) && !defined(_WIN32_WCE)
		x = _FixTrackMenuPopupX(x, y);
#endif // (WINVER >= 0x0500) && !defined(_WIN32_WCE)
		return ::TrackPopupMenuEx(m_hMenu, uFlags, x, y, hWnd, lptpm);
	}

#if (WINVER >= 0x0500) && !defined(_WIN32_WCE)
	// helper that fixes popup menu X position when it's off-screen
	static int _FixTrackMenuPopupX(int x, int y)
	{
		POINT pt = { x, y };
		HMONITOR hMonitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONULL);
		if(hMonitor == NULL)
		{
			HMONITOR hMonitorNear = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
			if(hMonitorNear != NULL)
			{
				MONITORINFO mi = { 0 };
				mi.cbSize = sizeof(MONITORINFO);
				if(::GetMonitorInfo(hMonitorNear, &mi) != FALSE)
				{
					if(x < mi.rcWork.left)
						x = mi.rcWork.left;
					else if(x > mi.rcWork.right)
						x = mi.rcWork.right;
				}
			}
		}

		return x;
	}

	Bool GetMenuInfo(LPMENUINFO lpMenuInfo) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::GetMenuInfo(m_hMenu, lpMenuInfo);
	}

	Bool SetMenuInfo(LPCMENUINFO lpMenuInfo)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::SetMenuInfo(m_hMenu, lpMenuInfo);
	}
#endif // (WINVER >= 0x0500) && !defined(_WIN32_WCE)

// Menu Item Operations
	Bool AppendMenu(uint nFlags, UINT_PTR nIDNewItem = 0, const char * lpszNewItem = NULL)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::AppendMenu(m_hMenu, nFlags, nIDNewItem, lpszNewItem);
	}

	Bool AppendMenu(uint nFlags, HMENU hSubMenu, const char * lpszNewItem)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		ATLASSERT(::IsMenu(hSubMenu));
		return ::AppendMenu(m_hMenu, nFlags | MF_POPUP, (UINT_PTR)hSubMenu, lpszNewItem);
	}

#ifndef _WIN32_WCE
	Bool AppendMenu(uint nFlags, UINT_PTR nIDNewItem, HBITMAP hBmp)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::AppendMenu(m_hMenu, nFlags | MF_BITMAP, nIDNewItem, (const char *)hBmp);
	}

	Bool AppendMenu(uint nFlags, HMENU hSubMenu, HBITMAP hBmp)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		ATLASSERT(::IsMenu(hSubMenu));
		return ::AppendMenu(m_hMenu, nFlags | (MF_BITMAP | MF_POPUP), (UINT_PTR)hSubMenu, (const char *)hBmp);
	}
#endif // !_WIN32_WCE

	uint CheckMenuItem(uint nIDCheckItem, uint nCheck)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return (uint)::CheckMenuItem(m_hMenu, nIDCheckItem, nCheck);
	}

	uint EnableMenuItem(uint nIDEnableItem, uint nEnable)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::EnableMenuItem(m_hMenu, nIDEnableItem, nEnable);
	}

#ifndef _WIN32_WCE
	Bool HiliteMenuItem(HWND hWnd, uint uIDHiliteItem, uint uHilite)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::HiliteMenuItem(hWnd, m_hMenu, uIDHiliteItem, uHilite);
	}

	int GetMenuItemCount() const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::GetMenuItemCount(m_hMenu);
	}

	uint GetMenuItemID(int nPos) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::GetMenuItemID(m_hMenu, nPos);
	}

	uint GetMenuState(uint nID, uint nFlags) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::GetMenuState(m_hMenu, nID, nFlags);
	}

	int GetMenuString(uint nIDItem, char* lpString, int nMaxCount, uint nFlags) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::GetMenuString(m_hMenu, nIDItem, lpString, nMaxCount, nFlags);
	}

	int GetMenuStringLen(uint nIDItem, uint nFlags) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::GetMenuString(m_hMenu, nIDItem, NULL, 0, nFlags);
	}

#ifndef _ATL_NO_COM
	Bool GetMenuString(uint nIDItem, BSTR& bstrText, uint nFlags) const
	{
		USES_CONVERSION;
		ATLASSERT(::IsMenu(m_hMenu));
		ATLASSERT(bstrText == NULL);

		int nLen = GetMenuStringLen(nIDItem, nFlags);
		if(nLen == 0)
		{
			bstrText = ::SysAllocString(OLESTR(""));
			return (bstrText != NULL) ? TRUE : FALSE;
		}

		nLen++;   // increment to include terminating NULL char
		CTempBuffer<TCHAR, _WTL_STACK_ALLOC_THRESHOLD> buff;
		char* lpszText = buff.Allocate(nLen);
		if(lpszText == NULL)
			return FALSE;

		if(!GetMenuString(nIDItem, lpszText, nLen, nFlags))
			return FALSE;

		bstrText = ::SysAllocString(T2OLE(lpszText));
		return (bstrText != NULL) ? TRUE : FALSE;
	}
#endif // !_ATL_NO_COM
#endif // !_WIN32_WCE

#if defined(_WTL_USE_CSTRING) || defined(__ATLSTR_H__)
	int GetMenuString(uint nIDItem, _CSTRING_NS::CString& strText, uint nFlags) const
	{
		ATLASSERT(::IsMenu(m_hMenu));

		int nLen = GetMenuStringLen(nIDItem, nFlags);
		if(nLen == 0)
			return 0;

		nLen++;   // increment to include terminating NULL char
		char* lpstr = strText.GetBufferSetLength(nLen);
		if(lpstr == NULL)
			return 0;
		int nRet = GetMenuString(nIDItem, lpstr, nLen, nFlags);
		strText.ReleaseBuffer();
		return nRet;
	}
#endif // defined(_WTL_USE_CSTRING) || defined(__ATLSTR_H__)

	CMenuHandle GetSubMenu(int nPos) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return CMenuHandle(::GetSubMenu(m_hMenu, nPos));
	}

	Bool InsertMenu(uint nPosition, uint nFlags, UINT_PTR nIDNewItem = 0, const char * lpszNewItem = NULL)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::InsertMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
	}

	Bool InsertMenu(uint nPosition, uint nFlags, HMENU hSubMenu, const char * lpszNewItem)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		ATLASSERT(::IsMenu(hSubMenu));
		return ::InsertMenu(m_hMenu, nPosition, nFlags | MF_POPUP, (UINT_PTR)hSubMenu, lpszNewItem);
	}

#ifndef _WIN32_WCE
	Bool InsertMenu(uint nPosition, uint nFlags, UINT_PTR nIDNewItem, HBITMAP hBmp)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::InsertMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem, (const char *)hBmp);
	}

	Bool InsertMenu(uint nPosition, uint nFlags, HMENU hSubMenu, HBITMAP hBmp)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		ATLASSERT(::IsMenu(hSubMenu));
		return ::InsertMenu(m_hMenu, nPosition, nFlags | (MF_BITMAP | MF_POPUP), (UINT_PTR)hSubMenu, (const char *)hBmp);
	}

	Bool ModifyMenu(uint nPosition, uint nFlags, UINT_PTR nIDNewItem = 0, const char * lpszNewItem = NULL)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::ModifyMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
	}

	Bool ModifyMenu(uint nPosition, uint nFlags, HMENU hSubMenu, const char * lpszNewItem)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		ATLASSERT(::IsMenu(hSubMenu));
		return ::ModifyMenu(m_hMenu, nPosition, nFlags | MF_POPUP, (UINT_PTR)hSubMenu, lpszNewItem);
	}

	Bool ModifyMenu(uint nPosition, uint nFlags, UINT_PTR nIDNewItem, HBITMAP hBmp)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::ModifyMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem, (const char *)hBmp);
	}

	Bool ModifyMenu(uint nPosition, uint nFlags, HMENU hSubMenu, HBITMAP hBmp)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		ATLASSERT(::IsMenu(hSubMenu));
		return ::ModifyMenu(m_hMenu, nPosition, nFlags | (MF_BITMAP | MF_POPUP), (UINT_PTR)hSubMenu, (const char *)hBmp);
	}
#endif // !_WIN32_WCE

	Bool RemoveMenu(uint nPosition, uint nFlags)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::RemoveMenu(m_hMenu, nPosition, nFlags);
	}

#ifndef _WIN32_WCE
	Bool SetMenuItemBitmaps(uint nPosition, uint nFlags, HBITMAP hBmpUnchecked, HBITMAP hBmpChecked)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::SetMenuItemBitmaps(m_hMenu, nPosition, nFlags, hBmpUnchecked, hBmpChecked);
	}
#endif // !_WIN32_WCE

	Bool CheckMenuRadioItem(uint nIDFirst, uint nIDLast, uint nIDItem, uint nFlags)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::CheckMenuRadioItem(m_hMenu, nIDFirst, nIDLast, nIDItem, nFlags);
	}

	Bool GetMenuItemInfo(uint uItem, Bool bByPosition, LPMENUITEMINFO lpmii) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return (Bool)::GetMenuItemInfo(m_hMenu, uItem, bByPosition, lpmii);
	}

	Bool SetMenuItemInfo(uint uItem, Bool bByPosition, LPMENUITEMINFO lpmii)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return (Bool)::SetMenuItemInfo(m_hMenu, uItem, bByPosition, lpmii);
	}

#ifndef _WIN32_WCE
	Bool InsertMenuItem(uint uItem, Bool bByPosition, LPMENUITEMINFO lpmii)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return (Bool)::InsertMenuItem(m_hMenu, uItem, bByPosition, lpmii);
	}

	uint GetMenuDefaultItem(Bool bByPosition = FALSE, uint uFlags = 0U) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::GetMenuDefaultItem(m_hMenu, (uint)bByPosition, uFlags);
	}

	Bool SetMenuDefaultItem(uint uItem = (uint)-1,  Bool bByPosition = FALSE)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::SetMenuDefaultItem(m_hMenu, uItem, (uint)bByPosition);
	}

	Bool GetMenuItemRect(HWND hWnd, uint uItem, LPRECT lprcItem) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::GetMenuItemRect(hWnd, m_hMenu, uItem, lprcItem);
	}

	int MenuItemFromPoint(HWND hWnd, POINT point) const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::MenuItemFromPoint(hWnd, m_hMenu, point);
	}

// Context Help Functions
	Bool SetMenuContextHelpId(DWORD dwContextHelpId)
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::SetMenuContextHelpId(m_hMenu, dwContextHelpId);
	}

	DWORD GetMenuContextHelpId() const
	{
		ATLASSERT(::IsMenu(m_hMenu));
		return ::GetMenuContextHelpId(m_hMenu);
	}
#endif // !_WIN32_WCE
};


///////////////////////////////////////////////////////////////////////////////
// CAccelerator

template <bool t_bManaged>
class CAcceleratorT
{
public:
	HACCEL m_hAccel;

// Constructor/destructor/operators
	CAcceleratorT(HACCEL hAccel = NULL) : m_hAccel(hAccel)
	{ }

	~CAcceleratorT()
	{
		if(t_bManaged && m_hAccel != NULL)
			::DestroyAcceleratorTable(m_hAccel);
	}

	CAcceleratorT<t_bManaged>& operator =(HACCEL hAccel)
	{
		Attach(hAccel);
		return *this;
	}

	void Attach(HACCEL hAccel)
	{
		if(t_bManaged && m_hAccel != NULL)
			::DestroyAcceleratorTable(m_hAccel);
		m_hAccel = hAccel;
	}

	HACCEL Detach()
	{
		HACCEL hAccel = m_hAccel;
		m_hAccel = NULL;
		return hAccel;
	}

	operator HACCEL() const { return m_hAccel; }

	bool IsNull() const { return m_hAccel == NULL; }

// Create/destroy methods
	HACCEL LoadAccelerators(ATL::_U_STRINGorID accel)
	{
		ATLASSERT(m_hAccel == NULL);
		m_hAccel = ::LoadAccelerators(ModuleHelper::GetResourceInstance(), accel.m_lpstr);
		return m_hAccel;
	}

	HACCEL CreateAcceleratorTable(LPACCEL pAccel, int cEntries)
	{
		ATLASSERT(m_hAccel == NULL);
		ATLASSERT(pAccel != NULL);
		m_hAccel = ::CreateAcceleratorTable(pAccel, cEntries);
		return m_hAccel;
	}

	void DestroyObject()
	{
		if(m_hAccel != NULL)
		{
			::DestroyAcceleratorTable(m_hAccel);
			m_hAccel = NULL;
		}
	}

// Operations
#ifndef _WIN32_WCE
	int CopyAcceleratorTable(LPACCEL lpAccelDst, int cEntries)
	{
		ATLASSERT(m_hAccel != NULL);
		ATLASSERT(lpAccelDst != NULL);
		return ::CopyAcceleratorTable(m_hAccel, lpAccelDst, cEntries);
	}

	int GetEntriesCount() const
	{
		ATLASSERT(m_hAccel != NULL);
		return ::CopyAcceleratorTable(m_hAccel, NULL, 0);
	}
#endif // !_WIN32_WCE

	Bool TranslateAccelerator(HWND hWnd, LPMSG pMsg)
	{
		ATLASSERT(m_hAccel != NULL);
		ATLASSERT(::IsWindow(hWnd));
		ATLASSERT(pMsg != NULL);
		return ::TranslateAccelerator(hWnd, m_hAccel, pMsg);
	}
};

typedef CAcceleratorT<false>   CAcceleratorHandle;
typedef CAcceleratorT<true>    CAccelerator;


///////////////////////////////////////////////////////////////////////////////
// CIcon

template <bool t_bManaged>
class CIconT
{
public:
	HICON m_hIcon;

// Constructor/destructor/operators
	CIconT(HICON hIcon = NULL) : m_hIcon(hIcon)
	{ }

	~CIconT()
	{
		if(t_bManaged && m_hIcon != NULL)
			::DestroyIcon(m_hIcon);
	}

	CIconT<t_bManaged>& operator =(HICON hIcon)
	{
		Attach(hIcon);
		return *this;
	}

	void Attach(HICON hIcon)
	{
		if(t_bManaged && m_hIcon != NULL)
			::DestroyIcon(m_hIcon);
		m_hIcon = hIcon;
	}

	HICON Detach()
	{
		HICON hIcon = m_hIcon;
		m_hIcon = NULL;
		return hIcon;
	}

	operator HICON() const { return m_hIcon; }

	bool IsNull() const { return m_hIcon == NULL; }

// Create/destroy methods
	HICON LoadIcon(ATL::_U_STRINGorID icon)
	{
		ATLASSERT(m_hIcon == NULL);
		m_hIcon = ::LoadIcon(ModuleHelper::GetResourceInstance(), icon.m_lpstr);
		return m_hIcon;
	}

	HICON LoadIcon(ATL::_U_STRINGorID icon, int cxDesired, int cyDesired, uint fuLoad = 0)
	{
		ATLASSERT(m_hIcon == NULL);
		m_hIcon = (HICON) ::LoadImage(ModuleHelper::GetResourceInstance(), icon.m_lpstr, IMAGE_ICON, cxDesired, cyDesired, fuLoad);
		return m_hIcon;
	}

#ifndef _WIN32_WCE
	HICON LoadOEMIcon(const char * lpstrIconName)
	{
		ATLASSERT(m_hIcon == NULL);
		ATLASSERT(IsOEMIcon(lpstrIconName));
		m_hIcon = ::LoadIcon(NULL, lpstrIconName);
		return m_hIcon;
	}

	HICON CreateIcon(int nWidth, int nHeight, uchar cPlanes, uchar cBitsPixel, CONST uchar* lpbANDbits, CONST uchar *lpbXORbits)
	{
		ATLASSERT(m_hIcon == NULL);
		ATLASSERT(lpbANDbits != NULL);
		ATLASSERT(lpbXORbits != NULL);
		m_hIcon = ::CreateIcon(ModuleHelper::GetResourceInstance(), nWidth, nHeight, cPlanes, cBitsPixel, lpbANDbits, lpbXORbits);
		return m_hIcon;
	}

	HICON CreateIconFromResource(puchar pBits, DWORD dwResSize, DWORD dwVersion = 0x00030000)
	{
		ATLASSERT(m_hIcon == NULL);
		ATLASSERT(pBits != NULL);
		m_hIcon = ::CreateIconFromResource(pBits, dwResSize, TRUE, dwVersion);
		return m_hIcon;
	}

	HICON CreateIconFromResourceEx(puchar pbBits, DWORD cbBits, DWORD dwVersion = 0x00030000, int cxDesired = 0, int cyDesired = 0, uint uFlags = LR_DEFAULTCOLOR)
	{
		ATLASSERT(m_hIcon == NULL);
		ATLASSERT(pbBits != NULL);
		ATLASSERT(cbBits > 0);
		m_hIcon = ::CreateIconFromResourceEx(pbBits, cbBits, TRUE, dwVersion, cxDesired, cyDesired, uFlags);
		return m_hIcon;
	}
#endif // !_WIN32_WCE

	HICON CreateIconIndirect(PICONINFO pIconInfo)
	{
		ATLASSERT(m_hIcon == NULL);
		ATLASSERT(pIconInfo != NULL);
		m_hIcon = ::CreateIconIndirect(pIconInfo);
		return m_hIcon;
	}

#ifndef _WIN32_WCE
	HICON ExtractIcon(const char * lpszExeFileName, uint nIconIndex)
	{
		ATLASSERT(m_hIcon == NULL);
		ATLASSERT(lpszExeFileName != NULL);
		m_hIcon = ::ExtractIcon(ModuleHelper::GetModuleInstance(), lpszExeFileName, nIconIndex);
		return m_hIcon;
	}

	HICON ExtractAssociatedIcon(HINSTANCE hInst, char* lpIconPath, LPWORD lpiIcon)
	{
		ATLASSERT(m_hIcon == NULL);
		ATLASSERT(lpIconPath != NULL);
		ATLASSERT(lpiIcon != NULL);
		m_hIcon = ::ExtractAssociatedIcon(hInst, lpIconPath, lpiIcon);
		return m_hIcon;
	}
#endif // !_WIN32_WCE

	Bool DestroyIcon()
	{
		ATLASSERT(m_hIcon != NULL);
		Bool bRet = ::DestroyIcon(m_hIcon);
		if(bRet != FALSE)
			m_hIcon = NULL;
		return bRet;
	}

// Operations
#ifndef _WIN32_WCE
	HICON CopyIcon()
	{
		ATLASSERT(m_hIcon != NULL);
		return ::CopyIcon(m_hIcon);
	}

	HICON DuplicateIcon()
	{
		ATLASSERT(m_hIcon != NULL);
		return ::DuplicateIcon(NULL, m_hIcon);
	}
#endif // !_WIN32_WCE

	Bool DrawIcon(HDC hDC, int x, int y)
	{
		ATLASSERT(m_hIcon != NULL);
#ifndef _WIN32_WCE
		return ::DrawIcon(hDC, x, y, m_hIcon);
#else // CE specific
		return ::DrawIconEx(hDC, x, y, m_hIcon, 0, 0, 0, NULL, DI_NORMAL);
#endif // _WIN32_WCE
	}

	Bool DrawIcon(HDC hDC, POINT pt)
	{
		ATLASSERT(m_hIcon != NULL);
#ifndef _WIN32_WCE
		return ::DrawIcon(hDC, pt.x, pt.y, m_hIcon);
#else // CE specific
		return ::DrawIconEx(hDC, pt.x, pt.y, m_hIcon, 0, 0, 0, NULL, DI_NORMAL);
#endif // _WIN32_WCE
	}

	Bool DrawIconEx(HDC hDC, int x, int y, int cxWidth, int cyWidth, uint uStepIfAniCur = 0, HBRUSH hbrFlickerFreeDraw = NULL, uint uFlags = DI_NORMAL)
	{
		ATLASSERT(m_hIcon != NULL);
		return ::DrawIconEx(hDC, x, y, m_hIcon, cxWidth, cyWidth, uStepIfAniCur, hbrFlickerFreeDraw, uFlags);
	}

	Bool DrawIconEx(HDC hDC, POINT pt, SIZE size, uint uStepIfAniCur = 0, HBRUSH hbrFlickerFreeDraw = NULL, uint uFlags = DI_NORMAL)
	{
		ATLASSERT(m_hIcon != NULL);
		return ::DrawIconEx(hDC, pt.x, pt.y, m_hIcon, size.cx, size.cy, uStepIfAniCur, hbrFlickerFreeDraw, uFlags);
	}

#ifndef _WIN32_WCE
	Bool GetIconInfo(PICONINFO pIconInfo) const
	{
		ATLASSERT(m_hIcon != NULL);
		ATLASSERT(pIconInfo != NULL);
		return ::GetIconInfo(m_hIcon, pIconInfo);
	}

#if (_WIN32_WINNT >= 0x0600)
	Bool GetIconInfoEx(PICONINFOEX pIconInfo) const
	{
		ATLASSERT(m_hIcon != NULL);
		ATLASSERT(pIconInfo != NULL);
		return ::GetIconInfoEx(m_hIcon, pIconInfo);
	}
#endif // (_WIN32_WINNT >= 0x0600)

#if defined(NTDDI_VERSION) && (NTDDI_VERSION >= NTDDI_LONGHORN)
	HRESULT LoadIconMetric(ATL::_U_STRINGorID icon, int lims)
	{
		ATLASSERT(m_hIcon == NULL);
		USES_CONVERSION;
		return ::LoadIconMetric(ModuleHelper::GetResourceInstance(), T2CW(icon.m_lpstr), lims, &m_hIcon);
	}

	HRESULT LoadIconWithScaleDown(ATL::_U_STRINGorID icon, int cx, int cy)
	{
		ATLASSERT(m_hIcon == NULL);
		USES_CONVERSION;
		return ::LoadIconWithScaleDown(ModuleHelper::GetResourceInstance(), T2CW(icon.m_lpstr), cx, cy, &m_hIcon);
	}

	HRESULT LoadOEMIconMetric(const char * lpstrIconName, int lims)
	{
		ATLASSERT(m_hIcon == NULL);
		ATLASSERT(IsOEMIcon(lpstrIconName));
		return ::LoadIconMetric(NULL, (LPCWSTR)lpstrIconName, lims, &m_hIcon);
	}

	HRESULT LoadOEMIconWithScaleDown(const char * lpstrIconName, int cx, int cy)
	{
		ATLASSERT(m_hIcon == NULL);
		ATLASSERT(IsOEMIcon(lpstrIconName));
		USES_CONVERSION;
		return ::LoadIconWithScaleDown(NULL, (LPCWSTR)lpstrIconName, cx, cy, &m_hIcon);
	}
#endif // defined(NTDDI_VERSION) && (NTDDI_VERSION >= NTDDI_LONGHORN)
#endif // !_WIN32_WCE

	// Helper
#ifndef _WIN32_WCE
	static bool IsOEMIcon(const char * lpstrIconName)
	{
#if (WINVER >= 0x0600)
		return (lpstrIconName == IDI_APPLICATION || lpstrIconName == IDI_ASTERISK || lpstrIconName == IDI_EXCLAMATION ||
		          lpstrIconName == IDI_HAND || lpstrIconName == IDI_QUESTION || lpstrIconName == IDI_WINLOGO ||
		          lpstrIconName == IDI_SHIELD);
#else // !(WINVER >= 0x0600)
		return (lpstrIconName == IDI_APPLICATION || lpstrIconName == IDI_ASTERISK || lpstrIconName == IDI_EXCLAMATION ||
		          lpstrIconName == IDI_HAND || lpstrIconName == IDI_QUESTION || lpstrIconName == IDI_WINLOGO);
#endif // !(WINVER >= 0x0600)
	}
#endif // !_WIN32_WCE
};

typedef CIconT<false>   CIconHandle;
typedef CIconT<true>    CIcon;


///////////////////////////////////////////////////////////////////////////////
// CCursor

// protect template member from a winuser.h macro
#ifdef CopyCursor
  #undef CopyCursor
#endif

template <bool t_bManaged>
class CCursorT
{
public:
	HCURSOR m_hCursor;

// Constructor/destructor/operators
	CCursorT(HCURSOR hCursor = NULL) : m_hCursor(hCursor)
	{ }

	~CCursorT()
	{
		if(t_bManaged && m_hCursor != NULL)
			DestroyCursor();
	}

	CCursorT<t_bManaged>& operator =(HCURSOR hCursor)
	{
		Attach(hCursor);
		return *this;
	}

	void Attach(HCURSOR hCursor)
	{
		if(t_bManaged && m_hCursor != NULL)
			DestroyCursor();
		m_hCursor = hCursor;
	}

	HCURSOR Detach()
	{
		HCURSOR hCursor = m_hCursor;
		m_hCursor = NULL;
		return hCursor;
	}

	operator HCURSOR() const { return m_hCursor; }

	bool IsNull() const { return m_hCursor == NULL; }

// Create/destroy methods
	HCURSOR LoadCursor(ATL::_U_STRINGorID cursor)
	{
		ATLASSERT(m_hCursor == NULL);
		m_hCursor = ::LoadCursor(ModuleHelper::GetResourceInstance(), cursor.m_lpstr);
		return m_hCursor;
	}

	HCURSOR LoadSysCursor(const char * lpstrCursorName)
	{
		ATLASSERT(m_hCursor == NULL);
#if (WINVER >= 0x0500)
		ATLASSERT(lpstrCursorName == IDC_ARROW || lpstrCursorName == IDC_IBEAM || lpstrCursorName == IDC_WAIT ||
			lpstrCursorName == IDC_CROSS || lpstrCursorName == IDC_UPARROW || lpstrCursorName == IDC_SIZE ||
			lpstrCursorName == IDC_ICON || lpstrCursorName == IDC_SIZENWSE || lpstrCursorName == IDC_SIZENESW ||
			lpstrCursorName == IDC_SIZEWE || lpstrCursorName == IDC_SIZENS || lpstrCursorName == IDC_SIZEALL ||
			lpstrCursorName == IDC_NO || lpstrCursorName == IDC_APPSTARTING || lpstrCursorName == IDC_HELP ||
			lpstrCursorName == IDC_HAND);
#else // !(WINVER >= 0x0500)
		ATLASSERT(lpstrCursorName == IDC_ARROW || lpstrCursorName == IDC_IBEAM || lpstrCursorName == IDC_WAIT ||
			lpstrCursorName == IDC_CROSS || lpstrCursorName == IDC_UPARROW || lpstrCursorName == IDC_SIZE ||
			lpstrCursorName == IDC_ICON || lpstrCursorName == IDC_SIZENWSE || lpstrCursorName == IDC_SIZENESW ||
			lpstrCursorName == IDC_SIZEWE || lpstrCursorName == IDC_SIZENS || lpstrCursorName == IDC_SIZEALL ||
			lpstrCursorName == IDC_NO || lpstrCursorName == IDC_APPSTARTING || lpstrCursorName == IDC_HELP);
#endif // !(WINVER >= 0x0500)
		m_hCursor = ::LoadCursor(NULL, lpstrCursorName);
		return m_hCursor;
	}

	// deprecated
	HCURSOR LoadOEMCursor(const char * lpstrCursorName)
	{
		return LoadSysCursor(lpstrCursorName);
	}

	HCURSOR LoadCursor(ATL::_U_STRINGorID cursor, int cxDesired, int cyDesired, uint fuLoad = 0)
	{
		ATLASSERT(m_hCursor == NULL);
		m_hCursor = (HCURSOR) ::LoadImage(ModuleHelper::GetResourceInstance(), cursor.m_lpstr, IMAGE_CURSOR, cxDesired, cyDesired, fuLoad);
		return m_hCursor;
	}

#ifndef _WIN32_WCE
	HCURSOR LoadCursorFromFile(const char * pstrFilename)
	{
		ATLASSERT(m_hCursor == NULL);
		ATLASSERT(pstrFilename != NULL);
		m_hCursor = ::LoadCursorFromFile(pstrFilename);
		return m_hCursor;
	}
#endif // !_WIN32_WCE

#if !defined(_WIN32_WCE) || ((_WIN32_WCE >= 0x400) && !(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)))
	HCURSOR CreateCursor(int xHotSpot, int yHotSpot, int nWidth, int nHeight, CONST VOID *pvANDPlane, CONST VOID *pvXORPlane)
	{
		ATLASSERT(m_hCursor == NULL);
		m_hCursor = ::CreateCursor(ModuleHelper::GetResourceInstance(), xHotSpot, yHotSpot, nWidth, nHeight, pvANDPlane, pvXORPlane);
		return m_hCursor;
	}
#endif // !defined(_WIN32_WCE) || ((_WIN32_WCE >= 0x400) && !(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)))

#ifndef _WIN32_WCE
	HCURSOR CreateCursorFromResource(puchar pBits, DWORD dwResSize, DWORD dwVersion = 0x00030000)
	{
		ATLASSERT(m_hCursor == NULL);
		ATLASSERT(pBits != NULL);
		m_hCursor = (HCURSOR)::CreateIconFromResource(pBits, dwResSize, FALSE, dwVersion);
		return m_hCursor;
	}

	HCURSOR CreateCursorFromResourceEx(puchar pbBits, DWORD cbBits, DWORD dwVersion = 0x00030000, int cxDesired = 0, int cyDesired = 0, uint uFlags = LR_DEFAULTCOLOR)
	{
		ATLASSERT(m_hCursor == NULL);
		ATLASSERT(pbBits != NULL);
		ATLASSERT(cbBits > 0);
		m_hCursor = (HCURSOR)::CreateIconFromResourceEx(pbBits, cbBits, FALSE, dwVersion, cxDesired, cyDesired, uFlags);
		return m_hCursor;
	}
#endif // !_WIN32_WCE

	Bool DestroyCursor()
	{
		ATLASSERT(m_hCursor != NULL);
#if !defined(_WIN32_WCE) || ((_WIN32_WCE >= 0x400) && !(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)))
		Bool bRet = ::DestroyCursor(m_hCursor);
		if(bRet != FALSE)
			m_hCursor = NULL;
		return bRet;
#else // !(!defined(_WIN32_WCE) || ((_WIN32_WCE >= 0x400) && !(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))))
		ATLTRACE2(atlTraceUI, 0, _T("Warning: This version of Windows CE does not have ::DestroyCursor()\n"));
		return FALSE;
#endif // !(!defined(_WIN32_WCE) || ((_WIN32_WCE >= 0x400) && !(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))))
	}

// Operations
#ifndef _WIN32_WCE
	HCURSOR CopyCursor()
	{
		ATLASSERT(m_hCursor != NULL);
		return (HCURSOR)::CopyIcon((HICON)m_hCursor);
	}
#endif // !_WIN32_WCE

#if (WINVER >= 0x0500) && !defined(_WIN32_WCE)
	Bool GetCursorInfo(LPCURSORINFO pCursorInfo)
	{
		ATLASSERT(m_hCursor != NULL);
		ATLASSERT(pCursorInfo != NULL);
		return ::GetCursorInfo(pCursorInfo);
	}
#endif // (WINVER >= 0x0500) && !defined(_WIN32_WCE)
};

typedef CCursorT<false>   CCursorHandle;
typedef CCursorT<true>    CCursor;


///////////////////////////////////////////////////////////////////////////////
// CResource - Wraps a generic Windows resource.
//             Use it with custom resource types other than the
//             standard RT_CURSOR, RT_BITMAP, etc.

class CResource
{
public:
	HGLOBAL m_hGlobal;
	HRSRC m_hResource;

// Constructor/destructor
	CResource() : m_hGlobal(NULL), m_hResource(NULL)
	{ }

	~CResource()
	{
		Release();
	}

// Load methods
	bool Load(ATL::_U_STRINGorID Type, ATL::_U_STRINGorID ID)
	{
		ATLASSERT(m_hResource == NULL);
		ATLASSERT(m_hGlobal == NULL);

		m_hResource = ::FindResource(ModuleHelper::GetResourceInstance(), ID.m_lpstr, Type.m_lpstr);
		if(m_hResource == NULL)
			return false;

		m_hGlobal = ::LoadResource(ModuleHelper::GetResourceInstance(), m_hResource);
		if(m_hGlobal == NULL)
		{
			m_hResource = NULL;
			return false;
		}

		return true;
	}

#ifndef _WIN32_WCE
	bool LoadEx(ATL::_U_STRINGorID Type, ATL::_U_STRINGorID ID, uint16_t wLanguage)
	{
		ATLASSERT(m_hResource == NULL);
		ATLASSERT(m_hGlobal == NULL);

		m_hResource = ::FindResourceEx(ModuleHelper::GetResourceInstance(), ID.m_lpstr, Type.m_lpstr, wLanguage);
		if(m_hResource == NULL)
			return false;

		m_hGlobal = ::LoadResource(ModuleHelper::GetResourceInstance(), m_hResource);
		if(m_hGlobal == NULL)
		{
			m_hResource = NULL;
			return false;
		}

		return true;
	}
#endif // !_WIN32_WCE

// Misc. operations
	DWORD GetSize() const
	{
		ATLASSERT(m_hResource != NULL);
		return ::SizeofResource(ModuleHelper::GetResourceInstance(), m_hResource);
	}

	pvoid Lock()
	{
		ATLASSERT(m_hResource != NULL);
		ATLASSERT(m_hGlobal != NULL);
		pvoid pVoid = ::LockResource(m_hGlobal);
		ATLASSERT(pVoid != NULL);
		return pVoid;
	}

	void Release()
	{
		if(m_hGlobal != NULL)
		{
			FreeResource(m_hGlobal);
			m_hGlobal = NULL;
			m_hResource = NULL;
		}
	}
};

}; // namespace WTL

#endif // __ATLUSER_H__
