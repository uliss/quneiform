#if !defined(AFX_EVENTS_H__B27F3D4C_AB44_488E_AD13_CC16BC09105A__INCLUDED_)
#define AFX_EVENTS_H__B27F3D4C_AB44_488E_AD13_CC16BC09105A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Events.h : header file
//
#include "sampleDlg.h"


/////////////////////////////////////////////////////////////////////////////
// Events command target

class Events : public CCmdTarget
{
	DECLARE_DYNCREATE(Events)

	Events(CSampleDlg    *dlg=NULL);           // protected constructor used by dynamic creation
	virtual ~Events();

// Attributes
public:

// Operations
public:
    CSampleDlg    *m_dlg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Events)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Events)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(Events)
	afx_msg Bool Start();
	afx_msg Bool Stop();
	afx_msg Bool Step(long lStep, const char * strName, long lPerc);
	afx_msg long EndThread(Bool rc, long lContext);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTS_H__B27F3D4C_AB44_488E_AD13_CC16BC09105A__INCLUDED_)
