; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "sample.h"
LastPage=0

ClassCount=5
Class1=CSampleDlgAutoProxy
Class2=Events
Class3=CSampleApp
Class4=CAboutDlg
Class5=CSampleDlg

ResourceCount=2
Resource1=IDD_ABOUTBOX (English (U.S.))
Resource2=IDD_SAMPLE_DIALOG (English (U.S.))

[CLS:CSampleDlgAutoProxy]
Type=0
BaseClass=CCmdTarget
HeaderFile=DlgProxy.h
ImplementationFile=DlgProxy.cpp

[CLS:Events]
Type=0
BaseClass=CCmdTarget
HeaderFile=Events.h
ImplementationFile=Events.cpp

[CLS:CSampleApp]
Type=0
BaseClass=CWinApp
HeaderFile=sample.h
ImplementationFile=sample.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=sampleDlg.cpp
ImplementationFile=sampleDlg.cpp
LastObject=IDOK

[CLS:CSampleDlg]
Type=0
BaseClass=CDialog
HeaderFile=sampleDlg.h
ImplementationFile=sampleDlg.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_SAMPLE_DIALOG]
Type=1
Class=CSampleDlg

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SAMPLE_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_CHECK_SPELL,button,1342242819
Control3=IDC_PROGRESS,edit,1484849280

