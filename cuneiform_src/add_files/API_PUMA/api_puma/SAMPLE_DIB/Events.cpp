// Events.cpp : implementation file
//
// CT: КЛАСС ДЛЯ ОРГАНИЗАЦИИ ПРОГРЕСС-МОНИТОРА
#include "stdafx.h"
#include "sample.h"
#include "Events.h"
#include "sampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Events

IMPLEMENT_DYNCREATE(Events, CCmdTarget)

Events::Events(CSampleDlg    *dlg)
{
	EnableAutomation();
    if( dlg )
        m_dlg=dlg;
}

Events::~Events()
{
}


void Events::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(Events, CCmdTarget)
	//{{AFX_MSG_MAP(Events)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(Events, CCmdTarget)
	//{{AFX_DISPATCH_MAP(Events)
	DISP_FUNCTION(Events, "Start", Start, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(Events, "Stop", Stop, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(Events, "Step", Step, VT_BOOL, VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION(Events, "EndThread", EndThread, VT_I4, VTS_BOOL VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IEvents to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {367DEC5D-2EED-447F-8290-41FAED27F28C}
const IID DIID__IRecognitionEvents = {0x229C1071,0x829F,0x11D2,{0xBA,0x6E,0x00,0x00,0xE8,0xD9,0xFD,0xF6}};

static const IID IID_IEvents = DIID__IRecognitionEvents;
//{ 0x367dec5d, 0x2eed, 0x447f, { 0x82, 0x90, 0x41, 0xfa, 0xed, 0x27, 0xf2, 0x8c } };

BEGIN_INTERFACE_MAP(Events, CCmdTarget)
	INTERFACE_PART(Events, IID_IEvents, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Events message handlers

Bool Events::Start()
{
    m_dlg->m_progress="";
    m_dlg->UpdateData(false);
    m_dlg->UpdateWindow();

	return TRUE;
}

Bool Events::Stop()
{
	m_dlg->m_progress="Распознавание закончено.";
    m_dlg->UpdateData(false);
    m_dlg->UpdateWindow();

	return TRUE;
}


Bool Events::Step(long lStep, const char * strName, long lPerc)
{
	m_dlg->m_progress.Format("%s %d%%",strName,lPerc);
    m_dlg->UpdateData(false);
    m_dlg->UpdateWindow();

	return TRUE;
}

long Events::EndThread(Bool rc, long lContext)
{
	// TODO: Add your dispatch handler code here

	return 0;
}
