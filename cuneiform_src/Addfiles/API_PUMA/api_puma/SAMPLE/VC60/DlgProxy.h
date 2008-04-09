// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__B6615907_1382_48FE_BCD2_0372DE0E6D87__INCLUDED_)
#define AFX_DLGPROXY_H__B6615907_1382_48FE_BCD2_0372DE0E6D87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSampleDlg;

/////////////////////////////////////////////////////////////////////////////
// CSampleDlgAutoProxy command target

class CSampleDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CSampleDlgAutoProxy)

	CSampleDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CSampleDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSampleDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CSampleDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSampleDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CSampleDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__B6615907_1382_48FE_BCD2_0372DE0E6D87__INCLUDED_)
