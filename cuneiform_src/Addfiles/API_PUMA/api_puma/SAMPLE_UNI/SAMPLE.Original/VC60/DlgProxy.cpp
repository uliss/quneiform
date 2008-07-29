// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "sample.h"
#include "DlgProxy.h"
#include "sampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleDlgAutoProxy

IMPLEMENT_DYNCREATE(CSampleDlgAutoProxy, CCmdTarget)

CSampleDlgAutoProxy::CSampleDlgAutoProxy()
{
	EnableAutomation();

	// To keep the application running as long as an automation
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CSampleDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CSampleDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CSampleDlgAutoProxy::~CSampleDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CSampleDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSampleDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CSampleDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSampleDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CSampleDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ISample to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {A164B6A6-ADDC-4FAF-991A-98A608089E41}
static const IID IID_ISample =
{ 0xa164b6a6, 0xaddc, 0x4faf, { 0x99, 0x1a, 0x98, 0xa6, 0x8, 0x8, 0x9e, 0x41 } };

BEGIN_INTERFACE_MAP(CSampleDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CSampleDlgAutoProxy, IID_ISample, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {93F4D24C-C9A7-4911-9209-BBBC9F1BF48C}
IMPLEMENT_OLECREATE2(CSampleDlgAutoProxy, "Sample.Application", 0x93f4d24c, 0xc9a7, 0x4911, 0x92, 0x9, 0xbb, 0xbc, 0x9f, 0x1b, 0xf4, 0x8c)

/////////////////////////////////////////////////////////////////////////////
// CSampleDlgAutoProxy message handlers
