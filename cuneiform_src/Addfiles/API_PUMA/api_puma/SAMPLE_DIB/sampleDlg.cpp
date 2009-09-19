// sampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sample.h"
#include "sampleDlg.h"
#include "DlgProxy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef HANDLE (*MYPROC)(HBITMAP, HPALETTE);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleDlg dialog

IMPLEMENT_DYNAMIC(CSampleDlg, CDialog);

CSampleDlg::CSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSampleDlg)
	m_progress = _T("");
	m_spell = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CSampleDlg::~CSampleDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSampleDlg)
	DDX_Text(pDX, IDC_PROGRESS, m_progress);
	DDX_Check(pDX, IDC_CHECK_SPELL, m_spell);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CSampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleDlg message handlers

Bool CSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSampleDlg::OnSysCommand(uint nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CSampleDlg::OnClose()
{
	if (CanExit())
		CDialog::OnClose();
}

void CSampleDlg::OnOK()
{
	if (CanExit())
		CDialog::OnOK();
}

void CSampleDlg::OnCancel()
{
	if (CanExit())
		CDialog::OnCancel();
}

Bool CSampleDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

// CT: ПОДКЛЮЧЕНИЕ ЯДРА И ОРГАНИЗАЦИЯ ПРОЦЕССА РАСПОЗНАВАНИЯ
#include"tiger.h"
#include "events.h"
const IID DIID__IRecognitionEvents = {0x229C1071,0x829F,0x11D2,{0xBA,0x6E,0x00,0x00,0xE8,0xD9,0xFD,0xF6}};

void CSampleDlg::OnButton1()
{
    UpdateData();
    IRecognition Puma;

    if(Puma.CreateDispatch("Cognitive.Puma"))
    {
        // CT: Делаем необходимые действия для получения событий.
        LPDISPATCH lpDispatch = Puma.m_lpDispatch;
        IConnectionPointContainer * lpContainer = NULL;
        Events events(this);

        if(lpDispatch->QueryInterface(IID_IConnectionPointContainer,(void**)&lpContainer)==S_OK)
        {
            IConnectionPoint * lpPoint;
            lpContainer->Release();
            if(lpContainer->FindConnectionPoint(DIID__IRecognitionEvents,&lpPoint)==S_OK)
            {
uint32_t wdCookie;
                lpPoint->Advise(dynamic_cast<IUnknown *>(events.GetIDispatch(FALSE)),&wdCookie);
                lpPoint->Release();
            }
        }
        Puma.Load();
        Puma.SetLanguage(7);        // CT: РУССКО-АНГЛИЙСКИЙ ЯЗЫК
        Puma.SetSpeller(m_spell);   // CT: ВКЛ/ВЫКЛ СЛОВАРНОГО ДОРАСПОЗНАВАНИЯ

		///работа функции RecogDIBtoMemory

		HANDLE hDIB = 0;
		//HGLOBAL buffer = 0;

		/*
		получаем hDIB
		*/
		typedef HANDLE (__cdecl *delegate_BitmapToDIB)(HBITMAP, HPALETTE);
		typedef HANDLE (__cdecl *delegate_LoadDIB)(const char * filename);
		typedef HANDLE (__cdecl *delegate_CopyScreenToDIB)(RECT* );
		typedef int (__cdecl *delegate_SaveDIB)(HANDLE hdib,const char * filename);

		HMODULE hDibApiDll = LoadLibrary("c:\\dibapi.dll");

		delegate_BitmapToDIB BitmapToDIB = (delegate_BitmapToDIB)GetProcAddress(hDibApiDll,"BitmapToDIB");
		delegate_LoadDIB LoadDIB = (delegate_LoadDIB)GetProcAddress(hDibApiDll,"LoadDIB");
		delegate_CopyScreenToDIB CopyScreenToDIB = (delegate_CopyScreenToDIB)GetProcAddress(hDibApiDll,"CopyScreenToDIB");
		delegate_SaveDIB SaveDIB = (delegate_SaveDIB)GetProcAddress(hDibApiDll,"SaveDIB");

		///ВАРИАНТ 1 - получение HDIB из картинки
		hDIB = LoadDIB("c:\\Clip.bmp");

		/*HINSTANCE hInst = NULL;
		LPBITMAPINFOHEADER lpb = NULL;
		HBITMAP hBmp = (HBITMAP)LoadImage(hInst, "c:\\Clib.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
        BITMAP bmp = {0};
		GetObject(hBmp, sizeof(bmp), (pvoid)&bmp);*/

		long buf = 0;
		short format = 2;
		short code = 2;

	    if(hDIB != 0)
		{
			long hd = reinterpret_cast<long>(hDIB);

			Puma.RecogDIBtoMemory(hd, &buf, format, code);
			//Puma.RecogDIBtoFile(hd, "C:\\dtofile", format, code);

			HANDLE HH = (HANDLE)buf;
			void* pHptr1 = GlobalLock(HH);
			char* pBufText = static_cast<char*>(pHptr1 );
			MessageBox(pBufText);
			GlobalUnlock(HH);
		}

		Puma.Unload();
    }
}

