// sampleDlg.h : header file
//

#if !defined(AFX_SAMPLEDLG_H__668961C5_C4FB_477D_A02E_CB1C9B1CDA6C__INCLUDED_)
#define AFX_SAMPLEDLG_H__668961C5_C4FB_477D_A02E_CB1C9B1CDA6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSampleDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CSampleDlg dialog

class CSampleDlg : public CDialog
{
	DECLARE_DYNAMIC(CSampleDlg);
	friend class CSampleDlgAutoProxy;

// Construction
public:
	CSampleDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CSampleDlg();

// Dialog Data
	//{{AFX_DATA(CSampleDlg)
	enum { IDD = IDD_SAMPLE_DIALOG };
	CString	m_progress;
	Bool	m_spell;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSampleDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	Bool CanExit();

	// Generated message map functions
	//{{AFX_MSG(CSampleDlg)
	virtual Bool OnInitDialog();
	afx_msg void OnSysCommand(uint nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEDLG_H__668961C5_C4FB_477D_A02E_CB1C9B1CDA6C__INCLUDED_)
